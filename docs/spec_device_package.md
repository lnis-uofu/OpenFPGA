# Spec: OpenFPGA device package

Status: draft for review. Implemented: the manifest and its checks, the corners and the substitution of the timing variables in `read_device_package` (`--corner`; R-16 to R-19, R-22 to R-24, R-46; the process, voltage, temperature and rc of a corner are not required yet), `read_device_package` and `report_device_package` in C++ (which Python, the MCP server and the web app call), the I/O map (`write_io_map`, `check_io_map`), `odc_defaults`, the settings and the recipe of the bitstream (docs/spec_bitstream.md), and the line `${ODC_SYNTHESIS}` in the scripts of the three packages of the repository (docs/plan_odc_constraints.md).

## Overview

This enhancement packages all files and settings required for a single FPGA device into one self-contained unit, making it easier to share, version, and use the device through the OpenFPGA interface.
At present, there is no standard format for bundling the complete set of files and settings required for one device into a single portable package. A task file links these files with paths and variables, but that information is spread across multiple files, which is beneficial for regression testing but is not portable. To share a device with someone else, or to use it from another tool, a person must know which files belong together and which settings apply to each device.

A **device package** is one directory (or one archive of it) that holds everything needed to target one device with the OpenFPGA flow, plus a manifest that says what each file is. A tool that gets a package needs no other input to synthesize, pack, place, route and generate a bitstream for a design on that device.

This spec defines the layout, the manifest, the rules a package must follow, and the commands that create, check and use one. It does not change the architecture or bitstream formats themselves.

## Glossary

| Term | Meaning |
|------|---------|
| Device | One FPGA fabric: a fixed architecture and a fixed grid size (for example `k4_N4_2x2`). |
| Package | The directory or archive described here. |
| Manifest | `device_package.json` at the root of a package. |
| Role | The kind of a file in the package, for example `vpr_arch` or `yosys_cell_sim`. Roles are listed in R-20. |
| Corner | One design corner of the device: a combination of process, voltage, temperature (PVT) and RC extraction corner for which the timing and power data are characterized, for example `ss_0p72v_125c_cworst`. |
| Package version | The version of this spec that the package follows (`format_version`). Not the version of the device. |

## Goals and non-goals

Goals:

- One self-contained unit per device that can be copied, versioned, archived and checked.
- The same package works from a task file, from the Python `Session`, and from the shell.
- A tool can tell, without running the flow, whether a package is complete and whether it fits the installed OpenFPGA.

Non-goals:

- A new architecture format. The files inside are the existing XML, Verilog and text files.
- A package registry or download service.
- Holding user designs, benchmarks or results. A package describes the device only.
- Several grid sizes in one package (R-7).
- Generated fabric netlists (R-8).
- Reading Liberty and SDF files in the flow (R-21).

## Layout

```
<device_name>/
  device_package.json          manifest (required)
  README.md                    free text for people (optional)
  arch/
    vpr_arch.xml               VPR architecture; timing values are variables, not numbers (R-22)
    openfpga_arch.xml          OpenFPGA architecture
    bitstream_setting.xml      bitstream settings
    simulation_setting.xml     simulation settings (optional)
    repack_constraints.xml     repack design constraints (optional)
    pin_constraints.xml        default pin constraints (optional)
    fabric_key.xml             fabric key (optional)
    tile_config.xml            tile config (optional)
    cell_library/              Verilog or SPICE cells the openfpga_arch refers to
  synthesis/
    synth_script.ys            Yosys script (or script template)
    techlib/                   cell_sim.v, *_map.v, *_bram.txt, lut and carry maps
    constraints/               default SDC, clock definitions (optional)
    defaults.odc               default constraints of the device, in ODC (optional, R-26)
  bitstream/
    arch_bitstream.xml(.gz)    architecture bitstream of a reference design (optional)
    fabric_bitstream.*         fabric bitstream of a reference design (optional)
    io_info.xml                I/O naming and location table
    io_map.csv                 I/O map: each I/O subtile, its fabric pin and its customer name (optional, R-25)
  corners/                     design corner files, one directory per corner (optional)
    <corner_name>/             for example tt_0p80v_25c_typ, ss_0p72v_125c_cworst, ff_0p88v_m40c_cbest
      timing_data.json         values of the vpr_arch.xml timing variables for this corner (R-22)
      timing/                  Liberty (.lib), SDF, delay tables: stored and checked only (R-21)
      power/                   power data for this corner (optional)
      corner.json              corner description: process, voltage, temperature, RC corner, notes (optional)
  reference/                   small reference design with its expected results (optional)
  CHECKSUMS.sha256             SHA-256 of every file except itself
```

Paths in the manifest are relative to the package root. Directories may be empty or absent when the device does not use that part.

## Manifest

`device_package.json` is validated by `openfpga_flow/schemas/device_package_manifest.schema.json` (`device_package.schema.json` is the schema of the answer of `report_device_package`, R-51).

```json
{
  "format_version": "1.0",
  "name": "k4_N4_2x2",
  "version": "0.3.1",
  "description": "K4 N4 tileable fabric, 2x2 core",
  "license": "Apache-2.0",
  "openfpga": { "min_version": "1.2.0", "max_version": null },
  "vtr": { "min_version": "9.0.0" },
  "timing_units": { "time": "ps", "capacitance": "fF", "resistance": "ohm" },
  "device": {
    "grid": { "width": 2, "height": 2 },
    "lut_size": 4,
    "channel_width": 20,
    "flat_routing": true,
    "tags": ["tileable", "bram", "dsp"]
  },
  "corners": {
    "default": "tt_0p80v_25c_typ",
    "list": [
      { "name": "tt_0p80v_25c_typ", "process": "tt", "voltage": 0.80, "temperature": 25, "rc": "typ" },
      { "name": "ss_0p72v_125c_cworst", "process": "ss", "voltage": 0.72, "temperature": 125, "rc": "cworst" }
    ]
  },
  "files": [
    { "role": "vpr_arch", "path": "arch/vpr_arch.xml" },
    { "role": "timing_data", "path": "corners/tt_0p80v_25c_typ/timing_data.json", "corner": "tt_0p80v_25c_typ" },
    { "role": "timing_lib", "path": "corners/tt_0p80v_25c_typ/timing/fabric.lib", "corner": "tt_0p80v_25c_typ" },
    { "role": "openfpga_arch", "path": "arch/openfpga_arch.xml" },
    { "role": "bitstream_setting", "path": "arch/bitstream_setting.xml" },
    { "role": "yosys_script", "path": "synthesis/synth_script.ys" },
    { "role": "yosys_cell_sim", "path": "synthesis/techlib/cell_sim.v", "license": "MIT" }
  ],
  "flow_defaults": {
    "vpr_options": { "route_chan_width": 20 },
    "openfpga_options": { "write_fabric_verilog": ["--default_net_type", "wire"] }
  }
}
```

## Requirements

### Structure

- **R-1** A package is a directory with `device_package.json` at its root, or an archive (`.tar.gz`, or `.zip`) that unpacks to such a directory.
- **R-2** `format_version` uses `major.minor`. A tool refuses a package with a higher major version than it knows. A higher minor version is accepted, and unknown fields are ignored with a warning.
- **R-3** `name` matches `[a-z][a-z0-9_]*` and equals the directory name.
- **R-4** `version` is a semantic version of the device content. Any change of a file in the package that changes a flow result raises it.
- **R-5** Every path in the manifest is relative, stays inside the package (no `..`, no absolute path, no symlink that leaves the package) and exists.
- **R-6** A package contains no file that is not listed in `files`, other than `device_package.json`, `README.md` and `CHECKSUMS.sha256`.
- **R-7** A package holds one grid size. A device with another grid size is another package.
- **R-8** A package holds no generated fabric netlist (Verilog or SPICE). It is made by `write_fabric_verilog` from the architecture files. `check` fails on a file in `files` that is a generated netlist, which it recognizes by the role: no role in R-20 allows one.
- **R-9** The manifest has a `license` field with an SPDX identifier (or `LicenseRef-<name>` with the text in a `LICENSE` file at the package root) for the package as a whole. A file in `files` may have its own `license` field when it comes from another project; it overrides the package license for that file. `check` fails when the package `license` is missing. `create` asks for the license and does not guess it.

### Content

- **R-10** A package must list exactly one file for each of the roles `vpr_arch` and `openfpga_arch`, and one for `bitstream_setting` when the device supports bitstream generation.
- **R-11** A package for a device with synthesis support must list a `yosys_script` and every techlib file that script reads. A script reads techlib files through paths the tool resolves from the manifest, and does not use absolute paths.
- **R-11a** A script that takes the synthesis commands of an ODC file has a line `${ODC_SYNTHESIS}` right after its `hierarchy` command, where the design is elaborated and still has its RTL names (spec_odc.md, R-24). A tool that runs the script without ODC replaces it with nothing.
- **R-12** A package for a device with bitstream support lists the files needed by `build_architecture_bitstream`, `build_fabric_bitstream` and the fabric writers. These include `fabric_key`, `tile_config`, `repack_constraints`, and `pin_constraints` when the architecture needs them.
- **R-13** Files in `arch/cell_library/` that the OpenFPGA architecture refers to are found through the package, not through `OPENFPGA_PATH`.
- **R-14** `device.grid`, `device.lut_size` and `device.channel_width` are facts about the files. `check` (R-40) compares them with the VPR architecture and reports a mismatch. It reads the architecture with the variables replaced by the values of the default corner.
- **R-14a** `device.flat_routing` (true or false, false when it is missing) says that the flow, up to the bitstream, was checked with flat routing (`router.flat_routing`, VPR's `--flat_routing on`) on this device. The flow of the web app routes flat on a package only when it is true.
- **R-14b** `flow_defaults.vpr_options` maps flags of VPR, without their dashes, to values (text, numbers or true/false): the options that the reference flow of the package gives to VPR, for example `{"constant_net_method": "route"}`. A session that uses the package sets them as application options, found by their VPR flag (`vpr_flag` of `report_app_option --json`), before it reads the architectures. A value that the flow refuses stops the use of the package. These entries are not set: `device` and `route_chan_width`, which `device.grid` and `device.channel_width` give (they are told when they say otherwise), `circuit_format`, which synthesis follows and the flow reads from the extension of the netlist, and a flag that the flow does not know (told).
- **R-14c** `flow_defaults.bitstream` says how the bitstream of the device is made in a session: `link_options` and `fabric_options` (flags of `link_openfpga_arch` and `build_fabric`, `--` and a name, no value) and `fixups` (`check_netlist_naming_conflict`, `pb_pin_fixup`, `lut_truth_table_fixup`). Anything else makes the package unusable. `device.bitstream` (true or false, true when missing) says whether the device can make one. See docs/spec_bitstream.md B-10 and B-11.
- **R-15** A reference bitstream, when present, is for the reference design in `reference/` and the manifest records the design and the SHA-256 of the files, so the package can be tested (R-43). `reference.expected.bitstreams.build_bitstream` is the SHA-256 of the file that `build_bitstream` writes for the reference design in a session (synthesized with the package's script, then pack, place, route; docs/spec_bitstream.md).

### Corners

- **R-16** Files that depend on a design corner are in `corners/<corner_name>/`, not in `arch/` or `synthesis/`. Files that do not depend on a corner (the netlist structure, the bitstream, the techlib) are never repeated in a corner directory.
- **R-17** A corner name matches `[a-z][a-z0-9_]*`, is unique in the package, and equals its directory name. It is listed in `corners.list` with `process`, `voltage` (V), `temperature` (degrees C) and `rc`, the RC extraction corner (for example `typ`, `cworst`, `cbest`, `rcworst`, `rcbest`). Two corners may differ in `rc` only.
- **R-18** When a package has corners, `corners.default` names one of them. The flow uses the default corner when the user gives none. A package with no `corners` field has no corners; its `arch/vpr_arch.xml` then has no timing variables and is used as it is.
- **R-19** A file with a corner-dependent role carries a `corner` field in `files`, and its path is inside that corner's directory. The `timing_data` file (`timing_data.json`) of a corner holds the values for that corner (R-22).
- **R-21** Liberty (`timing_lib`), SDF (`timing_sdf`) and other delay tables (`timing_table`) are only stored and checked for now. The checks are those of every file: listed with a role and a corner (R-19), path rules (R-5), checksum (R-30). The flow does not read them, and `check` does not parse their content.
- **R-22** `arch/vpr_arch.xml` holds a variable of the form `${name}` in place of each timing value (delays, capacitances, resistances) that depends on the corner. `timing_data.json` of a corner maps each variable name to its value for that corner. The tool replaces the variables in memory when it reads the architecture for the selected corner. The file in the package is never changed and no annotated copy is written into the package (R-32); a copy for a run goes to the output directory. Only the values change: the grid, routing and blocks are the same for all corners. The template has no default values, so it is not valid VPR input until the variables are replaced; a tool that does not know packages cannot read it directly.
- **R-23** `check` fails when a variable in `vpr_arch.xml` has no value in the `timing_data.json` of one of the corners, and when a package with no corners has variables in `vpr_arch.xml`. It warns when a `timing_data.json` has a name that is not a variable of `vpr_arch.xml`. A flow run fails with the variable name when a value is missing, and never runs with a variable left in the architecture.
- **R-24** The manifest has `timing_units` with a unit for `time` (`ns`, `ps`, `fs`), `capacitance` (`pF`, `fF`) and `resistance` (`ohm`, `kohm`). All numbers in all `timing_data.json` files of the package are in these units. The kind of a variable comes from the end of its name: `_T` is time, `_C` is capacitance and `_R` is resistance, for example `${lut_T}`, `${wire_C}`, `${switch_R}`. `timing_data.json` is a flat map from the name without `${}` to a number, for example `{"lut_T": 120, "wire_C": 3.5}`. The tool converts each value to the unit VPR reads (seconds, farads, ohms) when it replaces the variable. `check` fails when `timing_units` is missing in a package with `timing_data` files, when a unit is not in the lists, when a variable name does not end in `_T`, `_C` or `_R`, and when a value is not a number.

### I/O map

- **R-25** The I/O map (role `io_map`) is a CSV file, comma- or tab-separated, with a header row that names these columns, in any order. A column of another name is an error. There is one row per I/O subtile of the layout, and one per global port of the fabric.

  | Column | Meaning |
  |--------|---------|
  | `side` | `TOP`, `BOTTOM`, `LEFT`, `RIGHT` of the I/O ring, `CORE` for an I/O inside the grid, empty for a global port. It is the name that the maker gives, not checked against the grid: the pin tables of OpenFPGA call the row `y = 0` `TOP`, which is the bottom of the grid of VPR. `write_io_map` writes the side of the grid of VPR |
  | `pin_direction` | `input`, `output` or `inout` |
  | `pin_type` | `GPIO`, `CONTROL`, `SCAN` or `UNDEF` |
  | `row` | the `y` of the VPR grid, `(0,0)` at the bottom left |
  | `col` | the `x` of the VPR grid |
  | `subtile` | the subtile of the I/O in its tile |
  | `netlist_name` | the name of the pin in the fabric: a port of `fpga_top` (`gfpga_pad_GPIO_PAD[3]`, `clk[0]`) |
  | `customer_pin_alias` | optional: the name that a user of the device knows the pin by (a ball of the package, `LED0`) |

  The rules, which `check` applies with the line of each problem:
  - `row`, `col` and `subtile` name an I/O subtile of the layout. A location has one row for each pin of the fabric there (the input and the output of a pad are two pins, with two `netlist_name`).
  - A global port of the fabric (a clock or a reset) has no tile: its row leaves `row`, `col` and `subtile` empty, and its `pin_type` is `CONTROL`.
  - `netlist_name`, when it is given, is unique. `customer_pin_alias`, when it is given, is unique among the rows of one `pin_direction`: the input and the output of one pad may share it (`pad_fpga_io[0]` for `gfpga_pad_IO_A2F[0]` and `gfpga_pad_IO_F2A[0]`).
  - An I/O subtile of the layout with no row cannot be given by `PACKAGE_PIN`, but `LOC` still places a port on it.

  ODC places a port on a pin of the I/O map by `PACKAGE_PIN` (spec_odc.md, R-32). The pin table, the I/O location map and the PCF of the older commands can be written from the I/O map.
- **R-26** A package may have an ODC file of role `odc_defaults`, read before the ODC file of the user (spec_odc.md, R-46), for example the clock pins of the device.

### Roles

- **R-20** The manifest uses only these roles. A package may add roles that start with `x_`; tools ignore them.

| Role | File | Required when |
|------|------|---------------|
| `vpr_arch` | VPR architecture XML | always |
| `openfpga_arch` | OpenFPGA architecture XML | always |
| `bitstream_setting` | bitstream settings XML | bitstream support |
| `simulation_setting` | simulation settings XML | testbench generation |
| `repack_constraints` | repack design constraints | the arch needs them |
| `pin_constraints` | pin constraints | the arch needs them |
| `fabric_key` | fabric key | fabric uses a key |
| `tile_config` | tile config | tile-based fabric |
| `io_info` | I/O naming and location table | bitstream support |
| `io_map` | I/O map (R-25) | optional |
| `odc_defaults` | default constraints in ODC (R-26) | optional |
| `yosys_script` | Yosys script | synthesis support |
| `yosys_cell_sim` | cell simulation models | synthesis support |
| `yosys_map` | techmap files (LUT, carry, BRAM, DSP) | per architecture |
| `yosys_bram_rules` | memory inference rules | the device has BRAM |
| `sdc` | default timing constraints | optional |
| `timing_data` | values of the `vpr_arch` timing variables for one corner (has `corner`) | `vpr_arch` has variables |
| `timing_lib` | Liberty timing data of one corner (has `corner`), stored and checked only | optional |
| `timing_sdf` | SDF delays of one corner (has `corner`), stored and checked only | optional |
| `timing_table` | other delay tables of one corner (has `corner`), stored and checked only | optional |
| `power_data` | power data of one corner (has `corner`) | optional |
| `corner_info` | `corner.json` of one corner (has `corner`) | optional |
| `arch_bitstream` | architecture bitstream | reference design |
| `fabric_bitstream` | fabric bitstream | reference design |
| `reference_design` | reference RTL | reference design |

### Integrity and compatibility

- **R-30** `CHECKSUMS.sha256` lists every file. `check` fails when a file is missing, extra or changed. Comment lines that hold a date (`//` lines in bitstream files) are ignored in the hash.
- **R-31** `check` fails when the installed OpenFPGA or VTR version is outside the range in the manifest.
- **R-32** A package never writes into itself. A run takes an output directory, and all generated files go there.

### Commands

- **R-40** `openfpga-package check <pkg>` validates the manifest against the schema, applies R-2 to R-6, R-10 to R-14, R-30 and R-31, and exits non-zero with one line per problem.
- **R-41** `openfpga-package create <name> --from-task <task>` collects the files a task uses into a new package, rewrites the paths, and writes the manifest and checksums. It does not guess: a file it cannot classify is reported and left out.
- **R-42** `openfpga-package pack <dir>` and `unpack <archive>` convert between the directory and the archive. Packing is deterministic: the same directory gives the same archive bytes (sorted entries, fixed times, fixed owner).
- **R-43** `openfpga-package test <pkg>` runs the reference design in `reference/` through the flow and compares the results with those recorded in the manifest: the critical path delay, the Fmax, the wirelength, the channel width and the blocks of each type, as VPR logs them, and the SHA-256 of each bitstream file without its `//` comment lines, which hold the date of the run.
- **R-44** Task files accept `device_package = <path>` in place of the separate architecture variables. The variables that a package defines (`vpr_arch`, `openfpga_arch`, `bitstream_setting`, ...) are filled in from the manifest; a value given in the task file wins and is logged.
- **R-45** The Python `Session` takes `Session(device_package=<path>, corner=<name>)`, which reads the VPR architecture and the OpenFPGA architecture from the package, and gives the other files through `session.device_package.path(role)`. `corner` defaults to `corners.default` (R-18).
- **R-46** The shell has `read_device_package --file <dir> [--corner <name>]`, in C++, which the Python `Session`, the MCP server and the web app call. It is refused when `check` would fail, or when the corner is not in the package. It:
  - checks the manifest (R-2 to R-6, R-10, R-14 to R-14b, R-25, R-30);
  - reads the VPR architecture with the layout of `device.grid`, then the OpenFPGA architecture;
  - sets `router.route_chan_width` to `device.channel_width`, and the options of `flow_defaults.vpr_options` (R-14b);
  - keeps for the session what the flow needs later: `device.flat_routing`, the Yosys script and its techlib files, the I/O map, and the `odc_defaults`, which it reads;
  - reads the files of the roles `simulation_setting` and `bitstream_setting`, and keeps `flow_defaults.bitstream` and `device.bitstream` for `build_bitstream` (docs/spec_bitstream.md B-3).

  The package is an input of the session: a checkpoint saves it, the journal has the command, and a replay runs it.
- **R-48** `write_io_map --file <f.csv>` writes a first I/O map for the device of the session: a row for each I/O subtile of the grid with `side`, `row`, `col` and `subtile`, and `netlist_name` once the fabric is built. The maker of the package fills in `pin_type`, `pin_direction` and `customer_pin_alias`.
- **R-47** A task file accepts `device_corner = <name>` next to `device_package`. `openfpga-package check` also verifies that every corner in `corners.list` has a directory and every corner directory is listed.

### JSON

- **R-51** `report_device_package` answers, as JSON (kind `device_package`), the package of the session: its name, version, device, files, flow defaults and what `read_device_package` set or did not set, and why.
- **R-50** `openfpga-package info <pkg> --json` prints the manifest with the resolved absolute paths in the envelope `{schema_version, kind: "device_package", data}` used by the other JSON queries, with a schema in `openfpga_flow/schemas/`.

## Design notes

- A manifest in JSON, not XML: the schemas in `openfpga_flow/schemas` are already JSON, and it is read by Python and by C++ (nlohmann/json, vendored in `libs/EXTERNAL` for `read_device_package`).
- A role table, not a fixed file name per role, so that two devices may name their files differently and one device may have several `yosys_map` files.
- Checksums in a separate file so that a person can regenerate them with `sha256sum` and no tool.
- Code location: the C++ part is `read_device_package`, `report_device_package`, `write_io_map` and the reader of the I/O map. The `openfpga-package` tool is Python, in `openfpga_flow/scripts/`, next to `run_fpga_task.py`.
- Existing devices: a first set of packages is made with `create` from existing tasks, one with logic only, one with BRAM and one with DSP (`basic_tests/generate_fabric`, `fpga_verilog/bram/dpram1k` and `fpga_bitstream/extract_dsp_mode_bit`).

## Test plan

| ID | Check | Covers |
|----|-------|--------|
| T-1 | A valid package passes `check`. Each of these fails with a message that names the problem: missing file, path with `..`, symlink out of the package, extra file, changed file, wrong grid, bad `format_version`. | R-2 to R-6, R-14, R-30 |
| T-2 | `check` fails when `min_version` is higher than the installed OpenFPGA. | R-31 |
| T-3 | `pack` run twice gives identical bytes; `unpack` then `check` passes. | R-1, R-42 |
| T-4 | `create --from-task basic_tests/generate_fabric` gives a package that passes `check`, and `test` on it matches the golden baseline of that task. | R-41, R-43 |
| T-5 | The same task run with `device_package = ...` and with the old variables gives identical `.net`, `.place`, `.route` and bitstream files. | R-44 |
| T-6 | A run from a read-only package directory succeeds. | R-32 |
| T-7 | `Session(device_package=...)` and `read_device_package` give the same device as `read_vpr_arch` and `read_openfpga_arch` on the same files. | R-45, R-46 |
| T-8 | `info --json` validates against its schema. | R-50 |
| T-9 | A package with two corners passes `check`. Each of these fails with a message that names the problem: corner directory not in `corners.list`, listed corner without directory, `default` not in the list, a corner file whose `corner` field and directory differ, a corner with no `rc`, a variable of `vpr_arch.xml` with no value in a corner, a name in `timing_data.json` that is not a variable, a `.lib` file with a changed checksum. | R-16 to R-19, R-21, R-47 |
| T-10 | A run with `corner=A` and a run with `corner=B` read the files of their own corner, and a run with no corner uses `corners.default`. A package with no corners runs as in T-5. | R-18, R-45, R-46 |
| T-11 | `check` fails on a package with no `license`, and on a generated fabric netlist in `files`. A package with a file-level `license` passes, and `info --json` shows it. | R-8, R-9, R-50 |
| T-12 | The same package run for corner A and for corner B gives architectures that differ only in the timing values, equal to the values of each `timing_data.json`. The package files have the same checksums after the run. A run with a value deleted fails with the variable name. | R-22, R-23, R-32 |
| T-14 | An I/O map passes `check`. Each of these fails with its line: an unknown column, a location outside the grid or not an I/O subtile, a location given twice, a duplicate alias, a bad `pin_type`. A global port with no location passes. A tab-separated file passes. `write_io_map` writes a map that passes, and the CSV made from the pin table and the I/O location map of basic_tests/io_constraints/example_pcf places the pins of its PCF as `pcf2place` does. | R-25, R-48 |
| T-15 | `read_device_package` gives the session of the Python reading of today: the same architectures, layout, channel width and options set or not set, and the refusals with the same reasons. `report_device_package` validates against its schema. A checkpoint and a journal restore and replay it. | R-46, R-51 |
| T-13 | Two packages with the same timing, one in `ps` and one in `ns` with the values divided by 1000, give the same architecture after substitution, for `_T`, `_C` and `_R` variables. A missing `timing_units`, an unknown unit, or a variable name with no `_T`, `_C` or `_R` suffix fails `check`. | R-22, R-24 |

The tests are a task in `openfpga_flow/tasks/` and run in the regression tests.

## Open questions

None open.

Decided with the owner: one grid size per package (R-7), no generated fabric netlist (R-8), license in the manifest (R-9), Liberty and SDF only stored and checked (R-21), RC extraction corner is part of a corner (R-17), variables are `${name}` with no default values (R-22), timing units are in the manifest and the kind of a variable comes from its `_T`, `_C` or `_R` suffix (R-24).

