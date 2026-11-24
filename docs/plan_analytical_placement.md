# Analytical placement in the stage-by-stage flow: plan

VPR's analytical placement (AP, `vpr --analytical_place`) packs and places in
one step: global placement, a full legalizer that makes the clusters, and a
detailed placer. The stage-by-stage flow (pack, place, route, analysis) runs
the classic packer and placer only.

## What is known

- The one-shot `vpr --analytical_place --route --analysis` works with the
  architectures of OpenFPGA up to the bitstream (k6 and k4 golden designs,
  with a latch).
- AP is deterministic: two runs give the same `.net`, `.place`, `.route` and
  bitstream, so the staged flow can be compared with the one-shot command
  byte for byte.
- The AP options are in the option table (13 `aplace.*` options); they change
  PLACED, and the four `appack_*` ones change PACKED.
- VPR makes the device inside the AP flow (`DeviceSizeEstimator`,
  `FullLegalizer::recreate_device_if_needed`), while the staged flow makes it
  in DEVICE, before the stages. On a fixed layout, AP makes the grid again (of
  the same size) and keeps the routing graph that exists. The dedicated clock
  networks are set up again only with `--clock_modeling dedicated_network`.
- The shell's `vpr` command with no stage option already runs AP (the default
  of this VTR); it is not changed.

## Design

- The placer is an option of the flow, `place.placer`: `classic` (pack, then
  place) or `analytical`. With `analytical`, `pack` and `place` both run AP
  and reach PACKED and PLACED at once. It was first planned as a command
  (`analytical_place`), and changed in phase 1: an option is saved by a
  checkpoint and set again by restore, and `run_stage` of the model, which
  runs `pack` before `place`, works as it is.
- The packing and the placement of AP are one result. With the analytical
  placer, a change that drops the placement drops the packing too (an
  option `place.*` or `aplace.*`, a failure). `place.placer` changes PACKED.
- The stages that are run again on fresh data (`prepare_vpr`) run AP again,
  unless the packing was read from a file (a checkpoint that is restored).

## Phase 0: try it

Run AP after the DEVICE stage of the staged flow, and check that it gives the
files of the one-shot command, with `--clock_modeling route` and with
dedicated clock networks.

Status: done, with a trial command `analytical_place`
(`run_analytical_place` in staged_flow.cpp). It runs
`run_analytical_placement_flow` on the run at DEVICE, writes the `.place`
file as `vpr_flow` does, and completes PACKED and PLACED. Phase 1 replaced
the command with the option `place.placer`.

- The 9 golden designs of test_equivalence.py, the 2 that are routed flat
  among them, ran `analytical_place`, `route` and `analysis`. The `.net`,
  `.place`, `.route` and `.net.post_routing` files, the results in the log
  and the bitstream are the same as those of
  `vpr --analytical_place --route --analysis`. This holds although the staged
  flow starts VPR with `--pack --place --route --analysis`: the AP options are
  read whatever the stages are.
- Making the device before AP does no harm on a fixed layout. AP makes the
  grid again with the same size, and keeps the routing graph.
- Dedicated clock networks: no architecture of OpenFPGA has them, so the
  staged flow cannot run with them. `setup_clock_networks` clears what an
  earlier call made, so setting them up again in AP is safe in any case.
- Found for phase 1: the trial does not record that AP made the placement. A
  route that is run again after an option changed replays the classic pack
  and place on fresh data, and the placement changes. The record in the
  session (phase 1, item 1) is needed.

## Phase 1: the stage (C++)

1. `do_analytical_place` in staged_flow.cpp, the option `place.placer`, and
   AP run again in `prepare_vpr`.
2. The rule that drops both stages, and the failure messages.
3. `report_run`: what AP reports (global placement: wirelength and critical
   path; the detailed placement).
4. Tests: the files, the results and the bitstream of the one-shot command on
   the golden designs; the stages dropped by an option; a checkpoint restored;
   AP with flat routing.

Status: done (test_analytical_placement.py, 18 tests).

- `place.placer` (app_option_table.cpp) changes PACKED. With `analytical`,
  `pack` and `place` run AP (`run_analytical_stages`), and the log of the
  command is kept for both stages. The time is kept for PLACED.
- `OpenFpgaSession::dropped_with`: with the analytical placer, dropping
  PLACED drops PACKED, for an option and for a failure. A stage that is
  completed is not affected.
- `prepare_vpr` runs AP again for PLACED or ROUTED. A packing read from a
  file is read again instead. The placement alone cannot be made again on
  the packing of AP, so `read_placement` on it is refused with the reason.
- A failure of AP: "Analytical placement failed", on PACKED, which drops
  both.
- `report_run` placement: `placer`, and `global_placement` (estimated
  wirelength and critical path delay, from the log). The other figures are
  those of the detailed placer, which is the annealer by default and logs as
  the classic placer does.
- Tests: the 9 golden designs, the flat ones among them, give the files, the
  results and the bitstream of `vpr --analytical_place --route --analysis`.
  Also: what an option drops, a route that is run again (same placement), the
  run report against its schema, a checkpoint restored (same timing and
  bitstream, still one result), and a failure.
- For phase 2: with the analytical placer, `run_stage place` of the model
  runs `pack` first and then `place`, which runs AP twice. It should stop
  after `pack`.

## Phase 2: the model and the app

1. The model: `set_option place.placer analytical`, and `run_stage` that
   does not run AP twice; the tool descriptions within their budget.
2. The report of the stage: a section for AP.
3. The app: a choice of placer (classic or analytical) in the sidebar, which
   sets the buttons of the stages.
4. Docs: spec_model_capabilities.md, spec_web_app.md, python_session.rst.

Status: done.

- `run_stage` (tools.py) does not run AP twice: when the stages it ran
  first made the asked stage, it stops, and "ran" says what ran. With the
  analytical placer, pack and place write both reports. The report of place
  says the placer and the estimates of the global placement. The description
  of `run_stage` says it, within the budget of the tool descriptions.
- The app: PLACED shows the placer of the session after its name, greyed
  out (UI-26a). It is set with `set_app_option --name place.placer --value
  analytical`, not in the sidebar. The run buttons of PACKED and PLACED say
  that they run the analytical placer.
- Docs: spec_model_capabilities.md, DESIGN.md, spec_web_app.md (UI-26a,
  FEA-37), python_session.rst.
- Tests: test_analytical_placement.py (the model runs AP once, both
  reports), test_app_placer.py (the option set in the console, then the buttons), the
  tests of the sidebar in components.test.tsx.

## Phase 3: devices

Run AP to the bitstream on the three device packages. If one does not work
(for example the DSP tiles that are several rows high), the manifest says it,
as `device.flat_routing` does.

Status: done; no package needs a flag.

- The reference design of each package (logic only: and2; DSP: mult8;
  memory: dual_port_ram_1k) was synthesized with the package's script by
  the model's tools. Then it went through pack, route and analysis with the
  analytical placer, and on to the bitstream with the commands of the
  package's reference flow. The files, the results and the bitstream are
  those of `vpr --analytical_place --route --analysis` with the same options.
- This was done on the 3 packages of the repository, and on the 32x32
  packages of the same architectures in the work folder of the web app. On
  32x32, a package took 1.5 to 2.5 minutes for the synthesis and three flows
  (analytical, classic, and the one-shot command), and less than 0.9 GB.
- test_analytical_placement.py checks the 3 packages of the repository.
- Seen on the way, not about AP: a package's `flow_defaults.vpr_options`
  (for example `constant_net_method route`, or `clock_modeling` left at
  the default of VPR for the memory package) were not set by
  `use_device_package`. They are now (R-14b of spec_device_package.md).
  The memory package does not
  route at its channel width with `--clock_modeling route`, with either
  placer.

## Out of scope

Stopping after global placement (`--global_place`), and reading or writing
flat placement files (`--read_flat_place`, `--legalize`), unless they are
asked for.
