"""The arguments of a call, read and checked against the signature of the tool

A line of the chat gives them as key=value, a model as a JSON object; either way they are checked
here before a tool runs, because the tools use what they get (a word of a choice names a stage).
"""

import inspect
import json
import typing
from typing import Any, Callable, Dict, Optional


def literal(text: str) -> Any:
    """A value written in a chat line: a number, true, false, null or a quoted string if it is one,
    else the text itself"""
    try:
        return json.loads(text)
    except ValueError:
        return text


def check_type(name: str, hint: Any, value: Any) -> Any:
    """The value for an argument, or a ValueError that says what the tool takes"""
    origin = typing.get_origin(hint)
    if origin is typing.Union:
        kinds = typing.get_args(hint)
        if value is None and type(None) in kinds:
            return None
        options = [kind for kind in kinds if kind is not type(None)]
        if len(options) == 1:  # an optional argument: its own message
            return check_type(name, options[0], value)
        for kind in options:
            try:
                return check_type(name, kind, value)
            except ValueError:
                continue
        raise ValueError("%s has the wrong type" % name)
    if origin is typing.Literal:
        if value not in typing.get_args(hint):
            raise ValueError(
                "%s is one of: %s" % (name, ", ".join(map(str, typing.get_args(hint))))
            )
        return value
    if hint is bool:
        if isinstance(value, bool):
            return value
    elif hint is int:
        if isinstance(value, int) and not isinstance(value, bool):
            return value
        if isinstance(value, float) and value.is_integer():
            return int(value)  # a model writes 20.0
    elif hint is float:
        if isinstance(value, (int, float)) and not isinstance(value, bool):
            return float(value)
    elif hint is str:
        if isinstance(value, str):
            return value
        if isinstance(value, (int, float)) and not isinstance(value, bool):
            return str(value)  # a name that looks like a number
    else:
        return value
    raise ValueError("%s has the wrong type" % name)


def check_arguments(
    function: Callable, given: Dict[str, Any], session: Optional[str] = None
) -> Dict[str, Any]:
    """The arguments of a call, checked. The session is the current one if the call has none"""
    signature = inspect.signature(function)
    hints = typing.get_type_hints(function)
    for key in given:
        if key not in signature.parameters:
            raise ValueError(
                "There is no argument '%s' (it takes: %s)" % (key, ", ".join(signature.parameters))
            )
    given = dict(given)
    # A model that fills in a form writes "" for an argument that it has no value for
    for key, value in list(given.items()):
        if value == "" and signature.parameters[key].default is None:
            given[key] = None
    if "session" in signature.parameters and not given.get("session") and session:
        given["session"] = session
    checked = {key: check_type(key, hints.get(key, Any), value) for key, value in given.items()}
    for key, parameter in signature.parameters.items():
        if parameter.default is inspect.Parameter.empty and key not in checked:
            raise ValueError(
                "%s needs %s=…%s"
                % (
                    function.__name__,
                    key,
                    " (open a session first: session_open)" if key == "session" else "",
                )
            )
    return checked
