"""What a session knows of its flow (report_session), the checkpoints that save it, and the
sessions that are restored from them, in this process and in another one."""

import dataclasses
import json
import os
import subprocess
import sys
from pathlib import Path

import pytest

from test_equivalence import Golden, in_dir

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"
GOLDEN = "k6_routing_test"


def start(openfpga_module, golden, **session_options):
    session = openfpga_module.Session(**session_options)
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    return session


@pytest.fixture
def golden():
    return Golden(GOLDEN)


@pytest.fixture
def work(tmp_path, monkeypatch):
    """The directory the flow runs in, and the one of the checkpoint"""
    run = tmp_path / "run"
    run.mkdir()
    monkeypatch.chdir(run)
    return run, tmp_path / "checkpoint"


# The state ----------------------------------------------------------------------------


def test_the_state_follows_the_flow(openfpga_module, golden, work):
    run, _ = work
    session = openfpga_module.Session()
    state = session.state()
    assert state.reached == [] and state.inputs.vpr_arch is None
    assert state.results == openfpga_module.SessionResults(None, None, None)

    session = start(openfpga_module, golden)
    state = session.state()
    assert state.reached == ["DEVICE"]
    assert state.inputs.vpr_arch == openfpga_module.VprArchInput(
        str(golden.vpr_arch), golden.layout
    )
    assert state.inputs.openfpga_arch.file == str(golden.openfpga_arch)
    assert state.inputs.circuit.file == str(golden.circuit)

    expected = [("pack", "PACKED"), ("place", "PLACED"), ("route", "ROUTED")]
    for index, (stage, name) in enumerate(expected):
        getattr(session, stage)()
        state = session.state()
        assert state.reached[-1] == name
        files = [state.results.packing, state.results.placement, state.results.routing]
        assert [f is not None for f in files] == [i <= index for i in range(3)]
        for path in filter(None, files):
            assert Path(path).is_file() and Path(path).parent == run
    session.analysis()
    assert session.state().reached == ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]


def test_the_state_drops_what_an_option_changed(openfpga_module, golden, work):
    session = start(openfpga_module, golden)
    session.run_flow()
    session.options["place.seed"] = 3
    state = session.state()
    assert state.reached == ["DEVICE", "PACKED"]
    assert state.results.placement is None and state.results.routing is None


def test_the_inputs_are_whole_paths(openfpga_module, golden, work, tmp_path):
    local = tmp_path / "local"
    local.mkdir()
    for name in ("vpr_arch", "openfpga_arch", "circuit"):
        (local / f"{name}.blif" if name == "circuit" else local / name).write_bytes(
            getattr(golden, name).read_bytes()
        )
    session = openfpga_module.Session()
    session.options["router.route_chan_width"] = golden.chan_width
    in_dir(local, session.read_vpr_arch, "vpr_arch", golden.layout)
    in_dir(local, session.read_openfpga_arch, "openfpga_arch")
    in_dir(local, session.read_circuit, "circuit.blif")
    inputs = session.state().inputs
    assert inputs.vpr_arch.file == str(local / "vpr_arch")
    assert inputs.circuit.file == str(local / "circuit.blif")


def test_a_failed_input_is_not_in_the_state(openfpga_module, golden, work, tmp_path):
    session = start(openfpga_module, golden)
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.read_circuit(tmp_path / "missing.blif")
    state = session.state()
    assert state.inputs.circuit is None and state.reached == []
    assert state.inputs.vpr_arch is not None


def test_the_answer_follows_its_schema(openfpga_module, golden, work):
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "session.schema.json").read_text())
    jsonschema.Draft202012Validator.check_schema(schema)
    session = openfpga_module.Session()
    answers = [session.shell.run("report_session").data]
    session = start(openfpga_module, golden)
    answers.append(session.shell.run("report_session").data)
    session.run_flow()
    answers.append(session.shell.run("report_session").data)
    for answer in answers:
        jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)


# The checkpoint -----------------------------------------------------------------------


def test_a_checkpoint_has_the_stages_the_options_and_the_files(openfpga_module, golden, work):
    _, checkpoint = work
    session = start(openfpga_module, golden)
    session.options["place.seed"] = 7
    session.pack()
    session.place()
    assert session.checkpoint(checkpoint) == str(checkpoint)
    manifest = json.loads((checkpoint / "checkpoint.json").read_text())
    assert manifest["stages"] == ["DEVICE", "PACKED", "PLACED"]
    assert sorted(manifest["results"]) == ["packing", "placement"]
    assert sorted(p.name for p in checkpoint.iterdir()) == [
        "checkpoint.json",
        f"{golden.name}.net",
        f"{golden.name}.place",
    ]
    names = {option["name"]: option["value"] for option in manifest["options"]}
    # The options that the inputs set are not kept: restoring reads the inputs
    assert names == {
        "general.clock_modeling": "route",
        "router.route_chan_width": golden.chan_width,
        "place.seed": 7,
    }
    assert manifest["inputs"]["circuit"]["file"] == str(golden.circuit)


def test_nothing_is_saved_before_the_inputs_are_read(openfpga_module, work):
    _, checkpoint = work
    with pytest.raises(openfpga_module.CheckpointError, match="inputs"):
        openfpga_module.Session().checkpoint(checkpoint)
    assert not checkpoint.exists()


def test_a_new_checkpoint_replaces_the_old_one(openfpga_module, golden, work):
    _, checkpoint = work
    session = start(openfpga_module, golden)
    session.run_flow()
    session.checkpoint(checkpoint)
    assert (checkpoint / f"{golden.name}.route").exists()
    session.options["place.seed"] = 4
    session.place()
    session.checkpoint(checkpoint)
    assert not (checkpoint / f"{golden.name}.route").exists()
    manifest = json.loads((checkpoint / "checkpoint.json").read_text())
    assert manifest["stages"] == ["DEVICE", "PACKED", "PLACED"]


def test_a_checkpoint_after_each_stage(openfpga_module, golden, work):
    _, checkpoint = work
    session = start(openfpga_module, golden, checkpoint_dir=checkpoint)
    assert not checkpoint.exists()
    seen = []
    for stage in (session.pack, session.place, session.route, session.analysis):
        stage()
        seen.append(json.loads((checkpoint / "checkpoint.json").read_text())["stages"][-1])
    assert seen == ["PACKED", "PLACED", "ROUTED", "ANALYZED"]


# Restore ------------------------------------------------------------------------------


def answers(session):
    return (session.timing_summary(), session.utilization(), session.placement())


@pytest.mark.parametrize("stages", [1, 2, 3, 4])
def test_a_restored_session_is_where_the_checkpoint_was(openfpga_module, golden, work, stages):
    _, checkpoint = work
    session = start(openfpga_module, golden)
    session.options["place.seed"] = 5
    for stage in (session.pack, session.place, session.route, session.analysis)[:stages]:
        stage()
    session.checkpoint(checkpoint)
    # The data of VPR is one for the process: ask the session before another one reads its inputs
    wanted = {
        "reached": session.state().reached,
        "changed": [o.name for o in session.options.changed()],
        "utilization": session.utilization(),
    }
    if stages >= 2:
        wanted["placement"] = session.placement()
    if stages >= 3:
        wanted["timing"] = session.timing_summary()
        wanted["paths"] = session.critical_paths()

    restored = openfpga_module.Session.restore(checkpoint)
    assert restored.state().reached == wanted["reached"]
    assert restored.options["place.seed"] == 5
    assert [o.name for o in restored.options.changed()] == wanted["changed"]
    # The queries of the stages that are reached say the same
    assert restored.utilization() == wanted["utilization"]
    if stages >= 2:
        assert restored.placement() == wanted["placement"]
    if stages >= 3:
        assert restored.timing_summary() == wanted["timing"]
        assert restored.critical_paths() == wanted["paths"]


def without_header(path):
    """A file of VPR without its first line, which says where the file it follows is"""
    return Path(path).read_bytes().split(b"\n", 1)[1]


def test_the_flow_goes_on_after_a_restore(openfpga_module, golden, work):
    """A restored session routes to the same file as the session that went on"""
    run, checkpoint = work
    session = start(openfpga_module, golden)
    session.pack()
    session.place()
    session.checkpoint(checkpoint)
    session.route()
    wanted = (without_header(f"{golden.name}.route"), session.timing_summary())

    (run / f"{golden.name}.route").unlink()
    restored = openfpga_module.Session.restore(checkpoint)
    restored.route()
    assert without_header(f"{golden.name}.route") == wanted[0]
    assert restored.timing_summary() == wanted[1]


def test_the_bitstream_of_a_restored_session_is_the_same(openfpga_module, golden, work):
    run, checkpoint = work
    session = start(openfpga_module, golden)
    session.run_flow()
    session.checkpoint(checkpoint)
    golden.link(session.shell)
    wanted = golden.bitstream(session.shell)

    other = run.parent / "other"
    other.mkdir()

    def again():
        restored = openfpga_module.Session.restore(checkpoint)
        golden.link(restored.shell)
        return golden.bitstream(restored.shell)

    assert in_dir(other, again) == wanted


FRESH_PROCESS = """
import json, sys
import openfpga
session = openfpga.Session.restore(sys.argv[1])
state = session.state()
session.route()
timing = session.timing_summary()
json.dump({"reached": state.reached, "timing": openfpga.dataclasses.asdict(timing)}, sys.stdout)
"""


def test_a_session_is_restored_in_another_process(openfpga_module, golden, work):
    run, checkpoint = work
    session = start(openfpga_module, golden)
    session.options["place.seed"] = 11
    session.pack()
    session.place()
    session.checkpoint(checkpoint)
    session.route()
    wanted = dataclasses.asdict(session.timing_summary())
    wanted_route = without_header(f"{golden.name}.route")

    other = run.parent / "other"
    other.mkdir()
    module_dir = os.path.dirname(openfpga_module.__file__)
    env = dict(os.environ, PYTHONPATH=module_dir)
    result = subprocess.run(
        [sys.executable, "-c", FRESH_PROCESS, str(checkpoint)],
        cwd=other,
        env=env,
        capture_output=True,
        text=True,
    )
    assert result.returncode == 0, result.stderr[-2000:]
    answer = json.loads(result.stdout[result.stdout.rindex('{"reached"') :])
    assert answer["reached"] == ["DEVICE", "PACKED", "PLACED"]
    assert answer["timing"] == wanted
    assert without_header(other / f"{golden.name}.route") == wanted_route


# What is refused ----------------------------------------------------------------------


@pytest.fixture
def saved(openfpga_module, golden, work):
    _, checkpoint = work
    session = start(openfpga_module, golden)
    session.pack()
    session.place()
    session.checkpoint(checkpoint)
    return checkpoint


def test_no_checkpoint(openfpga_module, tmp_path):
    with pytest.raises(openfpga_module.CheckpointError, match="no checkpoint"):
        openfpga_module.Session.restore(tmp_path / "nothing")


def test_a_manifest_that_is_not_one(openfpga_module, saved):
    (saved / "checkpoint.json").write_text("{not json")
    with pytest.raises(openfpga_module.CheckpointError, match="not a checkpoint"):
        openfpga_module.Session.restore(saved)
    (saved / "checkpoint.json").write_text('{"format": 99}')
    with pytest.raises(openfpga_module.CheckpointError, match="format"):
        openfpga_module.Session.restore(saved)


def test_a_result_that_changed(openfpga_module, saved, golden):
    with open(saved / f"{golden.name}.place", "a") as handle:
        handle.write("# edited\n")
    with pytest.raises(openfpga_module.CheckpointError, match="placement"):
        openfpga_module.Session.restore(saved)


def test_a_result_that_is_missing(openfpga_module, saved, golden):
    (saved / f"{golden.name}.net").unlink()
    with pytest.raises(openfpga_module.CheckpointError, match="Cannot read"):
        openfpga_module.Session.restore(saved)


def test_an_input_that_changed(openfpga_module, golden, work, tmp_path):
    _, checkpoint = work
    circuit = tmp_path / "circuit.blif"
    circuit.write_bytes(golden.circuit.read_bytes())
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(circuit)
    session.pack()
    session.checkpoint(checkpoint)
    circuit.write_bytes(circuit.read_bytes() + b"\n")
    with pytest.raises(openfpga_module.CheckpointError, match="circuit"):
        openfpga_module.Session.restore(checkpoint)
    # With the new input the checks go on, VPR finds that the packing is not of this circuit
    with pytest.raises(openfpga_module.OpenfpgaError, match="different atom netlist"):
        openfpga_module.Session.restore(checkpoint, check_inputs=False)
