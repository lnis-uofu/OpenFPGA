// V6, the timing (docs/plan_design_views.md, section 4): the worst paths of the routed design, the
// histogram of the slack of its end points, and the slack of each net and block. The timing panel
// lists them; the placement draws a path through its blocks and the routing draws its nets; both
// can colour by slack.
import { type ViewData, numbers, texts } from "./format";
import { setWholeView, wholeView, wholeViewNow } from "./data";
import { rampColor } from "./palette";
import type { ObjectRef, Rgba } from "./scene";
import type { LegendEntry } from "./view";

/** The number of worst paths that the timing export has */
export const TIMING_PATHS = 50;
const SCOPE = { paths: TIMING_PATHS };

/** The timing export, once for the state of the session */
export const loadTiming = () => wholeView("timing", SCOPE);
/** For the tests: the timing export, as if it was fetched */
export const setTiming = (timing: ViewData) =>
  setWholeView("timing", timing, SCOPE);
/** The timing export if it is there; else asked for, and then() runs when it comes */
export const timingNow = (then: () => void) =>
  wholeViewNow("timing", then, SCOPE);

export interface PathPoint {
  pin: string;
  /** The kind of its atom (.latch, .names, .input...) */
  type: string;
  /** What the delay to it is: on a net ("net"), in a cell ("cell"); "" for the first */
  edge: string;
  /** The atom of the pin, and its net */
  atom: string;
  net: string;
  /** The index of the cluster of the atom (-1 if none), and where it is */
  cluster: number;
  x: number;
  y: number;
  arrival: number;
  /** The delay from the point before */
  delay: number;
}

export interface TimingPath {
  index: number;
  slack: number;
  launch: string;
  capture: string;
  start: string;
  end: string;
  arrival: number;
  required: number;
  points: PathPoint[];
}

export interface HistogramBin {
  low: number;
  high: number;
  count: number;
}

/** The worst paths of the export, worst first */
export function pathsOf(timing: ViewData): TimingPath[] {
  const slack = numbers(timing, "paths", "slack_ns");
  const launch = texts(timing, "paths", "launch_clock");
  const capture = texts(timing, "paths", "capture_clock");
  const start = texts(timing, "paths", "startpoint");
  const end = texts(timing, "paths", "endpoint");
  const arrival = numbers(timing, "paths", "arrival_ns");
  const required = numbers(timing, "paths", "required_ns");
  const first = numbers(timing, "paths", "point_start");
  const count = numbers(timing, "paths", "point_count");
  const pin = texts(timing, "points", "pin");
  const type = texts(timing, "points", "type");
  const edge = timing.tables.points?.columns.edge
    ? texts(timing, "points", "edge")
    : null;
  const atom = texts(timing, "points", "block");
  const net = texts(timing, "points", "net");
  const cluster = numbers(timing, "points", "cluster");
  const x = numbers(timing, "points", "x");
  const y = numbers(timing, "points", "y");
  const at = numbers(timing, "points", "arrival_ns");
  const delay = numbers(timing, "points", "delay_ns");
  return Array.from(slack, (s, i) => ({
    index: i,
    slack: s,
    launch: launch.get(i),
    capture: capture.get(i),
    start: start.get(i),
    end: end.get(i),
    arrival: arrival[i],
    required: required[i],
    points: Array.from({ length: count[i] }, (_, k) => {
      const p = first[i] + k;
      return {
        pin: pin.get(p),
        type: type.get(p),
        edge: edge?.get(p) ?? "",
        atom: atom.get(p),
        net: net.get(p),
        cluster: cluster[p],
        x: x[p],
        y: y[p],
        arrival: at[p],
        delay: delay[p],
      };
    }),
  }));
}

/** The histogram of the slack of the end points (none in an export that has no table of it) */
export function histogramOf(timing: ViewData): HistogramBin[] {
  if (!timing.tables.histogram) return [];
  const low = numbers(timing, "histogram", "low_ns");
  const high = numbers(timing, "histogram", "high_ns");
  const count = numbers(timing, "histogram", "count");
  return Array.from(low, (l, i) => ({
    low: l,
    high: high[i],
    count: count[i],
  }));
}

/** The least and the most finite slack of a column (the blocks, the nets): the ends of the colours */
export function slackRange(values: ArrayLike<number>): [number, number] {
  let least = Infinity;
  let most = -Infinity;
  for (let i = 0; i < values.length; i++) {
    const v = values[i];
    if (!Number.isFinite(v)) continue;
    least = Math.min(least, v);
    most = Math.max(most, v);
  }
  return least <= most ? [least, most] : [0, 0];
}

/** The colour of a slack between the ends: red the worst, green the best; grey with none */
export function slackColor(
  slack: number,
  [least, most]: [number, number],
  dark: boolean,
): Rgba {
  if (!Number.isFinite(slack))
    return dark ? [90, 92, 100, 255] : [175, 175, 170, 255];
  return rampColor(
    "slack",
    most > least ? (slack - least) / (most - least) : 0,
  );
}

/** A slack as one of a few steps, so that the styles of a scene stay few */
export const SLACK_STEPS = 16;
export function slackStep(
  slack: number,
  [least, most]: [number, number],
): number {
  if (!Number.isFinite(slack)) return -1;
  const t = most > least ? (slack - least) / (most - least) : 0;
  return Math.round(Math.min(1, Math.max(0, t)) * (SLACK_STEPS - 1));
}
export const stepSlack = (step: number, [least, most]: [number, number]) =>
  least + ((most - least) * step) / (SLACK_STEPS - 1);

/** The line of the legend of the colours of the slack */
export const slackLegend = ([least, most]: [number, number]): LegendEntry => ({
  label: "Slack",
  ramp: {
    ramp: "slack",
    from: `${least.toFixed(2)} ns`,
    to: `${most.toFixed(2)} ns`,
  },
});

/** The names of the clusters, by their index (the column id of the placement) */
export function clusterNames(placement: ViewData): Map<number, string> {
  const id = numbers(placement, "blocks", "id");
  const name = texts(placement, "blocks", "name");
  const found = new Map<number, string>();
  for (let i = 0; i < id.length; i++) found.set(id[i], name.get(i));
  return found;
}

/** The clusters that a path goes through, in its order, each once in a row */
export function pathClusters(path: TimingPath): PathPoint[] {
  const found: PathPoint[] = [];
  for (const p of path.points) {
    if (p.cluster < 0) continue;
    if (found.length && found[found.length - 1].cluster === p.cluster) continue;
    found.push(p);
  }
  return found;
}

/** What selecting a path selects: the path, its blocks and its nets */
export function pathRefs(
  path: TimingPath,
  names: Map<number, string>,
): ObjectRef[] {
  const refs: ObjectRef[] = [{ kind: "path", name: String(path.index) }];
  const seen = new Set<string>();
  for (const p of pathClusters(path)) {
    const name = names.get(p.cluster);
    if (name && !seen.has(`b:${name}`)) {
      seen.add(`b:${name}`);
      refs.push({ kind: "block", name });
    }
  }
  for (const p of path.points) {
    if (p.net && !seen.has(`n:${p.net}`)) {
      seen.add(`n:${p.net}`);
      refs.push({ kind: "net", name: p.net });
    }
  }
  return refs;
}

/** The selected path, if there is one and the timing has it */
export function selectedPath(
  selection: ObjectRef[],
  timing: ViewData | undefined,
): TimingPath | null {
  const ref = selection.find((r) => r.kind === "path");
  if (!ref || !timing) return null;
  return pathsOf(timing)[Number(ref.name)] ?? null;
}

/** The delay to a point is on a net (not in a cell) */
export const isNetDelay = (p: PathPoint) => p.edge === "net";
