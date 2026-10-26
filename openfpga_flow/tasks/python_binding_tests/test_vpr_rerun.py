"""Running VPR several times in one process.

VPR keeps its data in process-wide contexts. 'vpr' keeps it alive for the
commands that follow, so it used to crash when run a second time.
"""

import pytest

FREED_PREVIOUS = "Free the data of the previous VPR run"


@pytest.fixture
def vpr_cmd(and2_files):
    def make(command="vpr", chan_width=20):
        return (
            f"{command} {and2_files['vpr_arch']} {and2_files['blif']} "
            f"--clock_modeling route --device 2x2 --route_chan_width {chan_width}"
        )

    return make


def test_vpr_twice(shell, vpr_cmd):
    shell.run(vpr_cmd())
    second = shell.run(vpr_cmd())
    assert "VPR succeeded" in second.log
    assert FREED_PREVIOUS in second.log


def test_vpr_three_times(shell, vpr_cmd):
    for _ in range(3):
        assert "VPR succeeded" in shell.run(vpr_cmd()).log


def test_vpr_after_standalone(shell, vpr_cmd):
    shell.run(vpr_cmd("vpr_standalone"))
    assert "VPR succeeded" in shell.run(vpr_cmd()).log


def test_standalone_after_vpr(shell, vpr_cmd):
    shell.run(vpr_cmd())
    result = shell.run(vpr_cmd("vpr_standalone"))
    assert "VPR succeeded" in result.log
    assert FREED_PREVIOUS in result.log


def test_vpr_after_failed_vpr(shell, vpr_cmd, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError):
        shell.run("vpr /nonexistent/arch.xml /nonexistent/design.blif")
    assert "VPR succeeded" in shell.run(vpr_cmd()).log


def test_vpr_after_failure_with_arch_loaded(shell, vpr_cmd, and2_files):
    # VPR fails once the architecture is loaded, leaving data in the contexts
    bad = vpr_cmd().replace(
        str(and2_files["blif"]), str(and2_files["unsupported_blif"])
    )
    failed = shell.run(bad, check=False)
    assert failed.status != 0
    assert "no such model exists in the architecture" in failed.log
    assert "VPR succeeded" in shell.run(vpr_cmd()).log


def test_failed_vpr_releases_previous_run(shell, vpr_cmd, and2_files):
    shell.run(vpr_cmd())
    bad = vpr_cmd().replace(
        str(and2_files["blif"]), str(and2_files["unsupported_blif"])
    )
    failed = shell.run(bad, check=False)
    assert failed.status != 0
    assert FREED_PREVIOUS in failed.log
    assert "VPR succeeded" in shell.run(vpr_cmd()).log


def test_commands_after_rerun(shell, vpr_cmd, and2_files):
    """The data of the second run is what the next commands work on"""
    shell.run(vpr_cmd())
    shell.run(vpr_cmd())
    shell.run(f"read_openfpga_arch -f {and2_files['openfpga_arch']}")
    shell.run(f"read_openfpga_simulation_setting -f {and2_files['sim_setting']}")
    shell.run(
        f"link_openfpga_arch --activity_file {and2_files['activity']} "
        "--sort_gsb_chan_node_in_edges"
    )
    shell.run("build_fabric --compress_routing")
