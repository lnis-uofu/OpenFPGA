import { fireEvent, render, screen, waitFor, within } from "@testing-library/preact";
import { beforeEach, describe, expect, it, vi } from "vitest";
import { App, handleKey } from "./App";
import { AppEvent, SessionView } from "./api";
import { Console } from "./components/Console";
import { Sidebar } from "./components/Sidebar";
import { CallGroup, ToolCard } from "./components/ToolCard";
import { TopBar } from "./components/TopBar";
import { Workspace } from "./components/Workspace";
import {
  CHAT,
  activeTab,
  applyEvent,
  config,
  consoleOpen,
  emptyView,
  messages,
  openResult,
  resetStore,
  settingsOpen,
  sidebarOpen,
  split,
  tabs,
  view,
} from "./store";

const started = (callId: string, tool: string, args: Record<string, unknown> = {}): AppEvent => ({
  type: "tool_call_started",
  id: "m1",
  call_id: callId,
  tool,
  arguments: args,
});
const finished = (callId: string, ok: boolean, result: Record<string, unknown>, truncated = false): AppEvent => ({
  type: "tool_call_finished",
  id: "m1",
  call_id: callId,
  ok,
  seconds: 2.1,
  truncated,
  result: { ok, ...result },
});
const give = (callId: string, tool: string, ok = true, result: Record<string, unknown> = {}, args = {}) => {
  if (!messages.value.length) applyEvent({ type: "message_start", id: "m1", role: "assistant" });
  applyEvent(started(callId, tool, args));
  applyEvent(finished(callId, ok, result));
};

const filled = (): SessionView => ({
  ...emptyView(),
  session: { id: "s1", name: "run1", worker: "running", ended: null, idle_s: 0 },
  latest: { status: "reached", text: "s1 · PLACED ✔ 2.1 s" },
  stages: emptyView().stages.map((s, i) => ({
    ...s,
    status: i < 3 ? ("reached" as const) : ("not_reached" as const),
    seconds: i === 2 ? 2.1 : null,
  })),
  files: {
    inputs: [
      { label: "vpr arch", name: "k6_frac_N10_tileable_40nm_with_a_long_name.xml", extra: "layout 2x2" },
      { label: "circuit", name: "routing_test.blif", extra: null },
    ],
    constraints: ["chan width 24"],
    results: ["routing_test.net"],
  },
  options_changed: [{ name: "place.seed", value: 3 }],
  checkpoints: [{ name: "last", kind: "after the last stage", stages: ["DEVICE", "PACKED", "PLACED"] }],
});

beforeEach(() => {
  resetStore();
  vi.stubGlobal("fetch", vi.fn(() => Promise.reject(new Error("no server"))));
});

describe("the top bar", () => {
  it("shows only the latest state and the model", () => {
    view.value = filled();
    config.value = { schema_version: 1, console: false, model: { endpoint: "http://127.0.0.1:11434/v1", name: "llama3.1" } };
    render(<TopBar />);
    expect(screen.getByRole("status").textContent).toContain("s1 · PLACED ✔ 2.1 s");
    expect(screen.getByText(/llama3.1/)).toBeTruthy();
    expect(screen.queryByText("ANALYZED")).toBeNull();
  });

  it("says demo when there is no model", () => {
    render(<TopBar />);
    expect(screen.getByText(/demo/)).toBeTruthy();
  });

  it("opens the settings", () => {
    render(<TopBar />);
    fireEvent.click(screen.getByLabelText("Settings"));
    expect(settingsOpen.value).toBe(true);
  });
});

describe("the sidebar", () => {
  it("has every section, with nothing in it, before there is a session", () => {
    render(<Sidebar />);
    for (const title of ["Session", "Stages", "Design files", "Checkpoints"]) expect(screen.getByText(title)).toBeTruthy();
    expect(screen.getByText("(no session)")).toBeTruthy();
    expect(screen.getAllByText("(none)").length).toBeGreaterThanOrEqual(4);
    expect(screen.getAllByRole("listitem")).toHaveLength(5);
  });

  it("shows the stages, the files and the checkpoints of a session", () => {
    view.value = filled();
    render(<Sidebar />);
    expect(screen.getByText("worker running")).toBeTruthy();
    const stages = screen.getAllByRole("listitem").map((li) => li.textContent ?? "");
    expect(stages[0]).toContain("✔");
    expect(stages[2]).toContain("2.1 s");
    expect(stages[3]).toContain("○");
    expect(screen.getByText("layout 2x2")).toBeTruthy();
    expect(screen.getByText("chan width 24")).toBeTruthy();
    expect(screen.getByText("routing_test.net")).toBeTruthy();
    expect(screen.getByText(/last/)).toBeTruthy();
  });

  it("cuts a long name in the middle and keeps the whole one in the title", () => {
    view.value = filled();
    render(<Sidebar />);
    const name = "k6_frac_N10_tileable_40nm_with_a_long_name.xml";
    const button = screen.getByTitle(name);
    expect(button.textContent).toContain("…");
    expect(button.textContent!.length).toBeLessThan(name.length);
  });

  it("folds the options that changed", () => {
    view.value = filled();
    render(<Sidebar />);
    expect(screen.queryByText("place.seed = 3")).toBeNull();
    fireEvent.click(screen.getByText(/Options changed \(1\)/));
    expect(screen.getByText("place.seed = 3")).toBeTruthy();
  });

  it("says that a worker ended, and why", () => {
    view.value = { ...filled(), session: { id: "s1", name: "s1", worker: "ended", ended: "killed", idle_s: 0 } };
    render(<Sidebar />);
    expect(screen.getByText(/worker ended: killed/)).toBeTruthy();
  });
});

describe("a tool card", () => {
  it("is one line until it is opened", () => {
    give("a", "run_stage", true, { data: { stage: "place" } }, { session: "s1", stage: "place" });
    render(<ToolCard callId="a" />);
    expect(screen.getByText("run_stage")).toBeTruthy();
    expect(screen.getByText("stage=place")).toBeTruthy();
    expect(screen.getByText("2.1 s")).toBeTruthy();
    expect(screen.queryByText("Arguments")).toBeNull();
    fireEvent.click(screen.getByRole("button", { name: /run_stage/ }));
    expect(screen.getByText("Arguments")).toBeTruthy();
    expect(screen.getByText("Open in tab")).toBeTruthy();
  });

  it("shows the diagnostics and the hint of a failure", () => {
    give("a", "run_stage", false, {
      diagnostics: [{ kind: "router_failed", message: "Routing failed", hint: "a wider channel" }],
    });
    render(<ToolCard callId="a" startOpen />);
    expect(screen.getByText("router_failed")).toBeTruthy();
    expect(screen.getByText(/Try: a wider channel/)).toBeTruthy();
  });

  it("says when a result was cut, and how to get the rest", () => {
    applyEvent({ type: "message_start", id: "m1", role: "assistant" });
    applyEvent(started("a", "get_timing", { paths: 3 }));
    applyEvent(finished("a", true, { more: "Call get_timing again with fewer paths." }, true));
    render(<ToolCard callId="a" startOpen />);
    expect(screen.getByText(/⚠ Call get_timing again with fewer paths/)).toBeTruthy();
  });

  it("can cancel a call that runs", () => {
    applyEvent({ type: "message_start", id: "m1", role: "assistant" });
    applyEvent(started("a", "run_stage", { stage: "route" }));
    render(<ToolCard callId="a" />);
    expect(screen.getAllByText("running").length).toBeGreaterThan(0);
    fireEvent.click(screen.getByText("■ Cancel call"));
    expect((fetch as unknown as ReturnType<typeof vi.fn>).mock.calls[0][0]).toBe("/api/cancel");
  });

  it("cuts a long result and shows it all on request", () => {
    give("a", "find", true, { data: { text: "x".repeat(2000) } });
    render(<ToolCard callId="a" startOpen />);
    expect(screen.getByText("Show all")).toBeTruthy();
    fireEvent.click(screen.getByText("Show all"));
    expect(screen.getByText("Show less")).toBeTruthy();
  });

  it("opens its result in a tab", () => {
    give("a", "get_timing");
    render(<ToolCard callId="a" startOpen />);
    fireEvent.click(screen.getByText("Open in tab"));
    expect(tabs.value.map((t) => t.id)).toEqual(["a"]);
    expect(activeTab.value).toBe("a");
  });

  it("folds more than three calls in a row into one line", () => {
    for (const id of ["a", "b", "c", "d"]) give(id, "run_stage");
    render(<CallGroup callIds={["a", "b", "c", "d"]} />);
    expect(screen.getByText(/4 calls/)).toBeTruthy();
    expect(screen.queryAllByText("run_stage")).toHaveLength(0);
    fireEvent.click(screen.getByText(/4 calls/));
    expect(screen.getAllByText("run_stage")).toHaveLength(4);
  });

  it("says how many of a group failed", () => {
    for (const id of ["a", "b", "c"]) give(id, "run_stage");
    give("d", "run_stage", false, { diagnostics: [{ kind: "x", message: "m" }] });
    render(<CallGroup callIds={["a", "b", "c", "d"]} />);
    expect(screen.getByText(/4 calls, 1 failed/)).toBeTruthy();
  });
});

describe("the centre", () => {
  it("is the chat alone, with no tab bar, until there is a result", () => {
    render(<Workspace />);
    expect(screen.queryByRole("tablist")).toBeNull();
    expect(screen.getByLabelText("Chat")).toBeTruthy();
  });

  it("gets a bar of tabs with Chat first, and Chat cannot be closed", () => {
    give("a", "get_timing");
    openResult("a");
    render(<Workspace />);
    const tabsShown = screen.getAllByRole("tab").map((t) => t.textContent);
    expect(tabsShown[0]).toBe("Chat");
    expect(tabsShown[1]).toContain("get_timing");
    expect(screen.queryByLabelText("Close Chat")).toBeNull();
    expect(screen.getByLabelText("Close get_timing")).toBeTruthy();
    expect(screen.getByText(/Work in progress/)).toBeTruthy();
  });

  it("goes back to the chat, and to no bar, when the last tab is closed", () => {
    give("a", "get_timing");
    openResult("a");
    render(<Workspace />);
    fireEvent.click(screen.getByLabelText("Close get_timing"));
    expect(screen.queryByRole("tablist")).toBeNull();
    expect(screen.getByLabelText("Chat")).toBeTruthy();
  });

  it("shows the chat and a result side by side when split", () => {
    give("a", "get_timing");
    openResult("a");
    render(<Workspace />);
    fireEvent.click(screen.getByText("⫴ split"));
    expect(split.value).toBe(true);
    expect(screen.getByLabelText("Chat")).toBeTruthy();
    expect(screen.getByLabelText("Result of get_timing")).toBeTruthy();
    fireEvent.click(screen.getByText("⫴ one view"));
    expect(split.value).toBe(false);
  });

  it("pins a result tab", () => {
    give("a", "get_timing");
    openResult("a");
    render(<Workspace />);
    fireEvent.click(screen.getByText("Pin"));
    expect(tabs.value[0].pinned).toBe(true);
    expect(screen.getByText("Unpin")).toBeTruthy();
  });
});

describe("the chat", () => {
  const stream = (events: AppEvent[]) => {
    const text = events.map((e) => `event: ${e.type}\ndata: ${JSON.stringify(e)}\n\n`).join("");
    return new Response(
      new ReadableStream({
        start(controller) {
          controller.enqueue(new TextEncoder().encode(text));
          controller.close();
        },
      }),
      { status: 200, headers: { "Content-Type": "text/event-stream" } },
    );
  };

  it("sends on Enter, not on Shift+Enter, and shows the answer", async () => {
    const fetchMock = vi.fn((url: string, _init?: RequestInit) => {
      if (url === "/api/chat")
        return Promise.resolve(
          stream([
            { type: "message_start", id: "m1", role: "assistant" },
            { type: "text_delta", id: "m1", text: "Hello." },
            { type: "done", id: "m1", cancelled: false },
          ]),
        );
      return Promise.resolve(new Response(JSON.stringify(emptyView()), { status: 200 }));
    });
    vi.stubGlobal("fetch", fetchMock);
    render(<Workspace />);
    const box = screen.getByLabelText("Message");
    fireEvent.input(box, { target: { value: "hi" } });
    fireEvent.keyDown(box, { key: "Enter", shiftKey: true });
    expect(fetchMock).not.toHaveBeenCalled();
    fireEvent.keyDown(box, { key: "Enter" });
    await waitFor(() => expect(screen.getByText("Hello.")).toBeTruthy());
    expect(screen.getByText("hi")).toBeTruthy();
    const sent = fetchMock.mock.calls.find((c) => c[0] === "/api/chat")!;
    expect(JSON.parse((sent[1] as RequestInit).body as string)).toEqual({ message: "hi" });
    expect((box as HTMLTextAreaElement).value).toBe("");
  });

  it("does not send an empty message", () => {
    render(<Workspace />);
    expect((screen.getByText("Send") as HTMLButtonElement).disabled).toBe(true);
  });

  it("says when the server cannot be reached", async () => {
    render(<Workspace />);
    fireEvent.input(screen.getByLabelText("Message"), { target: { value: "hi" } });
    fireEvent.keyDown(screen.getByLabelText("Message"), { key: "Enter" });
    await waitFor(() => expect(screen.getByRole("alert").textContent).toContain("no server"));
  });
});

describe("the console", () => {
  it("is a line until it is opened, and says when it is off", () => {
    render(<Console />);
    expect(screen.getByText(/Console \(off: start with --console\)/)).toBeTruthy();
    fireEvent.click(screen.getByText(/Console/));
    expect(consoleOpen.value).toBe(true);
    expect(screen.getByText(/start the app with --console/)).toBeTruthy();
  });

  it("runs a command and shows the answer, and recalls it with the arrow", async () => {
    config.value = { schema_version: 1, console: true, model: { endpoint: null, name: null } };
    consoleOpen.value = true;
    const fetchMock = vi.fn(() =>
      Promise.resolve(new Response(JSON.stringify({ ok: true, output: "reached: DEVICE", state_changed: false }), { status: 200 })),
    );
    vi.stubGlobal("fetch", fetchMock);
    render(<Console />);
    const input = screen.getByLabelText("Command") as HTMLInputElement;
    fireEvent.input(input, { target: { value: "report_session" } });
    fireEvent.keyDown(input, { key: "Enter" });
    await waitFor(() => expect(screen.getByText("reached: DEVICE")).toBeTruthy());
    expect(screen.getByText(/openfpga> report_session/)).toBeTruthy();
    fireEvent.keyDown(input, { key: "ArrowUp" });
    await waitFor(() => expect(input.value).toBe("report_session"));
    fireEvent.keyDown(input, { key: "ArrowDown" });
    await waitFor(() => expect(input.value).toBe(""));
  });

  it("shows an error of the shell as an error", async () => {
    config.value = { schema_version: 1, console: true, model: { endpoint: null, name: null } };
    consoleOpen.value = true;
    vi.stubGlobal(
      "fetch",
      vi.fn(() => Promise.resolve(new Response(JSON.stringify({ ok: false, output: "Error: nope", state_changed: false }), { status: 200 }))),
    );
    render(<Console />);
    fireEvent.input(screen.getByLabelText("Command"), { target: { value: "bad" } });
    fireEvent.keyDown(screen.getByLabelText("Command"), { key: "Enter" });
    const line = await screen.findByText("Error: nope");
    expect(line.className).toContain("line-error");
  });
});

describe("the keys", () => {
  const key = (k: string, extra: Partial<KeyboardEventInit> = {}) => handleKey(new KeyboardEvent("keydown", { key: k, ctrlKey: true, ...extra }));

  it("toggles the console and the sidebar", () => {
    expect(key("`")).toBe(true);
    expect(consoleOpen.value).toBe(true);
    const before = sidebarOpen.value;
    key("b");
    expect(sidebarOpen.value).toBe(!before);
  });

  it("moves between tabs and closes a result tab but not the chat", () => {
    give("a", "get_timing");
    give("b", "get_utilization");
    openResult("a");
    openResult("b");
    activeTab.value = CHAT;
    expect(key("j")).toBe(true);
    expect(activeTab.value).toBe("a");
    expect(key("j", { shiftKey: true })).toBe(true);
    expect(activeTab.value).toBe(CHAT);
    expect(key("w")).toBe(false);
    activeTab.value = "a";
    expect(key("w")).toBe(true);
    expect(tabs.value.map((t) => t.id)).toEqual(["b"]);
  });

  it("closes the settings with Escape and leaves other keys alone", () => {
    settingsOpen.value = true;
    expect(handleKey(new KeyboardEvent("keydown", { key: "Escape" }))).toBe(true);
    expect(settingsOpen.value).toBe(false);
    expect(handleKey(new KeyboardEvent("keydown", { key: "a" }))).toBe(false);
    expect(key("z")).toBe(false);
  });
});

describe("the page", () => {
  it("loads the session and the config from the server", async () => {
    const fetchMock = vi.fn((url: string) =>
      Promise.resolve(
        new Response(
          JSON.stringify(
            url === "/api/config"
              ? { schema_version: 1, console: false, model: { endpoint: null, name: "demo-model" } }
              : filled(),
          ),
          { status: 200 },
        ),
      ),
    );
    vi.stubGlobal("fetch", fetchMock);
    render(<App />);
    await waitFor(() => expect(screen.getByText("routing_test.net")).toBeTruthy());
    expect(within(screen.getByRole("banner")).getByText(/s1 · PLACED/)).toBeTruthy();
    expect(screen.getByText(/demo-model/)).toBeTruthy();
    fireEvent.click(screen.getByLabelText("Settings"));
    expect(screen.getByRole("dialog")).toBeTruthy();
    expect(screen.getByText(/Not connected in this version/)).toBeTruthy();
  });
});
