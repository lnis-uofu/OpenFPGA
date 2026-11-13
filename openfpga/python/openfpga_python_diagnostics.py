# What went wrong, as data: part of the module 'openfpga', see openfpga_python.i,
# which includes it after the classes generated for OpenfpgaShell.
#
# The shell and VPR report a failure as lines of a log: "Error 3: ...", a block
# of "Type:", "File:", "Line:" and "Message:", or a plain sentence. A program
# that has to decide what to do next needs the kind of failure, the file and
# line it names, and what to try, so parse_diagnostics() reads the lines into
# Diagnostic objects. OpenfpgaError.diagnostics and WorkerError.diagnostics are
# made with it.
import dataclasses
import re
from typing import Any, Dict, List, Optional


@dataclasses.dataclass(frozen=True)
class Diagnostic:
    """One thing that went wrong, or that is worth a look.

    severity is 'error' or 'warning'. kind names what happened, in words that
    do not change with the text of the message (see DIAGNOSTIC_KINDS); an error
    that is not known has the kind 'error', or 'vpr_<type>' when VPR gave it a
    type. message is the text of the log. file and line are the file of the user
    that the message names (None when it names none, or the line is not known),
    and source is where in VPR the error was raised (for the report of a bug, not
    for the user). hint is what to try, None when there is nothing to say. data
    holds the facts that the message contains, e.g., the channel width that did
    not route.
    """

    severity: str
    kind: str
    message: str
    file: Optional[str] = None
    line: Optional[int] = None
    source: Optional[str] = None
    hint: Optional[str] = None
    data: Dict[str, Any] = dataclasses.field(default_factory=dict)


# The kinds that are known. A test lists them, and the documentation too
DIAGNOSTIC_KINDS = (
    "input_not_read",
    "stage_not_reached",
    "stage_failed",
    "no_vpr_data",
    "file_not_found",
    "xml_syntax",
    "option_unknown",
    "option_value_invalid",
    "command_unknown",
    "command_option_missing",
    "layout_not_found",
    "route_failed",
    "route_unroutable",
    "worker_crashed",
    "worker_timeout",
    "worker_cancelled",
    "worker_not_running",
    "warning",
    "error",
)

# "Error 12: text", "Warning 3: text". The number counts in the whole process
_PREFIXED = re.compile(r"^(Error|Warning) (\d+):[ \t]*(.*)$")
# The lines of a block that VPR writes for its own errors
_BLOCK_LINE = re.compile(r"^(Type|File|Line|Message): ?(.*)$")
# The loaders of XML files write "path:line text", with the line -1 for none
_LOCATED = re.compile(r"^(\S+):(-?\d+) (.+)$")
# How many warnings are kept: a long run writes many, and few matter
MAX_WARNINGS = 10

# Rules for the text of a message: (regex, kind, make the data, make the hint).
# The first that matches gives the kind
_MESSAGE_RULES = [
    (
        re.compile(r"The input (\w+) has not been read: use the command '(\w+)'"),
        "input_not_read",
        lambda m: {"input": m.group(1), "command": m.group(2)},
        lambda d: "Run '%s' first" % d["command"],
    ),
    (
        re.compile(
            r"The circuit is read against the VPR architecture, which has not been read: "
            r"use the command '(\w+)'"
        ),
        "input_not_read",
        lambda m: {"input": "VPR_ARCH", "command": m.group(1)},
        lambda d: "Run '%s' first" % d["command"],
    ),
    (
        re.compile(r"the stage DEVICE has not been reached, because not all the inputs are read"),
        "stage_not_reached",
        lambda m: {"stage": "DEVICE"},
        lambda d: "Read the three inputs: read_vpr_arch, read_openfpga_arch and read_circuit",
    ),
    (
        re.compile(
            r"Cannot run '(\w+)': the stage (\w+) has not been reached, because it failed "
            r"\((.*?)\)\. Change what made it fail, and use the command '(\w+)' again"
        ),
        "stage_failed",
        lambda m: {"stage": m.group(2), "command": m.group(4), "reason": m.group(3)},
        lambda d: "The stage %s failed (%s): change what made it fail, then run '%s' again"
        % (d["stage"], d["reason"], d["command"]),
    ),
    (
        re.compile(
            r"Cannot run (?:the stage \w+|'\w+'): .*?the stage (\w+) has not been reached.*?"
            r"[Uu]se the command '(\w+)' first"
        ),
        "stage_not_reached",
        lambda m: {"stage": m.group(1), "command": m.group(2)},
        lambda d: "Run '%s' first" % d["command"],
    ),
    (
        re.compile(r"there is no data of VPR"),
        "no_vpr_data",
        lambda m: {},
        lambda d: "Read the inputs and run the stages first",
    ),
    (
        re.compile(r"Unable to load XML file '([^']+)'"),
        "xml_syntax",
        lambda m: {"file": m.group(1)},
        lambda d: "Fix the XML of '%s' at the line that the message gives" % d["file"],
    ),
    (
        re.compile(r"Failed to open file|the file '([^']+)' does not exist"),
        "file_not_found",
        lambda m: {"file": m.group(1)} if m.group(1) else {},
        lambda d: "Check that the file exists and can be read",
    ),
    (
        re.compile(r"Application option '([^']+)' does not exist"),
        "option_unknown",
        lambda m: {"option": m.group(1)},
        lambda d: "The names of the options are in the list of the options (options.list())",
    ),
    (
        re.compile(r"Invalid value for the (\w+) application option '([^']+)': (.*)"),
        "option_value_invalid",
        lambda m: {"option": m.group(2), "type": m.group(1), "reason": m.group(3)},
        lambda d: "options.get('%s') tells its type and its choices" % d["option"],
    ),
    (
        re.compile(r"Failed to find grid layout named '([^']*)' \(valid grid layouts: ([^)]*)\)"),
        "layout_not_found",
        lambda m: {
            "layout": m.group(1),
            "valid_layouts": [name.strip().strip("'") for name in m.group(2).split(",")],
        },
        lambda d: "Choose one of the fixed layouts: %s"
        % ", ".join(name for name in d["valid_layouts"] if name != "auto"),
    ),
    (
        re.compile(r"^Routing failed$"),
        "route_failed",
        lambda m: {},
        lambda d: None,
    ),
]

# Rules for the lines that have no "Error n:" before them, or that say more than
# the error does: (regex, kind, make the data, make the hint)
_LINE_RULES = [
    (
        re.compile(r"Try to call a command '([^']*)' which is not defined"),
        "command_unknown",
        lambda m: {"command": m.group(1)},
        lambda d: "The commands of the shell are listed by 'help'",
    ),
    (
        re.compile(r"Required option '([^']*)' is missing"),
        "command_option_missing",
        lambda m: {"option": m.group(1)},
        lambda d: "Give the option '%s'" % d["option"],
    ),
    (
        re.compile(r"Circuit is unroutable with a channel width factor of (\d+)"),
        "route_unroutable",
        lambda m: {"channel_width": int(m.group(1))},
        lambda d: "Raise router.route_chan_width above %d, or change the placement"
        % d["channel_width"],
    ),
]


def _slug(text):
    return re.sub(r"[^a-z0-9]+", "_", text.lower()).strip("_")


def _classify(message):
    """The kind, the data and the hint of a message, or None if no rule knows it"""
    for regex, kind, make_data, make_hint in _MESSAGE_RULES:
        found = regex.search(message)
        if found:
            data = make_data(found)
            return kind, data, make_hint(data)
    return None


def parse_diagnostics(log, error=""):
    """The Diagnostic objects of the text of a failure, in the order of the log:
    the errors, then the warnings (at most MAX_WARNINGS).

    log is what the command printed, and error is the message of the exception
    that stopped it, when there is one. Nothing is made up: a line that no rule
    knows is an error of the kind 'error', and if the log has no error line at
    all but there is an error text, that text is the diagnostic."""
    errors = []
    warnings = []
    lines = (log or "").splitlines()
    index = 0
    while index < len(lines):
        line = lines[index]
        index += 1
        for regex, kind, make_data, make_hint in _LINE_RULES:
            found = regex.search(line)
            if found:
                data = make_data(found)
                errors.append(
                    Diagnostic("error", kind, line.strip(), hint=make_hint(data), data=data)
                )
        prefixed = _PREFIXED.match(line)
        if not prefixed:
            continue
        severity = prefixed.group(1).lower()
        text = prefixed.group(3).strip()
        fields = {}
        if not text:
            # A block of VPR: the fields follow on the lines after
            while index < len(lines):
                field = _BLOCK_LINE.match(lines[index])
                if not field:
                    break
                fields[field.group(1)] = field.group(2).strip()
                index += 1
            text = fields.get("Message", "")
        diagnostic = _diagnostic(severity, text, fields)
        if diagnostic is None:
            continue
        (errors if severity == "error" else warnings).append(diagnostic)
    # A message that does not say which file, when the command was given one
    for diagnostic in errors:
        if diagnostic.kind == "file_not_found" and diagnostic.file is None:
            for line in lines:
                given = re.match(r"^--file, -f: (.+)$", line)
                if given:
                    errors[errors.index(diagnostic)] = dataclasses.replace(
                        diagnostic, file=given.group(1).strip()
                    )
                    break
    if error and not errors:
        errors.append(_diagnostic("error", error, {}))
    return errors + warnings[:MAX_WARNINGS]


def _diagnostic(severity, text, fields):
    file = None
    line_number = None
    located = _LOCATED.match(text)
    if located:
        file = located.group(1)
        line_number = int(located.group(2))
        if line_number < 0:
            line_number = None
        text = located.group(3)
    source = None
    if "File" in fields:
        source = fields["File"] + (":" + fields["Line"] if "Line" in fields else "")
    known = _classify(text)
    if known:
        kind, data, hint = known
        if kind == "file_not_found" and file is None:
            file = data.get("file")
    elif "Type" in fields:
        kind, data, hint = "vpr_" + _slug(fields["Type"]), {}, None
    else:
        kind, data, hint = "error", {}, None
    if severity == "warning":
        kind = "warning"
        hint = None
    return Diagnostic(
        severity, kind, text, file=file, line=line_number, source=source, hint=hint, data=data
    )
