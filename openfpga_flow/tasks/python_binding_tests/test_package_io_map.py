"""The I/O map of a device package (docs/spec_device_package.md, R-25 and R-48): its rules
(check_io_map), its locations in the grid of the device, the map that write_io_map writes,
and a package that has one (read_device_package)."""

import csv
import hashlib
import json
import re
import shutil
import xml.etree.ElementTree as ET
from pathlib import Path

import pytest

FLOW = Path(__file__).resolve().parents[2]
TASK = FLOW / "tasks/basic_tests/io_constraints/example_pcf/config"
PACKAGES = FLOW / "device-packages"
COLUMNS = [
    "side",
    "pin_direction",
    "pin_type",
    "row",
    "col",
    "subtile",
    "netlist_name",
    "customer_pin_alias",
]


def task_rows():
    """The I/O map of the task example_pcf, from its pin table and its I/O location map"""
    with open(TASK / "pinmap_k4_N4_tileable_40nm.csv") as f:
        pins = {row["port_name"]: row for row in csv.DictReader(f)}
    rows = []
    for io in ET.parse(TASK / "fpga_io_location.xml").getroot().iter("io"):
        pin = pins[io.get("pad")]
        rows.append(
            {
                "side": pin["orientation"],
                "pin_direction": {"in": "input", "out": "output"}[pin["GPIO_type"]],
                "pin_type": "GPIO",
                "row": io.get("y"),
                "col": io.get("x"),
                "subtile": io.get("z"),
                "netlist_name": io.get("pad"),
                "customer_pin_alias": pin["mapped_pin"],
            }
        )
    return rows


def write(path, rows, columns=COLUMNS, sep=","):
    with open(path, "w", newline="") as f:
        w = csv.DictWriter(f, fieldnames=columns, delimiter=sep, extrasaction="ignore")
        w.writeheader()
        w.writerows(rows)
    return path


@pytest.fixture
def shell(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return openfpga_module.OpenfpgaShell()


def device(shell):
    """The device of the task: k4, 4x4, and2, through the stages and the fabric"""
    shell.run("set_app_option -n router.route_chan_width -v 20")
    shell.run(f"read_vpr_arch -f {FLOW}/vpr_arch/k4_N4_tileable_40nm.xml -l 4x4")
    shell.run(f"read_openfpga_arch -f {FLOW}/openfpga_arch/k4_N4_40nm_cc_openfpga.xml")
    shell.run(f"read_circuit -f {FLOW}/benchmarks/ubm/simple_gates/and2/and2.blif")
    for stage in ("pack", "place", "route", "analysis"):
        shell.run(stage)


def test_the_map_of_the_task_follows_the_rules_and_fits_the_device(
    openfpga_module, shell, tmp_path
):
    path = write(tmp_path / "io_map.csv", task_rows())
    assert "follows its rules (16 rows)" in shell.run(f"check_io_map -f {path}").log
    device(shell)
    assert "fits the device (16 rows)" in shell.run(f"check_io_map -f {path}").log
    # Tab-separated, the columns in another order, and a global port without a location
    rows = task_rows() + [
        {
            "side": "",
            "pin_direction": "input",
            "pin_type": "CONTROL",
            "row": "",
            "col": "",
            "subtile": "",
            "netlist_name": "clk[0]",
            "customer_pin_alias": "CLK",
        }
    ]
    other = write(tmp_path / "io_map.tsv", rows, list(reversed(COLUMNS)), "\t")
    assert "fits the device (17 rows)" in shell.run(f"check_io_map -f {other}").log


@pytest.mark.parametrize(
    "change, why",
    [
        (lambda rows, cols: cols.append("color"), "the column 'color' is not one"),
        (lambda rows, cols: cols.remove("subtile"), "the column 'subtile' is missing"),
        (lambda rows, cols: rows[0].update(pin_type="IO"), "pin_type 'IO' is not GPIO"),
        (lambda rows, cols: rows[0].update(pin_direction="in"), "pin_direction 'in'"),
        (lambda rows, cols: rows[0].update(side="NORTH"), "the side 'NORTH'"),
        (lambda rows, cols: rows[0].update(row="-1"), "whole numbers"),
        (lambda rows, cols: rows[0].update(subtile=""), "all given, or all empty"),
        (lambda rows, cols: rows[0].update(row="", col="", subtile=""), "pin_type is CONTROL"),
        (lambda rows, cols: rows[1].update(netlist_name=rows[0]["netlist_name"]), "is given twice"),
        (
            lambda rows, cols: rows[2].update(customer_pin_alias=rows[0]["customer_pin_alias"]),
            "customer_pin_alias 'pad_fpga_io[0]' is given twice for the direction input",
        ),
    ],
)
def test_a_map_that_breaks_a_rule_is_refused_with_its_line(
    openfpga_module, shell, tmp_path, change, why
):
    rows, columns = task_rows(), list(COLUMNS)
    change(rows, columns)
    path = write(tmp_path / "io_map.csv", rows, columns)
    with pytest.raises(openfpga_module.OpenfpgaError, match=re.escape(why)) as error:
        shell.run(f"check_io_map -f {path}")
    assert re.search(r"io_map\.csv:\d+: ", str(error.value))


@pytest.mark.parametrize(
    "change, why",
    [
        (lambda row: row.update(row="2", col="2"), "the tile X2Y2 is not an I/O tile"),
        (lambda row: row.update(subtile="99"), "subtiles, not a subtile 99"),
    ],
)
def test_a_location_that_is_not_in_the_grid_is_refused(
    openfpga_module, shell, tmp_path, change, why
):
    rows = task_rows()
    change(rows[0])  # a pin of the top side
    path = write(tmp_path / "io_map.csv", rows)
    shell.run(f"check_io_map -f {path}")  # no device yet: the locations are not checked
    device(shell)
    with pytest.raises(openfpga_module.OpenfpgaError, match=re.escape(why)):
        shell.run(f"check_io_map -f {path}")


def test_write_io_map_writes_the_pins_of_the_fabric(openfpga_module, shell, tmp_path):
    with pytest.raises(openfpga_module.OpenfpgaError, match="no device yet"):
        shell.run("write_io_map -f first.csv")
    device(shell)
    shell.run("write_io_map -f before.csv")  # the fabric is not built: no names
    shell.run(
        f"read_openfpga_simulation_setting -f {FLOW}/openfpga_simulation_settings/"
        "auto_sim_openfpga.xml"
    )
    shell.run(
        f"link_openfpga_arch --sort_gsb_chan_node_in_edges --activity_file "
        f"{FLOW}/benchmarks/ubm/simple_gates/and2/and2.act"
    )
    shell.run("build_fabric --compress_routing")
    shell.run("write_io_map -f written.csv")
    with open(tmp_path / "written.csv") as f:
        written = list(csv.DictReader(f))
    assert list(written[0]) == COLUMNS
    # Each pin of the I/O location map of the fabric (write_fabric_io_info) is at its place, with
    # its name: a row for each I/O subtile of the grid (16 I/O tiles of 8 subtiles)
    shell.run("write_fabric_io_info --file io_location.xml --no_time_stamp")
    by_name = {r["netlist_name"]: r for r in written if r["netlist_name"]}
    ios = list(ET.parse(tmp_path / "io_location.xml").getroot().iter("io"))
    assert len(written) == 128 and len(by_name) == len(ios) == 128
    for io in ios:
        mine = by_name[io.get("pad")]
        assert (mine["col"], mine["row"], mine["subtile"]) == (
            io.get("x"),
            io.get("y"),
            io.get("z"),
        )
        assert mine["side"] == {"0": "BOTTOM", "5": "TOP"}.get(io.get("y"), mine["side"])
    with open(tmp_path / "before.csv") as f:
        assert not any(r["netlist_name"] for r in csv.DictReader(f))
    # What it writes follows the rules and fits the device
    for name in ("before.csv", "written.csv"):
        assert "fits the device" in shell.run(f"check_io_map -f {name}").log


def package_with_map(tmp_path, rows):
    """A copy of the logic package with an I/O map, its manifest and its checksums"""
    copy = tmp_path / "packages" / "k6_frac_n10_2x2"
    shutil.copytree(PACKAGES / "k6_frac_n10_2x2", copy)
    write(copy / "arch" / "io_map.csv", rows)
    manifest = json.loads((copy / "device_package.json").read_text())
    manifest["files"].append({"role": "io_map", "path": "arch/io_map.csv"})
    (copy / "device_package.json").write_text(json.dumps(manifest, indent=2))
    lines = []
    for path in sorted(p for p in copy.rglob("*") if p.is_file() and p.name != "CHECKSUMS.sha256"):
        lines.append(
            "%s  %s"
            % (hashlib.sha256(path.read_bytes()).hexdigest(), path.relative_to(copy).as_posix())
        )
    (copy / "CHECKSUMS.sha256").write_text("\n".join(lines) + "\n")
    return copy


def test_a_package_with_a_map_that_breaks_a_rule_is_not_read(
    openfpga_module, tmp_path, monkeypatch
):
    monkeypatch.chdir(tmp_path)
    rows = [
        {
            "side": "TOP",
            "pin_direction": "input",
            "pin_type": "GPIO",
            "row": "3",
            "col": "1",
            "subtile": "0",
            "netlist_name": "gfpga_pad_x[0]",
            "customer_pin_alias": "A",
        }
    ]
    good = package_with_map(tmp_path / "good", rows)
    session = openfpga_module.Session()
    assert session.read_device_package(good)["name"] == "k6_frac_n10_2x2"
    bad = package_with_map(tmp_path / "bad", [dict(rows[0], pin_type="IO")])
    session = openfpga_module.Session()
    with pytest.raises(openfpga_module.OpenfpgaError, match="cannot be used: .*io_map.csv:2: "):
        session.read_device_package(bad)
    assert session.state().inputs.vpr_arch is None
