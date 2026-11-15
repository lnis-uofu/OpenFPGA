import { useState } from "preact/hooks";
import { sidebarOpen, view } from "../store";

const SYMBOL = { reached: "✔", not_reached: "○", failed: "✖", running: "░" } as const;

function Section(props: { title: string; children: preact.ComponentChildren }) {
  return (
    <section class="side-section">
      <h2>{props.title}</h2>
      {props.children}
    </section>
  );
}

function Name(props: { name: string }) {
  // A long name is cut in the middle: the whole name is in the title, and a click copies it
  const { name } = props;
  const shown = name.length > 26 ? `${name.slice(0, 14)}…${name.slice(-10)}` : name;
  return (
    <button class="file" title={name} onClick={() => navigator.clipboard?.writeText(name)}>
      {shown}
    </button>
  );
}

/** The left sidebar: the session, its stages, the design files and the checkpoints */
export function Sidebar() {
  const [optionsOpen, setOptionsOpen] = useState(false);
  const v = view.value;
  const session = v.session;
  const files = v.files;
  return (
    <aside class={`sidebar ${sidebarOpen.value ? "" : "closed"}`} aria-label="Session and design files">
      <Section title="Session">
        {session ? (
          <>
            <div class="session-row">
              <strong>{session.id}</strong> <span class="muted">{session.name !== session.id ? session.name : ""}</span>
            </div>
            <div class={session.worker === "running" ? "muted" : "warn"}>
              {session.worker === "running" ? "worker running" : session.worker === "none" ? "closed" : `⚠ worker ended${session.ended ? `: ${session.ended}` : ""}`}
            </div>
            <div class="buttons">
              <button disabled title="Not connected yet">+ new</button>
              <button disabled title="Not connected yet">close</button>
            </div>
          </>
        ) : (
          <div class="muted">(no session)</div>
        )}
      </Section>
      <Section title="Stages">
        <ol class="stages">
          {v.stages.map((s) => (
            <li key={s.stage} class={`stage stage-${s.status}`} title={s.message ?? undefined}>
              <span aria-hidden="true">{SYMBOL[s.status]}</span> <span class="sr-only">{s.status.replace("_", " ")}</span>
              {s.stage}
              {s.seconds != null && <span class="muted"> {s.seconds.toFixed(1)} s</span>}
            </li>
          ))}
        </ol>
      </Section>
      <Section title="Design files">
        <h3>Inputs</h3>
        {files.inputs.length ? (
          files.inputs.map((f) => (
            <div key={f.label} class="file-row">
              <span class="muted">{f.label}</span>
              <Name name={f.name} />
              {f.extra && <span class="muted">{f.extra}</span>}
            </div>
          ))
        ) : (
          <div class="muted">(none)</div>
        )}
        <h3>Constraints</h3>
        {files.constraints.length ? files.constraints.map((c) => <div key={c}>{c}</div>) : <div class="muted">(none)</div>}
        <h3>Results</h3>
        {files.results.length ? files.results.map((r) => <Name key={r} name={r} />) : <div class="muted">(none)</div>}
        <button class="fold" aria-expanded={optionsOpen} onClick={() => setOptionsOpen(!optionsOpen)}>
          {optionsOpen ? "▾" : "▸"} Options changed ({v.options_changed.length})
        </button>
        {optionsOpen &&
          (v.options_changed.length ? (
            <ul class="options">
              {v.options_changed.map((o) => (
                <li key={o.name}>
                  {o.name} = {String(o.value)}
                </li>
              ))}
            </ul>
          ) : (
            <div class="muted">(none)</div>
          ))}
      </Section>
      <Section title="Checkpoints">
        {v.checkpoints.length ? (
          v.checkpoints.map((c) => (
            <div key={c.name}>
              {c.name} <span class="muted">· {c.stages[c.stages.length - 1] ?? ""}</span>
            </div>
          ))
        ) : (
          <div class="muted">(none)</div>
        )}
      </Section>
    </aside>
  );
}
