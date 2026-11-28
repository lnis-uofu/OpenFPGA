// The netlist symbols (docs/plan_netlist_symbols.md): a file of SVG, in a subset of the format of
// netlistsvg, compiled into symbols that the netlist views draw: the outlines and lines of each
// symbol, its ports, its texts, and the light and dark colours of its roles. The tool has a file
// of default symbols; a device package may have one more, whose symbols replace, hide or add to
// the default ones. The SVG never goes into the page: it is parsed, and only what the subset allows
// is kept. Anything else is refused, with what and where.
import { signal } from "@preact/signals";
import type { Rgba } from "./scene";
import defaultText from "./symbols/default.svg?raw";

/** The namespace of the attributes and elements of netlistsvg (and of ours) */
export const SYMBOLS_NS = "https://github.com/nturley/netlistsvg";
const SVG_NS = "http://www.w3.org/2000/svg";
/** The namespaces of editors, whose elements and attributes are skipped (Inkscape writes them) */
const EDITOR_NS = [
  "http://www.inkscape.org/namespaces/inkscape",
  "http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd",
  "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
  "http://purl.org/dc/elements/1.1/",
  "http://creativecommons.org/ns#",
];

/** The roles that every file has; a file may define more with s:color */
export const BASE_ROLES = [
  "body",
  "body-alt",
  "outline",
  "pin",
  "label",
  "accent",
] as const;

export type Side = "left" | "right" | "top" | "bottom";

/** A port of a symbol: in the units of the symbol, y down from its top left */
export interface SymbolPort {
  pid: string;
  x: number;
  y: number;
  side: Side;
  /** For each bit of a bus: bit i is step further along its side (0: one bit) */
  step: number;
}

export interface SymbolText {
  /** The attribute it shows (name, short_name, model, type, count, ports...), or null: fixed */
  attribute: string | null;
  text: string;
  x: number;
  y: number;
  anchor: "start" | "middle" | "end";
  role: string;
}

/** A closed outline (filled unless fill="none") or an open line of a symbol */
export interface SymbolShape {
  role: string;
  closed: boolean;
  filled: boolean;
  /** Its box in the units of the symbol, y down: x, y, w, h */
  box: [number, number, number, number];
  /** Its points in its box, 0..1, y up (as ShapeDef.outline); for a line, in the symbol's units */
  points: number[];
  /** The body of a generic symbol: it grows with the ports */
  grows: boolean;
}

export interface ColorPair {
  light: Rgba;
  dark: Rgba;
}

export interface NetSymbol {
  type: string;
  aliases: string[];
  width: number;
  height: number;
  shapes: SymbolShape[];
  ports: SymbolPort[];
  texts: SymbolText[];
  colors: Map<string, ColorPair>;
  /** The screen size of the symbol, in px, under which its box is drawn in its place */
  lod: number;
  /** The space between two ports placed by the rule (ports the symbol doesn't name) */
  gap: number;
  /** Where it comes from */
  origin: "default" | "package";
}

export interface SymbolLayout {
  layerSpacing?: number;
  nodeSpacing?: number;
  direction?: "RIGHT" | "DOWN";
}

/** A file, compiled */
export interface SymbolFile {
  symbols: NetSymbol[];
  /** The types that a package hides */
  hidden: string[];
  colors: Map<string, ColorPair>;
  layout: SymbolLayout;
}

/** The symbols that the views use: the default ones, with those of a package over them */
export interface SymbolSet {
  byType: Map<string, NetSymbol>;
  byAlias: Map<string, NetSymbol>;
  colors: Map<string, ColorPair>;
  layout: SymbolLayout;
  /** What the package changed */
  replaced: string[];
  hiddenTypes: string[];
  added: string[];
  /** The package's file was refused: why (the default symbols are used) */
  problem: string;
}

export class SymbolError extends Error {}

const DEFAULT_LOD = 24;
const DEFAULT_GAP = 10;
/** The segments of a curve, and of a circle or an arc */
const CURVE_STEPS = 8;
const CIRCLE_STEPS = 24;

/** The elements of SVG that a file may have */
const SHAPES = new Set([
  "path",
  "rect",
  "circle",
  "ellipse",
  "line",
  "polyline",
  "polygon",
]);
const ALLOWED = new Set([...SHAPES, "svg", "g", "text", "title", "desc"]);
/** Elements that are skipped with what is in them */
const SKIPPED = new Set(["title", "desc", "metadata"]);
/** The attributes of SVG that a file may have (besides those of the s: namespace) */
const ATTRIBUTES = new Set([
  "id",
  "class",
  "transform",
  "d",
  "x",
  "y",
  "width",
  "height",
  "rx",
  "ry",
  "cx",
  "cy",
  "r",
  "x1",
  "y1",
  "x2",
  "y2",
  "points",
  "fill",
  "stroke-width",
  "text-anchor",
  "font-size",
  "version",
  "viewBox",
]);
const TEXT_ATTRIBUTES = new Set([
  "name",
  "short_name",
  "model",
  "type",
  "count",
  "ports",
]);

/** A colour of a file: #rgb, #rrggbb or #rrggbbaa */
export function parseColor(text: string): Rgba | null {
  const m = /^#([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})$/.exec(
    text.trim(),
  );
  if (!m) return null;
  let hex = m[1];
  if (hex.length === 3)
    hex = hex
      .split("")
      .map((c) => c + c)
      .join("");
  if (hex.length === 6) hex += "ff";
  return [0, 2, 4, 6].map((i) => parseInt(hex.slice(i, i + 2), 16)) as Rgba;
}

/** x' = sx x + tx, y' = sy y + ty */
interface Transform {
  sx: number;
  sy: number;
  tx: number;
  ty: number;
}
const IDENTITY: Transform = { sx: 1, sy: 1, tx: 0, ty: 0 };

function numbersOf(text: string): number[] {
  return (text.match(/[-+]?(?:\d+\.?\d*|\.\d+)(?:[eE][-+]?\d+)?/g) ?? []).map(
    Number,
  );
}

/** Where the n-th element of a name is in the text, for the messages */
function lineOf(source: string, element: Element): number {
  const name = element.tagName;
  const all = element.ownerDocument.getElementsByTagName(name);
  let index = 0;
  for (let i = 0; i < all.length; i++)
    if (all[i] === element) {
      index = i;
      break;
    }
  const pattern = new RegExp(
    `<${name.replace(/[.*+?^${}()|[\]\\]/g, "\\$&")}[\\s/>]`,
    "g",
  );
  let found: RegExpExecArray | null = null;
  for (let i = 0; i <= index; i++) {
    found = pattern.exec(source);
    if (!found) return 0;
  }
  return found ? source.slice(0, found.index).split("\n").length : 0;
}

/** A path's data as subpaths of points (curves and arcs flattened), each closed or not */
export function flattenPath(
  d: string,
): { points: number[]; closed: boolean }[] {
  const tokens = d.match(
    /[MmLlHhVvCcSsQqTtAaZz]|[-+]?(?:\d+\.?\d*|\.\d+)(?:[eE][-+]?\d+)?/g,
  );
  if (!tokens) return [];
  const out: { points: number[]; closed: boolean }[] = [];
  let current: number[] = [];
  let x = 0;
  let y = 0;
  let startX = 0;
  let startY = 0;
  // The control point of the last curve, for S and T
  let lastC: [number, number] | null = null;
  let lastQ: [number, number] | null = null;
  let i = 0;
  let command = "";
  const next = () => {
    const v = Number(tokens[i++]);
    if (Number.isNaN(v)) throw new SymbolError(`the path "${d}" is not valid`);
    return v;
  };
  const end = (closed: boolean) => {
    if (current.length >= 4) out.push({ points: current, closed });
    current = [];
  };
  const to = (px: number, py: number) => {
    current.push(px, py);
    x = px;
    y = py;
  };
  while (i < tokens.length) {
    if (/[A-Za-z]/.test(tokens[i])) command = tokens[i++];
    else if (!command) throw new SymbolError(`the path "${d}" is not valid`);
    const rel = command === command.toLowerCase();
    const c = command.toUpperCase();
    const ox = rel ? x : 0;
    const oy = rel ? y : 0;
    switch (c) {
      case "M": {
        end(false);
        const px = next() + ox;
        const py = next() + oy;
        startX = px;
        startY = py;
        to(px, py);
        command = rel ? "l" : "L"; // the pairs after it are lines
        lastC = lastQ = null;
        break;
      }
      case "L":
        to(next() + ox, next() + oy);
        lastC = lastQ = null;
        break;
      case "H":
        to(next() + ox, y);
        lastC = lastQ = null;
        break;
      case "V":
        to(x, next() + oy);
        lastC = lastQ = null;
        break;
      case "C":
      case "S": {
        let c1: [number, number];
        if (c === "C") c1 = [next() + ox, next() + oy];
        else c1 = lastC ? [2 * x - lastC[0], 2 * y - lastC[1]] : [x, y];
        const c2: [number, number] = [next() + ox, next() + oy];
        const e: [number, number] = [next() + ox, next() + oy];
        const [x0, y0] = [x, y];
        for (let k = 1; k <= CURVE_STEPS; k++) {
          const t = k / CURVE_STEPS;
          const u = 1 - t;
          to(
            u * u * u * x0 +
              3 * u * u * t * c1[0] +
              3 * u * t * t * c2[0] +
              t * t * t * e[0],
            u * u * u * y0 +
              3 * u * u * t * c1[1] +
              3 * u * t * t * c2[1] +
              t * t * t * e[1],
          );
        }
        lastC = c2;
        lastQ = null;
        break;
      }
      case "Q":
      case "T": {
        let q: [number, number];
        if (c === "Q") q = [next() + ox, next() + oy];
        else q = lastQ ? [2 * x - lastQ[0], 2 * y - lastQ[1]] : [x, y];
        const e: [number, number] = [next() + ox, next() + oy];
        const [x0, y0] = [x, y];
        for (let k = 1; k <= CURVE_STEPS; k++) {
          const t = k / CURVE_STEPS;
          const u = 1 - t;
          to(
            u * u * x0 + 2 * u * t * q[0] + t * t * e[0],
            u * u * y0 + 2 * u * t * q[1] + t * t * e[1],
          );
        }
        lastQ = q;
        lastC = null;
        break;
      }
      case "A": {
        const rx = Math.abs(next());
        const ry = Math.abs(next());
        const rotation = (next() * Math.PI) / 180;
        const large = next() !== 0;
        const sweep = next() !== 0;
        const ex = next() + ox;
        const ey = next() + oy;
        arc(x, y, rx, ry, rotation, large, sweep, ex, ey).forEach(
          (p, k, all) => {
            if (k % 2 === 0) to(p, all[k + 1]);
          },
        );
        lastC = lastQ = null;
        break;
      }
      case "Z":
        if (current.length) {
          x = startX;
          y = startY;
        }
        end(true);
        current = [startX, startY];
        lastC = lastQ = null;
        command = "";
        break;
      default:
        throw new SymbolError(`the path "${d}" is not valid`);
    }
  }
  end(false);
  return out;
}

/** The points of an arc of SVG (from its end points), without its first point */
function arc(
  x1: number,
  y1: number,
  rx: number,
  ry: number,
  phi: number,
  large: boolean,
  sweep: boolean,
  x2: number,
  y2: number,
): number[] {
  if (!rx || !ry) return [x2, y2];
  const cos = Math.cos(phi);
  const sin = Math.sin(phi);
  const dx = (x1 - x2) / 2;
  const dy = (y1 - y2) / 2;
  const x1p = cos * dx + sin * dy;
  const y1p = -sin * dx + cos * dy;
  // Radii too small: scaled up
  const lambda = (x1p * x1p) / (rx * rx) + (y1p * y1p) / (ry * ry);
  if (lambda > 1) {
    rx *= Math.sqrt(lambda);
    ry *= Math.sqrt(lambda);
  }
  const num = rx * rx * ry * ry - rx * rx * y1p * y1p - ry * ry * x1p * x1p;
  const den = rx * rx * y1p * y1p + ry * ry * x1p * x1p;
  let k = Math.sqrt(Math.max(0, num / den));
  if (large === sweep) k = -k;
  const cxp = (k * rx * y1p) / ry;
  const cyp = (-k * ry * x1p) / rx;
  const cx = cos * cxp - sin * cyp + (x1 + x2) / 2;
  const cy = sin * cxp + cos * cyp + (y1 + y2) / 2;
  const angle = (ux: number, uy: number, vx: number, vy: number) => {
    const a = Math.atan2(ux * vy - uy * vx, ux * vx + uy * vy);
    return a;
  };
  const t1 = angle(1, 0, (x1p - cxp) / rx, (y1p - cyp) / ry);
  let dt = angle(
    (x1p - cxp) / rx,
    (y1p - cyp) / ry,
    (-x1p - cxp) / rx,
    (-y1p - cyp) / ry,
  );
  if (!sweep && dt > 0) dt -= 2 * Math.PI;
  if (sweep && dt < 0) dt += 2 * Math.PI;
  const steps = Math.max(
    2,
    Math.ceil((Math.abs(dt) / (2 * Math.PI)) * CIRCLE_STEPS),
  );
  const out: number[] = [];
  for (let s = 1; s <= steps; s++) {
    const t = t1 + (dt * s) / steps;
    const px = rx * Math.cos(t);
    const py = ry * Math.sin(t);
    out.push(cos * px - sin * py + cx, sin * px + cos * py + cy);
  }
  out[out.length - 2] = x2;
  out[out.length - 1] = y2;
  return out;
}

function ellipse(cx: number, cy: number, rx: number, ry: number): number[] {
  const out: number[] = [];
  for (let k = 0; k < CIRCLE_STEPS; k++) {
    const t = (2 * Math.PI * k) / CIRCLE_STEPS;
    out.push(cx + rx * Math.cos(t), cy + ry * Math.sin(t));
  }
  return out;
}

/** Compile a file of symbols. origin: the tool's file or a package's. Throws SymbolError */
export function compileSymbols(
  source: string,
  origin: "default" | "package",
): SymbolFile {
  if (typeof DOMParser === "undefined")
    throw new SymbolError("SVG can't be read here");
  const doc = new DOMParser().parseFromString(source, "image/svg+xml");
  const broken = doc.getElementsByTagName("parsererror")[0];
  if (broken)
    throw new SymbolError(
      `it is not well-formed XML: ${(broken.textContent ?? "").trim().split("\n")[0]}`,
    );
  const root = doc.documentElement;
  const where = (e: Element) => {
    const line = lineOf(source, e);
    return `<${e.tagName}>${line ? ` (line ${line})` : ""}`;
  };
  const fail = (e: Element, why: string): never => {
    throw new SymbolError(`${where(e)}: ${why}`);
  };
  if (root.localName !== "svg" || root.namespaceURI !== SVG_NS)
    fail(root, "the root is not an svg element of SVG");
  const s = (e: Element, name: string) => e.getAttributeNS(SYMBOLS_NS, name);
  const isOurs = (e: Element) => e.namespaceURI === SYMBOLS_NS;
  const skipped = (e: Element) =>
    EDITOR_NS.includes(e.namespaceURI ?? "") || SKIPPED.has(e.localName);
  const number = (e: Element, name: string, value: string | null) => {
    if (value === null) return 0;
    const v = Number(value);
    if (!Number.isFinite(v)) fail(e, `${name}="${value}" is not a number`);
    return v;
  };

  // The checks of every element: what is refused, whatever its place
  const checkAttributes = (e: Element) => {
    for (const a of Array.from(e.attributes)) {
      const ns = a.namespaceURI ?? "";
      if (ns === SYMBOLS_NS || EDITOR_NS.includes(ns)) continue;
      if (a.name === "xmlns" || a.name.startsWith("xmlns:")) continue;
      if (a.localName === "href" || a.name.startsWith("on"))
        fail(e, `the attribute ${a.name} is refused`);
      if (a.name === "style")
        fail(e, "style is refused: colours are set by s:color, for a role");
      if (a.name === "stroke" || (a.name === "fill" && a.value !== "none"))
        fail(
          e,
          `${a.name}="${a.value}" is refused: colours are set by s:color, for a role (fill="none" is allowed)`,
        );
      if (!ATTRIBUTES.has(a.name))
        fail(e, `the attribute ${a.name} is refused`);
    }
  };

  const parseTransform = (e: Element, base: Transform): Transform => {
    const text = e.getAttribute("transform");
    if (!text) return base;
    let t = base;
    const re = /\s*(translate|scale)\s*\(([^)]*)\)\s*,?/gy;
    let m: RegExpExecArray | null;
    let used = 0;
    while ((m = re.exec(text))) {
      used = re.lastIndex;
      const v = numbersOf(m[2]);
      // The transform of the parent applies after this one
      if (m[1] === "translate")
        t = {
          ...t,
          tx: t.tx + t.sx * (v[0] ?? 0),
          ty: t.ty + t.sy * (v[1] ?? 0),
        };
      else
        t = {
          ...t,
          sx: t.sx * (v[0] ?? 1),
          sy: t.sy * (v[1] ?? v[0] ?? 1),
        };
    }
    if (used !== text.length && text.slice(used).trim())
      fail(e, `transform="${text}": only translate and scale are allowed`);
    return t;
  };
  const apply = (t: Transform, points: number[]) =>
    points.map((v, k) => (k % 2 === 0 ? t.sx * v + t.tx : t.sy * v + t.ty));

  // The colours of the root, and of each symbol
  const colorsOf = (parent: Element): Map<string, ColorPair> => {
    const out = new Map<string, ColorPair>();
    for (const c of Array.from(parent.children)) {
      if (!isOurs(c) || c.localName !== "color") continue;
      const role = c.getAttribute("role") ?? "";
      if (!/^[a-z][a-z0-9-]*$/.test(role))
        fail(c, `role="${role}" is not the name of a role`);
      const light = parseColor(c.getAttribute("light") ?? "");
      const dark = parseColor(c.getAttribute("dark") ?? "");
      if (!light || !dark)
        fail(
          c,
          `the role ${role} needs both light and dark colours (#rgb, #rrggbb or #rrggbbaa)`,
        );
      out.set(role, { light: light!, dark: dark! });
    }
    return out;
  };
  const rootColors = colorsOf(root);

  const layout: SymbolLayout = {};
  const symbols: NetSymbol[] = [];
  const hidden: string[] = [];
  const types = new Set<string>();

  const compileSymbol = (g: Element) => {
    const type = s(g, "type")!;
    if (!/^[A-Za-z_][A-Za-z0-9_.:-]*$/.test(type))
      fail(g, `s:type="${type}" is not a name`);
    if (types.has(type)) fail(g, `the symbol ${type} is defined twice`);
    types.add(type);
    if (s(g, "hide") !== null) {
      if (s(g, "hide") !== "true") fail(g, 's:hide is "true" or absent');
      if (origin === "default") fail(g, "the default file hides nothing");
      if (g.children.length) fail(g, `the hidden symbol ${type} holds nothing`);
      hidden.push(type);
      return;
    }
    const width = number(g, "s:width", s(g, "width"));
    const height = number(g, "s:height", s(g, "height"));
    if (!(width > 0) || !(height > 0))
      fail(g, `the symbol ${type} needs s:width and s:height`);
    const colors = colorsOf(g);
    const roles = new Set<string>([
      ...BASE_ROLES,
      ...rootColors.keys(),
      ...colors.keys(),
    ]);
    const symbol: NetSymbol = {
      type,
      aliases: [],
      width,
      height,
      shapes: [],
      ports: [],
      texts: [],
      colors,
      lod: s(g, "lod") === null ? DEFAULT_LOD : number(g, "s:lod", s(g, "lod")),
      gap: s(g, "gap") === null ? DEFAULT_GAP : number(g, "s:gap", s(g, "gap")),
      origin,
    };
    const roleOf = (e: Element, fallback: string) => {
      const role = (e.getAttribute("class") ?? "").trim() || fallback;
      if (/\s/.test(role)) fail(e, `class="${role}": one role only`);
      if (!roles.has(role))
        fail(
          e,
          `the role ${role} is not one of ${BASE_ROLES.join(", ")}, nor defined by an s:color of the file`,
        );
      return role;
    };
    const addShape = (
      e: Element,
      points: number[],
      closed: boolean,
      t: Transform,
    ) => {
      const p = apply(t, points);
      if (p.length < 4) return;
      let x0 = Infinity;
      let y0 = Infinity;
      let x1 = -Infinity;
      let y1 = -Infinity;
      for (let k = 0; k < p.length; k += 2) {
        x0 = Math.min(x0, p[k]);
        x1 = Math.max(x1, p[k]);
        y0 = Math.min(y0, p[k + 1]);
        y1 = Math.max(y1, p[k + 1]);
      }
      const filled = closed && e.getAttribute("fill") !== "none";
      const role = roleOf(e, closed && filled ? "body" : "outline");
      const w = Math.max(x1 - x0, 1e-6);
      const h = Math.max(y1 - y0, 1e-6);
      symbol.shapes.push({
        role,
        closed,
        filled,
        box: [x0, y0, x1 - x0, y1 - y0],
        // A closed outline in its box, 0..1 with y up (the shapes of the scene); a line as it is
        points: closed
          ? p.map((v, k) => (k % 2 === 0 ? (v - x0) / w : 1 - (v - y0) / h))
          : p,
        grows: s(e, "generic") === "body",
      });
    };
    const walk = (parent: Element, t: Transform) => {
      for (const e of Array.from(parent.children)) {
        if (skipped(e)) continue;
        if (isOurs(e)) {
          if (e.localName === "alias") {
            const val = e.getAttribute("val") ?? "";
            if (!/^[a-z]+:\S+$|^stub$/.test(val))
              fail(
                e,
                `the alias "${val}" is not <kind>:<name> (atom:.latch, cluster:clb...)`,
              );
            symbol.aliases.push(val);
          } else if (e.localName !== "color")
            fail(e, `s:${e.localName} is not allowed in a symbol`);
          continue;
        }
        if (e.namespaceURI !== SVG_NS || !ALLOWED.has(e.localName))
          fail(e, `the element ${e.tagName} is refused`);
        checkAttributes(e);
        const here = parseTransform(e, t);
        const n = (name: string) => number(e, name, e.getAttribute(name));
        switch (e.localName) {
          case "g": {
            const pid = s(e, "pid");
            if (pid !== null) {
              if (e.children.length) fail(e, `the port ${pid} holds nothing`);
              const [x, y] = apply(here, [
                number(e, "s:x", s(e, "x")),
                number(e, "s:y", s(e, "y")),
              ]);
              const position = s(e, "position");
              if (
                position !== null &&
                !["left", "right", "top", "bottom"].includes(position)
              )
                fail(
                  e,
                  `s:position="${position}" is left, right, top or bottom`,
                );
              const side: Side =
                (position as Side | null) ??
                (x <= 0
                  ? "left"
                  : x >= width
                    ? "right"
                    : y <= 0
                      ? "top"
                      : y >= height
                        ? "bottom"
                        : x < width / 2
                          ? "left"
                          : "right");
              symbol.ports.push({
                pid,
                x,
                y,
                side,
                step: number(e, "s:step", s(e, "step")),
              });
            } else walk(e, here);
            break;
          }
          case "path":
            for (const sub of flattenPath(e.getAttribute("d") ?? ""))
              addShape(e, sub.points, sub.closed, here);
            break;
          case "rect": {
            const [x, y, w, h] = [n("x"), n("y"), n("width"), n("height")];
            addShape(e, [x, y, x + w, y, x + w, y + h, x, y + h], true, here);
            break;
          }
          case "circle":
            addShape(e, ellipse(n("cx"), n("cy"), n("r"), n("r")), true, here);
            break;
          case "ellipse":
            addShape(
              e,
              ellipse(n("cx"), n("cy"), n("rx"), n("ry")),
              true,
              here,
            );
            break;
          case "line":
            addShape(e, [n("x1"), n("y1"), n("x2"), n("y2")], false, here);
            break;
          case "polyline":
          case "polygon":
            addShape(
              e,
              numbersOf(e.getAttribute("points") ?? ""),
              e.localName === "polygon",
              here,
            );
            break;
          case "text": {
            const attribute = s(e, "attribute");
            if (
              attribute !== null &&
              attribute !== "" &&
              !TEXT_ATTRIBUTES.has(attribute)
            )
              fail(
                e,
                `s:attribute="${attribute}" is not one of ${[...TEXT_ATTRIBUTES].join(", ")}`,
              );
            if (e.children.length) fail(e, "a text holds only its text");
            const [x, y] = apply(here, [n("x"), n("y")]);
            const anchor = e.getAttribute("text-anchor") ?? "middle";
            if (!["start", "middle", "end"].includes(anchor))
              fail(e, `text-anchor="${anchor}" is start, middle or end`);
            symbol.texts.push({
              attribute: attribute || null,
              text: (e.textContent ?? "").trim(),
              x,
              y,
              anchor: anchor as SymbolText["anchor"],
              role: roleOf(e, "label"),
            });
            break;
          }
        }
      }
    };
    walk(g, IDENTITY);
    if (!symbol.aliases.length)
      fail(g, `the symbol ${type} has no s:alias: nothing would use it`);
    symbols.push(symbol);
  };

  checkAttributes(root);
  for (const e of Array.from(root.children)) {
    if (skipped(e)) continue;
    if (isOurs(e)) {
      if (e.localName === "color") continue;
      if (e.localName === "layout") {
        for (const a of Array.from(e.attributes)) {
          if (a.name === "layerSpacing" || a.name === "nodeSpacing") {
            const v = Number(a.value);
            if (!(v > 0 && v <= 1000))
              fail(e, `${a.name}="${a.value}" is a number from 0 to 1000`);
            layout[a.name] = v;
          } else if (a.name === "direction") {
            if (a.value !== "RIGHT" && a.value !== "DOWN")
              fail(e, `direction="${a.value}" is RIGHT or DOWN`);
            layout.direction = a.value as "RIGHT" | "DOWN";
          } else
            fail(
              e,
              `${a.name}: s:layout sets layerSpacing, nodeSpacing and direction only`,
            );
        }
        continue;
      }
      fail(e, `s:${e.localName} is not allowed here`);
    }
    if (
      e.namespaceURI !== SVG_NS ||
      e.localName !== "g" ||
      s(e, "type") === null
    ) {
      if (e.namespaceURI === SVG_NS && e.localName === "g") {
        // A group of symbols (a layer of an editor)
        checkAttributes(e);
        for (const c of Array.from(e.children)) {
          if (skipped(c)) continue;
          if (
            c.namespaceURI === SVG_NS &&
            c.localName === "g" &&
            s(c, "type") !== null
          ) {
            checkAttributes(c);
            compileSymbol(c);
          } else fail(c, "a symbol is a g with s:type");
        }
        continue;
      }
      fail(e, "a symbol is a g with s:type");
    }
    checkAttributes(e);
    compileSymbol(e);
  }
  return { symbols, hidden, colors: rootColors, layout };
}

/** The symbols of the default file and those of a package, merged (docs/plan_netlist_symbols.md,
 * section 5). A package's file that refers to no default symbol by s:hide is refused: the default
 * symbols are used, and problem says why */
export function mergeSymbols(
  base: SymbolFile,
  pkg: SymbolFile | null,
  problem = "",
): SymbolSet {
  const byType = new Map(base.symbols.map((sym) => [sym.type, sym]));
  const colors = new Map(base.colors);
  const layout = { ...base.layout };
  const replaced: string[] = [];
  const hiddenTypes: string[] = [];
  const added: string[] = [];
  if (pkg) {
    const unknown = pkg.hidden.filter((t) => !byType.has(t));
    if (unknown.length) {
      problem = `the package hides ${unknown.join(", ")}, which ${unknown.length > 1 ? "are not default symbols" : "is not a default symbol"}`;
      pkg = null;
    }
  }
  if (pkg) {
    for (const t of pkg.hidden) {
      byType.delete(t);
      hiddenTypes.push(t);
    }
    for (const sym of pkg.symbols) {
      if (byType.has(sym.type)) replaced.push(sym.type);
      else added.push(sym.type);
      byType.set(sym.type, sym);
    }
    for (const [role, pair] of pkg.colors) colors.set(role, pair);
    Object.assign(layout, pkg.layout);
  }
  // An alias belongs to one symbol: a package's symbol takes it from a default one
  const byAlias = new Map<string, NetSymbol>();
  const ordered = [...byType.values()].sort(
    (a, b) => Number(a.origin === "package") - Number(b.origin === "package"),
  );
  for (const sym of ordered)
    for (const alias of sym.aliases) byAlias.set(alias, sym);
  return {
    byType,
    byAlias,
    colors,
    layout,
    replaced,
    hiddenTypes,
    added,
    problem,
  };
}

/** The first symbol of the aliases of an object, in order; null: drawn as a plain box */
export function symbolFor(set: SymbolSet, aliases: string[]): NetSymbol | null {
  for (const alias of aliases) {
    const found = set.byAlias.get(alias);
    if (found) return found;
  }
  return null;
}

/** The colour of a role of a symbol, in a theme: the symbol's, then the file's, else undefined
 * (the view's own) */
export function roleColor(
  set: SymbolSet,
  symbol: NetSymbol | null,
  role: string,
  dark: boolean,
): Rgba | undefined {
  const pair = symbol?.colors.get(role) ?? set.colors.get(role);
  return pair ? (dark ? pair.dark : pair.light) : undefined;
}

/** A line of what drew the view, for its legend */
export function symbolsNote(set: SymbolSet): string {
  const parts = [
    set.replaced.length ? `replaces ${set.replaced.length}` : "",
    set.hiddenTypes.length ? `hides ${set.hiddenTypes.length}` : "",
    set.added.length ? `adds ${set.added.length}` : "",
  ].filter(Boolean);
  return parts.length
    ? `Symbols: default; the package ${parts.join(", ")}`
    : "Symbols: default";
}

let defaultFile: SymbolFile | null = null;

/** The default symbols of the tool, compiled once */
export function defaultSymbols(): SymbolFile {
  defaultFile ??= compileSymbols(defaultText, "default");
  return defaultFile;
}

/** The symbols of a session: the default ones, with the package's file over them if it has one.
 * A file that is refused leaves the default symbols, and says why */
export function symbolsOf(packageText: string | null): SymbolSet {
  const base = defaultSymbols();
  if (!packageText) return mergeSymbols(base, null);
  try {
    return mergeSymbols(base, compileSymbols(packageText, "package"));
  } catch (error) {
    return mergeSymbols(
      base,
      null,
      `the symbols of the package are not used: ${(error as Error).message}`,
    );
  }
}

/** A pin of an object to place on its symbol: its key (unique in the object), port, bit, and
 * whether it drives */
export interface PinSpec {
  key: string;
  port: string;
  bit: number;
  out: boolean;
}

export interface PlacedPin {
  key: string;
  x: number;
  y: number;
  side: Side;
}

/** Where the pins of an object go on its symbol: a port of the same name (port[bit] first, then
 * port, each bit step further for a bus), else by the rule: inputs on the left, outputs on the
 * right, gap apart, after the named ones. The symbol grows down when they need it (its body,
 * s:generic, with it); the ports at its bottom move down with it. unnamed: the pins placed by
 * the rule */
export function placePins(
  symbol: NetSymbol,
  pins: PinSpec[],
): { width: number; height: number; pins: PlacedPin[]; unnamed: number } {
  const byPid = new Map(symbol.ports.map((p) => [p.pid, p]));
  const placed: PlacedPin[] = [];
  const taken = new Set<string>();
  const rest: PinSpec[] = [];
  let lowest = 0; // the lowest pin of the left and right sides
  for (const pin of pins) {
    let port = byPid.get(`${pin.port}[${pin.bit}]`);
    let offset = 0;
    if (!port) {
      const named = byPid.get(pin.port);
      if (named && (named.step > 0 || pin.bit === 0)) {
        port = named;
        offset = pin.bit * named.step;
      }
    }
    if (!port) {
      rest.push(pin);
      continue;
    }
    const along = port.side === "left" || port.side === "right";
    const x = along ? port.x : port.x + offset;
    const y = along ? port.y + offset : port.y;
    const where = `${x},${y}`;
    if (taken.has(where)) {
      rest.push(pin);
      continue;
    }
    taken.add(where);
    placed.push({ key: pin.key, x, y, side: port.side });
    if (along) lowest = Math.max(lowest, y);
  }
  // The rule: below what is named on each side
  const sideStart = (side: Side) =>
    Math.max(0, ...placed.filter((p) => p.side === side).map((p) => p.y));
  let left = sideStart("left");
  let right = sideStart("right");
  for (const pin of rest) {
    if (pin.out) {
      right += symbol.gap;
      placed.push({ key: pin.key, x: symbol.width, y: right, side: "right" });
      lowest = Math.max(lowest, right);
    } else {
      left += symbol.gap;
      placed.push({ key: pin.key, x: 0, y: left, side: "left" });
      lowest = Math.max(lowest, left);
    }
  }
  const height = Math.max(symbol.height, lowest + symbol.gap / 2 + 2);
  const grown = height - symbol.height;
  if (grown > 0) for (const p of placed) if (p.side === "bottom") p.y += grown;
  return { width: symbol.width, height, pins: placed, unnamed: rest.length };
}

// The symbols of the package of the session --------------------------------------------------

const USE_KEY = "openfpga-package-symbols";

function storedUse(): boolean {
  try {
    return window.localStorage.getItem(USE_KEY) !== "off";
  } catch {
    return true;
  }
}

/** Whether the views use the symbols of the package (a setting, on by default) */
export const usePackageSymbols = signal<boolean>(storedUse());

export function setUsePackageSymbols(use: boolean) {
  usePackageSymbols.value = use;
  try {
    if (use) window.localStorage.removeItem(USE_KEY);
    else window.localStorage.setItem(USE_KEY, "off");
  } catch {
    /* kept until the page is closed */
  }
}

/** The file of symbols of the session's package, from the app (null: it has none) */
type Source = () => Promise<{ name: string; text: string } | null>;
const fromApp: Source = async () => {
  const response = await fetch("/api/package/netlist_symbols", {
    cache: "no-store",
  });
  if (!response.ok) return null;
  const answer = await response.json();
  return answer?.ok && answer.data
    ? { name: String(answer.data.package), text: String(answer.data.text) }
    : null;
};
let source: Source = fromApp;

/** For the tests: where the package's file comes from (null: the app) */
export function setSymbolsSource(next: Source | null) {
  source = next ?? fromApp;
}

/** The symbols of the views of the session: the default ones, and those of its package unless
 * the setting turns them off. A file that can't be fetched counts as none */
export async function loadSymbols(): Promise<{
  set: SymbolSet;
  packageName: string | null;
}> {
  let found: { name: string; text: string } | null = null;
  if (usePackageSymbols.value) {
    try {
      found = await source();
    } catch {
      found = null;
    }
  }
  return {
    set: symbolsOf(found?.text ?? null),
    packageName: found?.name ?? null,
  };
}
