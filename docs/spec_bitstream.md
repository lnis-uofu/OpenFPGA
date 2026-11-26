# Spec: the bitstream of a session

Status: draft for review. Implemented (phases 0 to 4 of docs/plan_bitstream.md): the commands, the settings as inputs, the recipe of the packages, Python, checkpoints and journals, the model and the web app.

## Overview

A session (the flow run stage by stage, docs/source/manual/openfpga_shell/python_session.rst) runs a design to a routed device. This spec takes it to the bitstream that configures the fabric: one command, `build_bitstream`, runs the steps of OpenFPGA from the routing to the file, with the settings of the device package, and `report_bitstream` says what was made.

The commands of each step (`link_openfpga_arch`, `build_fabric`, `repack`, `build_architecture_bitstream`, `build_fabric_bitstream`, `write_fabric_bitstream`) stay as they are. `build_bitstream` runs them in the order and with the options that the device needs, and keeps what is still valid.

Out of scope: the Verilog of the fabric, its SDC, the testbenches, a simulation, and the custom commands of a PCF file (`pcf_config.xml`, see spec_odc.md R-35).

## Glossary

| Term | Meaning |
|---|---|
| Architecture bitstream | The bits of each block of the fabric, independent of how they are loaded (`build_architecture_bitstream`). |
| Fabric bitstream | The bits in the order of the configuration protocol (`build_fabric_bitstream`), written by `write_fabric_bitstream`. |
| Recipe | The options of the steps for a device: from the device package (`flow_defaults.bitstream`), or the defaults. |
| Settings | The simulation settings (`read_openfpga_simulation_setting`) and the bitstream settings (`read_openfpga_bitstream_setting`). |

## The settings as inputs

- **B-1** The files of the simulation settings and of the bitstream settings that the shell read are inputs of the session. `report_session` lists them (`inputs.simulation_setting`, `inputs.bitstream_setting`: the files, in the order they were read, or null). A read without `--append` replaces the list.
- **B-2** Reading bitstream settings drops `REPACKED` and `BITSTREAM`, and the link: `link_openfpga_arch` applies them. Reading simulation settings drops the link only: nothing that a bitstream is made of depends on them.
- **B-3** `read_device_package` reads the package's files of the roles `simulation_setting` and `bitstream_setting` (the first of each), after the architectures, with the commands of B-1.

## The recipe

- **B-10** A device package says how its bitstream is made with `flow_defaults.bitstream`, an object:
  - `link_options`: flags of `link_openfpga_arch` (default `["--sort_gsb_chan_node_in_edges"]`);
  - `fixups`: the commands run after the link, from `check_netlist_naming_conflict`, `pb_pin_fixup` and `lut_truth_table_fixup` (default none; `check_netlist_naming_conflict` runs with `--fix`);
  - `fabric_options`: flags of `build_fabric` (default `["--compress_routing"]`).

  A flag is `--` and a name of letters, digits and `_`, with no value. Anything else makes the package unusable, with the reason (spec_device_package.md).
- **B-11** `device.bitstream` (true or false, true when it is missing) says that the device can make a bitstream. With false, `build_bitstream` is refused with that reason.
- **B-12** A session without a package uses the defaults of B-10.

## build_bitstream

- **B-20** `build_bitstream [--file <f>] [--format plain_text|xml] [--arch_file <f>]` needs `ROUTED`, in the flow run stage by stage. It runs:
  1. `analysis`, when `ANALYZED` is not reached: it makes what `vpr` makes after the routing, the packing synchronized with the routing, which the link and `repack` need;
  2. `link_openfpga_arch` with the recipe's options, then its fixups, when the link is not valid: never made, made for another run of VPR (a stage ran again), or dropped by B-2;
  3. `build_fabric` with the recipe's options, when the fabric was not built on the current link;
  4. `repack`, `build_architecture_bitstream` (with `--write_file <arch_file>` when it is given), `build_fabric_bitstream`, and `write_fabric_bitstream --file <f> --format <format>`, always.

  The files are written without a time stamp. The default file is `bitstream/<circuit>.bit` in the current directory, where `<circuit>` is the name of the circuit's file without its extension; the folder is made.
- **B-21** `link_openfpga_arch` needs simulation settings, and the bitstream does not depend on them. Before the link:
  - when the session read none, `build_bitstream` writes fixed ones (`simulation_setting.xml`, 2 clock cycles) in the folder of the bitstream file and reads them, so they are in `report_session` and in the answer;
  - when the settings infer the number of clock cycles from the activity of the signals, which a session does not have, the number is set to the minimum (2), with a warning: it is used by testbenches only.
- **B-22** A step that fails stops the command. The error names the step, and `BITSTREAM` is not reached; `report_session` gives it as the stage that failed.
- **B-23** On success the session records the files (B-30), and the time of the command as the time of `BITSTREAM`. The answer is the one of `report_bitstream`.
- **B-24** Running a step by hand is still allowed, as today; the session tracks the link (with `record_link`) and the fabric (with the generation of the link it was built on) the same way.

## report_bitstream

- **B-30** `report_bitstream` answers JSON, kind `bitstream` (`openfpga_flow/schemas/bitstream.schema.json`), when `BITSTREAM` is reached:
  - `file`, `format`, `bytes`, `sha256` of the file that `build_bitstream` wrote (null when the bitstream was made by hand);
  - `arch_file` and its `sha256`, or null;
  - `bits`: the number of configuration bits of the fabric bitstream;
  - `protocol`: the type of the configuration protocol, and `regions`: the number of its regions;
  - `blocks`: for each block under the top of the architecture bitstream, its name and its bits (the first level of `report_bitstream_distribution`), largest first, at most `--limit` (default 50) with `truncated`;
  - `settings`: the simulation and bitstream settings files that the bitstream was made with;
  - `steps`: the steps that the last `build_bitstream` ran, and those it kept.
- **B-31** The SHA-256 of a file is of its bytes; the files have no time stamp (B-20).

## What drops what

| Change | Drops |
|---|---|
| A stage up to `ROUTED` runs again (an option, an input, a constraint) | `REPACKED`, `BITSTREAM`, the link and the fabric (as today) |
| Bitstream settings read | `REPACKED`, `BITSTREAM`, the link and the fabric |
| Simulation settings read | the link and the fabric |
| `ANALYZED` runs again | nothing of the bitstream |

## Python, the model and the app

- **B-40** `Session.bitstream(file=None, format="plain_text", arch_file=None)` runs `build_bitstream` and returns `report_bitstream()`. `Session.read_simulation_setting(file)` and `Session.read_bitstream_setting(file, append=False)` read the settings. `run_flow(bitstream=True)` runs the bitstream after the analysis.
- **B-41** A checkpoint records the settings files with their checksum, and the bitstream files as results with theirs. `restore` reads the settings again; it does not restore `REPACKED` nor `BITSTREAM` (the fabric is not saved), and says so. The journal has `build_bitstream` and the reads of the settings.
- **B-42** The model: `run_stage stage=bitstream` runs the stages up to `ROUTED` that are missing, then `build_bitstream`; its answer has the summary of `report_bitstream` (file, bytes, bits, protocol, settings). A package with `device.bitstream: false` gives the diagnostic `bitstream_unsupported`.
- **B-43** The web app: a `BITSTREAM` stage in the sidebar with its run button, a report `reports/bitstream.html`, and the bitstream file in the results, opened in the viewer and downloaded.

## Test plan

| ID | Check | Covers |
|----|-------|--------|
| BT-1 | On the golden designs and the three packages, the file of `build_bitstream` equals the one of the one-shot `vpr` flow with the same steps. | B-20 |
| BT-2 | A second `build_bitstream` keeps the link and the fabric; a change of placement, of bitstream settings, of simulation settings makes again what B-2 says. | B-2, B-20 |
| BT-3 | A package with `device.bitstream: false` is refused; a package with a bad `flow_defaults.bitstream` cannot be used. | B-10, B-11 |
| BT-4 | The settings of `k6_frac_n10_2x2` (cycles inferred) give a bitstream with a warning. | B-21 |
| BT-5 | `report_bitstream` validates against its schema; its SHA-256 is the one of the file. | B-30 |
| BT-6 | A checkpoint after the bitstream restores its settings, and `build_bitstream` makes the same file; a replay of the journal too. | B-41 |
| BT-7 | `run_stage stage=bitstream` through the MCP server, and the app's stage, report and download. | B-42, B-43 |
