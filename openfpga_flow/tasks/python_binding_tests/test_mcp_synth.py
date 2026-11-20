"""The `synthesize` tool: Verilog to a netlist of the LUTs of the device, with Yosys inside the worker"""

import json
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from openfpga_mcp import build_server, device_packages, synthesis  # noqa: E402
from openfpga_mcp.config import Config  # noqa: E402
from openfpga_mcp.errors import InvalidArgument  # noqa: E402
from test_mcp_tools import Script, golden  # noqa: E402,F401

PACKAGES = Path(__file__).resolve().parents[2] / "device-packages"
LOGIC = "k6_frac_n10_2x2"

AND2 = "module and2(input a, input b, output y);\n  assign y = a & b;\nendmodule\n"
COUNTER = """module counter(input clk, input rst, output reg [3:0] q);
  always @(posedge clk) begin
    if (rst) q <= 4'd0; else q <= q + 4'd1;
  end
endmodule
"""
WIDE = "module wide(input [4:0] a, output y);\n  assign y = ^a;\nendmodule\n"
BROKEN = "module broken(input a, output y);\n  assign y = a &;\nendmodule\n"


class Tools:
    """The tools of a server, called through a client. The session that was opened last is the
    one of the calls that name no session"""

    def __init__(self, config):
        self.config = config
        self.script = Script(build_server(config))
        self.sid = None

    def invoke(self, tool, arguments):
        answer = self.script.run(lambda call: call(tool, **arguments))
        if tool in ("session_open", "replay") and answer.get("ok"):
            self.sid = answer["data"]["session"]
        return answer

    def close(self):
        self.script.close()


@pytest.fixture
def server(openfpga_module, tmp_path, golden):
    tools = Tools(
        Config(
            root=tmp_path / "workspace",
            design_dirs=(golden.circuit.parent,),
            package_dirs=(PACKAGES,),
            call_timeout_s=120,
        )
    )
    yield tools
    tools.close()


def call(server, tool, /, **arguments):
    if tool not in ("session_open", "session_list", "replay", "list_device_packages"):
        arguments.setdefault("session", server.sid)
    return server.invoke(tool, arguments)


def opened(server, package=LOGIC):
    assert call(server, "session_open")["ok"]
    if package:
        assert call(server, "use_device_package", name=package)["ok"]
    return server.sid


def write(server, name, text):
    answer = call(server, "write_file", name=name, content=text)
    assert answer["ok"], answer


def test_a_design_is_mapped_to_luts_and_read_as_the_circuit_and_the_flow_goes_on(server):
    sid = opened(server)
    write(server, "and2.v", AND2)
    answer = call(server, "synthesize", top="and2", files="and2.v")
    assert answer["ok"], answer
    data = answer["data"]
    assert data["luts"] == 1 and data["flip_flops"] == 0 and data["widest_lut"] == 2
    assert data["inputs"] == 2 and data["outputs"] == 1 and data["lut_size"] == 6
    assert data["device_package"] == LOGIC and data["circuit_read"] is True
    assert data["netlist"] == sid + "/synth/and2.blif"  # a name that can be given back
    assert (server.config.root / sid / "synth" / "and2.blif").is_file()
    assert server.config.root.joinpath(sid, "synth", "synth.ys").read_text().startswith("# Yosys")
    assert call(server, "run_stage", stage="pack")["ok"]
    assert call(server, "run_stage", stage="route")["ok"]


def test_flip_flops_are_counted_and_the_clocked_design_goes_through_the_flow(server):
    opened(server)
    write(server, "counter.v", COUNTER)
    data = call(server, "synthesize", top="counter", files="counter.v")["data"]
    assert data["flip_flops"] == 4 and data["luts"] >= 4 and data["circuit_read"] is True
    assert call(server, "run_stage", stage="place")["ok"]


def test_a_syntax_error_has_the_file_and_the_line_and_the_next_call_works(server):
    opened(server)
    write(server, "broken.v", BROKEN)
    write(server, "and2.v", AND2)
    failed = call(server, "synthesize", top="broken", files="broken.v")
    assert failed["ok"] is False
    item = failed["diagnostics"][0]
    assert item["kind"] == "yosys_error" and "syntax error" in item["message"]
    assert item["file"] == "broken.v" and item["line"] == 2
    assert item["data"]["errors"][0]["line"] == 2 and item["data"]["log_tail"]
    # The worker is the same: Yosys was not shut down, and a good design goes through
    assert call(server, "synthesize", top="and2", files="and2.v")["ok"]
    # The fixed file is synthesized too
    write_over = call(server, "write_file", name="broken.v", content=AND2.replace("and2", "broken"), overwrite=True)
    assert write_over["ok"]
    assert call(server, "synthesize", top="broken", files="broken.v")["data"]["luts"] == 1


def test_a_top_that_is_not_in_the_files_lists_the_modules_that_are(server):
    opened(server)
    write(server, "and2.v", AND2)
    item = call(server, "synthesize", top="nothere", files="and2.v")["diagnostics"][0]
    assert item["kind"] == "yosys_error" and "nothere" in item["message"]
    assert "and2" in item["hint"]


@pytest.mark.parametrize(
    "text",
    [
        '`include "/etc/passwd"\nmodule m(output y); assign y = 0; endmodule\n',
        '`include "../secret.vh"\nmodule m(output y); assign y = 0; endmodule\n',
        'module m(output [7:0] y); reg [7:0] r [0:1]; initial $readmemh("/etc/passwd", r); assign y = r[0]; endmodule\n',
    ],
)
def test_a_source_that_takes_a_file_from_outside_the_session_is_refused(server, text):
    opened(server)
    write(server, "m.v", text)
    item = call(server, "synthesize", top="m", files="m.v")["diagnostics"][0]
    assert item["kind"] == "invalid_argument" and "outside the session" in item["message"]
    assert not (server.config.root / server.sid / "synth" / "m.blif").exists()


@pytest.mark.parametrize(
    "arguments,message",
    [
        ({"top": "and2; shell id", "files": "and2.v"}, "name of the top module"),
        ({"top": "and2", "files": ""}, "lists the Verilog files"),
        ({"top": "and2", "files": "notes.txt"}, "not a Verilog file"),
        ({"top": "and2", "files": ",".join("f%d.v" % i for i in range(25))}, "There is no file"),
    ],
)
def test_the_arguments_are_checked_before_anything_runs(server, arguments, message):
    opened(server)
    write(server, "and2.v", AND2)
    write(server, "notes.txt", "x")
    item = call(server, "synthesize", **arguments)["diagnostics"][0]
    assert message in item["message"]
    assert not (server.config.root / server.sid / "synth").exists() or not list(
        (server.config.root / server.sid / "synth").glob("*.blif")
    )


def test_with_no_device_package_the_lut_size_is_given_and_used(server):
    opened(server, package=None)
    write(server, "wide.v", WIDE)
    missing = call(server, "synthesize", top="wide", files="wide.v")["diagnostics"][0]
    assert missing["kind"] == "invalid_argument" and "lut_size" in missing["message"]
    data = call(server, "synthesize", top="wide", files="wide.v", lut_size=4)["data"]
    assert data["widest_lut"] <= 4 and data["luts"] >= 2 and data["device_package"] is None
    assert data["circuit_read"] is False and "use_device_package" in data["note"]
    # With 6-input LUTs the 5-input parity is one LUT
    assert call(server, "synthesize", top="wide", files="wide.v", lut_size=6)["data"]["luts"] == 1


def test_the_script_of_every_package_gets_its_values(tmp_path):
    cfg = Config(root=tmp_path, package_dirs=(PACKAGES,))
    source = tmp_path / "d.v"
    source.write_text(AND2)
    for name in ("k6_frac_n10_2x2", "k4_n4_dsp16_3x4", "k6_frac_n10_mem1k_3x2"):
        package = device_packages.find(cfg, name)
        plan = synthesis.make_plan("and2", [source], tmp_path / "synth", package, None)
        assert "${" not in plan.script and plan.lut_size == package.lut_size
        assert "read_verilog" in plan.script and str(source) in plan.script
        assert str(plan.blif) in plan.script and "write_blif" in plan.script
        # A device that reads extended BLIF gets a netlist with that extension
        assert plan.blif.suffix == (".eblif" if name == "k4_n4_dsp16_3x4" else ".blif")
    dsp = synthesis.make_plan("and2", [source], tmp_path, device_packages.find(cfg, "k4_n4_dsp16_3x4"), None)
    assert "-D DSP_A_MAXWIDTH=8" in dsp.script and "-nolatches" in dsp.script


def test_systemverilog_files_get_the_sv_option_and_a_value_with_a_space_is_refused(tmp_path):
    good = tmp_path / "d.sv"
    good.write_text(AND2)
    plan = synthesis.make_plan("and2", [good], tmp_path, None, 6)
    assert "read_verilog -sv" in plan.script
    odd = tmp_path / "my design.v"
    odd.write_text(AND2)
    with pytest.raises(InvalidArgument):
        synthesis.make_plan("and2", [odd], tmp_path, None, 6)


def test_the_log_and_the_netlist_are_read(tmp_path):
    log = "x.v:2: ERROR: syntax error\nParsing...\nx.v:2: ERROR: syntax error\nERROR: other thing\n"
    assert synthesis.log_errors(log) == [
        {"file": "x.v", "line": 2, "message": "syntax error"},
        {"file": None, "line": None, "message": "other thing"},
    ]
    blif = tmp_path / "n.blif"
    blif.write_text(
        ".model m\n.inputs a b \\\n c\n.outputs y\n.names $true\n1\n.names a b c y\n111 1\n"
        ".latch y q 0\n.names q y2\n1 1\n.end\n"
    )
    stats = synthesis.blif_stats(blif)
    assert stats == {
        "model": "m",
        "luts": 2,
        "flip_flops": 1,
        "widest_lut": 3,
        "constants": 1,
        "inputs": 3,
        "outputs": 1,
        "subckts": {},
    }


def test_a_file_of_the_wrong_kind_is_named_before_an_architecture_that_is_missing(server):
    # The person named a file as the VPR architecture and no other architecture: what is wrong with
    # that file is the answer, not that the OpenFPGA architecture is needed
    assert call(server, "session_open")["ok"]
    write(server, "oarch.xml", "<openfpga_architecture>\n</openfpga_architecture>\n")
    write(server, "c.blif", ".model m\n.inputs a\n.outputs y\n.names a y\n1 1\n.end\n")
    item = call(server, "read_inputs", circuit="c.blif", vpr_arch="oarch.xml", layout="2x2")["diagnostics"][0]
    assert item["kind"] == "wrong_file_kind" and "oarch.xml" in item["message"]
    # With the right files it is the missing architecture that is said
    write(server, "varch.xml", "<architecture>\n</architecture>\n")
    item = call(server, "read_inputs", circuit="c.blif", vpr_arch="varch.xml", layout="2x2")["diagnostics"][0]
    assert item["kind"] == "invalid_argument" and "openfpga_arch is needed" in item["message"]


MULTIPLIER = "module mul(input [7:0] a, input [7:0] b, output [15:0] y);\n  assign y = a * b;\nendmodule\n"


def test_a_device_with_extended_blif_gets_an_eblif_netlist_that_the_pack_stage_reads(server):
    # The DSP device reads extended BLIF (.param): a netlist named .blif is refused by the reader
    sid = opened(server, "k4_n4_dsp16_3x4")
    write(server, "mul.v", MULTIPLIER)
    data = call(server, "synthesize", top="mul", files="mul.v")["data"]
    assert data["netlist"] == sid + "/synth/mul.eblif" and data["circuit_read"] is True
    assert data["subckts"] == {"mult_8": 1} and data["luts"] == 0  # a multiplier is a cell, not LUTs
    assert call(server, "run_stage", stage="pack")["ok"]  # the synthesized circuit is what is packed
    state = call(server, "get_state")["data"]["state"]
    assert state["inputs"]["circuit"]["file"].endswith("synth/mul.eblif")


def test_a_netlist_that_the_flow_cannot_read_is_a_failure_that_says_what_it_has(server):
    # A multiplier on a device with no DSP stays a cell that the device does not know
    opened(server, "k6_frac_n10_mem1k_3x2")
    write(server, "mul.v", MULTIPLIER)
    answer = call(server, "synthesize", top="mul", files="mul.v")
    assert answer["ok"] is False
    item = answer["diagnostics"][0]
    assert item["kind"] == "circuit_not_read" and "no such model" in item["message"]
    assert "no primitive" in item["hint"] and "$macc_v2" in item["hint"]
    assert item["data"]["subckts"] and item["data"]["netlist"].endswith("synth/mul.blif")
    # The pack stage has no circuit, and the answer of synthesize said so
    assert call(server, "run_stage", stage="pack")["ok"] is False


def test_the_synthesized_circuit_replaces_the_one_that_was_read(server):
    # The session has a circuit and its stages: the synthesized netlist is the circuit that is packed
    sid = opened(server)
    write(server, "and2.v", AND2)
    write(server, "counter.v", COUNTER)
    assert call(server, "synthesize", top="and2", files="and2.v")["ok"]
    assert call(server, "run_stage", stage="pack")["ok"]
    assert call(server, "synthesize", top="counter", files="counter.v")["data"]["circuit_read"]
    state = call(server, "get_state")["data"]["state"]
    assert state["inputs"]["circuit"]["file"].endswith("synth/counter.blif")
    assert "PACKED" not in state["reached"]  # the stages of the old circuit are dropped
    assert call(server, "run_stage", stage="pack")["ok"]
    assert (server.config.root / sid / "counter.net").is_file()  # packed from the new netlist


def test_a_session_keeps_one_script_whatever_the_top_modules_that_were_synthesized(server):
    sid = opened(server)
    write(server, "and2.v", AND2)
    write(server, "counter.v", COUNTER)
    folder = server.config.root / sid / "synth"
    folder.mkdir(parents=True, exist_ok=True)
    (folder / "old_top.ys").write_text("# a script of an earlier version\n")
    assert call(server, "synthesize", top="and2", files="and2.v")["ok"]
    first = (folder / "synth.ys").read_text()
    assert [p.name for p in folder.glob("*.ys")] == ["synth.ys"]  # the old one is gone
    assert call(server, "synthesize", top="counter", files="counter.v")["ok"]
    assert [p.name for p in folder.glob("*.ys")] == ["synth.ys"]
    second = (folder / "synth.ys").read_text()
    assert "counter" in second and "counter" not in first  # the script is the one of the last run
    # Each top keeps its own netlist
    assert sorted(p.name for p in folder.glob("*.blif")) == ["and2.blif", "counter.blif"]
