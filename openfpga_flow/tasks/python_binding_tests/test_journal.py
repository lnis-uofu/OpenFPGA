"""The journal of a Session: the commands that changed the flow, written so that the flow can be
run again, as a Python script, as a shell script and with Session.replay().

A replay is right if it gives what the first run gave: the files that VPR wrote are compared
byte by byte.
"""

import os
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

from test_equivalence import Golden

REPO = Path(__file__).resolve().parents[3]
FILES = ("routing_test.net", "routing_test.place", "routing_test.route")
MODULE_DIR = Path(os.environ.get("OPENFPGA_PYTHON_PATH", REPO / "build" / "openfpga"))
BINARY = REPO / "build" / "openfpga" / "openfpga"


@pytest.fixture
def golden():
    return Golden("k6_routing_test")


def directory(base, name):
    path = base / name
    path.mkdir()
    return path


def run_flow(openfpga_module, golden, where, journal, stop_after="route", **options):
    """A flow in the directory where, with a journal"""
    os.chdir(where)
    session = openfpga_module.Session(journal=journal)
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    for stage in ("pack", "place", "route", "analysis"):
        getattr(session, stage)()
        if stage == stop_after:
            break
    return session


class Seen:
    """What a session said when it was run: another session in the process replaces the data
    of VPR that it worked with, so it is asked at once"""

    def __init__(self, session):
        self.session = session
        self.state = session.state()
        self.timing = session.timing_summary()
        self.congestion = session.congestion(hotspots=3)
        self.options = {o.name: o.value for o in session.options.changed()}


@pytest.fixture
def first(openfpga_module, golden, tmp_path, monkeypatch):
    """A flow with a journal: its directory, the journal and what the session said"""
    where = directory(tmp_path, "first")
    journal = tmp_path / "journal"
    monkeypatch.chdir(where)
    session = run_flow(openfpga_module, golden, where, journal)
    session.utilization()
    return where, journal, Seen(session)


def commands(journal):
    """The lines of the shell script that are commands"""
    lines = (Path(journal) / "replay.openfpga").read_text().splitlines()
    return [line for line in lines if line and not line.startswith("#")]


def same_files(one, other, files=FILES):
    for name in files:
        assert (Path(one) / name).read_bytes() == (Path(other) / name).read_bytes(), name


# What is written ----------------------------------------------------------------------------


def test_the_journal_has_the_commands_that_changed_the_flow(first, golden):
    _, journal, _ = first
    lines = commands(journal)
    assert [line.split()[0] for line in lines] == [
        "set_app_option",
        "set_app_option",
        "read_vpr_arch",
        "read_openfpga_arch",
        "read_circuit",
        "pack",
        "place",
        "route",
        "exit",
    ]
    assert lines[0] == "set_app_option --name general.clock_modeling --value route"
    assert lines[1] == f"set_app_option --name router.route_chan_width --value {golden.chan_width}"
    # The queries are not there: they do not change anything
    text = (Path(journal) / "replay.openfpga").read_text()
    assert "utilization" not in text and "congestion" not in text


def test_the_python_script_has_the_same_commands(first, golden):
    _, journal, _ = first
    script = (Path(journal) / "replay.py").read_text()
    compile(script, "replay.py", "exec")
    body = [line for line in script.splitlines() if line.startswith("session.")]
    assert body[0] == "session.options['general.clock_modeling'] = 'route'"
    assert body[1] == f"session.options['router.route_chan_width'] = {golden.chan_width}"
    assert (
        body[2].startswith("session.command('read_vpr_arch', file='") and "layout='2x2'" in body[2]
    )
    assert body[-2:] == ["session.command('place')", "session.command('route')"]
    assert len(body) == len(commands(journal)) - 1  # exit is not a command of the script


def test_the_files_are_whole_paths(openfpga_module, golden, tmp_path, monkeypatch):
    """A file given as a relative path is written as a whole one: a replay is run elsewhere"""
    where = directory(tmp_path, "here")
    monkeypatch.chdir(where)
    local = where / "arch.xml"
    shutil.copy(golden.vpr_arch, local)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.read_vpr_arch("arch.xml", layout=golden.layout)
    (line,) = [c for c in commands(tmp_path / "journal") if c.startswith("read_vpr_arch")]
    assert f"--file {local}" in line


def test_a_command_that_failed_is_a_comment(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.read_packing("/no/such/file.net")
    for name in ("replay.openfpga", "replay.py"):
        lines = (tmp_path / "journal" / name).read_text().splitlines()
        (failed,) = [line for line in lines if "read_packing" in line]
        assert failed.startswith("# Failed, not replayed: read_packing --file /no/such/file.net (")
        assert "does not exist" in failed
    assert [c.split()[0] for c in commands(tmp_path / "journal")] == [
        "read_vpr_arch",
        "read_openfpga_arch",
        "read_circuit",
        "exit",
    ]


def test_a_path_with_a_space_is_quoted(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    spaced = directory(tmp_path, "my files")
    arch = spaced / "arch.xml"
    shutil.copy(golden.vpr_arch, arch)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.read_vpr_arch(arch, layout=golden.layout)
    assert f'read_vpr_arch --file "{arch}" --layout {golden.layout}' in commands(
        tmp_path / "journal"
    )
    # And it is read again from there
    replayed = openfpga_module.Session.replay(tmp_path / "journal")
    assert replayed.state().inputs.vpr_arch.file == str(arch)


def test_a_journal_is_written_as_the_commands_end(openfpga_module, golden, tmp_path, monkeypatch):
    """A run that stops leaves a journal that runs, and the next session starts a new one"""
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.options["router.route_chan_width"] = 20
    assert commands(tmp_path / "journal") == [
        "set_app_option --name router.route_chan_width --value 20",
        "exit",
    ]
    session.options["router.route_chan_width"] = 30
    assert len(commands(tmp_path / "journal")) == 3
    openfpga_module.Session(journal=tmp_path / "journal")
    assert commands(tmp_path / "journal") == ["exit"]


def test_a_boolean_option_is_written_as_the_shell_reads_it(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    option = next(o for o in session.options.list() if o.type == "bool")
    session.options[option.name] = not option.value
    expected = "true" if not option.value else "false"
    assert f"set_app_option --name {option.name} --value {expected}" in commands(
        tmp_path / "journal"
    )


# The flow is run again ----------------------------------------------------------------------


def test_replay_gives_what_the_first_run_gave(openfpga_module, first, tmp_path, monkeypatch):
    where, journal, session = first
    second = directory(tmp_path, "second")
    monkeypatch.chdir(second)
    replayed = openfpga_module.Session.replay(journal)
    same_files(where, second)
    assert (
        replayed.state().reached
        == session.state.reached
        == ["DEVICE", "PACKED", "PLACED", "ROUTED"]
    )
    assert replayed.timing_summary() == session.timing
    assert replayed.congestion(hotspots=3) == session.congestion
    assert replayed.state().inputs == session.state.inputs


def test_replay_takes_either_file_of_the_journal(openfpga_module, first, tmp_path, monkeypatch):
    _, journal, _ = first
    for index, path in enumerate((journal / "replay.py", journal / "replay.openfpga")):
        monkeypatch.chdir(directory(tmp_path, f"again{index}"))
        assert openfpga_module.Session.replay(path).state().reached[-1] == "ROUTED"


def test_the_python_script_runs_by_itself(first, tmp_path):
    where, journal, _ = first
    second = directory(tmp_path, "script")
    env = dict(os.environ, PYTHONPATH=str(MODULE_DIR))
    done = subprocess.run(
        [sys.executable, str(journal / "replay.py")],
        cwd=second,
        env=env,
        capture_output=True,
        text=True,
    )
    assert done.returncode == 0, done.stdout[-500:] + done.stderr[-500:]
    same_files(where, second)


def test_the_shell_script_runs_in_the_shell(first, tmp_path):
    if not BINARY.exists():
        pytest.skip(f"missing {BINARY}")
    where, journal, _ = first
    second = directory(tmp_path, "shell")
    done = subprocess.run(
        [str(BINARY), "-f", str(journal / "replay.openfpga")],
        cwd=second,
        stdin=subprocess.DEVNULL,
        capture_output=True,
        text=True,
        timeout=300,
    )
    assert done.returncode == 0, done.stdout[-500:] + done.stderr[-500:]
    same_files(where, second)


def test_a_replay_is_journaled_like_the_first_run(openfpga_module, first, tmp_path, monkeypatch):
    _, journal, _ = first
    monkeypatch.chdir(directory(tmp_path, "second"))
    openfpga_module.Session.replay(journal, journal=tmp_path / "again")
    assert commands(tmp_path / "again") == commands(journal)


def test_replay_stops_at_the_command_that_fails(openfpga_module, first, tmp_path, monkeypatch):
    _, journal, _ = first
    script = tmp_path / "broken.openfpga"
    lines = commands(journal)[:-1]
    # A person edits it: a comment, a blank line, and a command that does not work
    lines.insert(6, "")
    lines.insert(6, "# the packing")
    lines.insert(8, "read_packing --file /no/such/file.net")
    script.write_text("\n".join(lines) + "\n")
    monkeypatch.chdir(directory(tmp_path, "broken"))
    with pytest.raises(openfpga_module.ReplayError) as stopped:
        openfpga_module.Session.replay(script)
    assert stopped.value.line == "read_packing --file /no/such/file.net"
    assert stopped.value.number == 9
    assert isinstance(stopped.value.__cause__, openfpga_module.OpenfpgaError)


def test_replay_goes_on_from_the_options_that_were_set(
    openfpga_module, first, tmp_path, monkeypatch
):
    """The replay is a session: it can be taken further, and the options are those of the journal"""
    _, journal, session = first
    monkeypatch.chdir(directory(tmp_path, "further"))
    replayed = openfpga_module.Session.replay(journal)
    assert {o.name: o.value for o in replayed.options.changed()} == session.options
    replayed.analysis()
    assert replayed.state().reached[-1] == "ANALYZED"


# What goes in a journal ---------------------------------------------------------------------


def test_the_journal_of_a_restored_session_replays_to_the_same_point(
    openfpga_module, first, tmp_path, monkeypatch
):
    where, _, seen = first
    saved = seen.session.checkpoint(tmp_path / "saved")
    monkeypatch.chdir(directory(tmp_path, "restored"))
    restored = openfpga_module.Session.restore(saved, journal=tmp_path / "restored_journal")
    assert restored.state().reached == seen.state.reached
    names = [c.split()[0] for c in commands(tmp_path / "restored_journal")]
    assert names[-4:] == ["read_packing", "read_placement", "read_routing", "exit"]
    monkeypatch.chdir(directory(tmp_path, "from_restored"))
    again = openfpga_module.Session.replay(tmp_path / "restored_journal")
    assert again.state().reached == seen.state.reached
    assert again.timing_summary() == seen.timing


def test_a_command_of_the_session_is_in_the_journal_with_its_options(
    openfpga_module, golden, tmp_path, monkeypatch
):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    session.command("pack")
    session.command("report_session")  # a query: not written
    assert [c.split()[0] for c in commands(tmp_path / "journal")] == [
        "read_vpr_arch",
        "read_openfpga_arch",
        "read_circuit",
        "pack",
        "exit",
    ]


def test_what_the_shell_runs_by_itself_is_not_journaled(openfpga_module, tmp_path, monkeypatch):
    """session.shell is the way to run what has no method, and to ask: it is not written"""
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.shell.run("report_session")
    session.shell.run("help")
    assert commands(tmp_path / "journal") == ["exit"]


def test_a_session_without_a_journal_writes_nothing(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    assert not list(tmp_path.glob("replay*"))
