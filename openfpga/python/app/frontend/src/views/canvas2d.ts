// The Canvas2D renderer (docs/plan_design_views.md, 3.4): every layer kind, the items on the screen
// only (from the spatial index), one path a style. It is the fallback of deck.gl and the reference of
// the parity tests.
import { css, paletteColor, viewColors } from "./palette";
import type { Renderer } from "./renderer";
import {
  type GridLayer,
  type Layer,
  type Scene,
  itemCount,
  layerVisible,
} from "./scene";
import type { SceneIndex } from "./spatial";
import { type ViewState, toScreen, visibleBox } from "./viewstate";

/** Below this, a layer draws all its items with no search of the index */
const SEARCH_FROM = 2000;

export class Canvas2dRenderer implements Renderer {
  readonly kind = "canvas2d" as const;
  private canvas: HTMLCanvasElement | null = null;
  private scene: Scene | null = null;
  private index: SceneIndex | null = null;
  private images = new WeakMap<GridLayer, HTMLCanvasElement>();
  onLost?: () => void;

  mount(host: HTMLElement) {
    this.canvas = document.createElement("canvas");
    this.canvas.className = "view-canvas";
    host.appendChild(this.canvas);
  }

  setScene(scene: Scene, index: SceneIndex) {
    this.scene = scene;
    this.index = index;
  }

  render(view: ViewState) {
    const canvas = this.canvas;
    if (!canvas || !this.scene) return;
    const dpr = window.devicePixelRatio || 1;
    const w = Math.max(1, Math.round(view.width * dpr));
    const h = Math.max(1, Math.round(view.height * dpr));
    if (canvas.width !== w || canvas.height !== h) {
      canvas.width = w;
      canvas.height = h;
      canvas.style.width = `${view.width}px`;
      canvas.style.height = `${view.height}px`;
    }
    const ctx = canvas.getContext("2d");
    if (!ctx) return;
    const colors = viewColors();
    ctx.setTransform(1, 0, 0, 1, 0, 0);
    ctx.fillStyle = css(colors.background);
    ctx.fillRect(0, 0, w, h);
    const box = visibleBox(view);
    for (const layer of this.scene.layers) {
      if (!layerVisible(layer, view.scale)) continue;
      // The world: y up, a unit is scale pixels
      const s = view.scale * dpr;
      ctx.setTransform(
        s,
        0,
        0,
        -s,
        (view.width / 2 - view.cx * view.scale) * dpr,
        (view.height / 2 + view.cy * view.scale) * dpr,
      );
      this.draw(ctx, layer, view, box, dpr);
    }
  }

  private items(
    layer: Layer,
    box: [number, number, number, number],
  ): Iterable<number> {
    const count = itemCount(layer);
    if (count < SEARCH_FROM || !this.index)
      return Array.from({ length: count }, (_, i) => i);
    return this.index.get(layer.id)?.search(box) ?? [];
  }

  /** The items of a layer by their style */
  private byStyle(
    layer: Layer & { style: Uint16Array },
    box: [number, number, number, number],
  ): Map<number, number[]> {
    const groups = new Map<number, number[]>();
    for (const i of this.items(layer, box)) {
      const style = layer.style[i];
      let group = groups.get(style);
      if (!group) groups.set(style, (group = []));
      group.push(i);
    }
    return groups;
  }

  private draw(
    ctx: CanvasRenderingContext2D,
    layer: Layer,
    view: ViewState,
    box: [number, number, number, number],
    dpr: number,
  ) {
    const styles = this.scene!.styles;
    const px = 1 / view.scale; // a pixel of the screen, in world units
    switch (layer.kind) {
      case "rects":
      case "shapes":
        for (const [s, items] of this.byStyle(layer, box)) {
          const style = styles[s] ?? {};
          const path = new Path2D();
          for (const i of items) {
            if (layer.kind === "rects")
              path.rect(layer.x[i], layer.y[i], layer.w[i], layer.h[i]);
            else {
              const outline = layer.shapes[layer.shape[i]]?.outline ?? [
                0, 0, 1, 0, 1, 1, 0, 1,
              ];
              for (let p = 0; p < outline.length; p += 2) {
                const x = layer.x[i] + outline[p] * layer.w[i];
                const y = layer.y[i] + outline[p + 1] * layer.h[i];
                if (p === 0) path.moveTo(x, y);
                else path.lineTo(x, y);
              }
              path.closePath();
            }
          }
          if (style.fill) {
            ctx.fillStyle = css(style.fill);
            ctx.fill(path);
          }
          if (style.stroke) {
            ctx.strokeStyle = css(style.stroke);
            ctx.lineWidth = (style.width ?? 1) * px;
            ctx.stroke(path);
          }
        }
        return;
      case "segments":
      case "polylines":
        for (const [s, items] of this.byStyle(layer, box)) {
          const style = styles[s] ?? {};
          const path = new Path2D();
          for (const i of items) {
            if (layer.kind === "segments") {
              path.moveTo(layer.xy[i * 4], layer.xy[i * 4 + 1]);
              path.lineTo(layer.xy[i * 4 + 2], layer.xy[i * 4 + 3]);
            } else {
              for (let p = layer.start[i]; p < layer.start[i + 1]; p++) {
                if (p === layer.start[i])
                  path.moveTo(layer.xy[p * 2], layer.xy[p * 2 + 1]);
                else path.lineTo(layer.xy[p * 2], layer.xy[p * 2 + 1]);
              }
            }
          }
          ctx.strokeStyle = css(
            style.stroke ?? style.fill ?? [128, 128, 128, 255],
          );
          ctx.lineWidth = (style.width ?? 1) * px;
          ctx.stroke(path);
        }
        return;
      case "points":
        for (const [s, items] of this.byStyle(layer, box)) {
          const style = styles[s] ?? {};
          const path = new Path2D();
          const r = layer.radius * px;
          for (const i of items) {
            path.moveTo(layer.x[i] + r, layer.y[i]);
            path.arc(layer.x[i], layer.y[i], r, 0, 2 * Math.PI);
          }
          ctx.fillStyle = css(style.fill ?? [128, 128, 128, 255]);
          ctx.fill(path);
        }
        return;
      case "labels": {
        // On the screen: the world is upside down for text
        ctx.setTransform(dpr, 0, 0, dpr, 0, 0);
        ctx.font = `${layer.px}px system-ui, sans-serif`;
        ctx.textAlign = "center";
        ctx.textBaseline = "middle";
        for (const i of this.items(layer, box)) {
          const [x, y] = toScreen(view, layer.x[i], layer.y[i]);
          ctx.fillStyle = css(
            styles[layer.style[i]]?.fill ?? [200, 200, 200, 255],
          );
          ctx.fillText(layer.text(i), x, y);
        }
        return;
      }
      case "grid": {
        let image = this.images.get(layer);
        if (!image) {
          image = gridImage(layer, false);
          this.images.set(layer, image);
        }
        ctx.imageSmoothingEnabled = false;
        ctx.drawImage(
          image,
          layer.x0,
          layer.y0,
          layer.cols * layer.cell,
          layer.rows * layer.cell,
        );
        return;
      }
    }
  }

  destroy() {
    this.canvas?.remove();
    this.canvas = null;
  }
}

/** The image of the values of a grid: a pixel a cell. Row 0 of the values is the bottom row of the
 * world: with topDown, row 0 of the image is the top one (an image of deck.gl), else the bottom one
 * (drawn in the world of Canvas2D, y up) */
export function gridImage(
  layer: GridLayer,
  topDown: boolean,
): HTMLCanvasElement {
  const canvas = document.createElement("canvas");
  canvas.width = Math.max(1, layer.cols);
  canvas.height = Math.max(1, layer.rows);
  const ctx = canvas.getContext("2d");
  if (!ctx) return canvas;
  const image = ctx.createImageData(canvas.width, canvas.height);
  for (let r = 0; r < layer.rows; r++) {
    const row = topDown ? layer.rows - 1 - r : r;
    for (let c = 0; c < layer.cols; c++) {
      const color = paletteColor(
        layer.palette,
        layer.value[r * layer.cols + c],
      );
      if (color) image.data.set(color, (row * layer.cols + c) * 4);
    }
  }
  ctx.putImageData(image, 0, 0);
  return canvas;
}
