"""The tools that keep and drive sessions: open, close, list, restore.

They are written for any holder of the tools (the MCP server, or the web app that calls them
itself): register() takes the decorator that stores a tool and the runner that calls one.
"""

import dataclasses
from typing import Callable, Optional

from . import answers, device_packages, tools
from .config import Config
from .registry import SessionRegistry


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
