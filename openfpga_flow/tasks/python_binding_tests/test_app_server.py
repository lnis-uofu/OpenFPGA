"""The server of the web app: who may call it, what the API says, and the events of a conversation.

The conversation is the recorded one (DemoBackend), whose tool results are the answers of the MCP
server's tests, so every event can be checked against its schema.
"""

import json
from pathlib import Path

import pytest

pytest.importorskip("starlette")
pytest.importorskip("jsonschema")

import jsonschema  # noqa: E402
from starlette.testclient import TestClient  # noqa: E402

from openfpga_app import AppConfig  # noqa: E402
from openfpga_app import events  # noqa: E402
from openfpga_app.backend import STAGES, DemoBackend, empty_view  # noqa: E402
from openfpga_app.security import COOKIE  # noqa: E402
from openfpga_app.server import create_app  # noqa: E402

SCHEMA = json.loads((Path(events.__file__).parent / "schemas" / "events.schema.json").read_text())
HOST = "127.0.0.1:8770"


@pytest.fixture
def config():
    return AppConfig(port=8770, token="secret-token", console=True)


@pytest.fixture
def backend():
    return DemoBackend(delay=0)


@pytest.fixture
def static(tmp_path):
    (tmp_path / "assets").mkdir()
    (tmp_path / "index.html").write_text("<!doctype html><title>app</title>")
    (tmp_path / "assets" / "app.js").write_text("console.log(1)")
    return tmp_path


@pytest.fixture
def client(config, backend, static):
    app = create_app(config, backend, static)
    with TestClient(app, base_url="http://" + HOST, follow_redirects=False) as client:
        yield client


@pytest.fixture
def authorized(client, config):
    client.headers["Authorization"] = "Bearer " + config.token
    return client


def read_events(response):
    found = []
    for block in response.text.split("\n\n"):
        if block.strip():
            lines = dict(line.split(": ", 1) for line in block.splitlines())
            item = json.loads(lines["data"])
            assert lines["event"] == item["type"]
            found.append(item)
    return found


def valid(item):
    jsonschema.validate(item, SCHEMA, cls=jsonschema.Draft202012Validator)


# Who may call -----------------------------------------------------------------------------------


def test_a_call_with_no_token_is_refused(client):
    assert client.get("/api/session").status_code == 401
    assert client.post("/api/chat", json={"message": "hi"}).status_code == 401
    page = client.get("/")
    assert page.status_code == 401 and "printed in the terminal" in page.text


def test_a_bad_token_is_refused(client):
    assert client.get("/api/session", headers={"Authorization": "Bearer nope"}).status_code == 401
    assert client.get("/?token=nope").status_code == 401
    client.cookies.set(COOKIE, "nope")
    assert client.get("/api/session").status_code == 401


def test_the_first_load_keeps_the_token_in_a_cookie_and_takes_it_out_of_the_address(client, config):
    first = client.get("/?token=" + config.token)
    assert first.status_code == 303 and first.headers["location"] == "/"
    cookie = first.headers["set-cookie"].lower()
    assert COOKIE in cookie and "httponly" in cookie and "samesite=strict" in cookie
    client.cookies.set(COOKIE, config.token)
    assert client.get("/").status_code == 200
    assert client.get("/api/session").status_code == 200


def test_the_token_in_the_address_does_not_open_a_post(client, config):
    """Only a first load (GET) takes the token from the address"""
    assert client.post("/api/chat?token=" + config.token, json={"message": "x"}).status_code == 401


def test_a_foreign_host_is_refused(authorized):
    for host in ("evil.example", "evil.example:8770", "127.0.0.1:9999", "127.0.0.1.evil.example"):
        assert authorized.get("/api/session", headers={"Host": host}).status_code == 421


def test_a_foreign_origin_is_refused(authorized):
    for origin in ("https://evil.example", "http://evil.example:8770", "null", "http://127.0.0.1"):
        got = authorized.post("/api/cancel", headers={"Origin": origin}, json={}).status_code
        assert got == 403, origin
    same = authorized.post("/api/cancel", headers={"Origin": "http://" + HOST}, json={})
    assert same.status_code == 200


def test_there_is_no_cors(authorized):
    answer = authorized.get("/api/session", headers={"Origin": "http://" + HOST})
    assert not any(k.lower().startswith("access-control-") for k in answer.headers)
    preflight = authorized.options("/api/chat", headers={"Origin": "https://evil.example"})
    assert preflight.status_code in (401, 403, 405)


def test_the_names_of_this_machine_are_the_app(authorized):
    for host in ("localhost:8770", "127.0.0.1:8770"):
        assert authorized.get("/api/session", headers={"Host": host}).status_code == 200


def test_an_address_that_is_not_local_is_refused_unless_it_is_meant():
    with pytest.raises(ValueError, match="local machine"):
        AppConfig(host="0.0.0.0")
    assert AppConfig(host="0.0.0.0", allow_public_bind=True).host == "0.0.0.0"
    from openfpga_app.cli import main

    with pytest.raises(SystemExit):
        main(["--host", "0.0.0.0", "--demo"])


def test_the_tokens_are_not_the_same_twice():
    assert AppConfig().token != AppConfig().token and len(AppConfig().token) >= 32


# The page -----------------------------------------------------------------------------------------


def test_the_page_and_its_files_are_served_with_the_token(authorized):
    assert "<title>app</title>" in authorized.get("/").text
    assert authorized.get("/").headers["cache-control"] == "no-store"
    assert authorized.get("/assets/app.js").status_code == 200
    assert authorized.get("/assets/../index.html").status_code in (200, 404)
    assert authorized.get("/assets/%2e%2e/%2e%2e/etc/passwd").status_code == 404


def test_without_a_build_the_page_says_how_to_build_it(config, backend, tmp_path):
    app = create_app(config, backend, tmp_path / "empty")
    with TestClient(app, base_url="http://" + HOST) as client:
        client.headers["Authorization"] = "Bearer " + config.token
        answer = client.get("/")
    assert answer.status_code == 503 and "npm run build" in answer.text


# The API ------------------------------------------------------------------------------------------


def test_the_view_of_a_new_session_has_every_section_and_nothing_in_it(authorized):
    view = authorized.get("/api/session").json()
    assert view == empty_view()
    assert [s["stage"] for s in view["stages"]] == list(STAGES)
    assert all(s["status"] == "not_reached" for s in view["stages"])


def test_the_config_says_whether_the_console_is_on(authorized, backend, static):
    assert authorized.get("/api/config").json()["console"] is True
    off = create_app(AppConfig(port=8770, token="t", console=False), backend, static)
    with TestClient(off, base_url="http://" + HOST) as client:
        client.headers["Authorization"] = "Bearer t"
        assert client.get("/api/config").json()["console"] is False
        assert client.post("/api/console", json={"command": "help"}).status_code == 404


def test_a_message_is_text(authorized):
    for body in ({}, {"message": ""}, {"message": "   "}, {"message": 3}, {"message": "x" * 8001}):
        assert authorized.post("/api/chat", json=body).status_code == 400
    assert authorized.post("/api/chat", content=b"not json").status_code == 400


# The conversation -----------------------------------------------------------------------------------


def test_a_conversation_gives_valid_events_in_order(authorized):
    answer = authorized.post("/api/chat", json={"message": "run the flow"})
    assert answer.headers["content-type"].startswith("text/event-stream")
    found = read_events(answer)
    for item in found:
        valid(item)
    assert found[0]["type"] == "message_start" and found[-1]["type"] == "done"
    assert found[-1]["cancelled"] is False
    # Each call starts before it finishes, and finishes once
    started = [e["call_id"] for e in found if e["type"] == "tool_call_started"]
    finished = [e["call_id"] for e in found if e["type"] == "tool_call_finished"]
    assert started == finished and len(started) == 24
    for call in started:
        first = next(i for i, e in enumerate(found) if e.get("call_id") == call)
        assert found[first]["type"] == "tool_call_started"
    text = "".join(e["text"] for e in found if e["type"] == "text_delta")
    assert "recorded demo" in text


def test_a_failed_call_is_a_finished_call_with_its_diagnostics(authorized):
    found = read_events(authorized.post("/api/chat", json={"message": "go"}))
    failed = [e for e in found if e["type"] == "tool_call_finished" and not e["ok"]]
    assert len(failed) == 1
    assert failed[0]["result"]["diagnostics"][0]["kind"] == "stage_not_reached"


def test_the_view_follows_the_conversation(authorized):
    authorized.post("/api/chat", json={"message": "go"})
    view = authorized.get("/api/session").json()
    assert view["session"]["id"] == "s1"
    reached = [s["stage"] for s in view["stages"] if s["status"] == "reached"]
    # The width of the channel is in the device: it dropped every stage
    assert reached == []
    assert view["latest"]["status"] == "none"
    assert [f["label"] for f in view["files"]["inputs"]] == ["vpr arch", "openfpga arch", "circuit"]
    assert view["files"]["inputs"][0]["extra"] == "layout 2x2"
    assert view["files"]["constraints"] == ["chan width 24"]
    assert {"name": "place.seed", "value": 3} in view["options_changed"]
    # The results were read while the stages were reached, and read again after they were dropped
    assert view["files"]["results"] == []


def test_the_result_files_are_names_while_the_stages_are_reached(config, static):
    import asyncio

    backend = DemoBackend(delay=0)
    seen = []

    async def run():
        async for item in backend.chat("go"):
            if item["type"] == "tool_call_finished" and item["call_id"].endswith("-9"):
                seen.append((await backend.view())["files"]["results"])

    asyncio.run(run())
    assert seen == [["routing_test.net", "routing_test.place", "routing_test.route"]]


def test_the_view_has_no_path_of_the_machine(authorized):
    authorized.post("/api/chat", json={"message": "go"})
    text = authorized.get("/api/session").text
    assert "/localproj" not in text and "/tmp" not in text


def test_a_second_message_after_the_demo_is_answered_in_a_few_words(authorized):
    authorized.post("/api/chat", json={"message": "go"})
    found = read_events(authorized.post("/api/chat", json={"message": "again?"}))
    assert [e["type"] for e in found][0] == "message_start" and found[-1]["type"] == "done"
    assert not any(e["type"] == "tool_call_started" for e in found)


def test_a_cancel_ends_the_conversation(config, static):
    import asyncio

    backend = DemoBackend(delay=0)
    found = []

    async def run():
        stream = backend.chat("go")
        async for item in stream:
            found.append(item)
            if item["type"] == "tool_call_finished" and len(found) > 8:
                backend.cancel()

    asyncio.run(run())
    assert found[-1] == events.done(found[-1]["id"], cancelled=True)
    for item in found:
        valid(item)
    assert sum(e["type"] == "tool_call_started" for e in found) < 24


def test_a_failure_of_the_backend_is_an_event(config, static):
    class Broken:
        kind = "demo"

        async def view(self):
            return empty_view()

        def cancel(self):
            pass

        async def chat(self, message):
            raise RuntimeError("the model is gone")
            yield  # pragma: no cover

        async def console(self, command):
            return {}

    with TestClient(create_app(config, Broken(), static), base_url="http://" + HOST) as client:
        client.headers["Authorization"] = "Bearer " + config.token
        found = read_events(client.post("/api/chat", json={"message": "hi"}))
    assert found == [events.error("the model is gone")]
    valid(found[0])


# The console -------------------------------------------------------------------------------------------


def test_the_console_runs_a_command_when_it_is_on(authorized):
    ok = authorized.post("/api/console", json={"command": "help"}).json()
    assert ok["ok"] is True and "Commands" in ok["output"]
    bad = authorized.post("/api/console", json={"command": "no_such"}).json()
    assert bad["ok"] is False and "unknown command" in bad["output"]
    for body in ({}, {"command": 3}, {"command": "a\nb"}, {"command": "x" * 9000}):
        assert authorized.post("/api/console", json=body).status_code == 400


# The schema ----------------------------------------------------------------------------------------------


def test_the_schema_refuses_what_is_not_an_event():
    for item in (
        {"type": "text_delta", "schema_version": 1, "id": "m1"},
        {"type": "text_delta", "schema_version": 2, "id": "m1", "text": "x"},
        {"type": "other", "schema_version": 1},
        {"type": "done", "schema_version": 1, "id": "m1", "cancelled": False, "extra": 1},
    ):
        with pytest.raises(jsonschema.ValidationError):
            valid(item)
    for kind in events.EVENT_TYPES:
        assert kind in json.dumps(SCHEMA)
