"""The flow of the web app and of its model runs the stages one by one at a fixed channel width: the
one-shot vpr command is refused, and so is a route while router.route_chan_width is not set (VPR
would search for the smallest width). A device package gives its own width, which is read only"""

from pathlib import Path

import pytest

pytest.importorskip("mcp")

from test_mcp_tools import Script, golden, make_config, read_inputs, script  # noqa: E402,F401

PACKAGES = Path(__file__).resolve().parents[2] / "device-packages"


def test_the_session_says_what_a_flow_at_a_fixed_width_does_not_run(
    openfpga_module, tmp_path, monkeypatch
):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    assert "whole flow in one command" in session.fixed_width_refusal("vpr")
    assert "channel width is not set" in session.fixed_width_refusal("route")
    assert "channel width is not set" in session.fixed_width_refusal("analysis")
    assert session.fixed_width_refusal("pack") is None
    session.options["router.route_chan_width"] = 20
    assert session.fixed_width_refusal("route") is None
    assert session.fixed_width_refusal("vpr") is not None


def test_tcl_refuses_them_when_asked_to(openfpga_module, tmp_path, monkeypatch):
    pytest.importorskip("_tkinter")
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    for script in ("route", "vpr arch.xml circuit.blif"):
        with pytest.raises(openfpga_module.TclRefused) as found:
            session.tcl(script, fixed_width=True)
        assert found.value.commands == []  # nothing ran
    # A script can catch the refusal, as any error
    caught = session.tcl("catch {route} message; set message", fixed_width=True)
    assert "channel width is not set" in caught.result


def test_the_model_cannot_route_without_a_width_nor_take_it_away(script, golden):
    async def steps(call):
        sid = (await call("session_open"))["data"]["session"]
        assert (await read_inputs(call, golden, sid))["ok"]
        route = await call("run_stage", session=sid, stage="route")
        analysis = await call("run_stage", session=sid, stage="analysis")
        state = await call("get_state", session=sid)
        cleared = await call("set_constraint", session=sid, kind="chan_width", clear=True)
        zero = await call("set_option", session=sid, name="router.route_chan_width", value=0)
        return route, analysis, state, cleared, zero

    route, analysis, state, cleared, zero = script.run(steps)
    for answer in (route, analysis, cleared, zero):
        assert answer["diagnostics"][0]["kind"] == "chan_width_required", answer
    assert "set_constraint" in route["diagnostics"][0]["hint"]
    assert state["data"]["state"]["reached"] == ["DEVICE"]  # pack and place did not run either


def test_a_device_package_sets_its_channel_width(openfpga_module, tmp_path, golden):
    from openfpga_mcp import build_server, device_packages

    config = make_config(tmp_path, golden, package_dirs=(PACKAGES,))
    package = next(p for p in device_packages.discover(config)["packages"] if p.channel_width)
    script = Script(build_server(config))
    try:

        async def steps(call):
            sid = (await call("session_open"))["data"]["session"]
            used = await call("use_device_package", session=sid, name=package.name)
            options = await call(
                "list_options", session=sid, query="router.route_chan_width", changed=True
            )
            width = package.channel_width
            changes = [
                await call("set_constraint", session=sid, kind="chan_width", width=width + 2),
                await call(
                    "set_option", session=sid, name="router.route_chan_width", value=width + 2
                ),
            ]
            same = await call("set_constraint", session=sid, kind="chan_width", width=width)
            return used, options, changes, same

        used, options, changes, same = script.run(steps)
    finally:
        script.close()
    assert used["ok"], used
    shown = str(options["data"])
    assert "router.route_chan_width" in shown and str(package.channel_width) in shown, shown
    # The width is the one of the package, read only: the same width is fine, another is not
    for changed in changes:
        assert changed["diagnostics"][0]["kind"] == "chan_width_fixed", changed
        assert package.name in changed["diagnostics"][0]["message"]
    assert same["ok"], same


def test_the_tcl_of_the_model_refuses_vpr(openfpga_module, tmp_path, golden):
    pytest.importorskip("_tkinter")
    from openfpga_mcp import build_server

    script = Script(build_server(make_config(tmp_path, golden, tcl_tool=True)))
    try:

        async def steps(call):
            sid = (await call("session_open"))["data"]["session"]
            return await call("run_tcl", session=sid, script="vpr")

        answer = script.run(steps)
    finally:
        script.close()
    assert answer["diagnostics"][0]["kind"] == "tcl_refused"
    assert "whole flow" in answer["diagnostics"][0]["message"]


def test_the_console_of_the_app_refuses_them_in_both_modes(openfpga_module, tmp_path, golden):
    pytest.importorskip("starlette")
    pytest.importorskip("_tkinter")
    from openfpga_app.config import AppConfig
    from openfpga_app.server import create_app
    from openfpga_app.session_backend import SessionBackend
    from starlette.testclient import TestClient
    from test_app_session import HOST

    backend = SessionBackend(make_config(tmp_path, golden))
    (tmp_path / "static").mkdir()
    (tmp_path / "static" / "index.html").write_text("<title>app</title>")
    try:
        app = create_app(
            AppConfig(port=8770, token="t", console=True), backend, tmp_path / "static"
        )
        with TestClient(app, base_url="http://" + HOST) as page:
            page.headers["Authorization"] = "Bearer t"
            assert page.post("/api/session/new", json={}).json()["ok"]

            def console(command, mode):
                return page.post("/api/console", json={"command": command, "mode": mode}).json()

            for mode in ("shell", "tcl"):
                vpr = console("vpr arch.xml circuit.blif", mode)
                assert not vpr["ok"] and "whole flow in one command" in vpr["output"], vpr
                route = console("route", mode)
                assert not route["ok"] and "channel width is not set" in route["output"], route
            assert console("version", "shell")["ok"]  # the rest of the shell is there
    finally:
        backend.close()


def test_the_console_refuses_a_flat_route_that_the_package_does_not_support(
    openfpga_module, tmp_path, golden
):
    pytest.importorskip("starlette")
    pytest.importorskip("_tkinter")
    from openfpga_app.config import AppConfig
    from openfpga_app.server import create_app
    from openfpga_app.session_backend import SessionBackend
    from starlette.testclient import TestClient
    from test_app_session import HOST
    from test_flat_routing import package_without_flat_routing

    folders = (PACKAGES, package_without_flat_routing(tmp_path))
    backend = SessionBackend(make_config(tmp_path, golden, package_dirs=folders))
    (tmp_path / "static").mkdir()
    (tmp_path / "static" / "index.html").write_text("<title>app</title>")
    try:
        app = create_app(
            AppConfig(port=8770, token="t", console=True), backend, tmp_path / "static"
        )
        with TestClient(app, base_url="http://" + HOST) as page:
            page.headers["Authorization"] = "Bearer t"

            def console(command, mode):
                return page.post("/api/console", json={"command": command, "mode": mode}).json()

            for package, refused in (("k6_no_flat", True), ("k6_frac_n10_2x2", False)):
                answer = page.post("/api/session/new", json={"device_package": package}).json()
                assert answer["ok"], answer
                assert console("set_app_option --name router.flat_routing --value true", "shell")[
                    "ok"
                ]
                for mode in ("shell", "tcl"):
                    route = console("route", mode)
                    said = "does not support it" in route["output"]
                    assert said == refused, (package, mode, route)
    finally:
        backend.close()
