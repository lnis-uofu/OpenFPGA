"""Errors in commands must surface as Python exceptions, never kill the interpreter."""

import pytest


def test_missing_arch_file_does_not_crash(shell):
    # Used to abort the whole process on an uncaught ArchFpgaError
    assert shell.run_command("read_openfpga_arch -f /nonexistent/arch.xml") != 0


def test_run_command_after_failure_still_works(shell):
    assert shell.run_command("read_openfpga_arch -f /nonexistent/arch.xml") != 0
    assert shell.run_command("help") == 0


def test_missing_arch_file_raises(shell, openfpga_module):
    command = "read_openfpga_arch -f /nonexistent/arch.xml"
    with pytest.raises(openfpga_module.OpenfpgaError) as excinfo:
        shell.run(command)
    err = excinfo.value
    assert isinstance(err, RuntimeError)
    assert err.command == command
    assert err.status == 1
    assert "Failed to open file" in err.error
    assert "Failed to open file" in str(err)


def test_malformed_xml_raises(shell, openfpga_module, tmp_path):
    bad_arch = tmp_path / "bad.xml"
    bad_arch.write_text("<openfpga_architecture><broken\n")
    with pytest.raises(openfpga_module.OpenfpgaError, match="Unable to load XML file"):
        shell.run(f"read_openfpga_arch -f {bad_arch}")


def test_empty_xml_raises(shell, openfpga_module, tmp_path):
    empty_arch = tmp_path / "empty.xml"
    empty_arch.write_text("")
    with pytest.raises(openfpga_module.OpenfpgaError):
        shell.run(f"read_openfpga_arch -f {empty_arch}")


def test_unknown_command_raises(shell, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError, match="not_a_command"):
        shell.run("not_a_command")


def test_missing_option_raises(shell, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError, match="Required option 'file'"):
        shell.run("read_openfpga_arch")


def test_vpr_error_message_has_error_line(shell, openfpga_module):
    # No exception here: VPR reports its error on stderr and returns a status
    with pytest.raises(openfpga_module.OpenfpgaError, match="Failed to open file"):
        shell.run("vpr /nonexistent/arch.xml /nonexistent/design.blif")


def test_check_false_returns_result(shell):
    result = shell.run("not_a_command", check=False)
    assert result.status != 0
    assert "not_a_command" in result.log


def test_exit_is_refused(shell, openfpga_module):
    # 'exit' would terminate the interpreter
    with pytest.raises(openfpga_module.OpenfpgaError, match="exit"):
        shell.run("exit")
    assert shell.run_command("help") == 0


def test_exit_with_leading_spaces_is_refused(shell):
    assert shell.execute("   exit  ").status != 0


def test_none_command(shell):
    assert shell.run_command(None) != 0
    assert shell.execute(None).status != 0


def test_shell_usable_after_error(shell, openfpga_module, and2_files):
    with pytest.raises(openfpga_module.OpenfpgaError):
        shell.run("read_openfpga_arch -f /nonexistent/arch.xml")
    result = shell.run(f"read_openfpga_arch -f {and2_files['openfpga_arch']}")
    assert result.status == 0
