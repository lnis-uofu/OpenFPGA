import { beforeEach, describe, expect, it, vi } from "vitest";
import { consoleLines, resetStore, tabs } from "../store";
import { attachController } from "./controller";
import { resetViewData, setDataSignature } from "./data";
import { floorplanView, type FloorplanBase } from "./floorplan";
import { makeView, type ViewData } from "./format";
import { logicBox, slotBox, trackOffset, wireSegment } from "./geometry";
import { pinsView, type PinsBase } from "./pins";
import { chooseBackend, p95 } from "./renderer";
import {
  addRouteTile,
  emptyShapes,
  routingView,
  type RoutingBase,
} from "./routing";
import {
  type Layer,
  type RectsLayer,
  type SegmentsLayer,
  itemBox,
  layerVisible,
} from "./scene";
import {
  highlighted,
  resetSelection,
  runViewCommand,
  selection,
  zoomRequest,
} from "./selection";
import { LayerIndex, SceneIndex } from "./spatial";
import { TILE, TileCache, levelFor, maxLevel, visibleTiles } from "./tiles";
import {
  type ViewState,
  fit,
  pan,
  toScreen,
  toWorld,
  visibleBox,
  zoomAt,
  zoomTo,
} from "./viewstate";

beforeEach(() => {
  resetStore();
  resetSelection();
  resetViewData();
});

const VIEW: ViewState = { cx: 10, cy: 5, scale: 20, width: 800, height: 600 };
const close = (a: number[], b: number[]) =>
  a.forEach((v, i) => expect(v).toBeCloseTo(b[i], 6));

describe("the view state", () => {
  it("goes from the world to the screen and back, y up", () => {
    const [px, py] = toScreen(VIEW, 12, 6);
    expect([px, py]).toEqual([440, 280]);
    close(toWorld(VIEW, px, py), [12, 6]);
    close(visibleBox(VIEW), [-10, -10, 30, 20]);
  });

  it("keeps the point under the cursor where it is when it zooms", () => {
    const before = toWorld(VIEW, 100, 450);
    const zoomed = zoomAt(VIEW, 100, 450, 3);
    expect(zoomed.scale).toBe(60);
    close(toWorld(zoomed, 100, 450), before);
  });

  it("pans by pixels, fits a box, and zooms to one no closer than its limit", () => {
    const moved = pan(VIEW, 40, -20);
    close([moved.cx, moved.cy], [8, 4]);
    const fitted = fit(VIEW, [0, 0, 34, 34], 16);
    expect(fitted.scale).toBeCloseTo((600 - 32) / 34);
    close([fitted.cx, fitted.cy], [17, 17]);
    expect(zoomTo(VIEW, [3, 3, 3.1, 3.1], 200).scale).toBe(200);
  });

  it("gives one view state for one sequence of input (P2.4)", () => {
    const run = () => {
      const element = document.createElement("div");
      element.getBoundingClientRect = () => ({
        left: 0,
        top: 0,
        width: 800,
        height: 600,
        right: 800,
        bottom: 600,
        x: 0,
        y: 0,
        toJSON: () => ({}),
      });
      let view = { ...VIEW };
      const picks: [number, number][] = [];
      attachController(element, {
        view: () => view,
        setView: (next) => (view = next),
        pick: (x, y) => picks.push([x, y]),
        hover: () => undefined,
        leave: () => undefined,
        fit: () => (view = fit(view, [0, 0, 34, 34])),
        clear: () => undefined,
      });
      const fire = (
        type: string,
        init: {
          button?: number;
          clientX: number;
          clientY: number;
          pointerId: number;
        },
      ) => {
        const event = new MouseEvent(type, {
          bubbles: true,
          button: init.button ?? 0,
          clientX: init.clientX,
          clientY: init.clientY,
        });
        Object.defineProperty(event, "pointerId", { value: init.pointerId });
        element.dispatchEvent(event);
      };
      element.dispatchEvent(
        new WheelEvent("wheel", { deltaY: -200, clientX: 300, clientY: 200 }),
      );
      fire("pointerdown", {
        button: 0,
        clientX: 300,
        clientY: 200,
        pointerId: 1,
      });
      fire("pointermove", { clientX: 340, clientY: 180, pointerId: 1 });
      fire("pointerup", { clientX: 340, clientY: 180, pointerId: 1 });
      fire("pointerdown", {
        button: 0,
        clientX: 50,
        clientY: 60,
        pointerId: 1,
      });
      fire("pointerup", { clientX: 51, clientY: 60, pointerId: 1 });
      element.dispatchEvent(new KeyboardEvent("keydown", { key: "+" }));
      element.dispatchEvent(new KeyboardEvent("keydown", { key: "ArrowLeft" }));
      return { view, picks };
    };
    const first = run();
    expect(run()).toEqual(first);
    expect(first.picks).toEqual([[51, 60]]); // a drag is not a click
    expect(first.view.scale).toBeCloseTo(20 * Math.pow(1.0015, 200) * 1.5);
  });
});

/** Rects at random places, n of them */
function randomRects(n: number): RectsLayer {
  let seed = 7;
  const r = () => (seed = (seed * 1664525 + 1013904223) >>> 0) / 4294967296;
  const layer: RectsLayer = {
    id: "r",
    kind: "rects",
    x: new Float32Array(n),
    y: new Float32Array(n),
    w: new Float32Array(n).fill(0.5),
    h: new Float32Array(n).fill(0.5),
    style: new Uint16Array(n),
    objectKind: "block",
    name: (i) => `b${i}`,
    size: 0.5,
  };
  for (let i = 0; i < n; i++) {
    layer.x[i] = r() * 500;
    layer.y[i] = r() * 500;
  }
  return layer;
}

describe("the spatial index", () => {
  it("picks one of 100k items in under 1 ms (P2.2)", () => {
    const layer = randomRects(100000);
    const index = new LayerIndex(layer);
    const picked = index.pick(layer.x[123] + 0.25, layer.y[123] + 0.25, 0.01);
    expect(picked?.item).toBe(123);
    const started = performance.now();
    for (let i = 0; i < 1000; i++)
      index.pick(layer.x[i] + 0.1, layer.y[i] + 0.1, 0.05);
    expect((performance.now() - started) / 1000).toBeLessThan(1);
  });

  it("picks a segment by its distance, and the top layer first, not an inert one", () => {
    const wire: SegmentsLayer = {
      id: "w",
      kind: "segments",
      xy: new Float32Array([0, 0, 10, 0, 0, 5, 10, 5]),
      style: new Uint16Array(2),
      objectKind: "net",
      name: (i) => `n${i}`,
      size: 1,
    };
    const index = new LayerIndex(wire);
    expect(index.pick(5, 0.3, 0.5)?.item).toBe(0);
    expect(index.pick(5, 2.5, 0.5)).toBeNull();
    expect(index.pick(5, 4.8, 0.5)?.item).toBe(1);
    const cover: RectsLayer = {
      id: "c",
      kind: "rects",
      x: new Float32Array([0]),
      y: new Float32Array([-1]),
      w: new Float32Array([10]),
      h: new Float32Array([2]),
      style: new Uint16Array(1),
      objectKind: "block",
      name: () => "b",
      size: 1,
      inert: true,
    };
    const scene = new SceneIndex({
      layers: [wire, cover],
      bounds: [0, 0, 10, 10],
      styles: [],
    });
    expect(scene.pick([wire, cover], 5, 0.1, 0.5)?.layer.id).toBe("w");
    expect(itemBox(wire, 1)).toEqual([0, 5, 10, 5]);
  });

  it("searches the items of a box", () => {
    const index = new LayerIndex(randomRects(1000));
    const found = index.search([0, 0, 50, 50]);
    const layer = index.layer as RectsLayer;
    expect(found.length).toBeGreaterThan(0);
    for (const i of found) expect(layer.x[i]).toBeLessThanOrEqual(50);
  });
});

describe("the levels of detail and the tiles", () => {
  it("draws a layer between the sizes on the screen it is made for", () => {
    const layer: Layer = {
      id: "l",
      kind: "points",
      x: new Float32Array(),
      y: new Float32Array(),
      radius: 2,
      style: new Uint16Array(),
      size: 0.5,
      minPx: 10,
      maxPx: 100,
    };
    expect(layerVisible(layer, 10)).toBe(false); // 5 px
    expect(layerVisible(layer, 40)).toBe(true); // 20 px
    expect(layerVisible(layer, 400)).toBe(false); // 200 px
  });

  it("asks for the objects close up and for a coarse level far out", () => {
    expect(maxLevel(34, 34)).toBe(2);
    expect(maxLevel(500, 500)).toBe(5);
    expect(levelFor(8, 6, 500, 500)).toBe(0);
    expect(levelFor(4, 6, 500, 500)).toBe(1);
    expect(levelFor(0.1, 6, 500, 500)).toBe(5);
    expect(levelFor(0.0001, 6, 34, 34)).toBe(2);
  });

  it("gives the tiles on the screen, inside the device", () => {
    const view: ViewState = {
      cx: 20,
      cy: 20,
      scale: 10,
      width: 400,
      height: 400,
    };
    expect(visibleTiles(view, 0, 34, 34)).toEqual([
      [0, 0],
      [1, 0],
      [2, 0],
      [0, 1],
      [1, 1],
      [2, 1],
      [0, 2],
      [1, 2],
      [2, 2],
    ]);
    expect(visibleTiles(view, 2, 34, 34)).toEqual([[0, 0]]);
    expect(TILE).toBe(16);
  });

  it("fetches a few tiles at a time, once each, and forgets them when the session changes", async () => {
    let running = 0;
    let most = 0;
    const fetch = vi.fn(
      async (kind: string, level: number, x: number, y: number) => {
        running++;
        most = Math.max(most, running);
        await new Promise((r) => setTimeout(r, 5));
        running--;
        return makeView(kind, {}, { level, x, y });
      },
    );
    const changed = vi.fn();
    const cache = new TileCache(changed, fetch, 2);
    for (let i = 0; i < 6; i++)
      expect(cache.get("grid", 0, i, 0)).toBeUndefined();
    cache.get("grid", 0, 0, 0);
    await vi.waitFor(() => expect(cache.pending).toBe(0));
    expect(fetch).toHaveBeenCalledTimes(6);
    expect(most).toBe(2);
    expect(cache.get("grid", 0, 3, 0)?.meta).toEqual({ level: 0, x: 3, y: 0 });
    expect(changed).toHaveBeenCalledTimes(6);
    cache.clear();
    expect(cache.get("grid", 0, 3, 0)).toBeUndefined();
  });

  it("keeps the refusal of a tile", async () => {
    const cache = new TileCache(
      () => undefined,
      () => Promise.reject(new Error("The design has no routing")),
    );
    cache.get("routing", 0, 0, 0);
    await vi.waitFor(() =>
      expect(cache.error("routing", 0, 0, 0)).toBe("The design has no routing"),
    );
  });
});

describe("the choice of the renderer", () => {
  const gpu = { ok: true, renderer: "ANGLE (NVIDIA)", software: false };
  it("is deck.gl unless WebGL2 is missing or software, or the person chose", () => {
    expect(chooseBackend("auto", gpu).kind).toBe("deck");
    expect(
      chooseBackend("auto", {
        ok: true,
        renderer: "SwiftShader",
        software: true,
      }),
    ).toEqual({
      kind: "canvas2d",
      reason: "WebGL is drawn in software (SwiftShader)",
    });
    expect(
      chooseBackend("auto", { ok: false, renderer: "", software: false }).kind,
    ).toBe("canvas2d");
    expect(
      chooseBackend("webgl", { ok: true, renderer: "llvmpipe", software: true })
        .kind,
    ).toBe("deck");
    expect(
      chooseBackend("webgl", { ok: false, renderer: "", software: false }).kind,
    ).toBe("canvas2d");
    expect(chooseBackend("canvas2d", gpu).kind).toBe("canvas2d");
  });

  it("measures the probe by its 95th percentile", () => {
    expect(p95([...Array(19).fill(10), 200])).toBe(200);
    expect(p95([...Array(40).fill(10), 200])).toBe(10);
  });
});

describe("the selection and the commands", () => {
  it("selects, adds, highlights, clears and asks to zoom, and logs each", () => {
    runViewCommand({ op: "select", kind: "block", name: "a" });
    runViewCommand({ op: "select", kind: "block", name: "b", add: true });
    expect(selection.value.map((r) => r.name)).toEqual(["a", "b"]);
    runViewCommand({ op: "select", kind: "net", name: "n" });
    expect(selection.value).toEqual([{ kind: "net", name: "n" }]);
    runViewCommand({ op: "highlight", kind: "net", name: "n" });
    expect(highlighted.value).toHaveLength(1);
    runViewCommand({ op: "zoom_to", kind: "block", name: "a" });
    expect(zoomRequest.value?.ref).toEqual({ kind: "block", name: "a" });
    runViewCommand({ op: "clear" });
    expect(selection.value).toEqual([]);
    runViewCommand({ op: "open_view", view: "routing" });
    expect(tabs.value.map((t) => t.id)).toContain("panel:routing");
    expect(consoleLines.value.map((l) => l.text)).toEqual([
      "view › select block a",
      "view › select block b",
      "view › select net n",
      "view › highlight net n",
      "view › zoom_to block a",
      "view › clear",
      "view › open_view routing",
    ]);
  });
});

/** A 4x4 device: I/O tiles of 3 slots on the sides, clbs inside, no corners */
function device(): ViewData {
  const x: number[] = [];
  const y: number[] = [];
  const type: number[] = [];
  for (let i = 0; i < 4; i++) {
    for (let j = 0; j < 4; j++) {
      const corner = (i === 0 || i === 3) && (j === 0 || j === 3);
      if (corner) continue;
      x.push(i);
      y.push(j);
      type.push(i === 0 || i === 3 || j === 0 || j === 3 ? 1 : 2);
    }
  }
  return makeView(
    "device",
    {
      tile_types: {
        name: ["EMPTY", "io", "clb"],
        width: Uint16Array.of(1, 1, 1),
        height: Uint16Array.of(1, 1, 1),
        capacity: Uint16Array.of(0, 3, 1),
        is_io: Uint8Array.of(0, 1, 0),
        is_empty: Uint8Array.of(1, 0, 0),
      },
      tiles: {
        x: Uint16Array.from(x),
        y: Uint16Array.from(y),
        w: new Uint16Array(x.length).fill(1),
        h: new Uint16Array(x.length).fill(1),
        type: Uint16Array.from(type),
        layer: new Uint8Array(x.length),
      },
    },
    { grid: { width: 4, height: 4 }, channel_width: { x: 4, y: 4 } },
  );
}

function placement(): ViewData {
  return makeView("placement", {
    block_types: { name: ["EMPTY", "io", "clb"] },
    blocks: {
      name: ["c1", "a", "out:y"],
      id: Uint32Array.of(0, 1, 2),
      type: Uint16Array.of(2, 1, 1),
      x: Int32Array.of(1, 0, 3),
      y: Int32Array.of(1, 1, 2),
      sub_tile: Int32Array.of(0, 2, 0),
      layer: new Uint8Array(3),
      fixed: Uint8Array.of(0, 1, 0),
    },
    partitions: { name: ["p"] },
    regions: {
      partition: Uint32Array.of(0),
      xmin: Int32Array.of(1),
      ymin: Int32Array.of(1),
      xmax: Int32Array.of(2),
      ymax: Int32Array.of(2),
    },
  });
}

function floorplanBase(): FloorplanBase {
  const d = device();
  const p = placement();
  const tileAt = new Map<string, number>();
  const tx = d.tables.tiles.columns.x as Uint16Array;
  const ty = d.tables.tiles.columns.y as Uint16Array;
  for (let i = 0; i < tx.length; i++) tileAt.set(`${tx[i]},${ty[i]}`, i);
  return {
    device: d,
    placement: p,
    unplaced: null,
    width: 4,
    height: 4,
    tileAt,
    blockRow: new Map([
      ["c1", 0],
      ["a", 1],
      ["out:y", 2],
    ]),
  };
}

const context = <B>(
  base: B,
  view: ViewState,
  extra: Partial<{ tiles: TileCache; hidden: Set<string> }> = {},
) => ({
  base,
  tiles:
    extra.tiles ??
    new TileCache(
      () => undefined,
      () => new Promise<ViewData>(() => undefined),
    ),
  view,
  selection: selection.value,
  highlighted: [],
  colors: {
    dark: false,
    background: [255, 255, 255, 255] as [number, number, number, number],
  },
  hidden: extra.hidden ?? new Set<string>(),
  changed: () => undefined,
});

describe("the floorplan", () => {
  it("draws every tile and every block on its slot close up, with the regions and the selection", () => {
    selection.value = [{ kind: "block", name: "a" }];
    const base = floorplanBase();
    const built = floorplanView.build(
      context(base, { cx: 2, cy: 2, scale: 100, width: 800, height: 600 }),
    );
    const layer = (id: string) =>
      built.scene.layers.find((l) => l.id === id) as RectsLayer;
    expect(layer("tiles").x.length).toBe(12);
    expect(layer("blocks").x.length).toBe(3);
    expect(layer("blocks").name!(1)).toBe("a");
    // a, an io block in slot 2 of 3 at 0,1, is in that slot of the logic of its tile
    close(
      [layer("blocks").x[1], layer("blocks").y[1]],
      slotBox(0, 1, 1, 1, 2, 3).slice(0, 2),
    );
    expect(layer("regions").w[0]).toBe(2);
    expect(layer("selection:blocks").x.length).toBe(1);
    expect(built.status).toContain("3 blocks");
    // The legend: the types of the tiles and of the blocks there are, the regions, the selection
    const labels = built.legend!.map((e) => e.label);
    expect(labels.slice(0, 1)).toEqual(["Tiles"]);
    expect(labels).toContain("Blocks");
    expect(labels).toContain("io");
    expect(labels).toContain("clb");
    expect(labels).toContain("Region");
    expect(labels).toContain("Selected");
    expect(floorplanView.describe(base, { kind: "block", name: "a" })).toBe(
      "a (io) at 0,1 slot 2, fixed",
    );
    close(
      floorplanView.locate(
        base,
        { kind: "block", name: "c1" },
        () => undefined,
      )!,
      slotBox(1, 1, 1, 1, 0, 1),
    );
  });

  it("draws the coarse tiles far out, as images", async () => {
    const base = floorplanBase();
    const fetch = vi.fn(async (kind: string) =>
      kind === "grid"
        ? makeView("grid", {
            cells: { type: new Uint16Array(256).fill(0xffff).fill(2, 0, 4) },
          })
        : makeView("placement", {
            cells: {
              used: new Float32Array(256).fill(1, 0, 2),
              slots: new Float32Array(256).fill(1),
              value: new Float32Array(256).fill(0.5),
            },
          }),
    );
    const tiles = new TileCache(() => undefined, fetch);
    const view = { cx: 2, cy: 2, scale: 2, width: 800, height: 600 };
    let built = floorplanView.build(context(base, view, { tiles }));
    expect(built.status).toContain("tiles loading");
    await vi.waitFor(() => expect(tiles.pending).toBe(0));
    built = floorplanView.build(context(base, view, { tiles }));
    const grids = built.scene.layers.filter((l) => l.kind === "grid");
    // A 4x4 device is one tile: level 1 is the coarsest
    expect(grids.map((g) => g.id)).toEqual(["grid:1:0:0", "use:1:0:0"]);
    expect(built.status).toContain("level 1");
    expect(built.legend!.find((e) => e.ramp)?.label).toBe("Use of the cells");
  });

  it("hides the groups that the person turned off", () => {
    const built = floorplanView.build(
      context(
        floorplanBase(),
        { cx: 2, cy: 2, scale: 100, width: 800, height: 600 },
        { hidden: new Set(["blocks", "regions"]) },
      ),
    );
    expect(built.scene.layers.map((l) => l.id)).toEqual(["tiles", "labels"]);
  });
});

describe("the pins", () => {
  const base = (): PinsBase => {
    const pins = makeView(
      "pins",
      {
        sites: {
          package_pin: ["A1", "", "", ...Array(27).fill("")],
          x: Uint16Array.from(
            { length: 30 },
            (_, i) => [0, 3, 1, 2][Math.floor(i / 3) % 4],
          ),
          y: Uint16Array.from(
            { length: 30 },
            (_, i) => 1 + (Math.floor(i / 12) % 2),
          ),
          sub_tile: Uint16Array.from({ length: 30 }, (_, i) => i % 3),
          side: new Uint8Array(30),
          layer: new Uint8Array(30),
        },
        ports: {
          name: ["a", "y", "z"],
          dir: Uint8Array.of(0, 1, 1),
          site: Int32Array.of(0, 4, -1),
          fixed: Uint8Array.of(1, 0, 0),
          cluster: Int32Array.of(1, 2, -1),
          block: ["a", "out:y", ""],
        },
      },
      { io_map_file: "io_map.csv" },
    );
    const capacity = new Map<string, number>();
    const xs = pins.tables.sites.columns.x as Uint16Array;
    const ys = pins.tables.sites.columns.y as Uint16Array;
    for (let i = 0; i < xs.length; i++)
      capacity.set(
        `${xs[i]},${ys[i]}`,
        (capacity.get(`${xs[i]},${ys[i]}`) ?? 0) + 1,
      );
    return {
      device: device(),
      pins,
      width: 4,
      height: 4,
      portOfBlock: new Map([
        ["a", 0],
        ["out:y", 1],
      ]),
      unplaced: ["z"],
      capacity,
    };
  };

  it("draws the sites, the ports on them by their block, and says which have no site", () => {
    selection.value = [{ kind: "block", name: "out:y" }];
    const b = base();
    const built = pinsView.build(
      context(b, { cx: 2, cy: 2, scale: 100, width: 800, height: 600 }),
    );
    const ports = built.scene.layers.find(
      (l) => l.id === "ports",
    ) as RectsLayer;
    expect(ports.x.length).toBe(2);
    expect([ports.name!(0), ports.name!(1)]).toEqual(["a", "out:y"]);
    expect(built.scene.layers.find((l) => l.id === "selection")).toBeTruthy();
    expect(built.status).toBe(
      "3 ports · 1 with no site yet · package pins from io_map.csv",
    );
    expect(built.legend!.map((e) => e.label)).toEqual(
      expect.arrayContaining(["Input port", "Output port", "Selected"]),
    );
    expect(pinsView.describe(b, { kind: "block", name: "a" })).toBe(
      "a (input) at 0,1 slot 0, package pin A1, constrained",
    );
    expect(pinsView.describe(b, { kind: "block", name: "nope" })).toBe(
      "block nope",
    );
  });
});

describe("the routing", () => {
  /** A tile of the routing: net n0 (OPIN, CHANX, CHANY, IPIN) and net n1 (a CHANX that the tile
   * next to it has too) */
  const tile = () =>
    makeView("routing", {
      nets: { name: ["n0", "n1"], net: Uint32Array.of(5, 9) },
      nodes: {
        net: Uint32Array.of(0, 0, 0, 0, 1),
        index: Int32Array.of(1, 2, 3, 4, 2),
        parent: Int32Array.of(0, 1, 2, 3, 1),
        type: Uint8Array.of(3, 4, 5, 2, 4),
        xlow: Uint16Array.of(1, 1, 2, 2, 1),
        ylow: Uint16Array.of(1, 1, 1, 2, 2),
        xhigh: Uint16Array.of(1, 2, 2, 2, 4),
        yhigh: Uint16Array.of(1, 1, 2, 2, 2),
        ptc: Uint16Array.of(0, 3, 1, 5, 0),
        layer: new Uint8Array(5),
        sink: Uint8Array.of(0, 0, 0, 1, 0),
      },
    });

  it("draws the wires of a net on their tracks, its pins, and the links to their parents (P4.2)", () => {
    const shapes = emptyShapes();
    const seen = new Set<string>();
    addRouteTile(tile(), 4, 4, shapes, seen);
    // The wires of n0 are its CHANX and CHANY nodes
    expect(shapes.wires.net.filter((n) => n === "n0")).toHaveLength(2);
    expect(shapes.wires.xy.slice(0, 4)).toEqual(
      wireSegment(true, 1, 1, 2, 1, 3, 4),
    );
    expect(shapes.wires.xy[1]).toBeCloseTo(1 + trackOffset(3, 4));
    expect(shapes.pins.net).toEqual(["n0", "n0"]);
    // OPIN -> CHANX -> CHANY -> IPIN: the CHANX and the CHANY cross, so two links are drawn
    expect(shapes.links.net.filter((n) => n === "n0")).toHaveLength(2);
    // The same tile again (a wire in two tiles): nothing twice
    addRouteTile(tile(), 4, 4, shapes, seen);
    expect(shapes.wires.net).toHaveLength(3);
  });

  it("draws the routes close up, the congestion far out, and the selected net over all", async () => {
    selection.value = [{ kind: "net", name: "n0" }];
    const base: RoutingBase = {
      device: device(),
      width: 4,
      height: 4,
      chanx: 4,
      chany: 4,
    };
    const fetch = vi.fn(async (kind: string, level: number) =>
      kind === "routing"
        ? tile()
        : makeView(
            "channels",
            level === 0
              ? {
                  wires: {
                    type: Uint8Array.of(4),
                    xlow: Uint16Array.of(1),
                    ylow: Uint16Array.of(1),
                    xhigh: Uint16Array.of(2),
                    yhigh: Uint16Array.of(1),
                    ptc: Uint16Array.of(0),
                    layer: Uint8Array.of(0),
                    used: Uint8Array.of(1),
                    capacity: Uint8Array.of(1),
                  },
                }
              : {
                  cells: {
                    used: new Float32Array(256).fill(2, 0, 3),
                    capacity: new Float32Array(256).fill(8, 0, 4),
                    value: new Float32Array(256).fill(0.25, 0, 3),
                    peak: new Float32Array(256),
                  },
                },
          ),
    );
    const tiles = new TileCache(() => undefined, fetch);
    const close = { cx: 2, cy: 2, scale: 30, width: 800, height: 600 };
    routingView.build(context(base, close, { tiles }));
    await vi.waitFor(() => expect(tiles.pending).toBe(0));
    const built = routingView.build(context(base, close, { tiles }));
    const ids = built.scene.layers.map((l) => l.id);
    expect(ids).toEqual(["tiles", "links", "wires", "pins", "selection:nets"]);
    const selected = built.scene.layers.find(
      (l) => l.id === "selection:nets",
    ) as SegmentsLayer;
    expect(selected.xy.length / 4).toBe(2 + 2); // its wires and its links
    expect(built.status).toContain("zoom in for the tracks");
    expect(built.legend!.map((e) => e.label)).toEqual([
      "Tile",
      "A net (a colour each)",
      "Selected net",
    ]);
    // Far out: the use of the channels, NaN where there is none
    const far = { cx: 2, cy: 2, scale: 2, width: 800, height: 600 };
    routingView.build(context(base, far, { tiles }));
    await vi.waitFor(() => expect(tiles.pending).toBe(0));
    const coarse = routingView.build(context(base, far, { tiles }));
    const grid = coarse.scene.layers.find((l) => l.kind === "grid");
    expect(grid?.kind === "grid" && Array.from(grid.value.slice(0, 5))).toEqual(
      [0.25, 0.25, 0.25, 0, NaN],
    );
    expect(coarse.legend!.map((e) => e.label)).toEqual(["Use of the channels"]);
    // Very close: the tracks
    const tracks = { cx: 2, cy: 2, scale: 300, width: 800, height: 600 };
    routingView.build(context(base, tracks, { tiles }));
    await vi.waitFor(() => expect(tiles.pending).toBe(0));
    expect(
      routingView
        .build(context(base, tracks, { tiles }))
        .scene.layers.map((l) => l.id),
    ).toContain("tracks");
  });
});

describe("the geometry of the device views", () => {
  it("keeps the logic of a tile in its lower left and the channels beside it", () => {
    const [x0, y0, x1, y1] = logicBox(3, 4);
    expect([x0, y0]).toEqual([3.04, 4.04]);
    expect(x1).toBeCloseTo(3.62);
    expect(y1).toBeCloseTo(4.62);
    expect(trackOffset(0, 10)).toBeGreaterThan(0.62);
    expect(trackOffset(9, 10)).toBeLessThan(1);
    const [, , sx1, sy1] = slotBox(0, 0, 1, 1, 8, 9);
    expect(sx1).toBeLessThanOrEqual(0.62);
    expect(sy1).toBeLessThanOrEqual(0.62);
  });

  it("follows the state of the session for its data", () => {
    setDataSignature("a");
    setDataSignature("a");
    setDataSignature("b");
  });
});
