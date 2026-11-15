"""openfpga-app: start the web app"""

import argparse
import signal
import sys
from pathlib import Path


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

            from .session_backend import SessionBackend

            backend = SessionBackend(
                Config(
                    root=args.root,
                    design_dirs=tuple(args.designs_dir),
                    max_sessions=args.max_sessions,
                    call_timeout_s=args.call_timeout,
                )
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
