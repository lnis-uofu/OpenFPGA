"""The example of the Python module runs from a shell, as a user would run it."""

import json
import os
import subprocess
import sys
from pathlib import Path

import pytest

SCRIPT = Path(__file__).resolve().parents[2] / "openfpga_python_scripts" / "session_quickstart.py"


def run_example(openfpga_module, tmp_path, *arguments):
    module_dir = os.path.dirname(openfpga_module.__file__)
    return subprocess.run(
        [sys.executable, str(SCRIPT), "--workdir", str(tmp_path / "work"), *arguments],
        capture_output=True,
        text=True,
        cwd=tmp_path,
        env=dict(os.environ, PYTHONPATH=module_dir),
    )


@pytest.mark.parametrize("extra", [[], ["--in-process"]])
def test_the_example_runs_end_to_end(openfpga_module, tmp_path, extra):
    result = run_example(openfpga_module, tmp_path, "--json", *extra)
    assert result.returncode == 0, result.stderr[-2000:]
    summary = json.loads(result.stdout.strip().splitlines()[-1])
    assert summary["stages"] == ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"]
    assert summary["fmax_mhz"] > 0
    assert 0 < summary["device_utilization"] <= 1
    assert summary["worst_path"]
    work = tmp_path / "work"
    assert (work / "and2_latch.route").exists()
    assert (work / "checkpoint" / "checkpoint.json").exists()


def test_the_example_says_what_it_did(openfpga_module, tmp_path):
    result = run_example(openfpga_module, tmp_path)
    assert result.returncode == 0, result.stderr[-2000:]
    assert "Stages reached: DEVICE, PACKED, PLACED, ROUTED, ANALYZED" in result.stdout
    assert "Fmax:" in result.stdout and "Worst path:" in result.stdout


def test_the_example_does_not_go_on_without_its_files(openfpga_module, tmp_path):
    result = run_example(openfpga_module, tmp_path, "--circuit", str(tmp_path / "missing.blif"))
    assert result.returncode != 0
    assert "missing.blif" in result.stderr
