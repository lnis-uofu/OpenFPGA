"""The bitstream of a session (docs/spec_bitstream.md): build_bitstream and report_bitstream, the
settings as inputs, what drops what, the recipe of a device package, checkpoints and journals.

The file that build_bitstream writes is compared with the one of the one-shot flow ('vpr', then
the commands of the bitstream by hand) on the golden designs and on the device packages."""

import hashlib
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

from test_equivalence import GOLDEN, GOLDEN_FLAT, Golden, in_dir
from test_odc import checksums

FLOW = Path(__file__).resolve().parents[2]
PACKAGES = FLOW / "device-packages"
SCHEMAS = FLOW / "schemas"
PACKAGE_NAMES = ["k4_n4_dsp16_3x4", "k6_frac_n10_2x2", "k6_frac_n10_mem1k_3x2"]


@pytest.fixture
def work(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return tmp_path


def sha256(path):
    return hashlib.sha256(Path(path).read_bytes()).hexdigest()


def validate(answer):
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "bitstream.schema.json").read_text())
    jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)


def routed(openfpga_module, golden):
    """A session of a golden design, routed"""
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    if golden.flat:
        session.options["router.flat_routing"] = True
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    for stage in ("pack", "place", "route"):
        getattr(session, stage)()
    return session


def one_shot(openfpga_module, vpr_command, openfpga_arch, steps):
    """The bitstream of the one-shot flow: vpr, then the steps by hand. Its file"""
    shell = openfpga_module.OpenfpgaShell()
    try:
        shell.run(f"read_openfpga_arch -f {openfpga_arch}")
        for line in vpr_command.splitlines():
            shell.run(line)
        for line in steps:
            shell.run(line)
        shell.run("repack")
        shell.run("build_architecture_bitstream --no_time_stamp")
        shell.run("build_fabric_bitstream")
        shell.run("write_fabric_bitstream --file one_shot.bit --format plain_text --no_time_stamp")
    finally:
        shell.reset()
    return Path("one_shot.bit").read_bytes()


# The file is the one of the one-shot flow (BT-1) ------------------------------------------------


@pytest.mark.parametrize("name", sorted(GOLDEN) + sorted(GOLDEN_FLAT))
def test_the_bitstream_is_the_one_of_the_vpr_flow(openfpga_module, work, name):
    golden = Golden(name)
    staged = work / "staged"
    staged.mkdir()

    def make():
        session = routed(openfpga_module, golden)
        return session.bitstream()

    answer = in_dir(staged, make)
    assert answer["file"] == str(staged / "bitstream" / (golden.name + ".bit"))
    assert answer["steps"]["kept"] == [] and answer["bits"] > 0
    validate({"schema_version": 1, "kind": "bitstream", "stage": "BITSTREAM", "data": answer})
    reference = one_shot(
        openfpga_module,
        golden.vpr_command(),
        golden.openfpga_arch,
        [
            f"read_openfpga_simulation_setting -f {golden.sim_setting}",
            "link_openfpga_arch --sort_gsb_chan_node_in_edges",
            "build_fabric --compress_routing",
        ],
    )
    assert Path(answer["file"]).read_bytes() == reference
    assert answer["sha256"] == hashlib.sha256(reference).hexdigest()


# A run of VPR in a process leaves state that changes the placement of a later design of another
# device in the same process (seen on k4_n4_dsp16_3x4 after k6_and2: same packing, another
# placement). The bitstream that a package records is the one of a fresh process: both flows of
# this test run in processes of their own
ONE_SHOT = """
import sys
sys.path.insert(0, sys.argv[1])
import openfpga
shell = openfpga.OpenfpgaShell()
for line in open(sys.argv[2]).read().splitlines():
    shell.run(line)
"""


def one_shot_alone(openfpga_module, lines, where):
    """The one-shot flow in a process of its own, in where. The file of its bitstream"""
    lines = lines + [
        "repack",
        "build_architecture_bitstream --no_time_stamp",
        "build_fabric_bitstream",
        "write_fabric_bitstream --file one_shot.bit --format plain_text --no_time_stamp",
    ]
    (where / "one_shot.openfpga").write_text("\n".join(lines) + "\n")
    subprocess.run(
        [
            sys.executable,
            "-c",
            ONE_SHOT,
            os.path.dirname(openfpga_module.__file__),
            str(where / "one_shot.openfpga"),
        ],
        cwd=where,
        check=True,
        capture_output=True,
    )
    return (where / "one_shot.bit").read_bytes()


@pytest.mark.parametrize("name", PACKAGE_NAMES)
def test_the_bitstream_of_a_package_is_the_one_of_its_vpr_flow(openfpga_module, work, name):
    package = PACKAGES / name
    manifest = json.loads((package / "device_package.json").read_text())
    staged = work / "staged"
    staged.mkdir()
    with openfpga_module.RemoteSession(cwd=staged) as session:
        session.read_device_package(package)
        reference = manifest["reference"]
        session.synthesize(
            reference["top"], [package / reference["design"]], output_dir=staged / "synth"
        )
        for stage in ("pack", "place", "route"):
            getattr(session, stage)()
        answer = session.bitstream()
        state = session.state()
        width = session.options["router.route_chan_width"]
    settings = (state.inputs.simulation_setting or []) + (state.inputs.bitstream_setting or [])
    assert answer["settings"] == settings
    types = {f["type"]: str(package / f["path"]) for f in manifest["files"]}
    assert state.inputs.simulation_setting == [types["simulation_setting"]]
    assert state.inputs.bitstream_setting == (
        [types["bitstream_setting"]] if "bitstream_setting" in types else None
    )
    assert state.results.bitstream == answer["file"]
    vpr_options = " ".join(
        "--%s %s" % (k, v)
        for k, v in manifest["flow_defaults"]["vpr_options"].items()
        if k not in ("device", "route_chan_width")
    )
    recipe = manifest["flow_defaults"].get("bitstream", {})
    # The bitstream does not depend on the simulation settings: fixed ones, as those of a
    # package may need the activities of the signals, which a session does not have
    fixed = FLOW / "openfpga_simulation_settings" / "fixed_sim_openfpga.xml"
    lines = [
        f"vpr {types['vpr_arch']} {state.inputs.circuit.file}"
        f" --device {state.inputs.vpr_arch.layout} --route_chan_width {width} {vpr_options}"
        " --pack --place --route --analysis",
        f"read_openfpga_arch -f {types['openfpga_arch']}",
        f"read_openfpga_simulation_setting -f {fixed}",
    ]
    if "bitstream_setting" in types:
        lines.append(f"read_openfpga_bitstream_setting -f {types['bitstream_setting']}")
    lines.append(
        "link_openfpga_arch "
        + " ".join(recipe.get("link_options", ["--sort_gsb_chan_node_in_edges"]))
    )
    lines += [
        f + (" --fix" if f == "check_netlist_naming_conflict" else "")
        for f in recipe.get("fixups", [])
    ]
    lines.append("build_fabric " + " ".join(recipe.get("fabric_options", ["--compress_routing"])))
    mono = work / "mono"
    mono.mkdir()
    assert Path(answer["file"]).read_bytes() == one_shot_alone(openfpga_module, lines, mono)
    # The package records the bitstream of its reference design in a session (R-15)
    expected = manifest["reference"]["expected"]["bitstreams"]
    assert expected.get("build_bitstream") == answer["sha256"]


# What is kept and what is made again (BT-2) ----------------------------------------------------


def test_the_link_and_the_fabric_are_kept_until_something_drops_them(openfpga_module, work):
    golden = Golden("k6_routing_test")
    session = routed(openfpga_module, golden)
    first = session.bitstream()
    again = session.bitstream()
    assert again["steps"]["kept"] == ["link_openfpga_arch", "build_fabric"]
    assert again["steps"]["ran"][0] == "repack" and again["sha256"] == first["sha256"]

    # The simulation settings drop the link and the fabric, not the bitstream
    session.read_simulation_setting(golden.sim_setting)
    assert "BITSTREAM" in session.state().reached
    assert session.bitstream()["steps"]["kept"] == []

    # The bitstream settings drop REPACKED and BITSTREAM too
    settings = work / "empty_bitstream_setting.xml"
    settings.write_text("<openfpga_bitstream_setting/>\n")
    session.read_bitstream_setting(settings)
    reached = session.state().reached
    assert "REPACKED" not in reached and "BITSTREAM" not in reached
    assert session.state().inputs.bitstream_setting == [str(settings)]
    assert session.state().results.bitstream is None
    answer = session.bitstream()
    assert answer["steps"]["kept"] == [] and str(settings) in answer["settings"]
    assert answer["sha256"] == first["sha256"]  # empty settings change nothing

    # A placement that runs again drops all of it
    session.options["place.seed"] = 7
    assert "BITSTREAM" not in session.state().reached
    session.place()
    session.route()
    moved = session.bitstream()
    assert moved["steps"]["kept"] == [] and moved["sha256"] != first["sha256"]
    # ANALYZED is another branch: it drops nothing of the bitstream
    session.analysis()
    assert "BITSTREAM" in session.state().reached


def test_the_file_the_format_and_the_architecture_bitstream_are_chosen(openfpga_module, work):
    session = routed(openfpga_module, Golden("k6_and2"))
    answer = session.bitstream(file="out/and2.xml", format="xml", arch_file="out/arch.xml")
    assert answer["file"] == str(work / "out" / "and2.xml") and answer["format"] == "xml"
    assert answer["arch_sha256"] == sha256(work / "out" / "arch.xml")
    assert Path(answer["file"]).read_text().lstrip().startswith("<")
    report = session.bitstream_report(limit=1)
    assert len(report["blocks"]) == 1 and report["truncated"] is True
    assert report["sha256"] == sha256(answer["file"]) and report["protocol"]
    with pytest.raises(openfpga_module.OpenfpgaError, match="--format is plain_text or xml"):
        session.bitstream(format="bin")


# Refusals (BT-3) --------------------------------------------------------------------------------


def test_a_bitstream_needs_a_routed_design_of_the_staged_flow(openfpga_module, work):
    golden = Golden("k6_and2")
    session = openfpga_module.Session()
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    session.pack()
    with pytest.raises(openfpga_module.OpenfpgaError, match="ROUTED"):
        session.bitstream()
    with pytest.raises(openfpga_module.OpenfpgaError, match="BITSTREAM"):
        session.bitstream_report()
    shell = openfpga_module.OpenfpgaShell()
    shell.run(f"read_openfpga_arch -f {golden.openfpga_arch}")
    shell.run(golden.vpr_command())
    with pytest.raises(openfpga_module.OpenfpgaError, match="stage by stage"):
        shell.run("build_bitstream")


def package_copy(tmp_path, change):
    """A copy of the logic package whose manifest change() edits, with its checksums"""
    copy = tmp_path / "packages" / "k6_frac_n10_2x2"
    shutil.copytree(PACKAGES / "k6_frac_n10_2x2", copy)
    manifest = json.loads((copy / "device_package.json").read_text())
    change(manifest)
    (copy / "device_package.json").write_text(json.dumps(manifest, indent=2))
    checksums(copy)
    return copy


def test_a_device_that_cannot_make_a_bitstream_refuses_it(openfpga_module, work):
    copy = package_copy(work, lambda m: m["device"].update(bitstream=False))
    session = openfpga_module.Session()
    session.read_device_package(copy)
    assert session.device_package()["bitstream"] is False
    session.read_circuit(copy / "reference" / "and2.blif")
    for stage in ("pack", "place", "route"):
        getattr(session, stage)()
    with pytest.raises(openfpga_module.OpenfpgaError, match="device.bitstream is false"):
        session.bitstream()


@pytest.mark.parametrize(
    "recipe, why",
    [
        ({"link_options": ["--a b"]}, "not '\"--a b\"' in link_options"),
        ({"fixups": ["write_fabric_verilog"]}, "in fixups"),
        ({"fabric_options": "--compress_routing"}, "flow_defaults.bitstream has"),
        ({"steps": []}, "not 'steps'"),
    ],
)
def test_a_package_with_a_bad_recipe_cannot_be_used(openfpga_module, work, recipe, why):
    copy = package_copy(work, lambda m: m["flow_defaults"].update(bitstream=recipe))
    with pytest.raises(openfpga_module.OpenfpgaError, match=why.replace("[", "\\[")):
        openfpga_module.Session().read_device_package(copy)


def test_the_recipe_of_a_package_is_what_runs(openfpga_module, work):
    recipe = {"fixups": ["check_netlist_naming_conflict", "lut_truth_table_fixup"]}
    copy = package_copy(work, lambda m: m["flow_defaults"].update(bitstream=recipe))
    session = openfpga_module.Session()
    session.read_device_package(copy)
    assert session.device_package()["bitstream_recipe"] == {
        "link_options": ["--sort_gsb_chan_node_in_edges"],
        "fixups": recipe["fixups"],
        "fabric_options": ["--compress_routing"],
    }
    session.read_circuit(copy / "reference" / "and2.blif")
    for stage in ("pack", "place", "route"):
        getattr(session, stage)()
    answer = session.bitstream()
    assert answer["steps"]["ran"][:5] == [
        "analysis",
        "link_openfpga_arch",
        "check_netlist_naming_conflict",
        "lut_truth_table_fixup",
        "build_fabric",
    ]


# Simulation settings that infer the number of cycles (BT-4) --------------------------------------


def test_settings_that_need_activities_give_a_bitstream_with_a_warning(openfpga_module, work):
    package = PACKAGES / "k6_frac_n10_2x2"
    session = openfpga_module.Session()
    session.read_device_package(package)
    session.read_circuit(package / "reference" / "and2.blif")
    for stage in ("pack", "place", "route"):
        getattr(session, stage)()
    step = session.command("build_bitstream")
    assert "infer the number of clock cycles from the activity" in step.log
    assert "BITSTREAM" in session.state().reached


# Checkpoints and journals (BT-6) ----------------------------------------------------------------


def test_a_checkpoint_keeps_the_settings_and_the_file_of_the_bitstream(openfpga_module, work):
    golden = Golden("k6_routing_test")
    session = routed(openfpga_module, golden)
    session.read_simulation_setting(golden.sim_setting)
    first = session.bitstream()
    session.checkpoint(work / "checkpoint")
    manifest = json.loads((work / "checkpoint" / "checkpoint.json").read_text())
    assert manifest["settings"]["simulation_setting"] == [
        {"file": str(golden.sim_setting), "sha256": sha256(golden.sim_setting)}
    ]
    saved = manifest["results"]["bitstream"]
    assert saved == {"file": "routing_test.bit", "sha256": first["sha256"]}
    assert "BITSTREAM" not in manifest["stages"]

    os.chdir(work / "checkpoint")  # another directory, as a new process would
    restored = openfpga_module.Session.restore(work / "checkpoint")
    state = restored.state()
    assert state.inputs.simulation_setting == [str(golden.sim_setting)]
    assert "BITSTREAM" not in state.reached
    assert restored.bitstream(file=work / "again.bit")["sha256"] == first["sha256"]


def test_a_journal_makes_the_same_bitstream(openfpga_module, work):
    golden = Golden("k6_routing_test")
    os.chdir(work)
    session = openfpga_module.Session(journal=work / "journal")
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    session.run_flow()
    session.read_simulation_setting(golden.sim_setting)
    first = session.bitstream(file=work / "first.bit")
    script = (work / "journal" / "replay.openfpga").read_text()
    assert "read_openfpga_simulation_setting" in script and "build_bitstream" in script
    assert "report_bitstream" not in script
    Path(first["file"]).unlink()
    replay = work / "replay"
    replay.mkdir()
    os.chdir(replay)
    replayed = openfpga_module.Session.replay(work / "journal")
    assert replayed.bitstream_report()["sha256"] == first["sha256"]


# The constraints of ODC in the bitstream (T-6 and T-8 of docs/spec_odc.md) ---------------------


def test_a_floorplan_of_odc_gives_the_bitstream_of_vpr_with_its_file(openfpga_module, work):
    """T-6: a pblock, a fixed cell and fixed ports; the one-shot vpr reads the XML that the ODC
    wrote"""
    golden = Golden("k6_routing_test")
    (work / "c.odc").write_text(
        "odc_version 1\n"
        "create_pblock pb\n"
        "resize_pblock pb -add X2Y2\n"
        "add_cells_to_pblock pb [get_cells {OUT0 OUT1}]\n"
        "set_property LOC X1Y1 [get_cells OUT2]\n"
        "set_property LOC X3Y2/5 [get_ports IN0]\n"
        "set_property LOC X1Y3/2 [get_ports OUT0]\n"
    )
    staged = work / "staged"
    staged.mkdir()

    def make():
        session = openfpga_module.Session()
        session.options["general.clock_modeling"] = "route"
        session.options["router.route_chan_width"] = golden.chan_width
        session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
        session.read_openfpga_arch(golden.openfpga_arch)
        session.read_circuit(golden.circuit)
        report = session.read_odc(work / "c.odc")
        for stage in ("pack", "place", "route"):
            getattr(session, stage)()
        xml = report["outputs"]["vpr_constraints"]
        return session.bitstream(), Path(xml).read_text()

    answer, xml = in_dir(staged, make)
    (work / "vpr_constraints.xml").write_text(xml)
    reference = one_shot(
        openfpga_module,
        golden.vpr_command(f"--read_vpr_constraints {work / 'vpr_constraints.xml'}"),
        golden.openfpga_arch,
        [
            f"read_openfpga_simulation_setting -f {golden.sim_setting}",
            "link_openfpga_arch --sort_gsb_chan_node_in_edges",
            "build_fabric --compress_routing",
        ],
    )
    assert Path(answer["file"]).read_bytes() == reference


def test_package_pins_of_odc_give_the_bitstream_of_pcf2place(openfpga_module, work):
    """T-8: PACKAGE_PIN through the I/O map of a package of the device of the task example_pcf
    gives the bitstream of its pcf2place flow"""
    from test_package_io_map import TASK, task_rows, write

    package = work / "packages" / "k4_n4_4x4"
    (package / "arch").mkdir(parents=True)
    shutil.copy(FLOW / "vpr_arch" / "k4_N4_tileable_40nm.xml", package / "arch" / "vpr_arch.xml")
    shutil.copy(
        FLOW / "openfpga_arch" / "k4_N4_40nm_cc_openfpga.xml",
        package / "arch" / "openfpga_arch.xml",
    )
    write(package / "arch" / "io_map.csv", task_rows())
    manifest = {
        "format_version": "1.0",
        "name": package.name,
        "version": "0.1.0",
        "description": "The device of basic_tests/io_constraints/example_pcf",
        "license": "MIT",
        "device": {"layout": "4x4", "lut_size": 4, "channel_width": 20},
        "files": [
            {"type": "vpr_arch", "path": "arch/vpr_arch.xml"},
            {"type": "openfpga_arch", "path": "arch/openfpga_arch.xml"},
            {"type": "io_map", "path": "arch/io_map.csv"},
        ],
        "flow_defaults": {"vpr_options": {"clock_modeling": "ideal"}},
    }
    (package / "device_package.json").write_text(json.dumps(manifest, indent=2))
    (package / "LICENSE").write_text("MIT License\n")
    checksums(package)
    pins = {"a": "pad_fpga_io[0]", "b": "pad_fpga_io[4]", "c": "pad_fpga_io[6]"}
    assert all(f"set_io {p} {pins[p]}" in (TASK / "and2.pcf").read_text() for p in pins)
    (work / "pins.odc").write_text(
        "odc_version 1\n"
        + "".join("set_property PACKAGE_PIN %s [get_ports %s]\n" % (pins[p], p) for p in pins)
    )
    circuit = FLOW / "benchmarks" / "ubm" / "simple_gates" / "and2" / "and2.blif"
    staged = work / "staged"
    staged.mkdir()

    def make():
        session = openfpga_module.Session()
        session.read_device_package(package)
        session.read_odc(work / "pins.odc")
        session.read_circuit(circuit)
        for stage in ("pack", "place", "route"):
            getattr(session, stage)()
        return session.bitstream()

    answer = in_dir(staged, make)
    arch = FLOW / "vpr_arch" / "k4_N4_tileable_40nm.xml"
    reference = one_shot(
        openfpga_module,
        f"pcf2place --pcf {TASK / 'and2.pcf'} --blif {circuit}"
        f" --pin_table {TASK / 'pinmap_k4_N4_tileable_40nm.csv'}"
        f" --fpga_io_map {TASK / 'fpga_io_location.xml'} --fpga_fix_pins fix_pins.place"
        " --pin_table_direction_convention explicit --reduce_error_to_warning\n"
        f"vpr {arch} {circuit} --clock_modeling ideal --device 4x4 --route_chan_width 20"
        " --fix_clusters fix_pins.place --pack --place --route --analysis",
        FLOW / "openfpga_arch" / "k4_N4_40nm_cc_openfpga.xml",
        [
            f"read_openfpga_simulation_setting -f {Golden('k4_and2_or2').sim_setting}",
            "link_openfpga_arch --sort_gsb_chan_node_in_edges",
            "build_fabric --compress_routing",
        ],
    )
    assert Path(answer["file"]).read_bytes() == reference
