import type { Answer } from "../api";

/** The drawings of the results of the tools that have a shape: bars of the utilization, the grid of the
 * placement, the heat map of the congestion. They draw the same data that the model reads (the
 * answer of the tool, as it is), so a number on the page is the number of the model. */

type Num = number | null | undefined;
interface Grid { width: number; height: number }

const data = (result: Answer): Record<string, unknown> | null =>
  result.ok && result.data && typeof result.data === "object" ? (result.data as Record<string, unknown>) : null;

/** The kinds that have a drawing */
export const DRAWN = new Set(["utilization", "placement", "congestion"]);

export function kindOf(result: Answer): string | null {
  const kind = data(result)?.kind;
  return typeof kind === "string" && DRAWN.has(kind) ? kind : null;
}

const percent = (v: Num) => (typeof v === "number" ? `${Math.round(v * 1000) / 10}%` : "–");

interface BlockType { name: string; is_io: boolean; used: number; available: number; utilization: number }

/** A bar for each type of block: how much of what the device has is used */
export function UtilizationDrawing(props: { data: Record<string, unknown> }) {
  const types = (props.data.block_types as BlockType[] | undefined) ?? [];
  const limiting = (props.data.limiting_resources as string[] | undefined) ?? [];
  return (
    <figure class="drawing drawing-utilization" aria-label="Utilization of the device">
      {types.map((t) => (
        <div key={t.name} class={`bar-row ${limiting.includes(t.name) ? "limiting" : ""}`} data-block-type={t.name}>
          <span class="bar-name">{t.name}</span>
          <span class="bar-track" role="img" aria-label={`${t.name}: ${t.used} of ${t.available}`}>
            <span class="bar-fill" style={{ width: `${Math.min(100, t.utilization * 100)}%` }} />
          </span>
          <span class="bar-value">
            {t.used} / {t.available} ({percent(t.utilization)})
          </span>
        </div>
      ))}
      <figcaption class="muted">
        Device utilization {percent(props.data.device_utilization as Num)}
        {limiting.length ? ` · limiting: ${limiting.join(", ")}` : ""}
      </figcaption>
    </figure>
  );
}

const PALETTE = ["#4e79a7", "#f28e2b", "#59a14f", "#b07aa1", "#edc948", "#76b7b2", "#e15759", "#9c755f"];

interface PlacedBlock { name: string; type: string; x: number; y: number; sub_tile?: number }

/** The grid of the device with a square for each block that is placed (a block of the answer: the
 * answer may hold only a part of them, and the caption says so) */
export function PlacementDrawing(props: { data: Record<string, unknown> }) {
  const grid = props.data.grid as Grid;
  const blocks = (props.data.blocks as PlacedBlock[] | undefined) ?? [];
  const cell = Math.max(4, Math.min(24, Math.floor(640 / Math.max(1, grid.width, grid.height))));
  // A tile that holds several blocks (the I/O tiles) is cut in slices, one for each sub tile
  const slices = new Map<string, number>();
  for (const b of blocks) slices.set(b.type, Math.max(slices.get(b.type) ?? 1, (b.sub_tile ?? 0) + 1));
  const types = [...new Set(blocks.map((b) => b.type))].sort();
  const color = (type: string) => PALETTE[types.indexOf(type) % PALETTE.length];
  return (
    <figure class="drawing drawing-placement" aria-label="Placement on the grid">
      <svg width={grid.width * cell} height={grid.height * cell} role="img" viewBox={`0 0 ${grid.width * cell} ${grid.height * cell}`}>
        <rect width={grid.width * cell} height={grid.height * cell} class="grid-bg" />
        {blocks.map((b) => {
          const n = slices.get(b.type) ?? 1;
          const width = (cell - 1) / n;
          return (
            <rect
              key={`${b.name}@${b.x},${b.y},${b.sub_tile ?? 0}`}
              class="block"
              data-block={b.name}
              x={b.x * cell + (b.sub_tile ?? 0) * width}
              y={(grid.height - 1 - b.y) * cell}
              width={width}
              height={cell - 1}
              fill={color(b.type)}
            >
              <title>{`${b.name} (${b.type}) at ${b.x}, ${b.y}`}</title>
            </rect>
          );
        })}
      </svg>
      <figcaption class="muted">
        {blocks.length} of {String(props.data.total ?? blocks.length)} blocks on a {grid.width} × {grid.height} grid
        {props.data.truncated ? " (the answer holds a part of them)" : ""}
        <span class="legend">
          {types.map((t) => (
            <span key={t} class="legend-item">
              <i style={{ background: color(t) }} /> {t}
            </span>
          ))}
        </span>
      </figcaption>
    </figure>
  );
}

interface Usage { used: number; capacity: number; occupancy: number }
interface Region { x_low: number; y_low: number; x_high: number; y_high: number; chanx: Usage; chany: Usage }

/** The most that a region is used: the larger of its horizontal and vertical channels */
export const regionOccupancy = (r: Region) => Math.max(r.chanx?.occupancy ?? 0, r.chany?.occupancy ?? 0);

/** Green to red as the occupancy goes from nothing to full; more than full (an overused node) is dark red */
function heat(v: number): string {
  if (v > 1) return "#7f0000";
  const hue = 120 - 120 * Math.min(1, v);
  return `hsl(${hue}, 70%, 45%)`;
}

/** A square for each region of the routing, coloured by how full its channels are */
export function CongestionDrawing(props: { data: Record<string, unknown> }) {
  const grid = props.data.grid as Grid;
  const regions = (props.data.regions as Region[] | undefined) ?? [];
  const summary = props.data.summary as { overused_nodes?: number } | undefined;
  const cell = Math.max(6, Math.min(40, Math.floor(640 / Math.max(1, grid.width, grid.height))));
  const worst = regions.reduce((m, r) => Math.max(m, regionOccupancy(r)), 0);
  return (
    <figure class="drawing drawing-congestion" aria-label="Congestion of the routing">
      <svg width={grid.width * cell} height={grid.height * cell} role="img" viewBox={`0 0 ${grid.width * cell} ${grid.height * cell}`}>
        <rect width={grid.width * cell} height={grid.height * cell} class="grid-bg" />
        {regions.map((r) => (
          <rect
            key={`${r.x_low},${r.y_low}`}
            class="region"
            data-occupancy={regionOccupancy(r)}
            x={r.x_low * cell}
            y={(grid.height - 1 - r.y_high) * cell}
            width={(r.x_high - r.x_low + 1) * cell - 1}
            height={(r.y_high - r.y_low + 1) * cell - 1}
            fill={heat(regionOccupancy(r))}
          >
            <title>{`x ${r.x_low}–${r.x_high}, y ${r.y_low}–${r.y_high}: horizontal ${percent(r.chanx?.occupancy)}, vertical ${percent(r.chany?.occupancy)}`}</title>
          </rect>
        ))}
      </svg>
      <figcaption class="muted">
        {regions.length} regions · fullest {percent(worst)} · overused nodes {summary?.overused_nodes ?? 0}
        {props.data.routed === false ? " · not routed" : ""}
      </figcaption>
    </figure>
  );
}

/** The drawing of a result that has one, or nothing */
export function Drawing(props: { result: Answer }) {
  const d = data(props.result);
  const kind = kindOf(props.result);
  if (!d || !kind) return null;
  if (kind === "utilization") return <UtilizationDrawing data={d} />;
  if (kind === "placement") return <PlacementDrawing data={d} />;
  return <CongestionDrawing data={d} />;
}
