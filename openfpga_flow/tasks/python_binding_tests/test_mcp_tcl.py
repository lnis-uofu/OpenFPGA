"""The tool run_tcl of the MCP server: off unless the server is asked for it, and then a safe Tcl
interpreter of the session, whose commands are those of the shell"""

import pytest

pytest.importorskip("mcp")
pytest.importorskip("_tkinter")

from openfpga_mcp import build_server  # noqa: E402
from test_mcp_tools import Script, golden, make_config, read_inputs, within  # noqa: E402,F401


@pytest.fixture
def tcl_script(openfpga_module, tmp_path, golden):
    server = build_server(make_config(tmp_path, golden, call_timeout_s=120, tcl_tool=True))
    script = Script(server)
    yield script
    script.close()


def tool_names(script):
    async def steps(call):
        return [t.name for t in (await script.client.list_tools()).tools]

    return script.run(steps)


def test_the_tool_is_off_unless_asked_for(openfpga_module, tmp_path, golden, tcl_script):
    server = build_server(make_config(tmp_path / "off", golden))
    off = Script(server)
    try:
        assert "run_tcl" not in tool_names(off)
    finally:
        off.close()
    assert "run_tcl" in tool_names(tcl_script)


SWEEP = """
set_app_option --name general.clock_modeling --value route
set_app_option --name router.route_chan_width --value %d
foreach stage {pack place route analysis} { $stage }
set fmax [dict get [timing_summary] setup fmax_mhz]
puts "fmax $fmax"
expr {$fmax > 0}
"""


def test_a_script_runs_the_flow_and_says_what_it_did(tcl_script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        assert (await read_inputs(call, golden, sid))["ok"]
        first = await call("run_tcl", session=sid, script=SWEEP % golden.chan_width)
        # The variables stay for the next script
        again = await call("run_tcl", session=sid, script="set fmax")
        state = await call("get_state", session=sid)
        return first, again, state

    first, again, state = tcl_script.run(steps)
    assert first["ok"], first
    data = first["data"]
    assert data["result"] == "1" and data["printed"].startswith("fmax ")
    assert data["reached"] == ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]
    assert data["commands"][2:6] == ["pack", "place", "route", "analysis"]
    assert again["data"]["result"] == data["printed"].split()[1]
    assert state["data"]["state"]["reached"][-1] == "ANALYZED"
    within(first)


def test_a_failure_has_the_line_the_output_and_the_diagnostics(tcl_script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        failed = await call("run_tcl", session=sid, script="puts start\n\nplace")
        refused = await call("run_tcl", session=sid, script="read_circuit --file /etc/hosts")
        program = await call("run_tcl", session=sid, script="ext_exec --command ls")
        empty = await call("run_tcl", session=sid, script="  ")
        return failed, refused, program, empty

    failed, refused, program, empty = tcl_script.run(steps)
    assert not failed["ok"]
    first = failed["diagnostics"][0]
    assert first["kind"] == "tcl_error" and first["data"]["where"] == 3
    assert failed["diagnostics"][1]["kind"] == "stage_not_reached"
    assert failed["commands"] == ["place"] and failed["printed"] == "start\n"
    assert failed["reached"] == []
    assert refused["diagnostics"][0]["kind"] == "tcl_refused" and refused["commands"] == []
    assert program["diagnostics"][0]["kind"] == "tcl_refused"
    assert empty["diagnostics"][0]["kind"] == "invalid_argument"
