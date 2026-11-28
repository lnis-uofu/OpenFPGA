"""Session.tcl: Tcl scripts over a Session, with every command of the shell as a Tcl command, the
answers of the commands as Tcl dicts, the journal of the commands they run, and a safe interpreter"""

import os

import pytest

from test_session import CIRCUIT

pytest.importorskip("_tkinter")


@pytest.fixture
def circuit(flow_dir):
    path = flow_dir / "benchmarks" / "ubm" / CIRCUIT
    if not path.exists():
        pytest.skip(f"missing {path}")
    return path


@pytest.fixture
def session(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return openfpga_module.Session(journal=tmp_path / "journal")


def flow_script(and2_files, circuit):
    return f"""
set_app_option --name general.clock_modeling --value route
set_app_option --name router.route_chan_width --value 20
read_vpr_arch --file {and2_files["vpr_arch"]} --layout 2x2
read_openfpga_arch --file {and2_files["openfpga_arch"]}
read_circuit --file {circuit}
foreach stage {{pack place route analysis}} {{
    $stage
}}
dict get [report_run] routing
"""


def test_every_command_of_the_shell_is_a_tcl_command(session):
    names = session.shell_commands()
    for expected in (
        "vpr",
        "build_fabric",
        "write_fabric_verilog",
        "repack",
        "report_run",
        "exit",
    ):
        assert expected in names
    result = session.tcl("lsort [info commands ::openfpga::*]")
    found = {name.split("::")[-1] for name in result.result.split()}
    assert {n for n in names if n != "exit"} <= found and "exit" not in found
    # The command of Tcl keeps its name; the one of the shell is in the namespace
    assert session.tcl("info commands ::source").result == "::source"
    assert session.tcl("namespace which openfpga::source").result == "::openfpga::source"


def test_a_flow_in_tcl_is_in_the_journal_and_answers_as_dicts(
    session, and2_files, circuit, tmp_path
):
    result = session.tcl(flow_script(and2_files, circuit))
    routing = dict(zip(result.result.split()[::2], result.result.split()[1::2]))
    assert int(routing["channel_width"]) == 20 and int(routing["wirelength"]) > 0, result.result
    assert result.commands[-1] == "report_run"
    assert [c.split()[0] for c in result.commands[5:9]] == ["pack", "place", "route", "analysis"]
    assert set(session.state().reached) >= {"PACKED", "PLACED", "ROUTED", "ANALYZED"}
    # The Tcl loop is not in the journal, the commands it ran are, with whole paths
    shell = (tmp_path / "journal" / "replay.openfpga").read_text().splitlines()
    run = [line for line in shell if line and not line.startswith("#")]
    assert run[:3] == [
        "set_app_option --name general.clock_modeling --value route",
        "set_app_option --name router.route_chan_width --value 20",
        "read_vpr_arch --file %s --layout 2x2" % os.path.abspath(and2_files["vpr_arch"]),
    ]
    assert run[5:] == ["pack", "place", "route", "analysis", "exit"]
    # A script takes the answer apart like any dict
    count = session.tcl("llength [dict get [report_run] packing blocks]").result
    assert int(count) >= 1


def test_variables_and_procedures_stay_for_the_next_script(session):
    session.tcl("set seed 9\nproc twice {x} { expr {2 * $x} }")
    assert session.tcl("twice $seed").result == "18"
    # The safe interpreter is another one
    assert session.tcl("info exists seed", safe=True).result == "0"


def test_puts_is_the_output_and_the_logs_of_the_commands_too(session):
    result = session.tcl('puts "one"\nputs -nonewline two\nputs stderr three\nversion')
    assert result.output.startswith("one\ntwothree\n")
    assert result.printed == "one\ntwothree\n"
    assert "Version:" in result.output and result.commands == ["version"]


def test_a_command_that_fails_is_an_error_that_tcl_catches(session, openfpga_module):
    result = session.tcl(
        "if {[catch {pack} message options]} { list $message [dict get $options -errorcode] }"
    )
    message, code = result.result.split("} {")
    assert "has not been read" in message and code.rstrip("}") == "OPENFPGA input_not_read"
    with pytest.raises(openfpga_module.TclError) as failure:
        session.tcl("proc go {} {\n    pack\n}\ngo")
    error = failure.value
    assert "has not been read" in str(error) and error.commands == ["pack"]
    assert '(procedure "go" line 2)' in error.error_info
    kinds = [d.kind for d in error.diagnostics]
    assert kinds[0] == "tcl_error" and "input_not_read" in kinds[1:]


def test_an_error_of_tcl_itself(session, openfpga_module):
    with pytest.raises(openfpga_module.TclError) as failure:
        session.tcl("set a 1\nexpr {$a +}")
    assert failure.value.diagnostics[0].data["where"] == 2
    with pytest.raises(openfpga_module.TclError, match="exit is not available"):
        session.tcl("exit")
    with pytest.raises(openfpga_module.TclError, match="cd is not available"):
        session.tcl("cd /")
    with pytest.raises(openfpga_module.TclError, match="cannot be written on a command line"):
        session.tcl('read_circuit --file "a\\"b"')  # a value that a command line cannot have


def test_a_safe_interpreter_keeps_to_the_directory_of_the_session(
    session, openfpga_module, tmp_path
):
    (tmp_path / "inside.blif").write_text("")
    os.symlink("/etc", tmp_path / "out")
    refused = {
        "read_circuit --file /etc/hosts": "out of the directory",
        "read_circuit --file ../x.blif": "out of the directory",
        "read_circuit --file out/hosts": "out of the directory",
        "write_fabric_verilog --file a,/tmp/b": "out of the directory",
        "ext_exec --command ls": "not available in a safe interpreter",
        "yosys --file s.ys": "not available in a safe interpreter",
        "source script.openfpga": "not available in a safe interpreter",
    }
    for script, message in refused.items():
        with pytest.raises(openfpga_module.TclRefused, match=message):
            session.tcl(script, safe=True)
    # Refused before it runs: nothing is in the journal
    assert "ext_exec" not in (tmp_path / "journal" / "replay.openfpga").read_text()
    for script in ("open /etc/hosts", "exec ls", "cd /", "exit", "puts file1 x"):
        with pytest.raises(openfpga_module.TclError):
            session.tcl(script, safe=True)
    # A name in the directory goes to the command, which says what it thinks of it
    with pytest.raises(openfpga_module.TclError) as failure:
        session.tcl("read_circuit --file inside.blif", safe=True)
    assert not isinstance(failure.value, openfpga_module.TclRefused)
    assert session.tcl("puts [lindex [version] 0]; expr {6 * 7}", safe=True).result == "42"


def test_the_values_of_the_answers(openfpga_module):
    import _tkinter

    tcl = _tkinter.create(None, "t", "Tk", False, False, False, False, None)
    value = openfpga_module._to_tcl(tcl, {"a b": [1, 2.5, None, True], "c": {"d": "x {y"}, "e": []})
    tcl.setvar("v", value)
    assert tcl.eval("lindex [dict get $v {a b}] 1") == "2.5"
    assert tcl.eval("lindex [dict get $v {a b}] 3") == "1"
    assert tcl.eval("llength [dict get $v {a b}]") == "4"
    assert tcl.eval("dict get $v c d") == "x {y"
    assert tcl.eval("dict get $v e") == ""


def test_in_a_worker_the_failures_come_back_and_a_loop_is_stopped(openfpga_module, tmp_path):
    with openfpga_module.RemoteSession(cwd=tmp_path / "w", timeout_s=60) as remote:
        remote.tcl("set kept 1")
        with pytest.raises(openfpga_module.TclError) as failure:
            remote.tcl("pack")
        assert failure.value.commands == ["pack"] and failure.value.output
        assert any(d.kind == "input_not_read" for d in failure.value.diagnostics)
        with pytest.raises(openfpga_module.TclRefused):
            remote.tcl("read_circuit --file /etc/hosts", safe=True)
        with pytest.raises(openfpga_module.WorkerTimeout):
            remote.tcl("while 1 {}", timeout_s=2)
        remote.restart()
        # The interpreter was the one of the worker that ended
        assert remote.tcl("info exists kept").result == "0"
