# Plan: one graphics stack for the views of a design

The web app (docs/spec_web_app.md) shows a flow as text, reports and a few small SVG drawings
(UI-43). This plan builds one rendering stack and puts six families of views on it:

| View | Shows | Main data |
|---|---|---|
| V1 Netlist (BLIF / atoms) | the atoms of the circuit and their nets, as a schematic | the atom netlist that VPR read |
| V2 Clustered netlist | the clusters, their pins and the nets between them; a cluster opens into its atoms | the packing |
| V3 Floorplan | the tile grid, the placed blocks, the regions of the constraints | the device, the placement, the ODC |
| V4 Pin constraints | the I/O ring, the pins of the package and the ports of the design on them | the device, the I/O map, PCF/ODC pins |
| V5 Routing | the wires of the routed nets on the channels, congestion as a heat map | the routing, the congestion |
| V6 Timing exploration | the critical paths, drawn on V3 and V5 and in V1; slack per net and per block | the timing analysis after routing |

It starts from the renderer plan in `docs/_notes/netlist-renderer-plan.md` (deck.gl with a Canvas2D
fallback, a scene of typed arrays, a CPU spatial index, LOD tiers, ELK layout). Section 1 reviews
that plan against the app as it is; the rest is the plan for the shared stack and the views.

The owner reviews this plan before code is written.

## 1. Review of the renderer plan

### What to keep

- **One scene, dumb renderers, view state outside them.** It is what lets six views share one
  stack, and what makes the views testable without a GPU.
- **The CPU spatial index (flatbush) is the truth for picking.** It also does the culling and the
  choice of labels. GPU picking stays an optimization.
- **Typed arrays, CSR connectivity, a version that changes with the data.** They suit every view
  here, and they cross a Web Worker boundary without a copy.
- **Primitives as data.** For FPGA views it goes further than LUT and FF symbols: the tile types,
  their sub-tiles and pins come from the device (`device` query), not from hand-written shapes.
- **Canvas2D built first as the reference, and a measured choice of backend.** Kept, with the
  change in "the platform" below.
- **A command API for the model (T5.4).** The app already has the loop; the view commands become
  tools of the MCP server or events of the page.

### What to change

1. **The platform is a browser, not Tauri.** The app is a Preact page served by Starlette, and
   the person's browser reaches it through an SSH tunnel (spec_web_app.md, section 2). The
   browser runs on the person's machine, with its GPU, so the WebKitGTK risks of the plan
   (software WebGL, masked renderer strings) apply only when the browser itself runs on a remote
   desktop (VNC, X11 forwarding). Tauri 2 also needs the 4.1 API of WebKitGTK, which RHEL 8 does
   not ship as far as I know (to check, if a desktop build is ever wanted). So: no Tauri; phase 0
   measures Chromium and Firefox locally and a browser on a remote desktop; the Rust hints of T6.1
   go away; settings are kept in `localStorage` as the other settings of the page are.
2. **The scene is not a schematic.** `SceneModel` has cells, nets and wire segments. The views
   here also need tiles, channels with tracks, routing nodes, pins on a ring, regions, heat maps
   and paths. The scene becomes a set of typed **layers** (rectangles, polygons by instancing a
   shape, segments, polylines, points, text, a grid of values), and each view is an adapter that
   fills layers. The renderers draw layers, not cells and nets (section 3.2).
3. **The selection is by object, not by cell index.** Cross-probing (Phase 8 of the plan, and
   X-12 of the web app spec) needs ids that mean the same in every view: an atom, a block, a net,
   a tile, a routing node, a timing path. Each layer item carries an object id, and a shared
   selection holds objects; each view maps them to its items (section 3.3).
4. **LOD by size on the screen, not by scale constants.** A tile of the floorplan, a LUT of the
   schematic and a track of a channel have different sizes in their worlds. A tier is chosen by
   how many pixels an item takes (a label when its box is 40 px wide, outlines from 6 px, dots
   below). One rule, one config, every view.
5. **WebGL is needed for routing, not for the schematic.** A schematic is shown by scope (a level
   of the hierarchy, a cone, a path), so it stays in the thousands of items, which Canvas2D draws
   well. A routed 32x32 device has hundreds of thousands of wire segments, and the congestion of
   every channel; that is where WebGL pays. The backend is chosen per view and per size, not once
   for the app (section 3.4).
6. **Two full renderers with visual parity is the costliest part of the plan.** The owner decided
   (2026-11-25): deck.gl draws every layer kind, in every view, and Canvas2D is the fallback that
   draws them all too. The parity tests compare every kind (3.4).
7. **ELK is for the schematic views only, and never flat.** The floorplan, pins and routing have
   their coordinates from the device; only V1 and V2 need a layout. ELK of a flat 10k-cell
   netlist takes far too long; the plan's scoped views (T2.2) become the only mode. elkjs is
   EPL-2.0 (OpenFPGA is MIT): fine to use unmodified in the bundle, to note in the licenses.
8. **The data path is the gap, not the renderer.** The queries of the shell (`placement`,
   `net_route`, `congestion`, `critical_paths`, `net_info`) answer the model: paged, one net at a
   time, truncated. A view of a whole device needs every block, every route and the slack of every
   net at once, compactly. Section 3.1 adds exports for that, in the core, as binary files.
9. **Tests run where the app's tests run.** vitest with jsdom has no canvas and no WebGL: the
   scene, the index, the LOD, the selection and the adapters are tested there as plain code.
   Drawing is tested with Playwright (headless Chromium has WebGL through SwiftShader, which is
   also the slow path that the probe must detect).
10. **Bundle size.** deck.gl and elkjs are each around a megabyte. They load with the first view
    that needs them (a dynamic import, as Monaco is loaded today), never with the page.

### Open questions on the renderer plan that this plan settles

- Frontend framework: Preact, as the app (the renderer stays framework-agnostic).
- Netlist interchange: a binary file of typed tables made by the core (section 3.1), not Arrow
  (no dependency for what a header and aligned arrays do) and not Yosys JSON (it is the netlist
  before VPR; V1 must use the names that the timing and the routing use).
- Where layout runs: in a Web Worker of the page, on a scope; a server-side layout only if a
  measure shows a scope that the worker cannot do in a second.

## 2. Requirements

- **R-1** One rendering stack for the six views: scene, index, view controller, selection,
  renderers. A new view is an adapter and a panel, with no change to the renderers.
- **R-2** The views work on the sessions of the app, on the data of the stage they need, and say
  which stage is missing (a floorplan before placement shows the device and asks for `place`).
- **R-3** The device views (V3, V4, V5, V6 on the device) stay responsive from a 32x32 device to a
  500x500 one, placed and routed: a frame under 16 ms at the 95th percentile in Chromium on the
  person's machine, under 33 ms in a browser on a remote desktop, at every zoom, with the backend
  that the probe chose.
- **R-4** A view opens in under a second, and a pan or a zoom shows its first picture in under
  200 ms and its full detail in under a second, on any device size: what is sent depends on what
  is on the screen, not on the size of the device.
- **R-11** The page never holds the data of a whole large device: its memory for a view stays
  under a bound (256 MB at first) whatever the size of the device and of the design.
- **R-5** Selection is shared: an object selected in one view is selected in every open view that
  has it, and a view can be asked to zoom to it.
- **R-6** Every view can be driven by commands (select, highlight, zoom to, show cone, show path),
  which the model can call and which are logged.
- **R-7** The data of a view is read once per version of the session's data, and cached.
- **R-8** Nothing changes the flow: the exports are queries.
- **R-9** The demo has the views on recorded data, as it has the rest of the page.
- **R-10** The netlist views expand and collapse interactively, in the page, with no round trip
  to the server: a cluster into its atoms, a cone one level further, a module of the RTL into
  its contents. What is already shown keeps its place when something next to it opens.

## 3. Architecture

```
core (C++), worker process                     app (Starlette)            page (Preact)
───────────────────────────                    ───────────────            ─────────────
export_view --kind K --file F  ──writes──►  session/views/K.<version>.bin
                                             GET /api/view?kind=K  ──►  decode (typed arrays, no copy)
                                                                            │
                                                                     ViewAdapter (per view)
                                                                            │  fills
                                                                         Scene (layers) ◄── LayoutWorker (ELK, V1/V2 only)
                                                                            │
                                                         SpatialIndex (flatbush, per layer)
                                                                            │
                                         Selection (shared) ◄──► ViewController (view state, LOD, input)
                                                                            │
                                                                  Renderer: Canvas2D | WebGL (deck.gl)
```

### 3.1 Data: exports of the core

The devices go up to 500x500. Scaled by area from the 32x32 measures (P0.1), a 500x500 device has
about 44 million routing nodes, 85 million channel track segments, and 2.5 million LUTs when it
is full. So the data comes in two shapes:

- **Whole exports** (this section), for what is small at any size: the tile types, the I/O sites
  of V4, the worst paths of V6, a scope of the netlist (a module, a cone, a path), and the whole
  of a small device.
- **Tiles** (3.1.1), for what grows with the area: tiles and blocks, routes, channels, congestion,
  slack on the device. The page asks for the tiles of what is on the screen, at the level of detail
  of the zoom, as a map viewer does.

A new query command of the plugin `ui`, `export_view --kind <kind> --file <path>`, writes the data
of a view as one binary file, and answers JSON (kind `view_export`): the file, its size, the
version, the counts. It is a query: not in the journal, nothing invalidated.

| kind | Tables | Needs |
|---|---|---|
| `device` | tiles (x, y, w, h, type), tile types (name, sub-tiles, pins, is_io), channels (x, y, dir, tracks), I/O sides | DEVICE |
| `atoms` | atoms (name, model, block, module, how the module is known), pins (atom, port, bit, dir), nets (name, driver pin, sink pins CSR) | a circuit read |
| `clusters` | blocks (name, type), block pins, block nets (CSR), atoms of each block | PACKED |
| `placement` | blocks (x, y, sub_tile, layer), fixed or not; regions of the constraints | PLACED |
| `routing` | per net: route nodes (type, xlow, ylow, xhigh, yhigh, ptc, layer, parent) in CSR; channel occupancy (x, y, dir, used, capacity) | ROUTED |
| `timing` | slack and criticality per net and per sink pin; the N worst paths as point lists (pin, block, net, arrival, delay) | ANALYZED |
| `pins` | the I/O sites (x, y, sub_tile, side, package pin name), the ports of the design and their site, the source of each (PCF, ODC, placer) | DEVICE (+ PLACED for the placer's choice) |

The file format: a magic and version, a JSON header listing the tables and their columns (name,
dtype, length, byte offset), then the columns as aligned little-endian arrays; strings as one
UTF-8 blob and a `Uint32Array` of offsets. The page maps every column as a typed array over the
one `ArrayBuffer`, with no parse. The JSON writer of the core writes the header; the columns are
`std::vector`s written as they are.

The file is written in the session directory (`views/`) by the worker, and the app serves it
(`GET /api/view?kind=K`), as it serves the bitstream (no data through the worker's pipe, which
pickles). The version is a counter of the stage the kind needs: the session gains a generation per
stage (it has one for the link and the fabric already), bumped when the stage completes again or
is invalidated. A file of the current version is not written twice (R-7).

Python: `Session.export_view(kind, file)`; the MCP server does not get a tool for it (the model
gets the view commands of 3.6, not the data).

`atoms` and `clusters` are exported by scope (a module and its depth, a cone, a path, a list of
blocks), never whole for a large design: a full 500x500 design has millions of atoms. The whole
netlist is an export only when it is under a bound (100k atoms at first).

#### 3.1.1 Tiles and levels of detail

`view_tile --kind <kind> --level <l> --x <i> --y <j>` answers one tile of the device: the square of
`T x 2^l` grid tiles at (i, j), with T set so that a tile holds a bounded amount of data (T = 16
at first). It writes the same binary format as 3.1, and the app caches tiles by version.

| kind | Level 0 (full detail) | Coarser levels |
|---|---|---|
| `grid` | the grid tiles with their type, the sub-tile slots | one value per cell: the type, or the share of each type |
| `placement` | the blocks on their slots | the used share of each cell, by type |
| `routing` | the route nodes in the tile, with their net | the wirelength and the occupancy per channel cell |
| `channels` | the tracks of the channels in the tile, used or not | the occupancy per channel cell (the congestion) |
| `timing` | the slack of each block and net in the tile | the worst slack per cell |

A coarse level is a grid of numbers (`grid` layer, 3.2), which both backends draw as one image:
its cost does not depend on the device. Only level 0 has objects, and the page asks for level 0
only when a tile is big enough on the screen to show them (the LOD rule of 3.2).

What a tile needs is in the worker's memory already (the device, the placement, the routing, the
timing of VPR); the coarse levels are computed once per version and kept beside the session.
Picking and search on the whole device (what is at x, y; where is a net or a block) are queries
to the worker (`find`, `block_info`, `net_info` exist), not to the page's index, which only knows
the tiles it has.

### 3.2 Scene: layers

```ts
type ObjectKind = "atom" | "block" | "net" | "pin" | "tile" | "site" | "node" | "path" | "region";
// An object: its kind and its index in the export of that kind; its name is the key across views
interface ObjectRef { kind: ObjectKind; index: number }

type Layer =
  | { kind: "rects"; x: Float32Array; y: Float32Array; w: Float32Array; h: Float32Array; style: Uint16Array; object: Uint32Array }
  | { kind: "shapes"; x: Float32Array; y: Float32Array; shape: Uint16Array; style: Uint16Array; object: Uint32Array } // instances of a ShapeDef
  | { kind: "segments"; xy: Float32Array /* x1 y1 x2 y2 */; style: Uint16Array; object: Uint32Array }
  | { kind: "polylines"; xy: Float32Array; start: Uint32Array; style: Uint16Array; object: Uint32Array }
  | { kind: "points"; x: Float32Array; y: Float32Array; style: Uint16Array; object: Uint32Array }
  | { kind: "labels"; x: Float32Array; y: Float32Array; text: StringTable; size: Float32Array; object: Uint32Array }
  | { kind: "grid"; x0: number; y0: number; cell: number; cols: number; rows: number; value: Float32Array; ramp: string };

interface SceneLayer { id: string; objectKind: ObjectKind; layer: Layer; minPx?: number; maxPx?: number }
interface Scene { layers: SceneLayer[]; bounds: [number, number, number, number]; version: string }
```

- `ShapeDef` is the plan's `PrimitiveDef` (outline, ports, decorations), made from the device for
  tiles and sub-tiles, and from a small library for atoms (LUT, FF, adder, memory, DSP, I/O).
- A style is an index into a table of the theme (fill, stroke, alpha by tier), so a theme change
  or a highlight does not touch the geometry.
- `minPx` and `maxPx` are the LOD rule: a layer is drawn when its typical item is that big on
  the screen (change 4 of section 1).

### 3.3 Objects, selection, cross-probing

- Every item of a layer has an object (its kind and index in the export). Names come from the
  export's string tables; a name is the key between views (an atom of V1 and its block in V3, a
  net of V2 and its route in V5).
- `Selection` is a store of the page (a signal): a set of `ObjectRef`, plus the objects that are
  highlighted. A view shows what it has of it, and maps what it does not have: a selected atom is
  its block in V3, a selected net is its route in V5 and its bounding box in V3, a selected path
  is its points in V1, its blocks in V3, its nets' routes in V5.
- The maps between kinds come from the exports (atom → block in `clusters`, block → site in
  `placement`, net → nodes in `routing`, path → pins, blocks and nets in `timing`), built once
  per version, as CSR.

### 3.4 Renderers

- One interface: `mount`, `setScene`, `render(view, state)`, `measure`, `destroy`, as in the plan.
- **deck.gl (`@deck.gl/core` and `@deck.gl/layers` 9.4, with no React) draws every layer kind,
  in every view** (the owner's decision, 2026-11-25): `segments` and `polylines` (`LineLayer`,
  `PathLayer` with binary attributes), `rects` and `shapes` (`SolidPolygonLayer` and
  `PolygonLayer` with binary polygons), `points` (`ScatterplotLayer`), `labels` (`TextLayer`,
  SDF), `grid` (`BitmapLayer` of the values through a ramp).
- **Canvas2D** draws every layer kind as well: it is the fallback when WebGL2 is missing, is
  measured too slow, or loses its context, and the reference of the parity tests.
- At the scale of a large device, the device views (V3, V5, V6 on the device) need WebGL at the
  zooms that show objects: a screen of level-0 tiles can hold a few hundred thousand segments.
  Canvas2D, the fallback, draws the same tiles with a lower limit of detail (it asks for a coarser
  level sooner), so it stays usable, with less on the screen.
- The choice: deck.gl by default; `auto` runs the plan's probe (30 frames of a zoom) and falls
  back to Canvas2D if it is slow or if the context is lost. The person can force either, per
  browser.

### 3.5 Layout (V1, V2)

- ELK (layered) in a Web Worker, on a scope: a level of the hierarchy, the cone of an object to N
  levels, a timing path with its side inputs, a cluster opened into its atoms. The layout gives
  positions to `shapes` and polylines to the nets.
- The scope has a limit (2000 atoms at first); a bigger one asks to narrow it.
- Layouts are cached by version and scope. The hierarchy for V1's levels comes from
  `report_design_hierarchy` (the instance tree) and the module of each atom in `atoms` (3.7).

### 3.6 Interaction and commands

- `ViewController`: pan, zoom about the cursor, fit, zoom to the selection, keys; the same for
  every view and backend.
- Hover: the object under the cursor (the index), a tooltip from the export, throttled to a frame.
- Click and box select; a selection in one view is the selection of all (R-5).
- Commands (`select`, `highlight`, `zoom_to`, `show_cone`, `show_path`, `open_view`), as plain
  data: the page runs them from the sidebar, from the console, and from the model. The model gets
  them as a tool `show` of the MCP server that the app turns into a page event (the same path as
  `state_changed`), so it can point at what it talks about. They are logged in the console.

### 3.7 Expanding and collapsing (R-10)

The whole netlist of a version is in the page (`atoms`, `clusters`), so an expansion is a change of
scope and a layout of it, never a request. Three kinds:

| Expand / collapse | From | Exact |
|---|---|---|
| A cluster into its atoms (V2) | the atoms of each block in `clusters` | yes |
| A cone one level further, either way | the CSR of the nets in `atoms` | yes |
| A module of the RTL into its contents (V1) | the module of each atom | no, inferred |

**The module of an atom is inferred.** The synthesis flattens the design, and ABC maps it to LUTs
that have no instance name: a `.names` of the BLIF is named by the net it drives
(`u_wide.u_leaf1.y`), and that name keeps the hierarchy. But ABC merges logic across the edges of
the modules: on a small test design, the XOR of `u_leaf0` went into the LUT that drives
`u_leaf1.y`. An atom is therefore given the module of the net it drives (of its output for a
latch, of its instance name for a `.subckt` that keeps one), and the export says how it was known:

- `net`: from the hierarchical name of the net it drives (the FFs, and the LUTs that drive a
  named signal);
- `connectivity`: an atom with a generated name (`$abc$...`), from the modules of the named atoms
  it feeds, or else of those that feed it, when they agree;
- `common`: when they do not agree, their deepest common module, marked as a guess in the view;
- `none`: nothing named is reached: the top.

Phase 0 measured 40% `net`, 60% `connectivity` and 0.1% `common` on a design that fills a 32x32
device. The attributes of the synthesis do not help (P0.3): Yosys writes them only for `.subckt`
cells, and the LUTs of ABC have no `src`.

**A collapsed module is one box.** Its ports are the nets that cross its edge (a net with a pin on
an atom inside and one outside), computed in the page from the CSR and the module of each atom.
A net inside it is not drawn. A box shows the module's name, its instance path and its count of
atoms.

**What is shown keeps its place.** An expansion lays out the new contents and keeps the order and
the positions of the nodes that were there (ELK's interactive options of the layered algorithm:
the order of a layer is kept, the new nodes are placed between). A collapse removes the contents
and puts the box where its first node was.

**Limits.** An expansion that would show more than the scope limit (2000 atoms) asks to narrow it
(a cone, a sub-module) or shows the module as a summary: its count of atoms by model, its ports.

**With the Design hierarchy.** The tree of the sidebar (`report_design_hierarchy`) is the RTL
before flattening, and is exact. V1 shows the mapped atoms grouped by the inferred module. A module
selected in one is selected in the other; a module of the tree with no atom (all of its logic
went into its neighbours) says so.

**Very large designs.** The whole netlist of a 32x32 device is small. If phase 0 shows that
a larger one makes `atoms` too big for one transfer, `export_view` gets a scope (a module, a
cone), which the format allows (a scope is the same tables, smaller).

## 4. The views

### V1 Netlist (BLIF / atoms)
- Data: `atoms` (+ `timing` for slack colors). Layout: ELK by scope.
- Opens from: the Design hierarchy (a module), a net or a block in any view, a path in V6.
- Shows: atoms as shapes, nets as polylines; a net's fanout over a limit is drawn as a stub with
  its count. Click a pin: its net; double click an atom: its cone.

### V2 Clustered netlist
- Data: `clusters`. Layout: ELK by scope (the clusters and the nets between them, or one cluster
  opened into its atoms and its pins).
- Shows the pin use of each cluster (`cluster_report` has it), the nets that leave a cluster.

### V3 Floorplan
- Data: `device`, `placement`, the regions of the ODC (`report_constraints`), `timing` (colors).
- Layers: tiles (`rects` by type), sub-tile slots at higher zoom, blocks on their slots, regions,
  net bounding boxes of the selection, a heat map of utilization or of slack.
- Replaces the placement drawing of UI-43.

### V4 Pin constraints
- Data: `device`, `pins`.
- Shows the I/O ring with each site, the package pin name, the port of the design on it, and where
  the assignment came from (PCF, ODC `PACKAGE_PIN`, placer). Ports with no site are listed beside.
- First read only; editing (writing ODC `set_property PACKAGE_PIN`) is X-30 of the web app spec,
  after this plan.

### V5 Routing
- Data: `device` (channels), `routing`.
- Layers: the channels with their tracks at high zoom (`segments`, a track at its `ptc` offset in
  the channel), the routes of the nets (`segments` by node), pins (OPIN, IPIN) as points, the
  congestion as a `grid` of occupancy. The selected net is drawn over all.
- The big one: at 500x500, tens of millions of track segments; the tracks and the routes are
  level-0 tiles of the screen only, the congestion is a coarse level; WebGL first here.
- Replaces the congestion drawing of UI-43.

### V6 Timing exploration
- Data: `timing` (+ the others it draws on).
- A table of the worst paths (slack, start, end, levels) with a histogram of the slack of every
  end point; a path selected is drawn on V3 (its blocks and the lines between them), on V5 (the
  routes of its nets), and in V1 (its atoms, laid out as a path). The bar of a path's delays, by
  point, with the net and the block of each.
- Slack as a color in V1, V3 and V5.

## 5. Phases

Each task has its acceptance. The order puts the data and the shared stack before the views, and
the views in the order the flow makes their data.

### Phase 0: measures (a decision gate)
- **P0.1** Size of the data on 32x32 versions of the three packages of the repository, routed at
  channel width 160: blocks, atoms, nets, route nodes, segments, bytes.
  Accept: a table in this plan.
- **P0.2** Port the three prototypes of the renderer plan into a page of the app (a dev route, not
  shipped), fed with synthetic scenes of those sizes. Accept: p95 and worst frame per backend in
  Chromium and Firefox locally and in a browser on a remote desktop; the thresholds of 3.4 set
  from them.
- **P0.3** The module of the atoms: on the designs of the measures, the share of atoms whose
  module is known by `src`, by the name of the net they drive, or not known, with the scripts of
  the packages as they are and with `write_blif -attr -cname`. Accept: a table in this plan, and a
  decision on changing the scripts.
- **P0.4** How the data grows with the device: the routing graph (nodes, edges, memory, time) at
  32x32, 64x64, 128x128 and as large as the machine allows, the same packages with a larger
  fixed layout; from them, the size of each kind of tile at each level, and the memory and time of
  VPR at 500x500 (for the worker's memory limit, gap G4 of spec_model_capabilities.md). Accept: a
  table and the value of T.
- Gate: WebGL is required for the device views at scale (3.4); what phase 0 decides is the limits
  of detail of each backend and the size of a tile.

### Results of phase 0 (2026-11-25)

Measured with scripts that are not in the repository, on the 32x32 packages at channel width 160
(debug build: the times are an upper bound).

**P0.1, the data of a full 32x32 device.** Of the benchmark designs tried, two went through the
flow on the k6 package (`spimaster`, `cf_ldpc`); the others stopped in synthesis on gaps of the
packages' scripts (async set or reset FFs on the DSP package, `$print`, `$alu`, `$macc_v2` cells
left unmapped, a vendor primitive), to be looked at apart. `spimaster` fills the device:

| | spimaster on k6_frac_n10_32x32 |
|---|---|
| atoms | 22,107 (13,302 LUTs, 8,805 FFs) |
| nets, pins | 22,129 nets, 83,497 pins |
| clusters | 964 placed blocks |
| routed nets | 8,225 |
| route nodes | 102,736 (40k channel wires covering 155k tile spans, 23k IPIN, 8.5k OPIN) |
| flow | pack 37 s, place 92 s, route 23 s, analysis 7 s |

Sizes in the format of 3.1 (16 bytes a route node, about 40 a named atom): the atoms of the
design about 3 MB with their pins, the routes 1.6 MB, the placement a few kB. At 32x32 every view
fits in one export.

**P0.4, the growth with the device.** The k6 architecture with larger fixed layouts, a tiny
design routed at width 160:

| layout | RR nodes | RR edges | peak memory | RR graph built in | whole run |
|---|---|---|---|---|---|
| 32x32 | 181,488 | 2.02 M | 166 MB | 7 s | 17 s |
| 64x64 | 696,560 | 7.99 M | 372 MB | 29 s | 69 s |
| 128x128 | 2,728,176 | 31.8 M | 1.23 GB | 111 s | 4.6 min |
| 500x500 (by area) | about 42 M | about 485 M | about 19 GB before a design | about 28 min | |

Everything grows with the area (x3.8 to x3.9 a doubling of the side). At 500x500 a full design is
about 5 million atoms and 25 million route nodes: hundreds of MB in any format, so tiles (3.1.1).
The 19 GB of VPR alone, before the netlist, the routing and the timing, sets the worker's memory
limit (G4) and the size of the server; a release build is to be measured for the times.

Tile size: a k6 grid tile has about 157 routing nodes at width 160, so a level-0 tile of 16x16
grid tiles has about 40k nodes (640 kB). Single tracks can be told apart only when a channel is at
least about 160 pixels wide, so the tracks of a screen are those of a few dozen grid tiles at
most: the LOD rule bounds what a screen holds, whatever the size of the device. T = 16 holds.

**P0.3, the module of the atoms.** `write_blif -attr -cname` adds nothing for LUTs and FFs: Yosys
writes attributes and names only for `.subckt` cells, and the LUTs that ABC makes have no `src`.
The names of the nets are what is left: the FFs keep the hierarchical name of their register,
the LUTs drive nets with generated names.

| | atoms | named (the net they drive) | one module by connectivity | several | none |
|---|---|---|---|---|---|
| spimaster | 22,107 | 8,835 (40%) | 13,251 (60%) | 21 (0.1%) | 0 |
| cf_ldpc | 167 | 81 (49%) | 83 (50%) | 3 (2%) | 0 |

Connectivity: an unnamed atom takes the modules of the named atoms it feeds (up to 6 levels of
LUTs), else of those that feed it; when they differ, their deepest common module. So 3.7 keeps
`net` and adds `connectivity` and `common` as the ways a module is known, drops `src`, and the
scripts of the packages are not changed. Known limit: the logic of a purely combinational module
goes to the module of the registers it feeds; P1.4 compares the counts with a synthesis that keeps
the hierarchy.

**P0.2, the renderers.** The prototypes of the renderer plan were not at hand, so the app has a
benchmark of its own: `bench.html` and `src/bench/bench.ts` (development server only, not in the
build), `npm run bench` for a table. Synthetic scenes of the measured sizes, a zoom sweep from the
whole device to a few tiles and back, deck.gl 9.4 (binary attributes) and Canvas2D (culled to the
screen, one path a colour). Headless Chromium draws WebGL with SwiftShader and its canvas in
software too: this is the slow path of a remote desktop, not a machine with a GPU.

| scene | items | deck.gl p95 / worst frame | Canvas2D p95 / worst frame |
|---|---|---|---|
| floorplan 32x32 | 1.8k | 17 ms / 40 ms | 17 ms / 416 ms |
| routing 32x32 | 43k | 385 ms / 11 s | 17 ms / 8.9 s |
| floorplan 128x128 | 29k | 17 ms / 365 ms | 17 ms / 3.7 s |
| routing 128x128 | 324k | over 150 s for the sweep | 27 ms / 40 s |
| channel tracks, a screen | 1M | over 150 s | over 150 s |
| heat map 500x500 (one image) | 1 | 17 ms / 119 ms | 17 ms / 68 ms |

On the software path a screen must hold tens of thousands of objects, not hundreds of thousands:
the coarse levels (one image, 17 ms at any device size) carry the zoomed-out views, and level 0
is asked for only when few tiles fill the screen. The worst frames are the first ones (shaders and
buffers on SwiftShader, the first path of everything on Canvas2D). The same page on a browser with
a GPU is the measure still to take, by the owner (`npm run dev`, then `/bench.html`, "Run all");
it sets the probe's threshold of 3.4.

### Phase 1: exports

Status: done (2026-11-25), not committed. In the plugin `ui`: `view_file.{h,cpp}` (the format),
`view_export.{h,cpp}` (the kinds and the tiles), `view_commands.{h,cpp}` (`export_view`,
`view_tile`), the slacks and the paths from one analysis in `query_timing.cpp`; a generation per
stage in the session (`stage_generation`), which the versions are made of. Python:
`Session.export_view`, `Session.view_tile`, `openfpga.read_view`. App: `GET /api/view`, `GET
/api/tile` (one export at a time, 409 while a call runs, files in `views/` of the session, the
header `X-View-Version`); the page: `src/views/format.ts` (`decodeView`, `fetchView`,
`fetchTile`). Tests: `test_view_file` (C++), `test_views.py` (14), `test_app_views.py` (3),
`src/views/format.test.ts` (5, on files that the core wrote). Not done: the demo's recorded
exports (the demo answers 404, as for the hierarchy), and `pins` has no package pin names yet
(the I/O map of the package is to be joined).

Measures, `spimaster` on k6_frac_n10_32x32 (debug build): device 1 ms, placement 1 ms, pins 3 ms,
routing 133 ms (2.0 MB), clusters 54 ms (2.1 MB), atoms 430 ms (5.0 MB; modules 40% `net`, 60%
`connectivity`, 21 `common`), timing 2.7 s and 105 kB: the timing analysis itself, once; the 500 ms
of P1.3 is to be checked on a release build. The timing has no names (they were 81% of its 554 kB,
and the other kinds have them): its nets and blocks are rows by their index, which `routing` and
`clusters` (`net`) and `placement` and `clusters` (`id` of the blocks) give. The analysis is kept
while the version is the same, so the tiles of `timing` after the export take 1.2 ms, not 3.5 s.
Tiles on a 128x128 device: every level of every kind under 3 ms once the cache of the version is
made; the first tile of a version makes it (channels 214 ms, routing 50 ms, timing one analysis).

Found: a new session in a process resets the data of VPR of the process, which every session of
that process reads (the tests that make one run last). The app has one worker process a session,
so it is not concerned.
- **P1.1** The format: the writer in the core, the reader in the page (`decodeView`), a test that
  writes and reads every dtype, strings and empty tables.
- **P1.2** `export_view --kind device|placement|pins` and the stage generations of the session.
  Accept: counts equal to the `device` and `placement` queries; a second export of the same
  version is not written; a re-run of `place` gives a new version.
- **P1.3** `routing` and `timing`. Accept: the nodes of each net equal `net_route`; the slack of
  each net equals `net_info`; 32x32 export under 500 ms.
- **P1.4** `atoms` and `clusters`, with the module of each atom (3.7). Accept: the counts of the
  atom netlist and of the packing (`report_cluster`); names equal to `block_info`; the shares of
  P0.3 found again on the same designs.
- **P1.5** `GET /api/view`, `Session.export_view`, the cache, the demo's recorded exports.
- **P1.6** `view_tile` for `grid` and `placement`, the coarse levels, `GET /api/tile`, the cache.
  Accept: the blocks of the level-0 tiles of a device are the blocks of `placement`; a coarse
  cell equals the sum of its level-0 tiles; a tile of any level answers in under 100 ms on a
  128x128 device.
- **P1.7** `view_tile` for `routing`, `channels`, `timing`. Accept: the nodes of a net over its
  tiles are its `net_route`; the occupancy of the coarse levels equals `congestion`.

### Phase 2: the shared stack
- **P2.1** Scene, styles, themes, the LOD rule. Pure code, vitest.
- **P2.2** Spatial index per layer: viewport, point, rectangle. Accept: a point pick on 100k
  items under 1 ms.
- **P2.3** Canvas2D renderer for every layer kind, drawing on demand. Accept: R-3 for the sizes
  where phase 0 says Canvas2D suffices.
- **P2.4** ViewController, selection store, hover, commands. Accept: one input sequence gives one
  view state; a selection made in a test view shows in a second.
- **P2.5** A view panel: a tab of the workspace (like the hierarchy tab) with the canvas, a small
  toolbar (fit, layers, backend), and the stage that is missing when there is no data (R-2).

Status of phase 2: done (2026-11-25), not committed. In `src/views/`: `scene.ts` (the seven
layer kinds, objects by kind and name, the rule of the levels of detail), `palette.ts`,
`spatial.ts` (flatbush 4.8, one index a layer: search, pick by distance, the top layer first),
`viewstate.ts`, `controller.ts` (drag, wheel about the cursor, click, double click, keys),
`selection.ts` (the shared selection, the commands, logged in the console), `tiles.ts` (the
level for a scale, the tiles on the screen, a cache that fetches four at a time), `data.ts` (the
whole exports, once a state of the session), `canvas2d.ts` and `deck.ts` (every layer kind each),
`renderer.ts` (the choice, the probe), `view.ts` (what a view gives the panel); the panel
`components/ViewPanel.tsx` (the toolbar: fit, the layers, the renderer; the tooltip; the
selection; the stage that is missing). The views open from the rows of the stages of the sidebar
(pins on DEVICE, floorplan on PLACED, routing on ROUTED), not from a menu. deck.gl is a chunk of
its own (940 kB), loaded by the first view that draws with it; the page stays at 150 kB.
Measures: a pick among 100k items takes far under 1 ms (a test); the frame times of Canvas2D are
those of P0.2.

### Phase 3: V3 Floorplan and V4 Pins (no layout, data from the device)
- **P3.1** V3 with the device and the placement; regions; the selection's nets as boxes.
- **P3.2** V4 read only.
- Accept: Playwright screenshots of a 4x4 and a 32x32 package; a block clicked in V3 is selected
  in the sidebar's tables and in V4 when it is an I/O.

Status of phase 3: done (2026-11-25), not committed. V3 (`floorplan.ts`) draws the objects from
the whole device and placement exports (bounded at 500x500, R-11), culled by the index, and far
out the coarse `grid` and `placement` tiles; the regions; the selection with the boxes of the nets
of a selected block or net (from the clusters export). V4 (`pins.ts`) shows the sites, the ports by
direction, the package pins of the I/O map (the core's `pins` export got the package pin of each
site and the block of each port), and the ports with no site. A block clicked in V3 is the
selection of every view: its box in V4 when it is an I/O, in the status line of each panel (there
are no block tables in the sidebar). `e2e/views.mjs` has the screenshots of a 4x4 package and of a
32x32 copy of it (close up and coarse).

### Phase 4: WebGL and V5 Routing
- **P4.1** The WebGL backend for `segments`, `rects`, `grid`, with the Canvas2D overlay; the probe
  and the fallback; context loss. Accept: R-3 on the routed 32x32; a forced software GL ends on
  Canvas2D.
- **P4.2** V5: channels, tracks, routes, congestion. Accept: the route of a net equals its
  `net_route` (drawn nodes); hotspots equal `congestion`.
- **P4.3** Parity tests of the shared layer kinds (Canvas2D vs WebGL), with a tolerance.

Status of phase 4: done (2026-11-25), not committed. deck.gl draws every layer kind (the owner's
decision, so no Canvas2D overlay); `auto` takes Canvas2D when there is no WebGL2 or when WebGL is
drawn in software (SwiftShader, llvmpipe: the slow path of P0.2), else deck.gl, which a probe of
30 frames sends to Canvas2D when its p95 is over 50 ms; a lost context goes to Canvas2D at once;
the choice is kept in the browser. V5 (`routing.ts`) draws the routes of the tiles of the screen
(wires on their tracks, pins, the links to their parents, one colour a net), the tracks when they
can be told apart (6 W pixels a grid tile), the congestion far out, and the selected net over all.
Checked: the wires of a net drawn are the CHANX and CHANY nodes of its route in the routing export
(the browser test, and a unit test), the coarse congestion is that of `congestion` (P1.7); deck.gl
and Canvas2D give the same picture (a mean difference of 1.3 of 255 a channel on the routing, 0.1
on the floorplan); a forced software GL ends on Canvas2D; a lost context too. Not measured: R-3 on
a routed 32x32 with deck.gl on a GPU (headless Chromium has none): the owner's `/bench.html` and a
look at the views on their machine.

### Phase 5: V6 Timing
- **P5.1** The paths table, the slack histogram, the delay bar.
- **P5.2** A path drawn on V3 and V5; slack colors.
- Accept: the points of a path equal `critical_paths`; selecting a path in the table selects its
  blocks in V3 and its nets in V5.

Status of phase 5: done (2026-11-26), not committed. The timing export gains the histogram of the
setup slack of the end points (32 bins, from the worst to the best finite slack) and, for each
point of a path, whether the delay to it is on a net or in a cell (the type of its edge in the
timing graph). The Timing tab (from the ANALYZED stage) shows the histogram, the 50 worst paths,
and for a path its delays as one bar (nets and cells) and point by point; a point selects its
block and its net. Selecting a path selects the path, its blocks in their order and its nets:
the placement draws a line through its blocks (and not the boxes of their nets), the routing its
nets over all; "Show on" opens either and zooms to the path. A "Slack" layer, off at first (it
needs the analysis), colours the blocks of the placement (the coarse levels by the timing tiles)
and the nets of the routing from red, the worst, to green, with its range in the legend.
Checked: the points of the exported paths are those of `critical_paths` and the histogram starts
at the slack of the worst path (Python); a path selected in the browser test puts a dot on each of
its blocks in the placement and draws its nets in the routing. Also fixed: the selection and a
request to zoom are forgotten when another session opens, and a view applies a request to zoom
once, not again each time its tab is shown. Slack in V1 waits for V1 (phase 6).

### Phase 6: layout and V1, V2
- **P6.1** ELK in a worker, scoped, cancellable, cached; an incremental layout that keeps the
  nodes already shown (3.7). Accept: expanding a node moves no node that was shown by more than
  its layer's spacing.
- **P6.2** V2 clusters; a cluster expanded into its atoms and collapsed again.
- **P6.3** V1 atoms by module, cone and path; modules collapsed into boxes with the nets that
  cross their edge as ports; the guessed atoms marked; opened from the Design hierarchy, and a
  module selected in one is selected in the other.
- **P6.4** The scope limit: an expansion over it narrows or summarizes.
- Accept: a 500-atom cone lays out in under 1 s; an expansion of one level in under 300 ms; a path
  from V6 is shown in V1; the ports of a collapsed module equal the nets that cross its edge.

### Phase 7: the model and the spec
- **P7.1** The tool `show` of the MCP server and its page events; within the budget of the tool
  descriptions.
- **P7.2** spec_web_app.md: UI-43 replaced, X-10 to X-14 marked by what is built; the app README.

## 6. Risks

| Risk | Mitigation |
|---|---|
| Routing data of a large device is too big for one file | Tiles and levels (3.1.1); nothing of the device is sent whole |
| VPR itself at 500x500: the routing graph in tens of GB | P0.4 measures; the worker's memory limit (G4) and the size of the server follow from it |
| A coarse level costs a pass over the device each version | Computed once per version in the worker, kept beside the session; level by level from level 0 |
| WebGL slow on remote desktops | The probe, the forced choice, Canvas2D always works |
| ELK too slow even on a scope | The scope limit; server-side layout only if measured necessary |
| Exports drift from the queries the model sees | The acceptance of each export compares it with the query |
| Two backends drift apart | WebGL draws only three layer kinds; parity tests on them |
| Bundle size | Dynamic imports per view; deck.gl and elkjs loaded only by the views that use them |
| The views multiply sidebar and tab clutter | One "Views" entry in the menu; a view opens in a tab, from an object's context as well |

## 7. Open decisions for the owner

- The order of the views after the shared stack (this plan: floorplan and pins, then routing,
  then timing, then the netlists).
- Whether the model gets the `show` tool in this plan or after it.
- Whether V4 should edit pins (writing ODC) in this plan or stay read only until X-30.
- Whether a desktop build (Tauri, or a browser in app mode) is wanted at all; this plan assumes
  the browser.
