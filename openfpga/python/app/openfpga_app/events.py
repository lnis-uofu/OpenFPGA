"""The events of a conversation, as the browser reads them (server-sent events).

Each event has a type and the version of its schema, and is one JSON object. The schema is in
schemas/events.schema.json; a test checks every event that the backends make against it.
"""

import json
from typing import Any, Dict, Optional

SCHEMA_VERSION = 1
EVENT_TYPES = (
    "message_start",
    "text_delta",
    "tool_call_started",
    "tool_call_finished",
    "state_changed",
    "error",
    "done",
)


def event(type_: str, **fields: Any) -> Dict[str, Any]:
    assert type_ in EVENT_TYPES, type_
    return {"type": type_, "schema_version": SCHEMA_VERSION, **fields}


def message_start(message_id: str) -> Dict[str, Any]:
    return event("message_start", id=message_id, role="assistant")


def text_delta(message_id: str, text: str) -> Dict[str, Any]:
    return event("text_delta", id=message_id, text=text)


def tool_call_started(message_id: str, call_id: str, tool: str, arguments: Dict[str, Any]):
    return event(
        "tool_call_started", id=message_id, call_id=call_id, tool=tool, arguments=arguments
    )


def tool_call_finished(
    message_id: str, call_id: str, ok: bool, seconds: float, result: Dict[str, Any]
) -> Dict[str, Any]:
    """result is the answer of the tool, as the model reads it (bounded, with its own truncated)"""
    return event(
        "tool_call_finished",
        id=message_id,
        call_id=call_id,
        ok=ok,
        seconds=round(seconds, 2),
        truncated=bool(result.get("truncated")),
        result=result,
    )


def state_changed(reason: str) -> Dict[str, Any]:
    """The page asks for the session again: something changed it, e.g. a command of the console"""
    return event("state_changed", reason=reason)


def error(message: str, kind: str = "error", hint: Optional[str] = None) -> Dict[str, Any]:
    return event("error", kind=kind, message=message, hint=hint)


def done(message_id: str, cancelled: bool = False) -> Dict[str, Any]:
    return event("done", id=message_id, cancelled=cancelled)


def format_sse(item: Dict[str, Any]) -> str:
    """One event as the text of a stream of server-sent events"""
    return "event: %s\ndata: %s\n\n" % (item["type"], json.dumps(item, separators=(",", ":")))
