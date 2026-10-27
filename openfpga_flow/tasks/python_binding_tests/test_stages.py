"""The flow that is run stage by stage: read_vpr_arch, read_circuit,
read_openfpga_arch, pack, place and route."""

import os
from pathlib import Path

import pytest

DESIGNS = {
    "and2": "simple_gates/and2/and2.blif",
    "and2_or2": "simple_gates/and2_or2/and2_or2.blif",
    "routing_test": "interface/routing_test/routing_test.blif",
}
RESULTS = ["{}.net", "{}.place", "{}.route"]


@pytest.fixture
def design(flow_dir):
    def find(name):
        path = flow_dir / "benchmarks" / "ubm" / DESIGNS[name]
        if not path.exists():
            pytest.skip(f"missing {path}")
        return path

    return find


@pytest.fixture
def staged(shell, and2_files, design):
    """Set up the stage-by-stage flow of a design, step by step"""

    class Flow:
        def __init__(self):
            self.arch = and2_files["vpr_arch"]
            self.openfpga_arch = and2_files["openfpga_arch"]
            self.shell = shell

        def options(self, chan_width=20):
            self.shell.run("set_app_option -n general.clock_modeling -v route")
            self.shell.run(f"set_app_option -n router.route_chan_width -v {chan_width}")

        def read_vpr_arch(self, layout="2x2", **kwargs):
            return self.shell.run(f"read_vpr_arch -f {self.arch} -l {layout}", **kwargs)

        def read_openfpga_arch(self, **kwargs):
            return self.shell.run(
                f"read_openfpga_arch -f {self.openfpga_arch}", **kwargs
            )

        def read_circuit(self, name="and2", **kwargs):
            return self.shell.run(f"read_circuit -f {design(name)}", **kwargs)

        def read_all(self, name="and2"):
            self.options()
            self.read_vpr_arch()
            self.read_openfpga_arch()
            self.read_circuit(name)

    return Flow()


def refused(result, text):
    assert result.status != 0
    assert text in result.log


def results_of(directory, name):
    return {
        pattern.format(name): (Path(directory) / pattern.format(name)).read_bytes()
        for pattern in RESULTS
    }


@pytest.mark.parametrize("name", sorted(DESIGNS))
def test_results_equal_the_monolithic_flow(
    staged, shell, tmp_path, and2_files, design, name
):
    staged.read_all(name)
    for command in ("pack", "place", "route"):
        shell.run(command)
    staged_results = results_of(Path.cwd(), name)

    monolithic = tmp_path / "monolithic"
    monolithic.mkdir()
    cwd = Path.cwd()
    os.chdir(monolithic)
    try:
        shell.run(
            f"vpr {and2_files['vpr_arch']} {design(name)} --clock_modeling route "
            "--device 2x2 --route_chan_width 20 --pack --place --route"
        )
    finally:
        os.chdir(cwd)
    assert results_of(monolithic, name) == staged_results


def test_the_three_inputs_make_the_device(staged):
    staged.options()
    staged.read_vpr_arch()
    staged.read_openfpga_arch()
    result = staged.read_circuit()
    assert "Build Device Grid" in result.log


def test_inputs_in_another_order(staged, shell):
    staged.options()
    staged.read_vpr_arch()
    staged.read_circuit()
    result = staged.read_openfpga_arch()
    assert "Build Device Grid" in result.log
    for command in ("pack", "place", "route"):
        shell.run(command)


def test_the_device_is_not_there_before_the_last_input(staged):
    staged.options()
    assert "Build Device Grid" not in staged.read_vpr_arch().log
    assert "Build Device Grid" not in staged.read_circuit().log


def test_missing_inputs_are_named(staged, shell):
    result = shell.run("pack", check=False)
    refused(result, "VPR_ARCH has not been read: use the command 'read_vpr_arch'")
    assert "use the command 'read_circuit'" in result.log
    assert "use the command 'read_openfpga_arch'" in result.log

    staged.options()
    staged.read_vpr_arch()
    staged.read_circuit()
    result = shell.run("pack", check=False)
    refused(
        result, "OPENFPGA_ARCH has not been read: use the command 'read_openfpga_arch'"
    )
    assert "CIRCUIT has not been read" not in result.log


def test_circuit_needs_the_architecture(staged):
    refused(staged.read_circuit(check=False), "read_vpr_arch")


def test_stages_need_the_stage_before(staged, shell):
    staged.read_all()
    refused(
        shell.run("place", check=False),
        "the stage PACKED has not been reached, use the command 'pack'",
    )
    refused(
        shell.run("route", check=False),
        "the stage PLACED has not been reached, use the command 'place'",
    )
    shell.run("pack")
    refused(
        shell.run("route", check=False),
        "the stage PLACED has not been reached, use the command 'place'",
    )


def test_reading_the_vpr_architecture_again_drops_the_circuit(staged, shell):
    staged.read_all()
    shell.run("pack")
    staged.read_vpr_arch()
    refused(shell.run("place", check=False), "PACKED has not been reached")
    refused(shell.run("pack", check=False), "CIRCUIT has not been read")
    staged.read_circuit()
    shell.run("pack")


def test_reading_the_circuit_again(staged, shell):
    staged.read_all("and2")
    shell.run("pack")
    staged.read_circuit("and2_or2")
    refused(shell.run("place", check=False), "PACKED has not been reached")
    shell.run("pack")
    shell.run("place")
    shell.run("route")
    assert Path("and2_or2.route").exists()


def test_reading_the_openfpga_architecture_again(staged, shell):
    staged.read_all()
    shell.run("pack")
    staged.read_openfpga_arch()
    refused(shell.run("place", check=False), "PACKED has not been reached")
    for command in ("pack", "place", "route"):
        shell.run(command)


def test_failed_circuit_is_dropped(staged, shell, tmp_path):
    staged.options()
    staged.read_vpr_arch()
    staged.read_openfpga_arch()
    refused(
        shell.run(f"read_circuit -f {tmp_path / 'nothing.blif'}", check=False),
        "nothing.blif",
    )
    refused(shell.run("pack", check=False), "CIRCUIT has not been read")
    staged.read_circuit()
    shell.run("pack")


def test_failed_architecture_is_dropped(staged, shell, tmp_path):
    staged.options()
    result = shell.run(
        f"read_vpr_arch -f {tmp_path / 'nothing.xml'} -l 2x2", check=False
    )
    assert result.status != 0
    refused(shell.run("pack", check=False), "VPR_ARCH has not been read")
    # A flow that was good before is not left half done
    staged.read_all()
    shell.run("pack")


def test_automatic_layout_is_refused(staged, shell):
    staged.options()
    staged.read_vpr_arch(layout="auto")
    staged.read_openfpga_arch()
    result = staged.read_circuit(check=False)
    refused(result, "sized from the result of packing")
    assert "general.device_layout" in result.log
    refused(shell.run("pack", check=False), "sized from the result of packing")
    # A fixed layout works afterwards
    staged.read_vpr_arch()
    staged.read_circuit()
    shell.run("pack")


def test_layout_of_another_name_is_refused(staged):
    staged.options()
    result = staged.read_vpr_arch(layout="9x9", check=False)
    # VPR reports the layout when the architecture is used by the device
    staged.read_openfpga_arch()
    result = staged.read_circuit(check=False)
    assert result.status != 0 or "9x9" in result.log


def test_vpr_replaces_the_data(staged, shell, and2_files):
    staged.read_all()
    shell.run("pack")
    shell.run(
        f"vpr {and2_files['vpr_arch']} {and2_files['blif']} --device 2x2 "
        "--clock_modeling route --route_chan_width 20"
    )
    # The session cannot rely on data that 'vpr' replaced
    refused(shell.run("place", check=False), "has not been reached")
    refused(shell.run("pack", check=False), "has not been read")
    staged.read_all()
    shell.run("pack")


def test_staged_flow_then_vpr_twice(staged, shell, and2_files):
    staged.read_all()
    for command in ("pack", "place", "route"):
        shell.run(command)
    for _ in range(2):
        result = shell.run(
            f"vpr {and2_files['vpr_arch']} {and2_files['blif']} --device 2x2 "
            "--clock_modeling route --route_chan_width 20"
        )
        assert "VPR succeeded" in result.log


def test_stage_again(staged, shell):
    staged.read_all()
    shell.run("pack")
    shell.run("place")
    # Running a stage again goes back to the stage it needs, on fresh data
    again = shell.run("place").log
    assert "Run the stages up to PACKED again" in again
    assert "placer_opts.seed" in again
    shell.run("route")
    assert "Run the stages up to PLACED again" in shell.run("route").log


def test_option_changes_what_the_next_stage_does(staged, shell):
    staged.read_all("routing_test")
    for command in ("pack", "place", "route"):
        shell.run(command)
    places = [Path("routing_test.place").read_bytes()]
    for seed in (2, 3):
        shell.run(f"set_app_option -n place.seed -v {seed}")
        # The option dropped the placement and the routing, not the packing
        refused(
            shell.run("route", check=False), "the stage PLACED has not been reached"
        )
        log = shell.run("place").log
        assert f"placer_opts.seed: {seed}" in log
        places.append(Path("routing_test.place").read_bytes())
        shell.run("route")
    assert len(set(places)) == 3


def test_option_of_a_stage_not_reached_changes_nothing_else(staged, shell):
    staged.read_all()
    shell.run("pack")
    shell.run("place")
    shell.run("set_app_option -n router.max_router_iterations -v 40")
    shell.run("set_app_option -n analysis.timing_report_npaths -v 3")
    result = shell.run("route")
    assert "max_router_iterations: 40" in result.log


def test_option_of_the_device_drops_the_device(staged, shell):
    staged.read_all()
    shell.run("pack")
    shell.run("set_app_option -n general.num_workers -v 2")
    # The device is created again from the inputs, which are still read
    refused(shell.run("place", check=False), "PACKED has not been reached")
    shell.run("pack")
    shell.run("place")


def test_same_value_keeps_the_results(staged, shell):
    staged.read_all()
    shell.run("pack")
    shell.run("place")
    shell.run("set_app_option -n router.route_chan_width -v 20")
    shell.run("route")


def test_channel_width_option_changes_the_device(staged, shell):
    staged.read_all()
    shell.run("pack")
    shell.run("place")
    shell.run("route")
    # The routing resource graph of the device is made for the channel width
    shell.run("set_app_option -n router.route_chan_width -v 30")
    refused(shell.run("route", check=False), "the stage PLACED has not been reached")
    shell.run("pack")
    shell.run("place")
    log = shell.run("route").log
    assert "channel width factor of 30" in log
