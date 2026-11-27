// The layout of the netlist views (docs/plan_design_views.md, 3.5): ELK's layered algorithm, in a
// Web Worker in the page (in the thread in the tests), on the graph of a scope. Each view (an
// owner) has its engine: a new layout of a view stops the one of the same view that still runs,
// never the one of another view. A layout is kept by the signature of its graph, until the data
// of the session change. An expansion gives the nodes that were shown their places as hints, keeps
// the order of the layers, and is moved as a whole onto the layout before it, so that what was
// shown stays where it was as far as the layout allows.
import type { ELK, ElkExtendedEdge, ElkNode } from "elkjs/lib/elk-api";
import type { Graph } from "./netgraph";

export interface Box {
  x: number;
  y: number;
  w: number;
  h: number;
}

/** Positions in the coordinates of the layout: x to the right, y down */
export interface Layout {
  signature: string;
  nodes: Map<string, Box>;
  /** The points of each edge, x, y */
  edges: Map<string, number[]>;
  width: number;
  height: number;
  /** How long it took */
  ms: number;
}

/** The space between the layers, and between the nodes of a layer */
export const LAYER_SPACING = 60;
export const NODE_SPACING = 18;

/** The engine of one owner: its ELK, the count of its layouts, and the one that runs (to stop it:
 * a worker that is terminated never answers, so its promise is rejected here) */
interface Engine {
  elk: Promise<ELK> | null;
  inWorker: boolean;
  count: number;
  stop: ((error: Error) => void) | null;
}

/** The owner of the layouts that give none (the tests) */
const SHARED = {};
const engines = new Map<object, Engine>();
/** The most engines kept: the one used the longest ago is closed (its view is likely gone) */
const ENGINES = 4;

function engineOf(owner: object): Engine {
  let engine = engines.get(owner);
  if (engine) {
    engines.delete(owner); // last used: at the end
    engines.set(owner, engine);
    return engine;
  }
  engine = { elk: null, inWorker: false, count: 0, stop: null };
  engines.set(owner, engine);
  if (engines.size > ENGINES) {
    const [oldest, old] = engines.entries().next().value!;
    engines.delete(oldest);
    closeEngine(old);
  }
  return engine;
}

/** Stop what an engine runs, and end its worker */
function closeEngine(engine: Engine) {
  engine.stop?.(new Error("stopped"));
  engine.stop = null;
  const elk = engine.elk;
  engine.elk = null;
  if (engine.inWorker && elk) void elk.then((e) => e.terminateWorker());
}

async function makeEngine(engine: Engine): Promise<ELK> {
  // In the tests, ELK runs in the thread; the page loads only its worker
  if (import.meta.env.MODE !== "test") {
    const [{ default: ElkApi }, { default: workerUrl }] = await Promise.all([
      import("elkjs/lib/elk-api.js"),
      import("elkjs/lib/elk-worker.min.js?url"),
    ]);
    engine.inWorker = true;
    return new ElkApi({ workerUrl });
  }
  const { default: ElkBundled } = await import("elkjs/lib/elk.bundled.js");
  engine.inWorker = false;
  return new ElkBundled();
}

/** The options of the layered algorithm: the faster ones for a large graph (P0 measures: ELK
 * lays out 500 netlist nodes in about 200 ms with them, 2000 in about a second) */
function options(nodes: number, interactive: boolean): Record<string, string> {
  return {
    "elk.algorithm": "layered",
    "elk.direction": "RIGHT",
    "elk.layered.thoroughness": "1",
    "elk.spacing.nodeNode": String(NODE_SPACING),
    "elk.layered.spacing.nodeNodeBetweenLayers": String(LAYER_SPACING),
    "elk.layered.spacing.edgeNodeBetweenLayers": "12",
    ...(nodes > 400
      ? {
          "elk.edgeRouting": "POLYLINE",
          "elk.layered.nodePlacement.strategy": "SIMPLE",
        }
      : { "elk.edgeRouting": "ORTHOGONAL" }),
    ...(interactive
      ? {
          "elk.layered.layering.strategy": "INTERACTIVE",
          "elk.layered.crossingMinimization.strategy": "INTERACTIVE",
          "elk.layered.cycleBreaking.strategy": "INTERACTIVE",
        }
      : {}),
  };
}

const kept = new Map<string, Layout>();

/** The layout of a graph. hint gives a place to a node (where it was, or where the group it came
 * out of was): with hints the layout keeps the order of what was shown, and is moved onto the
 * nodes that wasShown. owner is the view that asks: a layout is rejected with "stopped" when the
 * same owner starts another before it ends, and only then */
export async function layoutGraph(
  graph: Graph,
  hint?: (id: string) => { x: number; y: number } | undefined,
  wasShown: (id: string) => boolean = () => true,
  owner: object = SHARED,
): Promise<Layout> {
  const found = kept.get(graph.signature);
  if (found && !hint) return found;
  const engine = engineOf(owner);
  const mine = ++engine.count;
  // A layout of this owner still runs, and nobody waits for it any more: stop it (a worker that
  // is idle is kept, as starting one again loads ELK again)
  if (engine.stop) closeEngine(engine);
  engine.elk ??= makeEngine(engine);
  const elk = await engine.elk;
  if (mine !== engine.count) throw new Error("stopped");
  const hints = new Map<string, { x: number; y: number }>();
  for (const n of graph.nodes) {
    const h = hint?.(n.id);
    if (h) hints.set(n.id, h);
  }
  const interactive = hints.size > 0;
  const input: ElkNode = {
    id: "root",
    layoutOptions: options(graph.nodes.length, interactive),
    children: graph.nodes.map((n) => ({
      id: n.id,
      width: n.width,
      height: n.height,
      ...(hints.get(n.id) ?? {}),
    })),
    edges: graph.edges.map((e): ElkExtendedEdge => ({
      id: e.id,
      sources: [e.source],
      targets: [e.target],
    })),
  };
  const start = performance.now();
  let out: ElkNode;
  try {
    out = await new Promise<ElkNode>((resolve, reject) => {
      engine.stop = reject;
      elk.layout(input).then(resolve, reject);
    });
  } catch (error) {
    if (mine !== engine.count) throw new Error("stopped");
    throw error;
  } finally {
    if (mine === engine.count) engine.stop = null;
  }
  if (mine !== engine.count) throw new Error("stopped");
  const nodes = new Map<string, Box>();
  for (const c of out.children ?? [])
    nodes.set(c.id, {
      x: c.x ?? 0,
      y: c.y ?? 0,
      w: c.width ?? 0,
      h: c.height ?? 0,
    });
  const edges = new Map<string, number[]>();
  for (const e of (out.edges ?? []) as ElkExtendedEdge[]) {
    const points: number[] = [];
    for (const s of e.sections ?? []) {
      points.push(s.startPoint.x, s.startPoint.y);
      for (const b of s.bendPoints ?? []) points.push(b.x, b.y);
      points.push(s.endPoint.x, s.endPoint.y);
    }
    edges.set(e.id, points);
  }
  // Onto the layout before: the move of the nodes that were shown, on average, is undone
  if (interactive) {
    let dx = 0;
    let dy = 0;
    let count = 0;
    for (const [id, h] of hints) {
      const b = nodes.get(id);
      if (!b || !wasShown(id)) continue;
      dx += h.x - b.x;
      dy += h.y - b.y;
      count++;
    }
    if (count) {
      dx /= count;
      dy /= count;
      for (const b of nodes.values()) {
        b.x += dx;
        b.y += dy;
      }
      for (const points of edges.values())
        for (let i = 0; i < points.length; i += 2) {
          points[i] += dx;
          points[i + 1] += dy;
        }
    }
  }
  const layout: Layout = {
    signature: graph.signature,
    nodes,
    edges,
    width: out.width ?? 0,
    height: out.height ?? 0,
    ms: performance.now() - start,
  };
  kept.set(graph.signature, layout);
  if (kept.size > 24) kept.delete(kept.keys().next().value!);
  return layout;
}

/** The most that a node of both layouts moved from one to the other */
export function largestMove(before: Layout, after: Layout): number {
  let most = 0;
  for (const [id, a] of before.nodes) {
    const b = after.nodes.get(id);
    if (b) most = Math.max(most, Math.hypot(b.x - a.x, b.y - a.y));
  }
  return most;
}

/** The box of a layout: xmin, ymin, xmax, ymax */
export function layoutBox(layout: Layout): [number, number, number, number] {
  let box: [number, number, number, number] = [
    Infinity,
    Infinity,
    -Infinity,
    -Infinity,
  ];
  for (const b of layout.nodes.values())
    box = [
      Math.min(box[0], b.x),
      Math.min(box[1], b.y),
      Math.max(box[2], b.x + b.w),
      Math.max(box[3], b.y + b.h),
    ];
  return Number.isFinite(box[0]) ? box : [0, 0, 1, 1];
}

/** Forget the layouts that are kept: the data of the session changed, and a graph of the same
 * nodes may be another one */
export function forgetLayouts() {
  kept.clear();
}
