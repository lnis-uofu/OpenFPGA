// The scene of a view (docs/plan_design_views.md, 3.2): layers of typed arrays that both renderers
// draw, with no knowledge of netlists or devices. Each item of a layer is an object of the design
// (a block, a net, a tile...), named for the selection that all the views share.

/** The kinds of objects of the design that a view shows and selects */
export type ObjectKind =
  | "block"
  | "net"
  | "tile"
  | "site"
  | "port"
  | "node"
  | "region"
  | "cell"
  | "atom"
  | "path";

/** An object, by its kind and its name: the key across views (3.3) */
export interface ObjectRef {
  kind: ObjectKind;
  name: string;
}

export const refKey = (ref: ObjectRef) => `${ref.kind}:${ref.name}`;

/** An outline of a shape, in its own unit box (0..1), as a closed ring of x, y */
export interface ShapeDef {
  name: string;
  outline: number[];
}

interface LayerBase {
  /** Stable across scenes: renderers keep what they made for it */
  id: string;
  /** The kind of the objects of its items, and the name of each (for the selection, the tooltip) */
  objectKind?: ObjectKind;
  name?: (item: number) => string;
  /** The size of its typical item in world units, and the sizes on the screen in pixels that it is
   * drawn between: the rule of the levels of detail (3.2) */
  size: number;
  minPx?: number;
  maxPx?: number;
  /** Not picked by a click or a hover (a background, an overlay of the selection) */
  inert?: boolean;
}

export interface RectsLayer extends LayerBase {
  kind: "rects";
  x: Float32Array;
  y: Float32Array;
  w: Float32Array;
  h: Float32Array;
  style: Uint16Array;
}

/** Instances of shapes: each item is a shape of `shapes` in the box x, y, w, h */
export interface ShapesLayer extends LayerBase {
  kind: "shapes";
  x: Float32Array;
  y: Float32Array;
  w: Float32Array;
  h: Float32Array;
  shape: Uint16Array;
  shapes: ShapeDef[];
  style: Uint16Array;
}

export interface SegmentsLayer extends LayerBase {
  kind: "segments";
  /** x1, y1, x2, y2 of each item */
  xy: Float32Array;
  style: Uint16Array;
}

export interface PolylinesLayer extends LayerBase {
  kind: "polylines";
  /** The points of all the items, x, y; item i is the points start[i] to start[i + 1] */
  xy: Float32Array;
  start: Uint32Array;
  style: Uint16Array;
}

export interface PointsLayer extends LayerBase {
  kind: "points";
  x: Float32Array;
  y: Float32Array;
  /** The radius on the screen, in pixels */
  radius: number;
  style: Uint16Array;
}

export interface LabelsLayer extends LayerBase {
  kind: "labels";
  x: Float32Array;
  y: Float32Array;
  text: (item: number) => string;
  count: number;
  /** The height of the text on the screen, in pixels */
  px: number;
  style: Uint16Array;
}

/** Values on a grid of cells, drawn as one image through a palette: a coarse level of a tile */
export interface GridLayer extends LayerBase {
  kind: "grid";
  x0: number;
  y0: number;
  cell: number;
  cols: number;
  rows: number;
  /** By row from the bottom: value[r * cols + c]; NaN is drawn as nothing */
  value: Float32Array;
  palette: Palette;
}

/** How the values of a grid are coloured: a ramp from min to max, or a colour a category */
export type Palette =
  | { kind: "ramp"; min: number; max: number; ramp: Ramp }
  | { kind: "categorical"; colors: Rgba[] };

export type Ramp = "heat" | "slack" | "use";

export type Layer =
  | RectsLayer
  | ShapesLayer
  | SegmentsLayer
  | PolylinesLayer
  | PointsLayer
  | LabelsLayer
  | GridLayer;

export interface Scene {
  layers: Layer[];
  /** The box of the world: xmin, ymin, xmax, ymax */
  bounds: [number, number, number, number];
  /** The styles of the items, by the index in their style column */
  styles: Style[];
}

/** Red, green, blue, alpha, each 0..255 */
export type Rgba = [number, number, number, number];

export interface Style {
  fill?: Rgba;
  stroke?: Rgba;
  /** The width of a stroke or a line on the screen, in pixels */
  width?: number;
}

/** The number of items of a layer */
export function itemCount(layer: Layer): number {
  switch (layer.kind) {
    case "rects":
    case "shapes":
    case "points":
      return layer.x.length;
    case "segments":
      return layer.xy.length / 4;
    case "polylines":
      return Math.max(0, layer.start.length - 1);
    case "labels":
      return layer.count;
    case "grid":
      return 0;
  }
}

/** The box of an item: xmin, ymin, xmax, ymax */
export function itemBox(
  layer: Layer,
  i: number,
): [number, number, number, number] {
  switch (layer.kind) {
    case "rects":
    case "shapes":
      return [
        layer.x[i],
        layer.y[i],
        layer.x[i] + layer.w[i],
        layer.y[i] + layer.h[i],
      ];
    case "segments": {
      const [x1, y1, x2, y2] = [
        layer.xy[i * 4],
        layer.xy[i * 4 + 1],
        layer.xy[i * 4 + 2],
        layer.xy[i * 4 + 3],
      ];
      return [
        Math.min(x1, x2),
        Math.min(y1, y2),
        Math.max(x1, x2),
        Math.max(y1, y2),
      ];
    }
    case "polylines": {
      let box: [number, number, number, number] = [
        Infinity,
        Infinity,
        -Infinity,
        -Infinity,
      ];
      for (let p = layer.start[i]; p < layer.start[i + 1]; p++) {
        const x = layer.xy[p * 2];
        const y = layer.xy[p * 2 + 1];
        box = [
          Math.min(box[0], x),
          Math.min(box[1], y),
          Math.max(box[2], x),
          Math.max(box[3], y),
        ];
      }
      return box;
    }
    case "points":
    case "labels":
      return [layer.x[i], layer.y[i], layer.x[i], layer.y[i]];
    case "grid":
      return [
        layer.x0,
        layer.y0,
        layer.x0 + layer.cols * layer.cell,
        layer.y0 + layer.rows * layer.cell,
      ];
  }
}

/** True when a layer is drawn at a scale (pixels a world unit): its typical item is between its
 * minPx and maxPx on the screen */
export function layerVisible(layer: Layer, scale: number): boolean {
  const px = layer.size * scale;
  return (
    (layer.minPx === undefined || px >= layer.minPx) &&
    (layer.maxPx === undefined || px < layer.maxPx)
  );
}
