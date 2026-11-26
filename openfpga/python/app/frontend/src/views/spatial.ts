// The spatial index of a scene (docs/plan_design_views.md, 3.1 and P2.2): a flatbush index for each
// layer, which is the truth for picking (both renderers pick the same), and gives the items on the
// screen for culling and for the labels.
import Flatbush from "flatbush";
import { type Layer, type Scene, itemBox, itemCount } from "./scene";

export interface Picked {
  layer: Layer;
  item: number;
  /** Distance to the point, in world units: 0 inside a box */
  distance: number;
}

export class LayerIndex {
  readonly layer: Layer;
  private readonly index: Flatbush | null;

  constructor(layer: Layer) {
    this.layer = layer;
    const count = itemCount(layer);
    if (count === 0) {
      this.index = null;
      return;
    }
    const index = new Flatbush(count);
    for (let i = 0; i < count; i++) {
      const [x0, y0, x1, y1] = itemBox(layer, i);
      index.add(x0, y0, x1, y1);
    }
    index.finish();
    this.index = index;
  }

  /** The items whose box meets a box of the world */
  search(box: [number, number, number, number]): number[] {
    return this.index ? this.index.search(box[0], box[1], box[2], box[3]) : [];
  }

  /** The item nearest to a point, no farther than tolerance (world units), or null */
  pick(x: number, y: number, tolerance: number): Picked | null {
    if (!this.index) return null;
    let best: Picked | null = null;
    for (const item of this.index.search(
      x - tolerance,
      y - tolerance,
      x + tolerance,
      y + tolerance,
    )) {
      const distance = itemDistance(this.layer, item, x, y);
      if (distance <= tolerance && (!best || distance < best.distance))
        best = { layer: this.layer, item, distance };
    }
    return best;
  }
}

function segmentDistance(
  px: number,
  py: number,
  x1: number,
  y1: number,
  x2: number,
  y2: number,
): number {
  const dx = x2 - x1;
  const dy = y2 - y1;
  const length = dx * dx + dy * dy;
  const t =
    length > 0
      ? Math.max(0, Math.min(1, ((px - x1) * dx + (py - y1) * dy) / length))
      : 0;
  return Math.hypot(px - (x1 + t * dx), py - (y1 + t * dy));
}

function itemDistance(layer: Layer, i: number, x: number, y: number): number {
  if (layer.kind === "segments") {
    const p = layer.xy;
    return segmentDistance(
      x,
      y,
      p[i * 4],
      p[i * 4 + 1],
      p[i * 4 + 2],
      p[i * 4 + 3],
    );
  }
  if (layer.kind === "polylines") {
    let best = Infinity;
    for (let p = layer.start[i]; p < layer.start[i + 1] - 1; p++) {
      best = Math.min(
        best,
        segmentDistance(
          x,
          y,
          layer.xy[p * 2],
          layer.xy[p * 2 + 1],
          layer.xy[p * 2 + 2],
          layer.xy[p * 2 + 3],
        ),
      );
    }
    return best;
  }
  const [x0, y0, x1, y1] = itemBox(layer, i);
  return Math.hypot(Math.max(x0 - x, 0, x - x1), Math.max(y0 - y, 0, y - y1));
}

/** The indexes of the layers of a scene, by the id of the layer */
export class SceneIndex {
  private readonly layers = new Map<string, LayerIndex>();

  constructor(scene: Scene) {
    for (const layer of scene.layers) {
      if (layer.kind !== "grid")
        this.layers.set(layer.id, new LayerIndex(layer));
    }
  }

  get(id: string): LayerIndex | undefined {
    return this.layers.get(id);
  }

  /** The item under a point, of the layers that are drawn (the last layer drawn is on top) and are
   * not inert; tolerance in world units */
  pick(
    layers: Layer[],
    x: number,
    y: number,
    tolerance: number,
  ): Picked | null {
    for (let i = layers.length - 1; i >= 0; i--) {
      const layer = layers[i];
      if (layer.inert || !layer.objectKind) continue;
      const found = this.layers.get(layer.id)?.pick(x, y, tolerance);
      if (found) return found;
    }
    return null;
  }
}
