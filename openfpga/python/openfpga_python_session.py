# The Session of the Python module: the flow of OpenFPGA, run from Python.
#
# This file is part of the module 'openfpga', see openfpga_python.i, which
# includes it after the classes generated for OpenfpgaShell. It needs nothing
# else than the shell: a Session sends the commands of the shell, and reads the
# answers of the commands (JSON, see openfpga_flow/schemas) into typed objects.
import dataclasses
import hashlib
import json
import os
import shlex
import shutil
import time
import typing
from typing import List, Optional, Union

# The answers -------------------------------------------------------------------------


@dataclasses.dataclass(frozen=True)
class AppOption:
    """An application option, see Options. value and default are of the type
    of the option: str, int, float or bool (None for a number that is not
    finite). choices lists the values of a selection, and is empty for the
    others. stage is the first stage that the option changes: the stages from
    there on are run again after it is set"""

    name: str
    type: str
    value: Union[str, int, float, bool, None]
    default: Union[str, int, float, bool, None]
    is_default: bool
    help: str
    choices: List[str]
    stage: str
    # The flag of the option on the command line of VPR, e.g. --device for
    # general.device_layout; None for an option of OpenFPGA
    vpr_flag: Optional[str] = None
    # Who the option belongs to: vpr, openfpga, or the plugin that added it
    owner: Optional[str] = None


@dataclasses.dataclass(frozen=True)
class InputFile:
    """Where an input was read from, as a whole path"""

    file: str


@dataclasses.dataclass(frozen=True)
class VprArchInput:
    file: str
    layout: str


@dataclasses.dataclass(frozen=True)
class SessionInputs:
    """The inputs of the flow. None for one that is not read"""

    vpr_arch: Optional[VprArchInput]
    openfpga_arch: Optional[InputFile]
    circuit: Optional[InputFile]
    # The files of the settings that the bitstream is made with, in the order they were read
    simulation_setting: Optional[List[str]] = None
    bitstream_setting: Optional[List[str]] = None


@dataclasses.dataclass(frozen=True)
class SessionResults:
    """The files of the results of the stages: the packing (.net), the placement
    (.place) and the routing (.route). None for a stage that is not reached. The
    paths are whole, from the directory of the process when they were asked"""

    packing: Optional[str]
    placement: Optional[str]
    routing: Optional[str]
    # The file of the fabric bitstream that build_bitstream wrote (bitstream())
    bitstream: Optional[str] = None


@dataclasses.dataclass(frozen=True)
class StageFailure:
    """A stage that failed and left what it had made, e.g., a route that found
    no legal routing: the data of the stages before it is still there"""

    stage: str
    message: str


@dataclasses.dataclass(frozen=True)
class Change:
    """What setting an option did: the name, its value before and after (as the
    shell reports them), and the stages that were reached and are not any more,
    in the order of the flow. dropped is empty when the value did not change"""

    option: str
    before: object
    after: object
    dropped: List[str]


@dataclasses.dataclass(frozen=True)
class StageOptions:
    """The options that tune a stage: total of the stage, matched by the filters,
    returned (at most the limit), truncated if more matched than were returned"""

    total: int
    matched: int
    returned: int
    truncated: bool
    items: List["AppOption"]


@dataclasses.dataclass(frozen=True)
class StageInfo:
    """A stage of the flow and what can be changed for it. requires is the stage
    before it; commands reach it; dropped_by_change are the stages that are
    dropped when one of its options changes (it, and the ones after it); failed
    is why it failed, if it did"""

    stage: str
    requires: Optional[str]
    commands: List[str]
    reached: bool
    failed: Optional[str]
    dropped_by_change: List[str]
    options: StageOptions


@dataclasses.dataclass(frozen=True)
class StageFilter:
    stage: Optional[str]
    changed: bool
    query: Optional[str]


@dataclasses.dataclass(frozen=True)
class StageReport:
    """Answer of report_stage: the stages, and for each what can be changed"""

    stage: str
    filter: StageFilter
    stages: List[StageInfo]


@dataclasses.dataclass(frozen=True)
class StageRun:
    """A stage that is reached. seconds is None if it was not run by a command
    of this session (its results were read from a file)"""

    stage: str
    seconds: Optional[float]


@dataclasses.dataclass(frozen=True)
class BlockCount:
    type: str
    count: int


@dataclasses.dataclass(frozen=True)
class PackingReport:
    clusters: int
    atoms: int
    nets: int
    blocks: List[BlockCount]


@dataclasses.dataclass(frozen=True)
class PlacementReport:
    """half_perimeter is the sum over the nets of the half perimeter of the box
    around their blocks. The others are from the log of the placement, and None
    if the stage was not run by a command of this session"""

    half_perimeter: int
    cost: Optional[float]
    bb_cost: Optional[float]
    td_cost: Optional[float]
    estimated_critical_path_delay_ns: Optional[float]
    estimated_fmax_mhz: Optional[float]
    num_temperatures: Optional[int]
    swap_attempts: Optional[int]
    # The placer (option place.placer): classic, or analytical, whose detailed placer the
    # figures above are of, and whose global placement estimates are in global_placement
    placer: str = "classic"
    global_placement: Optional["GlobalPlacementReport"] = None


@dataclasses.dataclass(frozen=True)
class GlobalPlacementReport:
    """The estimates of the global placement of the analytical placer, from its log"""

    estimated_wirelength: Optional[float]
    estimated_critical_path_delay_ns: Optional[float]


@dataclasses.dataclass(frozen=True)
class RoutingReport:
    """wirelength is the tiles that the wires span, as VPR adds it up (the channels only, also
    when the routing is flat). iterations and critical_path_delay_ns are from the log of the
    routing, and None if the stage was not run by a command of this session"""

    channel_width: int
    wirelength: int
    wires: int
    iterations: Optional[int]
    critical_path_delay_ns: Optional[float]
    # True when the routing is flat: wires and wirelength are those of the channels
    flat_routing: bool = False


@dataclasses.dataclass(frozen=True)
class RunReport:
    """Answer of report_run: what each stage that is reached did. packing,
    placement and routing are None until their stage is reached"""

    stage: str
    stages: List[StageRun]
    packing: Optional[PackingReport]
    placement: Optional[PlacementReport]
    routing: Optional[RoutingReport]
    failed: Optional[StageFailure]


@dataclasses.dataclass(frozen=True)
class SessionState:
    """Answer of report_session: what the flow has done. reached lists the stages
    that are valid (DEVICE, PACKED, PLACED, ROUTED, ANALYZED, ...); a stage that
    an option or an input changed is not in it. It is empty if the data of VPR
    that the session worked with was replaced"""

    reached: List[str]
    inputs: SessionInputs
    results: SessionResults
    failed: Optional[StageFailure] = None
    # The data that plugins keep with the architectures (attachments), by key, as JSON values
    attachments: Optional[dict] = None


@dataclasses.dataclass(frozen=True)
class PluginHook:
    """A hook of a plugin: the command, and when it runs ("before" or "after")"""

    command: str
    when: str


@dataclasses.dataclass(frozen=True)
class Plugin:
    """A plugin of the shell that the build has (see report_plugins): the commands it
    added, the commands of others it overrode or removed, its hooks on commands, and
    every_command when it has a hook after every command"""

    name: str
    version: str
    commands: List[str]
    overrides: List[str]
    removes: List[str]
    hooks: List[PluginHook]
    every_command: bool


@dataclasses.dataclass(frozen=True)
class _PluginList:
    plugins: List[Plugin]


def _plugin_label(plugins):
    """The plugins of a build as one line, for example "a 1.0, b 2", or "none" """
    return ", ".join("%s %s" % (p["name"], p["version"]) for p in plugins) or "none"


def _convert(hint, value):
    """The JSON value, as an instance of the type hint"""
    if value is None:
        return None
    origin = typing.get_origin(hint)
    if origin is Union:
        kinds = [kind for kind in typing.get_args(hint) if kind is not type(None)]
        return _convert(kinds[0], value) if len(kinds) == 1 else value
    if origin is list:
        (item,) = typing.get_args(hint)
        return [_convert(item, element) for element in value]
    if dataclasses.is_dataclass(hint):
        return _build(hint, value)
    return value


def _build(cls, data):
    """An instance of the dataclass cls from a dict. A key that the class does
    not know is skipped: an answer may gain a field without a new version"""
    hints = typing.get_type_hints(cls)
    values = {}
    for field in dataclasses.fields(cls):
        if field.name in data:
            values[field.name] = _convert(hints[field.name], data[field.name])
        elif field.default is dataclasses.MISSING:
            raise KeyError("%s has no '%s'" % (cls.__name__, field.name))
    return cls(**values)


# The version of the schema of each kind of answer that is read
_SCHEMA_VERSIONS = {
    "session": 1,
    "run_report": 1,
    "app_options": 1,
    "stage_info": 1,
    "plugins": 1,
    "synthesis": 1,
    "design_hierarchy": 1,
    "bitstream": 1,
}


def register_answer_kind(kind, version):
    """Read the answers of the kind with the version of their schema. A command
    that a plugin adds gives its answer a kind of its own: registered here, its
    answer is read by Session._query like the others"""
    if not isinstance(kind, str) or not kind:
        raise ValueError("The kind of an answer is a name, not %r" % (kind,))
    if isinstance(version, bool) or not isinstance(version, int) or version < 1:
        raise ValueError("The version of a schema is a whole number from 1, not %r" % (version,))
    _SCHEMA_VERSIONS[kind] = version


class AnswerError(RuntimeError):
    """The answer of a command is not one that this Session reads: another
    kind, or a version of the schema that it does not know"""


def _answer_data(result, kind):
    """The data of the answer in the result of a command, as JSON (a dict), checked to be of
    the kind and of the version of its schema that the module reads"""
    answer = result.data
    if answer is None or answer.get("kind") != kind:
        raise AnswerError("Expected an answer of the kind '%s'" % kind)
    if answer.get("schema_version") != _SCHEMA_VERSIONS[kind]:
        raise AnswerError("The answer '%s' has an unknown schema version" % kind)
    return answer["data"]


def _answer(result, kind, cls):
    """The answer in the result of a command, as an instance of cls"""
    answer = result.data
    if answer is None:
        raise AnswerError("The command gave no answer, expected '%s'" % kind)
    if answer.get("kind") != kind:
        raise AnswerError(
            "Expected an answer of the kind '%s', got '%s'" % (kind, answer.get("kind"))
        )
    if kind not in _SCHEMA_VERSIONS:
        raise AnswerError("This module does not read answers of the kind '%s'" % kind)
    version = answer.get("schema_version")
    if version != _SCHEMA_VERSIONS[kind]:
        raise AnswerError(
            "The answer '%s' has the schema version %s, this module reads %d"
            % (kind, version, _SCHEMA_VERSIONS[kind])
        )
    data = dict(answer["data"])
    if "stage" in {field.name for field in dataclasses.fields(cls)}:
        data["stage"] = answer["stage"]
    return _build(cls, data)


# The options ---------------------------------------------------------------------


class Options:
    """The application options of a Session (see set_app_option).

    options["place.seed"] is the value of an option, and
    options["place.seed"] = 3 sets it, with a value of the type of the option:
    a str, an int, a float, a bool or a path. get() gives the AppOption, with
    its default, help text, choices and stage. An option that does not exist,
    or a value that the option refuses, raises OpenfpgaError.
    """

    def __init__(self, shell, run=None):
        self._shell = shell
        # How a command is run, when it has to be seen by the Session (its journal)
        self._run = run

    def _report(self, **flags):
        return _answer_options(self._shell.report_app_option(json=True, **flags))

    def get(self, name):
        """The AppOption of an option"""
        (option,) = self._report(name=name)
        return option

    def list(self):
        """All the options, as AppOption objects, in the order of the shell"""
        return self._report()

    def changed(self):
        """The options that do not have their default value"""
        return self._report(changed=True)

    def set(self, name, value):
        """Set an option. A bool is written as true or false"""
        if isinstance(value, bool):
            value = "true" if value else "false"
        if self._run is not None:
            self._run("set_app_option", name=name, value=value)
        else:
            self._shell.set_app_option(name=name, value=value)

    def reset(self, name):
        """Give an option its default value"""
        if self._run is not None:
            self._run("reset_app_option", name=name)
        else:
            self._shell.reset_app_option(name=name)

    def __getitem__(self, name):
        return self.get(name).value

    def __setitem__(self, name, value):
        self.set(name, value)

    def __iter__(self):
        return (option.name for option in self.list())

    def __contains__(self, name):
        return name in set(self)

    def __len__(self):
        return len(self.list())


def _answer_options(result):
    answer = result.data
    if answer is None or answer.get("kind") != "app_options":
        raise AnswerError("Expected an answer of the kind 'app_options'")
    if answer.get("schema_version") != _SCHEMA_VERSIONS["app_options"]:
        raise AnswerError("The answer 'app_options' has an unknown schema version")
    return [_build(AppOption, option) for option in answer["data"]["options"]]


def _read_manifest(path, missing_ok=False):
    try:
        with open(path) as handle:
            manifest = json.load(handle)
    except FileNotFoundError:
        if missing_ok:
            return None
        raise CheckpointError("There is no checkpoint in '%s'" % os.path.dirname(path))
    except ValueError as error:
        raise CheckpointError("'%s' is not a checkpoint: %s" % (path, error))
    if not isinstance(manifest, dict) or manifest.get("format") != _CHECKPOINT_FORMAT:
        raise CheckpointError("'%s' is not a checkpoint of a format that is read" % path)
    return manifest


# The journal -----------------------------------------------------------------------

JOURNAL_PYTHON = "replay.py"
JOURNAL_SHELL = "replay.openfpga"
JOURNAL_SCRIPTS = "scripts"  # the scripts of Yosys that the commands of the journal ran
JOURNAL_ATTACHMENTS = "attachments"  # the files of the values of set_attachment


class ReplayError(RuntimeError):
    """A command of a journal failed when it was replayed. line is the command,
    number its place in the script, and the OpenfpgaError that it raised is the
    cause"""

    def __init__(self, line, number, error):
        super().__init__("Replay stopped at the command %d, '%s': %s" % (number, line, error))
        self.line = line
        self.number = number


class _Journal:
    """The commands of a Session that changed the flow, in the order they were run, written
    as a Python script (replay.py) and as a shell script (replay.openfpga) of a directory.
    A command that failed is written as a comment: it is not replayed. Both files are written
    again when a command ends, so that a run that is cut short leaves scripts that run. The
    shell script ends with exit, as the shell does not stop at the end of a script"""

    # The line of the scripts that says the plugins of the build that wrote them
    PLUGINS = "# Plugins of the build: "

    def __init__(self, directory, resume=False, plugins="none"):
        self.directory = os.fspath(directory)
        os.makedirs(self.directory, exist_ok=True)
        self._scripts = 0  # the scripts of Yosys that were copied here (see record)
        self._attachments = 0  # the values of set_attachment that were copied here
        if resume and self._resume():
            return
        where = (
            "# The commands of an OpenFPGA session, written by openfpga.Session(journal=...)\n"
            "# Written in %s: the files of the flow are written in the current directory\n"
            % os.getcwd()
        )
        where += self.PLUGINS + plugins + "\n"
        self._python = [where, "import openfpga\n\nsession = openfpga.Session()\n"]
        self._shell = [where]
        self._write()

    def _resume(self):
        """Go on with the files that are in the directory (a worker that started again, a session
        that was opened again): what was written stays, and what comes next is added. False when
        there is nothing to go on with"""
        python = os.path.join(self.directory, JOURNAL_PYTHON)
        shell = os.path.join(self.directory, JOURNAL_SHELL)
        if not (os.path.isfile(python) and os.path.isfile(shell)):
            return False
        with open(python) as handle:
            self._python = handle.readlines()
        with open(shell) as handle:
            self._shell = handle.readlines()
        if (
            self._shell and self._shell[-1] == "exit\n"
        ):  # the end is written again with each command
            self._shell.pop()
        folder = os.path.join(self.directory, JOURNAL_SCRIPTS)
        self._scripts = len(os.listdir(folder)) if os.path.isdir(folder) else 0
        folder = os.path.join(self.directory, JOURNAL_ATTACHMENTS)
        self._attachments = len(os.listdir(folder)) if os.path.isdir(folder) else 0
        return True

    def _write(self):
        for file, lines, end in (
            (JOURNAL_PYTHON, self._python, []),
            (JOURNAL_SHELL, self._shell, ["exit\n"]),
        ):
            path = os.path.join(self.directory, file)
            with open(path + ".part", "w") as handle:
                handle.write("".join(lines + end))
            os.replace(path + ".part", path)

    def _keep_attachment(self, source):
        folder = os.path.join(self.directory, JOURNAL_ATTACHMENTS)
        os.makedirs(folder, exist_ok=True)
        self._attachments += 1
        kept = os.path.join(folder, "%03d.json" % self._attachments)
        shutil.copyfile(source, kept)
        return os.path.abspath(kept)

    def _keep_script(self, source):
        folder = os.path.join(self.directory, JOURNAL_SCRIPTS)
        os.makedirs(folder, exist_ok=True)
        self._scripts += 1
        kept = os.path.join(folder, "%03d.ys" % self._scripts)
        shutil.copyfile(source, kept)
        return os.path.abspath(kept)

    @staticmethod
    def _plain(value):
        return os.fspath(value) if isinstance(value, os.PathLike) else value

    def record(self, name, args, options, error=None):
        """Write a command. The files are written as whole paths, so that a replay in another
        directory reads the same files"""
        args = [self._plain(arg) for arg in args]
        options = {
            key: (os.path.abspath(self._plain(value)) if key == "file" else self._plain(value))
            for key, value in options.items()
            if value is not None and value is not False
        }
        if name == "yosys" and error is None and os.path.isfile(options.get("file", "")):
            # A script of Yosys is a file that its maker writes again for the next run: the journal
            # keeps the one that was run, so that a replay runs the same synthesis
            options["file"] = self._keep_script(options["file"])
        if name == "set_attachment" and error is None and os.path.isfile(options.get("file", "")):
            # The value is in a file of the moment: the journal keeps it
            options["file"] = self._keep_attachment(options["file"])
        line = _command_line(name, args, options)
        call = ", ".join(
            [repr(name)]
            + [repr(arg) for arg in args]
            + ["%s=%r" % (key, value) for key, value in options.items()]
        )
        if error is None:
            python = "session.command(%s)\n" % call
            if name == "set_app_option" and not args and set(options) == {"name", "value"}:
                python = "session.options[%r] = %r\n" % (options["name"], options["value"])
            if name == "reset_app_option" and not args and set(options) == {"name"}:
                python = "session.options.reset(%r)\n" % options["name"]
            shell = line + "\n"
        else:
            errors = getattr(error, "errors", None) or []
            reason = " ".join((errors[0].message if errors else str(error)).split())
            python = shell = "# Failed, not replayed: %s (%s)\n" % (line, reason)
        self._python.append(python)
        self._shell.append(shell)
        self._write()


def _parse_line(line):
    """The name, the positional arguments and the options of a command line, as
    Session.command takes them: --key value, and --key alone for a switch"""
    # Not the rules of a POSIX shell: a backslash is part of a path, and only the double quotes group
    words = [
        word[1:-1] if len(word) > 1 and word[0] == word[-1] == '"' else word
        for word in shlex.split(line, posix=False)
    ]
    args, options = _split_words(words[1:])
    return words[0], args, options


def _split_words(words):
    """The positional arguments and the options of the words of a command line
    (without its name), as Session.command takes them: --key value, and --key
    alone for a switch"""
    args, options = [], {}
    key = None
    for word in words:
        if word.startswith("--") and len(word) > 2:
            key = word[2:]
            options[key] = True
        elif key is not None and options[key] is True:
            options[key] = word
        else:
            args.append(word)
    return args, options


def _journal_script(path):
    """The shell script of a journal: a directory, or one of its two files"""
    path = os.fspath(path)
    if os.path.isdir(path):
        return os.path.join(path, JOURNAL_SHELL)
    if path.endswith(".py"):
        return os.path.join(os.path.dirname(path), JOURNAL_SHELL)
    return path


# The Session -------------------------------------------------------------------------


@dataclasses.dataclass(frozen=True)
class StepResult:
    """A command that succeeded: its name, everything it printed, and its time
    in seconds. A command that fails raises OpenfpgaError instead"""

    command: str
    log: str
    elapsed_s: float


class CheckpointError(RuntimeError):
    """A checkpoint cannot be written, or cannot be restored from"""


_CHECKPOINT_FORMAT = 1
_CHECKPOINT_MANIFEST = "checkpoint.json"
# The options that the commands which read the inputs set: restoring reads the inputs
_INPUT_OPTIONS = ("filename.arch_file", "filename.circuit_file", "general.device_layout")
# What a checkpoint keeps of the stages: the results as files, and the command that reads them
_CHECKPOINT_RESULTS = (
    ("packing", "PACKED", "read_packing"),
    ("placement", "PLACED", "read_placement"),
    ("routing", "ROUTED", "read_routing"),
)
_CHECKPOINT_STAGES = ("DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED")


def _sha256(path):
    digest = hashlib.sha256()
    try:
        with open(path, "rb") as handle:
            for block in iter(lambda: handle.read(1 << 20), b""):
                digest.update(block)
    except OSError as error:
        raise CheckpointError("Cannot read '%s': %s" % (path, error.strerror or error))
    return digest.hexdigest()


class Session:
    """The flow of OpenFPGA, run from Python, one step after the other.

    A Session wraps an OpenfpgaShell (the attribute shell, which stays
    available for any command that has no method here). Each method sends a
    command and raises OpenfpgaError if it does not succeed, with the log and
    the message of the failure; otherwise it returns a StepResult, or, for the
    commands that answer, an object of the answer.

        session = openfpga.Session()
        session.options["router.route_chan_width"] = 100
        session.read_vpr_arch("arch.xml", layout="2x2")
        session.read_openfpga_arch("openfpga_arch.xml")
        session.read_circuit("design.blif")
        for stage in (session.pack, session.place, session.route):
            stage()

    checkpoint() saves what the flow has done in a directory, and
    Session.restore() makes a session that is at that point, in this process or
    another. With checkpoint_dir, a checkpoint is made after each stage.

    With journal=directory, the commands that change the flow (the inputs, the
    options, the stages) are written as they are run, in replay.py and in
    replay.openfpga, so that the flow can be run again by a person, or with
    Session.replay(). A command that only answers (READ_ONLY_COMMANDS), and a
    command that is run with session.shell, are not written. With
    resume_journal=True the files of the directory are continued, not written
    again. The script of a yosys command is copied into the directory
    (scripts/), so that a replay runs the script that was run.

    The files of the flow are written in the current directory of the process.
    A stage fails if a stage that it needs is not valid, which is the case
    after an option or an input that changed it, with the command to run first.
    """

    # The commands that only answer: they do not change the flow, so the journal
    # does not have them. A subclass that adds such commands extends the set:
    # READ_ONLY_COMMANDS = Session.READ_ONLY_COMMANDS | {...}
    READ_ONLY_COMMANDS = frozenset(
        (
            "help",
            "version",
            "report_session",
            "report_run",
            "report_stage",
            "report_app_option",
            "report_plugins",
            "report_design_hierarchy",
            "report_bitstream",
        )
    )

    # The methods (names, "a.b" for a method of an attribute) that a worker
    # (RemoteSession) may call on top of the public methods of Session: a subclass
    # that adds methods lists them, REMOTE_METHODS = frozenset({...})
    REMOTE_METHODS = frozenset()

    def __init__(self, shell=None, checkpoint_dir=None, journal=None, resume_journal=False):
        self.shell = shell if shell is not None else OpenfpgaShell()
        self.options = Options(self.shell, run=self.command)
        self.checkpoint_dir = checkpoint_dir
        # resume_journal: go on with the files of the directory instead of writing them again
        self._journal = (
            _Journal(journal, resume_journal, _plugin_label(self._plugin_list()))
            if journal is not None
            else None
        )
        self._tcl_host = None  # made by the first tcl()

    # The commands ----------------------------------------------------------------

    def command(self, name, /, *args, **options):
        """Run any command of the shell, as in OpenfpgaShell.run: the options
        are written as --key value, and key=True as the switch --key"""
        started = time.monotonic()
        line, result = self._run_command(name, args, options)
        return StepResult(line, result.log, time.monotonic() - started)

    def _run_command(self, name, args, options):
        """Run a command and write it in the journal: its line and its CommandResult"""
        line = _command_line(name, args, options)
        try:
            result = self.shell.run(line)
        except OpenfpgaError as error:
            self._write_journal(name, args, options, error)
            raise
        self._write_journal(name, args, options)
        return line, result

    def shell_commands(self):
        """The names of the commands of the shell, in the order of its help. The help says
        where a command of a plugin comes from after its name, and lists the commands that
        plugins removed at its end, which are not commands any more"""
        names = []
        for line in self.shell.run("help").log.splitlines():
            if line.startswith("Removed by plugins:"):
                break
            if line.startswith("\t") and line.strip():
                names.append(line.split()[0])
        return names

    def plugins(self):
        """The plugins of the shell that the build has, in the order they registered: a
        list of Plugin"""
        return _answer(self.shell.run("report_plugins"), "plugins", _PluginList).plugins

    def _plugin_list(self):
        """The plugins of the build as [{name, version}], as checkpoints and journals keep
        them"""
        return [{"name": p.name, "version": p.version} for p in self.plugins()]

    def _write_journal(self, name, args, options, error=None):
        if self._journal is not None and name not in type(self).READ_ONLY_COMMANDS:
            self._journal.record(name, args, options, error)

    def fixed_width_refusal(self, command, flat_routing=True):
        """Why a flow at a fixed channel width does not run this command now, or None when it
        does. Such a flow (e.g. one that a program or a model drives) runs the stages one by one
        and routes with the width it is given: vpr, which runs the whole flow in one command, is
        not run, and route is not run while router.route_chan_width is not set, as VPR would
        then search for the smallest width that routes.

        flat_routing=False says that the device does not support flat routing: route is not run
        while router.flat_routing is set"""
        if command == "vpr":
            return (
                "vpr runs the whole flow in one command, which this flow does not do: run the "
                "stages pack, place, route and analysis"
            )
        routes = command == "route" or (
            command in ("analysis", "bitstream") and "ROUTED" not in self.state().reached
        )
        if routes and self.options["router.route_chan_width"] <= 0:
            return (
                "The channel width is not set, and the routing is only run at a fixed width: "
                "set router.route_chan_width"
            )
        if routes and not flat_routing and self.options["router.flat_routing"]:
            return (
                "Flat routing is set (router.flat_routing), and the device of the session does "
                "not support it: set router.flat_routing to false"
            )
        return None

    def tcl(self, script, safe=False, fixed_width=False, flat_routing=True):
        """Run a Tcl script, and return a TclResult: its result, what it
        printed and the command lines of the shell that it ran.

        The commands of the shell are Tcl commands, written as on the command
        line of the shell (read_vpr_arch --file arch.xml --layout 2x2); they
        run as command() runs them, so the journal has those that change the
        flow. A command that answers returns its answer as a Tcl dict (dict get
        [report_run] routing ...), and a command that fails is an error of Tcl,
        which catch takes. A command whose name is one of Tcl keeps it, and the
        one of the shell is in the namespace openfpga (openfpga::source); exit
        and cd are not available. The variables and procedures stay for the
        next script.

        With safe=True the script runs in a safe interpreter, kept apart from
        the other one: no file, no program, and no command of the shell that
        runs one (ext_exec, source, yosys); a path in the arguments of a
        command must be in the current directory. A script that fails raises
        TclError, or TclRefused for what a safe interpreter does not do. Tcl
        comes with tkinter: a Python without it raises TclError.

        With fixed_width=True a command that fixed_width_refusal() refuses (vpr,
        and route while the channel width is not set) raises TclRefused.
        flat_routing=False is passed to it: the device does not support flat
        routing.
        """
        if self._tcl_host is None:
            self._tcl_host = _TclHost(self)
        return self._tcl_host.run(script, safe, fixed_width, flat_routing)

    # The inputs ------------------------------------------------------------------

    def read_vpr_arch(self, file, layout):
        """Read the architecture of VPR. layout names a fixed device layout, as
        auto is not accepted"""
        return self.command("read_vpr_arch", file=file, layout=layout)

    def read_openfpga_arch(self, file):
        """Read the architecture of OpenFPGA"""
        return self.command("read_openfpga_arch", file=file)

    def read_circuit(self, file):
        """Read the circuit (a BLIF netlist)"""
        return self.command("read_circuit", file=file)

    # Synthesis --------------------------------------------------------------------

    def synthesize(
        self,
        top,
        files,
        lut_size=None,
        include_dirs=None,
        defines=None,
        read_options=None,
        output_dir=None,
        read=True,
        run=True,
    ):
        """Synthesize Verilog files (paths) into a netlist of the LUTs of the device, with
        Yosys (the shell's command synthesize): the script is made from a generic one with
        lut_size, or from the synthesis target that a plugin gives for the session. The script
        and the netlist go to output_dir (synth). The netlist is read as the circuit when the
        architectures are read, unless read is False. defines is a dict of NAME: value,
        read_options a list (-nolatches).

        With run=False the script is written and the plan is returned, and nothing else
        happens (not written in the journal). Returns what the command answers (kind
        synthesis): top, netlist, script, target, lut_size, files, synthesized, circuit_read.
        Arguments that are refused, and a failure of Yosys, raise OpenfpgaError ('Synthesis
        refused: ...' for the first)"""
        options = {
            "top": top,
            "files": ",".join(os.path.abspath(os.fspath(f)) for f in files),
            "lut_size": lut_size,
            "include_dirs": ",".join(os.path.abspath(os.fspath(d)) for d in include_dirs or []),
            "defines": ",".join("%s=%s" % item for item in (defines or {}).items()),
            "read_options": ",".join(read_options or []),
            "output_dir": None if output_dir is None else os.path.abspath(os.fspath(output_dir)),
        }
        return self._synthesis("synthesize", options, read, run, check=False)

    def read_design(
        self,
        directory,
        netlist=None,
        output_dir=None,
        read=True,
        run=True,
        check=False,
        lut_size=None,
    ):
        """Read a design directory with the shell's command read_design: its constraints file is
        read (with the command that a plugin names), its RTL is synthesized as synthesize() does
        (or its netlist is read, the one of the label netlist, or the only one of a design with
        no RTL), and its SDC is given to VPR. With check=True the design is checked and what
        would be synthesized is returned, without writing anything; with run=False the script
        is written and the plan returned. Neither is written in the journal. lut_size is for a
        session without a synthesis target. Returns what the command answers (kind synthesis),
        with design and design_folder"""
        options = {
            "file": os.path.abspath(os.fspath(directory)),
            "netlist": netlist,
            "lut_size": lut_size,
            "output_dir": None if output_dir is None else os.path.abspath(os.fspath(output_dir)),
        }
        return self._synthesis("read_design", options, read, run, check)

    def design_hierarchy(
        self,
        directory=None,
        top=None,
        files=None,
        include_dirs=None,
        defines=None,
        read_options=None,
        output_dir=None,
        limit=None,
    ):
        """The hierarchy of a design, with the shell's command report_design_hierarchy: its RTL
        is elaborated from its top by a Yosys script of its own (as the synthesis reads it, with
        the cell libraries of the synthesis target as blackboxes), and nothing of the flow
        changes (not written in the journal). Give directory (a design directory), or top and
        files as synthesize() takes them. The script and the JSON of Yosys go to output_dir
        (hierarchy).

        Returns a dict: design, top, files, script, json; modules (name, module, parameters,
        kind: module, library or unknown, instances in the whole tree, cells of Yosys, src:
        file and line), most instances first; tree (instance, name, module, kind, parameters,
        src, children, and more: the children that limit left out, breadth first); nodes,
        instances, truncated. A refusal or a failure of Yosys raises OpenfpgaError"""
        options = {
            "file": None if directory is None else os.path.abspath(os.fspath(directory)),
            "top": top,
            "files": ",".join(os.path.abspath(os.fspath(f)) for f in files or []),
            "include_dirs": ",".join(os.path.abspath(os.fspath(d)) for d in include_dirs or []),
            "defines": ",".join("%s=%s" % item for item in (defines or {}).items()),
            "read_options": ",".join(read_options or []),
            "output_dir": None if output_dir is None else os.path.abspath(os.fspath(output_dir)),
            "limit": None if limit is None else int(limit),
        }
        options = {k: v for k, v in options.items() if v not in (None, "")}
        result = self.shell.run(_command_line("report_design_hierarchy", (), options))
        return _answer_data(result, "design_hierarchy")

    def _synthesis(self, name, options, read, run, check):
        options = {k: v for k, v in options.items() if v not in (None, "")}
        if not read:
            options["no_read"] = True
        if check:
            options["check"] = True
        elif not run:
            options["no_run"] = True
        if run and not check:
            _, result = self._run_command(name, (), options)
        else:  # a plan or a check changes nothing of the flow: not in the journal
            result = self.shell.run(_command_line(name, (), options))
        return _answer_data(result, "synthesis")

    # Attachments ------------------------------------------------------------------

    def set_attachment(self, key, value):
        """Keep a value (anything that is JSON) with the architectures of the session, under
        a key that starts with the name of a plugin and a dot (see report_session). None drops
        it. Reading an architecture drops them all"""
        if value is None:
            return self.command("set_attachment", key=key)
        # The value goes in a file: JSON has double quotes, which a command line does not
        # take. The journal keeps a copy of it
        import tempfile

        handle, path = tempfile.mkstemp(prefix="openfpga_attachment_", suffix=".json")
        try:
            with os.fdopen(handle, "w") as out:
                json.dump(value, out)
            return self.command("set_attachment", key=key, file=path)
        finally:
            os.remove(path)

    def attachments(self):
        """The attachments of the session, by key (see set_attachment)"""
        return dict(self.state().attachments or {})

    # The stages ------------------------------------------------------------------

    def pack(self):
        return self._stage("pack")

    def place(self):
        return self._stage("place")

    def route(self):
        return self._stage("route")

    def analysis(self):
        return self._stage("analysis")

    def _stage(self, name, **options):
        step = self.command(name, **options)
        if self.checkpoint_dir is not None:
            self.checkpoint(self.checkpoint_dir)
        return step

    def run_flow(self, bitstream=False):
        """Run pack, place, route and analysis, and return their results. With bitstream, the
        bitstream is made after them (bitstream()), and its answer is the last item"""
        steps = [stage() for stage in (self.pack, self.place, self.route, self.analysis)]
        if bitstream:
            steps.append(self.bitstream())
        return steps

    # The bitstream ------------------------------------------------------------------

    def read_simulation_setting(self, file):
        """Read the simulation settings of OpenFPGA. The link of the architecture is made
        again by the next bitstream(); nothing else is dropped"""
        return self.command("read_openfpga_simulation_setting", file=os.path.abspath(file))

    def read_bitstream_setting(self, file, append=False):
        """Read the bitstream settings of OpenFPGA (with append, after the ones read before).
        They change the bitstream: REPACKED and BITSTREAM are dropped"""
        options = {"file": os.path.abspath(file)}
        if append:
            options["append"] = True
        return self.command("read_openfpga_bitstream_setting", **options)

    def repack(self):
        """Repack the routed design (REPACKED), as the first step of a bitstream does. The
        architecture must have been linked and the fabric built (bitstream() does both)"""
        return self.command("repack")

    def bitstream(self, file=None, format="plain_text", arch_file=None):
        """Make the bitstream of the routed design with the shell's build_bitstream: the link
        and the fabric (kept when still valid), repack, the architecture and fabric
        bitstreams, and the file (default bitstream/<circuit>.bit), with the recipe of the
        bitstream of the device (the defaults, or the one a plugin set). Returns what
        report_bitstream answers (kind bitstream): the file, its SHA-256, the bits, the
        protocol, the bits per type of block, the settings and the steps"""
        options = {"format": format}
        if file is not None:
            options["file"] = os.path.abspath(os.fspath(file))
        if arch_file is not None:
            options["arch_file"] = os.path.abspath(os.fspath(arch_file))
        _, result = self._run_command("build_bitstream", (), options)
        return _answer_data(result, "bitstream")

    def bitstream_report(self, limit=None):
        """What report_bitstream answers about the bitstream of the session (BITSTREAM)"""
        options = {} if limit is None else {"limit": int(limit)}
        result = self.shell.run(_command_line("report_bitstream", (), options))
        return _answer_data(result, "bitstream")

    def read_packing(self, file):
        """Take the packing (.net) of a file, instead of running pack"""
        return self._stage("read_packing", file=file)

    def read_placement(self, file):
        """Take the placement (.place) of a file, instead of running place"""
        return self._stage("read_placement", file=file)

    def read_routing(self, file):
        """Take the routing (.route) of a file, instead of running route"""
        return self._stage("read_routing", file=file)

    # The inputs that change a flow -------------------------------------------------

    def set_option(self, name, value):
        """Set an application option, as options[name] = value does, and tell
        what it changed: a Change with the stages that it dropped. The next
        command of a stage that was dropped runs with the new value"""
        return self._change(name, lambda: self.options.set(name, value))

    def _change(self, name, action):
        reached = self.state().reached
        before = self.options.get(name).value
        action()
        after = self.options.get(name).value
        now = self.state().reached
        return Change(name, before, after, [stage for stage in reached if stage not in now])

    def _file_option(self, name, file):
        """Set an option that names a file, as a whole path. A file that does not
        exist is refused before anything changes; None gives the option its
        default back, i.e., no file"""
        if file is None:
            return self._reset_option(name)
        path = os.path.abspath(os.fspath(file))
        if not os.path.isfile(path):
            raise FileNotFoundError("The file '%s' does not exist" % path)
        return self.set_option(name, path)

    def _reset_option(self, name):
        return self._change(name, lambda: self.options.reset(name))

    def read_vpr_constraints(self, file):
        """Take the floorplan constraints of VPR (XML: the atoms of a partition
        and the regions where they must be packed and placed) from a file, for
        the stages that follow. They are read when the device is made, so the
        whole flow runs again. None takes the constraints away"""
        return self._file_option("filename.read_vpr_constraints_file", file)

    def read_sdc(self, file):
        """Take the timing constraints of VPR (SDC: the clocks and their periods,
        the input and output delays) from a file. They are read when the device
        is made, so the whole flow runs again. None takes them away"""
        return self._file_option("filename.sdc_file", file)

    def set_route_chan_width(self, width):
        """Route with a channel of this many tracks, instead of looking for the
        smallest. The routing graph is made with it, so the whole flow runs
        again"""
        return self.set_option("router.route_chan_width", int(width))

    def clear_constraints(self):
        """Take away the constraints of VPR and the timing constraints. Returns
        the Changes, one for each that was set"""
        return [
            change
            for change in (
                self.read_vpr_constraints(None),
                self.read_sdc(None),
            )
            if change.before != change.after
        ]

    # The journal and the replay --------------------------------------------------

    @classmethod
    def replay(cls, path, shell=None, **session_options):
        """Run the commands of a journal again, in a new Session, and return it.

        path is the directory of the journal, or one of its files: the commands
        are read from the shell script (replay.openfpga), a command per line,
        and may be a script that a person wrote. The commands run in the
        current directory, with the options and inputs they name. Session
        options, e.g., journal=, are those of the new Session. A command that
        fails raises ReplayError, and so does a journal written by a build
        with other plugins (the line '# Plugins of the build: ...').
        """
        script = _journal_script(path)
        with open(script) as handle:
            lines = handle.read().splitlines()
        session = cls(shell, **session_options)
        built = _plugin_label(session._plugin_list())
        for number, line in enumerate(lines, 1):
            line = line.strip()
            if line.startswith(_Journal.PLUGINS) and line[len(_Journal.PLUGINS) :] != built:
                # A command may not do the same with other plugins: the journal is not replayed
                raise ReplayError(
                    line,
                    number,
                    RuntimeError(
                        "the journal was written with the plugins %s, and this build has %s"
                        % (line[len(_Journal.PLUGINS) :], built)
                    ),
                )
            if not line or line.startswith("#"):
                continue
            name, args, options = _parse_line(line)
            if name == "exit":
                break
            try:
                session.command(name, *args, **options)
            except OpenfpgaError as error:
                raise ReplayError(line, number, error) from error
        return session

    # The state and the checkpoints -----------------------------------------------

    def state(self):
        """What the flow has done: the stages that are reached, and the files
        of the inputs and of the results"""
        return _answer(self.shell.run("report_session"), "session", SessionState)

    def checkpoint(self, directory):
        """Save the flow in a directory, and return the directory.

        The directory gets the files of the packing, the placement and the
        routing of the stages that are reached, and checkpoint.json: the
        stages, the options that are not the default, and where the inputs
        come from, with the checksum of every file, the simulation and
        bitstream settings, the file of the bitstream, the plugins of the build,
        the attachments of the session (see set_attachment) and what a
        subclass adds (_checkpoint_extra(), under "extra"). The inputs are not
        copied: restore() needs them where they are. The stages after ANALYZED
        (repack, bitstream) are not saved. A checkpoint that was in the
        directory is replaced.
        """
        state = self.state()
        # The device is made from the architectures alone: a flow without a circuit has
        # nothing of its own to save
        if "DEVICE" not in state.reached or state.inputs.circuit is None:
            raise CheckpointError("Nothing to save: the flow has not read all its inputs")
        directory = os.fspath(directory)
        os.makedirs(directory, exist_ok=True)
        inputs = {}
        for key in ("vpr_arch", "openfpga_arch", "circuit"):
            entry = getattr(state.inputs, key)
            inputs[key] = dict(dataclasses.asdict(entry), sha256=_sha256(entry.file))
        results = {}
        for key in [item[0] for item in _CHECKPOINT_RESULTS] + ["bitstream"]:
            source = getattr(state.results, key)
            if source is None:
                continue
            name = os.path.basename(source)
            target = os.path.join(directory, name)
            shutil.copyfile(source, target + ".part")
            os.replace(target + ".part", target)
            results[key] = {"file": name, "sha256": _sha256(target)}
        manifest = {
            "format": _CHECKPOINT_FORMAT,
            "stages": [stage for stage in state.reached if stage in _CHECKPOINT_STAGES],
            "inputs": inputs,
            "options": [
                {"name": option.name, "value": option.value}
                for option in self.options.changed()
                if option.name not in _INPUT_OPTIONS and option.value is not None
            ],
            "results": results,
            "plugins": self._plugin_list(),
        }
        settings = {
            kind: [{"file": file, "sha256": _sha256(file)} for file in files]
            for kind, files in (
                ("simulation_setting", state.inputs.simulation_setting),
                ("bitstream_setting", state.inputs.bitstream_setting),
            )
            if files
        }
        if settings:
            manifest["settings"] = settings
        if state.attachments:
            manifest["attachments"] = state.attachments
        extra = self._checkpoint_extra(directory)
        if extra:
            manifest["extra"] = extra
        path = os.path.join(directory, _CHECKPOINT_MANIFEST)
        previous = _read_manifest(path, missing_ok=True)
        with open(path + ".part", "w") as handle:
            json.dump(manifest, handle, indent=2)
            handle.write("\n")
        # The manifest is the last to change: a checkpoint that was cut short is the old one
        os.replace(path + ".part", path)
        for key, entry in (previous or {}).get("results", {}).items():
            if key not in results and os.path.basename(entry["file"]) == entry["file"]:
                try:
                    os.remove(os.path.join(directory, entry["file"]))
                except FileNotFoundError:
                    pass
        return directory

    @classmethod
    def restore(cls, directory, shell=None, check_inputs=True, **session_options):
        """Make a Session that is at the point of a checkpoint.

        It sets the options, reads the inputs, and reads the results of the
        stages that were reached instead of running them, so that the flow
        goes on from there, in this process or in another one, and gives the
        same results. ANALYZED is run again. A file of the results that does
        not match its checksum raises CheckpointError, and so does an input
        that changed, unless check_inputs is False, and a checkpoint made by a
        build with other plugins, as their commands may not do the same. The
        other arguments are those of the Session, e.g., journal=.

        In order, on the new session (an instance of cls): the options are
        set; _restore_inputs() reads the architectures; the settings of the
        bitstream are read again (the bitstream is not made again: its file
        is in the checkpoint, results.bitstream); _restore_extra() gets
        what _checkpoint_extra() saved; the attachments that are not there
        already are set again; then the circuit is read, and the results of
        the stages. A subclass overrides these methods to save and read what
        it adds to a flow.
        """
        directory = os.fspath(directory)
        manifest = _read_manifest(os.path.join(directory, _CHECKPOINT_MANIFEST))
        for key, entry in manifest["results"].items():
            if _sha256(os.path.join(directory, entry["file"])) != entry["sha256"]:
                raise CheckpointError(
                    "The file '%s' of the %s is not the one of the checkpoint"
                    % (entry["file"], key)
                )
        if check_inputs:
            for key, entry in manifest["inputs"].items():
                if _sha256(entry["file"]) != entry["sha256"]:
                    raise CheckpointError(
                        "The input '%s' (%s) is not the one of the checkpoint"
                        % (key, entry["file"])
                    )
            for kind, entries in manifest.get("settings", {}).items():
                for entry in entries:
                    if _sha256(entry["file"]) != entry["sha256"]:
                        raise CheckpointError(
                            "The %s '%s' is not the one of the checkpoint" % (kind, entry["file"])
                        )
        session = cls(shell, **session_options)
        # A checkpoint made before the plugins were recorded (no "plugins") is taken
        saved, built = manifest.get("plugins"), session._plugin_list()
        if saved is not None and saved != built:
            raise CheckpointError(
                "The checkpoint was made with the plugins %s, and this build has %s"
                % (_plugin_label(saved), _plugin_label(built))
            )
        for option in manifest["options"]:
            session.options.set(option["name"], option["value"])
        session._restore_inputs(manifest, directory)
        # The settings of the bitstream, when they are not the ones that were read already
        now = session.state().inputs
        for kind, read in (
            ("simulation_setting", session.read_simulation_setting),
            ("bitstream_setting", session.read_bitstream_setting),
        ):
            files = [entry["file"] for entry in manifest.get("settings", {}).get(kind, [])]
            if files and files != (getattr(now, kind) or []):
                read(files[0])
                for file in files[1:]:
                    if kind == "bitstream_setting":
                        read(file, append=True)
                    else:
                        read(file)
        session._restore_extra(manifest.get("extra", {}), directory)
        attachments = manifest.get("attachments", {})
        if attachments:
            now = session.attachments()
            for key, value in attachments.items():
                if now.get(key, None) != value:
                    session.set_attachment(key, value)
        session.read_circuit(manifest["inputs"]["circuit"]["file"])
        for key, _, command in _CHECKPOINT_RESULTS:
            if key in manifest["results"]:
                path = os.path.join(directory, manifest["results"][key]["file"])
                getattr(session, command)(path)
        if "ANALYZED" in manifest["stages"]:
            session.analysis()
        return session

    # What a subclass adds to a checkpoint ----------------------------------------

    def _checkpoint_extra(self, directory):
        """What a subclass saves in a checkpoint besides the flow of OpenFPGA: a dict that
        json writes, kept in checkpoint.json under "extra" (when it is not empty), and given
        back to _restore_extra() by restore(). Files of its own go in directory. A subclass
        that overrides it keeps the keys of the classes it derives from:
        dict(super()._checkpoint_extra(directory), mykey=...). Nothing by default"""
        return {}

    def _restore_inputs(self, manifest, directory):
        """Read the architectures of a checkpoint, after its options are set: read_vpr_arch and
        read_openfpga_arch with the files of manifest["inputs"]. A subclass that reads them
        another way (e.g. from a package of files that sets options of its own) overrides it,
        and sets the options of manifest["options"] again after, so that those of the
        checkpoint win. The circuit is read later, after _restore_extra()"""
        inputs = manifest["inputs"]
        self.read_vpr_arch(inputs["vpr_arch"]["file"], layout=inputs["vpr_arch"]["layout"])
        self.read_openfpga_arch(inputs["openfpga_arch"]["file"])

    def _restore_extra(self, extra, directory):
        """Read again what _checkpoint_extra() saved (extra; {} for a checkpoint without
        it), after the architectures are read and before the circuit and the results of the
        stages: e.g. constraints that the circuit is checked against. Nothing by default"""

    # The answers ---------------------------------------------------------------

    def _query(self, command, answer_kind, cls, **options):
        """Run a command that answers in JSON, and give its answer as an
        instance of the dataclass cls. answer_kind is the kind of the answer,
        see register_answer_kind"""
        return _answer(self.shell.run(_command_line(command, (), options)), answer_kind, cls)

    def run_report(self):
        """What each stage that is reached did: the packing, the cost of the
        placement, the wirelength and iterations of the routing, and the time
        of each stage"""
        return self._query("report_run", "run_report", RunReport)

    def stage_info(self, stage=None, changed=False, query=None, limit=None):
        """What can be changed for a stage (all the stages if not set; a name
        like PLACED or place), as a StageReport: its options with their values
        and defaults, the commands that reach it and the stages that a change
        drops. changed keeps the options that are not at their default, query
        the ones whose name matches a pattern such as 'place.*'"""
        return self._query(
            "report_stage",
            "stage_info",
            StageReport,
            stage=stage,
            changed=changed,
            query=query,
            limit=limit,
        )
