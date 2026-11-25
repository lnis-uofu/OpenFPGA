"""The tools that keep and drive sessions: open, close, list, restore.

They are written for any holder of the tools (the MCP server, or the web app that calls them
itself): register() takes the decorator that stores a tool and the runner that calls one.
"""

import dataclasses
import os
from pathlib import Path
from typing import Callable, Literal, Optional

from . import answers, budget, device_packages, tools
from .config import Config
from .errors import InvalidArgument, PathRefused
from .registry import JOURNAL, SessionRegistry


# The commands that a journal may have: what changes the flow, as the Session writes them. A line
# of another command (a file that someone wrote) is not run
REPLAY_COMMANDS = frozenset(
    (
        "read_device_package",
        "read_vpr_arch",
        "read_openfpga_arch",
        "read_circuit",
        "pack",
        "place",
        "route",
        "analysis",
        "read_packing",
        "read_placement",
        "read_routing",
        "set_app_option",
        "reset_app_option",
        "yosys",
    )
)


def _inside(path: Path, base: Path) -> bool:
    try:
        path.resolve().relative_to(base.resolve())
    except ValueError:
        return False
    return True


def journal_folder(config: Config, journal: str) -> Path:
    """The folder of a journal: a session id (s3), or the path of the folder 'journal' of a session
    inside the workspace (s3/journal). Nothing else is a journal that is replayed"""
    if not isinstance(journal, str) or not journal.strip() or "\0" in journal:
        raise InvalidArgument("journal is a session id, or the folder journal of a session")
    if os.path.isabs(journal) or ".." in Path(journal).parts or "\\" in journal:
        raise PathRefused("'%s' is not a name inside the workspace" % journal)
    parts = Path(journal).parts
    if len(parts) == 1:
        folder = config.root / journal / JOURNAL
    elif len(parts) == 2 and parts[1] == JOURNAL:
        folder = config.root / journal
    else:
        raise InvalidArgument("journal is a session id (s3) or its folder (s3/journal)")
    if folder.is_symlink() or not _inside(folder, config.root) or not (folder / "replay.openfpga").is_file():
        raise InvalidArgument("There is no journal '%s'" % journal)
    return folder


def register(
    registry: SessionRegistry, config: Config, tool: Callable[[], Callable], run: Callable
) -> None:
    @tool()
    async def session_open(name: Optional[str] = None) -> str:
        """Start a session: a place-and-route flow of its own, in a directory of its own.
        Give a short name if it helps to tell sessions apart. Returns the session id to give to
        the other tools, and its directory (relative to the workspace). Fails with
        too_many_sessions when the limit is reached: close one first."""

        def work():
            entry = registry.open(name)
            return answers.ok(
                {"session": entry.id, "name": entry.name, "directory": entry.directory}
            )

        return await run(work)

    @tool()
    async def session_close(session: str) -> str:
        """End a session and its worker, also while it runs a stage. Its files stay in the
        workspace. Use it when the flow is done, or to stop one that takes too long."""

        def work():
            registry.close(session)
            return answers.ok({"session": session, "closed": True})

        return await run(work)

    @tool()
    async def session_list() -> str:
        """The open sessions: id, name, directory, whether the worker runs (and why it does
        not), the stages reached, and seconds since last used. A session that is idle for
        more than the limit is closed by the server."""

        def work():
            return answers.ok({"sessions": [dataclasses.asdict(i) for i in registry.list()]})

        return await run(work)

    @tool()
    async def session_restore(session: str, checkpoint: Optional[str] = None) -> str:
        """Start a new worker for a session whose worker ended (crashed, was killed, took longer
        than the limit of a call), and bring it back to the last stage that was finished: the
        server saves a checkpoint after each stage. The stage that was running is lost. With
        checkpoint (a name given to the checkpoint tool) it goes back to that one. Returns the
        state: the stages reached."""

        def work():
            entry = registry.get(session)
            if checkpoint is not None:
                tools.restore_named(config, entry, checkpoint)
            state = registry.restart(entry)
            return answers.ok(dataclasses.asdict(state))

        return await run(work)

    @tool()
    async def list_device_packages(name: Optional[str] = None) -> str:
        """The device packages that can be used (a package is one device with its architectures
        and the files that go with them). Without a name: for each package its name, version,
        description, layout (the grid, for example 2x2), LUT size, channel width and tags, and the
        packages that are damaged with the reason. With the name of a package: its resources, that
        is its files (role, path inside the package, size), its architectures, the reference
        design with the result it should give, and the start of its README. Fails with
        package_not_found for a name that is not there."""

        def work():
            if name is not None:
                found = device_packages.describe(device_packages.find(config, name))
                return answers.ok(found)
            found = device_packages.discover(config)
            return answers.ok(
                {
                    "packages": [
                        {
                            **p.summary(),
                            "channel_width": p.channel_width,
                            "files": len(p.files),
                        }
                        for p in found["packages"]
                    ],
                    "problems": found["problems"],
                }
            )

        return await run(work)

    @tool()
    async def get_journal(
        session: str, format: Literal["openfpga", "python"] = "openfpga", tail: Optional[int] = None
    ) -> str:
        """The commands that changed the flow of a session (the inputs, the options, the stages
        and the synthesis), in the order they were run, as the script that runs them again:
        format openfpga is the commands of the shell, python is a script. tail gives only the
        last lines. A command that failed is a comment. A long journal is cut from its start."""

        def work():
            if format not in ("openfpga", "python"):
                raise InvalidArgument("format is openfpga or python")
            entry = registry.get(session)
            folder = config.root / entry.directory / JOURNAL
            name = "replay.py" if format == "python" else "replay.openfpga"
            path = folder / name
            if not path.is_file():
                raise InvalidArgument("The session has no journal yet")
            lines = [l for l in path.read_text().splitlines() if l.strip()]
            commands = [l for l in lines if not l.startswith("#") and l != "exit"]
            if tail is not None:
                lines = lines[-max(1, int(tail)) :]
            data = {
                "format": format,
                "journal": str(folder),
                "commands": len(commands),
                "lines": lines,
            }
            return budget.fit(
                answers.relative(data, list(config.folders())), config.answer_chars, None, True
            )

        return await run(work)

    @tool()
    async def replay(journal: str, name: Optional[str] = None) -> str:
        """Run the commands of the journal of a session again, in a new session (the first session
        is not changed). journal is the id of that session, for example s1. The commands run in
        order and the replay stops at the first that fails: the new session stays, with what ran.
        Returns the new session, and how many commands ran. Use it to get back to the same point
        in a session of another time, or to run the same flow again."""

        def work():
            import openfpga

            folder = journal_folder(config, journal)
            allowed = [config.root] + list(config.design_dirs) + list(config.package_dirs)
            commands = []
            for number, raw in enumerate((folder / "replay.openfpga").read_text().splitlines(), 1):
                line = raw.strip()
                if not line or line.startswith("#"):
                    continue
                name_, args, options = openfpga._parse_line(line)
                if name_ == "exit":
                    break
                if name_ not in REPLAY_COMMANDS:
                    raise InvalidArgument(
                        "Line %d of the journal is the command '%s', which a replay does not run"
                        % (number, name_)
                    )
                file = options.get("file")
                if isinstance(file, str):
                    where = Path(file)
                    if name_ == "yosys":
                        ok = _inside(where, folder / "scripts")  # the script that was run
                    else:
                        ok = any(_inside(where, base) for base in allowed)
                    if not ok:
                        raise PathRefused(
                            "Line %d of the journal reads '%s', which is outside the workspace"
                            % (number, file)
                        )
                commands.append((number, line, name_, args, options))
            entry = registry.open(name)
            # What the page knows of the session that the journal is of: its device package
            meta = registry.read_meta(folder.parent.name)
            kept = {k: meta[k] for k in ("package_name", "device_package") if k in meta}
            if kept:
                registry.update_meta(entry, **kept)
            done = 0
            for number, line, name_, args, options in commands:
                try:
                    entry.session.command(name_, *args, **options)
                except Exception as error:  # noqa: BLE001 - the replay stops, the session stays
                    failure = answers.from_exception(error)
                    first = failure["diagnostics"][0]
                    first["message"] = "Replay stopped at line %d of the journal, '%s': %s" % (
                        number,
                        line,
                        first["message"].splitlines()[0],
                    )
                    first["data"] = {**first.get("data", {}), "session": entry.id, "ran": done}
                    first["hint"] = (
                        "Session %s has the %d commands that ran; fix what the command needs and "
                        "go on from there" % (entry.id, done)
                    )
                    return failure
                done += 1
            return answers.ok(
                {"session": entry.id, "name": entry.name, "replayed": done, "journal": str(folder)}
            )

        return await run(work)
