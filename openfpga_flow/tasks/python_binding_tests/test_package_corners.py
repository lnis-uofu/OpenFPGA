"""The corners of a device package (docs/spec_device_package.md R-16 to R-24): the timing values of
a corner replace the ${name} variables of its VPR architecture when read_device_package reads it."""

import hashlib
import json
import re
import shutil
from pathlib import Path

import pytest

PACKAGES = Path(__file__).resolve().parents[2] / "device-packages"
NAME = "k6_frac_n10_2x2"
DELAY = re.compile(r'delay_constant max="([^"]*)"')


def resum(package):
    lines = []
    for path in sorted(package.rglob("*")):
        relative = path.relative_to(package).as_posix()
        if path.is_file() and relative != "CHECKSUMS.sha256":
            lines.append(hashlib.sha256(path.read_bytes()).hexdigest() + "  " + relative)
    (package / "CHECKSUMS.sha256").write_text("\n".join(lines) + "\n")


@pytest.fixture
def package(tmp_path):
    """The logic package with its first two delays made variables, and two corners"""
    target = tmp_path / "packages" / NAME
    shutil.copytree(PACKAGES / NAME, target)
    arch = target / "arch" / "vpr_arch.xml"
    values = []

    def variable(match):
        if len(values) == 2:
            return match.group(0)
        values.append(float(match.group(1)))
        return 'delay_constant max="${d%d_T}"' % len(values)

    arch.write_text(DELAY.sub(variable, arch.read_text()))
    manifest_path = target / "device_package.json"
    manifest = json.loads(manifest_path.read_text())
    manifest["timing_units"] = {"time": "ps", "capacitance": "fF", "resistance": "ohm"}
    manifest["corners"] = {
        "default": "typ",
        "list": [{"name": "typ"}, {"name": "slow"}],
    }
    for corner, scale in (("typ", 1e12), ("slow", 2e12)):
        folder = target / "corners" / corner
        folder.mkdir(parents=True)
        (folder / "timing_data.json").write_text(
            json.dumps({"d1_T": values[0] * scale, "d2_T": values[1] * scale})
        )
        manifest["files"].append(
            {
                "role": "timing_data",
                "path": f"corners/{corner}/timing_data.json",
                "corner": corner,
            }
        )
    manifest_path.write_text(json.dumps(manifest, indent=2))
    resum(target)
    return target


@pytest.fixture
def session(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    return openfpga_module.Session()


def test_the_default_corner_is_read_and_the_package_is_not_changed(session, package):
    before = (package / "arch" / "vpr_arch.xml").read_bytes()
    answer = session.read_device_package(package)
    assert (answer["corner"], answer["corners"]) == ("typ", ["typ", "slow"])
    assert (package / "arch" / "vpr_arch.xml").read_bytes() == before


def test_a_corner_that_is_asked_for_is_read(session, package):
    assert session.read_device_package(package, corner="slow")["corner"] == "slow"


def test_a_corner_that_is_not_in_the_package_is_refused(session, package):
    with pytest.raises(Exception, match="corner 'fast' is not in the package"):
        session.read_device_package(package, corner="fast")


def test_a_variable_with_no_value_is_refused_with_its_name(session, package):
    path = package / "corners" / "slow" / "timing_data.json"
    path.write_text(json.dumps({"d1_T": 1.0}))
    resum(package)
    with pytest.raises(Exception, match="no value for the variable 'd2_T'"):
        session.read_device_package(package)


def test_a_variable_that_does_not_say_its_kind_is_refused(session, package):
    path = package / "corners" / "typ" / "timing_data.json"
    path.write_text(json.dumps({"d1_T": 1.0, "d2_T": 1.0, "delay": 3}))
    resum(package)
    with pytest.raises(Exception, match="does not end in _T, _C or _R"):
        session.read_device_package(package)


def test_variables_without_corners_are_refused(session, package):
    path = package / "device_package.json"
    manifest = json.loads(path.read_text())
    del manifest["corners"]
    path.write_text(json.dumps(manifest))
    resum(package)
    with pytest.raises(Exception, match="has variables .* and the package has no corners"):
        session.read_device_package(package)
