"""A flow that runs the stages one by one at a fixed channel width (one that a program or a model
drives) does not run the one-shot vpr command, nor a route while router.route_chan_width is not set
(VPR would search for the smallest width): Session.fixed_width_refusal() says why, and
Session.tcl(fixed_width=True) refuses them"""

import pytest


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


def test_a_device_without_flat_routing_refuses_a_flat_route(openfpga_module, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)
    session = openfpga_module.Session()
    session.options["router.route_chan_width"] = 20
    session.options["router.flat_routing"] = True
    assert session.fixed_width_refusal("route") is None
    assert "does not support it" in session.fixed_width_refusal("route", flat_routing=False)
    session.options["router.flat_routing"] = False
    assert session.fixed_width_refusal("route", flat_routing=False) is None


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
