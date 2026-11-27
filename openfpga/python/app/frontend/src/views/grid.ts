// The grid of the device as the views of the device draw it: the floorplan, the placement and the
// routing draw the same tiles, in the colours of their types, with the same legend, and the
// coordinates of each tile when it is large enough on the screen.
import { type ViewData, numbers, texts } from "./format";
import { logicBox } from "./geometry";
import { tileFill } from "./palette";
import type { LabelsLayer, RectsLayer } from "./scene";
import type { LegendEntry, StyleTable } from "./view";

/** The tiles are named from this many pixels a grid tile */
export const COORDINATE_PX = 44;
/** A grid with more tiles than this has no coordinates (too many texts) */
const COORDINATE_LIMIT = 50000;

/** The styles of the types of tiles, added to styles in the order of the types: a view that adds
 * them first to a new table each time gets the same indexes, and can keep its layer of tiles */
export function tileStyles(
  device: ViewData,
  styles: StyleTable,
  dark: boolean,
): number[] {
  const isIo = numbers(device, "tile_types", "is_io");
  return Array.from(isIo, (io, t) =>
    styles.add(`tile:${t}`, {
      fill: tileFill(t, !!io, dark),
      stroke: dark ? [90, 95, 110, 255] : [170, 170, 165, 255],
      width: 1,
    }),
  );
}

/** The tiles of the device, in the colours of their types: the layer "tiles" and its legend (a
 * heading, then each type there is) */
export function tileLayer(
  device: ViewData,
  styles: StyleTable,
  dark: boolean,
  extra: Partial<RectsLayer> = {},
): { layer: RectsLayer; legend: LegendEntry[] } {
  const type = numbers(device, "tiles", "type");
  const names = texts(device, "tile_types", "name");
  const isIo = numbers(device, "tile_types", "is_io");
  const tx = numbers(device, "tiles", "x");
  const ty = numbers(device, "tiles", "y");
  const tw = numbers(device, "tiles", "w");
  const th = numbers(device, "tiles", "h");
  const n = type.length;
  const layer: RectsLayer = {
    id: "tiles",
    kind: "rects",
    x: new Float32Array(n),
    y: new Float32Array(n),
    w: new Float32Array(n),
    h: new Float32Array(n),
    style: new Uint16Array(n),
    objectKind: "tile",
    name: (i) => `${names.get(type[i])} ${tx[i]},${ty[i]}`,
    size: 0.6,
    minPx: 2,
    ...extra,
  };
  const style = tileStyles(device, styles, dark);
  for (let i = 0; i < n; i++) {
    layer.style[i] = style[type[i]];
    const [x0, y0, x1, y1] = logicBox(tx[i], ty[i], tw[i], th[i]);
    layer.x[i] = x0;
    layer.y[i] = y0;
    layer.w[i] = x1 - x0;
    layer.h[i] = y1 - y0;
  }
  const legend: LegendEntry[] = [
    { label: "Tiles" },
    ...[...new Set(type)]
      .sort((a, b) => a - b)
      .map((t) => ({
        label: names.get(t),
        fill: tileFill(t, !!isIo[t], dark),
      })),
  ];
  return { layer, legend };
}

/** The coordinates x,y of each tile, above its logic (in the band of its channel), when a grid
 * tile is COORDINATE_PX on the screen; null for a grid that is too large */
export function coordinateLayer(
  device: ViewData,
  styles: StyleTable,
  dark: boolean,
): LabelsLayer | null {
  const tx = numbers(device, "tiles", "x");
  const ty = numbers(device, "tiles", "y");
  const tw = numbers(device, "tiles", "w");
  const th = numbers(device, "tiles", "h");
  const n = tx.length;
  if (n > COORDINATE_LIMIT) return null;
  const x = new Float32Array(n);
  const y = new Float32Array(n);
  for (let i = 0; i < n; i++) {
    const [x0, , x1, y1] = logicBox(tx[i], ty[i], tw[i], th[i]);
    x[i] = (x0 + x1) / 2;
    y[i] = y1 + 0.17;
  }
  return {
    id: "coordinates",
    kind: "labels",
    x,
    y,
    text: (i) => `${tx[i]},${ty[i]}`,
    count: n,
    px: 10,
    style: new Uint16Array(n).fill(
      styles.add("coordinate", {
        fill: dark ? [150, 155, 165, 255] : [120, 120, 115, 255],
      }),
    ),
    size: 1,
    minPx: COORDINATE_PX,
    inert: true,
  };
}
