"""The `yosys` command of the shell: Yosys runs inside the process, stays set up between commands, and an
error of Yosys is a failed command that does not end the process."""

import re

import pytest

AND2 = """module and2(input a, input b, output y);
  assign y = a & b;
endmodule
"""

# Maps to 6-input LUTs and writes a BLIF, like the Yosys step of the flow scripts
SYNTHESIS = """read_verilog and2.v
hierarchy -top and2
proc
flatten
opt
techmap
opt
abc -lut 6
opt_clean -purge
write_blif and2.blif
"""

BROKEN_VERILOG = """module broken(input a, output y);
  assign y = a &;
endmodule
"""


@pytest.fixture
def scripts(tmp_path):
    """Yosys scripts of a good run and of the ways that Yosys fails, in the current directory"""
    (tmp_path / "and2.v").write_text(AND2)
    (tmp_path / "broken.v").write_text(BROKEN_VERILOG)
    (tmp_path / "good.ys").write_text(SYNTHESIS)
    (tmp_path / "no_file.ys").write_text("read_verilog nothere.v\n")
    (tmp_path / "syntax.ys").write_text("read_verilog broken.v\n")
    (tmp_path / "no_command.ys").write_text("not_a_yosys_command\n")
    # An error that Yosys reports from inside a pass, not from a frontend or a command line
    (tmp_path / "in_pass.ys").write_text(
        "read_verilog and2.v\nhierarchy -top not_there\n"
    )
    return tmp_path


FAILURES = ["no_file.ys", "syntax.ys", "no_command.ys", "in_pass.ys"]


def synthesize(shell, tmp_path):
    (tmp_path / "and2.blif").unlink(missing_ok=True)
    return shell.run_command("yosys --file good.ys")


def test_yosys_maps_a_design_to_luts_and_writes_a_blif(shell, scripts):
    assert synthesize(shell, scripts) == 0
    blif = (scripts / "and2.blif").read_text()
    assert ".model and2" in blif and ".inputs a b" in blif and ".outputs y" in blif
    assert re.search(
        r"^\.names (a b|b a) y\n11 1$", blif, re.M
    )  # the AND gate is one LUT


@pytest.mark.parametrize("script", FAILURES)
def test_an_error_of_yosys_fails_the_command_and_leaves_the_process_alive(
    shell, scripts, script
):
    assert shell.run_command(f"yosys --file {script}") != 0
    # The same process runs Yosys again: it was neither shut down nor left half way
    assert synthesize(shell, scripts) == 0
    assert ".model and2" in (scripts / "and2.blif").read_text()


def test_yosys_runs_many_times_in_one_process_with_failures_between(shell, scripts):
    for _ in range(5):
        assert synthesize(shell, scripts) == 0
        for script in FAILURES:
            assert shell.run_command(f"yosys --file {script}") != 0
    assert synthesize(shell, scripts) == 0


def test_a_script_does_not_see_the_design_of_the_one_before(shell, scripts):
    # A design of its own for each script: the module that was read before is gone
    (scripts / "second.ys").write_text("hierarchy -top and2\n")
    assert synthesize(shell, scripts) == 0
    assert shell.run_command("yosys --file second.ys") != 0
