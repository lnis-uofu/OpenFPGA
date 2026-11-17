// The state of the page: signals that the components read, and the one function that turns an
// event of the server into a change of them.
import { effect, signal } from "@preact/signals";
import {
  Answer,
  AppConfig,
  AppEvent,
  DevicePackageInfo,
  MAX_UPLOAD_BYTES,
  SessionView,
  UPLOAD_EXTENSIONS,
  cancel,
  closeSession,
  fetchConfig,
  newSession,
  restoreSession,
  fetchSession,
  runConsole,
  streamChat,
  fetchDevicePackages,
  fetchFile,
  removeUploadedFile,
  uploadFile,
} from "./api";

export type Part = { kind: "text"; text: string } | { kind: "call"; callId: string };
export interface Message {
  id: string;
  role: "user" | "assistant";
  parts: Part[];
}
export type CallState = "running" | "done" | "failed" | "cancelled";
export interface ToolCall {
  callId: string;
  tool: string;
  arguments: Record<string, unknown>;
  state: CallState;
  seconds: number | null;
  truncated: boolean;
  result: Answer | null;
}
export interface ResultTab {
  id: string; // the call that it shows, or "file:NAME" for a file
  file?: string; // the name of the file, for a tab of the viewer
  title: string;
  pinned: boolean;
  unread: boolean;
}
export interface ConsoleLine {
  kind: "input" | "output" | "error" | "model";
  text: string;
}

export const CHAT = "chat";
// The calls that change the flow: the console shows them as the lines of the journal
const CHANGING = new Set(["session_open", "session_close", "read_inputs", "run_stage", "set_option", "set_constraint", "checkpoint"]);

export const emptyView = (): SessionView => ({
  schema_version: 1,
  session: null,
  latest: { status: "none", text: "No session" },
  stages: ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED"].map((stage) => ({
    stage,
    status: "not_reached" as const,
    seconds: null,
    message: null,
  })),
  files: { inputs: [], constraints: [], results: [] },
  options_changed: [],
  checkpoints: [],
});

export const view = signal<SessionView>(emptyView());
export const config = signal<AppConfig | null>(null);
export const messages = signal<Message[]>([]);
export const calls = signal<Record<string, ToolCall>>({});
export const tabs = signal<ResultTab[]>([]);
/** The files that are open in the viewer, by name */
export interface FileState {
  status: "loading" | "ready" | "failed";
  text: string;
  bytes: number;
  truncated: boolean;
  message: string;
}
export const openFiles = signal<Record<string, FileState>>({});
export const CONSOLE_HEIGHT_DEFAULT = 180;
export const CONSOLE_HEIGHT_MIN = 80;
const CONSOLE_HEIGHT_KEY = "openfpga-console-height";
export const activeTab = signal<string>(CHAT);
export const split = signal(false);

function storedHeight(): number {
  try {
    const n = Number(window.localStorage.getItem(CONSOLE_HEIGHT_KEY));
    return Number.isFinite(n) && n >= CONSOLE_HEIGHT_MIN ? n : CONSOLE_HEIGHT_DEFAULT;
  } catch {
    return CONSOLE_HEIGHT_DEFAULT;
  }
}
/** The height of the lines of the console in px: dragged by the person and kept in this browser */
export const consoleHeight = signal(storedHeight());

export function setConsoleHeight(px: number) {
  const most = Math.max(CONSOLE_HEIGHT_MIN + 40, Math.floor(window.innerHeight * 0.7));
  consoleHeight.value = Math.round(Math.min(most, Math.max(CONSOLE_HEIGHT_MIN, px)));
  try {
    window.localStorage.setItem(CONSOLE_HEIGHT_KEY, String(consoleHeight.value));
  } catch {
    /* not kept */
  }
}
export const busy = signal(false);
export const banner = signal<{ kind: string; message: string; hint?: string | null } | null>(null);
export const consoleOpen = signal(false);
export const consoleLines = signal<ConsoleLine[]>([]);
export const consoleBusy = signal(false);
const wide = () => typeof window === "undefined" || window.innerWidth > 900;
export const sidebarOpen = signal(wide());
export const settingsOpen = signal(false);
/** A file is on its way to the session */
export const uploading = signal(false);
/** What the last step of the model cost and how full its context is (none before a model answers) */
export const usage = signal<{ step: number; used: number; limit: number } | null>(null);

export function resetStore() {
  view.value = emptyView();
  config.value = null;
  messages.value = [];
  calls.value = {};
  tabs.value = [];
  openFiles.value = {};
  loaded.value = false;
  welcomeChat.value = false;
  consoleHeight.value = CONSOLE_HEIGHT_DEFAULT;
  activeTab.value = CHAT;
  split.value = false;
  busy.value = false;
  banner.value = null;
  consoleOpen.value = false;
  consoleLines.value = [];
  consoleBusy.value = false;
  sidebarOpen.value = wide();
  settingsOpen.value = false;
  uploading.value = false;
  packagesOpen.value = false;
  devicePackages.value = null;
  usage.value = null;
}

// The events -------------------------------------------------------------------------------

function updateMessage(id: string, change: (message: Message) => Message) {
  messages.value = messages.value.map((m) => (m.id === id ? change(m) : m));
}

function shortArguments(args: Record<string, unknown>): string {
  return Object.entries(args)
    .filter(([key]) => key !== "session")
    .map(([key, value]) => `${key}=${typeof value === "string" ? value : JSON.stringify(value)}`)
    .join(" ");
}

export function applyEvent(event: AppEvent) {
  switch (event.type) {
    case "message_start":
      messages.value = [...messages.value, { id: event.id, role: "assistant", parts: [] }];
      break;
    case "text_delta":
      updateMessage(event.id, (m) => {
        const parts = m.parts.slice();
        const last = parts[parts.length - 1];
        if (last && last.kind === "text") parts[parts.length - 1] = { kind: "text", text: last.text + event.text };
        else parts.push({ kind: "text", text: event.text });
        return { ...m, parts };
      });
      break;
    case "tool_call_started":
      calls.value = {
        ...calls.value,
        [event.call_id]: {
          callId: event.call_id,
          tool: event.tool,
          arguments: event.arguments,
          state: "running",
          seconds: null,
          truncated: false,
          result: null,
        },
      };
      updateMessage(event.id, (m) => ({ ...m, parts: [...m.parts, { kind: "call", callId: event.call_id }] }));
      if (CHANGING.has(event.tool)) {
        const text = `${event.tool} ${shortArguments(event.arguments)}`.trim();
        consoleLines.value = [...consoleLines.value, { kind: "model", text }];
      }
      break;
    case "tool_call_finished": {
      const call = calls.value[event.call_id];
      if (!call) break;
      const cancelled = !event.ok && event.result.diagnostics?.[0]?.kind === "cancelled";
      calls.value = {
        ...calls.value,
        [event.call_id]: {
          ...call,
          state: event.ok ? "done" : cancelled ? "cancelled" : "failed",
          seconds: event.seconds,
          truncated: event.truncated,
          result: event.result,
        },
      };
      // The tab of a result that was open shows the newer answer of the same call
      break;
    }
    case "state_changed":
      void refreshView();
      break;
    case "usage":
      usage.value = { step: event.step, used: event.context_tokens, limit: event.context_limit };
      break;
    case "error":
      banner.value = { kind: event.kind, message: event.message, hint: event.hint };
      break;
    case "done":
      break;
  }
}

export async function refreshView() {
  try {
    view.value = await fetchSession();
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  }
}

/** The first view and the configuration have come (or failed): until then the page shows nothing
 * of the session, so that it does not draw the full interface and then the welcome screen */
export const loaded = signal(false);

export async function loadAll() {
  try {
    await Promise.all([
      refreshView(),
      fetchConfig().then((c) => (config.value = c)).catch(() => undefined),
    ]);
  } finally {
    loaded.value = true;
  }
}

/** A message was sent from the welcome screen: its answer is shown there, in a small window, and the
 * screen stays until a session exists and the answer is complete */
export const welcomeChat = signal(false);

/** The welcome screen: real sessions, and either nothing has happened yet (no session, nothing said),
 * or the person asked something from it and the answer is still coming or there is no session yet.
 * A session that is created from the buttons, or one that exists when the answer ends, gives the
 * full interface */
export function welcoming(): boolean {
  if (!loaded.value || config.value?.backend !== "session") return false;
  if (welcomeChat.value) return true;
  return view.value.session === null && messages.value.length === 0;
}

// The answer is complete and there is a session: the welcome screen is done with
effect(() => {
  if (welcomeChat.value && view.value.session !== null && !busy.value) welcomeChat.value = false;
});

// The sessions -------------------------------------------------------------------------------------

/** What an answer of the sessions endpoints says went wrong, as a banner (nothing if it went well) */
function reportFailure(answer: Answer) {
  if (answer.ok) {
    banner.value = null;
    return;
  }
  const d = answer.diagnostics?.[0];
  banner.value = { kind: d?.kind ?? "error", message: d?.message ?? "The call failed", hint: d?.hint };
}

async function sessionAction(action: () => Promise<Answer>) {
  try {
    reportFailure(await action());
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  }
  await refreshView();
}

export const makeSession = (name?: string) => sessionAction(() => newSession(name));

/** The picker of device packages under the session buttons */
export const packagesOpen = signal(false);
export const devicePackages = signal<{ packages: DevicePackageInfo[]; problems: { name: string; message: string }[] } | null>(null);

export async function togglePackages() {
  packagesOpen.value = !packagesOpen.value;
  if (!packagesOpen.value) return;
  try {
    devicePackages.value = await fetchDevicePackages(); // read again each time: a package may have been added
  } catch (error) {
    packagesOpen.value = false;
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  }
}

/** A session that reads the files of a device package and waits for the circuit */
export async function makeSessionFromPackage(name: string) {
  packagesOpen.value = false;
  await sessionAction(() => newSession(undefined, name));
}
export const endSession = () => sessionAction(() => closeSession());
/** Back to the last checkpoint, or to a named one */
export const restore = (checkpoint?: string) => sessionAction(() => restoreSession(checkpoint));

/** Give files to the session (one is opened if there is none). A name that is taken is replaced
 * only if the person says so */
export async function uploadFiles(files: File[]) {
  if (!files.length || uploading.value) return;
  uploading.value = true;
  banner.value = null;
  try {
    for (const file of files) {
      const extension = file.name.includes(".") ? file.name.slice(file.name.lastIndexOf(".")).toLowerCase() : "";
      if (!UPLOAD_EXTENSIONS.includes(extension)) {
        banner.value = {
          kind: "invalid_type",
          message: `${file.name} is not a kind of file that a flow reads`,
          hint: `Accepted: ${UPLOAD_EXTENSIONS.join(", ")}`,
        };
        break;
      }
      if (file.size > MAX_UPLOAD_BYTES) {
        banner.value = { kind: "too_large", message: `${file.name} is larger than ${MAX_UPLOAD_BYTES / 1024 / 1024} MB` };
        break;
      }
      let result = await uploadFile(file);
      if (result.status === 409) {
        if (!window.confirm(`There is already a file "${file.name}" in the session. Replace it?`)) continue;
        result = await uploadFile(file, true);
      }
      if (!result.answer.ok) {
        reportFailure(result.answer);
        break;
      }
      // A circuit is read by itself when the session waits for one; if that failed the file is there
      const data = result.answer.data as { load_error?: string; load_hint?: string | null } | undefined;
      if (data?.load_error) {
        banner.value = {
          kind: "circuit_not_read",
          message: `${file.name} was uploaded, but reading it as the circuit failed: ${data.load_error}`,
          hint: data.load_hint,
        };
      }
    }
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  } finally {
    uploading.value = false;
    await refreshView();
  }
}

/** Take a file that was uploaded out of the session */
export async function removeUpload(name: string) {
  banner.value = null;
  try {
    const answer = await removeUploadedFile(name);
    if (!answer.ok) reportFailure(answer);
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  } finally {
    await refreshView();
  }
}

// The conversation -------------------------------------------------------------------------

let counter = 0;

export async function sendMessage(text: string) {
  const message = text.trim();
  if (!message || busy.value) return;
  banner.value = null;
  counter += 1;
  messages.value = [...messages.value, { id: `u${counter}`, role: "user", parts: [{ kind: "text", text: message }] }];
  busy.value = true;
  try {
    for await (const event of streamChat(message)) applyEvent(event);
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  } finally {
    busy.value = false;
    await refreshView();
  }
}

export async function stop() {
  await cancel().catch(() => undefined);
}

// The tabs of the results ------------------------------------------------------------------------

/** What names the result of a call in a tab: the tool and its main argument */
export function tabTitle(call: ToolCall): string {
  const main = ["net", "block", "pattern", "name", "stage", "kind"].map((k) => call.arguments[k]).find((v) => typeof v === "string");
  return main ? `${call.tool} ${main}` : call.tool;
}

export function openResult(callId: string, activate = true) {
  const call = calls.value[callId];
  if (!call || !call.result) return;
  const existing = tabs.value.find((t) => t.id === callId);
  if (existing) {
    tabs.value = tabs.value.map((t) => (t.id === callId ? { ...t, unread: activate ? false : t.unread } : t));
  } else {
    // A newer result of the same tool replaces the tab that is not pinned
    const title = tabTitle(call);
    const same = tabs.value.find((t) => !t.pinned && calls.value[t.id]?.tool === call.tool);
    const tab: ResultTab = { id: callId, title, pinned: false, unread: !activate };
    tabs.value = same ? tabs.value.map((t) => (t === same ? tab : t)) : [...tabs.value, tab];
  }
  if (activate) activeTab.value = callId;
}

export function closeTab(id: string) {
  const closed = tabs.value.find((t) => t.id === id);
  if (closed?.file) {
    const { [closed.file]: _gone, ...kept } = openFiles.value;
    openFiles.value = kept;
  }
  const rest = tabs.value.filter((t) => t.id !== id);
  tabs.value = rest;
  if (activeTab.value === id) activeTab.value = rest.length ? rest[rest.length - 1].id : CHAT;
  if (!rest.length) split.value = false;
}

export const fileTabId = (name: string) => `file:${name}`;

/** Open a file in a tab of the viewer (or go to it), and read it again: a result may have changed */
export async function openFile(name: string) {
  const id = fileTabId(name);
  if (!tabs.value.some((t) => t.id === id)) {
    tabs.value = [...tabs.value, { id, title: name.split("/").pop() || name, pinned: false, unread: false, file: name }];
  }
  activeTab.value = id;
  await loadFile(name);
}

export async function loadFile(name: string) {
  const before = openFiles.value[name];
  openFiles.value = {
    ...openFiles.value,
    [name]: { status: "loading", text: before?.text ?? "", bytes: before?.bytes ?? 0, truncated: false, message: "" },
  };
  let next: FileState;
  try {
    const answer = await fetchFile(name);
    if (answer.ok) {
      const data = answer.data as { text: string; bytes: number; truncated: boolean };
      next = { status: "ready", text: data.text, bytes: data.bytes, truncated: data.truncated, message: "" };
    } else {
      const d = answer.diagnostics?.[0];
      next = { status: "failed", text: "", bytes: 0, truncated: false, message: d ? d.message + (d.hint ? ` (${d.hint})` : "") : "The file could not be read" };
    }
  } catch (error) {
    next = { status: "failed", text: "", bytes: 0, truncated: false, message: String(error) };
  }
  if (name in openFiles.value) openFiles.value = { ...openFiles.value, [name]: next }; // not if the tab was closed
}

export function pinTab(id: string) {
  tabs.value = tabs.value.map((t) => (t.id === id ? { ...t, pinned: !t.pinned } : t));
}

export function nextTab(step: 1 | -1) {
  const order = [CHAT, ...tabs.value.map((t) => t.id)];
  const at = Math.max(0, order.indexOf(activeTab.value));
  activeTab.value = order[(at + step + order.length) % order.length];
}

// The console ---------------------------------------------------------------------------------------

export async function consoleRun(command: string) {
  const text = command.trim();
  if (!text || consoleBusy.value) return;
  consoleLines.value = [...consoleLines.value, { kind: "input", text }];
  consoleBusy.value = true;
  try {
    const answer = await runConsole(text);
    if (answer.output) {
      consoleLines.value = [...consoleLines.value, { kind: answer.ok ? "output" : "error", text: answer.output }];
    }
    if (answer.state_changed) {
      await refreshView();
      banner.value = null;
    }
  } catch (error) {
    consoleLines.value = [...consoleLines.value, { kind: "error", text: String(error) }];
  } finally {
    consoleBusy.value = false;
  }
}
