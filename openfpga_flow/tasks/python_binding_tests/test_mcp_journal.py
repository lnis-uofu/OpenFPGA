"""get_journal and replay: the commands of a session, and a new session that runs them again"""

import json

import pytest

pytest.importorskip("mcp")

from test_mcp_synth import AND2, COUNTER, LOGIC, call, opened, server, write  # noqa: E402,F401
from test_mcp_tools import golden  # noqa: E402,F401


def flow(server, top="and2", source="and2.v", text=AND2):
    sid = opened(server)
    write(server, source, text)
    assert call(server, "synthesize", top=top, files=source)["ok"]
    assert call(server, "run_stage", stage="pack")["ok"]
    return sid


def test_the_journal_has_the_commands_that_changed_the_flow(server):
    sid = flow(server)
    answer = call(server, "get_journal")
    assert answer["ok"], answer
    data = answer["data"]
    assert data["format"] == "openfpga" and data["journal"] == sid + "/journal"
    commands = [l.split()[0] for l in data["lines"] if not l.startswith("#") and l != "exit"]
    # The package (its options, architectures and channel width), the synthesis, the circuit,
    # the stage
    assert commands == ["read_device_package", "yosys", "read_circuit", "pack"]
    assert data["commands"] == 4
    assert str(server.config.root) not in json.dumps(data)  # names that can be given back
    yosys = next(l for l in data["lines"] if l.startswith("yosys"))
    assert "journal/scripts/001.ys" in yosys  # the script that was run, not the live one


def test_the_journal_in_python_and_the_tail(server):
    flow(server)
    python = call(server, "get_journal", format="python")["data"]
    assert python["format"] == "python" and any("session.command('pack')" in l for l in python["lines"])
    tail = call(server, "get_journal", tail=2)["data"]
    assert len(tail["lines"]) == 2 and tail["lines"][-1] == "exit"
    assert not call(server, "get_journal", format="yaml").get("ok")


def test_a_session_with_no_command_has_a_journal_that_is_empty(server):
    assert call(server, "session_open")["ok"]
    data = call(server, "get_journal")["data"]
    assert data["commands"] == 0


def test_a_replay_makes_a_new_session_that_is_at_the_same_point(server):
    first = flow(server)
    answer = call(server, "replay", journal=first)
    assert answer["ok"], answer
    data = answer["data"]
    assert data["session"] != first and data["replayed"] == 4
    state = call(server, "get_state")["data"]["state"]
    assert state["reached"] == ["DEVICE", "PACKED"]
    assert state["inputs"]["circuit"]["file"].endswith("synth/and2.blif")
    # The first session is as it was, and the new one goes on from there
    assert call(server, "get_state", session=first)["data"]["state"]["reached"] == ["DEVICE", "PACKED"]
    assert call(server, "run_stage", stage="route")["ok"]
    assert call(server, "get_state", session=first)["data"]["state"]["reached"] == ["DEVICE", "PACKED"]
    # And its own journal has the replayed commands, then the new one
    again = call(server, "get_journal")["data"]
    # (route runs the place that was missing before it)
    assert [l.split()[0] for l in again["lines"] if not l.startswith("#") and l != "exit"][-3:] == [
        "pack",
        "place",
        "route",
    ]


def test_a_replay_runs_the_synthesis_that_was_run_and_not_the_one_that_came_after(server):
    first = flow(server)
    write(server, "counter.v", COUNTER)
    assert call(server, "synthesize", top="counter", files="counter.v")["ok"]  # the one script is written again
    answer = call(server, "replay", journal=first)
    assert answer["ok"], answer
    # The journal of the first session has both syntheses; the replay ran both, in order
    assert answer["data"]["replayed"] == 6
    state = call(server, "get_state")["data"]["state"]
    assert state["inputs"]["circuit"]["file"].endswith("synth/counter.blif")


@pytest.mark.parametrize("journal", ["", "../s1", "/etc", "s1/other", "s9", "s1/journal/scripts"])
def test_only_the_journal_of_a_session_is_replayed(server, journal):
    flow(server)
    answer = call(server, "replay", journal=journal)
    assert answer["ok"] is False
    assert answer["diagnostics"][0]["kind"] in ("invalid_argument", "path_refused")


def test_a_line_of_a_journal_that_is_not_a_command_of_the_flow_is_not_run(server):
    sid = flow(server)
    path = server.config.root / sid / "journal" / "replay.openfpga"
    path.write_text(path.read_text().replace("exit\n", "") + "shell touch /tmp/should_not_exist\nexit\n")
    item = call(server, "replay", journal=sid)["diagnostics"][0]
    assert item["kind"] == "invalid_argument" and "which a replay does not run" in item["message"]
    # No session was made for it
    assert len(call(server, "session_list")["data"]["sessions"]) == 1


def test_a_file_outside_the_workspace_in_a_journal_is_refused(server):
    sid = flow(server)
    path = server.config.root / sid / "journal" / "replay.openfpga"
    path.write_text(path.read_text().replace("exit\n", "") + "read_circuit --file /etc/passwd\nexit\n")
    item = call(server, "replay", journal=sid)["diagnostics"][0]
    assert item["kind"] == "path_refused" and "/etc/passwd" in item["message"]


def test_a_replay_that_fails_stops_and_leaves_the_session_with_what_ran(server):
    sid = flow(server)
    # The Verilog that the synthesis reads is gone: the device was read, the rest did not run
    (server.config.root / sid / "created" / "and2.v").unlink()
    answer = call(server, "replay", journal=sid)
    assert answer["ok"] is False
    item = answer["diagnostics"][0]
    assert "Replay stopped at line" in item["message"] and "yosys --file" in item["message"]
    assert item["data"]["ran"] == 1 and item["data"]["session"] in item["hint"]
    new = call(server, "get_state", session=item["data"]["session"])["data"]["state"]
    assert new["reached"] == [] and new["inputs"]["vpr_arch"] and new["inputs"]["openfpga_arch"]
