"""The commands after the stages (repack and the bitstream commands) follow the
stages of the flow that is run stage by stage"""

import os
from pathlib import Path

import pytest

from test_stages import refused, staged, design  # noqa: F401


@pytest.fixture
def downstream(staged, shell, and2_files):  # noqa: F811
    """The commands after the stages, with the files they read"""

    class Downstream:
        def link(self, shell=shell):
            shell.run(f"read_openfpga_simulation_setting -f {and2_files['sim_setting']}")
            shell.run(
                f"link_openfpga_arch --activity_file {and2_files['activity']} "
                "--sort_gsb_chan_node_in_edges"
            )
            shell.run("build_fabric --compress_routing")

        def stages(self):
            for command in ("pack", "place", "route", "analysis"):
                shell.run(command)

        def bitstream(self, name="out.bit", shell=shell):
            shell.run("repack")
            shell.run("build_architecture_bitstream")
            shell.run("build_fabric_bitstream")
            shell.run(f"write_fabric_bitstream --file {name} --format plain_text")
            return Path(name).read_bytes()

    return Downstream()


def test_bitstream_of_the_stages_equals_the_monolithic_flow(
    staged, shell, downstream, and2_files, design, tmp_path, openfpga_module  # noqa: F811
):
    staged.read_all()
    downstream.stages()
    downstream.link()
    staged_bits = downstream.bitstream()

    monolithic = tmp_path / "monolithic"
    monolithic.mkdir()
    cwd = Path.cwd()
    os.chdir(monolithic)
    try:
        # The data of a flow is not released: another shell runs the other one
        other = openfpga_module.OpenfpgaShell()
        other.run(f"read_openfpga_arch -f {and2_files['openfpga_arch']}")
        other.run(
            f"vpr {and2_files['vpr_arch']} {design('and2')} --clock_modeling route "
            "--device 2x2 --route_chan_width 20 --pack --place --route --analysis"
        )
        downstream.link(other)
        monolithic_bits = downstream.bitstream(shell=other)
    finally:
        os.chdir(cwd)
    assert staged_bits == monolithic_bits


def test_commands_after_vpr_are_not_checked_against_stages(shell, downstream, and2_files):
    shell.run(f"read_openfpga_arch -f {and2_files['openfpga_arch']}")
    shell.run(
        f"vpr {and2_files['vpr_arch']} {and2_files['blif']} --clock_modeling route "
        "--device 2x2 --route_chan_width 20 --pack --place --route --analysis"
    )
    downstream.link()
    assert downstream.bitstream()


def test_link_needs_vpr_or_the_stages(staged, shell, downstream, and2_files):  # noqa: F811
    staged.read_all()
    shell.run("pack")
    shell.run("place")
    shell.run("route")
    shell.run(f"read_openfpga_simulation_setting -f {and2_files['sim_setting']}")
    # The stages up to the analysis do what 'vpr' does
    result = shell.run("link_openfpga_arch", check=False)
    assert "Command 'vpr' is required" in result.log
    shell.run("analysis")
    downstream.link()


STALE = [
    # Command, the stages to run again, the command that makes its stage
    ("repack", ["place"], "route"),
    ("build_architecture_bitstream", ["place", "route", "analysis"], "repack"),
    (
        "build_fabric_bitstream",
        ["place", "route", "analysis"],
        "build_architecture_bitstream",
    ),
    (
        "write_fabric_bitstream --file x.bit --format plain_text",
        ["place", "route", "analysis"],
        "build_architecture_bitstream",
    ),
]


@pytest.mark.parametrize("command,again,needed", STALE)
def test_stages_run_again_drop_what_follows(
    staged, shell, downstream, command, again, needed  # noqa: F811
):
    staged.read_all()
    downstream.stages()
    downstream.link()
    downstream.bitstream()
    for stage in again:
        shell.run(stage)
    refused(shell.run(command, check=False), f"Use the command '{needed}' first")


def test_linked_architecture_does_not_outlive_the_vpr_data(
    staged, shell, downstream  # noqa: F811
):
    staged.read_all()
    downstream.stages()
    downstream.link()
    downstream.bitstream()
    for stage in ("place", "route", "analysis"):
        shell.run(stage)
    # What link_openfpga_arch made refers to the data of the previous run of VPR
    refused(shell.run("repack", check=False), "linked to the data of an earlier run")
    refused(
        shell.run("build_fabric", check=False), "linked to the data of an earlier run"
    )


def test_analysis_again_keeps_the_bitstream(staged, shell, downstream):  # noqa: F811
    staged.read_all()
    downstream.stages()
    downstream.link()
    first = downstream.bitstream("first.bit")
    shell.run("analysis")
    shell.run("write_fabric_bitstream --file again.bit --format plain_text")
    assert Path("again.bit").read_bytes() == first


def test_option_of_the_placement_drops_the_bitstream(staged, shell, downstream):  # noqa: F811
    staged.read_all()
    downstream.stages()
    downstream.link()
    downstream.bitstream()
    shell.run("set_app_option -n place.seed -v 2")
    refused(shell.run("repack", check=False), "Use the command 'route' first")
