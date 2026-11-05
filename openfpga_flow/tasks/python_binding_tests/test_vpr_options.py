"""VPR's own parser ends the process after --help, after --version and on
an option it rejects. The shell reads the options without that exit, so
these commands return a status and the interpreter keeps running."""

import pytest


@pytest.fixture
def vpr_args(and2_files):
    return f"{and2_files['vpr_arch']} {and2_files['blif']}"


def test_help_returns(shell):
    result = shell.run("vpr --help")
    assert result.status == 0
    assert "--place_algorithm" in result.log


def test_version_returns(shell):
    result = shell.run("vpr --version")
    assert result.status == 0
    assert "Revision" in result.log


@pytest.mark.parametrize(
    "option",
    ["--bogus_option 3", "--place_algorithm nonsense", "--route_chan_width x"],
)
def test_rejected_option_raises(shell, openfpga_module, vpr_args, option):
    with pytest.raises(openfpga_module.OpenfpgaError, match="Invalid command line"):
        shell.run(f"vpr {vpr_args} {option}")


def test_vpr_runs_after_a_rejected_option(shell, vpr_args):
    assert shell.run(f"vpr {vpr_args} --bogus_option 3", check=False).status != 0
    assert shell.run("vpr --help").status == 0
    result = shell.run(
        f"vpr {vpr_args} --device 2x2 --route_chan_width 20 --clock_modeling route"
    )
    assert "VPR succeeded" in result.log


def test_staged_flow_option_rejected_by_vpr(shell, openfpga_module, and2_files):
    # The option table only knows that the value is text; VPR wants numbers
    shell.run("set_app_option -n place.place_static_move_prob -v abc")
    read_arch = f"read_vpr_arch -f {and2_files['vpr_arch']} -l 2x2"
    with pytest.raises(openfpga_module.OpenfpgaError, match="Invalid command line"):
        shell.run(read_arch)

    shell.run("set_app_option -n place.place_static_move_prob -v 100")
    assert shell.run(read_arch).status == 0
