"""Synthesis with Yosys, for the `synthesize` tool

A Verilog design becomes a BLIF netlist of the LUTs of one device. Yosys runs inside the worker
(the `yosys` command of the OpenFPGA shell), with a script that is made here from a template of
the device package (role `yosys_script`) or from the generic one below. The model gives the top
module and the files; it never writes a Yosys script, because a script can run commands, load
plugins and write anywhere. What goes into a script is checked here: plain file names, the top
module, and a few values of the package.
"""

import json
import re
import string
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Dict, List, Optional, Tuple

from .device_packages import DevicePackage
from .errors import InvalidArgument

SOURCE_SUFFIXES = (".v", ".sv", ".vh")
SCRIPT_NAME = "synth.ys"  # the only script of a session: each synthesis writes it again
MAX_SOURCE_FILES = 20
MAX_SOURCE_BYTES = 1024 * 1024

# The script of a device with no package: LUT mapping with ABC, as the flow scripts do it
GENERIC_SCRIPT = """# Yosys synthesis script for ${TOP_MODULE}
read_verilog ${READ_VERILOG_OPTIONS} ${VERILOG_FILES}
hierarchy -check -top ${TOP_MODULE}
proc
flatten
opt_expr
opt_clean
check
opt -nodffe -nosdff
fsm
opt -nodffe -nosdff
wreduce
peepopt
opt_clean
opt -nodffe -nosdff
memory -nomap
opt_clean
opt -fast -full -nodffe -nosdff
memory_map
opt -full -nodffe -nosdff
techmap
opt -fast -nodffe -nosdff
clean
abc -lut ${LUT_SIZE}
opt_clean -purge
write_blif ${OUTPUT_BLIF}
"""

_TOP = re.compile(r"^[A-Za-z_][A-Za-z0-9_$]{0,99}$")
_PATH = re.compile(r"^[A-Za-z0-9_./+-]+$")
_OPTION = re.compile(r"^-[A-Za-z]{1,20}$")
_DEFINE = re.compile(r"^-D [A-Za-z_][A-Za-z0-9_]*=[A-Za-z0-9_]+$")

# What a source file may name: `include and $readmem take a file, and a file outside the session
# would be read into the netlist (an initial value of a memory) or into the log
_FILE_USE = re.compile(
    r"""(?:`include|\$readmem[hb])\s*(?:\(\s*)?"([^"]*)"|`include\s*<([^>]*)>""", re.M
)


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


def check_top(top: Any) -> str:
    if not isinstance(top, str) or not _TOP.match(top):
        raise InvalidArgument("top is the name of the top module (letters, digits, _ and $)")
    return top


def check_sources(paths: List[Path]) -> None:
    """The files are Verilog of a size that a netlist is made from, and none of them takes a file
    from outside the session (`include "/etc/passwd"`, $readmemh with a path with '..')"""
    if not paths:
        raise InvalidArgument("files lists the Verilog files of the design (.v, .sv, .vh)")
    if len(paths) > MAX_SOURCE_FILES:
        raise InvalidArgument("at most %d files can be synthesized together" % MAX_SOURCE_FILES)
    for path in paths:
        if path.suffix.lower() not in SOURCE_SUFFIXES:
            raise InvalidArgument(
                "'%s' is not a Verilog file (%s)" % (path.name, ", ".join(SOURCE_SUFFIXES))
            )
        if path.stat().st_size > MAX_SOURCE_BYTES:
            raise InvalidArgument(
                "'%s' is larger than the %d bytes that are synthesized" % (path.name, MAX_SOURCE_BYTES)
            )
        text = path.read_text(errors="replace")
        for match in _FILE_USE.finditer(text):
            used = match.group(1) if match.group(1) is not None else match.group(2)
            if used.startswith(("/", "~")) or ".." in Path(used).parts or "\\" in used:
                raise InvalidArgument(
                    "'%s' names the file '%s', which is outside the session: use a name inside it"
                    % (path.name, used)
                )


def _package_values(package: DevicePackage, manifest: Dict[str, Any]) -> Dict[str, str]:
    """The values that the script of a package may use: its techlib files, and the options that
    its manifest gives the Yosys step"""
    values: Dict[str, str] = {}
    by_role: Dict[str, List[str]] = {}
    for entry in package.files:
        by_role.setdefault(entry["role"], []).append(str(package.path / entry["path"]))
    for role, key in (("yosys_cell_sim", "YOSYS_CELL_SIM_VERILOG"), ("yosys_bram_rules", "YOSYS_BRAM_MAP_RULES")):
        if len(by_role.get(role, [])) == 1:
            values[key] = by_role[role][0]
    for path in by_role.get("yosys_map", []):
        name = Path(path).name.lower()
        if "dsp" in name:
            values["YOSYS_DSP_MAP_VERILOG"] = path
        elif "bram" in name:
            values["YOSYS_BRAM_MAP_VERILOG"] = path
    options = (manifest.get("flow_defaults") or {}).get("yosys_options") or {}
    read = [str(o) for o in options.get("read_verilog", [])]
    dsp = [str(o) for o in options.get("dsp_map", [])]
    if not all(_OPTION.match(o) for o in read) or not all(_DEFINE.match(o) for o in dsp):
        raise InvalidArgument(
            "The options of Yosys in the manifest of '%s' are not ones that are accepted" % package.name
        )
    values["READ_VERILOG_OPTIONS"] = " ".join(read)
    values["YOSYS_DSP_MAP_PARAMETERS"] = " ".join(dsp)
    return values


def _manifest(package: DevicePackage) -> Dict[str, Any]:
    return json.loads((package.path / "device_package.json").read_text())


def make_plan(
    top: str,
    files: List[Path],
    folder: Path,
    package: Optional[DevicePackage],
    lut_size: Optional[int],
    include_dirs: Optional[List[Path]] = None,
    defines: Optional[Dict[str, str]] = None,
    read_options: Optional[List[str]] = None,
) -> Plan:
    """The script for a design, written for the folder `folder` of the session. The LUT size is the
    one of the package, or the one given when there is none"""
    top = check_top(top)
    check_sources(files)
    if package is not None:
        size = package.lut_size
        script_path = next(
            (package.path / e["path"] for e in package.files if e["role"] == "yosys_script"), None
        )
        if script_path is None:
            raise InvalidArgument("The device package '%s' has no Yosys script" % package.name)
        template = script_path.read_text()
        values = _package_values(package, _manifest(package))
    else:
        size = lut_size
        template = GENERIC_SCRIPT
        values = {"READ_VERILOG_OPTIONS": ""}
    if not isinstance(size, int) or isinstance(size, bool) or not 2 <= size <= 8:
        raise InvalidArgument(
            "lut_size (2 to 8) is needed: use_device_package gives it, or give it here"
        )
    # A device whose manifest says that its circuits are extended BLIF (.param, .attr: DSP and
    # memory cells) gets a netlist with the extension that the flow reads that format from
    extended = (
        package is not None
        and ((_manifest(package).get("flow_defaults") or {}).get("vpr_options") or {}).get(
            "circuit_format"
        )
        == "eblif"
    )
    blif = folder / ("%s.%s" % (top, "eblif" if extended else "blif"))
    names = [str(p) for p in files]
    for value in names + [str(blif)] + [v for k, v in values.items() if k.startswith("YOSYS_") and "PARAMETERS" not in k]:
        if not _PATH.match(value):
            raise InvalidArgument("The path '%s' has characters that a Yosys script does not take" % value)
    extra: List[str] = [str(o) for o in (read_options or [])]
    if not all(_OPTION.match(o) for o in extra):
        raise InvalidArgument("read_options of the design are options like -nolatches")
    for name, value in (defines or {}).items():
        if not (isinstance(name, str) and isinstance(value, str)):
            raise InvalidArgument("defines of the design are NAME: value pairs of text")
        option = "-D %s=%s" % (name, value)
        if not _DEFINE.match(option):
            raise InvalidArgument(
                "The define '%s' of the design has characters that a Yosys script does not take"
                % name
            )
        extra.append(option)
    for folder_path in include_dirs or []:
        if not _PATH.match(str(folder_path)):
            raise InvalidArgument(
                "The include folder '%s' has characters that a Yosys script does not take"
                % folder_path
            )
        extra.append("-I%s" % folder_path)
    if any(p.suffix.lower() == ".sv" for p in files):
        extra.append("-sv")
    values["READ_VERILOG_OPTIONS"] = (values["READ_VERILOG_OPTIONS"] + " " + " ".join(extra)).strip()
    values.update(
        {
            "TOP_MODULE": top,
            "VERILOG_FILES": " ".join(names),
            "LUT_SIZE": str(size),
            "OUTPUT_BLIF": str(blif),
        }
    )
    # A script may hold Yosys names with a dollar ($mul): only ${NAME} is a value
    script = string.Template(template).safe_substitute(values)
    left = sorted(set(re.findall(r"\$\{([A-Z0-9_]+)\}", script)))
    if left:
        raise InvalidArgument(
            "The script uses %s, which the tool does not give: the device needs a script of its own"
            % ", ".join("${%s}" % name for name in left)
        )
    return Plan(top, script, folder / SCRIPT_NAME, blif, size, files, package.name if package else None)


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
