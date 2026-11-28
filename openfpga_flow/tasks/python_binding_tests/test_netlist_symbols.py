"""The netlist symbols of a device package (docs/plan_netlist_symbols.md, docs/spec_device_package.md
R-52): the file views/netlist_symbols.svg, checked when the package is read, and given to the
page by the app for the session of the package."""

import asyncio
import json
import shutil
import sys
from pathlib import Path

import pytest

FLOW = Path(__file__).resolve().parents[2]
PACKAGES = FLOW / "device-packages"
NAME = "k6_frac_n10_mem1k_3x2"
sys.path.insert(0, str(FLOW / "scripts"))

import openfpga_package  # noqa: E402

GOOD = """<svg xmlns="http://www.w3.org/2000/svg" xmlns:s="https://github.com/nturley/netlistsvg">
  <s:color role="glow" light="#ff0" dark="#880"/>
  <g s:type="atom_lut" s:width="40" s:height="20">
    <s:alias val="atom:.names"/>
    <rect width="40" height="20" class="glow"/>
    <g s:pid="in" s:x="0" s:y="4" s:step="4"/>
  </g>
  <g s:type="atom_mult" s:hide="true"/>
</svg>
"""


@pytest.fixture
def copy(tmp_path):
    target = tmp_path / "packages" / NAME
    shutil.copytree(PACKAGES / NAME, target)
    return target


def write(package, text, path="views/netlist_symbols.svg"):
    """The package with a file of symbols in place of its own"""
    manifest_path = package / "device_package.json"
    manifest = json.loads(manifest_path.read_text())
    for entry in manifest["files"]:
        if entry["type"] == "netlist_symbols":
            (package / entry["path"]).unlink()
    manifest["files"] = [f for f in manifest["files"] if f["type"] != "netlist_symbols"]
    (package / path).parent.mkdir(parents=True, exist_ok=True)
    (package / path).write_text(text)
    manifest["files"].append({"type": "netlist_symbols", "path": path})
    manifest_path.write_text(json.dumps(manifest, indent=2))
    openfpga_package.write_checksums(package)


def report(openfpga_module, package):
    return openfpga_module.OpenfpgaShell().report_device_package(file=str(package)).data["data"]


def test_the_symbols_of_a_package_of_the_repository_load(openfpga_module):
    answer = report(openfpga_module, PACKAGES / NAME)
    assert {"type": "netlist_symbols", "path": "views/netlist_symbols.svg"}.items() <= next(
        f for f in answer["files"] if f["type"] == "netlist_symbols"
    ).items()


def test_a_file_in_the_subset_loads(openfpga_module, copy):
    write(copy, GOOD)
    assert report(openfpga_module, copy)["name"] == NAME


def symbol(inside):
    return GOOD.replace('<rect width="40" height="20" class="glow"/>', inside)


@pytest.mark.parametrize(
    "text, message",
    [
        (symbol("<script>alert(1)</script>"), "the element script is refused"),
        (symbol('<image href="x.png"/>'), "the element image is refused"),
        (symbol('<rect width="1" height="1" style="fill:red"/>'), "style is refused"),
        (symbol('<rect width="1" height="1" fill="#f00"/>'), 'fill="#f00" is refused'),
        (symbol('<rect width="1" height="1" onclick="x()"/>'), "onclick is refused"),
        (symbol('<rect width="1" height="1" transform="rotate(9)"/>'), "translate and scale"),
        (symbol('<rect width="1" height="1" class="shiny"/>'), "the role shiny is not one of"),
        (GOOD.replace('dark="#880"', ""), "needs both light and dark colours"),
        (GOOD.replace('<s:alias val="atom:.names"/>', ""), "has no s:alias"),
        (
            GOOD.replace(
                '<g s:type="atom_mult" s:hide="true"/>',
                '<g s:type="atom_mult" s:hide="true"><rect/></g>',
            ),
            "the hidden symbol atom_mult holds nothing",
        ),
        (GOOD.replace("<g s:type", '<s:layout algorithm="force"/><g s:type', 1), "direction only"),
        ("<svg", "not well-formed XML"),
    ],
)
def test_a_file_outside_the_subset_is_refused(openfpga_module, copy, text, message):
    write(copy, text)
    with pytest.raises(openfpga_module.OpenfpgaError, match="netlist symbols") as error:
        report(openfpga_module, copy)
    assert message in str(error.value.log if hasattr(error.value, "log") else error.value)


def test_the_refusal_says_where(openfpga_module, copy):
    write(copy, symbol('\n\n<image href="x.png"/>'))
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        report(openfpga_module, copy)
    text = getattr(error.value, "log", None) or str(error.value)
    assert "<image> (line 7)" in text


def test_the_file_has_its_own_path(openfpga_module, copy):
    write(copy, GOOD, path="views/other.svg")
    with pytest.raises(openfpga_module.OpenfpgaError, match="views/netlist_symbols.svg"):
        report(openfpga_module, copy)


# The app ---------------------------------------------------------------------------------------

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from openfpga_app.session_backend import SessionBackend  # noqa: E402
from openfpga_mcp.config import Config  # noqa: E402


def test_the_app_gives_the_symbols_of_the_package_of_the_session(openfpga_module, tmp_path):
    backend = SessionBackend(
        Config(root=tmp_path / "workspace", package_dirs=(PACKAGES,), call_timeout_s=120)
    )
    try:
        assert asyncio.run(backend.netlist_symbols()) == {"ok": True, "data": None}
        assert asyncio.run(backend.new_session(device_package=NAME))["ok"]
        data = asyncio.run(backend.netlist_symbols())["data"]
        assert data["package"] == NAME and data["path"] == "views/netlist_symbols.svg"
        assert data["text"] == (PACKAGES / NAME / "views" / "netlist_symbols.svg").read_text()
        # A package with no symbols: none
        asyncio.run(backend.close_session())
        assert asyncio.run(backend.new_session(device_package="k6_frac_n10_2x2"))["ok"]
        assert asyncio.run(backend.netlist_symbols())["data"] is None
    finally:
        backend.close()


def test_the_route_answers_none_in_the_demo():
    from starlette.testclient import TestClient

    from openfpga_app import AppConfig
    from openfpga_app.backend import DemoBackend
    from openfpga_app.server import create_app
    from test_app_session import HOST

    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)),
        base_url="http://" + HOST,
    ) as client:
        client.headers["Authorization"] = "Bearer t"
        assert client.get("/api/package/netlist_symbols").json() == {"ok": True, "data": None}
