import { useRef, useState } from "preact/hooks";
import { UPLOAD_EXTENSIONS, type SessionView } from "../api";
import { SessionPicker } from "./SessionPicker";
import {
  busy,
  config,
  endSession,
  makeSession,
  openFile,
  openPanel,
  openReport,
  openSession,
  removeUpload,
  restore,
  saveSession,
  sendMessage,
  sessionMenu,
  sessionsOpen,
  setSidebarWidth,
  sidebarOpen,
  sidebarWidth,
  SIDEBAR_WIDTH_DEFAULT,
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

function Name(props: { name: string; label?: string }) {
  // A long name is cut in the middle, as much as the width of the sidebar needs: the whole name is
  // in the title. A click opens the file in a tab (in the recorded demo there is no file: it copies
  // the name); the small button copies it
  const { name } = props;
  const label = props.label ?? name;
  const [copied, setCopied] = useState(false);
  const real = config.value?.backend === "session";
  const room = Math.max(26, Math.floor((sidebarWidth.value - 100) / 7.4));
  const tail = Math.max(10, Math.floor(room * 0.45));
  const shown = label.length > room ? `${label.slice(0, room - tail - 1)}…${label.slice(-tail)}` : label;
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

function size(bytes: number): string {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(bytes < 10 * 1024 ? 1 : 0)} kB`;
  return `${(bytes / 1024 / 1024).toFixed(1)} MB`;
}

type Origin = "input" | "design" | "upload" | "model" | "synthesis";

interface DesignRow {
  name: string;
  label?: string; // a file of the copy of a design: its path in the design, after design/<folder>/
  bytes?: number;
  origin: Origin;
  loaded: boolean;
}

/** Every design file in one list: the copy of a design directory that was synthesized, what the
 * person uploaded, what the model wrote, what a synthesis made, and the circuit that was read from
 * a designs folder. The architectures are not in it (the button of the device stage opens them).
 * The circuit that the session has read is marked */
function designFiles(files: SessionView["files"]): DesignRow[] {
  const circuit = files.inputs.find((f) => f.label === "circuit");
  const is = (name: string) => !!circuit && (circuit.name === name || circuit.name.endsWith("/" + name));
  const rows: DesignRow[] = [];
  for (const [list, origin] of [
    [files.design ?? [], "design"],
    [files.uploads ?? [], "upload"],
    [files.created ?? [], "model"],
    [files.synthesis ?? [], "synthesis"],
  ] as const) {
    for (const f of list) {
      const label = origin === "design" ? f.name.split("/").slice(2).join("/") || f.name : undefined;
      rows.push({ name: f.name, label, bytes: f.bytes, origin, loaded: is(f.name) });
    }
  }
  if (circuit && !rows.some((r) => r.loaded)) {
    rows.unshift({ name: circuit.name, origin: "input", loaded: true });
  }
  return rows;
}

/** The name of the report of a stage (reports/<name>.html of the session) */
const REPORT: Record<string, string> = {
  DEVICE: "device",
  SYNTHESIS: "synthesis",
  PACKED: "pack",
  PLACED: "place",
  ROUTED: "route",
  ANALYZED: "analysis",
};

/** The command of the run button of a stage (none for the stages that have no button) */
const RUN: Record<string, string> = { PACKED: "pack", PLACED: "place", ROUTED: "route" };

/** The open sessions as tabs (a click switches to one), and the panel of the current one: its
 * checkpoints (folded), save and close */
function SessionPanel(props: { real: boolean }) {
  const { real } = props;
  const [checkpointsOpen, setCheckpointsOpen] = useState(false);
  const v = view.value;
  const session = v.session;
  const open = v.sessions ?? (session ? [{ id: session.id, name: session.name, running: session.worker === "running" }] : []);
  const last = v.checkpoints.find((c) => c.name === "last");
  const named = v.checkpoints.filter((c) => c.name !== "last").length;
  const status = last
    ? `checkpoint · ${last.stages[last.stages.length - 1] ?? "no stage"}${named ? ` · +${named} saved` : ""}`
    : named
      ? `${named} saved checkpoint${named === 1 ? "" : "s"}`
      : "no checkpoint";
  return (
    <section class="side-section session-panel" aria-label="Sessions">
      <div class="session-tabs" role="tablist" aria-label="Open sessions">
        {open.length ? (
          open.map((s) => {
            const current = s.id === session?.id;
            return (
              <button
                key={s.id}
                role="tab"
                aria-selected={current}
                class={`session-tab ${current ? "selected" : ""}`}
                disabled={!real || (busy.value && !current)}
                title={`${s.name} (${s.id})${s.running ? "" : " · worker ended"}${current ? "" : ": switch to it"}`}
                onClick={() => {
                  if (!current) void openSession(s.id);
                }}
              >
                <span class={`status-dot ${s.running ? "on" : "off"}`} aria-hidden="true" />
                <span class="session-tab-name">{s.name}</span>
              </button>
            );
          })
        ) : (
          <span class="muted session-none">(no session)</span>
        )}
      </div>
      {session && (
        <>
          <div class="session-bar">
            <button
              class="fold checkpoint-status"
              aria-expanded={checkpointsOpen}
              title="The checkpoints of the session: restore one"
              onClick={() => setCheckpointsOpen(!checkpointsOpen)}
            >
              {checkpointsOpen ? "▾" : "▸"} {status}
            </button>
            <span class="spacer" />
            <button
              class="session-action"
              disabled={!real || busy.value || session.worker !== "running"}
              title={real ? "Save the flow as a checkpoint, to come back to" : "Not in the demo"}
              onClick={() => void saveSession()}
            >
              Save
            </button>
            <button
              class="session-action"
              disabled={!real || busy.value}
              title={real ? "End the worker; the files stay" : "Not in the demo"}
              onClick={() => void endSession()}
            >
              Close
            </button>
            {session.worker === "ended" && (
              <button class="session-action" disabled={!real} onClick={() => void restore()}>
                Restore
              </button>
            )}
          </div>
          {checkpointsOpen && (
            <div class="checkpoints">
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
                <div class="muted">(none: one is saved after each stage, or with Save)</div>
              )}
            </div>
          )}
        </>
      )}
    </section>
  );
}

/** The menu under the sessions: a new session, the question of Session (new or open), and the
 * lists of the devices and the designs, each in a tab */
function Menu(props: { real: boolean }) {
  const { real } = props;
  const item = (icon: string, label: string, detail: string, onClick: () => void, opts: { disabled?: boolean; expanded?: boolean } = {}) => (
    <button
      class="menu-item"
      role="menuitem"
      disabled={opts.disabled}
      aria-expanded={opts.expanded}
      onClick={onClick}
      title={detail}
    >
      <span class="menu-icon" aria-hidden="true">
        {icon}
      </span>
      <span class="menu-label">{label}</span>
      <span class="menu-detail muted">{detail}</span>
    </button>
  );
  return (
    <nav class="side-section side-menu" aria-label="Menu">
      <div role="menu" aria-label="Menu">
        {item("＋", "New", real ? "A new session" : "Not in the demo", () => void makeSession(), { disabled: !real || busy.value })}
        {item(
          "◫",
          "Session",
          real ? "New, or open a session" : "Not in the demo",
          () => {
            sessionMenu.value = !sessionMenu.value;
            if (!sessionMenu.value) sessionsOpen.value = false;
          },
          { disabled: !real, expanded: sessionMenu.value },
        )}
        {real && sessionMenu.value && (
          <div class="menu-choice" role="group" aria-label="Session">
            <button
              disabled={busy.value}
              onClick={() => {
                sessionMenu.value = false;
                sessionsOpen.value = false;
                void makeSession();
              }}
            >
              New
            </button>
            <button disabled={busy.value} aria-expanded={sessionsOpen.value} onClick={() => void toggleSessions()}>
              Open…
            </button>
          </div>
        )}
        {real && sessionMenu.value && sessionsOpen.value && <SessionPicker />}
        {item("▦", "Devices", "The device packages", () => openPanel("devices"))}
        {item("▤", "Designs", "The design directories", () => openPanel("designs"))}
      </div>
    </nav>
  );
}

/** The edge of the sidebar: dragged (or the arrow keys) to make it wider or narrower */
export function SidebarResize() {
  const drag = (e: PointerEvent) => {
    const handle = e.currentTarget as HTMLElement;
    const startX = e.clientX;
    const start = sidebarWidth.value;
    handle.setPointerCapture?.(e.pointerId);
    const move = (m: PointerEvent) => setSidebarWidth(start + (m.clientX - startX));
    const stopDrag = () => {
      handle.removeEventListener("pointermove", move);
      handle.removeEventListener("pointerup", stopDrag);
      handle.removeEventListener("pointercancel", stopDrag);
    };
    handle.addEventListener("pointermove", move);
    handle.addEventListener("pointerup", stopDrag);
    handle.addEventListener("pointercancel", stopDrag);
    e.preventDefault();
  };
  if (!sidebarOpen.value) return null;
  return (
    <div
      class="sidebar-resize"
      role="separator"
      aria-orientation="vertical"
      aria-label="Resize the sidebar"
      aria-valuenow={sidebarWidth.value}
      tabIndex={0}
      title="Drag to resize (arrow keys too; double click for the usual width)"
      onPointerDown={drag}
      onDblClick={() => setSidebarWidth(SIDEBAR_WIDTH_DEFAULT)}
      onKeyDown={(e) => {
        if (e.key === "ArrowRight") (e.preventDefault(), setSidebarWidth(sidebarWidth.value + 24));
        else if (e.key === "ArrowLeft") (e.preventDefault(), setSidebarWidth(sidebarWidth.value - 24));
      }}
    />
  );
}

/** The left sidebar: the sessions, the menu, the stages, the design files, constraints and results */
export function Sidebar() {
  const [optionsOpen, setOptionsOpen] = useState(false);
  const picker = useRef<HTMLInputElement>(null);
  const v = view.value;
  const session = v.session;
  // The buttons of the sessions work on real sessions, not on the recorded demo
  const real = config.value?.backend === "session";
  const files = v.files;
  // Synthesis follows the device (its LUT size and script come from the device); a skipped one
  // (the design is a netlist) is not counted
  const device = v.stages.findIndex((s) => s.stage === "DEVICE");
  const vpr = files.inputs.find((f) => f.label === "vpr arch");
  const oarch = files.inputs.find((f) => f.label === "openfpga arch");
  // The device is known once its architectures are read (VPR makes its grid with the circuit)
  const stages = v.stages.map((s) =>
    s.stage === "DEVICE" && s.status === "not_reached" && vpr && oarch ? { ...s, status: "reached" as const } : s,
  );
  if (v.synthesis) stages.splice(device + 1, 0, { ...v.synthesis, stage: "SYNTHESIS" });
  const counted = stages.filter((s) => s.status !== "skipped");
  const reached = counted.filter((s) => s.status === "reached").length;
  const rows = designFiles(files);
  const canRun = real && !busy.value && !!session && session.worker === "running";
  const run = (stage: string): string | null => {
    if (stage === "SYNTHESIS") return v.synthesis?.design ? `/synthesize design=${v.synthesis.design}` : null;
    return RUN[stage] ? `/run_stage stage=${RUN[stage]}` : null;
  };
  return (
    <aside
      class={`sidebar ${sidebarOpen.value ? "" : "closed"}`}
      aria-label="Session and design files"
      style={{ "--sidebar-w": `${sidebarWidth.value}px` }}
    >
      <SessionPanel real={real} />
      <Menu real={real} />
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
              <span class="stage-name">
                {s.stage}
                {s.stage === "DEVICE" && session?.device_package && (
                  <span class="stage-device muted"> {session.device_package.name}</span>
                )}
              </span>
              <span class="stage-tools">
                {s.seconds != null && <span class="stage-time muted">{s.seconds.toFixed(1)} s</span>}
                {(v.reports ?? []).includes(REPORT[s.stage]) && (
                  <button
                    class="stage-button stage-report"
                    aria-label={`Open the report of ${s.stage.toLowerCase()}`}
                    title="Open the HTML report of this stage: summary, warnings and errors"
                    onClick={() => openReport(REPORT[s.stage])}
                  >
                    ⎙
                  </button>
                )}
                {s.stage === "DEVICE" && (vpr || oarch) && (
                  <button
                    class="stage-button"
                    aria-label="Open the architecture files"
                    disabled={!real}
                    title={real ? `Open the VPR and the OpenFPGA architecture files${vpr?.extra ? ` (${vpr.extra})` : ""}` : "Not in the demo"}
                    onClick={() => {
                      if (vpr) void openFile(vpr.name);
                      if (oarch) void openFile(oarch.name);
                    }}
                  >
                    ≣
                  </button>
                )}
                {(s.stage === "SYNTHESIS" || RUN[s.stage]) && (
                  <button
                    class="stage-button"
                    aria-label={`Run ${s.stage.toLowerCase()}`}
                    disabled={!canRun || !run(s.stage)}
                    title={
                      !real
                        ? "Not in the demo"
                        : s.stage === "SYNTHESIS" && !v.synthesis?.design
                          ? "Add a design first (Designs)"
                          : s.stage === "SYNTHESIS"
                            ? `Synthesize ${v.synthesis?.design} again`
                            : `Run ${RUN[s.stage]} (and the stages before it that are not reached)`
                    }
                    onClick={() => {
                      const command = run(s.stage);
                      if (command) void sendMessage(command);
                    }}
                  >
                    ▶
                  </button>
                )}
              </span>
              {s.status === "failed" && s.message && <div class="stage-message">{s.message}</div>}
            </li>
          ))}
        </ol>
      </Section>
      <Section title="Design files">
        {rows.length ? (
          <div class="uploads design-files">
            {rows.map((f) => (
              <div key={f.name} class={`upload-row ${f.loaded ? "loaded" : ""}`} title={f.loaded ? "The circuit of the session" : undefined}>
                <Name name={f.name} label={f.label} />
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
          <div class="waiting">
            {v.synthesis?.design
              ? `Design ${v.synthesis.design} added: synthesize it (▶ of SYNTHESIS) to make the circuit.`
              : "Waiting for a circuit: add a design (Designs), or upload one below and say which to read."}
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
    </aside>
  );
}
