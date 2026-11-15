"""The limits and the places of a server"""

import dataclasses
import os
from pathlib import Path
from typing import Tuple


@dataclasses.dataclass(frozen=True)
class Config:
    """root is the workspace: every session has a directory in it, and no file outside
    it is written. The limits are those of DESIGN.md."""

    root: Path
    max_sessions: int = 4
    call_timeout_s: float = 600.0
    idle_timeout_s: float = 3600.0
    # Folders that the inputs of a flow (architectures, circuits) can be read from, besides root
    input_dirs: Tuple[Path, ...] = ()

    def __post_init__(self):
        root = Path(os.path.abspath(os.fspath(self.root)))
        object.__setattr__(self, "root", root)
        object.__setattr__(
            self, "input_dirs", tuple(Path(os.path.abspath(os.fspath(d))) for d in self.input_dirs)
        )
        if self.max_sessions < 1:
            raise ValueError("max_sessions must be at least 1")
        if self.call_timeout_s <= 0 or self.idle_timeout_s <= 0:
            raise ValueError("the timeouts must be more than 0")
