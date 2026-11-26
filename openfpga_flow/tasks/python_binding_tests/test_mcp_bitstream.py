"""The bitstream through the MCP server (docs/spec_bitstream.md B-42): run_stage stage=bitstream,
its answer and its report, the state, the replay of the journal, and a device without one."""

import json
import shutil
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from openfpga_mcp.config import Config  # noqa: E402
from test_mcp_synth import AND2, Tools  # noqa: E402
from test_mcp_tools import within  # noqa: E402
from test_odc import checksums  # noqa: E402

PACKAGES = Path(__file__).resolve().parents[2] / "device-packages"


@pytest.fixture
def server(openfpga_module, tmp_path):
    packages = tmp_path / "packages"
    shutil.copytree(PACKAGES / "k6_frac_n10_2x2", packages / "k6_frac_n10_2x2")
    # A device that makes no bitstream
    nobits = packages / "k6_frac_n10_mem1k_3x2"
    shutil.copytree(PACKAGES / nobits.name, nobits)
    manifest = json.loads((nobits / "device_package.json").read_text())
    manifest["device"]["bitstream"] = False
    (nobits / "device_package.json").write_text(json.dumps(manifest, indent=2))
    checksums(nobits)
    tools = Tools(Config(root=tmp_path / "workspace", package_dirs=(packages,), call_timeout_s=120))
    yield tools
    tools.close()


def call(server, tool, /, **arguments):
    if tool not in ("session_open", "session_list", "replay", "list_device_packages"):
        arguments.setdefault("session", server.sid)
    return server.invoke(tool, arguments)


def test_a_bitstream_is_made_by_the_stage_of_its_name(server, tmp_path):
    assert call(server, "session_open")["ok"]
    sid = server.sid
    assert call(server, "use_device_package", name="k6_frac_n10_2x2")["ok"]
    assert call(server, "write_file", name="and2.v", content=AND2)["ok"]
    assert call(server, "synthesize", top="and2", files="and2.v")["ok"]
    answer = call(server, "run_stage", stage="bitstream")
    assert answer["ok"], answer
    data = answer["data"]
    assert data["ran"] == ["pack", "place", "route", "analysis", "bitstream"]
    assert "BITSTREAM" in data["reached"] and data["report"] == f"{sid}/reports/bitstream.html"
    bits = data["bitstream"]
    assert bits["file"] == f"{sid}/bitstream/and2.bit"  # a name in the workspace
    assert bits["bits"] > 0 and bits["protocol"] and len(bits["blocks"]) == 3
    (setting,) = bits["settings"]
    assert setting.endswith("k6_frac_n10_2x2/arch/simulation_setting.xml")
    within(answer)
    workspace = tmp_path / "workspace"
    assert (workspace / bits["file"]).stat().st_size == bits["bytes"]
    report = (workspace / data["report"]).read_text()
    assert "Bits by type of block" in report and "grid_clb" in report

    state = call(server, "get_state")["data"]["state"]
    assert state["results"]["bitstream"] == bits["file"]
    assert state["inputs"]["simulation_setting"] == [setting]
    # Once made, it runs alone
    again = call(server, "run_stage", stage="bitstream")["data"]
    assert "ran" not in again and again["bitstream"]["file"] == bits["file"]
    refused = call(server, "run_stage", stage="bitstream", from_file="and2.bit")
    assert refused["diagnostics"][0]["kind"] == "invalid_argument"

    # The journal makes it again
    replayed = call(server, "replay", journal=sid)
    assert replayed["ok"], replayed
    assert "BITSTREAM" in call(server, "get_state")["data"]["state"]["reached"]


def test_a_device_without_a_bitstream_says_so(server):
    assert call(server, "session_open")["ok"]
    assert call(server, "use_device_package", name="k6_frac_n10_mem1k_3x2")["ok"]
    refused = call(server, "run_stage", stage="bitstream")
    assert refused["ok"] is False
    assert refused["diagnostics"][0]["kind"] == "bitstream_unsupported"
    assert "cannot make a bitstream" in refused["diagnostics"][0]["message"]
