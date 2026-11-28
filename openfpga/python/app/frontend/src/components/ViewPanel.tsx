// The panel of a view of the design (docs/plan_design_views.md, P2.5): a tab with the drawing, a
// toolbar (fit, the layers, the renderer), what is selected, and what is missing when the session
// has no data for it. It does the same for every view; the view gives its data and its scene.
import { signal } from "@preact/signals";
import { useEffect, useRef, useState } from "preact/hooks";
import { config, view as sessionView } from "../store";
import { Canvas2dRenderer } from "../views/canvas2d";
import { attachController } from "../views/controller";
import { setDataSignature } from "../views/data";
import { css, rampColor, viewColors } from "../views/palette";
import {
  type BackendChoice,
  type BackendKind,
  PROBE_FRAMES,
  PROBE_LIMIT_MS,
  type Renderer,
  backendChoice,
  chooseBackend,
  p95,
  setBackendChoice,
} from "../views/renderer";
import {
  type Layer,
  type ObjectRef,
  type Scene,
  itemCount,
  layerVisible,
  refKey,
} from "../views/scene";
import {
  highlighted,
  runViewCommand,
  selection,
  zoomRequest,
} from "../views/selection";
import { SceneIndex } from "../views/spatial";
import { TileCache } from "../views/tiles";
import { openMenu } from "./ContextMenu";
import type { LegendEntry, ViewDef } from "../views/view";
import {
  type ViewState,
  fit,
  resized,
  toWorld,
  zoomAt,
  zoomTo,
} from "../views/viewstate";

/** What a test sees of a panel (window.openfpgaViews[name]) */
export interface ViewProbe {
  view(): ViewState;
  scene(): Scene | null;
  backend(): BackendKind | null;
  /** The items of a layer that is drawn, by its id */
  drawn(id: string): number;
  /** Where an object is on the screen, if the view has it */
  screenOf(ref: ObjectRef): [number, number] | null;
}

declare global {
  interface Window {
    openfpgaViews?: Record<string, ViewProbe>;
  }
}

/** What the probe of auto found, once for the page: a view opened again draws at once with it,
 * with no probe */
let probed: { kind: BackendKind; reason: string } | null = null;

/** The swatch of a line of the legend */
function Swatch(props: { entry: LegendEntry }) {
  const { entry } = props;
  if (entry.ramp) {
    const stops = [0, 0.33, 0.67, 1].map((t) =>
      css(rampColor(entry.ramp!.ramp, t)),
    );
    return (
      <span class="legend-ramp">
        <span class="muted">{entry.ramp.from}</span>
        <span
          class="legend-bar"
          style={{
            background: `linear-gradient(to right, ${stops.join(", ")})`,
          }}
        />
        <span class="muted">{entry.ramp.to}</span>
      </span>
    );
  }
  if (entry.line)
    return (
      <span
        class="legend-line"
        style={{
          background: css(entry.stroke ?? entry.fill ?? [128, 128, 128, 255]),
        }}
      />
    );
  return (
    <span
      class="legend-box"
      style={{
        background: entry.fill ? css(entry.fill) : "transparent",
        borderColor: entry.stroke
          ? css(entry.stroke)
          : entry.fill
            ? css(entry.fill)
            : "transparent",
      }}
    />
  );
}

const LEGEND_KEY = "openfpga-view-legend";
/** The legend is open or folded in every view, and stays so in the browser */
const legendOpen = signal(
  (() => {
    try {
      return localStorage.getItem(LEGEND_KEY) !== "folded";
    } catch {
      return true;
    }
  })(),
);
function setLegendOpen(open: boolean) {
  legendOpen.value = open;
  try {
    localStorage.setItem(LEGEND_KEY, open ? "open" : "folded");
  } catch {
    // no storage: for this page only
  }
}

/** The legend of a view, in its top left corner: it can be folded */
function Legend(props: { entries: LegendEntry[] }) {
  const open = legendOpen.value;
  const setOpen = setLegendOpen;
  if (!props.entries.length) return null;
  const stop = (e: Event) => e.stopPropagation();
  return (
    <div
      class="view-legend"
      aria-label="Legend"
      onPointerDown={stop}
      onPointerUp={stop}
      onClick={stop}
      onDblClick={stop}
      onWheel={stop}
    >
      <button
        class="legend-fold"
        aria-expanded={open}
        onClick={() => setOpen(!open)}
      >
        {open ? "▾" : "▸"} Legend
      </button>
      {open && (
        <ul>
          {props.entries.map((e, i) =>
            !e.fill && !e.stroke && !e.ramp ? (
              <li key={i} class="legend-heading">
                {e.label}
              </li>
            ) : (
              <li key={i}>
                {!e.ramp && <Swatch entry={e} />}
                <span>{e.label}</span>
                {e.ramp && <Swatch entry={e} />}
              </li>
            ),
          )}
        </ul>
      )}
    </div>
  );
}

/** The last request to show an object that each view did, by its name */
const zoomsDone = new Map<string, number>();

/** The state of the session that the data of the views is of */
export function sessionSignature(): string {
  const v = sessionView.value;
  return `${v.session?.id ?? ""}|${v.stages.map((s) => `${s.stage}:${s.status}:${s.seconds ?? ""}`).join(",")}`;
}

export function ViewPanel<B>(props: {
  name: string;
  title: string;
  def: ViewDef<B>;
}) {
  const { def, name } = props;
  const host = useRef<HTMLDivElement>(null);
  const [base, setBase] = useState<B | null>(null);
  const [failure, setFailure] = useState<string | null>(null);
  const [status, setStatus] = useState("");
  const [legend, setLegend] = useState<LegendEntry[]>([]);
  const [backend, setBackend] = useState<{
    kind: BackendKind;
    reason: string;
  } | null>(null);
  const [hidden, setHidden] = useState<Set<string>>(
    () => new Set(def.groups.filter((g) => g.off).map((g) => g.id)),
  );
  const [tip, setTip] = useState<{ x: number; y: number; text: string } | null>(
    null,
  );
  /** The list beside the drawing: shown, and drawn again at each build */
  const [sideShown, setSideShown] = useState(true);
  const [version, setVersion] = useState(0);
  const real = config.value?.backend === "session";
  const signature = sessionSignature();
  const choice = backendChoice.value;
  // What does not draw the page again: the renderer, the view state, the scene
  const state = useRef({
    renderer: null as Renderer | null,
    view: { cx: 0, cy: 0, scale: 1, width: 1, height: 1 } as ViewState,
    scene: null as Scene | null,
    index: null as SceneIndex | null,
    key: "",
    frame: 0,
    tileVersion: 0,
    fitted: false,
    base: null as B | null,
    hidden: new Set<string>(),
    tiles: null as TileCache | null,
    legend: "",
    /** A request to show an object that waits for its data */
    pendingZoom: null as { ref: ObjectRef; seq: number } | null,
    /** The object at a point of the drawing */
    pickAt: null as ((px: number, py: number) => ObjectRef | null) | null,
  }).current;
  state.base = base;
  state.hidden = hidden;

  const draw = () => {
    if (state.frame) return;
    state.frame = requestAnimationFrame(() => {
      state.frame = 0;
      const b = state.base;
      if (!b || !state.renderer || !state.tiles) return;
      const sel = selection.value;
      const key = `${def.key(b, state.view, state.hidden)}|${sel.map(refKey).join(",")}|${highlighted.value.map(refKey).join(",")}|${state.tileVersion}|${viewColors().dark}`;
      if (key !== state.key) {
        state.key = key;
        const built = def.build({
          base: b,
          tiles: state.tiles,
          view: state.view,
          selection: sel,
          highlighted: highlighted.value,
          colors: viewColors(),
          hidden: state.hidden,
          changed: changed,
        });
        state.scene = built.scene;
        state.index = new SceneIndex(built.scene);
        state.renderer.setScene(built.scene, state.index);
        // A new layout (a netlist view): the view fits it
        if (built.refit) state.view = fit(state.view, built.scene.bounds);
        setStatus(built.status);
        if (def.side) setVersion((v) => v + 1);
        const entries = built.legend ?? [];
        const text = JSON.stringify(entries);
        if (text !== state.legend) {
          state.legend = text;
          setLegend(entries);
        }
      }
      state.renderer.render(state.view);
    });
  };
  // A request to show an object, once its data is there (a path needs the timing)
  const tryZoom = () => {
    const request = state.pendingZoom;
    if (!request || !state.base) return;
    const box = def.locate(state.base, request.ref, changed);
    if (!box) return;
    state.pendingZoom = null;
    zoomsDone.set(name, request.seq);
    state.view = zoomTo(state.view, box);
  };
  const changed = () => {
    state.tileVersion++;
    tryZoom();
    draw();
  };
  const setView = (next: ViewState) => {
    state.view = next;
    draw();
  };

  // The data: again when the session changes
  useEffect(() => {
    if (!real) return;
    setDataSignature(signature);
    state.tiles?.clear();
    state.tiles = new TileCache(changed);
    state.key = "";
    let gone = false;
    setFailure(null);
    def.load().then(
      (loaded) => {
        if (gone) return;
        state.fitted = false;
        setBase(loaded);
      },
      (error) => !gone && setFailure(String(error?.message ?? error)),
    );
    return () => {
      gone = true;
    };
  }, [signature, real]);

  // The renderer: the one chosen, deck.gl unless auto finds it missing, software or slow
  useEffect(() => {
    const element = host.current;
    if (!element || !base) return;
    let chosen = choice === "auto" && probed ? probed : chooseBackend(choice);
    let renderer: Renderer | null = null;
    let gone = false;
    let probing = 0;
    const start = (r: Renderer, why: { kind: BackendKind; reason: string }) => {
      renderer = r;
      state.renderer = r;
      r.onLost = () => {
        // A lost context: Canvas2D at once
        r.destroy();
        start(new Canvas2dRenderer(), {
          kind: "canvas2d",
          reason: "the WebGL context was lost",
        });
      };
      r.mount(element);
      state.key = "";
      setBackend(why);
      draw();
    };
    // The probe of auto: a zoom of PROBE_FRAMES frames, measured
    const probe = (measured: Renderer) => {
      const frames: number[] = [];
      const saved = state.view;
      let last = performance.now();
      const step = (i: number) => {
        probing = requestAnimationFrame(() => {
          const now = performance.now();
          if (i > 0) frames.push(now - last);
          last = now;
          if (i >= PROBE_FRAMES || state.renderer !== measured) {
            state.view = saved;
            if (state.renderer !== measured) return;
            if (frames.length && p95(frames) > PROBE_LIMIT_MS) {
              measured.destroy();
              chosen = {
                kind: "canvas2d",
                reason: `deck.gl was slow here (${Math.round(p95(frames))} ms a frame)`,
              };
              start(new Canvas2dRenderer(), chosen);
            }
            probed = chosen;
            state.key = "";
            draw();
            return;
          }
          // A change of the view too small to see: each frame is drawn again, and the person
          // sees no zoom
          state.view = zoomAt(
            saved,
            saved.width / 2,
            saved.height / 2,
            i % 2 ? 1.0001 : 1,
          );
          state.renderer?.render(state.view);
          step(i + 1);
        });
      };
      step(0);
    };
    if (chosen.kind === "deck") {
      // deck.gl is a chunk of its own: loaded by the first view that draws with it
      import("../views/deck").then(
        ({ DeckRenderer }) => {
          if (gone) return;
          const deck = new DeckRenderer();
          start(deck, chosen);
          if (choice === "auto" && !probed) probe(deck);
        },
        () => {
          if (gone) return;
          chosen = { kind: "canvas2d", reason: "deck.gl could not be loaded" };
          start(new Canvas2dRenderer(), chosen);
        },
      );
    } else start(new Canvas2dRenderer(), chosen);
    return () => {
      gone = true;
      cancelAnimationFrame(probing);
      (renderer as Renderer | null)?.destroy();
      state.renderer = null;
    };
  }, [base, choice]);

  // The size, the input, the probe for the tests
  useEffect(() => {
    const element = host.current;
    if (!element || !base) return;
    const size = () => {
      const box = element.getBoundingClientRect();
      state.view = resized(
        state.view,
        Math.max(1, box.width),
        Math.max(1, box.height),
      );
      if (!state.fitted && box.width > 1) {
        state.view = fit(state.view, def.bounds(base));
        state.fitted = true;
      }
      draw();
    };
    size();
    const observer = new ResizeObserver(size);
    observer.observe(element);
    const pickAt = (px: number, py: number) => {
      if (!state.scene || !state.index) return null;
      const [x, y] = toWorld(state.view, px, py);
      const drawn = state.scene.layers.filter((l) =>
        layerVisible(l, state.view.scale),
      );
      const found = state.index.pick(drawn, x, y, 5 / state.view.scale);
      if (!found || !found.layer.objectKind || !found.layer.name) return null;
      return {
        kind: found.layer.objectKind,
        name: found.layer.name(found.item),
      } as ObjectRef;
    };
    state.pickAt = pickAt;
    let hoverFrame = 0;
    const detach = attachController(element, {
      view: () => state.view,
      setView,
      pick: (px, py, add) => {
        const ref = pickAt(px, py);
        if (ref)
          runViewCommand({ op: "select", kind: ref.kind, name: ref.name, add });
        else if (!add) runViewCommand({ op: "clear" });
      },
      hover: (px, py) => {
        cancelAnimationFrame(hoverFrame);
        hoverFrame = requestAnimationFrame(() => {
          const ref = pickAt(px, py);
          setTip(
            ref && state.base
              ? { x: px, y: py, text: def.describe(state.base, ref) }
              : null,
          );
        });
      },
      leave: () => setTip(null),
      fit: () => setView(fit(state.view, def.bounds(base))),
      clear: () => runViewCommand({ op: "clear" }),
      activate: (px, py) => {
        const ref = pickAt(px, py);
        if (!ref || !def.activate || !state.base) return false;
        return def.activate(state.base, ref, changed);
      },
    });
    window.openfpgaViews = window.openfpgaViews ?? {};
    window.openfpgaViews[name] = {
      view: () => state.view,
      scene: () => state.scene,
      backend: () => state.renderer?.kind ?? null,
      drawn: (id) => {
        const layer: Layer | undefined = state.scene?.layers.find(
          (l) => l.id === id,
        );
        if (!layer || !layerVisible(layer, state.view.scale)) return 0;
        return layer.kind === "grid" ? 1 : itemCount(layer);
      },
      screenOf: (ref) => {
        const box = state.base ? def.locate(state.base, ref, changed) : null;
        if (!box) return null;
        const v = state.view;
        return [
          v.width / 2 + ((box[0] + box[2]) / 2 - v.cx) * v.scale,
          v.height / 2 - ((box[1] + box[3]) / 2 - v.cy) * v.scale,
        ];
      },
    };
    return () => {
      observer.disconnect();
      detach();
      state.pickAt = null;
      cancelAnimationFrame(hoverFrame);
      if (window.openfpgaViews) delete window.openfpgaViews[name];
    };
  }, [base]);

  // The selection, the theme, the layers: draw again
  useEffect(() => draw(), [selection.value, highlighted.value, hidden, base]);

  // A request to show an object: zoom to it if this view has it
  const request = zoomRequest.value;
  useEffect(() => {
    // Each request once in a view: not again when its tab is shown again
    if (!request || !base || (zoomsDone.get(name) ?? 0) >= request.seq) return;
    state.pendingZoom = request;
    tryZoom();
    draw();
  }, [request?.seq, base]);

  const toggle = (group: string) => {
    const next = new Set(hidden);
    if (next.has(group)) next.delete(group);
    else next.add(group);
    setHidden(next);
  };
  const chosen = selection.value;
  // A right click on an object: the commands of the view for it, else the menu of the page
  const onMenu = (e: MouseEvent) => {
    const element = host.current;
    if (!element || !state.base || !def.menu || !state.pickAt) return;
    const box = element.getBoundingClientRect();
    const ref = state.pickAt(e.clientX - box.left, e.clientY - box.top);
    if (ref) openMenu(e, def.menu(state.base, ref, changed));
  };
  const Side = def.side?.component;
  return (
    <section class="panel view-panel" aria-label={props.title} data-view={name}>
      <div class="view-toolbar">
        <strong>{props.title}</strong>
        <button
          disabled={!base}
          onClick={() => base && setView(fit(state.view, def.bounds(base)))}
          title="The whole device (double click, or f)"
        >
          Fit
        </button>
        {base &&
          def.tools?.(base, changed).map((t) => (
            <button key={t.label} title={t.title} onClick={t.run}>
              {t.label}
            </button>
          ))}
        <div class="view-groups" role="group" aria-label="Layers">
          {def.side && (
            <label>
              <input
                type="checkbox"
                checked={sideShown}
                onChange={() => setSideShown(!sideShown)}
              />{" "}
              {def.side.label}
            </label>
          )}
          {def.groups.map((g) => (
            <label key={g.id}>
              <input
                type="checkbox"
                checked={!hidden.has(g.id)}
                onChange={() => toggle(g.id)}
              />{" "}
              {g.label}
            </label>
          ))}
        </div>
        <span class="spacer" />
        <label
          class="view-backend"
          title={
            backend
              ? `Drawn with ${backend.kind === "deck" ? "deck.gl (WebGL2)" : "Canvas2D"}: ${backend.reason}`
              : ""
          }
        >
          Renderer{" "}
          <select
            aria-label="Renderer"
            value={choice}
            onChange={(e) =>
              setBackendChoice(
                (e.target as HTMLSelectElement).value as BackendChoice,
              )
            }
          >
            <option value="auto">Auto</option>
            <option value="webgl">WebGL (deck.gl)</option>
            <option value="canvas2d">Canvas2D</option>
          </select>
          {backend && (
            <span class="muted view-backend-now">
              {" "}
              {backend.kind === "deck" ? "deck.gl" : "Canvas2D"}
            </span>
          )}
        </label>
      </div>
      {!real ? (
        <div class="muted view-message">Not in the demo.</div>
      ) : failure ? (
        <div class="view-message" role="alert">
          {failure}
          <div class="muted">
            This view needs the stage {def.needs}: run it (▶ of {def.needs} in
            the sidebar).
          </div>
        </div>
      ) : !base ? (
        <div class="muted view-message">Reading the data of the view…</div>
      ) : null}
      <div class="view-body" hidden={!base || !!failure}>
        <div
          class="view-host"
          ref={host}
          tabIndex={0}
          aria-label={`${props.title}: drag to move, wheel to zoom, click to select`}
          onContextMenu={onMenu}
        >
          <Legend entries={legend} />
          {tip && (
            <div
              class="view-tip"
              style={{ left: `${tip.x + 12}px`, top: `${tip.y + 12}px` }}
            >
              {tip.text}
            </div>
          )}
        </div>
        {Side && base && sideShown && (
          <aside
            class="view-side"
            aria-label={`${props.title}: ${def.side!.label}`}
          >
            <Side base={base} changed={changed} version={version} />
          </aside>
        )}
      </div>
      <div class="view-status muted">
        <span>{status}</span>
        {chosen.length > 0 && base && (
          <span class="view-selected">
            {chosen.slice(0, 3).map((ref) => (
              <button
                key={refKey(ref)}
                class="link"
                title="Zoom to it"
                onClick={() =>
                  runViewCommand({
                    op: "zoom_to",
                    kind: ref.kind,
                    name: ref.name,
                  })
                }
              >
                {def.describe(base, ref)}
              </button>
            ))}
            {chosen.length > 3 && <span> and {chosen.length - 3} more</span>}
          </span>
        )}
      </div>
    </section>
  );
}
