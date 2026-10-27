"""Each command call returns its own log."""

import ctypes
import os


def test_execute_returns_log(shell):
    result = shell.execute("help")
    assert result.status == 0
    assert "vpr" in result.log
    assert result.error == ""


def test_logs_do_not_leak_between_commands(shell, and2_files):
    first = shell.run("help")
    arch = str(and2_files["openfpga_arch"])
    second = shell.run(f"read_openfpga_arch -f {arch}")

    assert "vpr_standalone" in first.log
    assert arch not in first.log
    assert arch in second.log
    # Nothing of the first command is repeated in the second one
    assert "vpr_standalone" not in second.log


def test_same_command_twice_gives_same_size_log(shell):
    assert shell.run("help").log == shell.run("help").log


def test_failed_command_log_has_stderr(shell):
    # The error is reported through stderr
    result = shell.execute("read_openfpga_arch -f /nonexistent/arch.xml")
    assert result.status != 0
    assert "Failed to open file" in result.log


def test_vpr_flow_log(shell, and2_files):
    # vpr_standalone frees its data at the end. test_smoke.py covers 'vpr'
    result = shell.run(
        f"vpr_standalone {and2_files['vpr_arch']} {and2_files['blif']} "
        "--clock_modeling route --device 2x2 --route_chan_width 20"
    )
    assert "VPR succeeded" in result.log


def test_output_is_not_printed(shell, capfd):
    shell.run("help")
    captured = capfd.readouterr()
    assert captured.out == ""
    assert captured.err == ""


def test_error_output_is_not_printed(shell, capfd):
    shell.execute("read_openfpga_arch -f /nonexistent/arch.xml")
    captured = capfd.readouterr()
    assert captured.out == ""
    assert captured.err == ""


def test_streams_restored_after_capture(shell, capfd):
    shell.run("help")
    os.write(1, b"after-stdout\n")
    os.write(2, b"after-stderr\n")
    captured = capfd.readouterr()
    assert captured.out == "after-stdout\n"
    assert captured.err == "after-stderr\n"


def test_streams_restored_after_error(shell, capfd):
    shell.execute("read_openfpga_arch -f /nonexistent/arch.xml")
    os.write(1, b"after-error\n")
    assert capfd.readouterr().out == "after-error\n"


def test_run_command_still_prints(shell, capfd):
    assert shell.run_command("help") == 0
    # stdout is block-buffered when redirected: push it out before reading
    ctypes.CDLL(None).fflush(None)
    assert "vpr" in capfd.readouterr().out
