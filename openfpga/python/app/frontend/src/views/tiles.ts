// The tiles of a view of the device (docs/plan_design_views.md, 3.1.1): which tiles a view state
// needs, at which level, and a cache of them that fetches a few at a time.
import { type ViewData, fetchTile } from "./format";
import { type ViewState, visibleBox } from "./viewstate";

/** The side of a tile at level 0, in grid tiles (VIEW_TILE_SIZE of the core) */
export const TILE = 16;

/** The coarsest level that is needed: one tile covers the device */
export function maxLevel(gridWidth: number, gridHeight: number): number {
  let level = 0;
  while (TILE << level < Math.max(gridWidth, gridHeight)) level++;
  return level;
}

/** The level for a scale: 0 (the objects) when a grid tile is objectPx pixels or more, else the
 * level whose cells are about cellPx pixels */
export function levelFor(
  scale: number,
  objectPx: number,
  gridWidth: number,
  gridHeight: number,
  cellPx = 6,
): number {
  if (scale >= objectPx) return 0;
  const level = Math.max(1, Math.ceil(Math.log2(cellPx / scale)));
  return Math.min(Math.max(1, maxLevel(gridWidth, gridHeight)), level);
}

/** The tiles of a level that a view state shows */
export function visibleTiles(
  view: ViewState,
  level: number,
  gridWidth: number,
  gridHeight: number,
): [number, number][] {
  const span = TILE << level;
  const [x0, y0, x1, y1] = visibleBox(view);
  const tiles: [number, number][] = [];
  const lastX = Math.floor((gridWidth - 1) / span);
  const lastY = Math.floor((gridHeight - 1) / span);
  for (
    let ty = Math.max(0, Math.floor(y0 / span));
    ty <= Math.min(lastY, Math.floor(y1 / span));
    ty++
  ) {
    for (
      let tx = Math.max(0, Math.floor(x0 / span));
      tx <= Math.min(lastX, Math.floor(x1 / span));
      tx++
    )
      tiles.push([tx, ty]);
  }
  return tiles;
}

interface Entry {
  data?: ViewData;
  error?: string;
  pending: boolean;
}

type Fetch = typeof fetchTile;

/** The tiles of the kinds of a view: fetched when asked for, a few at a time, kept until clear() */
export class TileCache {
  private entries = new Map<string, Entry>();
  private queue: [string, string, number, number, number][] = [];
  private running = 0;
  private generation = 0;

  constructor(
    private readonly changed: () => void,
    private readonly fetch: Fetch = fetchTile,
    private readonly parallel = 4,
  ) {}

  /** The tile if it is there; else it is asked for, and changed() is called when it comes */
  get(kind: string, level: number, x: number, y: number): ViewData | undefined {
    const key = `${kind}/${level}/${x}/${y}`;
    const entry = this.entries.get(key);
    if (entry) return entry.data;
    this.entries.set(key, { pending: true });
    this.queue.push([key, kind, level, x, y]);
    this.pump();
    return undefined;
  }

  /** An error of a tile that was asked for, if any */
  error(kind: string, level: number, x: number, y: number): string | undefined {
    return this.entries.get(`${kind}/${level}/${x}/${y}`)?.error;
  }

  get pending(): number {
    return this.running + this.queue.length;
  }

  /** Forget everything: the data of the session changed */
  clear() {
    this.entries.clear();
    this.queue = [];
    this.generation++;
  }

  private pump() {
    while (this.running < this.parallel && this.queue.length) {
      const [key, kind, level, x, y] = this.queue.shift()!;
      const generation = this.generation;
      this.running++;
      this.fetch(kind, level, x, y)
        .then(
          (data) => {
            if (generation === this.generation)
              this.entries.set(key, { data, pending: false });
          },
          (error) => {
            if (generation === this.generation)
              this.entries.set(key, {
                error: String(error?.message ?? error),
                pending: false,
              });
          },
        )
        .finally(() => {
          this.running--;
          if (generation === this.generation) this.changed();
          this.pump();
        });
    }
  }
}
