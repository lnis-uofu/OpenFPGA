// The deck.gl renderer (docs/plan_design_views.md, 3.4): every layer kind, from the typed arrays of
// the scene as binary attributes. The deck.gl layers of a scene layer are made once and kept; a
// change of the level of detail only toggles them, with no upload. Picking is not deck.gl's: the
// spatial index of the scene does it for both renderers.
import { Deck, type Layer as DeckLayer, OrthographicView } from "@deck.gl/core";
import {
  BitmapLayer,
  LineLayer,
  PathLayer,
  ScatterplotLayer,
  SolidPolygonLayer,
  TextLayer,
} from "@deck.gl/layers";
import { gridImage } from "./canvas2d";
import { viewColors } from "./palette";
import type { Renderer } from "./renderer";
import {
  type Layer,
  type Rgba,
  type Scene,
  type Style,
  itemCount,
  layerVisible,
} from "./scene";
import type { SceneIndex } from "./spatial";
import type { ViewState } from "./viewstate";

const GREY: Rgba = [128, 128, 128, 255];

/** The colours of the items of a layer, repeated for each vertex of an item */
function colors(
  styles: Style[],
  style: Uint16Array,
  pick: (s: Style) => Rgba | undefined,
  repeat = 1,
): Uint8Array {
  const out = new Uint8Array(style.length * 4 * repeat);
  for (let i = 0; i < style.length; i++) {
    const c = pick(styles[style[i]] ?? {}) ?? [0, 0, 0, 0];
    for (let k = 0; k < repeat; k++) out.set(c, (i * repeat + k) * 4);
  }
  return out;
}

function widths(styles: Style[], style: Uint16Array): Float32Array {
  const out = new Float32Array(style.length);
  for (let i = 0; i < style.length; i++) out[i] = styles[style[i]]?.width ?? 1;
  return out;
}

/** The width of a path, repeated for each of its vertices: deck.gl reads a binary attribute as one
 * value a vertex, and a path with fewer values than vertices loses the segments after them */
function vertexWidths(
  styles: Style[],
  style: Uint16Array,
  start: Uint32Array,
): Float32Array {
  const out = new Float32Array(start[style.length] ?? 0);
  for (let i = 0; i < style.length; i++)
    out.fill(styles[style[i]]?.width ?? 1, start[i], start[i + 1]);
  return out;
}

/** The outlines of the items of rects or shapes, as closed rings in one buffer */
function rings(layer: Extract<Layer, { kind: "rects" | "shapes" }>): {
  positions: Float32Array;
  starts: Uint32Array;
  points: number[];
} {
  const count = layer.x.length;
  const points: number[] = [];
  const all: number[] = [];
  const starts = new Uint32Array(count + 1);
  for (let i = 0; i < count; i++) {
    const outline =
      layer.kind === "rects"
        ? [0, 0, 1, 0, 1, 1, 0, 1]
        : (layer.shapes[layer.shape[i]]?.outline ?? [0, 0, 1, 0, 1, 1, 0, 1]);
    const n = outline.length / 2;
    for (let p = 0; p <= n; p++) {
      const q = (p % n) * 2;
      all.push(
        layer.x[i] + outline[q] * layer.w[i],
        layer.y[i] + outline[q + 1] * layer.h[i],
      );
    }
    starts[i + 1] = starts[i] + n + 1;
    points.push(n + 1);
  }
  return { positions: new Float32Array(all), starts, points };
}

/** The edges of rings as segments (the strokes of rects and shapes) */
function edges(
  positions: Float32Array,
  starts: Uint32Array,
): { xy: Float32Array; owner: Uint32Array } {
  const xy: number[] = [];
  const owner: number[] = [];
  for (let i = 0; i < starts.length - 1; i++) {
    for (let p = starts[i]; p < starts[i + 1] - 1; p++) {
      xy.push(
        positions[p * 2],
        positions[p * 2 + 1],
        positions[p * 2 + 2],
        positions[p * 2 + 3],
      );
      owner.push(i);
    }
  }
  return { xy: new Float32Array(xy), owner: new Uint32Array(owner) };
}

/** The deck.gl layers of a layer of the scene */
function deckLayers(layer: Layer, styles: Style[]): DeckLayer[] {
  const id = layer.id;
  switch (layer.kind) {
    case "rects":
    case "shapes": {
      const count = layer.x.length;
      if (!count) return [];
      const { positions, starts, points } = rings(layer);
      const fill = new Uint8Array(starts[count] * 4);
      const stroke = new Uint8Array(count * 4);
      let anyFill = false;
      let anyStroke = false;
      for (let i = 0; i < count; i++) {
        const style = styles[layer.style[i]] ?? {};
        if (style.fill) {
          anyFill = true;
          for (let k = 0; k < points[i]; k++)
            fill.set(style.fill, (starts[i] + k) * 4);
        }
        if (style.stroke) {
          anyStroke = true;
          stroke.set(style.stroke, i * 4);
        }
      }
      const out: DeckLayer[] = [];
      if (anyFill) {
        out.push(
          new SolidPolygonLayer({
            id: `${id}:fill`,
            data: {
              length: count,
              startIndices: starts,
              attributes: {
                getPolygon: { value: positions, size: 2 },
                getFillColor: { value: fill, size: 4, normalized: true },
              },
            },
            _normalize: false,
          }),
        );
      }
      if (anyStroke) {
        const { xy, owner } = edges(positions, starts);
        const color = new Uint8Array(owner.length * 4);
        const width = new Float32Array(owner.length);
        for (let e = 0; e < owner.length; e++) {
          color.set(stroke.subarray(owner[e] * 4, owner[e] * 4 + 4), e * 4);
          width[e] = styles[layer.style[owner[e]]]?.width ?? 1;
        }
        out.push(lines(`${id}:stroke`, xy, color, width));
      }
      return out;
    }
    case "segments": {
      const count = layer.xy.length / 4;
      if (!count) return [];
      return [
        lines(
          id,
          layer.xy,
          colors(styles, layer.style, (s) => s.stroke ?? s.fill ?? GREY),
          widths(styles, layer.style),
        ),
      ];
    }
    case "polylines": {
      const count = itemCount(layer);
      if (!count) return [];
      const perVertex = new Uint8Array((layer.xy.length / 2) * 4);
      for (let i = 0; i < count; i++) {
        const c =
          styles[layer.style[i]]?.stroke ??
          styles[layer.style[i]]?.fill ??
          GREY;
        for (let p = layer.start[i]; p < layer.start[i + 1]; p++)
          perVertex.set(c, p * 4);
      }
      return [
        new PathLayer({
          id,
          data: {
            length: count,
            startIndices: layer.start,
            attributes: {
              getPath: { value: layer.xy, size: 2 },
              getColor: { value: perVertex, size: 4, normalized: true },
              getWidth: {
                value: vertexWidths(styles, layer.style, layer.start),
                size: 1,
              },
            },
          },
          widthUnits: "pixels",
          _pathType: "open",
        }),
      ];
    }
    case "points": {
      const count = layer.x.length;
      if (!count) return [];
      const position = new Float32Array(count * 2);
      for (let i = 0; i < count; i++)
        position.set([layer.x[i], layer.y[i]], i * 2);
      return [
        new ScatterplotLayer({
          id,
          data: {
            length: count,
            attributes: {
              getPosition: { value: position, size: 2 },
              getFillColor: {
                value: colors(styles, layer.style, (s) => s.fill ?? GREY),
                size: 4,
                normalized: true,
              },
            },
          },
          radiusUnits: "pixels",
          getRadius: layer.radius,
        }),
      ];
    }
    case "labels": {
      const data = Array.from({ length: layer.count }, (_, i) => ({
        position: [layer.x[i], layer.y[i]],
        text: layer.text(i),
        color: styles[layer.style[i]]?.fill ?? [200, 200, 200, 255],
      }));
      return [
        new TextLayer({
          id,
          data,
          getPosition: (d: { position: number[] }) =>
            d.position as [number, number],
          getText: (d: { text: string }) => d.text,
          getColor: (d: { color: Rgba }) => d.color,
          getSize: layer.px,
          sizeUnits: "pixels",
          fontFamily: "system-ui, sans-serif",
          getTextAnchor: "middle",
          getAlignmentBaseline: "center",
        }),
      ];
    }
    case "grid":
      return [
        new BitmapLayer({
          id,
          image: gridImage(layer, true),
          bounds: [
            layer.x0,
            layer.y0,
            layer.x0 + layer.cols * layer.cell,
            layer.y0 + layer.rows * layer.cell,
          ],
          textureParameters: { minFilter: "nearest", magFilter: "nearest" },
        }),
      ];
  }
}

function lines(
  id: string,
  xy: Float32Array,
  color: Uint8Array,
  width: Float32Array,
): DeckLayer {
  return new LineLayer({
    id,
    data: {
      length: xy.length / 4,
      attributes: {
        getSourcePosition: { value: xy, size: 2, stride: 16, offset: 0 },
        getTargetPosition: { value: xy, size: 2, stride: 16, offset: 8 },
        getColor: { value: color, size: 4, normalized: true },
        getWidth: { value: width, size: 1 },
      },
    },
    widthUnits: "pixels",
  });
}

export class DeckRenderer implements Renderer {
  readonly kind = "deck" as const;
  private deck: Deck<OrthographicView> | null = null;
  private host: HTMLElement | null = null;
  private built: { layer: Layer; deck: DeckLayer[] }[] = [];
  private cache = new WeakMap<Layer, DeckLayer[]>();
  onLost?: () => void;

  mount(host: HTMLElement) {
    this.host = host;
    const background = viewColors().background;
    host.style.background = `rgb(${background[0]},${background[1]},${background[2]})`;
    this.deck = new Deck({
      parent: host as HTMLDivElement,
      width: host.clientWidth,
      height: host.clientHeight,
      views: new OrthographicView({ flipY: false }),
      controller: false,
      viewState: { target: [0, 0, 0], zoom: 0 },
      layers: [],
      onWebGLInitialized: () => {
        const canvas = host.querySelector("canvas");
        canvas?.classList.add("view-canvas");
        canvas?.addEventListener("webglcontextlost", (event) => {
          event.preventDefault();
          this.onLost?.();
        });
      },
      onError: () => this.onLost?.(),
    });
  }

  setScene(scene: Scene, _index: SceneIndex) {
    this.built = scene.layers.map((layer) => {
      let made = this.cache.get(layer);
      if (!made) {
        made = deckLayers(layer, scene.styles);
        this.cache.set(layer, made);
      }
      return { layer, deck: made };
    });
  }

  render(view: ViewState) {
    if (!this.deck) return;
    if (this.host) {
      const b = viewColors().background;
      this.host.style.background = `rgb(${b[0]},${b[1]},${b[2]})`;
    }
    const layers: DeckLayer[] = [];
    for (const entry of this.built) {
      const visible = layerVisible(entry.layer, view.scale);
      entry.deck = entry.deck.map((d) =>
        d.props.visible === visible ? d : d.clone({ visible }),
      );
      layers.push(...entry.deck);
    }
    this.deck.setProps({
      width: view.width,
      height: view.height,
      viewState: { target: [view.cx, view.cy, 0], zoom: Math.log2(view.scale) },
      layers,
    });
  }

  destroy() {
    this.deck?.finalize();
    this.deck = null;
    this.host = null;
  }
}
