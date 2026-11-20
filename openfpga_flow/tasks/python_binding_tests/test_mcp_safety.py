"""What the MCP server does not let a client do, and the limits that keep it standing.

The tests make each attempt a model could make by mistake or by being led: a file outside the
workspace, a text that is meant for the shell, a session that fills the disk, a call that never
ends, a page that another site opened.
"""

import asyncio
import json
import os
import socket
import subprocess
import sys
import time
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from test_mcp_tools import Script, golden, make_config, open_flow, read_inputs  # noqa: E402,F401

from openfpga_mcp import Config, build_server  # noqa: E402


@pytest.fixture
def script(openfpga_module, tmp_path, golden):
    server = build_server(make_config(tmp_path, golden, call_timeout_s=120))
    script = Script(server)
    yield script
    script.close()


# No way to run a command ---------------------------------------------------------------------


def test_no_tool_takes_a_command(script):
    async def steps(call):
        return (await script.client.list_tools()).tools

    tools = script.run(steps)
    words = {"command", "cmd", "shell", "script", "line", "exec", "eval", "path", "directory"}
    for tool in tools:
        for argument in tool.input_schema.get("properties", {}):
            assert argument not in words, (tool.name, argument)
    assert not any(t.name in ("command", "shell", "run_command", "exec") for t in tools)


HOSTILE = ["x;exit", "x&&exit", "x|exit", "$(exit)", "`exit`", "--limit", "# c", "x\\", "!exit"]


def test_a_text_for_the_shell_is_only_a_name(script, golden):
    """What the model gives as a name or a pattern is data: it is looked for and not found"""

    async def steps(call):
        sid = await open_flow(call, golden)
        seen = []
        for text in HOSTILE:
            for tool, argument in (
                ("find", "pattern"),
                ("get_block", "block"),
                ("get_net", "net"),
                ("describe_option", "name"),
                ("get_placement", "prefix"),
                ("get_device", "tile_type"),
            ):
                seen.append((await call(tool, session=sid, **{argument: text}), text))
                assert script.server.registry.get(sid).session.running, (tool, text)
        return sid, seen, await call("get_state", session=sid)

    sid, seen, state = script.run(steps)
    assert state["data"]["state"]["reached"] == ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]
    for answer, text in seen:
        # An answer, never a worker that ended or a command that ran
        assert "ok" in answer, text
        if not answer["ok"]:
            assert (
                answer["diagnostics"][0]["kind"]
                in (
                    "error",
                    "option_unknown",
                    "unknown_block",
                    "unknown_net",
                    "unknown_tile_type",
                )
                or answer["diagnostics"][0]["message"]
            ), text
    assert os.listdir(script.server.registry.config.root / sid)  # and the files are the flow's


def test_a_text_that_the_shell_cannot_write_is_refused(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=())
        return [
            await call("find", session=sid, pattern=text)
            for text in ('a"b', "a\nexit", "a\rexit", "x" * 201, "   ")
        ]

    for answer in script.run(steps):
        assert answer["ok"] is False
        assert answer["diagnostics"][0]["kind"] == "invalid_argument"


def test_a_value_that_is_a_text_is_checked_too(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=())
        return await call("set_option", session=sid, name="place.place_reward_fun", value='a"b')

    assert script.run(steps)["diagnostics"][0]["kind"] == "invalid_argument"


# Files ---------------------------------------------------------------------------------------


def test_a_link_inside_an_input_folder_that_leaves_it_is_refused(openfpga_module, tmp_path, golden):
    inputs = tmp_path / "inputs"
    inputs.mkdir()
    outside = tmp_path / "outside.blif"
    outside.write_text(".model x\n.end\n")
    os.symlink(outside, inputs / "link.blif")
    (inputs / "fine.blif").write_text(".model y\n.end\n")
    folders = {golden.vpr_arch.parent, golden.openfpga_arch.parent, inputs}
    server = build_server(Config(root=tmp_path / "workspace", design_dirs=tuple(folders)))
    script = Script(server)
    try:

        async def steps(call):
            sid = (await call("session_open"))["data"]["session"]
            return [
                await read_inputs(call, golden, sid, circuit=name)
                for name in ("link.blif", "fine.blif")
            ]

        link, fine = script.run(steps)
    finally:
        script.close()
    assert link["ok"] is False and link["diagnostics"][0]["kind"] == "path_refused"
    # A file inside is read: the refusal is for the link, not for the folder
    assert fine["ok"] or fine["diagnostics"][0]["kind"] != "path_refused"


def test_a_folder_next_to_an_input_folder_is_not_one(script, golden):
    sibling = golden.circuit.parent.parent / "no_such_sibling"

    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        return [
            await read_inputs(call, golden, sid, circuit=name)
            for name in (f"../{sibling.name}/x.blif", f"{golden.circuit.parent}/../x.blif")
        ]

    for answer in script.run(steps):
        assert answer["ok"] is False and answer["diagnostics"][0]["kind"] == "path_refused"


# The disk --------------------------------------------------------------------------------------


def test_a_session_that_has_taken_its_share_writes_no_more(openfpga_module, tmp_path, golden):
    config = make_config(tmp_path, golden, call_timeout_s=120, session_max_bytes=2000)
    server = build_server(config)
    script = Script(server)
    try:

        async def steps(call):
            sid = await open_flow(call, golden, stages=("pack",))
            # The packing is a few kilobytes: the share is used
            used = server.registry.directory_bytes(server.registry.get(sid))
            return (
                used,
                await call("run_stage", session=sid, stage="place"),
                await call("checkpoint", session=sid, name="more"),
                await call("get_state", session=sid),
                await call("session_close", session=sid),
            )

        used, place, checkpoint, state, closed = script.run(steps)
    finally:
        script.close()
    assert used > 2000
    for answer in (place, checkpoint):
        assert answer["ok"] is False and answer["diagnostics"][0]["kind"] == "session_full"
        assert "session_close" in answer["diagnostics"][0]["hint"]
    # What reads still works, and the session can be closed
    assert state["ok"] and closed["ok"]


def test_the_size_of_a_directory_does_not_follow_links(openfpga_module, tmp_path, golden):
    server = build_server(make_config(tmp_path, golden))
    script = Script(server)
    try:

        async def steps(call):
            sid = (await call("session_open"))["data"]["session"]
            entry = server.registry.get(sid)
            before = server.registry.directory_bytes(entry)
            big = tmp_path / "big.bin"
            big.write_bytes(b"x" * 100000)
            os.symlink(big, tmp_path / "workspace" / sid / "link")
            return before, server.registry.directory_bytes(entry)

        before, after = script.run(steps)
    finally:
        script.close()
    assert after == before


# A call that never ends -----------------------------------------------------------------------


def test_a_call_that_runs_can_be_ended_by_closing_its_session(script, golden):
    async def steps(call):
        sid = await open_flow(call, golden, stages=("pack",))
        await call("set_option", session=sid, name="place.place_inner_num", value=200000)
        started = time.monotonic()
        running = asyncio.ensure_future(call("run_stage", session=sid, stage="place"))
        await asyncio.sleep(1.0)
        listed = await call("session_list")
        closed = await call("session_close", session=sid)
        answer = await running
        return time.monotonic() - started, listed, closed, answer, await call("session_list")

    took, listed, closed, answer, after = script.run(steps)
    # The list is not held up by the call that runs
    assert [s["id"] for s in listed["data"]["sessions"]] == ["s1"]
    assert closed["ok"] and after["data"]["sessions"] == []
    assert answer["ok"] is False
    assert answer["diagnostics"][0]["kind"] in ("worker_cancelled", "worker_crashed")
    assert took < 10  # and not the 40 s or more of that placement


# The page of another site -----------------------------------------------------------------------


def free_port():
    with socket.socket() as sock:
        sock.bind(("127.0.0.1", 0))
        return sock.getsockname()[1]


@pytest.fixture
def http_server(mcp_environment, tmp_path):
    port = free_port()
    process = subprocess.Popen(
        [sys.executable, "-m", "openfpga_mcp", "--root", str(tmp_path / "w"), "--transport"]
        + ["http", "--port", str(port)],
        env=mcp_environment,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    deadline = time.monotonic() + 30
    while time.monotonic() < deadline:
        try:
            socket.create_connection(("127.0.0.1", port), timeout=0.2).close()
            break
        except OSError:
            time.sleep(0.1)
    else:
        process.kill()
        pytest.fail("the server did not start")
    yield port
    process.terminate()
    process.wait(20)


INITIALIZE = {
    "jsonrpc": "2.0",
    "id": 1,
    "method": "initialize",
    "params": {
        "protocolVersion": "2025-03-26",
        "capabilities": {},
        "clientInfo": {"name": "test", "version": "0"},
    },
}


def post(port, host=None, origin=None):
    import http.client

    connection = http.client.HTTPConnection("127.0.0.1", port, timeout=20)
    headers = {"Content-Type": "application/json", "Accept": "application/json, text/event-stream"}
    if host:
        headers["Host"] = host
    if origin:
        headers["Origin"] = origin
    connection.request("POST", "/mcp", json.dumps(INITIALIZE), headers)
    return connection.getresponse().status


def test_a_request_with_the_name_of_another_site_is_refused(http_server):
    port = http_server
    assert post(port, host=f"127.0.0.1:{port}") == 200
    assert post(port, host=f"localhost:{port}") == 200
    # DNS rebinding: a page of another site that resolves its name to this machine
    assert post(port, host=f"evil.example:{port}") in (400, 403, 421)
    # A page that another site opened sends its own Origin
    assert post(port, host=f"127.0.0.1:{port}", origin="https://evil.example") in (400, 403)


def test_a_body_that_is_too_large_is_refused(http_server):
    import http.client

    connection = http.client.HTTPConnection("127.0.0.1", http_server, timeout=30)
    connection.request(
        "POST",
        "/mcp",
        "x" * (5 * 1024 * 1024),
        {"Content-Type": "application/json", "Accept": "application/json, text/event-stream"},
    )
    assert connection.getresponse().status in (400, 413)


def test_paths_are_hidden_when_a_folder_is_a_link(openfpga_module, tmp_path, golden):
    """The designs folder is given as a link: the paths that resolve are hidden as well"""
    links = tmp_path / "links"
    links.mkdir()
    folders = []
    for index, source in enumerate(
        {golden.vpr_arch.parent, golden.openfpga_arch.parent, golden.circuit.parent}
    ):
        os.symlink(source, links / f"in{index}")
        folders.append(links / f"in{index}")
    server = build_server(Config(root=tmp_path / "workspace", design_dirs=tuple(folders)))
    script = Script(server)
    try:

        async def steps(call):
            sid = (await call("session_open"))["data"]["session"]
            return await read_inputs(call, golden, sid)

        answer = script.run(steps)
    finally:
        script.close()
    assert answer["ok"], answer
    shown = json.dumps(answer)
    assert str(golden.circuit.parent) not in shown and str(links) not in shown
    assert answer["data"]["inputs"]["circuit"]["file"] == golden.circuit.name
