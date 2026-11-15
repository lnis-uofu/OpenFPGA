"""The tool mode for the models that call tools badly: the model answers with one JSON object, and
the server constrains the output to the tools (a grammar made from a JSON schema)

    {"tool": "run_stage", "arguments": {"stage": "pack"}}      a call
    {"tool": "answer", "text": "The flow is done: ..."}        the end

The messages are rewritten for it: a tool result is a message of the user, a call is the JSON that
the model wrote.
"""

import json
from typing import Any, Dict, List, Optional

from .model import ModelError, ModelTurn, ToolCallRequest
from .schema import first_sentence

ANSWER = "answer"


def step_schema(tools: List[Dict[str, Any]]) -> Dict[str, Any]:
    """The JSON schema of one step: a call of one of the tools, with the arguments of that tool, or
    the answer"""
    options: List[Dict[str, Any]] = []
    for item in tools:
        function = item["function"]
        options.append(
            {
                "type": "object",
                "properties": {
                    "tool": {"const": function["name"]},
                    "arguments": function["parameters"],
                },
                "required": ["tool", "arguments"],
            }
        )
    options.append(
        {
            "type": "object",
            "properties": {"tool": {"const": ANSWER}, "text": {"type": "string"}},
            "required": ["tool", "text"],
        }
    )
    return {"oneOf": options}


def addendum(tools: List[Dict[str, Any]]) -> str:
    lines = [
        "Answer with one JSON object and nothing else.",
        'To call a tool: {"tool": NAME, "arguments": {...}}.',
        'To end, and say it to the user: {"tool": "answer", "text": "..."}.',
        "The tools:",
    ]
    for item in tools:
        function = item["function"]
        names = ", ".join(function["parameters"]["properties"])
        lines.append(
            "- %s(%s): %s" % (function["name"], names, first_sentence(function["description"]))
        )
    return "\n".join(lines)


def to_messages(
    messages: List[Dict[str, Any]], tools: List[Dict[str, Any]]
) -> List[Dict[str, Any]]:
    """The messages as a model that answers in JSON reads them"""
    names: Dict[str, str] = {}
    out: List[Dict[str, Any]] = []
    for message in messages:
        role = message["role"]
        if role == "system":
            out.append({"role": "system", "content": message["content"] + "\n\n" + addendum(tools)})
        elif role == "assistant" and message.get("tool_calls"):
            call = message["tool_calls"][0]
            names[call["id"]] = call["function"]["name"]
            arguments = call["function"]["arguments"]
            arguments = json.loads(arguments) if isinstance(arguments, str) else arguments
            body = {"tool": call["function"]["name"], "arguments": arguments}
            out.append({"role": "assistant", "content": json.dumps(body)})
        elif role == "tool":
            out.append(
                {
                    "role": "user",
                    "content": "Result of %s: %s"
                    % (names.get(message["tool_call_id"], "the call"), message["content"]),
                }
            )
        else:
            out.append({"role": role, "content": message.get("content") or ""})
    return out


def parse_step(text: str, call_id: str) -> ModelTurn:
    """The turn that one JSON object says"""
    try:
        body = json.loads(text)
    except ValueError as error:
        raise ModelError("model_bad_answer", "The model did not answer with JSON: %s" % error)
    if not isinstance(body, dict) or "tool" not in body:
        raise ModelError("model_bad_answer", "The model's answer has no 'tool'")
    if body["tool"] == ANSWER:
        return ModelTurn(text=str(body.get("text", "")))
    arguments: Optional[Any] = body.get("arguments", {})
    if not isinstance(arguments, dict):
        return ModelTurn(
            tool_calls=[
                ToolCallRequest(call_id, str(body["tool"]), {}, "arguments is not an object")
            ]
        )
    return ModelTurn(tool_calls=[ToolCallRequest(call_id, str(body["tool"]), arguments)])
