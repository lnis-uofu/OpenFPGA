# MCP server for OpenFPGA: tool design note

Status: reviewed. The server in `openfpga_mcp/` follows it, with 24 tools: 21 of the 23 of the design, `list_device_packages`, `use_device_package` and `write_file`: `get_journal` and `replay` wait for a journal in workers (G2), and the memory limit of a worker (G4) is not there yet. The directory is provisional: it depends on the answer to issue #2879, question 1.

This note fixes the tools, their arguments, the size of each answer and the limits, before the server is written. The server is a thin layer: every tool calls one method of `openfpga.RemoteSession` (see `openfpga/python/README.md`), and the facts in an answer are the ones of the queries, not text that the server writes.

## Principles

1. **Few tools, precise descriptions.** A small local model picks better among 20 tools with exact descriptions than among 60 with similar ones. A tool does one thing, and its description says when to use it and what it gives back.
2. **Bounded answers.** Every answer has a size budget (see below). When a list is cut, the answer says `truncated: true` and says how to get the rest, by name: a filter or a limit that the tool takes.
3. **Failures are data.** A command that fails does not raise an error to the model: the tool returns `ok: false` with the diagnostics (kind, message, file, line, hint), so the model can act on the kind.
4. **No path, no VPR type, no shell.** The model gives names relative to a workspace root. No tool runs a command line of the shell.
5. **Every action can be replayed.** Each session writes a journal; the model can read it, and a person can run it.

## Sessions

A session is a `RemoteSession`: a worker process, so that an abort of VPR, a kill or a timeout ends the session and not the server. The server keeps them by id in a registry.

- Each session has its own directory under the workspace root (`<root>/<session id>/`): the files of the flow are written there.
- Every worker is started from one thread that lives as long as the registry: the system ends a worker when the thread that started it ends (`PR_SET_PDEATHSIG`), and the threads that run the tools come and go.
- Limits, set in the server configuration: sessions open at once (default 4), the time of one call (default 600 s, the `timeout_s` of the worker), the memory of a worker, the idle time before a session is closed (default 1 h). A session that hits a limit is closed with a clear error; the others go on.
- After a crash or a timeout the tool answers `ok: false`, `kind: worker_crashed` (or `worker_timeout`), and says that `session_restore` can bring the session back from the last checkpoint. The server makes a checkpoint after each stage.

## The shape of every answer

```
{"ok": true,  "data": {...}, "truncated": false, "more": null}
{"ok": false, "diagnostics": [{"severity", "kind", "message", "file", "line", "hint", "data"}]}
```

`data` is the answer of the query, with its own `kind` and `schema_version` kept (`timing_summary`, `device`, ...), so a client can validate it against the schemas in `openfpga_flow/schemas`. `more` is a short sentence that names the argument to use: `"call find again with a narrower pattern or limit 50"`.

### Size budget

- Default: 4,000 characters per answer (about 1,000 tokens). The server configuration can raise it, up to 16,000. No tool exceeds the maximum, whatever the query returned.
- The queries that have `--limit` pass it down. For the ones that do not yet (G3 in the plan: `net_route`, `congestion` regions, `critical_paths` points, `timing_summary` clock domains, `report_app_option`), the server cuts the longest list of the answer from the end, sets `truncated`, and writes `more`. The queries get their own limit later; the tool arguments do not change when they do.
- Numbers are not rounded or summarised by the server.

## Tools

Arguments marked `?` are optional. `session` is the id that `session_open` gave.

### Sessions

| Tool | Arguments | Gives |
|------|-----------|-------|
| `session_open` | `name?` | the id, and the directory (relative to the workspace root) |
| `session_close` | `session` | `ok` |
| `session_restore` | `session`, `checkpoint?` | the state after the restore: from the last checkpoint of that session, or a named one |
| `session_list` | none | the open sessions: id, name, stages reached, whether the worker runs, idle time |
| `write_file` | `session`, `name`, `content`, `overwrite?` | writes a text file in `<session>/created/` only (plain name, an extension of a file that a flow reads, 256 KB, 50 files); `read_inputs` and `set_constraint` find it by name. Added after the design (owner, 2026-11-19) |
| `use_device_package` | `session`, `name` | reads the architectures of a package into a session that has read nothing (`session_not_empty` otherwise) and stops there: the circuit is next. Added after the design |
| `list_device_packages` | `name?` | the usable device packages with their device (layout, LUT size, channel width, tags), or with a name one package: files (role, path in the package, size), architectures, reference design, start of its README. Added after the design; the server needs `--device-packages FOLDER` |

### Flow

| Tool | Arguments | Gives |
|------|-----------|-------|
| `read_inputs` | `session`, `circuit`, and `vpr_arch` with `layout`, `openfpga_arch` | the state. The two architectures are optional when the session already has them (it was created from a device package, `openfpga_mcp/device_packages.py`): then only the circuit is read. The files are names inside the allowed designs folders (see Safety), or in the folder `uploads` of the session's own directory (what the web app puts there when a person gives a file to the session) |
| `run_stage` | `session`, `stage` (`pack`, `place`, `route`, `analysis`), `from_file?` | the stage, the seconds it took, the stages that are reached; the earlier stages not yet run are run first and listed in `ran`; on failure, the diagnostics and the `failed` record. With `from_file` the results are read from a file of the workspace (`read_packing`, `read_placement`, `read_routing`) instead of computed |
| `get_state` | `session` | `report_session` and `report_run`: stages reached, inputs, files of the results, `failed`, and what each stage did (clusters, placement cost, routing iterations, wirelength, seconds) |

### Options and constraints

| Tool | Arguments | Gives |
|------|-----------|-------|
| `list_options` | `session`, `stage?`, `query?`, `changed?`, `limit?` (default 20) | `report_stage`: for each stage asked for, its options with value, default and choices, and the stages that a change drops |
| `describe_option` | `session`, `name` | the option: type, value, default, help, choices, stage, and the stages that a change would drop |
| `set_option` | `session`, `name`, `value` | the `Change`: before, after, and the stages dropped |
| `set_constraint` | `session`, `kind` (`vpr_constraints`, `sdc`, `chan_width`), `file?`, `width?`, `clear?` | the `Change`. A file is a name inside the workspace |

### Queries (read only)

| Tool | Reads | Arguments | Notes |
|------|-------|-----------|-------|
| `get_timing` | `timing_summary`, `critical_paths` | `session`, `paths?` (default 3, at most 20) | the summary always, the worst `paths` paths with their points |
| `get_device` | `device` | `session`, `tile_type?`, `limit?` | works as soon as the inputs are read |
| `get_utilization` | `utilization` | `session` | |
| `find` | `find`, `report_cluster` | `session`, `pattern`, `kind?` (`block`, `net`, `cluster`), `limit?` (default 20) | by name pattern |
| `get_block` | `block_info` | `session`, `block`, `limit?` | pins and atoms; a long list is cut |
| `get_net` | `net_info`, `net_route` | `session`, `net`, `route?` (default false) | the route tree only when asked, and cut |
| `get_placement` | `placement` | `session`, `type?`, `prefix?`, `limit?` (default 20) | |
| `get_congestion` | `congestion` | `session`, `region_size?`, `hotspots?` (default 5) | works after a failed route (`routed: false`) |

A query on a stage that is not reached answers `ok: false` with the diagnostic `stage_not_reached` or `stage_failed`, which names the command to run.

### Logs, checkpoints, journal

| Tool | Arguments | Gives |
|------|-----------|-------|
| `read_log` | `session`, `stage`, `tail?` (lines, default 40) | the last lines of what the stage printed. Needs the log of each stage: the server keeps it (G5 in the plan) |
| `checkpoint` | `session`, `name?` | the name and the stages saved |
| `get_journal` | `session`, `format?` (`openfpga` or `python`), `tail?` | the commands that changed the flow; cut from the start if long. Needs a journal in workers (G2 in the plan) |
| `replay` | `session`, `journal` | a new session that has run the commands of a journal of the workspace. Needs G2 |

That is 23 tools. The first version has 24 (the 21, `list_device_packages`, `use_device_package` and `write_file`): `get_journal` and `replay` wait for a journal in workers (G2).

## Resources and prompts

Resources (read only, each bounded as an answer is):
- `openfpga://sessions/{id}/checkpoints` (the journal of a session waits for G2)
- `openfpga://options` (the stages with their number of options) and `openfpga://options/{stage}`: the catalogue of the options by stage, cut to the size of an answer
- `openfpga://stages`: the graph of stages (which stage requires which, the commands that reach it)

Prompts:
- `what_is_the_state(session)`: tells the model to call `get_state`, and what to say.
- `why_did_it_fail(session)`: tells the model to read `get_state`, then `get_congestion` or `read_log` according to the stage that failed, and to name the cause from the diagnostic kind, citing the number that supports it.

## Safety

- **Workspace root.** One directory, set when the server starts. Every file name in an argument is resolved inside it: no absolute path, no `..`, no symlink that leaves it (checked after resolving). A file the server writes goes in a session directory.
- **Inputs.** Architecture and circuit files are read from allowed folders listed in the configuration (the workspace, and for example `openfpga_flow/vpr_arch`), by name. Nothing else is readable.
- **No command execution.** There is no tool that takes a command line, and `session.command` is not reachable. `set_option` accepts the names of the option table only.
- **Sizes.** A file the model asks the server to create is not possible; the files written are the ones of the flow and the checkpoints, and a session directory has a size limit (`session_max_bytes`, 512 MB by default): a stage or a checkpoint is refused (`session_full`) when the directory is over it. Names and patterns that go to the shell are text of 1 to 200 characters with no double quote and no line break, and are data: `x;exit` is a name that is not found.
- **Time and memory.** A call that exceeds its time is stopped (`timeout_s`), and a worker that exceeds its memory limit is ended. Each gives a diagnostic, never a hung server.
- **Transport.** stdio first (the client starts the server). HTTP comes with the local web app and has the token and header checks described in issue #2879.

## What this needs from the rest of OpenFPGA

| Need | Where | Plan |
|------|-------|------|
| A journal in `RemoteSession`, and `replay` in a worker | `openfpga/python/openfpga_python_worker.py` | G2 |
| `limit` and `truncated` in the queries that list without one | the query commands | G3 |
| A memory limit for a worker | `RemoteSession` | G4 |
| The log of each stage, kept by the server (or a `report_log` query) | the server | G5 |

None of these blocks the first version of the server: it cuts lists itself, keeps the logs itself, and leaves out `get_journal` and `replay` until G2 is done.

## Testing

- **Transcripts.** A scripted client (no model) runs a flow through the tools and reads every query; the transcripts are golden files. A second one makes each kind of failure: a bad architecture, an unroutable width, a missing file, a killed worker.
- **Sizes.** A test runs every tool on a design and checks the answer against the budget, and that `truncated` and `more` are set when a list was cut.
- **Safety.** Tests of the escapes (`../`, an absolute path, a symlink that leaves the root), of the limits (sessions, memory, time) and of a call that never ends.
- **Schemas.** Every `data` validates against the schema of its `kind`.

## Decisions of the review (2026-11-14)

1. **Grouping of the queries: kept.** 8 query tools, not one tool per query kind (about 13) and not a single `query(kind, args)`. This differs from issue #2879, which says one tool per query kind. If the first measurements with the local models show wrong tool picks, `get_timing` and `get_net` are split; each tool is a thin wrapper, so this is a small change.
2. **Default sizes: kept.** 4,000 characters and 20 items. They are still to be checked against the first measurements, and the configuration lets them change.
3. **`read_log` is allowed**, and returns the last lines only, with paths inside the workspace shown as relative.

## Open points

1. Where the server directory lives (issue #2879, question 1).
