"""The analytical placer of VPR in the flow that is run stage by stage (option place.placer):
pack and place both run it, and reach PACKED and PLACED together, which are then one result. The
files, the results and the bitstream are those of 'vpr --analytical_place --route --analysis'.
"""

import json
from pathlib import Path

import pytest

from test_equivalence import (  # noqa: F401
    GOLDEN,
    GOLDEN_FLAT,
    Golden,
    in_dir,
    qor,
    two_shells,
)

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"
REACHED = ["DEVICE", "PACKED", "PLACED"]


def staged_ap(golden, shell):
    """The inputs, then pack (which places too), route and analysis. Return the log"""
    shell.run("set_app_option -n general.clock_modeling -v route")
    shell.run(f"set_app_option -n router.route_chan_width -v {golden.chan_width}")
    shell.run("set_app_option -n place.placer -v analytical")
    if golden.flat:
        shell.run("set_app_option -n router.flat_routing -v true")
    shell.run(f"read_vpr_arch -f {golden.vpr_arch} -l {golden.layout}")
    shell.run(f"read_openfpga_arch -f {golden.openfpga_arch}")
    shell.run(f"read_circuit -f {golden.circuit}")
    log = "".join(shell.run(stage).log for stage in ("pack", "route", "analysis"))
    golden.link(shell)
    return log


def monolithic_ap(golden, shell):
    shell.run(f"read_openfpga_arch -f {golden.openfpga_arch}")
    command = (
        f"vpr {golden.vpr_arch} {golden.circuit} --clock_modeling route --device "
        f"{golden.layout} --route_chan_width {golden.chan_width} --analytical_place --route "
        "--analysis" + (" --flat_routing on" if golden.flat else "")
    )
    log = shell.run(command).log
    golden.link(shell)
    return log


@pytest.mark.parametrize("name", sorted(GOLDEN) + sorted(GOLDEN_FLAT))
def test_the_staged_flow_equals_vpr_analytical_place(name, two_shells):
    golden = Golden(name)
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells

    def run(flow, shell):
        log = flow(golden, shell)
        return log, golden.files(), golden.bitstream(shell)

    staged_log, staged_files, staged_bits = in_dir(staged_dir, run, staged_ap, staged)
    vpr_log, vpr_files, vpr_bits = in_dir(monolithic_dir, run, monolithic_ap, monolithic)
    assert "AP Global Placer" in staged_log
    assert staged_files == vpr_files
    assert qor(staged_log) == qor(vpr_log)
    assert qor(staged_log)["wirelength"]
    assert staged_bits == vpr_bits


@pytest.fixture
def golden():
    return Golden("k4_routing_test")


def start(openfpga_module, golden):
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.options["place.placer"] = "analytical"
    session.read_vpr_arch(str(golden.vpr_arch), layout=golden.layout)
    session.read_openfpga_arch(str(golden.openfpga_arch))
    session.read_circuit(str(golden.circuit))
    return session


@pytest.fixture
def session(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return start(openfpga_module, golden)


def placement(golden):
    """The .place file without its first line, which names the files it follows"""
    return Path(f"{golden.name}.place").read_text().split("\n", 1)[1]


@pytest.mark.parametrize("command", ["pack", "place"])
def test_pack_and_place_both_pack_and_place_at_once(session, command):
    if command == "place":
        session.pack()
    log = getattr(session, command)().log
    assert "AP Global Placer" in log
    assert session.state().reached == REACHED


def test_a_change_to_the_placement_drops_the_packing_too(session):
    session.pack()
    session.options["place.seed"] = 7
    assert session.state().reached == ["DEVICE"]
    session.pack()
    session.options["aplace.ap_timing_tradeoff"] = 0.3
    assert session.state().reached == ["DEVICE"]
    session.pack()
    # A change of the placer drops the packing, as any packing option
    session.options["place.placer"] = "classic"
    assert session.state().reached == ["DEVICE"]


def test_a_route_that_is_run_again_makes_the_same_placement(session, golden):
    session.pack()
    session.route()
    before = placement(golden)
    session.options["router.max_router_iterations"] = 40
    assert session.state().reached == REACHED
    log = session.route().log
    # The stages before the routing were made again on fresh data, with the analytical placer
    assert "AP Global Placer" in log
    assert placement(golden) == before


def test_the_placement_of_the_analytical_placer_is_not_read_apart(openfpga_module, session, golden):
    session.pack()
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        session.read_placement(f"{golden.name}.place")
    assert "cannot be made again apart" in error.value.log


def test_the_run_report_says_the_placer(session, golden):
    jsonschema = pytest.importorskip("jsonschema")
    session.pack()
    report = session.run_report()
    assert report.placement.placer == "analytical"
    estimates = report.placement.global_placement
    assert estimates.estimated_wirelength > 0
    assert estimates.estimated_critical_path_delay_ns > 0
    # The detailed placer is the annealer, which reports as the classic placer does
    assert report.placement.estimated_critical_path_delay_ns > 0
    raw = session.shell.run("report_run").data
    schema = json.loads((SCHEMAS / "run_report.schema.json").read_text())
    jsonschema.validate(raw, schema, cls=jsonschema.Draft202012Validator)

    session.options["place.placer"] = "classic"
    session.pack()
    session.place()
    report = session.run_report()
    assert report.placement.placer == "classic"
    assert report.placement.global_placement is None


def test_a_session_is_restored_with_its_placer(openfpga_module, golden, tmp_path, monkeypatch):
    run, other, checkpoint = tmp_path / "run", tmp_path / "other", tmp_path / "checkpoint"
    run.mkdir()
    other.mkdir()

    def results(session):
        report = session.run_report()
        return report.placement.half_perimeter, report.routing.wirelength, report.routing.wires

    def flow():
        session = start(openfpga_module, golden)
        session.pack()
        session.route()
        session.analysis()
        session.checkpoint(checkpoint)
        wanted = results(session)
        golden.link(session.shell)
        return wanted, golden.bitstream(session.shell)

    wanted, bits = in_dir(run, flow)

    def again():
        restored = openfpga_module.Session.restore(checkpoint)
        assert restored.state().reached[-1] == "ANALYZED"
        assert restored.options["place.placer"] == "analytical"
        got = results(restored)
        golden.link(restored.shell)
        restored_bits = golden.bitstream(restored.shell)
        # The packing and the placement read from the checkpoint are still one result
        restored.options["place.seed"] = 7
        return got, restored_bits, restored.state().reached

    got, restored_bits, reached = in_dir(other, again)
    assert got == wanted
    assert restored_bits == bits
    assert reached == ["DEVICE"]


def test_a_failure_of_the_analytical_placer_is_told(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    golden = Golden("k6_routing_test")  # 6-input LUTs, which the circuit needs
    big = golden.circuit.parents[2] / ".." / "mcnc_big20" / "ex5p" / "ex5p.blif"
    if not big.exists():
        pytest.skip(f"missing {big}")
    golden.layout, golden.circuit = "2x2", big  # too small for the design
    session = start(openfpga_module, golden)
    result = session.shell.run("pack", check=False)
    assert result.status != 0
    state = session.state()
    assert state.reached == ["DEVICE"]
    assert state.failed.stage == "PACKED"
    assert "Analytical placement failed" in state.failed.message
