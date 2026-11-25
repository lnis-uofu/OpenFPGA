# What the model can do and what it can read

Status: draft 1, 2026-11-19. Local, not committed. Checked against the code of branch `gg_web_app`
(commit c02f68b3a): `openfpga/python/mcp/openfpga_mcp/`, `openfpga/python/app/openfpga_app/agent/`
and `openfpga/src/base/query_commands.cpp`.

The words "must", "may" and "does not" are used on purpose: each statement in sections 3 to 6 is
meant to be checked by a test or a scenario (section 8). Where the text says *the tool text*, it
quotes what the model is told in the definition of a tool.

## 1. Purpose and scope

This document says what a language model that is connected to OpenFPGA through the tools of
`openfpga_mcp` can **do** (its actions), what it can **read** (the answers and the files it can
name), what it **cannot** do, and the rules that it follows. It is written for three readers: a
person who asks the model for things (sections 2 and 7), a developer who changes the tools
(sections 3 to 6), and whoever measures a model (section 8).

The model works through **tools** only. It has no shell, no file system access of its own and no
way to run a command of the OpenFPGA shell: those belong to the person (the console of the web app).

## 2. What a person can ask (summary)

| Ask | Works | Notes |
|-----|-------|-------|
| Run the flow on a design at a given channel width, tell the Fmax | yes | pack, place, route, analysis, then timing |
| Change an option or a constraint, run again, compare | yes | the tool says which stages the change drops |
| Say why a stage failed (for example an unroutable design) | yes | from the diagnostics, the congestion hot spots and the log tail |
| Look at the device, the utilization, a block, a net, the placement, the congestion, the timing paths | yes | each answer is cut to a size limit and says how to get more |
| Go back to an earlier point after a crash or a bad change | yes | checkpoints |
| Find the smallest channel width that routes | no | out of scope (owner, 2026-11-19): the model runs the width it is given and does not search for one. The tools enforce it: `route` fails with `chan_width_required` while no width is set, and the one-shot `vpr` command is refused in `run_tcl` |
| Route flat (inside the clusters as well as between them) | yes | `set_option router.flat_routing true`; it costs memory and time, as the routing graph has the nodes inside the clusters. On a device package only if the package says it supports it (`flat_routing_unsupported` otherwise). The congestion, the run report and the routing of a net say `flat_routing`: the congestion and the wirelength count the channels only |
| Pack and place with the analytical placer of VPR | yes | `set_option place.placer analytical`: `run_stage` pack (or place) packs and places at once, as `vpr --analytical_place` does, and the two stages are one result (a change that drops the placement drops the packing). The options `aplace.*` tune it. `get_state` says the placer, and the estimates of the global placement |
| Improve Fmax with seeds and options | partly | needed; the tools exist, no strategy library (planned, M11). The options to try will come from the owner |
| Write an architecture, a constraints file, a circuit or a pin file | yes | `write_file`, in the directory of the session only; read back by name with `read_inputs` or `set_constraint`. Not outside the session, not a result of the flow |
| Answer questions about the OpenFPGA documentation | no | planned (M10) |
| Explain a failure beyond the diagnostics of the tools | no | planned (M10) |
| Synthesize Verilog for a device and run the flow on it | yes | `synthesize` (Yosys, with the script of the device package); the model does not write Yosys scripts |
| Generate fabric netlists, bitstreams, testbenches | no | the shell commands exist, there is no tool for them |

## 3. The answer that every tool returns

Every tool returns one JSON object, so a model learns one shape.

Success:

```
{"ok": true, "data": {...}, "truncated": false, "more": null}
```

- `truncated` is true when the answer was cut to the size limit; `more` then says how to ask for
  the rest (a smaller filter, a `limit`, a `tail`).
- A note may be added: `"note": "No session was open, so session s1 was opened for this call"`.

Failure (a failed call is a finished call, not an exception):

```
{"ok": false, "diagnostics": [{"severity", "kind", "message", "file", "line", "hint", "data"}]}
```

- `kind` is a stable word (section 5); `hint` says what to try, often the call to make next.
- A stage that failed also returns `reached` (the stages that are reached), `failed` (the stage and
  why) and, when earlier stages were run for it, `ran`.
- A call that was not made because an earlier call of the same reply failed gets the kind `not_run`.

Size: the default limit of an answer is 4,000 characters (about 1,000 tokens); a server may set a
limit between 500 and 16,000. A number is given as it is in VPR; the model must quote it as it is.

## 4. Actions: the 28 tools

`session` is an argument of every tool except `session_open`, `session_list` and `list_device_packages`; it is left out of
the table. A `?` marks an optional argument. "Needs" is the stage that must be reached; a query
that is asked too early fails with `stage_not_reached` and a hint.

### 4.1 Sessions

| Tool | Arguments | Effect | Result |
|------|-----------|--------|--------|
| `session_open` | `name?` | starts a session: a worker process with a directory of its own | the id (`s1`), the name, the directory |
| `session_close` | | ends the session and its worker, also while a stage runs; the files stay | confirmation |
| `session_list` | | none | the open sessions with their stages reached and idle seconds |
| `session_restore` | `checkpoint?` | starts a new worker for a session whose worker ended and goes back to the last finished stage, or to the named checkpoint; the stage that was running is lost | the state |
| `list_device_packages` | `name?` | none; read only | without a name, the usable device packages (name, version, description, layout, LUT size, channel width, tags, flat routing) and the damaged ones with the reason; with a name, its files (role, path inside the package, size), architectures, reference design and result, and the start of its README (`package_not_found` for an unknown name) |
| `use_device_package` | `name` | reads the VPR architecture (with the layout of its grid) and the OpenFPGA architecture of a device package into the session, and stops there: `read_inputs` with only `circuit` follows. It sets the channel width of the package and the options of its flow (`flow_defaults.vpr_options` of the manifest, for example `general.constant_net_method route`). Only a session with nothing read takes it (`session_not_empty`) | the package, `options_set`, `options_not_set` (with the reason, when there are), and the state |
| `write_file` | `name`, `content`, `overwrite?` | writes a text file in the folder `created` of the session directory and nowhere else. The name is plain (letters, digits, `. _ + -`, at most 100, no folder) with an extension that a flow reads (`.xml .blif .eblif .v .sv .vh .sdc .pcf .json .csv .txt`); at most 256 KB a file and 50 files; a name written before is kept unless `overwrite` (`file_exists`); the share of the session counts (`session_full`). The answer has the kind of the file (`vpr_arch`, `openfpga_arch`, `circuit`, `other`) and `xml_error` when an XML file is not well formed (entity declarations are not accepted). A text that has the two characters backslash and n and no line break is written with line breaks (the answer says `unescaped`); a text that has a line break is written as it is. The page lists the files under "Written by the model" | the name, the kind, the size |
| `synthesize` | `top`, `files`, `read?`, `lut_size?` | synthesizes Verilog (`.v`, `.sv`, `.vh`; at most 20 files of 1 MB) with Yosys into a netlist of the LUTs of the device: the files are names of the session (uploaded, or written with `write_file`) or of a designs folder, the top is the name of the top module. The device is the package of `use_device_package` (its LUT size and its Yosys script, with its techlib files and options); with no package `lut_size` (2 to 8) is given and a generic script is used. The netlist is `synth/<top>.blif` and the script that made it is `synth/synth.ys`, the only script of the session (each synthesis writes it again, so it does not take the name of a top module) (`.eblif` for a device whose manifest says `circuit_format: eblif`, the DSP device: its cells carry `.param`); with `read` (the default) it is read as the circuit when both architectures are read, so `run_stage` can follow. If the flow cannot read the netlist (for example a multiplier on a device with no DSP stays a cell that the device does not know) the call fails with `circuit_not_read`, not with a success, and says which cells (`subckts`) the netlist has. A source that takes a file from outside the session (`include, $readmem) is refused (`invalid_argument`); a Yosys error is `yosys_error` with the file and the line (a top that is not in the files lists the modules that are). Yosys runs in the worker of the session, set up once; an error does not end it | the netlist, LUTs, flip-flops, widest LUT, ports, the end of the log |
| `checkpoint` | `name?` | saves the flow under a name (the server also saves one after every stage) | the name and the stages saved |
| `get_journal` | `format?` (`openfpga`, `python`), `tail?` | the commands that changed the flow of the session, in order: the inputs, the options, the stages and the synthesis (a failed command is a comment); a long journal is cut from its start. Written by the worker in the folder `journal` of the session, and gone on with when a worker starts again or the session is opened again | the lines and the number of commands |
| `replay` | `journal`, `name?` | runs the journal of a session (its id) again in a new session; the first session does not change. Only commands that change the flow run, with files inside the workspace, the designs folders and the packages; a synthesis runs the script as it was run (the journal keeps it in `scripts/`). It stops at the first line that fails and the new session stays with what ran | the new session and how many commands ran |

Limits: at most 4 sessions (`too_many_sessions`), a call longer than 600 s ends the worker
(`session_restore` brings it back), a session idle for more than 3,600 s is closed, and the files
of a session may take at most 512 MB (`session_full`). The memory of a worker can be limited (`--worker-memory-mb`, off by default; below about 2,000 MB a worker does not start): a command that needs more fails with `out_of_memory` and the worker goes on. The web app opens a session by itself when a
tool needs one and none is open (the answer carries a note).

### 4.2 The flow

| Tool | Arguments | Effect | Needs |
|------|-----------|--------|-------|
| `read_inputs` | `circuit`, `vpr_arch?`, `layout?`, `openfpga_arch?` | reads the circuit (BLIF), the VPR architecture with the name of its device layout (a size such as `2x2`; never a file and never `auto`) and the OpenFPGA architecture. A session created from a device package has both architectures: only `circuit` is given. Each file is checked for its kind first (`wrong_file_kind`) | none; reaches DEVICE |
| `run_stage` | `stage` (`pack`, `place`, `route`, `analysis`), `from_file?` | runs the stage; the earlier stages that are not reached are run first and listed in `ran`. With `place.placer analytical`, pack places too, and place is not run again after it. With `from_file` the packing, placement or routing is read from a file of the workspace instead of computed (not for `analysis`). On failure the data of the stage is kept, so the queries of the earlier stages still answer | DEVICE; reaches PACKED, PLACED, ROUTED, ANALYZED |
| `get_state` | | the stages reached, the inputs, the result files, the stage that failed and why, and what each stage did (clusters, placement cost, routing iterations, wirelength, seconds) | none |
| `read_log` | `stage`, `tail?` | the last lines that a stage printed (default 40, at most 200) with their numbers; none for a stage that was read from a file | the stage ran |

### 4.3 Options and constraints

| Tool | Arguments | Effect |
|------|-----------|--------|
| `list_options` | `stage?`, `query?`, `changed?`, `limit?` | the options of a stage (or those that match a pattern such as `place.*`, or those not at their default) with value, default, choices and the stages that a change drops |
| `describe_option` | `name` | one option: type, value, default, help, choices, the stage it changes, the stages it drops |
| `set_option` | `name`, `value` | sets an option; the answer says which stages were dropped and must run again. An option that names a file is refused (`option_not_allowed`): use `set_constraint` or `read_inputs` |
| `set_constraint` | `kind` (`odc`, `vpr_constraints`, `sdc`, `chan_width`), `file?`, `width?`, `clear?` | sets or takes away an input that changes the whole flow; `odc` is the constraints of the design in one file (timing, synthesis, floorplan, routing, pins; docs/spec_odc.md), and the answer adds its warnings and the clocks and ports without timing; `chan_width` routes with a fixed number of tracks, which can be changed but not cleared (`chan_width_required`); `clear` takes a constraints file away |

| `check_constraints` | `limit?` | the ODC of the session checked again on what is read now: each command with its line and the objects its queries matched (default 20, at most 200), or the nearest names when one matched nothing; the errors and warnings; the clocks without a period and the ports without a delay; the files given to the tools (`constraints`) |

Changing an option or a constraint drops the stages after it; they must be run again.

### 4.4 Queries on the data of the flow (read only)

| Tool | Arguments | Result (`kind`) | Needs |
|------|-----------|-----------------|-------|
| `get_device` | `tile_type?`, `limit?` | the grid, the channel width, the tile types, the I/O of the sides; with `tile_type` where such tiles are (default 50) (`device`) | DEVICE |
| `get_utilization` | | blocks used for each type of block (`utilization`) | PACKED |
| `find` | `pattern`, `kind?`, `limit?` | blocks, nets or clusters by a shell-style pattern (`out:*`, `n1?`); default 20, at most 200; a block has its location once placed (`find`) | PACKED |
| `get_block` | `block` | a cluster: pins, nets, where it is placed, the circuit blocks it holds (`block_info`) | PACKED |
| `get_net` | `net`, `route?` | what a net connects, its bounding box, the size of its routing, its criticality; `route` adds the routing tree, which is long and cut (`net_info`, `net_route`) | PACKED; `route` needs ROUTED |
| `get_placement` | `type?`, `prefix?`, `limit?` | grid location of each block of a type and/or a name prefix; default 20, at most 200 (`placement`) | PLACED |
| `get_congestion` | `region_size?`, `hotspots?` | how full the channels are, by regions, and the fullest segments with their nets (default 5, at most 50). It also answers after a failed route (`routed` false): the shared nodes are the reason (`congestion`) | ROUTED, or a failed route |
| `get_timing` | `paths?` | for each pair of clocks the slack, critical path delay and Fmax, and the worst paths with their points (default 3, at most 20) (`timing_summary`, `critical_paths`) | ROUTED (the tool text says after analysis) |

Other kinds exist in the shell (`report_run`, `report_session`, `report_cluster`); the model reaches
their content through `get_state`.

### 4.5 Optional: Tcl scripts

With `--tcl-tool` (MCP server or app) the model has a 29th tool, `run_tcl(script)`: a Tcl script in a
safe interpreter of the session, for a loop or a sweep that would take many calls. Every command of
the shell is a Tcl command, a query returns a dict, and a failed command is an error that `catch`
takes. No files, no programs (`ext_exec`, `source` and `yosys` are refused, `tcl_refused`), and a
path must be in the session. The answer has the result, what `puts` printed, the last commands run
and the stages reached; a failure has `tcl_error` with the line of the script, then the diagnostics
of the command that failed. It is off by default: its description costs context, and a small model
does better with the tools above.

### 4.6 What the server also offers (MCP only)

The web app does not use these; an MCP client does. Resources: `checkpoints`, `stages` (the stage
graph), `options` (the catalogue), `stage_options`. Prompts: `what_is_the_state`,
`why_did_it_fail`.

## 5. What the model can interpret

### 5.1 The stages

`DEVICE` (inputs read) → `PACKED` → `PLACED` → `ROUTED` → `ANALYZED`. A stage needs the one before
it. `reached` in an answer lists them in this order. A failed stage is not reached; its data is
kept and `failed` says which one and why.

### 5.2 Diagnostics kinds that the model must understand

| Kind | Meaning | What to do (the hint says it too) |
|------|---------|-----------------------------------|
| `input_not_read` | an input of the flow was not read (the data says which) | call `read_inputs` |
| `stage_not_reached` | a query or stage needs an earlier stage | run the stage named in the hint |
| `stage_failed`, `route_failed` | a stage ended with an error | read the diagnostics after it; `read_log` for the router lines |
| `route_unroutable` | the router found no legal routing at this channel width (the data has the width) | raise `router.route_chan_width`, or change the placement; report it as the reason |
| `flat_routing_unsupported` | `set_option router.flat_routing true`, or `run_stage` route with it set, in a session of a device package that does not say it supports flat routing | set `router.flat_routing` to false; `list_device_packages` says which packages support it |
| `chan_width_required` | `run_stage` route or analysis with no channel width set, or a width that is cleared or not above 0 | set it with `set_constraint kind=chan_width`; a device package sets its own |
| `file_not_found`, `layout_not_found`, `load_error` | a file or a layout name does not exist or cannot be read | say so; do not guess another name |
| `wrong_file_kind` | a file is given for an input that it is not (for example an OpenFPGA architecture as the VPR one) | say what it is; the hint says where it belongs |
| `path_refused` | a path outside the workspace and the designs folders, an absolute path, or `..` | give a name only |
| `ambiguous_file` | a short name that two or more files under the designs folders have; the message lists their paths | give the path that the message shows |
| `invalid_argument`, `option_not_allowed` | an argument is wrong, or an option that names a file | read the message |
| `unknown_session`, `session_closed`, `too_many_sessions`, `session_full` | the session is not there, is closed, or a limit is reached | open or restore a session; close one |
| `yosys_error` | Yosys failed on the Verilog; the diagnostic has the file and the line, and `data.errors` has every error | fix the Verilog with `write_file` (when the person asked for it) and call `synthesize` again; otherwise say the file, the line and the message |
| `out_of_memory` | a command needed more memory than the worker may use (for example Yosys on a very large memory) | use a smaller design; the worker is the same and the session is as it was |
| `circuit_not_read` | `synthesize` made a netlist that the flow cannot read as the circuit (a cell that the device has no primitive for, a format); the data has what the netlist has | change the Verilog or use a device that has the cell; the stages cannot run until a circuit is read |
| `package_not_found`, `package_invalid` | a device package is missing or fails its checks | say so |
| `constraints_invalid` | `set_constraint kind=odc` with a file that has errors: one diagnostic per line, with `file` and `line`; a name that the flow does not know (a command, a port, a cell, a pin of the package) comes with the nearest names. The session keeps the constraints it had | fix the line with `write_file` and call `set_constraint` again; `check_constraints` lists what each query matched |
| `not_run` | the call was not made because an earlier call of the reply failed | repeat it after fixing the first |

### 5.3 Files the model can name

- Names only, relative to the workspace or to a designs folder (`--designs-dir`, once `--input-dir`): no
  absolute path, no `..`, no link out of the workspace. A short name that is not at the top of any
  folder is looked for under the designs folders (6 levels, 20,000 files; links and dot names skipped); two
  files of one name are `ambiguous_file`, never a choice. The system prompt lists the files that
  `read_inputs` can use by kind: the files of the session (uploaded or written, by kind) and, from the
  designs folders, only the BLIF circuits (a path when two have one name; 500 characters for each kind, a
  large folder is cut: planned fix `list_inputs`, T9.6). Architectures are not listed from a designs
  folder: they come from a device package (`list_device_packages`, `use_device_package`) or from files that
  the person names or uploads.
- Kinds: a VPR architecture (XML, root `<architecture>`), an OpenFPGA architecture (XML, root
  `<openfpga_architecture>`), a circuit (BLIF, starts with `.model`), a VPR constraints file (XML),
  an SDC file, an ODC file (`.odc`, the constraints of a design: docs/spec_odc.md), and the files that earlier stages wrote (packing, placement, routing) for
  `run_stage from_file`.
- A device package gives both architectures and the layout; the model gives the circuit.

### 5.4 Numbers

Slack, delay, Fmax, wirelength, counts and widths are quoted exactly as the tool returns them.
The model does not compute, round or convert them, and does not invent an option name, a file name
or a number: it looks it up (`find`, `list_options`, `describe_option`).

## 6. What the model cannot do

- Run a command of the shell, a script or any program; read or write a file by itself. (The person
  has a console in the web app, off by default.)
- Read a path outside the workspace and the folders of the inputs; use a network.
- Write or change an architecture, a circuit or a constraints file (it can set a constraint file
  that already exists).
- Run two sessions in one conversation at the same time (several sessions can exist; the app shows
  one).
- Generate a fabric netlist, a bitstream or a testbench; run a simulation.
- Search the documentation or explain a failure beyond the diagnostics and the log tail.
- Draw a result: the web app shows results as Markdown and JSON; the drawings are not made yet.

## 7. The rules that the model follows

These are the rules of the system prompt (`agent/prompt.py`) and of the loop (`agent/loop.py`).

| # | Rule | Where |
|---|------|-------|
| R1 | Use a session; if there is none, open one (the app also does it when a tool needs one). | prompt, loop |
| R2 | The order is `read_inputs`, then `run_stage` pack, place, route, analysis. `run_stage` runs the missing earlier stages. A change of option or constraint drops the stages after it. | prompt, tool |
| R3 | Use only what the tools return; quote numbers exactly; never invent a file, an option or a number. | prompt |
| R4 | A result marked `truncated` has more: ask again with a narrower filter or a smaller limit, as `more` says. | prompt |
| R5 | Make the calls yourself. Never end a reply by telling the person to run a stage or a tool. | prompt |
| R6 | Give `read_inputs` the files as the person named them, each in the role they said; never correct the choice or swap a file. If a file does not exist or is refused as `wrong_file_kind`, stop: say which file is of which kind (or that it does not exist), ask which file to use, run no stage. | prompt, loop (R11, R12) |
| R7 | When a call fails, read its diagnostics; do not repeat a call that failed in the same way. | prompt, loop |
| R8 | When there is an answer, say it briefly; if a stage failed, say which and why. | prompt |
| R9 | A note in square brackets at the start of a message comes from the system. | prompt |
| R10 | After a failure of order (`input_not_read`, `stage_not_reached`) a reply that makes no call is told once, by a system note, to make the call itself. | loop |
| R11 | Where the message says plainly "use F as the VPR architecture" (or the OpenFPGA architecture, or the circuit), `read_inputs` is called with F in that input, whatever the model wrote; the tool then judges the kind. The change is recorded (`as_stated`). | loop (`agent/stated.py`) |
| R12 | When `read_inputs` fails with `wrong_file_kind` or `file_not_found` on a call changed by R11, the model gets no tools for the next step and is asked to say what is wrong and ask which file to use (`final_only`). | loop |

Limits of one request (defaults of the loop): 20 steps, 900 seconds, a context of 8,192 tokens (the
app sets 16,384 for the larger models), 1,024 tokens per reply, the 2 most recent tool results kept
in full (older ones become one line), 3 identical calls in a row end the run, 3 failures of the same
kind in a row end the run. The reply then ends with "Stopped: <reason>."

## 8. Conformance: how a claim is checked

A scenario (`agent/scenarios.py`) has a task, a scripted plan that passes (CI, no model) and a
checker that reads the queries, not the model's words. A run also fails if a limit ended it.

| Scenario | Claim it checks | Rules |
|----------|-----------------|-------|
| `flow_fmax` | the whole flow at a given width, and the Fmax in the answer is the one of `get_timing` | R2, R3 |
| `set_width` | the width is set to the asked value and the design is routed | R2 |
| `missing_file` | a file that does not exist is named in the answer, and no stage is reached | R6, R7 |
| `wrong_kind` | a file of the wrong kind stops the run and is named; using another file fails the scenario even if the model says so (owner, 2026-11-15) | R6 |
| `unroutable_width` | a width of 2 makes the route stage fail and the answer says why | R7, R8 |
| `device_package` | the package is read into the session, then the flow runs and the Fmax is the one of `get_timing` | R2, R3 |
| `synth_flow` | `counter.v` is synthesized for the package (top `counter`), the netlist is read as the circuit, the flow runs at width 20 and the Fmax is the one of `get_timing` | R2, R3 |

Not yet covered by a scenario: a better Fmax by seeds, a congestion hot
spot, restore after a crash, truncated answers (R4), the first step of a conversation with no
session (R1).

Measured on the GPU (RTX PRO 4000 Blackwell), llama.cpp, native tool calls, five runs of each
scenario (T9.5). "Before" is 2026-11-17 on the CPU, before the changes of 2026-11-17 to 2026-11-19
(`run_stage` runs the earlier stages, R10 to R12, the wording of R6); "now" is 2026-11-19.

| Scenario | 7B before | 7B now | 14B before | 14B now |
|----------|-----------|--------|------------|---------|
| `flow_fmax` | 5/5 | 5/5 | 5/5 | 5/5 |
| `set_width` | 0/5 | 4/5 | 5/5 | 5/5 |
| `missing_file` | 5/5 | 5/5 | 5/5 | 5/5 |
| `wrong_kind` | 4/5 | 5/5 | 0/5 | 4/5 |
| `unroutable_width` | 0/5 | 3/5 | 5/5 | 5/5 |

Another full run of the same code gave 24/25 for the 7B and 25/25 for the 14B: the 7B varies by a
few runs between batches. Its failure on `unroutable_width` is that it raises the channel width that
the person set until the route passes; a prompt rule and a forced width both made it worse and were
removed. A model is called supported for this specification when it passes every scenario in at
least 4 of 5 runs: the 14B does as of 2026-11-19; the 7B does not.

## 9. Open points

- A check that this document and the code agree: a test that lists the tools of the server and
  fails if a name, an argument or a default in section 4 differs (the tool table can be generated
  from the definitions, as `DESIGN.md` is a hand-kept copy of them today).
- `list_inputs` (T9.6) and its place in section 5.3; the tool count then becomes 22.
- The limit of memory of a worker is not set (gap G4); sizes of some answers are not yet bounded
  (gap G3: `net_route`, `congestion`, `critical_paths`).
- Models for the GPU of the new machine (Qwen3-30B-A3B, gpt-oss-20b) are not measured.
