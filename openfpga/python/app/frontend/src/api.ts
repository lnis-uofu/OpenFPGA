// The API of the app: what the server sends, and the calls that the page makes.
// The events are those of openfpga_app/schemas/events.schema.json (schema_version 1).

export type StageStatus = "reached" | "not_reached" | "failed" | "running" | "skipped";

export interface StageView {
  stage: string;
  status: StageStatus;
  seconds: number | null;
  message: string | null;
  /** Of synthesis: the design directory that the session copied last, which its run button synthesizes */
  design?: string | null;
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
    device_package?: DevicePackageInfo | null;
  } | null;
  latest: { status: "none" | "running" | "reached" | "failed"; text: string };
  /** Synthesis, before the stages: skipped when the design is a netlist */
  synthesis?: StageView;
  stages: StageView[];
  files: { inputs: InputFile[]; constraints: string[]; results: string[]; uploads?: UploadedFile[]; created?: UploadedFile[]; synthesis?: UploadedFile[]; design?: UploadedFile[] };
  options_changed: { name: string; value: unknown }[];
  checkpoints: { name: string; kind: string; stages: string[] }[];
  /** The stages that have an HTML report: device, synthesis, pack, place, route, analysis */
  reports?: string[];
  /** The design directories of the designs folders (a folder with a design.json) */
  designs?: DesignView[];
}

/** A design directory: what the sidebar shows of it; the files are the ones of the manifest */
export interface DesignView {
  name: string;
  top: string | null;
  path: string;
  files: string[];
  /** The netlists of the manifest: read only when the design has no RTL */
  netlists?: string[];
  description: string | null;
}

/** A device package: what the page shows to choose one (the files of it are the server's) */
export interface DevicePackageInfo {
  name: string;
  version: string;
  description: string;
  layout: string;
  lut_size: number | null;
  tags: string[];
}

export interface UploadedFile {
  name: string;
  kind: "vpr_arch" | "openfpga_arch" | "circuit" | "other";
  bytes: number;
}

/** The most that the server takes for one file, and the kinds of file that a flow reads */
export const MAX_UPLOAD_BYTES = 50 * 1024 * 1024;
export const UPLOAD_EXTENSIONS = [".xml", ".blif", ".eblif", ".v", ".sv", ".vh", ".sdc", ".pcf", ".json", ".csv", ".txt"];

export interface AppConfig {
  schema_version: number;
  console: boolean;
  backend: "demo" | "session";
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
  | {
      type: "usage";
      id: string;
      step: number;
      prompt_tokens: number | null;
      completion_tokens: number | null;
      context_tokens: number;
      context_limit: number;
    }
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

/** The answer in the body of a response, also for a refusal (4xx has diagnostics); the status if there is none */
async function answerOf(response: Response): Promise<Answer> {
  try {
    return (await response.json()) as Answer;
  } catch {
    return { ok: false, diagnostics: [{ kind: "server", message: `${response.status} ${response.statusText}` }] };
  }
}

export const fetchSession = (): Promise<SessionView> => fetch("/api/session").then((r) => json<SessionView>(r));
export const fetchConfig = (): Promise<AppConfig> => fetch("/api/config").then((r) => json<AppConfig>(r));

export async function cancel(): Promise<void> {
  await fetch("/api/cancel", { method: "POST", headers: { "Content-Type": "application/json" }, body: "{}" });
}

/** An answer of a tool, as the sessions endpoints give it */
async function post(path: string, body: unknown): Promise<Answer> {
  const response = await fetch(path, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(body),
  });
  return json<Answer>(response);
}

/** A file for the session: the body is the file itself. 409 says that the name is taken */
export async function uploadFile(file: File, overwrite = false): Promise<{ status: number; answer: Answer }> {
  const query = `name=${encodeURIComponent(file.name)}${overwrite ? "&overwrite=1" : ""}`;
  const response = await fetch(`/api/upload?${query}`, {
    method: "POST",
    headers: { "Content-Type": "application/octet-stream" },
    body: file,
  });
  let answer: Answer;
  try {
    answer = (await response.json()) as Answer;
  } catch {
    answer = { ok: false, diagnostics: [{ kind: "server", message: `${response.status} ${response.statusText}` }] };
  }
  return { status: response.status, answer };
}

/** The text of a file of the session, an input or a package, for the viewer */
export async function fetchFile(name: string): Promise<Answer> {
  const response = await fetch(`/api/file?name=${encodeURIComponent(name)}`);
  try {
    return (await response.json()) as Answer;
  } catch {
    return { ok: false, diagnostics: [{ kind: "server", message: `${response.status} ${response.statusText}` }] };
  }
}

/** Save the text of a file of the session that may be edited */
export async function saveFileText(name: string, text: string): Promise<Answer> {
  const response = await fetch(`/api/file?name=${encodeURIComponent(name)}`, {
    method: "PUT",
    headers: { "Content-Type": "text/plain; charset=utf-8" },
    body: text,
  });
  return answerOf(response);
}

/** A copy of a file that cannot be edited, in the uploads of the session */
export const copyFileToSession = async (name: string): Promise<Answer> =>
  answerOf(await fetch(`/api/file/copy?name=${encodeURIComponent(name)}`, { method: "POST" }));

/** Delete a file that was uploaded to the session */
export async function removeUploadedFile(name: string): Promise<Answer> {
  const response = await fetch(`/api/upload?name=${encodeURIComponent(name)}`, { method: "DELETE" });
  return json<Answer>(response);
}

export const newSession = (name?: string, devicePackage?: string): Promise<Answer> =>
  post("/api/session/new", { ...(name ? { name } : {}), ...(devicePackage ? { device_package: devicePackage } : {}) });

/** Read the architectures of a package into the open session (it must be empty) */
export const applyDevicePackage = (devicePackage: string): Promise<Answer> =>
  post("/api/session/device", { device_package: devicePackage });

/** The packages that a session can be created from, and the ones that are damaged */
export async function fetchDevicePackages(): Promise<{ packages: DevicePackageInfo[]; problems: { name: string; message: string }[] }> {
  const answer = await json<Answer>(await fetch("/api/device-packages"));
  const data = (answer.data ?? {}) as { packages?: DevicePackageInfo[]; problems?: { name: string; message: string }[] };
  return { packages: data.packages ?? [], problems: data.problems ?? [] };
}
/** A session that the page may open: a live one (switch) or a saved one (started from its checkpoint) */
export interface SessionEntry {
  id: string;
  name: string;
  stages: string[];
  device_package: DevicePackageInfo | null;
  running?: boolean;
  idle_s?: number;
  current?: boolean;
  modified?: number; // seconds since 1970, of the last checkpoint of a saved session
}

export async function fetchSessions(): Promise<{ live: SessionEntry[]; saved: SessionEntry[] }> {
  const answer = await json<Answer>(await fetch("/api/sessions"));
  const data = (answer.data ?? {}) as { live?: SessionEntry[]; saved?: SessionEntry[] };
  return { live: data.live ?? [], saved: data.saved ?? [] };
}

export const openSessionById = async (session: string): Promise<Answer> =>
  answerOf(await fetch("/api/session/open", { method: "POST", headers: { "Content-Type": "application/json" }, body: JSON.stringify({ session }) }));

export const closeSession = (): Promise<Answer> => post("/api/session/close", {});
export const restoreSession = (checkpoint?: string): Promise<Answer> =>
  post("/api/session/restore", checkpoint ? { checkpoint } : {});

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
