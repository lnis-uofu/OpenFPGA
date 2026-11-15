"""The size of the context, and what to do when it is full

A tool answer is the part of a conversation that grows: the old ones are replaced by one line each.
It is done in a batch, down to half of the budget, because every change of an old message makes the
server read the prompt again from there.
"""

import json
from typing import Any, Dict, List, Optional

# Tokens for a character, until the endpoint says how many a request was
DEFAULT_RATIO = 0.3
_SUMMARY_CHARS = 240


def size(messages: List[Dict[str, Any]], tools: List[Dict[str, Any]]) -> int:
    """The characters of a request"""
    return len(json.dumps(messages, ensure_ascii=False)) + len(
        json.dumps(tools, ensure_ascii=False)
    )


def summarize(tool: str, arguments: Dict[str, Any], content: str) -> str:
    """One line for a tool answer that is old: what was asked, and how it went"""
    shown = ", ".join(
        "%s=%s" % (k, v if isinstance(v, str) else json.dumps(v))
        for k, v in arguments.items()
        if k != "session"
    )
    try:
        answer = json.loads(content)
    except ValueError:
        return "[earlier %s(%s): %d characters, not kept]" % (tool, shown, len(content))
    if not answer.get("ok", False):
        diagnostic = (answer.get("diagnostics") or [{}])[0]
        what = "failed: %s %s" % (
            diagnostic.get("kind", "error"),
            str(diagnostic.get("message", ""))[:120],
        )
    else:
        data = answer.get("data")
        keys = ", ".join(list(data)[:6]) if isinstance(data, dict) else ""
        what = "ok" + (" (%s)" % keys if keys else "")
        if answer.get("truncated"):
            what += ", was cut"
    line = "[earlier %s(%s): %s; %d characters, not kept]" % (tool, shown, what, len(content))
    return line[:_SUMMARY_CHARS]


def compact(
    messages: List[Dict[str, Any]],
    tools: List[Dict[str, Any]],
    ratio: float,
    limit: int,
    keep_recent: int,
) -> int:
    """Replace the old tool answers by a line each until the request is within half the limit.
    Returns how many were replaced. Messages are changed in place"""
    target = limit * 0.5
    tool_at = [i for i, m in enumerate(messages) if m["role"] == "tool" and not m.get("summarized")]
    old = tool_at[: max(0, len(tool_at) - keep_recent)]
    calls: Dict[str, Dict[str, Any]] = {}
    for message in messages:
        for call in message.get("tool_calls") or []:
            calls[call["id"]] = call["function"]
    replaced = 0
    for index in old:
        if size(messages, tools) * ratio <= target:
            break
        message = messages[index]
        function = calls.get(message["tool_call_id"], {"name": "tool", "arguments": "{}"})
        arguments = function["arguments"]
        arguments = json.loads(arguments) if isinstance(arguments, str) else arguments
        message["content"] = summarize(function["name"], arguments, message["content"])
        message["summarized"] = True
        replaced += 1
    return replaced


def estimate(messages: List[Dict[str, Any]], tools: List[Dict[str, Any]], ratio: float) -> int:
    return int(size(messages, tools) * ratio)


def calibrate(prompt_tokens: Optional[int], characters: int, ratio: float) -> float:
    """The tokens for a character that the endpoint measured, kept between sane limits"""
    if not prompt_tokens or not characters:
        return ratio
    return min(1.0, max(0.1, prompt_tokens / characters))
