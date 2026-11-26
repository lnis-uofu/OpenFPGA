// V1, the netlist, and V2, the clustered netlist (docs/plan_design_views.md, section 4 and 3.7): the
// atoms of a scope laid out by ELK, grouped by the modules of the RTL (V1) or by the clusters (V2).
// A double click on a group expands or collapses it; on an atom of V1, it shows its cone (a scope
// of the atoms around it), one level further each time. The scope is never more than SCOPE_LIMIT
// nodes: an expansion over it is refused and says so.
import { signal } from "@preact/signals";
import { type ViewData, numbers, texts } from "./format";
import { wholeView } from "./data";
import { type Layout, layoutBox, layoutGraph } from "./layout";
import {
  type Graph,
  type Grouping,
  type Netlist,
  SCOPE_LIMIT,
  buildGraph,
  clusterGrouping,
  cone,
  moduleGrouping,
  netlistOf,
} from "./netgraph";
import { categoryColor } from "./palette";
import type {
  Layer,
  LabelsLayer,
  ObjectRef,
  PolylinesLayer,
  RectsLayer,
  Rgba,
} from "./scene";
import { selection } from "./selection";
import {
  selectedPath,
  slackColor,
  slackLegend,
  slackRange,
  timingNow,
} from "./timing";
import {
  type BuildContext,
  type Built,
  type LegendEntry,
  StyleTable,
  type ViewDef,
  type ViewTool,
} from "./view";

export type NetlistKind = "atoms" | "clusters";

export interface NetlistBase {
  which: NetlistKind;
  netlist: Netlist;
  /** The clusters export, when the design is packed: the names of the clusters, the ids of nets */
  clusters: ViewData | null;
  clusterName: Map<number, string>;
  clusterType: Map<string, string>;
  /** The groups that are expanded; flat: no groups (a cone, a path) */
  expanded: Set<string>;
  flat: boolean;
  /** The atoms shown (null: all), and what they are */
  scope: Set<number> | null;
  scopeText: string;
  /** A change of the expansions or of the scope: a new graph */
  revision: number;
  graph?: { revision: number; graph: Graph | { tooBig: number } };
  /** The graph and the layout that are drawn, and the revision that a layout is asked for */
  shown: { graph: Graph; layout: Layout; revision: number } | null;
  asked: number;
  refit: boolean;
  /** What was refused or went wrong, for the status */
  message: string;
  /** How long the last layout took */
  ms: number;
}

/** A request of another part of the page to the netlist (V1): show a module, or atoms (a path) */
export const netlistRequest = signal<{
  seq: number;
  module?: string;
  atoms?: string[];
  what?: string;
} | null>(null);
let requestSeq = 0;
export function showInNetlist(request: {
  module?: string;
  atoms?: string[];
  what?: string;
}) {
  netlistRequest.value = { ...request, seq: ++requestSeq };
}

/** The colour of an atom by its model */
function modelFill(model: string, dark: boolean): Rgba {
  const index =
    model === ".names"
      ? 0
      : model === ".latch"
        ? 1
        : model === ".input" || model === ".output"
          ? 2
          : 3;
  return categoryColor([0, 3, 1, 4][index], dark, 255);
}
const MODELS: [string, string][] = [
  [".names", "LUT (.names)"],
  [".latch", "Flip-flop (.latch)"],
  [".input", "Input or output"],
  ["other", "Other (.subckt)"],
];

const SELECTED: Rgba = [250, 80, 40, 255];
const GUESS: Rgba = [230, 150, 20, 255];

async function loadNetlist(which: NetlistKind): Promise<NetlistBase> {
  const atoms = await wholeView("atoms");
  let clusters: ViewData | null = null;
  try {
    clusters = await wholeView("clusters");
  } catch (error) {
    if (which === "clusters") throw error;
  }
  const clusterName = new Map<number, string>();
  const clusterType = new Map<string, string>();
  if (clusters) {
    const id = numbers(clusters, "blocks", "id");
    const name = texts(clusters, "blocks", "name");
    const type = numbers(clusters, "blocks", "type");
    const types = texts(clusters, "block_types", "name");
    for (let i = 0; i < id.length; i++) {
      clusterName.set(id[i], name.get(i));
      clusterType.set(name.get(i), types.get(type[i]));
    }
  }
  return {
    which,
    netlist: netlistOf(atoms),
    clusters,
    clusterName,
    clusterType,
    expanded: new Set(),
    flat: false,
    scope: null,
    scopeText: "the whole design",
    revision: 1,
    shown: null,
    asked: 0,
    refit: true,
    message: "",
    ms: 0,
  };
}

function grouping(base: NetlistBase): Grouping {
  if (base.flat) return () => [];
  return base.which === "atoms"
    ? moduleGrouping(base.netlist)
    : clusterGrouping(base.netlist, base.clusterName);
}

function groupLabel(base: NetlistBase) {
  return (key: string, atoms: number[]) =>
    base.which === "atoms"
      ? `${key.split(".").pop()} (${atoms.length})`
      : `${key.length > 16 ? `${key.slice(0, 15)}…` : key}`;
}

/** The graph of the state now (made again only when it changed) */
function graphOf(base: NetlistBase): Graph | { tooBig: number } {
  if (base.graph?.revision !== base.revision)
    base.graph = {
      revision: base.revision,
      graph: buildGraph(
        base.netlist,
        grouping(base),
        base.expanded,
        base.scope,
        groupLabel(base),
      ),
    };
  return base.graph.graph;
}

/** A change of the state; undone if the graph would have too many nodes */
function change(base: NetlistBase, what: string, apply: () => void): boolean {
  const saved = {
    expanded: new Set(base.expanded),
    flat: base.flat,
    scope: base.scope,
    scopeText: base.scopeText,
  };
  apply();
  base.revision++;
  const graph = graphOf(base);
  if ("tooBig" in graph) {
    Object.assign(base, saved);
    base.revision++;
    base.message = `${what} would show ${graph.tooBig} nodes, more than ${SCOPE_LIMIT}: narrow it (a cone, a module inside it)`;
    return false;
  }
  base.message = "";
  return true;
}

/** Where a node of the new graph starts: where it was, or where the group it came out of was, or
 * the first node of a group that it collapses */
function hintOf(base: NetlistBase, graph: Graph) {
  const before = base.shown?.layout;
  if (!before) return undefined;
  const group = grouping(base);
  const byId = new Map(graph.nodes.map((n) => [n.id, n]));
  return (id: string) => {
    const was = before.nodes.get(id);
    if (was) return { x: was.x, y: was.y };
    const node = byId.get(id);
    if (!node) return undefined;
    for (const a of node.atoms) {
      // A group it came out of
      for (const key of group(a)) {
        const b = before.nodes.get(`g:${key}`);
        if (b) return { x: b.x, y: b.y };
      }
      // An atom it collapses
      const b = before.nodes.get(`a:${a}`);
      if (b) return { x: b.x, y: b.y };
      const outer =
        base.which === "atoms" ? base.netlist.module[a].split(".") : [];
      for (let k = outer.length; k > 0; k--) {
        const g = before.nodes.get(`g:${outer.slice(0, k).join(".")}`);
        if (g) return { x: g.x, y: g.y };
      }
    }
    return undefined;
  };
}

/** The key of the group of a ref that is a group of this view */
function groupKey(base: NetlistBase, ref: ObjectRef): string | null {
  if (base.which === "atoms" && ref.kind === "module") return ref.name;
  if (base.which === "clusters" && ref.kind === "block") return ref.name;
  return null;
}

/** The atoms of a ref (an atom, a cluster, a module, a net, a path) */
function atomsOf(
  base: NetlistBase,
  ref: ObjectRef,
  changed: () => void,
): number[] {
  const n = base.netlist;
  switch (ref.kind) {
    case "atom": {
      const a = n.atomIndex.get(ref.name);
      return a === undefined ? [] : [a];
    }
    case "block": {
      const found: number[] = [];
      for (const [id, name] of base.clusterName)
        if (name === ref.name)
          n.cluster.forEach((c, a) => {
            if (c === id) found.push(a);
          });
      return found;
    }
    case "module":
      return n.module.flatMap((m, a) =>
        m === ref.name || m.startsWith(`${ref.name}.`) ? [a] : [],
      );
    case "net": {
      const net = n.nets.indexOf(ref.name);
      if (net < 0) return [];
      return [n.driver[net], ...n.sinks[net]].filter((a) => a >= 0);
    }
    case "path": {
      const path = selectedPath([ref], timingNow(changed));
      if (!path) return [];
      return path.points.flatMap((p) => {
        const a = n.atomIndex.get(p.atom);
        return a === undefined ? [] : [a];
      });
    }
    default:
      return [];
  }
}

/** The boxes (in the world, y up) of the nodes that show some atoms */
function boxesOf(
  base: NetlistBase,
  atoms: number[],
): [number, number, number, number][] {
  const shown = base.shown;
  if (!shown) return [];
  const ids = new Set<string>();
  const group = grouping(base);
  for (const a of atoms) {
    const id = `a:${a}`;
    if (shown.layout.nodes.has(id)) ids.add(id);
    else
      for (const key of group(a))
        if (shown.layout.nodes.has(`g:${key}`)) ids.add(`g:${key}`);
  }
  return [...ids].map((id) => worldBox(shown.layout.nodes.get(id)!));
}

const worldBox = (b: {
  x: number;
  y: number;
  w: number;
  h: number;
}): [number, number, number, number] => [b.x, -(b.y + b.h), b.x + b.w, -b.y];

function union(
  boxes: [number, number, number, number][],
): [number, number, number, number] | null {
  if (!boxes.length) return null;
  return [
    Math.min(...boxes.map((b) => b[0])) - 10,
    Math.min(...boxes.map((b) => b[1])) - 10,
    Math.max(...boxes.map((b) => b[2])) + 10,
    Math.max(...boxes.map((b) => b[3])) + 10,
  ];
}

/** A request of the page (a module of the hierarchy, the atoms of a path), applied once */
let handled = 0;
function applyRequest(base: NetlistBase) {
  const request = netlistRequest.value;
  if (base.which !== "atoms" || !request || request.seq <= handled) return;
  handled = request.seq;
  if (request.module !== undefined) {
    const module = request.module;
    change(base, `The module ${module}`, () => {
      base.flat = false;
      base.scope = null;
      base.scopeText = "the whole design";
      // Its parents expanded, so that it shows
      const parts = module.split(".");
      for (let k = 1; k < parts.length; k++)
        base.expanded.add(parts.slice(0, k).join("."));
    });
    if (
      !base.netlist.module.some(
        (m) => m === module || m.startsWith(`${module}.`),
      )
    )
      base.message = `No atom is in ${module}: the synthesis put its logic into its neighbours`;
  } else if (request.atoms) {
    const found = request.atoms.flatMap((name) => {
      const a = base.netlist.atomIndex.get(name);
      return a === undefined ? [] : [a];
    });
    change(base, request.what ?? "The atoms", () => {
      base.flat = true;
      base.scope = new Set(found);
      base.scopeText = request.what ?? `${found.length} atoms`;
    });
  }
  base.refit = true;
}

function makeScene(context: BuildContext<NetlistBase>): Built {
  const { base, colors, hidden, selection, changed } = context;
  const dark = colors.dark;
  applyRequest(base);
  const graph = graphOf(base);
  let status = "";
  if ("tooBig" in graph) {
    status = `${base.scopeText}: ${graph.tooBig} nodes, more than ${SCOPE_LIMIT}. Select objects (in this or another view) and use "Around the selection", or expand less`;
  } else if (
    base.shown?.revision !== base.revision &&
    base.asked !== base.revision
  ) {
    const revision = base.revision;
    base.asked = revision;
    const before = base.shown;
    const hint = hintOf(base, graph);
    layoutGraph(graph, hint, (id) => !!before?.layout.nodes.has(id)).then(
      (layout) => {
        if (base.revision !== revision) return;
        base.shown = { graph, layout, revision };
        base.ms = layout.ms;
        changed();
      },
      (error) => {
        if (String(error?.message ?? error) === "stopped") return;
        base.message = `The layout failed: ${error?.message ?? error}`;
        changed();
      },
    );
  }
  const shown = base.shown;
  const styles = new StyleTable();
  const layers: Layer[] = [];
  const legend: LegendEntry[] = [];
  const timing = hidden.has("slack") ? undefined : timingNow(changed);
  if (shown) {
    const { graph: g, layout } = shown;
    const n = base.netlist;
    const edgeStroke: Rgba = dark ? [120, 125, 140, 255] : [130, 130, 125, 255];
    // The slack of the nets, by their names (the clusters give the index of each)
    let netSlack: ((net: string) => number) | null = null;
    let range: [number, number] = [0, 0];
    if (timing && base.clusters) {
      const slack = numbers(timing, "nets", "slack_ns");
      range = slackRange(slack);
      const names = texts(base.clusters, "nets", "name");
      const ids = numbers(base.clusters, "nets", "net");
      const byName = new Map<string, number>();
      for (let i = 0; i < ids.length; i++) byName.set(names.get(i), ids[i]);
      netSlack = (net) => {
        const id = byName.get(net);
        return id === undefined ? Infinity : slack[id];
      };
    }
    // The edges
    const xy: number[] = [];
    const start: number[] = [0];
    const edgeNet: string[] = [];
    const edgeStyle: number[] = [];
    for (const e of g.edges) {
      const points = layout.edges.get(e.id);
      if (!points?.length) continue;
      for (let i = 0; i < points.length; i += 2)
        xy.push(points[i], -points[i + 1]);
      start.push(xy.length / 2);
      const name = n.nets[e.net];
      edgeNet.push(name);
      const stroke = netSlack
        ? slackColor(netSlack(name), range, dark)
        : edgeStroke;
      edgeStyle.push(
        styles.add(`edge:${stroke.join(",")}`, { stroke, width: 1.2 }),
      );
    }
    const edges: PolylinesLayer = {
      id: "edges",
      kind: "polylines",
      xy: Float32Array.from(xy),
      start: Uint32Array.from(start),
      style: Uint16Array.from(edgeStyle),
      objectKind: "net",
      name: (i) => edgeNet[i],
      size: 1,
    };
    // The nodes
    const groupKind = base.which === "atoms" ? "module" : "block";
    const make = (kind: string) => ({
      x: [] as number[],
      y: [] as number[],
      w: [] as number[],
      h: [] as number[],
      style: [] as number[],
      name: [] as string[],
      kind,
    });
    const groups = make("groups");
    const atoms = make("atoms");
    const stubs = make("stubs");
    const lx: number[] = [];
    const ly: number[] = [];
    const ltext: string[] = [];
    for (const node of g.nodes) {
      const b = layout.nodes.get(node.id);
      if (!b) continue;
      const [x0, y0, x1, y1] = worldBox(b);
      const into =
        node.kind === "group" ? groups : node.kind === "atom" ? atoms : stubs;
      into.x.push(x0);
      into.y.push(y0);
      into.w.push(x1 - x0);
      into.h.push(y1 - y0);
      lx.push((x0 + x1) / 2);
      ly.push((y0 + y1) / 2);
      ltext.push(node.label);
      if (node.kind === "group") {
        const key = node.id.slice(2);
        into.name.push(key);
        into.style.push(
          styles.add("group", {
            fill: dark ? [52, 58, 74, 255] : [228, 234, 246, 255],
            stroke: dark ? [110, 125, 160, 255] : [120, 140, 190, 255],
            width: 1.5,
          }),
        );
      } else if (node.kind === "atom") {
        const a = node.atoms[0];
        into.name.push(n.atoms[a]);
        const model = n.model[a];
        const known = [".names", ".latch", ".input", ".output"].includes(model)
          ? model
          : "other";
        const guess = base.which === "atoms" && n.how[a] === 2;
        into.style.push(
          styles.add(`atom:${known}:${guess}`, {
            fill: modelFill(model, dark),
            stroke: guess
              ? GUESS
              : dark
                ? [20, 20, 20, 255]
                : [70, 70, 70, 255],
            width: guess ? 2 : 1,
          }),
        );
      } else {
        into.name.push(n.nets[Number(node.id.slice(2))]);
        into.style.push(
          styles.add("stub", {
            fill: dark ? [70, 70, 70, 255] : [240, 240, 235, 255],
            stroke: edgeStroke,
            width: 1,
          }),
        );
      }
    }
    const rectsOf = (
      r: ReturnType<typeof make>,
      objectKind: RectsLayer["objectKind"],
      id: string,
    ): RectsLayer => ({
      id,
      kind: "rects",
      x: Float32Array.from(r.x),
      y: Float32Array.from(r.y),
      w: Float32Array.from(r.w),
      h: Float32Array.from(r.h),
      style: Uint16Array.from(r.style),
      objectKind,
      name: (i) => r.name[i],
      size: 60,
      minPx: 0,
    });
    layers.push(edges);
    layers.push(rectsOf(groups, groupKind, "groups"));
    layers.push(rectsOf(atoms, "atom", "atoms"));
    layers.push(rectsOf(stubs, "net", "stubs"));
    if (!hidden.has("labels")) {
      const label = styles.add("label", {
        fill: dark ? [235, 235, 235, 255] : [20, 20, 20, 255],
      });
      const labels: LabelsLayer = {
        id: "labels",
        kind: "labels",
        x: Float32Array.from(lx),
        y: Float32Array.from(ly),
        text: (i) => ltext[i],
        count: ltext.length,
        px: 11,
        style: new Uint16Array(ltext.length).fill(label),
        size: 90,
        minPx: 55,
        inert: true,
      };
      layers.push(labels);
    }
    // The selection: the nodes of what is selected, and its nets over all
    const chosenNets = new Set(
      selection.filter((r) => r.kind === "net").map((r) => r.name),
    );
    const path = selection.find((r) => r.kind === "path");
    const chosenAtoms = selection
      .filter((r) => r.kind !== "net" && r.kind !== "path")
      .flatMap((r) => atomsOf(base, r, changed));
    if (path) chosenAtoms.push(...atomsOf(base, path, changed));
    const boxes = boxesOf(base, chosenAtoms);
    const selXy: number[] = [];
    const selStart: number[] = [0];
    const selNet: string[] = [];
    g.edges.forEach((e) => {
      const name = n.nets[e.net];
      if (!chosenNets.has(name)) return;
      const points = layout.edges.get(e.id);
      if (!points?.length) return;
      for (let i = 0; i < points.length; i += 2)
        selXy.push(points[i], -points[i + 1]);
      selStart.push(selXy.length / 2);
      selNet.push(name);
    });
    const sel = styles.add("selected", { stroke: SELECTED, width: 3 });
    if (selNet.length)
      layers.push({
        id: "selection:nets",
        kind: "polylines",
        xy: Float32Array.from(selXy),
        start: Uint32Array.from(selStart),
        style: new Uint16Array(selNet.length).fill(sel),
        name: (i) => selNet[i],
        size: 1,
        inert: true,
      });
    if (boxes.length)
      layers.push({
        id: "selection:nodes",
        kind: "rects",
        x: Float32Array.from(boxes, (b) => b[0] - 3),
        y: Float32Array.from(boxes, (b) => b[1] - 3),
        w: Float32Array.from(boxes, (b) => b[2] - b[0] + 6),
        h: Float32Array.from(boxes, (b) => b[3] - b[1] + 6),
        style: new Uint16Array(boxes.length).fill(sel),
        size: 60,
        inert: true,
      });
    // The legend
    if (groups.x.length)
      legend.push({
        label:
          base.which === "atoms"
            ? "Module (double click: expand)"
            : "Cluster (double click: expand)",
        ...styles.get("group"),
      });
    const models = new Set(
      atoms.name.map((name) => n.model[n.atomIndex.get(name)!]),
    );
    for (const [model, label] of MODELS)
      if (
        [...models].some((m) =>
          model === "other"
            ? ![".names", ".latch", ".input", ".output"].includes(m)
            : m === model || (model === ".input" && m === ".output"),
        )
      )
        legend.push({
          label,
          fill: modelFill(model === "other" ? ".subckt" : model, dark),
        });
    if (
      base.which === "atoms" &&
      atoms.style.some((s) => styles.list[s].stroke === GUESS)
    )
      legend.push({ label: "Module guessed", stroke: GUESS });
    if (stubs.x.length)
      legend.push({ label: "Net of a large fanout", ...styles.get("stub") });
    if (netSlack) legend.push(slackLegend(range));
    else legend.push({ label: "Net", stroke: edgeStroke, line: true });
    if (selection.length) legend.push({ label: "Selected", stroke: SELECTED });
  }
  const box = shown
    ? layoutBox(shown.layout)
    : ([0, 0, 1, 1] as [number, number, number, number]);
  const bounds: [number, number, number, number] = [
    box[0],
    -box[3],
    box[2],
    -box[1],
  ];
  if (!status)
    status = [
      base.scopeText,
      shown
        ? `${shown.graph.nodes.length} nodes, ${shown.graph.edges.length} edges`
        : "",
      shown?.revision !== base.revision
        ? "laying out…"
        : `laid out in ${Math.round(base.ms)} ms`,
      base.message,
      !hidden.has("slack") && !timing ? "analyzing the timing" : "",
    ]
      .filter(Boolean)
      .join(" · ");
  const refit = !!shown && shown.revision === base.revision && base.refit;
  if (refit) base.refit = false;
  return {
    scene: { layers, bounds, styles: styles.list },
    status,
    legend,
    refit,
  };
}

function makeDef(which: NetlistKind): ViewDef<NetlistBase> {
  return {
    needs: which === "atoms" ? "DEVICE" : "PACKED",
    load: () => loadNetlist(which),
    bounds: (base) => {
      if (!base.shown) return [0, 0, 1, 1];
      const b = layoutBox(base.shown.layout);
      return [b[0], -b[3], b[2], -b[1]];
    },
    groups: [
      { id: "labels", label: "Names" },
      { id: "slack", label: "Slack", off: true },
    ],
    key: (base, _view, hidden) =>
      `${base.revision}|${base.shown?.revision ?? 0}|${netlistRequest.value?.seq ?? 0}|${[...hidden].sort().join(",")}`,
    build: makeScene,
    locate(base, ref, changed) {
      if (!base.shown || base.shown.revision !== base.revision) return null;
      return union(boxesOf(base, atomsOf(base, ref, changed)));
    },
    describe(base, ref) {
      const n = base.netlist;
      if (ref.kind === "atom") {
        const a = n.atomIndex.get(ref.name);
        if (a === undefined) return `atom ${ref.name}`;
        const how =
          ["", "", ", its module guessed", ", in no module"][n.how[a]] ?? "";
        const cluster = base.clusterName.get(n.cluster[a]);
        return `${ref.name} (${n.model[a]}) in ${n.module[a] || "the top"}${how}${cluster ? `, cluster ${cluster}` : ""}`;
      }
      if (ref.kind === "module") {
        const node = base.shown?.graph.nodes.find(
          (g) => g.id === `g:${ref.name}`,
        );
        return `module ${ref.name}: ${atomsOf(base, ref, () => undefined).length} atoms${node ? `, ${node.ports} ports` : ""}`;
      }
      if (ref.kind === "block") {
        const node = base.shown?.graph.nodes.find(
          (g) => g.id === `g:${ref.name}`,
        );
        const type = base.clusterType.get(ref.name);
        return `cluster ${ref.name}${type ? ` (${type})` : ""}: ${atomsOf(base, ref, () => undefined).length} atoms${node ? `, ${node.ports} nets out` : ""}`;
      }
      if (ref.kind === "net") {
        const net = n.nets.indexOf(ref.name);
        if (net < 0) return `net ${ref.name}`;
        const d = n.driver[net];
        return `net ${ref.name}: from ${d >= 0 ? n.atoms[d] : "outside"} to ${n.sinks[net].length} pins`;
      }
      return `${ref.kind} ${ref.name}`;
    },
    activate(base, ref, changed) {
      const key = groupKey(base, ref);
      if (key !== null) {
        const open = base.expanded.has(key);
        change(base, `Expanding ${key}`, () => {
          if (open) {
            for (const k of [...base.expanded])
              if (k === key || k.startsWith(`${key}.`)) base.expanded.delete(k);
          } else base.expanded.add(key);
        });
        changed();
        return true;
      }
      if (base.which === "atoms" && ref.kind === "atom") {
        const a = base.netlist.atomIndex.get(ref.name);
        if (a === undefined) return false;
        // Its cone: one level more each time
        change(base, `The cone of ${ref.name}`, () => {
          const around = cone(base.netlist, [a], 1);
          if (base.flat && base.scope)
            for (const b of base.scope) around.add(b);
          else base.refit = true;
          base.flat = true;
          base.scope = around;
          base.scopeText = `a cone (${around.size} atoms)`;
        });
        changed();
        return true;
      }
      return false;
    },
    tools(base, changed): ViewTool[] {
      const tools: ViewTool[] = [];
      if (base.scope || base.flat)
        tools.push({
          label: "Whole design",
          title: "Back to the whole design, grouped",
          run: () => {
            change(base, "The whole design", () => {
              base.flat = false;
              base.scope = null;
              base.scopeText = "the whole design";
            });
            base.refit = true;
            changed();
          },
        });
      tools.push({
        label: "Around the selection",
        title:
          "The atoms of what is selected (here or in another view) and those one net away",
        run: () => {
          const from = selectionAtoms(base, changed);
          if (!from.length) {
            base.message = "Select something first";
            changed();
            return;
          }
          change(base, "Around the selection", () => {
            const around = cone(base.netlist, from, 1);
            if (base.which === "clusters") {
              // Whole clusters: the clusters of those atoms
              const clusters = new Set(
                [...around].map((a) => base.netlist.cluster[a]),
              );
              base.netlist.cluster.forEach((c, a) => {
                if (clusters.has(c)) around.add(a);
              });
            } else base.flat = true;
            base.scope = around;
            base.scopeText = `around the selection (${around.size} atoms)`;
          });
          base.refit = true;
          changed();
        },
      });
      if (base.expanded.size)
        tools.push({
          label: "Collapse all",
          title: "Every group collapsed again",
          run: () => {
            change(base, "Collapsing", () => base.expanded.clear());
            changed();
          },
        });
      return tools;
    },
  };
}

function selectionAtoms(base: NetlistBase, changed: () => void): number[] {
  return selection.value.flatMap((r) => atomsOf(base, r, changed));
}

export const netlistView = makeDef("atoms");
export const clustersView = makeDef("clusters");

/** For the tests */
export function resetNetlistRequests() {
  netlistRequest.value = null;
  handled = 0;
}
