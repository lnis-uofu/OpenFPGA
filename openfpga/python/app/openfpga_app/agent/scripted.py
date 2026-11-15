"""A model that follows a plan, for tests and for CI: no endpoint, no model

The turns are what the model says, one for each step of the loop. A turn can also be a function of
the messages that the model was given, so that a test can check what it was shown.
"""

import itertools
from typing import Any, AsyncIterator, Callable, Dict, List, Optional, Union

from .model import ModelError, ModelTurn, TextChunk, ToolCallRequest

_ids = itertools.count(1)


def call(tool: str, /, **arguments: Any) -> ToolCallRequest:
    return ToolCallRequest("call_%d" % next(_ids), tool, arguments)


def say(text: str, *calls: ToolCallRequest, prompt_tokens: Optional[int] = None) -> ModelTurn:
    return ModelTurn(text=text, tool_calls=list(calls), prompt_tokens=prompt_tokens)


class ScriptedModel:
    name = "scripted"

    def __init__(self, turns: List[Union[ModelTurn, Callable[[List[Dict[str, Any]]], ModelTurn]]]):
        self.turns = list(turns)
        self.requests: List[List[Dict[str, Any]]] = []  # the messages of each step, as given
        self.tools_seen: List[List[Dict[str, Any]]] = []
        self.cancelled = False

    def cancel(self) -> None:
        self.cancelled = True

    async def stream(
        self, messages: List[Dict[str, Any]], tools: List[Dict[str, Any]]
    ) -> AsyncIterator[Union[TextChunk, ModelTurn]]:
        self.requests.append([dict(m) for m in messages])
        self.tools_seen.append(tools)
        if not self.turns:
            raise ModelError("model_error", "The scripted model has no more turns")
        turn = self.turns.pop(0)
        if callable(turn):
            turn = turn(messages)
        words = turn.text.split(" ")
        for index, word in enumerate(words if turn.text else []):
            yield TextChunk(word + (" " if index < len(words) - 1 else ""))
        yield turn
