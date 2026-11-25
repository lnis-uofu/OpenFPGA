"""What the server asks of whoever runs the conversation and the session.

A backend gives the view of the session (what the sidebar and the top bar draw), streams the events
of a message, cancels, and runs a command of the console. The real one (the agent loop over the
tools of the MCP server) comes later; DemoBackend replays a recorded conversation, so that the page
can be built and tested before it.
"""

import asyncio
import copy
import json
from pathlib import Path
from typing import Any, AsyncIterator, Dict, List, Optional, Protocol

from . import events

STAGES = ("DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED")
VIEW_VERSION = 1


class Backend(Protocol):
    kind: str  # "demo" or "session": the page enables the buttons of the sessions for the second

    async def view(self) -> Dict[str, Any]: ...

    async def new_session(
        self, name: Optional[str] = None, device_package: Optional[str] = None
    ) -> Dict[str, Any]: ...

    async def device_packages(self) -> Dict[str, Any]: ...

    async def apply_device_package(self, device_package: str) -> Dict[str, Any]: ...

    async def list_sessions(self) -> Dict[str, Any]: ...

    async def open_session(self, session: str) -> Dict[str, Any]: ...

    async def read_file(self, name: str) -> Dict[str, Any]: ...

    async def read_report(self, stage: str) -> Optional[str]: ...

    async def write_file(self, name: str, text: str) -> Dict[str, Any]: ...

    async def copy_file(self, name: str) -> Dict[str, Any]: ...

    async def close_session(self) -> Dict[str, Any]: ...

    async def restore_session(self, checkpoint: Optional[str] = None) -> Dict[str, Any]: ...

    async def save_checkpoint(self, name: Optional[str] = None) -> Dict[str, Any]: ...

    async def add_design(self, design: str) -> Dict[str, Any]: ...

    async def upload(self, name: str, data: bytes, overwrite: bool) -> Dict[str, Any]: ...

    async def remove_upload(self, name: str) -> Dict[str, Any]: ...

    def chat(self, message: str) -> AsyncIterator[Dict[str, Any]]: ...

    def cancel(self) -> None: ...

    async def console(self, command: str, mode: str = "shell") -> Dict[str, Any]: ...


def empty_view() -> Dict[str, Any]:
    """The view when there is no session: the sections are there, with nothing in them"""
    return {
        "schema_version": VIEW_VERSION,
        "session": None,
        "latest": {"status": "none", "text": "No session"},
        # Synthesis comes before the stages of VPR: not_reached, reached (the circuit is the netlist
        # of a synthesis), failed, or skipped (the design is a netlist, so it needs none)
        "synthesis": {"status": "not_reached", "seconds": None, "message": None},
        "stages": [
            {"stage": s, "status": "not_reached", "seconds": None, "message": None} for s in STAGES
        ],
        "files": {"inputs": [], "constraints": [], "results": [], "uploads": [], "created": [], "synthesis": [], "design": []},
        "options_changed": [],
        "checkpoints": [],
        # The stages that have an HTML report (reports/<stage>.html of the session): device,
        # synthesis, pack, place, route, analysis
        "reports": [],
        # The design directories of the designs folders (a folder with a design.json): name, top
        # module and rtl files; not part of a session
        "designs": [],
        # The open sessions (id, name, running): the tabs of the sidebar
        "sessions": [],
        # The ODC of the session, checked on what is read now (None: none, and no I/O map)
        "constraints": None,
    }


# The recorded conversation: what the model says between its calls (the calls and their results
# are the ones of the transcript of the MCP server's tests, as the model read them)
_SEGMENTS = [
    (
        0,
        5,
        "I'll open a session, set the clock modeling and the channel width, and read the inputs.",
    ),
    (5, 9, "The inputs are read. Now the four stages, one after the other."),
    (9, 12, "The flow is done. Let me read the state, the timing and the utilization."),
    (12, 18, "Now the blocks, a net, the placement and the congestion."),
    (18, 21, "I'll change the seed and widen the channel to 24."),
    (21, 23, "Both changes dropped the stages. Let me check, and try to route."),
    (23, 24, "That is all for this session, so I'll close it."),
]
_CLOSING = (
    "The flow ran on routing_test: the critical path is 1.42 ns, which is 704 MHz. "
    "(This is a recorded demo; the model is not connected yet.)"
)


def _words(text: str) -> List[str]:
    parts = text.split(" ")
    return [p + (" " if i < len(parts) - 1 else "") for i, p in enumerate(parts)]


class DemoBackend:
    """A conversation that is replayed, with real answers of the tools.

    delay is the pause between events in seconds (0 in tests)."""

    def __init__(self, transcript: Optional[Path] = None, delay: float = 0.04):
        path = transcript or Path(__file__).with_name("demo_transcript.json")
        self._steps: List[Dict[str, Any]] = json.loads(Path(path).read_text())
        self._delay = delay
        self._view = empty_view()
        self._cancel = False
        self._busy = False
        self._played = False
        self._counter = 0

    # The view ------------------------------------------------------------------------------

    kind = "demo"

    async def view(self) -> Dict[str, Any]:
        return copy.deepcopy(self._view)

    async def _unavailable(self, *_args) -> Dict[str, Any]:
        return {
            "ok": False,
            "diagnostics": [
                {
                    "severity": "error",
                    "kind": "demo",
                    "message": "This is the recorded demo: it has no sessions of its own",
                    "hint": "Start the app with --root to work on real sessions",
                }
            ],
        }

    async def device_packages(self) -> Dict[str, Any]:
        return {"ok": True, "data": {"packages": [], "problems": []}}

    new_session = apply_device_package = close_session = restore_session = upload = remove_upload = read_file = write_file = copy_file = _unavailable
    open_session = save_checkpoint = add_design = _unavailable

    async def read_report(self, stage: str) -> Optional[str]:
        return None  # the recorded demo ran no stage

    async def list_sessions(self) -> Dict[str, Any]:
        return {"ok": True, "data": {"live": [], "saved": []}, "truncated": False, "more": None}


    def _latest(self, status: str, text: str):
        self._view["latest"] = {"status": status, "text": text}

    def _set_stages(self, reached: List[str], seconds: Optional[Dict[str, float]] = None):
        seconds = seconds or {}
        old = {s["stage"]: s for s in self._view["stages"]}
        self._view["stages"] = [
            {
                "stage": s,
                "status": "reached" if s in reached else "not_reached",
                "seconds": seconds.get(s, old[s]["seconds"] if s in reached else None),
                "message": None,
            }
            for s in STAGES
        ]
        # The demo reads a netlist: nothing is synthesized
        self._view["synthesis"] = (
            {"status": "skipped", "seconds": None, "message": "Not needed: the design is a netlist"}
            if "DEVICE" in reached
            else {"status": "not_reached", "seconds": None, "message": None}
        )

    def _apply(self, step: Dict[str, Any], seconds: float):
        """What a tool call changes in the view, from its answer"""
        tool, arguments, answer = step["tool"], step["arguments"], step["answer"]
        view = self._view
        name = view["session"]["name"] if view["session"] else ""
        if not answer.get("ok"):
            kind = answer["diagnostics"][0]["kind"]
            self._latest("failed", "%s: %s" % (arguments.get("stage", tool), kind))
            return
        data = answer["data"]
        if tool == "session_open":
            view["session"] = {
                "id": data["session"],
                "name": data["name"],
                "worker": "running",
                "ended": None,
                "idle_s": 0,
            }
            self._latest("running", "%s · session opened" % data["session"])
        elif tool == "session_close":
            view["session"]["worker"] = "none"
            self._latest("none", "%s · closed" % data["session"])
        elif tool == "read_inputs":
            inputs = data["inputs"]
            view["files"]["inputs"] = [
                {
                    "label": "vpr arch",
                    "name": inputs["vpr_arch"]["file"],
                    "extra": "layout %s" % inputs["vpr_arch"]["layout"],
                },
                {"label": "openfpga arch", "name": inputs["openfpga_arch"]["file"], "extra": None},
                {"label": "circuit", "name": inputs["circuit"]["file"], "extra": None},
            ]
            self._set_stages(data["reached"])
            self._latest("reached", "%s · DEVICE ✔" % view["session"]["id"])
        elif tool == "run_stage":
            stage = {
                "pack": "PACKED",
                "place": "PLACED",
                "route": "ROUTED",
                "analysis": "ANALYZED",
            }[data["stage"]]
            self._set_stages(data["reached"], {stage: round(seconds, 1)})
            self._latest("reached", "%s · %s ✔ %.1f s" % (view["session"]["id"], stage, seconds))
            view["checkpoints"] = [
                {"name": "last", "kind": "after the last stage", "stages": data["reached"]}
            ]
        elif tool == "get_state":
            results = data["state"]["results"]
            # Names in the directory of the session, which is where the sidebar is
            prefix = view["session"]["id"] + "/"
            view["files"]["results"] = [
                v[len(prefix) :] if v.startswith(prefix) else v for v in results.values() if v
            ]
        elif tool == "set_option":
            changed = [o for o in view["options_changed"] if o["name"] != data["option"]]
            changed.append({"name": data["option"], "value": data["after"]})
            view["options_changed"] = changed
            self._drop(data["dropped"])
        elif tool == "set_constraint":
            view["files"]["constraints"] = ["chan width %s" % data["after"]]
            self._drop(data["dropped"])

    def _drop(self, dropped: List[str]):
        if dropped:
            reached = [s["stage"] for s in self._view["stages"] if s["status"] == "reached"]
            kept = [s for s in reached if s not in dropped]
            self._set_stages(kept)
            sid = self._view["session"]["id"]
            if kept:
                self._latest("reached", "%s · %s ✔" % (sid, kept[-1]))
            else:
                self._latest("none", "%s · nothing reached" % sid)

    # The conversation ------------------------------------------------------------------------

    def cancel(self) -> None:
        self._cancel = True

    async def _pause(self, factor: float = 1.0):
        if self._delay:
            await asyncio.sleep(self._delay * factor)

    async def chat(self, message: str) -> AsyncIterator[Dict[str, Any]]:
        self._counter += 1
        message_id = "m%d" % self._counter
        self._cancel = False
        if self._busy:
            yield events.error("A message is being answered", "busy", "Stop it, or wait")
            return
        self._busy = True
        try:
            yield events.message_start(message_id)
            if self._played:
                for word in _words(
                    "That was the recorded demo; start the server again to replay it."
                ):
                    yield events.text_delta(message_id, word)
                yield events.done(message_id)
                return
            for first, last, text in _SEGMENTS:
                for word in _words(text):
                    if self._cancel:
                        yield events.done(message_id, cancelled=True)
                        return
                    yield events.text_delta(message_id, word)
                    await self._pause(0.3)
                for index in range(first, last):
                    step = self._steps[index]
                    call_id = "c%d-%d" % (self._counter, index)
                    yield events.tool_call_started(
                        message_id, call_id, step["tool"], step["arguments"]
                    )
                    seconds = 0.3 + (index % 5) * 0.4
                    if step["tool"] == "run_stage":
                        self._latest(
                            "running",
                            "%s · %s running"
                            % (self._view["session"]["id"], step["arguments"]["stage"].upper()),
                        )
                        yield events.state_changed("running")
                    await self._pause(4)
                    if self._cancel:
                        yield events.tool_call_finished(
                            message_id,
                            call_id,
                            False,
                            seconds / 2,
                            {
                                "ok": False,
                                "diagnostics": [{"kind": "cancelled", "message": "Cancelled"}],
                            },
                        )
                        yield events.done(message_id, cancelled=True)
                        return
                    self._apply(step, seconds)
                    answer = step["answer"]
                    yield events.tool_call_finished(
                        message_id, call_id, bool(answer.get("ok")), seconds, answer
                    )
                    yield events.state_changed(step["tool"])
            for word in _words(_CLOSING):
                yield events.text_delta(message_id, word)
                await self._pause(0.3)
            self._played = True
            yield events.done(message_id)
        finally:
            self._busy = False

    # The console -------------------------------------------------------------------------------

    async def console(self, command: str, mode: str = "shell") -> Dict[str, Any]:
        """A few commands of the shell, enough to try the console. The real console runs any
        command in the session"""
        if mode == "tcl":
            return {
                "ok": False,
                "output": "Tcl is not in the demo: start the app with --root and --console",
                "state_changed": False,
            }
        words = command.split()
        if not words:
            return {"ok": True, "output": "", "state_changed": False}
        if words[0] == "help":
            return {
                "ok": True,
                "output": "Demo console. Commands: help, report_session, set_app_option --name N "
                "--value V",
                "state_changed": False,
            }
        if words[0] == "report_session":
            stages = [s["stage"] for s in self._view["stages"] if s["status"] == "reached"]
            return {"ok": True, "output": "reached: %s" % ", ".join(stages), "state_changed": False}
        if words[0] == "set_app_option":
            return {
                "ok": True,
                "output": "Stages dropped: (demo)",
                "state_changed": False,
            }
        return {
            "ok": False,
            "output": "Error: unknown command '%s' (this is the demo console)" % words[0],
            "state_changed": False,
        }
