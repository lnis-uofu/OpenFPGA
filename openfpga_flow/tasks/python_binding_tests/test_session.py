"""The Session class: the commands and the queries of the shell, through methods that raise on
failure and give typed objects."""

import dataclasses
import json
from pathlib import Path

import pytest

from test_equivalence import Golden, in_dir

CIRCUIT = "simple_gates/and2_latch/and2_latch.blif"


@pytest.fixture
def session(openfpga_module):
    return openfpga_module.Session()


@pytest.fixture
def circuit(flow_dir):
    path = flow_dir / "benchmarks" / "ubm" / CIRCUIT
    if not path.exists():
        pytest.skip(f"missing {path}")
    return path


def inputs(session, and2_files, circuit):
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = 20
    session.read_vpr_arch(and2_files["vpr_arch"], layout="2x2")
    session.read_openfpga_arch(and2_files["openfpga_arch"])
    session.read_circuit(circuit)


@pytest.fixture
def ready(session, and2_files, circuit, tmp_path, monkeypatch):
    """A session in its own directory, with the inputs read"""
    monkeypatch.chdir(tmp_path)
    inputs(session, and2_files, circuit)
    return session


@pytest.fixture
def routed(ready):
    ready.run_flow()
    return ready


def plain(answer):
    """The answer as the dict that the shell gave, without what the object adds"""
    data = dataclasses.asdict(answer)
    del data["stage"]
    if data.get("stats", 0) is None:
        del data["stats"]
    return data


# The flow -----------------------------------------------------------------------------


def test_a_session_wraps_a_shell(openfpga_module):
    shell = openfpga_module.OpenfpgaShell()
    assert openfpga_module.Session(shell).shell is shell
    assert isinstance(openfpga_module.Session().shell, openfpga_module.OpenfpgaShell)


def test_the_steps_give_their_log_and_time(ready):
    for step in (ready.pack(), ready.place(), ready.route(), ready.analysis()):
        assert isinstance(step.log, str) and step.log
        assert step.elapsed_s >= 0
    assert ready.pack.__name__ == "pack"


def test_the_flow_equals_the_vpr_flow(openfpga_module, tmp_path):
    """The stages of a Session give the files of the vpr command"""
    golden = Golden("k6_routing_test")
    (tmp_path / "session").mkdir()
    (tmp_path / "vpr").mkdir()

    def staged():
        session = openfpga_module.Session()
        session.options["general.clock_modeling"] = "route"
        session.options["router.route_chan_width"] = golden.chan_width
        session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
        session.read_openfpga_arch(golden.openfpga_arch)
        session.read_circuit(golden.circuit)
        session.run_flow()
        return {s: Path(f"{golden.name}.{s}").read_bytes() for s in ("net", "place", "route")}

    def monolithic():
        shell = openfpga_module.OpenfpgaShell()
        shell.run(golden.vpr_command())
        return {s: Path(f"{golden.name}.{s}").read_bytes() for s in ("net", "place", "route")}

    assert in_dir(tmp_path / "session", staged) == in_dir(tmp_path / "vpr", monolithic)


def test_a_generic_command(ready):
    step = ready.command("set_app_option", name="place.seed", value=7)
    assert step.command == "set_app_option --name place.seed --value 7"
    assert ready.options["place.seed"] == 7


def test_imports(routed, openfpga_module, and2_files, circuit, tmp_path):
    """The results of a run are read by another session instead of the stages"""
    made = Path.cwd()
    files = {suffix: next(made.glob(f"*.{suffix}")) for suffix in ("net", "place", "route")}
    (tmp_path / "other").mkdir()

    def imported():
        other = openfpga_module.Session()
        inputs(other, and2_files, circuit)
        other.read_packing(files["net"])
        other.read_placement(files["place"])
        other.read_routing(files["route"])
        return other.timing_summary(), other.placement()

    timing, placement = in_dir(tmp_path / "other", imported)
    assert timing == routed.timing_summary()
    assert placement == routed.placement()


# Failures raise -----------------------------------------------------------------------


def test_a_stage_before_its_inputs_raises(session, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        session.pack()
    assert error.value.command == "pack"
    assert "read_vpr_arch" in str(error.value)


def test_a_missing_file_raises(session, openfpga_module, tmp_path):
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.read_openfpga_arch(tmp_path / "missing.xml")


def test_a_query_before_its_stage_raises(ready, openfpga_module):
    ready.pack()
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        ready.timing_summary()
    assert "route" in str(error.value)


def test_a_failure_does_not_stop_the_session(ready, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError):
        ready.route()
    ready.pack()


def test_an_option_that_changed_a_stage_makes_its_queries_fail(routed, openfpga_module):
    routed.timing_summary()
    routed.options["place.seed"] = 5
    with pytest.raises(openfpga_module.OpenfpgaError):
        routed.timing_summary()
    routed.place()
    routed.route()
    routed.timing_summary()


# The queries are typed objects ----------------------------------------------------------


def raw(session, command, **options):
    return session.shell.run(" ".join([command] + [f"--{k} {v}" for k, v in options.items()])).data


def test_the_answers_are_what_the_shell_says(routed):
    queries = [
        ("timing_summary", routed.timing_summary()),
        ("critical_paths", routed.critical_paths()),
        ("utilization", routed.utilization()),
        ("placement", routed.placement()),
        ("congestion", routed.congestion()),
        ("report_cluster", routed.cluster_report()),
    ]
    for command, answer in queries:
        said = raw(routed, command)
        assert plain(answer) == said["data"], command
        assert answer.stage == said["stage"], command


def test_the_options_of_the_queries(routed):
    paths = routed.critical_paths(count=2)
    assert paths.requested == 2 and paths.returned <= 2
    assert plain(paths) == raw(routed, "critical_paths", count=2)["data"]

    placement = routed.placement(limit=1)
    assert placement.returned == 1 and placement.truncated
    assert plain(placement) == raw(routed, "placement", limit=1)["data"]

    clb = routed.placement(block_type="clb")
    assert clb.filter.type == "clb"
    assert all(block.type == "clb" for block in clb.blocks)

    stats = routed.cluster_report(stats=True)
    assert stats.stats.num_clusters == stats.total
    assert plain(stats) == raw(routed, "report_cluster --stats")["data"]

    assert routed.congestion(region_size=1).region_size == 1


def test_a_net_route(routed):
    path = routed.critical_paths(count=1).paths[0]
    name = next(point.net for point in path.points if point.net)
    route = routed.net_route(name)
    assert route.net == name
    assert route.nodes and route.nodes[0].parent is None
    assert plain(route) == raw(routed, "net_route", net=name)["data"]


def test_the_objects_are_typed(routed):
    timing = routed.timing_summary()
    assert isinstance(timing.setup.fmax_mhz, float)
    assert isinstance(timing.setup.critical_path.launch_clock, str)
    assert all(type(domain).__name__ == "ClockDomain" for domain in timing.clock_domains)
    path = routed.critical_paths(count=1).paths[0]
    assert type(path).__name__ == "TimingPath"
    assert type(path.points[0]).__name__ == "PathPoint"
    util = routed.utilization()
    assert util.grid.width > 0
    assert {block.name for block in util.block_types}


def test_the_objects_are_immutable(routed):
    timing = routed.timing_summary()
    with pytest.raises(dataclasses.FrozenInstanceError):
        timing.setup = None


def test_the_answer_must_be_one_that_is_read(routed, openfpga_module, monkeypatch):
    monkeypatch.setitem(openfpga_module._SCHEMA_VERSIONS, "utilization", 2)
    with pytest.raises(openfpga_module.AnswerError, match="schema version"):
        routed.utilization()
    result = routed.shell.run("utilization")
    with pytest.raises(openfpga_module.AnswerError, match="kind"):
        openfpga_module._answer(result, "placement", openfpga_module.Placement)
    with pytest.raises(openfpga_module.AnswerError, match="no answer"):
        openfpga_module._answer(
            routed.shell.run("set_app_option -n place.seed -v 1"),
            "placement",
            openfpga_module.Placement,
        )


def test_a_field_that_an_answer_gains_is_skipped(openfpga_module):
    answer = {"width": 3, "height": 4, "added_later": 1}
    grid = openfpga_module._build(openfpga_module.Grid, answer)
    assert (grid.width, grid.height) == (3, 4)
    with pytest.raises(KeyError, match="height"):
        openfpga_module._build(openfpga_module.Grid, {"width": 1})


# The options --------------------------------------------------------------------------


def test_an_option_has_its_description(session):
    option = session.options.get("place.place_algorithm")
    assert option.type == "selection"
    assert option.value == option.default == "criticality_timing"
    assert option.is_default and "bounding_box" in option.choices
    assert option.help
    assert option.stage == "PLACED"


def test_the_values_have_their_type(session):
    assert session.options["place.seed"] == 1
    assert session.options["router.flat_routing"] is False
    assert isinstance(session.options["place.place_inner_num"], float)
    assert session.options["place.place_algorithm"] == "criticality_timing"


def test_set_and_reset(session):
    session.options["place.seed"] = 42
    session.options["router.flat_routing"] = True
    session.options["place.place_inner_num"] = 0.7
    assert session.options["place.seed"] == 42
    assert session.options["router.flat_routing"] is True
    assert session.options["place.place_inner_num"] == 0.7
    assert {option.name for option in session.options.changed()} == {
        "place.seed",
        "router.flat_routing",
        "place.place_inner_num",
    }
    session.options.reset("place.seed")
    assert session.options["place.seed"] == 1
    session.options["router.flat_routing"] = False
    assert "router.flat_routing" not in {o.name for o in session.options.changed()}


def test_a_path_is_a_value(session, tmp_path):
    session.options["filename.circuit_file"] = tmp_path / "a design.blif"
    assert session.options["filename.circuit_file"] == str(tmp_path / "a design.blif")


def test_the_options_as_a_collection(session):
    names = list(session.options)
    assert len(names) == len(session.options) > 100
    assert "place.seed" in session.options and "place.nothing" not in session.options
    assert [option.name for option in session.options.list()] == names
    assert session.options.changed() == []


def test_an_unknown_option_or_value_raises(session, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.options["place.nothing"]
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.options["place.nothing"] = 1
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.options["place.seed"] = "many"
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.options["place.place_algorithm"] = "random"
    assert session.options["place.seed"] == 1


def test_the_options_belong_to_the_shell(openfpga_module):
    one, other = openfpga_module.Session(), openfpga_module.Session()
    one.options["place.seed"] = 9
    assert other.options["place.seed"] == 1
    assert json.dumps(one.options["place.seed"]) == "9"
