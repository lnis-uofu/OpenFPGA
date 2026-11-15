"""The definitions of the tools that a model reads, made from the tools themselves

The MCP server gets the same from the SDK; here the loop calls the tools in the process, so the
definition is made from the signature and the docstring. The session is optional for a model: the
current one is used when it is left out.
"""

import inspect
import typing
from typing import Any, Callable, Dict, List

_SIMPLE = {bool: "boolean", int: "integer", float: "number", str: "string"}


def json_schema_for(hint: Any) -> Dict[str, Any]:
    origin = typing.get_origin(hint)
    if origin is typing.Union:
        kinds = [k for k in typing.get_args(hint) if k is not type(None)]
        if len(kinds) == 1:
            return json_schema_for(kinds[0])
        return {"anyOf": [json_schema_for(k) for k in kinds]}
    if origin is typing.Literal:
        values = list(typing.get_args(hint))
        return {"type": _SIMPLE.get(type(values[0]), "string"), "enum": values}
    if hint in _SIMPLE:
        return {"type": _SIMPLE[hint]}
    return {}


def first_sentence(text: str) -> str:
    flat = " ".join(text.split())
    return flat.split(". ")[0].rstrip(".") + "."


def tool_definition(name: str, function: Callable, compact: bool = False) -> Dict[str, Any]:
    """One tool as a function definition of the OpenAI API"""
    signature = inspect.signature(function)
    hints = typing.get_type_hints(function)
    properties: Dict[str, Any] = {}
    required: List[str] = []
    for key, parameter in signature.parameters.items():
        properties[key] = json_schema_for(hints.get(key, Any))
        if key == "session":
            properties[key]["description"] = "The session id. The current session if left out."
        elif parameter.default is inspect.Parameter.empty:
            required.append(key)
    description = " ".join((inspect.getdoc(function) or "").split())
    return {
        "type": "function",
        "function": {
            "name": name,
            "description": first_sentence(description) if compact else description,
            "parameters": {"type": "object", "properties": properties, "required": required},
        },
    }


def tool_definitions(tools: Dict[str, Callable], compact: bool = False) -> List[Dict[str, Any]]:
    """Every tool, in a fixed order: the order is part of the start of the prompt, which the server
    keeps between steps"""
    return [tool_definition(name, tools[name], compact) for name in sorted(tools)]
