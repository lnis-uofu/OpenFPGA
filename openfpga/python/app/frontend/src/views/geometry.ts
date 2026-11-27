// Where things are in the world of the device views: one unit a grid tile, row 0 at the bottom. The
// logic of a tile is its lower left part, the channels are the bands above it (CHANX of its row) and
// to its right (CHANY of its column), and a track is at its ptc across its band. V3 and V5 use the
// same, so that a block of the floorplan is where its pins are in the routing.

export const LOGIC = 0.62; // the share of a grid tile that its logic takes
export const MARGIN = 0.04;
const BAND0 = LOGIC + 0.03;
const BAND = 1 - BAND0 - 0.02;

/** The box of the logic of a tile of w by h grid tiles at x, y */
export function logicBox(
  x: number,
  y: number,
  w = 1,
  h = 1,
): [number, number, number, number] {
  return [x + MARGIN, y + MARGIN, x + w - 1 + LOGIC, y + h - 1 + LOGIC];
}

/** How the slots of a tile are laid out: in a column (a tile on the left or right edge of the
 * grid), in a row (on the bottom or top edge), or in a square (anywhere else) */
export type SlotStack = "column" | "row" | "square";

/** The stack of the slots of an I/O tile of w by h at x, y on a grid of width by height: along
 * its edge of the fabric, or in a square in the middle */
export function ioStack(
  x: number,
  y: number,
  w: number,
  h: number,
  width: number,
  height: number,
): SlotStack {
  if (x === 0 || x + w === width) return "column";
  if (y === 0 || y + h === height) return "row";
  return "square";
}

/** The box of a slot (a sub-tile) of a tile with capacity slots */
export function slotBox(
  x: number,
  y: number,
  w: number,
  h: number,
  slot: number,
  capacity: number,
  stack: SlotStack = "square",
): [number, number, number, number] {
  const [x0, y0, x1, y1] = logicBox(x, y, w, h);
  if (capacity <= 1) return [x0 + 0.06, y0 + 0.06, x1 - 0.06, y1 - 0.06];
  const cols =
    stack === "column"
      ? 1
      : stack === "row"
        ? capacity
        : Math.ceil(Math.sqrt(capacity));
  const rows = Math.ceil(capacity / cols);
  const cw = (x1 - x0) / cols;
  const rh = (y1 - y0) / rows;
  const c = slot % cols;
  const r = Math.floor(slot / cols);
  return [
    x0 + c * cw + cw * 0.08,
    y1 - (r + 1) * rh + rh * 0.08,
    x0 + (c + 1) * cw - cw * 0.08,
    y1 - r * rh - rh * 0.08,
  ];
}

/** The place of a track across its channel: the offset in the band above (CHANX) or right of
 * (CHANY) its grid tile */
export function trackOffset(ptc: number, width: number): number {
  return BAND0 + (BAND * (ptc + 0.5)) / Math.max(1, width);
}

/** A wire of a channel as a segment: CHANX from xlow to xhigh at its row, CHANY from ylow to yhigh at
 * its column */
export function wireSegment(
  horizontal: boolean,
  xlow: number,
  ylow: number,
  xhigh: number,
  yhigh: number,
  ptc: number,
  width: number,
): [number, number, number, number] {
  const o = trackOffset(ptc, width);
  return horizontal
    ? [xlow, ylow + o, xhigh + 1, ylow + o]
    : [xlow + o, ylow, xlow + o, yhigh + 1];
}

/** Where a pin of a tile is drawn: outputs on the right of the logic, inputs at its top */
export function pinPoint(
  output: boolean,
  x: number,
  y: number,
  ptc: number,
): [number, number] {
  const t = 0.1 + 0.8 * ((ptc * 0.618) % 1);
  return output
    ? [x + LOGIC, y + MARGIN + t * (LOGIC - MARGIN)]
    : [x + MARGIN + t * (LOGIC - MARGIN), y + LOGIC];
}
