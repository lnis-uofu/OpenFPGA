// The colours of the views: from the theme of the page (light or dark), so that a change of theme
// changes the style table and not the geometry (3.2)
import type { Palette, Ramp, Rgba } from "./scene";

export interface ViewColors {
  background: Rgba;
  dark: boolean;
}

/** The theme of the page as it is drawn now */
export function viewColors(): ViewColors {
  const root = document.documentElement;
  const mark = root.getAttribute("data-theme");
  const dark = mark
    ? mark === "dark"
    : window.matchMedia?.("(prefers-color-scheme: dark)").matches === true;
  return { dark, background: dark ? [24, 26, 31, 255] : [250, 250, 248, 255] };
}

const RAMPS: Record<Ramp, Rgba[]> = {
  // use: from quiet to full (blue, green, yellow, red)
  use: [
    [49, 99, 181, 255],
    [72, 168, 120, 255],
    [232, 197, 71, 255],
    [214, 69, 65, 255],
  ],
  heat: [
    [40, 40, 120, 255],
    [140, 60, 160, 255],
    [230, 110, 60, 255],
    [250, 230, 120, 255],
  ],
  // slack: negative is red, around zero yellow, positive green
  slack: [
    [214, 69, 65, 255],
    [232, 197, 71, 255],
    [72, 168, 120, 255],
  ],
};

/** The colour of a value through a ramp, t from 0 to 1 */
export function rampColor(ramp: Ramp, t: number): Rgba {
  const stops = RAMPS[ramp];
  const x = Math.min(1, Math.max(0, t)) * (stops.length - 1);
  const i = Math.min(stops.length - 2, Math.floor(x));
  const f = x - i;
  const a = stops[i];
  const b = stops[i + 1];
  return [0, 1, 2, 3].map((k) => Math.round(a[k] + (b[k] - a[k]) * f)) as Rgba;
}

/** The colour of a value of a grid, or null for none (NaN, out of a category) */
export function paletteColor(palette: Palette, value: number): Rgba | null {
  if (Number.isNaN(value)) return null;
  if (palette.kind === "categorical") return palette.colors[value] ?? null;
  if (!Number.isFinite(value)) return null;
  const span = palette.max - palette.min || 1;
  return rampColor(palette.ramp, (value - palette.min) / span);
}

/** Colours for the categories of something (tile types), stable for an index */
export function categoryColor(index: number, dark: boolean, alpha = 255): Rgba {
  const hues = [210, 30, 140, 280, 60, 0, 180, 100, 330, 240];
  const h = hues[index % hues.length];
  const [s, l] = dark ? [0.35, 0.32] : [0.35, 0.82];
  // hsl to rgb
  const k = (n: number) => (n + h / 30) % 12;
  const a = s * Math.min(l, 1 - l);
  const f = (n: number) =>
    l - a * Math.max(-1, Math.min(k(n) - 3, Math.min(9 - k(n), 1)));
  return [
    Math.round(255 * f(0)),
    Math.round(255 * f(8)),
    Math.round(255 * f(4)),
    alpha,
  ];
}

export const css = (c: Rgba) =>
  `rgba(${c[0]},${c[1]},${c[2]},${(c[3] / 255).toFixed(3)})`;
