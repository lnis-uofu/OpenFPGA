import { useState } from "preact/hooks";
import { Markdown } from "../markdown";
import { resultMarkdown } from "../resultMarkdown";
import { calls, openResult, stop } from "../store";

const SYMBOL = { running: "░", done: "✔", failed: "✖", cancelled: "■" } as const;
const SHOWN = 700;

function shortArguments(args: Record<string, unknown>): string {
  return Object.entries(args)
    .filter(([key]) => key !== "session")
    .map(([key, value]) => `${key}=${typeof value === "string" ? value : JSON.stringify(value)}`)
    .join(" ");
}

/** A tool call of the model: collapsed it is one line; opened, its arguments and its result */
export function ToolCard(props: { callId: string; startOpen?: boolean }) {
  const [open, setOpen] = useState(props.startOpen ?? false);
  const [all, setAll] = useState(false);
  const [raw, setRaw] = useState(false); // the JSON as the model read it, instead of the formatted text
  const call = calls.value[props.callId];
  if (!call) return null;
  const result = call.result;
  const text = result ? JSON.stringify(result, null, 1) : "";
  const diagnostics = result?.diagnostics ?? [];
  return (
    <div class={`card card-${call.state}`}>
      <button class="card-head" aria-expanded={open} onClick={() => setOpen(!open)}>
        <span aria-hidden="true">{SYMBOL[call.state]}</span>
        <span class="sr-only">{call.state}</span>
        <strong>{call.tool}</strong>
        <span class="args">{shortArguments(call.arguments)}</span>
        <span class="muted time">{call.state === "running" ? "running" : call.seconds != null ? `${call.seconds.toFixed(1)} s` : ""}</span>
        <span aria-hidden="true">{open ? "▾" : "▸"}</span>
      </button>
      {call.state === "running" && (
        <div class="card-actions">
          <button onClick={() => void stop()}>■ Cancel call</button>
        </div>
      )}
      {open && (
        <div class="card-body">
          <div>
            <span class="label">Arguments</span> <code>{JSON.stringify(call.arguments)}</code>
          </div>
          {call.state === "failed" &&
            raw && // the formatted result has them
            diagnostics.map((d, i) => (
              <div class="diagnostic" key={i}>
                <strong>{d.kind}</strong> {d.message}
                {d.hint && <div>Try: {d.hint}</div>}
              </div>
            ))}
          {result && (
            <>
              <div>
                <span class="label">Result</span> {result.ok ? "ok" : "failed"} · {text.length.toLocaleString()} characters
              </div>
              {call.truncated && <div class="notice">⚠ {result.more ?? "The result was cut."}</div>}
              {raw ? (
                <pre class="json">{all || text.length <= SHOWN ? text : `${text.slice(0, SHOWN)}…`}</pre>
              ) : (
                <div class="result-md">
                  <Markdown text={resultMarkdown(call.tool, result)} />
                </div>
              )}
              {raw && text.length > SHOWN && (
                <button class="link" onClick={() => setAll(!all)}>
                  {all ? "Show less" : "Show all"}
                </button>
              )}
              <div class="buttons">
                <button onClick={() => openResult(call.callId)}>Open in tab</button>
                <button onClick={() => setRaw(!raw)} aria-pressed={raw}>
                  {raw ? "Formatted" : "JSON"}
                </button>
                <button onClick={() => navigator.clipboard?.writeText(JSON.stringify(result))}>Copy JSON</button>
              </div>
            </>
          )}
        </div>
      )}
    </div>
  );
}

/** Calls in a row: the first three are cards; more are one line that opens the others */
export function CallGroup(props: { callIds: string[] }) {
  const [open, setOpen] = useState(false);
  const { callIds } = props;
  if (callIds.length <= 3 || open) {
    return (
      <>
        {callIds.map((id) => (
          <ToolCard key={id} callId={id} />
        ))}
        {callIds.length > 3 && (
          <button class="link" onClick={() => setOpen(false)}>
            Fold {callIds.length} calls
          </button>
        )}
      </>
    );
  }
  const states = callIds.map((id) => calls.value[id]?.state);
  const failed = states.filter((s) => s === "failed").length;
  const running = states.includes("running");
  return (
    <button class="card card-group" aria-expanded={false} onClick={() => setOpen(true)}>
      <span aria-hidden="true">{running ? "░" : failed ? "⚠" : "✔"}</span> {callIds.length} calls
      {failed ? `, ${failed} failed` : ""} ▸
    </button>
  );
}
