"""Reading a file for the page to show: a result of a flow, an input, a file that was uploaded.

Only the files that the tools may read (the directory of the session with its uploads, the
workspace, the designs folders) and the device packages. It is read only, at most one
megabyte of text goes to the page, and a file that is not text is refused.
"""

import os
from pathlib import Path
from typing import Any, Dict

from openfpga_mcp.config import Config
from openfpga_mcp.errors import FileNotInWorkspace, InvalidArgument, RegistryError
from openfpga_mcp.paths import resolve_file

MAX_SHOWN_BYTES = 1024 * 1024


class NotText(RegistryError):
    kind = "binary_file"
    hint = "Only text files can be shown"


def _in_packages(config: Config, name: str) -> str:
    """A file inside a device package, by its path from the folder of the packages"""
    if os.path.isabs(name) or "\\" in name or ".." in Path(name).parts:
        raise FileNotInWorkspace("There is no file '%s'" % name)
    for folder in config.package_dirs:
        candidate = folder / name
        if candidate.is_file():
            real = candidate.resolve()
            if folder.resolve() in real.parents:
                return str(real)
    raise FileNotInWorkspace("There is no file '%s'" % name)


def read_text(config: Config, session_dir: str, name: str) -> Dict[str, Any]:
    """The text of a file (cut at MAX_SHOWN_BYTES), with its size"""
    if not isinstance(name, str) or not name.strip() or len(name) > 400:
        raise InvalidArgument("A file name is needed")
    try:
        path = resolve_file(config, session_dir, name)
    except FileNotInWorkspace as not_found:
        try:
            path = _in_packages(config, name)
        except FileNotInWorkspace:
            raise not_found
    size = os.path.getsize(path)
    with open(path, "rb") as handle:
        raw = handle.read(MAX_SHOWN_BYTES + 1)
    if b"\0" in raw[:8192]:
        raise NotText("'%s' is not a text file" % name)
    truncated = len(raw) > MAX_SHOWN_BYTES
    return {
        "name": name,
        "text": raw[:MAX_SHOWN_BYTES].decode("utf-8", errors="replace"),
        "bytes": size,
        "truncated": truncated,
    }
