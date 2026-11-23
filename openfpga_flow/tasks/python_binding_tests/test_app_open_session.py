"""Opening a session from the first screen: the live ones are switched to, and the saved ones of the
workspace (made by an earlier server, or closed) are started again from their last checkpoint, or
with their files and an empty flow when no stage ran"""

import asyncio

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from starlette.testclient import TestClient  # noqa: E402

from openfpga_app import AppConfig  # noqa: E402
from openfpga_app.backend import DemoBackend  # noqa: E402
from openfpga_app.server import create_app  # noqa: E402
from openfpga_app.session_backend import SessionBackend  # noqa: E402
from test_app_device_package import LOGIC, backend, client, config  # noqa: E402,F401
from test_app_session import HOST, call, say, view  # noqa: E402,F401
from test_mcp_tools import golden  # noqa: E402,F401


def listing(client):
    return client.get("/api/sessions").json()["data"]


def opened(client, session):
    return client.post("/api/session/open", json={"session": session})


def packed_session(client, golden):
    """A session of the device package with a circuit that was packed: it has a checkpoint"""
    assert client.post("/api/session/new", json={"device_package": LOGIC}).json()["ok"]
    assert call(say(client, f"/read_inputs circuit={golden.circuit.name}"))["ok"]
    assert call(say(client, "/run_stage stage=pack"))["ok"]
    return view(client)["session"]["id"]


def test_a_live_session_is_listed_and_switched_to(client, backend):
    first = client.post("/api/session/new", json={"name": "one"}).json()["data"]["session"]
    second = client.post("/api/session/new", json={"name": "two"}).json()["data"]["session"]
    data = listing(client)
    assert [(s["id"], s["name"], s["current"]) for s in data["live"]] == [
        (first, "one", False),
        (second, "two", True),
    ]
    assert data["saved"] == []
    assert opened(client, first).json()["ok"]
    assert view(client)["session"]["id"] == first
    assert [s["current"] for s in listing(client)["live"]] == [True, False]


def test_a_closed_session_is_saved_and_opens_again_with_its_stages_and_its_device(client, backend, golden):
    sid = packed_session(client, golden)
    assert client.post("/api/session/close", json={}).json()["ok"]
    assert view(client)["session"] is None
    saved = listing(client)["saved"]
    assert [(s["id"], s["name"], s["stages"]) for s in saved] == [(sid, LOGIC, ["DEVICE", "PACKED"])]
    assert saved[0]["device_package"]["name"] == LOGIC

    answer = opened(client, sid)
    assert answer.json()["ok"], answer.json()
    shown = view(client)
    assert shown["session"]["id"] == sid and shown["session"]["worker"] == "running"
    assert shown["session"]["device_package"]["name"] == LOGIC
    assert [s["status"] for s in shown["stages"]][:2] == ["reached", "reached"]
    assert listing(client)["saved"] == []  # it is live now
    assert call(say(client, "/run_stage stage=place"))["ok"]  # and it goes on


def test_after_a_restart_the_sessions_of_the_workspace_open_and_new_ones_do_not_collide(
    openfpga_module, tmp_path, golden, client, backend
):
    sid = packed_session(client, golden)
    backend.close()  # the server ends: the workers go, the directory stays
    again = SessionBackend(config(tmp_path, golden))
    try:
        data = asyncio.run(again.list_sessions())["data"]
        assert data["live"] == [] and [s["id"] for s in data["saved"]] == [sid]
        assert asyncio.run(again.open_session(sid))["ok"]
        assert again.sid == sid
        other = asyncio.run(again.new_session())
        assert other["ok"] and other["data"]["session"] != sid  # a directory in use is not reused
        assert asyncio.run(again.open_session(sid))["ok"]  # back to the first one
        assert [s["id"] for s in asyncio.run(again.list_sessions())["data"]["live"]] == [
            sid,
            other["data"]["session"],
        ]
    finally:
        again.close()


@pytest.mark.parametrize("session", ["s999", "../s1", "", "x", "s1/../s1"])
def test_a_session_that_is_not_there_is_refused(client, session):
    answer = opened(client, session)
    assert answer.status_code in (400, 404)
    assert not answer.json().get("ok", False)


def test_a_session_with_no_checkpoint_opens_with_its_files_and_an_empty_flow(client, backend):
    sid = client.post("/api/session/new", json={"name": "bare"}).json()["data"]["session"]
    directory = backend.config.root / sid
    (directory / "design").mkdir()
    (directory / "design" / "top.v").write_text("module top; endmodule\n")
    assert client.post("/api/session/close", json={}).json()["ok"]
    notes = []
    backend._changed_outside = notes.append  # what the model is told
    saved = listing(client)["saved"]
    assert [(s["id"], s["name"], s["stages"], s["checkpoint"]) for s in saved] == [
        (sid, "bare", [], False)
    ]
    answer = opened(client, sid)
    assert answer.json()["ok"], answer.json()
    shown = view(client)
    assert shown["session"]["id"] == sid and shown["session"]["worker"] == "running"
    assert (directory / "design" / "top.v").is_file()  # its files are kept
    assert "no stage had run" in notes[-1]


def test_a_session_of_a_device_package_with_no_checkpoint_reads_its_package_again(client, backend):
    sid = client.post("/api/session/new", json={"device_package": LOGIC}).json()["data"]["session"]
    assert client.post("/api/session/close", json={}).json()["ok"]
    notes = []
    backend._changed_outside = notes.append
    assert opened(client, sid).json()["ok"]
    shown = view(client)
    assert shown["session"]["device_package"]["name"] == LOGIC
    assert "read again" in notes[-1]
    assert backend.registry.get(sid).session.state().inputs.vpr_arch is not None


def test_a_directory_that_is_not_a_session_is_not_listed(client, backend):
    (backend.config.root / "s77").mkdir()
    assert listing(client)["saved"] == []
    assert opened(client, "s77").status_code == 404


def test_the_list_and_open_need_the_token_and_the_demo_has_no_sessions(client):
    anonymous = TestClient(client.app, base_url="http://" + HOST)
    assert anonymous.get("/api/sessions").status_code == 401
    assert anonymous.post("/api/session/open", json={"session": "s1"}).status_code == 401
    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)), base_url="http://" + HOST
    ) as demo:
        demo.headers["Authorization"] = "Bearer t"
        assert demo.get("/api/sessions").json()["data"] == {"live": [], "saved": []}
        assert demo.post("/api/session/open", json={"session": "s1"}).status_code == 404
