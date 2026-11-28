"""What a plugin and a subclass of Session add to a session: the attachments of the session
(set_attachment, report_session, checkpoints), the owner of the options, the hooks of the
checkpoints (_checkpoint_extra, _restore_inputs, _restore_extra), and the class of the session of
a worker (RemoteSession(session_class=...), OPENFPGA_SESSION_CLASS, REMOTE_METHODS)."""

import json
from pathlib import Path

import pytest

from test_checkpoint import golden, start, work  # noqa: F401 (fixtures)

HERE = Path(__file__).resolve().parent
SCHEMAS = HERE.parents[2] / "openfpga_flow" / "schemas"
SUBCLASS = "session_subclass:Recording"


@pytest.fixture
def subclass(openfpga_module, monkeypatch):
    """The module of the subclass, which the workers import too"""
    monkeypatch.syspath_prepend(str(HERE))
    monkeypatch.setenv("PYTHONPATH", str(HERE))
    import session_subclass

    return session_subclass


# Attachments ----------------------------------------------------------------------------


def test_an_attachment_is_reported_and_goes_with_the_architectures(
    openfpga_module, golden, work  # noqa: F811
):
    jsonschema = pytest.importorskip("jsonschema")
    session = start(openfpga_module, golden)
    assert session.attachments() == {}
    session.set_attachment("myplugin.data", {"a": [1, 2], "b": None})
    session.set_attachment("myplugin.flag", True)
    assert session.attachments() == {
        "myplugin.data": {"a": [1, 2], "b": None},
        "myplugin.flag": True,
    }
    raw = session.shell.run("report_session").data
    schema = json.loads((SCHEMAS / "session.schema.json").read_text())
    jsonschema.validate(raw, schema, cls=jsonschema.Draft202012Validator)
    session.set_attachment("myplugin.flag", None)
    assert list(session.attachments()) == ["myplugin.data"]
    # The circuit keeps them, an architecture does not
    session.read_circuit(golden.circuit)
    assert list(session.attachments()) == ["myplugin.data"]
    session.read_openfpga_arch(golden.openfpga_arch)
    assert session.attachments() == {}


def test_an_attachment_is_refused_without_a_plugin_key_or_json(openfpga_module, work):  # noqa: F811
    session = openfpga_module.Session()
    with pytest.raises(openfpga_module.OpenfpgaError, match="starts with the name of its plugin"):
        session.set_attachment("nodot", 1)
    with pytest.raises(openfpga_module.OpenfpgaError, match="is not JSON"):
        session.command("set_attachment", key="myplugin.data", value="{not json")
    assert session.command("set_attachment", key="myplugin.data", value="[1,2]")
    assert session.attachments() == {"myplugin.data": [1, 2]}


def test_a_checkpoint_keeps_the_attachments(openfpga_module, golden, work):  # noqa: F811
    _, checkpoint = work
    session = start(openfpga_module, golden)
    session.set_attachment("myplugin.data", {"device": "k6"})
    session.pack()
    session.checkpoint(checkpoint)
    manifest = json.loads((checkpoint / "checkpoint.json").read_text())
    assert manifest["attachments"] == {"myplugin.data": {"device": "k6"}}
    assert "extra" not in manifest
    restored = openfpga_module.Session.restore(checkpoint)
    assert restored.attachments() == {"myplugin.data": {"device": "k6"}}
    assert restored.state().reached == ["DEVICE", "PACKED"]


# Options -------------------------------------------------------------------------------


def test_each_option_has_its_owner(openfpga_module, work):  # noqa: F811
    session = openfpga_module.Session()
    assert session.options.get("place.seed").owner == "vpr"
    assert session.options.get("place.placer").owner == "openfpga"
    plugins = {p.name for p in session.plugins()}
    assert {o.owner for o in session.options.list()} <= {"vpr", "openfpga"} | plugins


# A subclass of Session ----------------------------------------------------------------


def test_a_subclass_saves_and_restores_its_own_state(
    openfpga_module, subclass, golden, work  # noqa: F811
):
    _, checkpoint = work
    recording = subclass.Recording()
    recording.options["general.clock_modeling"] = "route"
    recording.options["router.route_chan_width"] = golden.chan_width
    recording.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    recording.read_openfpga_arch(golden.openfpga_arch)
    recording.read_circuit(golden.circuit)
    recording.note = {"kept": 42}
    recording.pack()
    recording.checkpoint(checkpoint)
    manifest = json.loads((checkpoint / "checkpoint.json").read_text())
    assert manifest["extra"] == {"recording": {"file": "note.json"}}

    restored = subclass.Recording.restore(checkpoint)
    assert isinstance(restored, subclass.Recording)
    assert restored.note == {"kept": 42}
    assert restored.hooks() == [("inputs", True), ("extra", True, True)]
    assert restored.state().reached == ["DEVICE", "PACKED"]
    # The base class reads the checkpoint too: what it does not know is left out
    assert openfpga_module.Session.restore(checkpoint).state().reached == ["DEVICE", "PACKED"]


def test_a_worker_runs_a_subclass(openfpga_module, subclass, golden, tmp_path):  # noqa: F811
    with openfpga_module.RemoteSession(cwd=tmp_path / "w", session_class=SUBCLASS) as remote:
        assert remote.session_class == SUBCLASS
        assert remote.twice(21) == 42
        assert remote.call("twice", 4) == 8
        assert remote.state().reached == []  # the methods of Session are there
        with pytest.raises(AttributeError):
            remote.not_remote()
        with pytest.raises(openfpga_module.WorkerError, match="not a method of a Session"):
            remote.call("not_remote")


def test_the_class_of_a_worker_from_the_environment_and_on_restore(
    openfpga_module, subclass, golden, work, monkeypatch  # noqa: F811
):
    _, checkpoint = work
    recording = subclass.Recording()
    recording.options["general.clock_modeling"] = "route"
    recording.options["router.route_chan_width"] = golden.chan_width
    recording.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    recording.read_openfpga_arch(golden.openfpga_arch)
    recording.read_circuit(golden.circuit)
    recording.note = "from the caller"
    recording.pack()
    recording.checkpoint(checkpoint)

    monkeypatch.setenv("OPENFPGA_SESSION_CLASS", SUBCLASS)
    with openfpga_module.RemoteSession.restore(checkpoint, cwd=work[0] / "w") as remote:
        assert remote.session_class == SUBCLASS
        assert remote.hooks() == [("inputs", True), ("extra", True, True)]
        assert remote.state().reached == ["DEVICE", "PACKED"]
    monkeypatch.delenv("OPENFPGA_SESSION_CLASS")
    with openfpga_module.RemoteSession.restore(
        checkpoint, cwd=work[0] / "x", session_class="openfpga:Session"
    ) as remote:
        assert remote.state().reached == ["DEVICE", "PACKED"]
        with pytest.raises(AttributeError):
            remote.hooks()


def test_a_class_that_is_not_a_session_is_refused(openfpga_module, tmp_path):
    for name in ("json:JSONDecoder", "no_such_module:Session", "openfpga", "openfpga:Nothing"):
        with pytest.raises(TypeError):
            openfpga_module.RemoteSession(cwd=tmp_path / "w", session_class=name)


def test_the_journal_keeps_the_value_of_an_attachment(
    openfpga_module, golden, work, tmp_path  # noqa: F811
):
    journal = tmp_path / "journal"
    session = start(openfpga_module, golden, journal=journal)
    session.set_attachment("myplugin.data", {"name": "x y"})
    kept = journal / "attachments" / "001.json"
    assert json.loads(kept.read_text()) == {"name": "x y"}
    assert str(kept) in (journal / "replay.openfpga").read_text()
    assert openfpga_module.Session.replay(journal).attachments() == {
        "myplugin.data": {"name": "x y"}
    }
