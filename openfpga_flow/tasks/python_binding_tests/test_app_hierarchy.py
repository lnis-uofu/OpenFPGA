"""The view of the hierarchy of a design in the app (/api/hierarchy): the design that the session
copied, elaborated by a Yosys run of its own, with the files of its lines named for the viewer"""

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from starlette.testclient import TestClient  # noqa: E402

from openfpga_app import AppConfig  # noqa: E402
from openfpga_app.backend import DemoBackend  # noqa: E402
from openfpga_app.server import create_app  # noqa: E402
from openfpga_app.session_backend import SessionBackend  # noqa: E402
from openfpga_mcp.config import Config  # noqa: E402
from test_app_device_package import LOGIC, PACKAGES  # noqa: E402
from test_app_session import HOST, say, view  # noqa: E402
from test_design_hierarchy import make_design  # noqa: E402


@pytest.fixture
def client(openfpga_module, tmp_path):
    designs = tmp_path / "designs"
    make_design(designs / "d")
    backend = SessionBackend(
        Config(
            root=tmp_path / "workspace",
            design_dirs=(designs,),
            package_dirs=(PACKAGES,),
            call_timeout_s=120,
        )
    )
    static = tmp_path / "static"
    static.mkdir()
    (static / "index.html").write_text("<title>app</title>")
    config = AppConfig(port=8770, token="t")
    try:
        with TestClient(create_app(config, backend, static), base_url="http://" + HOST) as client:
            client.headers["Authorization"] = "Bearer t"
            yield client
    finally:
        backend.close()


def test_the_hierarchy_of_the_design_of_the_session(client, tmp_path):
    assert client.get("/api/hierarchy").json()["diagnostics"][0]["kind"] == "no_session"
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    got = client.get("/api/hierarchy")
    assert got.status_code == 404 and got.json()["diagnostics"][0]["kind"] == "no_design"
    assert client.post("/api/design/add", json={"design": "d"}).json()["ok"]
    got = client.get("/api/hierarchy")
    assert got.status_code == 200, got.json()
    data = got.json()["data"]
    assert data["design"] == "d" and data["folder"] == "design/d" and data["nodes"] == 10
    wide = data["tree"]["children"][2]
    assert (wide["instance"], wide["parameters"]) == ("u_wide", {"W": 8})
    # The lines are in the files of the session, which the viewer opens
    assert wide["src"] == {"file": "design/d/rtl/top.v", "line": 10}
    text = client.get("/api/file", params={"name": wide["src"]["file"]}).json()
    assert text["ok"] and "u_wide" in text["data"]["text"]
    assert all(m["src"]["file"] == "design/d/rtl/top.v" for m in data["modules"])
    assert str(tmp_path) not in got.text
    # The flow is not changed, and the design is still not synthesized
    v = view(client)
    assert v["synthesis"]["status"] == "not_reached"
    assert client.get("/api/hierarchy", params={"design": "../x"}).status_code == 404
    # The run button synthesizes it afterwards as before
    assert any(
        e["type"] == "tool_call_finished" and e["ok"] for e in say(client, "/run_stage stage=pack")
    )
    assert view(client)["synthesis"]["status"] == "reached"


def test_the_demo_has_no_hierarchy():
    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)),
        base_url="http://" + HOST,
    ) as client:
        client.headers["Authorization"] = "Bearer t"
        got = client.get("/api/hierarchy")
        assert got.status_code == 404 and got.json()["diagnostics"][0]["kind"] == "demo"
