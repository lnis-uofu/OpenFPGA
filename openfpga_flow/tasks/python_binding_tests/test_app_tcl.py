"""Tcl in the app: the mode tcl of the console (the Tcl interpreter of the session, whose commands are
those of the shell), the tool run_tcl when the app is asked for it, and `openfpga-app tcl` in a
terminal, attached to a running app or with a session of its own"""

import json
import os
import socket
import subprocess
import sys
import time
import urllib.request

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")
pytest.importorskip("_tkinter")

from test_app_session import backend, client, say, set_up, text, view  # noqa: E402,F401
from test_mcp_tools import golden, make_config  # noqa: E402,F401


def tcl(client, script):
    return client.post("/api/console", json={"command": script, "mode": "tcl"}).json()


def test_a_script_runs_in_the_session_and_the_page_follows(client, golden):
    assert "No session" in tcl(client, "version")["output"]
    set_up(client, golden, stages=())
    answer = tcl(
        client,
        "foreach stage {pack place} {\n    $stage\n}\nset used [dict get [utilization] device_utilization]\nexpr {$used > 0}",
    )
    assert answer["ok"], answer
    assert answer["result"] == "1" and answer["state_changed"]
    assert "pack" in answer["output"].lower()
    reached = [s["stage"] for s in view(client)["stages"] if s["status"] == "reached"]
    assert reached[-1] == "PLACED"
    # The variables stay for the next script
    again = tcl(client, "set used")
    assert again["ok"] and float(again["result"]) > 0 and not again["state_changed"]


def test_a_script_that_fails_shows_the_trace_of_tcl(client, golden):
    set_up(client, golden, stages=())
    answer = tcl(client, "proc go {} {\n    route\n}\ngo")
    assert not answer["ok"]
    assert "while executing" in answer["output"] and '(procedure "go" line 2)' in answer["output"]
    assert "exit is not available" in tcl(client, "exit")["output"]


def test_the_console_takes_a_mode_and_a_script_of_lines(client):
    assert client.post("/api/console", json={"command": "x", "mode": "python"}).status_code == 400
    assert client.post("/api/console", json={"command": "a\nb"}).status_code == 400
    assert client.post("/api/console", json={"command": "a\nb", "mode": "tcl"}).status_code == 200
    assert (
        client.post("/api/console", json={"command": "x" * 64001, "mode": "tcl"}).status_code == 400
    )


def test_the_demo_has_no_tcl():
    import asyncio

    from openfpga_app.backend import DemoBackend

    answer = asyncio.run(DemoBackend().console("version", "tcl"))
    assert not answer["ok"] and "not in the demo" in answer["output"]


def test_the_tool_of_the_model_is_there_when_the_app_is_asked_for_it(
    openfpga_module, tmp_path, golden
):
    from openfpga_app.session_backend import SessionBackend

    for asked in (False, True):
        backend = SessionBackend(make_config(tmp_path / str(asked), golden, tcl_tool=asked))
        try:
            assert ("run_tcl" in backend.tools) is asked
        finally:
            backend.close()


def test_the_chat_calls_the_tool_as_any_other(openfpga_module, tmp_path, golden):
    from openfpga_app.config import AppConfig
    from openfpga_app.server import create_app
    from openfpga_app.session_backend import SessionBackend
    from starlette.testclient import TestClient
    from test_app_session import HOST

    backend = SessionBackend(make_config(tmp_path, golden, call_timeout_s=120, tcl_tool=True))
    (tmp_path / "static").mkdir()
    (tmp_path / "static" / "index.html").write_text("<title>app</title>")
    try:
        app = create_app(AppConfig(port=8770, token="t"), backend, tmp_path / "static")
        with TestClient(app, base_url="http://" + HOST) as chat:
            chat.headers["Authorization"] = "Bearer t"
            say(chat, "/session_open name=run1")
            found = say(chat, '/run_tcl script="expr {6 * 7}"')
            (done,) = [e for e in found if e["type"] == "tool_call_finished"]
            assert done["ok"] and done["result"]["data"]["result"] == "42", done
            assert "/run_tcl" in text(say(chat, "/help"))
    finally:
        backend.close()


# openfpga-app tcl ---------------------------------------------------------------------------------


def free_port():
    with socket.socket() as s:
        s.bind(("127.0.0.1", 0))
        return s.getsockname()[1]


def app_tcl(environment, *args, stdin=None):
    return subprocess.run(
        [sys.executable, "-m", "openfpga_app", "tcl", *args],
        capture_output=True,
        text=True,
        env=environment,
        input=stdin,
        timeout=120,
    )


def test_a_terminal_runs_tcl_with_a_session_of_its_own(mcp_environment, tmp_path):
    done = app_tcl(
        mcp_environment,
        "--local",
        str(tmp_path / "run"),
        "-c",
        "set_app_option --name place.seed --value 5\nexpr {2 + 3}",
    )
    assert done.returncode == 0, done.stderr
    assert done.stdout.strip().endswith("5")
    assert (
        "set_app_option --name place.seed --value 5"
        in (tmp_path / "run" / "journal" / "replay.openfpga").read_text()
    )
    failed = app_tcl(mcp_environment, "--local", str(tmp_path / "run"), "-c", "pack")
    assert failed.returncode == 1 and "has not been read" in failed.stderr
    # Lines from a terminal: a brace that is open goes on on the next line
    session = app_tcl(
        mcp_environment,
        "--local",
        str(tmp_path / "run"),
        stdin='foreach x {1 2} {\nputs "x$x"\n}\nexit\n',
    )
    assert session.returncode == 0 and "x1\nx2" in session.stdout


def test_a_terminal_attaches_to_a_running_app(mcp_environment, tmp_path, golden):
    root = tmp_path / "workspace"
    port = free_port()
    designs = sorted(
        {str(golden.vpr_arch.parent), str(golden.openfpga_arch.parent), str(golden.circuit.parent)}
    )
    server = subprocess.Popen(
        [
            sys.executable,
            "-m",
            "openfpga_app",
            "--root",
            str(root),
            "--port",
            str(port),
            "--console",
        ]
        + [part for d in designs for part in ("--designs-dir", d)],
        env=mcp_environment,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
    )
    address = root / ".openfpga-app.json"
    try:
        deadline = time.monotonic() + 60
        while not address.exists() and time.monotonic() < deadline:
            assert server.poll() is None, server.stdout.read()
            time.sleep(0.2)
        assert address.exists()
        assert oct(address.stat().st_mode & 0o777) == "0o600"
        found = json.loads(address.read_text())
        assert "/?token" not in found["url"]
        no_session = app_tcl(mcp_environment, "--root", str(root), "-c", "version")
        assert no_session.returncode == 1 and "No session" in no_session.stderr
        request = urllib.request.Request(
            found["url"] + "/api/session/new",
            data=json.dumps({"name": "from_page"}).encode(),
            headers={
                "Authorization": "Bearer " + found["token"],
                "Content-Type": "application/json",
            },
            method="POST",
        )
        urllib.request.urlopen(request).read()
        done = app_tcl(
            mcp_environment,
            "--root",
            str(root),
            "-c",
            "set_app_option --name place.seed --value 4\nset seed 4",
        )
        assert done.returncode == 0 and done.stdout.strip().endswith("4"), done.stderr
        # The same interpreter as the console of the page: the variable is there
        again = app_tcl(
            mcp_environment, "--url", found["url"] + "/?token=" + found["token"], "-c", "set seed"
        )
        assert again.stdout.strip() == "4", again.stderr
        wrong = app_tcl(mcp_environment, "--url", found["url"] + "/?token=nope", "-c", "set seed")
        assert wrong.returncode == 2 and "token" in wrong.stderr
    finally:
        server.terminate()
        server.wait(30)
    assert not address.exists()
    missing = app_tcl(mcp_environment, "--root", str(root), "-c", "version")
    assert missing.returncode == 2 and "No app runs" in missing.stderr
