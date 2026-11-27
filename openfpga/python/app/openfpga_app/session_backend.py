"""A backend over real sessions, with no model.

It holds the sessions of openfpga_mcp (workers, one directory each) and calls the same tools that
the MCP server gives a model, so that a person can drive a flow from the page: a chat line that
starts with `/` is a tool call (`/run_stage stage=pack`), the sidebar is the state of the session,
and the console runs commands of the OpenFPGA shell in it. A model that chooses the calls plugs in
where the chat line is read, later.
"""

import hashlib
import inspect
import json
import os
import re
import shlex
import time
from pathlib import Path
from typing import Any, AsyncIterator, Callable, Dict, List, Optional, Tuple

import anyio
import anyio.to_thread
from openfpga_mcp import answers, device_packages, reports, session_tools, tools
from openfpga_mcp.errors import InvalidArgument, RegistryError, SessionFull
from openfpga_mcp.config import Config
from openfpga_mcp.registry import SessionRegistry
from openfpga_mcp.resources import _checkpoints

from . import events, uploads, viewer
from .agent import AgentConfig, AgentLoop
from .agent.arguments import check_arguments, literal
from .agent.model import Model
from .backend import STAGES, empty_view

_NO_MODEL = (
    "No model is connected, so nothing here chooses the next step. Call a tool yourself: a line "
    "that starts with / is a tool call, for example /session_open or /run_stage stage=pack. "
    "/help lists the tools."
)
# What the console shows of a long output
_CONSOLE_MAX = 20000
# Options that are the inputs or the constraints of the flow: the sidebar shows them as files
_INPUT_OPTIONS = (
    "filename.arch_file",
    "filename.circuit_file",
    "general.device_layout",
    # The cached files of the device of a package (R-27): the package sets them
    "filename.read_rr_graph_file",
    "filename.read_router_lookahead",
    "filename.read_placement_delay_lookup",
)
_CONSTRAINT_OPTIONS = ("filename.sdc_file", "filename.read_vpr_constraints_file")
# The errors and warnings of the ODC that the sidebar lists
_ODC_DIAGNOSTICS = 50
# Commands that only read: the page does not need to refresh after them
_READS = ("report_", "help", "version")


class ToolTable:
    """Holds the tools that register() gives it, by name"""

    def __init__(self):
        self.tools: Dict[str, Callable] = {}

    def tool(self):
        def store(function):
            self.tools[function.__name__] = function
            return function

        return store


# How the designs are named in the list for the model: it says what to do with them, because a model
# that reads "design" runs "the wbscope design" as a circuit
# The hierarchy of a design: the instances of its tree at most, and the names of its folder
HIERARCHY_LIMIT = 2000
HIERARCHY_DESIGN = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,63}")
DESIGN_KIND = "design directory (Verilog: synthesize it with synthesize(design=name), never read it as a circuit)"


class SessionBackend:
    """The backend of the page over real sessions"""

    kind = "session"

    def __init__(
        self,
        config: Config,
        registry: Optional[SessionRegistry] = None,
        model: Optional[Model] = None,
        agent_config: Optional[AgentConfig] = None,
        record: Optional[Path] = None,
    ):
        self.config = config
        self.registry = registry or SessionRegistry(config)
        table = ToolTable()

        async def run(function) -> str:
            try:
                return answers.text(await anyio.to_thread.run_sync(function))
            except Exception as error:  # noqa: BLE001 - a failure is an answer, as for a model
                return answers.text(answers.from_exception(error))

        session_tools.register(self.registry, config, table.tool, run)
        tools.register(None, self.registry, config, table.tool, run)
        self.tools = table.tools
        self.sid: Optional[str] = None
        self._packages: Dict[str, Dict[str, Any]] = {}  # the device package of a session, by id
        self._running: Optional[str] = None
        # The constraints of the current session (report_constraints), read again after a call
        self._odc_cache: Optional[Tuple[Any, Optional[Dict[str, Any]]]] = None
        self._calls = 0  # the calls and commands that ended
        self._synthesis_runs: Dict[str, Dict[str, Any]] = {}  # the last synthesis of each session
        self._busy = False
        self._cancelled = False
        self._last = empty_view()
        self._view_lock = anyio.Lock()  # the exports of the views, one at a time
        self._counter = 0
        self.agent: Optional[AgentLoop] = None
        if model is not None:
            self.agent = AgentLoop(
                model,
                self.tools,
                agent_config or AgentConfig(),
                self.invoke,
                session=lambda: self.sid,
                files=self._files_listing,
                on_cancel=self._cancel_running,
                record=record,
            )

    def _files_listing(self) -> str:
        """The files that read_inputs can use, for the start of the prompt: a model cannot guess
        them, and it gives one for another (an OpenFPGA architecture as the VPR one). The files of
        the session (uploaded or written) are listed by what they are. From the designs folders the
        design directories (the folders with a manifest design.json) are listed with their top
        module, and the BLIF circuits that are not inside one, with the folders under
        them (a short name is enough when it is the only one of its name; otherwise its path), 500
        characters for each kind (1500 for the designs: synthesize takes the name of one)"""
        from openfpga_mcp.paths import (
            CREATED,
            UPLOADS,
            design_directories,
            design_files,
            looks_like_blif,
            root_element,
        )

        kinds = {"architecture": "vpr_arch", "openfpga_architecture": "openfpga_arch"}
        found: Dict[str, List[str]] = {
            DESIGN_KIND: [],
            "vpr_arch": [],
            "openfpga_arch": [],
            "circuit": [],
        }
        if self.sid is not None:
            try:
                directory = self.config.root / self.registry.get(self.sid).directory
            except Exception:  # noqa: BLE001 - no such session any more
                directory = None
            for folder in ([directory / UPLOADS, directory / CREATED] if directory else []):
                try:
                    paths = sorted(p for p in folder.iterdir() if p.is_file())
                except OSError:
                    continue
                for path in paths[:2000]:
                    if path.suffix == ".xml":
                        kind = kinds.get(root_element(str(path)) or "")
                    elif path.suffix == ".blif" and looks_like_blif(str(path)):
                        kind = "circuit"
                    else:
                        kind = None
                    if kind:
                        found[kind].append(path.name)
        designs = design_directories(self.config)
        by_name: Dict[str, int] = {}
        for design in designs:
            by_name[design["name"]] = by_name.get(design["name"], 0) + 1
        for design in designs:
            label = design["name"] if by_name[design["name"]] == 1 else design["relative"]
            found[DESIGN_KIND].append(
                "%s (top %s)" % (label, design["top"]) if design["top"] else label
            )
        inside = [d["folder"] for d in designs]
        circuits = sorted(
            (p, r)
            for p, r in design_files(self.config)
            if p.suffix == ".blif" and not any(f in p.parents for f in inside)
        )
        count: Dict[str, int] = {}
        for path, _relative in circuits:
            count[path.name] = count.get(path.name, 0) + 1
        for path, relative in circuits:
            found["circuit"].append(path.name if count[path.name] == 1 else relative)
        lines = []
        for kind, names in found.items():
            shown: List[str] = []
            room = 1500 if kind == DESIGN_KIND else 500
            for name in names:
                if room < len(name) + 2:
                    break
                shown.append(name)
                room -= len(name) + 2
            if shown:
                more = len(names) - len(shown)
                lines.append(
                    "%s: %s%s" % (kind, ", ".join(shown), " (+%d more)" % more if more else "")
                )
        return "\n".join(lines)

    def _cancel_running(self) -> None:
        if self._running and self.sid:
            try:
                self.registry.get(self.sid).session.cancel()
            except Exception:  # noqa: BLE001 - nothing to cancel
                pass

    def _changed_outside(self, what: str) -> None:
        """The state of the session changed, not by the conversation: the model is told"""
        if self.agent is not None:
            self.agent.note(
                "%s. The state may have changed since your last results: call get_state before "
                "you rely on them." % what
            )

    def close(self) -> None:
        self.registry.close_all()

    # The view ------------------------------------------------------------------------------

    async def view(self) -> Dict[str, Any]:
        self._last = await anyio.to_thread.run_sync(self._build_view)
        return self._last

    def _name(self, path: str, session_dir: str = "") -> str:
        """A path as a name that can be given back: inside the workspace, or a designs folder, and
        for the results inside the directory of the session"""
        for folder in self.config.package_dirs:
            for base in (str(folder), str(folder.resolve())):
                if path.startswith(base.rstrip("/") + "/"):
                    return path[len(base.rstrip("/")) + 1 :]  # package/arch/vpr_arch.xml
        shown = answers.relative(path, list(self.config.folders()))
        if session_dir and shown.startswith(session_dir + "/"):
            shown = shown[len(session_dir) + 1 :]
        return shown.rsplit("/", 1)[-1] if shown.startswith("/") else shown

    def _designs(self) -> List[Dict[str, Any]]:
        """The design directories for the sidebar: found again at most every 5 seconds (the walk of
        the designs folders is bounded, and the view is built for each event)"""
        from openfpga_mcp.paths import design_directories

        now = time.monotonic()
        cached = getattr(self, "_designs_cache", None)
        if cached is None or now - cached[0] > 5:
            found = [
                {
                    "name": d["name"],
                    "top": d["top"],
                    "path": d["relative"],
                    "files": d["rtl"],
                    "netlists": [Path(n).name for n in d["netlists"]],
                    "description": d["manifest"].get("description")
                    if isinstance(d["manifest"].get("description"), str)
                    else None,
                }
                for d in design_directories(self.config)
            ]
            cached = (now, found)
            self._designs_cache = cached
        return cached[1]

    def _build_view(self) -> Dict[str, Any]:
        view = empty_view()
        view["designs"] = self._designs()
        infos = self.registry.list()
        # The open sessions, for the tabs of the sidebar (a click switches to one)
        view["sessions"] = [{"id": i.id, "name": i.name, "running": bool(i.running)} for i in infos]
        if self.sid is None:
            return view
        try:
            entry = self.registry.get(self.sid)
        except Exception:  # closed by the idle limit, or gone
            self.sid = None
            return view
        view["files"]["uploads"] = uploads.listing(self.config.root, entry.directory)
        view["files"]["created"] = uploads.listing(self.config.root, entry.directory, "created")
        view["files"]["synthesis"] = [
            {**item, "name": "synth/" + item["name"]}  # a name that the viewer resolves in the session
            for item in uploads.listing(self.config.root, entry.directory, "synth")
        ]
        view["files"]["design"] = uploads.tree_listing(self.config.root, entry.directory, "design")
        view["reports"] = self._reports(entry.directory)
        view["constraints"] = None
        info = next((i for i in infos if i.id == entry.id), None)
        running = bool(info and info.running)
        view["session"] = {
            "id": entry.id,
            "name": entry.name,
            "worker": "running" if running else "ended",
            "ended": None if running else (info.ended if info else None),
            "idle_s": info.idle_s if info else 0,
            "device_package": self._packages.get(entry.id),
        }
        view["checkpoints"] = [
            {"name": c["name"], "kind": c["kind"], "stages": c["stages"]}
            for c in _checkpoints(self.config, entry.directory)["checkpoints"]
        ]
        if not running:
            view["latest"] = {"status": "failed", "text": "%s · worker ended" % entry.id}
            return view
        session = entry.session
        if self._running is not None or session._lock.locked():
            # A call is running: the worker answers when it ends, and the page must not wait for
            # it. What was known stays, and the latest state says what is going on
            previous = self._last
            if previous["session"] and previous["session"]["id"] == entry.id:
                for key in ("stages", "options_changed", "synthesis", "reports", "constraints"):
                    view[key] = previous[key]
                view["files"] = {
                    **previous["files"],
                    "uploads": view["files"]["uploads"],
                    "created": view["files"]["created"],
                    "synthesis": view["files"]["synthesis"],
                    "design": view["files"]["design"],
                }
            what = "a command" if self._running in (None, "console") else self._running
            view["latest"] = {"status": "running", "text": "%s · %s running" % (entry.id, what)}
            return view
        state = session.state()
        reached = list(state.reached)
        seconds: Dict[str, float] = {}
        try:
            seconds = {s.stage: s.seconds for s in session.run_report().stages if s.seconds}
        except Exception:  # no data of VPR before the inputs are read
            pass
        view["stages"] = [
            {
                "stage": s,
                "status": "reached" if s in reached else "not_reached",
                "seconds": round(seconds[s], 1) if s in reached and s in seconds else None,
                "message": None,
            }
            for s in STAGES
        ]
        if state.failed is not None:
            for item in view["stages"]:
                if item["stage"] == state.failed.stage:
                    item["status"] = "failed"
                    item["message"] = state.failed.message
        inputs = state.inputs
        view["synthesis"] = self._synthesis_stage(entry, inputs.circuit)
        view["synthesis"]["design"] = self._last_design(entry)
        files = view["files"]
        if inputs.vpr_arch:
            files["inputs"].append(
                {
                    "label": "vpr arch",
                    "name": self._name(inputs.vpr_arch.file),
                    "extra": "layout %s" % inputs.vpr_arch.layout,
                }
            )
        if inputs.openfpga_arch:
            files["inputs"].append(
                {
                    "label": "openfpga arch",
                    "name": self._name(inputs.openfpga_arch.file),
                    "extra": None,
                }
            )
        if inputs.circuit:
            files["inputs"].append(
                {"label": "circuit", "name": self._name(inputs.circuit.file), "extra": None}
            )
        for path in (
            state.results.packing,
            state.results.placement,
            state.results.routing,
            state.results.bitstream,
        ):
            if path:
                files["results"].append(self._name(path, entry.directory))
        odc = self._constraints(entry, session, state)
        view["constraints"] = {k: v for k, v in odc.items() if k != "outputs"} if odc else None
        given = set(odc["outputs"]) if odc else set()
        if odc and odc["file"]:
            files["constraints"].append("odc %s" % odc["file"])
        for option in session.options.changed():
            if option.name in _CONSTRAINT_OPTIONS and os.path.realpath(str(option.value)) in given:
                continue  # a file that the ODC gave to VPR: the ODC is what the sidebar shows
            if option.name == "router.route_chan_width":
                if option.value != -1:
                    # The width of a device package is read only
                    fixed = " (package)" if session.device_package() else ""
                    files["constraints"].append("chan width %s%s" % (option.value, fixed))
            elif option.name in _CONSTRAINT_OPTIONS:
                label = "sdc" if option.name.endswith("sdc_file") else "vpr constraints"
                files["constraints"].append("%s %s" % (label, self._name(str(option.value))))
            elif option.name not in _INPUT_OPTIONS:
                view["options_changed"].append({"name": option.name, "value": option.value})
        last = [s for s in STAGES if s in reached][-1:]
        if state.failed is not None:
            view["latest"] = {
                "status": "failed",
                "text": "%s · %s failed" % (entry.id, state.failed.stage),
            }
        elif last:
            took = seconds.get(last[0])
            view["latest"] = {
                "status": "reached",
                "text": "%s · %s ✔%s" % (entry.id, last[0], " %.1f s" % took if took else ""),
            }
        else:
            view["latest"] = {"status": "none", "text": "%s · opened" % entry.id}
        return view

    def _constraints(self, entry, session, state) -> Optional[Dict[str, Any]]:
        """What the sidebar shows of the ODC of the session: its file, how many commands, the
        errors and warnings with their lines (names that the viewer opens), the timing that is not
        constrained, the I/O map of the package. None when the session has no ODC and its package
        no I/O map. The report is read again when something ran, not for each view"""
        key = (entry.id, self._calls, tuple(state.reached))
        if self._odc_cache is not None and self._odc_cache[0] == key:
            return self._odc_cache[1]
        try:
            report = session.constraints(limit=1)
        except Exception:  # noqa: BLE001 - a shell without the ui plugin: no constraints
            report = None
        shown = None
        if report is not None and (report["file"] or report["design"]["io_map"]):

            def name(path: Optional[str]) -> Optional[str]:
                """The name that the sidebar opens: a file of the uploads or of the model by its
                own name, as they are listed"""
                if not path:
                    return None
                shown = self._name(path, entry.directory)
                for folder in (uploads.UPLOADS, uploads.CREATED):
                    if shown.startswith(folder + "/") and "/" not in shown[len(folder) + 1 :]:
                        return shown[len(folder) + 1 :]
                return shown

            levels = [d["level"] for d in report["diagnostics"]]
            timing = report["timing"]
            shown = {
                "file": name(report["file"]),
                "defaults": name(report["defaults"]),
                "commands": sum(1 for c in report["commands"] if not c["from_defaults"]),
                "errors": levels.count("error"),
                "warnings": levels.count("warning"),
                "diagnostics": [
                    {
                        "level": d["level"],
                        "file": name(d["file"]),
                        "line": d["line"],
                        "message": d["message"],
                    }
                    for d in report["diagnostics"][:_ODC_DIAGNOSTICS]
                ],
                "clocks": [c["name"] for c in timing["clocks"]],
                "timing_checked": timing["checked"],
                "unconstrained_clocks": timing["unconstrained_clocks"],
                "inputs_without_delay": len(timing["inputs_without_delay"]),
                "outputs_without_delay": len(timing["outputs_without_delay"]),
                "io_map": name(report["design"]["io_map"]),
                # The files of the ODC, as they are on disk: the options that name them are not
                # shown as constraints of their own
                "outputs": [
                    os.path.realpath(f)
                    for f in (
                        report["outputs"]["constraints"],
                        report["outputs"]["vpr_constraints"],
                    )
                    if f
                ],
            }
        self._odc_cache = (key, shown)
        return shown

    # The sessions ---------------------------------------------------------------------------

    async def _tool(self, tool: str, /, **arguments) -> Dict[str, Any]:
        return json.loads(await self.tools[tool](**arguments))

    async def invoke(self, name: str, arguments: Dict[str, Any]) -> Dict[str, Any]:
        """Run a tool, for a line of the chat or for the agent, and keep which session is current"""
        self._running = name
        started = time.monotonic()
        try:
            answer = json.loads(await self.tools[name](**arguments))
        finally:
            self._running = None
            self._calls += 1
        if name == "synthesize" and arguments.get("session"):
            failed = (answer.get("diagnostics") or [{}])[0].get("message", "Synthesis failed")
            self._synthesis_runs[arguments["session"]] = {
                "seconds": round(time.monotonic() - started, 1) if answer.get("ok") else None,
                "error": None if answer.get("ok") else str(failed)[:300],
            }
        if name == "session_open" and answer.get("ok"):
            self.sid = answer["data"]["session"]
        if name == "replay" and answer.get("ok"):
            self.sid = answer["data"]["session"]  # the page follows the new session
            meta = self.registry.read_meta(self.registry.get(self.sid).directory)
            if meta.get("device_package"):
                self._packages[self.sid] = meta["device_package"]
        if name == "use_device_package" and answer.get("ok"):
            self._set_package(arguments["session"], answer["data"]["device_package"])
        if name == "session_close" and answer.get("ok") and arguments.get("session") == self.sid:
            self.sid = None
        return answer

    async def new_session(
        self, name: Optional[str] = None, device_package: Optional[str] = None
    ) -> Dict[str, Any]:
        """Open a session. With a device package the session reads its two architectures, which
        make the device, and stops there: the person gives the design (the model is told what to
        do next)"""
        package = None
        if device_package is not None:
            try:
                package = await anyio.to_thread.run_sync(
                    device_packages.find, self.config, device_package
                )
            except RegistryError as error:
                return answers.from_exception(error)
        answer = await self._tool("session_open", name=name or (package.name if package else None))
        if not answer.get("ok"):
            return answer
        sid = answer["data"]["session"]
        if package is not None:
            # As the model does it: the device is made, and its report written
            read = await self._tool("use_device_package", session=sid, name=package.name)
            if not read.get("ok"):
                self.registry.close(sid, delete_files=True)  # nothing half made is kept
                return read
            self._set_package(sid, package.summary())
        self.sid = sid
        if package is None:
            self._changed_outside("The person opened session %s" % sid)
        else:
            self._changed_outside(
                "The person created session %s from the device package %s (%s): its VPR "
                "architecture (layout %s) and its OpenFPGA architecture are read, the device is "
                "made (stage DEVICE), and there is no circuit yet. When the person names a "
                "circuit, read it with read_inputs and only circuit=<file>, and do not give the "
                "architectures again"
                % (sid, package.name, package.description or "no description", package.layout)
            )
        return answer

    async def apply_device_package(self, device_package: str) -> Dict[str, Any]:
        """Read the architectures of a device package into the open session. Only a session with
        nothing read yet takes it: files that were read are not replaced (that drops the stages)"""
        if self.sid is None:
            return answers.failed([answers.diagnostic("no_session", "There is no session")])
        try:
            package = await anyio.to_thread.run_sync(
                device_packages.find, self.config, device_package
            )
            entry = self.registry.get(self.sid)
        except Exception as error:  # noqa: BLE001 - a package that is not there or is damaged
            return answers.from_exception(error)
        if entry.session._lock.locked():
            return answers.failed(
                [answers.diagnostic("busy", "A call is running in the session", hint="Wait for it to end")]
            )
        answer = await self._tool("use_device_package", session=self.sid, name=device_package)
        if not answer.get("ok"):
            return answer
        self._set_package(self.sid, answer["data"]["device_package"])
        options = answer["data"].get("options_set") or {}
        self._changed_outside(
            "The person loaded the device package %s (%s) into session %s: its VPR architecture "
            "(layout %s) and its OpenFPGA architecture are read%s, and there is no circuit yet. "
            "When the person names a circuit, read it with read_inputs and only circuit=<file>, "
            "and do not give the architectures again"
            % (
                package.name,
                package.description or "no description",
                self.sid,
                package.layout,
                ", with the options of its flow (%s)"
                % ", ".join("%s=%s" % item for item in sorted(options.items()))
                if options
                else "",
            )
        )
        return {"ok": True, "data": {"session": self.sid, "device_package": package.name}, "truncated": False, "more": None}

    async def _read_package_again(self, entry, name: Optional[str]) -> str:
        """A session of a device package that is opened with no checkpoint reads the architectures
        of its package again, as when it was created. Says what happened, for the model"""
        if not name:
            return ""
        try:
            package = await anyio.to_thread.run_sync(device_packages.find, self.config, name)
            await anyio.to_thread.run_sync(self._read_package, entry.session, package)
        except Exception as error:  # noqa: BLE001 - a package that is gone or that VPR refuses
            return (
                " and the architectures of its device package %s could not be read again (%s): "
                "read them with use_device_package or read_inputs" % (name, error)
            )
        return (
            " and the architectures of its device package %s read again (no circuit is read: "
            "read it with read_inputs and only circuit=<file>)" % name
        )

    @staticmethod
    def _read_package(session, package) -> None:
        device_packages.read_into(session, package)  # with its channel width and options

    async def read_file(self, name: str) -> Dict[str, Any]:
        """The text of a file that the sidebar names, for the viewer of the page"""
        try:
            directory = ""
            if self.sid is not None:
                directory = self.registry.get(self.sid).directory
            data = await anyio.to_thread.run_sync(
                viewer.read_text, self.config, directory, name
            )
        except RegistryError as error:
            return answers.from_exception(error)
        except Exception as error:  # noqa: BLE001 - a file that is gone, a disk that fails
            return answers.from_exception(error)
        return {"ok": True, "data": data, "truncated": False, "more": None}

    async def download_file(self, name: str) -> Optional[str]:
        """The whole path of a file that the page downloads (a bitstream), or None when it is
        not one that the viewer may read"""
        try:
            directory = self.registry.get(self.sid).directory if self.sid is not None else ""
            return await anyio.to_thread.run_sync(viewer.file_path, self.config, directory, name)
        except Exception:  # noqa: BLE001 - not there, refused: nothing to download
            return None

    def _reports(self, directory: str) -> List[str]:
        """The stages that have a report, in the order of the flow"""
        folder = self.config.root / directory / reports.FOLDER
        return [s for s in reports.TITLES if (folder / ("%s.html" % s)).is_file()]

    async def read_report(self, stage: str) -> Optional[str]:
        """The HTML of the report of a stage of the current session (None when there is none)"""
        if stage not in reports.TITLES or self.sid is None:
            return None
        try:
            directory = self.registry.get(self.sid).directory
        except RegistryError:
            return None
        path = self.config.root / directory / reports.FOLDER / ("%s.html" % stage)
        if path.is_symlink() or not path.is_file():
            return None
        return await anyio.to_thread.run_sync(lambda: path.read_text(encoding="utf-8", errors="replace"))

    def _last_design(self, entry) -> Optional[str]:
        """The design directory that the session copied last (synthesize with design copies it into
        the folder design of the session): the one that the run button of synthesis synthesizes"""
        folder = self.config.root / entry.directory / "design"
        try:
            copies = [p for p in folder.iterdir() if p.is_dir() and not p.is_symlink() and not p.name.startswith(".")]
        except OSError:
            return None
        return max(copies, key=lambda p: p.stat().st_mtime).name if copies else None

    async def _design_to_synthesize(self, sid: Optional[str]) -> Optional[str]:
        """The design that a run of a stage synthesizes first: the one the session copied last,
        when no circuit is read yet (None otherwise, or when the session is not known)"""
        try:
            entry = self.registry.get(sid)
        except (RegistryError, TypeError):
            return None
        try:
            circuit = (await anyio.to_thread.run_sync(entry.session.state)).inputs.circuit
        except Exception:
            return None
        return self._last_design(entry) if circuit is None else None

    def _synthesis_stage(self, entry, circuit) -> Dict[str, Any]:
        """The stage of synthesis for the sidebar. It is reached when the circuit is the netlist of a
        synthesis (a file of the folder synth of the session), skipped when it is a netlist that was
        given (nothing to synthesize), failed when the last synthesis failed, and not reached before
        a circuit is read"""
        run = self._synthesis_runs.get(entry.id) or {}
        if run.get("error"):
            return {"status": "failed", "seconds": None, "message": run["error"]}
        if circuit is None:
            return {"status": "not_reached", "seconds": None, "message": None}
        folder = os.path.realpath(self.config.root / entry.directory / "synth")
        made = os.path.dirname(os.path.realpath(circuit.file)) == folder
        if made:
            return {"status": "reached", "seconds": run.get("seconds"), "message": None}
        return {
            "status": "skipped",
            "seconds": None,
            "message": "Not needed: the design is a netlist (%s)" % os.path.basename(circuit.file),
        }

    def _set_package(self, sid: str, summary: Dict[str, Any]) -> None:
        """The device package of a session: shown by the page and kept for a later open"""
        self._packages[sid] = summary
        try:
            self.registry.update_meta(
                self.registry.get(sid), device_package=summary, package_name=summary.get("name")
            )  # the page shows it; synthesize reads the name
        except RegistryError:
            pass

    async def list_sessions(self) -> Dict[str, Any]:
        """The sessions that the page may open: the live ones (switch to one) and the saved ones of
        the workspace (a worker is started from the last checkpoint)"""
        infos = await anyio.to_thread.run_sync(self.registry.list)
        saved = await anyio.to_thread.run_sync(self.registry.saved)
        live = []
        for info in infos:
            meta = self.registry.read_meta(info.directory)
            live.append(
                {
                    "id": info.id,
                    "name": info.name,
                    "running": info.running,
                    "idle_s": info.idle_s,
                    "stages": info.stages,
                    "device_package": self._packages.get(info.id) or meta.get("device_package"),
                    "current": info.id == self.sid,
                }
            )
        return {"ok": True, "data": {"live": live, "saved": saved}, "truncated": False, "more": None}

    async def open_session(self, session: str) -> Dict[str, Any]:
        """Make a session the current one: a live one is switched to, a saved one is started again
        from its last checkpoint. The model is told"""
        if self._running is not None:
            return answers.failed(
                [answers.diagnostic("busy", "A call is running", hint="Wait for it to end")]
            )
        try:
            live = {i.id for i in await anyio.to_thread.run_sync(self.registry.list)}
            if session in live:
                entry = self.registry.get(session)
                how = "switched to session %s (%s)." % (entry.id, entry.name)
            else:
                entry = await anyio.to_thread.run_sync(self.registry.adopt, session)
                meta = self.registry.read_meta(entry.directory)
                if meta.get("device_package"):
                    self._packages[entry.id] = meta["device_package"]
                manifest = self.config.root / entry.directory / "checkpoints" / "checkpoint.json"
                how = "opened session %s (%s) again" % (entry.id, entry.name)
                if manifest.is_file():
                    how += ", from its last checkpoint."
                else:
                    how += ": no stage had run, so its flow is empty and its files are there"
                    how += await self._read_package_again(entry, meta.get("package_name")) + "."
        except RegistryError as error:
            return answers.from_exception(error)
        except Exception as error:  # noqa: BLE001 - a worker that does not start, a bad checkpoint
            return answers.from_exception(error)
        self.sid = entry.id
        self._changed_outside(
            "The person %s Call get_state before you rely on what you knew of another session"
            % how
        )
        return {"ok": True, "data": {"session": entry.id, "name": entry.name}, "truncated": False, "more": None}

    async def write_file(self, name: str, text: str) -> Dict[str, Any]:
        """Save the text of a file that the person edited (a file of the session's uploads or of
        those that the model wrote); the model is told, and the answer says if the session read it"""
        try:
            if self.sid is None:
                raise InvalidArgument("There is no session")
            entry = self.registry.get(self.sid)
            saved = await anyio.to_thread.run_sync(
                viewer.write_text, self.config, entry.directory, name, text
            )
            in_use = await anyio.to_thread.run_sync(self._is_input, entry, saved.pop("path"))
        except RegistryError as error:
            return answers.from_exception(error)
        except Exception as error:  # noqa: BLE001 - a file that is gone, a disk that fails
            return answers.from_exception(error)
        saved["in_use"] = in_use is not None
        what = "The person edited the file %s in session %s" % (name, self.sid)
        if in_use:
            what += (
                ", and the session had read it: the stages that used it are out of date until "
                "%s reads it again" % in_use
            )
        self._changed_outside(what)
        return {"ok": True, "data": saved, "truncated": False, "more": None}

    @staticmethod
    def _is_input(entry, path: str) -> Optional[str]:
        """The call that reads this file again, when the session has read it: one of its inputs,
        a constraints file, or a file of its ODC (one that the ODC includes too)"""
        session = entry.session
        if session._lock.locked():
            return None
        inputs = session.state().inputs
        path = os.path.realpath(path)
        if any(
            f is not None and os.path.realpath(f.file) == path
            for f in (inputs.vpr_arch, inputs.openfpga_arch, inputs.circuit)
        ):
            return "read_inputs"
        for option in _CONSTRAINT_OPTIONS:
            value = session.options.get(option).value
            if value and os.path.realpath(str(value)) == path:
                return "set_constraint"
        try:
            odc = session.constraints(limit=1)
        except Exception:  # noqa: BLE001 - a shell without the ui plugin
            return None
        if any(os.path.realpath(f) == path for f in odc["files"]):
            return "set_constraint kind=odc"
        return None

    async def copy_file(self, name: str) -> Dict[str, Any]:
        """A copy of a file that cannot be edited, in the uploads of the session, to edit there"""
        try:
            directory = ""
            if self.sid is not None:
                directory = self.registry.get(self.sid).directory
            else:
                opened = await self.new_session()
                if not opened.get("ok"):
                    return opened
                directory = self.registry.get(self.sid).directory
            saved = await anyio.to_thread.run_sync(
                viewer.copy_to_uploads, self.config, directory, name
            )
        except RegistryError as error:
            return answers.from_exception(error)
        except Exception as error:  # noqa: BLE001
            return answers.from_exception(error)
        self._changed_outside(
            "The person made a copy of %s in the uploads of session %s, to edit it" % (name, self.sid)
        )
        return {"ok": True, "data": saved, "truncated": False, "more": None}

    async def device_packages(self) -> Dict[str, Any]:
        """The packages that a session can be created from, and the ones that cannot be used"""
        found = await anyio.to_thread.run_sync(device_packages.discover, self.config)
        return {
            "ok": True,
            "data": {
                "packages": [p.summary() for p in found["packages"]],
                "problems": found["problems"],
            },
        }

    async def upload(self, name: str, data: bytes, overwrite: bool) -> Dict[str, Any]:
        """A file that the person gives to the session (one is opened if there is none). It is
        written in the folder uploads of the session, and the model is told that it is there"""
        try:
            uploads.check_name(name)
            if self.sid is None:
                opened = await self.new_session()
                if not opened.get("ok"):
                    return opened
            entry = self.registry.get(self.sid)
            used = await anyio.to_thread.run_sync(self.registry.directory_bytes, entry)
            if used + len(data) > self.config.session_max_bytes:
                raise uploads.UploadRefused(
                    "session_full",
                    "Session %s has %d bytes of files; this one would pass the %d that it may"
                    % (entry.id, used, self.config.session_max_bytes),
                )
            saved = await anyio.to_thread.run_sync(
                uploads.save, self.config.root, entry.directory, name, data, overwrite
            )
        except uploads.UploadRefused as refused:
            return answers.failed(
                [answers.diagnostic(refused.kind, refused.message, hint=refused.hint or None)]
            )
        except Exception as error:  # noqa: BLE001 - a session that is gone, a disk that is full
            return answers.from_exception(error)
        saved["loaded"] = False
        if saved["kind"] == "circuit" and await anyio.to_thread.run_sync(
            self._waits_for_circuit, entry
        ):
            # The session has its architectures and no circuit: this is the one to read
            read = await self.invoke("read_inputs", {"session": entry.id, "circuit": saved["name"]})
            if read.get("ok"):
                saved["loaded"] = True
            else:
                first = (read.get("diagnostics") or [{}])[0]
                saved["load_error"] = first.get("message", "The circuit could not be read")
                saved["load_hint"] = first.get("hint")
        what = "The person uploaded %s (%s) to session %s" % (
            saved["name"],
            saved["kind"].replace("_", " "),
            self.sid,
        )
        if saved["loaded"]:
            what += ", and it was read as the circuit (read_inputs): no need to read it again"
        elif "load_error" in saved:
            what += "; reading it as the circuit failed: %s" % saved["load_error"]
        else:
            what += "; read_inputs can use it by its name"
        self._changed_outside(what)
        return {"ok": True, "data": saved, "truncated": False, "more": None}

    def _waits_for_circuit(self, entry) -> bool:
        """The session has read both architectures (it was created from a device package) and has
        no circuit. A circuit that is there is not replaced: that drops the stages that were run"""
        session = entry.session
        if session._lock.locked():  # a call is running: it is not the moment
            return False
        inputs = session.state().inputs
        return bool(inputs.vpr_arch and inputs.openfpga_arch and not inputs.circuit)

    async def remove_upload(self, name: str) -> Dict[str, Any]:
        """Delete a file that the person gave to the session; the model is told that it is gone"""
        try:
            uploads.check_name(name)
            if self.sid is None:
                raise uploads.UploadRefused("no_session", "There is no session")
            entry = self.registry.get(self.sid)
            removed = await anyio.to_thread.run_sync(
                uploads.remove, self.config.root, entry.directory, name
            )
        except uploads.UploadRefused as refused:
            return answers.failed(
                [answers.diagnostic(refused.kind, refused.message, hint=refused.hint or None)]
            )
        except Exception as error:  # noqa: BLE001
            return answers.from_exception(error)
        self._changed_outside(
            "The person removed the uploaded file %s from session %s" % (removed["name"], self.sid)
        )
        return {"ok": True, "data": removed, "truncated": False, "more": None}

    async def close_session(self) -> Dict[str, Any]:
        if self.sid is None:
            return answers.failed([answers.diagnostic("no_session", "There is no session")])
        answer = await self._tool("session_close", session=self.sid)
        if answer.get("ok"):
            self._changed_outside("The person closed session %s" % self.sid)
            self.sid = None
        return answer

    async def save_checkpoint(self, name: Optional[str] = None) -> Dict[str, Any]:
        """Save the flow of the session as a checkpoint (a name is made when none is given)"""
        if self.sid is None:
            return answers.failed([answers.diagnostic("no_session", "There is no session")])
        if self._running is not None:
            return answers.failed(
                [answers.diagnostic("busy", "A call is running", hint="Wait for it to end")]
            )
        return await self._tool("checkpoint", session=self.sid, name=name)

    async def add_design(self, design: str) -> Dict[str, Any]:
        """Copy a design directory into the session (design/<folder>) and nothing more: it is not
        synthesized. The run button of synthesis, or the model, synthesizes it when asked"""
        from openfpga_mcp.paths import copy_design, find_design

        if self.sid is None:
            return answers.failed(
                [answers.diagnostic("no_session", "There is no session", hint="Open a session first")]
            )
        if self._running is not None:
            return answers.failed(
                [answers.diagnostic("busy", "A call is running", hint="Wait for it to end")]
            )
        try:
            entry = self.registry.get(self.sid)

            def work() -> Dict[str, Any]:
                found = find_design(self.config, design)
                used = self.registry.directory_bytes(entry)
                if used > self.config.session_max_bytes:
                    raise SessionFull(
                        "The files of session %s take %d bytes, more than the %d that it may"
                        % (entry.id, used, self.config.session_max_bytes)
                    )
                return copy_design(found, self.config.root / entry.directory)

            copied = await anyio.to_thread.run_sync(work)
        except Exception as error:  # noqa: BLE001 - not found, two of the name, too large, full
            return answers.from_exception(error)
        folder = "design/%s" % copied["folder"].name
        self._changed_outside(
            "The person added the design directory %s to session %s: it is copied to %s and not "
            "synthesized. Synthesize it with synthesize and design='%s' when asked"
            % (copied["name"], self.sid, folder, copied["name"])
        )
        data = {
            "session": self.sid,
            "design": copied["name"],
            "folder": folder,
            "files": copied["copied"],
            "rtl": list(copied["rtl"]),
        }
        return {"ok": True, "data": data, "truncated": False, "more": None}

    def _view_refusal(self) -> Optional[Dict[str, Any]]:
        """Why the data of a view cannot be made now, or None"""
        if self.sid is None:
            return answers.failed(
                [
                    answers.diagnostic(
                        "no_session", "There is no session", hint="Open a session first"
                    )
                ]
            )
        if self._running is not None or self._busy:
            return answers.failed(
                [answers.diagnostic("busy", "A call is running", hint="Wait for it to end")]
            )
        return None

    async def _view_call(self, name: str, call) -> Tuple[Dict[str, Any], Optional[str]]:
        """Run an export of the worker (one at a time: the page asks for several tiles at once),
        and give its answer and the path of its file"""
        refused = self._view_refusal()
        if refused is not None:
            return refused, None
        try:
            entry = self.registry.get(self.sid)
        except RegistryError as error:
            return answers.from_exception(error), None
        folder = self.config.root / entry.directory / "views"
        async with self._view_lock:
            try:
                data = await anyio.to_thread.run_sync(lambda: call(entry.session, folder))
            except Exception as error:  # noqa: BLE001 - not reached, refused, the worker failed
                return answers.from_exception(error), None
        return {"ok": True, "data": data, "truncated": False, "more": None}, data["file"]

    async def view_file(self, options: Dict[str, Any]) -> Tuple[Dict[str, Any], Optional[str]]:
        """The file of the data of a view of the session (export_view): kind, and a scope (module,
        cone with depth, blocks) or paths for timing. A file per kind and scope in views/, kept
        while its version is current"""
        kind = options["kind"]
        scope = {
            k: options[k] for k in ("module", "cone", "depth", "blocks", "paths") if k in options
        }
        name = kind
        if scope:
            name += "-" + hashlib.sha1(json.dumps(scope, sort_keys=True).encode()).hexdigest()[:12]

        def call(session, folder):
            folder.mkdir(parents=True, exist_ok=True)
            return session.export_view(kind, folder / (name + ".bin"), **scope)

        return await self._view_call("export_view", call)

    async def tile_file(
        self, kind: str, level: int, x: int, y: int
    ) -> Tuple[Dict[str, Any], Optional[str]]:
        """The file of a tile of a view of the device (view_tile), in views/tiles/"""

        def call(session, folder):
            where = folder / "tiles" / kind / str(level)
            where.mkdir(parents=True, exist_ok=True)
            return session.view_tile(kind, level, x, y, where / ("%d_%d.bin" % (x, y)))

        return await self._view_call("view_tile", call)

    async def design_hierarchy(self, design: Optional[str] = None) -> Dict[str, Any]:
        """The hierarchy of a design that the session copied (design/<folder>; the one added last
        by default): its RTL elaborated by a Yosys run of its own, which leaves the flow as it is.
        The files of its src are names of the session, for the viewer"""
        if self.sid is None:
            return answers.failed(
                [
                    answers.diagnostic(
                        "no_session", "There is no session", hint="Open a session first"
                    )
                ]
            )
        if self._running is not None or self._busy:
            return answers.failed(
                [answers.diagnostic("busy", "A call is running", hint="Wait for it to end")]
            )
        try:
            entry = self.registry.get(self.sid)
        except RegistryError as error:
            return answers.from_exception(error)
        session_dir = self.config.root / entry.directory
        name = design if design is not None else self._last_design(entry)
        folder = session_dir / "design" / name if name else None
        if (
            folder is None
            or not HIERARCHY_DESIGN.fullmatch(name)
            or folder.is_symlink()
            or not (folder / "design.json").is_file()
        ):
            return answers.failed(
                [
                    answers.diagnostic(
                        "no_design",
                        "The session has no design directory%s" % (" '%s'" % name if name else ""),
                        hint="Add a design to the session first (Designs)",
                    )
                ]
            )
        self._running = "design_hierarchy"
        try:
            data = await anyio.to_thread.run_sync(
                lambda: entry.session.design_hierarchy(
                    directory=folder, output_dir=session_dir / "hierarchy", limit=HIERARCHY_LIMIT
                )
            )
        except Exception as error:  # noqa: BLE001 - Yosys refused the RTL, the worker failed
            return answers.from_exception(error)
        finally:
            self._running = None
        base = os.path.realpath(session_dir)

        def named(src):
            if not src:
                return src
            path = os.path.realpath(src["file"])
            inside = path.startswith(base + os.sep)
            return {**src, "file": os.path.relpath(path, base) if inside else None}

        def walk(node):
            if "src" in node:
                node["src"] = named(node["src"])
            for child in node["children"]:
                walk(child)

        walk(data["tree"])
        for module in data["modules"]:
            module["src"] = named(module["src"])
        for key in ("files", "script", "json"):
            data.pop(key, None)  # whole paths of the disk: not for the page
        data["folder"] = "design/%s" % name
        return {"ok": True, "data": data, "truncated": bool(data["truncated"]), "more": None}

    async def restore_session(self, checkpoint: Optional[str] = None) -> Dict[str, Any]:
        if self.sid is None:
            return answers.failed([answers.diagnostic("no_session", "There is no session")])
        answer = await self._tool("session_restore", session=self.sid, checkpoint=checkpoint)
        if answer.get("ok"):
            self._changed_outside(
                "The person restored session %s from %s"
                % (self.sid, checkpoint or "the last checkpoint")
            )
        return answer

    # The conversation: a line is a tool call ---------------------------------------------------

    def cancel(self) -> None:
        self._cancelled = True
        if self.agent is not None:
            self.agent.cancel()  # also ends the call that runs
        else:
            self._cancel_running()

    def _help(self, name: Optional[str]) -> str:
        if name:
            function = self.tools.get(name)
            if function is None:
                return "There is no tool '%s'. /help lists them." % name
            signature = inspect.signature(function)
            arguments = ", ".join(signature.parameters)
            return "%s(%s)\n%s" % (name, arguments, inspect.getdoc(function) or "")
        lines = ["Tools (/name key=value ...; the session is the current one if left out):"]
        for tool_name in sorted(self.tools):
            doc = (inspect.getdoc(self.tools[tool_name]) or "").split(".")[0].replace("\n", " ")
            lines.append("/%s: %s" % (tool_name, doc))
        lines.append("/use sN switches to another session. /help NAME says what a tool takes.")
        return "\n".join(lines)

    def _arguments(self, function: Callable, tokens: List[str]) -> Dict[str, Any]:
        given: Dict[str, Any] = {}
        for token in tokens:
            key, equals, value = token.partition("=")
            if not equals or not key:
                raise ValueError("'%s' is not key=value" % token)
            given[key] = literal(value)
        return check_arguments(function, given, self.sid)

    async def chat(self, message: str) -> AsyncIterator[Dict[str, Any]]:
        self._counter += 1
        message_id = "m%d" % self._counter
        if self._busy:
            yield events.error("A call is running", "busy", "Stop it, or wait")
            return
        self._busy = True
        self._cancelled = False
        try:
            yield events.message_start(message_id)
            text = message.strip()
            if not text.startswith("/"):
                if self.agent is None:
                    yield events.text_delta(message_id, _NO_MODEL)
                    yield events.done(message_id)
                    return
                # The agent writes its own message_start: this one is the same message
                async for item in self.agent.run(message_id, text):
                    if item["type"] != "message_start":
                        yield item
                return
            try:
                words = shlex.split(text[1:])
            except ValueError as error:
                yield events.text_delta(message_id, "I cannot read that line: %s" % error)
                yield events.done(message_id)
                return
            name, tokens = (words[0], words[1:]) if words else ("help", [])
            if name == "help":
                yield events.text_delta(message_id, self._help(tokens[0] if tokens else None))
                yield events.done(message_id)
                return
            if name == "use":
                known = {i.id for i in self.registry.list()}
                if len(tokens) == 1 and tokens[0] in known:
                    self.sid = tokens[0]
                    self._changed_outside("The person switched to session %s" % self.sid)
                    yield events.text_delta(message_id, "Now on session %s." % self.sid)
                    yield events.state_changed("use")
                else:
                    yield events.text_delta(
                        message_id,
                        "/use takes the id of an open session (%s)." % ", ".join(sorted(known)),
                    )
                yield events.done(message_id)
                return
            function = self.tools.get(name)
            if function is None:
                yield events.text_delta(
                    message_id, "There is no tool '%s'. /help lists them." % name
                )
                yield events.done(message_id)
                return
            try:
                arguments = self._arguments(function, tokens)
            except ValueError as error:
                yield events.text_delta(
                    message_id, "%s. /help %s says what it takes." % (error, name)
                )
                yield events.done(message_id)
                return
            design = None
            if name == "run_stage" and arguments.get("from_file") is None:
                design = await self._design_to_synthesize(arguments.get("session"))
            if design is not None:
                # A design was added and no circuit is read: the stage starts with its synthesis,
                # as the stages before it that are not reached
                call_id = "c%d-synth" % self._counter
                synth = {"session": arguments["session"], "design": design}
                yield events.tool_call_started(message_id, call_id, "synthesize", synth)
                yield events.state_changed("running")
                started = time.monotonic()
                answer = await self.invoke("synthesize", synth)
                yield events.tool_call_finished(
                    message_id, call_id, bool(answer.get("ok")), time.monotonic() - started, answer
                )
                yield events.state_changed("synthesize")
                if not answer.get("ok") or self._cancelled:
                    yield events.done(message_id, cancelled=self._cancelled)
                    return
            call_id = "c%d" % self._counter
            yield events.tool_call_started(message_id, call_id, name, arguments)
            if name in ("run_stage", "read_inputs"):
                yield events.state_changed("running")
            started = time.monotonic()
            answer = await self.invoke(name, arguments)
            if self.agent is not None and name != "get_state":
                self._changed_outside("The person called /%s" % name)
            yield events.tool_call_finished(
                message_id, call_id, bool(answer.get("ok")), time.monotonic() - started, answer
            )
            yield events.state_changed(name)
            yield events.done(message_id, cancelled=self._cancelled)
        finally:
            self._busy = False
            self._running = None
            self._calls += 1

    # The console -----------------------------------------------------------------------------------

    def _flat_routing_allowed(self, entry) -> bool:
        """Whether the device of a session supports flat routing (its device package says)"""
        return device_packages.flat_routing_allowed(
            self.config, self.registry.read_meta(entry.directory)
        )

    async def console(self, command: str, mode: str = "shell") -> Dict[str, Any]:
        """A command of the OpenFPGA shell, in the current session. Any command: it is the user's
        own shell, except what the flow of the app does not do (Session.fixed_width_refusal):
        vpr, which runs the whole flow at once, a route with no channel width set, and a flat
        route on a device package that does not support flat routing. What it
        prints is the output. In the mode tcl the command is a Tcl script, run by the Tcl
        interpreter of the session (Session.tcl), whose commands are those of the shell"""
        if mode == "tcl":
            return await self._console_tcl(command)
        words = shlex.split(command) if command.strip() else []
        if not words:
            return {"ok": True, "output": "", "state_changed": False}
        if self.sid is None:
            return {
                "ok": False,
                "output": "No session: open one with the + new button, or /session_open in the chat.",
                "state_changed": False,
            }
        if self._running is not None:
            return {
                "ok": False,
                "output": "busy: %s is running" % self._running,
                "state_changed": False,
            }
        sid = self.sid
        self._running = "console"

        def work() -> Dict[str, Any]:
            entry = self.registry.get(sid)
            refusal = entry.session.fixed_width_refusal(words[0], self._flat_routing_allowed(entry))
            if refusal:
                return {"ok": False, "output": refusal, "state_changed": False}
            before = list(entry.session.state().reached)
            try:
                result = entry.session.command(words[0], *words[1:])
                output, ok = result.log, True
            except Exception as error:  # noqa: BLE001 - what the shell says is the answer
                output = getattr(error, "log", None) or str(error)
                if (
                    getattr(error, "diagnostics", None) is None
                    and getattr(error, "log", None) is None
                ):
                    output = "%s: %s" % (type(error).__name__, error)
                ok = False
            output = output.rstrip()
            if len(output) > _CONSOLE_MAX:
                output = "…" + output[-_CONSOLE_MAX:]
            changed = not words[0].startswith(_READS)
            try:
                after = list(entry.session.state().reached)
                changed = changed or after != before
            except Exception:  # noqa: BLE001 - the worker may have ended
                changed = True
            if changed and self.agent is not None:
                self._changed_outside("The person ran the console command: %s" % command[:100])
            return {"ok": ok, "output": output, "state_changed": changed}

        try:
            answer = await anyio.to_thread.run_sync(work)
        except Exception as error:  # noqa: BLE001 - a worker that ended, a closed session
            answer = {"ok": False, "output": str(error), "state_changed": True}
        finally:
            self._running = None
            self._calls += 1
        return answer

    async def _console_tcl(self, script: str) -> Dict[str, Any]:
        """A Tcl script of the console: the output is what it printed and the logs of the commands
        of the shell that it ran, then its result, as tclsh shows it"""
        if not script.strip():
            return {"ok": True, "output": "", "result": "", "state_changed": False}
        if self.sid is None:
            return {
                "ok": False,
                "output": "No session: open one with the + new button, or /session_open in the "
                "chat.",
                "state_changed": False,
            }
        if self._running is not None:
            return {
                "ok": False,
                "output": "busy: %s is running" % self._running,
                "state_changed": False,
            }
        sid = self.sid
        self._running = "console"

        def work() -> Dict[str, Any]:
            entry = self.registry.get(sid)
            before = list(entry.session.state().reached)
            result = ""
            try:
                done = entry.session.tcl(
                    script, fixed_width=True, flat_routing=self._flat_routing_allowed(entry)
                )
                output, result, ok, commands = done.output, done.result, True, done.commands
            except Exception as error:  # noqa: BLE001 - what Tcl says is the answer
                commands = list(getattr(error, "commands", None) or [])
                if getattr(error, "error_info", None) is not None:
                    output = (error.output or "") + (error.error_info or str(error))
                else:
                    output = "%s: %s" % (type(error).__name__, error)
                ok = False
            output = output.rstrip()
            if len(output) > _CONSOLE_MAX:
                output = "…" + output[-_CONSOLE_MAX:]
            changed = any(not line.startswith(_READS) for line in commands if line)
            try:
                changed = changed or list(entry.session.state().reached) != before
            except Exception:  # noqa: BLE001 - the worker may have ended
                changed = True
            if changed and self.agent is not None:
                ran = "; ".join(line.split()[0] for line in commands[:8]) or "nothing"
                self._changed_outside("The person ran a Tcl script in the console (%s)" % ran)
            return {"ok": ok, "output": output, "result": result, "state_changed": changed}

        try:
            answer = await anyio.to_thread.run_sync(work)
        except Exception as error:  # noqa: BLE001 - a worker that ended, a closed session
            answer = {"ok": False, "output": str(error), "state_changed": True}
        finally:
            self._running = None
            self._calls += 1
        return answer
