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


def test_a_design_with_only_a_netlist_says_to_read_it(server):
    make_design(server.designs / "chip", files=[{"role": "netlist", "path": "x.blif"}])
    opened(server)
    answer = call(server, "synthesize", design="chip")
    assert not answer["ok"] and "read_inputs" in json.dumps(answer)


def test_a_design_name_given_as_a_circuit_says_to_synthesize_it(server):
    make_design(server.designs / "chip")
    opened(server)
    answer = call(server, "read_inputs", circuit="chip")
    assert not answer["ok"]
    assert "design directory" in json.dumps(answer) and "design='chip'" in json.dumps(answer)
