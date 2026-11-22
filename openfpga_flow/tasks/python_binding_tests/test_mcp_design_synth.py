"""synthesize with `design`: the top, files, defines and include folders come from design.json"""

import json
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from openfpga_mcp.config import Config  # noqa: E402
from test_mcp_synth import LOGIC, PACKAGES, Tools, call  # noqa: E402

TOP = """`include "defs.vh"
module chip(input [`WIDTH-1:0] a, output y);
  assign y = ^a;
endmodule
"""


def make_design(folder: Path, name="chip", **manifest):
    (folder / "rtl" / "include").mkdir(parents=True)
    (folder / "rtl" / "chip.v").write_text(TOP)
    (folder / "rtl" / "include" / "defs.vh").write_text("`define WIDTH_DEFAULT 2\n")
    data = {
        "format_version": "1.0",
        "name": name,
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


@pytest.fixture
def server(openfpga_module, tmp_path):
    designs = tmp_path / "designs"
    tools = Tools(
        Config(
            root=tmp_path / "workspace",
            design_dirs=(designs,),
            package_dirs=(PACKAGES,),
            call_timeout_s=120,
        )
    )
    tools.designs = designs
    yield tools
    tools.close()


def opened(server):
    assert call(server, "session_open")["ok"]
    assert call(server, "use_device_package", name=LOGIC)["ok"]


def test_a_design_is_synthesized_with_the_top_define_and_include_folder_of_its_manifest(server):
    make_design(server.designs / "group" / "chip")
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert answer["ok"], answer
    data = answer["data"]
    assert data["top"] == "chip" and data["inputs"] == 5 and data["luts"] >= 1  # WIDTH is 5
    assert "-I" in (server.config.root / server.sid / "synth" / "synth.ys").read_text()


def test_design_and_top_with_files_are_not_given_together(server):
    make_design(server.designs / "chip")
    opened(server)
    answer = call(server, "synthesize", design="chip", top="chip", files="chip.v")
    assert not answer["ok"] and "not both" in json.dumps(answer)
    answer = call(server, "synthesize", top="chip")
    assert not answer["ok"] and "give design, or top and files" in json.dumps(answer)


def test_an_unknown_design_lists_the_ones_there_are(server):
    make_design(server.designs / "chip")
    opened(server)
    answer = call(server, "synthesize", design="nope")
    assert not answer["ok"] and "chip" in json.dumps(answer)


def test_two_designs_of_one_name_are_an_error_that_lists_both_and_a_path_picks_one(server):
    make_design(server.designs / "a" / "chip")
    make_design(server.designs / "b" / "chip")
    opened(server)
    answer = call(server, "synthesize", design="chip")
    text = json.dumps(answer)
    assert not answer["ok"] and "a/chip" in text and "b/chip" in text
    assert call(server, "synthesize", design="b/chip")["ok"]


def test_a_manifest_that_names_a_file_outside_its_folder_is_refused(server):
    make_design(server.designs / "chip", files=[{"role": "rtl", "path": "../outside.v"}])
    (server.designs / "outside.v").write_text("module chip(input a, output y); assign y = a; endmodule\n")
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert not answer["ok"] and "not inside the design" in json.dumps(answer)


def test_a_define_with_characters_that_a_script_does_not_take_is_refused(server):
    make_design(server.designs / "chip", rtl={"include_dirs": ["rtl/include"], "defines": {"WIDTH": "5; shell"}})
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert not answer["ok"] and "characters that a Yosys script does not take" in json.dumps(answer)


NETLIST = ".model chip\n.inputs a b\n.outputs y\n.names a b y\n11 1\n.end\n"


def with_netlist(folder: Path, rtl=True):
    """A design with a netlist; with rtl false, the netlist is all that it has"""
    (folder / "netlist").mkdir(parents=True, exist_ok=True)
    (folder / "netlist" / "chip.blif").write_text(NETLIST)
    data = json.loads((folder / "design.json").read_text())
    data["files"] = (data["files"] if rtl else []) + [{"role": "netlist", "path": "netlist/chip.blif"}]
    (folder / "design.json").write_text(json.dumps(data))
    return folder


def test_a_design_with_only_a_netlist_is_copied_and_its_netlist_is_read(server):
    with_netlist(make_design(server.designs / "chip"), rtl=False)
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert answer["ok"], answer
    data = answer["data"]
    assert data["synthesized"] is False and data["circuit_read"] is True
    assert data["netlist"] == server.sid + "/design/chip/netlist/chip.blif"  # the copy is read
    assert not (server.config.root / server.sid / "synth").exists()
    state = call(server, "get_state")["data"]["state"]
    assert state["inputs"]["circuit"]["file"].endswith("design/chip/netlist/chip.blif")
    # And read_inputs takes it too: the design has nothing else
    assert call(server, "read_inputs", circuit="design/chip/netlist/chip.blif")["ok"]


def test_a_design_with_rtl_is_synthesized_even_when_it_has_a_netlist(server):
    with_netlist(make_design(server.designs / "chip"))
    opened(server)
    data = call(server, "synthesize", design="chip")["data"]
    assert data["netlist"] == server.sid + "/synth/chip.blif" and data["inputs"] == 5  # from the RTL


@pytest.mark.parametrize("name", ["chip.blif", "chip/netlist/chip.blif", "design/chip/netlist/chip.blif"])
def test_the_netlist_of_a_design_with_rtl_is_not_read_as_a_circuit(server, name):
    with_netlist(make_design(server.designs / "chip"))
    opened(server)
    assert call(server, "synthesize", design="chip")["ok"]  # the copy is in the session too
    answer = call(server, "read_inputs", circuit=name)
    assert not answer["ok"], answer
    item = answer["diagnostics"][0]
    assert "has RTL" in item["message"] and "design='chip'" in item["hint"]


def test_a_design_with_only_a_netlist_that_the_flow_cannot_read_says_so(server):
    folder = make_design(server.designs / "chip", files=[{"role": "netlist", "path": "netlist/chip.edif"}])
    (folder / "netlist").mkdir()
    (folder / "netlist" / "chip.edif").write_text("(edif chip)")
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert not answer["ok"] and "does not read" in json.dumps(answer)
    make_design(server.designs / "gone", name="gone", files=[{"role": "netlist", "path": "x.blif"}])
    answer = call(server, "synthesize", design="gone")
    assert not answer["ok"] and "does not exist" in json.dumps(answer)


def test_a_design_name_given_as_a_circuit_says_to_synthesize_it(server):
    make_design(server.designs / "chip")
    opened(server)
    answer = call(server, "read_inputs", circuit="chip")
    assert not answer["ok"]
    assert "design directory" in json.dumps(answer) and "design='chip'" in json.dumps(answer)


def test_a_design_is_copied_into_the_session_and_the_copy_is_synthesized(server):
    source = make_design(server.designs / "group" / "chip")
    (source / ".hidden").write_text("x")
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert answer["ok"], answer
    copy = server.config.root / server.sid / "design" / "chip"
    assert (copy / "design.json").is_file() and (copy / "rtl" / "include" / "defs.vh").is_file()
    assert not (copy / ".hidden").exists()  # names with a dot are left out
    data = answer["data"]
    assert data["design_copy"] == server.sid + "/design/chip"
    assert data["files"] == [server.sid + "/design/chip/rtl/chip.v"]  # the Verilog of the copy is read
    script = (server.config.root / server.sid / "synth" / "synth.ys").read_text()
    assert str(copy / "rtl" / "chip.v") in script and str(source) not in script


def test_a_second_synthesis_copies_the_design_again(server):
    source = make_design(server.designs / "chip")
    opened(server)
    assert call(server, "synthesize", design="chip")["ok"]
    copy = server.config.root / server.sid / "design" / "chip"
    (copy / "stale.v").write_text("// left from before\n")
    (source / "rtl" / "chip.v").write_text(TOP.replace("^a", "&a"))
    assert call(server, "synthesize", design="chip")["ok"]
    assert "&a" in (copy / "rtl" / "chip.v").read_text()  # the copy follows the designs folder
    assert not (copy / "stale.v").exists()


def test_a_design_that_is_refused_is_not_copied(server):
    make_design(server.designs / "chip", files=[{"role": "rtl", "path": "../outside.v"}])
    (server.designs / "outside.v").write_text("module chip(input a, output y); assign y = a; endmodule\n")
    opened(server)
    assert not call(server, "synthesize", design="chip")["ok"]
    assert not (server.config.root / server.sid / "design").exists()


def test_a_design_directory_that_is_too_large_is_not_copied(server, monkeypatch):
    from openfpga_mcp import paths

    make_design(server.designs / "chip")
    monkeypatch.setattr(paths, "DESIGN_COPY_MAX_FILES", 2)
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert not answer["ok"] and "is not copied into the session" in json.dumps(answer)
    assert not (server.config.root / server.sid / "design" / "chip").exists()
