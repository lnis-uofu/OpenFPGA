"""The command that starts the server: openfpga-mcp, or python -m openfpga_mcp

It does not import the SDK of MCP until it runs, so that a missing SDK is a sentence and not a
traceback."""

import sys


def main(argv=None) -> int:
    try:
        from .server import main as serve
    except ModuleNotFoundError as error:
        if (error.name or "").split(".")[0] in ("mcp", "mcp_types", "anyio"):
            print(
                "The MCP server needs the 'mcp' package (Python 3.10 or later): "
                "pip install 'openfpga[mcp]'",
                file=sys.stderr,
            )
            return 2
        raise
    return serve(argv)
