"""The application options: set_app_option and report_app_option."""

import re

import pytest


def option_line(shell, name):
    """The line 'name = value' that report_app_option prints for an option"""
    log = shell.run(f"report_app_option -n {name}").log
    lines = [line for line in log.splitlines() if line.startswith(f"{name} = ")]
    assert len(lines) == 1, f"no single line for {name} in:\n{log}"
    return lines[0]


def option_value(shell, name):
    """The value of an option, without the list of choices of a selection"""
    value = option_line(shell, name)[len(f"{name} = ") :]
    return re.sub(r" \[.*\]$", "", value)


def test_default_values(shell):
    assert option_value(shell, "place.seed") == "1"
    assert option_value(shell, "router.flat_routing") == "false"
    assert option_value(shell, "place.place_algorithm") == "criticality_timing"
    assert option_value(shell, "general.num_workers") == "1"


@pytest.mark.parametrize(
    "name, text, shown",
    [
        ("place.seed", "42", "42"),
        ("place.seed", "-2", "-2"),
        ("place.seed", "0", "0"),
        ("place.place_inner_num", "0.7", "0.7"),
        ("place.place_inner_num", "1e-3", "0.001"),
        ("place.place_inner_num", "3", "3"),
        ("router.flat_routing", "true", "true"),
        ("router.flat_routing", "TRUE", "true"),
        ("router.flat_routing", "on", "true"),
        ("router.flat_routing", "yes", "true"),
        ("router.flat_routing", "1", "true"),
        ("place.place_algorithm", "bounding_box", "bounding_box"),
        ("filename.circuit_file", "and2.blif", "and2.blif"),
        ("filename.circuit_file", '"a b.blif"', "a b.blif"),
    ],
)
def test_set_then_read_back(shell, name, text, shown):
    """The value that is set is the value that is read, for every type"""
    shell.run(f"set_app_option -n {name} -v {text}")
    assert option_value(shell, name) == shown


def test_set_boolean_off(shell):
    shell.run("set_app_option -n router.flat_routing -v true")
    for text in ("false", "off", "no", "0"):
        shell.run("set_app_option -n router.flat_routing -v true")
        shell.run(f"set_app_option -n router.flat_routing -v {text}")
        assert option_value(shell, "router.flat_routing") == "false"


def test_long_option_names(shell):
    shell.run("set_app_option --name place.seed --value 7")
    assert option_value(shell, "place.seed") == "7"


def test_selection_lists_its_choices(shell):
    line = option_line(shell, "place.place_algorithm")
    assert "[" in line and "]" in line
    for choice in ("bounding_box", "criticality_timing", "slack_timing"):
        assert choice in line


def test_set_changes_only_that_option(shell):
    before = shell.run("report_app_option").log
    shell.run("set_app_option -n place.seed -v 99")
    after = shell.run("report_app_option").log
    changed = [
        (a, b)
        for a, b in zip(before.splitlines(), after.splitlines())
        if a != b and "=" in a and "=" in b
    ]
    assert changed == [("place.seed = 1", "place.seed = 99")]


def test_report_all(shell):
    log = shell.run("report_app_option").log
    lines = re.findall(r"^[a-z_]+\.[A-Za-z_0-9]+ = .*$", log, re.M)
    assert len(lines) > 200
    names = [line.split(" = ")[0] for line in lines]
    assert names == sorted(names)
    assert len(set(names)) == len(names)
    for group in ("filename", "general", "atom", "cluster", "place", "router"):
        assert any(name.startswith(group + ".") for name in names)


def test_options_survive_other_commands(shell):
    shell.run("set_app_option -n place.seed -v 5")
    shell.run("help")
    shell.run("report_app_option")
    assert option_value(shell, "place.seed") == "5"


def test_options_belong_to_the_shell(openfpga_module, shell):
    """Two shells in one process do not share their options"""
    other = openfpga_module.OpenfpgaShell()
    shell.run("set_app_option -n place.seed -v 5")
    assert option_value(shell, "place.seed") == "5"
    assert option_value(other, "place.seed") == "1"


# The errors: a clear message, and nothing changes
def test_unknown_option(shell, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError, match="does not exist") as exc:
        shell.run("set_app_option -n place.nope -v 1")
    assert "place.nope" in str(exc.value)
    assert exc.value.status != 0


def test_report_unknown_option(shell, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError, match="place.nope"):
        shell.run("report_app_option -n place.nope")


@pytest.mark.parametrize(
    "name, text, reason",
    [
        ("place.seed", "abc", "not a whole number"),
        ("place.seed", "1.5", "not a whole number"),
        ("place.seed", "99999999999", "out of the range"),
        ("place.place_inner_num", "fast", "not a number"),
        ("place.place_inner_num", "nan", "not a number|not a finite"),
        ("router.flat_routing", "maybe", "not a boolean"),
    ],
)
def test_invalid_value_is_refused(shell, openfpga_module, name, text, reason):
    before = option_value(shell, name)
    with pytest.raises(openfpga_module.OpenfpgaError) as exc:
        shell.run(f"set_app_option -n {name} -v {text}")
    assert name in str(exc.value)
    assert re.search(reason, str(exc.value)), str(exc.value)
    # The option keeps its value
    assert option_value(shell, name) == before


def test_value_outside_of_the_choices(shell, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError) as exc:
        shell.run("set_app_option -n place.place_algorithm -v not_an_algorithm")
    message = str(exc.value)
    assert "not_an_algorithm" in message
    # The message says what is accepted
    assert "bounding_box" in message and "criticality_timing" in message
    assert option_value(shell, "place.place_algorithm") == "criticality_timing"


def test_missing_value(shell, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError, match="Required option 'value'"):
        shell.run("set_app_option -n place.seed")
    assert option_value(shell, "place.seed") == "1"


def test_failed_set_does_not_stop_the_shell(shell, openfpga_module):
    with pytest.raises(openfpga_module.OpenfpgaError):
        shell.run("set_app_option -n place.seed -v abc")
    shell.run("set_app_option -n place.seed -v 8")
    assert option_value(shell, "place.seed") == "8"
