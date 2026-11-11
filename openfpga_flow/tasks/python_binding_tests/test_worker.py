"""RemoteSession: a Session in a process of its own. What happens to the worker (abort, kill,
timeout, cancel) does not reach the caller, and workers do not share their data."""

import os
import signal
import subprocess
import sys
import threading
import time
from pathlib import Path

import pytest

from test_equivalence import Golden

GOLDEN = "k6_routing_test"
# Enough moves for a placement of about half a second, and of many seconds
LONG_PLACEMENT = 100
VERY_LONG_PLACEMENT = 5000


@pytest.fixture
def golden():
    return Golden(GOLDEN)


@pytest.fixture
def make(openfpga_module, tmp_path):
    """Workers in their own directories, which are closed at the end"""
    workers = []

    def make_worker(name="worker", **arguments):
        worker = openfpga_module.RemoteSession(cwd=tmp_path / name, **arguments)
        workers.append(worker)
        return worker

    yield make_worker
    for worker in workers:
        worker.close()


def inputs(session, golden, inner_num=None):
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    if inner_num is not None:
        session.options["place.place_inner_num"] = inner_num
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)


def alive(pid):
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    # A process that ended and was not collected is not alive for us
    try:
        return Path(f"/proc/{pid}/stat").read_text().split()[2] != "Z"
    except FileNotFoundError:
        return False


def in_thread(function):
    outcome = {}

    def target():
        try:
            outcome["value"] = function()
        except BaseException as error:  # noqa: BLE001
            outcome["error"] = error

    thread = threading.Thread(target=target)
    thread.start()
    return thread, outcome


# The calls ----------------------------------------------------------------------------


def test_the_worker_is_another_process(make):
    worker = make()
    assert worker.running and worker.pid != os.getpid()
    assert alive(worker.pid)


def test_the_results_are_the_ones_of_a_session(
    openfpga_module, make, golden, tmp_path, monkeypatch
):
    local_dir = tmp_path / "local"
    local_dir.mkdir()
    monkeypatch.chdir(local_dir)
    local = openfpga_module.Session()
    inputs(local, golden)
    local.run_flow()
    wanted = (local.timing_summary(), local.utilization(), local.placement(), local.state().reached)

    worker = make()
    inputs(worker, golden)
    steps = worker.run_flow()
    assert [type(step).__name__ for step in steps] == ["StepResult"] * 4
    assert (
        worker.timing_summary(),
        worker.utilization(),
        worker.placement(),
        worker.state().reached,
    ) == wanted
    # The files of the flow are written in the directory of the worker
    assert (tmp_path / "worker" / f"{golden.name}.route").exists()
    assert Path(worker.state().results.routing).parent == tmp_path / "worker"


def test_the_options_of_a_worker(make):
    worker = make()
    worker.options["place.seed"] = 12
    assert worker.options["place.seed"] == 12
    assert worker.options.get("place.seed").is_default is False
    assert [option.name for option in worker.options.changed()] == ["place.seed"]
    assert "place.seed" in worker.options and len(worker.options) > 100
    worker.options.reset("place.seed")
    assert worker.options["place.seed"] == 1


def test_a_failed_command_is_the_failure_of_a_session(openfpga_module, make, tmp_path):
    worker = make()
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        worker.read_circuit(tmp_path / "missing.blif")
    assert isinstance(error.value, openfpga_module.OpenfpgaError)
    assert error.value.status == 1
    assert error.value.command.startswith("read_circuit --file ")
    assert error.value.log and "missing.blif" in str(error.value)
    # The worker goes on
    assert worker.running and worker.options["place.seed"] == 1


def test_other_failures_come_back_as_themselves(openfpga_module, make):
    worker = make()
    with pytest.raises(openfpga_module.CheckpointError):
        worker.checkpoint("nowhere")
    with pytest.raises(openfpga_module.WorkerError, match="TypeError"):
        worker.call("options.get")


def test_only_the_methods_of_a_session_are_called(make):
    worker = make()
    with pytest.raises(AttributeError):
        worker.shell
    with pytest.raises(AttributeError):
        worker.nothing()
    with pytest.raises(openfpga_module_error(), match="not a method"):
        worker.call("_command_line")
    with pytest.raises(openfpga_module_error(), match="not a method"):
        worker.call("shell.run", "report_app_option")
    with pytest.raises(openfpga_module_error(), match="not a method"):
        worker.call("options._shell")


def openfpga_module_error():
    import openfpga

    return openfpga.WorkerError


# Workers do not share data ----------------------------------------------------------------


def test_two_workers_run_flows_in_turns_and_at_the_same_time(openfpga_module, make):
    """The sessions of one process share the data of VPR. Two workers do not"""
    goldens = [Golden("k6_routing_test"), Golden("k4_routing_test")]
    alone = []
    for index, golden in enumerate(goldens):
        worker = make(f"alone{index}")
        inputs(worker, golden)
        worker.run_flow()
        alone.append(worker.timing_summary())

    workers = [make(f"turns{index}") for index in range(2)]
    for worker, golden in zip(workers, goldens):
        inputs(worker, golden)
    for stage in ("pack", "place", "route", "analysis"):
        for worker in workers:
            getattr(worker, stage)()
    assert [worker.timing_summary() for worker in workers] == alone
    assert workers[0].timing_summary() != workers[1].timing_summary()

    threaded = [make(f"threads{index}") for index in range(2)]
    for worker, golden in zip(threaded, goldens):
        inputs(worker, golden)
    runs = [in_thread(worker.run_flow) for worker in threaded]
    for thread, outcome in runs:
        thread.join()
        assert "error" not in outcome, outcome.get("error")
    assert [worker.timing_summary() for worker in threaded] == alone


# What happens to the worker ---------------------------------------------------------------


def long_flow(worker, golden, inner_num=VERY_LONG_PLACEMENT):
    inputs(worker, golden, inner_num)
    worker.pack()


@pytest.mark.parametrize("signal_number", [signal.SIGKILL, signal.SIGABRT, signal.SIGSEGV])
def test_a_worker_that_is_killed_during_a_call(openfpga_module, make, golden, signal_number):
    worker = make()
    long_flow(worker, golden)
    pid = worker.pid
    thread, outcome = in_thread(worker.place)
    time.sleep(0.3)
    os.kill(pid, signal_number)
    thread.join(10)
    assert not thread.is_alive()

    error = outcome["error"]
    assert isinstance(error, openfpga_module.WorkerCrashed)
    assert error.signal_name == signal_number.name
    assert error.exitcode == -signal_number
    assert signal_number.name in str(error)
    # The caller is alive, and the worker is gone
    assert not alive(pid) and not worker.running
    with pytest.raises(openfpga_module.WorkerNotRunning, match="restart"):
        worker.state()


def test_a_worker_that_is_started_again(openfpga_module, make, golden):
    worker = make()
    long_flow(worker, golden)
    first = worker.pid
    thread, outcome = in_thread(worker.place)
    time.sleep(0.2)
    os.kill(first, signal.SIGKILL)
    thread.join(10)
    assert isinstance(outcome["error"], openfpga_module.WorkerCrashed)

    # Without a checkpoint it starts from nothing
    state = worker.restart()
    assert state.reached == [] and worker.pid not in (None, first)
    inputs(worker, golden)
    worker.run_flow()
    assert worker.state().reached[-1] == "ANALYZED"


def test_a_worker_is_restored_from_its_checkpoint(openfpga_module, make, golden, tmp_path):
    """The flow loses the stage that was running, and nothing before it"""
    worker = make(checkpoint_dir=tmp_path / "ckpt")
    inputs(worker, golden, VERY_LONG_PLACEMENT)
    worker.options["place.seed"] = 3
    worker.pack()
    thread, outcome = in_thread(worker.place)
    time.sleep(0.3)
    os.kill(worker.pid, signal.SIGABRT)
    thread.join(10)
    assert isinstance(outcome["error"], openfpga_module.WorkerCrashed)

    state = worker.restart()
    assert state.reached == ["DEVICE", "PACKED"]
    assert worker.options["place.seed"] == 3
    # The flow goes on from the packing, with a placement that is quick
    worker.options["place.place_inner_num"] = 0.5
    worker.place()
    worker.route()
    assert worker.timing_summary().setup.fmax_mhz > 0

    # A worker is also made from a checkpoint
    with openfpga_module.RemoteSession.restore(tmp_path / "ckpt", cwd=tmp_path / "again") as other:
        assert other.state().reached == ["DEVICE", "PACKED", "PLACED", "ROUTED"]
        assert other.timing_summary() == worker.timing_summary()


def test_a_timeout(openfpga_module, make, golden):
    worker = make()
    long_flow(worker, golden)
    pid = worker.pid
    started = time.monotonic()
    with pytest.raises(openfpga_module.WorkerTimeout, match="0.5"):
        worker.place(timeout_s=0.5)
    assert time.monotonic() - started < 5
    assert not alive(pid)
    with pytest.raises(openfpga_module.WorkerNotRunning, match="timeout|took more"):
        worker.state()
    worker.restart()
    assert worker.state().reached == []


def test_the_timeout_of_the_object(openfpga_module, make, golden):
    worker = make(timeout_s=0.5)
    inputs(worker, golden, VERY_LONG_PLACEMENT)
    worker.pack()
    with pytest.raises(openfpga_module.WorkerTimeout):
        worker.place()


def test_a_call_that_is_cancelled(openfpga_module, make, golden):
    worker = make()
    long_flow(worker, golden)
    pid = worker.pid
    thread, outcome = in_thread(worker.place)
    time.sleep(0.3)
    started = time.monotonic()
    worker.cancel()
    thread.join(10)
    assert not thread.is_alive()
    assert time.monotonic() - started < 5
    assert isinstance(outcome["error"], openfpga_module.WorkerCancelled)
    assert not alive(pid)
    with pytest.raises(openfpga_module.WorkerNotRunning, match="cancelled"):
        worker.state()
    worker.restart()
    assert worker.running


def test_a_cancel_with_no_call_ends_the_worker(openfpga_module, make):
    worker = make()
    worker.cancel()
    deadline = time.monotonic() + 5
    while worker.running and time.monotonic() < deadline:
        time.sleep(0.01)
    assert not worker.running
    with pytest.raises(openfpga_module.WorkerError):
        worker.state()


def test_a_session_can_be_closed(openfpga_module, tmp_path):
    with openfpga_module.RemoteSession(cwd=tmp_path) as worker:
        pid = worker.pid
        assert alive(pid)
    assert not alive(pid)
    with pytest.raises(openfpga_module.WorkerNotRunning, match="closed"):
        worker.state()
    worker.close()
    worker.restart()
    assert worker.running
    worker.close()


def test_the_worker_ends_with_the_process_that_started_it(openfpga_module, golden, tmp_path):
    """Also in the middle of a command, when it is not reading its socket"""
    code = (
        "import sys, threading, time\n"
        "import openfpga\n"
        "cwd, arch, layout, arch_openfpga, circuit, width = sys.argv[1:]\n"
        "worker = openfpga.RemoteSession(cwd=cwd)\n"
        "worker.options['general.clock_modeling'] = 'route'\n"
        "worker.options['router.route_chan_width'] = int(width)\n"
        "worker.options['place.place_inner_num'] = %d\n"
        "worker.read_vpr_arch(arch, layout=layout)\n"
        "worker.read_openfpga_arch(arch_openfpga)\n"
        "worker.read_circuit(circuit)\n"
        "worker.pack()\n"
        "threading.Thread(target=worker.place, daemon=True).start()\n"
        "time.sleep(0.5)\n"
        "print(worker.pid, flush=True)\n"
        "time.sleep(60)\n"
    ) % VERY_LONG_PLACEMENT
    module_dir = os.path.dirname(openfpga_module.__file__)
    parent = subprocess.Popen(
        [
            sys.executable,
            "-c",
            code,
            str(tmp_path),
            str(golden.vpr_arch),
            golden.layout,
            str(golden.openfpga_arch),
            str(golden.circuit),
            str(golden.chan_width),
        ],
        stdout=subprocess.PIPE,
        text=True,
        env=dict(os.environ, PYTHONPATH=module_dir),
    )
    pid = int(parent.stdout.readline())
    assert alive(pid)
    parent.kill()
    parent.wait()
    deadline = time.monotonic() + 10
    while alive(pid) and time.monotonic() < deadline:
        time.sleep(0.05)
    assert not alive(pid)


def test_the_output_of_a_worker_is_in_its_failure(openfpga_module, make):
    worker = make()
    pid = worker.pid
    thread, outcome = in_thread(lambda: worker.call("command", "report_app_option", json=True))
    thread.join(10)
    assert "error" not in outcome
    os.kill(pid, signal.SIGKILL)
    deadline = time.monotonic() + 5
    while alive(pid) and time.monotonic() < deadline:
        time.sleep(0.01)
    with pytest.raises(openfpga_module.WorkerError):
        worker.state()
