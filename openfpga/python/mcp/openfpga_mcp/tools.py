"""The tools of the flow, the options, the queries and the logs, see DESIGN.md

Each tool is one call of a session and the answer that goes with it: the facts are the ones of the
queries. An answer has at most Config.answer_chars characters (see budget.py).
"""

import dataclasses
import os
import re
import shutil
from pathlib import Path
from typing import Any, Callable, Dict, List, Literal, Optional, Union

from . import answers, budget
from .config import Config
from .errors import InvalidArgument, OptionNotAllowed, SessionFull
from .paths import resolve_file
from .registry import Entry, SessionRegistry

Stage = Literal["pack", "place", "route", "analysis"]
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

    # The flow ----------------------------------------------------------------------------

    @tool()
    async def read_inputs(
        session: str, vpr_arch: str, layout: str, openfpga_arch: str, circuit: str
    ):
        """Read the inputs of a flow: the architecture of VPR (an XML file) with the name of its
        device layout (for example 2x2; 'auto' is not accepted), the architecture of OpenFPGA
        (XML) and the circuit (a BLIF netlist). File names are relative to the workspace or to a
        folder of the inputs: no absolute path and no '..'. Returns the state of the session."""

        def work():
            entry = session_of(session)
            where = entry.directory
            files = [
                resolve_file(config, where, name) for name in (vpr_arch, openfpga_arch, circuit)
            ]
            handle = entry.session
            handle.read_vpr_arch(files[0], layout=layout)
            handle.read_openfpga_arch(files[1])
            handle.read_circuit(files[2])
            return reply(_plain("session", handle.state()))

        return await run(work)

    @tool()
    async def run_stage(session: str, stage: Stage, from_file: Optional[str] = None):
        """Run a stage of the flow: pack, place, route or analysis, in this order; a stage needs
        the one before it. Returns the seconds it took and the stages reached. When it fails the
        answer has the diagnostics and what failed (for example the router finds no legal
        routing); the data of the stage is kept, so get_congestion and the queries of the earlier
        stages still answer, and the next run of a stage starts again. With from_file (a file of
        the workspace: the packing, the placement or the routing that an earlier flow wrote) the
        results are read instead of computed; there is none for analysis."""

        def work():
            entry = session_of(session)
            handle = entry.session
            check_room(entry)
            _choice("stage", stage, ("pack", "place", "route", "analysis"))
            try:
                if from_file is not None:
                    if stage not in _READS:
                        raise InvalidArgument("analysis has no results to read from a file")
                    path = resolve_file(config, entry.directory, from_file, workspace_only=True)
                    step = getattr(handle, _READS[stage])(path)
                else:
                    step = getattr(handle, stage)()
            except Exception as error:
                log = getattr(error, "log", None)
                if log is not None:
                    keep_log(entry, stage, log)
                    failure = answers.from_exception(error)
                    state = handle.state()
                    failure["reached"] = list(state.reached)
                    if state.failed is not None:
                        failure["failed"] = dataclasses.asdict(state.failed)
                    return failure
                raise
            keep_log(entry, stage, step.log)
            state = handle.state()
            return reply(
                {
                    "stage": stage,
                    "seconds": round(step.elapsed_s, 2),
                    "reached": list(state.reached),
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
        this many tracks (width). clear takes the constraint away (the channel width goes back
        to the search for the smallest)."""

        def work():
            entry = session_of(session)
            handle = entry.session
            _choice("kind", kind, ("vpr_constraints", "sdc", "chan_width"))
            if kind == "chan_width":
                if (width is None) == (not clear) or file is not None:
                    raise InvalidArgument("chan_width takes a width, or clear, and no file")
                value = -1 if clear else _number("width", width, 0, 1, 10000)
                return reply(dataclasses.asdict(handle.set_route_chan_width(value)))
            if (file is None) == (not clear) or width is not None:
                raise InvalidArgument("%s takes a file, or clear, and no width" % kind)
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
