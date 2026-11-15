"""File names of the arguments, resolved inside the workspace

A name is relative: no absolute path, no '..', and a symlink that leaves the allowed folders is
refused (checked after it is resolved). A name is looked for in the directory of the session, in
the workspace, and in the folders of the inputs, in this order.
"""

import difflib
import os
import re
from pathlib import Path
from typing import List, Optional

from .config import Config
from .errors import FileNotInWorkspace, InvalidArgument, PathRefused, WrongFileKind


def _inside(path: Path, base: Path) -> bool:
    try:
        path.relative_to(base)
    except ValueError:
        return False
    return True


UPLOADS = "uploads"


def resolve_file(config: Config, session_dir: str, name: str, workspace_only: bool = False) -> str:
    """The whole path of a file that exists. workspace_only leaves out the folders of the inputs
    (for the results of a flow, which are written in the workspace)"""
    if not isinstance(name, str) or not name.strip() or "\0" in name:
        raise InvalidArgument("A file name is needed")
    if os.path.isabs(name) or name.startswith("~") or "\\" in name:
        raise PathRefused("'%s' is not a name inside the workspace" % name)
    if ".." in Path(name).parts:
        raise PathRefused("'%s' has '..'" % name)
    root = config.root.resolve()
    allowed: List[Path] = [root]
    if not workspace_only:
        allowed += [d.resolve() for d in config.input_dirs]
    # The files that the person uploaded to the session are in its folder 'uploads'
    bases = [config.root / session_dir, config.root / session_dir / UPLOADS, config.root]
    if not workspace_only:
        bases += list(config.input_dirs)
    for base in bases:
        candidate = base / name
        if not candidate.is_file():
            continue
        real = candidate.resolve()
        if not any(_inside(real, a) for a in allowed):
            raise PathRefused("'%s' leads outside the workspace" % name)
        return str(real)
    error = FileNotInWorkspace(
        "There is no file '%s' in the workspace%s"
        % (name, "" if workspace_only else " or in the folders of the inputs"),
    )
    close, more = _close_names(name, bases)
    if close:
        error.hint = "Did you mean: %s%s?" % (
            ", ".join(close),
            " (and %d more that contain it)" % more if more else "",
        )
    raise error


def _close_names(name: str, bases: List[Path]):
    """The names of the files that a wrong name may have meant: one that contains it, or one that is
    like it. A model gives half of a name, or a name that it remembers wrong"""
    names = set()
    for base in bases:
        try:
            names.update(p.name for p in base.iterdir() if p.is_file())
        except OSError:
            continue
    inside = sorted(n for n in names if len(name) >= 4 and name.lower() in n.lower())
    like = difflib.get_close_matches(name, sorted(names), n=3, cutoff=0.6)
    found: List[str] = []
    for candidate in inside + like:
        if candidate not in found:
            found.append(candidate)
    return found[:3], max(0, len(found) - 3)


_ROOT = re.compile(r"<([A-Za-z_][\w:.-]*)")


def root_element(path: str) -> Optional[str]:
    """The name of the first element of an XML file, from its first 8 kB (comments and the
    declaration are skipped), or None if it does not look like XML"""
    try:
        with open(path, "rb") as handle:
            head = handle.read(8192).decode("utf-8", errors="replace")
    except OSError:
        return None
    head = re.sub(r"<!--.*?-->", "", head, flags=re.S)
    head = re.sub(r"<\?.*?\?>", "", head, flags=re.S)
    found = _ROOT.search(head)
    return found.group(1) if found else None


def looks_like_blif(path: str) -> bool:
    try:
        with open(path, "rb") as handle:
            for line in handle.read(8192).decode("utf-8", errors="replace").splitlines():
                text = line.strip()
                if text and not text.startswith("#"):
                    return text.startswith(".model")
    except OSError:
        pass
    return False


_KINDS = {"architecture": "vpr_arch", "openfpga_architecture": "openfpga_arch"}
_WHAT = {
    "vpr_arch": "an architecture of VPR (root element <architecture>)",
    "openfpga_arch": "an architecture of OpenFPGA (root element <openfpga_architecture>)",
    "circuit": "a circuit in BLIF (it starts with .model)",
}


def check_kind(argument: str, name: str, path: str) -> None:
    """Refuse a file that is given for an input that it is not, and say what it is and where it
    belongs: the reading of a wrong file fails with a message of the parser, which says little"""
    if argument == "circuit":
        if looks_like_blif(path):
            return
        found = root_element(path)
        belongs = _KINDS.get(found or "")
        is_what = "an XML file (<%s>)" % found if found else "not a BLIF file"
    else:
        found = root_element(path)
        if _KINDS.get(found or "") == argument:
            return
        belongs = _KINDS.get(found or "") or ("circuit" if looks_like_blif(path) else None)
        is_what = {
            "vpr_arch": "an architecture of VPR",
            "openfpga_arch": "an architecture of OpenFPGA",
            "circuit": "a circuit in BLIF",
        }.get(belongs or "", "an XML file (<%s>)" % found if found else "not an XML file")
    error = WrongFileKind(
        "'%s' is given as %s but it is %s; %s is %s"
        % (name, argument, is_what, argument, _WHAT[argument])
    )
    if belongs and belongs != argument:
        error.hint = "Give '%s' as %s" % (name, belongs)
    else:
        error.hint = "Look for another file with the names that the folders of the inputs have"
    raise error
