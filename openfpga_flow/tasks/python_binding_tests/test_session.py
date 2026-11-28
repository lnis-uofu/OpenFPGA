"""The Session class: the commands of the shell, through methods that raise on failure and give
typed objects."""

import dataclasses
import json
import re
from pathlib import Path
from typing import List

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


def analysis_numbers(log):
    """The wirelength and the critical path that the analysis prints"""
    return re.findall(r"^\s*(Total wirelength: .*|Final critical path delay.*)$", log, re.M)


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
    wanted = analysis_numbers(routed.analysis().log)
    assert len(wanted) == 2
    (tmp_path / "other").mkdir()

    def imported():
        other = openfpga_module.Session()
        inputs(other, and2_files, circuit)
        other.read_packing(files["net"])
        other.read_placement(files["place"])
        other.read_routing(files["route"])
        return analysis_numbers(other.analysis().log)

    assert in_dir(tmp_path / "other", imported) == wanted


# Failures raise -----------------------------------------------------------------------


def test_a_stage_before_its_inputs_raises(session, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        session.pack()
    assert error.value.command == "pack"
    assert "read_vpr_arch" in str(error.value)


def test_a_missing_file_raises(session, openfpga_module, tmp_path):
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.read_openfpga_arch(tmp_path / "missing.xml")


def test_a_failure_does_not_stop_the_session(ready, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError):
        ready.route()
    ready.pack()


def test_an_option_that_changed_a_stage_makes_the_next_stages_fail(routed, openfpga_module):
    routed.options["place.seed"] = 5
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        routed.route()
    assert "place" in str(error.value)
    routed.place()
    routed.route()
    routed.analysis()


# The answers are typed objects ----------------------------------------------------------


@pytest.fixture
def option_list(openfpga_module):
    """The answer of report_app_option --json, read as a dataclass of the test"""
    options_type = List[openfpga_module.AppOption]

    @dataclasses.dataclass(frozen=True)
    class Options:
        options: options_type

    return Options


def test_an_answer_is_what_the_shell_says(session, option_list):
    session.options["place.seed"] = 3
    answer = session._query("report_app_option", "app_options", option_list, json=True)
    said = session.shell.run("report_app_option --json").data
    assert [dataclasses.asdict(o) for o in answer.options] == said["data"]["options"]
    one = session._query(
        "report_app_option", "app_options", option_list, name="place.seed", json=True
    )
    (seed,) = one.options
    assert type(seed).__name__ == "AppOption"
    assert seed.value == 3 and not seed.is_default


def test_the_objects_are_immutable(session, option_list):
    answer = session._query("report_app_option", "app_options", option_list, json=True)
    with pytest.raises(dataclasses.FrozenInstanceError):
        answer.options = []
    with pytest.raises(dataclasses.FrozenInstanceError):
        answer.options[0].value = 1


def test_the_answer_must_be_one_that_is_read(session, openfpga_module, option_list, monkeypatch):
    monkeypatch.setitem(openfpga_module._SCHEMA_VERSIONS, "app_options", 2)
    with pytest.raises(openfpga_module.AnswerError, match="schema version"):
        session._query("report_app_option", "app_options", option_list, json=True)
    result = session.shell.run("report_app_option --json")
    with pytest.raises(openfpga_module.AnswerError, match="kind"):
        openfpga_module._answer(result, "another_kind", option_list)
    with pytest.raises(openfpga_module.AnswerError, match="no answer"):
        openfpga_module._answer(
            session.shell.run("set_app_option -n place.seed -v 1"), "app_options", option_list
        )


def test_a_kind_of_answer_is_registered(session, openfpga_module, option_list, monkeypatch):
    """The kind of the answer of a command that a plugin adds"""
    monkeypatch.setattr(openfpga_module, "_SCHEMA_VERSIONS", dict(openfpga_module._SCHEMA_VERSIONS))
    answer = {"kind": "plugin_answer", "schema_version": 1, "stage": "DEVICE", "data": {}}
    result = type("Result", (), {"data": answer})()
    with pytest.raises(openfpga_module.AnswerError, match="does not read"):
        openfpga_module._answer(result, "plugin_answer", option_list)
    openfpga_module.register_answer_kind("plugin_answer", 1)
    answer["data"] = {"options": []}
    assert openfpga_module._answer(result, "plugin_answer", option_list).options == []
    openfpga_module.register_answer_kind("plugin_answer", 2)
    with pytest.raises(openfpga_module.AnswerError, match="schema version"):
        openfpga_module._answer(result, "plugin_answer", option_list)
    for kind, version in (("", 1), ("plugin_answer", 0), ("plugin_answer", True)):
        with pytest.raises(ValueError):
            openfpga_module.register_answer_kind(kind, version)


def test_a_field_that_an_answer_gains_is_skipped(openfpga_module):
    @dataclasses.dataclass(frozen=True)
    class Grid:
        width: int
        height: int

    answer = {"width": 3, "height": 4, "added_later": 1}
    grid = openfpga_module._build(Grid, answer)
    assert (grid.width, grid.height) == (3, 4)
    with pytest.raises(KeyError, match="height"):
        openfpga_module._build(Grid, {"width": 1})


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
