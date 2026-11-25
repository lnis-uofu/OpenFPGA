"""The shell's commands synthesize and read_design (in C++), which the MCP server and the web
app use: the plan of a synthesis from the script of the device package, Yosys, the circuit,
the design directories (RTL, a netlist, a label, the SDC), the refusals, and the journal."""

import json
from pathlib import Path

import pytest

FLOW = Path(__file__).resolve().parents[2]
PACKAGES = FLOW / "device-packages"
SCHEMAS = FLOW.parent / "openfpga_flow" / "schemas"
LOGIC = "k6_frac_n10_2x2"
AND2 = "module and2(input a, input b, output c);\n  assign c = a & b;\nendmodule\n"
CHIP = """`include "defs.vh"
module chip(input [`WIDTH-1:0] a, output y);
  assign y = ^a;
endmodule
"""


@pytest.fixture
def session(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return openfpga_module.Session()


def validate(answer):
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "synthesis.schema.json").read_text())
    jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)


def make_design(folder, **manifest):
    (folder / "rtl" / "include").mkdir(parents=True)
    (folder / "rtl" / "chip.v").write_text(CHIP)
    (folder / "rtl" / "include" / "defs.vh").write_text("`define UNUSED 1\n")
    data = {
        "format_version": "1.0",
        "name": folder.name,
        "license": "MIT",
        "top": "chip",
        "files": [
            {"role": "rtl", "path": "rtl/chip.v"},
            {"role": "rtl_include", "path": "rtl/include/defs.vh"},
        ],
        "rtl": {"include_dirs": ["rtl/include"], "defines": {"WIDTH": "5"}},
    }
    data.update(manifest)
    (folder / "design.json").write_text(json.dumps(data))
    return folder


def test_synthesize_uses_the_package_and_reads_the_circuit(session, tmp_path):
    session.read_device_package(PACKAGES / LOGIC)
    (tmp_path / "and2.v").write_text(AND2)
    answer = session.synthesize("and2", [tmp_path / "and2.v"])
    assert answer["synthesized"] and answer["circuit_read"]
    assert answer["device_package"] == LOGIC and answer["lut_size"] == 6
    assert answer["netlist"] == str(tmp_path / "synth" / "and2.blif")
    assert Path(answer["netlist"]).is_file() and Path(answer["script"]).is_file()
    assert session.state().reached == ["DEVICE"]
    validate(session.shell.run("synthesize --top and2 --files and2.v --no_read").data)


def test_without_a_package_the_lut_size_is_given(session, tmp_path):
    (tmp_path / "and2.v").write_text(AND2)
    with pytest.raises(Exception, match="Synthesis refused: lut_size"):
        session.synthesize("and2", [tmp_path / "and2.v"])
    answer = session.synthesize("and2", [tmp_path / "and2.v"], lut_size=4)
    assert answer["device_package"] is None and answer["synthesized"]
    assert not answer["circuit_read"]  # no architectures: the netlist is only made
    design = make_design(tmp_path / "chip")
    assert session.read_design(design, lut_size=6)["lut_size"] == 6


@pytest.mark.parametrize(
    "text",
    [
        '`include "/etc/passwd"\nmodule m(output y); assign y = 0; endmodule\n',
        'module m(output [7:0] y); reg [7:0] r [0:1]; initial $readmemh("../x.hex", r); '
        "assign y = r[0]; endmodule\n",
    ],
)
def test_a_source_that_takes_a_file_from_outside_is_refused(
    openfpga_module, session, tmp_path, text
):
    (tmp_path / "m.v").write_text(text)
    with pytest.raises(openfpga_module.OpenfpgaError, match="outside the session"):
        session.synthesize("m", [tmp_path / "m.v"], lut_size=4)
    assert not (tmp_path / "synth" / "m.blif").exists()


def test_a_design_with_rtl_is_synthesized_with_its_manifest(session, tmp_path):
    session.read_device_package(PACKAGES / LOGIC)
    design = make_design(tmp_path / "chip")
    checked = session.read_design(design, check=True)
    assert checked["files"] == [str(design / "rtl" / "chip.v")] and checked["design"] == "chip"
    assert not (tmp_path / "synth").exists()  # a check writes nothing
    answer = session.read_design(design)
    validate({"schema_version": 1, "kind": "synthesis", "stage": None, "data": answer})
    assert answer["synthesized"] and answer["circuit_read"] and answer["top"] == "chip"
    script = Path(answer["script"]).read_text()
    assert "-D WIDTH=5" in script and "-I%s" % (design / "rtl" / "include") in script
    # The define of the manifest gives the width of the input
    inputs = [
        l for l in Path(answer["netlist"]).read_text().splitlines() if l.startswith(".inputs")
    ]
    assert inputs and inputs[0].split()[1:] == ["a[%d]" % i for i in range(5)]


def test_a_design_of_a_netlist_or_a_label_reads_it(session, tmp_path):
    session.read_device_package(PACKAGES / LOGIC)
    folder = tmp_path / "gate"
    (folder / "netlist").mkdir(parents=True)
    (folder / "netlist" / "and2.blif").write_text(
        (PACKAGES / LOGIC / "reference" / "and2.blif").read_text()
    )
    (folder / "design.json").write_text(
        json.dumps(
            {
                "format_version": "1.0",
                "name": "gate",
                "license": "MIT",
                "top": "and2",
                "files": [{"role": "netlist", "path": "netlist/and2.blif", "label": "ref"}],
            }
        )
    )
    answer = session.read_design(folder)
    assert answer["synthesized"] is False and answer["circuit_read"]
    assert answer["netlist"] == str(folder / "netlist" / "and2.blif")
    with pytest.raises(Exception, match="has no netlist labelled 'other'"):
        session.read_design(folder, netlist="other")
    assert session.read_design(folder, netlist="ref")["circuit_read"]


@pytest.mark.parametrize(
    "change, why",
    [
        (
            lambda m: m["files"].__setitem__(0, {"role": "rtl", "path": "../x.v"}),
            "not inside the design",
        ),
        (lambda m: m["rtl"].update(defines={"WIDTH": "5; x"}), "characters that a Yosys script"),
        (lambda m: m.pop("top"), "has no top"),
        (
            lambda m: m["files"].__setitem__(0, {"role": "rtl", "path": "rtl/gone.v"}),
            "does not exist",
        ),
    ],
)
def test_a_design_that_breaks_a_rule_is_refused(openfpga_module, session, tmp_path, change, why):
    session.read_device_package(PACKAGES / LOGIC)
    design = make_design(tmp_path / "chip")
    manifest = json.loads((design / "design.json").read_text())
    change(manifest)
    (design / "design.json").write_text(json.dumps(manifest))
    with pytest.raises(openfpga_module.OpenfpgaError, match="Synthesis refused: .*" + why):
        session.read_design(design, check=True)


def test_the_sdc_of_a_design_is_the_one_of_vpr(session, tmp_path):
    session.read_device_package(PACKAGES / LOGIC)
    design = make_design(tmp_path / "chip")
    (design / "constraints").mkdir()
    (design / "constraints" / "chip.sdc").write_text("create_clock -period 5 -name clk\n")
    manifest = json.loads((design / "design.json").read_text())
    manifest["files"].append({"role": "sdc", "path": "constraints/chip.sdc"})
    (design / "design.json").write_text(json.dumps(manifest))
    answer = session.read_design(design)
    assert answer["sdc"] == str(design / "constraints" / "chip.sdc")
    assert session.options["filename.sdc_file"] == answer["sdc"]


def test_the_journal_replays_the_synthesis(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.read_device_package(PACKAGES / LOGIC)
    design = make_design(tmp_path / "chip")
    session.read_design(design)
    session.read_design(design, check=True)  # not in the journal
    lines = [
        l.split()[0]
        for l in (tmp_path / "journal" / "replay.openfpga").read_text().splitlines()
        if l and not l.startswith("#") and l != "exit"
    ]
    assert lines == ["read_device_package", "read_design"]
    replayed = openfpga_module.Session.replay(tmp_path / "journal")
    assert replayed.state().reached == ["DEVICE"]
