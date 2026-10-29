"""The results of the packing, the placement and the routing made elsewhere are
read instead of the stages being run, and give the same results as running
them."""

from pathlib import Path

import pytest

from test_equivalence import Golden, in_dir, two_shells  # noqa: F401


def read_inputs(golden, shell, route_width=True):
    shell.run("set_app_option -n general.clock_modeling -v route")
    if route_width:
        shell.run(f"set_app_option -n router.route_chan_width -v {golden.chan_width}")
    shell.run(f"read_vpr_arch -f {golden.vpr_arch} -l {golden.layout}")
    shell.run(f"read_openfpga_arch -f {golden.openfpga_arch}")
    shell.run(f"read_circuit -f {golden.circuit}")


@pytest.mark.parametrize("name", ["k6_routing_test", "k4_routing_test", "k6_and2"])
def test_imported_results_equal_the_run_of_vpr(name, two_shells):  # noqa: F811
    golden = Golden(name)
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells

    def reference():
        log = golden.run_monolithic(monolithic)
        return log, golden.files(), golden.bitstream(monolithic)

    _, monolithic_files, monolithic_bits = in_dir(monolithic_dir, reference)

    def imported():
        read_inputs(golden, staged)
        for command, suffix in (
            ("read_packing", "net"),
            ("read_placement", "place"),
            ("read_routing", "route"),
        ):
            staged.run(f"{command} -f {monolithic_dir / (golden.name + '.' + suffix)}")
        log = staged.run("analysis").log
        golden.link(staged)
        post_routing = Path(f"{golden.name}.net.post_routing").read_bytes()
        return log, post_routing, golden.bitstream(staged)

    _, post_routing, staged_bits = in_dir(staged_dir, imported)

    assert post_routing == monolithic_files[f"{golden.name}.net.post_routing"]
    assert staged_bits == monolithic_bits and len(staged_bits) > 0


def test_a_stage_is_run_after_its_import_equals_the_vpr_flow(two_shells):  # noqa: F811
    """Read the packing, then place and route: same as the run of VPR"""
    golden = Golden("k6_routing_test")
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells

    def reference():
        golden.run_monolithic(monolithic)
        return golden.files(), golden.bitstream(monolithic)

    monolithic_files, monolithic_bits = in_dir(monolithic_dir, reference)

    def scenario():
        read_inputs(golden, staged)
        staged.run(f"read_packing -f {monolithic_dir / (golden.name + '.net')}")
        for stage in ("place", "route", "analysis"):
            staged.run(stage)
        golden.link(staged)
        return Path(f"{golden.name}.place").read_bytes(), golden.bitstream(staged)

    placement, staged_bits = in_dir(staged_dir, scenario)
    # The first line names the netlist file, which is not the same path here
    reference = monolithic_files[f"{golden.name}.place"]
    assert placement.split(b"\n", 1)[1] == reference.split(b"\n", 1)[1]
    assert staged_bits == monolithic_bits


def test_an_option_changed_after_an_import_keeps_the_packing(two_shells):  # noqa: F811
    """The imported packing is read again when the placement is run again with
    another seed: the placement differs, and the flow goes on"""
    golden = Golden("k6_routing_test")
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells
    in_dir(monolithic_dir, golden.run_monolithic, monolithic)
    net = monolithic_dir / (golden.name + ".net")
    place = Path(golden.name + ".place")

    def scenario():
        read_inputs(golden, staged)
        staged.run(f"read_packing -f {net}")
        assert staged.execute("place").status == 0
        first = place.read_bytes()
        staged.run("set_app_option -n place.seed -v 3")
        assert staged.execute("place").status == 0
        assert staged.execute("route").status == 0
        return first, place.read_bytes()

    first, second = in_dir(staged_dir, scenario)
    assert first != second


def test_reading_the_results_of_a_stage_needs_the_stage_before(shell):
    golden = Golden("k6_and2")
    read_inputs(golden, shell)
    for command in ("read_placement", "read_routing"):
        result = shell.execute(f"{command} -f whatever")
        assert result.status != 0
    assert "has not been reached" in shell.execute("read_placement -f whatever").log


def test_a_missing_file_is_refused(shell):
    golden = Golden("k6_and2")
    read_inputs(golden, shell)
    result = shell.execute("read_packing -f does_not_exist.net")
    assert result.status != 0
    assert "does not exist" in result.log
    # Nothing was reached
    assert shell.execute("place").status != 0


def test_routing_needs_a_fixed_channel_width(two_shells):  # noqa: F811
    golden = Golden("k6_and2")
    (staged, monolithic), (staged_dir, monolithic_dir) = two_shells
    in_dir(monolithic_dir, golden.run_monolithic, monolithic)

    def scenario():
        read_inputs(golden, staged, route_width=False)
        base = monolithic_dir / golden.name
        assert staged.execute(f"read_packing -f {base}.net").status == 0
        assert staged.execute(f"read_placement -f {base}.place").status == 0
        result = staged.execute(f"read_routing -f {base}.route")
        return result

    result = in_dir(staged_dir, scenario)
    assert result.status != 0
