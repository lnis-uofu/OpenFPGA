"""The data of the views of a design (docs/plan_design_views.md, phase 1): export_view and
view_tile, their versions and caches, and their agreement with the queries the model uses."""

import os
from pathlib import Path

import pytest

from test_design_hierarchy import make_design

FLOW = Path(__file__).resolve().parents[2]
PACKAGE = FLOW / "device-packages" / "k6_frac_n10_2x2"


@pytest.fixture(scope="module")
def flow(openfpga_module, tmp_path_factory):
    """A session of a hierarchical design, routed and analyzed, in a folder of its own"""
    folder = tmp_path_factory.mktemp("views")
    here = os.getcwd()
    os.chdir(folder)
    try:
        session = openfpga_module.Session()
        session.read_device_package(PACKAGE)
        session.read_design(make_design(folder / "d"))
        session.run_flow()
        yield session, folder
    finally:
        os.chdir(here)


def export(openfpga_module, session, folder, kind, name=None, **options):
    answer = session.export_view(kind, folder / (name or kind + ".bin"), **options)
    return answer, openfpga_module.read_view(answer["file"])


def tile(openfpga_module, session, folder, kind, level, x=0, y=0):
    answer = session.view_tile(
        kind, level, x, y, folder / ("%s_%d_%d_%d.bin" % (kind, level, x, y))
    )
    return openfpga_module.read_view(answer["file"])


def rows(view, table):
    """The rows of a table as dicts"""
    columns = view["tables"][table]
    names = list(columns)
    count = len(columns[names[0]]) if names else 0
    return [{n: columns[n][i] for n in names} for i in range(count)]


# P1.2: device, placement, pins; versions -------------------------------------------------------


def test_the_device_has_the_tiles_and_channels_of_the_device_query(openfpga_module, flow):
    session, folder = flow
    answer, view = export(openfpga_module, session, folder, "device")
    device = session.device()
    assert view["kind"] == "device" and answer["tables"]["tiles"] == len(rows(view, "tiles"))
    assert view["meta"]["grid"]["width"] == device.grid.width
    names = view["tables"]["tile_types"]["name"]
    counted = {}
    for t in rows(view, "tiles"):
        counted[names[t["type"]]] = counted.get(names[t["type"]], 0) + 1
    assert counted == {t.name: t.count for t in device.tile_types if t.count}
    assert set(view["tables"]["chanx"]["width"]) <= {0, device.channel_width.x}


def test_a_file_of_the_current_version_is_not_written_again(openfpga_module, flow):
    session, folder = flow
    first, _ = export(openfpga_module, session, folder, "device")
    again, _ = export(openfpga_module, session, folder, "device")
    assert again["cached"] is True and again["version"] == first["version"]
    assert again["tables"] is None


def test_the_blocks_are_where_the_placement_query_says(openfpga_module, flow):
    session, folder = flow
    _, view = export(openfpga_module, session, folder, "placement")
    placed = {
        b.name: (b.type, b.x, b.y, b.sub_tile) for b in session.placement(limit=100000).blocks
    }
    types = view["tables"]["block_types"]["name"]
    exported = {
        b["name"]: (types[b["type"]], b["x"], b["y"], b["sub_tile"]) for b in rows(view, "blocks")
    }
    assert exported == placed
    # The whole placement has every block, in the order of its id
    assert [b["id"] for b in rows(view, "blocks")] == list(range(len(placed)))
    assert rows(view, "regions") == []


def test_the_ports_are_on_the_io_sites_of_their_blocks(openfpga_module, flow):
    session, folder = flow
    _, view = export(openfpga_module, session, folder, "pins")
    sites = rows(view, "sites")
    ports = rows(view, "ports")
    io = [b for b in session.placement(limit=100000).blocks if b.type == "io"]
    # The ports that synthesis kept (it drops the inputs that drive nothing)
    assert {p["name"] for p in ports} == {
        b.name[4:] if b.name.startswith("out:") else b.name for b in io
    }
    assert {p["dir"] for p in ports if p["name"].startswith("y")} == {1}
    assert {p["dir"] for p in ports if p["name"].startswith("a")} == {0}
    where = {b.name: (b.x, b.y, b.sub_tile) for b in io}
    for port in ports:
        site = sites[port["site"]]
        block = port["name"] if port["dir"] == 0 else "out:" + port["name"]
        assert (site["x"], site["y"], site["sub_tile"]) == where[block]
    assert view["meta"]["sides"] == ["left", "right", "bottom", "top", "inside"]


def test_the_routes_are_those_of_net_route(openfpga_module, flow):
    session, folder = flow
    _, view = export(openfpga_module, session, folder, "routing")
    types = view["meta"]["node_types"]
    nodes = rows(view, "nodes")
    for net in rows(view, "nets"):
        mine = nodes[net["node_start"] : net["node_start"] + net["node_count"]]
        theirs = session.net_route(net["name"]).nodes
        assert [
            (types[n["type"]], n["xlow"], n["ylow"], n["xhigh"], n["yhigh"], n["ptc"], n["parent"])
            for n in mine
        ] == [
            (n.type, n.xlow, n.ylow, n.xhigh, n.yhigh, n.ptc, -1 if n.parent is None else n.parent)
            for n in theirs
        ]
    congestion = session.congestion()
    used = sum(view["tables"]["chanx_use"]["used"])
    assert used == congestion.summary.chanx.used
    assert sum(view["tables"]["chany_use"]["used"]) == congestion.summary.chany.used
    assert sum(view["tables"]["chanx_use"]["capacity"]) == congestion.summary.chanx.capacity


def test_the_slack_of_each_net_is_the_one_of_net_info(openfpga_module, flow):
    session, folder = flow
    _, view = export(openfpga_module, session, folder, "timing", paths=3)
    # No names: a net is its row, the number that clusters and routing give it
    assert "name" not in view["tables"]["nets"] and "name" not in view["tables"]["blocks"]
    _, clusters = export(openfpga_module, session, folder, "clusters")
    names = {n["net"]: n["name"] for n in rows(clusters, "nets")}
    _, routing = export(openfpga_module, session, folder, "routing")
    assert {n["net"]: n["name"] for n in rows(routing, "nets")}.items() <= names.items()
    checked = 0
    for index, net in enumerate(rows(view, "nets")):
        if index not in names:
            continue
        net["name"] = names[index]
        timing = session.net_info(net["name"]).timing
        if timing is None:
            assert net["slack_ns"] == float("inf")
            continue
        assert net["slack_ns"] == pytest.approx(timing.slack_ns, abs=1e-4)
        assert net["criticality"] == pytest.approx(timing.criticality, abs=1e-4)
        checked += 1
    assert checked > 0
    # A block is its row, the id that placement and clusters give it
    block_ids = {b["id"]: b["name"] for b in rows(clusters, "blocks")}
    assert sorted(block_ids) == list(range(len(rows(view, "blocks"))))
    paths = session.critical_paths(count=3).paths
    exported = rows(view, "paths")
    points = rows(view, "points")
    assert [round(p["slack_ns"], 4) for p in exported] == [round(p.slack_ns, 4) for p in paths]
    for mine, theirs in zip(exported, paths):
        assert [
            q["pin"]
            for q in points[mine["point_start"] : mine["point_start"] + mine["point_count"]]
        ] == [q.pin for q in theirs.points]


# P1.4: atoms and clusters ----------------------------------------------------------------------


def test_the_atoms_and_the_clusters_agree_with_block_info(openfpga_module, flow):
    session, folder = flow
    _, atoms = export(openfpga_module, session, folder, "atoms")
    _, clusters = export(openfpga_module, session, folder, "clusters")
    report = session.cluster_report(stats=True)
    assert len(rows(clusters, "blocks")) == report.stats.num_clusters
    names = clusters["tables"]["atoms"]["name"]
    for block, span in zip(rows(clusters, "blocks"), rows(clusters, "block_atoms")):
        mine = names[span["atom_start"] : span["atom_start"] + span["atom_count"]]
        assert sorted(mine) == sorted(a.name for a in session.block_info(block["name"]).atoms)
    assert len(names) == atoms["meta"]["total_atoms"] == len(rows(atoms, "atoms"))
    # Every pin of an atom is on a net of the table, and a net's driver is one of its pins
    pins = rows(atoms, "pins")
    for net_index, net in enumerate(rows(atoms, "nets")):
        if net["driver"] >= 0:
            assert pins[net["driver"]]["net"] == net_index and pins[net["driver"]]["dir"] == 1


def test_the_module_of_each_atom_is_inferred(openfpga_module, flow):
    session, folder = flow
    _, view = export(openfpga_module, session, folder, "atoms")
    modules = view["tables"]["modules"]["name"]
    found = {modules[a["module"]] for a in rows(view, "atoms")}
    assert {"", "u_wide", "u_narrow0", "u_narrow1"} & found
    by = view["meta"]["module_by"]
    assert sum(by.values()) == view["meta"]["total_atoms"] and by["net"] > 0
    # The scope of a module: its atoms and those of the modules in it
    _, wide = export(openfpga_module, session, folder, "atoms", "wide.bin", module="u_wide")
    assert wide["meta"]["scope"] == "module u_wide"
    inside = {modules_of(wide)[a["module"]] for a in rows(wide, "atoms")}
    assert inside and all(m == "u_wide" or m.startswith("u_wide.") for m in inside)
    # A net that leaves the scope says so
    assert any(n["inside"] < n["pins"] for n in rows(wide, "nets"))


def modules_of(view):
    return view["tables"]["modules"]["name"]


def test_a_cone_and_the_limits_of_a_scope(openfpga_module, flow):
    session, folder = flow
    _, everything = export(openfpga_module, session, folder, "atoms")
    first = rows(everything, "atoms")[0]["name"]
    _, cone = export(openfpga_module, session, folder, "atoms", "cone.bin", cone=first, depth=1)
    assert first in cone["tables"]["atoms"]["name"]
    assert len(rows(cone, "atoms")) <= len(rows(everything, "atoms"))
    with pytest.raises(openfpga_module.OpenfpgaError, match="give --module or --cone"):
        session.export_view("atoms", folder / "x.bin", limit=2)
    with pytest.raises(openfpga_module.OpenfpgaError, match="no atom or net"):
        session.export_view("atoms", folder / "x.bin", cone="nope")
    with pytest.raises(openfpga_module.OpenfpgaError, match="There is no view"):
        session.export_view("floorplan", folder / "x.bin")


# P1.6 and P1.7: tiles --------------------------------------------------------------------------


def test_the_tiles_of_level_0_hold_the_blocks_and_the_coarse_ones_count_them(openfpga_module, flow):
    session, folder = flow
    g0 = tile(openfpga_module, session, folder, "grid", 0)
    _, device = export(openfpga_module, session, folder, "device")
    assert len(rows(g0, "tiles")) == len(rows(device, "tiles"))  # a 4x4 grid is one tile
    p0 = tile(openfpga_module, session, folder, "placement", 0)
    placed = {b.name for b in session.placement(limit=100000).blocks}
    assert {b["name"] for b in rows(p0, "blocks")} == placed
    p1 = tile(openfpga_module, session, folder, "placement", 1)
    assert p1["meta"]["cell"] == 2 and len(rows(p1, "cells")) == 256
    assert sum(p1["tables"]["cells"]["used"]) == len(placed)
    for cell in rows(p1, "cells"):
        if cell["slots"] > 0:
            assert cell["value"] == pytest.approx(cell["used"] / cell["slots"])
    with pytest.raises(openfpga_module.OpenfpgaError, match="outside the device"):
        session.view_tile("grid", 0, 5, 0, folder / "x.bin")
    with pytest.raises(openfpga_module.OpenfpgaError, match="There is no tile"):
        session.view_tile("pins", 0, 0, 0, folder / "x.bin")


def test_the_route_tiles_hold_the_routes_and_the_coarse_ones_the_congestion(openfpga_module, flow):
    session, folder = flow
    _, routing = export(openfpga_module, session, folder, "routing")
    r0 = tile(openfpga_module, session, folder, "routing", 0)
    names = r0["tables"]["nets"]["name"]
    by_net = {}
    for node in rows(r0, "nodes"):
        by_net.setdefault(names[node["net"]], set()).add(node["index"])
    for net in rows(routing, "nets"):
        assert by_net[net["name"]] == set(range(net["node_count"]))
    c0 = tile(openfpga_module, session, folder, "channels", 0)
    wires = rows(c0, "wires")
    assert wires and all(w["type"] in (4, 5) for w in wires)
    assert sum(1 for w in wires if w["used"]) == sum(
        1 for n in rows(routing, "nodes") if n["type"] in (4, 5)
    )
    c1 = tile(openfpga_module, session, folder, "channels", 1)
    congestion = session.congestion()
    summary = congestion.summary
    assert sum(c1["tables"]["cells"]["used"]) == summary.chanx.used + summary.chany.used
    assert sum(c1["tables"]["cells"]["capacity"]) == summary.chanx.capacity + summary.chany.capacity
    t0 = tile(openfpga_module, session, folder, "timing", 0)
    assert {b["name"] for b in rows(t0, "blocks")} == {
        b.name for b in session.placement(limit=100000).blocks
    }
    t1 = tile(openfpga_module, session, folder, "timing", 1)
    worst = min(b["slack_ns"] for b in rows(t0, "blocks"))
    assert min(t1["tables"]["cells"]["value"]) == pytest.approx(worst)


# Sessions of their own: last, since a new session resets the data of VPR of the process, which
# the session of the module fixture reads -------------------------------------------------------


def test_a_stage_that_runs_again_gives_a_new_version(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.read_device_package(PACKAGE)
    session.read_design(make_design(tmp_path / "d"))
    session.pack()
    session.place()
    first = session.export_view("placement", tmp_path / "p.bin")
    session.options["place.seed"] = 7
    session.place()
    second = session.export_view("placement", tmp_path / "p.bin")
    assert second["version"] != first["version"] and second["cached"] is False


# P1.3: routing and timing ----------------------------------------------------------------------


def test_views_are_not_in_the_journal(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session(journal=tmp_path / "journal")
    session.read_device_package(PACKAGE)
    session.read_design(make_design(tmp_path / "d"))
    session.export_view("device", tmp_path / "d.bin")
    session.view_tile("grid", 0, 0, 0, tmp_path / "g.bin")
    script = (tmp_path / "journal" / "replay.openfpga").read_text()
    assert "export_view" not in script and "view_tile" not in script


def test_the_sites_have_the_pins_of_the_package_and_the_ports_their_block(
    openfpga_module, tmp_path, monkeypatch
):
    from test_package_io_map import package_with_map

    map_rows = [
        {
            "side": "BOTTOM",
            "pin_direction": "inout",
            "pin_type": "GPIO",
            "row": 0,
            "col": 1,
            "subtile": s,
            "netlist_name": "gfpga_pad_%d" % s,
            "customer_pin_alias": "P%d" % s,
        }
        for s in range(3)
    ]
    package = package_with_map(tmp_path, map_rows)
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.read_device_package(package)
    session.read_design(make_design(tmp_path / "d"))
    session.pack()
    session.place()
    view = openfpga_module.read_view(session.export_view("pins", tmp_path / "pins.bin")["file"])
    assert view["meta"]["io_map_file"] == "io_map.csv"
    named = {
        (s["x"], s["y"], s["sub_tile"]): s["package_pin"]
        for s in rows(view, "sites")
        if s["package_pin"]
    }
    assert named == {(1, 0, 0): "P0", (1, 0, 1): "P1", (1, 0, 2): "P2"}
    placed = {b.name for b in session.placement(limit=100000).blocks}
    assert all(p["block"] in placed for p in rows(view, "ports"))


def test_a_view_needs_its_stage(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.read_device_package(PACKAGE)
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.export_view("placement", tmp_path / "p.bin")
    with pytest.raises(openfpga_module.OpenfpgaError):
        session.view_tile("routing", 0, 0, 0, tmp_path / "r.bin")
