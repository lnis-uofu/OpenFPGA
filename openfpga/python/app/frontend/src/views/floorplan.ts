// V3, the floorplan (docs/plan_design_views.md, section 4): the tiles of the device by type, the
// blocks on their slots, the regions of the placement constraints, and the boxes of the nets of what
// is selected. Close up it draws the objects of the whole device and placement exports (bounded:
// R-11); far out, the coarse tiles of the core (one image a tile, whatever the size of the device).
import { type ViewData, numbers, texts } from "./format";
import { wholeView, wholeViewNow } from "./data";
import { logicBox, slotBox } from "./geometry";
import { categoryColor } from "./palette";
import type { GridLayer, Layer, ObjectRef, RectsLayer, Rgba } from "./scene";
import { TILE, levelFor, visibleTiles } from "./tiles";
import {
  type BuildContext,
  type Built,
  type LegendEntry,
  StyleTable,
  type ViewDef,
} from "./view";

export interface FloorplanBase {
  device: ViewData;
  placement: ViewData | null;
  /** Why there is no placement (the stage is not reached) */
  unplaced: string | null;
  width: number;
  height: number;
  /** The tile at a location: its row in the tiles of the device */
  tileAt: Map<string, number>;
  blockRow: Map<string, number>;
  /** The layers of the objects, made once */
  objects?: {
    layers: Layer[];
    styles: StyleTable;
    dark: boolean;
    /** The tile types and the block types that the device and the placement have */
    tileLegend: LegendEntry[];
    blockLegend: LegendEntry[];
  };
}

const SELECTED: Rgba = [250, 80, 40, 255];
const NET_BOX: Rgba = [40, 160, 230, 255];
const REGION: Rgba = [120, 90, 220, 255];

/** The colour of a type of tile, and of a type of block */
const tileFill = (type: number, io: boolean, dark: boolean): Rgba =>
  categoryColor(io ? 1 : type + 2, dark, 255);
const blockFill = (type: number, dark: boolean): Rgba =>
  categoryColor(type + 5, dark, 255).map((v, k) =>
    k < 3 ? Math.min(255, Math.round(v * (dark ? 1.6 : 0.75))) : v,
  ) as Rgba;

/** The objects are drawn from this many pixels a grid tile; under it, the coarse tiles */
export const OBJECT_PX = 6;

function grid(device: ViewData): { width: number; height: number } {
  const g = device.meta.grid as { width: number; height: number };
  return { width: g.width, height: g.height };
}

export async function loadFloorplan(): Promise<FloorplanBase> {
  const device = await wholeView("device");
  let placement: ViewData | null = null;
  let unplaced: string | null = null;
  try {
    placement = await wholeView("placement");
  } catch (error) {
    unplaced = String((error as Error).message ?? error);
  }
  const tileAt = new Map<string, number>();
  const tx = numbers(device, "tiles", "x");
  const ty = numbers(device, "tiles", "y");
  for (let i = 0; i < tx.length; i++) tileAt.set(`${tx[i]},${ty[i]}`, i);
  const blockRow = new Map<string, number>();
  if (placement) {
    const names = texts(placement, "blocks", "name");
    for (let i = 0; i < names.length; i++) blockRow.set(names.get(i), i);
  }
  return { device, placement, unplaced, ...grid(device), tileAt, blockRow };
}

/** The box of the slot of a block of the placement, by its row */
export function blockBox(
  base: FloorplanBase,
  row: number,
): [number, number, number, number] | null {
  if (!base.placement) return null;
  const x = numbers(base.placement, "blocks", "x")[row];
  const y = numbers(base.placement, "blocks", "y")[row];
  const slot = numbers(base.placement, "blocks", "sub_tile")[row];
  const tile = base.tileAt.get(`${x},${y}`);
  if (tile === undefined) return logicBox(x, y);
  const type = numbers(base.device, "tiles", "type")[tile];
  const capacity = numbers(base.device, "tile_types", "capacity")[type];
  return slotBox(
    x,
    y,
    numbers(base.device, "tiles", "w")[tile],
    numbers(base.device, "tiles", "h")[tile],
    slot,
    capacity,
  );
}

/** The box of the blocks of a net (the clusters export: the driver and the sinks), or null */
export function netBox(
  base: FloorplanBase,
  clusters: ViewData,
  net: string,
): [number, number, number, number] | null {
  const names = texts(clusters, "nets", "name");
  const blocks = texts(clusters, "blocks", "name");
  const driver = numbers(clusters, "nets", "driver");
  const start = numbers(clusters, "nets", "sink_start");
  const count = numbers(clusters, "nets", "sink_count");
  const sinks = numbers(clusters, "sinks", "block");
  for (let n = 0; n < names.length; n++) {
    if (names.get(n) !== net) continue;
    const members = [
      driver[n],
      ...Array.from(sinks.subarray(start[n], start[n] + count[n])),
    ].filter((b) => b >= 0);
    return unionBox(
      members
        .map((b) => base.blockRow.get(blocks.get(b)))
        .flatMap((row) => (row === undefined ? [] : [blockBox(base, row)])),
    );
  }
  return null;
}

/** The nets that have a pin on a block (by the clusters export) */
export function netsOfBlock(clusters: ViewData, block: string): string[] {
  const blocks = texts(clusters, "blocks", "name");
  let index = -1;
  for (let b = 0; b < blocks.length; b++)
    if (blocks.get(b) === block) index = b;
  if (index < 0) return [];
  const names = texts(clusters, "nets", "name");
  const driver = numbers(clusters, "nets", "driver");
  const start = numbers(clusters, "nets", "sink_start");
  const count = numbers(clusters, "nets", "sink_count");
  const sinks = numbers(clusters, "sinks", "block");
  const ignored = numbers(clusters, "nets", "ignored");
  const found: string[] = [];
  for (let n = 0; n < names.length; n++) {
    if (ignored[n]) continue;
    if (
      driver[n] === index ||
      Array.from(sinks.subarray(start[n], start[n] + count[n])).includes(index)
    )
      found.push(names.get(n));
  }
  return found;
}

function unionBox(
  boxes: ([number, number, number, number] | null)[],
): [number, number, number, number] | null {
  let out: [number, number, number, number] | null = null;
  for (const b of boxes) {
    if (!b) continue;
    out = out
      ? [
          Math.min(out[0], b[0]),
          Math.min(out[1], b[1]),
          Math.max(out[2], b[2]),
          Math.max(out[3], b[3]),
        ]
      : [...b];
  }
  return out;
}

/** Rects from a list of boxes */
export function rects(
  id: string,
  boxes: [number, number, number, number][],
  style: number,
  extra: Partial<RectsLayer> = {},
): RectsLayer {
  const n = boxes.length;
  const layer: RectsLayer = {
    id,
    kind: "rects",
    x: new Float32Array(n),
    y: new Float32Array(n),
    w: new Float32Array(n),
    h: new Float32Array(n),
    style: new Uint16Array(n).fill(style),
    size: 1,
    ...extra,
  };
  boxes.forEach(([x0, y0, x1, y1], i) => {
    layer.x[i] = x0;
    layer.y[i] = y0;
    layer.w[i] = x1 - x0;
    layer.h[i] = y1 - y0;
  });
  return layer;
}

/** The layers of the objects: the tiles, the blocks and their names, the regions */
function objectLayers(
  base: FloorplanBase,
  dark: boolean,
): {
  layers: Layer[];
  styles: StyleTable;
  tileLegend: LegendEntry[];
  blockLegend: LegendEntry[];
} {
  const styles = new StyleTable();
  const device = base.device;
  const tileType = numbers(device, "tiles", "type");
  const typeNames = texts(device, "tile_types", "name");
  const isIo = numbers(device, "tile_types", "is_io");
  const tx = numbers(device, "tiles", "x");
  const ty = numbers(device, "tiles", "y");
  const tw = numbers(device, "tiles", "w");
  const th = numbers(device, "tiles", "h");
  const tileStyles = new Uint16Array(tileType.length);
  const boxes: [number, number, number, number][] = [];
  for (let i = 0; i < tileType.length; i++) {
    const t = tileType[i];
    tileStyles[i] = styles.add(`tile:${t}`, {
      fill: tileFill(t, !!isIo[t], dark),
      stroke: dark ? [90, 95, 110, 255] : [170, 170, 165, 255],
      width: 1,
    });
    boxes.push(logicBox(tx[i], ty[i], tw[i], th[i]));
  }
  const tiles = rects("tiles", boxes, 0, {
    style: tileStyles,
    objectKind: "tile",
    name: (i) => `${typeNames.get(tileType[i])} ${tx[i]},${ty[i]}`,
    size: 0.6,
    minPx: 2,
  });
  const layers: Layer[] = [tiles];
  const tileLegend = [...new Set(tileType)]
    .sort((a, b) => a - b)
    .map((t) => ({
      label: typeNames.get(t),
      fill: tileFill(t, !!isIo[t], dark),
    }));
  let blockLegend: LegendEntry[] = [];
  if (base.placement) {
    const placement = base.placement;
    const names = texts(placement, "blocks", "name");
    const types = numbers(placement, "blocks", "type");
    const blockBoxes: [number, number, number, number][] = [];
    const blockStyles = new Uint16Array(names.length);
    for (let i = 0; i < names.length; i++) {
      blockBoxes.push(blockBox(base, i) ?? [0, 0, 0, 0]);
      blockStyles[i] = styles.add(`block:${types[i]}`, {
        fill: blockFill(types[i], dark),
        stroke: dark ? [20, 20, 20, 255] : [60, 60, 60, 255],
        width: 1,
      });
    }
    layers.push(
      rects("blocks", blockBoxes, 0, {
        style: blockStyles,
        objectKind: "block",
        name: (i) => names.get(i),
        size: 0.25,
        minPx: 1.5,
      }),
    );
    const blockTypes = texts(placement, "block_types", "name");
    blockLegend = [...new Set(types)]
      .sort((a, b) => a - b)
      .map((t) => ({ label: blockTypes.get(t), fill: blockFill(t, dark) }));
    if (names.length <= 50000) {
      const lx = new Float32Array(names.length);
      const ly = new Float32Array(names.length);
      blockBoxes.forEach((b, i) => {
        lx[i] = (b[0] + b[2]) / 2;
        ly[i] = (b[1] + b[3]) / 2;
      });
      layers.push({
        id: "labels",
        kind: "labels",
        x: lx,
        y: ly,
        text: (i) => names.get(i),
        count: names.length,
        px: 11,
        style: new Uint16Array(names.length).fill(
          styles.add("label", {
            fill: dark ? [235, 235, 235, 255] : [20, 20, 20, 255],
          }),
        ),
        size: 0.5,
        minPx: 110,
        inert: true,
      });
    }
    const region = numbers(placement, "regions", "partition");
    if (region.length) {
      const regionBoxes: [number, number, number, number][] = [];
      for (let i = 0; i < region.length; i++) {
        regionBoxes.push([
          numbers(placement, "regions", "xmin")[i],
          numbers(placement, "regions", "ymin")[i],
          numbers(placement, "regions", "xmax")[i] + 1,
          numbers(placement, "regions", "ymax")[i] + 1,
        ]);
      }
      const partitions = texts(placement, "partitions", "name");
      layers.push(
        rects(
          "regions",
          regionBoxes,
          styles.add("region", {
            stroke: REGION,
            width: 2,
            fill: [120, 90, 220, 28],
          }),
          {
            objectKind: "region",
            name: (i) => partitions.get(region[i]),
            size: 4,
          },
        ),
      );
    }
  }
  return { layers, styles, tileLegend, blockLegend };
}

/** What the colours of the floorplan mean, at a level */
function floorplanLegend(
  context: BuildContext<FloorplanBase>,
  level: number,
): LegendEntry[] {
  const { base, hidden, selection } = context;
  const objects = base.objects!;
  const legend: LegendEntry[] = [];
  if (!hidden.has("tiles"))
    legend.push({ label: "Tiles" }, ...objects.tileLegend);
  if (base.placement && !hidden.has("blocks")) {
    if (level === 0) legend.push({ label: "Blocks" }, ...objects.blockLegend);
    else
      legend.push({
        label: "Use of the cells",
        ramp: { ramp: "use", from: "empty", to: "full" },
      });
  }
  const regions = base.placement
    ? numbers(base.placement, "regions", "partition").length
    : 0;
  if (regions && !hidden.has("regions"))
    legend.push({ label: "Region", stroke: REGION, fill: [120, 90, 220, 28] });
  if (selection.length) {
    legend.push({ label: "Selected", stroke: SELECTED });
    if (selection.some((r) => r.kind === "block" || r.kind === "net"))
      legend.push({
        label: "Box of a net",
        stroke: NET_BOX,
        fill: [40, 160, 230, 20],
      });
  }
  return legend;
}

/** The coarse tiles of a level on the screen, as grid layers */
function coarseLayers(
  context: BuildContext<FloorplanBase>,
  level: number,
): { layers: Layer[]; loading: number } {
  const { base, tiles, view, colors, hidden } = context;
  const layers: Layer[] = [];
  let loading = 0;
  const span = TILE << level;
  const cell = 1 << level;
  const types = texts(base.device, "tile_types", "name").length;
  const isIo = numbers(base.device, "tile_types", "is_io");
  const typeColors = Array.from({ length: types }, (_, t) =>
    tileFill(t, !!isIo[t], colors.dark),
  );
  for (const [x, y] of visibleTiles(view, level, base.width, base.height)) {
    if (!hidden.has("tiles")) {
      const g = tiles.get("grid", level, x, y);
      if (!g) loading++;
      else {
        const type = numbers(g, "cells", "type");
        const value = Float32Array.from(type, (t) => (t === 0xffff ? NaN : t));
        layers.push(
          gridLayer(
            `grid:${level}:${x}:${y}`,
            x * span,
            y * span,
            cell,
            value,
            { kind: "categorical", colors: typeColors },
          ),
        );
      }
    }
    if (base.placement && !hidden.has("blocks")) {
      const p = tiles.get("placement", level, x, y);
      if (!p) loading++;
      else {
        const share = numbers(p, "cells", "value") as Float32Array;
        const used = numbers(p, "cells", "used");
        const value = Float32Array.from(share, (v, i) =>
          used[i] > 0 ? v : NaN,
        );
        layers.push(
          gridLayer(`use:${level}:${x}:${y}`, x * span, y * span, cell, value, {
            kind: "ramp",
            min: 0,
            max: 1,
            ramp: "use",
          }),
        );
      }
    }
  }
  return { layers, loading };
}

export function gridLayer(
  id: string,
  x0: number,
  y0: number,
  cell: number,
  value: Float32Array,
  palette: GridLayer["palette"],
): GridLayer {
  return {
    id,
    kind: "grid",
    x0,
    y0,
    cell,
    cols: TILE,
    rows: TILE,
    value,
    palette,
    size: cell,
    inert: true,
  };
}

/** The selection on the floorplan: its blocks, and the boxes of the nets of what is selected */
function selectionLayers(
  context: BuildContext<FloorplanBase>,
  styles: StyleTable,
): Layer[] {
  const { base, selection, changed } = context;
  const blocks: [number, number, number, number][] = [];
  const nets: string[] = [];
  let clusters: ViewData | undefined;
  for (const ref of selection) {
    if (ref.kind === "block") {
      const row = base.blockRow.get(ref.name);
      const box = row === undefined ? null : blockBox(base, row);
      if (box) blocks.push(box);
      clusters ??= wholeViewNow("clusters", changed);
      if (clusters) nets.push(...netsOfBlock(clusters, ref.name));
    } else if (ref.kind === "net") nets.push(ref.name);
  }
  const layers: Layer[] = [];
  if (nets.length) {
    clusters ??= wholeViewNow("clusters", changed);
    const boxList: [number, number, number, number][] = [];
    for (const n of clusters ? nets.slice(0, 200) : []) {
      const box = netBox(base, clusters!, n);
      if (box) boxList.push(box);
    }
    if (boxList.length)
      layers.push(
        rects(
          "selection:nets",
          boxList,
          styles.add("net-box", {
            stroke: NET_BOX,
            width: 1.5,
            fill: [40, 160, 230, 20],
          }),
          { inert: true },
        ),
      );
  }
  if (blocks.length)
    layers.push(
      rects(
        "selection:blocks",
        blocks,
        styles.add("selected", { stroke: SELECTED, width: 3 }),
        { inert: true },
      ),
    );
  return layers;
}

export const floorplanView: ViewDef<FloorplanBase> = {
  needs: "PLACED",
  load: loadFloorplan,
  bounds: (base) => [0, 0, base.width, base.height],
  groups: [
    { id: "tiles", label: "Tiles" },
    { id: "blocks", label: "Blocks" },
    { id: "labels", label: "Names" },
    { id: "regions", label: "Regions" },
  ],
  key(base, view, hidden) {
    const level = levelFor(view.scale, OBJECT_PX, base.width, base.height);
    const tiles =
      level === 0
        ? ""
        : visibleTiles(view, level, base.width, base.height).join(";");
    return `${level}|${tiles}|${[...hidden].sort().join(",")}`;
  },
  build(context): Built {
    const { base, view, colors, hidden } = context;
    const level = levelFor(view.scale, OBJECT_PX, base.width, base.height);
    if (!base.objects || base.objects.dark !== colors.dark)
      base.objects = { ...objectLayers(base, colors.dark), dark: colors.dark };
    const styles = base.objects.styles;
    let layers: Layer[];
    let loading = 0;
    if (level === 0)
      layers = base.objects.layers.filter((l) => !hidden.has(l.id));
    else {
      const coarse = coarseLayers(context, level);
      layers = [
        ...coarse.layers,
        ...base.objects.layers.filter(
          (l) => l.id === "regions" && !hidden.has("regions"),
        ),
      ];
      loading = coarse.loading;
    }
    layers.push(...selectionLayers(context, styles));
    const status = [
      level === 0
        ? "every tile and block"
        : `level ${level}: a cell is ${1 << level}×${1 << level} tiles`,
      base.unplaced
        ? "not placed yet: the tiles only"
        : `${base.blockRow.size} blocks`,
      loading ? `${loading} tiles loading` : "",
    ].filter(Boolean);
    return {
      scene: {
        layers,
        bounds: [0, 0, base.width, base.height],
        styles: styles.list,
      },
      status: status.join(" · "),
      legend: floorplanLegend(context, level),
    };
  },
  locate(base, ref, changed) {
    if (ref.kind === "block") {
      const row = base.blockRow.get(ref.name);
      return row === undefined ? null : blockBox(base, row);
    }
    if (ref.kind === "net") {
      const clusters = wholeViewNow("clusters", changed);
      return clusters ? netBox(base, clusters, ref.name) : null;
    }
    return null;
  },
  describe(base, ref: ObjectRef) {
    if (ref.kind === "block" && base.placement) {
      const row = base.blockRow.get(ref.name);
      if (row === undefined) return `block ${ref.name}`;
      const type = texts(base.placement, "block_types", "name").get(
        numbers(base.placement, "blocks", "type")[row],
      );
      const x = numbers(base.placement, "blocks", "x")[row];
      const y = numbers(base.placement, "blocks", "y")[row];
      const slot = numbers(base.placement, "blocks", "sub_tile")[row];
      const fixed = numbers(base.placement, "blocks", "fixed")[row]
        ? ", fixed"
        : "";
      return `${ref.name} (${type}) at ${x},${y} slot ${slot}${fixed}`;
    }
    return `${ref.kind} ${ref.name}`;
  },
};
