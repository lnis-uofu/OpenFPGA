"""The slack of every net of the atoms in the timing view (atom_nets), for the slack layer of the
netlist views: the nets inside a cluster too, which have no net of the clusters."""

import pytest

from test_views import PACKAGE, export, rows

# A LUT that feeds a flip-flop: the net between them stays inside their cluster
LUT_TO_FF = """.model lut_ff
.inputs a b clk
.outputs q
.names a b n
11 1
.latch n q re clk 0
.end
"""


def test_every_net_of_the_atoms_has_its_slack(openfpga_module, tmp_path, monkeypatch):
    """The nets inside a cluster have no net of the clusters, but a slack: the table atom_nets, by
    the id of the nets of the atoms view. A net of the clusters has the same slack in both"""
    monkeypatch.chdir(tmp_path)
    (tmp_path / "lut_ff.blif").write_text(LUT_TO_FF)
    session = openfpga_module.Session()
    session.read_device_package(PACKAGE)
    session.read_circuit(str(tmp_path / "lut_ff.blif"))
    session.pack()
    session.place()
    session.route()
    session.analysis()
    _, view = export(openfpga_module, session, tmp_path, "timing", paths=1)
    _, atoms = export(openfpga_module, session, tmp_path, "atoms")
    _, clusters = export(openfpga_module, session, tmp_path, "clusters")
    slack = view["tables"]["atom_nets"]["slack_ns"]
    by_cluster = {
        n["name"]: view["tables"]["nets"]["slack_ns"][n["net"]] for n in rows(clusters, "nets")
    }
    nets = {n["name"]: n["id"] for n in rows(atoms, "nets")}
    assert sorted(nets.values()) == list(range(len(slack)))
    for name, id in nets.items():
        if name in by_cluster:
            assert slack[id] == pytest.approx(by_cluster[name], abs=1e-4)
    # n, from the LUT to the flip-flop, is inside the cluster, and timed
    assert "n" not in by_cluster and slack[nets["n"]] != float("inf")
