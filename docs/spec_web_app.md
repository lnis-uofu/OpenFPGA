# Spec: the OpenFPGA web app — user interface, reports and features

Status: draft 1, 2026-11-21. Written against branch `gg_web_ui` with the working tree of that day
(`openfpga/python/app/`, `openfpga/python/mcp/openfpga_mcp/`). Each requirement has a state:
**done** (in the code and tested), **partial** (works, with the gap named), or **planned**.
Section 9 is the list of what an expert user will ask for next; nothing in it is built.

Related documents: `docs/spec_device_package.md` (the device), `docs/spec_design_directory.md`
(the design), `docs/spec_model_capabilities.md` (what the model may do through the tools),
`openfpga/python/app/README.md` (how to install and run).

## 1. Overview

The web app is a local page for running an FPGA implementation flow on OpenFPGA: choose a device,
add a design, synthesize, pack, place, route, analyse the timing, and read what each stage did. It
has three ways to drive the same flow, which can be mixed at any time:

- **buttons** of the sidebar (a session from a device, a design, a run button for each stage);
- **the chat**, where a language model chooses the tool calls from a request in plain words, or
  where a person types a tool call as a line that starts with `/`;
- **the console**, which runs any command of the OpenFPGA shell in the session.

All three go through the tools of the MCP server (`openfpga_mcp`), so what one does the others
see: the sidebar follows a stage that the model ran, and the model is told what the person did.

The app is one process on one machine. It serves the page on the local interface, and a browser on
another machine reaches it through an SSH tunnel.

## 2. Glossary

| Term | Meaning |
|------|---------|
| Session | One flow on one device and one design, with a worker process and a directory `sN` in the workspace. |
| Workspace | The folder given with `--root`; every session directory is under it. |
| Stage | A step of the flow whose result is kept: DEVICE, SYNTHESIS, PACKED, PLACED, ROUTED, ANALYZED. |
| Device package | A directory that describes one device (`docs/spec_device_package.md`). |
| Design directory | A directory with a `design.json` that describes one design (`docs/spec_design_directory.md`). |
| Tool | A function of the MCP server. A person, the page and the model call the same tools. |
| View | The JSON that the server gives the page for the sidebar (`GET /api/session`). |
| Report | The HTML file that a stage writes about its own run (`<session>/reports/<stage>.html`). |
| Checkpoint | The saved state of a session after a stage, which a session can go back to. |

## 3. Goals and non-goals

Goals:

- A person who knows FPGA flows, and not OpenFPGA's commands, gets from a device and a design to a
  routed design with timing, and can see why a stage failed.
- Every action is visible and repeatable: a tool call is a card in the chat, a command is a line of
  the journal, and a session can be replayed.
- Nothing is hidden behind the model: every button and every answer of the model is a tool call
  that a person can make by hand.
- The session owns its inputs: the design is copied into the session, so a run can be read and
  repeated after the source folder changed.
- Safe on a shared workstation: local only, a token for every request, files only inside the
  workspace, the designs folders and the device packages.

Non-goals:

- A cloud service or a multi-user server. One person, one machine.
- A replacement of the OpenFPGA shell. The console is the shell; the app adds a flow around it.
- A waveform viewer, a schematic editor or an RTL editor with a language server.
- Running on Windows or macOS natively (the flow is Linux; the page is any browser).

## 4. Architecture in one page

```
browser (Preact page)  ──HTTP + server-sent events──  openfpga_app (Starlette)
                                                          │  SessionBackend
                                                          │  agent loop ── model endpoint (optional)
                                                          ▼
                                                      openfpga_mcp tools
                                                          │  one worker process for each session
                                                          ▼
                                                      OpenFPGA shell (VPR, Yosys)
```

- The page keeps no state of the flow. It draws the view and the events of a conversation.
- The server answers `GET /api/session` with the view, streams a conversation as events
  (`POST /api/chat`), and has endpoints for files, uploads, sessions, device packages and reports.
- A tool returns one JSON shape: `{"ok": true, "data": ...}` or `{"ok": false, "diagnostics": [...]}`.
- A session's directory holds `uploads/` (the person's files), `created/` (the model's files),
  `design/<name>/` (the copy of a design directory), `synth/` (the script and the netlist),
  `reports/`, `journal/`, the results of VPR and the checkpoints.

## 5. User interface

### 5.1 Layout

```
┌───────────────────────────────────────────────────────────────────────────┐
│ top bar: latest state of the session · model and context use · theme · ⚙  │
├───────────────┬───────────────────────────────────────────────────────────┤
│ sidebar       │ tabs: Chat │ result │ file │ report …            [split]  │
│  Session      │                                                           │
│  Stages       │ centre: the chat, or the tab that is selected             │
│  Designs      │                                                           │
│  Design files │                                                           │
│  Constraints  │                                                           │
│   and results │                                                           │
│  Checkpoints  │                                                           │
├───────────────┴───────────────────────────────────────────────────────────┤
│ console (optional, resizable)                                             │
└───────────────────────────────────────────────────────────────────────────┘
```

- **UI-1** (done) The page has a top bar, a sidebar that can be closed, a centre with tabs, and a
  console at the bottom when the app was started with `--console`.
- **UI-2** (done) The centre shows the chat alone until a tab is opened; "split" shows the chat and
  a tab side by side.
- **UI-3** (done) Light and dark themes; the choice is kept in the browser. Reports follow the
  setting of the system.
- **UI-4** (done) With no session and nothing said, the page is a first screen: one prompt, and
  "Create session", "Create session from device package…" and "Open session…".

### 5.2 Session section

- **UI-10** (done) Shows the id and name of the session, its device package with layout and LUT
  size, and whether the worker runs.
- **UI-11** (done) Buttons: new session, session from a device package, list of sessions (switch to
  a live one, or open a saved one from its last checkpoint), close, and restore when the worker ended.
- **UI-12** (done) A device package is checked before use; one that fails is listed with the reason
  and cannot be chosen.

### 5.3 Stages section

- **UI-20** (done) Lists DEVICE, SYNTHESIS, PACKED, PLACED, ROUTED, ANALYZED with a mark for each
  state (reached, not reached, failed, running, skipped), the time of the run, and a progress bar.
- **UI-21** (done) DEVICE is shown as reached as soon as both architectures are read, that is when
  a device package is loaded, before any circuit. It names the device package.
- **UI-22** (done) DEVICE has a button that opens the VPR and the OpenFPGA architecture files in
  tabs. The architecture files are not listed anywhere else.
- **UI-23** (done) SYNTHESIS, PACKED, PLACED and ROUTED have a run button. A flow stage runs the
  earlier stages that are missing. The synthesis button synthesizes again the design that the
  session added last, and is disabled until one is added.
- **UI-24** (done) SYNTHESIS is shown as skipped, with the reason, when the circuit is a netlist
  that was given.
- **UI-25** (done) A failed stage shows its message under its name.
- **UI-26** (done) A stage that has a report has a button that opens it in a tab (section 6).
- **UI-26a** (done) PLACED shows the placer of the session after its name, greyed out:
  `classic` (pack, then place) or `analytical` (the analytical placer of VPR packs and places at
  once), from the option `place.placer`. The sidebar does not set it: it is set with
  `set_app_option --name place.placer --value analytical` (the console), or by the model.
  ROUTED shows the routing the same way: `classic`, or `flat` when `router.flat_routing` is on.
- **UI-27** (planned) ANALYZED has a run button; a stage that runs shows a live progress line.

### 5.4 Designs and design files

- **UI-30** (done) "Designs" lists the design directories of the designs folders with the top
  module and the number of RTL files, or "netlist only".
- **UI-31** (done) "Add" copies the design directory into the session (`design/<name>/`) and
  synthesizes its RTL for the device of the session. Every Add copies again, in place of the
  earlier copy.
- **UI-32** (done) The RTL of a design is always what is used. A netlist of the design is read only
  when the design has no RTL; then Add copies the design and reads the netlist, and nothing is
  synthesized. `read_inputs` refuses a netlist of a design that has RTL and names the call to make.
- **UI-33** (done) "Design files" is one list: the copy of the design, the uploads, the files that
  the model wrote, the files of the synthesis, and a circuit read from a designs folder. A name
  opens the file; the circuit that the session read is in bold; there is no label beside a file.
- **UI-34** (done) Files are added with an upload button or a drop on the chat (50 MB a file,
  512 MB a session, a list of accepted extensions). An uploaded file can be removed.
- **UI-35** (partial) A design copy is read only in the viewer. Editing it needs "Edit a copy",
  and the edited copy is not what Add synthesizes.
- **UI-36** (done) "Constraints" shows the ODC of the session (read with `set_constraint
  kind=odc`): its file, the number of commands, errors and warnings, the defaults and the I/O map of
  the device package, the clocks and the ports without a delay, and the checks, where a click opens
  the file at the line. The files that the ODC gives VPR are not listed as an SDC or a VPR
  constraints file. Saving an ODC that the session read says that it must be read again.

### 5.5 Centre: chat, tabs and viewer

- **UI-40** (done) A request in plain words is answered by the model as text and tool calls; each
  call is a card with its arguments, its time and its answer. "Stop" ends the step that runs.
- **UI-41** (done) A line that starts with `/` is a tool call typed by the person (`/help` lists
  them). It works with no model.
- **UI-42** (done) The answer of a tool is shown as formatted text (a sentence, a table of fields,
  a table of rows); "JSON" shows what the model read.
- **UI-43** (replaced by UI-47) A result tab draws utilization, placement and congestion. These
  first drawings stay in the result tabs of the model's answers; the views of the design (UI-47)
  are the drawings of the device, the timing and the netlist.
- **UI-44** (done) A file opens in a tab with an editor (Monaco): colours for XML, Verilog, Tcl,
  BLIF and ODC, search, folding. Uploads and the model's files are edited in place; other files through
  "Edit a copy". Text only, one megabyte at most.
- **UI-45** (done) A report opens in a tab (REP-20).
- **UI-46** (done) The errors and warnings of the session's ODC (docs/spec_odc.md R-49) are marked
  on the lines of the file that is open, and the head of the file says how many.
- **UI-47** (done) The views of the design open in tabs from the stages that make their data
  (docs/plan_design_views.md): the floorplan (DEVICE: tiles, I/O sites, package pins, ports), the
  netlist (a circuit read: the atoms by module of the RTL), the clusters (PACKED), the placement
  (PLACED), the routing (ROUTED) and the timing (ANALYZED). They pan and zoom up to devices of
  500x500 (tiles of the screen, coarse levels far out), draw with WebGL (deck.gl) or Canvas2D, have
  a legend, and share one selection.
- **UI-48** (done) The model points at what it talks about: the tool `show` (select, highlight,
  zoom to a block, a net, an atom, a module or a path; open a view; the cone of an atom or the
  atoms of a path in the netlist) sends a command to the views of the page, logged in the console.

### 5.6 Console

- **UI-50** (done) With `--console`, a console runs any command of the OpenFPGA shell in the
  current session, except `vpr` (the whole flow in one command) and a route with no channel width
  set: the app routes only at a fixed width, and does not search for the smallest (FEA-35). Its height is set by dragging and kept in the browser.
- **UI-51** (done) What the person does in the console, with a button or with a `/` line is told
  to the model at its next message.
- **UI-52** (done) The console has a mode Tcl beside the mode Shell: Tcl scripts in the Tcl
  interpreter of the session, whose commands are those of the shell (a query answers as a dict, a
  failed command is an error that `catch` takes). Enter runs a whole script; an open brace goes
  to a new line. The variables stay until the worker ends; `exit` and `cd` are not available.
- **UI-53** (done) `openfpga-app tcl` runs the same Tcl from a terminal: attached to the app of a
  workspace (`--root`, through a file with its address and token that only its owner reads) or of
  an address (`--url`), in its current session or another (`--session`); or with `--local DIR`, a
  session of its own with no app. A line, a file of script, or lines typed at a prompt.

### 5.7 Feedback and errors

- **UI-60** (done) A refused action is a banner with the reason and a hint. A failed tool call is a
  finished card with its diagnostics, not an error of the page.
- **UI-61** (done) Buttons that cannot work are disabled with the reason in their title (the demo,
  no session, a call that runs, a worker that ended).

## 6. Reports

### 6.1 What is written and when

- **REP-1** (done) Each stage writes `reports/<stage>.html` in the session directory when it runs:
  `device`, `synthesis`, `pack`, `place`, `route`, `analysis`. A new run replaces the report.
- **REP-2** (done) The reports are written by the MCP server, so they exist whoever ran the stage:
  the page, the model or another MCP client.
- **REP-3** (done) A stage that runs earlier stages first (route runs pack and place) writes a
  report for each.
- **REP-4** (done) A stage that fails writes a report with the status "failed" and its reason as
  the first error.
- **REP-5** (done) The device report is written when the circuit is read, because the device data
  of VPR exists from then on.
- **REP-6** (done) `run_stage` and `synthesize` return the name of the report as `report`.
- **REP-7** (done) A report that cannot be written never fails the stage; the answer then has no
  `report`.

### 6.2 Content

Every report is the template `openfpga_mcp/templates/stage_report.html` with these parts, in order:

| Part | Content |
|------|---------|
| Header | title, status (passed, passed with warnings, failed), date and time |
| Cards | the time, the main figures of the stage, the number of errors and of warnings |
| Summary | stage, status, time, session, circuit, device package, layout, stages reached |
| Figures | tables of the stage (below) |
| Errors | each error once, the reason of a failure first |
| Warnings | each warning once, with how many times it was said |
| Log | the last 80 lines of the log of the stage, folded |

| Stage | Figures |
|-------|---------|
| device | grid, layers, layout, channel width; tile types with tiles, capacity, slots, I/O, pins |
| synthesis | top, sources, netlist; LUTs, flip-flops, widest LUT, inputs, outputs, constants; cells that are not LUTs |
| pack | clusters, atoms, nets; clusters by type; device utilization, limiting resources, use by block type |
| place | half-perimeter wirelength, costs, estimated critical path and Fmax, temperatures, swap attempts |
| route | channel width, wirelength, wires, router iterations, critical path |
| analysis | critical path, Fmax, setup and hold worst and total negative slack; clock domains |

- **REP-10** (done) Warnings and errors are taken from the log of the stage in the forms of VPR
  (`Warning 12: …`), Yosys (`Warning: …`, `ERROR: …`) and OpenFPGA (`[Warning] …`). A line that
  only counts them is not one.
- **REP-11** (done) A report is bounded: 200 different warnings and 200 errors of 400 characters,
  200 rows a table, 80 lines of log of 400 characters.
- **REP-12** (done) Every value is escaped. A report has no script and loads nothing.
- **REP-13** (done) The folder `reports` does not count in the size limit of a session.
- **REP-14** (partial) Warnings have no identifier and no severity beyond warning and error; the
  same message with another number in it is counted as another message.

### 6.3 In the page

- **REP-20** (done) The report button of a stage opens the report in a tab of the centre, one tab
  for each stage, in a frame that allows nothing (no script, no access to the page).
- **REP-21** (done) The tab loads the report again after each run; "Open in browser" opens it as a
  page of its own.
- **REP-22** (done) `GET /api/report?stage=S` serves the report of the current session with the
  token, and with a content policy that allows only inline styles. A name that is not a stage, or a
  stage with no report, is 404.

## 7. Features

### 7.1 Flow

- **FEA-1** (done) A session from a device package: both architectures are read, and the session
  waits for a circuit.
- **FEA-2** (done) Synthesis with Yosys inside the worker, with the script and the techlib of the
  device package, from a design directory (top, files, defines and include folders of its manifest)
  or from named Verilog files. A syntax error is reported with its file and line.
- **FEA-3** (done) Pack, place, route and timing analysis, each a stage with its own run; results
  of an earlier flow can be read from files instead.
- **FEA-4** (done) Options of VPR are listed, described and set; a change says which stages it
  drops. Timing (SDC) and VPR constraints are set from files of the session.
- **FEA-5** (done) Queries: device, utilization, placement, congestion, timing summary and critical
  paths, a block, a net, a search by name, the log of a stage.
- **FEA-6** (partial) The stages after routing: the bitstream is a stage of the page (`BITSTREAM`,
  with its run button and its report; docs/spec_bitstream.md B-43). Fabric netlists and testbenches
  are not: the shell has the commands, there is no tool and no stage.

### 7.2 Sessions

- **FEA-10** (done) Several sessions, each with its own worker; an idle one is closed after a time.
- **FEA-11** (done) Checkpoints after each stage and by name; a session is restored to one after a
  crash, a timeout or a bad change.
- **FEA-12** (done) A saved session (a directory of the workspace) is opened again after the
  server restarted, with its stages, options and device package. One that ran no stage has no
  checkpoint: it opens with its files and an empty flow, and reads its device package again.
- **FEA-13** (done) A journal of the commands that changed the flow, as OpenFPGA commands or as
  Python; `replay` makes a new session that runs a journal again.
- **FEA-14** (done) Limits: a time for each call, a memory limit for a worker, a size for a session.

### 7.3 The model

- **FEA-20** (done) Any OpenAI-compatible endpoint (a local llama.cpp server in practice). The
  model sees the tools and the list of files that it can name.
- **FEA-21** (done) Scenarios measure a model on fixed requests (`openfpga_app/agent/scenarios.py`).
- **FEA-22** (done) `--record-dir` writes each conversation with its calls, tokens and seconds.
- **FEA-23** (done) `--demo` replays a recorded conversation with no flow behind it.
- **FEA-24** (planned) Answers from the OpenFPGA documentation, and strategies for timing closure.

### 7.4 Security

- **FEA-30** (done) Local interface only unless `--allow-public-bind`. Every request needs the
  token (a `SameSite=Strict` cookie, or a header); a foreign `Host` or `Origin` is refused.
- **FEA-31** (done) A file name is relative, has no `..`, and resolves inside the workspace, the
  designs folders or the device packages. Links that leave them are refused.
- **FEA-32** (done) The model writes only in the session directory, only text files of kinds that
  a flow reads. Verilog that names a file outside the session is refused before Yosys runs.
- **FEA-33** (done) The console is the person's own shell and is off unless `--console` is given.
- **FEA-34** (done) The Tcl of the console and of the terminal is the person's own too (it has
  `exec` and `open`), behind `--console`. The model gets Tcl only with `--tcl-tool`, in a safe
  interpreter: no files, no programs, paths only in the session.
- **FEA-35** (done) The flow of the app runs the stages one by one at a fixed channel width. The
  search for the smallest width and the one-shot `vpr` command are not supported: the tools, the
  console, the Tcl of the console, of the terminal and of the model refuse `vpr`, and refuse
  `route` while no width is set (`chan_width_required` for the tools). The width cannot be
  cleared, only changed, and a device package sets its own.
- **FEA-36** (done) Flat routing (`router.flat_routing`) runs in the flow of the app, stage by
  stage (docs/plan_flat_routing.md). On a device package it runs only if the manifest says
  `device.flat_routing: true`: the tools (`flat_routing_unsupported`), the console and the Tcl
  refuse `route` otherwise. The drawing of the congestion says that the channels only are counted.
- **FEA-37** (done) The analytical placer of VPR runs in the flow of the app
  (docs/plan_analytical_placement.md), chosen with `set_app_option` or by the model, and shown
  next to PLACED (UI-26a). The run
  buttons of PACKED and PLACED both run it, once, and the reports of both stages are written. The
  report of the placement has the estimates of its global placement.

## 8. Tests

| What | Where |
|------|-------|
| The tools, the worker, safety, the reports | `openfpga_flow/tasks/python_binding_tests/test_mcp_*.py`, `test_worker*.py` |
| The server, the backend, the agent loop | `test_app_*.py` in the same folder |
| The components of the page | `openfpga/python/app/frontend`, `npm test` (Vitest) |
| The page in a browser | `npm run e2e`, `npm run e2e:session` (Playwright) |

A requirement that is **done** has at least one test in these suites. A recorded flow
(`golden/mcp_transcript.json`) fails when the answer of a tool changes.

## 9. Future enhancements for expert usability

An expert runs many variants of one design, reads numbers and not sentences, and wants to get from
a number to its cause in one step. The items are grouped by what the expert is doing, with the
most useful first in each group. None is built, except where an item is marked.

### 9.1 Compare runs

- **X-1** Run history: each run of a stage is kept with its options, seed and figures, in a table
  that sorts and filters (Fmax, wirelength, channel width, time).
- **X-2** Compare two runs or two sessions side by side: the figures with their difference, the
  options that differ, the warnings that are new or gone.
- **X-3** Sweeps as one action: seeds, a range of channel widths, a list of option sets, with a
  queue, a limit of workers and a table of results; "keep the best" makes it the session.
- **X-4** Trend charts over the runs of a design (Fmax, wirelength, utilization, time).
- **X-5** Thresholds: a run that is worse than a kept baseline by more than a tolerance is marked.

### 9.2 From a number to its cause (cross-probing)

- **X-10** (done: UI-47) A floorplan view of the device with zoom and pan: tiles by type, the
  placed blocks, the routing of a selected net, congestion as an overlay. It replaces the first
  drawings.
- **X-11** (done: UI-47) A timing path viewer: the points of a path with their delays as a bar,
  the path drawn on the floorplan, the slack histogram of all end points.
- **X-12** (partial) Selection is shared: a click on a block, a net or a path in one view selects
  it in the others, and offers "open the RTL line" when the netlist keeps the source attribute.
  The selection is shared; the RTL line is not offered yet.
- **X-13** A message of a report or a log links to what it names: the file and line, the net, the
  block, the option.
- **X-14** (partial) A hierarchy view of the design with the area and the worst slack of each
  module. The Design hierarchy and the netlist by module are built; the area and the slack of a
  module are not shown.

### 9.3 Reports for sign-off

- **X-20** A report index for the session: one page with the status of every stage and links, and
  a single-file bundle (HTML or PDF) to attach to a review.
- **X-21** A JSON file beside each report with the same figures, for scripts and for CI.
- **X-22** Message identifiers and severities (info, warning, critical warning, error), with
  filters in the report and counts by identifier.
- **X-23** Waivers: a file of the design that marks known messages as accepted, with a reason and
  an owner; a report shows waived messages apart, and a waiver that matches nothing is itself reported.
- **X-24** Reports that the flow does not have yet: clock summary, unconstrained paths, I/O and pin
  use, high-fanout nets, per-module utilization, hold analysis by corner.
- **X-25** Multi-corner timing in one report, with the corners of the device package.
- **X-26** A report template that a team can replace (logo, sections, extra checks) without
  changing the code.

### 9.4 Constraints and options

- **X-30** A constraints editor: ODC with completion of clock, port and cell names (its checks
  on the lines are UI-46); a pin planner that shows the I/O ring and writes the pin file.
- **X-31** An options panel: every option with search, its stage, its default, and "changed only";
  a set of options can be saved by name and applied to another session.
- **X-32** A floorplanning aid: regions drawn on the floorplan become placement constraints.
- **X-33** What-if without losing the run: branch a session at a checkpoint, change one thing,
  and compare the branch with the trunk.

### 9.5 Speed of use

- **X-40** A command palette (Ctrl+K) with every action, and keyboard shortcuts for the stages,
  the tabs and the panes.
- **X-41** Panes that dock and a layout that is saved; several tabs side by side, and a tab that
  can be moved to its own window.
- **X-42** A log tab that follows a running stage, with filters by severity and a search.
- **X-43** A scripting console in Python beside the shell console, with the session as an
  object (Tcl is done: UI-52, UI-53); the journal exported as a script that runs with no app.
- **X-44** Notifications when a long stage ends, and an estimate of the time from earlier runs.
- **X-45** Editing the copy of a design in place, with "synthesize what I edited" and a diff
  against the source folder.

### 9.6 Beyond one person and one page

- **X-50** A command line with the same actions and the same reports (`openfpga-app run`,
  `report`, `compare`), for scripts and CI.
- **X-51** A CI mode: a design and a device in, the reports and a pass or fail against thresholds out.
- **X-52** Read-only sharing of a session (its reports and views) with a link, for a review.
- **X-53** Design and device version in every report (the checksums of the design directory and
  of the device package), so that a result can be traced to its inputs.
- **X-54** The stages after routing in the page (FEA-6), with a report for each. Done for the
  bitstream: its file is in "Results", opened in the viewer and downloaded (`/api/download`).

### 9.7 The model as an assistant for experts

- **X-60** "Explain this": a question about a selected message, path or hot spot, answered from
  the data of the session and the documentation, with the calls that it made.
- **X-61** Strategies that the model can propose and run as a sweep (X-3), with the result as a
  comparison (X-2) and not as a sentence.
- **X-62** The model's edits to files shown as a diff that the person accepts or refuses.

## 10. Open questions

1. Should "Add" keep an existing copy of a design when the session edited it (X-45), or always
   copy again as today?
2. Does a report keep its earlier runs (needed for X-1 and X-2), and where: in the session
   directory, with a limit on how many?
3. Which identifiers do messages get (X-22): the ones of VPR where they exist, or a list of this
   project?
4. Which floorplan comes first (X-10): the placement grid redrawn, or a routing view? (Settled:
   both, in docs/plan_design_views.md.)
5. Is the report template (X-26) a file of the device package, of the design, or of the installation?
