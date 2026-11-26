// The view of a scene (docs/plan_design_views.md, 3.6): the centre, the scale (pixels a world unit)
// and the size of the screen. It lives outside the renderers, so that both draw the same thing and a
// change of renderer keeps it. The world has y up (row 0 of the device at the bottom); the screen y
// goes down.

export interface ViewState {
  cx: number;
  cy: number;
  scale: number;
  width: number;
  height: number;
}

export const MIN_SCALE = 1e-3;
export const MAX_SCALE = 4096;

const clampScale = (scale: number) =>
  Math.min(MAX_SCALE, Math.max(MIN_SCALE, scale));

export function toScreen(v: ViewState, x: number, y: number): [number, number] {
  return [
    v.width / 2 + (x - v.cx) * v.scale,
    v.height / 2 - (y - v.cy) * v.scale,
  ];
}

export function toWorld(
  v: ViewState,
  px: number,
  py: number,
): [number, number] {
  return [
    v.cx + (px - v.width / 2) / v.scale,
    v.cy - (py - v.height / 2) / v.scale,
  ];
}

/** The box of the world on the screen: xmin, ymin, xmax, ymax */
export function visibleBox(v: ViewState): [number, number, number, number] {
  const hw = v.width / 2 / v.scale;
  const hh = v.height / 2 / v.scale;
  return [v.cx - hw, v.cy - hh, v.cx + hw, v.cy + hh];
}

/** Moved by dx, dy pixels of the screen */
export function pan(v: ViewState, dx: number, dy: number): ViewState {
  return { ...v, cx: v.cx - dx / v.scale, cy: v.cy + dy / v.scale };
}

/** Zoomed by factor about the point px, py of the screen: that point stays where it is */
export function zoomAt(
  v: ViewState,
  px: number,
  py: number,
  factor: number,
): ViewState {
  const [wx, wy] = toWorld(v, px, py);
  const scale = clampScale(v.scale * factor);
  return {
    ...v,
    scale,
    cx: wx - (px - v.width / 2) / scale,
    cy: wy + (py - v.height / 2) / scale,
  };
}

/** The whole box on the screen, with a margin of pixels */
export function fit(
  v: ViewState,
  box: [number, number, number, number],
  margin = 16,
): ViewState {
  const w = Math.max(1e-9, box[2] - box[0]);
  const h = Math.max(1e-9, box[3] - box[1]);
  const scale = clampScale(
    Math.min((v.width - 2 * margin) / w, (v.height - 2 * margin) / h),
  );
  return { ...v, scale, cx: (box[0] + box[2]) / 2, cy: (box[1] + box[3]) / 2 };
}

/** A box on the screen, no closer than maxScale (one item does not fill the screen) */
export function zoomTo(
  v: ViewState,
  box: [number, number, number, number],
  maxScale = 200,
): ViewState {
  const fitted = fit(v, box, 48);
  return fitted.scale > maxScale ? { ...fitted, scale: maxScale } : fitted;
}

export function resized(
  v: ViewState,
  width: number,
  height: number,
): ViewState {
  return { ...v, width, height };
}
