import { render, screen } from "@testing-library/preact";
import { beforeEach, describe, expect, it } from "vitest";
import { Drawing, kindOf, regionOccupancy } from "./components/Drawings";
import { ResultView } from "./components/Workspace";
import { calls, resetStore, tabs } from "./store";

const ok = (data: Record<string, unknown>) => ({ ok: true, data, truncated: false, more: null }) as never;

const utilization = ok({
  schema_version: 1,
  kind: "utilization",
  stage: "PACKED",
  device_utilization: 0.375,
  grid: { width: 4, height: 4 },
  block_types: [
    { name: "io", is_io: true, used: 3, available: 16, utilization: 0.1875 },
    { name: "clb", is_io: false, used: 3, available: 4, utilization: 0.75 },
  ],
  limiting_resources: ["clb"],
});

const placement = ok({
  schema_version: 1,
  kind: "placement",
  stage: "PLACED",
  grid: { width: 4, height: 4 },
  filter: { type: null, prefix: null },
  total: 5,
  returned: 4,
  truncated: true,
  blocks: [
    { name: "a", type: "io", x: 0, y: 1, sub_tile: 0, layer: 0 },
    { name: "b", type: "io", x: 0, y: 2, sub_tile: 0, layer: 0 },
    { name: "c", type: "clb", x: 1, y: 1, sub_tile: 0, layer: 0 },
    { name: "d", type: "clb", x: 2, y: 2, sub_tile: 0, layer: 0 },
  ],
});

const usage = (occupancy: number) => ({ used: Math.round(occupancy * 10), capacity: 10, occupancy });
const congestion = ok({
  schema_version: 1,
  kind: "congestion",
  stage: "ROUTED",
  grid: { width: 4, height: 4 },
  channel_width: { x: 10, y: 10 },
  routed: true,
  region_size: 2,
  summary: { chanx: usage(0.4), chany: usage(0.3), overused_nodes: 2 },
  regions: [
    { x_low: 0, y_low: 0, x_high: 1, y_high: 1, chanx: usage(0.2), chany: usage(0.1) },
    { x_low: 2, y_low: 0, x_high: 3, y_high: 1, chanx: usage(0.5), chany: usage(0.9) },
    { x_low: 0, y_low: 2, x_high: 1, y_high: 3, chanx: usage(1.2), chany: usage(0.3) },
    { x_low: 2, y_low: 2, x_high: 3, y_high: 3, chanx: usage(0), chany: usage(0) },
  ],
});

beforeEach(() => resetStore());

describe("the drawings of the results", () => {
  it("knows the kinds that have a drawing, and gives none to a failed answer or another kind", () => {
    expect(kindOf(utilization)).toBe("utilization");
    expect(kindOf(placement)).toBe("placement");
    expect(kindOf(congestion)).toBe("congestion");
    expect(kindOf(ok({ kind: "timing_summary" }))).toBeNull();
    expect(kindOf({ ok: false, diagnostics: [] } as never)).toBeNull();
    const { container } = render(<Drawing result={{ ok: false, diagnostics: [] } as never} />);
    expect(container.innerHTML).toBe("");
  });

  it("draws a bar for each type of block with the numbers of the answer, and marks the limiting one", () => {
    const { container } = render(<Drawing result={utilization} />);
    expect(container.querySelectorAll(".bar-row")).toHaveLength(2);
    expect(screen.getByText("3 / 4 (75%)")).toBeTruthy();
    expect(container.querySelector('[data-block-type="clb"]')?.className).toContain("limiting");
    expect(container.querySelector('[data-block-type="io"]')?.className).not.toContain("limiting");
    expect((container.querySelector('[data-block-type="clb"] .bar-fill') as HTMLElement).style.width).toBe("75%");
    expect(screen.getByText(/Device utilization 37.5%/)).toBeTruthy();
  });

  it("draws a square for each block of the answer, at its place, and says that the answer holds a part", () => {
    const { container } = render(<Drawing result={placement} />);
    expect(container.querySelectorAll("rect.block")).toHaveLength(4); // the number of blocks of the answer
    const c = container.querySelector('[data-block="c"]') as SVGRectElement;
    const d = container.querySelector('[data-block="d"]') as SVGRectElement;
    expect(Number(d.getAttribute("x"))).toBeGreaterThan(Number(c.getAttribute("x")));
    expect(Number(d.getAttribute("y"))).toBeLessThan(Number(c.getAttribute("y"))); // y grows upward
    expect(screen.getByText(/4 of 5 blocks on a 4 × 4 grid \(the answer holds a part of them\)/)).toBeTruthy();
    expect(container.querySelector('[data-block="c"] title')?.textContent).toBe("c (clb) at 1, 1");
  });

  it("cuts a tile that holds several blocks in slices, so that none is hidden behind another", () => {
    const shared = ok({
      kind: "placement",
      grid: { width: 4, height: 4 },
      total: 2,
      truncated: false,
      blocks: [
        { name: "p", type: "io", x: 0, y: 1, sub_tile: 0, layer: 0 },
        { name: "q", type: "io", x: 0, y: 1, sub_tile: 1, layer: 0 },
      ],
    });
    const { container } = render(<Drawing result={shared} />);
    const p = container.querySelector('[data-block="p"]') as SVGRectElement;
    const q = container.querySelector('[data-block="q"]') as SVGRectElement;
    expect(Number(q.getAttribute("x"))).toBeGreaterThan(Number(p.getAttribute("x")));
    expect(Number(p.getAttribute("width"))).toBeLessThan(Number(p.getAttribute("height")));
  });

  it("draws a square for each region, and the fullest region is the one that the data says", () => {
    const { container } = render(<Drawing result={congestion} />);
    const regions = [...container.querySelectorAll("rect.region")];
    expect(regions).toHaveLength(4);
    const fullest = regions.reduce((a, b) =>
      Number(b.getAttribute("data-occupancy")) > Number(a.getAttribute("data-occupancy")) ? b : a,
    );
    expect(fullest.getAttribute("data-occupancy")).toBe("1.2"); // the region with chanx 1.2
    expect(fullest.getAttribute("fill")).toBe("#7f0000"); // more than full: dark red
    expect(regions.find((r) => r.getAttribute("data-occupancy") === "0")?.getAttribute("fill")).toBe("hsl(120, 70%, 45%)");
    expect(screen.getByText(/4 regions · fullest 120% · overused nodes 2/)).toBeTruthy();
  });

  it("takes the larger of the two channels of a region", () => {
    expect(regionOccupancy({ x_low: 0, y_low: 0, x_high: 0, y_high: 0, chanx: usage(0.2), chany: usage(0.7) })).toBe(0.7);
  });
});

describe("a result tab", () => {
  const show = (result: unknown, tool = "get_utilization") => {
    calls.value = { c1: { callId: "c1", tool, arguments: {}, state: "done", seconds: 0.1, truncated: false, result: result as never } };
    tabs.value = [{ id: "c1", title: tool, pinned: false, unread: false }];
    return render(<ResultView callId="c1" />);
  };

  it("draws the kinds that have a drawing, with the table of numbers under it", () => {
    const { container } = show(utilization);
    expect(container.querySelector(".drawing-utilization")).not.toBeNull();
    expect(screen.getByText(/Drawn from the data that the model read/)).toBeTruthy();
  });

  it("says that another kind has no drawing yet, and shows the JSON when asked", () => {
    const { container } = show(ok({ kind: "timing_summary", setup: { fmax_mhz: 100 } }), "get_timing");
    expect(container.querySelector(".drawing")).toBeNull();
    expect(screen.getByText(/has no drawing yet/)).toBeTruthy();
  });
});
