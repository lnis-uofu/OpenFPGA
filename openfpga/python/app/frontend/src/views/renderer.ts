// The renderers of the views (docs/plan_design_views.md, 3.4): deck.gl (WebGL2) draws every layer
// kind, Canvas2D draws them all too and is the fallback. Both draw the same scene for the same view
// state; neither holds any logic of the design.
import { signal } from "@preact/signals";
import type { Scene } from "./scene";
import type { SceneIndex } from "./spatial";
import type { ViewState } from "./viewstate";

export type BackendKind = "deck" | "canvas2d";

export interface Renderer {
  readonly kind: BackendKind;
  mount(host: HTMLElement): void;
  setScene(scene: Scene, index: SceneIndex): void;
  /** Draw the scene for a view state; returns at once (deck.gl draws on its own frame) */
  render(view: ViewState): void;
  destroy(): void;
  /** Called when the renderer cannot draw any more (a WebGL context is lost) */
  onLost?: () => void;
}

/** What the person chose: auto (deck.gl unless it is missing, software or measured slow), or one */
export type BackendChoice = "auto" | "webgl" | "canvas2d";
const KEY = "openfpga-view-backend";

function storedChoice(): BackendChoice {
  try {
    const value = window.localStorage.getItem(KEY);
    return value === "webgl" || value === "canvas2d" ? value : "auto";
  } catch {
    return "auto";
  }
}

export const backendChoice = signal<BackendChoice>(storedChoice());

export function setBackendChoice(choice: BackendChoice) {
  backendChoice.value = choice;
  try {
    if (choice === "auto") window.localStorage.removeItem(KEY);
    else window.localStorage.setItem(KEY, choice);
  } catch {
    /* kept until the page is closed */
  }
}

export interface WebglInfo {
  ok: boolean;
  renderer: string;
  /** Drawn by the processor (SwiftShader, llvmpipe...): the slow path of phase 0 */
  software: boolean;
}

let info: WebglInfo | null = null;

/** Is there WebGL2, and what draws it (once per page) */
export function webglInfo(): WebglInfo {
  if (info) return info;
  try {
    const canvas = document.createElement("canvas");
    const gl = canvas.getContext("webgl2") as WebGL2RenderingContext | null;
    if (!gl)
      return (info = { ok: false, renderer: "no WebGL2", software: false });
    const debug = gl.getExtension("WEBGL_debug_renderer_info");
    const renderer = String(
      debug
        ? gl.getParameter(debug.UNMASKED_RENDERER_WEBGL)
        : gl.getParameter(gl.RENDERER),
    );
    gl.getExtension("WEBGL_lose_context")?.loseContext();
    return (info = {
      ok: true,
      renderer,
      software: /swiftshader|llvmpipe|softpipe|software|microsoft basic/i.test(
        renderer,
      ),
    });
  } catch {
    return (info = { ok: false, renderer: "no WebGL2", software: false });
  }
}

/** For the tests: forget what was found */
export function resetWebglInfo(next: WebglInfo | null = null) {
  info = next;
}

/** The renderer to start with, and why */
export function chooseBackend(
  choice: BackendChoice,
  gl: WebglInfo = webglInfo(),
): { kind: BackendKind; reason: string } {
  if (choice === "canvas2d") return { kind: "canvas2d", reason: "chosen" };
  if (!gl.ok) return { kind: "canvas2d", reason: "this browser has no WebGL2" };
  if (choice === "webgl") return { kind: "deck", reason: "chosen" };
  if (gl.software)
    return {
      kind: "canvas2d",
      reason: `WebGL is drawn in software (${gl.renderer})`,
    };
  return { kind: "deck", reason: "WebGL2" };
}

/** The 95th percentile of frame times: the measure of the probe of auto */
export function p95(frames: number[]): number {
  const sorted = [...frames].sort((a, b) => a - b);
  return sorted.length
    ? sorted[Math.min(sorted.length - 1, Math.floor(0.95 * sorted.length))]
    : 0;
}

/** Above this p95 (ms) over the probe, auto goes to Canvas2D (R-3: 33 ms on a remote desktop) */
export const PROBE_LIMIT_MS = 50;
export const PROBE_FRAMES = 30;
