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


@dataclasses.dataclass(frozen=True)
class SessionResults:
    """The files of the results of the stages: the packing (.net), the placement
    (.place) and the routing (.route). None for a stage that is not reached. The
    paths are whole, from the directory of the process when they were asked"""

    packing: Optional[str]
    placement: Optional[str]
    routing: Optional[str]


@dataclasses.dataclass(frozen=True)
class StageFailure:
    """A stage that failed and left what it had made, e.g., a route that found
    no legal routing: the data of the stages before it is still there"""

    stage: str
    message: str


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
    "app_options": 1,
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

    def __init__(self, shell):
        self._shell = shell

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
        self._shell.set_app_option(name=name, value=value)

    def reset(self, name):
        """Give an option its default value"""
        self.set(name, self.get(name).default)

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

    The files of the flow are written in the current directory of the process.
    A stage fails if a stage that it needs is not valid, which is the case
    after an option or an input that changed it, with the command to run first.
    """

    def __init__(self, shell=None, checkpoint_dir=None):
        self.shell = shell if shell is not None else OpenfpgaShell()
        self.options = Options(self.shell)
        self.checkpoint_dir = checkpoint_dir

    # The commands ----------------------------------------------------------------

    def command(self, name, /, *args, **options):
        """Run any command of the shell, as in OpenfpgaShell.run: the options
        are written as --key value, and key=True as the switch --key"""
        started = time.monotonic()
        line = _command_line(name, args, options)
        result = self.shell.run(line)
        return StepResult(line, result.log, time.monotonic() - started)

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

    def run_flow(self):
        """Run pack, place, route and analysis, and return their results"""
        return [stage() for stage in (self.pack, self.place, self.route, self.analysis)]

    def read_packing(self, file):
        """Take the packing (.net) of a file, instead of running pack"""
        return self._stage("read_packing", file=file)

    def read_placement(self, file):
        """Take the placement (.place) of a file, instead of running place"""
        return self._stage("read_placement", file=file)

    def read_routing(self, file):
        """Take the routing (.route) of a file, instead of running route"""
        return self._stage("read_routing", file=file)

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
        come from, with the checksum of every file. The inputs are not copied:
        restore() needs them where they are. The stages after ANALYZED
        (repack, bitstream) are not saved. A checkpoint that was in the
        directory is replaced.
        """
        state = self.state()
        if "DEVICE" not in state.reached:
            raise CheckpointError("Nothing to save: the flow has not read all its inputs")
        directory = os.fspath(directory)
        os.makedirs(directory, exist_ok=True)
        inputs = {}
        for key in ("vpr_arch", "openfpga_arch", "circuit"):
            entry = getattr(state.inputs, key)
            inputs[key] = dict(dataclasses.asdict(entry), sha256=_sha256(entry.file))
        results = {}
        for key, _, _ in _CHECKPOINT_RESULTS:
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
        }
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
    def restore(cls, directory, shell=None, check_inputs=True):
        """Make a Session that is at the point of a checkpoint.

        It sets the options, reads the inputs, and reads the results of the
        stages that were reached instead of running them, so that the flow
        goes on from there, in this process or in another one, and gives the
        same results. ANALYZED is run again. A file of the results that does
        not match its checksum raises CheckpointError, and so does an input
        that changed, unless check_inputs is False.
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
        session = cls(shell)
        for option in manifest["options"]:
            session.options.set(option["name"], option["value"])
        inputs = manifest["inputs"]
        session.read_vpr_arch(inputs["vpr_arch"]["file"], layout=inputs["vpr_arch"]["layout"])
        session.read_openfpga_arch(inputs["openfpga_arch"]["file"])
        session.read_circuit(inputs["circuit"]["file"])
        for key, _, command in _CHECKPOINT_RESULTS:
            if key in manifest["results"]:
                path = os.path.join(directory, manifest["results"][key]["file"])
                getattr(session, command)(path)
        if "ANALYZED" in manifest["stages"]:
            session.analysis()
        return session

    # The answers ---------------------------------------------------------------

    def _query(self, command, answer_kind, cls, **options):
        """Run a command that answers in JSON, and give its answer as an
        instance of the dataclass cls. answer_kind is the kind of the answer,
        see register_answer_kind"""
        return _answer(self.shell.run(_command_line(command, (), options)), answer_kind, cls)
