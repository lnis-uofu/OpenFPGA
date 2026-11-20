"""The names and kinds of the files that a person or a model gives to a session

Only a plain name (no folder) of a kind that a flow reads is accepted, in the folder 'uploads' (the
person) or 'created' (the model) of the directory of the session.
"""

import os
import re
from pathlib import Path
from typing import Optional

from .paths import looks_like_blif, root_element

# What an input of a flow can be: architectures and constraints (XML), circuits, timing constraints
EXTENSIONS = frozenset(
    (".xml", ".blif", ".eblif", ".v", ".sv", ".vh", ".sdc", ".pcf", ".json", ".csv", ".txt")
)
NAME = re.compile(r"^[A-Za-z0-9][A-Za-z0-9._+-]{0,99}$")
_KINDS = {"architecture": "vpr_arch", "openfpga_architecture": "openfpga_arch"}


def name_problem(name: object) -> Optional[str]:
    """Why a name is not accepted (None when it is)"""
    if not isinstance(name, str) or not NAME.match(name) or ".." in name:
        return "'%s' is not a plain file name (letters, digits and . _ + -, at most 100)" % (
            name if isinstance(name, str) else ""
        )
    if os.path.splitext(name)[1].lower() not in EXTENSIONS:
        return "'%s' is not a kind of file that a flow reads (%s)" % (
            name,
            ", ".join(sorted(EXTENSIONS)),
        )
    return None


def kind_of(path: Path) -> str:
    """What a file is, from its first lines: vpr_arch, openfpga_arch, circuit or other"""
    if path.suffix.lower() == ".xml":
        return _KINDS.get(root_element(str(path)) or "", "other")
    if path.suffix.lower() in (".blif", ".eblif") and looks_like_blif(str(path)):
        return "circuit"
    return "other"


def unescape_lines(text: str):
    """A model sometimes writes the two characters backslash and n for a line break, and the file
    is then one long line. A text that has no line break and has those characters is read as the
    escaped form of a text with line breaks (and tabs). One that has a line break is left as it
    is: a Verilog string may hold a backslash and n. Returns the text and whether it changed"""
    if "\n" in text or "\\n" not in text:
        return text, False
    return text.replace("\\r\\n", "\n").replace("\\n", "\n").replace("\\t", "\t"), True
