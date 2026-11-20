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
  banner,
  busy,
  usage,
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
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: "http://127.0.0.1:11434/v1", name: "llama3.1" } };
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

describe("what the model costs", () => {
  it("shows how full the context is, and warns when it is nearly full", () => {
    render(<TopBar />);
    expect(screen.queryByText(/ctx/)).toBeNull();
    applyEvent({ type: "usage", id: "m1", step: 2, prompt_tokens: 3000, completion_tokens: 20, context_tokens: 3100, context_limit: 8192 });
    expect(usage.value).toEqual({ step: 2, used: 3100, limit: 8192 });
  });

  it("draws it in the top bar", async () => {
    usage.value = { step: 3, used: 7000, limit: 8192 };
    render(<TopBar />);
    const label = screen.getByText(/ctx 7,000\/8,192/);
    expect(label.className).toContain("context-full");
    expect(label.title).toContain("Step 3");
  });

  it("says that it is working while a message is answered", () => {
    busy.value = true;
    render(<Workspace />);
    expect(screen.getByText(/working…/)).toBeTruthy();
    busy.value = false;
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

  it("draws how far the flow went, and what a failed stage said", () => {
    const v = filled();
    v.stages[3] = { ...v.stages[3], status: "failed", message: "no route at channel width 2" };
    view.value = v;
    render(<Sidebar />);
    const bar = screen.getByRole("progressbar", { name: "Stages reached" });
    expect(bar.getAttribute("aria-valuenow")).toBe("3");
    expect(bar.getAttribute("aria-valuemax")).toBe("5");
    expect((bar.firstChild as HTMLElement).style.width).toBe("60%");
    expect(screen.getByText("no route at channel width 2")).toBeTruthy();
  });

  it("says that a click on a name copied it", async () => {
    const writeText = vi.fn(() => Promise.resolve());
    vi.stubGlobal("navigator", { clipboard: { writeText } });
    view.value = filled();
    render(<Sidebar />);
    fireEvent.click(screen.getByText("routing_test.net"));
    expect(writeText).toHaveBeenCalledWith("routing_test.net");
    await waitFor(() => expect(screen.getByText(/✓ copied/)).toBeTruthy());
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

describe("the sessions of the sidebar", () => {
  const real = () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
  };
  const answers = (ok = true) =>
    vi.fn((url: string, _init?: RequestInit) =>
      Promise.resolve(
        new Response(
          JSON.stringify(
            url === "/api/session"
              ? emptyView()
              : ok
                ? { ok: true, data: {} }
                : { ok: false, diagnostics: [{ kind: "too_many_sessions", message: "4 sessions are open", hint: "close one" }] },
          ),
          { status: 200 },
        ),
      ),
    );

  it("are not there in the demo", () => {
    view.value = filled();
    render(<Sidebar />);
    expect((screen.getByText("+ new") as HTMLButtonElement).disabled).toBe(true);
    expect((screen.getByText("close") as HTMLButtonElement).disabled).toBe(true);
    expect(screen.queryByText("restore")).toBeNull();
  });

  it("open a session from the page, also when there is none", async () => {
    real();
    const fetchMock = answers();
    vi.stubGlobal("fetch", fetchMock);
    render(<Sidebar />);
    fireEvent.click(screen.getByText("+ new"));
    await waitFor(() => expect(fetchMock.mock.calls.map((c) => c[0])).toContain("/api/session/new"));
    await waitFor(() => expect(fetchMock.mock.calls.map((c) => c[0])).toContain("/api/session"));
  });

  it("close the session and restore a worker that ended", async () => {
    real();
    const fetchMock = answers();
    vi.stubGlobal("fetch", fetchMock);
    view.value = { ...filled(), session: { id: "s1", name: "s1", worker: "ended", ended: "killed", idle_s: 0 } };
    render(<Sidebar />);
    fireEvent.click(screen.getByText("Restore"));
    await waitFor(() => expect(fetchMock.mock.calls.map((c) => c[0])).toContain("/api/session/restore"));
    fireEvent.click(screen.getByText("close"));
    await waitFor(() => expect(fetchMock.mock.calls.map((c) => c[0])).toContain("/api/session/close"));
  });

  it("go back to a checkpoint by its name, and to the last with no name", async () => {
    real();
    const fetchMock = answers();
    vi.stubGlobal("fetch", fetchMock);
    view.value = {
      ...filled(),
      checkpoints: [
        { name: "last", kind: "after the last stage", stages: ["DEVICE", "PACKED"] },
        { name: "placed", kind: "named", stages: ["DEVICE", "PACKED", "PLACED"] },
      ],
    };
    render(<Sidebar />);
    const [last, placed] = screen.getAllByText("restore");
    fireEvent.click(placed);
    await waitFor(() => expect(fetchMock.mock.calls.length).toBeGreaterThan(0));
    const sent = fetchMock.mock.calls.find((c) => c[0] === "/api/session/restore")!;
    expect(JSON.parse((sent[1] as RequestInit).body as string)).toEqual({ checkpoint: "placed" });
    fireEvent.click(last);
    await waitFor(() => expect(fetchMock.mock.calls.filter((c) => c[0] === "/api/session/restore")).toHaveLength(2));
    const second = fetchMock.mock.calls.filter((c) => c[0] === "/api/session/restore")[1];
    expect(JSON.parse((second[1] as RequestInit).body as string)).toEqual({});
  });

  it("show what went wrong when the call fails", async () => {
    real();
    vi.stubGlobal("fetch", answers(false));
    render(<Sidebar />);
    fireEvent.click(screen.getByText("+ new"));
    const { banner } = await import("./store");
    await waitFor(() => expect(banner.value?.kind).toBe("too_many_sessions"));
    expect(banner.value?.hint).toBe("close one");
  });
});

describe("creating a session from a device package", () => {
  const real = () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
  };
  const reply = (body: unknown) => Promise.resolve(new Response(JSON.stringify(body)));
  const listing = {
    ok: true,
    data: {
      packages: [
        { name: "k6_frac_n10_2x2", version: "0.1.0", description: "K6 logic only", layout: "2x2", lut_size: 6, tags: ["tileable"] },
        { name: "k4_n4_dsp16_3x4", version: "0.1.0", description: "K4 with a multiplier", layout: "3x4", lut_size: 4, tags: [] },
      ],
      problems: [{ name: "broken", message: "The device package 'broken' cannot be used: the file 'a.xml' was changed (checksum)" }],
    },
  };

  it("is not there in the demo", () => {
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
    render(<Sidebar />);
    expect((screen.getByText("from device…") as HTMLButtonElement).disabled).toBe(true);
  });

  it("lists the packages, with a damaged one shown as such", async () => {
    real();
    vi.stubGlobal("fetch", vi.fn(() => reply(listing)));
    render(<Sidebar />);
    fireEvent.click(screen.getByText("from device…"));
    await screen.findByText("k6_frac_n10_2x2");
    expect(screen.getByText("K6 logic only")).toBeTruthy();
    expect(screen.getByText("3x4")).toBeTruthy();
    expect(screen.getByText("K4")).toBeTruthy();
    expect(screen.getByText(/cannot be used: the file 'a.xml' was changed/)).toBeTruthy();
    fireEvent.click(screen.getByText("from device…")); // a second click closes it
    expect(screen.queryByText("k6_frac_n10_2x2")).toBeNull();
  });

  it("says how to get packages when there are none", async () => {
    real();
    vi.stubGlobal("fetch", vi.fn(() => reply({ ok: true, data: { packages: [], problems: [] } })));
    render(<Sidebar />);
    fireEvent.click(screen.getByText("from device…"));
    await screen.findByText(/--device-packages/);
  });

  it("creates the session of the package that is chosen, and reads the session again", async () => {
    real();
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      reply(url === "/api/device-packages" ? listing : url === "/api/session" ? emptyView() : { ok: true, data: {} }),
    );
    vi.stubGlobal("fetch", fetchMock);
    render(<Sidebar />);
    fireEvent.click(screen.getByText("from device…"));
    await screen.findByText("k6_frac_n10_2x2");
    fireEvent.click(screen.getAllByText("Create session")[0]);
    await waitFor(() => expect(fetchMock.mock.calls.some((c) => c[0] === "/api/session")).toBe(true));
    const sent = fetchMock.mock.calls.find((c) => c[0] === "/api/session/new")!;
    expect(JSON.parse(String(sent[1]?.body))).toEqual({ device_package: "k6_frac_n10_2x2" });
    expect(screen.queryByText("Create session")).toBeNull(); // the picker closed
  });

  it("offers to load a package into an open session that is empty, and only then", async () => {
    real();
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      reply(url === "/api/device-packages" ? listing : url === "/api/session" ? filled() : { ok: true, data: {} }),
    );
    vi.stubGlobal("fetch", fetchMock);
    const v = filled();
    v.files.inputs = [];
    view.value = v;
    render(<Sidebar />);
    fireEvent.click(screen.getByText("from device…"));
    await screen.findByText("k6_frac_n10_2x2");
    fireEvent.click(screen.getAllByText("Load into this session")[0]);
    await waitFor(() => expect(fetchMock.mock.calls.some((c) => c[0] === "/api/session/device")).toBe(true));
    const sent = fetchMock.mock.calls.find((c) => c[0] === "/api/session/device")!;
    expect(JSON.parse(String(sent[1]?.body))).toEqual({ device_package: "k6_frac_n10_2x2" });
  });

  it("does not offer it when the session has files", async () => {
    real();
    vi.stubGlobal("fetch", vi.fn(() => reply(listing)));
    view.value = filled(); // has inputs
    render(<Sidebar />);
    fireEvent.click(screen.getByText("from device…"));
    await screen.findByText("k6_frac_n10_2x2");
    expect(screen.queryByText("Load into this session")).toBeNull();
  });

  it("shows the device of the session and waits for the circuit", async () => {
    real();
    const v = filled();
    v.session = {
      ...v.session!,
      device_package: { name: "k6_frac_n10_2x2", version: "0.1.0", description: "K6 logic only", layout: "2x2", lut_size: 6, tags: [] },
    };
    v.files.inputs = [
      { label: "vpr arch", name: "vpr_arch.xml", extra: "layout 2x2" },
      { label: "openfpga arch", name: "openfpga_arch.xml", extra: null },
    ];
    view.value = v;
    render(<Sidebar />);
    expect(screen.getByText(/device k6_frac_n10_2x2 · 2x2 · K6/)).toBeTruthy();
    expect(screen.getByText(/Waiting for a circuit/)).toBeTruthy();
    v.files.inputs = [...v.files.inputs, { label: "circuit", name: "mine.blif", extra: null }];
    view.value = { ...v };
    await waitFor(() => expect(screen.queryByText(/Waiting for a circuit/)).toBeNull());
  });
});

describe("uploading files to the session", () => {
  const real = () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
  };
  const answer = (status: number, body: unknown) => Promise.resolve(new Response(JSON.stringify(body), { status }));
  const ok = { ok: true, data: { name: "a.blif", kind: "circuit", bytes: 3 } };
  const taken = { ok: false, diagnostics: [{ kind: "file_exists", message: "There is already a file 'a.blif'" }] };
  const pick = (file: File) => fireEvent.change(screen.getByLabelText("Upload files"), { target: { files: [file] } });

  it("lists what was uploaded, with what it is and its size", () => {
    const v = filled();
    v.files.uploads = [
      { name: "mine.blif", kind: "circuit", bytes: 900 },
      { name: "arch.xml", kind: "openfpga_arch", bytes: 20480 },
    ];
    view.value = v;
    render(<Sidebar />);
    expect(screen.getByText("mine.blif")).toBeTruthy();
    expect(screen.getAllByText("circuit")).toHaveLength(2); // the input of that name, and the chip
    expect(screen.getByText("900 B")).toBeTruthy();
    expect(screen.getByText("OpenFPGA arch")).toBeTruthy();
    expect(screen.getByText("20 kB")).toBeTruthy();
  });

  it("lists the files that the model wrote, apart from the uploads, with no remove button", () => {
    const v = filled();
    v.files.created = [{ name: "my_arch.xml", kind: "vpr_arch", bytes: 2048 }];
    view.value = v;
    render(<Sidebar />);
    expect(screen.getByText("Written by the model")).toBeTruthy();
    expect(screen.getByText("my_arch.xml")).toBeTruthy();
    expect(screen.getByText("VPR arch")).toBeTruthy();
    expect(screen.queryByLabelText("Remove my_arch.xml")).toBeNull();
  });

  it("has no heading for them when there are none", () => {
    view.value = filled();
    render(<Sidebar />);
    expect(screen.queryByText("Written by the model")).toBeNull();
  });

  it("removes a file after the person confirms, and reads the session again", async () => {
    real();
    const v = filled();
    v.files.uploads = [{ name: "mine.blif", kind: "circuit", bytes: 900 }];
    view.value = v;
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      answer(200, url === "/api/session" ? emptyView() : { ok: true, data: {} }),
    );
    vi.stubGlobal("fetch", fetchMock);
    const confirm = vi.fn(() => false);
    vi.stubGlobal("confirm", confirm);
    render(<Sidebar />);
    fireEvent.click(screen.getByLabelText("Remove mine.blif"));
    expect(confirm).toHaveBeenCalled();
    expect(fetchMock).not.toHaveBeenCalled(); // not confirmed: nothing is sent
    confirm.mockReturnValue(true);
    fireEvent.click(screen.getByLabelText("Remove mine.blif"));
    await waitFor(() => expect(fetchMock.mock.calls.some((c) => c[0] === "/api/session")).toBe(true));
    const sent = fetchMock.mock.calls.find((c) => String(c[0]).startsWith("/api/upload"))!;
    expect(sent[0]).toBe("/api/upload?name=mine.blif");
    expect(sent[1]?.method).toBe("DELETE");
  });

  it("says when the circuit that was uploaded could not be read, and keeps quiet when it was", async () => {
    real();
    let body: unknown = { ok: true, data: { name: "a.blif", kind: "circuit", bytes: 3, loaded: true } };
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      url.startsWith("/api/upload") ? answer(200, body) : answer(200, emptyView()),
    );
    vi.stubGlobal("fetch", fetchMock);
    render(<Sidebar />);
    pick(new File(["abc"], "a.blif"));
    await waitFor(() => expect(fetchMock.mock.calls.some((c) => c[0] === "/api/session")).toBe(true));
    expect(banner.value).toBeNull();
    body = {
      ok: true,
      data: { name: "a.blif", kind: "circuit", bytes: 3, loaded: false, load_error: "no .model", load_hint: "check the file" },
    };
    pick(new File(["abc"], "a.blif"));
    await waitFor(() => expect(banner.value?.kind).toBe("circuit_not_read"));
    expect(banner.value?.message).toContain("a.blif was uploaded, but reading it as the circuit failed: no .model");
    expect(banner.value?.hint).toBe("check the file");
  });

  it("is not there in the demo", () => {
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
    render(<Sidebar />);
    expect((screen.getByText(/Upload files/) as HTMLButtonElement).disabled).toBe(true);
  });

  it("sends the file as it is, by its name, and reads the session again", async () => {
    real();
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      url.startsWith("/api/upload") ? answer(200, ok) : answer(200, emptyView()),
    );
    vi.stubGlobal("fetch", fetchMock);
    render(<Sidebar />);
    const file = new File(["abc"], "a b.blif");
    pick(file);
    await waitFor(() => expect(fetchMock.mock.calls.some((c) => c[0] === "/api/session")).toBe(true));
    const sent = fetchMock.mock.calls.find((c) => String(c[0]).startsWith("/api/upload"))!;
    expect(sent[0]).toBe("/api/upload?name=a%20b.blif");
    expect((sent[1] as RequestInit).body).toBe(file);
    expect(banner.value).toBeNull();
  });

  it("asks before it replaces a file, and sends it again only if the person agrees", async () => {
    real();
    const calls: string[] = [];
    vi.stubGlobal(
      "fetch",
      vi.fn((url: string) => {
        calls.push(url);
        if (url === "/api/upload?name=a.blif") return answer(409, taken);
        if (url === "/api/upload?name=a.blif&overwrite=1") return answer(200, ok);
        return answer(200, emptyView());
      }),
    );
    const confirm = vi.spyOn(window, "confirm");
    confirm.mockReturnValueOnce(false);
    render(<Sidebar />);
    pick(new File(["abc"], "a.blif"));
    await waitFor(() => expect(confirm).toHaveBeenCalledTimes(1));
    await waitFor(() => expect(calls).toContain("/api/session"));
    expect(calls.filter((u) => u.includes("overwrite"))).toHaveLength(0);
    confirm.mockReturnValueOnce(true);
    calls.length = 0;
    pick(new File(["abc"], "a.blif"));
    await waitFor(() => expect(calls).toContain("/api/upload?name=a.blif&overwrite=1"));
  });

  it("does not send a kind of file that a flow does not read, and says why", async () => {
    real();
    const fetchMock = vi.fn((_url: string) => answer(200, emptyView()));
    vi.stubGlobal("fetch", fetchMock);
    render(<Sidebar />);
    pick(new File(["x"], "run.exe"));
    await waitFor(() => expect(banner.value?.kind).toBe("invalid_type"));
    expect(banner.value?.hint).toContain(".blif");
    expect(fetchMock.mock.calls.some((c) => String(c[0]).startsWith("/api/upload"))).toBe(false);
  });

  it("shows what the server refused", async () => {
    real();
    vi.stubGlobal(
      "fetch",
      vi.fn((url: string) =>
        url.startsWith("/api/upload")
          ? answer(413, { ok: false, diagnostics: [{ kind: "session_full", message: "Session s1 has used its share", hint: null }] })
          : answer(200, emptyView()),
      ),
    );
    render(<Sidebar />);
    pick(new File(["abc"], "a.blif"));
    await waitFor(() => expect(banner.value?.message).toBe("Session s1 has used its share"));
  });

  it("takes files that are dropped on the chat, in a session and not in the demo", async () => {
    real();
    const fetchMock = vi.fn((url: string) => (url.startsWith("/api/upload") ? answer(200, ok) : answer(200, emptyView())));
    vi.stubGlobal("fetch", fetchMock);
    render(<Workspace />);
    const chat = screen.getByLabelText("Chat");
    const file = new File(["abc"], "a.blif");
    fireEvent.dragOver(chat, { dataTransfer: { types: ["Files"] } });
    expect(screen.getByText(/Drop files to copy/)).toBeTruthy();
    fireEvent.drop(chat, { dataTransfer: { types: ["Files"], files: [file] } });
    await waitFor(() => expect(fetchMock.mock.calls.some((c) => String(c[0]).startsWith("/api/upload"))).toBe(true));
    expect(screen.queryByText(/Drop files to copy/)).toBeNull();
    // In the demo the drop is left to the browser: no overlay, no request
    fetchMock.mockClear();
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
    fireEvent.drop(chat, { dataTransfer: { types: ["Files"], files: [file] } });
    expect(fetchMock).not.toHaveBeenCalled();
  });
});

describe("the chat of a session with a model", () => {
  it("does not say that there is no model, and offers examples that fill the box", () => {
    config.value = {
      schema_version: 1,
      console: false,
      backend: "session",
      model: { endpoint: "http://127.0.0.1:8081/v1", name: "qwen2.5-7b" },
    };
    render(<Workspace />);
    expect(screen.queryByText(/No model is connected/)).toBeNull();
    fireEvent.click(screen.getByText(/Which options of the router/));
    expect((screen.getByLabelText("Message") as HTMLTextAreaElement).value).toMatch(/^Which options of the router/);
  });

  it("shows an answer as Markdown, and what the person wrote as it was written", () => {
    messages.value = [
      { id: "u1", role: "user", parts: [{ kind: "text", text: "give **me** the Fmax" }] },
      { id: "m1", role: "assistant", parts: [{ kind: "text", text: "The Fmax is **1063.455 MHz**:\n\n- slack `-0.9`" }] },
    ];
    render(<Workspace />);
    expect(screen.getByText("1063.455 MHz").tagName).toBe("STRONG");
    expect(screen.getByText("-0.9").tagName).toBe("CODE");
    expect(screen.getByText("give **me** the Fmax")).toBeTruthy();
  });
});

describe("the chat of a session with no model", () => {
  it("says how to call a tool, in place of asking for a flow", () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
    render(<Workspace />);
    expect(screen.getByText(/No model is connected/)).toBeTruthy();
    expect(screen.getByText("/help")).toBeTruthy();
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

  it("shows the result as formatted text, with the JSON on request", () => {
    give("a", "run_stage", true, { data: { stage: "pack", seconds: 0.4, reached: ["DEVICE", "PACKED"] } });
    const { container } = render(<ToolCard callId="a" startOpen />);
    expect(screen.getByText(/finished in 0.4 s/)).toBeTruthy();
    expect(container.querySelector("pre.json")).toBeNull();
    fireEvent.click(screen.getByText("JSON"));
    expect(container.querySelector("pre.json")?.textContent).toContain('"stage"');
    fireEvent.click(screen.getByText("Formatted"));
    expect(container.querySelector("pre.json")).toBeNull();
  });

  it("makes a table of what has fields", () => {
    give("a", "get_utilization", true, { data: { device_utilization: 0.18, block_types: [{ name: "io", used: 7 }] } });
    const { container } = render(<ToolCard callId="a" startOpen />);
    expect(container.querySelectorAll(".result-md table").length).toBe(2); // the fields, and the blocks
    expect(screen.getByText("0.18")).toBeTruthy();
  });

  it("cuts a long JSON and shows it all on request", () => {
    give("a", "find", true, { data: { text: "x".repeat(2000) } });
    render(<ToolCard callId="a" startOpen />);
    fireEvent.click(screen.getByText("JSON"));
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
    config.value = { schema_version: 1, console: true, backend: "demo", model: { endpoint: null, name: null } };
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
    config.value = { schema_version: 1, console: true, backend: "demo", model: { endpoint: null, name: null } };
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
              ? { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: "demo-model" } }
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
