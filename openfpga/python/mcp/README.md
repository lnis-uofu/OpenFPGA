# openfpga-mcp: a server of the Model Context Protocol for OpenFPGA

It lets a language-model client (a local model behind an MCP-capable chat or agent, or any
other MCP client) run the place-and-route flow of OpenFPGA stage by stage and read what it did,
with small answers and no access to the machine beyond one workspace. The tools, the sizes of
the answers and the limits are in [DESIGN.md](DESIGN.md).

## Install

The server is part of the `openfpga` Python module, and is off unless asked for. It needs Python
3.10 or later (the SDK of MCP does):

    pip install '.[mcp]'            # in the OpenFPGA source tree, see the manual for the build

This gives the module `openfpga`, the package `openfpga_mcp` and the command `openfpga-mcp`.
Without the extra the package is there and says what is missing when it starts.

## Run

    openfpga-mcp --root WORKSPACE --input-dir DIR [--input-dir DIR ...]

- `--root` is the workspace. Each session has a directory in it, and nothing outside it is
  written. Give it a directory that you do not mind a model filling.
- `--input-dir` is a folder that architectures, circuits and constraints can be read from, by
  name. Nothing else on the machine can be read.
- `--transport http --port N` serves on `127.0.0.1` instead of stdio (a request with a `Host` or
  `Origin` of another site is refused). Another address is refused until the access checks of
  the web app exist.
- Limits: `--max-sessions` (4), `--call-timeout` (600 s), `--idle-timeout` (3600 s),
  `--answer-chars` (4,000; at most 16,000), `--session-max-mb` (512).

## A client

Most clients take a file like [examples/client_config.json](examples/client_config.json): the
command to start, and its arguments (the form that Claude Desktop uses; others read the same
keys). The client starts the server and speaks to it on its standard input and output.

Without a client, a generic stdio test of the command is

    python -m openfpga_mcp --root /tmp/work --input-dir openfpga_flow/vpr_arch

and a client of the SDK reads it with `mcp.client.Client(StdioServerParameters(...))`.

## A flow

A model opens a session, reads the inputs (`vpr_arch`, `layout`, `openfpga_arch` and `circuit`
are names inside the input folders), runs `pack`, `place`, `route` and `analysis` with
`run_stage`, and asks `get_timing`, `get_utilization`, `get_congestion`, `find` and so on. A
stage that fails comes back as `ok: false` with diagnostics, and what it made is kept for
`get_congestion` and the earlier queries. `session_restore` brings a session back after a crash
or a timeout.

## Tests

    pytest openfpga_flow/tasks/python_binding_tests/test_mcp_*.py

They need the SDK (`pip install 'mcp>=2,<3'`) and a built module; they are skipped without them.
