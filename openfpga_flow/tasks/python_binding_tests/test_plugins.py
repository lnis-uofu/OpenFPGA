"""The plugins of the shell that a build has (report_plugins, Session.plugins), and how
checkpoints and journals keep them: a checkpoint or a journal made by a build with other
plugins is not used, as their commands may not do the same.

The tests that need the example plugin (openfpga/plugins/example) run only in a build
made with OPENFPGA_PLUGINS=example."""

import json
from pathlib import Path

import pytest

from test_checkpoint import golden, start, work  # noqa: F401 (fixtures)

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"


def names(session):
    return [p.name for p in session.plugins()]


def test_the_answer_follows_its_schema(openfpga_module, tmp_path, monkeypatch):
    jsonschema = pytest.importorskip("jsonschema")
    monkeypatch.chdir(tmp_path)
    shell = openfpga_module.OpenfpgaShell()
    answer = shell.run("report_plugins").data
    schema = json.loads((SCHEMAS / "plugins.schema.json").read_text())
    jsonschema.validate(answer, schema, cls=jsonschema.Draft202012Validator)
    assert answer["kind"] == "plugins"
    # version says the same plugins
    built = ", ".join("%s %s" % (p["name"], p["version"]) for p in answer["data"]["plugins"])
    assert ("Plugins: %s" % (built or "none")) in shell.run("version").log


def test_the_ui_plugin_has_the_queries_and_the_options(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    (ui,) = [p for p in session.plugins() if p.name == "ui"]
    assert {"set_app_option", "reset_app_option", "report_app_option"} <= set(ui.commands)
    assert {"report_session", "timing_summary", "placement", "find"} <= set(ui.commands)
    # It adds commands, and changes none of the shell's
    assert ui.overrides == [] and ui.removes == [] and ui.hooks == [] and not ui.every_command
    # The flow itself stays in the shell
    assert not {"read_vpr_arch", "pack", "route", "report_plugins"} & set(ui.commands)


def test_a_checkpoint_keeps_the_plugins(openfpga_module, golden, work):  # noqa: F811
    _, checkpoint = work
    session = start(openfpga_module, golden)
    session.checkpoint(checkpoint)
    manifest_path = checkpoint / "checkpoint.json"
    manifest = json.loads(manifest_path.read_text())
    assert manifest["plugins"] == [
        {"name": p.name, "version": p.version} for p in session.plugins()
    ]
    openfpga_module.Session.restore(checkpoint)

    # Made by a build with another plugin: refused, and the message says both
    manifest["plugins"] = manifest["plugins"] + [{"name": "other", "version": "2"}]
    manifest_path.write_text(json.dumps(manifest))
    with pytest.raises(openfpga_module.CheckpointError, match="other 2"):
        openfpga_module.Session.restore(checkpoint)

    # Made before the plugins were kept, when the commands of the ui plugin were the
    # shell's own: taken
    del manifest["plugins"]
    manifest_path.write_text(json.dumps(manifest))
    openfpga_module.Session.restore(checkpoint)


def test_a_journal_keeps_the_plugins(openfpga_module, golden, tmp_path, monkeypatch):  # noqa: F811
    monkeypatch.chdir(tmp_path)
    journal = tmp_path / "journal"
    session = start(openfpga_module, golden, journal=journal)
    label = ", ".join("%s %s" % (p.name, p.version) for p in session.plugins()) or "none"
    for script in ("replay.openfpga", "replay.py"):
        assert ("# Plugins of the build: %s\n" % label) in (journal / script).read_text()
    openfpga_module.Session.replay(journal)

    shell_script = journal / "replay.openfpga"
    text = shell_script.read_text()
    shell_script.write_text(
        text.replace("# Plugins of the build: %s" % label, "# Plugins of the build: other 2")
    )
    with pytest.raises(openfpga_module.ReplayError, match="other 2") as error:
        openfpga_module.Session.replay(journal)
    assert error.value.line == "# Plugins of the build: other 2"

    # A script that a person wrote, with no such line, is replayed
    shell_script.write_text(text.replace("# Plugins of the build: %s\n" % label, ""))
    openfpga_module.Session.replay(journal)


# With the example plugin built in -----------------------------------------------------


@pytest.fixture
def example(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    if "example" not in names(session):
        pytest.skip("the build has no example plugin (OPENFPGA_PLUGINS=example)")
    return session


def test_the_example_plugin_is_reported(openfpga_module, example):
    (plugin,) = [p for p in example.plugins() if p.name == "example"]
    assert plugin.version == "1.0"
    assert plugin.commands == ["example_report"]
    assert plugin.overrides == ["write_fabric_spice"]
    assert plugin.removes == ["write_sdc_disable_timing_configure_ports"]
    assert openfpga_module.PluginHook("read_vpr_arch", "after") in plugin.hooks
    assert openfpga_module.PluginHook("read_circuit", "before") in plugin.hooks
    assert plugin.every_command


def test_the_commands_are_those_the_plugin_left(example):
    commands = example.shell_commands()
    assert "example_report" in commands and "write_fabric_spice" in commands
    assert "write_sdc_disable_timing_configure_ports" not in commands
    assert all(" " not in name for name in commands)


def test_a_hook_refuses_and_a_removed_command_fails(openfpga_module, example):
    with pytest.raises(openfpga_module.OpenfpgaError, match="refused by plugin 'example'"):
        example.command("read_circuit", file="refused_by_example.blif")
    with pytest.raises(openfpga_module.OpenfpgaError, match="removed by plugin 'example'"):
        example.command("write_sdc_disable_timing_configure_ports", file="x.sdc")
