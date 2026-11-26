// V5, the routing (docs/plan_design_views.md, section 4): the routes of the nets on the tracks of
// the channels, their pins, the tracks themselves when they can be told apart, and the congestion of
// the channels far out. Everything comes in tiles of the core: what is on the screen only.
import { type ViewData, numbers, texts } from "./format";
import { wholeView, wholeViewNow } from "./data";
import {
  blockBox,
  boxOf,
  gridLayer,
  netBox,
  pathCentres,
  rects,
} from "./floorplan";
import { logicBox, pinPoint, wireSegment } from "./geometry";
import type { Layer, PointsLayer, SegmentsLayer } from "./scene";
import { TILE, levelFor, visibleTiles } from "./tiles";
import {
  slackColor,
  slackLegend,
  slackRange,
  slackStep,
  stepSlack,
  timingNow,
} from "./timing";
import {
  type LegendEntry,
  StyleTable,
  type ViewDef,
  hsl,
  nameHue,
} from "./view";

export interface RoutingBase {
  device: ViewData;
  width: number;
  height: number;
  /** The width of the channels: horizontal, vertical */
  chanx: number;
  chany: number;
  /** The index of a net by its name, from the tiles read (for its slack) */
  netIds?: Map<string, number>;
  tilesLayer?: { layer: Layer; dark: boolean };
}

/** Routes are drawn from this many pixels a grid tile; under it, the congestion */
export const ROUTE_PX = 10;
/** The tracks can be told apart when a channel of width W is about 2 W pixels across */
export const trackPx = (width: number) => Math.max(60, 6 * width);

const IPIN = 2;
const OPIN = 3;
const CHANX = 4;
const CHANY = 5;

export async function loadRouting(): Promise<RoutingBase> {
  const device = await wholeView("device");
  // The routing must be there: ask for one tile now, for its message if it is not
  const grid = device.meta.grid as { width: number; height: number };
  const width = device.meta.channel_width as { x: number; y: number } | null;
  return {
    device,
    width: grid.width,
    height: grid.height,
    chanx: width?.x ?? 1,
    chany: width?.y ?? 1,
  };
}

/** What the routes of a tile draw, de-duplicated across tiles by (net, node) */
export interface RouteShapes {
  wires: { xy: number[]; net: string[] };
  pins: { x: number[]; y: number[]; net: string[] };
  links: { xy: number[]; net: string[] };
}

export const emptyShapes = (): RouteShapes => ({
  wires: { xy: [], net: [] },
  pins: { x: [], y: [], net: [] },
  links: { xy: [], net: [] },
});

/** The point of a node nearest to a point: an end of a wire or its pin */
function anchor(
  shape: { wire?: number[]; pin?: [number, number] },
  towards: [number, number],
): [number, number] {
  if (shape.pin) return shape.pin;
  const [x1, y1, x2, y2] = shape.wire!;
  const dx = x2 - x1;
  const dy = y2 - y1;
  const length = dx * dx + dy * dy;
  const t =
    length > 0
      ? Math.max(
          0,
          Math.min(
            1,
            ((towards[0] - x1) * dx + (towards[1] - y1) * dy) / length,
          ),
        )
      : 0;
  return [x1 + t * dx, y1 + t * dy];
}

/** The shapes of the route nodes of a tile of the routing (level 0), added to out; seen holds the
 * nodes already drawn ("net:index") */
export function addRouteTile(
  tile: ViewData,
  chanx: number,
  chany: number,
  out: RouteShapes,
  seen: Set<string>,
) {
  const names = texts(tile, "nets", "name");
  const ids = numbers(tile, "nets", "net");
  const net = numbers(tile, "nodes", "net");
  const index = numbers(tile, "nodes", "index");
  const parent = numbers(tile, "nodes", "parent");
  const type = numbers(tile, "nodes", "type");
  const xl = numbers(tile, "nodes", "xlow");
  const yl = numbers(tile, "nodes", "ylow");
  const xh = numbers(tile, "nodes", "xhigh");
  const yh = numbers(tile, "nodes", "yhigh");
  const ptc = numbers(tile, "nodes", "ptc");
  // The shape of each node of the tile, by net and index, for the links
  const shapes = new Map<string, { wire?: number[]; pin?: [number, number] }>();
  const order: number[] = [];
  for (let i = 0; i < net.length; i++) {
    const key = `${ids[net[i]]}:${index[i]}`;
    let shape: { wire?: number[]; pin?: [number, number] } | null = null;
    if (type[i] === CHANX || type[i] === CHANY) {
      shape = {
        wire: wireSegment(
          type[i] === CHANX,
          xl[i],
          yl[i],
          xh[i],
          yh[i],
          ptc[i],
          type[i] === CHANX ? chanx : chany,
        ),
      };
    } else if (type[i] === IPIN || type[i] === OPIN) {
      shape = { pin: pinPoint(type[i] === OPIN, xl[i], yl[i], ptc[i]) };
    }
    if (!shape) continue;
    shapes.set(key, shape);
    if (seen.has(key)) continue;
    seen.add(key);
    order.push(i);
    const name = names.get(net[i]);
    if (shape.wire) {
      out.wires.xy.push(...shape.wire);
      out.wires.net.push(name);
    } else {
      out.pins.x.push(shape.pin![0]);
      out.pins.y.push(shape.pin![1]);
      out.pins.net.push(name);
    }
  }
  // A link from each node to its parent, when both are in the tile
  for (const i of order) {
    if (parent[i] < 0) continue;
    const from = shapes.get(`${ids[net[i]]}:${parent[i]}`);
    const to = shapes.get(`${ids[net[i]]}:${index[i]}`)!;
    if (!from) continue;
    const toPoint = to.pin ?? [
      (to.wire![0] + to.wire![2]) / 2,
      (to.wire![1] + to.wire![3]) / 2,
    ];
    const a = anchor(from, toPoint as [number, number]);
    const b = anchor(to, a);
    if (Math.hypot(a[0] - b[0], a[1] - b[1]) < 1e-6) continue;
    out.links.xy.push(a[0], a[1], b[0], b[1]);
    out.links.net.push(names.get(net[i]));
  }
}

function netStyle(
  styles: StyleTable,
  name: string,
  dark: boolean,
  alpha = 230,
): number {
  const hue = Math.round(nameHue(name) / 10) * 10;
  return styles.add(`net:${hue}:${alpha}`, {
    stroke: hsl(hue, 0.65, dark ? 0.62 : 0.42, alpha),
    fill: hsl(hue, 0.65, dark ? 0.62 : 0.42, alpha),
    width: 1.5,
  });
}

function segmentsLayer(
  id: string,
  xy: number[],
  nets: string[],
  style: (net: string) => number,
  extra: Partial<SegmentsLayer> = {},
): SegmentsLayer {
  return {
    id,
    kind: "segments",
    xy: Float32Array.from(xy),
    style: Uint16Array.from(nets, style),
    objectKind: "net",
    name: (i) => nets[i],
    size: 1,
    ...extra,
  };
}

export const routingView: ViewDef<RoutingBase> = {
  needs: "ROUTED",
  load: loadRouting,
  bounds: (base) => [0, 0, base.width, base.height],
  groups: [
    { id: "tiles", label: "Tiles" },
    { id: "tracks", label: "Tracks" },
    { id: "routes", label: "Routes" },
    { id: "congestion", label: "Congestion" },
    { id: "slack", label: "Slack", off: true },
  ],
  key(base, view, hidden) {
    const level = levelFor(view.scale, ROUTE_PX, base.width, base.height);
    const tracks = view.scale >= trackPx(Math.max(base.chanx, base.chany));
    return `${level}|${tracks}|${visibleTiles(view, level, base.width, base.height).join(";")}|${[...hidden].sort().join(",")}`;
  },
  build({ base, tiles, view, colors, hidden, selection, changed }) {
    const styles = new StyleTable();
    const dark = colors.dark;
    // First: the kept layer of the tiles refers to it as style 0
    const tileStyle = styles.add("tile", {
      fill: dark ? [36, 38, 45, 255] : [236, 236, 232, 255],
      stroke: dark ? [64, 68, 78, 255] : [200, 200, 196, 255],
      width: 1,
    });
    const level = levelFor(view.scale, ROUTE_PX, base.width, base.height);
    const layers: Layer[] = [];
    let loading = 0;
    let waiting = false;
    const timing = hidden.has("slack") ? undefined : timingNow(changed);
    const errors = new Set<string>();
    const visible = visibleTiles(view, level, base.width, base.height);
    if (level > 0) {
      if (!hidden.has("congestion")) {
        const span = TILE << level;
        for (const [x, y] of visible) {
          const t = tiles.get("channels", level, x, y);
          const error = tiles.error("channels", level, x, y);
          if (error) errors.add(error);
          if (!t) {
            loading += error ? 0 : 1;
            continue;
          }
          // The use of the channels of each cell; a cell with no channel is not drawn
          const value = numbers(t, "cells", "value") as Float32Array;
          const capacity = numbers(t, "cells", "capacity");
          layers.push(
            gridLayer(
              `congestion:${level}:${x}:${y}`,
              x * span,
              y * span,
              1 << level,
              Float32Array.from(value, (v, i) => (capacity[i] > 0 ? v : NaN)),
              { kind: "ramp", min: 0, max: 1, ramp: "use" },
            ),
          );
        }
      }
    } else {
      if (!hidden.has("tiles")) {
        if (!base.tilesLayer || base.tilesLayer.dark !== dark) {
          const tx = numbers(base.device, "tiles", "x");
          const ty = numbers(base.device, "tiles", "y");
          const tw = numbers(base.device, "tiles", "w");
          const th = numbers(base.device, "tiles", "h");
          const boxes = Array.from({ length: tx.length }, (_, i) =>
            logicBox(tx[i], ty[i], tw[i], th[i]),
          );
          base.tilesLayer = {
            layer: rects("tiles", boxes, tileStyle, { size: 0.6, inert: true }),
            dark,
          };
        }
        layers.push(base.tilesLayer.layer);
      }
      const tracks = view.scale >= trackPx(Math.max(base.chanx, base.chany));
      if (tracks && !hidden.has("tracks")) {
        const xy: number[] = [];
        const style: number[] = [];
        const free = styles.add("track", {
          stroke: dark ? [70, 74, 84, 255] : [205, 205, 200, 255],
          width: 1,
        });
        const busy = styles.add("track-used", {
          stroke: dark ? [120, 124, 134, 255] : [150, 150, 145, 255],
          width: 1,
        });
        for (const [x, y] of visible) {
          const t = tiles.get("channels", 0, x, y);
          if (!t) {
            loading++;
            continue;
          }
          const type = numbers(t, "wires", "type");
          const used = numbers(t, "wires", "used");
          for (let i = 0; i < type.length; i++) {
            xy.push(
              ...wireSegment(
                type[i] === CHANX,
                numbers(t, "wires", "xlow")[i],
                numbers(t, "wires", "ylow")[i],
                numbers(t, "wires", "xhigh")[i],
                numbers(t, "wires", "yhigh")[i],
                numbers(t, "wires", "ptc")[i],
                type[i] === CHANX ? base.chanx : base.chany,
              ),
            );
            style.push(used[i] ? busy : free);
          }
        }
        layers.push({
          id: "tracks",
          kind: "segments",
          xy: Float32Array.from(xy),
          style: Uint16Array.from(style),
          size: 1,
          inert: true,
        });
      }
      const shapes = emptyShapes();
      const seen = new Set<string>();
      const netSlack = timing ? numbers(timing, "nets", "slack_ns") : null;
      const range = netSlack ? slackRange(netSlack) : null;
      if (!hidden.has("slack") && !timing) waiting = true;
      for (const [x, y] of visible) {
        const t = tiles.get("routing", 0, x, y);
        const error = tiles.error("routing", 0, x, y);
        if (error) errors.add(error);
        if (!t) {
          loading += error ? 0 : 1;
          continue;
        }
        addRouteTile(t, base.chanx, base.chany, shapes, seen);
        if (timing) {
          base.netIds ??= new Map();
          const names = texts(t, "nets", "name");
          const ids = numbers(t, "nets", "net");
          for (let i = 0; i < ids.length; i++)
            base.netIds.set(names.get(i), ids[i]);
        }
      }
      // The colour of a net: its own, or that of its slack
      const colour = (n: string, alpha = 230) => {
        if (!netSlack || !range) return netStyle(styles, n, dark, alpha);
        const id = base.netIds?.get(n);
        const step = slackStep(
          id === undefined ? Infinity : netSlack[id],
          range,
        );
        const c = slackColor(
          step < 0 ? Infinity : stepSlack(step, range),
          range,
          dark,
        );
        c[3] = alpha;
        return styles.add(`slack:${step}:${alpha}`, {
          stroke: c,
          fill: c,
          width: 1.5,
        });
      };
      if (!hidden.has("routes")) {
        layers.push(
          segmentsLayer(
            "links",
            shapes.links.xy,
            shapes.links.net,
            (n) => colour(n, 150),
            { size: 0.3 },
          ),
        );
        layers.push(
          segmentsLayer("wires", shapes.wires.xy, shapes.wires.net, (n) =>
            colour(n),
          ),
        );
        const pins: PointsLayer = {
          id: "pins",
          kind: "points",
          x: Float32Array.from(shapes.pins.x),
          y: Float32Array.from(shapes.pins.y),
          radius: 3,
          style: Uint16Array.from(shapes.pins.net, (n) => colour(n)),
          objectKind: "net",
          name: (i) => shapes.pins.net[i],
          size: 0.1,
        };
        layers.push(pins);
      }
      // The selected nets over everything
      const chosen = new Set(
        selection.filter((r) => r.kind === "net").map((r) => r.name),
      );
      if (chosen.size) {
        const xy: number[] = [];
        const names: string[] = [];
        shapes.wires.net.forEach((n, i) => {
          if (chosen.has(n)) {
            xy.push(...shapes.wires.xy.slice(i * 4, i * 4 + 4));
            names.push(n);
          }
        });
        shapes.links.net.forEach((n, i) => {
          if (chosen.has(n)) {
            xy.push(...shapes.links.xy.slice(i * 4, i * 4 + 4));
            names.push(n);
          }
        });
        const style = styles.add("selected", {
          stroke: [250, 80, 40, 255],
          width: 4,
        });
        layers.push(
          segmentsLayer("selection:nets", xy, names, () => style, {
            inert: true,
          }),
        );
      }
    }
    // A selected block: its box, from the placement
    const blocks = selection.filter((r) => r.kind === "block");
    if (blocks.length) {
      const placement = wholeViewNow("placement", changed);
      if (placement) {
        const fp = {
          device: base.device,
          placement,
          unplaced: null,
          width: base.width,
          height: base.height,
          tileAt: tileIndex(base.device),
          blockRow: blockIndex(placement),
        };
        const boxes = blocks.flatMap((r) => {
          const row = fp.blockRow.get(r.name);
          const box = row === undefined ? null : blockBox(fp, row);
          return box ? [box] : [];
        });
        if (boxes.length)
          layers.push(
            rects(
              "selection:blocks",
              boxes,
              styles.add("selected-block", {
                stroke: [250, 80, 40, 255],
                width: 3,
              }),
              { inert: true },
            ),
          );
      }
    }
    const status = [
      level === 0
        ? "every route on the screen"
        : `congestion, level ${level}: a cell is ${1 << level}×${1 << level} tiles`,
      level === 0 && view.scale < trackPx(Math.max(base.chanx, base.chany))
        ? "zoom in for the tracks"
        : "",
      `channel width ${base.chanx}`,
      loading ? `${loading} tiles loading` : "",
      waiting ? "analyzing the timing" : "",
      ...errors,
    ].filter(Boolean);
    const legend: LegendEntry[] =
      level > 0
        ? hidden.has("congestion")
          ? []
          : [
              {
                label: "Use of the channels",
                ramp: { ramp: "use", from: "free", to: "full" },
              },
            ]
        : [
            ...(hidden.has("tiles") ? [] : styles.legend([["tile", "Tile"]])),
            ...styles
              .legend([
                ["track", "Free track"],
                ["track-used", "Used track"],
              ])
              .map((e) => ({ ...e, line: true })),
            ...(hidden.has("routes")
              ? []
              : !hidden.has("slack") && timing
                ? [slackLegend(slackRange(numbers(timing, "nets", "slack_ns")))]
                : [
                    {
                      label: "A net (a colour each)",
                      stroke: hsl(200, 0.65, dark ? 0.62 : 0.42),
                      line: true,
                    },
                  ]),
            ...styles
              .legend([["selected", "Selected net"]])
              .map((e) => ({ ...e, line: true })),
          ];
    legend.push(...styles.legend([["selected-block", "Selected block"]]));
    return {
      scene: {
        layers,
        bounds: [0, 0, base.width, base.height],
        styles: styles.list,
      },
      status: status.join(" · "),
      legend,
    };
  },
  locate(base, ref, changed) {
    if (ref.kind === "path") {
      const placement = wholeViewNow("placement", changed);
      if (!placement) return null;
      const fp = {
        device: base.device,
        placement,
        unplaced: null,
        width: base.width,
        height: base.height,
        tileAt: tileIndex(base.device),
        blockRow: blockIndex(placement),
      };
      return boxOf(pathCentres(fp, changed, [ref]) ?? []);
    }
    if (ref.kind === "net" || ref.kind === "block") {
      const clusters =
        ref.kind === "net" ? wholeViewNow("clusters", changed) : undefined;
      const placement = wholeViewNow("placement", changed);
      if (!placement) return null;
      const fp = {
        device: base.device,
        placement,
        unplaced: null,
        width: base.width,
        height: base.height,
        tileAt: tileIndex(base.device),
        blockRow: blockIndex(placement),
      };
      if (ref.kind === "block") {
        const row = fp.blockRow.get(ref.name);
        return row === undefined ? null : blockBox(fp, row);
      }
      return clusters ? netBox(fp, clusters, ref.name) : null;
    }
    return null;
  },
  describe(_base, ref) {
    return `${ref.kind} ${ref.name}`;
  },
};

const tileIndexes = new WeakMap<ViewData, Map<string, number>>();
const blockIndexes = new WeakMap<ViewData, Map<string, number>>();

function tileIndex(device: ViewData): Map<string, number> {
  let found = tileIndexes.get(device);
  if (!found) {
    found = new Map();
    const x = numbers(device, "tiles", "x");
    const y = numbers(device, "tiles", "y");
    for (let i = 0; i < x.length; i++) found.set(`${x[i]},${y[i]}`, i);
    tileIndexes.set(device, found);
  }
  return found;
}

function blockIndex(placement: ViewData): Map<string, number> {
  let found = blockIndexes.get(placement);
  if (!found) {
    found = new Map();
    const names = texts(placement, "blocks", "name");
    for (let i = 0; i < names.length; i++) found.set(names.get(i), i);
    blockIndexes.set(placement, found);
  }
  return found;
}
