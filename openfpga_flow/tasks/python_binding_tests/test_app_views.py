"""The data of the views in the app (/api/view, /api/tile; docs/plan_design_views.md, P1.5): the
files of the worker, served as they are, with their version"""

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from starlette.testclient import TestClient  # noqa: E402

from openfpga_app import AppConfig  # noqa: E402
from openfpga_app.backend import DemoBackend  # noqa: E402
from openfpga_app.server import create_app  # noqa: E402
from test_app_device_package import LOGIC, backend, client  # noqa: E402,F401
from test_app_session import HOST, call, say  # noqa: E402,F401
from test_mcp_tools import golden  # noqa: E402,F401


def routed(client, golden):
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    assert call(say(client, f"/read_inputs circuit={golden.circuit.name}"))["ok"]
    assert call(say(client, "/run_stage stage=route"))["ok"]


def test_the_views_of_a_routed_session(openfpga_module, client, backend, golden):
    routed(client, golden)
    got = client.get("/api/view", params={"kind": "placement"})
    assert got.status_code == 200, got.text
    assert got.headers["content-type"] == "application/octet-stream"
    view = openfpga_module.read_view(got.content)
    assert view["kind"] == "placement" and got.headers["x-view-version"] == view["version"]
    blocks = view["tables"]["blocks"]["name"]
    assert len(blocks) > 0
    # The file is in the session, and kept while its version is current
    path = backend.config.root / "s1" / "views" / "placement.bin"
    assert path.read_bytes() == got.content
    again = client.get("/api/view", params={"kind": "placement"})
    assert again.content == got.content
    # A scope is a file of its own
    cone = client.get("/api/view", params={"kind": "atoms", "cone": blocks[0], "depth": "1"})
    assert cone.status_code == 200, cone.text
    assert openfpga_module.read_view(cone.content)["meta"]["scope"].startswith("cone ")
    assert len(list((backend.config.root / "s1" / "views").glob("atoms-*.bin"))) == 1
    # A tile
    tile = client.get("/api/tile", params={"kind": "routing", "level": "0", "x": "0", "y": "0"})
    assert tile.status_code == 200, tile.text
    data = openfpga_module.read_view(tile.content)
    assert data["meta"]["level"] == 0 and len(data["tables"]["nodes"]["net"]) > 0
    coarse = client.get("/api/tile", params={"kind": "channels", "level": "1", "x": "0", "y": "0"})
    assert len(openfpga_module.read_view(coarse.content)["tables"]["cells"]["value"]) == 256


def test_what_is_refused(client, golden):
    assert (
        client.get("/api/view", params={"kind": "placement"}).json()["diagnostics"][0]["kind"]
        == "no_session"
    )
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    assert client.get("/api/view", params={"kind": "nope"}).status_code == 400
    assert client.get("/api/view", params={"kind": "atoms", "depth": "99"}).status_code == 400
    assert (
        client.get(
            "/api/tile", params={"kind": "grid", "level": "x", "x": "0", "y": "0"}
        ).status_code
        == 400
    )
    assert (
        client.get(
            "/api/tile", params={"kind": "pins", "level": "0", "x": "0", "y": "0"}
        ).status_code
        == 400
    )
    # Before the stage the kind needs: the answer of the worker
    early = client.get("/api/view", params={"kind": "routing"})
    assert early.status_code == 400 and early.json()["ok"] is False


def test_the_demo_has_no_views():
    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)),
        base_url="http://" + HOST,
    ) as client:
        client.headers["Authorization"] = "Bearer t"
        assert client.get("/api/view", params={"kind": "device"}).status_code == 404
        assert (
            client.get(
                "/api/tile", params={"kind": "grid", "level": "0", "x": "0", "y": "0"}
            ).status_code
            == 404
        )
