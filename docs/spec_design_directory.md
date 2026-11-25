# Spec: OpenFPGA design directory

Status: proposal for discussion (https://github.com/lnis-uofu/OpenFPGA/discussions/2896). Partly implemented: the MCP server reads a design directory to synthesize it (in Python); `read_design` in C++ and the role `odc` are planned (docs/plan_odc_constraints.md). It is the counterpart of the device package spec (`docs/spec_device_package.md`): the device package describes a device, the design directory describes a design.

## Overview

A **design directory** is one directory that holds everything that belongs to one design: the RTL, optionally a netlist that was already synthesized, the testbench, the memory initialization files, the timing constraints, and a manifest that says what each file is.

Today a design is a set of loose files. Nothing says which of several Verilog files is the top, which `.hex` file a memory reads, which SDC file belongs to the design, or whether a BLIF file still matches the RTL next to it. This spec defines the layout, the manifest and the rules for a design directory. It does not change any file format.

## Glossary

| Term | Meaning |
|------|---------|
| Design | One piece of hardware with one top module. |
| Manifest | `design.json` at the root of a design directory. |
| Role | The kind of a file in the design directory, for example `rtl` or `sdc` (R-20). |
| Derived file | A file that a tool made from the sources, for example a synthesized netlist. |

## Goals and non-goals

Goals:

- One unit per design that can be copied, versioned, archived and checked.
- The top module, the file list, the constraints and the memory files are stated, not guessed.
- A tool can tell without running the flow whether the directory is complete and whether a derived file is out of date.
- A design is portable to any device.

Non-goals:

- A build system: the manifest names files and a few settings, with no rules or dependencies.
- A place for results. Netlists, placement, routing, bitstreams, logs and reports of a run are never written here (R-15).
- Several top modules in one directory. A variant is another directory.
- Device files. They belong to the device package.

## Layout

```
<design_name>/
  design.json                  manifest (required)
  README.md                    free text for people (optional)
  LICENSE                      license text when `license` is a LicenseRef (optional)
  rtl/                         Verilog, SystemVerilog and VHDL design files (.v, .sv, .vhd, .vhdl), any number
    include/                   headers read by `include (.vh, .svh)
  netlist/                     netlists that were synthesized already (optional): .blif, .eblif or .edif
  tb/                          testbench and the files it reads (optional)
  mem/                         memory initialization files: .hex, .bin, .mem, .mif (optional)
  constraints/                 ODC, SDC, pin, placement and repack constraints (optional)
  activity/                    switching activity, .act (optional)
  docs/                        notes, datasheet (optional)
  CHECKSUMS.sha256             SHA-256 of every file except itself (optional while a design is in development)
```

Paths in the manifest are relative to the design directory. The directory names are a convention; the manifest decides what a file is (R-20).

## Manifest

`design.json` is validated by `openfpga_flow/schemas/design.schema.json`.

```json
{
  "format_version": "1.0",
  "name": "dual_port_ram_8x16",
  "version": "0.1.0",
  "description": "Dual-port RAM, 8-bit address, 16-bit data, initialized from a hex file",
  "license": "MIT",
  "origin": { "source": "opencores", "url": "https://opencores.org/projects/example", "revision": "r42" },
  "top": "dual_port_ram_8x16",
  "files": [
    { "role": "rtl", "path": "rtl/dual_port_ram.v" },
    { "role": "rtl", "path": "rtl/dual_port_ram_8x16.v" },
    { "role": "rtl_include", "path": "rtl/include/defs.vh" },
    { "role": "mem_init", "path": "mem/init.hex", "format": "hex", "used_by": "rtl/dual_port_ram.v" },
    { "role": "sdc", "path": "constraints/dual_port_ram_8x16.sdc", "stage": "implementation" },
    { "role": "sdc", "path": "constraints/dual_port_ram_8x16_synth.sdc", "stage": "synthesis" },
    { "role": "testbench", "path": "tb/dual_port_ram_8x16_tb.v", "top": "dual_port_ram_8x16_tb", "stage": "functional" },
    { "role": "netlist", "path": "netlist/dual_port_ram_8x16_yosys.blif", "label": "yosys",
      "made_from": { "rtl_sha256": "<hash>", "tool": "yosys 0.51", "lut_size": 6 } }
  ],
  "rtl": {
    "include_dirs": ["rtl/include"],
    "defines": { "DATA_WIDTH": "16" },
    "read_options": ["-nolatches"]
  },
  "clocks": [
    { "name": "clk", "port": "clk", "period_ns": 5.0 }
  ],
  "needs": { "min_lut_size": 4, "features": ["bram"] }
}
```

## Requirements

### Structure

- **R-1** A design directory has `design.json` at its root, or it is an archive (`.tar.gz` or `.zip`) that unpacks to such a directory. The manifest is required.
- **R-2** `format_version` uses `major.minor`. A tool refuses a higher major version and accepts a higher minor version with a warning for fields it does not know.
- **R-3** `name` matches `[a-z][a-z0-9_]*` and equals the directory name.
- **R-4** Every path in the manifest is relative, stays inside the design directory (no `..`, no absolute path, no symlink that leaves it) and exists.
- **R-5** `top` names one module. `check` verifies that an `rtl` file declares a module (Verilog, SystemVerilog) or an entity (VHDL) of that name (a text scan, no elaboration). A design with a `netlist` and no `rtl` takes its top from the `.model` of a BLIF or EBLIF netlist, or from the `design` statement of an EDIF netlist.
- **R-6** The directory has no file that is not listed in `files`, other than `design.json`, `README.md`, `LICENSE` and `CHECKSUMS.sha256`.
- **R-7** A source file reads other files only from inside the design directory: `` `include`` through `rtl.include_dirs` or relative to the file, and `$readmemh` and `$readmemb` through `mem_init` files. A name that is absolute, starts with `~` or has `..` makes `check` fail.
- **R-8** The manifest has a `license` (SPDX identifier, or `LicenseRef-<name>` with the text in `LICENSE`). A file may have its own `license` when it comes from another project.
- **R-9** `origin` is optional and says where a design that was copied comes from: `source`, `url` and `revision`.

### Content

- **R-10** A design lists at least one `rtl` file or one `netlist` file. An `rtl` file is Verilog (`.v`), SystemVerilog (`.sv`) or VHDL (`.vhd`, `.vhdl`); the suffix says which, and a file may carry `"language"` with `verilog`, `systemverilog` or `vhdl` to override it. A design may mix languages. A tool that cannot read a language says so and does not skip the file.
- **R-11** `rtl.defines` are `NAME: value` pairs with a name and a value of letters, digits and `_`. `rtl.read_options` is a list of options of the front end that reads the RTL (for Yosys `read_verilog`, a letter or more after `-`); the options that a language needs, for example `-sv`, are added by the tool. Nothing else from the manifest is written into a synthesis script.
- **R-12** `needs` says what the design requires from a device: `min_lut_size`, and `features` from `bram`, `dsp`, `carry`. It is advice about the design, not a constraint on the flow. A tool that knows the device reports what is missing before synthesis.
- **R-13** `clocks` lists each clock with its `name`, the `port` that carries it and `period_ns`. When an `sdc` file of the stage `implementation` or of both stages is present, `check` compares: a clock with no `create_clock` for its port, or the reverse, is a warning; a different period is an error.
- **R-14** A design is portable to any device. The manifest has no field that names a device or a device package. The device is always chosen by the user of the design.
- **R-15** A run never writes into a design directory. Generated files go to the output directory of the run.
- **R-19** A `testbench` with the stage `post_layout` is for the netlist written after place and route, and one with the stage `functional` is for the RTL or the synthesized netlist. A design may have one of each; the manifest states which is which and a tool does not decide it from the file name.

### Roles

- **R-20** The manifest uses only these roles. A design may add roles that start with `x_`; tools ignore them.

| Role | File | Notes |
|------|------|-------|
| `rtl` | Verilog (`.v`), SystemVerilog (`.sv`) or VHDL (`.vhd`, `.vhdl`) design file | one or more |
| `rtl_other` | a file that the RTL needs and that is not one of the above | for example a structural Verilog model (`.vm`) or a data table |
| `rtl_include` | header read by `` `include`` | `.vh`, `.svh` |
| `netlist` | synthesized netlist | `.blif`, `.eblif` or `.edif`; derived; several allowed, each with a `label` (R-25) |
| `testbench` | simulation top | never synthesized; has its own `top`; `stage` is `functional` (default) or `post_layout` (R-19) |
| `stimulus` | a file the testbench reads | vectors, expected outputs |
| `mem_init` | memory initialization | `format`: `hex`, `bin`, `mem`, `mif`; `used_by` names the `rtl` file that reads it |
| `odc` | constraints in ODC (spec_odc.md): timing, synthesis, floorplanning, routing, pins | one at most; R-23a |
| `sdc` | timing constraints | `stage`: `synthesis`, `implementation` or both (default); R-23 |
| `pin_constraints` | pin constraints | `format`: `xml` (OpenFPGA, default), `csv` or `pcf`; R-24 |
| `placement_constraints` | placement constraints (`.place`) | R-24 |
| `repack_constraints` | OpenFPGA repack constraints XML | R-24 |
| `activity` | switching activity (`.act`) | for power |
| `doc` | documentation | stored and checked only |

### Memory initialization

- **R-21** A `mem_init` file is listed with its `format` and the `rtl` file that reads it. Yosys reads a `$readmemh` path relative to its working directory, so a tool that synthesizes the design provides the file there under the name the RTL uses, as a copy in the output directory.
- **R-22** `check` verifies that every `$readmemh` and `$readmemb` in an `rtl` file names a `mem_init` file by its base name, and that every `mem_init` file is named by one. A missing file is an error. A name that differs only in the directory part is a warning. A `mem_init` file with the format `mif` is a warning: Yosys does not read it, so the design does not synthesize with it as it is.

### Constraints

- **R-23** An `sdc` file of the stage `implementation` or of both stages is given to VPR (`--sdc_file`); a file of the stage `synthesis` is given to the synthesis step only. VPR reads `create_clock`, `set_input_delay`, `set_output_delay`, `set_clock_groups`, `set_false_path`, `set_max_delay` and `set_multicycle_path`; `check` warns for any other command because it is ignored.
- **R-23a** An `odc` file holds the constraints of the design in one file (spec_odc.md). A design with an `odc` file does not also have an `sdc` of the stage `implementation` or a `placement_constraints` file: `check` fails on the pair, as the two would set the same things. When the device package has `odc_defaults`, they are read first.
- **R-24** `pin_constraints`, `placement_constraints` and `repack_constraints` are given to the commands of the flow that take them. A `pin_constraints` file has a `format` (`xml`, `csv` or `pcf`), and a tool that cannot read a format says so. When the device also has default constraints of the same kind, the file of the design is used.

### Derived files

- **R-25** A `netlist` is derived and never overrides the RTL. A design may list several netlists, each with a unique `label` (for example the tool or the configuration that made it) and its own `made_from`: the hash of the `rtl`, `rtl_other` and `rtl_include` files and of the options of `rtl` (`rtl_sha256`), the tool and its version, and the LUT size. A tool uses the `rtl` and synthesizes it, and uses a `netlist` only when asked, by its label, or when the design has no `rtl` (and then the design has one netlist, or the user gives the label). `check` warns when `rtl_sha256` of a netlist differs from the hash of the current files. A netlist with no `made_from` is accepted as a source, for example a circuit from a benchmark set.

### Integrity

- **R-26** At most 20 `rtl` files, 1 MiB per source file and 256 MiB per design directory.
- **R-27** `CHECKSUMS.sha256` lists every file. `check` fails when a file is missing, extra or changed. The file is optional while a design is in development (`check --no-checksums`).

### Commands

- **R-30** `openfpga-design check <dir>` validates the manifest against the schema, applies R-2 to R-13, R-19, R-21 to R-27, and exits non-zero with one line per problem.
- **R-31** `openfpga-design create <dir>` writes a manifest for an existing directory: it classifies files by suffix and folder, finds the top when exactly one module is not instantiated by another, and reports any file it cannot classify and leaves it out. It does not guess a top that is not unique.
- **R-32** `openfpga-design pack <dir>` and `unpack <archive>` convert between the directory and the archive, deterministically (sorted entries, fixed times, fixed owner).
- **R-34** The shell has `read_design --file <dir> [--netlist <label>]`, in C++, which the Python `Session`, the MCP server and the web app call. It:
  - checks the manifest (R-2 to R-7, R-10, R-11, R-25, R-26);
  - synthesizes the `rtl` with the Yosys script of the device package of the session, through the shell's `yosys` command, or reads the `netlist` of the label (R-25), and reads the result as the circuit;
  - reads the `odc` file, or the `sdc` file of the stage `implementation` as the SDC of VPR.

  It needs a device package in the session when the design has RTL, and a build with Yosys to synthesize. The design is an input of the session: a checkpoint saves it, and a journal replays it.
- **R-33** `openfpga-design info <dir> --json` prints the manifest with resolved absolute paths in the envelope `{schema_version, kind: "design", data}` of the other JSON queries, with a schema in `openfpga_flow/schemas/`.

## Test plan

| ID | Check | Covers |
|----|-------|--------|
| T-1 | A valid design directory passes `check`. Each of these fails with a message that names the problem: missing file, path with `..`, symlink out of the directory, extra file, changed file, bad `format_version`, a `top` that no file declares, a `defines` value with a space or `;`. | R-2 to R-6, R-11, R-27 |
| T-2 | A source file with `` `include "/etc/passwd"``, `$readmemh("../x.hex")` or a path with `~` fails `check`. | R-7 |
| T-3 | `$readmemh("init.hex")` with `init.hex` in `mem/` passes. The same with no `mem_init` entry fails. A `mem_init` that no file reads is a warning, and so is a `mif` file. | R-21, R-22 |
| T-4 | An SDC whose clock period differs from `clocks` fails `check`. A clock in one and not the other is a warning. A command outside the VPR subset is a warning. | R-13, R-23 |
| T-5 | A design with `rtl` and a matching `netlist` is synthesized from the `rtl` by default. After a change of an RTL file, `check` warns about the netlist. A netlist without `made_from` gives no warning. | R-10, R-25 |
| T-6 | A design with a `.sv` file and a `.v` file passes `check`. A `.v` file with `"language": "systemverilog"` is read as SystemVerilog. A tool that cannot read SystemVerilog fails with the file name and does not skip it. A `top` declared in a `.sv` file is found. | R-5, R-10, R-11 |
| T-7 | A design with an EDIF `netlist` and no `rtl` passes `check`, and its top is the one in the `design` statement. The same holds for `.blif` and `.eblif`, with the top from `.model`. A `netlist` of another suffix fails. | R-5, R-20 |
| T-8 | `create` on a directory with one top gives a manifest that passes `check`. With two candidate tops it makes none and lists them. A file it cannot classify is reported and left out. | R-31 |
| T-9 | `pack` run twice gives the same bytes; `unpack` then `check` passes. | R-1, R-32 |
| T-10 | The same design runs on two device packages with no change of the design directory. After a run the checksums of the directory are unchanged, also when it is read-only. | R-14, R-15 |
| T-11 | `info --json` validates against its schema. | R-33 |
| T-12 | A design with a `.vhd` file and a `.sv` file and an `rtl_other` file passes `check`. A `top` declared as a VHDL entity is found. A design with `origin` passes, and `info --json` shows it. | R-5, R-9, R-10 |
| T-13 | An `sdc` file of the stage `synthesis` is not given to VPR and is not compared with `clocks`. A `pin_constraints` file with `format` `csv` or `pcf` passes `check`, and one with another format fails. | R-13, R-23, R-24 |
| T-14 | A design with a `functional` and a `post_layout` testbench passes `check`. A `testbench` with another stage fails. | R-19 |
| T-16 | `read_design` on a design with RTL gives the netlist that `synthesize design=...` of the MCP server gives today, and on a design with only a netlist reads it. A design with an `odc` file and an `sdc` of the stage `implementation` fails `check`. A checkpoint restores the design, and a journal replays it. | R-23a, R-34 |
| T-15 | A design with two netlists with different labels passes `check`. A duplicate label fails. Each netlist warns separately when the RTL changes. A tool with no label and with `rtl` synthesizes; with a label it reads that netlist. | R-25 |

The tests are a task in `openfpga_flow/tasks/` and run in the regression tests. The example designs are made from existing benchmarks.
