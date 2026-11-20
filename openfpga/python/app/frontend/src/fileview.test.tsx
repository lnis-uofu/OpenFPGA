import { act, fireEvent, render, screen, waitFor } from "@testing-library/preact";
import { beforeEach, describe, expect, it, vi } from "vitest";
import { Console } from "./components/Console";
import { Sidebar } from "./components/Sidebar";
import { Workspace } from "./components/Workspace";
import {
  CONSOLE_HEIGHT_DEFAULT,
  activeTab,
  closeTab,
  config,
  consoleHeight,
  consoleOpen,
  emptyView,
  openFile,
  openFiles,
  resetStore,
  setConsoleHeight,
  tabs,
  view,
} from "./store";
import { languageOf } from "./language";

// Monaco does not run in jsdom: a stand-in that keeps what it was given
type FakeEditor = { value: string; disposed: boolean; listener?: () => void; commands: { key: number; run: () => void }[]; type(v: string): void };
const fake = vi.hoisted(() => ({ fail: false, created: [] as { options: Record<string, unknown>; editor: FakeEditor }[] }));
vi.mock("./monaco", () => ({
  get monaco() {
    if (fake.fail) throw new Error("the editor did not load");
    return {
      KeyMod: { CtrlCmd: 2048 },
      KeyCode: { KeyS: 49 },
      editor: {
        setTheme: () => undefined,
        create: (_host: unknown, options: Record<string, unknown>) => {
          const editor = {
            value: String(options.value),
            disposed: false,
            listener: undefined as (() => void) | undefined,
            commands: [] as { key: number; run: () => void }[],
            type(v: string) {
              this.value = v; // what a person typing leaves, and the event of it
              this.listener?.();
            },
            onDidChangeModelContent(fn: () => void) {
              this.listener = fn;
            },
            addCommand(key: number, run: () => void) {
              this.commands.push({ key, run });
            },
            getValue() {
              return this.value;
            },
            setValue(v: string) {
              this.value = v;
            },
            dispose() {
              this.disposed = true;
            },
          };
          fake.created.push({ options, editor });
          return editor;
        },
      },
    };
  },
}));

const reply = (status: number, body: unknown) => Promise.resolve(new Response(JSON.stringify(body), { status }));
const file = (text: string, extra = {}) => ({ ok: true, data: { name: "x", text, bytes: text.length, truncated: false, ...extra } });

beforeEach(() => {
  resetStore();
  fake.fail = false;
  fake.created.length = 0;
  window.localStorage.clear();
  config.value = { schema_version: 1, console: true, backend: "session", model: { endpoint: null, name: null } };
});

describe("the languages of the viewer", () => {
  it("knows the files of a flow", () => {
    expect(languageOf("k6.xml")).toBe("xml");
    expect(languageOf("design.net")).toBe("xml"); // the packing is XML
    expect(languageOf("top.V")).toBe("systemverilog");
    expect(languageOf("constraints.sdc")).toBe("tcl");
    expect(languageOf("and2.blif")).toBe("blif");
    expect(languageOf("AND2.EBLIF")).toBe("blif");
    expect(languageOf("design.place")).toBe("plaintext");
    expect(languageOf("README")).toBe("plaintext");
  });
});

describe("opening a file in a tab", () => {
  it("reads it by its name, shows it in the editor with its language, and is one tab however often", async () => {
    const fetchMock = vi.fn((_url: string) => reply(200, file("<block/>", { bytes: 2048 })));
    vi.stubGlobal("fetch", fetchMock);
    render(<Workspace />);
    await openFile("s1/design.net");
    await openFile("s1/design.net");
    expect(fetchMock.mock.calls[0][0]).toBe("/api/file?name=s1%2Fdesign.net");
    expect(tabs.value.filter((t) => t.id === "file:s1/design.net")).toHaveLength(1);
    expect(tabs.value[0].title).toBe("design.net");
    await waitFor(() => expect(fake.created).toHaveLength(1));
    expect(fake.created[0].options).toMatchObject({ value: "<block/>", language: "xml", readOnly: true });
    expect(screen.getByText(/2\.0 kB/)).toBeTruthy();
  });

  it("opens from the sidebar by a click on the name, and the small button only copies it", async () => {
    vi.stubGlobal("fetch", vi.fn(() => reply(200, file("a"))));
    const writeText = vi.fn();
    vi.stubGlobal("navigator", { clipboard: { writeText } });
    const v = emptyView();
    v.session = { id: "s1", name: "s1", worker: "running", ended: null, idle_s: 0 };
    v.files.results = ["design.net"];
    view.value = v;
    render(<Sidebar />);
    fireEvent.click(screen.getByLabelText("Copy design.net"));
    expect(writeText).toHaveBeenCalledWith("design.net");
    expect(tabs.value).toHaveLength(0);
    fireEvent.click(screen.getByTitle("Open design.net"));
    await waitFor(() => expect(activeTab.value).toBe("file:design.net"));
  });

  it("says why a file cannot be shown", async () => {
    vi.stubGlobal(
      "fetch",
      vi.fn(() => reply(400, { ok: false, diagnostics: [{ kind: "binary_file", message: "'a.bin' is not a text file", hint: "Only text files can be shown" }] })),
    );
    render(<Workspace />);
    await openFile("a.bin");
    expect((await screen.findByRole("alert")).textContent).toContain("is not a text file (Only text files can be shown)");
    expect(fake.created).toHaveLength(0);
  });

  it("shows the text as it is when the editor cannot be loaded", async () => {
    fake.fail = true;
    vi.stubGlobal("fetch", vi.fn(() => reply(200, file("plain text here"))));
    const { container } = render(<Workspace />);
    await openFile("a.txt");
    await waitFor(() => expect(container.querySelector("pre.file-plain")?.textContent).toBe("plain text here"));
  });

  it("reads it again with Reload, and the editor takes the new text", async () => {
    let text = "one";
    vi.stubGlobal("fetch", vi.fn(() => reply(200, file(text))));
    render(<Workspace />);
    await openFile("a.txt");
    await waitFor(() => expect(fake.created).toHaveLength(1));
    text = "two";
    fireEvent.click(screen.getByText("Reload"));
    await waitFor(() => expect(fake.created[0].editor.value).toBe("two"));
    expect(fake.created).toHaveLength(1); // the same editor
  });

  it("tells that a long file is cut, and frees the file when the tab is closed", async () => {
    vi.stubGlobal("fetch", vi.fn(() => reply(200, file("x", { truncated: true, bytes: 5_000_000 }))));
    render(<Workspace />);
    await openFile("big.txt");
    await screen.findByText(/the first megabyte is shown/);
    await waitFor(() => expect(fake.created).toHaveLength(1));
    closeTab("file:big.txt");
    expect(openFiles.value).toEqual({});
    expect(tabs.value).toEqual([]);
    await waitFor(() => expect(fake.created[0].editor.disposed).toBe(true));
  });

  it("does not open files in the recorded demo: a click copies the name", () => {
    config.value = { schema_version: 1, console: false, backend: "demo", model: { endpoint: null, name: null } };
    const fetchMock = vi.fn();
    vi.stubGlobal("fetch", fetchMock);
    const v = emptyView();
    v.files.results = ["design.net"];
    view.value = v;
    render(<Sidebar />);
    fireEvent.click(screen.getByText("design.net"));
    expect(fetchMock).not.toHaveBeenCalled();
    expect(screen.queryByLabelText("Copy design.net")).toBeNull();
  });
});

describe("the height of the console", () => {
  const open = () => {
    consoleOpen.value = true;
    render(<Console />);
    return screen.getByRole("separator", { name: "Resize the console" });
  };

  it("grows and shrinks with the arrow keys, within limits, and is kept", () => {
    const handle = open();
    expect(consoleHeight.value).toBe(CONSOLE_HEIGHT_DEFAULT);
    fireEvent.keyDown(handle, { key: "ArrowUp" });
    expect(consoleHeight.value).toBe(CONSOLE_HEIGHT_DEFAULT + 24);
    fireEvent.keyDown(handle, { key: "ArrowDown" });
    fireEvent.keyDown(handle, { key: "ArrowDown" });
    expect(consoleHeight.value).toBe(CONSOLE_HEIGHT_DEFAULT - 24);
    for (let i = 0; i < 20; i++) fireEvent.keyDown(handle, { key: "ArrowDown" });
    expect(consoleHeight.value).toBe(80); // not lower than this
    setConsoleHeight(100000);
    expect(consoleHeight.value).toBeLessThanOrEqual(Math.floor(window.innerHeight * 0.7));
    setConsoleHeight(300);
    expect(window.localStorage.getItem("openfpga-console-height")).toBe("300");
  });

  it("is the height of the lines, and a double click gives the usual one back", () => {
    setConsoleHeight(320);
    const handle = open();
    expect((document.querySelector(".console") as HTMLElement).style.getPropertyValue("--console-h")).toBe("320px");
    expect(handle.getAttribute("aria-valuenow")).toBe("320");
    fireEvent.dblClick(handle);
    expect(consoleHeight.value).toBe(CONSOLE_HEIGHT_DEFAULT);
  });

  it("is not there while the console is closed", () => {
    consoleOpen.value = false;
    render(<Console />);
    expect(screen.queryByRole("separator")).toBeNull();
  });
});


describe("editing a file", () => {
  const editable = (text: string) => file(text, { editable: true, copyable: false });

  it("lets a file of the session be edited: Save is there when the text changed, and saves it", async () => {
    const calls: { url: string; init?: RequestInit }[] = [];
    vi.stubGlobal(
      "fetch",
      vi.fn((url: string, init?: RequestInit) => {
        calls.push({ url, init });
        if (init?.method === "PUT") return reply(200, { ok: true, data: { name: "a.blif", bytes: 5, kind: "circuit", in_use: false } });
        if (url === "/api/session") return reply(200, emptyView());
        return reply(200, editable("one"));
      }),
    );
    render(<Workspace />);
    await openFile("a.blif");
    await waitFor(() => expect(fake.created).toHaveLength(1));
    expect(fake.created[0].options).toMatchObject({ readOnly: false });
    const save = screen.getByText("Save") as HTMLButtonElement;
    expect(save.disabled).toBe(true); // nothing changed yet
    act(() => fake.created[0].editor.type("two!!"));
    await waitFor(() => expect((screen.getByText("Save") as HTMLButtonElement).disabled).toBe(false));
    expect(screen.getByText(/a\.blif ●/)).toBeTruthy(); // marked as not saved
    fireEvent.click(screen.getByText("Save"));
    await screen.findByText("Saved.");
    const put = calls.find((c) => c.init?.method === "PUT")!;
    expect(put.url).toBe("/api/file?name=a.blif");
    expect(put.init?.body).toBe("two!!");
    expect((screen.getByText("Save") as HTMLButtonElement).disabled).toBe(true);
    expect(screen.queryByText(/●/)).toBeNull();
  });

  it("saves with Ctrl+S, and says when the session had read the file", async () => {
    vi.stubGlobal(
      "fetch",
      vi.fn((url: string, init?: RequestInit) =>
        init?.method === "PUT"
          ? reply(200, { ok: true, data: { name: "a.blif", bytes: 3, kind: "circuit", in_use: true } })
          : url === "/api/session"
            ? reply(200, emptyView())
            : reply(200, editable("one")),
      ),
    );
    render(<Workspace />);
    await openFile("a.blif");
    await waitFor(() => expect(fake.created).toHaveLength(1));
    act(() => fake.created[0].editor.type("abc"));
    const command = fake.created[0].editor.commands.find((c) => c.key === (2048 | 49))!;
    act(() => command.run());
    expect((await screen.findByRole("status")).textContent).toContain("read it again");
  });

  it("shows the reason when a save is refused, and keeps the text", async () => {
    vi.stubGlobal(
      "fetch",
      vi.fn((_url: string, init?: RequestInit) =>
        init?.method === "PUT"
          ? reply(400, { ok: false, diagnostics: [{ kind: "invalid_argument", message: "cannot be edited here", hint: "x" }] })
          : reply(200, editable("one")),
      ),
    );
    render(<Workspace />);
    await openFile("a.blif");
    await waitFor(() => expect(fake.created).toHaveLength(1));
    act(() => fake.created[0].editor.type("two"));
    fireEvent.click(await screen.findByText("Save"));
    expect((await screen.findByRole("status")).textContent).toContain("cannot be edited here");
    expect(fake.created[0].editor.value).toBe("two");
    expect((screen.getByText("Save") as HTMLButtonElement).disabled).toBe(false);
  });

  it("keeps any other file read only, and offers a copy that is opened for editing", async () => {
    const calls: string[] = [];
    vi.stubGlobal(
      "fetch",
      vi.fn((url: string, init?: RequestInit) => {
        calls.push(`${init?.method ?? "GET"} ${url}`);
        if (url.startsWith("/api/file/copy")) return reply(200, { ok: true, data: { name: "d.blif", bytes: 3, kind: "circuit" } });
        if (url === "/api/session") return reply(200, emptyView());
        if (url === "/api/file?name=d.blif") return reply(200, editable("copy"));
        return reply(200, file("orig", { editable: false, copyable: true }));
      }),
    );
    render(<Workspace />);
    await openFile("/in/d.blif".slice(1));
    await waitFor(() => expect(fake.created).toHaveLength(1));
    expect(fake.created[0].options).toMatchObject({ readOnly: true });
    expect(screen.queryByText("Save")).toBeNull();
    expect(screen.getByText(/read only/)).toBeTruthy();
    fireEvent.click(screen.getByText("Edit a copy"));
    await waitFor(() => expect(activeTab.value).toBe("file:d.blif"));
    expect(calls).toContain("POST /api/file/copy?name=in%2Fd.blif");
    await waitFor(() => expect(screen.queryByText("Save")).toBeTruthy());
  });
});
