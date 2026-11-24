"""The placer of the app (option place.placer, docs/plan_analytical_placement.md): it is set with
set_app_option (the console), the sidebar shows it next to PLACED from the options of the
session, and the stage buttons run the analytical placer once for pack and place, with a report
for each."""

import asyncio

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from test_app_session import backend, inputs_line  # noqa: E402,F401
from test_mcp_tools import golden, make_config  # noqa: E402,F401


def test_the_analytical_placer_set_in_the_console_runs_from_the_buttons(backend, golden):
    async def main():
        async def lines(line):
            return [e async for e in backend.chat(line)]

        for line in (
            "/session_open",
            "/set_option name=general.clock_modeling value=route",
            f"/set_option name=router.route_chan_width value={golden.chan_width}",
            inputs_line(golden),
        ):
            await lines(line)
        console = await backend.console("set_app_option --name place.placer --value analytical")
        assert console["ok"], console
        # The button of PLACED
        events = await lines("/run_stage stage=place")
        return events, await backend.view()

    events, shown = asyncio.run(main())
    finished = [e for e in events if e["type"] == "tool_call_finished"][0]
    assert finished["ok"], finished
    assert finished["result"]["data"]["ran"] == ["pack"]  # the analytical placer ran once
    assert {"name": "place.placer", "value": "analytical"} in shown["options_changed"]
    status = {s["stage"]: s["status"] for s in shown["stages"]}
    assert status["PACKED"] == status["PLACED"] == "reached"
    assert {"pack", "place"} <= set(shown["reports"])
