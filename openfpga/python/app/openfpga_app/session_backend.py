"""A backend over real sessions, with no model.

It holds the sessions of openfpga_mcp (workers, one directory each) and calls the same tools that
the MCP server gives a model, so that a person can drive a flow from the page: a chat line that
starts with `/` is a tool call (`/run_stage stage=pack`), the sidebar is the state of the session,
and the console runs commands of the OpenFPGA shell in it. A model that chooses the calls plugs in
where the chat line is read, later.
"""

import inspect
import json
import shlex
import time
import typing
from typing import Any, AsyncIterator, Callable, Dict, List, Optional

import anyio.to_thread
from openfpga_mcp import answers, session_tools, tools
from openfpga_mcp.config import Config
from openfpga_mcp.registry import SessionRegistry
from openfpga_mcp.resources import _checkpoints

from . import events
from .backend import STAGES, empty_view

_NO_MODEL = (
    "No model is connected, so nothing here chooses the next step. Call a tool yourself: a line "
    "that starts with / is a tool call, for example /session_open or /run_stage stage=pack. "
    "/help lists the tools."
)
# What the console shows of a long output
_CONSOLE_MAX = 20000
# Options that are the inputs or the constraints of the flow: the sidebar shows them as files
_INPUT_OPTIONS = ("filename.arch_file", "filename.circuit_file", "general.device_layout")
_CONSTRAINT_OPTIONS = ("filename.sdc_file", "filename.read_vpr_constraints_file")
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


def _literal(text: str) -> Any:
    """A value written in a chat line: a number, true, false, null or a quoted string if it is
    one, else the text itself"""
    try:
        return json.loads(text)
    except ValueError:
        return text


def _check_type(name: str, hint: Any, value: Any) -> Any:
    """The value for an argument, or a ValueError that says what the tool takes: the tools check
    their own limits; this is the type, and the words of a Literal"""
    origin = typing.get_origin(hint)
    if origin is typing.Union:
        kinds = typing.get_args(hint)
        if value is None and type(None) in kinds:
            return None
        options = [kind for kind in kinds if kind is not type(None)]
        if len(options) == 1:  # an optional argument: its own message
            return _check_type(name, options[0], value)
        for kind in options:
            try:
                return _check_type(name, kind, value)
            except ValueError:
                continue
        raise ValueError("%s has the wrong type" % name)
    if origin is typing.Literal:
        if value not in typing.get_args(hint):
            raise ValueError(
                "%s is one of: %s" % (name, ", ".join(map(str, typing.get_args(hint))))
            )
        return value
    if hint is bool:
        if isinstance(value, bool):
            return value
    elif hint is int:
        if isinstance(value, int) and not isinstance(value, bool):
            return value
    elif hint is float:
        if isinstance(value, (int, float)) and not isinstance(value, bool):
            return float(value)
    elif hint is str:
        if isinstance(value, str):
            return value
        if isinstance(value, (int, float)) and not isinstance(value, bool):
            return str(value)  # a name that looks like a number
    else:
        return value
    raise ValueError("%s has the wrong type" % name)


class SessionBackend:
    """The backend of the page over real sessions"""

    kind = "session"

    def __init__(self, config: Config, registry: Optional[SessionRegistry] = None):
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
        self._running: Optional[str] = None
        self._busy = False
        self._cancelled = False
        self._last = empty_view()
        self._counter = 0

    def close(self) -> None:
        self.registry.close_all()

    # The view ------------------------------------------------------------------------------

    async def view(self) -> Dict[str, Any]:
        self._last = await anyio.to_thread.run_sync(self._build_view)
        return self._last

    def _name(self, path: str, session_dir: str = "") -> str:
        """A path as a name that can be given back: inside the workspace, or a designs folder, and
        for the results inside the directory of the session"""
        shown = answers.relative(path, list(self.config.folders()))
        if session_dir and shown.startswith(session_dir + "/"):
            shown = shown[len(session_dir) + 1 :]
        return shown.rsplit("/", 1)[-1] if shown.startswith("/") else shown

    def _build_view(self) -> Dict[str, Any]:
        view = empty_view()
        if self.sid is None:
            return view
        try:
            entry = self.registry.get(self.sid)
        except Exception:  # closed by the idle limit, or gone
            self.sid = None
            return view
        info = next((i for i in self.registry.list() if i.id == entry.id), None)
        running = bool(info and info.running)
        view["session"] = {
            "id": entry.id,
            "name": entry.name,
            "worker": "running" if running else "ended",
            "ended": None if running else (info.ended if info else None),
            "idle_s": info.idle_s if info else 0,
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
                for key in ("stages", "files", "options_changed"):
                    view[key] = previous[key]
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
        for path in (state.results.packing, state.results.placement, state.results.routing):
            if path:
                files["results"].append(self._name(path, entry.directory))
        for option in session.options.changed():
            if option.name == "router.route_chan_width":
                if option.value != -1:
                    files["constraints"].append("chan width %s" % option.value)
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

    # The sessions ---------------------------------------------------------------------------

    async def _tool(self, tool: str, /, **arguments) -> Dict[str, Any]:
        return json.loads(await self.tools[tool](**arguments))

    async def new_session(self, name: Optional[str] = None) -> Dict[str, Any]:
        answer = await self._tool("session_open", name=name)
        if answer.get("ok"):
            self.sid = answer["data"]["session"]
        return answer

    async def close_session(self) -> Dict[str, Any]:
        if self.sid is None:
            return answers.failed([answers.diagnostic("no_session", "There is no session")])
        answer = await self._tool("session_close", session=self.sid)
        if answer.get("ok"):
            self.sid = None
        return answer

    async def restore_session(self, checkpoint: Optional[str] = None) -> Dict[str, Any]:
        if self.sid is None:
            return answers.failed([answers.diagnostic("no_session", "There is no session")])
        return await self._tool("session_restore", session=self.sid, checkpoint=checkpoint)

    # The conversation: a line is a tool call ---------------------------------------------------

    def cancel(self) -> None:
        self._cancelled = True
        if self._running and self.sid:
            try:
                self.registry.get(self.sid).session.cancel()
            except Exception:  # noqa: BLE001 - nothing to cancel
                pass

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
        signature = inspect.signature(function)
        hints = typing.get_type_hints(function)
        given: Dict[str, Any] = {}
        for token in tokens:
            key, equals, value = token.partition("=")
            if not equals or not key:
                raise ValueError("'%s' is not key=value" % token)
            if key not in signature.parameters:
                raise ValueError(
                    "There is no argument '%s' (it takes: %s)"
                    % (key, ", ".join(signature.parameters))
                )
            given[key] = _literal(value)
        if "session" in signature.parameters and "session" not in given and self.sid:
            given["session"] = self.sid
        checked = {k: _check_type(k, hints.get(k, Any), v) for k, v in given.items()}
        for key, parameter in signature.parameters.items():
            if parameter.default is inspect.Parameter.empty and key not in checked:
                raise ValueError(
                    "%s needs %s=…%s"
                    % (
                        function.__name__,
                        key,
                        " (open a session first: /session_open)" if key == "session" else "",
                    )
                )
        return checked

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
                yield events.text_delta(message_id, _NO_MODEL)
                yield events.done(message_id)
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
            call_id = "c%d" % self._counter
            yield events.tool_call_started(message_id, call_id, name, arguments)
            self._running = name
            if name in ("run_stage", "read_inputs"):
                yield events.state_changed("running")
            started = time.monotonic()
            try:
                answer = json.loads(await function(**arguments))
            finally:
                self._running = None
            if name == "session_open" and answer.get("ok"):
                self.sid = answer["data"]["session"]
            if (
                name == "session_close"
                and answer.get("ok")
                and arguments.get("session") == self.sid
            ):
                self.sid = None
            yield events.tool_call_finished(
                message_id, call_id, bool(answer.get("ok")), time.monotonic() - started, answer
            )
            yield events.state_changed(name)
            yield events.done(message_id, cancelled=self._cancelled)
        finally:
            self._busy = False
            self._running = None

    # The console -----------------------------------------------------------------------------------

    async def console(self, command: str) -> Dict[str, Any]:
        """A command of the OpenFPGA shell, in the current session. Any command: it is the user's
        own shell. What it prints is the output"""
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
            return {"ok": ok, "output": output, "state_changed": changed}

        try:
            answer = await anyio.to_thread.run_sync(work)
        except Exception as error:  # noqa: BLE001 - a worker that ended, a closed session
            answer = {"ok": False, "output": str(error), "state_changed": True}
        finally:
            self._running = None
        return answer
