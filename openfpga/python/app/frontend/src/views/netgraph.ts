// The graph that the netlist views lay out (docs/plan_design_views.md, 3.5 and 3.7): the atoms of a
// scope, each shown as itself or inside a group that is collapsed (a module of the RTL in V1, a
// cluster in V2), and the nets between what is shown. Expanding and collapsing is a change of this
// graph, never a request: the netlist of the version is in the page.
import { type ViewData, numbers, texts } from "./format";

/** The atoms and their nets, from the atoms export */
export interface Netlist {
  atoms: string[];
  /** The model of each atom (.names, .latch, .input, .output, a .subckt) */
  model: string[];
  /** The module of each atom ("" the top), and how it is known (0 net, 1 connectivity, 2 common, 3 none) */
  module: string[];
  how: Uint8Array;
  /** The cluster of each atom, by its index (-1 none) */
  cluster: Int32Array;
  nets: string[];
  /** The atom that drives each net (-1 none in the scope), and the atoms of its sinks */
  driver: Int32Array;
  sinks: number[][];
  /** The nets of each atom, in and out */
  netsOf: number[][];
  /** A net with pins outside the export (a scope smaller than the design) */
  partial: Uint8Array;
  atomIndex: Map<string, number>;
}

export function netlistOf(view: ViewData): Netlist {
  const atoms = texts(view, "atoms", "name").all();
  const models = texts(view, "models", "name").all();
  const modules = texts(view, "modules", "name").all();
  const model = Array.from(numbers(view, "atoms", "model"), (m) => models[m]);
  const module = Array.from(
    numbers(view, "atoms", "module"),
    (m) => modules[m] ?? "",
  );
  const how = Uint8Array.from(numbers(view, "atoms", "how"));
  const cluster = Int32Array.from(numbers(view, "atoms", "cluster"));
  const nets = texts(view, "nets", "name").all();
  const pinAtom = numbers(view, "pins", "atom");
  const pinNet = numbers(view, "pins", "net");
  const pinDir = numbers(view, "pins", "dir");
  const driverPin = numbers(view, "nets", "driver");
  const total = numbers(view, "nets", "pins");
  const inside = numbers(view, "nets", "inside");
  const driver = Int32Array.from(driverPin, (p) => (p >= 0 ? pinAtom[p] : -1));
  const sinks: number[][] = nets.map(() => []);
  const netsOf: number[][] = atoms.map(() => []);
  for (let p = 0; p < pinAtom.length; p++) {
    const a = pinAtom[p];
    const n = pinNet[p];
    if (pinDir[p] !== 1) sinks[n].push(a);
    if (!netsOf[a].includes(n)) netsOf[a].push(n);
  }
  const partial = Uint8Array.from(total, (t, i) => (t > inside[i] ? 1 : 0));
  const atomIndex = new Map(atoms.map((a, i) => [a, i]));
  return {
    atoms,
    model,
    module,
    how,
    cluster,
    nets,
    driver,
    sinks,
    netsOf,
    partial,
    atomIndex,
  };
}

/** How the atoms are grouped: the path of the groups of an atom, outermost first. A group is known
 * by its key (the path to it); [] for an atom in no group */
export type Grouping = (atom: number) => string[];

/** The modules of the RTL: "u_a.u_b" is in the groups "u_a" and "u_a.u_b" */
export function moduleGrouping(netlist: Netlist): Grouping {
  return (atom) => {
    const m = netlist.module[atom];
    if (!m) return [];
    const parts = m.split(".");
    return parts.map((_, k) => parts.slice(0, k + 1).join("."));
  };
}

/** The clusters, by their names */
export function clusterGrouping(
  netlist: Netlist,
  names: Map<number, string>,
): Grouping {
  return (atom) => {
    const c = netlist.cluster[atom];
    const name = c >= 0 ? names.get(c) : undefined;
    return name ? [name] : [];
  };
}

export interface GraphNode {
  /** a:<atom index>, g:<group key>, s:<net index> (the stub of a net of a large fanout) */
  id: string;
  kind: "atom" | "group" | "stub";
  label: string;
  /** The atoms it stands for */
  atoms: number[];
  /** For a group: the nets that cross its edge (its ports); for a stub: the sinks it stands for */
  ports: number;
  width: number;
  height: number;
}

export interface GraphEdge {
  id: string;
  net: number;
  source: string;
  target: string;
}

export interface Graph {
  nodes: GraphNode[];
  edges: GraphEdge[];
  /** A text that is the same for the same graph: the key of its layout */
  signature: string;
}

/** The most nodes that a graph lays out; more asks to narrow the scope (3.7) */
export const SCOPE_LIMIT = 2000;
/** A net with more sinks than this is drawn to a stub with its count */
export const FANOUT_LIMIT = 16;

export const ATOM_W = 90;
export const ATOM_H = 28;

/** The short name of an atom: what follows its last '.' or '$' */
export function shortName(name: string): string {
  const cut = Math.max(name.lastIndexOf("."), name.lastIndexOf("$"));
  const short = cut >= 0 && cut < name.length - 1 ? name.slice(cut + 1) : name;
  return short.length > 18 ? `${short.slice(0, 17)}…` : short;
}

/** What an atom is shown as: itself, or its outermost group that is not expanded */
export function nodeOfAtom(
  atom: number,
  grouping: Grouping,
  expanded: Set<string>,
): string {
  for (const key of grouping(atom)) if (!expanded.has(key)) return `g:${key}`;
  return `a:${atom}`;
}

/** The graph of a scope (the atoms shown; all of them when null), with the groups that are expanded.
 * Null when it has more nodes than the limit */
export function buildGraph(
  netlist: Netlist,
  grouping: Grouping,
  expanded: Set<string>,
  scope: Set<number> | null,
  label: (key: string, atoms: number[]) => string = (key) =>
    key.split(".").pop() ?? key,
): Graph | { tooBig: number } {
  const nodeOf = new Map<number, string>();
  const members = new Map<string, number[]>();
  const each = scope ? [...scope] : netlist.atoms.map((_, i) => i);
  for (const a of each) {
    const id = nodeOfAtom(a, grouping, expanded);
    nodeOf.set(a, id);
    let list = members.get(id);
    if (!list) members.set(id, (list = []));
    list.push(a);
  }
  if (members.size > SCOPE_LIMIT) return { tooBig: members.size };
  const nodes: GraphNode[] = [];
  const ports = new Map<string, number>();
  const edges: GraphEdge[] = [];
  const seen = new Set<string>();
  const stubs: GraphNode[] = [];
  for (let n = 0; n < netlist.nets.length; n++) {
    const d = netlist.driver[n];
    const from = d >= 0 ? nodeOf.get(d) : undefined;
    const to = new Set<string>();
    for (const s of netlist.sinks[n]) {
      const t = nodeOf.get(s);
      if (t) to.add(t);
    }
    // The ports of a group: the nets with a pin inside it and one outside (or outside the scope)
    const touched = new Set<string>([...(from ? [from] : []), ...to]);
    const outside =
      netlist.partial[n] ||
      (d >= 0 && !from) ||
      netlist.sinks[n].some((s) => !nodeOf.has(s));
    for (const id of touched) {
      if (!id.startsWith("g:")) continue;
      if (touched.size > 1 || outside) ports.set(id, (ports.get(id) ?? 0) + 1);
    }
    if (!from) continue;
    to.delete(from);
    if (to.size > FANOUT_LIMIT) {
      const stub = `s:${n}`;
      stubs.push({
        id: stub,
        kind: "stub",
        label: `${shortName(netlist.nets[n])} (${to.size})`,
        atoms: [],
        ports: to.size,
        width: 70,
        height: 18,
      });
      edges.push({
        id: `${n}:${from}>${stub}`,
        net: n,
        source: from,
        target: stub,
      });
      continue;
    }
    for (const t of to) {
      const key = `${n}:${from}>${t}`;
      if (seen.has(key)) continue;
      seen.add(key);
      edges.push({ id: key, net: n, source: from, target: t });
    }
  }
  for (const [id, atoms] of members) {
    if (id.startsWith("g:")) {
      const key = id.slice(2);
      const p = ports.get(id) ?? 0;
      nodes.push({
        id,
        kind: "group",
        label: label(key, atoms),
        atoms,
        ports: p,
        width: 150,
        height: Math.max(48, Math.min(160, 30 + 4 * p)),
      });
    } else {
      const a = atoms[0];
      nodes.push({
        id,
        kind: "atom",
        label: shortName(netlist.atoms[a]),
        atoms,
        ports: 0,
        width: ATOM_W,
        height: ATOM_H,
      });
    }
  }
  nodes.push(...stubs);
  nodes.sort((a, b) => (a.id < b.id ? -1 : a.id > b.id ? 1 : 0));
  edges.sort((a, b) => (a.id < b.id ? -1 : a.id > b.id ? 1 : 0));
  const signature = `${nodes.map((n) => n.id).join(",")}|${edges.length}`;
  return { nodes, edges, signature };
}

/** The atoms around some atoms, depth nets away, both ways */
export function cone(
  netlist: Netlist,
  from: number[],
  depth: number,
): Set<number> {
  const found = new Set(from);
  let frontier = [...from];
  for (let level = 0; level < depth; level++) {
    const next: number[] = [];
    for (const a of frontier) {
      for (const n of netlist.netsOf[a]) {
        const around = [netlist.driver[n], ...netlist.sinks[n]];
        if (netlist.sinks[n].length > 64 && netlist.driver[n] !== a) continue; // not through a clock or a reset
        for (const b of around) {
          if (b >= 0 && !found.has(b)) {
            found.add(b);
            next.push(b);
          }
        }
      }
    }
    frontier = next;
  }
  return found;
}
