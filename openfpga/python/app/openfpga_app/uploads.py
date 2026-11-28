"""Files that the person gives to a session: where they go, what is accepted, what they are

They are written in the folder 'uploads' of the directory of the session, and the tools find them by
their plain name (paths.resolve_file looks there). Only the names and sizes that are safe are taken:
the page is the person's own, but a file is still data that comes through a socket.
"""

import os
from pathlib import Path
from typing import Any, Dict, List

from openfpga_mcp.filenames import EXTENSIONS, NAME as _NAME, kind_of
from openfpga_mcp.paths import CREATED, UPLOADS

MAX_FILE_BYTES = 50 * 1024 * 1024


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


def listing(root: Path, session_dir: str, name: str = UPLOADS) -> List[Dict[str, Any]]:
    """The files of the folder 'uploads' (the person's) or 'created' (the model's)"""
    target = root / session_dir / name
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


def tree_listing(root: Path, session_dir: str, name: str, limit: int = 200) -> List[Dict[str, Any]]:
    """The files under a folder of the session and its folders (the copies of design directories),
    named from the session, at most limit of them"""
    base = root / session_dir
    items: List[Dict[str, Any]] = []
    for folder, folders, names in os.walk(base / name):
        folders[:] = sorted(f for f in folders if not f.startswith(".") and not os.path.islink(os.path.join(folder, f)))
        for file_name in sorted(names):
            path = Path(folder) / file_name
            if file_name.startswith(".") or path.is_symlink() or not path.is_file():
                continue
            items.append(
                {"name": path.relative_to(base).as_posix(), "kind": kind_of(path), "bytes": path.stat().st_size}
            )
            if len(items) >= limit:
                return items
    return items


# Where a file can be removed from: the person's uploads, what the model wrote, and the copy of a
# design directory in the session (its files are named from the session: design/<folder>/...)
DESIGN = "design"
REMOVABLE = (UPLOADS, CREATED, DESIGN)


def _design_file(root: Path, session_dir: str, name: str) -> Path:
    """The file of a copy of a design in the session: a path of design/<folder>/<file>, plain
    names only, with no link on the way"""
    parts = name.split("/") if isinstance(name, str) else []
    if (
        len(parts) < 3
        or parts[0] != DESIGN
        or any(p in ("", ".", "..") or p.startswith(".") or "\\" in p for p in parts)
    ):
        raise UploadRefused(
            "invalid_name",
            "'%s' is not a file of a design in the session" % (name if isinstance(name, str) else ""),
            "Use a path of the form design/<folder>/<file>",
        )
    path = root / session_dir
    for part in parts:
        path = path / part
        if path.is_symlink():
            raise UploadRefused("invalid_name", "'%s' cannot be removed" % name)
    return path


def remove(root: Path, session_dir: str, name: str, where: str = UPLOADS) -> Dict[str, Any]:
    """Delete one file of the session, and only one that is in the place that was said: its
    uploads, what the model wrote (created), or the copy of a design (design)"""
    if where not in REMOVABLE:
        raise UploadRefused(
            "invalid_name",
            "'%s' is not a place to remove a file from" % where,
            "Use one of: %s" % ", ".join(REMOVABLE),
        )
    if where == DESIGN:
        destination = _design_file(root, session_dir, name)
    else:
        check_name(name)
        destination = root / session_dir / where / name
    if destination.is_symlink() or not destination.is_file():
        raise UploadRefused(
            "file_not_found", "There is no file '%s' in the %s of the session" % (name, where)
        )
    size = destination.stat().st_size
    destination.unlink()
    return {"name": name, "bytes": size}
