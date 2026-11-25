"""The tools of the MCP server for the flow, the options and the queries.

A scripted client (no model) runs a flow through the tools and reads every query. What a tool says
is compared with the query of the session that it calls, the sizes are checked against the budget,
and each kind of failure is made on purpose.
"""

import asyncio
import json
import os
import sys
import threading
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from mcp.client import Client  # noqa: E402

from openfpga_mcp import Config, build_server  # noqa: E402
from openfpga_mcp import budget  # noqa: E402
from test_equivalence import Golden  # noqa: E402

FLOW = ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]


@pytest.fixture
def golden():
    return Golden("k6_routing_test")


def make_config(tmp_path, golden, **kwargs):
    folders = {golden.vpr_arch.parent, golden.openfpga_arch.parent, golden.circuit.parent}
    return Config(root=tmp_path / "workspace", design_dirs=tuple(folders), **kwargs)


class Script:
    """A client of a server that stays connected between the steps of a test.

    The client runs in a thread of its own: the server of a client in the same process lives as
    long as the connection, and the sessions end with it"""

    def __init__(self, server):
        self.server = server
        self.loop = asyncio.new_event_loop()
        self.thread = threading.Thread(target=self.loop.run_forever, daemon=True)
        self.thread.start()
        self.ready = threading.Event()
        self.client = None
        self.stop = None
        self.main = asyncio.run_coroutine_threadsafe(self._main(), self.loop)
        if not self.ready.wait(30):
            raise RuntimeError("the client did not connect")

    async def _main(self):
        self.stop = asyncio.Event()
        async with Client(self.server) as client:
            self.client = client
            self.ready.set()
            await self.stop.wait()

    async def call(self, tool, **arguments):
        result = await self.client.call_tool(tool, arguments)
        text = result.content[0].text
        return json.loads(text) if not result.is_error else {"error": text}

    def run(self, steps):
        """Run steps(call), a coroutine function, and give what it returns"""
        return asyncio.run_coroutine_threadsafe(steps(self.call), self.loop).result(300)

    def close(self):
        self.loop.call_soon_threadsafe(self.stop.set)
        self.main.result(30)
        self.loop.call_soon_threadsafe(self.loop.stop)
        self.thread.join(10)
        self.server.registry.close_all()


@pytest.fixture
def script(openfpga_module, tmp_path, golden):
    server = build_server(make_config(tmp_path, golden, call_timeout_s=120))
    script = Script(server)
    yield script
    script.close()


def read_inputs(call, golden, session, **changes):
    arguments = dict(
        session=session,
        vpr_arch=golden.vpr_arch.name,
        layout=golden.layout,
        openfpga_arch=golden.openfpga_arch.name,
        circuit=golden.circuit.name,
    )
    arguments.update(changes)
    return call("read_inputs", **arguments)


async def open_flow(call, golden, stages=("pack", "place", "route", "analysis")):
    sid = (await call("session_open"))["data"]["session"]
    await call("set_option", session=sid, name="general.clock_modeling", value="route")
    await call("set_option", session=sid, name="router.route_chan_width", value=golden.chan_width)
    answer = await read_inputs(call, golden, sid)
    assert answer["ok"], answer
    for stage in stages:
        answer = await call("run_stage", session=sid, stage=stage)
        assert answer["ok"], answer
    return sid


def within(answer, limit=4000):
    assert len(json.dumps(answer, separators=(",", ":"), ensure_ascii=False)) <= limit


# The flow ----------------------------------------------------------------------------------


def test_a_flow_runs_through_the_tools(script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        await call("set_option", session=sid, name="general.clock_modeling", value="route")
        await call("set_option", session=sid, name="router.route_chan_width", value=20)
        inputs = await read_inputs(call, golden, sid)
        before = await call("get_state", session=sid)
        stages = [await call("run_stage", session=sid, stage=s) for s in ("pack", "place")]
        stages += [await call("run_stage", session=sid, stage=s) for s in ("route", "analysis")]
        return sid, inputs, before, stages, await call("get_state", session=sid)

    sid, inputs, before, stages, after = script.run(steps)
    assert inputs["ok"] and inputs["data"]["kind"] == "session"
    assert before["data"]["state"]["reached"] == ["DEVICE"]
    assert before["data"]["run"]["stages"] == []
    assert [s["data"]["reached"][-1] for s in stages] == FLOW[1:]
    assert all(s["data"]["seconds"] >= 0 for s in stages)
    assert after["data"]["state"]["reached"] == FLOW
    assert [s["stage"] for s in after["data"]["run"]["stages"]] == FLOW[1:]
    assert after["data"]["run"]["routing"]["channel_width"] == 20
    # The workspace is not shown: only names inside it
    shown = json.dumps(after)
    assert str(script.server.registry.config.root) not in shown
    for folder in script.server.registry.config.design_dirs:
        assert str(folder) not in shown
    assert after["data"]["state"]["inputs"]["circuit"]["file"] == golden.circuit.name
    within(after)


def test_the_state_of_a_session_without_inputs(script):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        return await call("get_state", session=sid)

    state = script.run(steps)
    assert state["ok"] and state["data"]["run"] is None
    assert state["data"]["state"]["reached"] == []


# The queries -------------------------------------------------------------------------------


@pytest.fixture
def flow(script, golden):
    holder = {}

    async def steps(call):
        holder["sid"] = await open_flow(call, golden)
        return holder["sid"]

    script.run(steps)
    return script, holder["sid"]


def ask(script, sid, tool, **arguments):
    async def steps(call):
        return await call(tool, session=sid, **arguments)

    return script.run(steps)


def test_every_query_answers_within_the_budget(flow, golden):
    script, sid = flow
    queries = [
        ("get_timing", {}),
        ("get_device", {}),
        ("get_device", {"tile_type": "io"}),
        ("get_utilization", {}),
        ("find", {"pattern": "*"}),
        ("find", {"pattern": "*", "kind": "net"}),
        ("find", {"pattern": "*", "kind": "cluster"}),
        ("get_placement", {}),
        ("get_congestion", {}),
        ("list_options", {"stage": "place"}),
        ("list_options", {}),
        ("describe_option", {"name": "place.seed"}),
    ]
    for tool, arguments in queries:
        answer = ask(script, sid, tool, **arguments)
        assert answer["ok"], (tool, arguments, json.dumps(answer)[:3000])
        within(answer)
        assert answer["truncated"] in (True, False)
        if answer["truncated"]:
            assert answer["more"], (tool, arguments)


def test_the_timing_is_the_one_of_the_session(flow, golden):
    script, sid = flow
    answer = ask(script, sid, "get_timing", paths=2)
    summary = answer["data"]["timing_summary"]
    assert summary["kind"] == "timing_summary" and summary["schema_version"] == 1
    assert summary["setup"]["critical_path_delay_ns"] > 0
    paths = answer["data"]["critical_paths"]["paths"]
    assert 1 <= len(paths) <= 2
    assert paths[0]["slack_ns"] == pytest.approx(summary["setup"]["worst_negative_slack_ns"], abs=1)


def test_a_block_and_a_net_are_found_and_read(flow):
    script, sid = flow
    found = ask(script, sid, "find", pattern="*", kind="block", limit=3)
    items = found["data"]["blocks"]["items"]
    assert 1 <= len(items) <= 3
    block = ask(script, sid, "get_block", block=items[0]["name"])
    assert block["ok"] and block["data"]["name"] == items[0]["name"]
    nets = ask(script, sid, "find", pattern="*", kind="net", limit=3)["data"]["nets"]["items"]
    net = ask(script, sid, "get_net", net=nets[0]["name"])
    assert net["ok"] and net["data"]["kind"] == "net_info"
    with_route = ask(script, sid, "get_net", net=nets[0]["name"], route=True)
    assert set(with_route["data"]) == {"net_info", "net_route"}
    within(with_route)


def test_a_limit_that_cuts_is_said(flow):
    script, sid = flow
    answer = ask(script, sid, "find", pattern="*", kind="block", limit=2)
    assert answer["truncated"] is True and "find" in answer["more"]
    assert len(answer["data"]["blocks"]["items"]) == 2


def test_an_answer_that_is_too_long_is_cut_by_the_server(openfpga_module, tmp_path, golden):
    server = build_server(make_config(tmp_path, golden, call_timeout_s=120, answer_chars=1500))
    script = Script(server)
    try:

        async def steps(call):
            sid = await open_flow(call, golden)
            return [
                await call("get_timing", session=sid, paths=5),
                await call("list_options", session=sid, stage="place", limit=200),
                await call("find", session=sid, pattern="*", limit=200),
            ]

        answers = script.run(steps)
    finally:
        script.close()
    for answer in answers:
        assert answer["ok"]
        within(answer, 1500)
    # The timing with 5 paths and the options of a stage are longer than the budget
    assert answers[0]["truncated"] and answers[1]["truncated"]
    assert all(a["more"] for a in answers if a["truncated"])
    assert "cut to fit" in answers[0]["more"]


def test_the_limits_of_the_arguments_are_checked(flow):
    script, sid = flow
    for tool, arguments in [
        ("get_timing", {"paths": 0}),
        ("get_timing", {"paths": 21}),
        ("find", {"pattern": "*", "limit": 1000}),
        ("get_congestion", {"hotspots": 0}),
        ("read_log", {"stage": "place", "tail": 500}),
    ]:
        answer = ask(script, sid, tool, **arguments)
        assert answer["ok"] is False, (tool, arguments)
        assert answer["diagnostics"][0]["kind"] == "invalid_argument"


def test_a_query_before_its_stage_says_which_stage(script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        await read_inputs(call, golden, sid)
        return (
            await call("get_timing", session=sid),
            await call("get_device", session=sid),
            await call("get_placement", session=sid),
        )

    timing, device, placement = script.run(steps)
    assert timing["ok"] is False and placement["ok"] is False
    assert timing["diagnostics"][0]["kind"] in ("stage_not_reached", "stage_failed")
    assert timing["diagnostics"][0]["hint"]
    assert device["ok"] is True


# The options and the constraints --------------------------------------------------------------


def test_an_option_is_described_set_and_says_what_it_dropped(flow):
    script, sid = flow
    described = ask(script, sid, "describe_option", name="place.seed")
    assert described["data"]["stage"] == "PLACED"
    assert described["data"]["dropped_by_change"][:3] == ["PLACED", "ROUTED", "ANALYZED"]
    change = ask(script, sid, "set_option", name="place.seed", value=3)
    assert change["data"]["after"] == 3 and change["data"]["dropped"] == [
        "PLACED",
        "ROUTED",
        "ANALYZED",
    ]
    assert ask(script, sid, "get_state")["data"]["state"]["reached"] == ["DEVICE", "PACKED"]
    listed = ask(script, sid, "list_options", changed=True)
    names = [o["name"] for stage in listed["data"]["stages"] for o in stage["options"]["items"]]
    assert "place.seed" in names
    unknown = ask(script, sid, "describe_option", name="place.no_such")
    assert unknown["ok"] is False


def test_an_option_that_names_a_file_is_not_set(flow):
    script, sid = flow
    for name in ("filename.sdc_file", "place.constraints_file", "analysis.write_timing_summary"):
        answer = ask(script, sid, "set_option", name=name, value="/etc/passwd")
        assert answer["ok"] is False and answer["diagnostics"][0]["kind"] == "option_not_allowed"
    assert ask(script, sid, "get_state")["data"]["state"]["reached"] == FLOW


def test_a_constraint_is_set_and_taken_away(flow, tmp_path):
    script, sid = flow
    workspace = tmp_path / "workspace"
    (workspace / sid / "ten.sdc").write_text(
        "create_clock -period 10 clk\nset_input_delay 0 -clock clk [get_ports {*}]\n"
        "set_output_delay 0 -clock clk [get_ports {*}]\n"
    )
    change = ask(script, sid, "set_constraint", kind="sdc", file="ten.sdc")
    assert change["ok"] and change["data"]["dropped"] == FLOW
    assert change["data"]["after"] == f"{sid}/ten.sdc"
    cleared = ask(script, sid, "set_constraint", kind="sdc", clear=True)
    assert cleared["data"]["before"] == f"{sid}/ten.sdc" and cleared["data"]["after"] == ""
    width = ask(script, sid, "set_constraint", kind="chan_width", width=24)
    assert (width["data"]["before"], width["data"]["after"]) == (20, 24)
    # The routing runs at a fixed width: it is changed, not taken away
    back = ask(script, sid, "set_constraint", kind="chan_width", clear=True)
    assert back["diagnostics"][0]["kind"] == "chan_width_required"
    unset = ask(script, sid, "set_option", name="router.route_chan_width", value=-1)
    assert unset["diagnostics"][0]["kind"] == "chan_width_required"
    for arguments in (
        {"kind": "sdc"},
        {"kind": "sdc", "file": "ten.sdc", "clear": True},
        {"kind": "chan_width"},
        {"kind": "chan_width", "width": 20, "file": "ten.sdc"},
    ):
        assert ask(script, sid, "set_constraint", **arguments)["ok"] is False
    missing = ask(script, sid, "set_constraint", kind="sdc", file="missing.sdc")
    assert missing["diagnostics"][0]["kind"] == "file_not_found"


def test_an_odc_is_set_checked_and_taken_away(flow):
    script, sid = flow
    bad = "odc_version 1\ncreate_clok -period 10 clk\n"
    assert ask(script, sid, "write_file", name="bad.odc", content=bad)["ok"]
    refused = ask(script, sid, "set_constraint", kind="odc", file="bad.odc")
    assert refused["ok"] is False
    first = refused["diagnostics"][0]
    assert first["kind"] == "constraints_invalid" and first["line"] == 2
    assert first["file"] == f"{sid}/created/bad.odc"  # a name in the workspace, not a path
    assert "the nearest: create_clock" in first["message"]
    assert ask(script, sid, "get_state")["data"]["state"]["reached"] == FLOW  # nothing changed

    # A name that matches nothing is a warning, with the nearest names
    good = (
        "odc_version 1\n"
        "create_clock -period 10 clk\n"
        "set_input_delay 0 -clock clk [get_ports {IN0 IN1 INX}]\n"
        "set_property LOC X1Y1 [get_cells OUT0]\n"
    )
    ask(script, sid, "write_file", name="top.odc", content=good)
    read = ask(script, sid, "set_constraint", kind="odc", file="top.odc")
    assert read["ok"], read
    data = read["data"]
    assert data["dropped"] == FLOW and data["after"] == f"{sid}/created/top.odc"
    assert data["before"] is None and data["commands"] == 3
    (warning,) = data["warnings"]
    assert warning["line"] == 3 and "'INX'" in warning["message"]
    assert "the nearest names: IN" in warning["message"]
    assert data["timing"]["outputs_without_delay"] == ["OUT0", "OUT1", "OUT2"]
    within(read)

    report = ask(script, sid, "check_constraints", limit=1)
    assert report["ok"] and report["data"]["kind"] == "constraints"
    loc = report["data"]["commands"][2]
    assert loc["name"] == "set_property" and loc["line"] == 4
    assert loc["queries"][0]["matched"] == ["OUT0"]
    within(report)
    assert ask(script, sid, "check_constraints", limit=0)["ok"] is False

    cleared = ask(script, sid, "set_constraint", kind="odc", clear=True)
    assert cleared["data"]["before"] == f"{sid}/created/top.odc"
    assert cleared["data"]["after"] is None
    assert ask(script, sid, "check_constraints")["data"]["file"] is None
    assert ask(script, sid, "set_constraint", kind="odc")["ok"] is False


# The files ---------------------------------------------------------------------------------------


def test_a_name_outside_the_workspace_is_refused(script, golden, tmp_path):
    outside = tmp_path / "secret.xml"
    outside.write_text("<x/>")
    (tmp_path / "workspace").mkdir(exist_ok=True)
    os.symlink(outside, tmp_path / "workspace" / "link.xml")

    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        attempts = [str(outside), "../secret.xml", "~/x", "a/../../secret.xml", "link.xml"]
        return [await read_inputs(call, golden, sid, vpr_arch=name) for name in attempts] + [
            await call("run_stage", session=sid, stage="pack", from_file="../secret.xml")
        ]

    for answer in script.run(steps):
        assert answer["ok"] is False
        assert answer["diagnostics"][0]["kind"] == "path_refused", answer


def test_the_results_of_a_flow_are_read_from_files(flow, tmp_path, golden):
    script, sid = flow
    state = ask(script, sid, "get_state")["data"]["state"]
    packing = state["results"]["packing"]
    assert not packing.startswith("/")  # a name inside the workspace

    async def steps(call):
        other = (await call("session_open"))["data"]["session"]
        await call("set_option", session=other, name="general.clock_modeling", value="route")
        await call("set_option", session=other, name="router.route_chan_width", value=20)
        await read_inputs(call, golden, other)
        return await call("run_stage", session=other, stage="pack", from_file=packing)

    # The packing is in the directory of the first session: a name in the workspace
    answer = script.run(steps)
    assert answer["ok"], answer
    assert answer["data"]["reached"][-1] == "PACKED"
    wrong = ask(script, sid, "run_stage", stage="analysis", from_file=packing)
    assert wrong["ok"] is False


# The failures ----------------------------------------------------------------------------------


def test_a_stage_that_fails_says_why_and_keeps_its_data(script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        await call("set_option", session=sid, name="general.clock_modeling", value="route")
        await call("set_option", session=sid, name="router.route_chan_width", value=20)
        await call("set_option", session=sid, name="router.max_router_iterations", value=1)
        await read_inputs(call, golden, sid)
        await call("run_stage", session=sid, stage="pack")
        await call("run_stage", session=sid, stage="place")
        failed = await call("run_stage", session=sid, stage="route")
        if failed["ok"]:
            pytest.skip("the design routes in one iteration")
        return (
            failed,
            await call("get_state", session=sid),
            await call("get_congestion", session=sid),
            await call("get_timing", session=sid),
            await call("read_log", session=sid, stage="route", tail=5),
            await call("run_stage", session=sid, stage="analysis"),
        )

    failed, state, congestion, timing, log, analysis = script.run(steps)
    assert failed["diagnostics"][0]["kind"]
    assert failed["failed"]["stage"] == "ROUTED" and failed["reached"] == [
        "DEVICE",
        "PACKED",
        "PLACED",
    ]
    assert state["data"]["state"]["failed"]["stage"] == "ROUTED"
    assert congestion["ok"] and congestion["data"]["routed"] is False
    assert timing["ok"] is False
    assert log["ok"] and 1 <= len(log["data"]["lines"]) <= 5
    assert analysis["ok"] is False


def test_a_missing_file_is_a_diagnostic(script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        return await read_inputs(call, golden, sid, circuit="no_such.blif")

    answer = script.run(steps)
    assert answer["ok"] is False and answer["diagnostics"][0]["kind"] == "file_not_found"
    assert "no_such.blif" in answer["diagnostics"][0]["message"]


def test_a_stage_that_takes_too_long_ends_the_worker_and_can_be_restored(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=("pack",))
        # A placement that takes far longer than the limit, which ends it
        await call("set_option", session=sid, name="place.place_inner_num", value=20000)
        script.server.registry.get(sid).session.timeout_s = 0.3
        slow = await call("run_stage", session=sid, stage="place")
        script.server.registry.get(sid).session.timeout_s = 120
        restored = await call("session_restore", session=sid)
        await call("set_option", session=sid, name="place.place_inner_num", value=1)
        return slow, restored, await call("run_stage", session=sid, stage="place")

    slow, restored, again = script.run(steps)
    assert slow["ok"] is False and slow["diagnostics"][0]["kind"] == "worker_timeout"
    assert "session_restore" in slow["diagnostics"][0]["hint"]
    assert restored["ok"] and restored["data"]["reached"] == ["DEVICE", "PACKED"]
    assert again["ok"] and again["data"]["reached"][-1] == "PLACED"


# The logs and the checkpoints ------------------------------------------------------------------


def test_the_log_of_a_stage_is_read_from_its_end(flow):
    script, sid = flow
    short = ask(script, sid, "read_log", stage="route", tail=3)
    longer = ask(script, sid, "read_log", stage="route")
    assert short["data"]["lines"] == longer["data"]["lines"][-3:]
    assert short["truncated"] is True and "tail" in short["more"]
    assert short["data"]["total_lines"] >= len(longer["data"]["lines"])
    assert any(
        "Stage ROUTED took" in line
        for line in ask(script, sid, "read_log", stage="route", tail=200)["data"]["lines"]
    )
    within(longer)
    assert str(script.server.registry.config.root) not in json.dumps(longer)


def test_a_log_of_a_stage_that_did_not_run_says_so(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=("pack",))
        return await call("read_log", session=sid, stage="place")

    answer = script.run(steps)
    assert answer["ok"] is False and answer["diagnostics"][0]["kind"] == "invalid_argument"


def test_a_named_checkpoint_is_restored(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=("pack", "place"))
        saved = await call("checkpoint", session=sid, name="after-place")
        again = await call("checkpoint", session=sid, name="after-place")
        bad = await call("checkpoint", session=sid, name="../x")
        await call("run_stage", session=sid, stage="route")
        back = await call("session_restore", session=sid, checkpoint="after-place")
        unknown = await call("session_restore", session=sid, checkpoint="nothing")
        return saved, again, bad, back, unknown, await call("run_stage", session=sid, stage="route")

    saved, again, bad, back, unknown, route = script.run(steps)
    assert saved["data"] == {"checkpoint": "after-place", "stages": FLOW[:3]}
    assert again["ok"] is False and bad["ok"] is False
    assert back["ok"] and back["data"]["reached"] == FLOW[:3]
    assert unknown["ok"] is False and "after-place" in unknown["diagnostics"][0]["message"]
    assert route["ok"]


# The budget ------------------------------------------------------------------------------------


def test_the_budget_cuts_the_longest_list_and_says_so():
    data = {
        "name": "x",
        "short": [1, 2],
        "long": [{"n": i, "text": "word " * 5} for i in range(200)],
    }
    answer = budget.fit(data, 1000, "ask for less")
    assert len(json.dumps(answer, separators=(",", ":"))) <= 1000
    assert (
        answer["truncated"] is True
        and "data.long" in answer["more"]
        and "ask for less" in answer["more"]
    )
    assert answer["data"]["short"] == [1, 2]
    assert answer["data"]["long"] == data["long"][: len(answer["data"]["long"])]
    # An answer that fits is not touched
    small = budget.fit({"a": [1]}, 1000)
    assert small == {"ok": True, "data": {"a": [1]}, "truncated": False, "more": None}
    # A cut from the start keeps the end (a log)
    tail = budget.fit({"lines": [str(i) for i in range(500)]}, 300, from_start=True)
    assert tail["data"]["lines"][-1] == "499"
    # A query that cut says so too, and the data are not cut
    said = budget.fit({"items": [1], "truncated": True}, 1000, "more")
    assert said["truncated"] is True and said["more"] == "more" and said["data"]["items"] == [1]
    # No list to cut
    assert budget.fit({"text": "x" * 5000}, 1000)["diagnostics"][0]["kind"] == "answer_too_large"


def test_the_budget_keeps_the_items_inside_a_list_of_one():
    """The answer for a stage is a list of one stage that has a long list of options"""
    inner = [{"name": "option%d" % i, "help": "text " * 10} for i in range(100)]
    data = {"stages": [{"stage": "PLACED", "items": inner}]}
    answer = budget.fit(data, 1500)
    assert len(json.dumps(answer, separators=(",", ":"))) <= 1500
    (stage,) = answer["data"]["stages"]
    assert 1 <= len(stage["items"]) < 100 and answer["truncated"] is True
    assert stage["items"] == inner[: len(stage["items"])]
    assert "stages[0].items" in answer["more"]


# What a model gets wrong when it names the inputs ------------------------------------------------------------


def test_a_name_that_is_half_a_name_gets_the_names_it_may_mean(script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        return [
            await read_inputs(call, golden, sid, vpr_arch="tileable_40nm.xml"),
            await read_inputs(call, golden, sid, circuit="routing_tset.blif"),
            await read_inputs(call, golden, sid, vpr_arch="auto"),
        ]

    half, typo, auto = script.run(steps)
    assert half["diagnostics"][0]["kind"] == "file_not_found"
    # Many files of the folder contain it: some are named, and how many more
    assert half["diagnostics"][0]["hint"].startswith("Did you mean: ")
    assert "more that contain it" in half["diagnostics"][0]["hint"]
    assert golden.circuit.name in typo["diagnostics"][0]["hint"]
    assert auto["diagnostics"][0]["kind"] == "file_not_found"


def test_a_file_that_is_given_for_the_wrong_input_says_what_it_is_and_where_it_belongs(
    script, golden
):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        swapped = await call(
            "read_inputs",
            session=sid,
            vpr_arch=golden.openfpga_arch.name,
            layout=golden.layout,
            openfpga_arch=golden.vpr_arch.name,
            circuit=golden.circuit.name,
        )
        circuit = await read_inputs(call, golden, sid, circuit=golden.vpr_arch.name)
        blif_as_arch = await read_inputs(call, golden, sid, vpr_arch=golden.circuit.name)
        return swapped, circuit, blif_as_arch, await call("get_state", session=sid)

    swapped, circuit, blif, state = script.run(steps)
    d = swapped["diagnostics"][0]
    assert d["kind"] == "wrong_file_kind"
    assert "an architecture of OpenFPGA" in d["message"] and d["hint"].endswith("as openfpga_arch")
    assert circuit["diagnostics"][0]["hint"].endswith("as vpr_arch")
    assert blif["diagnostics"][0]["hint"].endswith("as circuit")
    # Nothing was read: the session is as it was
    assert state["data"]["state"]["reached"] == []
