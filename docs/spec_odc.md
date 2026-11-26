# Spec: ODC, the constraints file of OpenFPGA

Status: draft for review. Implemented (phases 3 to 5 of `docs/plan_odc_constraints.md`): `libs/libodc` (the reader, the checks, the writers), the commands of the plugin `ui`, the tools of the model (R-48) and the web app (R-49). Not supported: the custom properties of a `pcf_config.xml` (R-35). Not yet: the objects of the synthesis commands reported from the log of Yosys (R-23), and the share of the endpoints that are constrained (R-41).

## Overview

An **ODC file** (OpenFPGA Design Constraints, `.odc`) holds the constraints of one design in one place, as an XDC file does for Xilinx devices: timing, synthesis, floorplanning, routing and pins. The flow reads it once, and each step gets the part it needs:

- Yosys gets the synthesis part;
- VPR gets an SDC file and a VPR constraints XML;
- the OpenFPGA commands after routing get a pin constraints XML and bitstream settings.

Today each kind of constraint has its own format, read by a different tool at a different time. Nothing checks them against the design, and some mistakes are silent:

- VPR skips a name that matches no atom, with only a warning;
- VPR drops a name of an SDC list that matches nothing, without a word;
- VPR fails with a misleading packing message when a block is placed on a tile of another type.

ODC checks every constraint against the design and the device, and says what it matched, before the flow runs.

ODC is also the place where any later kind of constraint goes: each command is a row of a table (R-60).

## Glossary

| Term | Meaning |
|------|---------|
| ODC file | A file of commands as described here, ending in `.odc`. |
| Command | One line of an ODC file (after joining continued lines), for example `create_clock ...`. |
| Object query | A bracketed command that names objects, for example `[get_ports clk]`. |
| Location | A place in the VPR grid: `X3Y2`, with a subtile `X3Y2/1`, a range `X1Y1:X4Y6`. |
| Pblock | A named region of the grid that cells are kept in. |
| I/O map | The optional CSV of a device package that names the I/O subtiles (spec_device_package.md, R-25). |
| Consumer | The tool that a command is for: Yosys, VPR's SDC, VPR's constraints, OpenFPGA's pin constraints, the bitstream settings. |

## Goals and non-goals

Goals:

- One file for all the constraints of a design, which a person, a script or a language model can write.
- The same file reads the same in the shell, in Tcl, in Python, through the MCP server and in the web app.
- Every constraint is checked against the netlist and the device, and what matched nothing is reported with its line.
- An SDC file that VPR reads today is a valid ODC file.

Non-goals:

- Evaluating Tcl. The file is parsed, not run: no variables, loops, procedures or expressions (R-2).
- The channel width. It stays a setting of the flow (`router.route_chan_width`).
- Reading Xilinx XDC files. Importing a subset of XDC is for later.

## Format

### Lines and words

- **R-1** A file is UTF-8 text. A command is one line. A line that ends with `\` continues on the next line. A `#` outside quotes and braces starts a comment, to the end of the line. Blank lines are skipped.
- **R-2** A command is words separated by spaces and tabs. A word is:
  - a bare word (`clk`, `-period`, `2.5`, `X3Y2/1`);
  - a quoted word, `"..."`, where `\"` and `\\` are the only escapes;
  - a braced word, `{...}`, taken as it is, nested braces included, as a list of words;
  - an object query, `[...]` (R-10).

  `$` has no meaning: there are no variables. A `[` that does not start a word is part of the word, so `a[0]` is one bare word.
- **R-3** The first command of a file is `odc_version 1`. A file without it is read as version 1, with a warning. A higher version than the reader knows is refused.
- **R-4** A command that the reader does not know is an error with the file and the line. With `read_odc --lenient` it is a warning, and the command is skipped.
- **R-5** Times are in ns and frequencies in MHz, as in SDC. A number is a decimal number, with an optional exponent.

### Locations

- **R-6** A location is written in the coordinates of the VPR grid, as the `.place` file and the VPR constraints XML use them: `x` is the column and `y` the row, with `(0,0)` at the bottom left.
  - `X<x>Y<y>` is a tile.
  - `X<x>Y<y>/<s>` is the subtile `s` of a tile.
  - `X<x1>Y<y1>:X<x2>Y<y2>` is a rectangle, corners included.
  - `-layer <n>` gives the layer of a 3D device; it is 0 by default.
- **R-7** A location outside the grid of the device is an error. A subtile that the tile does not have is an error.

### Object queries

- **R-10** An object query is `[get_<kind> <options> <patterns>]`, with these kinds:

| Query | Objects | Names from |
|-------|---------|------------|
| `get_ports` | the top ports of the design | the netlist (the RTL for synthesis commands) |
| `get_cells` | cells: registers, hard blocks (memory, DSP), LUTs, instances | the netlist (the RTL for synthesis commands) |
| `get_nets` | nets | the netlist (the RTL for synthesis commands) |
| `get_pins` | pins of cells, `cell/pin` | the netlist |
| `get_clocks` | clocks made by `create_clock` | the file |
| `get_pblocks` | pblocks made by `create_pblock` | the file |
| `get_sites` | I/O subtiles of the device | the I/O map of the device package, or the grid |

- **R-11** A pattern is matched against the whole name.
  - By default it is a glob, where `*` stands for any text and `?` for one character. `[` and `]` are not special, so `out[0]` and `data[*]` name bus bits.
  - With `-regexp`, a pattern is a regular expression (ECMAScript), still matched against the whole name.
  - When ODC writes a pattern for VPR, which searches a regular expression anywhere in a name, it writes it anchored and escaped, so that VPR matches what ODC matched.
- **R-12** `-hierarchical` matches the names at any depth of the hierarchy (the separator of the netlist, `.` or `/`, is taken as text). Without it, `*` does not cross the separator.
- **R-13** `-filter {expr}` keeps the objects for which `expr` is true. `expr` compares a property to a value with `==`, `!=`, `=~` (glob) and `!~`, joined with `&&` and `||`. The properties are:
  - `direction` for ports and pins (`in`, `out`, `inout`);
  - `type` for cells (the model of the netlist, for example `latch`, `names`, `mult_16`);
  - for sites, the columns of the I/O map: `side`, `pin_direction`, `pin_type`, `netlist_name`, `customer_pin_alias`.

  Another property is an error.
- **R-14** A query that matches nothing is not an error by itself. The command that uses it is reported (R-41), and a command whose every query matched nothing does nothing.

## Commands of version 1

Each command has a **stage**, the first stage of the flow whose result it changes, and a **consumer**. A command whose stage is not reached yet is checked when that stage starts.

### Timing (consumer: VPR's SDC, stage DEVICE)

- **R-20** These commands are those of VPR's SDC reader, with the same arguments, and are given to VPR as an SDC file:
  - `create_clock`, `set_clock_groups`, `set_clock_latency`, `set_clock_uncertainty`;
  - `set_input_delay`, `set_output_delay`;
  - `set_false_path`, `set_max_delay`, `set_min_delay`, `set_multicycle_path`;
  - `set_disable_timing`, `set_timing_derate`.

  So an SDC file that VPR reads is a valid ODC file (R-3 makes its missing header a warning).
- **R-21** ODC checks the objects of each timing command against the netlist before VPR reads it. An object of a list that matches nothing is reported, although VPR would drop it without a word.

### Synthesis (consumer: Yosys, stage: the circuit)

- **R-22** These commands name objects of the RTL, and are given to Yosys at the hook of the package's script (R-24):
  - `set_property KEEP true <objects>` on cells or nets: the object is kept by synthesis. It becomes `setattr -set keep 1`.
  - `set_property DONT_TOUCH true <cells>`: the cell is kept and not optimized. It becomes `setattr -set keep 1` and `setattr -set keep_hierarchy 1` on the cell.
  - `set_property MAX_FANOUT <n> <nets>`: it becomes the attribute `max_fanout`, which the script of a package may use. A script that does not use it ignores it, with a warning.
  - `set_property RAM_STYLE block|distributed <cells>`: it becomes the attribute `ram_style`, read by the memory inference of the package's script.
  - `set_synth_option -flatten | -keep_hierarchy`, `-top <module>`, `-define <NAME>=<value>`: options of the synthesis.
- **R-23** The objects of a synthesis command are matched by Yosys (`select`) at the hook, with its globs: `-regexp`, `-nocase` and `-filter` are refused there. One that matches nothing is reported from the log of Yosys (not yet: the report shows the query as not checked).
- **R-24** A device package whose script takes ODC has a line `${ODC_SYNTHESIS}` right after its `hierarchy` command: the design is elaborated there and still has its RTL names. A package whose script has no such line refuses an ODC file with synthesis commands, with the reason.

### Floorplanning (consumer: VPR's constraints, stage DEVICE)

- **R-25** The commands:
  - `create_pblock <name>` makes an empty pblock.
  - `resize_pblock <name> -add <location>` adds a rectangle to it (R-6).
  - `add_cells_to_pblock <name> <cells>` keeps the cells inside it.
  - `set_property LOC <location> <cells>` fixes a cell to a tile, or to a subtile when the location has one.

  They become partitions of the VPR constraints XML:
  - a pblock is one partition, with a region for each rectangle;
  - a `LOC` is a partition with one region of one tile.
- **R-26** Before VPR runs, ODC checks:
  - that each tile of a `LOC`, and each pblock, has a type that can hold the cells given to it;
  - that a pblock has room for its cells.

  The reason for each is that VPR fails with "Failed to find pack clusters densely enough" without naming the cause. The check names the cell, the location and the type of the tile. The room is counted by primitives: for each model of the cells of a pblock, the primitives of that model in its tiles.
- **R-27** A cell is in one pblock at most, and has one `LOC` at most. A cell with both must have its `LOC` inside the pblock.

### Routing (consumer: VPR's constraints, stage DEVICE)

- **R-28** `set_global_signal -route_model ideal|route|dedicated_network [-network <name>] <nets>` says how a global net (a clock or a reset) is routed. It becomes a `set_global_signal` of the global route constraints of the VPR constraints XML.
  - `ideal`: the net is not routed.
  - `route`: the net is routed with the general routing.
  - `dedicated_network`: the net uses the clock network `<name>` of the architecture.
- **R-29** ODC refuses, with the reason:
  - `route` while `general.clock_modeling` is not `route`, as VPR fails on it;
  - `dedicated_network` without `-network`;
  - `dedicated_network` with a network that the architecture does not have, as VPR routes the net as an ordinary one and says nothing.

  The architectures of OpenFPGA have no clock network today.
- **R-30** The channel width is not a constraint. It stays the option `router.route_chan_width`.

### Pins (consumer: VPR's constraints and the pin constraints, stage DEVICE)

- **R-31** `set_property LOC <location>/<s> <ports>` fixes a port to the I/O subtile `s` of a tile. With or without an I/O map, the location is that of the grid. It becomes a one-tile region with the subtile.
- **R-32** `set_property PACKAGE_PIN <name> <port>` puts a port on the pin of the I/O map whose `customer_pin_alias`, or else whose `netlist_name`, is `<name>`. Where several rows have that name (an input and an output on one pad), the one whose `pin_direction` fits the port is taken.
  - A pin with a location becomes a one-tile region, as `LOC` does.
  - A pin without a location (a global port of the fabric, such as `clk[0]`) becomes a `set_io` of the pin constraints XML, with the pin `netlist_name` and the net of the port. The commands after routing that take it (`route_clock_rr_graph`, the testbenches, the bitstream) are given that file.
- **R-33** A pin is checked against its row of the I/O map:
  - a direction that does not fit is an error;
  - a port on a pin whose `pin_type` is `UNDEF` is a warning;
  - two ports on one pin is an error.
- **R-34** `set_property PIN_DEFAULT 0|1 <ports>` gives the `default_value` of the pin constraints XML to a port that `PACKAGE_PIN` puts on a pin without a location (a global port of the fabric). On another port it is an error.
- **R-35** `set_property <property> <value> <ports>`, for a property that the `pcf_config.xml` of the device package defines (for example an I/O mode), is not supported: the custom commands of a PCF file are out of the scope of the bitstream of a session (docs/spec_bitstream.md), and such a property is refused with the reason.

### Other commands

- **R-36** `odc_version 1` (R-3).
- **R-37** `include <file>` reads another ODC file at that point, with a path relative to the file that includes it. A file that includes itself, directly or not, is an error.

## Reading the file

- **R-40** `read_odc --file <f> [--lenient]` reads a file into the session. It replaces the ODC that the session had. The steps:
  1. The file is parsed. An error stops the read, and the session keeps its old ODC.
  2. Each command is checked as far as the inputs that the session has allow: the device when the architecture is read, the netlist when the circuit is read. A check that needs an input that is not read yet runs when that input is read, and its result is in the report.
  3. The files for the consumers are written in the folder `odc/` of the current directory: `constraints.sdc`, `vpr_constraints.xml`, `pin_constraints.xml` and `synthesis.ys`, each only when it has something. They are given to the flow: the SDC as `filename.sdc_file`, the XML as `filename.read_vpr_constraints_file`, the pin constraints to the commands that take `--pin_constraints_file` and are not given one, and the Yosys commands to the synthesis. A file that changes where it is makes the device again.

  The objects are matched in the netlist that VPR read, as VPR names its blocks: a buffer LUT that VPR absorbs is not a cell. Before the circuit is read, they are matched in the BLIF file that `read_circuit` is given, and the files are written again once VPR has read it. A pattern is written for VPR as the names it matched (`is_regex="false"`), or, before the netlist is known, as an anchored regular expression.
- **R-41** `report_constraints` answers, as JSON (kind `constraints`, schema `openfpga_flow/schemas/constraints.schema.json`):
  - each command, with its file and line, its kind, its stage, and whether it is checked yet;
  - for each object query, the objects it matched (at most a limit, with `truncated`), or the nearest names when it matched nothing;
  - the errors and warnings, with their line;
  - the coverage of the timing: the clocks and their periods, the clocks of the netlist that no `create_clock` names, the input and output ports without a delay, and the share of the endpoints of the timing graph that are constrained (after the analysis; not yet).
- **R-42** `write_odc --file <f>` writes the ODC of the session back as a file, one command per line, in the order of the file, without its comments.
- **R-43** `clear_odc` takes the ODC away. It drops what reading it dropped.
- **R-44** The ODC is an input of the session, as the SDC and the VPR constraints are:
  - a change of its timing, floorplanning, routing or pin commands drops the stages from DEVICE, as VPR reads them when it makes the device;
  - a change of its synthesis commands drops the circuit;
  - it is saved by a checkpoint, written in the journal and replayed.
- **R-45** An ODC with timing commands and a raw SDC file (`filename.sdc_file` set by hand) cannot be used together; the same holds for floorplanning, routing or pin commands and a raw VPR constraints file. The read that would mix them is refused, with the reason and the command that clears the other one.
- **R-46** A device package may have an ODC file of role `odc_defaults` (spec_device_package.md). It is read before the file of the user, as if the user's file included it first. A command of the user's file that sets the same thing (the same `LOC`, the same port's pin) wins.
- **R-47** A design directory may have an ODC file of role `odc` (spec_design_directory.md). `read_design` reads it.

## The model and the web app

- **R-48** The model (docs/spec_model_capabilities.md) writes an ODC with `write_file` and reads it with `set_constraint kind=odc` (`clear` takes it away; an unknown command is an error, so that the model fixes it). The answer says what was dropped, the number of commands, the warnings and the timing that is not constrained. A file with errors is refused with one diagnostic `constraints_invalid` per line, with its file (a name in the workspace) and line; a name that the flow does not know (a command, a port, a cell, a pin of the package) comes with the nearest names, in the error or in a warning. `check_constraints` answers `report_constraints`.
- **R-49** The web app (docs/spec_web_app.md):
  - opens an `.odc` with colours of its own: the commands that the reader knows (another word at the start of a line is marked), queries, options, properties, locations;
  - marks the errors and warnings of the session's ODC on the lines of the files they are in, and says how many in the head of the file;
  - shows in the sidebar the ODC of the session with its number of commands, errors and warnings, the defaults and the I/O map of the package, the clocks and the ports without a delay, and the list of checks, where a click opens the file at the line;
  - does not show the files that the ODC gives VPR as a raw SDC or VPR constraints file;
  - says, when an ODC that the session read is saved, that `set_constraint kind=odc` reads it again.

## The table of commands

- **R-60** The commands are a table, as the application options are. Each row has:
  - the name;
  - the arguments, with their types;
  - the stage;
  - the consumer;
  - the check;
  - the writer that turns the command into the file of its consumer.

  A new kind of constraint is a new row and, if its consumer is new, a new writer. A reader that does not know a row refuses the command (R-4). Rows planned for later:
  - routing constraints of a net beyond `set_global_signal`, where VPR can take them;
  - the I/O standards of a device package;
  - repack constraints.

## Example

```tcl
odc_version 1

# Timing
create_clock -period 5.0 -name clk [get_ports clk]
set_input_delay -clock clk -max 1.0 [get_ports {din[*] wen ren}]
set_output_delay -clock clk -max 1.5 [get_ports dout[*]]
set_false_path -from [get_ports rst]

# Synthesis
set_property KEEP true [get_nets debug_state]
set_property RAM_STYLE block [get_cells mem]

# Floorplanning
create_pblock pb_ctrl
resize_pblock pb_ctrl -add X1Y1:X2Y3
add_cells_to_pblock pb_ctrl [get_cells -hierarchical ctrl_*]
set_property LOC X2Y3 [get_cells dout[0]]

# Routing
set_global_signal -route_model route [get_nets clk]

# Pins
set_property PACKAGE_PIN LED0 [get_ports dout[0]]
set_property LOC X0Y2/4 [get_ports clk]
set_property PACKAGE_PIN clk[0] [get_ports clk]
```

## Design notes

- **Parsed, not Tcl:** the parser is in C++ (`libs/libodc`), so the shell, the Tcl module and Python read a file the same way, give errors with a line, and run no code that a file brings. Tcl may still make a file: a script that writes ODC.
- **VPR's own formats underneath:** ODC writes the files that VPR and OpenFPGA read today, so their behaviour does not change, and the written files can be read by a person in `odc/`.
- **Pins:** in phase 0, `PACKAGE_PIN` through an I/O map, written as one-tile regions, gave the same placement and bitstream as `pcf2place` with `--fix_clusters`, so ODC does not need `pcf2place`.
- **Names:** registers keep their RTL names through synthesis, LUTs do not. A vector that Yosys narrows to one bit loses its index (`state_q[0]` becomes `state_q`), which the nearest names of R-41 show.

## Test plan

| ID | Check | Covers |
|----|-------|--------|
| T-1 | Each rule of the format: a continued line, a comment, nested braces, a quoted word with `\"`, `a[0]` as one word, a file without header (warning), a higher version (refused), an unknown command (error, and a warning with `--lenient`). Each error names its file and line. | R-1 to R-5, R-36 |
| T-2 | Locations: a tile, a subtile, a range, a location outside the grid (error), a subtile the tile does not have (error). | R-6, R-7 |
| T-3 | Queries: globs with `*` and `?`, `out[0]` as a name, `-regexp` matched whole, `-hierarchical`, each `-filter` property and an unknown one (error). A pattern written for VPR matches in VPR what it matched in ODC (`OUT[0-2]` does not take `out:OUT0`). | R-10 to R-13 |
| T-4 | A file of VPR's SDC commands read as ODC gives VPR the same SDC: the same timing as `filename.sdc_file`. An object of a list that matches nothing is in the report. | R-20, R-21, R-41 |
| T-5 | `KEEP` keeps a wire and a register that synthesis removes otherwise, with the scripts of the three device packages. A script without `${ODC_SYNTHESIS}` refuses an ODC with synthesis commands. | R-22 to R-24 |
| T-6 | A pblock and a `LOC` of a cell are obeyed on the golden designs and the DSP and memory packages; the files and the bitstream equal those of the one-shot `vpr` with the written XML. A `LOC` on a tile of the wrong type, and a pblock without room, are refused with the cell and the tile. | R-25 to R-27 |
| T-7 | `set_global_signal` `ideal` leaves the clock unrouted, and the flow equals the one-shot one. `route` with `clock_modeling` `ideal`, `dedicated_network` without a network and with an unknown network are refused. | R-28, R-29 |
| T-8 | `PACKAGE_PIN` through an I/O map gives the placement and the bitstream of `pcf2place` with `--fix_clusters` on basic_tests/io_constraints/example_pcf. A pin without a location writes a `set_io` of the pin constraints XML. A wrong direction, two ports on a pin (errors) and an `UNDEF` pin (warning) are reported. | R-31 to R-33 |
| T-9 | `report_constraints` validates against its schema, and lists the matches, the nearest names and the coverage. `write_odc` of a read file reads back to the same commands. | R-41, R-42 |
| T-10 | A change of a timing command drops DEVICE and after; a change of a synthesis command drops the circuit; `clear_odc` drops the same. A checkpoint restores the ODC, and a journal replays it. A raw SDC with timing commands in the ODC is refused. | R-43 to R-45 |
| T-11 | `odc_defaults` of a package is read before the file of the user, and a command of the user wins. | R-46 |
| T-12 | `include` reads a file relative to the one that includes it, and a loop is an error. | R-37 |
| T-13 | Through the MCP server: a file with an unknown command is refused with its line and the nearest command, and the session keeps its stages; a list with a name that matches nothing is read with a warning and the nearest names; `check_constraints` lists the matches; `clear` takes the ODC away. | R-48 |
| T-14 | In the app: the sidebar shows the ODC, its checks and its timing, and not its SDC; the editor marks the lines of the file; a saved ODC that the session read says so; a click on a check opens the line. | R-49 |
