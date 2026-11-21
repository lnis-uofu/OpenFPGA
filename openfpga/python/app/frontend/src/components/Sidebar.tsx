import { useRef, useState } from "preact/hooks";
import { UPLOAD_EXTENSIONS, type SessionView } from "../api";
import { PackagePicker } from "./PackagePicker";
import { SessionPicker } from "./SessionPicker";
import {
  busy,
  config,
  endSession,
  makeSession,
  openFile,
  packagesOpen,
  removeUpload,
  restore,
  sendMessage,
  sessionsOpen,
  sidebarOpen,
  togglePackages,
  toggleSessions,
  uploadFiles,
  uploading,
  view,
} from "../store";

const SYMBOL = { reached: "✔", not_reached: "○", failed: "✖", running: "░", skipped: "–" } as const;

function Section(props: { title: string; children: preact.ComponentChildren }) {
  return (
    <section class="side-section">
      <h2>{props.title}</h2>
      {props.children}
    </section>
  );
}

function Name(props: { name: string }) {
  // A long name is cut in the middle: the whole name is in the title. A click opens the file in a
  // tab (in the recorded demo there is no file: it copies the name); the small button copies it
  const { name } = props;
  const [copied, setCopied] = useState(false);
  const real = config.value?.backend === "session";
  const shown = name.length > 26 ? `${name.slice(0, 14)}…${name.slice(-10)}` : name;
  const copy = () => {
    void navigator.clipboard?.writeText(name);
    setCopied(true);
    setTimeout(() => setCopied(false), 1200);
  };
  return (
    <span class="file-name">
      <button class="file" title={real ? `Open ${name}` : name} onClick={() => (real ? void openFile(name) : copy())}>
        {shown}
      </button>
      {real && (
        <button class="copy-name" aria-label={`Copy ${name}`} title="Copy the name" onClick={copy}>
          ⧉
        </button>
      )}
      {copied && <span class="copied"> ✓ copied</span>}
    </span>
  );
}

const KIND = { vpr_arch: "VPR arch", openfpga_arch: "OpenFPGA arch", circuit: "circuit", other: "file" } as const;

function size(bytes: number): string {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(bytes < 10 * 1024 ? 1 : 0)} kB`;
  return `${(bytes / 1024 / 1024).toFixed(1)} MB`;
}

const ORIGIN = { input: "design folder", upload: "uploaded", model: "by the model", synthesis: "synthesis" } as const;

interface DesignRow {
  name: string;
  kind: keyof typeof KIND;
  bytes?: number;
  origin: keyof typeof ORIGIN;
  loaded: boolean;
}

/** Every design file in one list: what the person uploaded, what the model wrote, what a synthesis
 * made, and the circuit that was read from a designs folder. The architectures are not in it (the
 * button of the architecture opens them). The circuit that the session has read is marked */
function designFiles(files: SessionView["files"]): DesignRow[] {
  const circuit = files.inputs.find((f) => f.label === "circuit");
  const is = (name: string) => !!circuit && (circuit.name === name || circuit.name.endsWith("/" + name));
  const rows: DesignRow[] = [];
  for (const [list, origin] of [
    [files.uploads ?? [], "upload"],
    [files.created ?? [], "model"],
    [files.synthesis ?? [], "synthesis"],
  ] as const) {
    for (const f of list) rows.push({ name: f.name, kind: f.kind, bytes: f.bytes, origin, loaded: is(f.name) });
  }
  if (circuit && !rows.some((r) => r.loaded)) {
    rows.unshift({ name: circuit.name, kind: "circuit", origin: "input", loaded: true });
  }
  return rows;
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
  const designs = v.designs ?? [];
  // Synthesis follows the device (its LUT size and script come from the device); a skipped one
  // (the design is a netlist) is not counted
  const device = v.stages.findIndex((s) => s.stage === "DEVICE");
  const stages = [...v.stages];
  if (v.synthesis) stages.splice(device + 1, 0, { ...v.synthesis, stage: "SYNTHESIS" });
  const counted = stages.filter((s) => s.status !== "skipped");
  const reached = counted.filter((s) => s.status === "reached").length;
  const rows = designFiles(files);
  const vpr = files.inputs.find((f) => f.label === "vpr arch");
  const oarch = files.inputs.find((f) => f.label === "openfpga arch");
  return (
    <aside class={`sidebar ${sidebarOpen.value ? "" : "closed"}`} aria-label="Session and design files">
      <Section title="Session">
        {session ? (
          <>
            <div class="session-row">
              <span class={`status-dot ${session.worker === "running" ? "on" : "off"}`} aria-hidden="true" />
              <strong>{session.id}</strong> <span class="muted">{session.name !== session.id ? session.name : ""}</span>
            </div>
            {session.device_package && (
              <div class="muted device-line" title={session.device_package.description}>
                device {session.device_package.name} · {session.device_package.layout}
                {session.device_package.lut_size != null ? ` · K${session.device_package.lut_size}` : ""}
              </div>
            )}
            <div class={`session-state ${session.worker === "running" ? "muted" : "warn"}`}>
              {session.worker === "running" ? "worker running" : session.worker === "none" ? "closed" : `⚠ worker ended${session.ended ? `: ${session.ended}` : ""}`}
            </div>
            <div class="buttons">
              <button disabled={!real || busy.value} onClick={() => void makeSession()} title={real ? "Open another session" : "Not in the demo"}>
                + new
              </button>
              <button
                disabled={!real || busy.value}
                aria-expanded={packagesOpen.value}
                onClick={() => void togglePackages()}
                title={real ? "Create a session from a device package: its architectures are read" : "Not in the demo"}
              >
                from device…
              </button>
              <button
                disabled={!real || busy.value}
                aria-expanded={sessionsOpen.value}
                onClick={() => void toggleSessions()}
                title={real ? "Switch to another session, or open a saved one" : "Not in the demo"}
              >
                sessions…
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
              <button
                disabled={!real || busy.value}
                aria-expanded={packagesOpen.value}
                onClick={() => void togglePackages()}
                title={real ? "Create a session from a device package: its architectures are read" : "Not in the demo"}
              >
                from device…
              </button>
              <button
                disabled={!real || busy.value}
                aria-expanded={sessionsOpen.value}
                onClick={() => void toggleSessions()}
                title={real ? "Switch to another session, or open a saved one" : "Not in the demo"}
              >
                sessions…
              </button>
            </div>
          </>
        )}
        {real && packagesOpen.value && <PackagePicker />}
        {real && sessionsOpen.value && <SessionPicker />}
      </Section>
      <Section title="Stages">
        <div
          class="progress"
          role="progressbar"
          aria-label="Stages reached"
          aria-valuemin={0}
          aria-valuemax={counted.length}
          aria-valuenow={reached}
        >
          <div class="progress-fill" style={{ width: `${(reached / Math.max(1, counted.length)) * 100}%` }} />
        </div>
        <ol class="stages">
          {stages.map((s) => (
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
      <Section title="Designs">
        {designs.length ? (
          <div class="designs">
            {designs.map((d) => (
              <div key={d.path} class="design-row" title={d.description ?? undefined}>
                <strong class="design-name">{d.name}</strong>
                <button
                  class="design-synthesize"
                  disabled={!real || busy.value || !session}
                  title={
                    !real
                      ? "Not in the demo"
                      : session
                        ? "Synthesize this design for the device of the session"
                        : "Open a session first"
                  }
                  onClick={() => void sendMessage(`/synthesize design=${d.name}`)}
                >
                  Synthesize
                </button>
                <span class="design-detail muted">
                  {d.top ? `top ${d.top} · ` : ""}
                  {d.files.length} {d.files.length === 1 ? "file" : "files"}
                </span>
              </div>
            ))}
          </div>
        ) : (
          <div class="muted">(no design directory: a folder with a design.json in a designs folder)</div>
        )}
      </Section>
      <Section title="Design files">
        <div class="architecture-row">
          <span class="file-label">Architecture</span>
          {vpr || oarch ? (
            <>
              {session?.device_package && <span class="chip">{session.device_package.name}</span>}
              {vpr?.extra && <span class="chip">{vpr.extra}</span>}
              <button
                class="show-architecture"
                disabled={!real}
                title={real ? "Open the VPR and the OpenFPGA architecture files" : "Not in the demo"}
                onClick={() => {
                  if (vpr) void openFile(vpr.name);
                  if (oarch) void openFile(oarch.name);
                }}
              >
                Show architecture
              </button>
            </>
          ) : (
            <span class="muted">(not read)</span>
          )}
        </div>
        {rows.length ? (
          <div class="uploads design-files">
            {rows.map((f) => (
              <div key={f.name} class={`upload-row ${f.loaded ? "loaded" : ""}`}>
                <Name name={f.name} />
                <span class="chip">{KIND[f.kind]}</span>
                {f.loaded && <span class="chip chip-loaded">loaded</span>}
                <span class="chip chip-origin">{ORIGIN[f.origin]}</span>
                {f.bytes != null && <span class="muted upload-size">{size(f.bytes)}</span>}
                {f.origin === "upload" && (
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
                )}
              </div>
            ))}
          </div>
        ) : (
          <div class="muted">(none)</div>
        )}
        {session?.device_package && !files.inputs.some((f) => f.label === "circuit") && (
          <div class="waiting">Waiting for a circuit: upload one below, then say which to read.</div>
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
      </Section>
      <Section title="Constraints and results">
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
