"""A server for the Model Context Protocol over OpenFPGA sessions.

See README.md and DESIGN.md next to this package. The server needs the `mcp` package
(pip install 'openfpga[mcp]') and the `openfpga` module; the rest of the package does not.
"""

from .config import Config
from .errors import SessionClosed, SessionLimit, UnknownSession
from .registry import SessionRegistry


def build_server(*arguments, **options):
    """A server over the sessions of a registry, see server.build_server. The SDK is imported
    here and not with the package"""
    from .server import build_server as build

    return build(*arguments, **options)


__all__ = [
    "Config",
    "SessionClosed",
    "SessionLimit",
    "SessionRegistry",
    "UnknownSession",
    "build_server",
]
