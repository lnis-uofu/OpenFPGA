"""The journal and the memory limit of a worker: the commands of a session are written by its worker,
a worker that starts again goes on with them, a script of Yosys is kept as it was run, and a command
that needs more memory than the limit fails and the worker goes on."""

import os
import signal
import time
from pathlib import Path

import pytest

from test_equivalence import Golden  # noqa: F401
from test_worker import golden, in_thread, inputs, make  # noqa: F401

GOOD = "read_verilog and2.v\nhierarchy -top and2\nproc\ntechmap\nabc -lut 6\nopt_clean -purge\nwrite_blif %s\n"
AND2 = "module and2(input a, input b, output y);\n  assign y = a & b;\nendmodule\n"
NAND2 = "module and2(input a, input b, output y);\n  assign y = ~(a & b);\nendmodule\n"


def lines(path):
    return [l for l in Path(path).read_text().splitlines() if l and not l.startswith("#") and l != "exit"]


def test_a_worker_writes_the_journal_of_its_session(make, golden, tmp_path):
    worker = make(journal=tmp_path / "journal")
    inputs(worker, golden)
    worker.pack()
    commands = lines(tmp_path / "journal" / "replay.openfpga")
    assert [c.split()[0] for c in commands] == [
        "set_app_option",
        "set_app_option",
        "read_vpr_arch",
        "read_openfpga_arch",
        "read_circuit",
        "pack",
    ]
    assert (tmp_path / "journal" / "replay.py").is_file()


def test_a_worker_that_starts_again_goes_on_with_the_journal(openfpga_module, make, golden, tmp_path):
    worker = make(journal=tmp_path / "journal", checkpoint_dir=tmp_path / "ckpt")
    inputs(worker, golden, 0.5)
    worker.pack()
    before = lines(tmp_path / "journal" / "replay.openfpga")
    os.kill(worker.pid, signal.SIGKILL)
    thread, outcome = in_thread(worker.place)
    thread.join(10)
    assert isinstance(outcome["error"], openfpga_module.WorkerError)
    state = worker.restart()  # restored from the checkpoint of the packing
    assert state.reached == ["DEVICE", "PACKED"]
    # The restore is not written twice: the journal is what it was
    assert lines(tmp_path / "journal" / "replay.openfpga") == before
    worker.place()
    worker.route()
    after = lines(tmp_path / "journal" / "replay.openfpga")
    assert after[: len(before)] == before and [c.split()[0] for c in after[len(before) :]] == [
        "place",
        "route",
    ]
    # And the whole journal replays to where the worker is
    replayed = openfpga_module.Session.replay(tmp_path / "journal")
    assert replayed.state().reached == worker.state().reached


def test_the_script_of_yosys_that_was_run_is_kept_and_replayed(openfpga_module, make, tmp_path, monkeypatch):
    work = tmp_path / "w"
    work.mkdir()
    (work / "and2.v").write_text(AND2)
    script = work / "synth.ys"
    worker = make(name="w", journal=tmp_path / "journal")
    script.write_text(GOOD % (work / "first.blif"))
    worker.command("yosys", file=str(script))
    (work / "and2.v").write_text(NAND2)
    script.write_text(GOOD % (work / "second.blif"))  # the next run writes the same file again
    worker.command("yosys", file=str(script))
    commands = lines(tmp_path / "journal" / "replay.openfpga")
    assert len(commands) == 2 and all("scripts" in c and "synth.ys" not in c for c in commands)
    kept = sorted((tmp_path / "journal" / "scripts").glob("*.ys"))
    assert [k.name for k in kept] == ["001.ys", "002.ys"]
    assert "first.blif" in kept[0].read_text() and "second.blif" in kept[1].read_text()
    # A replay runs the scripts as they were: the first one writes first.blif again
    (work / "first.blif").unlink()
    (work / "second.blif").unlink()
    monkeypatch.chdir(work)
    openfpga_module.Session.replay(tmp_path / "journal")
    assert (work / "first.blif").is_file() and (work / "second.blif").is_file()


def test_a_worker_has_the_memory_limit_and_a_command_that_needs_more_fails_and_the_worker_goes_on(
    openfpga_module, make, tmp_path
):
    def limit(pid):
        for line in Path("/proc/%d/limits" % pid).read_text().splitlines():
            if line.startswith("Max address space"):
                return line.split()[3]

    free = make(name="free")
    assert limit(free.pid) == "unlimited"
    worker = make(name="limited", memory_limit_mb=4096)
    assert limit(worker.pid) == str(4096 << 20)
    folder = tmp_path / "limited"
    (folder / "big.v").write_text(
        "module big(input [31:0] a, output [7:0] y);\n  reg [7:0] m [0:2**30-1];\n  assign y = m[a];\nendmodule\n"
    )
    (folder / "hog.ys").write_text("read_verilog big.v\nhierarchy -top big\nproc\nmemory_map\nopt\n")
    with pytest.raises(openfpga_module.OpenfpgaError) as raised:
        worker.command("yosys", file=str(folder / "hog.ys"))
    assert "bad_alloc" in str(raised.value)
    pid = worker.pid
    assert worker.running and worker.pid == pid  # the same worker
    (folder / "and2.v").write_text(AND2)
    (folder / "ok.ys").write_text(GOOD % (folder / "and2.blif"))
    worker.command("yosys", file=str(folder / "ok.ys"))  # and it still works
    assert (folder / "and2.blif").is_file()


def test_a_limit_that_is_too_small_for_the_worker_to_start_is_an_error(openfpga_module, tmp_path):
    with pytest.raises(openfpga_module.WorkerError):
        openfpga_module.RemoteSession(cwd=tmp_path / "tiny", memory_limit_mb=64)


def test_a_journal_that_is_resumed_has_what_was_written_before(openfpga_module, tmp_path):
    first = openfpga_module.Session(journal=tmp_path / "j")
    first.options["place.seed"] = 3
    second = openfpga_module.Session(journal=tmp_path / "j", resume_journal=True)
    second.options["place.seed"] = 4
    assert [c.split()[-1] for c in lines(tmp_path / "j" / "replay.openfpga")] == ["3", "4"]
    third = openfpga_module.Session(journal=tmp_path / "j")  # not resumed: written again
    assert lines(tmp_path / "j" / "replay.openfpga") == []
    assert third
