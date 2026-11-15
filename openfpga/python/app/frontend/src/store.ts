// The state of the page: signals that the components read, and the one function that turns an
// event of the server into a change of them.
import { signal } from "@preact/signals";
import {
  Answer,
  AppConfig,
  AppEvent,
  SessionView,
  cancel,
  closeSession,
  fetchConfig,
  newSession,
  restoreSession,
  fetchSession,
  runConsole,
  streamChat,
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
  id: string; // the call that it shows
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
export const activeTab = signal<string>(CHAT);
export const split = signal(false);
export const busy = signal(false);
export const banner = signal<{ kind: string; message: string; hint?: string | null } | null>(null);
export const consoleOpen = signal(false);
export const consoleLines = signal<ConsoleLine[]>([]);
export const consoleBusy = signal(false);
const wide = () => typeof window === "undefined" || window.innerWidth > 900;
export const sidebarOpen = signal(wide());
export const settingsOpen = signal(false);

export function resetStore() {
  view.value = emptyView();
  config.value = null;
  messages.value = [];
  calls.value = {};
  tabs.value = [];
  activeTab.value = CHAT;
  split.value = false;
  busy.value = false;
  banner.value = null;
  consoleOpen.value = false;
  consoleLines.value = [];
  consoleBusy.value = false;
  sidebarOpen.value = wide();
  settingsOpen.value = false;
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

export async function loadAll() {
  await Promise.all([
    refreshView(),
    fetchConfig().then((c) => (config.value = c)).catch(() => undefined),
  ]);
}

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
export const endSession = () => sessionAction(() => closeSession());
/** Back to the last checkpoint, or to a named one */
export const restore = (checkpoint?: string) => sessionAction(() => restoreSession(checkpoint));

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
  const rest = tabs.value.filter((t) => t.id !== id);
  tabs.value = rest;
  if (activeTab.value === id) activeTab.value = rest.length ? rest[rest.length - 1].id : CHAT;
  if (!rest.length) split.value = false;
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
