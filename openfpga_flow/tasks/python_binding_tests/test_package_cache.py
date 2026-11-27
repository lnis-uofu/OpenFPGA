"""The cached files of the device of a package (docs/spec_device_package.md R-27, R-49), and the
rules of the review of the format: type in place of role, device.layout, a LICENSE file, the
types of R-20 (R-5 to R-9, R-20)."""

import hashlib
import json
import shutil
import sys
from pathlib import Path

import pytest

FLOW = Path(__file__).resolve().parents[2]
PACKAGES = FLOW / "device-packages"
NAME = "k6_frac_n10_2x2"
sys.path.insert(0, str(FLOW / "scripts"))

import openfpga_package  # noqa: E402


def resum(package):
    openfpga_package.write_checksums(package)


def change_manifest(package, change):
    path = package / "device_package.json"
    manifest = json.loads(path.read_text())
    change(manifest)
    path.write_text(json.dumps(manifest, indent=2))
    resum(package)


@pytest.fixture
def copy(tmp_path):
    target = tmp_path / "packages" / NAME
    shutil.copytree(PACKAGES / NAME, target)
    return target


@pytest.fixture
def cached(copy, tmp_path, monkeypatch):
    """A copy of the package with its cached files, made by openfpga_package.py cache"""
    monkeypatch.chdir(tmp_path)
    openfpga_package.main(["cache", str(copy)])
    return copy


def report(openfpga_module, package):
    shell = openfpga_module.OpenfpgaShell()
    return shell.report_device_package(file=str(package)).data["data"]


def route(openfpga_module, package, folder, cache=True):
    """Route the reference of the package from a folder of its own, and make its bitstream:
    the log of read_device_package, and the SHA-256 of the files the flow wrote"""
    folder.mkdir()
    here = Path.cwd()
    import os

    os.chdir(folder)
    try:
        session = openfpga_module.Session()
        line = "read_device_package --file %s%s" % (package, "" if cache else " --no_cache")
        log = session.shell.run(line).log
        session.read_circuit(str(package / "reference" / "and2.blif"))
        session.pack()
        session.place()
        session.route()
        session.analysis()
        session.command("build_bitstream", file="and2.bit")
        options = {
            name: session.options[name]
            for name in ("filename.read_rr_graph_file", "filename.read_router_lookahead")
        }
    finally:
        os.chdir(here)
    sums = {
        name: hashlib.sha256((folder / name).read_bytes()).hexdigest()
        for name in ("and2.net", "and2.place", "and2.route", "and2.bit")
    }
    return log, sums, options


def test_the_cached_files_are_written_and_read_with_the_same_results(
    openfpga_module, cached, tmp_path
):
    answer = report(openfpga_module, cached)
    assert answer["cache"]["usable"] is True and answer["cache"]["reason"] is None
    assert answer["cache"]["files"] == {
        "rr_graph": "cache/rr_graph.bin",
        "router_lookahead": "cache/router_lookahead.bin",
    }
    manifest = json.loads((cached / "device_package.json").read_text())
    assert manifest["cache"]["vtr_version"] == answer["cache"]["build_vtr_version"]
    log, with_cache, options = route(openfpga_module, cached, tmp_path / "with")
    assert "Read the cached rr_graph of the device" in log
    assert options["filename.read_rr_graph_file"] == str(cached / "cache" / "rr_graph.bin")
    log, without, options = route(openfpga_module, cached, tmp_path / "without", cache=False)
    assert "--no_cache" in log and options["filename.read_rr_graph_file"] == ""
    assert with_cache == without


@pytest.mark.parametrize(
    "change, reason",
    [
        (lambda m: m["cache"].update(vtr_version="8.0.0"), "made by VTR 8.0.0"),
        (lambda m: m["device"].update(channel_width=22), "channel width changed"),
    ],
)
def test_cached_files_of_another_vtr_or_other_files_are_not_read(
    openfpga_module, cached, tmp_path, change, reason
):
    change_manifest(cached, change)
    answer = report(openfpga_module, cached)
    assert answer["cache"]["usable"] is False and reason in answer["cache"]["reason"]
    log, _, options = route(openfpga_module, cached, tmp_path / "run")
    assert "are not read, and VPR makes them" in log
    assert options["filename.read_rr_graph_file"] == ""


def test_the_cached_files_go_with_the_package(openfpga_module, cached, tmp_path, monkeypatch):
    """Reading an architecture by hand forgets the package, and the files of its device"""
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.read_device_package(cached)
    assert session.options["filename.read_rr_graph_file"].endswith("rr_graph.bin")
    session.read_openfpga_arch(str(cached / "arch" / "openfpga_arch.xml"))
    assert session.options["filename.read_rr_graph_file"] == ""
    assert session.options["filename.read_router_lookahead"] == ""


def test_cached_files_need_the_version_and_the_key(openfpga_module, cached):
    change_manifest(cached, lambda m: m.pop("cache"))
    with pytest.raises(openfpga_module.OpenfpgaError, match="cache.vtr_version"):
        report(openfpga_module, cached)


def drop_license(package):
    (package / "LICENSE").unlink()
    resum(package)


def two_dsp_maps(package):
    (package / "synthesis" / "a.v").write_text("")
    (package / "synthesis" / "b.v").write_text("")
    change_manifest(
        package,
        lambda m: m["files"].extend(
            [
                {"type": "yosys_dsp_map", "path": "synthesis/a.v"},
                {"type": "yosys_dsp_map", "path": "synthesis/b.v"},
            ]
        ),
    )


def link(package):
    (package / "synthesis" / "link.v").symlink_to(package / "synthesis" / "synth_script.ys")
    change_manifest(
        package, lambda m: m["files"].append({"type": "x_link", "path": "synthesis/link.v"})
    )


@pytest.mark.parametrize(
    "change, message",
    [
        (drop_license, "no LICENSE file"),
        (
            lambda p: change_manifest(
                p, lambda m: m["files"][0].update(role=m["files"][0].pop("type"))
            ),
            "the field is type",
        ),
        (
            lambda p: change_manifest(
                p, lambda m: m["device"].update(grid={"width": 2, "height": 2})
            ),
            "device.grid is replaced by device.layout",
        ),
        (
            lambda p: change_manifest(p, lambda m: m["device"].pop("layout")),
            "device.layout names a fixed layout",
        ),
        (
            lambda p: change_manifest(p, lambda m: m["files"][0].update(type="arch_bitstream")),
            "not one of R-20",
        ),
        (two_dsp_maps, "more than one file of the type yosys_dsp_map"),
        (link, "is a link"),
    ],
)
def test_a_package_that_breaks_a_rule_of_the_review_does_not_load(
    openfpga_module, copy, change, message
):
    change(copy)
    with pytest.raises(openfpga_module.OpenfpgaError, match=message):
        report(openfpga_module, copy)


def test_the_layout_names_the_size_that_the_architecture_gives(openfpga_module, copy):
    answer = report(openfpga_module, copy)
    assert answer["layout"] == "2x2"
    assert answer["grid"] == {"width": 4, "height": 4}  # the core and its ring of I/O
    assert {"type", "path", "bytes"} <= set(answer["files"][0])


def test_the_next_device_is_not_the_one_of_the_cached_graph(openfpga_module, cached, tmp_path):
    """A grid made from a routing resource graph that was read is fixed by it in VPR: the next
    run of the process, on another layout, makes its own grid"""
    route(openfpga_module, cached, tmp_path / "cached")
    other = PACKAGES / "k6_frac_n10_mem1k_3x2"
    session = openfpga_module.Session()
    session.read_device_package(other)
    grid = session.device().grid
    assert (
        (grid.width, grid.height)
        == (5, 4)
        == tuple(report(openfpga_module, other)["grid"].values())
    )
