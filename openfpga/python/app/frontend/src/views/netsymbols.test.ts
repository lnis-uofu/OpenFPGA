import { beforeEach, describe, expect, it } from "vitest";
import { resetStore } from "../store";
import { resetViewData, setWholeView } from "./data";
import { type ViewData, makeView } from "./format";
import { forgetLayouts, setPinsLimit, PINS_LIMIT } from "./layout";
import type { Graph } from "./netgraph";
import {
  type NetlistBase,
  clustersView,
  netlistView,
  resetNetlistRequests,
} from "./netlist";
import type {
  PointsLayer,
  PolylinesLayer,
  RectsLayer,
  Scene,
  ShapesLayer,
} from "./scene";
import { resetSelection, selection } from "./selection";
import { TileCache } from "./tiles";
import { setSymbolsSource, setUsePackageSymbols } from "./symbols";

/** in0 (.input) -> x1 (.names, 2 inputs) -> q (.latch) -> out (.output); clk (.input) -> q.
 * Clusters: io_in0, clb (x1, q), io_out, io_clk */
function atoms(): ViewData {
  // atom, port, bit, dir (0 in, 1 out, 2 clock), net
  const pins: [number, string, number, number, number][] = [
    [0, "inpad", 0, 1, 0], // in0 -> n_in
    [1, "in", 0, 0, 0],
    [1, "in", 1, 0, 0],
    [1, "out", 0, 1, 1], // x1 -> n1
    [2, "D", 0, 0, 1],
    [2, "clk", 0, 2, 3],
    [2, "Q", 0, 1, 2], // q -> n_q
    [3, "outpad", 0, 0, 2],
    [4, "inpad", 0, 1, 3], // clk -> n_clk
  ];
  return makeView("atoms", {
    models: { name: [".input", ".names", ".latch", ".output"] },
    modules: { name: [""] },
    atoms: {
      name: ["in0", "x1", "q", "out:out", "clk"],
      model: Uint16Array.of(0, 1, 2, 3, 0),
      module: new Uint32Array(5),
      how: Uint8Array.of(3, 0, 0, 3, 3),
      cluster: Int32Array.of(0, 1, 1, 2, 3),
      pin_start: Uint32Array.of(0, 1, 4, 7, 8),
      pin_count: Uint32Array.of(1, 3, 3, 1, 1),
    },
    pins: {
      atom: Uint32Array.from(pins, (p) => p[0]),
      port: pins.map((p) => p[1]),
      bit: Uint16Array.from(pins, (p) => p[2]),
      dir: Uint8Array.from(pins, (p) => p[3]),
      net: Uint32Array.from(pins, (p) => p[4]),
    },
    nets: {
      name: ["n_in", "n1", "n_q", "n_clk"],
      driver: Int32Array.of(0, 3, 6, 8),
      pins: Uint32Array.of(3, 2, 2, 2),
      inside: Uint32Array.of(3, 2, 2, 2),
      id: Uint32Array.of(0, 1, 2, 3),
    },
  });
}

/** The clusters, with their pins (the clb: I[0] from n_in, clk, O[3] to n_q) */
function clusters(withPins = true): ViewData {
  // block, port, bit, dir, net (the cluster nets: 0 n_in, 1 n_q, 2 n_clk)
  const pins: [number, string, number, number, number][] = [
    [0, "inpad", 0, 1, 0],
    [1, "I", 0, 0, 0],
    [1, "clk", 0, 2, 2],
    [1, "O", 3, 1, 1],
    [2, "outpad", 0, 0, 1],
    [3, "inpad", 0, 1, 2],
  ];
  return makeView("clusters", {
    block_types: { name: ["EMPTY", "io", "clb"] },
    blocks: {
      name: ["in0", "clb_a", "out:out", "clk"],
      id: Uint32Array.of(0, 1, 2, 3),
      type: Uint16Array.of(1, 2, 1, 1),
    },
    nets: { name: ["n_in", "n_q", "n_clk"], net: Uint32Array.of(0, 1, 2) },
    ...(withPins
      ? {
          pins: {
            block: Uint32Array.from(pins, (p) => p[0]),
            port: pins.map((p) => p[1]),
            bit: Uint16Array.from(pins, (p) => p[2]),
            dir: Uint8Array.from(pins, (p) => p[3]),
            net: Uint32Array.from(pins, (p) => p[4]),
          },
        }
      : {}),
  });
}

const SVG = (body: string, root = "") =>
  `<svg xmlns="http://www.w3.org/2000/svg" xmlns:s="https://github.com/nturley/netlistsvg">${root}${body}</svg>`;

const context = (
  base: NetlistBase,
  dark = false,
  changed: () => void = () => undefined,
) => ({
  base,
  tiles: new TileCache(
    () => undefined,
    () => new Promise<ViewData>(() => undefined),
  ),
  view: { cx: 0, cy: 0, scale: 1, width: 800, height: 600 },
  selection: selection.value,
  highlighted: [],
  colors: {
    dark,
    background: [255, 255, 255, 255] as [number, number, number, number],
  },
  hidden: new Set(["slack"]),
  changed,
});

async function laidOut(
  def: typeof netlistView,
  base: NetlistBase,
  dark = false,
) {
  let done!: () => void;
  const came = new Promise<void>((resolve) => (done = resolve));
  def.build(context(base, dark, () => done()));
  await came;
  return def.build(context(base, dark));
}

const layer = <T>(scene: Scene, id: string) =>
  scene.layers.find((l) => l.id === id) as T;
const graphOf = (base: NetlistBase) => base.graph!.graph as Graph;

beforeEach(() => {
  resetStore();
  resetSelection();
  resetViewData();
  resetNetlistRequests();
  forgetLayouts();
  setPinsLimit(PINS_LIMIT);
  setSymbolsSource(async () => null);
  setWholeView("atoms", atoms());
  setWholeView("clusters", clusters());
});

describe("the netlist symbols in V1 (phase 2)", () => {
  it("draws each atom with the symbol of its model", async () => {
    const base = await netlistView.load();
    await laidOut(netlistView, base);
    const symbolOf = Object.fromEntries(
      graphOf(base).nodes.map((n) => [n.label, n.symbol]),
    );
    expect(symbolOf).toEqual({
      in0: "atom_input",
      x1: "atom_lut",
      q: "atom_ff",
      "out:out": "atom_output",
      clk: "atom_input",
    });
  });

  it("puts the pins on the ports of the symbols, bit by bit", async () => {
    const base = await netlistView.load();
    await laidOut(netlistView, base);
    const node = (label: string) =>
      graphOf(base).nodes.find((n) => n.label === label)!;
    const at = (label: string) =>
      node(label).pins!.map((p) => `${p.label}@${p.x},${p.y}:${p.side}`);
    // in[k] each 8 down from 6; D, clk and Q where the flip-flop has them
    expect(at("x1")).toEqual([
      "in[0]@0,6:left",
      "in[1]@0,14:left",
      "out[0]@60,14:right",
    ]);
    expect(at("q")).toEqual([
      "D[0]@0,9:left",
      "clk[0]@0,27:left",
      "Q[0]@50,9:right",
    ]);
    expect(base.unnamedPins).toBe(0);
  });

  it("routes each edge from the pin that drives it to the pin of its sink", async () => {
    const base = await netlistView.load();
    const built = await laidOut(netlistView, base);
    const g = graphOf(base);
    const layout = base.shown!.layout;
    expect(g.edges.every((e) => e.sourcePort && e.targetPort)).toBe(true);
    // n_in goes to both inputs of the LUT: two edges
    expect(g.edges.filter((e) => e.net === 0)).toHaveLength(2);
    for (const e of g.edges) {
      const points = layout.edges.get(e.id)!;
      const from = layout.ports.get(e.sourcePort!)!;
      const to = layout.ports.get(e.targetPort!)!;
      expect(Math.hypot(points[0] - from.x, points[1] - from.y)).toBeLessThan(
        1,
      );
      const n = points.length;
      expect(
        Math.hypot(points[n - 2] - to.x, points[n - 1] - to.y),
      ).toBeLessThan(1);
    }
    // The pins are drawn, and picked as pins
    const pins = layer<PointsLayer>(built.scene, "pins");
    expect(pins.objectKind).toBe("pin");
    expect(pins.x.length).toBe(9);
    expect(built.legend!.map((e) => e.label)).toContain("Pin");
  });

  it("draws the symbols over their size on the screen, and their boxes under it", async () => {
    const base = await netlistView.load();
    const built = await laidOut(netlistView, base);
    const shapes = layer<ShapesLayer>(built.scene, "symbols:24:shapes");
    const boxes = layer<RectsLayer>(built.scene, "symbols:24:boxes");
    const lines = layer<PolylinesLayer>(built.scene, "symbols:24:lines");
    expect(shapes.minPx).toBe(24);
    expect(boxes.maxPx).toBe(24);
    expect(boxes.x.length).toBe(5);
    // The LUT: its body and its folded corner; the flip-flop: its body and its clock wedge
    expect(shapes.x.length).toBeGreaterThanOrEqual(5);
    expect(lines.start.length - 1).toBeGreaterThanOrEqual(2);
    // The atoms are still picked by their boxes, drawn by nothing
    const atomsLayer = layer<RectsLayer>(built.scene, "atoms");
    expect(atomsLayer.x.length).toBe(5);
    expect(built.scene.styles[atomsLayer.style[0]]).toEqual({});
    // Their names, from the texts of the symbols
    const labels = layer<{ count: number; text(i: number): string }>(
      built.scene,
      "labels",
    );
    const texts = Array.from({ length: labels.count }, (_, i) =>
      labels.text(i),
    );
    expect(texts).toEqual(
      expect.arrayContaining(["in0", "x1", "q", "out:out", "clk"]),
    );
    expect(built.legend!.map((e) => e.label)).toContain("Symbols: default");
  });

  it("colours the roles in the light and the dark theme", async () => {
    const base = await netlistView.load();
    const light = await laidOut(netlistView, base);
    const dark = netlistView.build(context(base, true));
    const stroke = (built: typeof light) => {
      const shapes = layer<ShapesLayer>(built.scene, "symbols:24:shapes");
      return built.scene.styles[shapes.style[0]].stroke;
    };
    // outline: #464646 light, #141414 dark (the default file)
    expect(stroke(light)).toEqual([70, 70, 70, 255]);
    expect(stroke(dark)).toEqual([20, 20, 20, 255]);
  });

  it("tells what a pin is, and selects its net", async () => {
    const base = await netlistView.load();
    await laidOut(netlistView, base);
    const pin = [...base.pinInfo].find(([, p]) => p.label === "D[0]")![0];
    expect(netlistView.describe(base, { kind: "pin", name: pin })).toBe(
      "pin D[0] of q: net n1",
    );
    selection.value = [{ kind: "pin", name: pin }];
    const built = netlistView.build(context(base));
    const chosen = layer<PolylinesLayer>(built.scene, "selection:nets");
    expect(chosen.name!(0)).toBe("n1");
  });

  it("draws no pins over the limit of nodes, and says so", async () => {
    setPinsLimit(2);
    const base = await netlistView.load();
    const built = await laidOut(netlistView, base);
    expect(layer(built.scene, "pins")).toBeUndefined();
    expect(base.shown!.layout.ports.size).toBe(0);
    expect(built.status).toContain("no pins over 2 nodes");
  });
});

describe("the symbols of a package (phase 4)", () => {
  it("replaces, hides and adds symbols, and says so in the legend", async () => {
    setSymbolsSource(async () => ({
      name: "my_pkg",
      text: SVG(
        `<g s:type="atom_lut" s:width="40" s:height="40">
           <s:alias val="atom:.names"/>
           <s:color role="body" light="#ffeedd" dark="#332211"/>
           <rect width="40" height="40"/>
           <g s:pid="in" s:x="0" s:y="10" s:step="20"/>
           <g s:pid="out" s:x="40" s:y="20"/>
         </g>
         <g s:type="atom_ff" s:hide="true"/>
         <g s:type="atom_generic" s:hide="true"/>`,
      ),
    }));
    const base = await netlistView.load();
    const built = await laidOut(netlistView, base);
    const node = (label: string) =>
      graphOf(base).nodes.find((n) => n.label === label)!;
    // Replaced: its ports and its colour
    expect(node("x1").pins!.map((p) => `${p.x},${p.y}`)).toEqual([
      "0,10",
      "0,30",
      "40,20",
    ]);
    const boxes = layer<RectsLayer>(built.scene, "symbols:24:boxes");
    const at = Array.from({ length: boxes.x.length }, (_, i) => i).find(
      (i) => boxes.name!(i) === "x1",
    )!;
    expect(built.scene.styles[boxes.style[at]].fill).toEqual([
      255, 238, 221, 255,
    ]);
    // Hidden, and the generic symbol too: a plain box, its edges to the node
    expect(node("q").symbol).toBeUndefined();
    expect(node("q").pins).toBeUndefined();
    expect(
      graphOf(base).edges.find((e) => e.target === node("q").id)!.targetPort,
    ).toBeUndefined();
    expect(built.legend!.map((e) => e.label)).toContain(
      "Symbols: default; the package replaces 1, hides 2 (my_pkg)",
    );
  });

  it("leaves the package's symbols out when the setting is off", async () => {
    let asked = 0;
    setSymbolsSource(async () => {
      asked++;
      return {
        name: "my_pkg",
        text: SVG('<g s:type="atom_ff" s:hide="true"/>'),
      };
    });
    setUsePackageSymbols(false);
    try {
      expect(netlistView.reloadKey!()).toBe("false");
      const base = await netlistView.load();
      expect(asked).toBe(0);
      expect(base.symbols.hiddenTypes).toEqual([]);
      expect(window.localStorage.getItem("openfpga-package-symbols")).toBe(
        "off",
      );
    } finally {
      setUsePackageSymbols(true);
    }
    expect(netlistView.reloadKey!()).toBe("true");
    expect((await netlistView.load()).symbols.hiddenTypes).toEqual(["atom_ff"]);
  });

  it("uses the default symbols when the package's file is refused", async () => {
    setSymbolsSource(async () => ({
      name: "bad",
      text: SVG(
        '<g s:type="t" s:width="1" s:height="1"><s:alias val="atom:t"/><script/></g>',
      ),
    }));
    const base = await netlistView.load();
    const built = await laidOut(netlistView, base);
    expect(graphOf(base).nodes.find((n) => n.label === "q")!.symbol).toBe(
      "atom_ff",
    );
    expect(built.legend!.map((e) => e.label)).toContain(
      "the symbols of the package are not used: <script> (line 1): the element script is refused",
    );
  });
});

describe("the pins of the clusters in V2 (phase 3)", () => {
  it("puts the pins of a cluster on its symbol, and the edges on them", async () => {
    const base = await clustersView.load();
    await laidOut(clustersView, base);
    const g = graphOf(base);
    const clb = g.nodes.find((n) => n.id === "g:clb_a")!;
    expect(clb.symbol).toBe("cluster_clb");
    // No port of the clb symbol is named: by the rule, inputs left and outputs right, 8 apart
    expect(clb.pins!.map((p) => `${p.label}@${p.x},${p.y}:${p.side}`)).toEqual([
      "I[0]@0,8:left",
      "clk[0]@0,16:left",
      "O[3]@120,8:right",
    ]);
    expect(base.unnamedPins).toBe(3 + 3); // the clb, and the pads that name no inpad port
    const layout = base.shown!.layout;
    // n_in from the pad to I[0], n_q from O[3] to the pad
    const into = g.edges.find((e) => e.targetPort === "g:clb_a#0")!;
    expect(g.nodes.find((n) => n.id === into.source)!.id).toBe("g:in0");
    const out = g.edges.find((e) => e.sourcePort === "g:clb_a#2")!;
    expect(out.target).toBe("g:out:out");
    for (const e of [into, out]) {
      const points = layout.edges.get(e.id)!;
      const pin = layout.ports.get(
        e.targetPort === "g:clb_a#0" ? e.targetPort : e.sourcePort!,
      )!;
      const ends = [
        [points[0], points[1]],
        [points[points.length - 2], points[points.length - 1]],
      ];
      expect(ends.some(([x, y]) => Math.hypot(x - pin.x, y - pin.y) < 1)).toBe(
        true,
      );
    }
  });

  it("gives a cluster a port on a side per net when the export has no pins", async () => {
    setWholeView("clusters", clusters(false));
    const base = await clustersView.load();
    await laidOut(clustersView, base);
    const clb = graphOf(base).nodes.find((n) => n.id === "g:clb_a")!;
    expect(clb.pins!.every((p) => p.x === undefined)).toBe(true);
    expect(clb.pins!.map((p) => `${p.label}:${p.side}`).sort()).toEqual([
      "n_clk:left",
      "n_in:left",
      "n_q:right",
    ]);
    // ELK placed them on their sides
    const layout = base.shown!.layout;
    const box = layout.nodes.get("g:clb_a")!;
    for (const p of clb.pins!) {
      const at = layout.ports.get(p.id)!;
      expect(at.x).toBeCloseTo(p.side === "left" ? box.x : box.x + box.w, 0);
    }
  });
});
