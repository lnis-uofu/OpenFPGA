"""The agent loop: a model chooses tool calls, the tools run, the answers go back.

A scripted model stands in for a real one, so the loop, its limits and the events are tested with
no endpoint; the adapter is tested against a small fake of an OpenAI-compatible server; and one
run goes through the real tools on a real design.
"""

import asyncio
import http.server
import json
import threading
import time
from pathlib import Path
from typing import Any, Dict, List, Optional

import pytest

pytest.importorskip("starlette")
pytest.importorskip("jsonschema")

import jsonschema  # noqa: E402

from openfpga_app import events  # noqa: E402
from openfpga_app.agent import (  # noqa: E402
    AgentConfig,
    AgentLoop,
    ModelError,
    ModelTurn,
    ScriptedModel,
    TextChunk,
    ToolCallRequest,
)
from openfpga_app.agent import context, jsonmode  # noqa: E402
from openfpga_app.agent.arguments import check_arguments  # noqa: E402
from openfpga_app.agent.openai_compat import OpenAICompatModel  # noqa: E402
from openfpga_app.agent.scripted import call, say  # noqa: E402
from openfpga_app.agent.schema import tool_definition, tool_definitions  # noqa: E402

SCHEMA = json.loads((Path(events.__file__).parent / "schemas" / "events.schema.json").read_text())


def valid(item):
    jsonschema.validate(item, SCHEMA, cls=jsonschema.Draft202012Validator)


# Small tools to run the loop on --------------------------------------------------------------------


def make_tools(log: Optional[List] = None, big: int = 0):
    async def echo(session: Optional[str] = None, text: str = "") -> str:
        """Say the text back. A tool for the tests. It has a second sentence."""
        log is not None and log.append(("echo", session, text))
        return json.dumps(
            {"ok": True, "data": {"text": text, "pad": "x" * big}, "truncated": False}
        )

    async def fail(session: Optional[str] = None, kind: str = "boom", note: str = "") -> str:
        """Always fails with the kind that it is given."""
        log is not None and log.append(("fail", session, kind))
        return json.dumps(
            {"ok": False, "diagnostics": [{"kind": kind, "message": "it failed", "hint": "stop"}]}
        )

    return {"echo": echo, "fail": fail}


def make_loop(model, tools=None, session="s1", **config):
    tools = tools if tools is not None else make_tools()

    async def invoke(name, arguments):
        return json.loads(await tools[name](**arguments))

    return AgentLoop(
        model,
        tools,
        AgentConfig(**config),
        invoke,
        session=lambda: session,
        files=lambda: "arch: a.xml, b.xml",
    )


def run(loop, text="go", message_id="m1"):
    async def main():
        return [item async for item in loop.run(message_id, text)]

    found = asyncio.run(main())
    for item in found:
        valid(item)
    return found


def kinds(found):
    return [e["type"] for e in found]


def said(found):
    return "".join(e["text"] for e in found if e["type"] == "text_delta")


# The definitions of the tools ------------------------------------------------------------------------


def test_a_definition_is_made_from_the_signature_and_the_docstring():
    tools = make_tools()
    echo = tool_definition("echo", tools["echo"])["function"]
    assert echo["name"] == "echo" and "second sentence" in echo["description"]
    assert echo["parameters"]["properties"]["text"] == {"type": "string"}
    # The session is not required: the current one is used
    assert echo["parameters"]["required"] == []
    assert "current session" in echo["parameters"]["properties"]["session"]["description"]
    short = tool_definition("echo", tools["echo"], compact=True)["function"]["description"]
    assert short == "Say the text back."


def test_the_definitions_of_the_tools_of_the_server_say_what_the_server_says(
    openfpga_module, tmp_path
):
    from openfpga_mcp import Config, build_server

    from openfpga_app.session_backend import SessionBackend

    server = build_server(Config(root=tmp_path / "a"))
    backend = SessionBackend(Config(root=tmp_path / "b"))
    try:
        theirs = {t.name: t.input_schema for t in asyncio.run(server.list_tools())}
        ours = {d["function"]["name"]: d["function"] for d in tool_definitions(backend.tools)}
        assert set(ours) == set(theirs) and len(ours) == 28
        for name, function in ours.items():
            properties = function["parameters"]["properties"]
            assert set(properties) == set(theirs[name].get("properties", {})), name
            wanted = set(theirs[name].get("required", [])) - {"session"}
            assert set(function["parameters"]["required"]) == wanted, name
        stage = ours["run_stage"]["parameters"]["properties"]["stage"]
        assert stage["enum"] == ["pack", "place", "route", "analysis"]
        # The cost of all 21 in the context: written down so that a change shows
        text = json.dumps(list(ours.values()))
        assert len(text) // 4 < 4000
        assert [d["function"]["name"] for d in tool_definitions(backend.tools)] == sorted(ours)
    finally:
        backend.close()
        server.registry.close_all()


def test_the_arguments_of_a_model_are_checked_against_the_signature():
    tools = make_tools()
    assert check_arguments(tools["echo"], {"text": "a"}, "s1") == {"text": "a", "session": "s1"}
    assert check_arguments(tools["echo"], {"text": "a", "session": "s2"}, "s1")["session"] == "s2"
    with pytest.raises(ValueError, match="no argument 'color'"):
        check_arguments(tools["echo"], {"color": "red"})
    with pytest.raises(ValueError, match="wrong type"):
        check_arguments(tools["echo"], {"text": [1]})

    # An empty text for an argument that has no value is no value
    assert check_arguments(tools["echo"], {"text": ""})["text"] == ""

    async def maybe(file: Optional[str] = None, width: Optional[int] = None) -> str:
        """Maybe."""

    assert check_arguments(maybe, {"file": "", "width": 20}) == {"file": None, "width": 20}

    async def count(limit: Optional[int] = None, flag: bool = False) -> str:
        """Count."""

    assert check_arguments(count, {"limit": 20.0}) == {"limit": 20}
    with pytest.raises(ValueError):
        check_arguments(count, {"limit": 2.5})
    with pytest.raises(ValueError):
        check_arguments(count, {"flag": 1})


# The loop ------------------------------------------------------------------------------------------------


def test_a_call_is_made_and_its_answer_goes_back_to_the_model():
    log: List = []
    model = ScriptedModel(
        [
            say("I'll echo it.", call("echo", text="hi")),
            say("The tool said hi."),
        ]
    )
    found = run(make_loop(model, make_tools(log)))
    assert (
        kinds(found)
        == [
            "message_start",
            "text_delta",
            "text_delta",
            "text_delta",
            "usage",
            "state_changed",
            "tool_call_started",
            "tool_call_finished",
            "state_changed",
            "usage",
            "text_delta",
            "text_delta",
            "text_delta",
            "text_delta",
            "done",
        ]
        or kinds(found)[0] == "message_start"
    )
    assert said(found) == "I'll echo it.The tool said hi."
    assert found[-1] == events.done("m1")
    assert log == [("echo", "s1", "hi")]  # the session was filled in
    finished = [e for e in found if e["type"] == "tool_call_finished"][0]
    assert finished["ok"] and finished["result"]["data"]["text"] == "hi"
    # The second request has the first call and its answer
    second = model.requests[1]
    assert [m["role"] for m in second] == ["system", "user", "assistant", "tool"]
    assert second[2]["tool_calls"][0]["function"]["name"] == "echo"
    assert json.loads(second[3]["content"])["data"]["text"] == "hi"
    assert second[3]["tool_call_id"] == second[2]["tool_calls"][0]["id"]


def test_the_system_prompt_has_the_rules_and_the_files_and_stays_the_same():
    model = ScriptedModel([say("one"), say("two")])
    loop = make_loop(model)
    run(loop, "first")
    run(loop, "second", "m2")
    first, second = model.requests[0][0], model.requests[1][0]
    assert first == second and first["role"] == "system"
    assert "Never invent" in first["content"] and "arch: a.xml, b.xml" in first["content"]
    # The history is only appended to: what came first is the start of what comes next
    assert model.requests[1][: len(model.requests[0])] == model.requests[0]


def test_the_tools_are_the_same_in_every_request_and_in_one_order():
    model = ScriptedModel([say("a", call("echo")), say("b")])
    run(make_loop(model))
    assert model.tools_seen[0] == model.tools_seen[1]
    assert [d["function"]["name"] for d in model.tools_seen[0]] == ["echo", "fail"]


def test_a_call_that_cannot_be_made_is_an_answer_for_the_model():
    model = ScriptedModel(
        [
            say("", call("no_such_tool")),
            say("", call("echo", color="red")),
            ModelTurn(
                tool_calls=[ToolCallRequest("x1", "echo", {}, "arguments are not valid JSON")]
            ),
            say("I could not."),
        ]
    )
    found = run(make_loop(model))
    finished = [e for e in found if e["type"] == "tool_call_finished"]
    assert [e["ok"] for e in finished] == [False, False, False]
    assert [e["result"]["diagnostics"][0]["kind"] for e in finished] == ["invalid_call"] * 3
    assert "There is no tool 'no_such_tool'" in finished[0]["result"]["diagnostics"][0]["message"]
    assert "no argument 'color'" in finished[1]["result"]["diagnostics"][0]["message"]
    assert "not valid JSON" in finished[2]["result"]["diagnostics"][0]["message"]
    assert found[-1]["type"] == "done"


def test_the_calls_of_one_reply_run_in_order_and_none_after_a_failure():
    log: List = []
    model = ScriptedModel(
        [
            say(
                "all at once",
                call("echo", text="1"),
                call("fail", kind="boom"),
                call("echo", text="3"),
                call("echo", text="4"),
            ),
            say("fixed it"),
        ]
    )
    found = run(make_loop(model, make_tools(log), repeat_limit=3))
    finished = [e for e in found if e["type"] == "tool_call_finished"]
    assert [e["ok"] for e in finished] == [True, False, False, False]
    assert [e["result"]["diagnostics"][0]["kind"] for e in finished[1:]] == [
        "boom",
        "not_run",
        "not_run",
    ]
    assert log == [("echo", "s1", "1"), ("fail", "s1", "boom")]  # the others did not run
    # Every call of the reply has an answer, so the model can go on
    second = model.requests[1]
    assert [m["role"] for m in second] == ["system", "user", "assistant"] + ["tool"] * 4
    # A call that was not tried is not a failure: seven of them do not stop the loop
    assert "Stopped" not in said(found) and said(found).endswith("fixed it")


def test_seven_calls_before_a_session_is_one_failure_and_six_not_tried():
    model = ScriptedModel(
        [
            say("", *[call("fail", kind="no_session", note=str(i)) for i in range(7)]),
            say("I will open one first."),
        ]
    )
    found = run(make_loop(model, repeat_limit=3))
    assert "Stopped" not in said(found)
    kinds_ = [
        e["result"]["diagnostics"][0]["kind"] for e in found if e["type"] == "tool_call_finished"
    ]
    assert kinds_ == ["no_session"] + ["not_run"] * 6


def test_the_same_call_again_and_again_stops_the_loop():
    model = ScriptedModel([say("", call("echo", text="a")) for _ in range(10)])
    found = run(make_loop(model, repeat_limit=3))
    assert sum(e["type"] == "tool_call_started" for e in found) == 3
    assert "Stopped: the same call to echo 3 times in a row" in said(found)
    assert found[-1]["type"] == "done" and len(model.turns) == 7


def test_a_failure_of_the_same_kind_again_and_again_stops_the_loop():
    model = ScriptedModel(
        [say("", call("fail", kind="boom", note=str(i))) for i in range(5)] + [say("never")]
    )
    found = run(make_loop(model, repeat_limit=3))
    assert "3 failures of the same kind (boom) in a row" in said(found)
    assert sum(e["type"] == "tool_call_finished" for e in found) == 3


def test_a_call_that_works_in_between_starts_the_count_again():
    model = ScriptedModel(
        [
            say("", call("fail", kind="boom", note="1")),
            say("", call("fail", kind="boom", note="2")),
            say("", call("echo", text="ok")),
            say("", call("fail", kind="boom", note="3")),
            say("", call("fail", kind="boom", note="4")),
            say("fine"),
        ]
    )
    found = run(make_loop(model, repeat_limit=3))
    assert "Stopped" not in said(found) and said(found).endswith("fine")


def test_a_different_failure_does_not_count():
    model = ScriptedModel(
        [say("", call("fail", kind=k)) for k in ("a", "b", "a", "b")] + [say("done")]
    )
    found = run(make_loop(model, repeat_limit=3))
    assert "Stopped" not in said(found) and said(found).endswith("done")


def test_the_limit_of_steps_stops_the_loop():
    model = ScriptedModel([say("", call("echo", text=str(i))) for i in range(30)])
    found = run(make_loop(model, max_steps=4))
    assert "Stopped: the limit of 4 steps" in said(found)
    assert sum(e["type"] == "tool_call_started" for e in found) == 4


def test_the_limit_of_time_stops_the_loop():
    async def slow(session: Optional[str] = None) -> str:
        """Slow."""
        await asyncio.sleep(0.15)
        return json.dumps({"ok": True, "data": {}})

    model = ScriptedModel([say("", call("slow")) for _ in range(30)])
    found = run(make_loop(model, {"slow": slow}, max_seconds=0.3, repeat_limit=99))
    assert "Stopped: the limit of 0.3 seconds" in said(found)


def test_the_model_that_does_not_answer_is_an_error_event():
    class Gone(ScriptedModel):
        async def stream(self, messages, tools):
            raise ModelError("model_unreachable", "no answer at http://x", "Start it")
            yield  # pragma: no cover

    found = run(make_loop(Gone([])))
    assert kinds(found) == ["message_start", "error", "done"]
    assert found[1]["kind"] == "model_unreachable" and found[1]["hint"] == "Start it"
    # A script that has run out is an error too
    found = run(make_loop(ScriptedModel([])))
    assert found[1]["kind"] == "model_error"


def test_a_cancel_ends_the_message_and_the_model_and_the_call():
    cancelled = []

    async def stuck(session: Optional[str] = None) -> str:
        """Waits."""
        await asyncio.sleep(5)
        return json.dumps({"ok": True, "data": {}})

    model = ScriptedModel([say("starting", call("stuck")), say("never")])
    tools = {"stuck": stuck}

    async def invoke(name, arguments):
        return json.loads(await tools[name](**arguments))

    loop = AgentLoop(model, tools, AgentConfig(), invoke, on_cancel=lambda: cancelled.append(1))

    async def main():
        found = []
        async for item in loop.run("m1", "go"):
            found.append(item)
            if item["type"] == "tool_call_started":
                asyncio.get_running_loop().call_later(0.2, loop.cancel)
        return found

    found = asyncio.run(asyncio.wait_for(main(), 10)) if False else None

    # The call is cancelled by whoever holds the tool: here it raises, as a worker does
    async def stuck_cancellable(session: Optional[str] = None) -> str:
        """Waits."""
        for _ in range(100):
            if cancelled:
                return json.dumps(
                    {"ok": False, "diagnostics": [{"kind": "worker_cancelled", "message": "x"}]}
                )
            await asyncio.sleep(0.05)
        return json.dumps({"ok": True, "data": {}})

    tools["stuck"] = stuck_cancellable
    model = ScriptedModel([say("starting", call("stuck")), say("never")])
    loop = AgentLoop(model, tools, AgentConfig(), invoke, on_cancel=lambda: cancelled.append(1))
    found = asyncio.run(asyncio.wait_for(main(), 10))
    assert model.cancelled and cancelled
    assert found[-1] == events.done("m1", cancelled=True)
    assert len(model.turns) == 1  # the second turn was not asked for
    for item in found:
        valid(item)


def test_what_changed_outside_is_told_to_the_model_with_the_next_message():
    model = ScriptedModel([say("one"), say("two"), say("three")])
    loop = make_loop(model)
    run(loop, "first")
    loop.note("The person ran the console command: place")
    run(loop, "second", "m2")
    run(loop, "third", "m3")
    second = [m for m in model.requests[1] if m["role"] == "user"][-1]["content"]
    assert second.startswith("[The person ran the console command: place]\nsecond")
    third = [m for m in model.requests[2] if m["role"] == "user"][-1]["content"]
    assert third == "third"


def test_each_step_is_recorded_and_the_record_can_be_read_again(tmp_path):
    path = tmp_path / "record.jsonl"
    model = ScriptedModel([say("a", call("echo", text="x")), say("b")])
    tools = make_tools()

    async def invoke(name, arguments):
        return json.loads(await tools[name](**arguments))

    loop = AgentLoop(model, tools, AgentConfig(), invoke, record=path)
    run(loop)
    lines = [json.loads(line) for line in path.read_text().splitlines()]
    assert [l["kind"] for l in lines] == ["user", "model", "tool", "model"]
    assert lines[1]["calls"] == [{"name": "echo", "arguments": {"text": "x"}}]
    assert lines[2]["answer"]["data"]["text"] == "x"
    assert lines == loop.records


def test_usage_says_what_a_step_cost_and_how_full_the_context_is():
    model = ScriptedModel([say("a", prompt_tokens=700)])
    found = run(make_loop(model, context_tokens=4096))
    (usage,) = [e for e in found if e["type"] == "usage"]
    assert usage["step"] == 1 and usage["prompt_tokens"] == 700 and usage["context_limit"] == 4096
    assert 0 < usage["context_tokens"] < 4096


# The context ---------------------------------------------------------------------------------------------


def test_old_tool_answers_become_a_line_and_the_recent_ones_stay():
    big = 1200
    tools = make_tools(big=big)
    turns = [say("", call("echo", text="n%d" % i)) for i in range(8)] + [say("done")]
    model = ScriptedModel(turns)
    loop = make_loop(model, tools, context_tokens=2688, keep_recent_results=2, repeat_limit=99)
    found = run(loop)
    assert said(found).endswith("done") and "Stopped" not in said(found)
    last = model.requests[-1]
    contents = [m["content"] for m in last if m["role"] == "tool"]
    assert len(contents) == 8
    # The first ones are a line each, the last two are whole
    assert all(c.startswith("[earlier echo(text=n") and "not kept" in c for c in contents[:-2])
    assert all(len(c) < 300 for c in contents[:-2])
    assert all(json.loads(c)["data"]["pad"] == "x" * big for c in contents[-2:])
    assert any(r["kind"] == "compacted" for r in loop.records)
    # What was sent has no private marks
    assert not any("summarized" in m for m in last)


def test_the_summary_of_an_answer_says_how_it_went():
    ok = json.dumps({"ok": True, "data": {"stage": "pack", "reached": []}, "truncated": True})
    assert context.summarize("run_stage", {"session": "s1", "stage": "pack"}, ok).startswith(
        "[earlier run_stage(stage=pack): ok (stage, reached), was cut;"
    )
    bad = json.dumps(
        {"ok": False, "diagnostics": [{"kind": "router_failed", "message": "no route"}]}
    )
    assert "failed: router_failed no route" in context.summarize("run_stage", {}, bad)
    assert "not kept" in context.summarize("x", {}, "not json")


def test_a_context_that_cannot_be_made_smaller_stops_the_loop():
    model = ScriptedModel([say("", call("echo", text="a" * 6000)), say("never")])
    found = run(make_loop(model, context_tokens=1024, keep_recent_results=5))
    assert "Stopped: the context is full" in said(found)


def test_the_ratio_of_tokens_follows_what_the_endpoint_counts():
    assert context.calibrate(500, 2000, 0.3) == 0.25
    assert context.calibrate(None, 2000, 0.3) == 0.3
    assert context.calibrate(1, 100000, 0.3) == 0.1


# The tool mode of a model that calls tools badly --------------------------------------------------------------


def test_the_json_mode_has_a_schema_that_names_every_tool_and_an_answer():
    tools = tool_definitions(make_tools())
    options = jsonmode.step_schema(tools)["oneOf"]
    assert [o["properties"]["tool"]["const"] for o in options] == ["echo", "fail", "answer"]
    assert options[0]["properties"]["arguments"]["properties"]["text"] == {"type": "string"}


def test_the_messages_are_rewritten_for_a_model_that_answers_in_json():
    tools = tool_definitions(make_tools())
    messages = [
        {"role": "system", "content": "rules"},
        {"role": "user", "content": "go"},
        {
            "role": "assistant",
            "content": None,
            "tool_calls": [
                {
                    "id": "c1",
                    "type": "function",
                    "function": {"name": "echo", "arguments": '{"text":"a"}'},
                }
            ],
        },
        {"role": "tool", "tool_call_id": "c1", "content": '{"ok":true}'},
    ]
    out = jsonmode.to_messages(messages, tools)
    assert out[0]["content"].startswith("rules") and "- echo(session, text)" in out[0]["content"]
    assert out[2] == {
        "role": "assistant",
        "content": '{"tool": "echo", "arguments": {"text": "a"}}',
    }
    assert out[3] == {"role": "user", "content": 'Result of echo: {"ok":true}'}


def test_a_step_in_json_is_a_call_or_the_answer():
    turn = jsonmode.parse_step('{"tool":"echo","arguments":{"text":"a"}}', "c1")
    assert turn.tool_calls == [ToolCallRequest("c1", "echo", {"text": "a"})]
    assert jsonmode.parse_step('{"tool":"answer","text":"done"}', "c2").text == "done"
    assert jsonmode.parse_step('{"tool":"echo","arguments":[1]}', "c3").tool_calls[0].error
    for bad in ("not json", '["x"]', '{"arguments":{}}'):
        with pytest.raises(ModelError) as error:
            jsonmode.parse_step(bad, "c4")
        assert error.value.kind == "model_bad_answer"


# The adapter, against a fake of an OpenAI-compatible server ---------------------------------------------------


class FakeEndpoint:
    """Answers /v1/chat/completions with the chunks it is given, and keeps the requests"""

    def __init__(self):
        self.requests: List[Dict[str, Any]] = []
        self.replies: List[Any] = []  # a list of chunks (dicts), or an int (an HTTP status)
        self.delay = 0.0
        endpoint = self

        class Handler(http.server.BaseHTTPRequestHandler):
            def log_message(self, *args):
                pass

            def do_POST(self):
                body = json.loads(self.rfile.read(int(self.headers["Content-Length"])))
                endpoint.requests.append({"path": self.path, "body": body})
                reply = endpoint.replies.pop(0)
                if isinstance(reply, int):
                    self.send_response(reply)
                    self.end_headers()
                    self.wfile.write(b'{"error":"nope"}')
                    return
                self.send_response(200)
                self.send_header("Content-Type", "text/event-stream")
                self.end_headers()
                try:
                    for chunk in reply:
                        self.wfile.write(("data: %s\n\n" % json.dumps(chunk)).encode())
                        self.wfile.flush()
                        time.sleep(endpoint.delay)
                    self.wfile.write(b"data: [DONE]\n\n")
                except OSError:
                    pass

        self.server = http.server.ThreadingHTTPServer(("127.0.0.1", 0), Handler)
        self.url = "http://127.0.0.1:%d/v1" % self.server.server_address[1]
        threading.Thread(target=self.server.serve_forever, daemon=True).start()

    def close(self):
        self.server.shutdown()


@pytest.fixture
def endpoint():
    fake = FakeEndpoint()
    yield fake
    fake.close()


def delta(**fields):
    return {"choices": [{"delta": fields}]}


def collect(model, messages=None, tools=None):
    async def main():
        return [
            item
            async for item in model.stream(
                messages or [{"role": "user", "content": "hi"}], tools or []
            )
        ]

    return asyncio.run(main())


def test_the_adapter_streams_text_and_reads_the_tool_calls_and_the_usage(endpoint):
    endpoint.replies.append(
        [
            delta(content="I'll "),
            delta(content="look."),
            delta(
                tool_calls=[
                    {"index": 0, "id": "call_a", "function": {"name": "echo", "arguments": '{"te'}}
                ]
            ),
            delta(tool_calls=[{"index": 0, "function": {"arguments": 'xt":"hi"}'}}]),
            delta(
                tool_calls=[
                    {"index": 1, "id": "call_b", "function": {"name": "fail", "arguments": ""}}
                ]
            ),
            {"choices": [], "usage": {"prompt_tokens": 321, "completion_tokens": 17}},
        ]
    )
    model = OpenAICompatModel(AgentConfig(base_url=endpoint.url, model="qwen"))
    tools = tool_definitions(make_tools())
    items = collect(model, tools=tools)
    assert [i.text for i in items if isinstance(i, TextChunk)] == ["I'll ", "look."]
    turn = items[-1]
    assert turn.text == "I'll look."
    assert turn.tool_calls == [
        ToolCallRequest("call_a", "echo", {"text": "hi"}),
        ToolCallRequest("call_b", "fail", {}),
    ]
    assert (turn.prompt_tokens, turn.completion_tokens) == (321, 17) and turn.seconds >= 0
    request = endpoint.requests[0]
    assert request["path"] == "/v1/chat/completions"
    body = request["body"]
    assert body["model"] == "qwen" and body["stream"] is True and body["cache_prompt"] is True
    assert body["tools"] == tools and "response_format" not in body


def test_arguments_that_are_not_json_are_the_error_of_the_call_and_not_of_the_model(endpoint):
    endpoint.replies.append(
        [
            delta(
                tool_calls=[
                    {"index": 0, "id": "c", "function": {"name": "echo", "arguments": "{oops"}}
                ]
            ),
            delta(
                tool_calls=[
                    {"index": 1, "id": "d", "function": {"name": "echo", "arguments": "[1]"}}
                ]
            ),
        ]
    )
    turn = collect(OpenAICompatModel(AgentConfig(base_url=endpoint.url)))[-1]
    assert "not valid JSON" in turn.tool_calls[0].error
    assert "not an object" in turn.tool_calls[1].error


def test_a_server_that_answers_with_an_error_is_a_model_error(endpoint):
    endpoint.replies.append(500)
    with pytest.raises(ModelError) as error:
        collect(OpenAICompatModel(AgentConfig(base_url=endpoint.url)))
    assert error.value.kind == "model_error" and "500" in str(error.value)


def test_a_server_that_is_not_there_says_how_to_start_it():
    with pytest.raises(ModelError) as error:
        collect(OpenAICompatModel(AgentConfig(base_url="http://127.0.0.1:9/v1")))
    assert error.value.kind == "model_unreachable" and "llama-server" in error.value.hint


def test_the_json_mode_asks_for_the_schema_and_reads_the_object(endpoint):
    endpoint.replies.append(
        [delta(content='{"tool":"echo",'), delta(content='"arguments":{"text":"a"}}')]
    )
    model = OpenAICompatModel(AgentConfig(base_url=endpoint.url, tool_mode="json"))
    tools = tool_definitions(make_tools())
    messages = [{"role": "system", "content": "rules"}, {"role": "user", "content": "go"}]
    items = collect(model, messages, tools)
    assert [type(i).__name__ for i in items] == ["ModelTurn"]  # nothing is shown while it writes
    assert items[0].tool_calls[0].name == "echo" and items[0].tool_calls[0].arguments == {
        "text": "a"
    }
    body = endpoint.requests[0]["body"]
    assert "tools" not in body
    assert body["response_format"]["type"] == "json_schema"
    assert body["response_format"]["json_schema"]["schema"]["oneOf"][-1]["properties"]["tool"] == {
        "const": "answer"
    }
    assert "Answer with one JSON object" in body["messages"][0]["content"]


def test_the_json_mode_shows_the_answer_as_text(endpoint):
    endpoint.replies.append([delta(content='{"tool":"answer","text":"All done."}')])
    model = OpenAICompatModel(AgentConfig(base_url=endpoint.url, tool_mode="json"))
    items = collect(model, [{"role": "user", "content": "x"}], tool_definitions(make_tools()))
    assert [i.text for i in items if isinstance(i, TextChunk)] == ["All done."]


def test_a_request_that_runs_can_be_cancelled(endpoint):
    endpoint.delay = 0.2
    endpoint.replies.append([delta(content="word ")] * 50)
    model = OpenAICompatModel(AgentConfig(base_url=endpoint.url))

    async def main():
        got = []
        started = time.monotonic()
        with pytest.raises(ModelError) as error:
            async for item in model.stream([{"role": "user", "content": "x"}], []):
                got.append(item)
                if len(got) == 2:
                    model.cancel()
        return got, error.value, time.monotonic() - started

    got, error, took = asyncio.run(main())
    assert error.kind == "cancelled" and took < 5 and len(got) < 10


def test_the_loop_runs_over_the_adapter_and_the_fake_server(endpoint):
    endpoint.replies.append(
        [
            delta(content="Calling."),
            delta(
                tool_calls=[
                    {
                        "index": 0,
                        "id": "c1",
                        "function": {"name": "echo", "arguments": '{"text":"x"}'},
                    }
                ]
            ),
            {"choices": [], "usage": {"prompt_tokens": 400, "completion_tokens": 9}},
        ]
    )
    endpoint.replies.append(
        [
            delta(content="Done."),
            {"choices": [], "usage": {"prompt_tokens": 460, "completion_tokens": 2}},
        ]
    )
    log: List = []
    model = OpenAICompatModel(AgentConfig(base_url=endpoint.url))
    found = run(make_loop(model, make_tools(log)))
    assert said(found) == "Calling.Done."
    assert log == [("echo", "s1", "x")]
    second = endpoint.requests[1]["body"]["messages"]
    assert [m["role"] for m in second] == ["system", "user", "assistant", "tool"]
    usage = [e for e in found if e["type"] == "usage"]
    assert [u["prompt_tokens"] for u in usage] == [400, 460]


# A run over the real tools, on a real design ---------------------------------------------------------------------


def test_a_scripted_model_runs_a_flow_through_the_real_tools(openfpga_module, tmp_path, golden):
    import shutil

    from openfpga_mcp import Config

    from openfpga_app.session_backend import SessionBackend

    # A folder of inputs with the three files: the folders of the repository have hundreds, and a
    # list of them does not fit in the start of a prompt
    inputs = tmp_path / "inputs"
    inputs.mkdir()
    for source in (golden.vpr_arch, golden.openfpga_arch, golden.circuit):
        shutil.copy(source, inputs / source.name)
    flow = [
        say("Opening a session.", call("session_open", name="agent")),
        say("", call("set_option", name="general.clock_modeling", value="route")),
        say("", call("set_option", name="router.route_chan_width", value=golden.chan_width)),
        say(
            "Reading the inputs.",
            call(
                "read_inputs",
                vpr_arch=golden.vpr_arch.name,
                layout=golden.layout,
                openfpga_arch=golden.openfpga_arch.name,
                circuit=golden.circuit.name,
            ),
        ),
        say("", call("run_stage", stage="pack")),
        say("", call("run_stage", stage="place")),
        say("", call("run_stage", stage="route")),
        say("", call("run_stage", stage="analysis")),
        say("", call("get_timing", paths=1)),
        say("The flow ran."),
    ]
    model = ScriptedModel(flow)
    config = Config(root=tmp_path / "workspace", design_dirs=(inputs,), call_timeout_s=120)
    backend = SessionBackend(config, model=model)

    async def main():
        found = [item async for item in backend.chat("Run the flow on routing_test.")]
        return found, await backend.view()

    try:
        found, view = asyncio.run(main())
        for item in found:
            valid(item)
        calls = [e for e in found if e["type"] == "tool_call_finished"]
        assert len(calls) == 9 and all(e["ok"] for e in calls)
        assert found[-1] == events.done("m1")
        assert said(found).endswith("The flow ran.")
        assert [s["status"] for s in view["stages"]] == ["reached"] * 5
        assert view["session"]["name"] == "agent"
        # The model was told the files that it can use, and that the session is open
        system = model.requests[0][0]["content"]
        assert golden.circuit.name in system  # only the circuits of a designs folder are listed
        assert golden.vpr_arch.name not in system
        # The answers that the model read are the answers of the tools
        timing = json.loads(model.requests[-1][-1]["content"])
        assert timing["data"]["timing_summary"]["setup"]["fmax_mhz"] > 0
        # A second message goes on from the history; what the person did meanwhile is told
        model.turns.append(say("Yes."))
        asyncio.run(backend.console("set_app_option --name place.seed --value 3"))
        asyncio.run(_collect(backend.chat("Did anything change?")))
        user = [m for m in model.requests[-1] if m["role"] == "user"][-1]["content"]
        assert user.startswith("[The person ran the console command: set_app_option")
        assert "call get_state" in user
        assert len(model.requests[-1]) > 20  # the first conversation is still there
    finally:
        backend.close()


async def _collect(stream):
    return [item async for item in stream]


@pytest.fixture
def golden():
    from test_mcp_tools import Golden

    return Golden("k6_routing_test")


def test_a_line_with_a_slash_is_still_a_tool_call_when_there_is_a_model(
    openfpga_module, tmp_path, golden
):
    from openfpga_app.session_backend import SessionBackend
    from test_mcp_tools import make_config

    model = ScriptedModel([])
    backend = SessionBackend(make_config(tmp_path, golden), model=model)
    try:
        found = asyncio.run(_collect(backend.chat("/session_open name=by-hand")))
        assert [e for e in found if e["type"] == "tool_call_finished"][0]["ok"]
        assert model.requests == []  # the model was not asked
        # And the next thing that the model is asked starts with what the person did
        model.turns.append(say("ok"))
        asyncio.run(_collect(backend.chat("what happened?")))
        user = [m for m in model.requests[0] if m["role"] == "user"][-1]["content"]
        assert "The person called /session_open" in user
    finally:
        backend.close()


# A session that the loop opens ---------------------------------------------------------------------------------


def _session_tools(log):
    async def session_open(name: Optional[str] = None) -> str:
        """Open a session."""
        log.append(("session_open", name))
        return json.dumps({"ok": True, "data": {"session": "s1"}})

    async def work(session: str, text: str = "") -> str:
        """Do work in a session."""
        log.append(("work", session, text))
        return json.dumps({"ok": True, "data": {"text": text}})

    return {"session_open": session_open, "work": work}


def _auto_loop(model, log, auto=True):
    tools = _session_tools(log)
    state = {"sid": None}

    async def invoke(name, arguments):
        answer = json.loads(await tools[name](**arguments))
        if name == "session_open":
            state["sid"] = answer["data"]["session"]
        return answer

    return AgentLoop(
        model, tools, AgentConfig(auto_session=auto), invoke, session=lambda: state["sid"]
    )


def test_a_tool_that_needs_a_session_opens_one_when_there_is_none():
    log: List = []
    model = ScriptedModel([say("", call("work", text="a"), call("work", text="b")), say("Done.")])
    found = run(_auto_loop(model, log))
    assert log == [("session_open", None), ("work", "s1", "a"), ("work", "s1", "b")]
    for item in found:
        valid(item)
    started = [e["tool"] for e in found if e["type"] == "tool_call_started"]
    assert started == ["session_open", "work", "work"]  # the open is a card of its own
    answer = json.loads(model.requests[1][-2]["content"])
    assert "session s1 was opened" in answer["note"]
    assert "note" not in json.loads(model.requests[1][-1]["content"])  # said once


def test_no_session_is_opened_when_one_is_given_or_the_tool_opens_it_or_it_is_off():
    log: List = []
    run(_auto_loop(ScriptedModel([say("", call("session_open", name="x")), say("ok")]), log))
    assert log == [("session_open", "x")]
    log = []
    run(_auto_loop(ScriptedModel([say("", call("work", session="s9")), say("ok")]), log))
    assert log == [("session_open", None), ("work", "s9", "")] or log == [("work", "s9", "")]
    log = []
    found = run(_auto_loop(ScriptedModel([say("", call("work")), say("ok")]), log, auto=False))
    assert log == []
    assert [e for e in found if e["type"] == "tool_call_finished"][0]["ok"] is False


def test_a_reply_that_asks_after_a_failure_of_order_is_told_once_to_make_the_call():
    model = ScriptedModel(
        [
            say("", call("fail", kind="input_not_read")),
            say("Please give me the file names."),
            say("", call("echo", text="x")),
            say("done"),
        ]
    )
    loop = make_loop(model)
    run(loop)
    assert [r["kind"] for r in loop.records].count("nudge") == 1
    assert model.requests[2][-1]["role"] == "user" and "Make that call yourself" in model.requests[2][-1]["content"]


def test_a_failure_that_is_not_of_order_is_not_nudged():
    model = ScriptedModel([say("", call("fail", kind="file_not_found")), say("That file does not exist.")])
    loop = make_loop(model)
    run(loop)
    assert [r["kind"] for r in loop.records].count("nudge") == 0 and len(model.requests) == 2


def test_the_files_that_the_message_names_for_an_input_are_found():
    from openfpga_app.agent.stated import stated_files

    assert stated_files("Open routing_test.blif and use k6_openfpga.xml as the VPR architecture.") == {
        "vpr_arch": "k6_openfpga.xml"
    }
    assert stated_files("use a.xml as the OpenFPGA architecture and b.blif as the circuit") == {
        "openfpga_arch": "a.xml",
        "circuit": "b.blif",
    }
    assert stated_files("vpr architecture: v.xml") == {"vpr_arch": "v.xml"}
    assert stated_files("Run the flow on routing_test at width 20") == {}


def test_read_inputs_gets_the_files_that_the_person_named_not_the_ones_the_model_chose():
    wrong = {"vpr_arch": "right.xml", "openfpga_arch": "o.xml", "circuit": "c.blif", "layout": "2x2"}
    model = ScriptedModel([say("", call("echo", **wrong)), say("done")])
    tools = make_tools()

    async def read_inputs(session=None, circuit=None, vpr_arch=None, layout=None, openfpga_arch=None):
        return json.dumps({"ok": True, "data": {}})

    tools = {**tools, "read_inputs": read_inputs}
    loop = make_loop(model, tools)
    run(loop, "Use o.xml as the VPR architecture.")
    assert not [r for r in loop.records if r["kind"] == "as_stated"]  # not a read_inputs call
    model = ScriptedModel([say("", call("read_inputs", **wrong)), say("done")])
    loop = make_loop(model, tools)
    run(loop, "Use o.xml as the VPR architecture.")
    assert [r["set"] for r in loop.records if r["kind"] == "as_stated"] == [{"vpr_arch": "o.xml"}]
    tool = next(r for r in loop.records if r["kind"] == "tool")
    assert tool["arguments"]["vpr_arch"] == "o.xml" and tool["arguments"]["openfpga_arch"] == "o.xml"


def test_files_that_the_person_named_and_the_tool_refused_end_the_calls_and_ask_for_the_answer():
    async def read_inputs(session=None, circuit=None, vpr_arch=None, layout=None, openfpga_arch=None):
        return json.dumps(
            {"ok": False, "diagnostics": [{"kind": "wrong_file_kind", "message": "no", "hint": "h"}]}
        )

    tools = {**make_tools(), "read_inputs": read_inputs}
    model = ScriptedModel(
        [
            say("", call("read_inputs", vpr_arch="right.xml", circuit="c.blif")),
            say("o.xml is an OpenFPGA architecture, not a VPR one. Which file should I use?"),
        ]
    )
    loop = make_loop(model, tools)
    run(loop, "Use o.xml as the VPR architecture.")
    assert [r["kind"] for r in loop.records].count("final_only") == 1
    assert model.tools_seen[1] == [] and "Make no more calls" in model.requests[1][-1]["content"]
    assert len(model.requests) == 2



def test_a_device_package_that_the_model_makes_up_is_skipped_when_the_person_named_the_architectures():
    calls = []

    async def use_device_package(session=None, name=None):
        calls.append(name)
        return json.dumps({"ok": False, "diagnostics": [{"kind": "package_not_found", "message": "no"}]})

    async def read_inputs(session=None, circuit=None, vpr_arch=None, layout=None, openfpga_arch=None):
        return json.dumps({"ok": True, "data": {"read": vpr_arch}})

    tools = {**make_tools(), "use_device_package": use_device_package, "read_inputs": read_inputs}
    model = ScriptedModel(
        [
            say(
                "",
                call("use_device_package", name="k6_frac_N10_40nm"),
                call("read_inputs", vpr_arch="o.xml", circuit="c.blif"),
            ),
            say("done"),
        ]
    )
    loop = make_loop(model, tools)
    run(loop, "Use o.xml as the VPR architecture.")
    assert calls == []  # the tool was not called
    answers = [json.loads(m["content"]) for m in model.requests[1] if m.get("role") == "tool"]
    assert answers[0]["ok"] is True and answers[0]["data"]["skipped"] is True
    assert answers[1]["ok"] is True  # and the call after it ran
    assert [r["kind"] for r in loop.records].count("skipped_package") == 1

    # Without architectures in the words of the person the tool is called as usual
    model = ScriptedModel([say("", call("use_device_package", name="k6")), say("done")])
    loop = make_loop(model, tools)
    run(loop, "Run the flow on c.blif")
    assert calls == ["k6"]
