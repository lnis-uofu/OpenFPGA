"""The HTML report of each stage: written in reports/ of the session after the stage runs, with a
summary, the figures of the stage, its warnings and its errors"""

import datetime

import pytest

pytest.importorskip("mcp")

from openfpga_mcp import reports  # noqa: E402
from test_mcp_synth import AND2, BROKEN, call, opened, server, write  # noqa: E402,F401
from test_mcp_tools import golden, open_flow, read_inputs, script  # noqa: E402,F401


FAILED = 'class="status status-failed"'


def report(root, sid, stage):
    path = root / sid / "reports" / ("%s.html" % stage)
    assert path.is_file(), "no report of %s" % stage
    return path.read_text()


# The template and the log ----------------------------------------------------------------------


def test_the_warnings_and_errors_of_vpr_yosys_and_openfpga_are_found_once_each():
    log = "\n".join(
        [
            "Warning 12: Net 'n1' has no sinks.",
            "Warning 13: Net 'n1' has no sinks.",
            "Warning: Ignoring module 'x'",
            "ERROR: syntax error, unexpected ';'",
            "[Warning] Ignore the unused pin",
            "Error 1: the router found no legal routing",
            "Warnings: 3",  # a count, not a warning
            "Routing took 1.2 s",
        ]
    )
    warnings, errors = reports.messages(log)
    assert warnings == [("Net 'n1' has no sinks.", 2), ("Ignoring module 'x'", 1), ("Ignore the unused pin", 1)]
    assert [e for e, _ in errors] == ["syntax error, unexpected ';'", "the router found no legal routing"]


def test_a_repeated_warning_is_counted():
    warnings, _ = reports.messages("Warning 1: same\nWarning 2: same\nWarning 3: other\n")
    assert warnings == [("same", 2), ("other", 1)]


def test_every_value_is_escaped_and_the_template_is_filled():
    section = reports.Section("Packing").row("Clusters", 1234).table("Nets", ["Net", "Fanout"], [("<script>x</script>", 3)])
    text = reports.render(
        "pack",
        "passed",
        "s1",
        [("Circuit", "a<b>.blif")],
        [section],
        [("Clusters", 1234)],
        log="Warning 1: net <evil> & co\n",
        now=datetime.datetime(2026, 11, 21, 12, 0, 0),
    )
    assert "$" not in text.split("<style>")[0] and "<script>" not in text
    assert "&lt;script&gt;x&lt;/script&gt;" in text and "a&lt;b&gt;.blif" in text
    assert "net &lt;evil&gt; &amp; co" in text
    assert "1,234" in text and "2026-11-21 12:00:00" in text
    assert "passed with warnings" in text and "Packing report" in text


def test_a_failed_stage_says_its_reason_first():
    text = reports.render("route", "failed", "s1", [], errors=["no legal routing"], log="Error 1: other\n")
    assert FAILED in text
    assert text.index("no legal routing") < text.index("other")


# The flow ---------------------------------------------------------------------------------------


def test_each_stage_of_a_flow_writes_its_report(script, golden):
    holder = {}

    async def steps(call):
        sid = await open_flow(call, golden, stages=())
        holder["sid"] = sid
        holder["answers"] = [await call("run_stage", session=sid, stage=s) for s in ("pack", "place", "route", "analysis")]

    script.run(steps)
    sid, answers = holder["sid"], holder["answers"]
    root = script.server.registry.config.root
    assert [a["data"]["report"] for a in answers] == [
        "%s/reports/%s.html" % (sid, s) for s in ("pack", "place", "route", "analysis")
    ]
    device = report(root, sid, "device")
    assert "Device report" in device and "Tile types" in device and golden.circuit.name in device
    pack = report(root, sid, "pack")
    assert "Packing" in pack and "Clusters" in pack and "Utilization" in pack and FAILED not in pack
    assert "Half perimeter" in report(root, sid, "place")
    route = report(root, sid, "route")
    assert "Channel width" in route and str(golden.chan_width) in route
    timing = report(root, sid, "analysis")
    assert "Critical path" in timing and "Fmax" in timing


def test_the_stages_that_run_before_a_stage_write_their_reports_too(script, golden):
    holder = {}

    async def steps(call):
        sid = await open_flow(call, golden, stages=())
        holder["sid"] = sid
        holder["route"] = await call("run_stage", session=sid, stage="route")

    script.run(steps)
    root = script.server.registry.config.root
    assert holder["route"]["data"]["ran"] == ["pack", "place", "route"]
    for stage in ("pack", "place", "route"):
        report(root, holder["sid"], stage)


def test_a_stage_that_fails_writes_a_report_with_its_error(script, golden):
    holder = {}

    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        holder["sid"] = sid
        await call("set_option", session=sid, name="general.clock_modeling", value="route")
        await call("set_option", session=sid, name="router.route_chan_width", value=20)
        await call("set_option", session=sid, name="router.max_router_iterations", value=1)
        await read_inputs(call, golden, sid)
        await call("run_stage", session=sid, stage="place")
        holder["route"] = await call("run_stage", session=sid, stage="route")

    script.run(steps)
    failed = holder["route"]
    if failed["ok"]:
        pytest.skip("the design routes in one iteration")
    assert failed["report"] == "%s/reports/route.html" % holder["sid"]
    text = report(script.server.registry.config.root, holder["sid"], "route")
    assert FAILED in text and failed["diagnostics"][0]["message"][:40] in text.replace("&#x27;", "'")


def test_a_synthesis_writes_its_report_and_a_failed_one_its_error(server):
    sid = opened(server)
    write(server, "and2.v", AND2)
    write(server, "broken.v", BROKEN)
    answer = call(server, "synthesize", top="and2", files="and2.v")
    assert answer["data"]["report"] == sid + "/reports/synthesis.html"
    text = report(server.config.root, sid, "synthesis")
    assert "Synthesis report" in text and "LUTs" in text and "and2" in text
    assert "Device report" in report(server.config.root, sid, "device")  # the netlist was read
    failed = call(server, "synthesize", top="broken", files="broken.v")
    assert failed["ok"] is False and failed["report"] == sid + "/reports/synthesis.html"
    text = report(server.config.root, sid, "synthesis")
    assert FAILED in text and "syntax error" in text


def test_the_reports_do_not_count_in_the_share_of_a_session(server):
    sid = opened(server)
    write(server, "and2.v", AND2)
    assert call(server, "synthesize", top="and2", files="and2.v")["ok"]
    entry = server.script.server.registry.get(sid)
    folder = server.config.root / sid / "reports"
    before = server.script.server.registry.directory_bytes(entry)
    (folder / "big.html").write_text("x" * 100000)
    assert server.script.server.registry.directory_bytes(entry) == before
