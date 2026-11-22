"""File names of the arguments, resolved inside the workspace

A name is relative: no absolute path, no '..', and a symlink that leaves the allowed folders is
refused (checked after it is resolved). A name is looked for in the directory of the session, in
the workspace, and in the designs folders, in this order. A short name (no folder in it) that is in
none of these places directly is looked for in the folders under the designs folders, up to a depth
and a number of files; two files of that name are an error that lists them, never a choice.
"""

import difflib
import json
import os
import re
import shutil
from pathlib import Path
from typing import List, Optional

from .config import Config
from .errors import AmbiguousFile, FileNotInWorkspace, InvalidArgument, PathRefused, WrongFileKind


def _inside(path: Path, base: Path) -> bool:
    try:
        path.relative_to(base)
    except ValueError:
        return False
    return True


DESIGN_DEPTH = 6  # the folders under a designs folder that are looked into
DESIGN_MAX_FILES = 20000  # the files that are looked at in all of them
MANIFEST_MAX_BYTES = 256 * 1024
UPLOADS = "uploads"
CREATED = "created"  # the files that the model wrote in the session
DESIGN = "design"  # the copies of the design directories that the session synthesized
DESIGN_COPY_MAX_FILES = 500  # the files of one design directory that are copied into a session
DESIGN_COPY_MAX_BYTES = 64 * 1024 * 1024


def design_files(config: Config):
    """The files under the designs folders as (path, name from the folder), folders first by name.
    Links and names that start with a dot are skipped, and the walk is bounded"""
    seen = 0
    for base in config.design_dirs:
        stack = [(base, 0)]
        while stack:
            folder, depth = stack.pop()
            try:
                with os.scandir(folder) as scan:
                    entries = sorted(scan, key=lambda e: e.name, reverse=True)
            except OSError:
                continue
            for entry in entries:
                if entry.name.startswith(".") or entry.is_symlink():
                    continue
                if entry.is_dir(follow_symlinks=False):
                    if depth < DESIGN_DEPTH:
                        stack.append((Path(entry.path), depth + 1))
                elif entry.is_file(follow_symlinks=False):
                    seen += 1
                    if seen > DESIGN_MAX_FILES:
                        return
                    path = Path(entry.path)
                    yield path, path.relative_to(base).as_posix()


def design_directories(config: Config) -> List[dict]:
    """The design directories under the designs folders: the folders that have a manifest
    design.json (docs/spec_design_directory.md). Each is {name, folder, relative, top, rtl,
    netlists}: the name from the manifest (the folder name when it has none), the folder as a path
    and relative to its designs folder, the top module, the names of the rtl files and the paths of
    the netlists. A manifest that is not a
    JSON object, or is larger than MANIFEST_MAX_BYTES, is skipped: it is not a design directory"""
    found = []
    for path, relative in design_files(config):
        if path.name != "design.json":
            continue
        try:
            if path.stat().st_size > MANIFEST_MAX_BYTES:
                continue
            data = json.loads(path.read_text(errors="replace"))
        except (OSError, ValueError):
            continue
        if not isinstance(data, dict):
            continue
        entries = data.get("files") if isinstance(data.get("files"), list) else []
        rtl = [
            Path(e["path"]).name
            for e in entries
            if isinstance(e, dict) and e.get("role") == "rtl" and isinstance(e.get("path"), str)
        ]
        netlists = [
            e["path"]
            for e in entries
            if isinstance(e, dict) and e.get("role") == "netlist" and isinstance(e.get("path"), str)
        ]
        folder = path.parent
        name = data.get("name") if isinstance(data.get("name"), str) and data["name"] else folder.name
        top = data.get("top") if isinstance(data.get("top"), str) else None
        found.append(
            {
                "name": name,
                "folder": folder,
                "relative": Path(relative).parent.as_posix(),
                "top": top,
                "rtl": rtl,
                "netlists": netlists,
                "manifest": data,
            }
        )
    return sorted(found, key=lambda d: d["relative"])


def copy_design(found: dict, session_folder: Path) -> dict:
    """Copy a design directory into the folder 'design' of a session, under the name of its folder,
    in place of an earlier copy, and give it as find_design does with the copy as its folder. The
    files are copied as they are (links and names that start with a dot are left out) and the copy
    is bounded: the session synthesizes its own copy, which a change of the designs folder does not
    touch"""
    source = found["folder"].resolve()
    name = found["folder"].name
    if not re.fullmatch(r"[A-Za-z0-9][A-Za-z0-9_.+-]{0,127}", name):
        raise InvalidArgument("The design directory '%s' has a name that cannot be a folder of a session" % name)
    files, total = [], 0
    for folder, folders, names in os.walk(source):
        folders[:] = sorted(
            f for f in folders if not f.startswith(".") and not os.path.islink(os.path.join(folder, f))
        )
        for file_name in sorted(names):
            path = Path(folder) / file_name
            if file_name.startswith(".") or path.is_symlink() or not path.is_file():
                continue
            files.append(path)
            total += path.stat().st_size
            if len(files) > DESIGN_COPY_MAX_FILES or total > DESIGN_COPY_MAX_BYTES:
                raise InvalidArgument(
                    "The design directory '%s' has more than %d files or %d bytes: it is not copied "
                    "into the session" % (found["name"], DESIGN_COPY_MAX_FILES, DESIGN_COPY_MAX_BYTES)
                )
    base = session_folder / DESIGN
    base.mkdir(parents=True, exist_ok=True)
    temporary = base / (".%s.part" % name)
    shutil.rmtree(temporary, ignore_errors=True)
    for path in files:
        target = temporary / path.relative_to(source)
        target.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(path, target)
    target = base / name
    if target.is_symlink() or target.is_file():
        target.unlink()
    shutil.rmtree(target, ignore_errors=True)
    os.replace(temporary, target)  # a half copy is never seen under the name
    return {**found, "folder": target, "source": found["folder"], "copied": len(files)}


def _has_rtl(manifest_path: Path) -> Optional[str]:
    """The name of the design of a manifest that has RTL (None when it has none, or is not one)"""
    try:
        if manifest_path.stat().st_size > MANIFEST_MAX_BYTES:
            return None
        data = json.loads(manifest_path.read_text(errors="replace"))
    except (OSError, ValueError):
        return None
    if not isinstance(data, dict) or not isinstance(data.get("files"), list):
        return None
    if not any(isinstance(e, dict) and e.get("role") == "rtl" for e in data["files"]):
        return None
    name = data.get("name")
    return name if isinstance(name, str) and name else manifest_path.parent.name


def rtl_design_of(config: Config, session_dir: str, path: str) -> Optional[str]:
    """The name of the design directory that a file is inside when that design has RTL: in a
    designs folder, or a copy of one in the session. The RTL of a design is what is used, never a
    netlist that it has too (only a design with no RTL is read as its netlist)"""
    target = Path(path).resolve()
    bases = [Path(b).resolve() for b in config.design_dirs]
    bases.append((config.root / session_dir / DESIGN).resolve())
    for base in bases:
        if not _inside(target, base):
            continue
        folder = target.parent
        while folder != base and _inside(folder, base):
            manifest = folder / "design.json"
            if manifest.is_file() and not manifest.is_symlink():
                return _has_rtl(manifest)  # the nearest manifest is the design of the file
            folder = folder.parent
    return None


def find_design(config: Config, name: str) -> dict:
    """The design directory of that name (its manifest name, its folder name, or its path under
    a designs folder). Two of the same name are an error that lists them, never a choice"""
    if not isinstance(name, str) or not name.strip():
        raise InvalidArgument("design is the name of a design directory")
    designs = design_directories(config)
    found = [d for d in designs if name in (d["name"], d["relative"], d["folder"].name)]
    if len(found) == 1:
        return found[0]
    if found:
        raise AmbiguousFile(
            "There are %d design directories named '%s': %s; give the path"
            % (len(found), name, ", ".join(d["relative"] for d in found))
        )
    raise FileNotInWorkspace(
        "There is no design directory '%s' (a folder with a design.json) in the designs folders%s"
        % (name, ": " + ", ".join(d["name"] for d in designs[:30]) if designs else "")
    )


def resolve_file(config: Config, session_dir: str, name: str, workspace_only: bool = False) -> str:
    """The whole path of a file that exists. workspace_only leaves out the designs folders
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
        allowed += [d.resolve() for d in config.design_dirs]
    # The files that the person uploaded to the session are in its folder 'uploads'
    bases = [
        config.root / session_dir,
        config.root / session_dir / UPLOADS,
        config.root / session_dir / CREATED,
        config.root,
    ]
    if not workspace_only:
        bases += list(config.design_dirs)
    for base in bases:
        candidate = base / name
        if not candidate.is_file():
            continue
        real = candidate.resolve()
        if not any(_inside(real, a) for a in allowed):
            raise PathRefused("'%s' leads outside the workspace" % name)
        return str(real)
    if not workspace_only and "/" not in name:
        found = {}
        for path, relative in design_files(config):
            if path.name == name:
                found.setdefault(path.resolve(), relative)
        if len(found) == 1:
            real = next(iter(found))
            if not any(_inside(real, a) for a in allowed):
                raise PathRefused("'%s' leads outside the workspace" % name)
            return str(real)
        if found:
            shown = sorted(found.values())
            error = AmbiguousFile(
                "There are %d files named '%s' in the designs folders: %s%s"
                % (len(shown), name, ", ".join(shown[:5]), ", ..." if len(shown) > 5 else "")
            )
            error.hint = "Give the path, for example %s" % shown[0]
            raise error
    error = FileNotInWorkspace(
        "There is no file '%s' in the workspace%s"
        % (name, "" if workspace_only else " or in the designs folders"),
    )
    if not workspace_only and any(
        name in (d["name"], d["relative"]) for d in design_directories(config)
    ):
        # A design directory is not a file: say what to do with it, in the words of the tool
        error = FileNotInWorkspace(
            "'%s' is a design directory, not a file: use synthesize with design='%s'" % (name, name)
        )
        error.hint = "Verilog is synthesized (use_device_package first); only a BLIF is read"
        raise error
    close, more = _close_names(config, name, bases, not workspace_only)
    if close:
        error.hint = "Did you mean: %s%s?" % (
            ", ".join(close),
            " (and %d more that contain it)" % more if more else "",
        )
    raise error


def _close_names(config: Config, name: str, bases: List[Path], designs: bool = True):
    """The names of the files that a wrong name may have meant: one that contains it, or one that is
    like it. A model gives half of a name, or a name that it remembers wrong"""
    names = set()
    for base in bases:
        try:
            names.update(p.name for p in base.iterdir() if p.is_file())
        except OSError:
            continue
    if designs:
        names.update(path.name for path, _ in design_files(config))
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
        error.hint = "Look for another file with the names that the designs folders have"
    raise error
