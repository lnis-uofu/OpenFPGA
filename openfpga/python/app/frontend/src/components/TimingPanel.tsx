// V6, the timing exploration (docs/plan_design_views.md, section 4): the histogram of the slack of
// the end points, the worst paths, and the delays of the path that is selected, point by point.
// Selecting a path selects its blocks and its nets: the placement draws it, the routing its nets.
import { useEffect, useState } from "preact/hooks";
import { config, openPanel } from "../store";
import { setDataSignature, wholeView } from "../views/data";
import { css } from "../views/palette";
import { refKey } from "../views/scene";
import { runViewCommand, selectRefs, selection } from "../views/selection";
import {
  type HistogramBin,
  type TimingPath,
  clusterNames,
  histogramOf,
  isNetDelay,
  loadTiming,
  pathRefs,
  pathsOf,
  slackColor,
} from "../views/timing";
import { viewColors } from "../views/palette";
import { sessionSignature } from "./ViewPanel";

interface TimingData {
  paths: TimingPath[];
  histogram: HistogramBin[];
  names: Map<number, string>;
}

const ns = (v: number) => (Number.isFinite(v) ? v.toFixed(3) : "—");

/** The histogram of the slack of the end points, with the slack of the selected path marked */
function Histogram(props: { bins: HistogramBin[]; marked: number | null }) {
  const { bins, marked } = props;
  if (!bins.length) return <div class="muted">No end point has a slack.</div>;
  const W = 600;
  const H = 120;
  const most = Math.max(...bins.map((b) => b.count));
  const least = bins[0].low;
  const span = bins[bins.length - 1].high - least || 1;
  const at = (v: number) => ((v - least) / span) * W;
  const range: [number, number] = [least, bins[bins.length - 1].high];
  const dark = viewColors().dark;
  const total = bins.reduce((n, b) => n + b.count, 0);
  return (
    <figure class="timing-histogram">
      <svg
        viewBox={`0 0 ${W} ${H + 16}`}
        role="img"
        aria-label={`The slack of ${total} end points`}
      >
        {bins.map((b, i) => {
          const h = most ? Math.max(b.count ? 2 : 0, (b.count / most) * H) : 0;
          const x = bins.length === 1 ? 0 : at(b.low);
          const w =
            bins.length === 1 ? W : Math.max(1, at(b.high) - at(b.low) - 1);
          return (
            <rect
              key={i}
              x={x}
              y={H - h}
              width={w}
              height={h}
              fill={css(slackColor((b.low + b.high) / 2, range, dark))}
            >
              <title>
                {b.count} end points with a slack from {ns(b.low)} to{" "}
                {ns(b.high)} ns
              </title>
            </rect>
          );
        })}
        {least < 0 && range[1] > 0 && (
          <line class="timing-zero" x1={at(0)} x2={at(0)} y1={0} y2={H} />
        )}
        {marked !== null && (
          <line
            class="timing-mark"
            x1={at(marked)}
            x2={at(marked)}
            y1={0}
            y2={H}
          />
        )}
        <text x={0} y={H + 13} class="timing-axis">
          {ns(least)} ns
        </text>
        <text x={W} y={H + 13} class="timing-axis" text-anchor="end">
          {ns(range[1])} ns
        </text>
      </svg>
      <figcaption class="muted">
        The setup slack of {total} end points
        {marked !== null ? "; the line is the selected path" : ""}
      </figcaption>
    </figure>
  );
}

/** The delays of a path, point by point, as one bar: on the nets and in the cells */
function DelayBar(props: { path: TimingPath }) {
  const total =
    props.path.points.reduce((t, p) => t + Math.max(0, p.delay), 0) || 1;
  return (
    <div class="timing-bar" role="img" aria-label="The delays of the path">
      {props.path.points.map((p, i) =>
        p.delay > 0 ? (
          <span
            key={i}
            class={isNetDelay(p) ? "net" : "cell"}
            style={{ flexGrow: p.delay / total }}
            title={`${p.pin} (${p.type}): +${ns(p.delay)} ns${p.net ? `, net ${p.net}` : ""}`}
          />
        ) : null,
      )}
    </div>
  );
}

function PathDetail(props: { path: TimingPath; names: Map<number, string> }) {
  const { path, names } = props;
  const net = path.points.filter(isNetDelay).reduce((t, p) => t + p.delay, 0);
  const show = (panel: "floorplan" | "routing") => {
    openPanel(panel);
    runViewCommand({ op: "zoom_to", kind: "path", name: String(path.index) });
  };
  return (
    <section class="timing-detail" aria-label={`Path ${path.index + 1}`}>
      <div class="timing-detail-head">
        <strong>Path {path.index + 1}</strong>
        <span class="muted">
          slack {ns(path.slack)} ns · arrival {ns(path.arrival)} ns · required{" "}
          {ns(path.required)} ns · {ns(net)} ns on nets,{" "}
          {ns(path.arrival - net)} ns in cells
        </span>
        <span class="spacer" />
        <button onClick={() => show("floorplan")}>Show on the placement</button>
        <button onClick={() => show("routing")}>Show on the routing</button>
      </div>
      <DelayBar path={path} />
      <div class="timing-key muted">
        <span class="timing-swatch net" /> on a net{" "}
        <span class="timing-swatch cell" /> in a cell
      </div>
      <div class="timing-table-wrap">
        <table class="timing-table">
          <thead>
            <tr>
              <th>Pin</th>
              <th>Type</th>
              <th>Net</th>
              <th>Block</th>
              <th class="num">Incr (ns)</th>
              <th class="num">Arrival (ns)</th>
            </tr>
          </thead>
          <tbody>
            {path.points.map((p, i) => {
              const block = names.get(p.cluster);
              return (
                <tr
                  key={i}
                  class={block ? "clickable" : ""}
                  title={
                    block
                      ? `Select ${block}${p.net ? ` and ${p.net}` : ""}`
                      : undefined
                  }
                  onClick={() => {
                    if (!block) return;
                    selectRefs(
                      [
                        { kind: "block", name: block },
                        ...(p.net
                          ? [{ kind: "net" as const, name: p.net }]
                          : []),
                      ],
                      `block ${block}`,
                    );
                    runViewCommand({
                      op: "zoom_to",
                      kind: "block",
                      name: block,
                    });
                  }}
                >
                  <td>{p.pin}</td>
                  <td>{p.type}</td>
                  <td>{p.net}</td>
                  <td>{block ?? ""}</td>
                  <td class="num">{ns(p.delay)}</td>
                  <td class="num">{ns(p.arrival)}</td>
                </tr>
              );
            })}
          </tbody>
        </table>
      </div>
    </section>
  );
}

export function TimingPanel() {
  const real = config.value?.backend === "session";
  const signature = sessionSignature();
  const [data, setData] = useState<TimingData | null>(null);
  const [failure, setFailure] = useState<string | null>(null);

  useEffect(() => {
    if (!real) return;
    setDataSignature(signature);
    setData(null);
    setFailure(null);
    let gone = false;
    Promise.all([loadTiming(), wholeView("placement")]).then(
      ([timing, placement]) => {
        if (gone) return;
        setData({
          paths: pathsOf(timing),
          histogram: histogramOf(timing),
          names: clusterNames(placement),
        });
      },
      (error) => !gone && setFailure(String(error?.message ?? error)),
    );
    return () => {
      gone = true;
    };
  }, [signature, real]);

  const chosenRef = selection.value.find((r) => r.kind === "path");
  const chosen =
    data && chosenRef ? (data.paths[Number(chosenRef.name)] ?? null) : null;
  const select = (path: TimingPath) => {
    if (!data) return;
    if (chosen?.index === path.index) runViewCommand({ op: "clear" });
    else selectRefs(pathRefs(path, data.names), `path ${path.index + 1}`);
  };
  const worst = data?.paths[0];
  return (
    <section class="panel timing-panel" aria-label="Timing">
      <div class="view-toolbar">
        <strong>Timing</strong>
        {worst && (
          <span class="muted">
            worst slack {ns(worst.slack)} ns ({worst.launch} to {worst.capture})
            · the {data!.paths.length} worst paths
          </span>
        )}
      </div>
      {!real ? (
        <div class="muted view-message">Not in the demo.</div>
      ) : failure ? (
        <div class="view-message" role="alert">
          {failure}
          <div class="muted">
            The timing needs the routing: run it (▶ of ROUTED in the sidebar).
          </div>
        </div>
      ) : !data ? (
        <div class="muted view-message">Analyzing the timing…</div>
      ) : (
        <div class="timing-body">
          <Histogram
            bins={data.histogram}
            marked={chosen ? chosen.slack : null}
          />
          <div class="timing-table-wrap timing-paths">
            <table class="timing-table">
              <thead>
                <tr>
                  <th class="num">#</th>
                  <th class="num">Slack (ns)</th>
                  <th>Start</th>
                  <th>End</th>
                  <th class="num">Points</th>
                  <th class="num">Arrival (ns)</th>
                </tr>
              </thead>
              <tbody>
                {data.paths.map((p) => (
                  <tr
                    key={p.index}
                    class={`clickable ${chosen?.index === p.index ? "selected" : ""}`}
                    aria-selected={chosen?.index === p.index}
                    onClick={() => select(p)}
                    title="Select this path: its blocks and its nets in the placement and the routing"
                  >
                    <td class="num">{p.index + 1}</td>
                    <td
                      class="num"
                      style={{
                        color: p.slack < 0 ? "var(--bad, #d64541)" : undefined,
                      }}
                    >
                      {ns(p.slack)}
                    </td>
                    <td>{p.start}</td>
                    <td>{p.end}</td>
                    <td class="num">{p.points.length}</td>
                    <td class="num">{ns(p.arrival)}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
          {chosen ? (
            <PathDetail
              key={refKey(chosenRef!)}
              path={chosen}
              names={data.names}
            />
          ) : (
            <div class="muted">
              Select a path for its delays, and to see it on the placement and
              the routing.
            </div>
          )}
        </div>
      )}
    </section>
  );
}
