import { useState } from "preact/hooks";
import { Markdown } from "../markdown";
import { resultMarkdown } from "../resultMarkdown";
import {
  CHAT,
  type Side,
  activeTab,
  calls,
  closeTab,
  moveTab,
  pinTab,
  reportUrl,
  shownOnSide,
  sideOf,
  split,
  tabs,
  tabsOfSide,
  view,
} from "../store";
import { Chat } from "./Chat";
import { Drawing, kindOf } from "./Drawings";
import { FileView } from "./FileView";
import { DesignsPanel, DevicesPanel, HierarchyPanel } from "./Panels";
import { TimingPanel } from "./TimingPanel";
import { ClustersPanel, FloorplanPanel, NetlistPanel, PinsPanel, RoutingPanel } from "./Views";

/** What a result tab shows: the drawing for the kinds that have a shape (utilization, placement, congestion), the result as
 * formatted text (Markdown), and the JSON as the model read it on request */
export function ResultView(props: { callId: string }) {
  const [raw, setRaw] = useState(false);
  const call = calls.value[props.callId];
  const tab = tabs.value.find((t) => t.id === props.callId);
  if (!call || !call.result) return <p class="muted">The result is gone.</p>;
  return (
    <section class="result" aria-label={`Result of ${call.tool}`}>
      <div class="result-head">
        <strong>{call.tool}</strong>
        <span class="spacer" />
        <button onClick={() => pinTab(props.callId)} aria-pressed={tab?.pinned ?? false}>
          {tab?.pinned ? "Unpin" : "Pin"}
        </button>
        <button onClick={() => setRaw(!raw)} aria-pressed={raw}>
          {raw ? "Formatted" : "JSON"}
        </button>
        <button onClick={() => navigator.clipboard?.writeText(JSON.stringify(call.result))}>Copy JSON</button>
      </div>
      {kindOf(call.result) ? (
        <p class="muted drawn-note">Drawn from the data that the model read; the table below has the numbers.</p>
      ) : (
        <p class="wip">Work in progress: this kind of result has no drawing yet. This is the result as text.</p>
      )}
      {!raw && <Drawing result={call.result} />}
      {raw ? (
        <pre class="json">{JSON.stringify(call.result, null, 1)}</pre>
      ) : (
        <div class="result-md result-md-tab">
          <Markdown text={resultMarkdown(call.tool, call.result)} />
        </div>
      )}
    </section>
  );
}

/** The HTML report of a stage, in a frame. The frame is sandboxed with nothing allowed: the report
 * runs no script and cannot reach the page. It loads again when the session changes (a new run) */
export function ReportView(props: { stage: string; title: string }) {
  const v = view.value;
  const has = (v.reports ?? []).includes(props.stage);
  const version = `${v.session?.id ?? ""}|${v.latest.text}`;
  const src = reportUrl(props.stage, version);
  return (
    <section class="report-view" aria-label={props.title}>
      <div class="result-head">
        <strong>{props.title}</strong>
        <span class="spacer" />
        {has && (
          <a class="button-link" href={src} target="_blank" rel="noopener" title="Open the report in a tab of the browser">
            Open in browser ↗
          </a>
        )}
      </div>
      {has ? (
        <iframe class="report-frame" title={props.title} src={src} sandbox="" referrerpolicy="no-referrer" />
      ) : (
        <p class="muted">This session has no report of this stage (yet): it is written when the stage runs.</p>
      )}
    </section>
  );
}

function TabContent(props: { tab: { id: string; file?: string; report?: string; panel?: string; title: string } }) {
  if (props.tab.panel === "devices") return <DevicesPanel />;
  if (props.tab.panel === "designs") return <DesignsPanel />;
  if (props.tab.panel === "hierarchy") return <HierarchyPanel />;
  if (props.tab.panel === "floorplan") return <FloorplanPanel />;
  if (props.tab.panel === "pins") return <PinsPanel />;
  if (props.tab.panel === "routing") return <RoutingPanel />;
  if (props.tab.panel === "timing") return <TimingPanel />;
  if (props.tab.panel === "netlist") return <NetlistPanel />;
  if (props.tab.panel === "clusters") return <ClustersPanel />;
  if (props.tab.report) return <ReportView stage={props.tab.report} title={props.tab.title} />;
  return props.tab.file ? <FileView name={props.tab.file} /> : <ResultView callId={props.tab.id} />;
}

/** What a tab carries when it is dragged to the other side of the split */
const TAB_DRAG = "application/x-openfpga-tab";

function TabButton(props: {
  id: string;
  title: string;
  closable: boolean;
  pinned?: boolean;
  unread?: boolean;
  /** In the split: the tab can be dragged to the other side, or moved by its button */
  movable?: boolean;
}) {
  const selected = activeTab.value === props.id;
  const other: Side = sideOf(props.id) === "left" ? "right" : "left";
  return (
    <span
      class={`tab ${selected ? "selected" : ""}`}
      role="presentation"
      draggable={props.movable}
      onDragStart={(e) => {
        e.dataTransfer?.setData(TAB_DRAG, props.id);
        if (e.dataTransfer) e.dataTransfer.effectAllowed = "move";
      }}
    >
      <button role="tab" aria-selected={selected} onClick={() => (activeTab.value = props.id)}>
        {props.pinned && <span aria-hidden="true">📌 </span>}
        {props.title}
        {props.unread && <span class="dot-unread" aria-label="new result"> ●</span>}
      </button>
      {props.movable && selected && (
        <button
          class="move"
          aria-label={`Move ${props.title} to the ${other} side`}
          title={`Move to the ${other} side (or drag the tab)`}
          onClick={() => moveTab(props.id, other)}
        >
          {other === "left" ? "⇠" : "⇢"}
        </button>
      )}
      {props.closable && (
        <button class="close" aria-label={`Close ${props.title}`} onClick={() => closeTab(props.id)}>
          ×
        </button>
      )}
    </span>
  );
}

/** A side of the split: its tabs, and the one it shows. A tab dragged onto it moves to it */
function SidePane(props: { side: Side }) {
  const [over, setOver] = useState(false);
  const ids = tabsOfSide(props.side);
  const shown = shownOnSide(props.side);
  const byId = new Map(tabs.value.map((t) => [t.id, t]));
  const dragged = (e: DragEvent) => !!e.dataTransfer?.types.includes(TAB_DRAG);
  return (
    <div
      class={`pane ${over ? "drop" : ""}`}
      aria-label={`The ${props.side} side`}
      onDragOver={(e) => {
        if (!dragged(e)) return;
        e.preventDefault();
        setOver(true);
      }}
      onDragLeave={(e) => {
        if (!(e.currentTarget as HTMLElement).contains(e.relatedTarget as Node)) setOver(false);
      }}
      onDrop={(e) => {
        setOver(false);
        const id = e.dataTransfer?.getData(TAB_DRAG);
        if (!id) return;
        e.preventDefault();
        moveTab(id, props.side);
      }}
    >
      <div class="tabbar" role="tablist">
        {ids.map((id) => {
          const t = byId.get(id);
          return t ? (
            <TabButton key={id} id={id} title={t.title} closable pinned={t.pinned} unread={t.unread} movable />
          ) : (
            <TabButton key={id} id={CHAT} title="Chat" closable={false} movable />
          );
        })}
        <span class="spacer" />
        {props.side === "right" && (
          <button class="split-toggle" onClick={() => (split.value = false)}>⫴ one view</button>
        )}
      </div>
      {shown === null ? (
        <p class="muted pane-empty">Drag a tab here.</p>
      ) : shown === CHAT ? (
        <Chat />
      ) : (
        byId.get(shown) && <TabContent tab={byId.get(shown)!} />
      )}
    </div>
  );
}

/** The centre: the chat alone until there is a result; then a bar of tabs, with Chat the first,
 * or two sides with their tabs, which move from one to the other */
export function Workspace() {
  const results = tabs.value;
  if (results.length === 0) return <main class="centre"><Chat /></main>;
  if (split.value)
    return (
      <main class="centre split">
        <SidePane side="left" />
        <SidePane side="right" />
      </main>
    );
  const shown = results.find((t) => t.id === activeTab.value) ?? null;
  return (
    <main class="centre">
      <div class="tabbar" role="tablist">
        <TabButton id={CHAT} title="Chat" closable={false} />
        {results.map((t) => (
          <TabButton key={t.id} id={t.id} title={t.title} closable pinned={t.pinned} unread={t.unread} />
        ))}
        <span class="spacer" />
        <button class="split-toggle" onClick={() => (split.value = true)}>⫴ split</button>
      </div>
      {shown ? <TabContent tab={shown} /> : <Chat />}
    </main>
  );
}
