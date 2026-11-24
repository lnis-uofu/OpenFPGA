"""Flat routing in the flow that is run stage by stage (docs/plan_flat_routing.md): route and
analysis run with router.flat_routing, and the queries answer on the routing of the atom nets.

test_equivalence.py checks that the files, the results and the bitstream are those of
'vpr --flat_routing on'; the tests at the end check the outputs that follow the bitstream and the
checkpoints.
"""

import hashlib
import json
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


def test_the_timing_is_what_the_analysis_reports(routed):
    log = routed.logs["analysis"]
    setup = routed.timing_summary().setup
    delay = number(log, r"Final critical path delay \(least slack\): ([-\d.e]+) ns")
    assert math.isclose(setup.critical_path_delay_ns, delay, rel_tol=2e-5)
    assert routed.critical_paths(count=1).paths


def test_a_net_is_routed_inside_the_clusters(routed):
    # 'a' drives the two LUTs of and2_or2, which are packed in one cluster: with flat routing,
    # the route goes to each LUT, inside the cluster
    route = routed.net_route("a")
    assert route.num_sinks == 2
    assert route.num_sinks == sum(1 for node in route.nodes if node.sink_pin is not None)


def test_a_net_is_found_by_the_name_of_its_atom_net(routed):
    # The analysis makes the nets of the clusters again, named '<atom net>_<n>'
    info = routed.net_info("a")
    assert info.name == "a"
    assert info.routing.nodes == len(routed.net_route("a").nodes)
    assert info.timing is not None


def test_the_congestion_counts_the_channels(routed):
    congestion = routed.congestion()
    assert congestion.routed
    assert congestion.summary.chanx.used + congestion.summary.chany.used > 0


def test_a_flat_routing_is_read_from_its_files(routed, openfpga_module, tmp_path):
    saved = tmp_path / "saved"
    saved.mkdir()
    for suffix in ("net", "place", "route"):
        shutil.copy(tmp_path / f"and2_or2.{suffix}", saved / f"and2_or2.{suffix}")
    expected = routed.net_route("a")
    delay = routed.timing_summary().setup.critical_path_delay_ns

    session = openfpga_module.Session()
    read_inputs(session)
    session.read_packing(str(saved / "and2_or2.net"))
    session.read_placement(str(saved / "and2_or2.place"))
    session.read_routing(str(saved / "and2_or2.route"))
    session.analysis()
    assert session.state().reached[-1] == "ANALYZED"
    loaded = session.net_route("a")
    # A tree read from a file does not know its number of sinks: they are counted
    assert loaded.num_sinks == expected.num_sinks
    # The same nodes: the branches may come in another order
    assert sorted(n.rr_node for n in loaded.nodes) == sorted(n.rr_node for n in expected.nodes)
    assert math.isclose(session.timing_summary().setup.critical_path_delay_ns, delay)


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
        net = session.find("*", kind="net").nets.items[0].name
        wanted = (session.timing_summary(), session.critical_paths(), session.net_route(net))
        golden.link(session.shell)
        return net, wanted, golden.bitstream(session.shell)

    net, wanted, bits = in_dir(run, flow)

    def again():
        restored = openfpga_module.Session.restore(checkpoint)
        assert restored.state().reached[-1] == "ANALYZED"
        assert restored.options["router.flat_routing"] is True
        route = restored.net_route(net)
        assert route.flat_routing
        got = (restored.timing_summary(), restored.critical_paths(), route)
        golden.link(restored.shell)
        return got, golden.bitstream(restored.shell)

    got, restored_bits = in_dir(other, again)
    assert got[:2] == wanted[:2]
    # The same tree: the branches of a routing read from a file may come in another order
    assert sorted(n.rr_node for n in got[2].nodes) == sorted(n.rr_node for n in wanted[2].nodes)
    assert got[2].num_sinks == wanted[2].num_sinks
    assert restored_bits == bits


# The model and the device packages -------------------------------------------------------------

PACKAGES = FLOW / "device-packages"


def package_without_flat_routing(tmp_path):
    """A folder with a copy of a device package that does not say it supports flat routing"""
    folder = tmp_path / "packages"
    copy = folder / "k6_no_flat"
    shutil.copytree(PACKAGES / "k6_frac_n10_2x2", copy)
    manifest_path = copy / "device_package.json"
    manifest = json.loads(manifest_path.read_text())
    manifest["name"] = "k6_no_flat"
    del manifest["device"]["flat_routing"]
    manifest_path.write_text(json.dumps(manifest, indent=2))
    sums = copy / "CHECKSUMS.sha256"
    digest = hashlib.sha256(manifest_path.read_bytes()).hexdigest()
    sums.write_text(
        re.sub(
            r"^\w+  device_package.json$",
            digest + "  device_package.json",
            sums.read_text(),
            flags=re.M,
        )
    )
    return folder


def test_the_session_refuses_a_flat_route_that_the_device_does_not_support(routed):
    assert routed.fixed_width_refusal("route") is None
    refusal = routed.fixed_width_refusal("route", flat_routing=False)
    assert "does not support it" in refusal
    routed.options["router.flat_routing"] = False
    assert routed.fixed_width_refusal("route", flat_routing=False) is None


def test_the_model_routes_flat(openfpga_module, tmp_path):
    pytest.importorskip("mcp")
    from openfpga_mcp import build_server
    from test_mcp_tools import Script, make_config, read_inputs

    golden = Golden("k4_flat_routing_test")
    script = Script(build_server(make_config(tmp_path, golden)))
    try:

        async def steps(call):
            sid = (await call("session_open"))["data"]["session"]
            for name, value in (
                ("general.clock_modeling", "route"),
                ("router.route_chan_width", golden.chan_width),
                ("router.flat_routing", True),
            ):
                assert (await call("set_option", session=sid, name=name, value=value))["ok"]
            assert (await read_inputs(call, golden, sid))["ok"]
            analysis = await call("run_stage", session=sid, stage="analysis")
            congestion = await call("get_congestion", session=sid)
            net = await call("get_net", session=sid, net="clk", route=True)
            return analysis, congestion, net

        analysis, congestion, net = script.run(steps)
    finally:
        script.close()
    assert analysis["ok"], analysis
    assert congestion["data"]["flat_routing"] is True
    assert net["data"]["net_route"]["flat_routing"] is True
    assert net["data"]["net_info"]["name"] == "clk"


def test_flat_routing_is_refused_on_a_package_that_does_not_support_it(openfpga_module, tmp_path):
    pytest.importorskip("mcp")
    pytest.importorskip("_tkinter")
    from openfpga_mcp import build_server
    from test_mcp_tools import Script, make_config

    golden = Golden("k4_flat_and2_or2")
    folders = (PACKAGES, package_without_flat_routing(tmp_path))
    config = make_config(tmp_path, golden, package_dirs=folders, tcl_tool=True)
    script = Script(build_server(config))
    try:

        async def steps(call):
            sessions = {}
            for package in ("k6_no_flat", "k6_frac_n10_2x2"):
                sid = (await call("session_open"))["data"]["session"]
                assert (await call("use_device_package", session=sid, name=package))["ok"]
                sessions[package] = sid
            sid = sessions["k6_no_flat"]
            listed = await call("list_device_packages")
            option = await call("set_option", session=sid, name="router.flat_routing", value=True)
            # A script sets the option, and its route is refused
            tcl = await call(
                "run_tcl",
                session=sid,
                script="set_app_option --name router.flat_routing --value true; route",
            )
            stage = await call("run_stage", session=sid, stage="route")
            supported = await call(
                "set_option",
                session=sessions["k6_frac_n10_2x2"],
                name="router.flat_routing",
                value=True,
            )
            return listed, option, tcl, stage, supported

        listed, option, tcl, stage, supported = script.run(steps)
    finally:
        script.close()
    flags = {p["name"]: p["flat_routing"] for p in listed["data"]["packages"]}
    assert flags["k6_no_flat"] is False and flags["k6_frac_n10_2x2"] is True
    assert option["diagnostics"][0]["kind"] == "flat_routing_unsupported", option
    assert "router.flat_routing" in option["diagnostics"][0]["hint"]
    assert tcl["diagnostics"][0]["kind"] == "tcl_refused", tcl
    assert "does not support it" in tcl["diagnostics"][0]["message"]
    assert stage["diagnostics"][0]["kind"] == "flat_routing_unsupported", stage
    assert supported["ok"], supported
