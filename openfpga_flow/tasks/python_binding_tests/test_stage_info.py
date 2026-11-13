"""The query report_stage: what can be changed for each stage.

The answer is compared with what the options do when they are set: the stages that a change drops
are the ones that the answer says, and every option of the table is under the stage it tunes.
"""

import dataclasses
import json
from pathlib import Path

import pytest

from test_equivalence import Golden

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"
FLOW = ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]
ALL = FLOW + ["REPACKED", "BITSTREAM"]


@pytest.fixture
def golden():
    return Golden("k6_routing_test")


@pytest.fixture
def session(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    return session


def by_stage(report):
    return {s.stage: s for s in report.stages}


def run(session):
    for stage in ("pack", "place", "route", "analysis"):
        getattr(session, stage)()


# The options ----------------------------------------------------------------------------------


def test_every_option_is_under_the_stage_it_tunes(session):
    stages = by_stage(session.stage_info())
    assert list(stages) == ALL
    listed = [item for s in stages.values() for item in s.options.items]
    table = session.options.list()
    assert sorted(o.name for o in listed) == sorted(o.name for o in table)
    for stage in stages.values():
        assert all(item.stage == stage.stage for item in stage.options.items)
        assert stage.options.total == stage.options.matched == stage.options.returned
        assert stage.options.returned == len(stage.options.items) and not stage.options.truncated
    # What the table says of an option is what the stage says of it
    assert {o.name: o for o in listed} == {o.name: o for o in table}
    assert sum(s.options.total for s in stages.values()) == len(table)


def test_the_stages_are_the_tree_of_the_flow(session):
    stages = by_stage(session.stage_info())
    assert {name: s.requires for name, s in stages.items()} == {
        "DEVICE": None,
        "PACKED": "DEVICE",
        "PLACED": "PACKED",
        "ROUTED": "PLACED",
        "ANALYZED": "ROUTED",
        "REPACKED": "ROUTED",
        "BITSTREAM": "REPACKED",
    }
    # The stages that depend on a stage are dropped with it: not the ones on another branch
    assert stages["PLACED"].dropped_by_change == [
        "PLACED",
        "ROUTED",
        "ANALYZED",
        "REPACKED",
        "BITSTREAM",
    ]
    assert stages["ANALYZED"].dropped_by_change == ["ANALYZED"]
    assert stages["REPACKED"].dropped_by_change == ["REPACKED", "BITSTREAM"]
    assert stages["DEVICE"].dropped_by_change == ALL


def test_a_change_drops_the_stages_that_the_answer_says(session):
    """An option of each stage is set when the whole flow is reached: what it drops is what the
    answer says, among the stages that are reached"""
    run(session)
    stages = by_stage(session.stage_info())
    chosen = {
        "DEVICE": ("place.place_chan_width", 102),
        "PACKED": ("cluster.enable_clustering_pin_feasibility_filter", False),
        "PLACED": ("place.seed", 4),
        "ROUTED": ("router.first_iter_pres_fac", 0.25),
        "ANALYZED": ("analysis.generate_post_implementation_sdc", True),
    }
    for stage, (name, value) in chosen.items():
        assert name in [o.name for o in stages[stage].options.items], name
        change = session.set_option(name, value)
        assert change.before != change.after, name
        assert change.dropped == [s for s in stages[stage].dropped_by_change if s in FLOW], stage
        run(session)
        assert session.state().reached == FLOW


# What is asked --------------------------------------------------------------------------------


def test_a_stage_is_asked_by_its_name_or_its_command(session):
    placed = session.stage_info(stage="PLACED")
    assert [s.stage for s in placed.stages] == ["PLACED"] and placed.filter.stage == "PLACED"
    for name in ("placed", "Placed", "place", "read_placement"):
        assert session.stage_info(stage=name).stages == placed.stages, name
    assert [s.stage for s in session.stage_info(stage="read_routing").stages] == ["ROUTED"]
    assert [s.stage for s in session.stage_info(stage="read_circuit").stages] == ["DEVICE"]
    assert session.stage_info().filter == dataclasses.replace(placed.filter, stage=None)


def test_a_stage_that_does_not_exist_is_refused(session):
    result = session.shell.run("report_stage --stage routing", check=False)
    assert result.status != 0 and result.data is None
    assert "'routing' does not exist" in result.log
    for stage in ALL:
        assert stage in result.log


def test_the_options_that_changed(session):
    assert all(s.options.matched == 0 for s in session.stage_info(changed=True).stages) is False
    session.options["place.seed"] = 9
    changed = by_stage(session.stage_info(changed=True))
    seed = next(o for o in changed["PLACED"].options.items if o.name == "place.seed")
    assert (seed.value, seed.is_default) == (9, False)
    # The options that the session set before are changed too: the others are not there
    names = {o.name for s in changed.values() for o in s.options.items}
    assert names == {o.name for o in session.options.changed()}
    assert changed["PLACED"].options.total > changed["PLACED"].options.matched


def test_the_options_that_match_a_pattern(session):
    answer = by_stage(session.stage_info(query="place.*"))
    assert answer["PLACED"].options.matched > 0
    for stage in answer.values():
        assert all(o.name.startswith("place.") for o in stage.options.items)
    only = session.stage_info(stage="PLACED", query="place.seed")
    assert [o.name for o in only.stages[0].options.items] == ["place.seed"]
    none = session.stage_info(stage="PLACED", query="no.such*").stages[0]
    assert none.options.matched == 0 and none.options.items == [] and none.options.total > 0


def test_the_list_is_cut_at_the_limit(session):
    placed = session.stage_info(stage="PLACED", limit=3).stages[0]
    assert placed.options.returned == len(placed.options.items) == 3
    assert placed.options.truncated and placed.options.matched == placed.options.total > 3
    full = session.stage_info(stage="PLACED").stages[0]
    assert placed.options.items == full.options.items[:3]
    assert session.shell.run("report_stage --limit 0", check=False).status != 0


# What the session knows -----------------------------------------------------------------------


def test_the_stages_that_are_reached(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    assert not any(s.reached for s in session.stage_info().stages)
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    session.pack()
    reached = [s.stage for s in session.stage_info().stages if s.reached]
    assert reached == session.state().reached == ["DEVICE", "PACKED"]
    run_rest = ("place", "route", "analysis")
    for stage in run_rest:
        getattr(session, stage)()
    assert [s.stage for s in session.stage_info().stages if s.reached] == FLOW


def test_a_stage_that_failed_says_why(session):
    session.options["router.max_router_iterations"] = 1
    session.pack()
    session.place()
    if session.shell.run("route", check=False).status == 0:
        pytest.skip("the design routes in one iteration")
    stages = by_stage(session.stage_info())
    assert stages["ROUTED"].failed == session.state().failed.message
    assert not stages["ROUTED"].reached and stages["PLACED"].failed is None


def test_the_commands_exist(session):
    listed = {word for word in session.shell.run("help").log.split() if word.isidentifier()}
    for stage in session.stage_info().stages:
        for command in stage.commands:
            assert command in listed, command
    assert "no_such_command" not in listed


def test_it_needs_no_data_of_vpr(openfpga_module, tmp_path):
    # A worker is a process of its own, with no data of VPR
    with openfpga_module.RemoteSession(cwd=tmp_path) as worker:
        report = worker.stage_info(stage="place")
    assert report.stages[0].options.total > 0 and not report.stages[0].reached


# The answer -----------------------------------------------------------------------------------


def test_the_object_is_the_answer(session):
    jsonschema = pytest.importorskip("jsonschema")
    said = session.shell.run("report_stage --stage PLACED --query place.s*").data
    schema = json.loads((SCHEMAS / "stage_info.schema.json").read_text())
    jsonschema.validate(said, schema, cls=jsonschema.Draft202012Validator)
    assert said["kind"] == "stage_info" and said["stage"] == "DEVICE"
    report = session.stage_info(stage="PLACED", query="place.s*")
    plain = dataclasses.asdict(report)
    del plain["stage"]
    assert plain == said["data"]
    jsonschema.validate(
        session.shell.run("report_stage").data, schema, cls=jsonschema.Draft202012Validator
    )


def test_the_query_is_not_a_command_of_the_journal(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.stage_info()
    session.command("report_stage")
    assert "report_stage" not in (tmp_path / "journal" / "replay.openfpga").read_text()
