"""The tools of the flow, the options, the queries and the logs, see DESIGN.md

Each tool is one call of a session and the answer that goes with it: the facts are the ones of the
queries. An answer has at most Config.answer_chars characters (see budget.py).
"""

import dataclasses
import os
import re
import shutil
import time
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import Any, Callable, Dict, List, Literal, Optional, Union

from . import answers, budget, device_packages, filenames, reports, synthesis
from .config import Config
from .errors import (
    FileExists,
    FixedWidthOnly,
    InvalidArgument,
    OptionNotAllowed,
    SessionFull,
    SessionNotEmpty,
)
from .paths import CREATED, check_kind, copy_design, find_design, resolve_file, rtl_design_of
from .registry import Entry, SessionRegistry

_WRITE_MAX_BYTES = 256 * 1024  # the text that a model may write in one file
_WRITE_MAX_FILES = 50
Stage = Literal["pack", "place", "route", "analysis"]
_ORDER = ("pack", "place", "route", "analysis")
_STAGE_NAMES = {"pack": "PACKED", "place": "PLACED", "route": "ROUTED", "analysis": "ANALYZED"}
_READS = {"pack": "read_packing", "place": "read_placement", "route": "read_routing"}
_NAME = re.compile(r"[A-Za-z0-9][A-Za-z0-9_.-]{0,63}")
_LOG_KEEP = 256 * 1024  # of the log of a stage: the end of it
_LOG_LINE = 400  # characters of a line of a log
_LOG_TAIL_MAX = 200
# Options that name a file or a folder, besides those that start with 'filename.' (see
# _check_option): set_constraint and read_inputs take those, with the checks of the workspace
_PATH_OPTIONS = frozenset(
    ("analysis.echo_dot_timing_graph_node", "analysis.write_timing_summary", "crr.sb_maps")
    + ("crr.sb_templates",)
)


def _plain(kind: str, obj: Any) -> Dict[str, Any]:
    """An answer of a query as data: its kind and version of schema, and its fields"""
    import openfpga

    version = getattr(openfpga, "_SCHEMA_VERSIONS", {}).get(kind, 1)
    return {"kind": kind, "schema_version": version, **dataclasses.asdict(obj)}


def _number(name: str, value: Optional[int], default: int, low: int, high: int) -> int:
    value = default if value is None else value
    if not isinstance(value, int) or isinstance(value, bool) or not low <= value <= high:
        raise InvalidArgument("%s is a whole number from %d to %d" % (name, low, high))
    return value


def _text(name: str, value: Optional[str], required: bool = False) -> Optional[str]:
    """Text that goes on the command line of the shell: a name, a pattern. The shell has no way to
    write a double quote or a line break, and what a model gives is not worth more than 200
    characters"""
    if value is None and not required:
        return None
    if (
        not isinstance(value, str)
        or not value.strip()
        or len(value) > 200
        or any(c in value for c in '"\n\r\0')
    ):
        raise InvalidArgument(
            "%s is text of 1 to 200 characters, with no double quote and no line break" % name
        )
    return value


def _choice(name: str, value: Optional[str], allowed) -> Optional[str]:
    """One of the words that a tool accepts. The signature says it (a Literal) for a client that
    checks, and this is for one that does not: a word is never used to find a method by name"""
    if value is not None and value not in allowed:
        raise InvalidArgument("%s is one of: %s" % (name, ", ".join(allowed)))
    return value


def _check_option(name: str, kind: str):
    if name.startswith("filename.") or "file" in name or name in _PATH_OPTIONS:
        raise OptionNotAllowed(
            "'%s' names a file, which an option cannot take here" % name,
            # The tools that check a file name
        )
    return kind


def register(
    server,
    registry: SessionRegistry,
    config: Config,
    tool: Callable[[], Callable],
    run: Callable[[Callable[[], Dict[str, Any]]], Any],
) -> None:
    folders = list(config.folders())

    def reply(data: Any, more: Optional[str] = None, from_start: bool = False):
        return budget.fit(answers.relative(data, folders), config.answer_chars, more, from_start)

    def session_of(sid: str) -> Entry:
        return registry.get(sid)

    def check_room(entry: Entry):
        """Refuse a call that writes files when the session has used its share"""
        used = registry.directory_bytes(entry)
        if used > config.session_max_bytes:
            raise SessionFull(
                "The files of session %s take %d bytes, more than the %d that it may"
                % (entry.id, used, config.session_max_bytes)
            )

    def keep_log(entry: Entry, stage: str, log: str):
        entry.logs[stage] = log[-_LOG_KEEP:]

    # The reports of the stages -----------------------------------------------------------

    def short(path: Optional[str]) -> str:
        """A file as a report names it: from the workspace, else its name"""
        if not path:
            return "–"
        try:
            return Path(path).resolve().relative_to(config.root.resolve()).as_posix()
        except ValueError:
            return Path(path).name

    def stage_figures(handle, stage: str):
        """The sections and the cards of what a stage that ran made, from the queries of the
        session. A query that does not answer leaves its part out"""
        sections: List[reports.Section] = []
        cards: List[Any] = []
        number = reports.number
        if stage == "device":
            d = handle.device()
            part = reports.Section("Device")
            part.row("Grid", "%d × %d tiles" % (d.grid.width, d.grid.height)).row("Layers", d.grid.layers)
            part.row("Layout", d.grid.layout)
            if d.channel_width is not None:
                part.row(
                    "Channel width",
                    "%d × %d%s" % (d.channel_width.x, d.channel_width.y, " (fixed)" if d.channel_width.fixed else ""),
                )
            part.table(
                "Tile types",
                ["Type", "Tiles", "Capacity", "Slots", "I/O", "Inputs", "Outputs"],
                [(t.name, t.count, t.capacity, t.slots, "yes" if t.is_io else "", t.input_pins, t.output_pins)
                 for t in d.tile_types],
            )
            sections.append(part)
            cards += [("Grid", "%d × %d" % (d.grid.width, d.grid.height)), ("Tile types", len(d.tile_types)),
                      ("I/O slots", sum(t.slots for t in d.tile_types if t.is_io))]
        elif stage == "pack":
            packing = handle.run_report().packing
            if packing is not None:
                part = reports.Section("Packing")
                part.row("Clusters", packing.clusters).row("Atoms", packing.atoms).row("Nets", packing.nets)
                part.table("Clusters by type", ["Type", "Count"], [(b.type, b.count) for b in packing.blocks])
                sections.append(part)
                cards += [("Clusters", packing.clusters), ("Atoms", packing.atoms), ("Nets", packing.nets)]
            use = handle.utilization()
            part = reports.Section("Utilization")
            part.row("Device utilization", number(use.device_utilization * 100, "%", 1))
            part.row("Limiting resources", ", ".join(use.limiting_resources) or "none")
            part.table(
                "By block type",
                ["Block type", "Used", "Available", "Utilization %"],
                [(b.name, b.used, b.available, round(b.utilization * 100, 1)) for b in use.block_types],
            )
            sections.append(part)
            cards.append(("Device use", number(use.device_utilization * 100, "%", 1)))
        elif stage == "place":
            placement = handle.run_report().placement
            if placement is not None:
                part = reports.Section("Placement")
                part.row("Wirelength (half perimeter)", placement.half_perimeter)
                part.row("Cost", placement.cost).row("Bounding box cost", placement.bb_cost)
                part.row("Timing cost", placement.td_cost)
                part.row("Estimated critical path", placement.estimated_critical_path_delay_ns, "ns")
                part.row("Estimated Fmax", placement.estimated_fmax_mhz, "MHz")
                part.row("Temperatures", placement.num_temperatures).row("Swap attempts", placement.swap_attempts)
                sections.append(part)
                cards += [("Half perimeter", placement.half_perimeter),
                          ("Est. critical path", number(placement.estimated_critical_path_delay_ns, "ns")),
                          ("Est. Fmax", number(placement.estimated_fmax_mhz, "MHz", 1))]
        elif stage == "route":
            routing = handle.run_report().routing
            if routing is not None:
                part = reports.Section("Routing")
                part.row("Channel width", routing.channel_width).row("Wirelength", routing.wirelength, "tiles")
                part.row("Wires", routing.wires).row("Router iterations", routing.iterations)
                part.row("Critical path", routing.critical_path_delay_ns, "ns")
                sections.append(part)
                cards += [("Channel width", routing.channel_width), ("Wirelength", routing.wirelength),
                          ("Iterations", routing.iterations)]
        elif stage == "analysis":
            timing = handle.timing_summary()
            part = reports.Section("Timing")
            part.row("Critical path", timing.setup.critical_path_delay_ns, "ns")
            part.row("Fmax", timing.setup.fmax_mhz, "MHz")
            part.row("Setup worst negative slack", timing.setup.worst_negative_slack_ns, "ns")
            part.row("Setup total negative slack", timing.setup.total_negative_slack_ns, "ns")
            part.row("Hold worst negative slack", timing.hold.worst_negative_slack_ns, "ns")
            part.row("Hold total negative slack", timing.hold.total_negative_slack_ns, "ns")
            part.table(
                "Clock domains",
                ["Launch", "Capture", "Critical path (ns)", "Worst slack (ns)", "Fmax (MHz)"],
                [(c.launch_clock, c.capture_clock, c.critical_path_delay_ns, c.worst_slack_ns, c.fmax_mhz)
                 for c in timing.clock_domains],
            )
            sections.append(part)
            cards += [("Critical path", number(timing.setup.critical_path_delay_ns, "ns")),
                      ("Fmax", number(timing.setup.fmax_mhz, "MHz", 1)),
                      ("Setup WNS", number(timing.setup.worst_negative_slack_ns, "ns"))]
        return sections, cards

    def write_report(
        entry: Entry,
        stage: str,
        status: str,
        log: Optional[str] = None,
        seconds: Optional[float] = None,
        errors: List[str] = (),
        sections: List[Any] = (),
        cards: List[Any] = (),
        extra: List[Any] = (),
    ) -> Optional[str]:
        """Write reports/<stage>.html of the session and give its name from the workspace. A report
        that cannot be made never fails the stage: the answer then has no report"""
        try:
            handle = entry.session
            state = None
            try:
                state = handle.state()
            except Exception:  # noqa: BLE001 - a worker that ended: the report says what is known
                pass
            meta = registry.read_meta(entry.directory)
            inputs = state.inputs if state is not None else None
            summary: List[Any] = [
                ("Stage", reports.TITLES.get(stage, stage)),
                ("Status", status),
                ("Time", reports.number(round(seconds, 2), "s") if seconds is not None else "–"),
                ("Session", entry.id),
                ("Circuit", short(inputs.circuit.file) if inputs and inputs.circuit else "–"),
                ("Device package", meta.get("package_name") or "–"),
                ("Layout", inputs.vpr_arch.layout if inputs and inputs.vpr_arch else "–"),
            ]
            summary += list(extra)
            summary.append(("Stages reached", ", ".join(state.reached) if state is not None and state.reached else "none"))
            all_sections, all_cards = list(sections), list(cards)
            if status == "passed" and stage in reports.TITLES and stage != "synthesis":
                try:
                    more_sections, more_cards = stage_figures(handle, stage)
                    all_sections += more_sections
                    all_cards += more_cards
                except Exception:  # noqa: BLE001 - a query that does not answer leaves its part out
                    pass
            if seconds is not None:
                all_cards.insert(0, ("Time", reports.number(round(seconds, 2), "s")))
            text = reports.render(
                stage, status, entry.id, summary, all_sections, all_cards, log, list(errors)
            )
            path = reports.write(config.root / entry.directory, stage, text)
            return path.relative_to(config.root).as_posix()
        except Exception:  # noqa: BLE001 - see above
            return None

    def failure_text(error: BaseException) -> str:
        found = getattr(error, "diagnostics", None)
        if found:
            first = found[0]
            return str(getattr(first, "message", None) or (first.get("message") if isinstance(first, dict) else first))
        return str(error)

    # The flow ----------------------------------------------------------------------------

    @tool()
    async def read_inputs(
        session: str,
        circuit: str,
        vpr_arch: Optional[str] = None,
        layout: Optional[str] = None,
        openfpga_arch: Optional[str] = None,
    ):
        """Read the inputs of a flow: the circuit (a BLIF netlist), the architecture of VPR (an
        XML file) with the name of its device layout (for example 2x2; 'auto' is not accepted)
        and the architecture of OpenFPGA (XML). File names are relative to the workspace or to a
        designs folder: no absolute path and no '..'. A session that was created from a
        device package has both architectures already: give only the circuit. Returns the state
        of the session."""

        def work():
            entry = session_of(session)
            where = entry.directory
            handle = entry.session
            # The files first: a file that is not there, or is of the wrong kind, is what the answer
            # says, before an argument that is missing
            given = [
                (argument, name, resolve_file(config, where, name))
                for argument, name in (
                    ("vpr_arch", vpr_arch),
                    ("openfpga_arch", openfpga_arch),
                    ("circuit", circuit),
                )
                if name is not None
            ]
            for argument, name, path in given:
                check_kind(argument, name, path)
                owner = rtl_design_of(config, where, path) if argument == "circuit" else None
                if owner:
                    error = InvalidArgument(
                        "'%s' is a netlist of the design directory '%s', which has RTL: the RTL is "
                        "used, not the netlist" % (name, owner)
                    )
                    error.hint = "synthesize with design='%s' (use_device_package first)" % owner
                    raise error
            if vpr_arch is None and layout is not None:
                raise InvalidArgument("layout goes with vpr_arch: give both, or neither")
            if vpr_arch is not None and layout is None:
                raise InvalidArgument("layout is needed with vpr_arch (for example 2x2)")
            have = handle.state().inputs
            for argument, name, kept in (
                ("vpr_arch", vpr_arch, have.vpr_arch),
                ("openfpga_arch", openfpga_arch, have.openfpga_arch),
            ):
                if name is None and kept is None:
                    raise InvalidArgument(
                        "%s is needed: no architecture has been read in this session "
                        "(a session created from a device package has them)" % argument
                    )
            paths = {argument: path for argument, _name, path in given}
            logs: List[str] = []
            started = time.monotonic()
            try:
                if "vpr_arch" in paths:
                    logs.append(handle.read_vpr_arch(paths["vpr_arch"], layout=layout).log)
                if "openfpga_arch" in paths:
                    logs.append(handle.read_openfpga_arch(paths["openfpga_arch"]).log)
                logs.append(handle.read_circuit(paths["circuit"]).log)
            except Exception as error:
                log = getattr(error, "log", None)
                if log is not None:
                    write_report(entry, "device", "failed", "\n".join(logs + [log]),
                                 time.monotonic() - started, [failure_text(error)])
                raise
            write_report(entry, "device", "passed", "\n".join(logs), time.monotonic() - started)
            return reply(_plain("session", handle.state()))

        return await run(work)

    @tool()
    async def use_device_package(session: str, name: str):
        """Read a device package into a session: its VPR architecture (with the layout of its
        grid) and its OpenFPGA architecture, so that the session targets that device. Give the
        name of a package from list_device_packages. Only a session with nothing read yet takes a
        package (fails with session_not_empty otherwise), and its channel width is set. It stops
        there: read_inputs with only the circuit is the next call, and the architectures are not
        given again. Returns the package and the state of the session."""

        def work():
            entry = session_of(session)
            handle = entry.session
            package = device_packages.find(config, name)
            have = handle.state().inputs
            if have.vpr_arch or have.openfpga_arch or have.circuit:
                raise SessionNotEmpty(
                    "Session %s has already read files, and a device package would replace them"
                    % session
                )
            device_packages.read_into(handle, package)
            registry.update_meta(entry, package_name=package.name)  # for synthesize
            return reply(
                {"device_package": package.summary(), "state": _plain("session", handle.state())}
            )

        return await run(work)

    @tool()
    async def write_file(session: str, name: str, content: str, overwrite: bool = False):
        """Write a text file in the directory of the session: an architecture or a constraints
        file (XML), a circuit (BLIF or Verilog), a timing constraint (SDC) or a pin constraint
        (PCF). Only here, never anywhere else: give a plain name (letters, digits and . _ + -)
        with no folder, and an extension of a file that a flow reads. read_inputs and
        set_constraint find the file by its name afterwards. A name that was written before is
        kept unless overwrite is true. Returns the name, what the file is (vpr_arch,
        openfpga_arch, circuit or other), its size, and what is wrong with it when an XML file is
        not well formed. Put real line breaks in content, not the two characters backslash and n
        (a text that has only those is read as one that has line breaks)."""

        def work():
            entry = session_of(session)
            problem = filenames.name_problem(name)
            if problem:
                raise InvalidArgument(problem)
            if not isinstance(content, str) or not content.strip():
                raise InvalidArgument("content is the text of the file, and it is not empty")
            text, unescaped = filenames.unescape_lines(content)
            data = text.encode("utf-8")
            if len(data) > _WRITE_MAX_BYTES:
                raise InvalidArgument(
                    "the file is %d bytes; the most that can be written is %d"
                    % (len(data), _WRITE_MAX_BYTES)
                )
            check_room(entry)
            folder = config.root / entry.directory / CREATED
            folder.mkdir(parents=True, exist_ok=True)
            destination = folder / name
            if destination.is_symlink() or (destination.exists() and not destination.is_file()):
                raise InvalidArgument("'%s' cannot be replaced" % name)
            if destination.exists() and not overwrite:
                raise FileExists(
                    "There is already a file '%s' that was written in this session" % name
                )
            if not destination.exists() and sum(1 for _ in folder.iterdir()) >= _WRITE_MAX_FILES:
                raise InvalidArgument("at most %d files can be written in a session" % _WRITE_MAX_FILES)
            temporary = folder / (".%s.part" % name)
            temporary.write_bytes(data)
            os.replace(temporary, destination)  # a half file is never seen under the name
            result: Dict[str, Any] = {
                "name": name,
                "kind": filenames.kind_of(destination),
                "bytes": len(data),
            }
            if unescaped:
                result["unescaped"] = "the text had the two characters \\n and no line break: they were written as line breaks"
            if name.lower().endswith(".xml"):
                if "<!ENTITY" in text:  # a flow does not need them, and they can grow a parse
                    result["xml_error"] = "entity declarations are not accepted"
                else:
                    try:
                        ET.fromstring(text)
                    except ET.ParseError as error:
                        result["xml_error"] = str(error)
            return reply(result)

        return await run(work)

    @tool()
    async def synthesize(
        session: str,
        top: Optional[str] = None,
        files: Optional[str] = None,
        read: bool = True,
        lut_size: Optional[int] = None,
        design: Optional[str] = None,
    ):
        """Synthesize Verilog into a netlist of the LUTs of the device, with Yosys. Give design,
        the name of a design directory (a folder with a design.json: its top, files, defines and
        include folders come from the manifest; it is copied into the session and the copy is
        synthesized; one with only a netlist has that read), or give top (the top module) and
        files (.v, .sv, separated by commas; names of files of the session or of a designs folder). The device
        is the one of use_device_package (its LUT size and script); with none give lut_size. The
        netlist is synth/<top>.blif; with read true it is read as the circuit, and run_stage
        can follow. Returns LUTs, flip-flops, the widest LUT, ports and the end of the log. Fails
        with yosys_error and the file and line: fix the Verilog (write_file) and call again."""

        def work():
            entry = session_of(session)
            handle = entry.session
            nonlocal top
            include_dirs: List[Path] = []
            options: Dict[str, Any] = {}
            if design is not None:
                if top is not None or files is not None:
                    raise InvalidArgument("give design, or top and files, not both")
                found = find_design(config, design)
                netlist = _design_netlist(found)  # only for a design that has no RTL
                if netlist is None:
                    _design_inputs(found)  # what is wrong with the design is said before it is copied
                check_room(entry)
                found = copy_design(found, config.root / entry.directory)
                if netlist is not None:
                    return _read_design_netlist(entry, found, netlist, read)
                names, paths, top, include_dirs, options = _design_inputs(found)
            else:
                if top is None or files is None:
                    raise InvalidArgument("give design, or top and files")
                names = [n.strip() for n in str(files).split(",") if n.strip()]
                paths = [Path(resolve_file(config, entry.directory, n)) for n in names]
            package = None
            package_name = registry.read_meta(entry.directory).get("package_name")
            if package_name:
                package = device_packages.find(config, package_name)
            check_room(entry)
            folder = config.root / entry.directory / "synth"
            plan = synthesis.make_plan(
                top,
                paths,
                folder,
                package,
                lut_size,
                include_dirs=include_dirs,
                defines=options.get("defines"),
                read_options=options.get("read_options"),
            )
            folder.mkdir(parents=True, exist_ok=True)
            for old in folder.glob("*.ys"):  # one script: not one for each top module
                if old != plan.script_file:
                    old.unlink()
            plan.script_file.write_text(plan.script)
            plan.blif.unlink(missing_ok=True)
            started = time.monotonic()
            try:
                log = handle.command("yosys", file=str(plan.script_file)).log
            except Exception as error:  # noqa: BLE001 - the command failed, or the worker did
                log = getattr(error, "log", None)
                if log is None:
                    raise
                return _synthesis_failure(entry, plan, log, names, paths, time.monotonic() - started)
            if not plan.blif.is_file():
                return _synthesis_failure(entry, plan, log, names, paths, time.monotonic() - started)
            took = time.monotonic() - started
            result: Dict[str, Any] = {
                "top": plan.top,
                "netlist": str(plan.blif),
                "script": str(plan.script_file),
                "device_package": plan.package,
                "lut_size": plan.lut_size,
                **synthesis.blif_stats(plan.blif),
                "circuit_read": False,
                "log_tail": synthesis.last_lines(log),
            }
            if design is not None:
                result["design_copy"] = str(found["folder"])
                result["files"] = [str(p) for p in paths]
            if result["widest_lut"] > plan.lut_size:
                result["warning"] = "a LUT has %d inputs; the device has LUTs of %d" % (
                    result["widest_lut"],
                    plan.lut_size,
                )
            figures = reports.Section("Netlist")
            for key, label in (("luts", "LUTs"), ("flip_flops", "Flip-flops"), ("widest_lut", "Widest LUT"),
                               ("inputs", "Inputs"), ("outputs", "Outputs"), ("constants", "Constants")):
                figures.row(label, result.get(key))
            figures.row("LUT size of the device", plan.lut_size)
            figures.table("Cells that are not LUTs", ["Cell", "Count"], sorted((result.get("subckts") or {}).items()))
            report = write_report(
                entry, "synthesis", "passed", log, took,
                errors=[result["warning"]] if result.get("warning") else [],
                sections=[figures],
                cards=[("LUTs", result.get("luts")), ("Flip-flops", result.get("flip_flops")),
                       ("Widest LUT", result.get("widest_lut"))],
                extra=[("Top module", plan.top), ("Sources", ", ".join(short(str(p)) for p in paths)),
                       ("Netlist", short(str(plan.blif)))],
            )
            if report:
                result["report"] = report
            have = handle.state().inputs
            if read and have.vpr_arch and have.openfpga_arch:
                started = time.monotonic()
                try:
                    step = handle.read_circuit(str(plan.blif))
                    result["circuit_read"] = True
                except Exception as error:  # noqa: BLE001 - a netlist that the flow refuses
                    write_report(entry, "device", "failed", getattr(error, "log", None),
                                 time.monotonic() - started, [failure_text(error)])
                    return _circuit_not_read(plan, result, error)
                write_report(entry, "device", "passed", step.log, step.elapsed_s)
            elif read:
                result["note"] = (
                    "The architectures are not read: use_device_package, then read_inputs with "
                    "circuit=%s" % plan.blif.relative_to(config.root / entry.directory)
                )
            return reply(result)

        return await run(work)

    def _design_netlist(found: Dict[str, Any]) -> Optional[str]:
        """The netlist of a design directory that has no RTL (its path in the directory), or None
        when the design has RTL, which is what is synthesized even when it has a netlist too"""
        entries = [e for e in found["manifest"].get("files", []) if isinstance(e, dict)]
        if any(e.get("role") == "rtl" for e in entries):
            return None
        netlists = [e.get("path") for e in entries if e.get("role") == "netlist"]
        if not netlists:
            return None  # no RTL and no netlist: _design_inputs says so
        relative = netlists[0]
        if not isinstance(relative, str) or os.path.isabs(relative) or ".." in Path(relative).parts:
            raise InvalidArgument(
                "The manifest of '%s' has the netlist '%s', which is not inside the design" % (found["name"], relative)
            )
        if Path(relative).suffix.lower() not in (".blif", ".eblif"):
            raise InvalidArgument(
                "The design '%s' has only the netlist '%s', which the flow does not read (it reads "
                "BLIF and extended BLIF)" % (found["name"], relative)
            )
        path = (found["folder"] / relative).resolve()
        if found["folder"].resolve() not in path.parents or not path.is_file():
            raise InvalidArgument("The netlist '%s' of '%s' does not exist" % (relative, found["name"]))
        return relative

    def _read_design_netlist(entry: Entry, found: Dict[str, Any], relative: str, read: bool):
        """The copy of a design that has only a netlist: the netlist is read as the circuit"""
        path = found["folder"] / relative
        check_kind("circuit", relative, str(path))
        result: Dict[str, Any] = {
            "design": found["name"],
            "design_copy": str(found["folder"]),
            "netlist": str(path),
            "synthesized": False,
            "circuit_read": False,
            "note": "The design has no RTL, only a netlist: nothing was synthesized",
        }
        have = entry.session.state().inputs
        if read and have.vpr_arch and have.openfpga_arch:
            started = time.monotonic()
            try:
                step = entry.session.read_circuit(str(path))
            except Exception as error:
                write_report(entry, "device", "failed", getattr(error, "log", None),
                             time.monotonic() - started, [failure_text(error)])
                raise
            result["circuit_read"] = True
            report = write_report(entry, "device", "passed", step.log, step.elapsed_s)
            if report:
                result["report"] = report
        elif read:
            result["note"] += (
                "; the architectures are not read: use_device_package, then read_inputs with "
                "circuit=%s" % path.relative_to(config.root / entry.directory)
            )
        return reply(result)

    def _design_inputs(found: Dict[str, Any]):
        """The names, whole paths, top, include folders and options of a design directory: the
        files of its manifest, each inside the folder (the manifest is read as data, never as
        paths to trust)"""
        manifest = found["manifest"]
        folder = found["folder"].resolve()

        def inside(relative: Any, what: str) -> Path:
            if not isinstance(relative, str) or os.path.isabs(relative) or ".." in Path(relative).parts:
                raise InvalidArgument(
                    "The manifest of '%s' has the %s '%s', which is not inside the design"
                    % (found["name"], what, relative)
                )
            path = (folder / relative).resolve()
            if folder not in path.parents:
                raise InvalidArgument(
                    "The %s '%s' of '%s' leads outside the design" % (what, relative, found["name"])
                )
            return path

        entries = [e for e in manifest.get("files", []) if isinstance(e, dict)]
        rtl = [e for e in entries if e.get("role") == "rtl"]
        if not rtl:
            raise InvalidArgument(
                "The design '%s' has no RTL to synthesize%s"
                % (found["name"], "; it has a netlist: read it with read_inputs" if any(
                    e.get("role") == "netlist" for e in entries) else "")
            )
        paths = [inside(e.get("path"), "file") for e in rtl]
        for path, entry in zip(paths, rtl):
            if path.suffix.lower() not in synthesis.SOURCE_SUFFIXES:
                raise InvalidArgument(
                    "The design '%s' has the file '%s', which is not Verilog or SystemVerilog "
                    "(this tool synthesizes those)" % (found["name"], entry.get("path"))
                )
            if not path.is_file():
                raise InvalidArgument("The file '%s' of '%s' does not exist" % (entry.get("path"), found["name"]))
        rtl_block = manifest.get("rtl") if isinstance(manifest.get("rtl"), dict) else {}
        dirs = [inside(d, "include folder") for d in rtl_block.get("include_dirs", [])]
        top = manifest.get("top")
        if not isinstance(top, str):
            raise InvalidArgument("The manifest of '%s' has no top" % found["name"])
        options = {
            "defines": rtl_block.get("defines") or {},
            "read_options": rtl_block.get("read_options") or [],
        }
        return [p.name for p in paths], paths, top, dirs, options

    def _circuit_not_read(plan, result: Dict[str, Any], error: BaseException):
        """The netlist was made and the flow cannot read it: not a success, because the stages
        that follow have no circuit. What the netlist has is in the data"""
        text = str(error)
        reason = text.split(":\n", 1)[-1].strip().splitlines()[-1][:300] if text else "unknown"
        models = sorted(result.get("subckts") or {})
        if "Subckt instantiates model" in text or "no such model" in text:
            hint = (
                "The design has cells that the device has no primitive for (%s), for example a "
                "multiplier on a device with no DSP: change the Verilog, or use a device that has "
                "them" % (", ".join(models) or "see subckts")
            )
        elif "extended BLIF" in text:
            hint = "The device reads extended BLIF (.eblif): its package must say circuit_format eblif"
        else:
            hint = "read_log or the netlist (%s) tells more" % plan.blif.name
        item = answers.diagnostic(
            "circuit_not_read",
            "The netlist %s was made but cannot be read as the circuit: %s" % (plan.blif.name, reason),
            hint,
            **{k: v for k, v in result.items() if k not in ("log_tail", "circuit_read")}
        )
        return answers.failed([item])

    def _synthesis_failure(entry: Entry, plan, log: str, names: List[str], paths: List[Path], took: float):
        answer = _synthesis_failure_answer(plan, log, names, paths)
        message = answer["diagnostics"][0]["message"] if answer.get("diagnostics") else "Synthesis failed"
        report = write_report(
            entry, "synthesis", "failed", log, took, errors=[message],
            extra=[("Top module", plan.top), ("Sources", ", ".join(short(str(p)) for p in paths))],
        )
        if report:
            answer["report"] = report
        return answer

    def _synthesis_failure_answer(plan, log: str, names: List[str], paths: List[Path]):
        if "std::bad_alloc" in log:
            return answers.failed(
                [
                    answers.diagnostic(
                        "out_of_memory",
                        "Yosys ran out of memory on the design",
                        "The worker of the session has a limit on its memory (--worker-memory-mb), "
                        "or the machine is full: use a smaller design",
                        top=plan.top,
                        log_tail=synthesis.last_lines(log, 8),
                    )
                ]
            )
        message, where, line, errors = synthesis.error_details(log)
        hint = "Fix the Verilog (write_file) and call synthesize again"
        if re.search(r"Module `[^']*' not found", message):
            modules = sorted(
                {m for p in paths for m in re.findall(r"^\s*module\s+([A-Za-z_][\w$]*)", p.read_text(), re.M)}
            )
            hint = "top is the name of a module of the files: %s" % (", ".join(modules) or "none found")
        item = answers.diagnostic(
            "yosys_error",
            message,
            hint,
            top=plan.top,
            errors=errors[:5],
            log_tail=synthesis.last_lines(log, 12),
        )
        if where:
            base = Path(where).name
            item["file"] = next((n for n, p in zip(names, paths) if p.name == base), base)
            item["line"] = line
        return answers.failed([item])

    @tool()
    async def run_stage(session: str, stage: Stage, from_file: Optional[str] = None):
        """Run a stage of the flow: pack, place, route or analysis, in this order. The stages
        before it that were not run yet are run first (route runs pack and place when needed),
        and "ran" lists what was run. Returns the seconds and the stages reached. When it fails the
        answer has the diagnostics and what failed (for example the router finds no legal
        routing); the data of the stage is kept, so get_congestion and the queries of the earlier
        stages still answer, and the next run of a stage starts again. With from_file (a file of
        the workspace: the packing, the placement or the routing that an earlier flow wrote) the
        results are read instead of computed; there is none for analysis. route needs a channel
        width (set_constraint)."""

        def work():
            entry = session_of(session)
            handle = entry.session
            check_room(entry)
            _choice("stage", stage, _ORDER)
            if from_file is None:
                refusal = handle.fixed_width_refusal(stage)
                if refusal:
                    raise FixedWidthOnly(refusal)
            ran: List[str] = []
            seconds = 0.0
            current = stage
            started = time.monotonic()
            try:
                if from_file is not None:
                    if stage not in _READS:
                        raise InvalidArgument("analysis has no results to read from a file")
                    path = resolve_file(config, entry.directory, from_file, workspace_only=True)
                    step = getattr(handle, _READS[stage])(path)
                else:
                    for earlier in _ORDER[: _ORDER.index(stage)]:
                        if _STAGE_NAMES[earlier] not in handle.state().reached:
                            current, started = earlier, time.monotonic()
                            step = getattr(handle, earlier)()
                            keep_log(entry, earlier, step.log)
                            write_report(entry, earlier, "passed", step.log, step.elapsed_s)
                            seconds += step.elapsed_s
                            ran.append(earlier)
                    current, started = stage, time.monotonic()
                    step = getattr(handle, stage)()
            except Exception as error:
                log = getattr(error, "log", None)
                if log is not None:
                    keep_log(entry, current, log)
                    failure = answers.from_exception(error)
                    report = write_report(entry, current, "failed", log, time.monotonic() - started,
                                          [failure_text(error)])
                    if report:
                        failure["report"] = report
                    state = handle.state()
                    failure["reached"] = list(state.reached)
                    if ran:
                        failure["ran"] = ran
                    if state.failed is not None:
                        failure["failed"] = dataclasses.asdict(state.failed)
                    return failure
                raise
            keep_log(entry, stage, step.log)
            report = write_report(entry, stage, "passed", step.log, step.elapsed_s)
            state = handle.state()
            return reply(
                {
                    "stage": stage,
                    "seconds": round(seconds + step.elapsed_s, 2),
                    "reached": list(state.reached),
                    **({"ran": ran + [stage]} if ran else {}),
                    **({"report": report} if report else {}),
                }
            )

        return await run(work)

    @tool()
    async def get_state(session: str):
        """Where the flow of a session is: the stages reached, the inputs, the files of the
        results, the stage that failed and why, and what each stage did (clusters, placement
        cost, routing iterations and wirelength, seconds). Use it first to see what to do
        next."""

        def work():
            handle = session_of(session).session
            data: Dict[str, Any] = {"state": _plain("session", handle.state()), "run": None}
            try:
                data["run"] = _plain("run_report", handle.run_report())
            except Exception as error:  # no data of VPR before the inputs are read
                if getattr(error, "diagnostics", None) is None:
                    raise
            return reply(data)

        return await run(work)

    # The options and the constraints -------------------------------------------------------

    @tool()
    async def list_options(
        session: str,
        stage: Optional[str] = None,
        query: Optional[str] = None,
        changed: bool = False,
        limit: int = 20,
    ):
        """The options that tune the stages: for each stage, the options with their value,
        default and choices, and the stages that a change drops. stage is pack, place, route,
        analysis or a name like PLACED (all if not given); query is a pattern of option names,
        such as 'place.*'; changed keeps the options that are not at their default; limit is
        for each stage. Ask for a stage or a pattern, not for all."""

        def work():
            count = _number("limit", limit, 20, 1, 200)
            report = session_of(session).session.stage_info(
                stage=_text("stage", stage),
                changed=changed,
                query=_text("query", query),
                limit=count,
            )
            return reply(
                _plain("stage_info", report),
                "call list_options again with a stage or a query that is narrower, or a higher "
                "limit",
            )

        return await run(work)

    @tool()
    async def describe_option(session: str, name: str):
        """One option: its type, value, default, help text, choices, the stage it changes and
        the stages that a change drops (for example place.seed)."""

        def work():
            handle = session_of(session).session
            option = handle.options.get(_text("name", name, True))
            data = dataclasses.asdict(option)
            info = handle.stage_info(stage=option.stage, limit=1).stages
            data["dropped_by_change"] = list(info[0].dropped_by_change) if info else []
            return reply(data)

        return await run(work)

    @tool()
    async def set_option(session: str, name: str, value: Union[bool, int, float, str]):
        """Set an option of the flow (see list_options) and say what it dropped: the stages that
        have to run again. value has the type of the option. An option that names a file is not
        set here: set_constraint takes a constraints file, read_inputs the inputs."""

        def work():
            handle = session_of(session).session
            _text("name", name, True)
            if isinstance(value, str):
                _text("value", value, True)
            _check_option(name, handle.options.get(name).type)
            if name == "router.route_chan_width" and not (
                isinstance(value, int) and not isinstance(value, bool) and value > 0
            ):
                raise FixedWidthOnly(
                    "The routing runs at a fixed channel width: router.route_chan_width takes a "
                    "number of tracks above 0"
                )
            return reply(dataclasses.asdict(handle.set_option(name, value)))

        return await run(work)

    @tool()
    async def set_constraint(
        session: str,
        kind: Literal["vpr_constraints", "sdc", "chan_width"],
        file: Optional[str] = None,
        width: Optional[int] = None,
        clear: bool = False,
    ):
        """Set or take away an input that changes the whole flow, and say what it dropped.
        kind vpr_constraints: the floorplan constraints of VPR (an XML file of the workspace).
        kind sdc: the timing constraints (an SDC file). kind chan_width: route with a channel of
        this many tracks (width), which can be changed but not cleared. clear takes a file away."""

        def work():
            entry = session_of(session)
            handle = entry.session
            _choice("kind", kind, ("vpr_constraints", "sdc", "chan_width"))
            if kind == "chan_width":
                if clear:
                    raise FixedWidthOnly(
                        "The routing runs at a fixed channel width, which cannot be taken away: "
                        "give another width"
                    )
                if width is None or file is not None:
                    error = InvalidArgument("chan_width takes a width, and no file")
                    error.hint = "For a channel width: kind=chan_width and width=N"
                    raise error
                value = _number("width", width, 0, 1, 10000)
                return reply(dataclasses.asdict(handle.set_route_chan_width(value)))
            if (file is None) == (not clear) or width is not None:
                error = InvalidArgument("%s takes a file, or clear, and no width" % kind)
                error.hint = (
                    "For a channel width use kind=chan_width and width=N; "
                    "%s takes file=NAME (or clear=true)" % kind
                )
                raise error
            path = None if clear else resolve_file(config, entry.directory, file)
            change = (
                handle.read_vpr_constraints if kind == "vpr_constraints" else handle.read_sdc
            )(path)
            return reply(dataclasses.asdict(change))

        return await run(work)

    # The queries ---------------------------------------------------------------------------

    @tool()
    async def get_timing(session: str, paths: Optional[int] = None):
        """The timing of the routed design (after analysis): slack, critical path delay and Fmax
        for each pair of clocks, and the worst paths with their points (default 3, at most 20)."""

        def work():
            count = _number("paths", paths, 3, 1, 20)
            handle = session_of(session).session
            return reply(
                {
                    "timing_summary": _plain("timing_summary", handle.timing_summary()),
                    "critical_paths": _plain("critical_paths", handle.critical_paths(count)),
                },
                "call get_timing again with fewer paths",
            )

        return await run(work)

    @tool()
    async def get_device(
        session: str, tile_type: Optional[str] = None, limit: Optional[int] = None
    ):
        """The device, as soon as the inputs are read: the grid, the channel width, the types of
        tiles and the I/O of the sides. With tile_type it lists where the tiles of that type
        are (at most limit, default 50)."""

        def work():
            count = _number("limit", limit, 50, 1, 1000)
            device = session_of(session).session.device(
                tile_type=_text("tile_type", tile_type), limit=count
            )
            return reply(_plain("device", device), "call get_device again with a higher limit")

        return await run(work)

    @tool()
    async def get_utilization(session: str):
        """How much of the device the design uses, for each type of block (after pack)."""

        def work():
            return reply(_plain("utilization", session_of(session).session.utilization()))

        return await run(work)

    @tool()
    async def find(
        session: str,
        pattern: str,
        kind: Optional[Literal["block", "net", "cluster"]] = None,
        limit: Optional[int] = None,
    ):
        """Find blocks, nets or clusters by a shell-style name pattern ('out:*', 'n1?'). Without
        kind it looks for blocks and nets. A block has its location once placed. Gives names to
        use with get_block and get_net. limit is at most 200, default 20."""

        def work():
            count = _number("limit", limit, 20, 1, 200)
            handle = session_of(session).session
            _text("pattern", pattern, True)
            _choice("kind", kind, ("block", "net", "cluster"))
            if kind == "cluster":
                data = _plain("cluster_report", handle.cluster_report(pattern, limit=count))
            else:
                data = _plain("find", handle.find(pattern, kind=kind, limit=count))
            return reply(data, "call find again with a narrower pattern or a higher limit")

        return await run(work)

    @tool()
    async def get_block(session: str, block: str):
        """A cluster: its pins and nets, where it is placed, and the blocks of the circuit that
        it holds. block is a name from find. A long list is cut."""

        def work():
            data = _plain(
                "block_info", session_of(session).session.block_info(_text("block", block, True))
            )
            return reply(data, "the lists are cut; find gives the nets, get_net one of them")

        return await run(work)

    @tool()
    async def get_net(session: str, net: str, route: bool = False):
        """A net between clusters: what it connects, its bounding box, the size of its routing
        and how critical it is, as far as the stages reached tell. With route the routing tree
        is added (after route), which is long and is cut."""

        def work():
            handle = session_of(session).session
            wanted = _text("net", net, True)
            data = _plain("net_info", handle.net_info(wanted))
            if route:
                data = {
                    "net_info": data,
                    "net_route": _plain("net_route", handle.net_route(wanted)),
                }
            return reply(data, "the routing tree is cut; ask with route false for the summary")

        return await run(work)

    @tool()
    async def get_placement(
        session: str,
        type: Optional[str] = None,
        prefix: Optional[str] = None,
        limit: Optional[int] = None,
    ):
        """Where the blocks are placed (after place): the grid location of each block, of one
        type of block (type, for example io or clb) and/or whose name starts with prefix. limit
        is at most 200, default 20."""

        def work():
            count = _number("limit", limit, 20, 1, 200)
            data = session_of(session).session.placement(
                block_type=_text("type", type), prefix=_text("prefix", prefix), limit=count
            )
            return reply(
                _plain("placement", data),
                "call get_placement again with a type or a prefix, or a higher limit",
            )

        return await run(work)

    @tool()
    async def get_congestion(
        session: str, region_size: Optional[int] = None, hotspots: Optional[int] = None
    ):
        """How full the routing channels are, by regions of region_size tiles, and the hotspots
        (default 5, at most 50): the fullest channel segments with the nets through them. It
        answers after a route that failed too (routed is false): it shows the nodes that nets
        share, which is the reason of the failure."""

        def work():
            count = _number("hotspots", hotspots, 5, 1, 50)
            data = session_of(session).session.congestion(region_size=region_size, hotspots=count)
            return reply(
                _plain("congestion", data), "ask for fewer hotspots or a larger region_size"
            )

        return await run(work)

    # The logs and the checkpoints ------------------------------------------------------------

    @tool()
    async def read_log(session: str, stage: Stage, tail: Optional[int] = None):
        """The last lines that a stage printed when it ran (default 40, at most 200), with their
        number. Use it for what the queries do not tell, such as the lines of the router. A stage
        that was read from a file has no log."""

        def work():
            entry = session_of(session)
            _choice("stage", stage, ("pack", "place", "route", "analysis"))
            lines = _number("tail", tail, 40, 1, _LOG_TAIL_MAX)
            log = entry.logs.get(stage)
            if log is None:
                raise InvalidArgument(
                    "The stage %s has not been run in this session (run_stage runs it)" % stage
                )
            every = log.splitlines()
            shown: List[str] = [
                line if len(line) <= _LOG_LINE else line[:_LOG_LINE] + "..."
                for line in every[-lines:]
            ]
            data = {"stage": stage, "total_lines": len(every), "lines": shown}
            answer = reply(data, "call read_log again with a higher tail", from_start=True)
            if len(shown) < len(every) and not answer["truncated"]:
                answer["truncated"] = True
                answer["more"] = (
                    "call read_log again with a higher tail (at most %d)" % _LOG_TAIL_MAX
                )
            return answer

        return await run(work)

    if config.tcl_tool:

        @tool()
        async def run_tcl(session: str, script: str):
            """Run a Tcl script in the session, for a loop or a sweep that would take many calls.
            Every command of the OpenFPGA shell is a Tcl command, written as on its command line
            (set_app_option --name place.seed --value 3; place); a query returns its answer as a
            dict (dict get [timing_summary] setup fmax_mhz) and a command that fails is an error
            that catch takes. The interpreter is a safe one: no files, no programs, paths only in
            the session. As in run_stage, route needs the channel width to be set, and vpr (the
            whole flow in one command) is refused. Variables and procedures stay for the next
            script. Returns the result
            (the value of the last command), what puts printed (not the logs: read_log has those
            of the stages), the commands run and the stages reached."""

            def work():
                if not isinstance(script, str) or not script.strip() or len(script) > _TCL_MAX:
                    raise InvalidArgument("script is Tcl of 1 to %d characters" % _TCL_MAX)
                entry = session_of(session)
                check_room(entry)
                handle = entry.session
                try:
                    done = handle.tcl(script, safe=True, fixed_width=True)
                except Exception as error:
                    if getattr(error, "error_info", None) is None:
                        raise
                    failure = answers.from_exception(error)
                    failure["commands"] = error.commands[-_TCL_COMMANDS:]
                    if error.printed:
                        failure["printed"] = _tail(error.printed, _TCL_OUTPUT)
                    failure["reached"] = list(handle.state().reached)
                    return failure
                return reply(
                    {
                        "result": done.result,
                        "printed": _tail(done.printed, _TCL_OUTPUT),
                        "commands": done.commands[-_TCL_COMMANDS:],
                        "commands_run": len(done.commands),
                        "seconds": round(done.elapsed_s, 2),
                        "reached": list(handle.state().reached),
                    }
                )

            return await run(work)

    @tool()
    async def checkpoint(session: str, name: Optional[str] = None):
        """Save the flow under a name, to come back to with session_restore. The server also
        saves one after every stage. Returns the name and the stages saved."""

        def work():
            entry = session_of(session)
            check_room(entry)
            label = name or "checkpoint-%d" % (len(_named(config, entry)) + 1)
            if not _NAME.fullmatch(label):
                raise InvalidArgument(
                    "A name is 1 to 64 letters, digits, '_', '.' or '-', and starts with a "
                    "letter or a digit"
                )
            target = config.root / entry.directory / "named" / label
            if target.exists():
                raise InvalidArgument("There is a checkpoint '%s' already" % label)
            entry.session.checkpoint(target)
            return reply({"checkpoint": label, "stages": list(entry.session.state().reached)})

        return await run(work)


_TCL_MAX = 16000  # characters of a script
_TCL_OUTPUT = 2000  # characters of what a script printed: the end of it
_TCL_COMMANDS = 20  # command lines of the shell that an answer lists: the last ones


def _tail(text: str, size: int) -> str:
    return text if len(text) <= size else "…" + text[-size:]


def _named(config: Config, entry: Entry) -> List[str]:
    directory = config.root / entry.directory / "named"
    return sorted(p.name for p in directory.iterdir()) if directory.is_dir() else []


def restore_named(config: Config, entry: Entry, name: str):
    """Make a named checkpoint the one that a restart restores from"""
    if not _NAME.fullmatch(name):
        raise InvalidArgument("'%s' is not the name of a checkpoint" % name)
    source = config.root / entry.directory / "named" / name
    if not source.is_dir():
        known = ", ".join(_named(config, entry)) or "none"
        raise InvalidArgument("There is no checkpoint '%s' (there are: %s)" % (name, known))
    target = config.root / entry.directory / "checkpoints"
    if target.exists():
        shutil.rmtree(target)
    shutil.copytree(source, target)
