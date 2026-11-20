"""Scenarios for the agent: a task in words, a plan for the scripted model, and a checker

A checker reads what the session holds (stages reached, the constraint, the failed stage, the
answers of the tools), and from the words of the model only what cannot be anywhere else (a number
that it was asked to report). A scenario runs with the scripted plan in CI, and with a real model
by `python -m openfpga_app.agent.scenarios --model-url ...`.
"""

import json
import re
import shutil
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any, Callable, Dict, List, Optional

from .. import events
from .config import AgentConfig
from .model import Model, ModelTurn
from .scripted import ScriptedModel, call, say

STAGES = ("DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED")


@dataclass
class Design:
    """The files of one design, and the width and layout that the flow is known to work with"""

    vpr_arch: Path
    openfpga_arch: Path
    circuit: Path
    layout: str = "2x2"
    chan_width: int = 20

    @property
    def name(self) -> str:
        return self.circuit.stem


@dataclass
class Call:
    tool: str
    arguments: Dict[str, Any]
    ok: bool
    answer: Dict[str, Any]


@dataclass
class Outcome:
    """What a run left: the calls, the words, the state of the session (as the tools give it)"""

    text: str = ""
    calls: List[Call] = field(default_factory=list)
    state: Dict[str, Any] = field(default_factory=dict)
    stages: Dict[str, str] = field(default_factory=dict)  # DEVICE: reached | failed | not_reached
    stopped: Optional[str] = None  # the loop's reason, if a limit ended the run
    seconds: float = 0.0
    steps: int = 0
    prompt_tokens: int = 0
    completion_tokens: int = 0
    errors: List[str] = field(default_factory=list)

    def reached(self) -> List[str]:
        return [s for s in STAGES if self.stages.get(s) == "reached"]

    def failed(self) -> Optional[str]:
        return next((s for s in STAGES if self.stages.get(s) == "failed"), None)

    def ok_calls(self, tool: str) -> List[Call]:
        return [c for c in self.calls if c.tool == tool and c.ok]

    def numbers(self) -> List[float]:
        found = re.findall(r"\d[\d,]*\.?\d*", self.text)
        values = []
        for item in found:
            try:
                values.append(float(item.replace(",", "").rstrip(".")))
            except ValueError:
                pass
        return values


@dataclass
class Scenario:
    name: str
    summary: str
    task: Callable[[Design], str]
    plan: Callable[[Design], List[ModelTurn]]
    check: Callable[[Outcome, Design], List[str]]  # the problems; none means the task was done
    prepare: Optional[Callable[[Path, Design], None]] = None  # more files in the designs folder

    def problems(self, outcome: "Outcome", design: Design) -> List[str]:
        """What is wrong with a run: a limit that ended it, then what the checker finds"""
        found = []
        if outcome.stopped:
            found.append("a limit ended the run: %s" % outcome.stopped)
        return found + self.check(outcome, design)


def _fmax(outcome: Outcome) -> Optional[float]:
    for item in reversed(outcome.ok_calls("get_timing")):
        try:
            return float(item.answer["data"]["timing_summary"]["setup"]["fmax_mhz"])
        except (KeyError, TypeError, ValueError):
            continue
    return None


def _said(outcome: Outcome, *words: str) -> bool:
    text = outcome.text.lower()
    return any(word.lower() in text for word in words)


def _width_set(outcome: Outcome) -> Optional[int]:
    """The channel width that the router used (what the session holds, not what was asked)"""
    try:
        return int(outcome.state["run"]["routing"]["channel_width"])
    except (KeyError, TypeError, ValueError):
        return None


# The plans: what a model that does the task right would call ------------------------------------------


def _open_and_read(design: Design, layout: Optional[str] = None) -> List[ModelTurn]:
    return [
        say("Opening a session.", call("session_open", name=design.name)),
        say(
            "Reading the inputs.",
            call(
                "read_inputs",
                vpr_arch=design.vpr_arch.name,
                layout=layout or design.layout,
                openfpga_arch=design.openfpga_arch.name,
                circuit=design.circuit.name,
            ),
        ),
    ]


def _stages(*names: str) -> List[ModelTurn]:
    return [say("", call("run_stage", stage=name)) for name in names]


def _plan_flow_fmax(design: Design) -> List[ModelTurn]:
    return (
        [say("", call("session_open", name=design.name))]
        + [say("", call("set_constraint", kind="chan_width", width=design.chan_width))]
        + _open_and_read(design)[1:]
        + _stages("pack", "place", "route", "analysis")
        + [say("", call("get_timing", paths=1))]
        + [lambda messages: say("The Fmax is %.2f MHz." % _fmax_in(messages))]
    )


def _fmax_in(messages: List[Dict[str, Any]]) -> float:
    """What a model would read in the answer of get_timing that it was given"""
    for message in reversed(messages):
        if message.get("role") == "tool" and "fmax_mhz" in message.get("content", ""):
            return json.loads(message["content"])["data"]["timing_summary"]["setup"]["fmax_mhz"]
    raise AssertionError("no timing in the messages")


PACKAGE = "k6_frac_n10_2x2"  # the device package of the architectures of the design

# A design in Verilog, put in the designs folder of the synthesis scenario
COUNTER_V = """module counter(input clk, input rst, output reg [3:0] q);
  always @(posedge clk) begin
    if (rst) q <= 4'd0; else q <= q + 4'd1;
  end
endmodule
"""


def _prepare_synthesis(inputs: Path, design: Design) -> None:
    (inputs / "counter.v").write_text(COUNTER_V)


def _plan_synthesis(design: Design) -> List[ModelTurn]:
    return (
        [say("", call("session_open", name="counter"))]
        + [say("", call("use_device_package", name=PACKAGE))]
        + [say("", call("synthesize", top="counter", files="counter.v"))]
        + [say("", call("set_constraint", kind="chan_width", width=design.chan_width))]
        + _stages("pack", "place", "route", "analysis")
        + [say("", call("get_timing", paths=1))]
        + [lambda messages: say("The Fmax is %.2f MHz." % _fmax_in(messages))]
    )


def _plan_device_package(design: Design) -> List[ModelTurn]:
    return (
        [say("", call("list_device_packages"))]
        + [say("", call("session_open", name=design.name))]
        + [say("", call("use_device_package", name=PACKAGE))]
        + [say("", call("set_constraint", kind="chan_width", width=design.chan_width))]
        + [say("", call("read_inputs", circuit=design.circuit.name))]
        + _stages("pack", "place", "route", "analysis")
        + [say("", call("get_timing", paths=1))]
        + [lambda messages: say("The Fmax is %.2f MHz." % _fmax_in(messages))]
    )


def _plan_set_width(design: Design) -> List[ModelTurn]:
    return (
        [say("", call("session_open", name=design.name))]
        + [say("", call("set_constraint", kind="chan_width", width=22))]
        + _open_and_read(design)[1:]
        + _stages("pack", "place", "route")
        + [say("Routed with a channel width of 22.")]
    )


def _plan_missing_file(design: Design) -> List[ModelTurn]:
    return [
        say("", call("session_open", name="missing")),
        say(
            "",
            call(
                "read_inputs",
                vpr_arch="no_such_arch.xml",
                layout=design.layout,
                openfpga_arch=design.openfpga_arch.name,
                circuit=design.circuit.name,
            ),
        ),
        say("The file no_such_arch.xml does not exist in the designs folders, so nothing was run."),
    ]


def _plan_wrong_kind(design: Design) -> List[ModelTurn]:
    return [
        say("", call("session_open", name="wrong")),
        say(
            "",
            call(
                "read_inputs",
                vpr_arch=design.openfpga_arch.name,
                layout=design.layout,
                openfpga_arch=design.openfpga_arch.name,
                circuit=design.circuit.name,
            ),
        ),
        say(
            "%s is an OpenFPGA architecture, not a VPR architecture, so the inputs were not read."
            % design.openfpga_arch.name
        ),
    ]


def _plan_unroutable(design: Design) -> List[ModelTurn]:
    return (
        [say("", call("session_open", name="narrow"))]
        + [say("", call("set_constraint", kind="chan_width", width=2))]
        + _open_and_read(design)[1:]
        + _stages("pack", "place", "route")
        + [say("", call("read_log", stage="route", tail=10))]
        + [say("The router could not route the design with a channel width of 2: too narrow.")]
    )


# The checkers ---------------------------------------------------------------------------------------


def _architectures(design: Design) -> str:
    """The architectures in the words of the person: a designs folder lists only the circuits"""
    return "Use %s as the VPR architecture (layout %s) and %s as the OpenFPGA architecture." % (
        design.vpr_arch.name,
        design.layout,
        design.openfpga_arch.name,
    )


def _check_flow_fmax(outcome: Outcome, design: Design) -> List[str]:
    problems = []
    if outcome.reached() != list(STAGES[: len(STAGES)]):
        problems.append("stages reached: %s, not all five" % ", ".join(outcome.reached()))
    if _width_set(outcome) != design.chan_width:
        problems.append(
            "the channel width is %s, not %d" % (_width_set(outcome), design.chan_width)
        )
    fmax = _fmax(outcome)
    if fmax is None:
        problems.append("get_timing was never answered")
    elif not any(abs(n - fmax) <= 0.005 * fmax for n in outcome.numbers()):
        problems.append("the Fmax in the answer is not %.2f MHz (what get_timing says)" % fmax)
    return problems


def _check_device_package(outcome: Outcome, design: Design) -> List[str]:
    problems = []
    if not any(c.arguments.get("name") == PACKAGE for c in outcome.ok_calls("use_device_package")):
        problems.append("the device package %s was not read into the session" % PACKAGE)
    return problems + _check_flow_fmax(outcome, design)


def _check_synthesis(outcome: Outcome, design: Design) -> List[str]:
    problems = []
    done = [c for c in outcome.ok_calls("synthesize") if c.arguments.get("top") == "counter"]
    if not done:
        problems.append("counter.v was not synthesized with top counter")
    elif not done[-1].answer.get("data", {}).get("circuit_read"):
        problems.append("the netlist was not read as the circuit")
    return problems + _check_flow_fmax(outcome, design)


def _check_set_width(outcome: Outcome, design: Design) -> List[str]:
    problems = []
    if "ROUTED" not in outcome.reached():
        problems.append("the design was not routed")
    if _width_set(outcome) != 22:
        problems.append("the channel width is %s, not 22" % _width_set(outcome))
    return problems


def _check_missing_file(outcome: Outcome, design: Design) -> List[str]:
    problems = []
    if outcome.reached():
        problems.append("a stage was reached with a file that does not exist")
    if not _said(outcome, "no_such_arch"):
        problems.append("the answer does not name the missing file")
    return problems


def _check_wrong_kind(outcome: Outcome, design: Design) -> List[str]:
    problems = []
    if outcome.reached():
        problems.append("a stage was reached with the wrong kind of file")
    if not (_said(outcome, design.openfpga_arch.name) and _said(outcome, "openfpga")):
        problems.append("the answer does not say that the file is of the wrong kind")
    return problems


def _check_unroutable(outcome: Outcome, design: Design) -> List[str]:
    problems = []
    if outcome.failed() != "ROUTED":
        problems.append("the route stage did not fail (%s)" % (outcome.failed() or "no stage"))
    if not _said(outcome, "width", "channel", "congest", "unroutable", "could not route"):
        problems.append("the answer does not say why the route failed")
    return problems


SCENARIOS: Dict[str, Scenario] = {
    s.name: s
    for s in (
        Scenario(
            "flow_fmax",
            "Run the whole flow at a given channel width and report the Fmax",
            lambda d: "Run the flow on %s with a channel width of %d and tell me the Fmax. %s"
            % (d.name, d.chan_width, _architectures(d)),
            _plan_flow_fmax,
            _check_flow_fmax,
        ),
        Scenario(
            "device_package",
            "Use a device package: list, read it into the session, run the flow",
            lambda d: "Use the device package %s and run the flow on %s with a channel width "
            "of %d. Tell me the Fmax." % (PACKAGE, d.circuit.name, d.chan_width),
            _plan_device_package,
            _check_device_package,
        ),
        Scenario(
            "synth_flow",
            "A design in Verilog: synthesize it for a device package, run the flow, report the Fmax",
            lambda d: "Use the device package %s. Synthesize counter.v (the top module is counter) "
            "and run the whole flow at a channel width of %d. Tell me the Fmax."
            % (PACKAGE, d.chan_width),
            _plan_synthesis,
            _check_synthesis,
            prepare=_prepare_synthesis,
        ),
        Scenario(
            "set_width",
            "Set a channel width and route",
            lambda d: "Route %s with a channel width of 22. %s" % (d.name, _architectures(d)),
            _plan_set_width,
            _check_set_width,
        ),
        Scenario(
            "missing_file",
            "A file that does not exist: say so, run nothing",
            lambda d: "Read the inputs for %s with the VPR architecture no_such_arch.xml." % d.name,
            _plan_missing_file,
            _check_missing_file,
        ),
        Scenario(
            "wrong_kind",
            "A file of the wrong kind: say what it is",
            lambda d: "Open %s and use %s as the VPR architecture."
            % (d.circuit.name, d.openfpga_arch.name),
            _plan_wrong_kind,
            _check_wrong_kind,
        ),
        Scenario(
            "unroutable_width",
            "A width that cannot route: find out and explain",
            lambda d: "Route %s with a channel width of 2 and tell me what happens. %s"
            % (d.name, _architectures(d)),
            _plan_unroutable,
            _check_unroutable,
        ),
    )
}


# The runner ----------------------------------------------------------------------------------------------


async def run_scenario(
    scenario: Scenario,
    design: Design,
    workdir: Path,
    model: Optional[Model] = None,
    agent_config: Optional[AgentConfig] = None,
    record: Optional[Path] = None,
    call_timeout_s: int = 300,
) -> Outcome:
    """Run one scenario in a folder of its own and return what it left (the checker is not run)"""
    from openfpga_mcp import Config

    from ..session_backend import SessionBackend

    inputs = workdir / "inputs"
    inputs.mkdir(parents=True, exist_ok=True)
    for source in (design.vpr_arch, design.openfpga_arch, design.circuit):
        shutil.copy(source, inputs / source.name)
    if scenario.prepare:
        scenario.prepare(inputs, design)
    model = model or ScriptedModel(scenario.plan(design))
    packages = Path(__file__).resolve().parents[5] / "openfpga_flow" / "device-packages"
    config = Config(
        root=workdir / "workspace",
        design_dirs=(inputs,),
        package_dirs=(packages,) if packages.is_dir() else (),
        call_timeout_s=call_timeout_s,
    )
    backend = SessionBackend(config, model=model, agent_config=agent_config, record=record)
    out = Outcome()
    started = time.monotonic()
    pending: Dict[str, Dict[str, Any]] = {}
    try:
        async for item in backend.chat(scenario.task(design)):
            kind = item["type"]
            if kind == "text_delta":
                out.text += item["text"]
            elif kind == "tool_call_started":
                pending[item["call_id"]] = item
            elif kind == "tool_call_finished":
                start = pending.pop(item["call_id"], {})
                out.calls.append(
                    Call(
                        start.get("tool", "?"),
                        start.get("arguments", {}),
                        item["ok"],
                        item["result"],
                    )
                )
            elif kind == "usage":
                out.steps += 1
                out.prompt_tokens += item.get("prompt_tokens") or 0
                out.completion_tokens += item.get("completion_tokens") or 0
            elif kind == "error":
                out.errors.append(item["message"])
        out.seconds = time.monotonic() - started
        stopped = re.search(r"Stopped: (.*)", out.text)
        out.stopped = stopped.group(1) if stopped else None
        view = await backend.view()
        out.stages = {s["stage"]: s["status"] for s in view["stages"]}
        if backend.sid:
            answer = json.loads(await backend.tools["get_state"](session=backend.sid))
            out.state = answer.get("data", {}) if answer.get("ok") else {}
    finally:
        backend.close()
    return out


def design_from_args(args: List[str]) -> Design:
    vpr, openfpga, circuit, layout, width = args
    return Design(Path(vpr), Path(openfpga), Path(circuit), layout, int(width))


def main(argv: Optional[List[str]] = None) -> int:
    """Run scenarios against a real model: python -m openfpga_app.agent.scenarios --help"""
    import argparse
    import asyncio
    import tempfile

    from .openai_compat import OpenAICompatModel

    parser = argparse.ArgumentParser(
        prog="python -m openfpga_app.agent.scenarios", description=main.__doc__
    )
    parser.add_argument("--model-url", required=True)
    parser.add_argument("--model-name", default="local")
    parser.add_argument("--tool-mode", choices=("native", "json"), default="native")
    parser.add_argument("--context-tokens", type=int, default=8192)
    parser.add_argument("--max-steps", type=int, default=20)
    parser.add_argument(
        "--design",
        nargs=5,
        required=True,
        metavar=("VPR_ARCH", "OPENFPGA_ARCH", "CIRCUIT", "LAYOUT", "CHAN_WIDTH"),
    )
    parser.add_argument("--scenario", action="append", choices=sorted(SCENARIOS))
    parser.add_argument("--repeat", type=int, default=1)
    parser.add_argument("--out", type=Path, help="a folder for the records and results.json")
    args = parser.parse_args(argv)
    design = design_from_args(args.design)
    config = AgentConfig(
        base_url=args.model_url,
        model=args.model_name,
        tool_mode=args.tool_mode,
        context_tokens=args.context_tokens,
        max_steps=args.max_steps,
    )
    out = args.out or Path(tempfile.mkdtemp(prefix="scenarios_"))
    out.mkdir(parents=True, exist_ok=True)
    results = []
    for name in args.scenario or list(SCENARIOS):
        for n in range(args.repeat):
            scenario = SCENARIOS[name]
            work = out / ("%s_%d" % (name, n + 1))
            outcome = asyncio.run(
                run_scenario(
                    scenario,
                    design,
                    work,
                    OpenAICompatModel(config),
                    config,
                    record=work / "record.jsonl",
                )
            )
            problems = scenario.problems(outcome, design)
            results.append(
                {
                    "scenario": name,
                    "run": n + 1,
                    "passed": not problems,
                    "problems": problems,
                    "stopped": outcome.stopped,
                    "steps": outcome.steps,
                    "seconds": round(outcome.seconds, 1),
                    "prompt_tokens": outcome.prompt_tokens,
                    "completion_tokens": outcome.completion_tokens,
                    "calls": [c.tool for c in outcome.calls],
                    "failed_calls": [c.tool for c in outcome.calls if not c.ok],
                    "answer": outcome.text[-400:],
                }
            )
            last = results[-1]
            print(
                "%-18s run %d  %s  %2d steps  %5.0fs  %s"
                % (
                    name,
                    n + 1,
                    "PASS" if last["passed"] else "FAIL",
                    last["steps"],
                    last["seconds"],
                    "; ".join(problems) or (outcome.stopped or ""),
                )
            )
    (out / "results.json").write_text(json.dumps(results, indent=1))
    passed = sum(r["passed"] for r in results)
    print("%d of %d passed; results in %s" % (passed, len(results), out))
    return 0 if passed == len(results) else 1


if __name__ == "__main__":
    raise SystemExit(main())
