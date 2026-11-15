import { beforeEach, describe, expect, it, vi } from "vitest";
import { parseEvents, AppEvent } from "./api";
import {
  CHAT,
  activeTab,
  applyEvent,
  calls,
  closeTab,
  consoleLines,
  messages,
  nextTab,
  openResult,
  pinTab,
  resetStore,
  split,
  tabTitle,
  tabs,
} from "./store";

const started = (callId: string, tool: string, args: Record<string, unknown> = {}): AppEvent => ({
  type: "tool_call_started",
  id: "m1",
  call_id: callId,
  tool,
  arguments: args,
});
const finished = (callId: string, ok = true, result: Record<string, unknown> = { ok }): AppEvent => ({
  type: "tool_call_finished",
  id: "m1",
  call_id: callId,
  ok,
  seconds: 1.5,
  truncated: false,
  result: { ok, ...result },
});

beforeEach(() => {
  resetStore();
  vi.stubGlobal("fetch", vi.fn(() => Promise.reject(new Error("no server"))));
});

describe("events", () => {
  it("builds a message from deltas, with the calls where they came", () => {
    applyEvent({ type: "message_start", id: "m1", role: "assistant" });
    applyEvent({ type: "text_delta", id: "m1", text: "I'll " });
    applyEvent({ type: "text_delta", id: "m1", text: "run it." });
    applyEvent(started("c1", "run_stage", { stage: "pack" }));
    applyEvent({ type: "text_delta", id: "m1", text: "Done." });
    expect(messages.value[0].parts).toEqual([
      { kind: "text", text: "I'll run it." },
      { kind: "call", callId: "c1" },
      { kind: "text", text: "Done." },
    ]);
  });

  it("follows a call from running to done, failed or cancelled", () => {
    applyEvent({ type: "message_start", id: "m1", role: "assistant" });
    for (const id of ["a", "b", "c"]) applyEvent(started(id, "run_stage"));
    expect(calls.value.a.state).toBe("running");
    applyEvent(finished("a"));
    applyEvent(finished("b", false, { diagnostics: [{ kind: "router_failed", message: "no" }] }));
    applyEvent(finished("c", false, { diagnostics: [{ kind: "cancelled", message: "Cancelled" }] }));
    expect([calls.value.a.state, calls.value.b.state, calls.value.c.state]).toEqual(["done", "failed", "cancelled"]);
    expect(calls.value.a.seconds).toBe(1.5);
  });

  it("shows only the calls that change the flow in the console, as the model's lines", () => {
    applyEvent({ type: "message_start", id: "m1", role: "assistant" });
    applyEvent(started("1", "run_stage", { session: "s1", stage: "place" }));
    applyEvent(started("2", "get_timing", { session: "s1" }));
    applyEvent(started("3", "set_option", { session: "s1", name: "place.seed", value: 3 }));
    expect(consoleLines.value).toEqual([
      { kind: "model", text: "run_stage stage=place" },
      { kind: "model", text: "set_option name=place.seed value=3" },
    ]);
  });

  it("makes an error a banner", async () => {
    const { banner } = await import("./store");
    applyEvent({ type: "error", kind: "busy", message: "A message is being answered", hint: "Wait" });
    expect(banner.value).toEqual({ kind: "busy", message: "A message is being answered", hint: "Wait" });
  });
});

describe("the stream", () => {
  it("reads events that arrive in pieces", () => {
    const buffer = { text: "" };
    const one = 'event: text_delta\ndata: {"type":"text_delta","schema_version":1,"id":"m1","text":"a"}\n\n';
    const two = 'event: done\ndata: {"type":"done","schema_version":1,"id":"m1","cancelled":false}\n\n';
    const found: string[] = [];
    for (const piece of [one.slice(0, 20), one.slice(20) + two.slice(0, 10), two.slice(10)]) {
      buffer.text += piece;
      for (const e of parseEvents(buffer)) found.push(e.type);
    }
    expect(found).toEqual(["text_delta", "done"]);
    expect(buffer.text).toBe("");
  });
});

describe("the tabs of the results", () => {
  const give = (callId: string, tool: string, args: Record<string, unknown> = {}) => {
    applyEvent({ type: "message_start", id: "m1", role: "assistant" });
    applyEvent(started(callId, tool, args));
    applyEvent(finished(callId));
  };

  it("has none until a result is opened, and names a tab by the tool and its main argument", () => {
    give("a", "get_timing");
    give("b", "get_net", { session: "s1", net: "n3" });
    expect(tabs.value).toEqual([]);
    expect(tabTitle(calls.value.b)).toBe("get_net n3");
    openResult("a");
    expect(tabs.value.map((t) => t.title)).toEqual(["get_timing"]);
    expect(activeTab.value).toBe("a");
  });

  it("opens an open result again instead of making a second tab", () => {
    give("a", "get_timing");
    openResult("a");
    openResult("a");
    expect(tabs.value).toHaveLength(1);
  });

  it("replaces the tab of the same tool that is not pinned, and keeps one that is", () => {
    give("a", "get_timing");
    give("b", "get_timing");
    give("c", "get_timing");
    openResult("a");
    openResult("b");
    expect(tabs.value.map((t) => t.id)).toEqual(["b"]);
    pinTab("b");
    openResult("c");
    expect(tabs.value.map((t) => t.id)).toEqual(["b", "c"]);
  });

  it("does not take you from the chat when the page opens a tab, and marks it", () => {
    give("a", "get_timing");
    openResult("a", false);
    expect(activeTab.value).toBe(CHAT);
    expect(tabs.value[0].unread).toBe(true);
    openResult("a", true);
    expect(tabs.value[0].unread).toBe(false);
  });

  it("goes back to the chat and to one view when the last tab is closed", () => {
    give("a", "get_timing");
    give("b", "get_utilization");
    openResult("a");
    openResult("b");
    split.value = true;
    closeTab("b");
    expect(activeTab.value).toBe("a");
    closeTab("a");
    expect(activeTab.value).toBe(CHAT);
    expect(split.value).toBe(false);
    expect(tabs.value).toEqual([]);
  });

  it("goes round the tabs, the chat first", () => {
    give("a", "get_timing");
    give("b", "get_utilization");
    openResult("a");
    openResult("b");
    activeTab.value = CHAT;
    nextTab(1);
    expect(activeTab.value).toBe("a");
    nextTab(1);
    nextTab(1);
    expect(activeTab.value).toBe(CHAT);
    nextTab(-1);
    expect(activeTab.value).toBe("b");
  });
});
