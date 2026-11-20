import { busy, openSession, sessionList } from "../store";

const STAGE_NAMES: Record<string, string> = { DEVICE: "device", PACKED: "packed", PLACED: "placed", ROUTED: "routed", ANALYZED: "analyzed" };

/** What a session has reached, in one word: the last stage */
function reached(stages: string[]): string {
  const last = stages[stages.length - 1];
  return last ? STAGE_NAMES[last] ?? last.toLowerCase() : "nothing yet";
}

function ago(seconds: number | undefined): string {
  if (seconds == null) return "";
  const s = Math.max(0, Date.now() / 1000 - seconds);
  if (s < 90) return "a moment ago";
  if (s < 5400) return `${Math.round(s / 60)} min ago`;
  if (s < 129600) return `${Math.round(s / 3600)} h ago`;
  return `${Math.round(s / 86400)} days ago`;
}

/** The sessions that can be opened: the live ones are switched to, the saved ones (made by an earlier
 * run of the server, or closed) are started again from their last checkpoint */
export function SessionPicker() {
  const found = sessionList.value;
  return (
    <div class="packages sessions" role="region" aria-label="Sessions">
      {found === null ? (
        <div class="muted">Looking…</div>
      ) : (
        <>
          {found.live.length + found.saved.length === 0 && (
            <div class="muted">No session to open. A session that ran a stage is saved and shows here.</div>
          )}
          {found.live.map((s) => (
            <div key={s.id} class="package session-entry">
              <div class="package-head">
                <strong>{s.name}</strong>
                <span class="chip">{s.id}</span>
                <span class="chip">{s.running === false ? "worker ended" : "open"}</span>
              </div>
              <div class="muted package-text">
                {s.device_package ? `${s.device_package.name} · ` : ""}
                {reached(s.stages)}
              </div>
              <button disabled={busy.value || s.current} onClick={() => void openSession(s.id)}>
                {s.current ? "Current" : "Open"}
              </button>
            </div>
          ))}
          {found.saved.map((s) => (
            <div key={s.id} class="package session-entry">
              <div class="package-head">
                <strong>{s.name}</strong>
                <span class="chip">{s.id}</span>
                <span class="chip">saved</span>
              </div>
              <div class="muted package-text">
                {s.device_package ? `${s.device_package.name} · ` : ""}
                {reached(s.stages)} · {ago(s.modified)}
              </div>
              <button disabled={busy.value} onClick={() => void openSession(s.id)} title="Start it again from its last checkpoint">
                Open
              </button>
            </div>
          ))}
        </>
      )}
    </div>
  );
}
