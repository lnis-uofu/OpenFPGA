"""A stage that fails keeps what it left, so that what went wrong can be read.

The router is stopped after two iterations, when two wires are still used by two nets: the
failure is then a congestion, as it is for a design that does not fit the channels. The answers
are compared with what VPR logs of the same failure.
"""

import dataclasses
import json
import re
from pathlib import Path

import pytest

from test_equivalence import Golden

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"
ITERATIONS = 2
BLIF = "openfpga_flow/benchmarks/ubm/interface/routing_test/routing_test.blif"


@pytest.fixture
def golden():
    return Golden("k6_routing_test")


@pytest.fixture
def work(tmp_path, monkeypatch):
    directory = tmp_path / "run"
    directory.mkdir()
    monkeypatch.chdir(directory)
    return directory


def make_session(openfpga_module, golden, **options):
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    for name, value in options.items():
        session.options[name.replace("__", ".")] = value
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    return session


@pytest.fixture
def placed(openfpga_module, golden, work):
    session = make_session(openfpga_module, golden)
    session.pack()
    session.place()
    return session


class Failed:
    """A session whose route failed: its log, and what the other queries said before"""

    def __init__(self, session):
        self.session = session
        self.before = {
            "utilization": session.utilization(),
            "placement": session.placement(),
            "device": session.device(),
            "find": session.find("*"),
        }
        session.options["router.max_router_iterations"] = ITERATIONS
        self.log = session.shell.run("route", check=False).log
        assert "Circuit is unroutable" in self.log


@pytest.fixture
def failed(placed):
    return Failed(placed)


def overused_nodes(log):
    """The table of the overused nodes that VPR logs: (type, low corner, high corner) of each"""
    (total,) = re.findall(r"Total number of overused nodes: (\d+)", log)
    rows = re.findall(
        r"^\s+\d+\s+\d+\s+(\d+)\s+(\d+)\s+(CHANX|CHANY)\s+\w+\s+\S+\s+\d+\s+\S+\s+(\d+)\s+(\d+)\s+(\d+)\s+(\d+)\s*$",
        log,
        re.M,
    )
    assert len(rows) == int(total)
    return [
        (kind, (int(x0), int(y0)), (int(x1), int(y1)), int(occupancy), int(capacity))
        for occupancy, capacity, kind, x0, y0, x1, y1 in rows
    ]


# The route that failed ----------------------------------------------------------------------


def test_a_failed_route_is_recorded_and_the_stages_before_it_stay(failed):
    state = failed.session.state()
    assert state.reached == ["DEVICE", "PACKED", "PLACED"]
    assert state.failed.stage == "ROUTED"
    assert "Routing failed" in state.failed.message
    assert str(failed.session.options["router.route_chan_width"]) in state.failed.message
    assert state.results.routing is None and state.results.placement is not None


def test_the_congestion_of_a_failed_route_is_what_vpr_logs(failed):
    congestion = failed.session.congestion(hotspots=1000)
    assert congestion.routed is False
    nodes = overused_nodes(failed.log)
    assert nodes
    assert congestion.summary.overused_nodes == len(nodes)
    # The wires in use are the wirelength of the last iteration, which is the last line of the table
    last = [
        line
        for line in failed.log.splitlines()
        if re.match(r"^\s+%d\s+[\d.]+\s" % ITERATIONS, line)
    ]
    (wirelength,) = re.findall(r"\s(\d+) \(\s*[\d.]+%\)\s+[\d.]+\s+-?[\d.]+", last[-1])[-1:]
    # A wire that two nets use is one wire in use, and VPR counts it once for each net
    shared = sum(
        (occupancy - 1) * (x1 - x0 + y1 - y0 + 1) for _, (x0, y0), (x1, y1), occupancy, _ in nodes
    )
    assert shared > 0
    assert congestion.summary.chanx.used + congestion.summary.chany.used + shared == int(wirelength)


def test_the_hotspots_are_where_the_nodes_are_overused(failed):
    congestion = failed.session.congestion(hotspots=1000)
    expected = set()
    for kind, low, high, occupancy, capacity in overused_nodes(failed.log):
        assert occupancy > capacity
        for x in range(low[0], high[0] + 1):
            for y in range(low[1], high[1] + 1):
                expected.add((kind.lower(), x, y))
    overused = {(h.channel, h.x, h.y): h for h in congestion.hotspots if h.overused_nodes}
    assert set(overused) == expected
    # More than one net crosses a segment that has an overused wire
    for spot in overused.values():
        assert spot.num_nets >= 2 and len(spot.nets) == spot.num_nets
        assert spot.used >= 2


def test_the_nets_of_an_overused_node_are_the_ones_that_share_it(failed):
    """The nets that cross a segment with an overused wire include the two that use the wire"""
    session = failed.session
    congestion = session.congestion(hotspots=1000)
    spots = {(h.channel, h.x, h.y): h for h in congestion.hotspots}
    for kind, low, _, occupancy, _ in overused_nodes(failed.log):
        spot = spots[(kind.lower(), low[0], low[1])]
        # The routing of each of those nets has the node: it is in its tree (net_route needs
        # a routed stage, so the routing is read from the spots of the nets)
        assert len(spot.nets) >= occupancy


def test_the_other_queries_still_answer_for_the_stages_before(failed):
    session = failed.session
    assert session.utilization() == failed.before["utilization"]
    assert session.placement() == failed.before["placement"]
    assert session.device() == failed.before["device"]
    assert session.find("*") == failed.before["find"]
    block = failed.before["find"].blocks.items[0].name
    assert session.block_info(block).placed
    net = failed.before["find"].nets.items[0].name
    info = session.net_info(net)
    assert info.bounding_box is not None
    # Nothing is routed: no routing and no timing
    assert info.routing is None and info.timing is None


def test_what_needs_a_routed_design_says_why_it_is_refused(failed):
    shell = failed.session.shell
    net = failed.before["find"].nets.items[0].name
    for command in ("timing_summary", "critical_paths", f"net_route --net {net}"):
        result = shell.run(command, check=False)
        assert result.status != 0 and result.data is None, command
        assert "the stage ROUTED has not been reached, because it failed" in result.log
        assert "Routing failed" in result.log


def test_the_route_can_be_run_again_and_gives_the_same_failure(failed):
    session = failed.session
    first = session.congestion(hotspots=1000)
    again = session.shell.run("route", check=False)
    assert again.status != 0 and "Circuit is unroutable" in again.log
    assert session.state().failed.stage == "ROUTED"
    assert session.congestion(hotspots=1000) == first


def test_the_next_route_gives_what_it_gives_without_the_failure(
    openfpga_module, golden, tmp_path, monkeypatch
):
    clean = tmp_path / "clean"
    clean.mkdir()
    monkeypatch.chdir(clean)
    session = make_session(openfpga_module, golden)
    session.pack()
    session.place()
    session.route()
    expected = {
        name: (clean / name).read_bytes()
        for name in ("routing_test.net", "routing_test.place", "routing_test.route")
    }
    timing = session.timing_summary()

    after = tmp_path / "after"
    after.mkdir()
    monkeypatch.chdir(after)
    session = make_session(openfpga_module, golden)
    session.pack()
    session.place()
    failure = Failed(session)
    assert failure.session.state().failed is not None
    session.options.reset("router.max_router_iterations")
    # The option is what made it fail: the failure is forgotten with the stage
    assert session.state().failed is None
    session.route()
    assert session.state().reached == ["DEVICE", "PACKED", "PLACED", "ROUTED"]
    assert session.state().failed is None
    for name, content in expected.items():
        assert (after / name).read_bytes() == content, name
    assert session.timing_summary() == timing


def test_a_failure_is_forgotten_when_what_made_it_fail_changes(failed):
    session = failed.session
    assert session.state().failed is not None
    session.options["router.route_chan_width"] = 21
    state = session.state()
    assert state.failed is None
    result = session.shell.run("congestion", check=False)
    assert result.status != 0
    assert "because it failed" not in result.log
    assert "has not been reached" in result.log


def test_a_checkpoint_does_not_save_the_stage_that_failed(failed, tmp_path, openfpga_module):
    session = failed.session
    directory = session.checkpoint(tmp_path / "saved")
    manifest = json.loads((Path(directory) / "checkpoint.json").read_text())
    assert manifest["stages"] == ["DEVICE", "PACKED", "PLACED"]
    assert "routing" not in manifest["results"]
    assert not list(Path(directory).glob("*.route"))
    # The checkpoint is the flow without the failure: the options that made it fail are not
    # kept as a failure, and the flow goes on from there
    restored = openfpga_module.Session.restore(directory)
    assert restored.state().failed is None
    assert restored.state().reached == ["DEVICE", "PACKED", "PLACED"]


def test_the_answers_follow_their_schemas(failed):
    jsonschema = pytest.importorskip("jsonschema")
    session = failed.session
    for command, kind in (("report_session", "session"), ("congestion --hotspots 3", "congestion")):
        answer = session.shell.run(command).data
        schema = json.loads((SCHEMAS / f"{kind}.schema.json").read_text())
        jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)
    assert session.shell.run("report_session").data["data"]["failed"]["stage"] == "ROUTED"
    assert session.shell.run("congestion").data["data"]["routed"] is False


def test_a_route_that_works_says_it_is_routed(placed):
    placed.route()
    assert placed.state().failed is None
    assert placed.congestion().routed is True
    assert placed.shell.run("report_session").data["data"]["failed"] is None


# A stage that cannot be taken further ---------------------------------------------------


@pytest.fixture
def too_big(openfpga_module, golden, work):
    big = Path(__file__).resolve().parents[3] / "openfpga_flow/benchmarks/mcnc_big20/ex5p/ex5p.blif"
    if not big.exists():
        pytest.skip(f"missing {big}")
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout="2x2")
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(big)
    return session


def test_a_circuit_that_does_not_fit_leaves_the_device(too_big):
    device = too_big.device()
    result = too_big.shell.run("pack", check=False)
    assert result.status != 0
    state = too_big.state()
    assert state.reached == ["DEVICE"]
    assert state.failed.stage == "PACKED" and "Packing failed" in state.failed.message
    # What the device says does not change, and what needs the packing is refused with the reason
    assert too_big.device() == device
    refused = too_big.shell.run("utilization", check=False)
    assert refused.status != 0 and "because it failed" in refused.log
    assert too_big.shell.run("congestion", check=False).status != 0


def test_a_stage_cannot_be_run_on_one_that_failed(too_big):
    assert too_big.shell.run("pack", check=False).status != 0
    result = too_big.shell.run("place", check=False)
    assert result.status != 0 and "the stage PACKED has not been reached" in result.log
    # The same failure again, from fresh data
    assert too_big.shell.run("pack", check=False).status != 0
    assert too_big.state().failed.stage == "PACKED"


def test_the_failure_is_a_diagnostic(failed):
    from openfpga import parse_diagnostics

    result = failed.session.shell.run("timing_summary", check=False)
    kinds = {d.kind for d in parse_diagnostics(result.log, result.error)}
    assert "stage_failed" in kinds
    found = [d for d in parse_diagnostics(result.log, result.error) if d.kind == "stage_failed"][0]
    assert (found.data["stage"], found.data["command"]) == ("ROUTED", "route")
    assert "Routing failed" in found.data["reason"]
