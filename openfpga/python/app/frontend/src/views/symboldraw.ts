// The drawing of the netlist symbols (docs/plan_netlist_symbols.md, section 4) in the layers of a
// scene that both renderers draw: the closed shapes of each symbol as items of a shapes layer (an
// outline in its box), its lines as polylines, its texts as labels. Under the screen size of a
// symbol (s:lod) its box is drawn in its place, in the colour of its body.
import type {
  Layer,
  PolylinesLayer,
  RectsLayer,
  Rgba,
  ShapeDef,
  ShapesLayer,
} from "./scene";
import { type NetSymbol, type SymbolSet, roleColor } from "./symbols";
import type { StyleTable } from "./view";

/** The colours of the roles that no file sets */
export function fallbackRole(role: string, dark: boolean): Rgba {
  switch (role) {
    case "outline":
      return dark ? [20, 20, 20, 255] : [70, 70, 70, 255];
    case "pin":
      return dark ? [200, 200, 210, 255] : [60, 60, 70, 255];
    case "label":
      return dark ? [235, 235, 235, 255] : [20, 20, 20, 255];
    case "accent":
      return dark ? [240, 160, 64, 255] : [200, 120, 30, 255];
    case "body-alt":
      return dark ? [40, 44, 54, 255] : [244, 246, 251, 255];
    default:
      return dark ? [90, 90, 90, 255] : [200, 200, 200, 255];
  }
}

/** The size of a symbol on the screen is measured on this many world units (a LUT) */
const LOD_SIZE = 60;

interface Collected {
  shapes: {
    x: number[];
    y: number[];
    w: number[];
    h: number[];
    shape: number[];
    style: number[];
  };
  lines: { xy: number[]; start: number[]; style: number[] };
  boxes: {
    x: number[];
    y: number[];
    w: number[];
    h: number[];
    style: number[];
    name: string[];
  };
}

export interface SymbolLabel {
  x: number;
  y: number;
  text: string;
  style: number;
}

export class SymbolDrawer {
  private readonly defs: ShapeDef[] = [];
  private readonly defIndex = new Map<string, number>();
  private readonly byLod = new Map<number, Collected>();
  readonly labels: SymbolLabel[] = [];

  constructor(
    private readonly set: SymbolSet,
    private readonly styles: StyleTable,
    private readonly dark: boolean,
  ) {}

  /** The colour of a role of a symbol: the files', else the view's (body: the colour given) */
  color(symbol: NetSymbol | null, role: string, body?: Rgba): Rgba {
    return (
      roleColor(this.set, symbol, role, this.dark) ??
      (role === "body" && body ? body : fallbackRole(role, this.dark))
    );
  }

  private collected(lod: number): Collected {
    let c = this.byLod.get(lod);
    if (!c) {
      c = {
        shapes: { x: [], y: [], w: [], h: [], shape: [], style: [] },
        lines: { xy: [], start: [0], style: [] },
        boxes: { x: [], y: [], w: [], h: [], style: [], name: [] },
      };
      this.byLod.set(lod, c);
    }
    return c;
  }

  /** A symbol drawn in a box of the world (x0, y0, x1, y1; y up). body: the colour of its body
   * when no file sets it; outline: a colour over the outline's (a guessed module); text: the
   * value of an attribute of the object; name: the name of the object, for its box */
  add(
    symbol: NetSymbol,
    box: [number, number, number, number],
    body: Rgba,
    text: (attribute: string) => string,
    outline?: Rgba,
    name = "",
  ) {
    const [x0, y0, x1, y1] = box;
    const c = this.collected(symbol.lod);
    const bodyColor = this.color(symbol, "body", body);
    const outlineColor = outline ?? this.color(symbol, "outline");
    const width = outline ? 2 : 1;
    // Its box, under its size on the screen
    c.boxes.name.push(name);
    c.boxes.x.push(x0);
    c.boxes.y.push(y0);
    c.boxes.w.push(x1 - x0);
    c.boxes.h.push(y1 - y0);
    c.boxes.style.push(
      this.styles.add(`symbox:${bodyColor}:${outlineColor}:${width}`, {
        fill: bodyColor,
        stroke: outlineColor,
        width,
      }),
    );
    // Grown down: the body (s:generic) with it
    const grown = Math.max(0, y1 - y0 - symbol.height);
    symbol.shapes.forEach((shape, k) => {
      const role = this.color(symbol, shape.role, body);
      if (shape.closed) {
        const key = `${symbol.origin}:${symbol.type}#${k}`;
        let index = this.defIndex.get(key);
        if (index === undefined) {
          index = this.defs.length;
          this.defs.push({ name: key, outline: shape.points });
          this.defIndex.set(key, index);
        }
        const [sx, sy, sw, sh] = shape.box;
        const h = sh + (shape.grows ? grown : 0);
        c.shapes.x.push(x0 + sx);
        c.shapes.y.push(y1 - sy - h);
        c.shapes.w.push(sw);
        c.shapes.h.push(h);
        c.shapes.shape.push(index);
        const fill = shape.filled ? role : undefined;
        const stroke = shape.filled ? outlineColor : role;
        c.shapes.style.push(
          this.styles.add(`symshape:${fill ?? "-"}:${stroke}:${width}`, {
            ...(fill ? { fill } : {}),
            stroke,
            width,
          }),
        );
      } else {
        for (let p = 0; p < shape.points.length; p += 2)
          c.lines.xy.push(x0 + shape.points[p], y1 - shape.points[p + 1]);
        c.lines.start.push(c.lines.xy.length / 2);
        c.lines.style.push(
          this.styles.add(`symline:${role}`, { stroke: role, width: 1 }),
        );
      }
    });
    for (const t of symbol.texts) {
      const value = t.attribute ? text(t.attribute) : t.text;
      if (!value) continue;
      const color = this.color(symbol, t.role);
      this.labels.push({
        x: x0 + t.x,
        y: y1 - t.y,
        text: value,
        style: this.styles.add(`symlabel:${color}`, { fill: color }),
      });
    }
  }

  /** The layers of the symbols: the boxes under their size, the shapes and the lines over it */
  layers(): Layer[] {
    const out: Layer[] = [];
    for (const [lod, c] of [...this.byLod].sort((a, b) => a[0] - b[0])) {
      const boxes: RectsLayer = {
        id: `symbols:${lod}:boxes`,
        kind: "rects",
        x: Float32Array.from(c.boxes.x),
        y: Float32Array.from(c.boxes.y),
        w: Float32Array.from(c.boxes.w),
        h: Float32Array.from(c.boxes.h),
        style: Uint16Array.from(c.boxes.style),
        name: (i) => c.boxes.name[i],
        size: LOD_SIZE,
        maxPx: lod,
        inert: true,
      };
      const shapes: ShapesLayer = {
        id: `symbols:${lod}:shapes`,
        kind: "shapes",
        x: Float32Array.from(c.shapes.x),
        y: Float32Array.from(c.shapes.y),
        w: Float32Array.from(c.shapes.w),
        h: Float32Array.from(c.shapes.h),
        shape: Uint16Array.from(c.shapes.shape),
        shapes: this.defs,
        style: Uint16Array.from(c.shapes.style),
        size: LOD_SIZE,
        minPx: lod,
        inert: true,
      };
      const lines: PolylinesLayer = {
        id: `symbols:${lod}:lines`,
        kind: "polylines",
        xy: Float32Array.from(c.lines.xy),
        start: Uint32Array.from(c.lines.start),
        style: Uint16Array.from(c.lines.style),
        size: LOD_SIZE,
        minPx: lod,
        inert: true,
      };
      out.push(boxes, shapes, lines);
    }
    return out;
  }
}
