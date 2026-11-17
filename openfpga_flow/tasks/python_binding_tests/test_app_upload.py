"""A file that the person gives to a session: where it goes, what is refused, how the tools and the
model find it"""

import json

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from starlette.testclient import TestClient  # noqa: E402

from openfpga_app import AppConfig, uploads  # noqa: E402
from openfpga_app.backend import DemoBackend  # noqa: E402
from openfpga_app.server import create_app  # noqa: E402
from openfpga_app.session_backend import SessionBackend  # noqa: E402
from test_app_session import HOST, backend, call, client, say, text, view  # noqa: E402,F401
from test_mcp_tools import golden, make_config  # noqa: E402,F401


def put(client, name, data, overwrite=False):
    query = "name=%s%s" % (name, "&overwrite=1" if overwrite else "")
    return client.post(
        "/api/upload?" + query, content=data, headers={"Content-Type": "application/octet-stream"}
    )


def circuit(golden):
    return golden.circuit.read_bytes()


def test_a_file_goes_to_the_session_and_opens_one_if_there_is_none(client, backend, golden):
    answer = put(client, "mine.blif", circuit(golden))
    assert answer.status_code == 200
    assert answer.json()["data"] == {
        "name": "mine.blif",
        "kind": "circuit",
        "bytes": len(circuit(golden)),
        "loaded": False,  # a session with no architectures has nothing to read it with
    }
    shown = view(client)
    assert shown["session"]["id"] == "s1"
    assert shown["files"]["uploads"] == [
        {"name": "mine.blif", "kind": "circuit", "bytes": len(circuit(golden))}
    ]
    folder = backend.config.root / "s1" / "uploads"
    assert (folder / "mine.blif").read_bytes() == circuit(golden)
    assert [p.name for p in folder.iterdir()] == ["mine.blif"]  # no half file is left


def test_what_a_file_is_comes_from_its_first_lines_and_not_its_name(client, golden):
    assert put(client, "a.xml", golden.vpr_arch.read_bytes()).json()["data"]["kind"] == "vpr_arch"
    assert put(client, "b.xml", golden.openfpga_arch.read_bytes()).json()["data"]["kind"] == (
        "openfpga_arch"
    )
    assert put(client, "c.xml", b"<other/>").json()["data"]["kind"] == "other"
    assert put(client, "notes.txt", b"hello").json()["data"]["kind"] == "other"
    assert put(client, "d.blif", b"not a circuit").json()["data"]["kind"] == "other"


def test_the_tools_read_an_uploaded_file_by_its_name_and_other_sessions_do_not(
    client, backend, golden
):
    put(client, "mine.blif", circuit(golden))
    inputs = "/read_inputs vpr_arch=%s layout=%s openfpga_arch=%s circuit=mine.blif" % (
        golden.vpr_arch.name,
        golden.layout,
        golden.openfpga_arch.name,
    )
    assert call(say(client, inputs))["ok"] is True
    assert [s["status"] for s in view(client)["stages"]][:1] == ["reached"]
    # Another session has not got it
    client.post("/api/session/new", json={})
    other = call(say(client, inputs))
    assert other["ok"] is False
    assert other["result"]["diagnostics"][0]["kind"] == "file_not_found"


def test_the_names_and_types_that_are_not_safe_are_refused(client):
    for name in ("../x.blif", "a%2Fb.xml", ".hidden.xml", "x.exe", "noextension", "a b.xml", ""):
        answer = put(client, name, b"x")
        assert answer.status_code == 400, name
        assert answer.json()["diagnostics"][0]["kind"] in ("invalid_name", "invalid_type"), name
    assert put(client, "x" * 120 + ".xml", b"x").status_code == 400
    assert view(client)["session"] is None  # a refused upload does not open a session


def test_an_empty_file_is_refused(client):
    answer = put(client, "e.blif", b"")
    assert answer.status_code == 400
    assert answer.json()["diagnostics"][0]["kind"] == "empty"


def test_a_name_that_is_there_is_kept_unless_the_person_says_replace(client, backend):
    assert put(client, "m.blif", b".model a\n").status_code == 200
    again = put(client, "m.blif", b".model b\n")
    assert again.status_code == 409
    assert again.json()["diagnostics"][0]["kind"] == "file_exists"
    assert (backend.config.root / "s1" / "uploads" / "m.blif").read_bytes() == b".model a\n"
    assert put(client, "m.blif", b".model b\n", overwrite=True).status_code == 200
    assert (backend.config.root / "s1" / "uploads" / "m.blif").read_bytes() == b".model b\n"


def test_a_file_that_is_too_large_is_refused_and_nothing_is_written(client, backend, monkeypatch):
    monkeypatch.setattr(uploads, "MAX_FILE_BYTES", 100)
    answer = put(client, "big.blif", b"x" * 101)
    assert answer.status_code == 413
    assert answer.json()["diagnostics"][0]["kind"] == "too_large"
    assert not (backend.config.root / "s1" / "uploads").exists()


def test_a_body_that_does_not_say_its_size_is_cut_at_the_limit(client, backend, monkeypatch):
    monkeypatch.setattr(uploads, "MAX_FILE_BYTES", 100)

    def chunks():
        for _ in range(5):
            yield b"x" * 40

    answer = client.post("/api/upload?name=s.blif", content=chunks())
    assert answer.status_code == 413
    assert not (backend.config.root / "s1" / "uploads").exists()


def test_a_session_that_has_used_its_share_takes_no_more(openfpga_module, tmp_path, golden):
    backend = SessionBackend(make_config(tmp_path, golden, session_max_bytes=8000))
    static = tmp_path / "static"
    static.mkdir()
    (static / "index.html").write_text("x")
    try:
        with TestClient(
            create_app(AppConfig(port=8770, token="t"), backend, static),
            base_url="http://" + HOST,
        ) as client:
            client.headers["Authorization"] = "Bearer t"
            assert put(client, "a.blif", b"." * 5000).status_code == 200
            full = put(client, "b.blif", b"." * 5000)
            assert full.status_code == 413
            assert full.json()["diagnostics"][0]["kind"] == "session_full"
    finally:
        backend.close()


def test_an_upload_needs_the_token_and_the_origin_of_the_page(client):
    assert put(client, "a.blif", b"x").status_code == 200
    anonymous = TestClient(client.app, base_url="http://" + HOST)
    assert anonymous.post("/api/upload?name=a.blif", content=b"x").status_code == 401
    foreign = client.post(
        "/api/upload?name=b.blif", content=b"x", headers={"Origin": "http://evil.example"}
    )
    assert foreign.status_code == 403


def test_the_model_is_told_about_the_file_and_finds_it_in_its_list(
    openfpga_module, tmp_path, golden
):
    from openfpga_app.agent.scripted import ScriptedModel, say as say_
    from test_app_agent import _collect

    import asyncio

    model = ScriptedModel([say_("Yes, I see it.")])
    backend = SessionBackend(make_config(tmp_path, golden, call_timeout_s=120), model=model)
    try:
        answer = asyncio.run(backend.upload("mine.blif", circuit(golden), False))
        assert answer["ok"]
        assert "mine.blif" in backend._files_listing()
        line = [l for l in backend._files_listing().splitlines() if l.startswith("circuit:")][0]
        assert "mine.blif" in line  # first, with the circuits of the designs folders after it
        asyncio.run(_collect(backend.chat("Is it there?")))
        user = [m for m in model.requests[-1] if m["role"] == "user"][-1]["content"]
        assert "uploaded mine.blif (circuit) to session s1" in user
        assert "read_inputs can use it by its name" in user
    finally:
        backend.close()


def test_an_uploaded_file_can_be_removed_and_nothing_else_can(client, backend):
    assert put(client, "m.blif", b".model a\n").status_code == 200
    (backend.config.root / "s1" / "keep.txt").write_text("not an upload")
    gone = client.delete("/api/upload?name=m.blif")
    assert gone.status_code == 200
    assert gone.json()["data"] == {"name": "m.blif", "bytes": 9}
    assert view(client)["files"]["uploads"] == []
    assert not (backend.config.root / "s1" / "uploads" / "m.blif").exists()
    again = client.delete("/api/upload?name=m.blif")
    assert again.status_code == 404
    assert again.json()["diagnostics"][0]["kind"] == "file_not_found"
    assert client.delete("/api/upload?name=keep.txt").status_code == 404  # not in uploads
    assert client.delete("/api/upload?name=../keep.txt").status_code == 400
    assert (backend.config.root / "s1" / "keep.txt").exists()


def test_removing_needs_a_session_and_the_token(client):
    assert client.delete("/api/upload?name=a.blif").status_code == 404  # no session yet
    anonymous = TestClient(client.app, base_url="http://" + HOST)
    assert anonymous.delete("/api/upload?name=a.blif").status_code == 401


def test_the_model_is_told_that_a_file_is_gone(openfpga_module, tmp_path, golden):
    from openfpga_app.agent.scripted import ScriptedModel, say as say_
    from test_app_agent import _collect

    import asyncio

    model = ScriptedModel([say_("Understood.")])
    backend = SessionBackend(make_config(tmp_path, golden, call_timeout_s=120), model=model)
    try:
        assert asyncio.run(backend.upload("mine.blif", circuit(golden), False))["ok"]
        assert asyncio.run(backend.remove_upload("mine.blif"))["ok"]
        assert "mine.blif" not in backend._files_listing()
        asyncio.run(_collect(backend.chat("Is it there?")))
        user = [m for m in model.requests[-1] if m["role"] == "user"][-1]["content"]
        assert "removed the uploaded file mine.blif from session s1" in user
    finally:
        backend.close()


def test_the_demo_has_no_upload():
    with TestClient(
        create_app(AppConfig(port=8770, token="t"), DemoBackend(delay=0)),
        base_url="http://" + HOST,
    ) as client:
        client.headers["Authorization"] = "Bearer t"
        answer = put(client, "a.blif", b"x")
        assert answer.status_code == 404
        assert answer.json()["diagnostics"][0]["kind"] == "demo"
        assert client.delete("/api/upload?name=a.blif").status_code == 404
