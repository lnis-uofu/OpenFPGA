// The state of the page: signals that the components read, and the one function that turns an
// event of the server into a change of them.
import { computed, effect, signal } from "@preact/signals";
import {
  Answer,
  AppConfig,
  AppEvent,
  type ConsoleMode,
  DevicePackageInfo,
  MAX_UPLOAD_BYTES,
  SessionView,
  ViewCommandEvent,
  UPLOAD_EXTENSIONS,
  cancel,
  closeSession,
  fetchConfig,
  newSession,
  applyDevicePackage,
  fetchSessions,
  openSessionById,
  type SessionEntry,
  restoreSession,
  fetchSession,
  runConsole,
  streamChat,
  fetchDevicePackages,
  fetchFile,
  fetchHierarchy,
  type Hierarchy,
  saveFileText,
  copyFileToSession,
  removeUploadedFile,
  type FileFolder,
  uploadFile,
  saveCheckpoint,
  addDesignToSession,
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
  report?: string; // the stage whose HTML report it shows (device, synthesis, pack, place, route, analysis)
  panel?: Panel; // a list to choose from: the device packages or the design directories
  title: string;
  pinned: boolean;
  unread: boolean;
}
export interface ConsoleLine {
  kind: "input" | "output" | "error" | "model";
  text: string;
  /** Of an input: the mode it was run in, for its prompt */
  mode?: ConsoleMode;
}

export const CHAT = "chat";
/** The lists that open in a tab from the menu of the sidebar */
export type Panel = "devices" | "designs" | "hierarchy" | "floorplan" | "pins" | "routing" | "timing" | "netlist" | "clusters";
// The calls that change the flow: the console shows them as the lines of the journal
const CHANGING = new Set(["session_open", "session_close", "read_inputs", "run_stage", "set_option", "set_constraint", "checkpoint"]);

export const emptyView = (): SessionView => ({
  schema_version: 1,
  session: null,
  latest: { status: "none", text: "No session" },
  synthesis: { stage: "SYNTHESIS", status: "not_reached" as const, seconds: null, message: null },
  stages: ["DEVICE", "PACKED", "PLACED", "ROUTED", "ANALYZED", "BITSTREAM"].map((stage) => ({
    stage,
    status: "not_reached" as const,
    seconds: null,
    message: null,
  })),
  files: { inputs: [], constraints: [], results: [] },
  options_changed: [],
  checkpoints: [],
  designs: [],
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
  editable?: boolean; // a file of the session's uploads or one that the model wrote
  copyable?: boolean; // another file: a copy can be made in the session and edited there
}
export const openFiles = signal<Record<string, FileState>>({});
/** A line to show in a file once its editor is there (a check of the ODC that was clicked) */
export const fileLine = signal<{ name: string; line: number } | null>(null);
export const CONSOLE_HEIGHT_DEFAULT = 180;
export const CONSOLE_HEIGHT_MIN = 80;
const CONSOLE_HEIGHT_KEY = "openfpga-console-height";
export const activeTab = signal<string>(CHAT);
export const split = signal(false);

// The two sides of the split: a tab is on the left or on the right, and is moved by the person. The
// chat starts on the left and the rest on the right. Each side shows its tab that was active last.
export type Side = "left" | "right";
/** The tabs on the left side (the chat by its id CHAT); the others are on the right */
export const leftTabs = signal<string[]>([CHAT]);
/** The tab that each side shows when the active tab is on the other side */
export const sideShown = signal<Record<Side, string | null>>({ left: CHAT, right: null });

export const sideOf = (id: string): Side => (leftTabs.value.includes(id) ? "left" : "right");

/** The tabs of a side, in the order of the tab bar (the chat first) */
export function tabsOfSide(side: Side): string[] {
  return [CHAT, ...tabs.value.map((t) => t.id)].filter((id) => sideOf(id) === side);
}

/** The tab that a side shows: the active tab if it is there, else the one it showed last, else
 * its first */
export function shownOnSide(side: Side): string | null {
  const ids = tabsOfSide(side);
  if (ids.includes(activeTab.value)) return activeTab.value;
  const last = sideShown.value[side];
  return last && ids.includes(last) ? last : (ids[0] ?? null);
}

/** Move a tab to a side of the split, and show it there */
export function moveTab(id: string, side: Side) {
  const rest = leftTabs.value.filter((t) => t !== id);
  leftTabs.value = side === "left" ? [...rest, id] : rest;
  activeTab.value = id;
}

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
export const SIDEBAR_WIDTH_DEFAULT = 280;
export const SIDEBAR_WIDTH_MIN = 200;
const SIDEBAR_WIDTH_KEY = "openfpga-sidebar-width";

function storedWidth(): number {
  try {
    const n = Number(window.localStorage.getItem(SIDEBAR_WIDTH_KEY));
    return Number.isFinite(n) && n >= SIDEBAR_WIDTH_MIN ? n : SIDEBAR_WIDTH_DEFAULT;
  } catch {
    return SIDEBAR_WIDTH_DEFAULT;
  }
}
/** The width of the sidebar in px: dragged by the person and kept in this browser */
export const sidebarWidth = signal(storedWidth());

export function setSidebarWidth(px: number) {
  const most = Math.max(SIDEBAR_WIDTH_MIN + 40, Math.floor(window.innerWidth * 0.6));
  sidebarWidth.value = Math.round(Math.min(most, Math.max(SIDEBAR_WIDTH_MIN, px)));
  try {
    window.localStorage.setItem(SIDEBAR_WIDTH_KEY, String(sidebarWidth.value));
  } catch {
    /* not kept */
  }
}
export const busy = signal(false);
export const banner = signal<{ kind: string; message: string; hint?: string | null } | null>(null);
export const consoleOpen = signal(false);
export const consoleLines = signal<ConsoleLine[]>([]);
export const consoleBusy = signal(false);
const CONSOLE_MODE_KEY = "openfpga-console-mode";

function storedMode(): ConsoleMode {
  try {
    return window.localStorage.getItem(CONSOLE_MODE_KEY) === "tcl" ? "tcl" : "shell";
  } catch {
    return "shell";
  }
}
/** What the console runs: commands of the shell, or Tcl scripts; kept in this browser */
export const consoleMode = signal<ConsoleMode>(storedMode());

export function setConsoleMode(mode: ConsoleMode) {
  consoleMode.value = mode;
  try {
    window.localStorage.setItem(CONSOLE_MODE_KEY, mode);
  } catch {
    /* not kept */
  }
}
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
  fileLine.value = null;
  loaded.value = false;
  welcomeChat.value = false;
  closedSession.value = false;
  sessionsOpen.value = false;
  sessionList.value = null;
  consoleHeight.value = CONSOLE_HEIGHT_DEFAULT;
  sidebarWidth.value = SIDEBAR_WIDTH_DEFAULT;
  sessionMenu.value = false;
  activeTab.value = CHAT;
  split.value = false;
  leftTabs.value = [CHAT];
  sideShown.value = { left: CHAT, right: null };
  busy.value = false;
  banner.value = null;
  consoleOpen.value = false;
  consoleLines.value = [];
  consoleBusy.value = false;
  consoleMode.value = "shell";
  sidebarOpen.value = wide();
  settingsOpen.value = false;
  uploading.value = false;
  packagesOpen.value = false;
  devicePackages.value = null;
  usage.value = null;
  hierarchy.value = { loading: false, session: null, design: null, data: null, error: null };
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

/** What runs a command of the views that the model sent (set by views/commands.ts, which the
 * store cannot import: the views import the store) */
let viewCommandHandler: (command: ViewCommandEvent) => void = () => undefined;
export function setViewCommandHandler(handler: (command: ViewCommandEvent) => void) {
  viewCommandHandler = handler;
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
    case "view_command":
      viewCommandHandler(event.command);
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
  if (view.value.session === null && closedSession.value) return true;
  return view.value.session === null && messages.value.length === 0;
}

/** The person closed the session: with none open the page goes back to the welcome screen (what
 * was said stays, and shows again with the next session) */
export const closedSession = signal(false);
// The tab that each side of the split showed last
effect(() => {
  const id = activeTab.value;
  const side = sideOf(id);
  if (sideShown.peek()[side] !== id) sideShown.value = { ...sideShown.peek(), [side]: id };
});

effect(() => {
  if (view.value.session !== null) closedSession.value = false;
});

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

/** The session actions in progress (a new session, a design added...) */
export const acting = signal(0);

/** Something is being done: an answer, a command of the console, an upload, a session action, a
 * stage that runs */
export const working = computed(
  () =>
    busy.value ||
    consoleBusy.value ||
    uploading.value ||
    acting.value > 0 ||
    hierarchy.value.loading ||
    view.value.latest.status === "running",
);

async function sessionAction(action: () => Promise<Answer>): Promise<boolean> {
  acting.value++;
  let ok = false;
  try {
    const answer = await action();
    ok = answer.ok;
    reportFailure(answer);
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  } finally {
    acting.value--;
  }
  await refreshView();
  return ok;
}

export const makeSession = (name?: string) => sessionAction(() => newSession(name));

/** The picker of device packages under the session buttons */
export const packagesOpen = signal(false);
export const devicePackages = signal<{ packages: DevicePackageInfo[]; problems: { name: string; message: string }[] } | null>(null);

export async function togglePackages() {
  packagesOpen.value = !packagesOpen.value;
  if (!packagesOpen.value) return;
  sessionsOpen.value = false;
  try {
    devicePackages.value = await fetchDevicePackages(); // read again each time: a package may have been added
  } catch (error) {
    packagesOpen.value = false;
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  }
}

/** The picker of sessions to open (the first screen): the live ones and the saved ones of the workspace */
export const sessionsOpen = signal(false);
export const sessionList = signal<{ live: SessionEntry[]; saved: SessionEntry[] } | null>(null);

export async function toggleSessions() {
  sessionsOpen.value = !sessionsOpen.value;
  if (!sessionsOpen.value) return;
  packagesOpen.value = false;
  try {
    sessionList.value = await fetchSessions(); // read again each time: a session may have been made or closed
  } catch (error) {
    sessionsOpen.value = false;
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  }
}

/** The question of the Session item of the menu: a new session, or open one */
export const sessionMenu = signal(false);

/** Open a session from the list: the full interface shows it */
export async function openSession(id: string) {
  sessionsOpen.value = false;
  sessionMenu.value = false;
  await sessionAction(() => openSessionById(id));
}

/** A session that reads the files of a device package and waits for the circuit */
export async function makeSessionFromPackage(name: string) {
  packagesOpen.value = false;
  return sessionAction(() => newSession(undefined, name));
}
/** Read the architectures of a package into the open session */
export async function loadPackageIntoSession(name: string) {
  packagesOpen.value = false;
  return sessionAction(() => applyDevicePackage(name));
}
/** The button of the Devices tab: the package into the open session if it is empty, else into a
 * new session; the tab closes when it worked */
export async function addPackage(name: string, intoOpen: boolean) {
  const ok = await (intoOpen ? loadPackageIntoSession(name) : makeSessionFromPackage(name));
  if (ok) closeTab("panel:devices");
}
export async function endSession() {
  await sessionAction(async () => {
    const answer = await closeSession();
    if (answer.ok) closedSession.value = true;
    return answer;
  });
}
/** Back to the last checkpoint, or to a named one */
export const restore = (checkpoint?: string) => sessionAction(() => restoreSession(checkpoint));
/** Save the flow of the session as a checkpoint */
export const saveSession = (name?: string) => sessionAction(() => saveCheckpoint(name));
/** Copy a design directory into the session. It is not synthesized: the run button of synthesis
 * (or the model) does that when asked */
export async function addDesign(name: string) {
  const ok = await sessionAction(() => addDesignToSession(name));
  if (ok) closeTab("panel:designs");
}

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

const NEW_FILE_NAME = /^[A-Za-z0-9][A-Za-z0-9._+-]{0,99}$/;

/** The file of the uploads of the session with this name, as the view lists it */
function uploadedNamed(name: string) {
  const all = [...(view.value.files.uploads ?? []), ...(view.value.files.created ?? [])];
  return all.find((f) => f.name === name || f.name.endsWith(`/${name}`));
}

/** Make a file in the session from the name that the person gave, and open it to write. The
 * server refuses an empty file, so it starts as one line break. A name that is taken opens the
 * file that is there: nothing is replaced */
export async function createFile(typed: string) {
  const name = typed.trim();
  if (!name) return;
  const extension = name.includes(".") ? name.slice(name.lastIndexOf(".")).toLowerCase() : "";
  if (!NEW_FILE_NAME.test(name) || name.includes("..")) {
    banner.value = {
      kind: "invalid_name",
      message: `'${name}' is not a name for a file`,
      hint: "Use letters, digits and . _ + - (at most 100), with no folder",
    };
    return;
  }
  if (!UPLOAD_EXTENSIONS.includes(extension)) {
    banner.value = {
      kind: "invalid_type",
      message: `${name} is not a kind of file that a flow reads`,
      hint: `Accepted: ${UPLOAD_EXTENSIONS.join(", ")}`,
    };
    return;
  }
  const taken = uploadedNamed(name);
  if (!taken) {
    await uploadFiles([new File(["\n"], name, { type: "text/plain" })]);
  }
  const made = taken ?? uploadedNamed(name); // none if it failed: the banner says why
  if (made) await openFile(made.name);
}

/** Take a file out of the session: an upload, a file that the model wrote, or one of the copy of
 * a design */
export async function removeUpload(name: string, folder: FileFolder = "uploads") {
  banner.value = null;
  try {
    const answer = await removeUploadedFile(name, folder);
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
  const side = sideOf(id);
  tabs.value = rest;
  leftTabs.value = leftTabs.value.filter((t) => t !== id);
  if (activeTab.value === id) {
    // The side it was on shows another of its tabs, if it has one
    const same = split.value ? tabsOfSide(side) : [];
    activeTab.value = same.length ? same[same.length - 1] : rest.length ? rest[rest.length - 1].id : CHAT;
  }
  if (!rest.length) split.value = false;
}

export const fileTabId = (name: string) => `file:${name}`;

/** The title of the tab of the report of a stage */
export const REPORT_TITLES: Record<string, string> = {
  device: "Device report",
  synthesis: "Synthesis report",
  pack: "Packing report",
  place: "Placement report",
  route: "Routing report",
  analysis: "Timing report",
};

/** The HTML report of a stage, in a tab of its own (one tab for each stage) */
export function openReport(stage: string) {
  const id = `report:${stage}`;
  if (!tabs.value.some((t) => t.id === id)) {
    tabs.value = [...tabs.value, { id, title: REPORT_TITLES[stage] ?? `${stage} report`, pinned: false, unread: false, report: stage }];
  }
  activeTab.value = id;
}

/** The titles of the tabs of the lists */
export const PANEL_TITLES: Record<Panel, string> = {
  devices: "Devices",
  designs: "Designs",
  hierarchy: "Hierarchy",
  floorplan: "Placement",
  pins: "Floorplan",
  timing: "Timing",
  netlist: "Netlist",
  clusters: "Clusters",
  routing: "Routing",
};

/** A list in a tab of its own: the device packages (read again each time, a package may have been
 * added) or the design directories (they come with the view) */
export function openPanel(panel: Panel) {
  const id = `panel:${panel}`;
  if (!tabs.value.some((t) => t.id === id)) {
    tabs.value = [...tabs.value, { id, title: PANEL_TITLES[panel], pinned: false, unread: false, panel }];
  }
  activeTab.value = id;
  if (panel === "devices") void loadDevicePackages();
}

/** The hierarchy of the design of the session: elaborated again each time it is asked (the RTL
 * may have been edited) */
export const hierarchy = signal<{
  loading: boolean;
  session: string | null; // the session that it is of
  design: string | null; // and the design (the one that the session added last)
  data: Hierarchy | null;
  error: { message: string; hint?: string | null } | null;
}>({ loading: false, session: null, design: null, data: null, error: null });

export async function loadHierarchy() {
  if (hierarchy.value.loading) return;
  const session = view.value.session?.id ?? null;
  const design = view.value.synthesis?.design ?? null;
  hierarchy.value = { ...hierarchy.value, loading: true };
  try {
    const answer = await fetchHierarchy();
    if (answer.ok) hierarchy.value = { loading: false, session, design, data: answer.data as Hierarchy, error: null };
    else {
      const d = answer.diagnostics?.[0];
      hierarchy.value = { loading: false, session, design, data: null, error: { message: d?.message ?? "The hierarchy failed", hint: d?.hint } };
    }
  } catch (error) {
    hierarchy.value = { loading: false, session, design, data: null, error: { message: String(error), hint: "Is the app still running?" } };
  }
}

export async function loadDevicePackages() {
  try {
    devicePackages.value = await fetchDevicePackages();
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  }
}

/** Where the report of a stage is: the version changes after each run, so the frame loads it again */
export const reportUrl = (stage: string, version = "") =>
  `/api/report?stage=${encodeURIComponent(stage)}${version ? `&v=${encodeURIComponent(version)}` : ""}`;

/** Open a file in a tab of the viewer (or go to it), and read it again: a result may have changed.
 * With a line, the editor shows that line */
export async function openFile(name: string, line?: number) {
  fileLine.value = line ? { name, line } : null;
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
      const data = answer.data as { text: string; bytes: number; truncated: boolean; editable?: boolean; copyable?: boolean };
      next = { status: "ready", text: data.text, bytes: data.bytes, truncated: data.truncated, message: "", editable: data.editable, copyable: data.copyable };
    } else {
      const d = answer.diagnostics?.[0];
      next = { status: "failed", text: "", bytes: 0, truncated: false, message: d ? d.message + (d.hint ? ` (${d.hint})` : "") : "The file could not be read" };
    }
  } catch (error) {
    next = { status: "failed", text: "", bytes: 0, truncated: false, message: String(error) };
  }
  if (name in openFiles.value) openFiles.value = { ...openFiles.value, [name]: next }; // not if the tab was closed
}

/** Save the text that the person edited. The answer says if it worked, why not, and whether the
 * session had read the file (then the stages that used it are out of date) */
export async function saveFile(name: string, text: string): Promise<{ ok: boolean; message: string; inUse: boolean }> {
  try {
    const answer = await saveFileText(name, text);
    if (!answer.ok) {
      const d = answer.diagnostics?.[0];
      return { ok: false, message: d ? d.message + (d.hint ? ` (${d.hint})` : "") : "The file was not saved", inUse: false };
    }
    const data = answer.data as { bytes: number; in_use?: boolean };
    const before = openFiles.value[name];
    if (before) openFiles.value = { ...openFiles.value, [name]: { ...before, text, bytes: data.bytes, truncated: false } };
    await refreshView();
    return { ok: true, message: "", inUse: !!data.in_use };
  } catch (error) {
    return { ok: false, message: String(error), inUse: false };
  }
}

/** Make a copy of a file that cannot be edited, in the uploads of the session, and open the copy */
export async function editCopy(name: string) {
  try {
    const answer = await copyFileToSession(name);
    if (!answer.ok) {
      reportFailure(answer);
      return;
    }
    banner.value = null;
    const copy = (answer.data as { name: string }).name;
    await refreshView();
    await openFile(copy);
  } catch (error) {
    banner.value = { kind: "server", message: String(error), hint: "Is the app still running?" };
  }
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
  const mode = consoleMode.value;
  // A Tcl script keeps its lines
  const text = mode === "tcl" ? command.replace(/^\s*\n|\s+$/g, "") : command.trim();
  if (!text.trim() || consoleBusy.value) return;
  consoleLines.value = [...consoleLines.value, { kind: "input", text, mode }];
  consoleBusy.value = true;
  try {
    const answer = await runConsole(text, mode);
    if (answer.output) {
      consoleLines.value = [...consoleLines.value, { kind: answer.ok ? "output" : "error", text: answer.output }];
    }
    if (answer.ok && answer.result) {
      consoleLines.value = [...consoleLines.value, { kind: "output", text: answer.result }];
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
