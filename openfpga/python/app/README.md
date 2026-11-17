# The local web app

A chat, the state of a session and a console of the OpenFPGA shell, in the browser. It listens on
the local machine only; on a remote workstation the browser reaches it through an SSH tunnel
(`ssh -L 8770:localhost:8770 host`). The layout is described in `docs/notes/ui_wireframes.md`.

**State of this version.** The page, the server and their security are built, and the app works on
real sessions. With `--model-url` a model (llama.cpp's server, or any OpenAI-compatible endpoint)
chooses the tool calls of the MCP server from the chat, one step after another (the agent loop,
`openfpga_app/agent/`); without it a person calls the same tools from the chat with lines that
start with `/`, and runs commands of the shell in the console. `--demo` replays a recording
instead. The results tabs show the JSON of a result; their drawings are not designed.

## Install it

    # the page first (Node 18 or later); the build puts it in openfpga_app/static
    (cd openfpga/python/app/frontend && npm ci && npm run build)
    pip install '.[app]'            # or '.[mcp,app]'; gives openfpga-app and the page

`pip install .` does not run Node: it takes the files in `openfpga_app/static` as they are, and
the CI job builds them first. Without them the app says how to build the page.

## Run it

    # the front end, once (Node 18 or later; the bundle goes into openfpga_app/static)
    cd frontend && npm install && npm run build

    # real sessions, no model (the module `openfpga` and the folders of designs to read from):
    openfpga-app --root ~/workspace --designs-dir DESIGNS [--designs-dir DIR ...] \
        [--device-packages DIR] [--console] [--port N] [--open]
    # the recorded conversation:
    openfpga-app --demo
    # from the source tree: PYTHONPATH=openfpga/python/app:openfpga/python/mcp python -m openfpga_app ...

It prints an address with a token. Open it: the token goes into a cookie (`SameSite=Strict`) and
out of the address. Without it every call is refused, and so is a foreign `Host` or `Origin`. An
address that is not local is refused unless `--allow-public-bind` says that it is meant.

## With a model

    /localproj/projects/ggore/models/serve.sh 7b          # llama-server, Qwen2.5-7B, port 8081
    openfpga-app --root ~/workspace --designs-dir … --model-url http://127.0.0.1:8081/v1 \
        --model-name qwen2.5-7b [--tool-mode native|json] [--context-tokens 8192] [--record-dir DIR]

Type a request in plain words ("Run the flow on routing_test with a channel width of 20 and tell
me the Fmax"). Each tool call is a card; the sidebar follows; the top bar shows how full the
model's context is. Stop ends the step and the call that runs. `--tool-mode native` uses the tool
calls of the endpoint, `json` makes the model answer with a JSON object that the server
constrains to the tools (a smaller prompt, and a different way to go wrong). `--record-dir`
writes each conversation as lines of JSON (what the model said, each call and its answer, the
tokens and the seconds). What the person does meanwhile (console, buttons, `/` lines) is told to
the model at its next message. All 21 tools are shown to the model; the files it can read are
listed at the start of the prompt (at most 1,500 characters).

## The first screen

With real sessions the page starts on an empty screen: a prompt in the middle, and at the bottom the
two ways to start a session, "Create session" and "Create session from device package…" (it lists the
packages to choose from). A session created from a button gives the full interface at once.

What you ask from the prompt is answered on the same screen, in a small window below it (the text as it
comes, the tool calls, the state of the session on top, a Stop button in place of Send). The screen
stays while the model works, and gives the full interface when the answer is complete and a session
exists, that is when the session is loaded. An answer that needs no session stays on the screen; "Open
the full interface ›" leaves it. If the session is closed later the first screen does not come back
(only when nothing was ever said). The recorded demo starts on the full interface.

## Creating a session from a device package

A device package (`openfpga_flow/device-packages/`, format in `docs/spec_device_package.md`) holds the
files of one device. "from device…" under Session lists the packages; "Create session" opens a
session, reads the VPR architecture (with the layout that the package's grid names) and the OpenFPGA
architecture from the package, and stops there: the sidebar says it waits for a circuit, and nothing
is run. Give the circuit and run the stages as usual: a circuit that you upload (a `.blif`, found by
its first lines) is read by itself, as `read_inputs` with only the circuit; or ask the model for a
circuit of the designs folders (they are looked for under the folders, by their name; the prompt of the model
lists only the BLIF circuits of them, a path when two have one name, and the architectures come from a
device package or from files that you name or upload), or type `/read_inputs circuit=NAME` (the architectures need not be given
again). A circuit is read by itself only when the session has both architectures and no circuit yet: a
second upload is only copied, because reading another circuit drops the stages that were run. If the
reading fails, the file stays uploaded and a banner gives the reason. The model is told what was done.

A package is checked before it is used (format version, name, license, that its paths are files inside
it, the layout of the grid, its checksums); a package that fails is shown as such with the reason, and
no session is made. Nothing is written into a package. The folders of packages are `--device-packages
DIR` (repeat it), or `openfpga_flow/device-packages` of `$OPENFPGA_PATH` when that is set.
Not in the demo.

## Results as formatted text

The answer of a tool (in a card of the chat, in the window of the first screen and in a result tab) is
shown as formatted text and not as JSON: a sentence for the simple tools (`run_stage`, `session_open`,
`set_option` ...), a table of fields for an answer with fields, a table of rows for a list of records
(at most 25 rows, with how many more there are), a heading for each part, numbers to five digits,
names in code, long text in a code block, and the diagnostics of a failure with their hint. "JSON"
shows what the model read, unchanged; "Copy JSON" copies it. Only the page changes: the answers of the
tools are the same.

## Opening files, and the console

A click on a file name in the sidebar (an input, an uploaded file, a result such as the `.net`, `.place`
or `.route` of the flow, a file of a device package) opens it in a tab of the centre, in Monaco, the
editor of VS Code, read only: colours for XML (the architectures and the packing), SystemVerilog and
Tcl (the constraints), line numbers, folding, search (Ctrl+F) and go to line (Ctrl+G). "Reload" reads
the file again (a result may have changed), "Copy text" copies it. The small ⧉ beside a name copies the
name. The server reads only what the tools may read (the directory of the session with its uploads, the
workspace, the designs folders, the device packages), only text, and at most one megabyte of it.
Not in the demo, which has no files.

Monaco is a separate chunk of about 3 MB (800 kB compressed) that the page loads the first time that
a file is opened: the page does not carry it before. It is bundled in `openfpga_app/static/` by
`npm run build` (the package `monaco-editor` is a development dependency; a user needs no Node).

The console (`--console`) is made higher or lower by dragging its top edge, or with the arrow keys when
the edge has the focus; a double click gives the usual height back. The height is kept in the browser.

## Giving files to a session

"⤒ Upload files…" in the sidebar (Design files, under Inputs), or a drop of files on the chat, copies
them into the session: the folder `uploads` of the session's directory in the workspace. If there
is no session, one is opened. The tools and the model use them by their plain name, as they use the
files of the designs folders (`read_inputs ... circuit=mine.blif`); another session does not see them.
What a file is (VPR architecture, OpenFPGA architecture, circuit) is read from its first lines and
shown next to it, and the model is told at its next message that the file is there. A name that
is taken is replaced only if you say so. The ✕ beside a file in the list deletes it from the
session (after a question), and the model is told. Files are per session; there is no folder
shared by all of them. Accepted: `.xml .blif .eblif .v .sv .vh .sdc .pcf .json
.csv .txt`, a plain name (letters, digits and `. _ + -`, at most 100), 50 MB at most for a file,
and what a session may hold in all (512 MB). Not in the demo.

## Without a model

The chat takes lines that start with `/`: `/help` lists the tools, `/session_open`,
`/read_inputs vpr_arch=… layout=2x2 openfpga_arch=… circuit=…` (names inside the designs folders),
`/run_stage stage=pack`, `/get_timing`, `/find pattern=*`, ... The session is the current one if
`session=` is left out; `/use s2` changes it. A line that does not start with `/` is answered
with a reminder that no model is connected. The buttons of the sidebar open, close and restore
sessions, and each checkpoint has a restore.

`--console` turns on the console. It runs any command of the OpenFPGA shell, so it is the user's
own shell and not a sandbox; with `--root` it runs in the current session (what it prints is the output); the demo console
knows only `help` and `report_session`. A command typed there is not in a journal yet (workers
have none, G2), so it cannot be replayed.

## Layout of the code

| Where | What |
|-------|------|
| `openfpga_app/events.py`, `schemas/events.schema.json` | the events of a conversation (server-sent events) |
| `openfpga_app/backend.py` | what the server asks of a backend; `DemoBackend` replays a recording |
| `openfpga_app/agent/` | the loop (`loop.py`), the model adapters (`openai_compat.py`, `scripted.py`), the definitions of the tools (`schema.py`), the context (`context.py`), the JSON mode (`jsonmode.py`) |
| `openfpga_app/session_backend.py` | the backend over real sessions: the tools of `openfpga_mcp`, the view, the console |
| `openfpga_app/security.py`, `server.py`, `cli.py` | the checks of every request, the API, the command |
| `frontend/src` | the page (Preact and TypeScript): `store.ts` has the state and the events |

## Tests

    pytest openfpga_flow/tasks/python_binding_tests/test_app_server.py   # the server
    pytest openfpga_flow/tasks/python_binding_tests/test_app_session.py  # over real sessions
    pytest openfpga_flow/tasks/python_binding_tests/test_app_agent.py    # the loop, with a scripted model
    cd frontend && npm test                                             # the components
    cd frontend && npx playwright install chromium && npm run e2e       # the page, demo
    cd frontend && npm run e2e:session                                  # the page, real session
