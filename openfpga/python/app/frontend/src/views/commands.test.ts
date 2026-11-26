import { beforeEach, describe, expect, it } from "vitest";
import {
  activeTab,
  applyEvent,
  consoleLines,
  resetStore,
  tabs,
} from "../store";
import "./commands";
import { runModelCommand } from "./commands";
import { resetViewData, setWholeView } from "./data";
import { makeView } from "./format";
import { netlistRequest, resetNetlistRequests } from "./netlist";
import { resetSelection, selection, zoomRequest } from "./selection";
import { setTiming } from "./timing";

beforeEach(() => {
  resetStore();
  resetSelection();
  resetViewData();
  resetNetlistRequests();
});

function timing() {
  return makeView("timing", {
    nets: { slack_ns: Float32Array.of(-1), criticality: Float32Array.of(1) },
    blocks: { slack_ns: Float32Array.of(-1, 2) },
    paths: {
      slack_ns: Float32Array.of(-1),
      launch_clock: ["clk"],
      capture_clock: ["clk"],
      startpoint: ["a.q"],
      endpoint: ["c.d"],
      arrival_ns: Float32Array.of(2),
      required_ns: Float32Array.of(1),
      point_start: Uint32Array.of(0),
      point_count: Uint32Array.of(2),
    },
    points: {
      pin: ["a.q", "c.d"],
      type: [".latch", ".names"],
      edge: ["", "net"],
      block: ["a", "c"],
      net: ["a", ""],
      cluster: Int32Array.of(0, 1),
      x: Int32Array.of(0, 1),
      y: Int32Array.of(0, 1),
      arrival_ns: Float32Array.of(0.5, 2),
      delay_ns: Float32Array.of(0.5, 1.5),
    },
  });
}

const prime = () => {
  setTiming(timing());
  setWholeView(
    "placement",
    makeView("placement", {
      blocks: { name: ["clb_a", "clb_c"], id: Uint32Array.of(0, 1) },
    }),
  );
};

describe("the commands of the model in the views (P7.1)", () => {
  it("selects an object, opens the view of its kind when none is open, and zooms to it", () => {
    applyEvent({
      type: "view_command",
      command: { action: "select", kind: "block", name: "clb_a" },
    });
    expect(selection.value).toEqual([{ kind: "block", name: "clb_a" }]);
    expect(activeTab.value).toBe("panel:floorplan");
    expect(zoomRequest.value?.ref).toEqual({ kind: "block", name: "clb_a" });
    expect(consoleLines.value.map((l) => l.text)).toContain(
      "view › select block clb_a",
    );
  });

  it("keeps the view that is open", async () => {
    await runModelCommand({ action: "open_view", view: "routing" });
    await runModelCommand({ action: "zoom_to", kind: "block", name: "clb_a" });
    expect(tabs.value.map((t) => t.panel)).toEqual(["routing"]);
    expect(selection.value).toEqual([]); // zoom_to does not select
  });

  it("opens the views by the names of their tabs", async () => {
    await runModelCommand({ action: "open_view", view: "floorplan" });
    expect(activeTab.value).toBe("panel:pins");
    await runModelCommand({ action: "open_view", view: "placement" });
    expect(activeTab.value).toBe("panel:floorplan");
    await runModelCommand({ action: "open_view", view: "clusters" });
    expect(activeTab.value).toBe("panel:clusters");
  });

  it("selects a path as its blocks and nets", async () => {
    prime();
    await runModelCommand({ action: "select", kind: "path", name: "0" });
    expect(selection.value).toEqual([
      { kind: "path", name: "0" },
      { kind: "block", name: "clb_a" },
      { kind: "block", name: "clb_c" },
      { kind: "net", name: "a" },
    ]);
    expect(activeTab.value).toBe("panel:timing");
  });

  it("shows the cone of an atom and the atoms of a path in the netlist", async () => {
    await runModelCommand({ action: "show_cone", kind: "atom", name: "a" });
    expect(netlistRequest.value).toMatchObject({ cone: "a" });
    expect(activeTab.value).toBe("panel:netlist");
    prime();
    await runModelCommand({ action: "show_path", kind: "path", name: "0" });
    expect(netlistRequest.value).toMatchObject({
      atoms: ["a", "c"],
      what: "path 1",
    });
    expect(selection.value[0]).toEqual({ kind: "path", name: "0" });
  });

  it("says in the console when a command cannot be done", async () => {
    prime();
    applyEvent({
      type: "view_command",
      command: { action: "show_path", kind: "path", name: "7" },
    });
    await new Promise((resolve) => setTimeout(resolve, 0));
    await new Promise((resolve) => setTimeout(resolve, 0));
    expect(consoleLines.value.map((l) => l.text).join("\n")).toContain(
      "show_path failed: The timing has no path 8",
    );
  });

  it("clears the selection", async () => {
    selection.value = [{ kind: "net", name: "n" }];
    await runModelCommand({ action: "clear" });
    expect(selection.value).toEqual([]);
  });
});
