"""Reading a file for the page to show: a result of a flow, an input, a file that was uploaded.

Only the files that the tools may read (the directory of the session with its uploads, the
workspace, the designs folders) and the device packages. It is read only, at most one
megabyte of text goes to the page, and a file that is not text is refused.
"""

import os
import shutil
from pathlib import Path
from typing import Any, Dict

from openfpga_mcp.config import Config
from openfpga_mcp.errors import FileNotInWorkspace, InvalidArgument, RegistryError
from openfpga_mcp import filenames
from openfpga_mcp.paths import CREATED, UPLOADS, resolve_file

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
        # A file that the person gave or that the model wrote is edited in place; a cut text is not
        "editable": not truncated and _editable(config, session_dir, path),
        # Any other file of a kind that a flow reads can be copied into the session and edited there
        "copyable": not _editable(config, session_dir, path)
        and filenames.name_problem(os.path.basename(path)) is None,
    }


def _folders(config: Config, session_dir: str):
    return [(config.root / session_dir / name).resolve() for name in (UPLOADS, CREATED)]


def _editable(config: Config, session_dir: str, path: str) -> bool:
    """Only the files of the folders 'uploads' and 'created' of the session (never a link), with a
    name that a flow reads: the inputs of the person, the results and the packages are not changed"""
    if not session_dir or os.path.islink(path):
        return False
    return (
        Path(path).resolve().parent in _folders(config, session_dir)
        and filenames.name_problem(os.path.basename(path)) is None
    )


def write_text(config: Config, session_dir: str, name: str, text: str) -> Dict[str, Any]:
    """Replace the text of a file of the session that may be edited (see _editable)"""
    path = resolve_file(config, session_dir, name)
    if not _editable(config, session_dir, path):
        raise InvalidArgument(
            "'%s' cannot be edited here: only the files of the session's uploads and of those "
            "that the model wrote" % name
        )
    data = text.encode("utf-8")
    if len(data) > MAX_SHOWN_BYTES:
        raise InvalidArgument("The text is %d bytes; the most that is saved is %d" % (len(data), MAX_SHOWN_BYTES))
    target = Path(path)
    temporary = target.with_name(".%s.part" % target.name)
    temporary.write_bytes(data)
    os.replace(temporary, target)  # a half file is never seen under the name
    return {"name": name, "bytes": len(data), "kind": filenames.kind_of(target), "path": str(target)}


def copy_to_uploads(config: Config, session_dir: str, name: str) -> Dict[str, Any]:
    """A copy of a file that may not be edited, in the uploads of the session under its own plain
    name: the person edits the copy. A copy that is there is not replaced"""
    path = resolve_file(config, session_dir, name)
    base = os.path.basename(path)
    problem = filenames.name_problem(base)
    if problem:
        raise InvalidArgument(problem)
    folder = config.root / session_dir / UPLOADS
    folder.mkdir(parents=True, exist_ok=True)
    target = folder / base
    if target.exists():
        raise InvalidArgument("There is already a file '%s' in the session's uploads" % base)
    if os.path.getsize(path) > MAX_SHOWN_BYTES:
        raise InvalidArgument("The file is larger than the %d bytes that can be edited" % MAX_SHOWN_BYTES)
    temporary = folder / (".%s.part" % base)
    shutil.copyfile(path, temporary)
    os.replace(temporary, target)
    return {"name": base, "bytes": target.stat().st_size, "kind": filenames.kind_of(target)}
