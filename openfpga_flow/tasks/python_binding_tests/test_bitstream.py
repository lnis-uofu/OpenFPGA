"""The bitstream of a session: build_bitstream and report_bitstream, the settings as inputs, what
drops what, checkpoints and journals.

The file that build_bitstream writes is compared with the one of the one-shot flow ('vpr', then
the commands of the bitstream by hand) on the golden designs. The recipe that a plugin sets
(BitstreamRecipe) is tested in C++ (test_openfpga_session.cpp)."""

import hashlib
import json
import os
from pathlib import Path

import pytest

from test_equivalence import GOLDEN, GOLDEN_FLAT, Golden, in_dir

FLOW = Path(__file__).resolve().parents[2]
SCHEMAS = FLOW / "schemas"


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


# The file is the one of the one-shot flow ------------------------------------------------


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


# What is kept and what is made again ----------------------------------------------------


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


# Refusals --------------------------------------------------------------------------------


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


def test_without_simulation_settings_fixed_ones_are_read(openfpga_module, work):
    session = routed(openfpga_module, Golden("k6_and2"))
    step = session.command("build_bitstream")
    assert "No simulation settings were read" in step.log
    assert (work / "bitstream" / "simulation_setting.xml").is_file()
    assert session.state().inputs.simulation_setting == [
        str(work / "bitstream" / "simulation_setting.xml")
    ]
    validate(session.shell.run("report_bitstream").data)


# Checkpoints and journals ----------------------------------------------------------------


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
