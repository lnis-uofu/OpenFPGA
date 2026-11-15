"""File names of the arguments, resolved inside the workspace

A name is relative: no absolute path, no '..', and a symlink that leaves the allowed folders is
refused (checked after it is resolved). A name is looked for in the directory of the session, in
the workspace, and in the folders of the inputs, in this order.
"""

import os
from pathlib import Path
from typing import List

from .config import Config
from .errors import FileNotInWorkspace, InvalidArgument, PathRefused


def _inside(path: Path, base: Path) -> bool:
    try:
        path.relative_to(base)
    except ValueError:
        return False
    return True


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
    bases = [config.root / session_dir, config.root]
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
    raise FileNotInWorkspace(
        "There is no file '%s' in the workspace%s"
        % (name, "" if workspace_only else " or in the folders of the inputs"),
    )
