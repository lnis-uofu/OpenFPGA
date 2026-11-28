"""A stage that fails keeps what it left, so that what went wrong can be read.

The router is stopped after two iterations, when two wires are still used by two nets: the
failure is then a congestion, as it is for a design that does not fit the channels. What the
session says is compared with what VPR logs and writes.
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


def without_header(path):
    """A file of VPR without its first line, which says where the file it follows is"""
    return Path(path).read_bytes().split(b"\n", 1)[1]


def results(session):
    """The content of the files of the results of the stages that are reached"""
    files = dataclasses.asdict(session.state().results)
    return {key: without_header(path) for key, path in files.items() if path is not None}


def analysis_numbers(log):
    """The wirelength and the critical path that the analysis prints"""
    numbers = re.findall(r"^\s*(Total wirelength: .*|Final critical path delay.*)$", log, re.M)
    assert len(numbers) == 2, log[-2000:]
    return numbers


class Failed:
    """A session whose route failed: its log, and the results of the stages before"""

    def __init__(self, session):
        self.session = session
        self.before = results(session)
        session.options["router.max_router_iterations"] = ITERATIONS
        self.log = session.shell.run("route", check=False).log
        assert "Circuit is unroutable" in self.log


@pytest.fixture
def failed(placed):
    return Failed(placed)


@pytest.fixture
def failed_after_fabric(openfpga_module, golden, work):
    """A route that failed after the fabric was built on the routing that worked: the commands
    of the bitstream, e.g., repack, need a routed design"""
    session = make_session(openfpga_module, golden)
    session.run_flow()
    golden.link(session.shell)
    return Failed(session)


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
    # The report of the run says the same, and has the stages before
    report = failed.session.run_report()
    assert report.failed == state.failed
    assert [s.stage for s in report.stages] == ["PACKED", "PLACED"]
    assert report.placement is not None and report.routing is None
    # The report of the stages has the failure with the stage
    (routed,) = failed.session.stage_info(stage="ROUTED").stages
    assert not routed.reached and "Routing failed" in routed.failed
    (placed,) = failed.session.stage_info(stage="PLACED").stages
    assert placed.reached and placed.failed is None


def test_the_results_of_the_stages_before_stay(failed):
    assert set(failed.before) == {"packing", "placement"}
    assert results(failed.session) == failed.before


def test_what_needs_a_routed_design_says_why_it_is_refused(failed_after_fabric):
    result = failed_after_fabric.session.shell.run("repack", check=False)
    assert result.status != 0 and result.data is None
    assert "the stage ROUTED has not been reached, because it failed" in result.log
    assert "Routing failed" in result.log


def test_the_route_can_be_run_again_and_gives_the_same_failure(failed):
    session = failed.session
    again = session.shell.run("route", check=False)
    assert again.status != 0 and "Circuit is unroutable" in again.log
    assert session.state().failed.stage == "ROUTED"
    nodes = overused_nodes(failed.log)
    assert nodes and overused_nodes(again.log) == nodes


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
    timing = analysis_numbers(session.analysis().log)

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
    assert analysis_numbers(session.analysis().log) == timing


def test_a_failure_is_forgotten_when_what_made_it_fail_changes(failed_after_fabric):
    session = failed_after_fabric.session
    assert session.state().failed is not None
    session.options["router.route_chan_width"] = 21
    state = session.state()
    assert state.failed is None
    result = session.shell.run("repack", check=False)
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


def test_the_answer_follows_its_schema(failed):
    jsonschema = pytest.importorskip("jsonschema")
    answer = failed.session.shell.run("report_session").data
    schema = json.loads((SCHEMAS / "session.schema.json").read_text())
    jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)
    assert answer["data"]["failed"]["stage"] == "ROUTED"


def test_a_route_that_works_has_no_failure(placed):
    placed.route()
    assert placed.state().failed is None
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
    result = too_big.shell.run("pack", check=False)
    assert result.status != 0
    state = too_big.state()
    assert state.reached == ["DEVICE"]
    assert state.failed.stage == "PACKED" and "Packing failed" in state.failed.message
    assert state.results.packing is None


def test_a_stage_cannot_be_run_on_one_that_failed(too_big):
    assert too_big.shell.run("pack", check=False).status != 0
    result = too_big.shell.run("place", check=False)
    assert result.status != 0 and "the stage PACKED has not been reached" in result.log
    # The same failure again, from fresh data
    assert too_big.shell.run("pack", check=False).status != 0
    assert too_big.state().failed.stage == "PACKED"


def test_the_failure_is_a_diagnostic(failed_after_fabric):
    from openfpga import parse_diagnostics

    result = failed_after_fabric.session.shell.run("repack", check=False)
    kinds = {d.kind for d in parse_diagnostics(result.log, result.error)}
    assert "stage_failed" in kinds
    found = [d for d in parse_diagnostics(result.log, result.error) if d.kind == "stage_failed"][0]
    assert (found.data["stage"], found.data["command"]) == ("ROUTED", "route")
    assert "Routing failed" in found.data["reason"]
