"""The MCP server: sessions started, listed, killed and restored from a client.

A client of the SDK talks to the server in the same process; the sessions are workers, which are
processes of their own, so that a worker that is killed does not stop the server.
"""

import asyncio
import json
import os
import signal
import sys
import time
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from mcp.client import Client  # noqa: E402

from openfpga_mcp import Config, build_server  # noqa: E402


@pytest.fixture
def config(tmp_path):
    return Config(root=tmp_path / "workspace", max_sessions=2, call_timeout_s=60)


def client_run(server, steps):
    """Run `steps(call)` with a client of the server; call(tool, **arguments) gives the answer"""

    async def main():
        async with Client(server) as client:

            async def call(tool, **arguments):
                result = await client.call_tool(tool, arguments)
                assert not result.is_error, result
                return json.loads(result.content[0].text)

            return await steps(call, client)

    return asyncio.run(main())


@pytest.fixture
def served(openfpga_module, config):
    server = build_server(config)
    yield server
    server.registry.close_all()


def test_the_tools_are_listed_with_their_descriptions(served):
    async def steps(call, client):
        return {t.name: t.description for t in (await client.list_tools()).tools}

    tools = client_run(served, steps)
    # The 23 tools of DESIGN.md, plus list_device_packages, use_device_package, write_file and
    # synthesize
    assert len(tools) == 28
    assert {"session_open", "session_close", "session_list", "session_restore"} <= set(tools)
    assert all(len(text) > 40 for text in tools.values())


def test_a_session_is_opened_listed_and_closed(served, config):
    async def steps(call, client):
        opened = await call("session_open", name="first")
        listed = await call("session_list")
        closed = await call("session_close", session=opened["data"]["session"])
        return opened, listed, closed, await call("session_list")

    opened, listed, closed, after = client_run(served, steps)
    assert opened["ok"] and opened["truncated"] is False and opened["more"] is None
    sid = opened["data"]["session"]
    assert opened["data"]["name"] == "first" and opened["data"]["directory"] == sid
    assert (config.root / sid).is_dir()
    (info,) = listed["data"]["sessions"]
    assert (info["id"], info["name"], info["running"]) == (sid, "first", True)
    assert info["stages"] == [] and info["ended"] is None
    assert closed["data"] == {"session": sid, "closed": True}
    assert after["data"]["sessions"] == []
    assert (config.root / sid).is_dir()  # the files stay


def test_the_limit_of_sessions_is_an_answer(served):
    async def steps(call, client):
        first = await call("session_open")
        second = await call("session_open")
        third = await call("session_open")
        await call("session_close", session=first["data"]["session"])
        return second, third, await call("session_open")

    second, third, fourth = client_run(served, steps)
    assert second["ok"]
    assert third["ok"] is False
    assert third["diagnostics"][0]["kind"] == "too_many_sessions"
    assert "session_close" in third["diagnostics"][0]["hint"]
    assert fourth["ok"]


def test_an_unknown_session_and_a_closed_one_say_which(served):
    async def steps(call, client):
        unknown = await call("session_close", session="s99")
        opened = (await call("session_open"))["data"]["session"]
        await call("session_close", session=opened)
        return unknown, await call("session_restore", session=opened)

    unknown, closed = client_run(served, steps)
    assert unknown["ok"] is False and unknown["diagnostics"][0]["kind"] == "unknown_session"
    assert closed["diagnostics"][0]["kind"] == "session_closed"


def test_a_name_that_is_not_a_name_is_refused(served):
    async def steps(call, client):
        return await call("session_open", name="../../etc")

    refused = client_run(served, steps)
    assert refused["ok"] is False
    assert served.registry.list() == []


def test_a_worker_that_is_killed_does_not_stop_the_server(served):
    async def steps(call, client):
        first = (await call("session_open"))["data"]["session"]
        second = (await call("session_open"))["data"]["session"]
        pid = served.registry.get(first).session.pid
        os.kill(pid, signal.SIGKILL)
        for _ in range(100):  # the kill takes a moment
            if not served.registry.get(first).session.running:
                break
            await asyncio.sleep(0.05)
        listed = await call("session_list")
        restored = await call("session_restore", session=first)
        return first, second, listed, restored, await call("session_list")

    first, second, listed, restored, after = client_run(served, steps)
    by_id = {i["id"]: i for i in listed["data"]["sessions"]}
    assert by_id[first]["running"] is False and by_id[first]["ended"]
    assert by_id[second]["running"] is True
    assert restored["ok"] and restored["data"]["reached"] == []
    assert all(i["running"] for i in after["data"]["sessions"])


def test_a_session_that_is_idle_is_closed(openfpga_module, tmp_path):
    clock = [0.0]
    config = Config(root=tmp_path / "w", idle_timeout_s=10)
    from openfpga_mcp import SessionRegistry

    registry = SessionRegistry(config, clock=lambda: clock[0])
    server = build_server(config, registry)

    async def steps(call, client):
        sid = (await call("session_open"))["data"]["session"]
        clock[0] = 5
        assert [i["idle_s"] for i in (await call("session_list"))["data"]["sessions"]] == [5.0]
        clock[0] = 20
        assert (await call("session_list"))["data"]["sessions"] == []
        return await call("session_restore", session=sid)

    try:
        gone = client_run(server, steps)
    finally:
        registry.close_all()
    assert gone["diagnostics"][0]["kind"] == "session_closed"
    assert "idle" in gone["diagnostics"][0]["message"]


def test_the_workers_end_when_the_registry_is_closed(openfpga_module, config):
    server = build_server(config)
    pids = []

    async def steps(call, client):
        for _ in range(2):
            sid = (await call("session_open"))["data"]["session"]
            pids.append(server.registry.get(sid).session.pid)

    client_run(server, steps)
    # The sessions belong to the server and not to a connection: they are still there
    assert all(_alive(pid) for pid in pids)
    server.registry.close_all()
    deadline = time.monotonic() + 10
    while time.monotonic() < deadline and any(_alive(pid) for pid in pids):
        time.sleep(0.1)
    assert not any(_alive(pid) for pid in pids)


def _alive(pid):
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    return True


def test_the_command_line_refuses_another_address(tmp_path):
    from openfpga_mcp.server import main

    with pytest.raises(SystemExit):
        main(["--root", str(tmp_path), "--transport", "http", "--host", "0.0.0.0"])


def test_the_server_runs_as_a_program_over_stdio(mcp_environment, tmp_path):
    from mcp.client.stdio import StdioServerParameters

    program = StdioServerParameters(
        command=sys.executable,
        args=["-m", "openfpga_mcp", "--root", str(tmp_path / "workspace")],
        env=mcp_environment,
    )

    async def main():
        async with Client(program) as client:
            opened = json.loads((await client.call_tool("session_open", {})).content[0].text)
            listed = json.loads((await client.call_tool("session_list", {})).content[0].text)
            return opened, listed

    opened, listed = asyncio.run(main())
    assert opened["ok"] and [i["id"] for i in listed["data"]["sessions"]] == ["s1"]
    assert (tmp_path / "workspace" / "s1").is_dir()


def test_a_worker_does_not_end_with_the_thread_that_asked_for_it(openfpga_module, config):
    """The system ends a worker when the thread that started it ends, and the threads that run
    the tools come and go"""
    import threading

    from openfpga_mcp import SessionRegistry

    registry = SessionRegistry(config)
    made = {}

    def ask():
        made["entry"] = registry.open()

    try:
        thread = threading.Thread(target=ask)
        thread.start()
        thread.join()
        time.sleep(0.5)
        entry = made["entry"]
        assert entry.session.running and entry.session.state().reached == []
        os.kill(entry.session.pid, signal.SIGKILL)
        deadline = time.monotonic() + 5
        while entry.session.running and time.monotonic() < deadline:
            time.sleep(0.05)
        thread = threading.Thread(target=lambda: registry.restart(entry))
        thread.start()
        thread.join()
        time.sleep(0.5)
        assert entry.session.running and entry.session.state().reached == []
    finally:
        registry.close_all()
