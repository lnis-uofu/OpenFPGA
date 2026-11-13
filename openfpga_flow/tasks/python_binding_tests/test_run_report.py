"""The query report_run: what each stage of the flow did.

Every number is compared with what VPR printed for the same stage, or with another query of the
same data, so that a change in the log of VPR is noticed.
"""

import json
import re
import time
from pathlib import Path

import pytest

from test_equivalence import Golden

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"
GOLDENS = ["k6_routing_test", "k6_and2_latch"]


@pytest.fixture(params=GOLDENS)
def golden(request):
    return Golden(request.param)


@pytest.fixture
def work(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return tmp_path


def make_session(openfpga_module, golden, **kwargs):
    session = openfpga_module.Session(**kwargs)
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    return session


class Flow:
    """A flow that was run: the session, the log and the wall time of each stage"""

    def __init__(self, openfpga_module, golden):
        self.session = make_session(openfpga_module, golden)
        self.logs, self.wall = {}, {}
        self.chan_width = golden.chan_width
        for stage in ("pack", "place", "route", "analysis"):
            started = time.monotonic()
            self.logs[stage] = getattr(self.session, stage)().log
            self.wall[stage] = time.monotonic() - started
        self.report = self.session.run_report()


@pytest.fixture
def flow(openfpga_module, golden, work):
    return Flow(openfpga_module, golden)


def number(log, pattern):
    (found,) = re.findall(pattern, log)
    return float(found)


# What each stage did ------------------------------------------------------------------------


def test_the_packing_is_what_vpr_logs(flow):
    packing = flow.report.packing
    log = flow.logs["pack"]
    assert packing.clusters == number(log, r"Netlist num_blocks: (\d+)")
    assert packing.nets == number(log, r"Netlist num_nets: (\d+)")
    logged = {name: int(count) for name, count in re.findall(r"Netlist (\w+) blocks: (\d+)", log)}
    logged = {name: count for name, count in logged.items() if name != "EMPTY"}
    assert {b.type: b.count for b in packing.blocks} == logged
    assert sum(b.count for b in packing.blocks) == packing.clusters


def test_the_atoms_are_the_ones_the_clusters_hold(flow):
    session = flow.session
    held = sum(
        len(session.block_info(b.name).atoms) for b in session.find("*", kind="block").blocks.items
    )
    assert flow.report.packing.atoms == held


def test_the_placement_is_what_vpr_logs(flow):
    placement = flow.report.placement
    log = flow.logs["place"]
    cost, bb_cost, td_cost = re.findall(
        r"Placement cost: ([-\d.e+]+), bb_cost: ([-\d.e+]+), td_cost: ([-\d.e+]+),", log
    )[-1]
    assert placement.cost == pytest.approx(float(cost))
    assert placement.bb_cost == pytest.approx(float(bb_cost))
    assert placement.td_cost == pytest.approx(float(td_cost))
    delay = number(log, r"Placement estimated critical path delay \(least slack\): ([-\d.e+]+) ns")
    assert placement.estimated_critical_path_delay_ns == pytest.approx(delay)
    assert placement.estimated_fmax_mhz == pytest.approx(number(log, r"Fmax: ([-\d.e+]+) MHz\n"))
    assert placement.num_temperatures == number(log, r"Placement number of temperatures: (\d+)")
    assert placement.swap_attempts == number(log, r"Placement total # of swap attempts: (\d+)")
    assert placement.num_temperatures > 0 and placement.swap_attempts > 0


def test_the_half_perimeter_is_the_one_of_the_nets(flow):
    session = flow.session
    total = 0
    for net in session.find("*", kind="net").nets.items:
        if not net.ignored:
            total += session.net_info(net.name).bounding_box.half_perimeter
    assert flow.report.placement.half_perimeter == total


def test_the_routing_is_what_vpr_logs(flow):
    routing = flow.report.routing
    log = flow.logs["route"]
    # One line of the table of the router for each iteration
    rows = re.findall(r"^\s+\d+\s+[\d.]+\s+[\d.]+\s+\d+\s+\d+\s+\d+\s+\d+\s+\d+ \(", log, re.M)
    assert routing.iterations == len(rows) == number(log, r"routed after (\d+) routing iterations")
    assert routing.wirelength == number(flow.logs["analysis"], r"Total wirelength: (\d+)")
    assert routing.channel_width == flow.chan_width
    timing = flow.session.timing_summary()
    assert routing.critical_path_delay_ns == pytest.approx(
        timing.setup.critical_path_delay_ns, rel=1e-4
    )


def test_the_wires_are_the_ones_of_the_nets(flow):
    session = flow.session
    wires = wirelength = 0
    for net in session.find("*", kind="net").nets.items:
        routing = session.net_info(net.name).routing
        if routing is not None:
            wires += routing.wires
            wirelength += routing.wirelength
    assert (flow.report.routing.wires, flow.report.routing.wirelength) == (wires, wirelength)


def test_the_time_of_a_stage_is_the_time_it_took(flow):
    seconds = {s.stage: s.seconds for s in flow.report.stages}
    logged = {
        "PACKED": number(flow.logs["pack"], r"Stage PACKED took ([\d.]+) seconds"),
        "PLACED": number(flow.logs["place"], r"Stage PLACED took ([\d.]+) seconds"),
        "ROUTED": number(flow.logs["route"], r"Stage ROUTED took ([\d.]+) seconds"),
        "ANALYZED": number(flow.logs["analysis"], r"Stage ANALYZED took ([\d.]+) seconds"),
    }
    walls = dict(zip(logged, flow.wall.values()))
    for stage, took in seconds.items():
        assert took == pytest.approx(logged[stage], abs=0.05), stage
        # The command takes at least as long as the stage that it runs
        assert 0 <= took <= walls[stage] + 0.01, stage


# What is reached ----------------------------------------------------------------------------


def test_the_report_follows_the_stages(openfpga_module, golden, work):
    session = make_session(openfpga_module, golden)
    report = session.run_report()
    assert report.stage == "DEVICE"
    assert (report.stages, report.packing, report.placement, report.routing) == (
        [],
        None,
        None,
        None,
    )
    assert report.failed is None
    session.pack()
    report = session.run_report()
    assert [s.stage for s in report.stages] == ["PACKED"]
    assert report.packing is not None and report.placement is None
    session.place()
    report = session.run_report()
    assert [s.stage for s in report.stages] == ["PACKED", "PLACED"]
    assert report.placement is not None and report.routing is None
    session.route()
    session.analysis()
    report = session.run_report()
    assert [s.stage for s in report.stages] == ["PACKED", "PLACED", "ROUTED", "ANALYZED"]
    assert report.routing is not None


def test_a_stage_that_runs_again_has_its_own_numbers(openfpga_module, golden, work):
    session = make_session(openfpga_module, golden)
    session.pack()
    session.place()
    session.route()
    first = session.run_report()
    session.options["place.seed"] = 7
    dropped = session.run_report()
    assert [s.stage for s in dropped.stages] == ["PACKED"]
    assert dropped.placement is None and dropped.routing is None
    log = session.place().log
    second = session.run_report()
    cost = re.findall(r"Placement cost: ([-\d.e+]+), bb_cost", log)[-1]
    assert second.placement.cost == pytest.approx(float(cost))
    assert second.routing is None
    # The same packing, so the same report of it
    assert second.packing == first.packing


def test_a_stage_that_was_read_has_no_log(openfpga_module, golden, work):
    session = make_session(openfpga_module, golden)
    session.pack()
    session.place()
    session.route()
    placed = session.run_report()
    files = session.state().results
    again = make_session(openfpga_module, golden)
    again.read_packing(files.packing)
    again.read_placement(files.placement)
    again.read_routing(files.routing)
    report = again.run_report()
    assert [(s.stage, s.seconds) for s in report.stages] == [
        ("PACKED", None),
        ("PLACED", None),
        ("ROUTED", None),
    ]
    # What is in the data is the same, what is only in the log is not there
    assert report.packing == placed.packing
    assert report.placement.half_perimeter == placed.placement.half_perimeter
    assert (report.placement.cost, report.placement.swap_attempts) == (None, None)
    assert report.routing.wirelength == placed.routing.wirelength
    assert (report.routing.iterations, report.routing.critical_path_delay_ns) == (None, None)


def test_a_route_that_failed_is_reported_as_failed(openfpga_module, golden, work):
    session = make_session(openfpga_module, golden)
    session.options["router.max_router_iterations"] = 1
    session.pack()
    session.place()
    if session.shell.run("route", check=False).status == 0:
        pytest.skip("the design routes in one iteration")
    report = session.run_report()
    assert [s.stage for s in report.stages] == ["PACKED", "PLACED"]
    assert report.routing is None
    assert report.failed.stage == "ROUTED" and "Routing failed" in report.failed.message


def test_the_flow_of_vpr_has_the_numbers_of_its_data(openfpga_module, golden, work):
    """After the command vpr there is no session of stages: the data of VPR is read, and there is
    no log to read from"""
    shell = openfpga_module.OpenfpgaShell()
    shell.run(golden.vpr_command())
    answer = shell.run("report_run").data["data"]
    assert [s["stage"] for s in answer["stages"]] == ["PACKED", "PLACED", "ROUTED"]
    assert all(s["seconds"] is None for s in answer["stages"])
    assert answer["packing"]["clusters"] > 0
    assert answer["placement"]["half_perimeter"] > 0 and answer["placement"]["cost"] is None
    assert answer["routing"]["wirelength"] > 0 and answer["routing"]["iterations"] is None


def test_the_report_needs_the_inputs(openfpga_module, work):
    # A worker is a process of its own, with no data of VPR
    with openfpga_module.RemoteSession(cwd=work) as worker:
        with pytest.raises(openfpga_module.OpenfpgaError) as refused:
            worker.run_report()
    assert "no data of VPR" in str(refused.value)


# The answer ---------------------------------------------------------------------------------


def test_the_object_is_the_answer(flow):
    said = flow.session.shell.run("report_run").data
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "run_report.schema.json").read_text())
    jsonschema.validate(said, schema, cls=jsonschema.Draft202012Validator)
    assert said["kind"] == "run_report" and said["stage"] == "DEVICE"
    import dataclasses

    plain = dataclasses.asdict(flow.report)
    del plain["stage"]
    assert plain == said["data"]


def test_the_report_is_not_a_command_of_the_journal(openfpga_module, golden, work):
    session = make_session(openfpga_module, golden, journal=work / "journal")
    session.pack()
    session.run_report()
    text = (work / "journal" / "replay.openfpga").read_text()
    assert "report_run" not in text
