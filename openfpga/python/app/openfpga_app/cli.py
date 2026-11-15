"""openfpga-app: start the web app"""

import argparse
import sys
from typing import Optional


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
    if not args.demo:
        print("The agent is not connected yet: start with --demo to try the page.", file=sys.stderr)
        return 2
    try:
        import uvicorn

        from .backend import DemoBackend
        from .server import create_app
    except ModuleNotFoundError as error:
        print("The web app needs starlette and uvicorn: %s" % error, file=sys.stderr)
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
    uvicorn.run(
        create_app(config, DemoBackend()),
        host=config.host,
        port=config.port,
        log_level="warning",
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
