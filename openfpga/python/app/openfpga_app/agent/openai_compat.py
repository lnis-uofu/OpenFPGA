"""A model over an OpenAI-compatible endpoint: llama.cpp's server, Ollama, vLLM

Only the standard library: the request runs in a thread, and its chunks come to the loop as they
are read. Two modes (AgentConfig.tool_mode): the tool calls of the endpoint, or a JSON object that
the server constrains to the tools.
"""

import asyncio
import itertools
import json
import threading
import time
import urllib.error
import urllib.request
from typing import Any, AsyncIterator, Dict, Iterator, List, Optional, Union

from . import jsonmode
from .config import AgentConfig
from .model import ModelError, ModelTurn, TextChunk, ToolCallRequest

_ids = itertools.count(1)


class OpenAICompatModel:
    def __init__(self, config: AgentConfig):
        if not config.base_url:
            raise ValueError(
                "base_url is the address of the endpoint, e.g. http://127.0.0.1:8080/v1"
            )
        self.config = config
        self.name = config.model
        self._response: Optional[Any] = None
        self._cancelled = False

    # The request ---------------------------------------------------------------------------

    def _body(self, messages: List[Dict[str, Any]], tools: List[Dict[str, Any]]) -> Dict[str, Any]:
        config = self.config
        body: Dict[str, Any] = {
            "model": config.model,
            "stream": True,
            "temperature": config.temperature,
            "max_tokens": config.max_tokens,
            # llama.cpp keeps the start of the prompt between requests when asked to
            "cache_prompt": True,
            "stream_options": {"include_usage": True},
        }
        if config.tool_mode == "json":
            body["messages"] = jsonmode.to_messages(messages, tools)
            body["response_format"] = {
                "type": "json_schema",
                "json_schema": {"name": "step", "schema": jsonmode.step_schema(tools)},
            }
        else:
            body["messages"] = messages
            body["tools"] = tools
        return body

    def _open(self, body: Dict[str, Any]):
        url = self.config.base_url.rstrip("/") + "/chat/completions"
        request = urllib.request.Request(
            url,
            data=json.dumps(body).encode(),
            headers={"Content-Type": "application/json", "Accept": "text/event-stream"},
        )
        try:
            return urllib.request.urlopen(request, timeout=self.config.max_seconds)
        except urllib.error.HTTPError as error:
            detail = error.read().decode(errors="replace")[:400]
            raise ModelError("model_error", "The endpoint answered %d: %s" % (error.code, detail))
        except (urllib.error.URLError, OSError) as error:
            raise ModelError(
                "model_unreachable",
                "The model does not answer at %s: %s"
                % (self.config.base_url, getattr(error, "reason", error)),
                "Start llama-server, or change the endpoint",
            )

    def _run(self, messages: List[Dict[str, Any]], tools: List[Dict[str, Any]]) -> Iterator[Any]:
        """The chunks and the turn of one request, read as they arrive"""
        started = time.monotonic()
        response = self._response = self._open(self._body(messages, tools))
        text: List[str] = []
        calls: Dict[int, Dict[str, str]] = {}
        usage: Dict[str, Any] = {}
        json_mode = self.config.tool_mode == "json"
        try:
            for raw in response:
                line = raw.decode(errors="replace").strip()
                if not line.startswith("data:"):
                    continue
                data = line[5:].strip()
                if data == "[DONE]":
                    break
                try:
                    chunk = json.loads(data)
                except ValueError:
                    continue
                if chunk.get("usage"):
                    usage = chunk["usage"]
                for choice in chunk.get("choices") or []:
                    delta = choice.get("delta") or {}
                    piece = delta.get("content")
                    if piece:
                        text.append(piece)
                        if not json_mode:
                            yield TextChunk(piece)
                    for part in delta.get("tool_calls") or []:
                        slot = calls.setdefault(
                            part.get("index", 0), {"id": "", "name": "", "arguments": ""}
                        )
                        slot["id"] = part.get("id") or slot["id"]
                        function = part.get("function") or {}
                        slot["name"] = function.get("name") or slot["name"]
                        slot["arguments"] += function.get("arguments") or ""
        except (OSError, ValueError) as error:
            if self._cancelled:
                raise ModelError("cancelled", "The request was cancelled")
            raise ModelError("model_error", "The answer of the model was cut: %s" % error)
        finally:
            response.close()
            self._response = None
        if self._cancelled:
            raise ModelError("cancelled", "The request was cancelled")
        if json_mode:
            turn = jsonmode.parse_step("".join(text), "call_%d" % next(_ids))
            if turn.text:
                yield TextChunk(turn.text)
        else:
            turn = ModelTurn(text="".join(text))
            for index in sorted(calls):
                slot = calls[index]
                try:
                    arguments = json.loads(slot["arguments"] or "{}")
                    error = None if isinstance(arguments, dict) else "arguments is not an object"
                    arguments = arguments if isinstance(arguments, dict) else {}
                except ValueError as failure:
                    arguments, error = {}, "arguments are not valid JSON: %s" % failure
                turn.tool_calls.append(
                    ToolCallRequest(
                        slot["id"] or "call_%d" % next(_ids), slot["name"], arguments, error
                    )
                )
        turn.prompt_tokens = usage.get("prompt_tokens")
        turn.completion_tokens = usage.get("completion_tokens")
        turn.seconds = time.monotonic() - started
        yield turn

    # The loop's side -------------------------------------------------------------------------

    async def stream(
        self, messages: List[Dict[str, Any]], tools: List[Dict[str, Any]]
    ) -> AsyncIterator[Union[TextChunk, ModelTurn]]:
        self._cancelled = False
        loop = asyncio.get_running_loop()
        queue: "asyncio.Queue[Any]" = asyncio.Queue()

        def work():
            try:
                for item in self._run(messages, tools):
                    loop.call_soon_threadsafe(queue.put_nowait, item)
            except BaseException as error:  # noqa: BLE001 - handed to the loop
                loop.call_soon_threadsafe(queue.put_nowait, error)
            finally:
                loop.call_soon_threadsafe(queue.put_nowait, None)

        threading.Thread(target=work, daemon=True).start()
        while True:
            item = await queue.get()
            if item is None:
                return
            if isinstance(item, BaseException):
                raise item
            yield item

    def cancel(self) -> None:
        self._cancelled = True
        response = self._response
        if response is not None:
            try:
                response.close()
            except OSError:
                pass
