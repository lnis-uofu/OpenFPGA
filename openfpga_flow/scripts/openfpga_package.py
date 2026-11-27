#!/usr/bin/env python3
"""Commands of the device packages (docs/spec_device_package.md).

    openfpga_package.py cache <package> [--corner NAME ...]

cache (R-49) makes the device of each corner of a package (of the package itself, when it has
no corners) and writes into the package the files that VPR would otherwise compute each time
(R-27): the routing resource graph, the router lookahead and, when the placer of the package
stores one, the placement delay lookup. It then records in the manifest the version of VTR that
wrote them and the key of the files they were made from (cache.vtr_version, cache.key), and
writes the checksums again. A session reads them only while both still hold.

The device is made by routing a design of one LUT, which needs no synthesis: what is written
depends on the device only. The Python module of OpenFPGA (openfpga) must be importable.
"""

import argparse
import hashlib
import json
import os
import shutil
import sys
import tempfile
from pathlib import Path

MANIFEST = "device_package.json"
CHECKSUMS = "CHECKSUMS.sha256"
# The cached files: type, name in the package, option of VPR that writes it
CACHED = (
    ("rr_graph", "rr_graph.bin", "filename.write_rr_graph_file"),
    ("router_lookahead", "router_lookahead.bin", "filename.write_router_lookahead"),
    ("place_delay_lookup", "place_delay_lookup.bin", "filename.write_placement_delay_lookup"),
)
# A design of one LUT of two inputs, which every device of LUTs routes
ONE_LUT = """.model cache_design
.inputs a b
.outputs y
.names a b y
11 1
.end
"""


def write_checksums(package: Path) -> None:
    """CHECKSUMS.sha256 of every file but itself, sorted by path"""
    lines = []
    for path in sorted(package.rglob("*"), key=lambda p: p.relative_to(package).as_posix()):
        relative = path.relative_to(package).as_posix()
        if path.is_file() and relative != CHECKSUMS:
            lines.append("%s  %s" % (hashlib.sha256(path.read_bytes()).hexdigest(), relative))
    (package / CHECKSUMS).write_text("\n".join(lines) + "\n")


def make_cached_files(openfpga, package: Path, corner, work: Path) -> dict:
    """Make the device of one corner and write its cached files in work. Returns them by type"""
    os.chdir(work)
    session = openfpga.Session()
    session.read_device_package(package, corner=corner, cache=False)
    written = {}
    for kind, name, option in CACHED:
        if kind == "place_delay_lookup" and session.options["tplace.place_delay_model"] == "simple":
            continue  # the simple delay model of the placer stores no lookup
        session.options[option] = str(work / name)
        written[kind] = work / name
    (work / "cache_design.blif").write_text(ONE_LUT)
    session.read_circuit(str(work / "cache_design.blif"))
    session.pack()
    session.place()
    session.route()
    missing = [kind for kind, path in written.items() if not path.is_file()]
    if missing:
        raise SystemExit("VPR did not write the %s of the device" % ", ".join(missing))
    return written


def cache(package: Path, corners) -> None:
    import openfpga

    package = package.resolve()
    answer = openfpga.OpenfpgaShell().report_device_package(file=os.fspath(package)).data["data"]
    names = corners or answer["corners"] or [None]
    for name in names:
        if name is not None and name not in answer["corners"]:
            raise SystemExit("The package has no corner '%s'" % name)
    manifest_path = package / MANIFEST
    here = os.getcwd()
    with tempfile.TemporaryDirectory(prefix="openfpga_package_cache_") as temp:
        made = {}
        for name in names:
            work = Path(temp) / (name or "device")
            work.mkdir()
            try:
                made[name] = make_cached_files(openfpga, package, name, work)
            finally:
                os.chdir(here)
        manifest = json.loads(manifest_path.read_text())
        kinds = {kind for kind, _, _ in CACHED}
        files = [
            f for f in manifest["files"] if not (f.get("type") in kinds and f.get("corner") in made)
        ]
        for name, written in made.items():
            folder = Path("corners") / name if name else Path("cache")
            (package / folder).mkdir(parents=True, exist_ok=True)
            for kind, source in written.items():
                target = folder / source.name
                shutil.copyfile(source, package / target)
                entry = {"type": kind, "path": target.as_posix()}
                if name:
                    entry["corner"] = name
                files.append(entry)
                print(
                    "Wrote %s (%d bytes)" % (target.as_posix(), (package / target).stat().st_size)
                )
    manifest["files"] = files
    manifest["cache"] = {
        "vtr_version": answer["cache"]["build_vtr_version"],
        "key": answer["cache"]["expected_key"],
    }
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n")
    write_checksums(package)
    check = openfpga.OpenfpgaShell().report_device_package(file=os.fspath(package)).data["data"]
    if not check["cache"]["usable"]:
        raise SystemExit(
            "The cached files are written but not usable: %s" % check["cache"]["reason"]
        )
    print(
        "The cached files of '%s' are written (VTR %s)"
        % (answer["name"], answer["cache"]["build_vtr_version"])
    )


def main(argv=None) -> int:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    commands = parser.add_subparsers(dest="command", required=True)
    command = commands.add_parser("cache", help="write the cached files of the device (R-49)")
    command.add_argument("package", type=Path, help="the directory of the package")
    command.add_argument(
        "--corner", action="append", help="a corner to make (default: every corner)"
    )
    args = parser.parse_args(argv)
    if args.command == "cache":
        cache(args.package, args.corner)
    return 0


if __name__ == "__main__":
    sys.exit(main())
