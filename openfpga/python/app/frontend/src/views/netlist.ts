// V1, the netlist, and V2, the clustered netlist (docs/plan_design_views.md, section 4 and 3.7): the
// atoms of a scope laid out by ELK, grouped by the modules of the RTL (V1) or by the clusters (V2).
// A double click on a group expands or collapses it; on an atom of V1, it shows its cone (a scope
// of the atoms around it), one level further each time. The scope is never more than SCOPE_LIMIT
// nodes: an expansion over it is refused and says so.
import { signal } from "@preact/signals";
import { type ViewData, numbers, texts } from "./format";
import { wholeView } from "./data";
import {
  FRAME_PAD,
  type Layout,
  pinsLimitNow,
  layoutBox,
  layoutGraph,
  withPins,
} from "./layout";
import {
  type Graph,
  type GraphNode,
  type Grouping,
  type Netlist,
  type Shaper,
  GROUP_CHARS,
  SCOPE_LIMIT,
  buildGraph,
  clusterGrouping,
  cone,
  cut,
  emptyNetlist,
  moduleGrouping,
  moduleKeys,
  moduleParts,
  netlistOf,
} from "./netgraph";
import { blockFill, categoryColor } from "./palette";
import type {
  Layer,
  LabelsLayer,
  PointsLayer,
  ObjectRef,
  PolylinesLayer,
  RectsLayer,
  Rgba,
} from "./scene";
import { runViewCommand, selection } from "./selection";
import { SymbolDrawer } from "./symboldraw";
import {
  type SymbolSet,
  loadSymbols,
  placePins,
  symbolFor,
  symbolsNote,
  usePackageSymbols,
} from "./symbols";
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
  /** The type of each cluster by its name: its index in the block types of the export */
  clusterTypeIndex: Map<string, number>;
  /** The atoms of each cluster, by its name */
  clusterAtoms: Map<string, number[]>;
  /** The design is over the limit of a whole export (the message of the core): the netlist is
   * the scope that was asked for last (a module, a cone), fetched on its own */
  tooLarge: string;
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
  /** Layouts stopped while still wanted, asked again: part of the key, so that the scene is made */
  retries: number;
  refit: boolean;
  /** What was refused or went wrong, for the status */
  message: string;
  /** How long the last layout took */
  ms: number;
  /** The clusters and their atoms, for the list of V2 (made once) */
  contents?: ClusterContents[];
  /** The netlist symbols (docs/plan_netlist_symbols.md), and the package they come from */
  symbols: SymbolSet;
  packageName: string | null;
  /** The pins of each cluster on a net, from the clusters export (null: an export without them):
   * port, bit, direction (0 in, 1 out, 2 clock), and the net of the atoms (-1 none) */
  clusterPins: Map<
    string,
    { port: string; bit: number; dir: number; net: number }[]
  > | null;
  /** The pins drawn now, by their ids: what they are, for the tooltip and the selection */
  pinInfo: Map<string, { owner: string; label: string; net: number }>;
  /** The pins of the graph placed by the rule, not by a port of their symbol */
  unnamedPins: number;
  /** The modules and their atoms, for the list of V1 (made once for a netlist) */
  hierarchy?: ModuleNode;
}

/** A module of the list of V1: the modules in it, and the atoms that are directly in it */
export interface ModuleNode {
  /** The path of the module ("" the top), the key of its group in the layout */
  key: string;
  /** Its last part */
  label: string;
  children: ModuleNode[];
  /** The atoms directly in it, by their index */
  atoms: number[];
  /** The atoms in it and in the modules below */
  count: number;
}

/** A cluster of the list of V2: its name, its type of block, its atoms */
export interface ClusterContents {
  name: string;
  type: string;
  typeIndex: number | undefined;
  atoms: number[];
}

/** A request of another part of the page to the netlist (V1): show a module, or atoms (a path) */
export const netlistRequest = signal<{
  seq: number;
  module?: string;
  atoms?: string[];
  /** An atom: its cone, two nets away */
  cone?: string;
  what?: string;
} | null>(null);
let requestSeq = 0;
export function showInNetlist(request: {
  module?: string;
  atoms?: string[];
  cone?: string;
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

/** The error of the core for a scope over its limit */
const OVER_LIMIT = /more than \d+/;

async function loadNetlist(which: NetlistKind): Promise<NetlistBase> {
  let netlist: Netlist;
  let tooLarge = "";
  try {
    netlist = netlistOf(await wholeView("atoms"));
  } catch (error) {
    const text = String((error as Error)?.message ?? error);
    // V1 of a large design: a module or a cone at a time
    if (which !== "atoms" || !OVER_LIMIT.test(text)) throw error;
    netlist = emptyNetlist();
    tooLarge = text;
  }
  let clusters: ViewData | null = null;
  try {
    clusters = await wholeView("clusters");
  } catch (error) {
    if (which === "clusters") throw error;
  }
  const clusterName = new Map<number, string>();
  const clusterType = new Map<string, string>();
  const clusterTypeIndex = new Map<string, number>();
  const clusterAtoms = new Map<string, number[]>();
  if (clusters) {
    const id = numbers(clusters, "blocks", "id");
    const name = texts(clusters, "blocks", "name");
    const type = numbers(clusters, "blocks", "type");
    const types = texts(clusters, "block_types", "name");
    for (let i = 0; i < id.length; i++) {
      clusterName.set(id[i], name.get(i));
      clusterType.set(name.get(i), types.get(type[i]));
      clusterTypeIndex.set(name.get(i), type[i]);
    }
  }
  netlist.cluster.forEach((c, a) => {
    const name = clusterName.get(c);
    if (name === undefined) return;
    let list = clusterAtoms.get(name);
    if (!list) clusterAtoms.set(name, (list = []));
    list.push(a);
  });
  // The pins of the clusters, on the nets of the atoms (a net of the clusters has the name of the
  // net of the atoms that it carries)
  let clusterPins: NetlistBase["clusterPins"] = null;
  if (clusters?.tables.pins) {
    clusterPins = new Map();
    const blockName = texts(clusters, "blocks", "name");
    const netName = new Map<number, string>();
    const ids = numbers(clusters, "nets", "net");
    const names = texts(clusters, "nets", "name");
    for (let i = 0; i < ids.length; i++) netName.set(ids[i], names.get(i));
    const block = numbers(clusters, "pins", "block");
    const port = texts(clusters, "pins", "port");
    const bit = numbers(clusters, "pins", "bit");
    const dir = numbers(clusters, "pins", "dir");
    const net = numbers(clusters, "pins", "net");
    for (let i = 0; i < block.length; i++) {
      const name = blockName.get(block[i]);
      let list = clusterPins.get(name);
      if (!list) clusterPins.set(name, (list = []));
      list.push({
        port: port.get(i),
        bit: bit[i],
        dir: dir[i],
        net: netlist.netIndex.get(netName.get(net[i]) ?? "") ?? -1,
      });
    }
  }
  const { set: symbols, packageName } = await loadSymbols();
  return {
    which,
    netlist,
    clusters,
    clusterName,
    clusterType,
    clusterTypeIndex,
    clusterAtoms,
    tooLarge,
    symbols,
    packageName,
    clusterPins,
    pinInfo: new Map(),
    unnamedPins: 0,
    expanded: new Set(),
    flat: false,
    scope: null,
    scopeText: tooLarge ? "nothing yet" : "the whole design",
    revision: 1,
    shown: null,
    asked: 0,
    retries: 0,
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
  return (key: string, atoms: number[]) => {
    if (base.which !== "atoms") return cut(key, GROUP_CHARS);
    const count = ` (${atoms.length})`;
    return `${cut(moduleParts(key).pop() ?? key, GROUP_CHARS - count.length)}${count}`;
  };
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
        shaperOf(base),
      ),
    };
  return base.graph.graph;
}

/** The aliases of an atom, in the order they are looked for (section 3 of the plan) */
function atomAliases(base: NetlistBase, a: number): string[] {
  const n = base.netlist;
  const model = n.model[a];
  if (model === ".names") {
    const inputs = n.pins
      ? n.pins.of[a].filter((p) => n.pins!.dir[p] !== 1).length
      : 0;
    return [`atom:.names/${inputs}`, "atom:.names", "atom:generic"];
  }
  if ([".latch", ".input", ".output"].includes(model))
    return [`atom:${model}`, "atom:generic"];
  return [`atom:${model}`, "atom:.subckt", "atom:generic"];
}

/** The slack of a net of the atoms, when the timing is there */
function slackOfNet(base: NetlistBase, net: string): number | undefined {
  const timing = timingNow(() => undefined);
  const index = base.netlist.netIndex.get(net);
  const ids = base.netlist.netId;
  if (!timing || index === undefined || !ids || !timing.tables.atom_nets)
    return undefined;
  const slack = numbers(timing, "atom_nets", "slack_ns")[ids[index]];
  return Number.isFinite(slack) ? slack : undefined;
}

/** The body of a symbol whose box has no fill */
const fallbackBody = (dark: boolean): Rgba =>
  dark ? [70, 70, 70, 255] : [240, 240, 235, 255];

/** The value of an attribute of a node, for the texts of its symbol (s:attribute) */
function attributeOf(
  base: NetlistBase,
  node: GraphNode,
  attribute: string,
): string {
  const n = base.netlist;
  if (node.kind === "atom") {
    const a = node.atoms[0];
    switch (attribute) {
      case "name":
        return n.atoms[a];
      case "short_name":
        return node.label;
      case "model":
        return n.model[a];
      case "type":
        return (
          base.clusterType.get(base.clusterName.get(n.cluster[a]) ?? "") ?? ""
        );
      case "count":
        return "1";
      case "ports":
        return String(n.pins?.of[a].length ?? 0);
    }
    return "";
  }
  const key = node.id.slice(2);
  switch (attribute) {
    case "name":
    case "short_name":
      return node.label;
    case "model":
      return "";
    case "type":
      return node.kind === "group"
        ? base.which === "clusters"
          ? (base.clusterType.get(key) ?? "")
          : "module"
        : "";
    case "count":
      return String(node.atoms.length);
    case "ports":
      return String(node.ports);
  }
  return "";
}

/** The symbol of a group (a module, a cluster), or null: a box */
function groupSymbol(base: NetlistBase, key: string) {
  return base.which === "atoms"
    ? symbolFor(base.symbols, [`module:${key}`, "module:generic"])
    : symbolFor(base.symbols, [
        `cluster:${base.clusterType.get(key) ?? ""}`,
        "cluster:generic",
      ]);
}

/** The shapes of the nodes of the graph: their symbols, sizes and ports */
function shaperOf(base: NetlistBase): Shaper {
  const n = base.netlist;
  base.unnamedPins = 0;
  return {
    atom(a) {
      const pins = n.pins;
      const symbol = symbolFor(base.symbols, atomAliases(base, a));
      if (!symbol || !pins) return null;
      const placed = placePins(
        symbol,
        pins.of[a].map((p) => ({
          key: String(p),
          port: pins.port[p],
          bit: pins.bit[p],
          out: pins.dir[p] === 1,
        })),
      );
      base.unnamedPins += placed.unnamed;
      const portOfPin = new Map<number, string>();
      const ports = placed.pins.map((pp) => {
        const p = Number(pp.key);
        const id = `a:${a}#${p}`;
        portOfPin.set(p, id);
        return {
          id,
          side: pp.side,
          x: pp.x,
          y: pp.y,
          label: `${pins.port[p]}[${pins.bit[p]}]`,
          net: pins.net[p],
        };
      });
      return {
        symbol: symbol.type,
        width: placed.width,
        height: placed.height,
        ports,
        portOfPin,
      };
    },
    group(key, _atoms, ins, outs) {
      const symbol = groupSymbol(base, key);
      if (!symbol) return null;
      const pins =
        base.which === "clusters" ? base.clusterPins?.get(key) : undefined;
      if (!pins)
        // Its ports on its sides, one per net, ordered by the layout
        return {
          symbol: symbol.type,
          width: symbol.width,
          height: Math.max(
            symbol.height,
            30 + 10 * Math.max(ins.length, outs.length),
          ),
        };
      const placed = placePins(
        symbol,
        pins.map((p, i) => ({
          key: String(i),
          port: p.port,
          bit: p.bit,
          out: p.dir === 1,
        })),
      );
      base.unnamedPins += placed.unnamed;
      const byNet = new Map<string, string>();
      const ports = placed.pins.map((pp) => {
        const p = pins[Number(pp.key)];
        const id = `g:${key}#${pp.key}`;
        const where = `${p.net}:${p.dir === 1}`;
        if (p.net >= 0 && !byNet.has(where)) byNet.set(where, id);
        return {
          id,
          side: pp.side,
          x: pp.x,
          y: pp.y,
          label: `${p.port}[${p.bit}]`,
          net: p.net,
        };
      });
      return {
        symbol: symbol.type,
        width: placed.width,
        height: placed.height,
        ports,
        portOfNet: (net, out) => byNet.get(`${net}:${out}`),
      };
    },
    stub() {
      const symbol = symbolFor(base.symbols, ["stub"]);
      return symbol
        ? { symbol: symbol.type, width: symbol.width, height: symbol.height }
        : null;
    },
  };
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
        base.which === "atoms" ? moduleKeys(base.netlist.module[a]) : [];
      for (let k = outer.length - 1; k >= 0; k--) {
        const g = before.nodes.get(`g:${outer[k]}`);
        if (g) return { x: g.x, y: g.y };
      }
    }
    return undefined;
  };
}

/** The clusters, in the order of the export, with their atoms */
export function clusterContents(base: NetlistBase): ClusterContents[] {
  if (!base.contents)
    base.contents = [...base.clusterName.values()].map((name) => ({
      name,
      type: base.clusterType.get(name) ?? "",
      typeIndex: base.clusterTypeIndex.get(name),
      atoms: base.clusterAtoms.get(name) ?? [],
    }));
  return base.contents;
}

/** The modules of the netlist as a tree, from the module of each atom: a module has the modules
 * below it and the atoms that are directly in it (the top has the atoms of no module) */
export function moduleTree(base: NetlistBase): ModuleNode {
  if (base.hierarchy) return base.hierarchy;
  const root: ModuleNode = {
    key: "",
    label: "(top)",
    children: [],
    atoms: [],
    count: 0,
  };
  const byKey = new Map<string, ModuleNode>([["", root]]);
  const n = base.netlist;
  for (let a = 0; a < n.atoms.length; a++) {
    let parent = root;
    for (const key of moduleKeys(n.module[a])) {
      let node = byKey.get(key);
      if (!node) {
        node = {
          key,
          label: moduleParts(key).at(-1) ?? key,
          children: [],
          atoms: [],
          count: 0,
        };
        byKey.set(key, node);
        parent.children.push(node);
      }
      node.count++;
      parent = node;
    }
    parent.atoms.push(a);
    root.count++;
  }
  base.hierarchy = root;
  return root;
}

/** Whether a group (a module, a cluster) is expanded in the layout */
export const groupOpen = (base: NetlistBase, key: string) =>
  base.expanded.has(key);

/** Expand a group in the layout, or collapse it (and the groups inside it). False if refused:
 * the graph would be too big, and base.message says so */
export function setGroupOpen(
  base: NetlistBase,
  key: string,
  open: boolean,
  changed: () => void,
): boolean {
  if (groupOpen(base, key) === open) return true;
  const done = change(base, `Expanding ${key}`, () => {
    if (!open) {
      for (const k of [...base.expanded])
        if (k === key || k.startsWith(`${key}.`)) base.expanded.delete(k);
    } else base.expanded.add(key);
  });
  changed();
  return done;
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
    case "block":
      return base.clusterAtoms.get(ref.name) ?? [];
    case "module":
      return n.module.flatMap((m, a) =>
        m === ref.name || m.startsWith(`${ref.name}.`) ? [a] : [],
      );
    case "net": {
      const net = n.netIndex.get(ref.name);
      if (net === undefined) return [];
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
function applyRequest(base: NetlistBase, changed: () => void) {
  const request = netlistRequest.value;
  if (base.which !== "atoms" || !request || request.seq <= handled) return;
  handled = request.seq;
  if (base.tooLarge) {
    fetchScope(base, request, changed);
    return;
  }
  if (request.module !== undefined) {
    const module = request.module;
    change(base, `The module ${module}`, () => {
      base.flat = false;
      base.scope = null;
      base.scopeText = "the whole design";
      // Its parents expanded, so that it shows
      for (const key of moduleKeys(module).slice(0, -1)) base.expanded.add(key);
    });
    if (
      !base.netlist.module.some(
        (m) => m === module || m.startsWith(`${module}.`),
      )
    )
      base.message = `No atom is in ${module}: the synthesis put its logic into its neighbours`;
  } else if (request.cone !== undefined) {
    const a = base.netlist.atomIndex.get(request.cone);
    if (a === undefined) base.message = `There is no atom ${request.cone}`;
    else
      change(base, request.what ?? "The cone", () => {
        const around = cone(base.netlist, [a], 2);
        base.flat = true;
        base.scope = around;
        base.scopeText = `${request.what ?? "a cone"} (${around.size} atoms)`;
      });
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

/** A design over the limit: the netlist of the module or the cone asked for, fetched on its own.
 * A path is the cone of its first atom, the atoms of the path in it shown flat */
function fetchScope(
  base: NetlistBase,
  request: NonNullable<typeof netlistRequest.value>,
  changed: () => void,
) {
  const first = request.cone ?? request.atoms?.[0];
  const scope: Record<string, string | number> | null =
    request.module !== undefined
      ? { module: request.module }
      : first !== undefined
        ? { cone: first, depth: request.cone !== undefined ? 2 : 1 }
        : null;
  if (!scope) return;
  const what =
    request.what ??
    (request.module !== undefined
      ? `the module ${request.module}`
      : `the cone of ${first}`);
  base.message = `Reading ${what}…`;
  const seq = request.seq;
  wholeView("atoms", scope).then(
    (view) => {
      if (seq !== handled) return; // another was asked for since
      base.netlist = netlistOf(view);
      base.contents = undefined;
      base.hierarchy = undefined;
      base.clusterAtoms.clear();
      base.shown = null;
      change(base, what, () => {
        base.expanded = new Set(
          request.module ? moduleKeys(request.module) : [],
        );
        base.flat = request.module === undefined;
        base.scope = null;
        if (request.atoms) {
          // The atoms of the path that are in the cone of its first one
          const found = request.atoms.flatMap((name) => {
            const a = base.netlist.atomIndex.get(name);
            return a === undefined ? [] : [a];
          });
          if (found.length) base.scope = cone(base.netlist, found, 0);
        }
        base.scopeText = what;
      });
      base.refit = true;
      changed();
    },
    (error) => {
      if (seq !== handled) return;
      base.message = String(error?.message ?? error);
      changed();
    },
  );
}

function makeScene(context: BuildContext<NetlistBase>): Built {
  const { base, colors, hidden, selection, changed } = context;
  const dark = colors.dark;
  applyRequest(base, changed);
  const graph = graphOf(base);
  let status = "";
  if (base.tooLarge && !base.netlist.atoms.length) {
    status = `The design is too large to show whole (${base.tooLarge}). Show a module from the Design hierarchy, or a path or an atom from the Timing tab`;
    if (base.message) status += ` · ${base.message}`;
  } else if ("tooBig" in graph) {
    status = `${base.scopeText}: ${graph.tooBig} nodes, more than ${SCOPE_LIMIT}. Select objects (in this or another view) and use "Around the selection", or expand less`;
  } else if (
    base.shown?.revision !== base.revision &&
    base.asked !== base.revision
  ) {
    const revision = base.revision;
    base.asked = revision;
    const before = base.shown;
    const hint = hintOf(base, graph);
    layoutGraph(
      graph,
      hint,
      (id) => !!before?.layout.nodes.has(id),
      base, // the engine of this view: another view never stops its layout
    ).then(
      (layout) => {
        if (base.revision !== revision) return;
        base.shown = { graph, layout, revision };
        base.ms = layout.ms;
        changed();
      },
      (error) => {
        if (String(error?.message ?? error) === "stopped") {
          // Stopped while still wanted (not by a later change of this view): ask again
          if (base.revision === revision && base.asked === revision) {
            base.asked = 0;
            base.retries++;
            changed();
          }
          return;
        }
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
    // The slack of the nets: by the id of each net of the atoms (every net, inside a cluster too),
    // else by the names of the nets of the clusters
    let netSlack: ((net: string) => number) | null = null;
    let range: [number, number] = [0, 0];
    const atomSlack = timing?.tables.atom_nets?.columns.slack_ns
      ? numbers(timing, "atom_nets", "slack_ns")
      : null;
    const ids = n.netId;
    if (atomSlack && ids) {
      range = slackRange(atomSlack);
      netSlack = (net) => {
        const index = n.netIndex.get(net);
        return index === undefined ? Infinity : atomSlack[ids[index]];
      };
    } else if (timing && base.clusters) {
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
    const frames = make("frames");
    const drawer = new SymbolDrawer(base.symbols, styles, dark);
    const lx: number[] = [];
    const ly: number[] = [];
    const ltext: string[] = [];
    // The expanded groups: a frame around what they hold, their name at its top
    for (const f of g.frames) {
      const b = layout.nodes.get(f.id);
      if (!b) continue;
      const [x0, y0, x1, y1] = worldBox(b);
      frames.x.push(x0);
      frames.y.push(y0);
      frames.w.push(x1 - x0);
      frames.h.push(y1 - y0);
      frames.name.push(f.key);
      const type =
        base.which === "clusters"
          ? base.clusterTypeIndex.get(f.key)
          : undefined;
      // Its symbol gives its colours: the role of its first shape, and its outline
      const frameSymbol = symbolFor(
        base.symbols,
        base.which === "clusters"
          ? [
              `frame:cluster:${base.clusterType.get(f.key) ?? ""}`,
              "frame:cluster",
            ]
          : ["frame:module"],
      );
      const frameFill = frameSymbol
        ? drawer.color(frameSymbol, frameSymbol.shapes[0]?.role ?? "body-alt")
        : ((dark ? [40, 44, 54, 255] : [244, 246, 251, 255]) as Rgba);
      const frameStroke: Rgba =
        type !== undefined
          ? blockFill(type, dark)
          : frameSymbol
            ? drawer.color(frameSymbol, "outline")
            : dark
              ? [110, 125, 160, 255]
              : [120, 140, 190, 255];
      frames.style.push(
        styles.add(`frame:${frameFill}:${frameStroke}`, {
          fill: frameFill,
          stroke: frameStroke,
          width: 1.5,
        }),
      );
      lx.push((x0 + x1) / 2);
      ly.push(y1 - FRAME_PAD.top / 2);
      ltext.push(f.label);
    }
    // A node with a symbol: picked by its box (drawn by nothing), drawn by the symbol
    const unseen = styles.add("unseen", {});
    let guessShown = false;
    const symbolBoxes: {
      node: GraphNode;
      box: [number, number, number, number];
      into: ReturnType<typeof make>;
      index: number;
    }[] = [];
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
      if (node.symbol)
        symbolBoxes.push({
          node,
          box: [x0, y0, x1, y1],
          into,
          index: into.x.length - 1,
        });
      else {
        lx.push((x0 + x1) / 2);
        ly.push((y0 + y1) / 2);
        ltext.push(node.label);
      }
      if (node.kind === "group") {
        const key = node.id.slice(2);
        into.name.push(key);
        // A cluster in the colour of its type of block (as in the placement), a module plain
        const type =
          base.which === "clusters"
            ? base.clusterTypeIndex.get(key)
            : undefined;
        into.style.push(
          type === undefined
            ? styles.add("group", {
                fill: dark ? [52, 58, 74, 255] : [228, 234, 246, 255],
                stroke: dark ? [110, 125, 160, 255] : [120, 140, 190, 255],
                width: 1.5,
              })
            : styles.add(`group:${type}`, {
                fill: blockFill(type, dark),
                stroke: dark ? [20, 20, 20, 255] : [60, 60, 60, 255],
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
    // The symbols: in the colour that the box would have, by the style of the box
    for (const { node, box, into, index } of symbolBoxes) {
      const symbol = base.symbols.byType.get(node.symbol!);
      if (!symbol) continue;
      const boxStyle = styles.list[into.style[index]];
      into.style[index] = unseen;
      const guessed =
        node.kind === "atom" &&
        base.which === "atoms" &&
        n.how[node.atoms[0]] === 2;
      drawer.add(
        symbol,
        box,
        boxStyle.fill ?? fallbackBody(dark),
        (attribute) => attributeOf(base, node, attribute),
        guessed ? GUESS : undefined,
        into.name[index],
      );
      if (guessed) guessShown = true;
    }
    for (const l of drawer.labels) {
      lx.push(l.x);
      ly.push(l.y);
      ltext.push(l.text);
    }
    // The pins: where the layout put the ports of the nodes
    const pinX: number[] = [];
    const pinY: number[] = [];
    const pinName: string[] = [];
    base.pinInfo = new Map();
    for (const node of g.nodes)
      for (const port of node.pins ?? []) {
        const at = layout.ports.get(port.id);
        if (!at) continue;
        pinX.push(at.x);
        pinY.push(-at.y);
        pinName.push(port.id);
        base.pinInfo.set(port.id, {
          owner:
            node.kind === "atom" ? n.atoms[node.atoms[0]] : node.id.slice(2),
          label: port.label,
          net: port.net,
        });
      }
    const pinColor = drawer.color(null, "pin");
    const pinsLayer: PointsLayer = {
      id: "pins",
      kind: "points",
      x: Float32Array.from(pinX),
      y: Float32Array.from(pinY),
      radius: 2.5,
      style: new Uint16Array(pinX.length).fill(
        styles.add("pin", { fill: pinColor }),
      ),
      objectKind: "pin",
      name: (i) => pinName[i],
      size: 60,
      minPx: 40,
    };
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
    layers.push(rectsOf(frames, groupKind, "frames"));
    layers.push(edges);
    layers.push(rectsOf(groups, groupKind, "groups"));
    layers.push(rectsOf(atoms, "atom", "atoms"));
    layers.push(rectsOf(stubs, "net", "stubs"));
    layers.push(...drawer.layers());
    if (pinX.length) layers.push(pinsLayer);
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
    // The nets of the selection: a net, or the net of a pin
    const chosenNets = new Set(
      selection.flatMap((r) => {
        if (r.kind === "net") return [r.name];
        const pin = r.kind === "pin" ? base.pinInfo.get(r.name) : undefined;
        return pin && pin.net >= 0 ? [n.nets[pin.net]] : [];
      }),
    );
    const path = selection.find((r) => r.kind === "path");
    // A group that is expanded: its frame
    const framed = (r: ObjectRef) => {
      const key = groupKey(base, r);
      const b = key === null ? undefined : layout.nodes.get(`f:${key}`);
      return b ? worldBox(b) : null;
    };
    const chosenAtoms = selection
      .filter((r) => r.kind !== "net" && r.kind !== "path" && !framed(r))
      .flatMap((r) => atomsOf(base, r, changed));
    if (path) chosenAtoms.push(...atomsOf(base, path, changed));
    const boxes = [
      ...boxesOf(base, chosenAtoms),
      ...selection
        .map(framed)
        .filter((b): b is [number, number, number, number] => b !== null),
    ];
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
    if (groups.x.length && base.which === "atoms")
      legend.push({
        label: "Module (double click: expand)",
        ...styles.get("group"),
      });
    if (frames.x.length)
      legend.push({
        label: `${base.which === "atoms" ? "Module" : "Cluster"} expanded (double click: collapse)`,
        ...(styles.get("frame:") ?? styles.list[frames.style[0]]),
      });
    if (groups.x.length && base.which === "clusters") {
      // A cluster by its type: one entry for each type shown
      const shownTypes = new Map<number, string>();
      for (const key of groups.name) {
        const type = base.clusterTypeIndex.get(key);
        if (type !== undefined)
          shownTypes.set(type, base.clusterType.get(key) ?? `type ${type}`);
      }
      legend.push({ label: "Clusters (double click: expand)" });
      for (const [type, label] of [...shownTypes].sort((a, b) => a[0] - b[0]))
        legend.push({ label, ...styles.get(`group:${type}`) });
      if (groups.name.some((key) => !base.clusterTypeIndex.has(key)))
        legend.push({ label: "Other", ...styles.get("group") });
    }
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
      (guessShown || atoms.style.some((s) => styles.list[s].stroke === GUESS))
    )
      legend.push({ label: "Module guessed", stroke: GUESS });
    if (stubs.x.length)
      legend.push({ label: "Net of a large fanout", ...styles.get("stub") });
    if (netSlack) legend.push(slackLegend(range));
    else legend.push({ label: "Net", stroke: edgeStroke, line: true });
    if (pinX.length) legend.push({ label: "Pin", fill: pinColor });
    if (selection.length) legend.push({ label: "Selected", stroke: SELECTED });
    // What drew the symbols
    legend.push({
      label: `${symbolsNote(base.symbols)}${base.packageName && (base.symbols.replaced.length || base.symbols.hiddenTypes.length || base.symbols.added.length) ? ` (${base.packageName})` : ""}`,
    });
    if (base.symbols.problem)
      legend.push({ label: base.symbols.problem, stroke: GUESS });
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
      shown && !withPins(shown.graph)
        ? `no pins over ${pinsLimitNow()} nodes`
        : base.unnamedPins
          ? `${base.unnamedPins} pins placed by the rule (no port of their symbol)`
          : "",
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
    // The symbols of the package, or not: the views are read again
    reloadKey: () => String(usePackageSymbols.value),
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
      `${base.revision}|${base.shown?.revision ?? 0}|${base.retries}|${netlistRequest.value?.seq ?? 0}|${[...hidden].sort().join(",")}`,
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
      if (ref.kind === "pin") {
        const pin = base.pinInfo.get(ref.name);
        if (!pin) return `pin ${ref.name}`;
        const net = pin.net >= 0 ? n.nets[pin.net] : "";
        const slack = slackOfNet(base, net);
        return `pin ${pin.label} of ${pin.owner}${net ? `: net ${net}` : ""}${slack === undefined ? "" : `, slack ${slack.toFixed(3)} ns`}`;
      }
      if (ref.kind === "net") {
        const net = n.netIndex.get(ref.name);
        if (net === undefined) return `net ${ref.name}`;
        const d = n.driver[net];
        return `net ${ref.name}: from ${d >= 0 ? n.atoms[d] : "outside"} to ${n.sinks[net].length} pins`;
      }
      return `${ref.kind} ${ref.name}`;
    },
    activate(base, ref, changed) {
      const key = groupKey(base, ref);
      if (key !== null) {
        setGroupOpen(base, key, !groupOpen(base, key), changed);
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
    menu(base, ref, changed): ViewTool[] {
      const items: ViewTool[] = [];
      const key = groupKey(base, ref);
      const what = base.which === "atoms" ? "module" : "cluster";
      if (key !== null)
        items.push(
          groupOpen(base, key)
            ? {
                label: "Collapse in the layout",
                title: `The ${what} ${key} as one box`,
                run: () => setGroupOpen(base, key, false, changed),
              }
            : {
                label: "Expand in the layout",
                title: `The atoms of the ${what} ${key}`,
                run: () => setGroupOpen(base, key, true, changed),
              },
        );
      if (ref.kind === "atom" && base.which === "clusters") {
        // The cluster of an atom shown alone: collapse it
        const a = base.netlist.atomIndex.get(ref.name);
        const cluster =
          a === undefined
            ? undefined
            : base.clusterName.get(base.netlist.cluster[a]);
        if (cluster && groupOpen(base, cluster))
          items.push({
            label: `Collapse ${cluster}`,
            title: `The cluster of ${ref.name} as one box`,
            run: () => setGroupOpen(base, cluster, false, changed),
          });
      }
      if (items.length)
        items.push({
          label: "Select",
          title: `Select ${ref.name} in every view`,
          run: () => runViewCommand({ op: "select", ...ref }),
        });
      return items;
    },
    tools(base, changed): ViewTool[] {
      const tools: ViewTool[] = [];
      if (base.scope || base.flat) {
        // A design over the limit has only the scope it fetched: all of it, grouped
        const all = base.tooLarge ? "All of the scope" : "Whole design";
        const text = base.tooLarge
          ? `${base.netlist.atoms.length} atoms fetched`
          : "the whole design";
        tools.push({
          label: all,
          title: base.tooLarge
            ? "All the atoms that were fetched, grouped (the design is too large to show whole)"
            : "Back to the whole design, grouped",
          run: () => {
            change(base, all, () => {
              base.flat = false;
              base.scope = null;
              base.scopeText = text;
            });
            base.refit = true;
            changed();
          },
        });
      }
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
