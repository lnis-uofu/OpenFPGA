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
        [--device-packages DIR] [--console] [--tcl-tool] [--port N] [--open]
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
the model at its next message. All 27 tools are shown to the model; the files it can read are
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

**The sidebar.** Stages: SYNTHESIS comes first, then the five stages of the flow. Synthesis is reached when
the circuit is the netlist of a `synthesize` call (a file of the session's `synth` folder), failed when the
last one failed (with its message), and greyed out (skipped, struck through, not counted in the progress bar)
when the design is a netlist that was given, with the reason as its title; it is not reached before a circuit
is read. Design files: one line for the architecture, with the device package and the layout as labels and a
button "Show architecture" that opens the VPR and the OpenFPGA architecture files in tabs (they are not
listed as files), then one list of every design file (what you uploaded, what the model wrote, what a
synthesis made, and the circuit read from a designs folder), each with its kind and its origin, the circuit
that the session has read marked "loaded", and the upload button. Constraints, results and the options that
changed are in their own section. The view has `synthesis` (status, seconds, message) beside `stages`.

A click on a file name in the sidebar (an input, an uploaded file, a result such as the `.net`, `.place`
or `.route` of the flow, a file of a device package) opens it in a tab of the centre, in Monaco, the
editor of VS Code: colours for XML (the architectures and the packing), SystemVerilog and
Tcl (the constraints), line numbers, folding, search (Ctrl+F) and go to line (Ctrl+G). "Reload" reads
the file again (a result may have changed), "Copy text" copies it. The small ⧉ beside a name copies the
name. The server reads only what the tools may read (the directory of the session with its uploads, the
workspace, the designs folders, the device packages), only text, and at most one megabyte of it.
Not in the demo, which has no files.

**Opening a session.** The first screen has "Open session…": a list of the sessions that can be opened.
A live one (a worker holds it) is switched to; a saved one is a directory `sN` of the workspace that no
worker holds (it was closed, or an earlier run of the server made it): a worker is started under the same id,
from the last checkpoint, with its stages, options and device package (the name and the package are kept in
`session.json` in its directory). A session that ran no stage has no checkpoint (it only copied or synthesized
a design): it opens with its files and an empty flow, and a session of a device package reads the
architectures of its package again. The model is told that the session changed. `GET /api/sessions` lists, `POST /api/session/open`
(`{session}`) opens; the sidebar has no list yet.

**Editing.** The files of the session's `uploads` (what the person gave) and of `created` (what the model
wrote) are edited in the tab: "Save" (or Ctrl+S) writes the text in place, a dot after the name says that
there are changes that are not saved, and "Reload" asks before it throws them away. Any other file of a kind
that a flow reads (an input of a folder, a package file) stays read only; "Edit a copy" copies it into the
session's uploads, under its own name, and opens the copy (a copy that is there is not replaced). Results
and the files of packages are never changed. The model is told that the person edited a file; if the session
had read it as an input the answer says so, and `read_inputs` must read it again for the change to count.
The text saved is UTF-8, at most one megabyte. `PUT /api/file?name=N` saves and `POST /api/file/copy?name=N`
copies, with the token as the other endpoints; not in the demo. BLIF has its own colours (`blif.ts`).

Monaco is a separate chunk of about 3 MB (800 kB compressed) that the page loads the first time that
a file is opened: the page does not carry it before. It is bundled in `openfpga_app/static/` by
`npm run build` (the package `monaco-editor` is a development dependency; a user needs no Node).

The console (`--console`) is made higher or lower by dragging its top edge, or with the arrow keys when
the edge has the focus; a double click gives the usual height back. The height is kept in the browser.

## Giving files to a session

"⤒ Upload files…" in the sidebar (Design files, under the list of files), or a drop of files on the chat, copies
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
knows only `help` and `report_session`. Two things are left out, as everywhere in the app, whose flow runs
the stages one by one at a fixed channel width: `vpr`, which runs the whole flow in one command, and `route`
while `router.route_chan_width` is not set (VPR would search for the smallest width). A route with
`router.flat_routing` set is also left out on a device package that does not support flat routing
(`device.flat_routing` in its manifest). A device package sets
its own width; the model sets one with `set_constraint kind=chan_width`. The Tcl of the console, of
`openfpga-app tcl` and of the model refuses the same (`Session.fixed_width_refusal`). The commands that change the flow are in the journal of the
session, as those of the tools are.

## Tcl

The console has two modes, **Shell** and **Tcl** (kept in the browser). In the mode Tcl it runs
Tcl scripts in the Tcl interpreter of the session (`Session.tcl`, in its worker): every command of
the shell is a Tcl command, a query returns its answer as a dict, and a command that fails is an
error that `catch` takes. Enter runs a script whose braces, brackets and quotes are closed, and
goes to a new line otherwise; Shift+Enter is always a new line. The variables and procedures stay
until the worker ends. `exit` and `cd` are not available; `source` is the one of Tcl and
`openfpga::source` the one of the shell.

The same interpreter is reached from a terminal:

    openfpga-app tcl --root ~/workspace            # the app that runs on ~/workspace (--console)
    openfpga-app tcl --root ~/workspace flow.tcl   # run a script and end (-c SCRIPT for a line)
    openfpga-app tcl --url 'http://localhost:8770/?token=…' --session s2
    openfpga-app tcl --local run1 [flow.tcl]       # no app: a session of its own, in run1

An app started with `--console` writes its address and token in `.openfpga-app.json` of the
workspace (readable by its owner only, removed when it stops), which `--root` reads. What a script
does is on the page, in the journal, and told to the model, as for the console of the page. With
`--local` the session is a worker of the command, with its journal and checkpoints in the folder;
Ctrl+C stops a script and starts the worker again from the last checkpoint.

`--tcl-tool` gives the model the tool `run_tcl`: the same commands in a safe interpreter (no files,
no programs, paths only in the session), see the README of `openfpga_mcp`.

## Layout of the code

| Where | What |
|-------|------|
| `openfpga_app/events.py`, `schemas/events.schema.json` | the events of a conversation (server-sent events) |
| `openfpga_app/backend.py` | what the server asks of a backend; `DemoBackend` replays a recording |
| `openfpga_app/agent/` | the loop (`loop.py`), the model adapters (`openai_compat.py`, `scripted.py`), the definitions of the tools (`schema.py`), the context (`context.py`), the JSON mode (`jsonmode.py`) |
| `openfpga_app/session_backend.py` | the backend over real sessions: the tools of `openfpga_mcp`, the view, the console |
| `openfpga_app/security.py`, `server.py`, `cli.py` | the checks of every request, the API, the command |
| `openfpga_app/tcl_cli.py` | `openfpga-app tcl`: Tcl in a terminal, over a running app or a session of its own |
| `frontend/src` | the page (Preact and TypeScript): `store.ts` has the state and the events |

## Tests

    pytest openfpga_flow/tasks/python_binding_tests/test_app_server.py   # the server
    pytest openfpga_flow/tasks/python_binding_tests/test_app_session.py  # over real sessions
    pytest openfpga_flow/tasks/python_binding_tests/test_app_agent.py    # the loop, with a scripted model
    cd frontend && npm test                                             # the components
    cd frontend && npx playwright install chromium && npm run e2e       # the page, demo
    cd frontend && npm run e2e:session                                  # the page, real session
