"""What the loop and the model are configured with"""

import dataclasses
from typing import Optional


@dataclasses.dataclass(frozen=True)
class AgentConfig:
    # The model: an OpenAI-compatible endpoint (llama.cpp's server, Ollama, vLLM)
    base_url: Optional[str] = None
    model: str = "local"
    # "native": the tool calls of the endpoint; "json": the model answers with a JSON object that
    # the server constrains to the tools (for the models that call tools badly)
    tool_mode: str = "native"
    temperature: float = 0.2
    max_tokens: int = 1024
    # The loop
    max_steps: int = 20
    max_seconds: float = 900.0
    # The context: the tokens that the messages and the tool definitions may take. Old tool answers
    # become one line when it is 80% full, down to half of it
    context_tokens: int = 8192
    keep_recent_results: int = 2
    # The same call this many times in a row, or a failure of the same kind, stops the loop
    repeat_limit: int = 3
    # A tool that needs a session when none is open: open one (shown as a call) instead of an error
    auto_session: bool = True
    # The descriptions of the tools: all of the text, or the first sentence only
    compact_descriptions: bool = False

    def __post_init__(self):
        if self.tool_mode not in ("native", "json"):
            raise ValueError("tool_mode is native or json")
        if self.max_steps < 1 or self.context_tokens < 1024:
            raise ValueError("max_steps is at least 1 and context_tokens at least 1024")
