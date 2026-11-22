"""What the sidebar of the app does by itself: the open sessions as tabs, a checkpoint saved from the
panel of the session, and a design directory added to the session without being synthesized"""

import asyncio

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from test_app_device_package import LOGIC, PACKAGES  # noqa: E402
from test_app_session import backend, client, set_up, view  # noqa: E402,F401
from test_app_synth import call, opened  # noqa: E402
from test_mcp_tools import golden  # noqa: E402,F401


@pytest.fixture
def designed(openfpga_module, tmp_path):
    """A backend with a designs folder that has one design directory, chip"""
    from openfpga_app.session_backend import SessionBackend
    from openfpga_mcp.config import Config
    from test_mcp_design_synth import make_design

    designs = tmp_path / "designs"
    make_design(designs / "chip")
    backend = SessionBackend(
        Config(root=tmp_path / "workspace", design_dirs=(designs,), package_dirs=(PACKAGES,), call_timeout_s=120)
    )
    yield backend
    backend.close()


def test_the_view_lists_the_open_sessions_for_the_tabs(client):
    assert view(client)["sessions"] == []
    client.post("/api/session/new", json={"name": "first"})
    client.post("/api/session/new", json={"name": "second"})
    shown = view(client)
    assert [(s["name"], s["running"]) for s in shown["sessions"]] == [("first", True), ("second", True)]
    assert shown["session"]["id"] == shown["sessions"][1]["id"]  # the newest is the current one


def test_a_checkpoint_is_saved_from_the_page_with_a_name_or_without(client, golden):
    set_up(client, golden, stages=("pack",))
    answer = client.post("/api/session/checkpoint", json={}).json()
    assert answer["ok"], answer
    answer = client.post("/api/session/checkpoint", json={"name": "packed"}).json()
    assert answer["ok"], answer
    names = [c["name"] for c in view(client)["checkpoints"]]
    assert "packed" in names and "checkpoint-1" in names


def test_a_checkpoint_needs_a_session_and_a_name_that_is_text(client):
    assert client.post("/api/session/checkpoint", json={}).status_code == 404
    assert client.post("/api/session/checkpoint", json={"name": 3}).status_code == 400
    assert client.post("/api/session/checkpoint", json={"name": "x" * 65}).status_code == 400


def test_a_design_is_added_to_the_session_and_not_synthesized(designed):
    opened(designed, LOGIC)
    answer = asyncio.run(designed.add_design("chip"))
    assert answer["ok"], answer
    assert answer["data"]["folder"] == "design/chip" and answer["data"]["files"] == 3
    shown = asyncio.run(designed.view())
    assert [f["name"] for f in shown["files"]["design"]] == [
        "design/chip/design.json",
        "design/chip/rtl/chip.v",
        "design/chip/rtl/include/defs.vh",
    ]
    # Nothing was synthesized nor read: the run button of synthesis synthesizes the design
    assert shown["synthesis"]["status"] == "not_reached" and shown["synthesis"]["design"] == "chip"
    assert shown["files"]["synthesis"] == []
    assert not any(f["label"] == "circuit" for f in shown["files"]["inputs"])
    # ... and that works on the copy
    assert call(designed, "synthesize", design="chip")["ok"]
    assert asyncio.run(designed.view())["synthesis"]["status"] == "reached"


def test_a_design_that_is_not_there_is_refused_and_nothing_is_copied(designed, tmp_path):
    opened(designed, LOGIC)
    answer = asyncio.run(designed.add_design("nothing"))
    assert not answer["ok"] and "nothing" in answer["diagnostics"][0]["message"]
    assert asyncio.run(designed.view())["files"]["design"] == []


def test_a_design_needs_a_session(designed):
    answer = asyncio.run(designed.add_design("chip"))
    assert not answer["ok"] and answer["diagnostics"][0]["kind"] == "no_session"


def test_the_endpoint_of_a_design_checks_its_argument(client):
    assert client.post("/api/design/add", json={}).status_code == 400
    assert client.post("/api/design/add", json={"design": 3}).status_code == 400
    assert client.post("/api/design/add", json={"design": "chip"}).status_code == 404  # no session
