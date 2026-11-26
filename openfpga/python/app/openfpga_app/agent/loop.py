"""The loop: the model chooses a tool call, the tool runs, the answer goes back, until the model has
the answer for the person, or a limit is reached

    message of the user -> [ model -> tool calls -> answers ] ... -> text for the person

Everything it does is an event of the kind that the page reads, and a line of a record that can be
read again. The limits: steps, time, the size of the context (old tool answers become a line),
the same call again and again, a failure of the same kind again and again, and a cancel.
"""

import dataclasses
import inspect
import json
import time
from pathlib import Path
from typing import Any, AsyncIterator, Awaitable, Callable, Dict, List, Optional

from .. import events
from . import context
from .arguments import check_arguments
from .config import AgentConfig
from .model import Model, ModelError, ModelTurn, TextChunk, ToolCallRequest
from .prompt import system_prompt
from .schema import tool_definitions
from .stated import stated_files

# The failures whose hint names the call to make next, and what is said when the model does not
NEXT_CALL_KINDS = ("input_not_read", "stage_not_reached")
NUDGE = (
    "[The last call failed because something comes first. Make that call yourself now, with file "
    "names from the list of files; do not ask the person and do not tell them to do it.]"
)

FINAL_ONLY = (
    "[The files that the person named were refused, as the last answer says. Make no more calls: "
    "say in your answer which file is of which kind (or that it does not exist), and ask which "
    "file to use.]"
)

Invoke = Callable[[str, Dict[str, Any]], Awaitable[Dict[str, Any]]]


def _invalid(kind: str, message: str, hint: Optional[str] = None) -> Dict[str, Any]:
    return {
        "ok": False,
        "diagnostics": [
            {
                "severity": "error",
                "kind": kind,
                "message": message,
                "file": None,
                "line": None,
                "hint": hint,
                "data": {},
            }
        ],
    }


class AgentLoop:
    """One conversation: the history stays between the messages of the user.

    tools are the functions by name (for the definitions and the checks of the arguments); invoke
    runs one and gives its answer, with whatever the holder does about it (the current session).
    session is the id that a call gets when it has none."""

    def __init__(
        self,
        model: Model,
        tools: Dict[str, Callable],
        config: AgentConfig,
        invoke: Invoke,
        session: Callable[[], Optional[str]] = lambda: None,
        files: Callable[[], str] = lambda: "",
        on_cancel: Callable[[], None] = lambda: None,
        record: Optional[Path] = None,
    ):
        self.model = model
        self.tools = tools
        self.config = config
        self._invoke = invoke
        self._session = session
        self._files = files
        self._on_cancel = on_cancel
        self.record_path = record
        self.records: List[Dict[str, Any]] = []
        self.definitions = tool_definitions(tools, config.compact_descriptions)
        self.history: List[Dict[str, Any]] = []
        self._system: Optional[str] = None
        self._notes: List[str] = []
        self._ratio = context.DEFAULT_RATIO
        self._cancelled = False

    # What the holder tells ------------------------------------------------------------------

    def note(self, text: str) -> None:
        """Something changed outside the conversation: the next message of the user starts with it"""
        self._notes.append(text)

    def cancel(self) -> None:
        self._cancelled = True
        self.model.cancel()
        self._on_cancel()

    def reset(self) -> None:
        """A new conversation"""
        self.history = []
        self._system = None
        self._notes = []

    # The record ------------------------------------------------------------------------------

    def _record(self, kind: str, /, **fields: Any) -> None:
        entry = {"t": round(time.time(), 3), "kind": kind, **fields}
        self.records.append(entry)
        if self.record_path is not None:
            with open(self.record_path, "a") as handle:
                handle.write(json.dumps(entry, ensure_ascii=False) + "\n")

    # The messages ------------------------------------------------------------------------------

    def _messages(self) -> List[Dict[str, Any]]:
        if self._system is None:
            self._system = system_prompt(self._files())
        out = [{"role": "system", "content": self._system}]
        for message in self.history:
            out.append({k: v for k, v in message.items() if k != "summarized"})
        return out

    def _tokens(self) -> int:
        return context.estimate(self._messages(), self.definitions, self._ratio)

    def _make_room(self) -> bool:
        """Compact if the context is 80% full. False if it is full and nothing more can be done"""
        limit = self.config.context_tokens
        if self._tokens() <= limit * 0.8:
            return True
        replaced = context.compact(
            self.history, self.definitions, self._ratio, limit, self.config.keep_recent_results
        )
        if replaced:
            self._record("compacted", replaced=replaced, tokens=self._tokens())
        return self._tokens() <= limit

    # One message of the user ----------------------------------------------------------------------

    def _as_stated(self, request: ToolCallRequest, stated: Dict[str, str]) -> ToolCallRequest:
        """read_inputs gets the files that the person named, in the inputs they named them for"""
        if request.name != "read_inputs" or request.error:
            return request
        changed = {k: v for k, v in stated.items() if request.arguments.get(k) != v}
        if not changed:
            return request
        self._record("as_stated", before=request.arguments, set=changed)
        return dataclasses.replace(request, arguments={**request.arguments, **changed})

    async def run(self, message_id: str, text: str) -> AsyncIterator[Dict[str, Any]]:
        config = self.config
        self._cancelled = False
        yield events.message_start(message_id)
        notes = "".join("[%s]\n" % n for n in self._notes)
        self._notes = []
        self.history.append({"role": "user", "content": notes + text})
        self._record("user", text=text, notes=notes or None)
        started = time.monotonic()
        previous: Optional[str] = None
        repeats = failures = 0
        nudged = False
        stated = stated_files(text)
        as_stated: set = set()
        final_only = told = False
        last_kind: Optional[str] = None
        stop: Optional[str] = None
        step = 0
        while stop is None:
            if self._cancelled:
                yield events.done(message_id, cancelled=True)
                return
            if step >= config.max_steps:
                stop = "the limit of %d steps" % config.max_steps
                break
            if time.monotonic() - started > config.max_seconds:
                stop = "the limit of %g seconds" % config.max_seconds
                break
            if not self._make_room():
                stop = "the context is full (%d tokens)" % self._tokens()
                break
            step += 1
            if final_only and not told:
                told = True  # after the answers of all the calls of the reply that failed
                self.history.append({"role": "user", "content": FINAL_ONLY})
            messages = self._messages()
            definitions = [] if final_only else self.definitions
            characters = context.size(messages, definitions)
            turn: Optional[ModelTurn] = None
            try:
                async for item in self.model.stream(messages, definitions):
                    if isinstance(item, TextChunk):
                        yield events.text_delta(message_id, item.text)
                    else:
                        turn = item
            except ModelError as error:
                self._record("model_error", error_kind=error.kind, message=str(error))
                if error.kind == "cancelled" or self._cancelled:
                    yield events.done(message_id, cancelled=True)
                else:
                    yield events.error(str(error), error.kind, error.hint)
                    yield events.done(message_id)
                return
            assert turn is not None
            self._ratio = context.calibrate(turn.prompt_tokens, characters, self._ratio)
            self._record(
                "model",
                step=step,
                text=turn.text,
                calls=[{"name": c.name, "arguments": c.arguments} for c in turn.tool_calls],
                prompt_tokens=turn.prompt_tokens,
                completion_tokens=turn.completion_tokens,
                seconds=round(turn.seconds, 2),
            )
            yield events.usage(
                message_id,
                step,
                turn.prompt_tokens,
                turn.completion_tokens,
                self._tokens(),
                config.context_tokens,
            )
            if final_only:
                turn.tool_calls = []  # no tools were offered: what the model says is the answer
            if stated:
                for i, requested in enumerate(turn.tool_calls):
                    fixed = self._as_stated(requested, stated)
                    if fixed is not requested:
                        as_stated.add(fixed.id)
                        turn.tool_calls[i] = fixed
            assistant: Dict[str, Any] = {"role": "assistant", "content": turn.text or None}
            if turn.tool_calls:
                assistant["tool_calls"] = [
                    {
                        "id": c.id,
                        "type": "function",
                        "function": {"name": c.name, "arguments": json.dumps(c.arguments)},
                    }
                    for c in turn.tool_calls
                ]
            self.history.append(assistant)
            if not turn.tool_calls:
                # A reply that ends after a call that failed because something comes first, and asks
                # the person to do it: say once that the call is the model's to make
                if last_kind in NEXT_CALL_KINDS and not nudged:
                    nudged = True
                    self._record("nudge", after=last_kind)
                    self.history.append({"role": "user", "content": NUDGE})
                    continue
                break
            skipped = False
            for request in turn.tool_calls:
                key = request.name + json.dumps(request.arguments, sort_keys=True)
                repeats = repeats + 1 if key == previous else 1
                previous = key
                if repeats >= config.repeat_limit:
                    stop = "the same call to %s %d times in a row" % (request.name, repeats)
                answer: Dict[str, Any]
                seconds = 0.0
                function = self.tools.get(request.name)
                opened = None
                if (
                    config.auto_session
                    and not self._cancelled
                    and not skipped
                    and not request.error
                    and function is not None
                    and "session_open" in self.tools
                    and self._session() is None
                    and not request.arguments.get("session")
                    and "session" in inspect.signature(function).parameters
                ):
                    # A model that forgets to open a session is not sent back to do it: the loop
                    # opens one, as a call that the person sees, and says so in the answer
                    opened = "open_%s" % request.id
                    yield events.tool_call_started(message_id, opened, "session_open", {})
                    yield events.state_changed("running")
                    begun = time.monotonic()
                    first = await self._invoke("session_open", {})
                    took = time.monotonic() - begun
                    yield events.tool_call_finished(
                        message_id, opened, bool(first.get("ok")), took, first
                    )
                    self._record(
                        "tool",
                        name="session_open",
                        arguments={},
                        answer=first,
                        seconds=round(took, 2),
                        automatic=True,
                    )
                    if not first.get("ok"):
                        opened = None
                yield events.tool_call_started(
                    message_id, request.id, request.name, request.arguments
                )
                if self._cancelled:
                    answer = _invalid("cancelled", "Cancelled")
                elif skipped:
                    # The calls of one reply run in order, and none after a failure: each needs
                    # the answer of the one before (a stage, a session)
                    answer = _invalid(
                        "not_run",
                        "Not run: the call before it in this reply failed",
                        "Make one call at a time and read its answer before the next",
                    )
                elif request.name == "use_device_package" and (
                    stated.keys() & {"vpr_arch", "openfpga_arch"}
                ):
                    # The person named architecture files: read_inputs reads those, and a package
                    # that a model makes up (from the name of a file) must not stop the calls after it
                    answer = {
                        "ok": True,
                        "data": {
                            "skipped": True,
                            "reason": "The person named the architecture files, so no device "
                            "package is used: read_inputs reads those files",
                        },
                        "truncated": False,
                        "more": None,
                    }
                    self._record("skipped_package", arguments=request.arguments)
                elif function is None:
                    answer = _invalid(
                        "invalid_call",
                        "There is no tool '%s'" % request.name,
                        "The tools are: %s" % ", ".join(sorted(self.tools)),
                    )
                elif request.error:
                    answer = _invalid(
                        "invalid_call", request.error, "Give the arguments as a JSON object"
                    )
                else:
                    try:
                        arguments = check_arguments(function, request.arguments, self._session())
                    except ValueError as error:
                        answer = _invalid(
                            "invalid_call", str(error), "Fix the arguments and call again"
                        )
                    else:
                        yield events.state_changed("running")
                        begun = time.monotonic()
                        answer = await self._invoke(request.name, arguments)
                        seconds = time.monotonic() - begun
                        if opened and isinstance(answer, dict):
                            answer["note"] = (
                                "No session was open, so session %s was opened for this call"
                                % self._session()
                            )
                content = json.dumps(answer, separators=(",", ":"), ensure_ascii=False)
                self._record(
                    "tool",
                    name=request.name,
                    arguments=request.arguments,
                    answer=answer,
                    seconds=round(seconds, 2),
                )
                self.history.append(
                    {"role": "tool", "tool_call_id": request.id, "content": content}
                )
                yield events.tool_call_finished(
                    message_id, request.id, bool(answer.get("ok")), seconds, answer
                )
                command = (answer.get("data") or {}).get("command") if answer.get("ok") else None
                if request.name == "show" and isinstance(command, dict):
                    # What the model points at, shown in the views of the page
                    yield events.view_command(command)
                yield events.state_changed(request.name)
                if self._cancelled:
                    yield events.done(message_id, cancelled=True)
                    return
                kind = (
                    None if answer.get("ok") else (answer.get("diagnostics") or [{}])[0].get("kind")
                )
                if kind == "not_run":
                    continue  # it did not fail: it was not tried
                if request.id in as_stated and kind in ("wrong_file_kind", "file_not_found"):
                    # The person's own choice of files is what failed: it is theirs to settle
                    final_only = True
                    self._record("final_only", after=kind)
                skipped = skipped or not answer.get("ok")
                failures = failures + 1 if kind and kind == last_kind else (1 if kind else 0)
                last_kind = kind
                if failures >= config.repeat_limit and stop is None:
                    stop = "%d failures of the same kind (%s) in a row" % (failures, kind)
        if stop:
            note = "Stopped: %s." % stop
            self._record("stopped", reason=stop)
            self.history.append({"role": "assistant", "content": note})
            yield events.text_delta(message_id, "\n" + note)
        yield events.done(message_id)
