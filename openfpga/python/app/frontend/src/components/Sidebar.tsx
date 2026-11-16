import { useRef, useState } from "preact/hooks";
import { UPLOAD_EXTENSIONS } from "../api";
import { busy, config, endSession, makeSession, removeUpload, restore, sidebarOpen, uploadFiles, uploading, view } from "../store";

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
  const [copied, setCopied] = useState(false);
  const shown = name.length > 26 ? `${name.slice(0, 14)}…${name.slice(-10)}` : name;
  const copy = () => {
    void navigator.clipboard?.writeText(name);
    setCopied(true);
    setTimeout(() => setCopied(false), 1200);
  };
  return (
    <button class="file" title={name} onClick={copy}>
      {shown}
      {copied && <span class="copied"> ✓ copied</span>}
    </button>
  );
}

const KIND = { vpr_arch: "VPR arch", openfpga_arch: "OpenFPGA arch", circuit: "circuit", other: "file" } as const;

function size(bytes: number): string {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(bytes < 10 * 1024 ? 1 : 0)} kB`;
  return `${(bytes / 1024 / 1024).toFixed(1)} MB`;
}

/** The left sidebar: the session, its stages, the design files and the checkpoints */
export function Sidebar() {
  const [optionsOpen, setOptionsOpen] = useState(false);
  const picker = useRef<HTMLInputElement>(null);
  const v = view.value;
  const session = v.session;
  // The buttons of the sessions work on real sessions, not on the recorded demo
  const real = config.value?.backend === "session";
  const files = v.files;
  const reached = v.stages.filter((s) => s.status === "reached").length;
  return (
    <aside class={`sidebar ${sidebarOpen.value ? "" : "closed"}`} aria-label="Session and design files">
      <Section title="Session">
        {session ? (
          <>
            <div class="session-row">
              <span class={`status-dot ${session.worker === "running" ? "on" : "off"}`} aria-hidden="true" />
              <strong>{session.id}</strong> <span class="muted">{session.name !== session.id ? session.name : ""}</span>
            </div>
            <div class={`session-state ${session.worker === "running" ? "muted" : "warn"}`}>
              {session.worker === "running" ? "worker running" : session.worker === "none" ? "closed" : `⚠ worker ended${session.ended ? `: ${session.ended}` : ""}`}
            </div>
            <div class="buttons">
              <button disabled={!real || busy.value} onClick={() => void makeSession()} title={real ? "Open another session" : "Not in the demo"}>
                + new
              </button>
              <button disabled={!real || busy.value} onClick={() => void endSession()} title={real ? "End the worker; the files stay" : "Not in the demo"}>
                close
              </button>
              {session.worker === "ended" && (
                <button disabled={!real} onClick={() => void restore()}>
                  Restore
                </button>
              )}
            </div>
          </>
        ) : (
          <>
            <div class="muted">(no session)</div>
            <div class="buttons">
              <button disabled={!real} onClick={() => void makeSession()} title={real ? "Open a session" : "Not in the demo"}>
                + new
              </button>
            </div>
          </>
        )}
      </Section>
      <Section title="Stages">
        <div
          class="progress"
          role="progressbar"
          aria-label="Stages reached"
          aria-valuemin={0}
          aria-valuemax={v.stages.length}
          aria-valuenow={reached}
        >
          <div class="progress-fill" style={{ width: `${(reached / Math.max(1, v.stages.length)) * 100}%` }} />
        </div>
        <ol class="stages">
          {v.stages.map((s) => (
            <li key={s.stage} class={`stage stage-${s.status}`} title={s.message ?? undefined}>
              <span class="stage-mark" aria-hidden="true">
                {SYMBOL[s.status]}
              </span>
              <span class="sr-only">{s.status.replace("_", " ")}</span>
              <span class="stage-name">{s.stage}</span>
              {s.seconds != null && <span class="stage-time muted">{s.seconds.toFixed(1)} s</span>}
              {s.status === "failed" && s.message && <div class="stage-message">{s.message}</div>}
            </li>
          ))}
        </ol>
      </Section>
      <Section title="Design files">
        <h3>Inputs</h3>
        {files.inputs.length ? (
          files.inputs.map((f) => (
            <div key={f.label} class="file-row">
              <span class="file-label">{f.label}</span>
              <Name name={f.name} />
              {f.extra && <span class="chip">{f.extra}</span>}
            </div>
          ))
        ) : (
          <div class="muted">(none)</div>
        )}
        <h3>Uploaded</h3>
        {(files.uploads ?? []).length > 0 && (
          <div class="uploads">
            {(files.uploads ?? []).map((f) => (
              <div key={f.name} class="upload-row">
                <Name name={f.name} />
                <span class="chip">{KIND[f.kind]}</span>
                <span class="muted upload-size">{size(f.bytes)}</span>
                <button
                  class="upload-remove"
                  aria-label={`Remove ${f.name}`}
                  title="Remove this file from the session"
                  onClick={() => {
                    if (window.confirm(`Remove "${f.name}" from the session?`)) void removeUpload(f.name);
                  }}
                >
                  ✕
                </button>
              </div>
            ))}
          </div>
        )}
        <input
          ref={picker}
          type="file"
          multiple
          hidden
          aria-label="Upload files"
          accept={UPLOAD_EXTENSIONS.join(",")}
          onChange={(e) => {
            const input = e.target as HTMLInputElement;
            void uploadFiles(Array.from(input.files ?? []));
            input.value = ""; // the same file can be chosen again
          }}
        />
        <button
          class="upload-button"
          disabled={!real || uploading.value}
          title={real ? "Copy files into this session (architectures, circuits, constraints)" : "Not in the demo"}
          onClick={() => picker.current?.click()}
        >
          {uploading.value ? "Uploading…" : "⤒ Upload files…"}
        </button>
        <h3>Constraints</h3>
        {files.constraints.length ? (
          <div class="chips">
            {files.constraints.map((c) => (
              <span class="chip" key={c}>
                {c}
              </span>
            ))}
          </div>
        ) : (
          <div class="muted">(none)</div>
        )}
        <h3>Results</h3>
        {files.results.length ? (
          <div class="results">
            {files.results.map((r) => (
              <Name key={r} name={r} />
            ))}
          </div>
        ) : (
          <div class="muted">(none)</div>
        )}
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
            <div key={c.name} class="checkpoint">
              <span class="checkpoint-name">
                {c.name} <span class="muted">· {c.stages[c.stages.length - 1] ?? ""}</span>
              </span>
              {real && (
                <button class="link" title="Go back to this checkpoint" onClick={() => void restore(c.name === "last" ? undefined : c.name)}>
                  restore
                </button>
              )}
            </div>
          ))
        ) : (
          <div class="muted">(none)</div>
        )}
      </Section>
    </aside>
  );
}
