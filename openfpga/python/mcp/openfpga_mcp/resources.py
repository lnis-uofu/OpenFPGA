"""The resources and the prompts of the server, see DESIGN.md

Resources are what a client reads without a tool call: the checkpoints of a session, the catalogue
of the options, the graph of the stages. Each is bounded as an answer is. Prompts tell a model how
to go about the two questions that come up all the time.
"""

import dataclasses
import json
import threading
from typing import Any, Dict, List, Optional

import anyio.to_thread

from . import answers, budget
from .config import Config
from .errors import InvalidArgument, RegistryError
from .registry import SessionRegistry

STAGES = ("pack", "place", "route", "analysis")


class Catalogue:
    """What every session has in common: the options of each stage and the graph of the stages.

    It is asked of a worker that is started for it and ended at once, and kept: it does not change
    while the server runs. No data of VPR is needed for it, so it works before any session."""

    def __init__(self, registry: SessionRegistry, config: Config):
        self._registry = registry
        self._config = config
        self._lock = threading.Lock()
        self._stages: Optional[List[Dict[str, Any]]] = None

    def stages(self) -> List[Dict[str, Any]]:
        with self._lock:
            if self._stages is None:
                self._stages = self._load()
            return self._stages

    def _load(self) -> List[Dict[str, Any]]:
        import openfpga

        directory = self._config.root / ".catalogue"
        worker = self._registry.spawn(
            lambda: openfpga.RemoteSession(cwd=directory, timeout_s=self._config.call_timeout_s)
        )
        try:
            report = worker.stage_info()
        finally:
            worker.close()
        return [dataclasses.asdict(stage) for stage in report.stages]


def _stage_graph(stages: List[Dict[str, Any]]) -> Dict[str, Any]:
    """The stages and how they follow one another, without their options"""
    return {
        "kind": "stage_graph",
        "stages": [
            {
                "stage": s["stage"],
                "requires": s["requires"],
                "commands": s["commands"],
                "dropped_by_change": s["dropped_by_change"],
                "options": s["options"]["total"],
            }
            for s in stages
        ],
    }


def _checkpoints(config: Config, directory: str) -> Dict[str, Any]:
    base = config.root / directory
    found = []

    def describe(name: str, kind: str, path):
        try:
            manifest = json.loads((path / "checkpoint.json").read_text())
        except (OSError, ValueError):
            return
        found.append(
            {
                "name": name,
                "kind": kind,
                "stages": manifest.get("stages", []),
                "options_changed": [o["name"] for o in manifest.get("options", [])],
            }
        )

    describe("last", "after the last stage that was finished", base / "checkpoints")
    named = base / "named"
    if named.is_dir():
        for path in sorted(named.iterdir()):
            describe(path.name, "named", path)
    return {"kind": "checkpoints", "checkpoints": found}


def register(server, registry: SessionRegistry, config: Config) -> Catalogue:
    catalogue = Catalogue(registry, config)

    def reply(data: Any, more: Optional[str] = None) -> str:
        folders = list(config.folders())
        return answers.text(budget.fit(answers.relative(data, folders), config.answer_chars, more))

    async def guarded(function) -> str:
        try:
            return await anyio.to_thread.run_sync(function)
        except Exception as error:  # noqa: BLE001 - a failure is an answer, as for a tool
            return answers.text(answers.from_exception(error))

    @server.resource(
        "openfpga://sessions/{session}/checkpoints",
        name="checkpoints",
        description="The checkpoints of a session: the one the server saves after every stage "
        "('last') and the ones saved with the checkpoint tool, with the stages each holds. "
        "session_restore goes back to one.",
        mime_type="application/json",
    )
    async def session_checkpoints(session: str) -> str:
        def work():
            entry = registry.get(session)
            return reply(_checkpoints(config, entry.directory))

        return await guarded(work)

    @server.resource(
        "openfpga://stages",
        name="stages",
        description="The stages of the flow and how they follow one another: what each stage "
        "requires, the commands that reach it, the stages that a change to its options drops, "
        "and how many options it has.",
        mime_type="application/json",
    )
    async def stages() -> str:
        return await guarded(lambda: reply(_stage_graph(catalogue.stages())))

    @server.resource(
        "openfpga://options",
        name="options",
        description="The stages that have options, with how many each has. Read "
        "openfpga://options/{stage} for the options of one stage.",
        mime_type="application/json",
    )
    async def options() -> str:
        def work():
            return reply(
                {
                    "kind": "option_catalogue",
                    "stages": [
                        {"stage": s["stage"], "options": s["options"]["total"]}
                        for s in catalogue.stages()
                    ],
                }
            )

        return await guarded(work)

    @server.resource(
        "openfpga://options/{stage}",
        name="stage_options",
        description="The options that tune one stage (PACKED, PLACED, ROUTED, ANALYZED, DEVICE, "
        "or pack, place, route, analysis): name, type, default, choices and help. It is cut to "
        "the size of an answer; list_options with a query narrows it.",
        mime_type="application/json",
    )
    async def stage_options(stage: str) -> str:
        def work():
            wanted = {"pack": "PACKED", "place": "PLACED", "route": "ROUTED"}.get(
                stage, {"analysis": "ANALYZED"}.get(stage, stage.upper())
            )
            for item in catalogue.stages():
                if item["stage"] == wanted:
                    shown = dict(item["options"], kind="stage_options", stage=wanted)
                    # Their values are those of the first session: the catalogue has the defaults
                    for option in shown["items"]:
                        option.pop("value", None)
                        option.pop("is_default", None)
                    return reply(
                        shown, "list_options(session, stage, query) gives them with their values"
                    )
            known = ", ".join(s["stage"] for s in catalogue.stages())
            raise InvalidArgument("There is no stage '%s' (there are: %s)" % (stage, known))

        return await guarded(work)

    # The prompts -------------------------------------------------------------------------

    @server.prompt(
        name="what_is_the_state",
        description="Say where the flow of a session is and what to do next",
    )
    def what_is_the_state(session: str) -> str:
        return (
            "Call get_state for the session %s. Say which stages are reached, what each did (the "
            "clusters, the placement cost, the routing iterations and wirelength, the seconds) "
            "and whether a stage failed. Use only the numbers that get_state gave, as they are. "
            "Then name the next stage to run (run_stage), or the query that fits the question."
            % session
        )

    @server.prompt(
        name="why_did_it_fail",
        description="Find the cause of a stage that failed and say what to try",
    )
    def why_did_it_fail(session: str) -> str:
        return (
            "A stage of the session %s failed. Call get_state and read failed (the stage and its "
            "message) and the diagnostics kind. If the stage is ROUTED, call get_congestion: it "
            "answers after a failed route, and the hotspots and the nets through them tell where "
            "the router could not find room. For any stage, read_log with the last lines of the "
            "stage if the diagnostics do not say enough. Name the cause from the diagnostic kind "
            "and cite the number that supports it (the channel width, the overuse, the line of "
            "the log). Then give one thing to try: set_constraint chan_width, set_option, or "
            "another stage. Do not guess what the data do not show." % session
        )

    return catalogue
