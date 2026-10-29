"""The commands of the shell called as methods: shell.read_circuit(file=...) runs
'read_circuit --file ...'."""

from pathlib import Path

import pytest


@pytest.fixture
def module(openfpga_module):
    return openfpga_module


@pytest.mark.parametrize(
    "name, args, options, expected",
    [
        ("pack", (), {}, "pack"),
        ("read_circuit", (), {"file": "a.blif"}, "read_circuit --file a.blif"),
        ("read_circuit", (), {"file": Path("dir/a.blif")}, "read_circuit --file dir/a.blif"),
        ("place_seed", (), {"seed": 3}, "place_seed --seed 3"),
        ("scale", (), {"factor": 0.5}, "scale --factor 0.5"),
        # True is a switch, False and None leave the option out
        (
            "build_fabric",
            (),
            {"compress_routing": True, "verbose": False, "frame": None},
            "build_fabric --compress_routing",
        ),
        # The options keep the order they were given in
        ("c", (), {"b": 1, "a": 2}, "c --b 1 --a 2"),
        # The positional arguments come first
        (
            "vpr",
            ("arch.xml", Path("top.blif")),
            {"pack": True, "route_chan_width": 20},
            "vpr arch.xml top.blif --pack --route_chan_width 20",
        ),
        # A value with a space is one piece for the shell
        (
            "read_circuit",
            (),
            {"file": "my dir/a.blif"},
            'read_circuit --file "my dir/a.blif"',
        ),
    ],
)
def test_command_line(module, name, args, options, expected):
    assert module._command_line(name, args, options) == expected


@pytest.mark.parametrize(
    "args, options, error",
    [
        ((), {"file": 'a"b'}, ValueError),  # the shell has no escape for a quote
        ((), {"file": ""}, ValueError),
        ((), {"file": "a\nb"}, ValueError),  # a second line would be a second command
        ((), {"file": ["a", "b"]}, TypeError),
        ((True,), {}, TypeError),
    ],
)
def test_values_that_cannot_be_written_are_refused(module, args, options, error):
    with pytest.raises(error):
        module._command_line("cmd", args, options)


def test_a_command_is_called_as_a_method(shell):
    result = shell.set_app_option(name="place.seed", value=3)
    assert result.status == 0
    assert "place.seed = 3" in shell.report_app_option(name="place.seed").log


def test_a_failing_command_raises_unless_checked(shell, module):
    with pytest.raises(module.OpenfpgaError) as err:
        shell.set_app_option(name="place.no_such_option", value=1)
    assert err.value.command == "set_app_option --name place.no_such_option --value 1"

    result = shell.set_app_option(name="place.no_such_option", value=1, check=False)
    assert result.status != 0


def test_a_command_that_does_not_exist_is_reported_by_the_shell(shell, module):
    with pytest.raises(module.OpenfpgaError) as err:
        shell.no_such_command()
    assert "no_such_command" in err.value.log


def test_names_with_an_underscore_are_not_commands(shell):
    assert not hasattr(shell, "_hidden")
    with pytest.raises(AttributeError):
        shell._hidden
    # The methods of the class are still the ones of the class
    assert shell.run("report_app_option -n place.seed").status == 0


def test_a_value_with_a_space_reaches_the_shell_as_one_value(shell, module):
    with pytest.raises(module.OpenfpgaError) as err:
        shell.report_app_option(name="place seed")
    assert "'place seed' does not exist" in err.value.log


def test_the_stage_by_stage_flow_through_methods(shell, and2_files):
    shell.set_app_option(name="general.clock_modeling", value="route")
    shell.set_app_option(name="router.route_chan_width", value=20)
    shell.read_vpr_arch(file=and2_files["vpr_arch"], layout="2x2")
    shell.read_openfpga_arch(file=and2_files["openfpga_arch"])
    shell.read_circuit(file=and2_files["blif"])
    for stage in ("pack", "place", "route", "analysis"):
        assert getattr(shell, stage)().status == 0
    for suffix in ("net", "place", "route", "net.post_routing"):
        assert Path(f"and2.{suffix}").exists()
