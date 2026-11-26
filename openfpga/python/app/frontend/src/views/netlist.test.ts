import { beforeEach, describe, expect, it } from "vitest";
import { resetStore } from "../store";
import { resetViewData, setWholeView } from "./data";
import { makeView } from "./format";
import {
  forgetLayouts,
  largestMove,
  layoutGraph,
  LAYER_SPACING,
} from "./layout";
import {
  FANOUT_LIMIT,
  type Graph,
  type Netlist,
  SCOPE_LIMIT,
  buildGraph,
  clusterGrouping,
  cone,
  moduleGrouping,
  netlistOf,
} from "./netgraph";
import {
  type NetlistBase,
  clustersView,
  netlistView,
  resetNetlistRequests,
  showInNetlist,
} from "./netlist";
import type { RectsLayer } from "./scene";
import { resetSelection, selection } from "./selection";
import { TileCache } from "./tiles";
import type { ViewData } from "./format";

beforeEach(() => {
  resetStore();
  resetSelection();
  resetViewData();
  resetNetlistRequests();
  forgetLayouts();
});

/** in0 -> x1 (u_a) -> x2, x3 (u_a.u_b) -> q (u_c) -> out; x3 is guessed */
function atoms(): ViewData {
  // pins: atom, dir (0 in, 1 out), net
  const pins: [number, number, number][] = [
    [0, 1, 0], // in0 drives n_in
    [1, 0, 0],
    [1, 1, 1], // x1: n_in -> n1
    [2, 0, 1],
    [2, 1, 2], // x2: n1 -> n2
    [3, 0, 1],
    [3, 0, 2],
    [3, 1, 3], // x3: n1, n2 -> n3
    [4, 0, 3],
    [4, 1, 4], // q: n3 -> n4
    [5, 0, 4], // out
  ];
  const pinStart = [0, 1, 3, 5, 8, 10];
  return makeView("atoms", {
    models: { name: [".input", ".names", ".latch", ".output"] },
    modules: { name: ["", "u_a", "u_a.u_b", "u_c"] },
    atoms: {
      name: ["in0", "u_a.x1", "u_a.u_b.x2", "$abc$7$x3", "u_c.q", "out:out"],
      model: Uint16Array.of(0, 1, 1, 1, 2, 3),
      module: Uint32Array.of(0, 1, 2, 2, 3, 0),
      how: Uint8Array.of(3, 0, 0, 2, 0, 3),
      cluster: Int32Array.of(0, 1, 1, 1, 2, 3),
      pin_start: Uint32Array.from(pinStart),
      pin_count: Uint32Array.of(1, 2, 2, 3, 2, 1),
    },
    pins: {
      atom: Uint32Array.from(pins, (p) => p[0]),
      port: pins.map(() => "p"),
      bit: new Uint16Array(pins.length),
      dir: Uint8Array.from(pins, (p) => p[1]),
      net: Uint32Array.from(pins, (p) => p[2]),
    },
    nets: {
      name: ["n_in", "n1", "n2", "n3", "n4"],
      driver: Int32Array.of(0, 2, 4, 7, 9),
      pins: Uint32Array.of(2, 3, 2, 2, 2),
      inside: Uint32Array.of(2, 3, 2, 2, 2),
    },
  });
}

function clusters(): ViewData {
  return makeView("clusters", {
    block_types: { name: ["EMPTY", "io", "clb"] },
    blocks: {
      name: ["in0", "clb_a", "clb_q", "out:out"],
      id: Uint32Array.of(0, 1, 2, 3),
      type: Uint16Array.of(1, 2, 2, 1),
    },
    nets: { name: ["n_in", "n3", "n4"], net: Uint32Array.of(0, 1, 2) },
  });
}

const ids = (g: Graph | { tooBig: number }) =>
  "tooBig" in g ? [] : g.nodes.map((n) => n.id);

describe("the graph of a netlist", () => {
  it("reads the drivers and the sinks of the nets", () => {
    const n = netlistOf(atoms());
    expect(Array.from(n.driver)).toEqual([0, 1, 2, 3, 4]);
    expect(n.sinks).toEqual([[1], [2, 3], [3], [4], [5]]);
    expect(n.module[2]).toBe("u_a.u_b");
  });

  it("shows a module collapsed as one box, with the nets that cross its edge as its ports", () => {
    const n = netlistOf(atoms());
    const g = buildGraph(n, moduleGrouping(n), new Set(), null) as Graph;
    expect(ids(g)).toEqual(["a:0", "a:5", "g:u_a", "g:u_c"]);
    // n_in comes in, n3 goes out; n1 and n2 are inside
    expect(g.nodes.find((x) => x.id === "g:u_a")!.ports).toBe(2);
    expect(g.edges.map((e) => `${e.source}>${e.target}`)).toEqual([
      "a:0>g:u_a",
      "g:u_a>g:u_c",
      "g:u_c>a:5",
    ]);
  });

  it("expands a module into its atoms and the modules in it", () => {
    const n = netlistOf(atoms());
    const g = buildGraph(n, moduleGrouping(n), new Set(["u_a"]), null) as Graph;
    expect(ids(g)).toEqual(["a:0", "a:1", "a:5", "g:u_a.u_b", "g:u_c"]);
    expect(g.nodes.find((x) => x.id === "g:u_a.u_b")!.ports).toBe(2); // n1 in, n3 out
    const all = buildGraph(
      n,
      moduleGrouping(n),
      new Set(["u_a", "u_a.u_b", "u_c"]),
      null,
    );
    expect(ids(all)).toEqual(["a:0", "a:1", "a:2", "a:3", "a:4", "a:5"]);
  });

  it("groups the atoms by their clusters", () => {
    const n = netlistOf(atoms());
    const names = new Map([
      [0, "in0"],
      [1, "clb_a"],
      [2, "clb_q"],
      [3, "out:out"],
    ]);
    const g = buildGraph(
      n,
      clusterGrouping(n, names),
      new Set(),
      null,
    ) as Graph;
    expect(ids(g)).toEqual(["g:clb_a", "g:clb_q", "g:in0", "g:out:out"]);
  });

  it("draws a net of a large fanout to a stub with its count", () => {
    const sinks = FANOUT_LIMIT + 4;
    const n: Netlist = {
      atoms: ["d", ...Array.from({ length: sinks }, (_, i) => `s${i}`)],
      model: [],
      module: Array(sinks + 1).fill(""),
      how: new Uint8Array(sinks + 1),
      cluster: new Int32Array(sinks + 1),
      nets: ["clk"],
      driver: Int32Array.of(0),
      sinks: [Array.from({ length: sinks }, (_, i) => i + 1)],
      netsOf: [[0], ...Array.from({ length: sinks }, () => [0])],
      partial: new Uint8Array(1),
      atomIndex: new Map(),
    };
    const g = buildGraph(n, () => [], new Set(), null) as Graph;
    const stub = g.nodes.find((x) => x.kind === "stub")!;
    expect(stub.label).toBe(`clk (${sinks})`);
    expect(g.edges).toHaveLength(1);
  });

  it("refuses a graph of more nodes than the limit", () => {
    const count = SCOPE_LIMIT + 1;
    const n: Netlist = {
      atoms: Array.from({ length: count }, (_, i) => `a${i}`),
      model: [],
      module: Array(count).fill(""),
      how: new Uint8Array(count),
      cluster: new Int32Array(count),
      nets: [],
      driver: new Int32Array(0),
      sinks: [],
      netsOf: Array.from({ length: count }, () => []),
      partial: new Uint8Array(0),
      atomIndex: new Map(),
    };
    expect(buildGraph(n, () => [], new Set(), null)).toEqual({ tooBig: count });
  });

  it("finds the cone of an atom, a level at a time", () => {
    const n = netlistOf(atoms());
    expect([...cone(n, [2], 1)].sort()).toEqual([1, 2, 3]);
    expect([...cone(n, [2], 2)].sort()).toEqual([0, 1, 2, 3, 4]);
  });
});

/** A netlist-like graph: levels of nodes, each fed by 2 to 4 nodes of the one or two before */
function levelled(count: number): Graph {
  let seed = 7;
  const random = () =>
    (seed = (seed * 1103515245 + 12345) & 0x7fffffff) / 0x7fffffff;
  const per = Math.ceil(count / 10);
  const nodes = Array.from({ length: count }, (_, i) => ({
    id: `a:${i}`,
    kind: "atom" as const,
    label: `${i}`,
    atoms: [i],
    ports: 0,
    width: 90,
    height: 28,
  }));
  const edges = [];
  for (let i = per; i < count; i++) {
    const level = Math.floor(i / per);
    for (let k = 0; k < 2 + Math.floor(random() * 3); k++) {
      const from =
        Math.max(0, level - (random() < 0.8 ? 1 : 2)) * per +
        Math.floor(random() * per);
      if (from < count)
        edges.push({
          id: `${i}:${k}`,
          net: i,
          source: `a:${from}`,
          target: `a:${i}`,
        });
    }
  }
  return { nodes, edges, signature: `levelled ${count}` };
}

describe("the layout", () => {
  it("places every node and routes every edge", async () => {
    const n = netlistOf(atoms());
    const g = buildGraph(n, moduleGrouping(n), new Set(), null) as Graph;
    const layout = await layoutGraph(g);
    expect([...layout.nodes.keys()].sort()).toEqual(ids(g));
    for (const e of g.edges)
      expect(layout.edges.get(e.id)!.length).toBeGreaterThanOrEqual(4);
    // Left to right: the input before the module before the output
    expect(layout.nodes.get("a:0")!.x).toBeLessThan(
      layout.nodes.get("g:u_a")!.x,
    );
    expect(layout.nodes.get("g:u_c")!.x).toBeLessThan(
      layout.nodes.get("a:5")!.x,
    );
  });

  it("lays out a cone of 500 atoms in under a second (P6)", async () => {
    await layoutGraph(levelled(100)); // the first one loads ELK
    const layout = await layoutGraph(levelled(500));
    console.log(`500 nodes laid out in ${Math.round(layout.ms)} ms`);
    expect(layout.ms).toBeLessThan(1000);
  });

  it("keeps the nodes that were shown in their order when a module expands", async () => {
    const n = netlistOf(atoms());
    const group = moduleGrouping(n);
    const before = await layoutGraph(
      buildGraph(n, group, new Set(), null) as Graph,
    );
    const expanded = buildGraph(n, group, new Set(["u_a"]), null) as Graph;
    const after = await layoutGraph(
      expanded,
      (id) => before.nodes.get(id) ?? before.nodes.get("g:u_a"),
      (id) => before.nodes.has(id),
    );
    // The input stays left of the output, and u_c right of what came out of u_a
    expect(after.nodes.get("a:0")!.x).toBeLessThan(after.nodes.get("a:1")!.x);
    expect(after.nodes.get("a:1")!.x).toBeLessThan(after.nodes.get("g:u_c")!.x);
    const moved = largestMove(before, after);
    console.log(
      `expanding u_a moved a node that was shown by ${Math.round(moved)} (layer spacing ${LAYER_SPACING})`,
    );
  });
});

const context = (
  base: NetlistBase,
  hidden = new Set(["slack"]),
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
    dark: false,
    background: [255, 255, 255, 255] as [number, number, number, number],
  },
  hidden,
  changed,
});

/** Build, wait for the layout, build again */
async function laidOut(def: typeof netlistView, base: NetlistBase) {
  let done!: () => void;
  const came = new Promise<void>((resolve) => (done = resolve));
  const first = def.build(context(base, undefined, () => done()));
  expect(first.status).toContain("laying out");
  await came;
  return def.build(context(base));
}

describe("the netlist view (V1)", () => {
  beforeEach(() => {
    setWholeView("atoms", atoms());
    setWholeView("clusters", clusters());
  });

  it("shows the modules collapsed, and fits the first layout", async () => {
    const base = await netlistView.load();
    const built = await laidOut(netlistView, base);
    const groups = built.scene.layers.find(
      (l) => l.id === "groups",
    ) as RectsLayer;
    expect([groups.name!(0), groups.name!(1)]).toEqual(["u_a", "u_c"]);
    expect(built.refit).toBe(true);
    expect(built.status).toContain("4 nodes, 3 edges");
    expect(netlistView.describe(base, { kind: "module", name: "u_a" })).toBe(
      "module u_a: 3 atoms, 2 ports",
    );
    expect(built.legend!.map((e) => e.label)).toContain(
      "Module (double click: expand)",
    );
  });

  it("expands a module on a double click and collapses it on another", async () => {
    const base = await netlistView.load();
    await laidOut(netlistView, base);
    expect(
      netlistView.activate!(
        base,
        { kind: "module", name: "u_a" },
        () => undefined,
      ),
    ).toBe(true);
    let built = await laidOut(netlistView, base);
    const atomsLayer = built.scene.layers.find(
      (l) => l.id === "atoms",
    ) as RectsLayer;
    expect(atomsLayer.x.length).toBe(3); // in0, x1, out
    expect(built.scene.layers.find((l) => l.id === "groups")!).toMatchObject({
      x: expect.objectContaining({ length: 2 }), // u_a.u_b and u_c
    });
    netlistView.activate!(
      base,
      { kind: "module", name: "u_a" },
      () => undefined,
    );
    built = await laidOut(netlistView, base);
    expect(
      (built.scene.layers.find((l) => l.id === "atoms") as RectsLayer).x.length,
    ).toBe(2);
  });

  it("shows the cone of an atom on a double click, one level more each time", async () => {
    const base = await netlistView.load();
    netlistView.activate!(
      base,
      { kind: "atom", name: "u_a.u_b.x2" },
      () => undefined,
    );
    expect(base.scopeText).toBe("a cone (3 atoms)");
    netlistView.activate!(
      base,
      { kind: "atom", name: "u_c.q" },
      () => undefined,
    );
    expect(base.scope!.size).toBe(5); // and q, out
    const built = await laidOut(netlistView, base);
    // The guessed module of x3 is marked
    expect(built.legend!.map((e) => e.label)).toContain("Module guessed");
    expect(
      netlistView.tools!(base, () => undefined).map((t) => t.label),
    ).toContain("Whole design");
  });

  it("opens a module of the hierarchy, and the atoms of a path", async () => {
    const base = await netlistView.load();
    showInNetlist({ module: "u_a.u_b" });
    await laidOut(netlistView, base);
    expect(base.expanded.has("u_a")).toBe(true);
    expect(
      netlistView.locate(
        base,
        { kind: "module", name: "u_a.u_b" },
        () => undefined,
      ),
    ).not.toBeNull();
    showInNetlist({ atoms: ["in0", "u_a.x1", "u_c.q"], what: "path 1" });
    const built = await laidOut(netlistView, base);
    expect(built.status).toContain("path 1");
    expect(
      (built.scene.layers.find((l) => l.id === "atoms") as RectsLayer).x.length,
    ).toBe(3);
  });

  it("says a module of the hierarchy has no atom", async () => {
    const base = await netlistView.load();
    showInNetlist({ module: "u_gone" });
    const built = netlistView.build(context(base));
    expect(built.status).toContain("No atom is in u_gone");
  });

  it("draws the selected atoms and nets over the graph", async () => {
    const base = await netlistView.load();
    netlistView.activate!(
      base,
      { kind: "module", name: "u_a" },
      () => undefined,
    );
    await laidOut(netlistView, base);
    selection.value = [
      { kind: "atom", name: "u_a.x1" },
      { kind: "net", name: "n_in" },
    ];
    const built = netlistView.build(context(base));
    expect(built.scene.layers.some((l) => l.id === "selection:nodes")).toBe(
      true,
    );
    expect(built.scene.layers.some((l) => l.id === "selection:nets")).toBe(
      true,
    );
  });
});

describe("the clusters view (V2)", () => {
  beforeEach(() => {
    setWholeView("atoms", atoms());
    setWholeView("clusters", clusters());
  });

  it("shows the clusters, and a cluster opened into its atoms", async () => {
    const base = await clustersView.load();
    let built = await laidOut(clustersView, base);
    const groups = built.scene.layers.find(
      (l) => l.id === "groups",
    ) as RectsLayer;
    expect(groups.x.length).toBe(4);
    expect(groups.objectKind).toBe("block"); // the selection of the placement
    expect(clustersView.describe(base, { kind: "block", name: "clb_a" })).toBe(
      "cluster clb_a (clb): 3 atoms, 2 nets out",
    );
    clustersView.activate!(
      base,
      { kind: "block", name: "clb_a" },
      () => undefined,
    );
    built = await laidOut(clustersView, base);
    expect(
      (built.scene.layers.find((l) => l.id === "atoms") as RectsLayer).x.length,
    ).toBe(3);
    clustersView.activate!(
      base,
      { kind: "block", name: "clb_a" },
      () => undefined,
    );
    built = await laidOut(clustersView, base);
    expect(
      (built.scene.layers.find((l) => l.id === "atoms") as RectsLayer).x.length,
    ).toBe(0);
  });

  it("narrows to the selection and its neighbours", async () => {
    const base = await clustersView.load();
    selection.value = [{ kind: "block", name: "clb_q" }];
    const around = clustersView.tools!(base, () => undefined).find(
      (t) => t.label === "Around the selection",
    )!;
    around.run();
    // clb_q, clb_a (n3) and out:out (n4), whole
    expect(base.scope!.size).toBe(5);
  });
});
