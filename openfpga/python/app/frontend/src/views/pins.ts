// V4, the pins (docs/plan_design_views.md, section 4), read only: the I/O sites of the device on
// their tiles, the pin of the package on each (from the I/O map of the device package), and the
// ports of the design on them. A port is selected as its block, so that the floorplan shows it too.
import { type ViewData, numbers, texts } from "./format";
import { wholeView } from "./data";
import { rects } from "./floorplan";
import { ioStack, slotBox } from "./geometry";
import { coordinateLayer, tileLayer } from "./grid";
import type { Layer } from "./scene";
import { type LegendEntry, StyleTable, type ViewDef } from "./view";

export interface PinsBase {
  device: ViewData;
  pins: ViewData;
  width: number;
  height: number;
  /** The site of a block of a port, and the row of the port */
  portOfBlock: Map<string, number>;
  /** The ports that have no site (not placed yet) */
  unplaced: string[];
  /** The sites of each I/O tile: its capacity, by "x,y" */
  capacity: Map<string, number>;
  layers?: {
    layers: Layer[];
    styles: StyleTable;
    tileLegend: LegendEntry[];
    dark: boolean;
  };
}

export async function loadPins(): Promise<PinsBase> {
  const [device, pins] = await Promise.all([
    wholeView("device"),
    wholeView("pins"),
  ]);
  const grid = device.meta.grid as { width: number; height: number };
  const portOfBlock = new Map<string, number>();
  const unplaced: string[] = [];
  const names = texts(pins, "ports", "name");
  const blocks = texts(pins, "ports", "block");
  const site = numbers(pins, "ports", "site");
  for (let i = 0; i < names.length; i++) {
    if (blocks.get(i)) portOfBlock.set(blocks.get(i), i);
    if (site[i] < 0) unplaced.push(names.get(i));
  }
  const capacity = new Map<string, number>();
  const xs = numbers(pins, "sites", "x");
  const ys = numbers(pins, "sites", "y");
  for (let i = 0; i < xs.length; i++)
    capacity.set(
      `${xs[i]},${ys[i]}`,
      (capacity.get(`${xs[i]},${ys[i]}`) ?? 0) + 1,
    );
  return {
    device,
    pins,
    width: grid.width,
    height: grid.height,
    portOfBlock,
    unplaced,
    capacity,
  };
}

/** The box of a site, by its row */
export function siteBox(
  base: PinsBase,
  row: number,
): [number, number, number, number] {
  const x = numbers(base.pins, "sites", "x")[row];
  const y = numbers(base.pins, "sites", "y")[row];
  const slot = numbers(base.pins, "sites", "sub_tile")[row];
  return slotBox(
    x,
    y,
    1,
    1,
    slot,
    base.capacity.get(`${x},${y}`) ?? 1,
    ioStack(x, y, 1, 1, base.width, base.height),
  );
}

function layersOf(
  base: PinsBase,
  dark: boolean,
): { layers: Layer[]; styles: StyleTable; tileLegend: LegendEntry[] } {
  const styles = new StyleTable();
  const { device, pins } = base;
  // The tiles, as in the placement and the routing
  const tiles = tileLayer(device, styles, dark);
  const layers: Layer[] = [tiles.layer];
  // The sites: free, or with a pin of the package
  const packagePin = texts(pins, "sites", "package_pin");
  const siteBoxes: [number, number, number, number][] = [];
  const siteStyle = new Uint16Array(packagePin.length);
  for (let i = 0; i < packagePin.length; i++) {
    siteBoxes.push(siteBox(base, i));
    siteStyle[i] = packagePin.get(i)
      ? styles.add("site-pin", {
          fill: dark ? [70, 80, 60, 255] : [222, 236, 210, 255],
          stroke: [120, 150, 90, 255],
          width: 1,
        })
      : styles.add("site", {
          fill: dark ? [50, 54, 62, 255] : [250, 250, 250, 255],
          stroke: dark ? [90, 95, 105, 255] : [180, 180, 180, 255],
          width: 1,
        });
  }
  layers.push(
    rects("sites", siteBoxes, 0, {
      style: siteStyle,
      objectKind: "site",
      name: (i) =>
        `${numbers(pins, "sites", "x")[i]},${numbers(pins, "sites", "y")[i]}.${numbers(pins, "sites", "sub_tile")[i]}${packagePin.get(i) ? ` ${packagePin.get(i)}` : ""}`,
      size: 0.15,
    }),
  );
  // The ports on their sites, by direction
  const names = texts(pins, "ports", "name");
  const blocks = texts(pins, "ports", "block");
  const site = numbers(pins, "ports", "site");
  const dir = numbers(pins, "ports", "dir");
  const placed = Array.from({ length: names.length }, (_, i) => i).filter(
    (i) => site[i] >= 0,
  );
  const portBoxes = placed.map((i) => siteBoxes[site[i]]);
  const portStyle = Uint16Array.from(placed, (i) =>
    dir[i] === 0
      ? styles.add("in", {
          fill: [60, 140, 220, 255],
          stroke: [20, 60, 120, 255],
          width: 1,
        })
      : styles.add("out", {
          fill: [230, 140, 50, 255],
          stroke: [120, 60, 10, 255],
          width: 1,
        }),
  );
  layers.push(
    rects("ports", portBoxes, 0, {
      style: portStyle,
      objectKind: "block",
      name: (k) => blocks.get(placed[k]) || names.get(placed[k]),
      size: 0.15,
    }),
  );
  // Names: the port, or the pin of the package of a free site
  const label = styles.add("label", {
    fill: dark ? [230, 230, 230, 255] : [25, 25, 25, 255],
  });
  const texts_: string[] = [];
  const lx: number[] = [];
  const ly: number[] = [];
  const used = new Set(placed.map((i) => site[i]));
  placed.forEach((i, k) => {
    texts_.push(
      packagePin.get(site[i])
        ? `${names.get(i)} · ${packagePin.get(site[i])}`
        : names.get(i),
    );
    lx.push((portBoxes[k][0] + portBoxes[k][2]) / 2);
    ly.push((portBoxes[k][1] + portBoxes[k][3]) / 2);
  });
  for (let s = 0; s < packagePin.length; s++) {
    if (used.has(s) || !packagePin.get(s)) continue;
    texts_.push(packagePin.get(s));
    lx.push((siteBoxes[s][0] + siteBoxes[s][2]) / 2);
    ly.push((siteBoxes[s][1] + siteBoxes[s][3]) / 2);
  }
  layers.push({
    id: "labels",
    kind: "labels",
    x: Float32Array.from(lx),
    y: Float32Array.from(ly),
    text: (i) => texts_[i],
    count: texts_.length,
    px: 10,
    style: new Uint16Array(texts_.length).fill(label),
    size: 0.15,
    minPx: 70,
    inert: true,
  });
  const coordinates = coordinateLayer(device, styles, dark);
  if (coordinates) layers.push(coordinates);
  return { layers, styles, tileLegend: tiles.legend };
}

export const pinsView: ViewDef<PinsBase> = {
  needs: "DEVICE",
  load: loadPins,
  bounds: (base) => [0, 0, base.width, base.height],
  groups: [
    { id: "tiles", label: "Tiles" },
    { id: "sites", label: "Sites" },
    { id: "ports", label: "Ports" },
    { id: "labels", label: "Names" },
    { id: "coordinates", label: "Coordinates" },
  ],
  key: (_base, _view, hidden) => [...hidden].sort().join(","),
  build({ base, colors, hidden, selection }) {
    if (!base.layers || base.layers.dark !== colors.dark)
      base.layers = { ...layersOf(base, colors.dark), dark: colors.dark };
    const { styles } = base.layers;
    const layers = base.layers.layers.filter((l) => !hidden.has(l.id));
    const selected: [number, number, number, number][] = [];
    for (const ref of selection) {
      const port =
        ref.kind === "block" ? base.portOfBlock.get(ref.name) : undefined;
      const site =
        port === undefined ? -1 : numbers(base.pins, "ports", "site")[port];
      if (site >= 0) selected.push(siteBox(base, site));
    }
    if (selected.length)
      layers.push(
        rects(
          "selection",
          selected,
          styles.add("selected", { stroke: [250, 80, 40, 255], width: 3 }),
          { inert: true },
        ),
      );
    const ports = texts(base.pins, "ports", "name").length;
    const mapFile = base.pins.meta.io_map_file as string | null;
    const status = [
      `${ports} ports`,
      base.unplaced.length ? `${base.unplaced.length} with no site yet` : "",
      mapFile ? `package pins from ${mapFile}` : "no I/O map in the package",
    ]
      .filter(Boolean)
      .join(" · ");
    const legend: LegendEntry[] = [
      ...(hidden.has("tiles") ? [] : base.layers.tileLegend),
      ...(hidden.has("sites")
        ? []
        : styles.legend([
            ["site-pin", "Site with a package pin"],
            ["site", "Site"],
          ])),
      ...(hidden.has("ports")
        ? []
        : styles.legend([
            ["in", "Input port"],
            ["out", "Output port"],
          ])),
      ...(selected.length ? styles.legend([["selected", "Selected"]]) : []),
    ];
    return {
      scene: {
        layers,
        bounds: [0, 0, base.width, base.height],
        styles: styles.list,
      },
      status,
      legend,
    };
  },
  locate(base, ref) {
    if (ref.kind !== "block") return null;
    const port = base.portOfBlock.get(ref.name);
    const site =
      port === undefined ? -1 : numbers(base.pins, "ports", "site")[port];
    return site >= 0 ? siteBox(base, site) : null;
  },
  describe(base, ref) {
    if (ref.kind === "block") {
      const port = base.portOfBlock.get(ref.name);
      if (port === undefined) return `block ${ref.name}`;
      const name = texts(base.pins, "ports", "name").get(port);
      const dir =
        numbers(base.pins, "ports", "dir")[port] === 0 ? "input" : "output";
      const site = numbers(base.pins, "ports", "site")[port];
      if (site < 0) return `${name} (${dir}): no site yet`;
      const pin = texts(base.pins, "sites", "package_pin").get(site);
      const fixed = numbers(base.pins, "ports", "fixed")[port]
        ? ", constrained"
        : ", by the placer";
      return `${name} (${dir}) at ${numbers(base.pins, "sites", "x")[site]},${numbers(base.pins, "sites", "y")[site]} slot ${numbers(base.pins, "sites", "sub_tile")[site]}${pin ? `, package pin ${pin}` : ""}${fixed}`;
    }
    return `${ref.kind} ${ref.name}`;
  },
};
