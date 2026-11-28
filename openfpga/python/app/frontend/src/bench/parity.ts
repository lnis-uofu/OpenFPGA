// A scene of polylines in both renderers, for e2e/parity.mjs: deck.gl must draw every segment of
// every path, as Canvas2D does. Dev only: open /parity.html on the development server.
import { Canvas2dRenderer } from "../views/canvas2d";
import { DeckRenderer } from "../views/deck";
import type { Scene } from "../views/scene";
import { SceneIndex } from "../views/spatial";

// Paths of 2 to 5 points: the first segment, the middle ones and the last one all have to show
const paths: number[][] = [
  [0, 0, 100, 0],
  [0, 20, 50, 20, 50, 60],
  [0, 70, 30, 70, 30, 90, 70, 90, 70, 70],
  [100, 10, 100, 100],
  [10, 100, 10, 40, 90, 40],
];
const xy: number[] = [];
const start = [0];
for (const p of paths) {
  xy.push(...p);
  start.push(xy.length / 2);
}
const scene: Scene = {
  layers: [
    {
      id: "edges",
      kind: "polylines",
      xy: Float32Array.from(xy),
      start: Uint32Array.from(start),
      style: new Uint16Array(paths.length),
      size: 1,
    },
  ],
  bounds: [0, 0, 100, 100],
  styles: [{ stroke: [255, 0, 0, 255], width: 3 }],
};
const view = { cx: 50, cy: 50, scale: 2.5, width: 400, height: 300 };

for (const [id, Renderer] of [
  ["deck", DeckRenderer],
  ["canvas2d", Canvas2dRenderer],
] as const) {
  const renderer = new Renderer();
  renderer.mount(document.getElementById(id)!);
  renderer.setScene(scene, new SceneIndex(scene));
  renderer.render(view);
}

// The middle of the last segment of each path, on the screen (y down), to probe
(window as unknown as Record<string, unknown>).parityProbes = paths.map((p) => {
  const n = p.length;
  const x = (p[n - 4] + p[n - 2]) / 2;
  const y = (p[n - 3] + p[n - 1]) / 2;
  return [
    Math.round(view.width / 2 + (x - view.cx) * view.scale),
    Math.round(view.height / 2 - (y - view.cy) * view.scale),
  ];
});
(window as unknown as Record<string, unknown>).parityReady = true;
