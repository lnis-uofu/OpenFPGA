# The Session of the Python module: the flow of OpenFPGA, run from Python.
#
# This file is part of the module 'openfpga', see openfpga_python.i, which
# includes it after the classes generated for OpenfpgaShell. It needs nothing
# else than the shell: a Session sends the commands of the shell, and reads the
# answers of the queries (JSON, see openfpga_flow/schemas) into typed objects.
import dataclasses
import time
import typing
from typing import List, Optional, Union

# The answers -------------------------------------------------------------------------


@dataclasses.dataclass(frozen=True)
class Grid:
    """Size of the device grid, in tiles"""

    width: int
    height: int


@dataclasses.dataclass(frozen=True)
class ClockPair:
    launch_clock: str
    capture_clock: str


@dataclasses.dataclass(frozen=True)
class SetupTiming:
    # The slacks and delays are None when the circuit has no path
    worst_negative_slack_ns: Optional[float]
    total_negative_slack_ns: Optional[float]
    critical_path_delay_ns: Optional[float]
    fmax_mhz: Optional[float]
    critical_path: ClockPair


@dataclasses.dataclass(frozen=True)
class HoldTiming:
    worst_negative_slack_ns: Optional[float]
    total_negative_slack_ns: Optional[float]


@dataclasses.dataclass(frozen=True)
class ClockDomain:
    launch_clock: str
    capture_clock: str
    intra_domain: bool
    critical_path_delay_ns: Optional[float]
    worst_slack_ns: Optional[float]
    fmax_mhz: Optional[float]


@dataclasses.dataclass(frozen=True)
class TimingSummary:
    """Answer of timing_summary: the timing of the routed design"""

    stage: str
    setup: SetupTiming
    hold: HoldTiming
    clock_domains: List[ClockDomain]


@dataclasses.dataclass(frozen=True)
class PathPoint:
    pin: str
    type: str
    block: Optional[str]
    net: Optional[str]
    arrival_ns: Optional[float]
    delay_ns: Optional[float]


@dataclasses.dataclass(frozen=True)
class TimingPath:
    index: int
    launch_clock: str
    capture_clock: str
    startpoint: str
    endpoint: str
    slack_ns: Optional[float]
    points: List[PathPoint]
    data_arrival_ns: Optional[float]
    data_required_ns: Optional[float]


@dataclasses.dataclass(frozen=True)
class CriticalPaths:
    """Answer of critical_paths: the worst setup paths, the worst first"""

    stage: str
    requested: int
    returned: int
    paths: List[TimingPath]


@dataclasses.dataclass(frozen=True)
class BlockTypeUsage:
    name: str
    is_io: bool
    used: int
    available: int
    utilization: float


@dataclasses.dataclass(frozen=True)
class Utilization:
    """Answer of utilization: how much of the device the design uses"""

    stage: str
    device_utilization: float
    grid: Grid
    block_types: List[BlockTypeUsage]
    limiting_resources: List[str]


@dataclasses.dataclass(frozen=True)
class PlacedBlock:
    name: str
    type: str
    x: int
    y: int
    sub_tile: int
    layer: int


@dataclasses.dataclass(frozen=True)
class PlacementFilter:
    type: Optional[str]
    prefix: Optional[str]


@dataclasses.dataclass(frozen=True)
class Placement:
    """Answer of placement: where the blocks are. truncated is True when more
    blocks matched than the limit allows"""

    stage: str
    grid: Grid
    filter: PlacementFilter
    total: int
    returned: int
    truncated: bool
    blocks: List[PlacedBlock]


@dataclasses.dataclass(frozen=True)
class RouteNode:
    index: int
    parent: Optional[int]
    rr_node: int
    type: str
    xlow: int
    ylow: int
    xhigh: int
    yhigh: int
    ptc: int
    layer: int
    sink_pin: Optional[int]


@dataclasses.dataclass(frozen=True)
class NetRoute:
    """Answer of net_route: the routing tree of a net, from its source"""

    stage: str
    net: str
    num_sinks: int
    nodes: List[RouteNode]


@dataclasses.dataclass(frozen=True)
class ChannelUsage:
    used: int
    capacity: int
    occupancy: float


@dataclasses.dataclass(frozen=True)
class ChannelWidth:
    x: int
    y: int


@dataclasses.dataclass(frozen=True)
class CongestionSummary:
    chanx: ChannelUsage
    chany: ChannelUsage
    overused_nodes: int


@dataclasses.dataclass(frozen=True)
class CongestionRegion:
    x_low: int
    y_low: int
    x_high: int
    y_high: int
    chanx: ChannelUsage
    chany: ChannelUsage


@dataclasses.dataclass(frozen=True)
class Congestion:
    """Answer of congestion: the use of the routing channels, by region"""

    stage: str
    grid: Grid
    channel_width: ChannelWidth
    region_size: int
    summary: CongestionSummary
    regions: List[CongestionRegion]


@dataclasses.dataclass(frozen=True)
class ClusterInfo:
    name: str
    type: str
    input_pin_utils: float
    output_pin_utils: float


@dataclasses.dataclass(frozen=True)
class ClusterTypeStats:
    type: str
    count: int
    average_input_pin_utils: float
    average_output_pin_utils: float


@dataclasses.dataclass(frozen=True)
class ClusterStats:
    num_clusters: int
    by_type: List[ClusterTypeStats]


@dataclasses.dataclass(frozen=True)
class ClusterReport:
    """Answer of report_cluster: the clusters of the packing. stats is set when
    it was asked for"""

    stage: str
    query: str
    total: int
    matched: int
    returned: int
    truncated: bool
    clusters: List[ClusterInfo]
    stats: Optional[ClusterStats] = None


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


# The version of the schema that each class reads
_SCHEMA_VERSIONS = {
    "app_options": 1,
    "timing_summary": 1,
    "critical_paths": 1,
    "utilization": 1,
    "placement": 1,
    "net_route": 1,
    "congestion": 1,
    "cluster_report": 1,
}


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


# The Session -------------------------------------------------------------------------


@dataclasses.dataclass(frozen=True)
class StepResult:
    """A command that succeeded: its name, everything it printed, and its time
    in seconds. A command that fails raises OpenfpgaError instead"""

    command: str
    log: str
    elapsed_s: float


class Session:
    """The flow of OpenFPGA, run from Python, one step after the other.

    A Session wraps an OpenfpgaShell (the attribute shell, which stays
    available for any command that has no method here). Each method sends a
    command and raises OpenfpgaError if it does not succeed, with the log and
    the message of the failure; otherwise it returns a StepResult, or, for the
    queries, an object of the answer.

        session = openfpga.Session()
        session.options["router.route_chan_width"] = 100
        session.read_vpr_arch("arch.xml", layout="2x2")
        session.read_openfpga_arch("openfpga_arch.xml")
        session.read_circuit("design.blif")
        for stage in (session.pack, session.place, session.route):
            stage()
        print(session.timing_summary().setup.fmax_mhz)

    The files of the flow are written in the current directory of the process.
    The queries read the stage that they need, and fail if it is not valid,
    which is the case after an option or an input that changed it, with the
    command to run first.
    """

    def __init__(self, shell=None):
        self.shell = shell if shell is not None else OpenfpgaShell()
        self.options = Options(self.shell)

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
        return self.command("pack")

    def place(self):
        return self.command("place")

    def route(self):
        return self.command("route")

    def analysis(self):
        return self.command("analysis")

    def run_flow(self):
        """Run pack, place, route and analysis, and return their results"""
        return [stage() for stage in (self.pack, self.place, self.route, self.analysis)]

    def read_packing(self, file):
        """Take the packing (.net) of a file, instead of running pack"""
        return self.command("read_packing", file=file)

    def read_placement(self, file):
        """Take the placement (.place) of a file, instead of running place"""
        return self.command("read_placement", file=file)

    def read_routing(self, file):
        """Take the routing (.route) of a file, instead of running route"""
        return self.command("read_routing", file=file)

    # The queries -----------------------------------------------------------------

    def _query(self, command, kind, cls, **options):
        return _answer(self.shell.run(_command_line(command, (), options)), kind, cls)

    def timing_summary(self):
        """The timing of the routed design: slacks, critical path delay and
        Fmax, and the same for each pair of clocks"""
        return self._query("timing_summary", "timing_summary", TimingSummary)

    def critical_paths(self, count=None):
        """The count worst setup paths (the default of the command if not
        set), with their points"""
        return self._query("critical_paths", "critical_paths", CriticalPaths, count=count)

    def utilization(self):
        """The use of the device, per type of block"""
        return self._query("utilization", "utilization", Utilization)

    def placement(self, block_type=None, prefix=None, limit=None):
        """The location of the blocks, of one type and/or whose name starts
        with prefix. At most limit blocks are returned"""
        return self._query(
            "placement", "placement", Placement, type=block_type, prefix=prefix, limit=limit
        )

    def net_route(self, net):
        """The routing tree of a net"""
        return self._query("net_route", "net_route", NetRoute, net=net)

    def congestion(self, region_size=None):
        """The use of the channels, by regions of region_size tiles"""
        return self._query("congestion", "congestion", Congestion, region_size=region_size)

    def cluster_report(self, query=None, stats=False, limit=None):
        """The clusters whose name matches the pattern query, with the use of
        their pins. stats adds the averages by type of cluster"""
        return self._query(
            "report_cluster", "cluster_report", ClusterReport, query=query, stats=stats, limit=limit
        )
