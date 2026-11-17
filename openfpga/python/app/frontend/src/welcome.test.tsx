import { fireEvent, render, screen, waitFor } from "@testing-library/preact";
import { beforeEach, describe, expect, it, vi } from "vitest";
import { App } from "./App";
import { SessionView } from "./api";
import { emptyView, resetStore } from "./store";

const real = { schema_version: 1, console: true, backend: "session", model: { endpoint: null, name: null } };
const withModel = { ...real, model: { endpoint: "http://127.0.0.1:8081/v1", name: "qwen" } };
const demo = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
const opened = (): SessionView => ({
  ...emptyView(),
  session: { id: "s1", name: "s1", worker: "running", ended: null, idle_s: 0 },
  latest: { status: "none", text: "s1 · opened" },
});

/** A server: the configuration, the view (which a call of the sessions changes) and the packages */
function server(config: unknown, calls: { url: string; body?: string }[] = []) {
  let current: SessionView = emptyView();
  chat.open = () => {
    current = opened();
  };
  const reply = (body: unknown, status = 200) => Promise.resolve(new Response(JSON.stringify(body), { status }));
  const fetchMock = vi.fn((url: string, init?: RequestInit) => {
    calls.push({ url, body: typeof init?.body === "string" ? init.body : undefined });
    if (url === "/api/config") return reply(config);
    if (url === "/api/session") return reply(current);
    if (url === "/api/chat") {
      return Promise.resolve(
        new Response(
          new ReadableStream<Uint8Array>({
            start(controller) {
              chat.controller = controller;
            },
          }),
        ),
      );
    }
    if (url === "/api/session/new") {
      current = opened();
      return reply({ ok: true, data: { session: "s1" } });
    }
    if (url === "/api/session/close") {
      current = emptyView();
      return reply({ ok: true, data: {} });
    }
    if (url === "/api/device-packages") {
      return reply({
        ok: true,
        data: {
          packages: [{ name: "k6_frac_n10_2x2", version: "0.1.0", description: "K6 logic only", layout: "2x2", lut_size: 6, tags: [] }],
          problems: [],
        },
      });
    }
    return Promise.reject(new Error("no server for " + url));
  });
  vi.stubGlobal("fetch", fetchMock);
  return fetchMock;
}

/** The answer to a message, which a test writes event by event, and the session that the model opens */
const chat = {
  controller: null as ReadableStreamDefaultController<Uint8Array> | null,
  open: () => undefined as void,
  send(event: Record<string, unknown>) {
    chat.controller?.enqueue(new TextEncoder().encode(`data: ${JSON.stringify({ schema_version: 1, ...event })}\n\n`));
  },
  end() {
    chat.send({ type: "done", id: "m1", cancelled: false });
    chat.controller?.close();
  },
};
const say = async (text: string) => {
  const box = await screen.findByRole("textbox", { name: "Message" });
  fireEvent.input(box, { target: { value: text } });
  fireEvent.keyDown(box, { key: "Enter" });
  await waitFor(() => expect(chat.controller).not.toBeNull());
};

const sidebar = () => screen.queryByLabelText("Session and design files");

beforeEach(() => {
  resetStore();
  chat.controller = null;
});

describe("the welcome screen", () => {
  it("is the first screen of real sessions: a prompt in the middle, two buttons, nothing else", async () => {
    server(real);
    render(<App />);
    await screen.findByRole("textbox", { name: "Message" });
    expect(screen.getByText("Create session")).toBeTruthy();
    expect(screen.getByText("Create session from device package…")).toBeTruthy();
    expect(sidebar()).toBeNull();
    expect(screen.queryByText(/▸ Console/)).toBeNull();
    expect(screen.getByText(/No model is connected/)).toBeTruthy();
  });

  it("shows nothing of the session until the configuration has come", async () => {
    let release: (r: Response) => void = () => undefined;
    vi.stubGlobal("fetch", vi.fn(() => new Promise<Response>((r) => (release = r))));
    render(<App />);
    expect(screen.queryByRole("textbox", { name: "Message" })).toBeNull();
    expect(sidebar()).toBeNull();
    release(new Response("{}", { status: 500 }));
  });

  it("is not the first screen of the recorded demo", async () => {
    server(demo);
    render(<App />);
    await waitFor(() => expect(sidebar()).toBeTruthy());
    expect(screen.queryByText("Create session")).toBeNull();
  });

  it("gives the full interface when a session is created", async () => {
    const calls: { url: string; body?: string }[] = [];
    server(real, calls);
    render(<App />);
    fireEvent.click(await screen.findByText("Create session"));
    await waitFor(() => expect(sidebar()).toBeTruthy());
    expect(calls.some((c) => c.url === "/api/session/new" && c.body === "{}")).toBe(true);
    expect(screen.queryByText("Create session from device package…")).toBeNull();
    expect(screen.getByText(/▸ Console/)).toBeTruthy();
  });

  it("creates a session from a device package that is chosen", async () => {
    const calls: { url: string; body?: string }[] = [];
    server(real, calls);
    render(<App />);
    fireEvent.click(await screen.findByText("Create session from device package…"));
    await screen.findByText("k6_frac_n10_2x2");
    expect(screen.getByText("K6 logic only")).toBeTruthy();
    expect(sidebar()).toBeNull(); // still the welcome screen
    fireEvent.click(screen.getByText("Create session", { selector: ".package button" }));
    await waitFor(() => expect(sidebar()).toBeTruthy());
    expect(calls.find((c) => c.url === "/api/session/new")?.body).toBe(JSON.stringify({ device_package: "k6_frac_n10_2x2" }));
  });

  it("shows the answer to a message in a small window below the prompt, not in the full interface", async () => {
    server(withModel);
    render(<App />);
    await say("Run the flow");
    chat.send({ type: "message_start", id: "m1", role: "assistant" });
    chat.send({ type: "text_delta", id: "m1", text: "I will open a session" });
    const answer = await screen.findByRole("region", { name: "Answer" });
    await waitFor(() => expect(answer.textContent).toContain("I will open a session"));
    expect(answer.textContent).toContain("Run the flow"); // what was asked
    expect(sidebar()).toBeNull(); // still the first screen
    expect(screen.getByText("■ Stop")).toBeTruthy();
    expect(screen.getByText("Create session")).toBeTruthy(); // the buttons stay
    expect(screen.queryByText(/Run the flow on routing_test/)).toBeNull(); // the examples are gone
  });

  it("stays while the model loads the session, and gives the full interface when it is done", async () => {
    server(withModel);
    render(<App />);
    await say("Run the flow");
    chat.send({ type: "message_start", id: "m1", role: "assistant" });
    chat.send({ type: "tool_call_started", id: "m1", call_id: "c1", tool: "session_open", arguments: {} });
    chat.open(); // the session exists now
    chat.send({ type: "tool_call_finished", id: "m1", call_id: "c1", ok: true, seconds: 0.1, truncated: false, result: { ok: true } });
    chat.send({ type: "state_changed", id: "m1", reason: "session_open" });
    await waitFor(() => expect(screen.getAllByText(/s1 · opened/).length).toBeGreaterThan(0)); // the status of the session
    expect(sidebar()).toBeNull(); // the model is still working: it stays
    chat.send({ type: "text_delta", id: "m1", text: "The flow is loaded." });
    chat.end();
    await waitFor(() => expect(sidebar()).toBeTruthy());
    expect(screen.queryByRole("region", { name: "Answer" })).toBeNull();
    expect(screen.getAllByText("Run the flow").length).toBeGreaterThan(0); // the chat has it all
  });

  it("stays on the screen when the answer needs no session, until the person leaves it", async () => {
    server(withModel);
    render(<App />);
    await say("Which router options can I change?");
    chat.send({ type: "message_start", id: "m1", role: "assistant" });
    chat.send({ type: "text_delta", id: "m1", text: "Many of them." });
    chat.end();
    await waitFor(() => expect(screen.queryByText("■ Stop")).toBeNull());
    expect(sidebar()).toBeNull();
    expect(screen.getByRole("region", { name: "Answer" }).textContent).toContain("Many of them.");
    fireEvent.click(screen.getByText("Open the full interface ›"));
    await waitFor(() => expect(sidebar()).toBeTruthy());
  });

  it("stops the answer from the first screen", async () => {
    const calls: { url: string; body?: string }[] = [];
    server(withModel, calls);
    render(<App />);
    await say("Run the flow");
    fireEvent.click(await screen.findByText("■ Stop"));
    await waitFor(() => expect(calls.some((c) => c.url === "/api/cancel")).toBe(true));
  });

  it("does not come back when the session is closed after it was used", async () => {
    server(withModel);
    render(<App />);
    await say("Run the flow");
    chat.open();
    chat.send({ type: "message_start", id: "m1", role: "assistant" });
    chat.send({ type: "state_changed", id: "m1", reason: "session_open" });
    chat.end();
    await waitFor(() => expect(sidebar()).toBeTruthy());
    fireEvent.click(screen.getByText("close"));
    await waitFor(() => expect(screen.getByText(/No session/)).toBeTruthy());
    expect(screen.queryByRole("textbox", { name: "Message" })).toBeTruthy(); // the chat's own box
    expect(screen.queryByText("Create session from device package…")).toBeNull(); // not the first screen
  });

  it("offers the examples when there is a model, and the first one fills the prompt", async () => {
    server(withModel);
    render(<App />);
    const example = await screen.findByText(/Run the flow on routing_test/);
    fireEvent.click(example);
    expect((screen.getByRole("textbox", { name: "Message" }) as HTMLTextAreaElement).value).toMatch(/routing_test/);
  });

  it("comes back when the session is closed and nothing was said", async () => {
    server(real);
    render(<App />);
    fireEvent.click(await screen.findByText("Create session"));
    await waitFor(() => expect(sidebar()).toBeTruthy());
    fireEvent.click(screen.getByText("close"));
    await screen.findByText("Create session");
    expect(sidebar()).toBeNull();
  });
});
