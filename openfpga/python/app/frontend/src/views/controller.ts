// The input of a view (docs/plan_design_views.md, 3.6): pan by a drag, zoom about the cursor with the
// wheel, a click to pick, a double click to fit, and the keys. The same for every view and backend:
// it only changes the view state, which the renderers draw.
import { type ViewState, pan, zoomAt } from "./viewstate";

export interface ControllerEvents {
  view(): ViewState;
  setView(next: ViewState): void;
  /** A click (not a drag) at a point of the screen; add when Shift or Ctrl is held */
  pick(px: number, py: number, add: boolean): void;
  hover(px: number, py: number): void;
  leave(): void;
  fit(): void;
  clear(): void;
}

const DRAG = 4; // pixels before a press is a drag

export function attachController(
  element: HTMLElement,
  on: ControllerEvents,
): () => void {
  let down: { x: number; y: number; moved: boolean; id: number } | null = null;
  const local = (e: { clientX: number; clientY: number }) => {
    const box = element.getBoundingClientRect();
    return [e.clientX - box.left, e.clientY - box.top] as const;
  };
  const wheel = (e: WheelEvent) => {
    e.preventDefault();
    const [x, y] = local(e);
    // A wheel step zooms by about 1.25; a trackpad, by its small deltas
    const delta = e.deltaMode === 1 ? e.deltaY * 33 : e.deltaY;
    on.setView(zoomAt(on.view(), x, y, Math.pow(1.0015, -delta)));
  };
  const pointerdown = (e: PointerEvent) => {
    if (e.button !== 0) return;
    element.focus();
    const [x, y] = local(e);
    down = { x, y, moved: false, id: e.pointerId };
    element.setPointerCapture?.(e.pointerId);
  };
  const pointermove = (e: PointerEvent) => {
    const [x, y] = local(e);
    if (!down) {
      on.hover(x, y);
      return;
    }
    if (!down.moved && Math.hypot(x - down.x, y - down.y) < DRAG) return;
    down.moved = true;
    on.setView(pan(on.view(), x - down.x, y - down.y));
    down.x = x;
    down.y = y;
  };
  const pointerup = (e: PointerEvent) => {
    if (!down) return;
    const [x, y] = local(e);
    if (!down.moved) on.pick(x, y, e.shiftKey || e.ctrlKey || e.metaKey);
    element.releasePointerCapture?.(down.id);
    down = null;
  };
  const dblclick = () => on.fit();
  const keydown = (e: KeyboardEvent) => {
    const v = on.view();
    const step = 60;
    const keys: Record<string, () => void> = {
      "+": () => on.setView(zoomAt(v, v.width / 2, v.height / 2, 1.5)),
      "=": () => on.setView(zoomAt(v, v.width / 2, v.height / 2, 1.5)),
      "-": () => on.setView(zoomAt(v, v.width / 2, v.height / 2, 1 / 1.5)),
      ArrowLeft: () => on.setView(pan(v, step, 0)),
      ArrowRight: () => on.setView(pan(v, -step, 0)),
      ArrowUp: () => on.setView(pan(v, 0, step)),
      ArrowDown: () => on.setView(pan(v, 0, -step)),
      f: () => on.fit(),
      Escape: () => on.clear(),
    };
    const run = keys[e.key];
    if (run && !e.ctrlKey && !e.metaKey && !e.altKey) {
      e.preventDefault();
      run();
    }
  };
  const leave = () => on.leave();
  element.addEventListener("wheel", wheel, { passive: false });
  element.addEventListener("pointerdown", pointerdown);
  element.addEventListener("pointermove", pointermove);
  element.addEventListener("pointerup", pointerup);
  element.addEventListener("pointerleave", leave);
  element.addEventListener("dblclick", dblclick);
  element.addEventListener("keydown", keydown);
  return () => {
    element.removeEventListener("wheel", wheel);
    element.removeEventListener("pointerdown", pointerdown);
    element.removeEventListener("pointermove", pointermove);
    element.removeEventListener("pointerup", pointerup);
    element.removeEventListener("pointerleave", leave);
    element.removeEventListener("dblclick", dblclick);
    element.removeEventListener("keydown", keydown);
  };
}
