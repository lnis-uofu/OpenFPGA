"""What the loop asks of a model, and what it gets back"""

import dataclasses
from typing import Any, AsyncIterator, Dict, List, Optional, Protocol, Union


@dataclasses.dataclass(frozen=True)
class ToolCallRequest:
    id: str
    name: str
    arguments: Dict[str, Any]
    # Set when the model's arguments were not an object of JSON: the loop tells the model
    error: Optional[str] = None


@dataclasses.dataclass
class ModelTurn:
    """What a model said in one step: text, and the tools that it wants called. The tokens are
    those that the endpoint counted (None when it does not say)"""

    text: str = ""
    tool_calls: List[ToolCallRequest] = dataclasses.field(default_factory=list)
    prompt_tokens: Optional[int] = None
    completion_tokens: Optional[int] = None
    seconds: float = 0.0


@dataclasses.dataclass(frozen=True)
class TextChunk:
    """A piece of the text of a turn, as it is written"""

    text: str


class ModelError(Exception):
    """The model cannot answer: not reachable, an error of the endpoint, an answer that is not
    one. kind is a word for the page ('model_unreachable', 'model_error', 'model_bad_answer')"""

    def __init__(self, kind: str, message: str, hint: Optional[str] = None):
        super().__init__(message)
        self.kind = kind
        self.hint = hint


class Model(Protocol):
    name: str

    def stream(
        self, messages: List[Dict[str, Any]], tools: List[Dict[str, Any]]
    ) -> AsyncIterator[Union[TextChunk, ModelTurn]]:
        """The chunks of the text as they come, and the turn at the end. messages are in the form
        of the OpenAI API (system, user, assistant with tool_calls, tool), tools are its
        definitions of functions"""
        ...

    def cancel(self) -> None:
        """End the request that runs"""
        ...
