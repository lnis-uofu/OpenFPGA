"""A server for the Model Context Protocol over OpenFPGA sessions.

See DESIGN.md next to this package. It needs the `mcp` package (pip install
.[mcp]) and the `openfpga` module.
"""

from .config import Config
from .registry import SessionClosed, SessionLimit, SessionRegistry, UnknownSession
from .server import build_server

__all__ = [
    "Config",
    "SessionClosed",
    "SessionLimit",
    "SessionRegistry",
    "UnknownSession",
    "build_server",
]
