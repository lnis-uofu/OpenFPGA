"""Synthesis with Yosys, for the `synthesize` tool

A Verilog design becomes a BLIF netlist of the LUTs of one device. The script is made, and the
arguments, the sources and the design directories are checked, by the shell's commands
synthesize and read_design (in C++), from the template of the device package (role
`yosys_script`) or from a generic one: the model gives the top module and the files, and never
writes a Yosys script, because a script can run commands, load plugins and write anywhere.
Yosys runs inside the worker (the `yosys` command of the shell). This module reads what it
gives back: the errors of its log and the figures of the netlist.
"""

import re
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple


@dataclass
class Plan:
    """What one synthesis runs: the script, and where its result goes"""

    top: str
    script: str
    script_file: Path
    blif: Path  # the netlist: .blif, or .eblif when the device reads extended BLIF
    lut_size: int
    files: List[Path]
    package: Optional[str]


# The result ------------------------------------------------------------------------------------

_LOCATED = re.compile(r"^(?P<file>[^\s:][^:]*):(?P<line>\d+):(?:\d+:)? ERROR: (?P<message>.*)$")
_ERROR = re.compile(r"^ERROR: (?P<message>.*)$")


def log_errors(log: str) -> List[Dict[str, Any]]:
    """The errors that Yosys printed, with the file and the line when it says them, in order and
    without the repeats (Yosys prints an error on two streams)"""
    found: List[Dict[str, Any]] = []
    seen = set()
    for line in log.splitlines():
        located = _LOCATED.match(line)
        plain = _ERROR.match(line)
        if located:
            item = (located["file"], int(located["line"]), located["message"].strip())
        elif plain:
            item = (None, None, plain["message"].strip())
        else:
            continue
        if item in seen:
            continue
        seen.add(item)
        found.append({"file": item[0], "line": item[1], "message": item[2]})
    return found


def blif_stats(path: Path) -> Dict[str, Any]:
    """What the netlist has: LUTs (a .names with an input), flip-flops (.latch), the widest LUT,
    the ports, the constants. Read from the BLIF, which is what the flow reads next"""
    luts = latches = widest = constants = 0
    subckts: Dict[str, int] = {}
    inputs: List[str] = []
    outputs: List[str] = []
    model = None
    text = path.read_text()
    for raw in text.replace("\\\n", " ").splitlines():
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        words = line.split()
        if words[0] == ".model" and model is None and len(words) > 1:
            model = words[1]
        elif words[0] == ".inputs":
            inputs += words[1:]
        elif words[0] == ".outputs":
            outputs += words[1:]
        elif words[0] == ".latch":
            latches += 1
        elif words[0] == ".subckt" and len(words) > 1:
            subckts[words[1]] = subckts.get(words[1], 0) + 1
        elif words[0] == ".names":
            width = len(words) - 2
            if width == 0:
                constants += 1
            else:
                luts += 1
                widest = max(widest, width)
    return {
        "model": model,
        "luts": luts,
        "flip_flops": latches,
        "widest_lut": widest,
        "constants": constants,
        "inputs": len(inputs),
        "outputs": len(outputs),
        "subckts": subckts,  # the cells that are not LUTs or flip-flops: model name and count
    }


def last_lines(log: str, count: int = 15) -> str:
    return "\n".join([l for l in log.splitlines() if l.strip()][-count:])


def error_details(log: str) -> Tuple[str, Optional[str], Optional[int], List[Dict[str, Any]]]:
    """The message of the first error, its place, and every error"""
    errors = log_errors(log)
    if not errors:
        return "Yosys failed", None, None, []
    first = errors[0]
    return first["message"], first["file"], first["line"], errors
