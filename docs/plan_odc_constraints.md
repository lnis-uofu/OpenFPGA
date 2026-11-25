# ODC: one constraints file for the flow: plan

An ODC file (OpenFPGA Design Constraints, `.odc`) holds the constraints of a
design in one place: timing, synthesis, floorplanning and pin placement. It
is also where any later kind of constraint goes. The flow reads it once, and each stage gets the part it
needs.

Before it, `read_device_package` and `read_design` become commands of the
shell (C++), so the shell, Python, the model and the app read a package, a
design and its constraints the same way.

## Decisions (owner, 2026-11-24)

- The file is parsed, not evaluated as Tcl.
- The parser is C++, and the shell has `read_odc`.
- Routing constraints are in ODC (`set_global_signal` in version 1, other
  constraints per net later). The channel width is not: it stays a setting of
  the flow.
- Locations are VPR grid locations: `X Y` with a subtile.
- A device package may have a CSV that maps each I/O subtile to a name of its
  own (the I/O map, below).
- `read_device_package` and `read_design` move to C++.
- The I/O map is comma- or tab-separated with a header; `row` is the `y` and
  `col` the `x` of the VPR grid; `pin_type` is `GPIO`, `CONTROL`, `SCAN` or
  `UNDEF`.
- nlohmann/json is vendored for the JSON reader of C++.

## What is there today

| Kind | Format the flow reads | Who reads it | In the session |
|---|---|---|---|
| Timing | SDC (`--sdc_file`), parsed by VPR's libsdcparse: `create_clock`, `set_input_delay`, `set_output_delay`, `set_false_path`, `set_max_delay`, `set_min_delay`, `set_multicycle_path`, `set_clock_groups`, `set_clock_latency`, `set_clock_uncertainty`, `set_disable_timing`, `set_timing_derate`; objects `get_ports`, `get_clocks`, `get_pins`, `get_cells`, `get_nets` | VPR, at DEVICE | `read_sdc`, `set_constraint kind=sdc` |
| Floorplanning | VPR constraints XML (`--read_vpr_constraints`): partitions of atoms (`add_atom`, names or regexes) with regions (`x_low`..`y_high`, layer, subtile) | VPR, at DEVICE | `read_vpr_constraints`, `set_constraint kind=vpr_constraints` |
| Routing | The same XML: `global_route_constraints` with `set_global_signal` (a net, its route model `ideal`, `route` or `dedicated_network`, and a network name). The channel width is a setting | VPR, at DEVICE | The XML above; `set_route_chan_width`, `set_constraint kind=chan_width` |
| Pin placement | PCF (`set_io`, custom commands from a `pcf_config.xml`), a pin table CSV, an I/O location map XML. `pcf2place` writes a `.place` of the I/O for `--fix_clusters` (`place.constraints_file`, not wrapped); `pcf2bitstream_setting`; the pin constraints XML (`pin_constraints_file`) of the clock routing, the testbenches and the bitstream | OpenFPGA commands, around VPR | Not in the session. Device packages have an optional `pin_table` role; none of the three has one |
| Synthesis | The Yosys script of the package, attributes in the RTL | Yosys (`synthesize`) | Options of `synthesize` |
| Package | `device_package.json` and `CHECKSUMS.sha256` | Python (`openfpga_mcp/device_packages.py`) | `use_device_package` (MCP), the app. No shell command (R-46 of the spec is not built) |
| Design | `design.json` (docs/spec_design_directory.md); roles `sdc`, `pin_constraints`, `placement_constraints`, `repack_constraints`, `clocks` | Python (`tools.py`, `paths.py`) | `synthesize design=...` (MCP). No shell command |

In C++: picosha2 is in libvtrutil (`vtr_digest`). There is no JSON reader,
only OpenFPGA's JSON writer (`openfpga/src/utils/json_writer`).
spec_device_package.md says nlohmann/json is in the tree; it is not.

## Design

### The file

- **Syntax:** like SDC:
  - one command per line, `#` comments, `\` continues a line;
  - `{...}` and `"..."` for words, `[get_ports ...]` for objects;
  - parsed, not evaluated: no variables, loops or procedures.
- **Header:** the first command is `odc_version 1`. A file without it is read
  as version 1, with a warning.
- **Unknown commands** are an error with the file and the line, unless
  `read_odc --lenient` is used (then a warning).
- **Units:** the time unit is ns, as in SDC.
- **Locations:** VPR grid locations, written as `X3Y2` for a tile, `X3Y2/1`
  for its subtile 1, and `X1Y1:X4Y6` for a range. They are the `x`, `y` and
  subtile of the `.place` file and of the VPR constraints XML.

### Commands of version 1

**Timing** (to VPR as SDC): every command of libsdcparse above, unchanged,
so an SDC file is a valid ODC file.

**Synthesis** (applied by the synthesis of `read_design`, or by
`synthesize`):
- `set_property KEEP true [get_cells|get_nets ...]`
- `set_property DONT_TOUCH true [get_cells ...]`
- `set_property MAX_FANOUT n [get_nets ...]`
- `set_property RAM_STYLE block|distributed [get_cells ...]`
- `set_synth_option -flatten|-keep_hierarchy|-top name|-define NAME=VALUE`

These become Yosys `setattr` and options, at a hook of the package's script.
The names are those of the RTL, resolved by Yosys.

**Floorplanning** (to the VPR constraints XML):
- `create_pblock name`
- `resize_pblock name -add X1Y1:X4Y6` (also `-layer n`; a subtile with `/n`)
- `add_cells_to_pblock name [get_cells ...]`
- `set_property LOC X3Y2/0 [get_cells ...]` (a block fixed to a tile and a
  subtile)

**Routing** (to the `global_route_constraints` of the VPR constraints XML):
- `set_global_signal -route_model ideal|route|dedicated_network
  [-network name] [get_nets ...]`: how a global net (a clock, a reset) is
  routed. `dedicated_network` needs the clock network of the architecture,
  which the architectures of OpenFPGA have not (phase 0 of the analytical
  placement), so it is refused there with the reason.

The channel width is not a constraint: it stays a setting of the flow
(`set_route_chan_width`).

**Pins** (to the placement of the I/O, and to the pin constraints XML of the
later commands):
- `set_property LOC X0Y3/2 [get_ports p]`: a grid location, with or without
  an I/O map.
- `set_property PACKAGE_PIN name [get_ports p]`: a `customer_pin_alias` (or a
  `netlist_name`) of the package's I/O map.
- `set_property PIN_DEFAULT 0|1|z [get_pins ...]`: the default value of an
  unused fabric pin, as in the pin constraints XML today.
- `set_property <custom> value [get_ports p]`: for the custom commands that a
  package's `pcf_config.xml` defines (I/O mode bits, for
  `pcf2bitstream_setting`).

A port is checked against the row of its location:
- the direction of the port must agree with `pin_direction`;
- a port on a pin whose `pin_type` is `UNDEF` is a warning.

**Objects:**
- `get_ports`, `get_cells`, `get_nets`, `get_pins`, `get_clocks`, with
  patterns (`*`, `?`), `-regexp`, `-hierarchical`, and
  `-filter {PROPERTY == value}` limited to the properties the flow knows;
- `get_pblocks`;
- `get_sites` over the I/O map (`-filter {side == TOP && pin_type ==
  GPIO}`) for constraining pins in groups.

**For later kinds:** a table of commands, as the option table is, where each
command says:
- its stage (SYNTH, DEVICE, BITSTREAM, ...);
- its consumer (Yosys, VPR's SDC, VPR's placement or routing constraints,
  the pin constraints, the bitstream settings);
- its arguments.

A new kind is a row and a writer. Examples: a routing constraint per net,
the I/O standards of a package, a repack constraint.

### The I/O map of a device package

An optional file of the package, role `io_map`: comma-separated (`.csv`) or tab-separated (`.tsv`, or a `.csv` whose header has tabs), with a header row. It replaces the `pin_table`
role of the spec, which no package uses. There is one row per I/O subtile of
the package's layout, with a header row of these columns:

| Column | Meaning |
|---|---|
| `side` | `TOP`, `BOTTOM`, `LEFT`, `RIGHT` of the I/O ring (or `CORE` for an I/O inside the grid) |
| `pin_direction` | `input`, `output`, `inout` |
| `pin_type` | `GPIO`, `CONTROL`, `SCAN` or `UNDEF` |
| `row` | the `y` of the VPR grid |
| `col` | the `x` of the VPR grid |
| `subtile` | the subtile of the I/O in the tile |
| `netlist_name` | the name of the pin in the fabric (the port of `fpga_top`, for example `gfpga_pad_GPIO_PAD[3]`), as the pin constraints XML names it |
| `customer_pin_alias` | optional: the name a customer uses (for example a ball of the package, or `LED0`) |

Rules:
- `row` is the `y` and `col` the `x` of the VPR grid, `(0,0)` at the bottom left, as in VPR.
- `row`, `col` and `subtile` name an I/O subtile of the layout, once each.
- A global port of the fabric (a clock or a reset, such as `clk[0]` or
  `op_reset[0]`) has no tile: its row leaves `row`, `col` and `subtile`
  empty, and its `pin_type` is `CONTROL`. `PACKAGE_PIN` on such a pin
  writes a `set_io` of the pin constraints XML (pin = `netlist_name`, net =
  the net of the design) instead of a placement (found in phase 0).
- The header names the eight columns, in any order; a column of another name is an error.
- `netlist_name` is unique; `customer_pin_alias` is unique among the rows of one `pin_direction` (the input and the output of one pad share it).
- An I/O subtile of the layout that has no row cannot be used by
  `PACKAGE_PIN`, but `LOC` can still place a port on it.
- `check` reports each broken rule with its line.

The existing formats (pin table, I/O location map, PCF) can be written from
the I/O map for the OpenFPGA commands that read them. A shell command
`write_io_map --file f.csv` writes a starting CSV for a device:
- one row for each I/O subtile of the grid, with `side`, `row`, `col` and
  `subtile`;
- `netlist_name` once the fabric is built;
- the other columns left for the maker of the package.

### `read_device_package` and `read_design` in C++

- **`read_device_package --file dir [--corner name]`** does what
  `device_packages.load` and `read_into` do today:
  - checks the manifest (schema, roles, paths inside the package, the
    checksums with picosha2);
  - reads the VPR architecture with the layout, then the OpenFPGA
    architecture;
  - sets the channel width and the options of `flow_defaults.vpr_options`
    (through `vpr_flag`);
  - keeps `device.flat_routing`, the Yosys script and the I/O map;
  - reads the package's `odc_defaults`, if it has one.

  The package becomes an input of the session: saved by checkpoints, in the
  journal, replayed. `report_device_package --json` answers what
  `describe()` answers today.
- **`read_design --file dir [--netlist label]`** does what `synthesize
  design=...` and the design checks do today:
  - checks `design.json` (spec_design_directory.md);
  - synthesizes the RTL with the package's script through the shell's
    `yosys` command, or reads a listed netlist;
  - reads the design's constraints: a new role `odc`, and the `sdc` and
    `placement_constraints` roles read as ODC or as raw files.
- **JSON in C++:** nlohmann/json (one header, MIT) vendored in
  `libs/EXTERNAL`; the spec's sentence then becomes true.
- **Python** (`Session.read_device_package`, `Session.read_design`), the MCP
  tools (`use_device_package`, `synthesize design=`) and the app call these
  commands. `device_packages.py` keeps only what the server needs:
  - finding packages in its folders;
  - the hints of its errors;
  - the answers of the tools.
- **Behaviour** is unchanged: the tests of today (`test_package_options.py`,
  the package and synthesis tests of MCP, the app's package tests) pass
  without change, except where an answer gains a field.

### Where ODC lives

- **C++ library `libs/libodc`:**
  - the lexer and parser, with the file and line of each command;
  - the model (commands, objects, pblocks, pins);
  - the I/O map reader and its checks;
  - writers to SDC, to the VPR constraints XML (partitions with regions, a
    fixed block or port being a one-tile region with its subtile; global
    route constraints), to the pin
    constraints XML, to a Yosys fragment, and to a PCF and a pin table for
    the older commands;
  - `write_odc` from the model.

  libpcf's readers and writers are reused.
- **Shell:**
  - `read_odc --file f [--lenient]`;
  - `write_odc --file f`;
  - `clear_odc`;
  - `report_constraints [--json]`: each command with its line, the objects
    it matched, what matched nothing, and the coverage of the timing
    (endpoints constrained, clocks without a period, ports without a delay),
    with a schema in `openfpga_flow/schemas/`.
- **Session:**
  - the ODC is an input, as the SDC and the VPR constraints are today;
  - it drops what they drop: everything from DEVICE, since VPR reads both in
    its init;
  - the synthesis part drops the circuit;
  - checkpoints, journal and replay;
  - the files generated from it go to the session directory (`odc/`).
- **Raw files:** a raw SDC or VPR constraints file and the ODC can't set the
  same kind at once; a read that tries is refused with the reason.
- **MCP:** `set_constraint kind=odc` and `check_constraints` (the report
  above), within the budget of the tool descriptions.
- **App:**
  - an `.odc` language in the editor (Monaco), with the checks on the lines;
  - the constraints in the sidebar;
  - later, pblocks and pins drawn on the device view (T14.1).

## Names across the flow

Synthesis constraints name RTL objects. Placement and timing constraints
name the objects of the netlist after synthesis. Yosys keeps the names of
ports and registers, but renames the LUTs (`$abc$...`). In version 1:

- `get_ports` works everywhere;
- `get_cells` and `get_nets` work on the names that are in the netlist:
  the registers keep their RTL names, the LUTs do not;
- an object that matches nothing is reported by ODC, with the stage that
  looked for it and the nearest name. VPR does not report it (phase 0):
  - placement skips an unmatched atom with a warning;
  - in SDC, an unmatched object in a list is dropped without a word, and
    VPR warns only when nothing matches.

## Phase 0: try it

1. **Floorplanning:** on the golden designs and the three packages, by hand,
   a VPR constraints XML with a pblock of a few cells, and a fixed port (a
   one-tile region with a subtile). In the staged flow, the placement obeys
   them, and the bitstream equals the one-shot flow's with the same file.
2. **Pins:** the same I/O placement from `pcf2place` (with a pin table made
   for the test) and from a one-tile region give the same `.place` I/O and
   the same bitstream. Check that the pin constraints XML of the later
   commands can be made from `netlist_name`.
3. **Synthesis:** `setattr -set keep 1` on a net at a hook in each package's
   script keeps it in the netlist. Count the RTL register names left in the
   BLIF.
4. **Timing:** an SDC read through the new path gives the same timing as
   today.
5. **Routing:** `set_global_signal -route_model ideal` and `route` on the
   clock of a golden design, in the staged flow: the routing of the clock
   follows the model, and the bitstream equals the one-shot flow's with the
   same XML. Check what VPR does with `dedicated_network` on an architecture
   without a clock network.
6. **Packages in C++:** list what `device_packages.py`, `synthesis.py` and
   the design code of `tools.py` do. Mark what moves to C++ and what stays in
   the server.

Status: done (2026-11-25). The trials were run by hand with a script that
runs one flow, staged or one-shot, with constraint files; it is not in the
repository.

1. **Floorplanning:**
   - A region for the logic and fixed ports (one-tile regions with a
     subtile) are obeyed on k4_routing_test (4x4), k6_routing_test (2x2) and
     k4_flat_routing_test (flat routing).
   - The DSP block is moved to (2,3) on k4_n4_dsp16_3x4, the memory is given
     its only tile on k6_frac_n10_mem1k_3x2, and a port is fixed in each.
   - Each time, the staged flow gives the `.net`, `.place`, `.route`, the
     results and the bitstream of the one-shot `vpr` with the same file.
   - Found:
     - **A regex is searched, not matched** (`std::regex_search`):
       `OUT[0-2]` also took the pads `out:OUT0..2`, and packing failed with
       "Failed to find pack clusters densely enough to fit in the designated
       floorplan regions". ODC writes its patterns anchored (`^...$`), with
       the special characters of a name escaped (`out[0]`), or as exact
       names.
     - **A block placed on a tile of another type** (the DSP on a logic
       tile) fails with the same packing message, which does not say why.
       ODC checks the type of the tile of `LOC` and of a pblock against the
       blocks before VPR runs.
     - **An unmatched atom is only a warning** ("Atom X was not found,
       skipping atom"). ODC reports it.
2. **Pins:**
   - The PCF of basic_tests/io_constraints/example_pcf (k4, 4x4) was placed
     two ways: by `pcf2place` and `--fix_clusters`, as the task does; and by
     one-tile regions, through an I/O map CSV in the columns of this plan,
     made from the task's pin table and I/O location map (16 rows, all with
     an alias).
   - All four runs give the same places for the ports and the same
     bitstream: fix and region, each staged and one-shot. The staged flow
     takes `--fix_clusters` too (`place.constraints_file`).
   - So the CSV holds what pcf2place needs, and `PACKAGE_PIN` can be a
     one-tile region: ODC does not need pcf2place.
   - The pin constraints XML of the later commands maps nets to the global
     ports of the fabric (`clk[0]`, `op_reset[0]`), which are not at a tile.
     Hence the rule above: a CONTROL row without a location.
3. **Synthesis:**
   - Hook: a line after `hierarchy` in each package's script (the design is
     elaborated and has its RTL names).
   - `setattr -set keep 1 w:...` there keeps a register and two wires that
     synthesis removes otherwise, with each of the three scripts.
   - Register names survive:
     - diffeq2: all 96 flip-flops have the name of an RTL register;
     - memset (ch_intrinsics): all 307 flip-flops, for 14 of the 23
       registers of the RTL;
     - but a vector that Yosys narrows to one bit loses its index
       (`state_q[0]` becomes `state_q`), which the nearest name of the
       report covers.
   - The memory package's script makes flip-flops cells (`.subckt`), not
     `.latch`; their names are still the RTL ones.
4. **Timing:** an SDC (a clock, input and output delays) through
   `filename.sdc_file` in the staged flow gives the files and the timing of
   the one-shot `--sdc_file`. ODC writes an SDC and sets the same option.
   VPR drops an unmatched object of a list without a word (`get_ports {IN0
   NOPE}`).
5. **Routing:**
   - `ideal` on the clock, in a flow that routes clocks: the clock becomes a
     global net that is not routed, with wirelength 26 instead of 47, and
     staged and one-shot agree, the bitstream too.
   - `route` on the clock while `general.clock_modeling` is `ideal` fails in
     VPR, in both flows ("Routing failed"; "net #0 has no routing"). ODC
     refuses it with the reason unless the clocks are routed. The memory
     package keeps VPR's default, `ideal`.
   - `dedicated_network` needs a network name (VPR says so). With a name the
     architecture does not have, VPR routes the clock as an ordinary net and
     says nothing. ODC checks the name against the clock networks of the
     architecture, and the architectures of OpenFPGA have none.
6. **Packages in C++** (what `read_device_package` and `read_design` take
   over, and what stays in the server):
   - **To C++:**
     - device_packages.py: `DevicePackage`, `load` and its checks (`_inside`,
       `_layouts`, `_verify`: checksums), `read_into` and
       `_given_otherwise`, `describe` (as `report_device_package`), and the
       flat routing flag (the session keeps it, so `flat_routing_allowed`
       reads the session);
     - synthesis.py: `check_top`, `check_sources`, `_package_values` and
       `make_plan` (the template of the script), `blif_stats` (the answer of
       the synthesis);
     - tools.py: `_design_inputs` (the files of the manifest, inside the
       design), `_design_netlist` and `_read_design_netlist`.
   - **Stays in the server:**
     - finding things in its folders: `discover`, `find`, `_names`,
       `_not_found`;
     - the design folders: paths.py (`design_directories`, `find_design`,
       `copy_design`, `resolve_file`, `_close_names`, `check_kind`);
     - the answers and hints for the model: `_synthesis_failure`, and
       `log_errors` and `error_details` (they read the log of Yosys).
   - **Yosys:** synthesis already runs through the shell's `yosys` command,
     so `read_design` does the same, and a design with RTL needs a build
     with Yosys. The pip build has no Yosys, so it synthesizes nothing
     today, and a design with only a netlist still works there.

## Phase 1: the specs

- `docs/spec_odc.md`:
  - the grammar;
  - the commands of version 1 with their arguments and stage;
  - the objects and the locations;
  - the errors and warnings;
  - examples.
- spec_device_package.md:
  - the `io_map` role with its columns and rules;
  - `odc_defaults`;
  - R-46 as built.
- spec_design_directory.md: the `odc` role, and `read_design`.

The owner reviews the specs before code is written.

Status: drafted (2026-11-25), for the owner's review.

- **docs/spec_odc.md (new):**
  - the format, in R-1 to R-7;
  - the object queries, in R-10 to R-14;
  - the commands of version 1, by kind, in R-20 to R-37;
  - reading, the report, the session and conflicts, in R-40 to R-47;
  - the table of commands, in R-60;
  - an example, design notes and a test plan (T-1 to T-12).

  The findings of phase 0 are in it: patterns anchored for VPR, the tile type checked, unmatched objects reported, the `set_global_signal` refusals, pins without a location, `${ODC_SYNTHESIS}`.
- **spec_device_package.md:**
  - R-11a, the hook of the script;
  - R-25, the I/O map, which replaces the role `pin_table`;
  - R-26, `odc_defaults`;
  - R-46, `read_device_package` in C++;
  - R-48, `write_io_map`;
  - R-51, `report_device_package`;
  - T-14 and T-15.
- **spec_design_directory.md:** the role `odc` (R-23a), `read_design` in C++ (R-34), and T-16.

## Phase 2: `read_device_package` and `read_design` in C++

- nlohmann/json vendored.
- Both commands, as the session's inputs, with `report_device_package
  --json`.
- `write_io_map`, and the I/O map reader and checks (the CSV needs no ODC).
- Python, MCP and the app moved onto the commands. The tests of today pass;
  new C++ and shell tests for each check of a manifest.

Status: done (2026-11-25, committed on `gg_web_ui`, not pushed).

**synthesize and read_design** (the owner chose both in C++, so the script is made in one place):
- `design_synthesis.{h,cpp}`:
  - the plan of a synthesis: the checks of the top, the sources and the values, and the template of the package's script (filled as Python's `string.Template.safe_substitute`), or the generic script;
  - the reading of a design directory: the checks and the messages of the MCP server.
- `synthesis_commands.{h,cpp}`:
  - `synthesize --top --files [--lut_size --include_dirs --defines --read_options --output_dir --no_run --no_read]`;
  - `read_design --file [--netlist label] [--lut_size] [--check] [--no_run] [--no_read]`.

  Both answer the kind `synthesis` (schema `synthesis.schema.json`). A refusal before Yosys runs is logged as `Synthesis refused: <reason>`. `read_design` sets the SDC of the design (`filename.sdc_file`).
- The generic script has the hook `${ODC_SYNTHESIS}` after `hierarchy`. The scripts of the three packages do not have it yet: phase 4 adds it, with their checksums.
- Checked against the Python plan: the same script for the three packages (the generic one differs only by the hook), and the same messages for the refusals.
- **Python:**
  - `Session.synthesize()` and `Session.read_design()`; `run=False` (the plan) and `check=True` are not journaled.
  - The MCP tool `synthesize` asks the shell for the plan (`--no_run`), then runs `yosys` as before, so its journal keeps the script that ran.
  - `synthesis.py` keeps only what reads the results (`Plan`, the errors of the log, `blif_stats`); `make_plan`, its checks, `_design_inputs` and `_design_netlist` are gone.
- **Tests:**
  - the MCP and app synthesis tests pass unchanged, except the two that called `make_plan`, which ask a Session for the plan now;
  - new: test_synthesis_commands.py (12).

**The packages and the I/O map**, done earlier the same day: `read_device_package` and the I/O map.

- **nlohmann/json 3.10.5:**
  - vendored in `libs/EXTERNAL/nlohmann_json` (the single header, its MIT license, a CMake interface target), copied from the copy in the Yosys submodule;
  - `make format-cpp` now leaves `libs/EXTERNAL` out, as CI would reformat the header otherwise.
- **C++ (openfpga/src/base):**
  - `device_package.{h,cpp}`: the loader, with the checks and the messages of the Python code it replaces, in the same order.
  - `device_package_commands.{h,cpp}`:
    - `read_device_package --file`: refuses a package with a broken I/O map, sets the options of `flow_defaults.vpr_options` by their VPR flag, runs `read_vpr_arch` and `read_openfpga_arch` through the shell, sets the channel width, and records the package in the session;
    - `report_device_package [--file] [--no_checksums]` (kind `device_package`, schema `device_package.schema.json`);
    - `write_io_map --file`;
    - `check_io_map --file`.
  - `package_io_map.{h,cpp}`: the reader and the rules of R-25, the check against the grid, and the writer.
  - `OpenFpgaSession` keeps the package (`device_package()`); reading an architecture by hand, or failing to, forgets it.
  - `IoLocationMap` gains `io_ports(x, y, z)` and `empty()`.
- **Found on the way:**
  - the pin tables of OpenFPGA call the row y = 0 `TOP`, the bottom of VPR's grid, so the side is the maker's name and is not checked against the grid (R-25);
  - a location may have several rows, one for each pin of the fabric (an input and an output pad), so the rule is "once per `netlist_name`";
  - the committed `fpga_io_location.xml` of basic_tests/io_constraints/example_pcf is older than the fabric of its architecture: it names `gfpga_pad_IO_A2F[...]`, while the fabric now has `gfpga_pad_GPIO_PAD[...]`;
  - VPR's data is process-wide, so "is there a device" is asked of the session (its VPR generation), not of the grid.
- **Python:**
  - `Session.read_device_package()` and `Session.device_package()`;
  - a checkpoint records the package, and restore reads it again before the options of the checkpoint.
  - `openfpga_mcp/device_packages.py` keeps the folders of the server and the hints: `load` is `report_device_package --file` in a shell of the server's process, `read_into` is `Session.read_device_package`, and `describe` is the answer without the directory.
  - `read_device_package` is a command that a replay of the MCP server runs.
- **Tests:**
  - test_package_options.py gains 5 tests (the command, checkpoint and journal, the schema of both answers);
  - test_package_io_map.py (15): the rules with their line, the grid, `write_io_map` against `write_fabric_io_info`, a package with an I/O map;
  - test_mcp_journal.py: the journal has `read_device_package` instead of four commands, so its counts changed (7 to 4, 9 to 6, 4 to 1);
  - the tests of the packages (the damaged ones included) pass unchanged with the checks in C++.
- **Not done yet:**
  - `read_design`;
  - the grid check of a package's I/O map when its device is made (it is checked by `check_io_map`, and ODC will check it when it uses it).

## Phase 3: libodc and the shell

- The parser, the model, the checks and the writers, with unit tests:
  - every command;
  - each error with its line;
  - a round trip through `write_odc`.
- `read_odc`, `write_odc`, `clear_odc`, `report_constraints --json` and its
  schema.
- An SDC read as ODC gives the same SDC to VPR.

## Phase 4: in the session, synthesis and pins

- **The ODC as an input:** what it drops, checkpoints, the journal, replay,
  and conflicts with raw files. Python `Session.read_odc`.
- **Tests on the golden designs:**
  - timing equals the raw SDC;
  - a pblock is obeyed;
  - a fixed port is at its location;
  - a restored checkpoint gives the same result.
- **Routing:** `set_global_signal` on the golden designs, the route model
  checked in the routing of the net.
- **Synthesis:** the hook in the scripts of the packages; `KEEP`,
  `DONT_TOUCH`, `RAM_STYLE`, `MAX_FANOUT` and `set_synth_option`, each tested
  on a design where it changes the netlist.
- **Pins:**
  - `LOC` and `PACKAGE_PIN` placed, with the checks of direction and type;
  - the pin constraints XML given to the later commands;
  - the custom properties given to the bitstream settings;
  - the bitstream equals that of the PCF flow.
- **I/O map in a package:** one of the three packages of the repository gets
  an I/O map, and so does a 32x32 package of the work folder.

Status of phases 3 and 4: done (2026-11-25), except what is listed under
"Not yet".

- **libodc** (`libs/libodc`, no dependency on VPR but libblifparse):
  - the reader: lines, words, queries, locations, the table of commands
    with the checks of their arguments, `include` with loops found,
    `odc_version`, `--lenient`;
  - the matcher: globs matched whole (`*` does not cross `.` or `/`
    without `-hierarchical`), `-regexp`, `-nocase`, `-filter` with `&&`,
    `||` and the properties of R-13, the nearest names;
  - `resolve()`: the objects of each query, an object of a list that
    matched nothing, the grid (R-7), the type of a tile and the room of a
    pblock (R-26), one pblock and one LOC a cell (R-27), the route model
    against `general.clock_modeling` and the clock networks (R-29), pins
    against the rows of the I/O map (R-33), the defaults of a package
    overridden by the user (R-46), the coverage of the timing;
  - the writers: SDC, VPR constraints XML (exact names once the netlist is
    known, else anchored regexes), pin constraints XML, Yosys commands,
    `write_odc`;
  - `test_odc` (ctest): the format, the commands, the locations, the
    queries, each check, the writers, a round trip.
- **The ui plugin** (`odc_commands`, `odc_design`): `read_odc`,
  `write_odc`, `clear_odc`, `report_constraints` (schema
  `constraints.schema.json`); hooks that make the files again when the
  circuit is read (before, from the BLIF; after, from the netlist of VPR,
  whose buffers are absorbed) and check before a command makes the device;
  `set_app_option` and `reset_app_option` refused on the files the ODC
  gives; the package's I/O map and `odc_defaults` taken after
  `read_device_package`.
- **The core:** the session keeps what the constraints give to the core
  (`SessionConstraints`: the Yosys commands, `-top`, `-define`, the pin
  constraints file); `flow_input_file_changed()` makes the device again
  when a file changes where it is; the commands that take
  `--pin_constraints_file` use the session's when they are not given one;
  `synthesize` and `read_design` fill `${ODC_SYNTHESIS}` and refuse a script
  without it; `read_design` reads the role `odc`, and refuses a design with
  `odc` and an `sdc` or `placement_constraints` file.
- **Packages:** the three scripts have `${ODC_SYNTHESIS}` after
  `hierarchy`, with their checksums.
- **Python:** `Session.read_odc`, `write_odc`, `clear_odc`, `constraints`;
  `clear_constraints` clears the ODC; checkpoints record it with its
  checksum and `restore` reads it; the journal has `read_odc`; the MCP
  replay takes `read_odc` and `clear_odc`.
- **Tests** (`test_odc.py`, 34): the report and its schema, errors with
  their line, `--lenient`, `write_odc` back; the timing of an ODC equals the
  same SDC; a pblock, a fixed cell and fixed ports obeyed; each refusal of
  floorplanning, routing and pins; a check that waits for the netlist and
  stops `read_circuit`; an ideal clock not routed; the drops of a change,
  of the same file, of `clear_odc`; raw files against the ODC; checkpoint
  and journal; `PACKAGE_PIN` through an I/O map, a pin without a location,
  the defaults of a package; `KEEP` on the three packages, `-define` and
  `-top`, a script without the hook, `read_design` with `odc`.
- **Not yet:**
  - the custom properties of a `pcf_config.xml` (R-35);
  - the objects of synthesis commands reported from the log of Yosys
    (R-23);
  - the share of constrained endpoints (R-41);
  - the bitstream compared with the one-shot `vpr` (T-6) and with
    `pcf2place` (T-8): phase 0 compared them by hand; the tests compare the
    placement;
  - an I/O map in one of the packages of the repository, and a 32x32
    package: the tests make a copy of k6_frac_n10_2x2 with a map from
    `write_io_map`;
  - the plugin phase 4 item `write_io_map` stays in the core, next to
    `check_io_map` and `read_device_package`.

## Phase 5: the model and the app

- `set_constraint kind=odc` and `check_constraints`. Diagnostics name a
  command, an object or a pin alias the flow does not know, with the nearest
  name.
- The editor's language and checks; the constraints and the I/O map in the
  sidebar.
- Docs: spec_model_capabilities.md, spec_web_app.md, python_session.rst, the
  shell's command reference.

Status of phase 5: done (2026-11-25).

- **libodc:** an unknown command names the nearest command; a
  `create_clock` without a query (`create_clock -period 10 clk`, as in an
  SDC) is the clock of its bare target, in the report and in the coverage.
- **MCP:** `set_constraint kind=odc` (`file`, `clear`; no `lenient`, to
  stay within the budget of the tool definitions) answers
  what it dropped, the number of commands, the warnings and the timing that
  is not constrained; a refused file gives one `constraints_invalid` per
  line, with the file as a name of the workspace. `check_constraints` (the
  28th tool) answers `report_constraints`. `write_file` and the uploads take
  `.odc`. The prompt of the app's agent says where constraints go.
- **App:** the language `odc` in Monaco (`frontend/src/odc.ts`); the
  checks as markers on the lines of the open file, with their count; the
  sidebar's ODC (file, commands, errors, warnings, defaults, I/O map,
  clocks, ports without a delay, the checks, a click opens the line); the
  files that the ODC gives VPR are not listed as raw constraints; saving an
  ODC that the session read says to read it again.
- **Docs:** spec_odc.md R-48, R-49, T-13, T-14; spec_model_capabilities.md
  (28 tools, `constraints_invalid`); spec_web_app.md UI-36, UI-44, UI-46;
  `openfpga/python/mcp/DESIGN.md`; the app README; `read_odc` in the
  command reference.
- **Tests:** `test_odc` (the nearest command, a bare clock);
  `test_mcp_tools.py` (an ODC set, refused, checked and cleared);
  `test_app_viewer.py` (the view and the saved file); the frontend
  (the language, the markers, the sidebar, a click on a check).
- **Not yet:** completion of names in the editor (X-30); the I/O map as a
  table in the app; pblocks and pins on the device view (Later).

## Later

- Importing a subset of the constraints files of other tools: the commands
  that have a meaning here, with the rest reported.
- Drawing and editing pblocks and pins on the device view.
- Authoring constraints from words (T11.2).
- Routing constraints per net beyond `set_global_signal` (for example a net
  kept to a region, or its routing priority), where VPR can take them.
- I/O standards per package, and repack constraints as ODC commands.
