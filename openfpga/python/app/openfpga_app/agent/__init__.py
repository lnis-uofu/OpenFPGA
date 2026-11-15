"""The agent loop: a model that chooses the tool calls of a session, one step after another.

The loop knows nothing of HTTP: it takes a model (an adapter over an OpenAI-compatible endpoint, or a
scripted one for tests), the tools, and a budget, and gives the events that the page reads. See
loop.py.
"""

from .config import AgentConfig
from .loop import AgentLoop
from .model import Model, ModelError, ModelTurn, TextChunk, ToolCallRequest
from .scripted import ScriptedModel

__all__ = [
    "AgentConfig",
    "AgentLoop",
    "Model",
    "ModelError",
    "ModelTurn",
    "ScriptedModel",
    "TextChunk",
    "ToolCallRequest",
]
