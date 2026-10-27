"""Smoke tests: drive the OpenFPGA shell from Python through run_command."""


def test_import(openfpga_module):
    assert hasattr(openfpga_module, "OpenfpgaShell")


def test_create_shell(shell):
    assert shell is not None


def test_help_command(shell):
    assert shell.run_command("help") == 0


def test_read_openfpga_arch(shell, and2_files):
    cmd = f"read_openfpga_arch -f {and2_files['openfpga_arch']}"
    assert shell.run_command(cmd) == 0


def test_monolithic_vpr_flow(shell, and2_files, tmp_path):
    cmd = (
        f"vpr {and2_files['vpr_arch']} {and2_files['blif']} "
        "--clock_modeling route --device 2x2 --route_chan_width 20"
    )
    assert shell.run_command(cmd) == 0
    # VPR leaves its packing, placement and routing results behind
    for ext in ("net", "place", "route"):
        assert (tmp_path / f"and2.{ext}").exists(), f"missing and2.{ext}"
