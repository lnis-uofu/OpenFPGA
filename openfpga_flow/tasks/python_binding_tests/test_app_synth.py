"""Synthesis in the app: the files and the stage that the page shows, and the device package that a
session keeps for it. The synthesize tool itself is tested in test_mcp_synth.py"""

import asyncio

import pytest

pytest.importorskip("starlette")
pytest.importorskip("mcp")

from test_app_device_package import LOGIC, backend, client, config  # noqa: E402,F401
from test_mcp_synth import AND2, BROKEN  # noqa: E402
from test_mcp_tools import golden  # noqa: E402,F401


def call(backend, tool, /, **arguments):
    if tool not in ("session_open", "session_list", "replay", "list_device_packages"):
        arguments.setdefault("session", backend.sid)  # as the loop does for a model
    return asyncio.run(backend.invoke(tool, arguments))


def opened(backend, package=LOGIC):
    assert call(backend, "session_open")["ok"]
    if package:
        assert call(backend, "use_device_package", name=package)["ok"]
    return backend.sid


def write(backend, name, text):
    answer = call(backend, "write_file", name=name, content=text)
    assert answer["ok"], answer


def test_the_page_shows_the_netlist_and_the_script_of_a_synthesis(backend):
    opened(backend)
    write(backend, "and2.v", AND2)
    assert call(backend, "synthesize", top="and2", files="and2.v")["ok"]
    shown = asyncio.run(backend.view())["files"]["synthesis"]
    assert [(f["name"], f["kind"]) for f in shown] == [
        ("synth/and2.blif", "circuit"),
        ("synth/synth.ys", "other"),
    ]
    assert asyncio.run(backend.read_file("synth/and2.blif"))["ok"]  # the viewer opens it by this name


def test_a_session_that_the_page_made_from_a_package_synthesizes_for_that_package(backend):
    # The person created it from the device (no use_device_package of the model): the package is known
    assert asyncio.run(backend.new_session(device_package=LOGIC))["ok"]
    write(backend, "and2.v", AND2)
    data = call(backend, "synthesize", top="and2", files="and2.v")["data"]
    assert data["device_package"] == LOGIC and data["circuit_read"] is True


def test_a_session_opened_again_keeps_its_package_for_the_synthesis(openfpga_module, tmp_path, golden, backend):
    from openfpga_app.session_backend import SessionBackend
    from test_app_device_package import config as make_config

    assert asyncio.run(backend.new_session(device_package=LOGIC))["ok"]
    sid = backend.sid
    write(backend, "and2.v", AND2)
    assert call(backend, "synthesize", top="and2", files="and2.v")["ok"]
    assert call(backend, "run_stage", stage="pack")["ok"]  # a checkpoint, so the session can be opened
    backend.close()
    again = SessionBackend(make_config(tmp_path, golden))
    try:
        assert asyncio.run(again.open_session(sid))["ok"]
        data = asyncio.run(again.invoke("synthesize", {"session": sid, "top": "and2", "files": "and2.v"}))
        assert data["ok"] and data["data"]["device_package"] == LOGIC
    finally:
        again.close()


def synthesis_stage(backend):
    return asyncio.run(backend.view())["synthesis"]


def test_synthesis_is_the_first_stage_of_the_view_and_follows_what_the_session_did(backend, golden):
    assert asyncio.run(backend.view())["synthesis"]["status"] == "not_reached"  # no session
    sid = opened(backend)
    assert synthesis_stage(backend)["status"] == "not_reached"  # no circuit yet
    write(backend, "broken.v", BROKEN)
    write(backend, "and2.v", AND2)
    assert call(backend, "synthesize", top="broken", files="broken.v")["ok"] is False
    failed = synthesis_stage(backend)
    assert failed["status"] == "failed" and "syntax error" in failed["message"]
    assert call(backend, "synthesize", top="and2", files="and2.v")["ok"]
    done = synthesis_stage(backend)
    assert done == {"status": "reached", "seconds": done["seconds"], "message": None}
    assert done["seconds"] is not None and done["seconds"] >= 0
    # The flow stages are as they were: synthesis is not one of them
    assert [s["stage"] for s in asyncio.run(backend.view())["stages"]] == [
        "DEVICE",
        "PACKED",
        "PLACED",
        "ROUTED",
        "ANALYZED",
    ]
    # A netlist that is given is not synthesized: the stage is not needed
    answer = call(backend, "read_inputs", circuit=golden.circuit.name)
    assert answer["ok"], answer
    skipped = synthesis_stage(backend)
    assert skipped["status"] == "skipped" and golden.circuit.name in skipped["message"]
    assert sid  # the same session throughout


def test_the_stage_is_reached_again_after_the_session_is_opened_from_its_checkpoint(
    openfpga_module, tmp_path, golden, backend
):
    from openfpga_app.session_backend import SessionBackend
    from test_app_device_package import config as make_config

    assert asyncio.run(backend.new_session(device_package=LOGIC))["ok"]
    sid = backend.sid
    write(backend, "and2.v", AND2)
    assert call(backend, "synthesize", top="and2", files="and2.v")["ok"]
    assert call(backend, "run_stage", stage="pack")["ok"]
    backend.close()
    again = SessionBackend(make_config(tmp_path, golden))
    try:
        assert asyncio.run(again.open_session(sid))["ok"]
        stage = asyncio.run(again.view())["synthesis"]
        assert stage["status"] == "reached" and stage["seconds"] is None  # the time was not kept
    finally:
        again.close()


def test_the_demo_has_no_synthesis_either():
    from openfpga_app.backend import DemoBackend

    demo = DemoBackend(delay=0)
    view = asyncio.run(demo.view())
    assert view["synthesis"]["status"] == "not_reached"
    demo._set_stages(["DEVICE"])  # the demo reads a netlist: there is nothing to synthesize
    assert asyncio.run(demo.view())["synthesis"]["status"] == "skipped"
