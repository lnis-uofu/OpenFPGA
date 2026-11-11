"""A command that runs for long does not stop the other threads of the interpreter, and the
commands of different threads wait for each other."""

import threading
import time

import pytest

from test_equivalence import Golden

# Enough moves for a placement of about half a second
LONG_PLACEMENT = 100


@pytest.fixture
def placing(openfpga_module, shell):
    """A session with the circuit packed, which has a long placement to run"""
    golden = Golden("k6_routing_test")
    session = openfpga_module.Session(shell)
    session.options["general.clock_modeling"] = "route"
    session.options["router.route_chan_width"] = golden.chan_width
    session.options["place.place_inner_num"] = LONG_PLACEMENT
    session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
    session.read_openfpga_arch(golden.openfpga_arch)
    session.read_circuit(golden.circuit)
    session.pack()
    return session


def in_thread(function):
    """Run the function in a thread, and give the thread and what the function gave"""
    outcome = {}

    def target():
        try:
            outcome["value"] = function()
        except BaseException as error:  # noqa: BLE001
            outcome["error"] = error

    thread = threading.Thread(target=target)
    thread.start()
    return thread, outcome


def test_another_thread_runs_during_a_command(placing):
    thread, outcome = in_thread(placing.place)
    ticks = 0
    started = time.monotonic()
    while thread.is_alive():
        time.sleep(0.005)
        ticks += 1
    duration = time.monotonic() - started
    thread.join()
    assert "error" not in outcome, outcome.get("error")
    # The test is not worth anything if the command was short
    assert duration > 0.2, duration
    # A thread that waited for the command would have run at the start and at the end only
    assert ticks > duration / 0.005 / 4, (ticks, duration)


def test_the_result_is_the_same_as_in_the_main_thread(placing, openfpga_module):
    thread, outcome = in_thread(placing.place)
    thread.join()
    assert "error" not in outcome
    assert "Placement" in outcome["value"].log
    placing.route()
    assert placing.timing_summary().setup.fmax_mhz > 0


def test_a_command_that_fails_in_a_thread_raises_there(openfpga_module, shell):
    thread, outcome = in_thread(lambda: openfpga_module.Session(shell).pack())
    thread.join()
    assert isinstance(outcome.get("error"), openfpga_module.OpenfpgaError)
    # The commands that come after run
    assert openfpga_module.Session(shell).options["place.seed"] == 1


def test_the_commands_of_the_threads_wait_for_each_other(placing, openfpga_module):
    """The log of a command is its own, even if another shell is asked at the same time"""
    other = openfpga_module.OpenfpgaShell()
    thread, outcome = in_thread(placing.place)
    logs = []
    while thread.is_alive():
        logs.append(other.run("report_app_option -n place.seed").log)
    thread.join()
    assert "error" not in outcome
    assert logs
    for log in logs:
        assert "place.seed" in log
        assert "Placement" not in log and "placement" not in log.replace("place.", "")
    assert "place.seed" not in outcome["value"].log


def test_many_threads_with_a_shell_each(openfpga_module):
    results = {}

    def work(index):
        session = openfpga_module.Session()
        for turn in range(20):
            session.options["place.seed"] = index * 100 + turn
            assert session.options["place.seed"] == index * 100 + turn
        results[index] = session.options["place.seed"]

    threads = [threading.Thread(target=work, args=(index,)) for index in range(4)]
    for thread in threads:
        thread.start()
    for thread in threads:
        thread.join()
    assert results == {index: index * 100 + 19 for index in range(4)}
