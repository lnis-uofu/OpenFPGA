"""What went wrong, as data: the kind of each failure, the file and line it names, and a hint.

The parser is tested on the text that the shell and VPR write for real failures (copied from runs),
and the failures are made for real, through a Session and through a worker.
"""

import os
import signal
import time

import pytest

from test_equivalence import Golden

# Texts of failures as they are in the log of a command ------------------------------------

XML_NOT_FOUND = """Error 1: /nope/arch.xml:-1 Failed to open file
Confirm selected options when call command 'read_vpr_arch':
--file, -f: /nope/arch.xml
--layout, -l: 2x2
"""

XML_SYNTAX = """Reading VPR architecture 'bad.xml' with layout '2x2'...
Error 2: bad.xml:1 Unable to load XML file 'bad.xml', Start-end tags mismatch (line: 1 col: 26)
"""

OPENFPGA_ARCH_NOT_FOUND = """Error 3: Failed to open file
Confirm selected options when call command 'read_openfpga_arch':
--file, -f: /nope/o.xml
"""

VPR_BLOCK = """Building complex block graph took 0.07 seconds (max_rss 110.0 MiB, delta_rss +0.0 MiB)
Error 4:
Type: Unrecognized Error
File: /src/vpr/src/base/netlist.tpp
Line: 1554
Message: Inconsistent block data sizes
Circuit file: bad.blif
"""

LAYOUT_BLOCK = """Error 14:
Type: Architecture file
File: /src/vpr/src/base/setup_grid.cpp
Line: 135
Message: Failed to find grid layout named '1x1' (valid grid layouts: 'auto', '2x2')
Create Device
"""

INPUTS = """Error 6: The input VPR_ARCH has not been read: use the command 'read_vpr_arch'
Error 7: The input OPENFPGA_ARCH has not been read: use the command 'read_openfpga_arch'
Error 8: The input CIRCUIT has not been read: use the command 'read_circuit'
"""

STAGE = """Error 9: Cannot run the stage PLACED: the stage PACKED has not been reached, use the command 'pack' first
"""

QUERY_STAGE = """Error 2: Cannot run 'congestion': the stage ROUTED has not been reached, or it was dropped because an earlier stage or an option changed. Use the command 'route' first
"""

NO_DATA = """Error 4: Cannot run 'placement': there is no data of VPR. Run the flow first, e.g., with the commands that read the inputs and run the stages
"""

OPTION_UNKNOWN = "Error 10: Application option 'place.nothing' does not exist\n"
OPTION_VALUE = (
    "Error 11: Invalid value for the int application option 'place.seed': "
    "'many' is not a whole number\n"
)
COMMAND_UNKNOWN = "Try to call a command 'no_such_command' which is not defined!\n"
OPTION_MISSING = "Required option 'file' is missing!\nCommand 'read_packing' usage:\n"
UNROUTABLE = """Error 12: Routing failed
Failed to route connection from 'OUT1' to 'out:OUT1' for net 'OUT1' (#1)
Circuit is unroutable with a channel width factor of 2.
"""
FILE_MISSING_STAGE = (
    "Error 13: Cannot read the results of the stage ROUTED: the file '/nope/x.route' "
    "does not exist\n"
)


@pytest.fixture
def parse(openfpga_module):
    return openfpga_module.parse_diagnostics


def only(diagnostics):
    assert len(diagnostics) == 1, diagnostics
    return diagnostics[0]


def test_a_file_that_is_missing_with_its_path(parse):
    found = only(parse(XML_NOT_FOUND))
    assert (found.severity, found.kind) == ("error", "file_not_found")
    assert found.file == "/nope/arch.xml" and found.line is None
    assert found.message == "Failed to open file"
    assert found.hint


def test_a_file_that_is_missing_with_the_path_in_the_options(parse):
    found = only(parse(OPENFPGA_ARCH_NOT_FOUND))
    assert found.kind == "file_not_found" and found.file == "/nope/o.xml"


def test_an_xml_file_with_a_syntax_error(parse):
    found = only(parse(XML_SYNTAX))
    assert found.kind == "xml_syntax"
    assert (found.file, found.line) == ("bad.xml", 1)
    assert "Start-end tags mismatch" in found.message
    assert "bad.xml" in found.hint


def test_a_block_of_vpr(parse):
    found = only(parse(VPR_BLOCK))
    assert found.kind == "vpr_unrecognized_error"
    assert found.message == "Inconsistent block data sizes"
    # The file in the block is where VPR raised the error, not a file of the user
    assert found.file is None
    assert found.source == "/src/vpr/src/base/netlist.tpp:1554"


def test_a_layout_that_does_not_exist(parse):
    found = only(parse(LAYOUT_BLOCK))
    assert found.kind == "layout_not_found"
    assert found.data == {"layout": "1x1", "valid_layouts": ["auto", "2x2"]}
    assert "2x2" in found.hint and "auto" not in found.hint
    assert found.source.endswith("setup_grid.cpp:135")


def test_the_inputs_that_are_not_read(parse):
    found = parse(INPUTS)
    assert [d.kind for d in found] == ["input_not_read"] * 3
    assert [d.data["command"] for d in found] == [
        "read_vpr_arch",
        "read_openfpga_arch",
        "read_circuit",
    ]
    assert found[0].hint == "Run 'read_vpr_arch' first"


@pytest.mark.parametrize(
    "text, stage, command",
    [(STAGE, "PACKED", "pack"), (QUERY_STAGE, "ROUTED", "route")],
)
def test_a_stage_that_is_not_reached(parse, text, stage, command):
    found = only(parse(text))
    assert found.kind == "stage_not_reached"
    assert found.data == {"stage": stage, "command": command}
    assert found.hint == f"Run '{command}' first"


def test_no_data_of_vpr(parse):
    assert only(parse(NO_DATA)).kind == "no_vpr_data"


def test_options(parse):
    found = only(parse(OPTION_UNKNOWN))
    assert found.kind == "option_unknown" and found.data == {"option": "place.nothing"}
    found = only(parse(OPTION_VALUE))
    assert found.kind == "option_value_invalid"
    assert found.data["option"] == "place.seed" and found.data["type"] == "int"
    assert "place.seed" in found.hint


def test_lines_that_have_no_error_prefix(parse):
    found = only(parse(COMMAND_UNKNOWN))
    assert found.kind == "command_unknown" and found.data == {"command": "no_such_command"}
    found = only(parse(OPTION_MISSING))
    assert found.kind == "command_option_missing" and found.data == {"option": "file"}


def test_a_route_that_fails(parse):
    found = parse(UNROUTABLE)
    assert [d.kind for d in found] == ["route_failed", "route_unroutable"]
    assert found[1].data == {"channel_width": 2}
    assert "2" in found[1].hint and "router.route_chan_width" in found[1].hint


def test_a_file_that_is_missing_for_a_stage(parse):
    found = only(parse(FILE_MISSING_STAGE))
    assert found.kind == "file_not_found" and found.file == "/nope/x.route"


def test_the_errors_come_before_the_warnings(parse):
    log = "Warning 1: Something odd\nError 2: Application option 'a.b' does not exist\nWarning 3: Odd\n"
    found = parse(log)
    assert [(d.severity, d.kind) for d in found] == [
        ("error", "option_unknown"),
        ("warning", "warning"),
        ("warning", "warning"),
    ]
    assert found[1].message == "Something odd" and found[1].hint is None


def test_the_warnings_are_limited(openfpga_module, parse):
    log = "".join(f"Warning {n}: odd {n}\n" for n in range(100)) + "Error 200: Bad\n"
    found = parse(log)
    assert found[0].severity == "error"
    assert len(found) == 1 + openfpga_module.MAX_WARNINGS


def test_an_error_that_no_rule_knows(parse):
    found = only(parse("Error 5: Something that nobody foresaw\n"))
    assert (found.kind, found.message) == ("error", "Something that nobody foresaw")
    assert found.hint is None and found.data == {}


def test_the_text_of_the_exception_when_the_log_has_no_error(parse):
    found = only(parse("Reading something\n", "The exception says this"))
    assert (found.kind, found.message) == ("error", "The exception says this")
    # ... and it is not added again when the log has the error
    assert len(parse("Error 1: Failed to open file\n", "Failed to open file")) == 1


def test_a_log_without_a_failure(parse):
    assert parse("") == []
    assert parse(None) == []
    log = "Placement took 0.1 seconds\nThe error rate is low\nWarningless line\nErrors: none\n"
    assert parse(log) == []


def test_the_kinds_are_listed(openfpga_module, parse):
    kinds = set(openfpga_module.DIAGNOSTIC_KINDS)
    for text in (
        XML_NOT_FOUND + XML_SYNTAX + INPUTS + STAGE + QUERY_STAGE + NO_DATA + OPTION_UNKNOWN,
        OPTION_VALUE + COMMAND_UNKNOWN + OPTION_MISSING + UNROUTABLE + LAYOUT_BLOCK,
    ):
        for found in parse(text):
            assert found.kind in kinds, found
    # What has no rule has a kind that says what VPR called it
    assert parse(VPR_BLOCK)[0].kind.startswith("vpr_")


# Failures that are made for real ------------------------------------------------------


@pytest.fixture
def golden():
    return Golden("k6_routing_test")


@pytest.fixture
def ready(openfpga_module, golden, tmp_path, monkeypatch):
    monkeypatch.chdir(tmp_path)

    def make(inputs=True, chan_width=None):
        session = openfpga_module.Session()
        session.options["general.clock_modeling"] = "route"
        session.options["router.route_chan_width"] = chan_width or golden.chan_width
        if inputs:
            session.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
            session.read_openfpga_arch(golden.openfpga_arch)
            session.read_circuit(golden.circuit)
        return session

    return make


def failure(openfpga_module, call):
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        call()
    return error.value


def test_every_failure_has_a_diagnostic(openfpga_module, ready, golden, tmp_path):
    """The kind that each failure of a Session gets, made for real"""
    (tmp_path / "bad.xml").write_text("<architecture><models></architecture>")

    def unroutable():
        session = ready(chan_width=2)
        session.pack()
        session.place()
        session.route()

    def no_data_in_a_worker():
        # A worker is a process of its own, which has no data of VPR
        with openfpga_module.RemoteSession(cwd=tmp_path) as worker:
            worker.placement()

    def after_a_failed_route():
        session = ready(chan_width=2)
        session.pack()
        session.place()
        with pytest.raises(openfpga_module.OpenfpgaError):
            session.route()
        session.timing_summary()

    def small_device():
        session = ready(inputs=False)
        session.read_vpr_arch(golden.vpr_arch, layout="1x1")
        session.read_openfpga_arch(golden.openfpga_arch)
        session.read_circuit(golden.circuit)

    cases = {
        "no_vpr_data": lambda: no_data_in_a_worker(),
        "file_not_found": lambda: ready(False).read_vpr_arch("/nope/a.xml", layout="2x2"),
        "xml_syntax": lambda: ready(False).read_vpr_arch("bad.xml", layout="2x2"),
        "input_not_read": lambda: ready(False).pack(),
        "stage_not_reached": lambda: ready().place(),
        "stage_failed": after_a_failed_route,
        "option_unknown": lambda: ready().options.__setitem__("place.nothing", 1),
        "option_value_invalid": lambda: ready().options.__setitem__("place.seed", "many"),
        "command_unknown": lambda: ready().command("no_such_command"),
        "command_option_missing": lambda: ready().command("read_packing"),
        "layout_not_found": small_device,
        "route_unroutable": unroutable,
    }
    for kind, call in cases.items():
        error = failure(openfpga_module, call)
        errors = error.errors
        assert errors, f"{kind}: no diagnostic for\n{error}"
        assert kind in {d.kind for d in errors}, (kind, errors)
        assert all(
            d.kind in openfpga_module.DIAGNOSTIC_KINDS or d.kind.startswith("vpr_") for d in errors
        )


def test_the_file_and_the_line_are_the_ones_of_the_user(openfpga_module, ready, tmp_path):
    (tmp_path / "bad.xml").write_text("<architecture><models></architecture>")
    error = failure(openfpga_module, lambda: ready(False).read_vpr_arch("bad.xml", layout="2x2"))
    found = error.errors[0]
    assert (found.file, found.line) == ("bad.xml", 1)


def test_the_unroutable_width_is_in_the_data(openfpga_module, ready):
    session = ready(chan_width=2)
    session.pack()
    session.place()
    error = failure(openfpga_module, session.route)
    unroutable = [d for d in error.errors if d.kind == "route_unroutable"]
    assert unroutable and unroutable[0].data["channel_width"] == 2


def test_a_failure_of_a_worker_has_diagnostics_too(openfpga_module, golden, tmp_path):
    with openfpga_module.RemoteSession(cwd=tmp_path) as worker:
        # The failure of a command: the same diagnostics, made in the caller from the log
        worker.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
        error = failure(openfpga_module, lambda: worker.read_circuit(tmp_path / "missing.blif"))
        assert error.errors[0].kind == "file_not_found"

        # The worker that is killed
        worker.options["place.place_inner_num"] = 5000
        worker.options["general.clock_modeling"] = "route"
        worker.options["router.route_chan_width"] = golden.chan_width
        worker.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
        worker.read_openfpga_arch(golden.openfpga_arch)
        worker.read_circuit(golden.circuit)
        worker.pack()
        pid = worker.pid
        import threading

        outcome = {}

        def call():
            try:
                worker.place()
            except openfpga_module.WorkerError as exc:
                outcome["error"] = exc

        thread = threading.Thread(target=call)
        thread.start()
        time.sleep(0.3)
        os.kill(pid, signal.SIGKILL)
        thread.join(10)
        crashed = outcome["error"]
        found = crashed.diagnostics[0]
        assert (found.severity, found.kind) == ("error", "worker_crashed")
        assert found.data == {"exitcode": -9, "signal": "SIGKILL"}
        assert "restart()" in found.hint

        # And after it
        with pytest.raises(openfpga_module.WorkerNotRunning) as not_running:
            worker.state()
        assert not_running.value.diagnostics[0].kind == "worker_not_running"
        assert "restart()" in not_running.value.diagnostics[0].hint


def test_a_timeout_and_a_cancel_have_a_kind(openfpga_module, golden, tmp_path):
    with openfpga_module.RemoteSession(cwd=tmp_path) as worker:
        worker.options["place.place_inner_num"] = 5000
        worker.options["general.clock_modeling"] = "route"
        worker.options["router.route_chan_width"] = golden.chan_width
        worker.read_vpr_arch(golden.vpr_arch, layout=golden.layout)
        worker.read_openfpga_arch(golden.openfpga_arch)
        worker.read_circuit(golden.circuit)
        worker.pack()
        with pytest.raises(openfpga_module.WorkerTimeout) as timeout:
            worker.place(timeout_s=0.5)
        assert timeout.value.diagnostics[0].kind == "worker_timeout"


def test_an_error_in_the_middle_of_a_line_is_found(openfpga_module, ready):
    """stdout is buffered and stderr is not: the error of VPR can land after the text of a line
    that was not finished, and its message is on the lines after it"""
    log = (
        "placer_opts.seed: 1\nanneal_sched.inner_num: 0.500000Error 1: \nType: Other\n"
        "File: /vtr/vpr/src/base/check_setup.cpp\nLine: 210\n"
        "Message: Place channel width must be even for unidirectional.\n\n"
        "placer_opts.anneal_init_t_estimator: EQUILIBRIUM\n"
    )
    (found,) = openfpga_module.parse_diagnostics(log)
    assert found.severity == "error"
    assert "Place channel width must be even" in found.message
    # The same for the message of the exception, and in a real command
    session = ready(chan_width=20)
    session.pack()
    session.options["place.place_chan_width"] = 101
    with pytest.raises(openfpga_module.OpenfpgaError) as error:
        session.pack()
    assert "must be even" in str(error.value)
    assert any("must be even" in d.message for d in error.value.errors)
