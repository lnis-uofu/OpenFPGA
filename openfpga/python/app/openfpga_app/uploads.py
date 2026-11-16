"""Files that the person gives to a session: where they go, what is accepted, what they are

They are written in the folder 'uploads' of the directory of the session, and the tools find them by
their plain name (paths.resolve_file looks there). Only the names and sizes that are safe are taken:
the page is the person's own, but a file is still data that comes through a socket.
"""

import os
import re
from pathlib import Path
from typing import Any, Dict, List

from openfpga_mcp.paths import UPLOADS, looks_like_blif, root_element

# What an input of a flow can be: architectures and constraints (XML), circuits, timing constraints
EXTENSIONS = {
    ".xml",
    ".blif",
    ".eblif",
    ".v",
    ".sv",
    ".vh",
    ".sdc",
    ".pcf",
    ".json",
    ".csv",
    ".txt",
}
MAX_FILE_BYTES = 50 * 1024 * 1024
_NAME = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._+-]{0,99}$")
_KINDS = {"architecture": "vpr_arch", "openfpga_architecture": "openfpga_arch"}


class UploadRefused(Exception):
    def __init__(self, kind: str, message: str, hint: str = ""):
        super().__init__(message)
        self.kind = kind
        self.message = message
        self.hint = hint


def check_name(name: str) -> str:
    """A plain file name: letters, digits and . _ + -, up to 100, of a kind that a flow reads"""
    if not isinstance(name, str) or not _NAME.match(name) or ".." in name:
        raise UploadRefused(
            "invalid_name",
            "'%s' is not a name that can be uploaded" % (name if isinstance(name, str) else ""),
            "Use letters, digits and . _ + - (at most 100), with no folder",
        )
    if os.path.splitext(name)[1].lower() not in EXTENSIONS:
        raise UploadRefused(
            "invalid_type",
            "'%s' is not a kind of file that a flow reads" % name,
            "Accepted: %s" % ", ".join(sorted(EXTENSIONS)),
        )
    return name


def kind_of(path: Path) -> str:
    """What a file is, from its first lines: vpr_arch, openfpga_arch, circuit or other"""
    if path.suffix.lower() == ".xml":
        return _KINDS.get(root_element(str(path)) or "", "other")
    if path.suffix.lower() in (".blif", ".eblif") and looks_like_blif(str(path)):
        return "circuit"
    return "other"


def folder(root: Path, session_dir: str) -> Path:
    return root / session_dir / UPLOADS


def save(root: Path, session_dir: str, name: str, data: bytes, overwrite: bool) -> Dict[str, Any]:
    """Write the file in the uploads of the session. A name that is there is kept unless the
    person said to replace it"""
    check_name(name)
    if len(data) > MAX_FILE_BYTES:
        raise UploadRefused(
            "too_large", "'%s' is %d bytes; the most is %d" % (name, len(data), MAX_FILE_BYTES)
        )
    if not data:
        raise UploadRefused("empty", "'%s' is empty" % name)
    target = folder(root, session_dir)
    target.mkdir(parents=True, exist_ok=True)
    destination = target / name
    if destination.is_symlink() or (destination.exists() and not destination.is_file()):
        raise UploadRefused("invalid_name", "'%s' cannot be replaced" % name)
    if destination.exists() and not overwrite:
        raise UploadRefused("file_exists", "There is already a file '%s' in the session" % name)
    temporary = target / (".%s.part" % name)
    temporary.write_bytes(data)
    os.replace(temporary, destination)  # a half file is never seen under the name
    return {"name": name, "kind": kind_of(destination), "bytes": len(data)}


def listing(root: Path, session_dir: str) -> List[Dict[str, Any]]:
    target = folder(root, session_dir)
    items: List[Dict[str, Any]] = []
    try:
        for entry in sorted(target.iterdir(), key=lambda p: p.name):
            if entry.is_file() and not entry.is_symlink() and not entry.name.startswith("."):
                items.append(
                    {"name": entry.name, "kind": kind_of(entry), "bytes": entry.stat().st_size}
                )
    except OSError:
        pass
    return items


def remove(root: Path, session_dir: str, name: str) -> Dict[str, Any]:
    """Delete one uploaded file of the session (and only a file that is in its uploads)"""
    check_name(name)
    destination = folder(root, session_dir) / name
    if destination.is_symlink() or not destination.is_file():
        raise UploadRefused("file_not_found", "There is no uploaded file '%s' in the session" % name)
    size = destination.stat().st_size
    destination.unlink()
    return {"name": name, "bytes": size}
