import { useState } from "preact/hooks";
import type { DesignView, DevicePackageInfo } from "../api";
import { addDesign, busy, config, devicePackages, loadPackageIntoSession, makeSessionFromPackage, view } from "../store";

/** The words of a search: every one must be in the text of an entry (any case) */
export function matches(query: string, ...texts: (string | null | undefined)[]): boolean {
  const haystack = texts.filter(Boolean).join(" ").toLowerCase();
  return query
    .toLowerCase()
    .split(/\s+/)
    .filter(Boolean)
    .every((word) => haystack.includes(word));
}

function Search(props: { label: string; value: string; onInput: (text: string) => void; shown: number; total: number }) {
  return (
    <div class="panel-search">
      <input
        type="search"
        aria-label={props.label}
        placeholder={props.label}
        value={props.value}
        onInput={(e) => props.onInput((e.target as HTMLInputElement).value)}
      />
      <span class="muted panel-count">
        {props.shown === props.total ? props.total : `${props.shown} of ${props.total}`}
      </span>
    </div>
  );
}

const packageText = (p: DevicePackageInfo) => [p.name, p.description, p.layout, p.lut_size != null ? `K${p.lut_size}` : "", ...p.tags];

/** The device packages that were found, with a search: a session is created from one, or an empty
 * session reads its architectures */
export function DevicesPanel() {
  const [query, setQuery] = useState("");
  const found = devicePackages.value;
  const real = config.value?.backend === "session";
  const v = view.value;
  const empty = !!v.session && v.session.worker === "running" && v.files.inputs.length === 0;
  const current = v.session?.device_package?.name;
  const shown = found ? found.packages.filter((p) => matches(query, ...packageText(p))) : [];
  return (
    <section class="panel" aria-label="Device packages">
      <Search label="Search the device packages" value={query} onInput={setQuery} shown={shown.length} total={found?.packages.length ?? 0} />
      {found === null ? (
        <div class="muted">Looking…</div>
      ) : (
        <div class="panel-list">
          {found.packages.length === 0 && <div class="muted">No device packages. Start the app with --device-packages FOLDER.</div>}
          {found.packages.length > 0 && shown.length === 0 && <div class="muted">No device package matches “{query}”.</div>}
          {shown.map((p) => (
            <div key={p.name} class={`panel-item ${p.name === current ? "current" : ""}`}>
              <div class="panel-item-head">
                <strong>{p.name}</strong>
                <span class="chip">{p.layout}</span>
                {p.lut_size != null && <span class="chip">K{p.lut_size}</span>}
                {p.version && <span class="muted">v{p.version}</span>}
                {p.name === current && <span class="chip chip-current">this session</span>}
                <span class="spacer" />
                <button disabled={!real || busy.value} onClick={() => void makeSessionFromPackage(p.name)} title={real ? "A new session that reads its architectures" : "Not in the demo"}>
                  Create session
                </button>
                {empty && (
                  <button disabled={busy.value} onClick={() => void loadPackageIntoSession(p.name)} title="Read its architectures into the open session">
                    Load into this session
                  </button>
                )}
              </div>
              {p.description && <div class="muted panel-item-text">{p.description}</div>}
              {p.tags.length > 0 && <div class="muted panel-item-text">{p.tags.join(" · ")}</div>}
            </div>
          ))}
          {found.problems
            .filter((p) => matches(query, p.name, p.message))
            .map((p) => (
              <div key={p.name} class="panel-item package-bad" title={p.message}>
                <strong>{p.name}</strong>{" "}
                <span class="muted">cannot be used: {p.message.replace(/^The device package '[^']*' cannot be used: /, "")}</span>
              </div>
            ))}
        </div>
      )}
    </section>
  );
}

/** The folder of a design directory: the last part of its path */
const folderOf = (d: DesignView) => d.path.split("/").filter(Boolean).pop() ?? d.name;

/** The design directories of the designs folders, with a search. Add copies one into the session;
 * it is not synthesized (the run button of synthesis does that) */
export function DesignsPanel() {
  const [query, setQuery] = useState("");
  const v = view.value;
  const designs = v.designs ?? [];
  const real = config.value?.backend === "session";
  const session = v.session;
  const copied = new Set((v.files.design ?? []).map((f) => f.name.split("/")[1]).filter(Boolean));
  const shown = designs.filter((d) => matches(query, d.name, d.top, d.path, d.description, ...d.files, ...(d.netlists ?? [])));
  return (
    <section class="panel" aria-label="Design directories">
      <Search label="Search the designs" value={query} onInput={setQuery} shown={shown.length} total={designs.length} />
      <div class="panel-list">
        {designs.length === 0 && <div class="muted">No design directory: a folder with a design.json in a designs folder (--designs-dir).</div>}
        {designs.length > 0 && shown.length === 0 && <div class="muted">No design matches “{query}”.</div>}
        {shown.map((d) => {
          const added = copied.has(folderOf(d));
          return (
            <div key={d.path} class={`panel-item ${added ? "current" : ""}`}>
              <div class="panel-item-head">
                <strong>{d.name}</strong>
                {d.top && <span class="chip">top {d.top}</span>}
                <span class="chip">
                  {d.files.length ? `${d.files.length} ${d.files.length === 1 ? "RTL file" : "RTL files"}` : d.netlists?.length ? "netlist only" : "no RTL"}
                </span>
                {added && <span class="chip chip-current">in this session</span>}
                <span class="spacer" />
                <button
                  disabled={!real || busy.value || !session}
                  title={
                    !real
                      ? "Not in the demo"
                      : !session
                        ? "Open a session first"
                        : `Copy this design into the session${added ? " again (in place of its copy)" : ""}; it is not synthesized`
                  }
                  onClick={() => {
                    if (added && !window.confirm(`The session has a copy of ${d.name}. Copy it again, in place of that copy?`)) return;
                    void addDesign(d.name);
                  }}
                >
                  {added ? "Add again" : "Add to session"}
                </button>
              </div>
              {d.description && <div class="muted panel-item-text">{d.description}</div>}
              <div class="muted panel-item-text panel-path">{d.path}</div>
            </div>
          );
        })}
      </div>
    </section>
  );
}
