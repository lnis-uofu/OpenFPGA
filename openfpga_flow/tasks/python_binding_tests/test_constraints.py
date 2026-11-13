"""The inputs that change a flow: constraints of VPR, timing constraints, the channel width, and
what setting them drops.

Each is checked by its effect on the flow: where a block is placed, what the clock requires of a
path, how wide the channels are.
"""

import re
from pathlib import Path

import pytest

from test_equivalence import Golden

FLOW = ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]
SDC = """create_clock -period {period} clk
set_input_delay 0 -clock clk [get_ports {{*}}]
set_output_delay 0 -clock clk [get_ports {{*}}]
"""


@pytest.fixture
def golden():
    return Golden("k6_routing_test")


@pytest.fixture
def work(tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return tmp_path


@pytest.fixture
def session(openfpga_module, golden, work):
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    return session


def run(session, *stages):
    for stage in stages or ("pack", "place", "route", "analysis"):
        getattr(session, stage)()
    return session


def floorplan(path, **blocks):
    """A constraints file of VPR: each block (an atom name) in a tile"""
    partitions = "".join(
        f'<partition name="p{index}"><add_atom name_pattern="{name}"/>'
        f'<add_region x_low="{x}" y_low="{y}" x_high="{x}" y_high="{y}"/></partition>'
        for index, (name, (x, y)) in enumerate(blocks.items())
    )
    Path(path).write_text(
        f'<vpr_constraints tool_name="vpr"><partition_list>{partitions}</partition_list>'
        "</vpr_constraints>"
    )
    return path


def where(session, name):
    block = session.placement(prefix=name).blocks[0]
    return (block.x, block.y)


def free_tile(session, name):
    """A tile of inputs and outputs that is not the one where the block is"""
    here = where(session, name)
    return next(
        (t.x, t.y) for t in session.device(tile_type="io").locations.tiles if (t.x, t.y) != here
    )


# The constraints of VPR -----------------------------------------------------------------------


def test_a_floorplan_puts_a_block_where_it_says(session, work):
    run(session, "pack", "place")
    tile = free_tile(session, "out:OUT0")
    change = session.read_vpr_constraints(floorplan(work / "floor.xml", **{"out:OUT0": tile}))
    assert change.option == "filename.read_vpr_constraints_file"
    assert change.before == "" and change.after == str(work / "floor.xml")
    assert change.dropped == ["DEVICE", "PACKED", "PLACED"]
    run(session, "pack", "place")
    assert where(session, "out:OUT0") == tile
    assert (work / "routing_test.place").read_text().count("out:OUT0") == 1


def test_a_floorplan_of_two_blocks_keeps_both(session, work):
    run(session, "pack", "place")
    here = {name: where(session, name) for name in ("out:OUT0", "out:OUT1")}
    tiles = [t for t in session.device(tile_type="io").locations.tiles]
    free = [(t.x, t.y) for t in tiles if (t.x, t.y) not in here.values()]
    wanted = {"out:OUT0": free[0], "out:OUT1": free[-1]}
    session.read_vpr_constraints(floorplan(work / "floor.xml", **wanted))
    run(session)
    assert {name: where(session, name) for name in wanted} == wanted
    assert session.state().reached == FLOW


def test_taking_the_floorplan_away_gives_the_flow_back(session, work):
    run(session, "pack", "place")
    unconstrained = (work / "routing_test.place").read_bytes()
    tile = free_tile(session, "out:OUT0")
    session.read_vpr_constraints(floorplan(work / "floor.xml", **{"out:OUT0": tile}))
    run(session, "pack", "place")
    assert (work / "routing_test.place").read_bytes() != unconstrained
    change = session.read_vpr_constraints(None)
    assert (change.before, change.after) == (str(work / "floor.xml"), "")
    assert change.dropped == ["DEVICE", "PACKED", "PLACED"]
    run(session, "pack", "place")
    assert (work / "routing_test.place").read_bytes() == unconstrained


def test_a_relative_path_is_made_whole(session, work):
    floorplan(work / "floor.xml")
    change = session.read_vpr_constraints("floor.xml")
    assert change.after == str(work / "floor.xml")


def test_a_file_that_does_not_exist_changes_nothing(session, work):
    run(session)
    state = session.state()
    for call in (session.read_vpr_constraints, session.read_sdc):
        with pytest.raises(FileNotFoundError) as error:
            call(work / "missing.xml")
        assert "missing.xml" in str(error.value)
    assert session.state() == state
    assert session.state().reached == FLOW


# The timing constraints -----------------------------------------------------------------------


def paths(session):
    return session.critical_paths(count=1).paths[0]


def test_the_clock_period_is_what_a_path_requires(session, work):
    run(session)
    assert paths(session).data_required_ns == 0
    (work / "ten.sdc").write_text(SDC.format(period=10))
    change = session.read_sdc(work / "ten.sdc")
    assert change.dropped == FLOW
    run(session)
    path = paths(session)
    assert path.data_required_ns == pytest.approx(10, abs=1e-6)
    assert path.slack_ns == pytest.approx(10 - path.data_arrival_ns, abs=1e-6)
    assert path.slack_ns > 0
    summary = session.timing_summary()
    assert summary.setup.worst_negative_slack_ns == 0 and summary.setup.total_negative_slack_ns == 0


def test_a_period_that_is_too_short_fails_the_timing(session, work):
    (work / "short.sdc").write_text(SDC.format(period=0.5))
    session.read_sdc(work / "short.sdc")
    run(session)
    path = paths(session)
    assert path.data_required_ns == pytest.approx(0.5, abs=1e-6)
    summary = session.timing_summary()
    assert summary.setup.worst_negative_slack_ns == pytest.approx(path.slack_ns, abs=1e-6)
    assert summary.setup.worst_negative_slack_ns < 0
    assert summary.setup.total_negative_slack_ns <= summary.setup.worst_negative_slack_ns


def test_a_new_sdc_replaces_the_old_one(session, work):
    (work / "ten.sdc").write_text(SDC.format(period=10))
    (work / "twenty.sdc").write_text(SDC.format(period=20))
    session.read_sdc(work / "ten.sdc")
    run(session)
    change = session.read_sdc(work / "twenty.sdc")
    assert (change.before, change.after) == (str(work / "ten.sdc"), str(work / "twenty.sdc"))
    run(session)
    assert paths(session).data_required_ns == pytest.approx(20, abs=1e-6)


def test_clearing_the_constraints_tells_which_there_were(session, work):
    assert session.clear_constraints() == []
    (work / "ten.sdc").write_text(SDC.format(period=10))
    session.read_sdc(work / "ten.sdc")
    cleared = session.clear_constraints()
    assert [c.option for c in cleared] == ["filename.sdc_file"]
    run(session)
    assert paths(session).data_required_ns == 0
    assert session.options["filename.sdc_file"] == ""


# The channel width ----------------------------------------------------------------------------


def test_the_channel_width_is_the_one_that_is_set(session):
    run(session, "pack", "place")
    change = session.set_route_chan_width(24)
    assert (change.before, change.after) == (20, 24)
    assert change.dropped == ["DEVICE", "PACKED", "PLACED"]
    run(session)
    device = session.device()
    assert device.channel_width.fixed and device.channel_width.x == device.channel_width.y == 24
    congestion = session.congestion()
    assert (congestion.channel_width.x, congestion.channel_width.y) == (24, 24)
    assert session.run_report().routing.channel_width == 24


def test_a_width_that_is_the_same_drops_nothing(session):
    run(session)
    change = session.set_route_chan_width(20)
    assert (change.before, change.after, change.dropped) == (20, 20, [])
    assert session.state().reached == FLOW


# Any option -----------------------------------------------------------------------------------


def test_an_option_says_which_stages_it_dropped(session):
    run(session)
    change = session.set_option("place.seed", 3)
    assert (change.option, change.before, change.after) == ("place.seed", 1, 3)
    assert change.dropped == ["PLACED", "ROUTED", "ANALYZED"]
    assert session.state().reached == ["DEVICE", "PACKED"]
    # An option of a stage that is not reached changes nothing else
    assert session.set_option("router.max_router_iterations", 40).dropped == []
    assert session.set_option("place.seed", 3).dropped == []
    # Each stage takes the new value when it runs
    run(session, "place", "route", "analysis")
    assert session.state().reached == FLOW


def test_an_option_with_no_default_value_can_be_reset(session, work):
    """The default of a file option is empty, which a command line cannot say"""
    (work / "ten.sdc").write_text(SDC.format(period=10))
    session.read_sdc(work / "ten.sdc")
    session.options.reset("filename.sdc_file")
    assert session.options["filename.sdc_file"] == ""
    with pytest.raises(Exception, match="does not exist"):
        session.options.reset("no.such_option")


# What the journal and the workers do with them ------------------------------------------------


def test_the_journal_replays_the_constraints(openfpga_module, golden, work, monkeypatch):
    first = work / "first"
    first.mkdir()
    monkeypatch.chdir(first)
    session = openfpga_module.Session(journal=work / "journal")
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    run(session, "pack", "place")
    tile = free_tile(session, "out:OUT0")
    (first / "ten.sdc").write_text(SDC.format(period=10))
    session.read_vpr_constraints(floorplan(first / "floor.xml", **{"out:OUT0": tile}))
    session.read_sdc("ten.sdc")
    session.read_vpr_constraints(None)
    session.read_vpr_constraints(first / "floor.xml")
    run(session)
    required = paths(session).data_required_ns
    text = (work / "journal" / "replay.openfpga").read_text()
    assert f"set_app_option --name filename.sdc_file --value {first / 'ten.sdc'}" in text
    assert "reset_app_option --name filename.read_vpr_constraints_file" in text
    assert (
        "session.options.reset('filename.read_vpr_constraints_file')"
        in (work / "journal" / "replay.py").read_text()
    )

    second = work / "second"
    second.mkdir()
    monkeypatch.chdir(second)
    replayed = openfpga_module.Session.replay(work / "journal")
    assert where(replayed, "out:OUT0") == tile
    assert paths(replayed).data_required_ns == pytest.approx(required)


def test_a_worker_takes_them_too(openfpga_module, golden, work):
    with openfpga_module.RemoteSession(cwd=work / "worker") as worker:
        change = worker.set_route_chan_width(22)
        assert (change.before, change.after) == (-1, 22)
        assert worker.options["router.route_chan_width"] == 22
        (work / "worker" / "a.sdc").write_text(SDC.format(period=5))
        assert worker.read_sdc(work / "worker" / "a.sdc").after == str(work / "worker" / "a.sdc")
        assert [c.option for c in worker.clear_constraints()] == ["filename.sdc_file"]
