import { blockFill } from "./palette";
import { beforeEach, describe, expect, it } from "vitest";
import { resetStore } from "../store";
import {
  resetViewData,
  setDataSignature,
  setViewError,
  setWholeView,
} from "./data";
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
  moduleKeys,
  moduleParts,
  netlistOf,
} from "./netgraph";
import {
  type NetlistBase,
  clusterContents,
  clustersView,
  netlistView,
  resetNetlistRequests,
  showInNetlist,
} from "./netlist";
import type { PolylinesLayer, RectsLayer } from "./scene";
import { resetSelection, selection } from "./selection";
import { TileCache } from "./tiles";
import { setTiming, slackColor } from "./timing";
import type { ViewData } from "./format";
import { atoms, clusters } from "./netlist.fixtures";

beforeEach(() => {
  resetStore();
  resetSelection();
  resetViewData();
  resetNetlistRequests();
  forgetLayouts();
});

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
      netIndex: new Map([["clk", 0]]),
      netId: null,
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
      netIndex: new Map(),
      netId: null,
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
  return { nodes, edges, frames: [], signature: `levelled ${count}` };
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

  it("lays out for two views at once: one never stops the other, only its own next one", async () => {
    const v1 = {};
    const v2 = {};
    const outcome = (p: Promise<unknown>) =>
      p.then(
        () => "done",
        (e: Error) => e.message,
      );
    const both = await Promise.all([
      outcome(layoutGraph(levelled(30), undefined, undefined, v1)),
      outcome(layoutGraph(levelled(40), undefined, undefined, v2)),
    ]);
    expect(both).toEqual(["done", "done"]);
    // The same view: its earlier layout is stopped, the later one is drawn
    forgetLayouts();
    const again = await Promise.all([
      outcome(layoutGraph(levelled(30), undefined, undefined, v1)),
      outcome(layoutGraph(levelled(40), undefined, undefined, v1)),
    ]);
    expect(again).toEqual(["stopped", "done"]);
  });

  it("keeps a layout by its nodes and its edges, until the data of the session change", async () => {
    const n = netlistOf(atoms());
    const g = buildGraph(n, moduleGrouping(n), new Set(), null) as Graph;
    // The same nodes and as many edges, joined otherwise (another design): another graph. The
    // input drives q (in u_c) instead of x1 (in u_a)
    const rebuilt = buildGraph(
      { ...n, sinks: n.sinks.map((s, i) => (i === 0 ? [4] : s)) },
      moduleGrouping(n),
      new Set(),
      null,
    ) as Graph;
    expect(rebuilt.nodes.map((x) => x.id)).toEqual(g.nodes.map((x) => x.id));
    expect(rebuilt.edges.length).toBe(g.edges.length);
    expect(rebuilt.signature).not.toBe(g.signature);
    const first = await layoutGraph(g);
    expect(await layoutGraph(g)).toBe(first); // kept
    setDataSignature("another state of the session");
    expect(await layoutGraph(g)).not.toBe(first); // laid out again
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

describe("two netlist views", () => {
  beforeEach(() => {
    setWholeView("atoms", atoms());
    setWholeView("clusters", clusters());
  });

  it("are both laid out when they are shown at once (the split)", async () => {
    const [v1, v2] = await Promise.all([
      netlistView.load(),
      clustersView.load(),
    ]);
    const [b1, b2] = await Promise.all([
      laidOut(netlistView, v1),
      laidOut(clustersView, v2),
    ]);
    expect(b1.status).toMatch(/laid out in/);
    expect(b2.status).toMatch(/laid out in/);
  });

  it("asks again for a layout that was stopped while it was still wanted", async () => {
    const base = await netlistView.load();
    let rebuilt!: () => void;
    const again = new Promise<void>((resolve) => (rebuilt = resolve));
    const first = netlistView.build(context(base, undefined, () => rebuilt()));
    expect(first.status).toContain("laying out");
    // Something else of the same view stops it (as a worker that is restarted would)
    void layoutGraph(levelled(20), undefined, undefined, base).catch(
      () => undefined,
    );
    await again;
    // The view is made again, asks again, and draws
    const built = await laidOut(netlistView, base);
    expect(built.status).toMatch(/laid out in/);
    expect(base.retries).toBeGreaterThan(0);
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
    // A cluster in the colour of its type of block, as in the placement: io and clb
    const fill = (i: number) =>
      built.scene.styles[groups.style![i]].fill!.join(",");
    const at = (name: string) =>
      Array.from({ length: groups.x.length }, (_, i) => i).find(
        (i) => groups.name!(i) === name,
      )!;
    expect(fill(at("clb_a"))).toBe(fill(at("clb_q")));
    expect(fill(at("clb_a"))).not.toBe(fill(at("in0")));
    expect(fill(at("clb_a"))).toBe(blockFill(2, false).join(","));
    const labels = built.legend!.map((e) => e.label);
    expect(labels).toContain("Clusters (double click: expand)");
    expect(labels).toContain("io");
    expect(labels).toContain("clb");
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

  it("expands and collapses a cluster from the menu of a right click", async () => {
    const base = await clustersView.load();
    const labels = (name: string) =>
      clustersView.menu!(base, { kind: "block", name }, () => undefined).map(
        (t) => t.label,
      );
    expect(labels("clb_a")).toEqual(["Expand in the layout", "Select"]);
    clustersView.menu!(
      base,
      { kind: "block", name: "clb_a" },
      () => undefined,
    )[0].run();
    expect(base.expanded.has("clb_a")).toBe(true);
    expect(labels("clb_a")).toEqual(["Collapse in the layout", "Select"]);
    // An atom of an expanded cluster collapses its cluster
    const atom = clustersView.menu!(
      base,
      { kind: "atom", name: "u_a.x1" },
      () => undefined,
    );
    expect(atom.map((t) => t.label)).toEqual(["Collapse clb_a", "Select"]);
    atom[0].run();
    expect(base.expanded.has("clb_a")).toBe(false);
    // Nothing for a net: the menu of the page
    expect(
      clustersView.menu!(base, { kind: "net", name: "n1" }, () => undefined),
    ).toEqual([]);
  });

  it("lists the clusters with their atoms", async () => {
    const base = await clustersView.load();
    expect(
      clusterContents(base).map((c) => [c.name, c.type, c.atoms.length]),
    ).toEqual([
      ["in0", "io", 1],
      ["clb_a", "clb", 3],
      ["clb_q", "clb", 1],
      ["out:out", "io", 1],
    ]);
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

describe("the fixes of the report on the netlist views", () => {
  beforeEach(() => {
    setWholeView("atoms", atoms());
    setWholeView("clusters", clusters());
  });

  it("keeps the dots of an escaped name of Verilog in one level (B11)", () => {
    expect(moduleParts("u_a.u_b")).toEqual(["u_a", "u_b"]);
    expect(moduleParts("u_a.\\gen.blk[0] .u_c")).toEqual([
      "u_a",
      "\\gen.blk[0] ",
      "u_c",
    ]);
    expect(moduleKeys("u_a.\\gen.blk[0] .u_c")).toEqual([
      "u_a",
      "u_a.\\gen.blk[0] ",
      "u_a.\\gen.blk[0] .u_c",
    ]);
    expect(moduleKeys("")).toEqual([]);
  });

  it("cuts the names of the modules to their boxes (B8)", async () => {
    const view = atoms();
    const names = view.tables.modules.columns.name as unknown as {
      get(i: number): string;
    };
    expect(names.get(1)).toBe("u_a");
    setWholeView(
      "atoms",
      makeView("atoms", {
        ...Object.fromEntries(
          Object.entries(view.tables).map(([t, table]) => [t, table.columns]),
        ),
        modules: {
          name: [
            "",
            "u_a_very_long_module_instance_name",
            "u_a_very_long_module_instance_name.u_b",
            "u_c",
          ],
        },
      } as never),
    );
    const base = await netlistView.load();
    const built = await laidOut(netlistView, base);
    const labels = built.scene.layers.find((l) => l.id === "labels")!;
    const texts = Array.from(
      { length: (labels as { count: number }).count },
      (_, i) => (labels as { text(i: number): string }).text(i),
    );
    const long = texts.find((t) => t.startsWith("u_a_very"))!;
    expect(long).toMatch(/…\ \(3\)$/);
    expect(long.length).toBeLessThanOrEqual(21);
  });

  it("colours the nets inside a cluster by their own slack (B7)", async () => {
    // By the id of each net of the atoms: n1 (id 3) is inside clb_a
    setTiming(
      makeView("timing", {
        nets: {
          slack_ns: Float32Array.of(1, 1, 1),
          criticality: new Float32Array(3),
        },
        atom_nets: { slack_ns: Float32Array.of(1, 3, 0.5, -2, 1) },
      }),
    );
    const base = await clustersView.load();
    clustersView.activate!(
      base,
      { kind: "block", name: "clb_a" },
      () => undefined,
    );
    await laidOut(clustersView, base);
    const built = clustersView.build(context(base, new Set()));
    const edges = built.scene.layers.find(
      (l) => l.id === "edges",
    ) as PolylinesLayer;
    const strokeOf = (net: string) => {
      const i = Array.from(
        { length: edges.start.length - 1 },
        (_, k) => k,
      ).find((k) => edges.name!(k) === net)!;
      return built.scene.styles[edges.style![i]].stroke;
    };
    expect(strokeOf("n1")).toEqual(slackColor(-2, [-2, 3], false));
    expect(strokeOf("n2")).toEqual(slackColor(0.5, [-2, 3], false));
  });

  it("draws an expanded module as a frame around what it holds (B5)", async () => {
    const base = await netlistView.load();
    await laidOut(netlistView, base);
    netlistView.activate!(
      base,
      { kind: "module", name: "u_a" },
      () => undefined,
    );
    netlistView.activate!(
      base,
      { kind: "module", name: "u_a.u_b" },
      () => undefined,
    );
    selection.value = [{ kind: "module", name: "u_a.u_b" }];
    const built = await laidOut(netlistView, base);
    const frames = built.scene.layers.find(
      (l) => l.id === "frames",
    ) as RectsLayer;
    expect(frames.objectKind).toBe("module");
    expect([frames.name!(0), frames.name!(1)]).toEqual(["u_a", "u_a.u_b"]);
    const box = (layer: RectsLayer, i: number) => [
      layer.x[i],
      layer.y[i],
      layer.x[i] + layer.w[i],
      layer.y[i] + layer.h[i],
    ];
    const inside = (b: number[], f: number[]) =>
      b[0] >= f[0] && b[1] >= f[1] && b[2] <= f[2] && b[3] <= f[3];
    // x1, x2 and x3 in u_a; x2 and x3 in u_a.u_b, which is in u_a
    const atomsLayer = built.scene.layers.find(
      (l) => l.id === "atoms",
    ) as RectsLayer;
    const at = (name: string) =>
      box(
        atomsLayer,
        Array.from({ length: atomsLayer.x.length }, (_, i) => i).find(
          (i) => atomsLayer.name!(i) === name,
        )!,
      );
    for (const name of ["u_a.x1", "u_a.u_b.x2", "$abc$7$x3"])
      expect(inside(at(name), box(frames, 0))).toBe(true);
    expect(inside(at("u_a.x1"), box(frames, 1))).toBe(false);
    expect(inside(at("u_a.u_b.x2"), box(frames, 1))).toBe(true);
    expect(inside(box(frames, 1), box(frames, 0))).toBe(true);
    // Its name at its top, and the selection around the frame
    const labels = built.scene.layers.find(
      (l) => l.id === "labels",
    ) as unknown as {
      count: number;
      text(i: number): string;
    };
    const texts = Array.from({ length: labels.count }, (_, i) =>
      labels.text(i),
    );
    expect(texts).toContain("u_b (2)");
    const chosen = built.scene.layers.find(
      (l) => l.id === "selection:nodes",
    ) as RectsLayer;
    expect(chosen.x.length).toBe(1);
    expect(chosen.w[0]).toBeCloseTo(frames.w[1] + 6);
    expect(built.legend!.map((e) => e.label)).toContain(
      "Module expanded (double click: collapse)",
    );
  });

  it("shows a module at a time when the design is over the limit (B9)", async () => {
    setViewError(
      "atoms",
      "The scope has 200000 atoms, more than 100000: give --module or --cone (or a smaller one)",
    );
    const base = await netlistView.load();
    let built = netlistView.build(context(base));
    expect(built.status).toContain("too large to show whole");
    // The clusters view needs the whole netlist
    await expect(clustersView.load()).rejects.toThrow("more than 100000");
    // A module of the hierarchy: its own export, with it expanded
    setWholeView("atoms", atoms(), { module: "u_a" });
    showInNetlist({ module: "u_a" });
    let done!: () => void;
    const came = new Promise<void>((resolve) => (done = resolve));
    netlistView.build(context(base, undefined, () => done()));
    await came;
    expect(base.netlist.atoms.length).toBe(6);
    expect(base.expanded.has("u_a")).toBe(true);
    built = await laidOut(netlistView, base);
    expect(built.status).toContain("the module u_a");
    expect(
      netlistView.tools!(base, () => undefined).map((t) => t.label),
    ).not.toContain("Whole design");
  });
});
