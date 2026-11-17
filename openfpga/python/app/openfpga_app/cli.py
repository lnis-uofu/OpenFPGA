"""openfpga-app: start the web app"""

import argparse
import os
import signal
import sys
import time
from pathlib import Path


def _package_dirs(given):
    """The folders of device packages: the ones asked for, or those of this repository"""
    if given:
        return tuple(given)
    base = os.environ.get("OPENFPGA_PATH")
    folder = Path(base) / "openfpga_flow" / "device-packages" if base else None
    return (folder,) if folder is not None and folder.is_dir() else ()


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(
        prog="openfpga-app",
        description="The local web app: a chat, the state of a session, a console",
    )
    parser.add_argument("--host", default="127.0.0.1")
    parser.add_argument("--port", type=int, default=8770)
    parser.add_argument(
        "--console", action="store_true", help="turn on the console: it runs any command"
    )
    parser.add_argument(
        "--allow-public-bind", action="store_true", help="listen on an address that is not local"
    )
    parser.add_argument("--demo", action="store_true", help="replay a recorded conversation")
    parser.add_argument("--root", type=Path, help="the workspace: a directory for each session")
    parser.add_argument(
        "--designs-dir", action="append", default=[], type=Path, help="a folder to read inputs from"
    )
    parser.add_argument(
        "--device-packages",
        action="append",
        default=[],
        type=Path,
        help="a folder of device packages, to create sessions from (default: "
        "openfpga_flow/device-packages of $OPENFPGA_PATH)",
    )
    parser.add_argument(
        "--model-url",
        help="the endpoint of the model, e.g. http://127.0.0.1:8081/v1 (llama-server)",
    )
    parser.add_argument("--model-name", default="local", help="the name that the endpoint knows")
    parser.add_argument(
        "--tool-mode",
        choices=("native", "json"),
        default="native",
        help="how the model calls tools",
    )
    parser.add_argument("--context-tokens", type=int, default=8192)
    parser.add_argument("--max-steps", type=int, default=20)
    parser.add_argument(
        "--no-auto-session",
        action="store_true",
        help="do not open a session for a tool call that needs one when none is open",
    )
    parser.add_argument(
        "--record-dir", type=Path, help="a record of each conversation is written here"
    )
    parser.add_argument("--max-sessions", type=int, default=4)
    parser.add_argument("--call-timeout", type=float, default=600.0, metavar="S")
    parser.add_argument("--open", action="store_true", help="open the browser")
    args = parser.parse_args(argv)
    try:
        from .config import AppConfig

        config = AppConfig(
            host=args.host,
            port=args.port,
            console=args.console,
            allow_public_bind=args.allow_public_bind,
            model_endpoint=args.model_url,
            model_name=args.model_name if args.model_url else None,
        )
    except ValueError as error:
        parser.error(str(error))
    if not args.demo and args.root is None:
        parser.error("give --root, the workspace, or --demo to replay a recorded conversation")
    try:
        import uvicorn

        from .backend import DemoBackend
        from .server import create_app

        if args.demo:
            backend = DemoBackend()
        else:
            from openfpga_mcp.config import Config

            from .agent import AgentConfig
            from .agent.openai_compat import OpenAICompatModel
            from .session_backend import SessionBackend

            model = agent_config = record = None
            if args.model_url:
                agent_config = AgentConfig(
                    base_url=args.model_url,
                    model=args.model_name,
                    tool_mode=args.tool_mode,
                    context_tokens=args.context_tokens,
                    max_steps=args.max_steps,
                    auto_session=not args.no_auto_session,
                )
                model = OpenAICompatModel(agent_config)
                if args.record_dir:
                    args.record_dir.mkdir(parents=True, exist_ok=True)
                    record = args.record_dir / ("conversation-%d.jsonl" % int(time.time()))
            backend = SessionBackend(
                Config(
                    root=args.root,
                    design_dirs=tuple(args.designs_dir),
                    package_dirs=_package_dirs(args.device_packages),
                    max_sessions=args.max_sessions,
                    call_timeout_s=args.call_timeout,
                ),
                model=model,
                agent_config=agent_config,
                record=record,
            )
    except ModuleNotFoundError as error:
        print(
            "The web app cannot start: %s is missing (it needs starlette, uvicorn, and for real "
            "sessions the openfpga module)" % error.name,
            file=sys.stderr,
        )
        return 2
    address = "http://%s:%d/?token=%s" % (
        "localhost" if config.host in ("127.0.0.1", "::1") else config.host,
        config.port,
        config.token,
    )
    print("OpenFPGA app: %s" % address, flush=True)
    if args.open:
        import webbrowser

        webbrowser.open(address)
    signal.signal(signal.SIGTERM, lambda *_: sys.exit(143))
    try:
        uvicorn.run(
            create_app(config, backend), host=config.host, port=config.port, log_level="warning"
        )
    finally:
        close = getattr(backend, "close", None)
        if close:
            close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
