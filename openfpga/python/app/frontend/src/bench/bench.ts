// The renderer benchmark of docs/plan_design_views.md (P0.2): synthetic scenes at the sizes that
// phase 0 measured, drawn by deck.gl (WebGL2) and by Canvas2D, through a zoom sweep. Dev only: open
// /bench.html on the development server (npm run dev), or run e2e/bench.mjs for a table.
//
// ?scene=NAME&backend=deck|canvas2d runs one; with neither, "Run all" runs every pair. The results
// are in window.benchResults.
import { Deck, OrthographicView } from "@deck.gl/core";
import { BitmapLayer, LineLayer, SolidPolygonLayer } from "@deck.gl/layers";

interface Scene {
  name: string;
  world: number; // the side of the world, in grid units
  rects: Float32Array; // x, y, w, h
  rectColor: Uint8Array; // r, g, b, a
  segments: Float32Array; // x1, y1, x2, y2
  segmentColor: Uint8Array;
  grid?: { cols: number; rows: number; values: Float32Array };
}

interface Result {
  scene: string;
  backend: string;
  items: number;
  buildMs: number;
  p50: number;
  p95: number;
  worst: number;
}

// A random number generator with a seed: the scenes are the same each time
function random(seed: number) {
  return () => {
    seed = (seed * 1664525 + 1013904223) >>> 0;
    return seed / 4294967296;
  };
}

/** A device of side n: its tiles, the blocks placed on them, and the routed wires (L4) */
function deviceScene(
  name: string,
  n: number,
  wiresPerTile: number,
  withGrid: boolean,
): Scene {
  const r = random(n * 7 + wiresPerTile);
  const tiles = n * n;
  const rects = new Float32Array(tiles * 2 * 4);
  const rectColor = new Uint8Array(tiles * 2 * 4);
  let k = 0;
  for (let y = 0; y < n; y++) {
    for (let x = 0; x < n; x++) {
      const edge = x === 0 || y === 0 || x === n - 1 || y === n - 1;
      rects.set([x + 0.1, y + 0.1, 0.8, 0.8], k * 4);
      rectColor.set(edge ? [70, 90, 140, 255] : [60, 60, 70, 255], k * 4);
      k++;
      if (r() < 0.8) {
        rects.set([x + 0.25, y + 0.25, 0.5, 0.5], k * 4);
        rectColor.set([200, 140, 60, 255], k * 4);
        k++;
      }
    }
  }
  const count = Math.round(tiles * wiresPerTile);
  const segments = new Float32Array(count * 4);
  const segmentColor = new Uint8Array(count * 4);
  for (let i = 0; i < count; i++) {
    const x = Math.floor(r() * n);
    const y = Math.floor(r() * n);
    const track = r() * 0.1;
    const length = 1 + Math.floor(r() * 4);
    if (r() < 0.5)
      segments.set(
        [x, y + 0.9 + track, Math.min(n, x + length), y + 0.9 + track],
        i * 4,
      );
    else
      segments.set(
        [x + 0.9 + track, y, x + 0.9 + track, Math.min(n, y + length)],
        i * 4,
      );
    const hot = r();
    segmentColor.set([80 + hot * 175, 200 - hot * 150, 120, 200], i * 4);
  }
  let grid: Scene["grid"];
  if (withGrid) {
    const values = new Float32Array(n * n);
    for (let i = 0; i < values.length; i++) values[i] = r();
    grid = { cols: n, rows: n, values };
  }
  return {
    name,
    world: n,
    rects: rects.subarray(0, k * 4),
    rectColor: rectColor.subarray(0, k * 4),
    segments,
    segmentColor,
    grid,
  };
}

const SCENES: Record<string, () => Scene> = {
  // The sizes of phase 0: a full 32x32 (spimaster), a 128x128 one, and a screen of level-0 tiles
  "floorplan-32": () => deviceScene("floorplan-32", 32, 0, false),
  "routing-32": () => deviceScene("routing-32", 32, 40, false), // ~40k routed wires
  "floorplan-128": () => deviceScene("floorplan-128", 128, 0, false),
  "routing-128-300k": () => deviceScene("routing-128-300k", 128, 18, false),
  "tracks-1m": () => deviceScene("tracks-1m", 128, 61, false), // a screen of channel tracks
  "heat-500": () => ({
    ...deviceScene("heat-500", 500, 0, true),
    rects: new Float32Array(0),
    rectColor: new Uint8Array(0),
  }),
};

/** The colours of a heat map, as an image */
function heatImage(grid: NonNullable<Scene["grid"]>): ImageData {
  const image = new ImageData(grid.cols, grid.rows);
  for (let i = 0; i < grid.values.length; i++) {
    const v = grid.values[i];
    image.data.set([255 * v, 80, 255 * (1 - v), 255], i * 4);
  }
  return image;
}

interface Backend {
  draw(view: { x: number; y: number; scale: number }): void;
  destroy(): void;
}

function canvas2d(scene: Scene, host: HTMLElement): Backend {
  const canvas = document.createElement("canvas");
  host.appendChild(canvas);
  const dpr = window.devicePixelRatio || 1;
  canvas.width = host.clientWidth * dpr;
  canvas.height = host.clientHeight * dpr;
  canvas.style.width = `${host.clientWidth}px`;
  canvas.style.height = `${host.clientHeight}px`;
  const ctx = canvas.getContext("2d")!;
  let heat: HTMLCanvasElement | null = null;
  if (scene.grid) {
    heat = document.createElement("canvas");
    heat.width = scene.grid.cols;
    heat.height = scene.grid.rows;
    heat.getContext("2d")!.putImageData(heatImage(scene.grid), 0, 0);
  }
  return {
    draw(view) {
      const s = view.scale * dpr;
      ctx.setTransform(1, 0, 0, 1, 0, 0);
      ctx.fillStyle = "#111";
      ctx.fillRect(0, 0, canvas.width, canvas.height);
      ctx.setTransform(
        s,
        0,
        0,
        s,
        canvas.width / 2 - view.x * s,
        canvas.height / 2 - view.y * s,
      );
      // What is on the screen only, as the spatial index would give it
      const x0 = view.x - canvas.width / 2 / s;
      const x1 = view.x + canvas.width / 2 / s;
      const y0 = view.y - canvas.height / 2 / s;
      const y1 = view.y + canvas.height / 2 / s;
      if (heat) {
        ctx.imageSmoothingEnabled = false;
        ctx.drawImage(heat, 0, 0, scene.grid!.cols, scene.grid!.rows);
      }
      // Rectangles by colour (a fill a colour), wires in one path a colour bucket
      const r = scene.rects;
      const c = scene.rectColor;
      let last = -1;
      for (let i = 0; i < r.length / 4; i++) {
        const x = r[i * 4];
        const y = r[i * 4 + 1];
        if (x > x1 || y > y1 || x + r[i * 4 + 2] < x0 || y + r[i * 4 + 3] < y0)
          continue;
        const key = (c[i * 4] << 16) | (c[i * 4 + 1] << 8) | c[i * 4 + 2];
        if (key !== last) {
          ctx.fillStyle = `rgb(${c[i * 4]},${c[i * 4 + 1]},${c[i * 4 + 2]})`;
          last = key;
        }
        ctx.fillRect(x, y, r[i * 4 + 2], r[i * 4 + 3]);
      }
      const g = scene.segments;
      if (g.length) {
        ctx.lineWidth = 1 / s;
        const buckets: Path2D[] = [
          new Path2D(),
          new Path2D(),
          new Path2D(),
          new Path2D(),
        ];
        for (let i = 0; i < g.length / 4; i++) {
          const ax = g[i * 4];
          const ay = g[i * 4 + 1];
          const bx = g[i * 4 + 2];
          const by = g[i * 4 + 3];
          if (
            Math.max(ax, bx) < x0 ||
            Math.min(ax, bx) > x1 ||
            Math.max(ay, by) < y0 ||
            Math.min(ay, by) > y1
          )
            continue;
          const path =
            buckets[
              Math.min(3, Math.floor((scene.segmentColor[i * 4] - 80) / 44))
            ];
          path.moveTo(ax, ay);
          path.lineTo(bx, by);
        }
        ["#5c8", "#9a6", "#c74", "#e44"].forEach((colour, i) => {
          ctx.strokeStyle = colour;
          ctx.stroke(buckets[i]);
        });
      }
    },
    destroy() {
      canvas.remove();
    },
  };
}

function deckBackend(scene: Scene, host: HTMLElement): Backend {
  const n = scene.rects.length / 4;
  // Each rectangle as a closed ring of 5 points, in one binary buffer
  const positions = new Float32Array(n * 10);
  const starts = new Uint32Array(n + 1);
  for (let i = 0; i < n; i++) {
    const [x, y, w, h] = scene.rects.subarray(i * 4, i * 4 + 4);
    positions.set([x, y, x + w, y, x + w, y + h, x, y + h, x, y], i * 10);
    starts[i + 1] = (i + 1) * 5;
  }
  const colors = new Uint8Array(n * 5 * 4);
  for (let i = 0; i < n; i++)
    for (let j = 0; j < 5; j++)
      colors.set(scene.rectColor.subarray(i * 4, i * 4 + 4), (i * 5 + j) * 4);
  const layers = [];
  if (scene.grid) {
    const canvas = document.createElement("canvas");
    canvas.width = scene.grid.cols;
    canvas.height = scene.grid.rows;
    canvas.getContext("2d")!.putImageData(heatImage(scene.grid), 0, 0);
    layers.push(
      new BitmapLayer({
        id: "heat",
        image: canvas,
        bounds: [0, scene.grid.rows, scene.grid.cols, 0],
        textureParameters: { minFilter: "nearest", magFilter: "nearest" },
      }),
    );
  }
  if (n) {
    layers.push(
      new SolidPolygonLayer({
        id: "rects",
        data: {
          length: n,
          startIndices: starts,
          attributes: {
            getPolygon: { value: positions, size: 2 },
            getFillColor: { value: colors, size: 4, normalized: true },
          },
        },
        _normalize: false,
      }),
    );
  }
  const segments = scene.segments.length / 4;
  if (segments) {
    layers.push(
      new LineLayer({
        id: "wires",
        data: {
          length: segments,
          attributes: {
            getSourcePosition: {
              value: scene.segments,
              size: 2,
              stride: 16,
              offset: 0,
            },
            getTargetPosition: {
              value: scene.segments,
              size: 2,
              stride: 16,
              offset: 8,
            },
            getColor: { value: scene.segmentColor, size: 4, normalized: true },
          },
        },
        widthUnits: "pixels",
        getWidth: 1,
      }),
    );
  }
  const deck = new Deck({
    parent: host as HTMLDivElement,
    width: host.clientWidth,
    height: host.clientHeight,
    views: new OrthographicView({ flipY: false }),
    controller: false,
    viewState: { target: [scene.world / 2, scene.world / 2, 0], zoom: 0 },
    layers,
  });
  return {
    draw(view) {
      deck.setProps({
        viewState: { target: [view.x, view.y, 0], zoom: Math.log2(view.scale) },
      });
      deck.redraw("bench");
    },
    destroy() {
      deck.finalize();
    },
  };
}

function percentile(values: number[], p: number): number {
  const sorted = [...values].sort((a, b) => a - b);
  return sorted[Math.min(sorted.length - 1, Math.floor(p * sorted.length))];
}

/** A zoom from the whole device to a few tiles and back, one frame a step */
async function run(name: string, backendName: string): Promise<Result> {
  const host = document.getElementById("stage")!;
  host.innerHTML = "";
  const scene = SCENES[name]();
  const started = performance.now();
  const backend =
    backendName === "deck" ? deckBackend(scene, host) : canvas2d(scene, host);
  const fit = Math.min(host.clientWidth, host.clientHeight) / scene.world;
  backend.draw({ x: scene.world / 2, y: scene.world / 2, scale: fit });
  await new Promise((resolve) =>
    requestAnimationFrame(() => requestAnimationFrame(resolve)),
  );
  const buildMs = performance.now() - started;
  const frames: number[] = [];
  const steps = 90;
  let last = performance.now();
  for (let i = 0; i <= steps; i++) {
    const t = i <= steps / 2 ? i / (steps / 2) : (steps - i) / (steps / 2);
    const scale = fit * Math.pow(scene.world / 4, t);
    backend.draw({
      x: scene.world * (0.5 - 0.2 * t),
      y: scene.world * (0.5 - 0.15 * t),
      scale,
    });
    await new Promise((resolve) => requestAnimationFrame(resolve));
    const now = performance.now();
    frames.push(now - last);
    last = now;
  }
  backend.destroy();
  const items =
    scene.rects.length / 4 + scene.segments.length / 4 + (scene.grid ? 1 : 0);
  return {
    scene: name,
    backend: backendName,
    items,
    buildMs: Math.round(buildMs),
    p50: +percentile(frames, 0.5).toFixed(1),
    p95: +percentile(frames, 0.95).toFixed(1),
    worst: +Math.max(...frames).toFixed(1),
  };
}

declare global {
  interface Window {
    benchResults: Result[];
    benchDone: boolean;
    benchRenderer: string;
  }
}

function show(result: Result) {
  const row = document.createElement("tr");
  for (const value of [
    result.scene,
    result.backend,
    result.items,
    result.buildMs,
    result.p50,
    result.p95,
    result.worst,
  ]) {
    const cell = document.createElement("td");
    cell.textContent = String(value);
    row.appendChild(cell);
  }
  document.getElementById("results")!.appendChild(row);
}

async function runAll(pairs: [string, string][]) {
  window.benchResults = [];
  window.benchDone = false;
  for (const [scene, backend] of pairs) {
    document.getElementById("status")!.textContent = `${scene} on ${backend}…`;
    const result = await run(scene, backend);
    window.benchResults.push(result);
    show(result);
  }
  document.getElementById("status")!.textContent = "done";
  window.benchDone = true;
}

// What draws WebGL here: a software renderer (SwiftShader, llvmpipe) is the slow path
const gl = document.createElement("canvas").getContext("webgl2");
const info = gl?.getExtension("WEBGL_debug_renderer_info");
window.benchRenderer = gl
  ? String(
      info
        ? gl.getParameter(info.UNMASKED_RENDERER_WEBGL)
        : gl.getParameter(gl.RENDERER),
    )
  : "no WebGL2";

const params = new URLSearchParams(location.search);
const all: [string, string][] = Object.keys(SCENES).flatMap(
  (s) =>
    [
      [s, "deck"],
      [s, "canvas2d"],
    ] as [string, string][],
);
const only = params.get("scene");
const chosen = all.filter(
  ([s, b]) =>
    (!only || s === only) &&
    (!params.get("backend") || b === params.get("backend")),
);
document
  .getElementById("run")!
  .addEventListener("click", () => void runAll(chosen));
if (params.get("auto") === "1") void runAll(chosen);
