import { useState } from "preact/hooks";
import { Markdown } from "../markdown";
import { resultMarkdown } from "../resultMarkdown";
import { CHAT, activeTab, calls, closeTab, pinTab, split, tabs } from "../store";
import { Chat } from "./Chat";
import { FileView } from "./FileView";

/** What a result tab shows. The drawings of each kind are not designed yet: the result as
 * formatted text (Markdown), and the JSON as the model read it on request (work in progress) */
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
      <p class="wip">Work in progress: the drawings of the results are not designed yet. This is the result as text.</p>
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

function TabContent(props: { tab: { id: string; file?: string } }) {
  return props.tab.file ? <FileView name={props.tab.file} /> : <ResultView callId={props.tab.id} />;
}

function TabButton(props: { id: string; title: string; closable: boolean; pinned?: boolean; unread?: boolean }) {
  const selected = activeTab.value === props.id;
  return (
    <span class={`tab ${selected ? "selected" : ""}`} role="presentation">
      <button role="tab" aria-selected={selected} onClick={() => (activeTab.value = props.id)}>
        {props.pinned && <span aria-hidden="true">📌 </span>}
        {props.title}
        {props.unread && <span class="dot-unread" aria-label="new result"> ●</span>}
      </button>
      {props.closable && (
        <button class="close" aria-label={`Close ${props.title}`} onClick={() => closeTab(props.id)}>
          ×
        </button>
      )}
    </span>
  );
}

/** The centre: the chat alone until there is a result; then a bar of tabs, with Chat the first,
 * or the chat and a result side by side */
export function Workspace() {
  const results = tabs.value;
  if (results.length === 0) return <main class="centre"><Chat /></main>;
  const shown = results.find((t) => t.id === activeTab.value) ?? null;
  const resultTabs = results.map((t) => (
    <TabButton key={t.id} id={t.id} title={t.title} closable pinned={t.pinned} unread={t.unread} />
  ));
  if (split.value) {
    const current = shown ?? results[0];
    return (
      <main class="centre split">
        <div class="pane">
          <div class="tabbar" role="tablist">
            <span class="tab selected"><button role="tab" aria-selected>Chat</button></span>
          </div>
          <Chat />
        </div>
        <div class="pane">
          <div class="tabbar" role="tablist">
            {resultTabs}
            <span class="spacer" />
            <button class="split-toggle" onClick={() => (split.value = false)}>⫴ one view</button>
          </div>
          <TabContent tab={current} />
        </div>
      </main>
    );
  }
  return (
    <main class="centre">
      <div class="tabbar" role="tablist">
        <TabButton id={CHAT} title="Chat" closable={false} />
        {resultTabs}
        <span class="spacer" />
        <button class="split-toggle" onClick={() => (split.value = true)}>⫴ split</button>
      </div>
      {shown ? <TabContent tab={shown} /> : <Chat />}
    </main>
  );
}
