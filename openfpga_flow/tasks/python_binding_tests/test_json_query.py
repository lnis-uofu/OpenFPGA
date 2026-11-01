"""The answers of the queries: JSON given to Python as the data of a command result.

The first query is 'report_app_option --json'. The envelope of an answer is
{"schema_version", "kind", "stage", "data"}, and the answer of each kind has a
schema in openfpga_flow/schemas.
"""

import json
import math
import re
from pathlib import Path

import pytest

SCHEMAS = Path(__file__).resolve().parents[3] / "openfpga_flow" / "schemas"


def strict_loads(text):
    """Parse JSON as the standard says: NaN and Infinity are not JSON"""

    def refuse(constant):
        raise ValueError(f"{constant} is not JSON")

    return json.loads(text, parse_constant=refuse)


@pytest.fixture(scope="module")
def validate():
    jsonschema = pytest.importorskip("jsonschema")

    def check(document, kind):
        schema = json.loads((SCHEMAS / f"{kind}.schema.json").read_text())
        jsonschema.Draft202012Validator.check_schema(schema)
        jsonschema.validate(document, schema, cls=jsonschema.Draft202012Validator)

    return check


def options_of(result):
    return {option["name"]: option for option in result.data["data"]["options"]}


def test_a_command_without_an_answer_has_no_data(shell):
    result = shell.report_app_option(name="place.seed")
    assert result.data is None
    assert result.raw_data == ""


def test_the_answer_is_parsed_data_and_raw_text(shell):
    result = shell.report_app_option(name="place.seed", json=True)
    assert strict_loads(result.raw_data) == result.data
    assert result.data["schema_version"] == 1
    assert result.data["kind"] == "app_options"
    # Not read from a stage
    assert result.data["stage"] is None
    # The shell prints the answer as well, for the log of a script
    assert result.raw_data in result.log


def test_one_option(shell):
    result = shell.report_app_option(name="place.seed", json=True)
    assert list(options_of(result)) == ["place.seed"]
    seed = options_of(result)["place.seed"]
    assert seed["type"] == "int"
    assert seed["value"] == seed["default"] == 1
    assert seed["is_default"] is True
    assert seed["stage"] == "PLACED"
    assert seed["choices"] == []
    assert seed["help"]


def test_all_the_options_follow_the_schema(shell, validate):
    result = shell.report_app_option(json=True)
    validate(result.data, "app_options")
    options = options_of(result)
    assert len(options) > 200
    types = {option["type"] for option in options.values()}
    assert types == {"string", "int", "float", "bool", "selection"}
    # The names are sorted, as in the text report
    names = [option["name"] for option in result.data["data"]["options"]]
    assert names == sorted(names)


def test_a_changed_option_is_not_the_default(shell, validate):
    shell.set_app_option(name="place.seed", value=7)
    shell.set_app_option(name="place.place_inner_num", value=2.5)
    result = shell.report_app_option(json=True)
    validate(result.data, "app_options")
    options = options_of(result)
    assert options["place.seed"]["value"] == 7
    assert options["place.seed"]["default"] == 1
    assert options["place.seed"]["is_default"] is False
    assert options["place.place_inner_num"]["value"] == 2.5
    assert options["place.place_inner_num"]["is_default"] is False
    # What was not changed is still the default
    assert options["place.place_checkpointing"]["is_default"] is True


def test_the_json_says_what_the_text_report_says(shell):
    """The value of every option is the one the text report prints"""
    shell.set_app_option(name="place.seed", value=11)
    options = options_of(shell.report_app_option(json=True))
    text = shell.report_app_option().log
    for name, option in options.items():
        match = re.search(rf"^{re.escape(name)} = (.*?)(?: \[.*\])?$", text, re.M)
        assert match, f"{name} is not in the text report"
        value = option["value"]
        printed = str(value).lower() if isinstance(value, bool) else str(value)
        if option["type"] == "float":
            assert math.isclose(float(match.group(1)), value, rel_tol=1e-12), name
        else:
            assert match.group(1) == printed, name


def test_selections_list_their_choices(shell):
    options = options_of(shell.report_app_option(json=True))
    selections = [o for o in options.values() if o["type"] == "selection"]
    assert selections
    for option in selections:
        assert option["choices"]
        assert option["value"] in option["choices"]
        assert option["default"] in option["choices"]
    assert all(o["choices"] == [] for o in options.values() if o["type"] != "selection")


def test_every_option_has_a_stage_of_the_flow(shell):
    stages = {
        "DEVICE",
        "PACKED",
        "PLACED",
        "ROUTED",
        "ANALYZED",
        "REPACKED",
        "BITSTREAM",
    }
    options = options_of(shell.report_app_option(json=True))
    assert {o["stage"] for o in options.values()} <= stages
    assert options["place.seed"]["stage"] == "PLACED"
    assert options["cluster.timing_gain_weight"]["stage"] == "PACKED"
    assert options["router.max_router_iterations"]["stage"] == "ROUTED"


def test_a_failed_query_gives_no_data(shell):
    result = shell.report_app_option(name="place.nothing", json=True, check=False)
    assert result.status != 0
    assert result.data is None
    assert "does not exist" in result.log


def test_the_answer_is_the_one_of_the_last_command(shell):
    assert shell.report_app_option(name="place.seed", json=True).data is not None
    # A command that gives no answer does not return the previous one
    assert shell.report_app_option(name="place.seed").data is None
    assert shell.set_app_option(name="place.seed", value=2).data is None
    # And a failed one does not either
    assert shell.report_app_option(name="place.seed", json=True).data is not None
    assert shell.report_app_option(name="nothing", json=True, check=False).data is None


def test_the_short_option_works(shell):
    result = shell.run("report_app_option -n place.seed -j")
    assert result.data["data"]["options"][0]["name"] == "place.seed"


def test_the_same_answer_is_the_same_text(shell):
    first = shell.report_app_option(json=True).raw_data
    second = shell.report_app_option(json=True).raw_data
    assert first == second


def test_the_schema_rejects_what_is_not_an_answer(validate):
    jsonschema = pytest.importorskip("jsonschema")
    good = {
        "schema_version": 1,
        "kind": "app_options",
        "stage": None,
        "data": {
            "options": [
                {
                    "name": "place.seed",
                    "type": "int",
                    "value": 1,
                    "default": 1,
                    "is_default": True,
                    "help": "h",
                    "choices": [],
                    "stage": "PLACED",
                }
            ]
        },
    }
    validate(good, "app_options")

    def bad(change):
        document = json.loads(json.dumps(good))
        change(document)
        with pytest.raises(jsonschema.ValidationError):
            validate(document, "app_options")

    bad(lambda d: d.update(schema_version=2))
    bad(lambda d: d.update(kind="other"))
    bad(lambda d: d.update(stage="PLACED"))
    bad(lambda d: d.pop("data"))
    bad(lambda d: d.update(extra=1))
    bad(lambda d: d["data"]["options"][0].update(value="1"))  # an int is not text
    bad(lambda d: d["data"]["options"][0].update(stage="NOWHERE"))
    bad(lambda d: d["data"]["options"][0].pop("help"))
    bad(lambda d: d["data"]["options"][0].update(type="selection"))  # needs choices


def test_only_the_changed_options(shell, validate):
    # Nothing is changed in a new shell
    none = shell.report_app_option(changed=True, json=True)
    validate(none.data, "app_options")
    assert none.data["data"]["options"] == []

    shell.set_app_option(name="place.seed", value=9)
    shell.set_app_option(name="router.max_router_iterations", value=30)
    answer = shell.report_app_option(changed=True, json=True)
    validate(answer.data, "app_options")
    options = options_of(answer)
    assert set(options) == {"place.seed", "router.max_router_iterations"}
    assert all(not option["is_default"] for option in options.values())

    # The text report lists the same ones
    text = shell.report_app_option(changed=True).log
    assert "place.seed = 9" in text and "router.max_router_iterations = 30" in text
    assert "place.place_algorithm" not in text

    # With a name, an option that has its default value is not reported
    assert options_of(shell.report_app_option(name="place.seed", changed=True, json=True))
    unchanged = shell.report_app_option(name="place.inner_num_nothing_here", check=False)
    assert unchanged.status != 0
    none = shell.report_app_option(name="place.place_checkpointing", changed=True, json=True)
    assert none.data["data"]["options"] == []

    # Setting an option back to its default takes it out
    shell.set_app_option(name="place.seed", value=1)
    assert set(options_of(shell.report_app_option(changed=True, json=True))) == {
        "router.max_router_iterations"
    }
