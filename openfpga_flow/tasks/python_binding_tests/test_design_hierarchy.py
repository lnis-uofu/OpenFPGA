"""The hierarchy of a design (report_design_hierarchy, Session.design_hierarchy): the RTL
elaborated by a Yosys script of its own, its tree of instances and its modules, and a flow that
it leaves as it is."""

import json
from pathlib import Path

import pytest

FLOW = Path(__file__).resolve().parents[2]
PACKAGES = FLOW / "device-packages"
SCHEMAS = FLOW / "schemas"

RTL = """module leaf(input a, input b, output y);
  assign y = a ^ b;
endmodule
module sub #(parameter W = 4) (input [W-1:0] a, output y);
  wire t;
  leaf u_leaf0 (.a(a[0]), .b(a[1]), .y(t));
  leaf u_leaf1 (.a(t), .b(a[W-1]), .y(y));
endmodule
module top(input [7:0] a, output [2:0] y);
  sub #(.W(8)) u_wide (.a(a), .y(y[0]));
  sub u_narrow0 (.a(a[3:0]), .y(y[1]));
  sub u_narrow1 (.a(a[7:4]), .y(y[2]));
endmodule
"""


def make_design(folder, rtl=RTL, top="top", **manifest):
    (folder / "rtl").mkdir(parents=True)
    (folder / "rtl" / "top.v").write_text(rtl)
    data = {
        "format_version": "1.0",
        "name": folder.name,
        "license": "MIT",
        "top": top,
        "files": [{"role": "rtl", "path": "rtl/top.v"}],
    }
    data.update(manifest)
    (folder / "design.json").write_text(json.dumps(data))
    return folder


def validate(answer):
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "design_hierarchy.schema.json").read_text())
    jsonschema.validate(
        {"schema_version": 1, "kind": "design_hierarchy", "stage": None, "data": answer},
        schema,
        cls=jsonschema.Draft202012Validator,
    )


def shape(node):
    """The tree as (instance, module, its children)"""
    return (node["instance"], node["module"], [shape(c) for c in node["children"]])


@pytest.fixture
def work(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return tmp_path


def test_the_tree_has_the_instances_their_modules_and_their_parameters(openfpga_module, work):
    session = openfpga_module.Session()
    session.read_device_package(PACKAGES / "k6_frac_n10_2x2")
    answer = session.design_hierarchy(make_design(work / "d"))
    validate(answer)
    assert answer["design"] == "d" and answer["top"] == "top"
    leaves = [("u_leaf0", "leaf", []), ("u_leaf1", "leaf", [])]
    assert shape(answer["tree"]) == (
        None,
        "top",
        [("u_narrow0", "sub", leaves), ("u_narrow1", "sub", leaves), ("u_wide", "sub", leaves)],
    )
    wide = answer["tree"]["children"][2]
    assert wide["parameters"] == {"W": 8} and wide["name"].startswith("$paramod")
    assert "parameters" not in answer["tree"]["children"][0]
    assert wide["src"]["line"] == 10 and wide["src"]["file"].endswith("d/rtl/top.v")
    modules = {(m["module"], json.dumps(m["parameters"])): m for m in answer["modules"]}
    assert modules[("leaf", "{}")]["instances"] == 6 and modules[("leaf", "{}")]["cells"] == 1
    assert modules[("sub", "{}")]["instances"] == 2
    assert modules[("sub", '{"W": 8}')]["instances"] == 1
    assert answer["modules"][0]["module"] == "leaf"  # most instances first
    assert answer["nodes"] == 10 and answer["instances"] == 10 and answer["truncated"] is False
    assert Path(answer["json"]).is_file() and Path(answer["script"]).parent == work / "hierarchy"


def test_a_limit_keeps_the_upper_levels(openfpga_module, work):
    answer = openfpga_module.Session().design_hierarchy(
        top="top", files=[make_design(work / "d") / "rtl" / "top.v"], limit=6
    )
    validate(answer)
    assert answer["design"] is None
    assert answer["nodes"] == 6 and answer["instances"] == 10 and answer["truncated"] is True
    children = answer["tree"]["children"]
    assert [len(c["children"]) for c in children] == [2, 0, 0]
    assert [c.get("more") for c in children] == [None, 2, 2]


def test_a_cell_of_the_device_is_a_library_cell_and_a_missing_module_is_unknown(
    openfpga_module, work
):
    rtl = """module top(input [7:0] a, input [7:0] b, output [15:0] y, output z);
  mult_8 u_mul (.A(a), .B(b), .Y(y));
  vendor_ip u_ip (.x(a[0]), .z(z));
endmodule
"""
    session = openfpga_module.Session()
    session.read_device_package(PACKAGES / "k4_n4_dsp16_3x4")
    answer = session.design_hierarchy(make_design(work / "e", rtl))
    validate(answer)
    kinds = {c["instance"]: (c["module"], c["kind"]) for c in answer["tree"]["children"]}
    assert kinds == {"u_ip": ("vendor_ip", "unknown"), "u_mul": ("mult_8", "library")}
    assert "read_verilog -lib -specify " in Path(answer["script"]).read_text()


def test_the_flow_and_the_journal_are_left_as_they_are(openfpga_module, work):
    session = openfpga_module.Session(journal=work / "journal")
    session.read_device_package(PACKAGES / "k6_frac_n10_2x2")
    design = make_design(work / "d")
    session.read_design(design)
    session.pack()
    before = session.state()
    answer = session.design_hierarchy(design)
    assert answer["nodes"] == 10
    after = session.state()
    assert after.reached == before.reached and after.inputs.circuit == before.inputs.circuit
    assert "report_design_hierarchy" not in (work / "journal" / "replay.openfpga").read_text()
    session.place()  # the flow goes on from where it was


def test_what_is_refused(openfpga_module, work):
    session = openfpga_module.Session()
    with pytest.raises(openfpga_module.OpenfpgaError, match="--top and --files"):
        session.design_hierarchy()
    netlist = work / "n"
    netlist.mkdir()
    (netlist / "top.blif").write_text(".model top\n.inputs a\n.outputs y\n.names a y\n1 1\n.end\n")
    (netlist / "design.json").write_text(
        json.dumps(
            {
                "format_version": "1.0",
                "name": "n",
                "license": "MIT",
                "top": "top",
                "files": [{"role": "netlist", "path": "top.blif"}],
            }
        )
    )
    with pytest.raises(openfpga_module.OpenfpgaError, match="no RTL"):
        session.design_hierarchy(netlist)
    with pytest.raises(openfpga_module.OpenfpgaError, match="limit"):
        session.design_hierarchy(make_design(work / "d"), limit=0)
    broken = make_design(work / "b", "module top(input a, output y);\n  assign y = ;\nendmodule\n")
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.design_hierarchy(broken)
