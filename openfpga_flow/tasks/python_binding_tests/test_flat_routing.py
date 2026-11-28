"""Flat routing in the flow that is run stage by stage: route and analysis run with
router.flat_routing, as 'vpr --flat_routing on' does.

test_equivalence.py checks that the files, the results and the bitstream are those of
'vpr --flat_routing on'; the tests here check the run report, the files read back, the outputs
that follow the bitstream and the checkpoints.
"""

import math
import re
import shutil
from pathlib import Path

import pytest

from test_equivalence import Golden, in_dir, two_shells  # noqa: F401

FLOW = Path(__file__).resolve().parents[3] / "openfpga_flow"
VPR_ARCH = FLOW / "vpr_arch" / "k4_N4_tileable_40nm.xml"
OPENFPGA_ARCH = FLOW / "openfpga_arch" / "k4_N4_40nm_cc_4FlatRouter_openfpga.xml"
CIRCUIT = FLOW / "benchmarks" / "ubm" / "simple_gates" / "and2_or2" / "and2_or2.blif"


def read_inputs(session, flat=True):
    for path in (VPR_ARCH, OPENFPGA_ARCH, CIRCUIT):
        if not path.exists():
            pytest.skip(f"missing {path}")
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = 40
    session.options["router.flat_routing"] = flat
    session.read_vpr_arch(str(VPR_ARCH), layout="4x4")
    session.read_openfpga_arch(str(OPENFPGA_ARCH))
    session.read_circuit(str(CIRCUIT))


@pytest.fixture
def routed(openfpga_module, tmp_path, monkeypatch):
    """and2_or2 routed flat and analyzed, with the log of each stage"""
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    read_inputs(session)
    session.logs = {name: getattr(session, name)().log for name in ("pack", "place", "route")}
    session.logs["analysis"] = session.analysis().log
    return session


def number(log, pattern):
    (value,) = re.findall(pattern, log)
    return float(value)


def test_route_and_analysis_run_flat(routed):
    assert routed.state().reached[-2:] == ["ROUTED", "ANALYZED"]
    assert "flat routing is enabled" in routed.logs["route"]


def test_the_run_report_says_the_routing_is_flat(routed, openfpga_module, tmp_path):
    routing = routed.run_report().routing
    assert routing.flat_routing
    assert routing.wires > 0 and routing.wirelength > 0
    assert routing.channel_width == 40
    delay = number(routed.logs["route"], r"Critical path: ([-\d.e]+) ns")
    assert math.isclose(routing.critical_path_delay_ns, delay)

    other = openfpga_module.Session()
    read_inputs(other, flat=False)
    other.run_flow()
    assert not other.run_report().routing.flat_routing


def test_a_flat_routing_is_read_from_its_files(routed, openfpga_module, tmp_path):
    saved = tmp_path / "saved"
    saved.mkdir()
    for suffix in ("net", "place", "route"):
        shutil.copy(tmp_path / f"and2_or2.{suffix}", saved / f"and2_or2.{suffix}")
    expected = routed.run_report().routing
    delay = number(
        routed.logs["analysis"], r"Final critical path delay \(least slack\): ([-\d.e]+)"
    )

    session = openfpga_module.Session()
    read_inputs(session)
    session.read_packing(str(saved / "and2_or2.net"))
    session.read_placement(str(saved / "and2_or2.place"))
    session.read_routing(str(saved / "and2_or2.route"))
    log = session.analysis().log
    assert session.state().reached[-1] == "ANALYZED"
    loaded = session.run_report().routing
    assert loaded.flat_routing
    assert (loaded.wires, loaded.wirelength) == (expected.wires, expected.wirelength)
    assert math.isclose(
        number(log, r"Final critical path delay \(least slack\): ([-\d.e]+)"), delay
    )


# The rest of the flow --------------------------------------------------------------------------


def write_outputs(golden, shell):
    """What the regression task of flat routing writes after the bitstream: the fabric, the
    testbench and the SDC files. The files, by path, with the directory they are in left out"""
    reference = golden.circuit.with_suffix(".v")
    for command in (
        "write_fabric_verilog --file SRC --explicit_port_mapping --include_timing "
        "--print_user_defined_template --no_time_stamp",
        f"write_full_testbench --file SRC --reference_benchmark_file_path {reference} "
        "--include_signal_init --explicit_port_mapping --bitstream fabric.bit --no_time_stamp",
        "write_pnr_sdc --file SDC --no_time_stamp",
        "write_analysis_sdc --file SDC_analysis --no_time_stamp",
    ):
        shell.run(command)
    here = str(Path.cwd()).encode()
    return {
        str(path): path.read_bytes().replace(here, b"<run>")
        for folder in ("SRC", "SDC", "SDC_analysis")
        for path in sorted(Path(folder).rglob("*"))
        if path.is_file()
    }


def test_the_fabric_and_the_testbench_are_those_of_vpr(two_shells):
    golden = Golden("k4_flat_and2_or2")
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells

    def staged_flow():
        golden.run_staged(staged)
        golden.bitstream(staged)
        return write_outputs(golden, staged)

    def monolithic_flow():
        golden.run_monolithic(monolithic)
        golden.bitstream(monolithic)
        return write_outputs(golden, monolithic)

    staged_files = in_dir(staged_dir, staged_flow)
    monolithic_files = in_dir(monolithic_dir, monolithic_flow)
    assert any(name.endswith("_autocheck_top_tb.v") for name in staged_files), sorted(staged_files)
    assert staged_files.keys() == monolithic_files.keys()
    different = [name for name in staged_files if staged_files[name] != monolithic_files[name]]
    assert not different


def test_a_session_routed_flat_is_restored_from_its_checkpoint(openfpga_module, tmp_path):
    golden = Golden("k4_flat_routing_test")
    run, other, checkpoint = tmp_path / "run", tmp_path / "other", tmp_path / "checkpoint"
    run.mkdir()
    other.mkdir()

    def routing(session):
        report = session.run_report().routing
        return report.flat_routing, report.wires, report.wirelength, report.channel_width

    def flow():
        session = openfpga_module.Session()
        session.options["general.clock_modeling"] = "route"
        session.options["router.route_chan_width"] = golden.chan_width
        session.options["router.flat_routing"] = True
        session.read_vpr_arch(str(golden.vpr_arch), layout=golden.layout)
        session.read_openfpga_arch(str(golden.openfpga_arch))
        session.read_circuit(str(golden.circuit))
        session.run_flow()
        session.checkpoint(checkpoint)
        wanted = routing(session)
        golden.link(session.shell)
        return wanted, golden.bitstream(session.shell)

    wanted, bits = in_dir(run, flow)

    def again():
        restored = openfpga_module.Session.restore(checkpoint)
        assert restored.state().reached[-1] == "ANALYZED"
        assert restored.options["router.flat_routing"] is True
        got = routing(restored)
        golden.link(restored.shell)
        return got, golden.bitstream(restored.shell)

    got, restored_bits = in_dir(other, again)
    assert got == wanted and got[0]
    assert restored_bits == bits
