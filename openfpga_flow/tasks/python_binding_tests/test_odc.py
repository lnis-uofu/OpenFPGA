"""ODC, the constraints file of OpenFPGA (docs/spec_odc.md): read_odc, report_constraints,
write_odc and clear_odc in the session, the files they give to VPR, Yosys and the OpenFPGA
commands, and their checks, on the golden designs and the device packages.

Each constraint is checked by its effect: where a block is placed, what the clock requires of a
path, what the synthesis keeps, what the session drops."""

import csv
import hashlib
import json
import re
import shutil
from pathlib import Path

import pytest

import xml.etree.ElementTree as ET

from test_constraints import paths, run
from test_equivalence import Golden
from test_package_io_map import package_with_map

FLOW = Path(__file__).resolve().parents[2]
PACKAGES = FLOW / "device-packages"
SCHEMAS = FLOW / "schemas"
STAGES = ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]
SDC = """create_clock -period 10 clk
set_input_delay 0 -clock clk [get_ports {IN0 IN1 IN2}]
set_output_delay 0 -clock clk [get_ports {OUT0 OUT1 OUT2}]
"""


@pytest.fixture
def work(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return tmp_path


def golden_session(openfpga_module, name="k6_routing_test", circuit=True, clocks="route"):
    golden = Golden(name)
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = clocks
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    if circuit:
        session.read_circuit(golden.circuit)
    return session


@pytest.fixture
def session(openfpga_module, work):
    return golden_session(openfpga_module)


def odc(path, *lines):
    Path(path).write_text("odc_version 1\n" + "".join(line + "\n" for line in lines))
    return path


def validate(answer):
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "constraints.schema.json").read_text())
    jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)


def where(session, atom):
    """The tile of the cluster that holds an atom: from the packing (.net) and the placement
    (.place) of the session's circuit, in the current directory"""
    circuit = Path(session.state().inputs.circuit.file).stem
    root = ET.parse(circuit + ".net").getroot()
    cluster = next(
        top.get("name")
        for top in root.findall("block")
        if top.get("name") == atom or any(b.get("name") == atom for b in top.iter("block"))
    )
    for line in Path(circuit + ".place").read_text().splitlines():
        words = line.split()
        if words and words[0] == cluster:
            return (int(words[1]), int(words[2]))
    raise AssertionError("%s is not placed" % atom)


def checksums(package):
    """Write the checksums of a package again, after a change"""
    lines = [
        "%s  %s" % (hashlib.sha256(p.read_bytes()).hexdigest(), p.relative_to(package).as_posix())
        for p in sorted(package.rglob("*"))
        if p.is_file() and p.name != "CHECKSUMS.sha256"
    ]
    (package / "CHECKSUMS.sha256").write_text("\n".join(lines) + "\n")


def messages(report, level=None):
    return [d["message"] for d in report["diagnostics"] if level in (None, d["level"])]


# Reading and reporting (R-40, R-41) -----------------------------------------------------------


def test_a_session_without_odc_reports_none(session):
    answer = session.shell.run("report_constraints").data
    validate(answer)
    report = answer["data"]
    assert report["file"] is None and report["commands"] == []
    assert all(path is None for path in report["outputs"].values())


def test_the_report_has_the_matches_the_misses_and_the_coverage(session, work):
    odc(
        work / "c.odc",
        "create_clock -period 10 [get_ports clk]",
        "set_input_delay 1 -clock clk [get_ports {IN0 IN1 INX}]",
        "set_false_path -from [get_ports nothing*]",
    )
    report = session.read_odc(work / "c.odc")
    validate(session.shell.run("report_constraints").data)
    assert report["file"] == str(work / "c.odc")
    assert [c["line"] for c in report["commands"]] == [2, 3, 4]
    assert all(c["checked"] and c["kind"] == "timing" for c in report["commands"])
    delay = report["commands"][1]["queries"][0]
    assert delay["matched"] == ["IN0", "IN1"] and delay["count"] == 2
    warnings = messages(report, "warning")
    assert any("'INX'" in m and "IN0" in m for m in warnings), warnings
    assert any("matches [get_ports nothing*]" in m for m in warnings), warnings
    timing = report["timing"]
    assert [c["name"] for c in timing["clocks"]] == ["clk"] and timing["clocks"][0][
        "period"
    ] == "10"
    assert timing["inputs_without_delay"] == ["IN2"]
    assert timing["outputs_without_delay"] == ["OUT0", "OUT1", "OUT2"]
    assert report["outputs"]["constraints"] == str(work / "odc" / "constraints.sdc")
    assert session.options["filename.sdc_file"] == report["outputs"]["constraints"]
    assert report["outputs"]["vpr_constraints"] is None


def test_a_file_with_an_error_is_not_read_and_the_session_keeps_its_own(
    openfpga_module, session, work
):
    odc(work / "good.odc", "create_clock -period 10 clk")
    session.read_odc(work / "good.odc")
    odc(work / "bad.odc", "create_clock -period 5 clk", "frobnicate 3")
    with pytest.raises(openfpga_module.OpenfpgaError, match=r"bad.odc:3: error: unknown command"):
        session.read_odc(work / "bad.odc")
    assert session.constraints()["file"] == str(work / "good.odc")
    lenient = session.read_odc(work / "bad.odc", lenient=True)
    assert len(lenient["commands"]) == 1
    assert any("frobnicate" in m for m in messages(lenient, "warning"))


def test_write_odc_reads_back_the_same(session, work):
    odc(
        work / "c.odc",
        "create_clock -period 10 [get_ports clk]  # a comment",
        "create_pblock pb",
        "resize_pblock pb -add X1Y1",
        "add_cells_to_pblock pb [get_cells {OUT0 OUT1}]",
        "set_property LOC X1Y1 [get_cells OUT2]",
    )
    first = session.read_odc(work / "c.odc")
    session.write_odc(work / "back.odc")
    text = (work / "back.odc").read_text()
    assert text.startswith("odc_version 1\n") and "comment" not in text
    again = session.read_odc(work / "back.odc")
    assert [c["text"] for c in again["commands"]] == [c["text"] for c in first["commands"]]


# Timing (R-20, R-21, T-4) ---------------------------------------------------------------------


def test_the_timing_of_an_odc_is_the_one_of_the_same_sdc(openfpga_module, work):
    raw = golden_session(openfpga_module)
    (work / "raw.sdc").write_text(SDC)
    raw.read_sdc(work / "raw.sdc")
    run(raw)
    expected = paths(raw)
    by_odc = golden_session(openfpga_module)
    (work / "same.odc").write_text(SDC)  # an SDC is an ODC, with a warning for the header
    report = by_odc.read_odc(work / "same.odc")
    assert any("odc_version 1" in m for m in messages(report, "warning"))
    run(by_odc)
    path = paths(by_odc)
    assert path.data_required_ns == pytest.approx(expected.data_required_ns, abs=1e-9)
    assert path.data_arrival_ns == pytest.approx(expected.data_arrival_ns, abs=1e-9)
    assert path.data_required_ns == pytest.approx(10, abs=1e-6)


# Floorplanning (R-25 to R-27, T-6) ------------------------------------------------------------


def test_a_pblock_a_fixed_cell_and_fixed_ports_are_obeyed(session, work):
    odc(
        work / "c.odc",
        "create_pblock pb",
        "resize_pblock pb -add X2Y2",
        "add_cells_to_pblock pb [get_cells {OUT0 OUT1}]",
        "set_property LOC X1Y1 [get_cells OUT2]",
        "set_property LOC X3Y2/5 [get_ports IN0]",
        "set_property LOC X1Y3/2 [get_ports OUT0]",
    )
    report = session.read_odc(work / "c.odc")
    assert not messages(report, "error")
    xml = Path(report["outputs"]["vpr_constraints"]).read_text()
    assert '<add_atom name_pattern="out:OUT0"/>' in xml and 'subtile="5"' in xml
    run(session, "pack", "place")
    assert where(session, "OUT0") == (2, 2) and where(session, "OUT1") == (2, 2)
    assert where(session, "OUT2") == (1, 1)
    assert where(session, "IN0") == (3, 2) and where(session, "out:OUT0") == (1, 3)
    place = (work / "routing_test.place").read_text()
    assert re.search(r"^IN0\s+3\s+2\s+5\s", place, re.M)
    assert re.search(r"^out:OUT0\s+1\s+3\s+2\s", place, re.M)


@pytest.mark.parametrize(
    "lines, why",
    [
        (["set_property LOC X0Y1 [get_cells OUT1]"], "which cannot hold the cell 'OUT1' (latch)"),
        (["set_property LOC X9Y1 [get_cells OUT1]"], "outside the grid of the device"),
        (["set_property LOC X1Y1/3 [get_cells OUT1]"], "has 1 subtiles"),
        (["set_property LOC X1Y1/0 [get_ports IN0]"], "which holds no port of the design"),
        (
            [
                "create_pblock p",
                "resize_pblock p -add X0Y1",
                "add_cells_to_pblock p [get_cells OUT*]",
            ],
            "no tile of the pblock 'p' can hold the cell",
        ),
        (
            [
                "create_pblock p",
                "resize_pblock p -add X1Y1",
                "add_cells_to_pblock p [get_cells OUT1]",
                "set_property LOC X2Y2 [get_cells OUT1]",
            ],
            "outside its pblock 'p'",
        ),
        (
            ["set_property LOC X3Y2/5 [get_ports IN0]", "set_property LOC X3Y2/5 [get_ports IN1]"],
            "are on the same pin X3Y2/5",
        ),
    ],
)
def test_a_constraint_that_vpr_cannot_keep_is_refused_with_its_line(
    openfpga_module, session, work, lines, why
):
    odc(work / "c.odc", *lines)
    with pytest.raises(openfpga_module.OpenfpgaError, match=re.escape(why)) as error:
        session.read_odc(work / "c.odc")
    assert "c.odc:" in str(error.value)
    assert session.constraints()["file"] is None
    assert session.options["filename.read_vpr_constraints_file"] == ""


def test_a_constraint_is_checked_when_the_netlist_is_read(openfpga_module, work):
    session = golden_session(openfpga_module, circuit=False)
    odc(work / "c.odc", "set_property LOC X0Y1 [get_cells OUT1]")
    report = session.read_odc(work / "c.odc")
    assert not report["commands"][0]["checked"]
    xml = Path(report["outputs"]["vpr_constraints"]).read_text()
    assert 'name_pattern="^OUT1$" is_regex="true"' in xml
    with pytest.raises(openfpga_module.OpenfpgaError, match="cannot hold the cell 'OUT1'"):
        session.read_circuit(Golden("k6_routing_test").circuit)
    assert session.state().inputs.circuit is None
    session.clear_odc()
    session.read_circuit(Golden("k6_routing_test").circuit)
    assert session.state().reached == ["DEVICE"]


# Routing (R-28, R-29, T-7) --------------------------------------------------------------------


def test_an_ideal_clock_is_not_routed(openfpga_module, work):
    routed = golden_session(openfpga_module)
    run(routed, "pack", "place", "route")
    assert routed.net_route("clk").nodes
    session = golden_session(openfpga_module)
    odc(work / "c.odc", "set_global_signal -route_model ideal [get_nets clk]")
    report = session.read_odc(work / "c.odc")
    xml = Path(report["outputs"]["vpr_constraints"]).read_text()
    assert '<set_global_signal name="clk" route_model="ideal"/>' in xml
    run(session, "pack", "place", "route")
    with pytest.raises(openfpga_module.OpenfpgaError, match="has no routing"):
        session.net_route("clk")


@pytest.mark.parametrize(
    "line, clocks, why",
    [
        ("-route_model route", "ideal", "general.clock_modeling is 'ideal'"),
        ("-route_model dedicated_network -network gclk", "route", "no clock network 'gclk'"),
        ("-route_model dedicated_network", "route", "needs -network"),
    ],
)
def test_a_global_signal_that_vpr_cannot_route_is_refused(openfpga_module, work, line, clocks, why):
    session = golden_session(openfpga_module, clocks=clocks)
    odc(work / "c.odc", f"set_global_signal {line} [get_nets clk]")
    with pytest.raises(openfpga_module.OpenfpgaError, match=re.escape(why)):
        session.read_odc(work / "c.odc")


# The session (R-43 to R-45, T-10) -------------------------------------------------------------


def test_a_change_of_the_constraints_drops_the_device_and_clear_odc_too(session, work):
    run(session)
    assert session.state().reached == STAGES
    odc(work / "c.odc", SDC)
    session.read_odc(work / "c.odc")
    assert session.state().reached == []
    run(session)
    # The same file read again changes nothing; a change where it is drops the device
    session.read_odc(work / "c.odc")
    assert session.state().reached == STAGES
    odc(work / "c.odc", SDC.replace("-period 10", "-period 20"))
    session.read_odc(work / "c.odc")
    assert session.state().reached == []
    run(session)
    assert paths(session).data_required_ns == pytest.approx(20, abs=1e-6)
    session.clear_odc()
    assert session.state().reached == []
    assert session.options["filename.sdc_file"] == ""
    assert session.constraints()["file"] is None
    run(session)
    assert paths(session).data_required_ns == 0


def test_a_raw_file_and_the_odc_do_not_set_the_same_thing(openfpga_module, session, work):
    (work / "raw.sdc").write_text(SDC)
    session.read_sdc(work / "raw.sdc")
    odc(work / "c.odc", "create_clock -period 10 clk")
    with pytest.raises(openfpga_module.OpenfpgaError, match="take it away first"):
        session.read_odc(work / "c.odc")
    session.read_sdc(None)
    session.read_odc(work / "c.odc")
    with pytest.raises(openfpga_module.OpenfpgaError, match="use clear_odc first"):
        session.read_sdc(work / "raw.sdc")
    # Floorplanning only: a raw SDC still goes with it
    session.clear_odc()
    session.read_sdc(work / "raw.sdc")
    odc(work / "f.odc", "set_property LOC X1Y1 [get_cells OUT2]")
    session.read_odc(work / "f.odc")
    assert session.options["filename.sdc_file"] == str(work / "raw.sdc")
    assert [c.option for c in session.clear_constraints()] == ["filename.sdc_file"]
    assert session.constraints()["file"] is None


def test_a_checkpoint_restores_the_odc(openfpga_module, session, work):
    odc(work / "c.odc", SDC, "set_property LOC X1Y1 [get_cells OUT2]")
    session.read_odc(work / "c.odc")
    run(session)
    session.checkpoint(work / "cp")
    manifest = json.loads((work / "cp" / "checkpoint.json").read_text())
    assert manifest["odc"]["file"] == str(work / "c.odc")
    assert not any(o["name"].startswith("filename.") for o in manifest["options"])
    restored = openfpga_module.Session.restore(work / "cp")
    assert restored.state().reached == STAGES
    assert restored.constraints()["file"] == str(work / "c.odc")
    assert where(restored, "OUT2") == (1, 1)
    assert paths(restored).data_required_ns == pytest.approx(10, abs=1e-6)
    odc(work / "c.odc", "create_clock -period 5 clk")
    with pytest.raises(Exception, match="is not the one of the checkpoint"):
        openfpga_module.Session.restore(work / "cp")


def test_the_journal_replays_the_odc(openfpga_module, work):
    golden = Golden("k6_routing_test")
    session = openfpga_module.Session(journal=work / "journal")
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    odc(work / "c.odc", "set_property LOC X1Y1 [get_cells OUT2]")
    session.read_odc(work / "c.odc")
    session.read_circuit(golden.circuit)
    session.constraints()  # a query: not in the journal
    run(session, "pack", "place")
    script = (work / "journal" / "replay.openfpga").read_text()
    assert "read_odc" in script and "report_constraints" not in script
    replayed = openfpga_module.Session.replay(work / "journal")
    assert where(replayed, "OUT2") == (1, 1)


# Pins through the I/O map of a device package (R-31 to R-34, R-46, T-8, T-11) ----------------


def pin_rows(openfpga_module, work):
    """An I/O map for k6_frac_n10_2x2, from write_io_map: each I/O subtile a pin P<n>,
    inputs and outputs, and a global pin without a location"""
    probe = openfpga_module.Session()
    probe.read_device_package(PACKAGES / "k6_frac_n10_2x2")
    probe.read_circuit(PACKAGES / "k6_frac_n10_2x2" / "reference" / "and2.blif")
    probe.shell.run(f"write_io_map --file {work / 'first.csv'}")
    with open(work / "first.csv") as f:
        rows = list(csv.DictReader(f))
    for index, row in enumerate(rows):
        row.update(
            pin_direction="inout",
            pin_type="UNDEF" if index == 3 else "GPIO",
            netlist_name=f"gfpga_pad[{index}]",
            customer_pin_alias=f"P{index}",
        )
    rows[1]["pin_direction"] = "output"
    rows.append(
        dict(
            side="",
            pin_direction="input",
            pin_type="CONTROL",
            row="",
            col="",
            subtile="",
            netlist_name="op_reset[0]",
            customer_pin_alias="RST",
        )
    )
    return rows


def at(row):
    return (int(row["col"]), int(row["row"]))


def test_package_pins_place_the_ports_and_write_the_pin_constraints(openfpga_module, work):
    rows = pin_rows(openfpga_module, work)
    package = package_with_map(work / "pkg", rows)
    session = openfpga_module.Session()
    session.read_device_package(package)
    session.read_circuit(package / "reference" / "and2.blif")
    odc(
        work / "c.odc",
        "set_property PACKAGE_PIN P0 [get_ports a]",
        "set_property PACKAGE_PIN gfpga_pad[2] [get_ports b]",
        "set_property PACKAGE_PIN P4 [get_ports c]",
    )
    report = session.read_odc(work / "c.odc")
    assert not messages(report, "error") and report["design"]["io_map"].endswith("io_map.csv")
    run(session, "pack", "place")
    for port, atom, row in (("a", "a", rows[0]), ("b", "b", rows[2]), ("c", "out:c", rows[4])):
        assert where(session, atom) == at(row), port
    place = Path("and2.place").read_text()
    assert re.search(
        r"^out:c\s+%s\s+%s\s+%s\s" % (rows[4]["col"], rows[4]["row"], rows[4]["subtile"]),
        place,
        re.M,
    )
    # A pin without a location: the pin constraints of the OpenFPGA commands
    odc(
        work / "r.odc",
        "set_property PACKAGE_PIN RST [get_ports a]",
        "set_property PIN_DEFAULT 1 [get_ports a]",
    )
    report = session.read_odc(work / "r.odc")
    pins = Path(report["outputs"]["pin_constraints"]).read_text()
    assert '<set_io pin="op_reset[0]" net="a" default_value="1"/>' in pins


@pytest.mark.parametrize(
    "lines, level, why",
    [
        (["set_property PACKAGE_PIN P1 [get_ports a]"], "error", "and the pin 'P1' is an output"),
        (["set_property PACKAGE_PIN P3 [get_ports a]"], "warning", "has the type UNDEF"),
        (
            [
                "set_property PACKAGE_PIN P0 [get_ports a]",
                "set_property PACKAGE_PIN P0 [get_ports b]",
            ],
            "error",
            "are on the same pin",
        ),
        (["set_property PACKAGE_PIN Q7 [get_ports a]"], "error", "the I/O map has no pin 'Q7'"),
    ],
)
def test_a_pin_is_checked_against_its_row(openfpga_module, work, lines, level, why):
    package = package_with_map(work / "pkg", pin_rows(openfpga_module, work))
    session = openfpga_module.Session()
    session.read_device_package(package)
    session.read_circuit(package / "reference" / "and2.blif")
    odc(work / "c.odc", *lines)
    if level == "error":
        with pytest.raises(openfpga_module.OpenfpgaError, match=re.escape(why)):
            session.read_odc(work / "c.odc")
    else:
        assert any(why in m for m in messages(session.read_odc(work / "c.odc"), "warning"))


def test_the_defaults_of_the_package_come_first_and_the_user_wins(openfpga_module, work):
    rows = pin_rows(openfpga_module, work)
    (work / "defaults.odc").write_text(
        "odc_version 1\nset_property PACKAGE_PIN P0 [get_ports a]\n"
        "set_property PACKAGE_PIN P2 [get_ports b]\n"
    )
    package = package_with_map(work / "pkg", rows)
    # The defaults are a file of the package
    shutil.copy(work / "defaults.odc", package / "arch" / "defaults.odc")
    manifest = json.loads((package / "device_package.json").read_text())
    manifest["files"].append({"role": "odc_defaults", "path": "arch/defaults.odc"})
    (package / "device_package.json").write_text(json.dumps(manifest, indent=2))
    checksums(package)
    session = openfpga_module.Session()
    session.read_device_package(package)
    session.read_circuit(package / "reference" / "and2.blif")
    odc(work / "c.odc", "set_property PACKAGE_PIN P5 [get_ports a]")
    report = session.read_odc(work / "c.odc")
    assert report["defaults"] == str(package / "arch" / "defaults.odc")
    assert [c["from_defaults"] for c in report["commands"]] == [True, True, False]
    run(session, "pack", "place")
    assert where(session, "a") == at(rows[5]) and where(session, "b") == at(rows[2])
    session.write_odc(work / "back.odc")
    assert "P0" not in (work / "back.odc").read_text()


# Synthesis (R-22 to R-24, R-47, T-5) ----------------------------------------------------------

KEEP_RTL = """module keep_test(input clk, input rst, input [3:0] a, input [3:0] b,
                 output reg [3:0] count, output y);
  reg [3:0] state_q;
  reg [3:0] shadow_q;
  wire dead_w = ^(a & b);
  assign y = a[0] & b[0] | state_q[0];
  always @(posedge clk) begin
    if (rst) begin count <= 0; state_q <= 0; shadow_q <= 0; end
    else begin count <= count + 1; state_q <= a ^ b; shadow_q <= a + b; end
  end
endmodule
"""


def has_yosys(session):
    return "yosys" in session.shell_commands()


@pytest.mark.parametrize("package", ["k6_frac_n10_2x2", "k4_n4_dsp16_3x4", "k6_frac_n10_mem1k_3x2"])
def test_keep_keeps_what_synthesis_removes(openfpga_module, work, package):
    session = openfpga_module.Session()
    if not has_yosys(session):
        pytest.skip("a build without Yosys")
    session.read_device_package(PACKAGES / package)
    (work / "keep_test.v").write_text(KEEP_RTL)
    plain = Path(session.synthesize("keep_test", [work / "keep_test.v"], read=False)["netlist"])
    plain_text = plain.read_text()
    assert "shadow_q" not in plain_text and "dead_w" not in plain_text
    odc(work / "k.odc", "set_property KEEP true [get_nets {shadow_q dead_w}]")
    session.read_odc(work / "k.odc")
    answer = session.synthesize("keep_test", [work / "keep_test.v"], read=False)
    assert answer["constraints"]
    script = Path(answer["script"]).read_text()
    assert re.search(
        r"hierarchy[^\n]*\n(#[^\n]*\n)*# .*k\.odc:2\nsetattr -set keep 1 w:shadow_q w:dead_w\n",
        script,
    )
    kept = Path(answer["netlist"]).read_text()
    assert "shadow_q" in kept and "dead_w" in kept


def test_synthesis_options_and_a_change_of_them_drop_the_circuit(openfpga_module, work):
    session = openfpga_module.Session()
    if not has_yosys(session):
        pytest.skip("a build without Yosys")
    session.read_device_package(PACKAGES / "k6_frac_n10_2x2")
    (work / "chip.v").write_text(
        "module chip(input [`W-1:0] a, output y); assign y = ^a; endmodule\n"
        "module other(input a, output y); assign y = ~a; endmodule\n"
    )
    odc(work / "s.odc", "set_synth_option -define W=3")
    session.read_odc(work / "s.odc")
    answer = session.synthesize("chip", [work / "chip.v"])
    assert answer["circuit_read"]
    assert Path(answer["netlist"]).read_text().count("a[") >= 3
    odc(work / "s.odc", "set_synth_option -define W=3", "set_synth_option -top other")
    session.read_odc(work / "s.odc")
    assert session.state().inputs.circuit is None
    assert session.synthesize("chip", [work / "chip.v"])["top"] == "other"


def test_a_script_without_the_hook_refuses_synthesis_commands(openfpga_module, work):
    session = openfpga_module.Session()
    session.read_device_package(PACKAGES / "k6_frac_n10_2x2")
    (work / "and2.v").write_text("module and2(input a, b, output c); assign c = a & b; endmodule\n")
    odc(work / "k.odc", "set_property KEEP true [get_nets c]")
    session.read_odc(work / "k.odc")
    assert session.synthesize("and2", [work / "and2.v"], run=False)["constraints"]
    # A package whose script has no ${ODC_SYNTHESIS}
    copy = work / "nohook" / "k6_frac_n10_2x2"
    shutil.copytree(PACKAGES / "k6_frac_n10_2x2", copy)
    script = copy / "synthesis" / "synth_script.ys"
    script.write_text(script.read_text().replace("${ODC_SYNTHESIS}", ""))
    checksums(copy)
    other = openfpga_module.Session()
    other.read_device_package(copy)
    other.read_odc(work / "k.odc")
    with pytest.raises(openfpga_module.OpenfpgaError, match=r"has no line \$\{ODC_SYNTHESIS\}"):
        other.synthesize("and2", [work / "and2.v"], run=False)


def test_read_design_reads_the_odc_of_the_design(openfpga_module, work):
    session = openfpga_module.Session()
    session.read_device_package(PACKAGES / "k6_frac_n10_2x2")
    design = work / "d"
    (design / "constraints").mkdir(parents=True)
    (design / "and2.blif").write_text(
        (PACKAGES / "k6_frac_n10_2x2" / "reference" / "and2.blif").read_text()
    )
    odc(design / "constraints" / "d.odc", "create_clock -period 4 -name vclk")
    manifest = {
        "format_version": "1.0",
        "name": "d",
        "license": "MIT",
        "top": "and2",
        "files": [
            {"role": "netlist", "path": "and2.blif"},
            {"role": "odc", "path": "constraints/d.odc"},
        ],
    }
    (design / "design.json").write_text(json.dumps(manifest))
    answer = session.read_design(design)
    assert answer["odc"] == str(design / "constraints" / "d.odc")
    assert session.constraints()["file"] == answer["odc"]
    assert session.state().inputs.circuit is not None
    manifest["files"].append({"role": "sdc", "path": "constraints/d.odc"})
    (design / "design.json").write_text(json.dumps(manifest))
    with pytest.raises(openfpga_module.OpenfpgaError, match="has an odc file and an sdc file"):
        session.read_design(design, check=True)
