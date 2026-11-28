// What a view of the design is to the panel that shows it (docs/plan_design_views.md, P2.5): its
// data, how it builds a scene for a view state, and how it finds and names its objects. The panel
// does the rest the same way for every view: the renderer, the input, the selection, the tiles.
import type { ComponentType } from "preact";
import type { ViewColors } from "./palette";
import type { ObjectRef, Ramp, Rgba, Scene, Style } from "./scene";
import type { TileCache } from "./tiles";
import type { ViewState } from "./viewstate";

export interface BuildContext<B> {
  base: B;
  tiles: TileCache;
  view: ViewState;
  selection: ObjectRef[];
  highlighted: ObjectRef[];
  colors: ViewColors;
  /** The groups of layers that the person turned off */
  hidden: Set<string>;
  /** Ask for a build again (data came) */
  changed: () => void;
}

export interface Built {
  scene: Scene;
  /** A line about what is shown (the level, what is loading) */
  status: string;
  /** What the colours of the scene mean, shown in a corner of the view */
  legend?: LegendEntry[];
  /** Fit the view to the scene: its layout is new (a netlist view, laid out again) */
  refit?: boolean;
}

/** A button of the toolbar of a view */
export interface ViewTool {
  label: string;
  title: string;
  run(): void;
}

/** A line of the legend: a heading (a label alone), a swatch (a fill, an outline, a line), or a
 * ramp from one end to the other */
export interface LegendEntry {
  label: string;
  fill?: Rgba;
  stroke?: Rgba;
  /** Drawn as a line, not a box (a wire, a route) */
  line?: boolean;
  ramp?: { ramp: Ramp; from: string; to: string };
}

export interface LayerGroup {
  id: string;
  label: string;
  /** Off until the person turns it on (a colouring that costs an analysis) */
  off?: boolean;
}

export interface ViewDef<B> {
  /** The stage whose data it needs, for the message when there is none */
  needs: string;
  load(): Promise<B>;
  /** What else the data depends on: when it changes, load() runs again (a setting) */
  reloadKey?(): string;
  bounds(base: B): [number, number, number, number];
  groups: LayerGroup[];
  /** What the scene depends on besides the selection: when it is the same, the scene is kept */
  key(base: B, view: ViewState, hidden: Set<string>): string;
  build(context: BuildContext<B>): Built;
  /** Where an object is, to zoom to it; null if the view does not have it */
  locate(
    base: B,
    ref: ObjectRef,
    changed: () => void,
  ): [number, number, number, number] | null;
  /** A line about an object, for the tooltip and the selection */
  describe(base: B, ref: ObjectRef): string;
  /** A double click on an object (expand, collapse, its cone): true if it did something, else the
   * view fits */
  activate?(base: B, ref: ObjectRef, changed: () => void): boolean;
  /** The buttons of the view in its toolbar, for its state now */
  tools?(base: B, changed: () => void): ViewTool[];
  /** The commands of a right click on an object of the view (none: the menu of the page) */
  menu?(base: B, ref: ObjectRef, changed: () => void): ViewTool[];
  /** A list beside the drawing (the clusters of V2), drawn again at each build (version) */
  side?: {
    label: string;
    component: ComponentType<{
      base: B;
      changed: () => void;
      version: number;
    }>;
  };
}

/** The styles of a scene, by a key: the same key gives the same index */
export class StyleTable {
  readonly list: Style[] = [];
  private readonly keys = new Map<string, number>();

  add(key: string, style: Style): number {
    let index = this.keys.get(key);
    if (index === undefined) {
      index = this.list.length;
      this.list.push(style);
      this.keys.set(key, index);
    }
    return index;
  }

  /** The style of a key, if a layer uses it */
  get(key: string): Style | undefined {
    const index = this.keys.get(key);
    return index === undefined ? undefined : this.list[index];
  }

  /** The lines of a legend of the keys that are used: [key, label] */
  legend(entries: [string, string][]): LegendEntry[] {
    return entries.flatMap(([key, label]) => {
      const style = this.get(key);
      return style ? [{ label, fill: style.fill, stroke: style.stroke }] : [];
    });
  }
}

/** A colour for a name: the same name, the same colour (the nets of the routing) */
export function nameHue(name: string): number {
  let h = 2166136261;
  for (let i = 0; i < name.length; i++)
    h = Math.imul(h ^ name.charCodeAt(i), 16777619);
  return (h >>> 0) % 360;
}

export function hsl(
  h: number,
  s: number,
  l: number,
  alpha = 255,
): [number, number, number, number] {
  const k = (n: number) => (n + h / 30) % 12;
  const a = s * Math.min(l, 1 - l);
  const f = (n: number) =>
    l - a * Math.max(-1, Math.min(k(n) - 3, Math.min(9 - k(n), 1)));
  return [
    Math.round(255 * f(0)),
    Math.round(255 * f(8)),
    Math.round(255 * f(4)),
    alpha,
  ];
}
