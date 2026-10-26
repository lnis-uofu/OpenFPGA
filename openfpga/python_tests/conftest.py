"""Fixtures for the OpenFPGA Python binding tests.

The module is looked up in $OPENFPGA_PYTHON_PATH (a directory holding
openfpga.py and _openfpga.so), defaulting to <repo>/build/openfpga.
"""
import os
import sys
from pathlib import Path

import pytest

REPO_ROOT = Path(__file__).resolve().parents[2]
MODULE_DIR = Path(
    os.environ.get("OPENFPGA_PYTHON_PATH", REPO_ROOT / "build" / "openfpga")
)

if (MODULE_DIR / "openfpga.py").exists():
    sys.path.insert(0, str(MODULE_DIR))


@pytest.fixture(scope="session")
def openfpga_module():
    try:
        import openfpga
    except ImportError as err:
        pytest.skip(f"openfpga module not found in {MODULE_DIR}: {err}")
    return openfpga


@pytest.fixture
def shell(openfpga_module, tmp_path, monkeypatch):
    """A fresh shell, running in its own scratch directory.

    VPR and OpenFPGA write result files in the current directory.
    """
    monkeypatch.chdir(tmp_path)
    s = openfpga_module.OpenfpgaShell()
    yield s
    s.reset()


@pytest.fixture(scope="session")
def flow_dir():
    return REPO_ROOT / "openfpga_flow"


@pytest.fixture(scope="session")
def and2_files(flow_dir):
    """Public architecture and benchmark files used by the smoke tests."""
    files = {
        "vpr_arch": flow_dir / "vpr_arch" / "k6_frac_N10_tileable_40nm.xml",
        "openfpga_arch": flow_dir / "openfpga_arch" / "k6_frac_N10_40nm_openfpga.xml",
        "blif": flow_dir / "benchmarks" / "ubm" / "simple_gates" / "and2" / "and2.blif",
        # Needs a model that the architecture does not have: VPR fails right
        # after loading the architecture
        "unsupported_blif": flow_dir
        / "benchmarks"
        / "ubm"
        / "ram"
        / "pipelined_8bit_adder"
        / "pipelined_8bit_adder.blif",
        "activity": flow_dir
        / "benchmarks"
        / "ubm"
        / "simple_gates"
        / "and2"
        / "and2.act",
        "sim_setting": flow_dir
        / "openfpga_simulation_settings"
        / "auto_sim_openfpga.xml",
    }
    missing = [str(p) for p in files.values() if not p.exists()]
    if missing:
        pytest.skip(f"missing flow files: {missing}")
    return files
