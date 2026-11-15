# The local web app

A chat, the state of a session and a console of the OpenFPGA shell, in the browser. It listens on
the local machine only; on a remote workstation the browser reaches it through an SSH tunnel
(`ssh -L 8770:localhost:8770 host`). The layout is described in `docs/notes/ui_wireframes.md`.

**State of this version.** The page, the server and their security are built. The conversation is
a recorded one (`--demo`, with real answers of the tools), because the agent loop and the
connection to the tools are not built yet. The results tabs show the JSON of a result; their
drawings are not designed.

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

`--console` turns on the console. It runs any command of the OpenFPGA shell, so it is the user's
own shell and not a sandbox; the demo console knows only `help` and `report_session`.

## Layout of the code

| Where | What |
|-------|------|
| `openfpga_app/events.py`, `schemas/events.schema.json` | the events of a conversation (server-sent events) |
| `openfpga_app/backend.py` | what the server asks of a backend; `DemoBackend` replays a recording |
| `openfpga_app/security.py`, `server.py`, `cli.py` | the checks of every request, the API, the command |
| `frontend/src` | the page (Preact and TypeScript): `store.ts` has the state and the events |

## Tests

    pytest openfpga_flow/tasks/python_binding_tests/test_app_server.py   # the server
    cd frontend && npm test                                             # the components
    cd frontend && npx playwright install chromium && npm run e2e       # the page in a browser
