"""The query device: the grid, the channels, the types of tiles and where they are.

The answer is compared with what other things say of the same device: the architecture file, the
log of VPR, the packing and the placement of a design.
"""

import dataclasses
import json
import re
import xml.etree.ElementTree as ET
from pathlib import Path

import pytest

from test_equivalence import Golden

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"


@pytest.fixture
def golden():
    return Golden("k6_routing_test")


@pytest.fixture
def session(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.logs = {"circuit": session.read_circuit(golden.circuit).log}
    return session


def raw(session, command="device"):
    return session.shell.run(command).data


def tiles_of(architecture):
    """The tiles of the architecture file: name -> (width, height, {sub tile: (capacity, sites)})"""
    found = {}
    for tile in ET.parse(architecture).getroot().find("tiles"):
        sub_tiles = {}
        for sub_tile in tile.findall("sub_tile"):
            sites = [s.get("pb_type") for s in sub_tile.find("equivalent_sites")]
            sub_tiles[sub_tile.get("name")] = (int(sub_tile.get("capacity", "1")), sites)
        found[tile.get("name")] = (
            int(tile.get("width", "1")),
            int(tile.get("height", "1")),
            sub_tiles,
        )
    return found


def test_the_grid_is_the_one_vpr_sized(session, golden):
    device = session.device()
    assert device.stage == "DEVICE"
    sized = re.search(r"FPGA sized to (\d+) x (\d+)", session.logs["circuit"])
    assert (device.grid.width, device.grid.height) == (int(sized.group(1)), int(sized.group(2)))
    assert device.grid.layers == 1
    # The layout that was asked for, as the architecture names it
    assert device.grid.layout == golden.layout


def test_the_tile_types_are_the_ones_of_the_architecture(session, golden):
    device = session.device()
    wanted = tiles_of(golden.vpr_arch)
    assert {t.name for t in device.tile_types} == set(wanted)
    for tile in device.tile_types:
        width, height, sub_tiles = wanted[tile.name]
        assert (tile.width, tile.height) == (width, height)
        assert {s.name: (s.capacity, s.equivalent_blocks) for s in tile.sub_tiles} == sub_tiles
        assert tile.capacity == sum(capacity for capacity, _ in sub_tiles.values())
        assert tile.is_io == (tile.name == "io")


def test_the_counts_are_the_places_that_vpr_reports(session):
    """A tile holds as many blocks as its capacity: the 'Architecture' lines of VPR count the places"""
    log = session.pack().log
    reported = {
        name: int(n)
        for n, name in re.findall(r"Architecture\n\t\t(\d+)\tblocks of type: (\w+)", log)
    }
    device = session.device()
    assert reported
    for tile in device.tile_types:
        assert tile.slots == reported[tile.name]
        assert tile.slots == tile.count * tile.capacity
    # The query utilization counts the places the same way
    available = {t.name: t.available for t in session.utilization().block_types}
    assert available == reported


def test_the_grid_is_made_of_its_tiles(session):
    device = session.device()
    tile_area = sum(t.count * t.width * t.height for t in device.tile_types)
    empty = device.grid.width * device.grid.height - tile_area
    # What is left is the empty tiles: the corners of a grid with I/O on its sides
    assert empty == 4


def test_the_io_are_on_the_sides(session):
    device = session.device()
    sides = device.io_sides
    io = next(t for t in device.tile_types if t.is_io)
    assert (sides.left, sides.right, sides.bottom, sides.top) == (2, 2, 2, 2)
    assert sides.left + sides.right + sides.bottom + sides.top == io.count
    located = session.device(tile_type="io", limit=1000).locations
    for tile in located.tiles:
        on_side = tile.x in (0, device.grid.width - 1) or tile.y in (0, device.grid.height - 1)
        assert on_side


def test_the_locations_of_a_type(session):
    device = session.device(tile_type="clb")
    clb = next(t for t in device.tile_types if t.name == "clb")
    found = device.locations
    assert (found.type, found.total) == ("clb", clb.count)
    assert found.returned == len(found.tiles) == clb.count and not found.truncated
    inside = {
        (x, y) for x in range(1, device.grid.width - 1) for y in range(1, device.grid.height - 1)
    }
    assert {(t.x, t.y) for t in found.tiles} == inside
    assert len({(t.x, t.y, t.layer) for t in found.tiles}) == found.returned
    # The answer without a type has no locations
    assert session.device().locations is None


def test_the_locations_are_limited(session):
    found = session.device(tile_type="io", limit=3).locations
    assert (found.total, found.returned, found.truncated) == (8, 3, True)
    assert len(found.tiles) == 3


def test_the_placed_blocks_are_on_tiles_of_their_type(session):
    """Every block of the placement is at a place that the device says is of its type"""
    session.pack()
    session.place()
    where = {
        name: {(t.x, t.y) for t in session.device(tile_type=name, limit=1000).locations.tiles}
        for name in ("io", "clb")
    }
    placement = session.placement()
    assert placement.total
    for block in placement.blocks:
        assert (block.x, block.y) in where[block.type], block
    # and an I/O block uses a place of the capacity of its tile
    capacity = {t.name: t.capacity for t in session.device().tile_types}
    assert all(b.sub_tile < capacity[b.type] for b in placement.blocks)


def test_the_channel_width_that_was_set(session, golden):
    width = session.device().channel_width
    assert (width.x, width.y, width.fixed) == (golden.chan_width, golden.chan_width, True)


def test_the_channel_width_that_is_estimated(openfpga_module, golden, tmp_path, monkeypatch):
    """No width is set: the graph is made with the width that the placement estimates with"""
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    width = session.device().channel_width
    assert width.fixed is False
    assert width.x == width.y == session.options["place.place_chan_width"]


def test_the_answer_is_the_object(session):
    device = session.device(tile_type="io", limit=2)
    said = raw(session, "device --type io --limit 2")
    assert dataclasses.asdict(device) == {**said["data"], "stage": said["stage"]}


def test_the_answer_follows_its_schema(session):
    jsonschema = pytest.importorskip("jsonschema")
    schema = json.loads((SCHEMAS / "device.schema.json").read_text())
    jsonschema.Draft202012Validator.check_schema(schema)
    for command in ("device", "device --type clb", "device --type io --limit 2"):
        jsonschema.validate(raw(session, command), schema, cls=jsonschema.Draft202012Validator)


def test_a_type_that_does_not_exist(openfpga_module, session):
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        session.device(tile_type="dsp")
    assert "has no tile type 'dsp'" in str(error.value)
    assert "io" in str(error.value) and "clb" in str(error.value)


def test_a_limit_that_is_not_a_number(openfpga_module, session):
    for bad in (0, "many"):
        with pytest.raises(openfpga_module.OpenfpgaError):
            session.device(tile_type="io", limit=bad)


def test_the_device_needs_the_inputs(openfpga_module, golden, tmp_path):
    # In a process of its own: the sessions of one process share the data of VPR
    with openfpga_module.RemoteSession(cwd=tmp_path) as worker:
        with pytest.raises(openfpga_module.OpenfpgaError) as error:
            worker.device()
        assert error.value.errors[0].kind == "no_vpr_data"
    session = openfpga_module.Session()
    session.options["general.clock_modeling"] = "route"
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        session.device()
    found = error.value.errors[0]
    assert found.kind == "stage_not_reached" and found.data == {"stage": "DEVICE"}
    assert "read_circuit" in found.hint


def test_the_device_is_the_same_after_the_stages(session):
    before = session.device()
    session.pack()
    session.place()
    session.route()
    assert session.device() == before
