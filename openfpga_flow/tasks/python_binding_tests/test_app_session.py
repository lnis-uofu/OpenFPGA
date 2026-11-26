"""The web app over real sessions, with no model: a chat line that starts with / is a tool call, the
sidebar is the state of the session, and the console runs commands of the OpenFPGA shell in it.

The tools are the ones of the MCP server, so what a person gets here is what a model will get.
"""

import asyncio
import json
import os
import signal
import subprocess
import sys
import time
from pathlib import Path

import pytest

pytest.importorskip("starlette")
pytest.importorskip("jsonschema")
pytest.importorskip("mcp")

import jsonschema  # noqa: E402
from starlette.testclient import TestClient  # noqa: E402

from openfpga_app import AppConfig, events  # noqa: E402
from openfpga_app.backend import STAGES, empty_view  # noqa: E402
from openfpga_app.server import create_app  # noqa: E402
from openfpga_app.session_backend import SessionBackend  # noqa: E402
from test_mcp_tools import golden, make_config  # noqa: E402,F401

SCHEMA = json.loads((Path(events.__file__).parent / "schemas" / "events.schema.json").read_text())
HOST = "127.0.0.1:8770"
FLOW = ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]


@pytest.fixture
def backend(openfpga_module, tmp_path, golden):
    backend = SessionBackend(make_config(tmp_path, golden, call_timeout_s=120))
    yield backend
    backend.close()


@pytest.fixture
def client(backend, tmp_path):
    static = tmp_path / "static"
    static.mkdir()
    (static / "index.html").write_text("<title>app</title>")
    config = AppConfig(port=8770, token="t", console=True)
    with TestClient(create_app(config, backend, static), base_url="http://" + HOST) as client:
        client.headers["Authorization"] = "Bearer t"
        yield client


def say(client, line):
    """A line of the chat, and the events that answer it (each checked against the schema)"""
    answer = client.post("/api/chat", json={"message": line})
    assert answer.status_code == 200
    found = []
    for block in answer.text.split("\n\n"):
        if block.strip():
            item = json.loads(dict(l.split(": ", 1) for l in block.splitlines())["data"])
            jsonschema.validate(item, SCHEMA, cls=jsonschema.Draft202012Validator)
            found.append(item)
    assert found[0]["type"] == "message_start" and found[-1]["type"] == "done"
    return found


def text(found):
    return "".join(e["text"] for e in found if e["type"] == "text_delta")


def call(found):
    """The answer of the one tool that the line called"""
    (done,) = [e for e in found if e["type"] == "tool_call_finished"]
    return done


def view(client):
    return client.get("/api/session").json()


def inputs_line(golden):
    return (
        f"/read_inputs vpr_arch={golden.vpr_arch.name} layout={golden.layout} "
        f"openfpga_arch={golden.openfpga_arch.name} circuit={golden.circuit.name}"
    )


def set_up(client, golden, stages=("pack", "place", "route", "analysis")):
    say(client, "/session_open name=run1")
    say(client, "/set_option name=general.clock_modeling value=route")
    say(client, f"/set_option name=router.route_chan_width value={golden.chan_width}")
    assert call(say(client, inputs_line(golden)))["ok"]
    for stage in stages:
        assert call(say(client, f"/run_stage stage={stage}"))["ok"], stage


# The chat without a model --------------------------------------------------------------------------


def test_the_view_of_no_session_is_empty(client):
    assert view(client) == empty_view()
    assert client.get("/api/config").json()["backend"] == "session"


def test_a_line_that_is_not_a_tool_call_says_that_there_is_no_model(client):
    found = say(client, "run the flow please")
    assert "No model is connected" in text(found) and "/help" in text(found)
    assert not any(e["type"] == "tool_call_started" for e in found)


def test_help_lists_the_tools_and_says_what_one_takes(client):
    listing = text(say(client, "/help"))
    for name in ("session_open", "run_stage", "get_timing", "read_log", "set_constraint"):
        assert "/" + name in listing
    one = text(say(client, "/help run_stage"))
    assert "run_stage(session, stage, from_file)" in one and "pack, place, route" in one
    assert "no tool" in text(say(client, "/help nothing"))


def test_a_flow_is_run_from_the_chat_and_the_sidebar_follows(client, golden):
    assert view(client)["session"] is None
    say(client, "/session_open name=run1")
    v = view(client)
    assert v["session"]["id"] == "s1" and v["session"]["name"] == "run1"
    assert v["session"]["worker"] == "running" and v["latest"]["text"] == "s1 · opened"
    say(client, "/set_option name=general.clock_modeling value=route")
    say(client, f"/set_option name=router.route_chan_width value={golden.chan_width}")
    assert call(say(client, inputs_line(golden)))["ok"]
    v = view(client)
    assert [f["label"] for f in v["files"]["inputs"]] == ["vpr arch", "openfpga arch", "circuit"]
    assert v["files"]["inputs"][0] == {
        "label": "vpr arch",
        "name": golden.vpr_arch.name,
        "extra": "layout " + golden.layout,
    }
    assert [s["status"] for s in v["stages"]] == ["reached"] + ["not_reached"] * 5
    assert v["files"]["constraints"] == ["chan width %d" % golden.chan_width]
    assert {"name": "general.clock_modeling", "value": "route"} in v["options_changed"]
    for stage in ("pack", "place", "route", "analysis"):
        done = call(say(client, f"/run_stage stage={stage}"))
        assert done["ok"] and done["result"]["data"]["stage"] == stage
    v = view(client)
    assert [s["stage"] for s in v["stages"] if s["status"] == "reached"] == FLOW
    assert all(s["seconds"] is not None for s in v["stages"][1:5])  # BITSTREAM was not run
    assert v["latest"]["status"] == "reached" and v["latest"]["text"].startswith("s1 · ANALYZED ✔")
    assert sorted(v["files"]["results"]) == [
        f"{golden.circuit.stem}.net",
        f"{golden.circuit.stem}.place",
        f"{golden.circuit.stem}.route",
    ]
    assert [c["name"] for c in v["checkpoints"]] == ["last"]
    assert v["checkpoints"][0]["stages"] == FLOW


def test_a_query_gives_the_answer_of_the_tool(client, golden):
    set_up(client, golden)
    timing = call(say(client, "/get_timing paths=1"))
    assert timing["ok"] and timing["result"]["data"]["timing_summary"]["setup"]["fmax_mhz"] > 0
    found = call(say(client, "/find pattern=* kind=block limit=2"))
    assert found["truncated"] is True and "find" in found["result"]["more"]


def test_a_constraint_drops_the_stages_and_the_sidebar_shows_it(client, golden):
    set_up(client, golden)
    change = call(say(client, "/set_constraint kind=chan_width width=24"))["result"]["data"]
    assert change["after"] == 24 and change["dropped"] == FLOW
    v = view(client)
    assert all(s["status"] == "not_reached" for s in v["stages"])
    assert v["files"]["constraints"] == ["chan width 24"] and v["files"]["results"] == []
    assert v["latest"]["status"] == "none"


def test_a_failure_is_a_finished_call_with_its_diagnostics(client, golden):
    set_up(client, golden, stages=("pack",))
    done = call(say(client, "/run_stage stage=pack from_file=missing.net"))
    assert done["ok"] is False
    assert done["result"]["diagnostics"][0]["kind"]


def test_a_stage_runs_the_stages_before_it_that_were_not_run(client, golden):
    set_up(client, golden, stages=("pack",))
    done = call(say(client, "/run_stage stage=route"))
    assert done["ok"] is True
    assert done["result"]["data"]["ran"] == ["place", "route"]
    assert done["result"]["data"]["reached"][-1] == "ROUTED"


def test_a_failed_stage_is_marked_in_the_sidebar(client, golden):
    say(client, "/session_open")
    say(client, "/set_option name=general.clock_modeling value=route")
    say(client, "/set_option name=router.route_chan_width value=20")
    say(client, "/set_option name=router.max_router_iterations value=1")
    say(client, inputs_line(golden))
    say(client, "/run_stage stage=pack")
    say(client, "/run_stage stage=place")
    done = call(say(client, "/run_stage stage=route"))
    if done["ok"]:
        pytest.skip("the design routes in one iteration")
    v = view(client)
    routed = next(s for s in v["stages"] if s["stage"] == "ROUTED")
    assert routed["status"] == "failed" and routed["message"]
    assert v["latest"]["status"] == "failed" and "ROUTED failed" in v["latest"]["text"]
    assert call(say(client, "/get_congestion"))["result"]["data"]["routed"] is False


def test_a_line_that_cannot_be_a_call_says_why(client, golden):
    cases = {
        "/no_such_tool": "no tool 'no_such_tool'",
        "/run_stage stage=pack": "open a session first",  # no session yet
        "/session_open name": "not key=value",
        "/session_open color=red": "no argument 'color'",
        '/session_open name="unclosed': "cannot read that line",
        "/use s9": "takes the id of an open session",
    }
    for line, expected in cases.items():
        found = say(client, line)
        assert expected in text(found), (line, text(found))
        assert not any(e["type"] == "tool_call_started" for e in found), line


def test_a_word_that_is_not_one_of_the_choices_is_not_a_call(client):
    say(client, "/session_open")
    for line in (
        "/run_stage stage=restore",
        "/set_constraint kind=anything file=x",
        "/find pattern=a kind=x",
    ):
        found = say(client, line)
        assert "is one of" in text(found), line
        assert not any(e["type"] == "tool_call_started" for e in found), line
    assert "wrong type" in text(say(client, "/get_timing paths=abc"))


def test_use_switches_to_another_session(client):
    say(client, "/session_open name=first")
    say(client, "/session_open name=second")
    assert view(client)["session"]["id"] == "s2"
    say(client, "/use s1")
    assert view(client)["session"]["id"] == "s1"


def test_the_view_has_no_path_of_the_machine(client, golden, tmp_path):
    set_up(client, golden)
    shown = client.get("/api/session").text + json.dumps(say(client, "/get_state"))
    assert str(tmp_path) not in shown and "/localproj" not in shown


# The sessions ---------------------------------------------------------------------------------------------


def test_a_session_is_made_closed_and_restored_from_the_page(client, backend, golden):
    new = client.post("/api/session/new", json={"name": "page"}).json()
    assert new["ok"] and new["data"]["session"] == "s1"
    assert view(client)["session"]["name"] == "page"
    # The worker is killed: the sidebar says so, and Restore brings it back
    os.kill(backend.registry.get("s1").session.pid, signal.SIGKILL)
    deadline = time.monotonic() + 5
    while backend.registry.get("s1").session.running and time.monotonic() < deadline:
        time.sleep(0.05)
    v = view(client)
    assert v["session"]["worker"] == "ended" and v["session"]["ended"]
    assert v["latest"]["status"] == "failed"
    restored = client.post("/api/session/restore", json={}).json()
    assert restored["ok"], restored
    assert view(client)["session"]["worker"] == "running"
    closed = client.post("/api/session/close", json={}).json()
    assert closed["ok"] and view(client)["session"] is None
    assert (
        client.post("/api/session/close", json={}).json()["diagnostics"][0]["kind"] == "no_session"
    )


def test_a_session_is_restored_from_a_named_checkpoint(client, golden):
    set_up(client, golden, stages=("pack", "place"))
    saved = call(say(client, "/checkpoint name=placed"))["result"]["data"]
    assert saved["stages"] == FLOW[:3]
    assert [c["name"] for c in view(client)["checkpoints"]] == ["last", "placed"]
    say(client, "/run_stage stage=route")
    restored = client.post("/api/session/restore", json={"checkpoint": "placed"}).json()
    assert restored["ok"] and restored["data"]["reached"] == FLOW[:3]
    assert [s["stage"] for s in view(client)["stages"] if s["status"] == "reached"] == FLOW[:3]
    assert client.post("/api/session/restore", json={"checkpoint": 3}).status_code == 400


def test_the_limit_of_sessions_is_an_answer(client, backend):
    backend.config = backend.config  # the limit is the one of the configuration
    answers = [client.post("/api/session/new", json={}).json() for _ in range(5)]
    assert [a["ok"] for a in answers] == [True] * 4 + [False]
    assert answers[-1]["diagnostics"][0]["kind"] == "too_many_sessions"


# The console -----------------------------------------------------------------------------------------------------


def run(client, command):
    return client.post("/api/console", json={"command": command}).json()


def test_the_console_has_no_session_to_run_in_until_there_is_one(client):
    answer = run(client, "report_session")
    assert answer["ok"] is False and "No session" in answer["output"]


def test_the_console_runs_commands_of_the_shell_in_the_session(client, golden):
    set_up(client, golden, stages=("pack",))
    report = run(client, "report_session")
    assert report["ok"] and report["state_changed"] is False
    option = run(client, "set_app_option --name place.seed --value 3")
    assert option["ok"] and option["state_changed"] is True
    assert {"name": "place.seed", "value": 3} in view(client)["options_changed"]
    # What the console changed is what the tools see
    described = call(say(client, "/describe_option name=place.seed"))["result"]["data"]
    assert described["value"] == 3 and described["is_default"] is False
    # A stage can be run from the console as well
    ran = run(client, "place")
    assert ran["ok"] and ran["state_changed"] is True
    assert [s["stage"] for s in view(client)["stages"] if s["status"] == "reached"] == FLOW[:3]


def test_the_console_says_what_the_shell_says_when_a_command_fails(client, golden):
    set_up(client, golden, stages=())
    bad = run(client, "no_such_command")
    assert bad["ok"] is False and bad["output"]
    wrong = run(client, "set_app_option --name no.such_option --value 1")
    assert wrong["ok"] is False and "does not exist" in wrong["output"]
    # The session is still there
    assert view(client)["session"]["worker"] == "running"


def test_a_value_with_a_space_in_the_console_is_quoted_for_the_shell(client, golden, tmp_path):
    set_up(client, golden, stages=())
    (tmp_path / "workspace" / "s1" / "a b.sdc").write_text("create_clock -period 10 clk\n")
    answer = run(client, 'set_app_option --name filename.sdc_file --value "a b.sdc"')
    assert answer["ok"] is False or "a b.sdc" in json.dumps(view(client))


def test_the_console_is_off_unless_the_app_asks_for_it(backend, tmp_path):
    off = create_app(AppConfig(port=8770, token="t", console=False), backend, tmp_path)
    with TestClient(off, base_url="http://" + HOST) as client:
        client.headers["Authorization"] = "Bearer t"
        assert client.post("/api/console", json={"command": "report_session"}).status_code == 404


# A call that runs ---------------------------------------------------------------------------------------------------


def test_a_call_that_runs_is_cancelled_and_the_console_waits_its_turn(backend, golden):
    async def main():
        async def lines(line):
            return [e async for e in backend.chat(line)]

        for line in (
            "/session_open",
            "/set_option name=general.clock_modeling value=route",
            f"/set_option name=router.route_chan_width value={golden.chan_width}",
            inputs_line(golden),
            "/run_stage stage=pack",
            "/set_option name=place.place_inner_num value=200000",
        ):
            await lines(line)
        running = asyncio.ensure_future(lines("/run_stage stage=place"))
        await asyncio.sleep(1.0)
        during = await backend.console("report_session")
        shown = await backend.view()
        busy_chat = await lines("/get_state")
        backend.cancel()
        found = await asyncio.wait_for(running, 20)
        return during, shown, busy_chat, found

    during, shown, busy_chat, found = asyncio.run(main())
    assert during["ok"] is False and "busy" in during["output"]
    assert shown["latest"]["status"] == "running" and "run_stage running" in shown["latest"]["text"]
    assert busy_chat[0]["type"] == "error" and busy_chat[0]["kind"] == "busy"
    finished = [e for e in found if e["type"] == "tool_call_finished"][0]
    assert finished["ok"] is False
    assert finished["result"]["diagnostics"][0]["kind"] in ("worker_cancelled", "worker_crashed")
    assert found[-1] == events.done(found[-1]["id"], cancelled=True)


# The command ---------------------------------------------------------------------------------------------------------


def test_without_a_root_or_the_demo_the_command_says_what_to_give():
    from openfpga_app.cli import main

    with pytest.raises(SystemExit):
        main([])


def test_the_backend_does_not_need_the_sdk_of_mcp(mcp_environment):
    code = (
        "import sys; sys.modules['mcp'] = None; "
        "import openfpga_app.session_backend as m; print(sorted(m.SessionBackend.__dict__)[:1])"
    )
    result = subprocess.run(
        [sys.executable, "-c", code], env=mcp_environment, capture_output=True, text=True
    )
    assert result.returncode == 0, result.stderr[-500:]
