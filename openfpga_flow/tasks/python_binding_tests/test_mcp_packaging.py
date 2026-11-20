"""The server as a user gets it: the command, the example of a client's configuration, the
message when the SDK is missing, and a flow through the tools that is compared with a recording.

Run from an environment that has only the installed wheel (see the job "Python: pip install" of
the workflow), these tests use the installed package and not the sources.
"""

import asyncio
import json
import os
import shutil
import subprocess
import sys
from pathlib import Path

import pytest

pytest.importorskip("mcp")

from mcp.client import Client  # noqa: E402
from mcp.client.stdio import StdioServerParameters  # noqa: E402

from test_mcp_tools import golden, make_config, open_flow  # noqa: E402,F401

from openfpga_mcp import build_server  # noqa: E402

REPO = Path(__file__).resolve().parents[3]
EXAMPLE = REPO / "openfpga" / "python" / "mcp" / "examples" / "client_config.json"
TRANSCRIPT = Path(__file__).resolve().parent / "golden" / "mcp_transcript.json"


def server_command():
    """The command of the example if the environment has it (an installed wheel), else the same
    server as a module"""
    installed = shutil.which("openfpga-mcp", path=str(Path(sys.executable).parent))
    if installed:
        return installed, []
    return sys.executable, ["-m", "openfpga_mcp"]


# The example of a client's configuration --------------------------------------------------------


def test_the_example_of_a_configuration_starts_the_server(mcp_environment, tmp_path, golden):
    example = json.loads(EXAMPLE.read_text())
    entry = example["mcpServers"]["openfpga"]
    assert entry["command"] == "openfpga-mcp"
    # The same arguments, with this machine's directories
    arguments = list(entry["args"])
    assert arguments.count("--root") == 1 and arguments.count("--designs-dir") >= 1
    workspace = tmp_path / "workspace"
    inputs = [golden.vpr_arch.parent, golden.openfpga_arch.parent, golden.circuit.parent]
    arguments[arguments.index("--root") + 1] = str(workspace)
    values = iter(inputs)
    for index, word in enumerate(arguments):
        if word == "--designs-dir":
            arguments[index + 1] = str(next(values, inputs[0]))
    command, prefix = server_command()
    program = StdioServerParameters(command=command, args=prefix + arguments, env=mcp_environment)

    async def main():
        async with Client(program) as client:
            tools = {t.name for t in (await client.list_tools()).tools}
            opened = json.loads((await client.call_tool("session_open", {})).content[0].text)
            sid = opened["data"]["session"]
            read = json.loads(
                (
                    await client.call_tool(
                        "read_inputs",
                        dict(
                            session=sid,
                            vpr_arch=golden.vpr_arch.name,
                            layout=golden.layout,
                            openfpga_arch=golden.openfpga_arch.name,
                            circuit=golden.circuit.name,
                        ),
                    )
                )
                .content[0]
                .text
            )
            return tools, opened, read

    tools, opened, read = asyncio.run(main())
    assert len(tools) == 24 and {"session_open", "run_stage", "get_timing"} <= tools
    assert opened["ok"] and (workspace / "s1").is_dir()
    assert read["ok"], read


def test_the_documented_options_exist(tmp_path):
    from openfpga_mcp.server import main

    for word in ("--root", "--designs-dir", "--transport", "--max-sessions", "--call-timeout"):
        assert word in (REPO / "openfpga/python/mcp/README.md").read_text()
    with pytest.raises(SystemExit) as stopped:
        main(["--help"])
    assert stopped.value.code == 0


# The metadata of the package ----------------------------------------------------------------------


def test_the_extra_and_the_command_are_declared():
    tomllib = pytest.importorskip("tomllib")  # Python 3.11
    project = tomllib.loads((REPO / "pyproject.toml").read_text())
    extra = project["project"]["optional-dependencies"]["mcp"]
    assert len(extra) == 1 and extra[0].startswith("mcp>=2") and "python_version" in extra[0]
    assert project["project"]["scripts"]["openfpga-mcp"] == "openfpga_mcp.cli:main"
    assert (
        "openfpga/python/mcp/openfpga_mcp" in project["tool"]["scikit-build"]["wheel"]["packages"]
    )


def test_without_the_sdk_the_command_says_what_to_install(mcp_environment, tmp_path):
    """The package is there and the SDK is not: a sentence, not a traceback"""
    code = (
        "import sys; sys.modules['mcp'] = None; "
        "from openfpga_mcp.cli import main; sys.exit(main(['--root', %r]))" % str(tmp_path)
    )
    result = subprocess.run(
        [sys.executable, "-c", code], env=mcp_environment, capture_output=True, text=True
    )
    assert result.returncode == 2
    assert "pip install 'openfpga[mcp]'" in result.stderr and "Traceback" not in result.stderr


# A flow that is compared with a recording ------------------------------------------------------------


def _normal(node):
    """What is the same on every machine: no seconds, floats to 3 digits"""
    if isinstance(node, dict):
        return {
            k: (0.0 if k in ("seconds", "elapsed_s", "idle_s") else _normal(v))
            for k, v in node.items()
        }
    if isinstance(node, list):
        return [_normal(v) for v in node]
    if isinstance(node, float):
        return float("%.3g" % node)
    return node


async def _transcript(call, golden):
    steps = []

    async def ask(tool, **arguments):
        answer = await call(tool, **arguments)
        steps.append({"tool": tool, "arguments": arguments, "answer": _normal(answer)})
        return answer

    sid = (await ask("session_open", name="transcript"))["data"]["session"]
    await ask("set_option", session=sid, name="general.clock_modeling", value="route")
    await ask("set_option", session=sid, name="router.route_chan_width", value=golden.chan_width)
    await ask(
        "read_inputs",
        session=sid,
        vpr_arch=golden.vpr_arch.name,
        layout=golden.layout,
        openfpga_arch=golden.openfpga_arch.name,
        circuit=golden.circuit.name,
    )
    await ask("get_device", session=sid)
    for stage in ("pack", "place", "route", "analysis"):
        await ask("run_stage", session=sid, stage=stage)
    await ask("get_state", session=sid)
    await ask("get_timing", session=sid, paths=1)
    await ask("get_utilization", session=sid)
    found = await ask("find", session=sid, pattern="*", kind="block", limit=3)
    await ask("get_block", session=sid, block=found["data"]["blocks"]["items"][0]["name"])
    nets = await ask("find", session=sid, pattern="*", kind="net", limit=2)
    await ask("get_net", session=sid, net=nets["data"]["nets"]["items"][0]["name"])
    await ask("get_placement", session=sid, limit=3)
    await ask("get_congestion", session=sid, hotspots=1)
    await ask("describe_option", session=sid, name="place.seed")
    await ask("set_option", session=sid, name="place.seed", value=3)
    await ask("set_constraint", session=sid, kind="chan_width", width=24)
    await ask("get_state", session=sid)
    await ask("run_stage", session=sid, stage="route")
    await ask("session_close", session=sid)
    return steps


def test_a_flow_through_the_tools_is_the_one_that_was_recorded(openfpga_module, tmp_path, golden):
    from test_mcp_tools import Script

    server = build_server(make_config(tmp_path, golden, call_timeout_s=120))
    script = Script(server)
    try:
        steps = script.run(lambda call: _transcript(call, golden))
    finally:
        script.close()
    if os.environ.get("UPDATE_GOLDEN"):
        TRANSCRIPT.write_text(json.dumps(steps, indent=1, sort_keys=True) + "\n")
    recorded = json.loads(TRANSCRIPT.read_text())
    assert [s["tool"] for s in steps] == [s["tool"] for s in recorded]
    for now, before in zip(steps, recorded):
        assert json.loads(json.dumps(now, sort_keys=True)) == before, (
            "the answer of %s changed; UPDATE_GOLDEN=1 records it again" % now["tool"]
        )
    # Every answer of the recording is within the budget
    assert all(len(json.dumps(s["answer"], separators=(",", ":"))) <= 4000 for s in recorded)
