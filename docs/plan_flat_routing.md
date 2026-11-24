# Flat routing in the stage-by-stage flow: plan

Flat routing (`router.flat_routing`, VPR's `--flat_routing on`) routes the nets
of the atom netlist through the routing inside the clusters as well as between
them. The one-shot `vpr` command supports it, and regression tasks use it
(`fpga_bitstream/flat_routing/*`, `fpga_bitstream/repack_bus_mux`). The
stage-by-stage flow (pack, place, route, analysis) refuses it today.

## Scope

- The stage-by-stage flow only, as the web app and its model run it.
- A fixed channel width only: the app refuses to route without one
  (`Session.fixed_width_refusal`), and loading a routing needs one anyway.
- Out of scope: VPR's search for the smallest channel width and the one-shot
  `vpr` command, which the app does not run. They keep working in the shell
  and the Python API as they do today.

## What VPR does differently with flat routing

In `vpr_flow` (vpr/src/base/vpr_api.cpp):

- the router works on the atom netlist, not the clustered one, so the route
  trees are indexed by atom net;
- the port equivalences of the architecture are turned off before routing;
- the routing resource graph includes the nodes inside the clusters;
- analysis syncs the clustering to the routing with
  `sync_netlists_to_routing_flat`, which OpenFPGA's repack relies on;
- the delay calculators are made with `is_flat`.

## Phase 1: route and analysis (C++)

1. `do_route` (openfpga/src/base/staged_flow.cpp): drop the refusal. When
   flat, route the atom netlist and turn off the port equivalences first, as
   `vpr_flow` does. The run is always fresh for a route (`prepare_vpr` starts
   again when the last stage is not PLACED), so the device is changed once.
2. `do_analysis`: pass the atom netlist and `is_flat`, so the clustering is
   synced to the flat routing.
3. `query_timing.cpp`: drop the two refusals; load the net delays of the
   netlist that was routed, and make the delay calculator with `is_flat`.
4. `query_design.cpp`: the route of a cluster net is found through its atom
   net when the routing is flat (`net_info`, `net_route`, the nets of the
   congestion hot spots).
5. Tests: route and analyze a small design flat at a fixed width through the
   Python session; check that the stages are reached, that the net and timing
   queries answer, and that the totals are close to the one-shot `vpr` run of
   the same design.

Status: done.

- `test_flat_routing.py` covers route, analysis, the timing, net and
  congestion queries, and reading a flat routing back from its files.
- `test_equivalence.py` has three flat designs (`GOLDEN_FLAT`). Each gives
  the same files, results and bitstream as `vpr --flat_routing on`.

Found on the way:

- The analysis of a flat routing makes the nets of the clusters again, named
  `<atom net>_<n>`. `net_info` also finds a net by the name of its atom net.
- A route tree read from a `.route` file has an unset number of sinks (VPR's
  `RouteTree(RRNodeId)` does not set it). `net_route` counts the sinks in the
  tree instead, for flat and cluster routing.

## Phase 2: the model and the app

1. `router.flat_routing` through `set_option` (it already invalidates ROUTED
   as every `router.*` option does); document its cost in memory and runtime.
2. Device packages may say whether flat routing is supported; a design that
   asks for it on a package that does not is refused.
3. Reports: congestion and the run summary count the channels between the
   clusters only; say so when the routing is flat.
4. The net view: draw the routing inside the clusters, or mark it as not
   drawn.

Status: done.

- The model sets `router.flat_routing` with `set_option`. Its cost is in
  docs/spec_model_capabilities.md.
- A manifest says `device.flat_routing: true` (docs/spec_device_package.md,
  R-14a). The three packages of the repository say it: each was routed flat
  and made its bitstream. On a package that does not say it,
  `Session.fixed_width_refusal(command, flat_routing=False)` refuses the
  route. It is used by `run_stage` (`flat_routing_unsupported`), `run_tcl`,
  the console in both modes and `openfpga-app tcl`. `set_option` refuses to
  set the option there.
- `congestion`, `report_run` (routing) and `net_route` have `flat_routing`.
  The stage report of route and the drawing of the congestion say that the
  channels only are counted.
- The app has no drawing of the route of a net. `net_route` says
  `flat_routing`, and its nodes inside a cluster are in the tile of the
  cluster.

## Phase 3: the rest of the flow

1. Bitstream and testbench outputs from a design routed flat, compared with
   the regression task `flat_routing/basic_full_tb`.
2. Save and reload a session routed flat (the routing is loaded at the same
   fixed width).

Status: done.

- `test_equivalence.py` checks the bitstream of the flat designs.
  `test_flat_routing.py` checks the fabric Verilog, the full testbench and
  the SDC files of the commands of `flat_routing/basic_full_tb`. They are the
  same as those of `vpr --flat_routing on`.
- A session routed flat is restored from its checkpoint with the same
  timing, routes and bitstream.
- `net_route` takes the name of an atom net or of a net of the clusters
  (`find` lists the latter).
