# The local web app

A chat, the state of a session and a console of the OpenFPGA shell, in the browser. It listens on
the local machine only; on a remote workstation the browser reaches it through an SSH tunnel
(`ssh -L 8770:localhost:8770 host`). The layout is described in `docs/notes/ui_wireframes.md`.

**State of this version.** The page, the server and their security are built, and the app works
on real sessions with **no model**: a person calls the tools of the MCP server (the same that a
model will get) from the chat, and runs commands of the shell in the console. `--demo` replays a
recording instead. The agent loop that chooses the calls is not built yet. The results tabs show
the JSON of a result; their drawings are not designed.

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
| `openfpga_app/session_backend.py` | the backend over real sessions: the tools of `openfpga_mcp`, the view, the console |
| `openfpga_app/security.py`, `server.py`, `cli.py` | the checks of every request, the API, the command |
| `frontend/src` | the page (Preact and TypeScript): `store.ts` has the state and the events |

## Tests

    pytest openfpga_flow/tasks/python_binding_tests/test_app_server.py   # the server
    pytest openfpga_flow/tasks/python_binding_tests/test_app_session.py  # over real sessions
    cd frontend && npm test                                             # the components
    cd frontend && npx playwright install chromium && npm run e2e       # the page, demo
    cd frontend && npm run e2e:session                                  # the page, real session
