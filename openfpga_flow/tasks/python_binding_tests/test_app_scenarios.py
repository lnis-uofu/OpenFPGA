"""The scenarios of the agent, with the scripted plans on the real tools (no model)

Each scenario has a task, a plan of what a right model calls, and a checker that reads the session.
A plan passes its checker; a run that does the wrong thing does not.
"""

import asyncio

import pytest

pytest.importorskip("starlette")

from test_equivalence import Golden  # noqa: E402

from openfpga_app.agent import scenarios as sc  # noqa: E402
from openfpga_app.agent.scripted import ScriptedModel, call, say  # noqa: E402


@pytest.fixture
def design():
    g = Golden("k6_routing_test")
    return sc.Design(g.vpr_arch, g.openfpga_arch, g.circuit, g.layout, g.chan_width)


def run(scenario, design, tmp_path, model=None):
    return asyncio.run(sc.run_scenario(scenario, design, tmp_path, model, call_timeout_s=120))


@pytest.mark.parametrize("name", sorted(sc.SCENARIOS))
def test_the_scripted_plan_of_each_scenario_passes_its_checker(
    openfpga_module, tmp_path, design, name
):
    scenario = sc.SCENARIOS[name]
    outcome = run(scenario, design, tmp_path)
    assert scenario.problems(outcome, design) == []
    assert outcome.steps >= 1 and not outcome.errors


def test_the_checker_reads_the_session_and_not_the_words(openfpga_module, tmp_path, design):
    # Says that it routed, and does nothing
    scenario = sc.SCENARIOS["set_width"]
    outcome = run(scenario, design, tmp_path, ScriptedModel([say("Done: routed with width 22.")]))
    problems = scenario.problems(outcome, design)
    assert "the design was not routed" in problems
    assert any("channel width" in p for p in problems)


def test_a_wrong_number_for_the_fmax_is_found(openfpga_module, tmp_path, design):
    scenario = sc.SCENARIOS["flow_fmax"]
    plan = [t for t in scenario.plan(design)[:-1]] + [  # all of the calls, then a wrong answer
        say("The Fmax is 999.9 MHz.")
    ]
    outcome = run(scenario, design, tmp_path, ScriptedModel(plan))
    assert any("Fmax" in p for p in scenario.problems(outcome, design))


def test_a_run_that_stops_on_the_limit_is_reported(openfpga_module, tmp_path, design):
    scenario = sc.SCENARIOS["missing_file"]
    bad = call(
        "read_inputs", vpr_arch="x.xml", layout="2x2", openfpga_arch="y.xml", circuit="z.blif"
    )
    plan = [say("", call("session_open", name="m"))] + [say("", bad)] * 5
    outcome = run(scenario, design, tmp_path, ScriptedModel(plan))
    assert outcome.stopped
    assert scenario.problems(outcome, design)  # it never said what the file was
