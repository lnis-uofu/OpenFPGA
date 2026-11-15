// The API of the app: what the server sends, and the calls that the page makes.
// The events are those of openfpga_app/schemas/events.schema.json (schema_version 1).

export type StageStatus = "reached" | "not_reached" | "failed" | "running";

export interface StageView {
  stage: string;
  status: StageStatus;
  seconds: number | null;
  message: string | null;
}

export interface InputFile {
  label: string;
  name: string;
  extra: string | null;
}

export interface SessionView {
  schema_version: number;
  session: {
    id: string;
    name: string;
    worker: "running" | "ended" | "none";
    ended: string | null;
    idle_s: number;
  } | null;
  latest: { status: "none" | "running" | "reached" | "failed"; text: string };
  stages: StageView[];
  files: { inputs: InputFile[]; constraints: string[]; results: string[] };
  options_changed: { name: string; value: unknown }[];
  checkpoints: { name: string; kind: string; stages: string[] }[];
}

export interface AppConfig {
  schema_version: number;
  console: boolean;
  model: { endpoint: string | null; name: string | null };
}

export type Answer = {
  ok: boolean;
  data?: unknown;
  truncated?: boolean;
  more?: string | null;
  diagnostics?: { kind: string; message: string; hint?: string | null }[];
  [key: string]: unknown;
};

export type AppEvent =
  | { type: "message_start"; id: string; role: "assistant" }
  | { type: "text_delta"; id: string; text: string }
  | { type: "tool_call_started"; id: string; call_id: string; tool: string; arguments: Record<string, unknown> }
  | {
      type: "tool_call_finished";
      id: string;
      call_id: string;
      ok: boolean;
      seconds: number;
      truncated: boolean;
      result: Answer;
    }
  | { type: "state_changed"; reason: string }
  | { type: "error"; kind: string; message: string; hint?: string | null }
  | { type: "done"; id: string; cancelled: boolean };

export interface ConsoleAnswer {
  ok: boolean;
  output: string;
  state_changed: boolean;
}

async function json<T>(response: Response): Promise<T> {
  if (!response.ok) throw new Error(`${response.status} ${response.statusText}`);
  return (await response.json()) as T;
}

export const fetchSession = (): Promise<SessionView> => fetch("/api/session").then((r) => json<SessionView>(r));
export const fetchConfig = (): Promise<AppConfig> => fetch("/api/config").then((r) => json<AppConfig>(r));

export async function cancel(): Promise<void> {
  await fetch("/api/cancel", { method: "POST", headers: { "Content-Type": "application/json" }, body: "{}" });
}

export async function runConsole(command: string): Promise<ConsoleAnswer> {
  const response = await fetch("/api/console", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ command }),
  });
  return json<ConsoleAnswer>(response);
}

/** The events of a stream of server-sent events, as text that arrives in pieces */
export function* parseEvents(buffer: { text: string }): Generator<AppEvent> {
  let end: number;
  while ((end = buffer.text.indexOf("\n\n")) >= 0) {
    const block = buffer.text.slice(0, end);
    buffer.text = buffer.text.slice(end + 2);
    const data = block
      .split("\n")
      .filter((line) => line.startsWith("data: "))
      .map((line) => line.slice(6))
      .join("\n");
    if (data) yield JSON.parse(data) as AppEvent;
  }
}

/** Send a message and give the events of the answer as they arrive */
export async function* streamChat(message: string): AsyncGenerator<AppEvent> {
  const response = await fetch("/api/chat", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ message }),
  });
  if (!response.ok || !response.body) throw new Error(`${response.status} ${response.statusText}`);
  const reader = response.body.getReader();
  const decoder = new TextDecoder();
  const buffer = { text: "" };
  for (;;) {
    const { done, value } = await reader.read();
    if (done) break;
    buffer.text += decoder.decode(value, { stream: true });
    yield* parseEvents(buffer);
  }
}
