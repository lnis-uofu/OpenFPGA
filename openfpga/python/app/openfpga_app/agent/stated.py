"""The files that the person names for an input, in the words of the message

A model that is told "use A as the VPR architecture" may decide that the person meant something
else, and read other files, and the person is not told. Where the message says plainly which file
goes in which input, read_inputs is called with that: if the file is of another kind the tool says
so (wrong_file_kind) and the model has a failure to report, not a choice to make.
"""

import re
from typing import Dict

_ROLES = (
    ("vpr_arch", r"vpr\s+arch(?:itecture)?(?:\s+file)?"),
    ("openfpga_arch", r"openfpga\s+arch(?:itecture)?(?:\s+file)?"),
    ("circuit", r"(?:circuit|netlist|blif)(?:\s+file)?"),
)
_FILE = r"([A-Za-z0-9_][A-Za-z0-9_.\-]*\.(?:xml|blif))"
_FORWARD = r"\b%s\b\s+(?:as|for)\s+(?:the\s+)?%s"
_BACKWARD = r"\b%s\b\s*(?:is|=|:)?\s*%s"


def stated_files(text: str) -> Dict[str, str]:
    """{'vpr_arch': 'a.xml', ...} for the inputs that the message gives a file for, plainly"""
    found: Dict[str, str] = {}
    for role, words in _ROLES:
        for pattern, groups in (
            (_FILE + r"\s+(?:as|for)\s+(?:the\s+)?" + words + r"\b", 1),
            (r"\b" + words + r"\s*(?:is|=|:)\s*(?:the\s+file\s+)?" + _FILE, 1),
        ):
            match = re.search(pattern, text, re.IGNORECASE)
            if match:
                found[role] = match.group(groups)
                break
    return found

