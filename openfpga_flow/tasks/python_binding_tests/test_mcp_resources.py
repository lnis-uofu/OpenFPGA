"""The resources and the prompts of the MCP server: listed, readable, bounded, and true.

What they say is compared with what the tools and the queries say, and the prompts only name tools
that exist.
"""

import json
import re

import pytest

pytest.importorskip("mcp")

from test_mcp_tools import FLOW, Script, golden, make_config, open_flow, within  # noqa: E402,F401

from openfpga_mcp import build_server  # noqa: E402


@pytest.fixture
def script(openfpga_module, tmp_path, golden):
    server = build_server(make_config(tmp_path, golden, call_timeout_s=120))
    script = Script(server)
    yield script
    script.close()


def read(script, uri):
    async def steps(call):
        result = await script.client.read_resource(uri)
        return json.loads(result.contents[0].text)

    return script.run(steps)


def test_the_resources_are_listed(script):
    async def steps(call):
        client = script.client
        return (
            [r.uri for r in (await client.list_resources()).resources],
            [t.uri_template for t in (await client.list_resource_templates()).resource_templates],
            [p.name for p in (await client.list_prompts()).prompts],
        )

    resources, templates, prompts = script.run(steps)
    assert sorted(resources) == ["openfpga://options", "openfpga://stages"]
    assert sorted(templates) == [
        "openfpga://options/{stage}",
        "openfpga://sessions/{session}/checkpoints",
    ]
    assert sorted(prompts) == ["what_is_the_state", "why_did_it_fail"]


def test_the_graph_of_the_stages_is_the_one_of_the_flow(script):
    answer = read(script, "openfpga://stages")
    assert answer["ok"]
    graph = {s["stage"]: s for s in answer["data"]["stages"]}
    assert graph["DEVICE"]["requires"] is None
    assert [graph[s]["requires"] for s in ("PACKED", "PLACED", "ROUTED", "ANALYZED")] == FLOW[:4]
    assert graph["PLACED"]["commands"] == ["place", "read_placement"]
    assert graph["PLACED"]["dropped_by_change"][:3] == ["PLACED", "ROUTED", "ANALYZED"]
    within(answer)


def test_the_catalogue_agrees_with_what_a_session_says(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=("pack",))
        return await call("list_options", session=sid, stage="place", limit=200)

    listed = script.run(steps)["data"]["stages"][0]
    counts = {
        s["stage"]: s["options"] for s in read(script, "openfpga://options")["data"]["stages"]
    }
    assert counts["PLACED"] == listed["options"]["total"]
    place = read(script, "openfpga://options/place")
    assert place["ok"] and place["data"]["stage"] == "PLACED"
    # The catalogue has defaults: what a session set is not in it
    assert all("value" not in o for o in place["data"]["items"])
    assert place["truncated"] is True and "list_options" in place["more"]
    within(place)
    assert read(script, "openfpga://options/PLACED")["data"]["stage"] == "PLACED"
    unknown = read(script, "openfpga://options/nothing")
    assert unknown["ok"] is False and "PLACED" in unknown["diagnostics"][0]["message"]


def test_the_checkpoints_of_a_session_are_listed(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=("pack", "place"))
        await call("set_option", session=sid, name="place.seed", value=2)
        await call("run_stage", session=sid, stage="place")
        await call("checkpoint", session=sid, name="placed")
        return sid

    sid = script.run(steps)
    answer = read(script, f"openfpga://sessions/{sid}/checkpoints")
    by_name = {c["name"]: c for c in answer["data"]["checkpoints"]}
    assert set(by_name) == {"last", "placed"}
    assert by_name["last"]["stages"] == by_name["placed"]["stages"] == FLOW[:3]
    assert by_name["placed"]["kind"] == "named"
    assert "place.seed" in by_name["last"]["options_changed"]
    # Nothing of the machine: no path of the inputs
    shown = json.dumps(answer)
    assert str(script.server.registry.config.root) not in shown and "/localproj" not in shown


def test_a_session_that_is_not_there_is_an_answer(script):
    answer = read(script, "openfpga://sessions/s9/checkpoints")
    assert answer["ok"] is False and answer["diagnostics"][0]["kind"] == "unknown_session"

    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        return read_empty(sid)

    def read_empty(sid):
        return sid

    sid = script.run(steps)
    assert read(script, f"openfpga://sessions/{sid}/checkpoints")["data"]["checkpoints"] == []


def test_the_prompts_name_tools_that_exist(script):
    async def steps(call):
        client = script.client
        tools = {t.name for t in (await client.list_tools()).tools}
        texts = {}
        for name in ("what_is_the_state", "why_did_it_fail"):
            result = await client.get_prompt(name, {"session": "s7"})
            texts[name] = result.messages[0].content.text
        return tools, texts

    tools, texts = script.run(steps)
    for name, text in texts.items():
        assert "s7" in text
        said = set(re.findall(r"\b(?:get|set|run|read|list|describe)_[a-z_]+\b", text))
        assert said and said <= tools, (name, said - tools)
    assert (
        "get_congestion" in texts["why_did_it_fail"] and "get_state" in texts["what_is_the_state"]
    )
