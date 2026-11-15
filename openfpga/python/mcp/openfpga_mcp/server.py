"""The server: tools that keep and drive sessions, over stdio or HTTP"""

import argparse
import signal
import sys
from pathlib import Path
from typing import Any, Dict, Optional

import anyio.to_thread
from mcp.server.mcpserver import MCPServer

from . import answers
from .config import Config
from . import resources, session_tools, tools
from .registry import SessionRegistry

_LOOPBACK = ("127.0.0.1", "localhost", "::1")


def build_server(config: Config, registry: Optional[SessionRegistry] = None) -> MCPServer:
    """A server over the sessions of a registry (a new one for config if none is given).
    The sessions belong to the registry and not to a connection: whoever runs the server ends the
    workers with registry.close_all(), as main() does"""
    registry = registry or SessionRegistry(config)

    server = MCPServer(
        "openfpga",
        instructions=(
            "Runs the OpenFPGA place-and-route flow in sessions. Open a session first, and give "
            "its id to every other tool. A failure comes back as ok: false with diagnostics "
            "that say what happened and what to try."
        ),
    )
    server.registry = registry  # for tests and for the code that embeds the server

    async def run(function) -> str:
        """Run a call that waits (on a worker) in a thread, so that the server answers others
        meanwhile, and make its failure an answer. The tool returns the text of the answer"""
        try:
            return answers.text(await anyio.to_thread.run_sync(function))
        except Exception as error:  # noqa: BLE001 - every failure is data for the model
            return answers.text(answers.from_exception(error))

    def tool():
        return server.tool(structured_output=False)

    session_tools.register(registry, config, tool, run)
    tools.register(server, registry, config, tool, run)
    resources.register(server, registry, config)
    return server


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        prog="openfpga-mcp", description="A Model Context Protocol server over OpenFPGA sessions"
    )
    parser.add_argument("--root", required=True, type=Path, help="the workspace directory")
    parser.add_argument("--input-dir", action="append", default=[], type=Path)
    parser.add_argument("--max-sessions", type=int, default=Config.max_sessions)
    parser.add_argument("--call-timeout", type=float, default=Config.call_timeout_s, metavar="S")
    parser.add_argument("--idle-timeout", type=float, default=Config.idle_timeout_s, metavar="S")
    parser.add_argument("--answer-chars", type=int, default=Config.answer_chars, metavar="N")
    parser.add_argument("--session-max-mb", type=int, default=512, metavar="MB")
    parser.add_argument("--transport", choices=("stdio", "http"), default="stdio")
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8765)
    args = parser.parse_args(argv)
    if args.transport == "http" and args.host not in _LOOPBACK:
        parser.error(
            "HTTP only listens on the local machine for now (127.0.0.1): the checks that make "
            "another address safe are not there"
        )
    config = Config(
        root=args.root,
        input_dirs=tuple(args.input_dir),
        max_sessions=args.max_sessions,
        call_timeout_s=args.call_timeout,
        idle_timeout_s=args.idle_timeout,
        answer_chars=args.answer_chars,
        session_max_bytes=args.session_max_mb * 1024 * 1024,
    )
    server = build_server(config)
    # A termination of the process ends the workers too
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))
    try:
        if args.transport == "stdio":
            server.run("stdio")
        else:
            server.run("streamable-http", host=args.host, port=args.port)
    finally:
        server.registry.close_all()
    return 0


if __name__ == "__main__":
    sys.exit(main())
