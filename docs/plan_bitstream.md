# The bitstream of a session: plan

A session runs a design from its RTL or netlist to a routed device. This
plan takes it to the end: the bitstream that configures the fabric, made
stage by stage in the session, the same way from the shell, Python, the
model and the web app, with the settings of the device package.

## Decisions (owner, 2026-11-25)

- The scope is the bitstream: repack, the architecture bitstream, the
  fabric bitstream and its file, with a report. The Verilog of the fabric,
  the testbenches and a simulation that checks the bitstream are later.
- `build_bitstream` and `report_bitstream` are commands of the core, next
  to `repack` and the bitstream commands, not of the plugin `ui`.
- The custom commands of a PCF file (a `pcf_config.xml`, as
  `pcf2bitstream_setting` reads them) are out: the bitstream takes only the
  bitstream settings of the package, or a file read by hand. ODC R-35 stays
  refused.

## What is there today

| Part | State |
|---|---|
| Stages of the core | `REPACKED` (needs `ROUTED`, not `ANALYZED`) and `BITSTREAM` (needs `REPACKED`) exist in `OpenFpgaSession`. `repack` reaches `REPACKED`; `build_architecture_bitstream` reaches `BITSTREAM`; `build_fabric_bitstream`, `write_fabric_bitstream` and `report_bitstream_distribution` need it (`stage_guard.h`). A change of an earlier stage drops them. |
| Prerequisites | `link_openfpga_arch` records the VPR run it links to (`record_link`), and `build_fabric` refuses a link that is out of date (`require_link`). They are not stages. Options `build_fabric.*` exist. |
| Tests | `test_equivalence.py`: on the golden designs, the staged flow's fabric bitstream (`plain_text`, no time stamp) equals the one-shot `vpr` flow's, also after a change of seed once the bitstream is made. `test_stage_info.py` and `test_json_query.py` know the two stages. |
| Inputs | The session tracks three inputs: the VPR architecture, the OpenFPGA architecture, the circuit. The simulation settings (`read_openfpga_simulation_setting`) and the bitstream settings (`read_openfpga_bitstream_setting`) are read by hand and are not inputs of the session. |
| Packages | `read_device_package` reads `vpr_arch` and `openfpga_arch` only. The three packages have a `simulation_setting`; only `k4_n4_dsp16_3x4` has a `bitstream_setting`. `reference.expected.bitstreams` is empty in all three, and the bitstream steps are only in each package's `reference/flow.openfpga` (role `x_openfpga_script`), which runs the one-shot `vpr`. |
| Python | No method for the stages after `ANALYZED`. Checkpoints do not save `REPACKED` or `BITSTREAM` (documented). |
| Reports | No JSON answer about a bitstream. `report_bitstream_distribution` writes a file. |
| Model | `run_stage` takes `pack`, `place`, `route`, `analysis`. spec_model_capabilities.md says the model cannot make a bitstream. The tool definitions are at the edge of their budget (`test_app_agent.py`: under 4,000 tokens; 16,0xx characters today). |
| App | FEA-6 and X-54 (spec_web_app.md) are planned: the stages after routing, a report, a download. |
| ODC | `PACKAGE_PIN` and `LOC` place the ports; the pin constraints XML goes to the commands that take `--pin_constraints_file`. R-35 is refused: no package has a `pcf_config.xml`. |

## Design

### The steps from ROUTED to a bitstream file

What a package's `reference/flow.openfpga` does after VPR, in the session:

1. `read_openfpga_simulation_setting` and `read_openfpga_bitstream_setting`
   (the package's files);
2. `link_openfpga_arch` (`--activity_file` when the package has one,
   `--sort_gsb_chan_node_in_edges` from the option of `build_fabric.*`);
3. `check_netlist_naming_conflict --fix`, and the fixups that the package
   needs (`pb_pin_fixup`, `lut_truth_table_fixup`);
4. `build_fabric` (`--compress_routing`, and the fabric key or tile
   options of the package);
5. `repack` (reaches `REPACKED`), `build_architecture_bitstream` (reaches
   `BITSTREAM`), `build_fabric_bitstream`;
6. `write_fabric_bitstream --file bitstream/<design>.bit --format
   plain_text` (or `xml`), and the architecture bitstream
   `bitstream/<design>_arch.xml` when asked.

The steps differ by device (fixups, fabric key, activity file). They come
from the package, not from the person:

- **Package:** the manifest gets `flow_defaults.bitstream` (spec_device_
  package.md R-14b): the options of `link_openfpga_arch` and `build_fabric`,
  the fixups to run, and the formats of the bitstream file. A package
  without it uses the defaults above. A package that cannot make a
  bitstream says `device.bitstream: false`, and the stage is refused with
  that reason.
- **Without a package** (architectures read by hand): the defaults, with
  the simulation and bitstream settings read by hand.

### The shell

- **Inputs:** the simulation settings and the bitstream settings become
  inputs of the session, as the architectures are: in `report_session`,
  in checkpoints (with their checksum) and in the journal. A change of one
  drops `REPACKED` and `BITSTREAM` (the bitstream settings) or nothing
  before `ROUTED` (the simulation settings are read by `link_openfpga_arch`
  only).
- **`read_device_package`** reads the roles `simulation_setting` and
  `bitstream_setting`, and `flow_defaults.bitstream`.
- **`build_bitstream`** (new, in the core, with the bitstream commands of
  `openfpga_bitstream_command_template.h`): runs the steps from the
  session's `ROUTED` to the files, with the recipe of the package. It runs
  the steps that are not valid any more and skips the others (a link and a
  fabric that are still valid are kept). `--file`, `--format plain_text|
  xml`. The commands of each step stay usable by hand, as today.
- **`report_bitstream`** (new, in the core, `--json`, kind `bitstream`, schema
  `openfpga_flow/schemas/bitstream.schema.json`): the files and their
  SHA-256 (without the `//` lines of a date), the number of configuration
  bits, the configuration protocol and its regions, the bits per type of
  block (what `report_bitstream_distribution` counts), and the files of
  the simulation and bitstream settings that were applied.
- **Stages:** `BITSTREAM` is reached by `build_architecture_bitstream` as
  today. `build_bitstream` fails, and the stage is not reached, when a step
  fails; the answer names the step.

### Python

- `Session.read_simulation_setting(file)`, `Session.read_bitstream_setting
  (file)`, as the architectures.
- `Session.bitstream(file=None, format="plain_text")`: runs
  `build_bitstream`, returns `report_bitstream()`. `Session.repack()` for
  the stage alone.
- `run_flow()` stays pack to analysis; `run_flow(bitstream=True)` adds it.
- **Checkpoints:** `REPACKED` and `BITSTREAM` are still not restored (the
  fabric is not saved); the bitstream files are results of the checkpoint
  with their checksum, and `restore` says the bitstream must be made again.
  The journal has `build_bitstream`, so a replay makes the same file.

### The model (MCP)

- `run_stage stage=bitstream`: runs what is missing (pack to route, then
  `build_bitstream`), lists it in `ran`, and answers the summary of
  `report_bitstream`: the file (a name in the workspace), its size, the
  bits, the settings applied. No new tool: the tool definitions are at the
  edge of their budget, and the answer of `run_stage` is enough. The full
  report is in `get_state` and in the HTML report.
- Diagnostics: `bitstream_unsupported` (the package says
  `device.bitstream: false`), `stage_failed` with the step that failed.
- spec_model_capabilities.md: "Generate a bitstream" becomes yes; the
  fabric netlist and the testbenches stay no.

### The web app

- **Stages:** a `BITSTREAM` row after `ANALYZED` in the sidebar, with its
  run button; `REPACKED` is not shown as a row of its own (it is a step of
  the bitstream).
- **Report:** `reports/bitstream.html` (REP-*): the summary, the bits per
  block type, the settings applied, the log of the steps.
- **Files:** the bitstream files in "Results", opened in the viewer (the
  first megabyte, as any file) and downloaded with a link (X-54).

## Names and files

- The files go to `bitstream/` in the session's directory: `<design>.bit`
  (plain text), `<design>.xml` when asked, `<design>_arch.xml`.
- They are written without a time stamp (`--no_time_stamp`) so that a
  checksum compares two runs; the date is in the report.

## Phase 0: try it

1. Run the steps by hand in a session on the three packages (and2, mult8,
   dpram1k): which steps each needs, the time of each, the size of the
   files. The bitstream equals the package's one-shot
   `reference/flow.openfpga`.
2. Time `build_fabric` on the 32x32 package of the work folder, to know
   whether keeping the fabric across a change of placement is worth it
   (Later).

Status: done (2026-11-25), by hand with a script that is not in the
repository.

- The three packages, staged in a session (the reference design, the steps
  above) and one-shot (`vpr ... --pack --place --route --analysis`, then
  the same steps): the fabric bitstream and the architecture bitstream are
  the same, byte for byte (`--no_time_stamp`). The reference scripts of the
  packages run `vpr` without the stage flags, which uses VPR's analytical
  placer: their results are not those of the staged flow.
- The steps that each package needs: `link_openfpga_arch
  --sort_gsb_chan_node_in_edges`, `check_netlist_naming_conflict --fix`,
  `lut_truth_table_fixup`, `build_fabric --compress_routing`; the DSP
  package reads its bitstream settings first.
- The simulation settings: `link_openfpga_arch` needs them read (an unread
  setting leaves the number of clock cycles undefined), and the settings
  of `k6_frac_n10_2x2` infer the number of cycles from the activity of the
  signals, which a design read in a session does not have ("Signal
  activities are not defined"). The number only matters for testbenches:
  `build_bitstream` sets it to the minimum with a warning when there is no
  activity file, and reads no settings at all when the package has none.
- Times on the 2x2 to 3x4 packages: under 0.4 s for all the steps. On
  `k6_frac_n10_mem1k_32x32` (2,046,232 bits): link 3.8 s, `build_fabric`
  15.3 s, the architecture bitstream with its XML file 15.7 s, the fabric
  bitstream 4.5 s, the file 0.5 s. So `build_bitstream` keeps a link and a
  fabric that are still valid, and writes the architecture bitstream only
  when asked (`--arch_file`).

## Phase 1: the specs

- A new `docs/spec_bitstream.md`: the steps, `build_bitstream`,
  `report_bitstream` and its schema, the inputs, what drops what, the
  files, the errors.
- spec_device_package.md: the roles read by `read_device_package`,
  `flow_defaults.bitstream`, `device.bitstream`, the reference bitstreams
  recorded (R-15, R-43).
- spec_odc.md: R-35 says that custom PCF properties are not supported,
  with the reason.
- spec_model_capabilities.md and spec_web_app.md: `run_stage bitstream`,
  FEA-6 and X-54.

## Phase 2: the shell

- The simulation and bitstream settings as inputs of the session, in
  `report_session`, checkpoints and the journal.
- `read_device_package` reads the new roles and `flow_defaults.bitstream`.
- `build_bitstream` and `report_bitstream` in the core, with the schema.
- **Tests:**
  - C++: the recipe of a package, the steps that are skipped when valid;
  - Python: on the golden designs and the three packages, the file of
    `build_bitstream` equals the one-shot flow's (T-6 of ODC);
  - a change of placement, of a bitstream setting, of a simulation setting
    drops what it should;
  - a package with `device.bitstream: false` is refused.

## Phase 3: Python and the packages

- `Session.bitstream`, `repack`, the settings, checkpoints and the journal.
- The three packages:
  - `flow_defaults.bitstream` from their `reference/flow.openfpga`;
  - the expected SHA-256 of the reference bitstream in the manifest;
  - the package test (R-43) compares it.
- **Tests:**
  - a checkpoint after the bitstream restores and makes the same file;
  - a replay of the journal makes the same file;
  - the package tests (`expected.bitstreams.build_bitstream`, in
    `test_bitstream.py`);
  - an ODC with `PACKAGE_PIN` through an I/O map gives the bitstream of
    `pcf2place` on `basic_tests/io_constraints/example_pcf` (T-8 of ODC),
    and one with a pblock and `LOC` the bitstream of the one-shot `vpr`
    with the written XML (T-6).

## Phase 4: the model and the app

- `run_stage stage=bitstream`, its answer within the budget,
  `bitstream_unsupported`.
- The app:
  - the `BITSTREAM` row with its run button;
  - `reports/bitstream.html`;
  - the files in "Results" with a download link.
- The prompt of the agent: one line on the bitstream.
- **Tests:** the MCP test of a flow to the bitstream; the app's view and
  its download; the e2e session runs the bitstream once.

## Status of phases 1 to 4

Done (2026-11-25).

- **Specs:** `docs/spec_bitstream.md` (B-1 to B-43, BT-1 to BT-7);
  spec_device_package.md R-14c, R-15 (`expected.bitstreams.build_bitstream`)
  and R-46; spec_odc.md R-35 (custom PCF properties not supported);
  spec_model_capabilities.md and spec_web_app.md (FEA-6 partial, X-54 for
  the bitstream).
- **Core** (`bitstream_flow_commands.cpp`, in the core next to the
  bitstream commands): `build_bitstream` and `report_bitstream` with
  `bitstream.schema.json`. The session keeps the settings files
  (`read_settings`, recorded by the two read commands), the link and the
  fabric (`record_fabric`), the recipe of the package and what
  `build_bitstream` wrote. `report_session` lists the settings and the
  bitstream. `read_device_package` reads the two settings roles,
  `flow_defaults.bitstream` and `device.bitstream`.
- **Found on the way:**
  - `link_openfpga_arch` needs `read_openfpga_simulation_setting` (a
    dependency of the shell) and `vpr` or `analysis` before it: with no
    settings, `build_bitstream` writes fixed ones next to the file and reads
    them; it runs `analysis` when `ANALYZED` is not reached (it
    synchronizes the packing with the routing);
  - a run of VPR in a process changes the placement of a later design of
    another device in the same process (`k4_n4_dsp16_3x4` after `k6_and2`:
    the same packing, another placement, so another bitstream). It is not
    in the bitstream code: routing another design first is enough. The
    staged and the one-shot flows are equal either way, but the bitstream
    that a package records is the one of a fresh process, and its test
    runs both flows in processes of their own. To look into separately;
  - `read_openfpga_arch` must follow `vpr` in a fresh process (its checks
    of the tile annotations need the VPR architecture);
  - the recipes of the packages have the fix-ups of their reference
    scripts; they do not change the bits of the reference designs.
- **Python:** `Session.bitstream`, `bitstream_report`, `repack`,
  `read_simulation_setting`, `read_bitstream_setting`,
  `run_flow(bitstream=True)`; checkpoints record the settings and the file;
  `restore` reads the settings again.
- **Packages:** the three manifests have `flow_defaults.bitstream` and
  `expected.bitstreams.build_bitstream`, with their checksums.
- **Model:** `run_stage stage=bitstream` with the summary,
  `bitstream_unsupported`, the replay of `build_bitstream` and the reads of
  the settings, the report `reports/bitstream.html`.
- **App:** the `BITSTREAM` stage with its run button and report, the file
  in Results with a download (`/api/download`), one line in the prompt.
- **Tests:** `test_openfpga_session` (the settings, the link, the fabric,
  the output); `test_bitstream.py` (25: the golden designs and the three
  packages against the one-shot flow, what is kept and dropped, the
  file, the format and the architecture bitstream, refusals, the recipe,
  settings that need activities, checkpoints, journals);
  `test_mcp_bitstream.py`; `test_app_viewer.py`; the frontend tests.
- **ODC T-6 and T-8** (`test_bitstream.py`): a pblock, a fixed cell and
  fixed ports give the bitstream of the one-shot `vpr` with the XML that the
  ODC wrote; `PACKAGE_PIN` through the I/O map of a package of the device of
  `basic_tests/io_constraints/example_pcf` gives the bitstream of its
  `pcf2place` flow.

## Later

- The Verilog of the fabric, the SDC of the fabric, the testbenches, and a
  simulation that checks the bitstream against the design.
- Keeping the fabric (`build_fabric`) across a change of placement or
  routing, when phase 0 shows it is worth it: the fabric depends on the
  architecture, not on the design.
- The bitstream drawn on the device view (the bits of a tile).
- Other formats of `write_fabric_bitstream` per package (the protocols'
  own), and the bitstream of a partial region.
- The custom commands of a PCF file (`pcf_config.xml`) as ODC properties
  (R-35), if a package needs them.
