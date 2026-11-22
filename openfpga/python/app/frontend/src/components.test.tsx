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
  openReport,
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
      { label: "openfpga arch", name: "k6_frac_N10_40nm_openfpga.xml", extra: null },
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
    for (const title of ["Session", "Stages", "Design files", "Constraints and results", "Checkpoints"]) expect(screen.getByText(title)).toBeTruthy();
    expect(screen.getByText("(no session)")).toBeTruthy();
    expect(screen.getAllByText("(none)").length).toBeGreaterThanOrEqual(4);
    expect(screen.getAllByRole("listitem")).toHaveLength(6); // synthesis, then the five stages of the flow
  });

  it("shows the stages, the files and the checkpoints of a session", () => {
    view.value = filled();
    render(<Sidebar />);
    expect(screen.getByText("worker running")).toBeTruthy();
    const stages = screen.getAllByRole("listitem").map((li) => li.textContent ?? "");
    expect(stages[0]).toContain("✔"); // DEVICE
    expect(stages[1]).toContain("SYNTHESIS");
    expect(stages[3]).toContain("2.1 s");
    expect(stages[4]).toContain("○");
    expect(screen.getByLabelText("Open the architecture files")).toBeTruthy();
    expect(screen.queryByText(/k6_frac_N10_tileable_40nm_with_a_long_name/)).toBeNull(); // the architecture is a button
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
    expect(bar.getAttribute("aria-valuemax")).toBe("6");
    expect((bar.firstChild as HTMLElement).style.width).toBe("50%");
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
    const v = filled();
    v.files.uploads = [{ name: "k6_frac_N10_tileable_40nm_with_a_long_name.blif", kind: "circuit", bytes: 10 }];
    view.value = v;
    render(<Sidebar />);
    const name = "k6_frac_N10_tileable_40nm_with_a_long_name.blif";
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

  it("lists what was uploaded with its size and no label", () => {
    const v = filled();
    v.files.uploads = [
      { name: "mine.blif", kind: "circuit", bytes: 900 },
      { name: "arch.xml", kind: "openfpga_arch", bytes: 20480 },
    ];
    view.value = v;
    render(<Sidebar />);
    expect(screen.getByText("mine.blif")).toBeTruthy();
    expect(screen.getByText("900 B")).toBeTruthy();
    expect(screen.getByText("20 kB")).toBeTruthy();
    expect(document.querySelector(".design-files .chip")).toBeNull(); // no kind, origin or loaded label
  });

  it("lists the files that the model wrote with the others, with no remove button", () => {
    const v = filled();
    v.files.created = [{ name: "my_arch.xml", kind: "vpr_arch", bytes: 2048 }];
    view.value = v;
    render(<Sidebar />);
    expect(screen.queryByText("Written by the model")).toBeNull(); // one list, not one for each origin
    expect(screen.getByText("my_arch.xml")).toBeTruthy();
    expect(screen.queryByLabelText("Remove my_arch.xml")).toBeNull();
  });

  it("lists the files of the synthesis in the same list, with the netlist that is read marked as loaded", () => {
    const v = filled();
    v.files.inputs = [
      { label: "vpr arch", name: "vpr_arch.xml", extra: "layout 2x2" },
      { label: "circuit", name: "s1/synth/and2.blif", extra: null },
    ];
    v.files.synthesis = [
      { name: "synth/and2.blif", kind: "circuit", bytes: 300 },
      { name: "synth/synth.ys", kind: "other", bytes: 800 },
    ];
    view.value = v;
    render(<Sidebar />);
    expect(screen.queryByText("Synthesis", { selector: "h3" })).toBeNull();
    expect(screen.getByText("synth/and2.blif")).toBeTruthy();
    expect(screen.getByText("synth/synth.ys")).toBeTruthy();
    expect(document.querySelectorAll(".design-files .upload-row.loaded")).toHaveLength(1);
    expect(document.querySelector(".design-files .upload-row.loaded")?.textContent).toContain("synth/and2.blif");
    expect(screen.queryByText("s1/synth/and2.blif")).toBeNull(); // the circuit input is not listed twice
  });

  it("lists the copy of a design directory first", () => {
    const v = filled();
    v.files.design = [
      { name: "design/chip/design.json", kind: "other", bytes: 400 },
      { name: "design/chip/rtl/chip.v", kind: "other", bytes: 120 },
    ];
    v.files.synthesis = [{ name: "synth/chip.blif", kind: "circuit", bytes: 300 }];
    view.value = v;
    render(<Sidebar />);
    expect(screen.getByText("design/chip/rtl/chip.v")).toBeTruthy();
    const rows = [...document.querySelectorAll(".design-files .upload-row")].map((e) => e.textContent ?? "");
    const at = (name: string) => rows.findIndex((r) => r.startsWith(name));
    expect(at("design/chip/rtl/chip.v")).toBeGreaterThanOrEqual(0);
    expect(at("design/chip/rtl/chip.v")).toBeLessThan(at("synth/chip.blif"));
  });

  it("lists only the circuit that was read when there is nothing else", () => {
    view.value = filled();
    render(<Sidebar />);
    expect(screen.getByText("routing_test.blif")).toBeTruthy(); // the circuit from a designs folder
    expect(document.querySelector(".design-files .upload-row.loaded")?.textContent).toContain("routing_test.blif");
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

describe("the device stage of the sidebar", () => {
  const real = () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
  };
  const pkg = { name: "k6_frac_n10_2x2", version: "0.1.0", description: "d", layout: "2x2", lut_size: 6, tags: [] };
  const device = () => screen.getAllByRole("listitem")[0];

  it("has a button instead of the two architecture files, and the button opens both", async () => {
    real();
    vi.stubGlobal("fetch", vi.fn(() => Promise.resolve(new Response(JSON.stringify({ ok: true, data: { name: "x", text: "<a/>", bytes: 4, truncated: false } })))));
    view.value = filled();
    render(<Sidebar />);
    expect(screen.queryByText("k6_frac_N10_40nm_openfpga.xml")).toBeNull();
    expect(screen.queryByText("Architecture")).toBeNull(); // not in the design files any more
    fireEvent.click(screen.getByLabelText("Open the architecture files"));
    await waitFor(() =>
      expect(tabs.value.map((t) => t.id)).toEqual([
        "file:k6_frac_N10_tileable_40nm_with_a_long_name.xml",
        "file:k6_frac_N10_40nm_openfpga.xml",
      ]),
    );
  });

  it("has no button before the architectures are read", () => {
    real();
    const v = filled();
    v.files.inputs = [];
    view.value = v;
    render(<Sidebar />);
    expect(screen.queryByLabelText("Open the architecture files")).toBeNull();
  });

  it("names the device package next to DEVICE", () => {
    real();
    const v = filled();
    v.session = { ...v.session!, device_package: pkg };
    view.value = v;
    render(<Sidebar />);
    expect(device().textContent).toContain("DEVICE k6_frac_n10_2x2");
  });

  it("is reached as soon as the device package is loaded, before the circuit", () => {
    real();
    const v = filled();
    v.session = { ...v.session!, device_package: pkg };
    v.stages = v.stages.map((st) => ({ ...st, status: "not_reached" as const, seconds: null }));
    v.files.inputs = v.files.inputs.filter((f) => f.label !== "circuit");
    view.value = v;
    render(<Sidebar />);
    expect(device().className).toContain("stage-reached");
    expect(screen.getAllByRole("listitem")[2].className).toContain("stage-not_reached"); // PACKED
  });

  it("is not reached in a session with no architecture", () => {
    real();
    const v = filled();
    v.stages = v.stages.map((st) => ({ ...st, status: "not_reached" as const, seconds: null }));
    v.files.inputs = [];
    view.value = v;
    render(<Sidebar />);
    expect(device().className).toContain("stage-not_reached");
  });
});

describe("the reports of the stages", () => {
  it("has a report button on each stage that has one, which opens the report in a tab", () => {
    const v = filled();
    v.reports = ["device", "pack"];
    view.value = v;
    render(<Sidebar />);
    expect(screen.queryByLabelText("Open the report of placed")).toBeNull();
    fireEvent.click(screen.getByLabelText("Open the report of packed"));
    expect(tabs.value.map((t) => [t.id, t.title, t.report])).toEqual([["report:pack", "Packing report", "pack"]]);
    expect(activeTab.value).toBe("report:pack");
    fireEvent.click(screen.getByLabelText("Open the report of packed")); // the same tab, not a second one
    fireEvent.click(screen.getByLabelText("Open the report of device"));
    expect(tabs.value.map((t) => t.id)).toEqual(["report:pack", "report:device"]);
  });

  it("shows the report in a sandboxed frame, loaded again after a new run", async () => {
    const v = filled();
    v.reports = ["pack"];
    view.value = v;
    openReport("pack");
    render(<Workspace />);
    const frame = screen.getByTitle("Packing report") as HTMLIFrameElement;
    expect(frame.tagName).toBe("IFRAME");
    expect(frame.getAttribute("sandbox")).toBe("");
    const first = frame.getAttribute("src") ?? "";
    expect(first.startsWith("/api/report?stage=pack&v=")).toBe(true);
    expect((screen.getByText(/Open in browser/) as HTMLAnchorElement).target).toBe("_blank");
    view.value = { ...v, latest: { status: "reached", text: "s1 · PACKED ✔ 0.3 s" } };
    await waitFor(() =>
      expect((screen.getByTitle("Packing report") as HTMLIFrameElement).getAttribute("src")).not.toBe(first),
    );
  });

  it("says so when the session has no report of the stage", () => {
    view.value = filled();
    openReport("route");
    render(<Workspace />);
    expect(screen.getByText(/no report of this stage/)).toBeTruthy();
    expect(document.querySelector("iframe")).toBeNull();
  });
});

describe("the run buttons of the stages", () => {
  const real = () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
  };
  const chat = () => {
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      Promise.resolve(new Response(url === "/api/session" ? JSON.stringify(emptyView()) : "")),
    );
    vi.stubGlobal("fetch", fetchMock);
    return () => JSON.parse(String(fetchMock.mock.calls.find((c) => c[0] === "/api/chat")?.[1]?.body ?? "null"));
  };

  it("are next to synthesis, pack, place and route only", () => {
    real();
    view.value = filled();
    render(<Sidebar />);
    for (const name of ["synthesis", "packed", "placed", "routed"]) expect(screen.getByLabelText(`Run ${name}`)).toBeTruthy();
    expect(screen.queryByLabelText("Run device")).toBeNull();
    expect(screen.queryByLabelText("Run analyzed")).toBeNull();
  });

  it("run a stage as the slash line of run_stage", async () => {
    real();
    const sent = chat();
    view.value = filled();
    render(<Sidebar />);
    fireEvent.click(screen.getByLabelText("Run routed"));
    await waitFor(() => expect(sent()).toEqual({ message: "/run_stage stage=route" }));
  });

  it("synthesize again the design that the session added last", async () => {
    real();
    const sent = chat();
    const v = filled();
    v.synthesis = { stage: "SYNTHESIS", status: "reached", seconds: 1.2, message: null, design: "chip" };
    view.value = v;
    render(<Sidebar />);
    fireEvent.click(screen.getByLabelText("Run synthesis"));
    await waitFor(() => expect(sent()).toEqual({ message: "/synthesize design=chip" }));
  });

  it("cannot synthesize before a design is added, nor run in the demo or with no worker", () => {
    real();
    view.value = filled();
    const { unmount } = render(<Sidebar />);
    expect((screen.getByLabelText("Run synthesis") as HTMLButtonElement).disabled).toBe(true);
    expect((screen.getByLabelText("Run packed") as HTMLButtonElement).disabled).toBe(false);
    unmount();
    const ended = filled();
    ended.session = { ...ended.session!, worker: "ended" };
    view.value = ended;
    const second = render(<Sidebar />);
    expect((screen.getByLabelText("Run packed") as HTMLButtonElement).disabled).toBe(true);
    second.unmount();
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
    view.value = filled();
    render(<Sidebar />);
    expect((screen.getByLabelText("Run packed") as HTMLButtonElement).disabled).toBe(true);
  });
});

describe("synthesis as a stage", () => {
  const withSynthesis = (status: "reached" | "failed" | "skipped" | "not_reached", extra = {}) => {
    const v = filled();
    v.synthesis = { stage: "SYNTHESIS", status, seconds: null, message: null, ...extra };
    view.value = v;
  };

  it("is the stage after the device", () => {
    withSynthesis("not_reached");
    render(<Sidebar />);
    const items = screen.getAllByRole("listitem");
    expect(items).toHaveLength(6);
    expect(items[0].textContent).toContain("DEVICE");
    expect(items[1].textContent).toContain("SYNTHESIS");
    expect(items[1].className).toContain("stage-not_reached");
  });

  it("shows the time when it ran, and what it said when it failed", () => {
    withSynthesis("reached", { seconds: 1.4 });
    const { unmount } = render(<Sidebar />);
    expect(screen.getAllByRole("listitem")[1].textContent).toContain("1.4 s");
    unmount();
    withSynthesis("failed", { message: "syntax error, unexpected ';'" });
    render(<Sidebar />);
    expect(screen.getByText("syntax error, unexpected ';'")).toBeTruthy();
  });

  it("is greyed out when the design is a netlist, says why, and is not counted in the progress", () => {
    withSynthesis("skipped", { message: "Not needed: the design is a netlist (routing_test.blif)" });
    render(<Sidebar />);
    const first = screen.getAllByRole("listitem")[1];
    expect(first.className).toContain("stage-skipped");
    expect(first.getAttribute("title")).toContain("the design is a netlist");
    expect(first.textContent).toContain("–");
    const bar = screen.getByRole("progressbar", { name: "Stages reached" });
    expect(bar.getAttribute("aria-valuemax")).toBe("5"); // the five stages of the flow
    expect(bar.getAttribute("aria-valuenow")).toBe("3");
    expect((bar.firstChild as HTMLElement).style.width).toBe("60%");
  });
});

describe("the sessions list of the sidebar", () => {
  const real = () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
  };
  const found = {
    ok: true,
    data: {
      live: [
        { id: "s1", name: "first", stages: ["DEVICE"], device_package: null, running: true, current: true },
        { id: "s2", name: "second", stages: ["DEVICE", "PACKED"], device_package: null, running: true, current: false },
      ],
      saved: [{ id: "s3", name: "old", stages: ["DEVICE", "PACKED", "PLACED"], device_package: null, modified: 0 }],
    },
  };

  it("opens a list of the sessions from the button, with the current one marked, and closes it again", async () => {
    real();
    vi.stubGlobal("fetch", vi.fn(() => Promise.resolve(new Response(JSON.stringify(found)))));
    view.value = filled();
    render(<Sidebar />);
    fireEvent.click(screen.getByText("sessions…"));
    await screen.findByText("second");
    expect(screen.getByText("Current")).toBeTruthy();
    expect(screen.getByText("old")).toBeTruthy();
    expect(screen.getAllByText("Open")).toHaveLength(2); // the other live one, and the saved one
    fireEvent.click(screen.getByText("sessions…"));
    expect(screen.queryByText("second")).toBeNull();
  });

  it("switches to a live session from the list", async () => {
    real();
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      Promise.resolve(new Response(JSON.stringify(url === "/api/sessions" ? found : url === "/api/session" ? emptyView() : { ok: true, data: {} }))),
    );
    vi.stubGlobal("fetch", fetchMock);
    view.value = filled();
    render(<Sidebar />);
    fireEvent.click(screen.getByText("sessions…"));
    await screen.findByText("second");
    fireEvent.click(screen.getAllByText("Open")[0]);
    await waitFor(() => {
      const call = fetchMock.mock.calls.find((c) => c[0] === "/api/session/open");
      expect(JSON.parse(String(call?.[1]?.body))).toEqual({ session: "s2" });
    });
  });

  it("is not there in the demo", () => {
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
    render(<Sidebar />);
    expect((screen.getByText("sessions…") as HTMLButtonElement).disabled).toBe(true);
  });
});

describe("the designs of the sidebar", () => {
  const real = () => {
    config.value = { schema_version: 1, console: false, backend: "session", model: { endpoint: null, name: null } };
  };
  const designs = [
    { name: "opencores_gpio", top: "gpio_top", path: "interface/opencores_gpio", files: ["gpio_top.v"], description: "GPIO" },
    { name: "rtcclock", top: "rtcclock", path: "interface/rtcclock", files: ["a.v", "b.v"], description: null },
  ];

  it("lists the design directories with their top module and the number of files", () => {
    real();
    view.value = { ...filled(), designs };
    render(<Sidebar />);
    expect(screen.getByText("opencores_gpio")).toBeTruthy();
    expect(screen.getByText("top gpio_top · 1 file")).toBeTruthy();
    expect(screen.getByText("top rtcclock · 2 files")).toBeTruthy();
  });

  it("says when a design has only a netlist, which Add reads instead of synthesizing", () => {
    real();
    const only = { name: "gates", top: "gates", path: "x/gates", files: [], netlists: ["gates.blif"], description: null };
    view.value = { ...filled(), designs: [only] };
    render(<Sidebar />);
    expect(screen.getByText("top gates · netlist only")).toBeTruthy();
    expect(screen.getByText("Add").title).toContain("read its netlist");
  });

  it("says what a design directory is when there is none", () => {
    real();
    view.value = { ...filled(), designs: [] };
    render(<Sidebar />);
    expect(screen.getByText(/no design directory/)).toBeTruthy();
  });

  it("adds a design by its name, from the button, as the slash line of synthesize", async () => {
    real();
    const fetchMock = vi.fn((url: string, _init?: RequestInit) =>
      Promise.resolve(new Response(url === "/api/session" ? JSON.stringify(emptyView()) : "")),
    );
    vi.stubGlobal("fetch", fetchMock);
    view.value = { ...filled(), designs };
    render(<Sidebar />);
    fireEvent.click(screen.getAllByText("Add")[0]);
    await waitFor(() => {
      const call = fetchMock.mock.calls.find((c) => c[0] === "/api/chat");
      expect(JSON.parse(String(call?.[1]?.body))).toEqual({ message: "/synthesize design=opencores_gpio" });
    });
  });

  it("needs a session, and is not there in the demo", () => {
    real();
    view.value = { ...emptyView(), designs };
    const { unmount } = render(<Sidebar />);
    expect((screen.getAllByText("Add")[0] as HTMLButtonElement).disabled).toBe(true);
    unmount();
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
    view.value = { ...filled(), designs };
    render(<Sidebar />);
    expect((screen.getAllByText("Add")[0] as HTMLButtonElement).disabled).toBe(true);
  });
});
