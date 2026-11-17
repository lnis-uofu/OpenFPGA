"""The limits and the places of a server"""

import dataclasses
import os
from pathlib import Path
from typing import Tuple

MAX_ANSWER_CHARS = 16000


@dataclasses.dataclass(frozen=True)
class Config:
    """root is the workspace: every session has a directory in it, and no file outside
    it is written. The limits are those of DESIGN.md."""

    root: Path
    max_sessions: int = 4
    call_timeout_s: float = 600.0
    idle_timeout_s: float = 3600.0
    # What the files of one session may take, in bytes: a stage or a checkpoint that would add to a
    # directory that is over it is refused
    session_max_bytes: int = 512 * 1024 * 1024
    # The size of an answer, in characters: 4,000 is about 1,000 tokens; 16,000 at most
    answer_chars: int = 4000
    # Folders that the inputs of a flow (architectures, circuits) can be read from, besides root
    input_dirs: Tuple[Path, ...] = ()
    # Folders that hold device packages (one folder for each package, see docs/spec_device_package.md)
    package_dirs: Tuple[Path, ...] = ()

    def folders(self) -> Tuple[str, ...]:
        """The workspace and the folders of the inputs, as they are written and as they resolve
        (a folder that is a link): paths under them are shown without them"""
        found = []
        for folder in (self.root,) + tuple(self.input_dirs):
            for text in (str(folder), str(folder.resolve())):
                if text not in found:
                    found.append(text)
        return tuple(found)

    def __post_init__(self):
        root = Path(os.path.abspath(os.fspath(self.root)))
        object.__setattr__(self, "root", root)
        object.__setattr__(
            self, "input_dirs", tuple(Path(os.path.abspath(os.fspath(d))) for d in self.input_dirs)
        )
        object.__setattr__(
            self,
            "package_dirs",
            tuple(Path(os.path.abspath(os.fspath(d))) for d in self.package_dirs),
        )
        if self.max_sessions < 1:
            raise ValueError("max_sessions must be at least 1")
        if not 500 <= self.answer_chars <= MAX_ANSWER_CHARS:
            raise ValueError("answer_chars is between 500 and %d" % MAX_ANSWER_CHARS)
        if self.session_max_bytes < 1:
            raise ValueError("session_max_bytes must be at least 1")
        if self.call_timeout_s <= 0 or self.idle_timeout_s <= 0:
            raise ValueError("the timeouts must be more than 0")
