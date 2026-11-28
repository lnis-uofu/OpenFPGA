# Plan: netlist symbols for the netlist and cluster views

Status: phases 1 to 4 implemented (2026-11-27); see "As built" at the end. Decided with the owner: the format is
an SVG subset (O-1), a symbol file sets light and dark colours (O-2), it sets no ELK option but
spacing and direction (O-3), the package's file is `netlist_symbols.svg` (O-4), and a package can
replace or hide a default symbol (O-5).

The netlist views (V1 Netlist, V2 Clusters; docs/plan_design_views.md, sections 3.5, 3.7 and 4) draw
every atom and every collapsed group as a plain box. Edges meet a box anywhere on its sides (B10 in
the review of the netlist viewer). **Netlist symbols** give each kind of object its own drawing: a
LUT, a flip-flop, a pad, a carry chain, a DSP block, a cluster of a given block type. A symbol also
says where each of its pins is, so that the edges end on the pin they belong to.

The tool ships a file of default symbols. A device package may carry a file of its own, whose
symbols replace or hide default symbols, or add new ones.

This plan starts from the skins of netlistsvg (https://github.com/nturley/netlistsvg, MIT license)
and says what we take from them.

## 1. The skins of netlistsvg

netlistsvg draws a Yosys JSON netlist as an SVG schematic, laid out by ELK. Its symbols are in one
SVG file, which it calls a skin:

- **Symbols.** A symbol is a `<g s:type="name" s:width=".." s:height="..">` that holds plain SVG
  shapes (`path`, `rect`, `circle`, `line`, `text`). Its `<s:alias val="$and"/>` children match it
  to the types of cells. The namespace is `xmlns:s="https://github.com/nturley/netlistsvg"`.
- **Ports.** A port is an empty `<g s:pid="A" s:x="0" s:y="5"/>` at a fixed place in the symbol.
  netlistsvg gives these places to ELK as ports with `portConstraints = FIXED_POS`, so the edges end
  on them. A port may also have `s:position` (left, right, top, bottom) and `s:dir="lateral"`.
- **Generic symbol.** A cell whose type has no alias is drawn with `s:type="generic"`. Its first
  `in*` and `out*` ports are copied once per port of the cell, and its body (`s:generic="body"`)
  grows by a fixed gap for each port.
- **Special types.** `inputExt`, `outputExt`, `constant`, `split` and `join`. A cell whose `WIDTH`
  is more than 1 uses the `-bus` alias.
- **Labels.** `<text s:attribute="ref">` shows the name of the cell. `s:attribute="x"` shows the
  cell's attribute `x`, and is left empty when the cell has none.
- **Layout.** `<s:properties>` holds `<s:layoutEngine ...>`, whose attributes go to ELK unchanged
  as options (spacing, layering, direction).
- **Style.** A `<style>` block of CSS is copied into the output, and the symbols use its classes.
- **Loading.** A whole skin is given with `--skin`, and there are two, digital and analog. One skin
  cannot include, extend or override another.
- **Limits.**
  - Ports are only at fixed places, except in the generic, split and join symbols.
  - The format is not frozen before 1.0.
  - The README says node and edge properties "aren't configurable (yet)".

### What we take

| netlistsvg | Our views | Decision |
|---|---|---|
| A symbol per cell type, found by alias | An atom has a model (`.names`, `.latch`, `.input`, `.output`, `.subckt <model>`); a cluster has a block type | **Keep**: our aliases name models and block types (section 3) |
| Ports at fixed places, with ELK `FIXED_POS` | Our edges go from box to box, but the atoms export already has every pin (port, bit, direction) | **Keep**: this also fixes B10 |
| A generic symbol, with its ports copied per port | A LUT has `in[0..k-1]`, a `.subckt` has buses, a cluster has many pins | **Keep, extended**: a port can stand for each bit of a bus (`s:step`) |
| `s:attribute` labels | Names, models, block types, counts | **Keep**, with our own attribute names |
| `<s:layoutEngine>` passes any option to ELK | `layout.ts` sets the options, tuned for speed (P0) | **Restrict**: a file sets spacing and direction only; every other option stays the view's |
| `<style>` CSS, copied to the output | We draw with deck.gl or Canvas2D, not the DOM, in a light and a dark theme | **Replace**: shapes name a colour role, and the file gives each role a light and a dark colour (section 4) |
| A whole skin per file | Default symbols, and the package's on top | **Add**: replace, hide or add, symbol by symbol (section 5) |
| Yosys cell types (`$and`, `$dff`) | VPR atoms, after synthesis and techmap | The gate symbols only matter for a later RTL view (section 9) |

We don't use netlistsvg's symbols as they are. Our default symbols are FPGA primitives (LUT,
flip-flop, pads, carry, DSP, memory) and clusters, which netlistsvg doesn't have. Any symbol adapted
from netlistsvg keeps its MIT notice in the file.

## 2. The format: an SVG subset

People draw symbols in any SVG editor (Inkscape), a package carries SVG, and a symbol file opens in
a browser as a picture. The page never puts the file in the DOM. It parses the file, keeps the
allowed elements, and compiles them to symbols in JSON (section 4). The views draw from that JSON,
and the tests compare it. It exists only inside the tool, so nobody writes a second format.

What is allowed. Anything else is refused, with the element and its line in the message.

- **Elements:**
  - `svg`, `g`, `path`, `rect`, `circle`, `ellipse`, `line`, `polyline`, `polygon`, `text`, `title`;
  - the elements of the `s:` namespace.
- **`transform`:** `translate` and `scale` only.
- **`path` data:** every command. Curves and arcs are flattened to lines when the file is read.
- **`class`:** a colour role (section 4).
- **`text`:** only with `s:attribute`, or as fixed text.
- **Colours:** only in `s:color` (section 4). `fill` and `stroke` on a shape are refused, so every
  colour has both a light and a dark value.
- **Refused:** `script`, `style`, `image`, `use`, `foreignObject`, `a`, any `href`, any event
  attribute, and any external reference. A package's file is not trusted, so it can't run code or
  load anything.

## 3. What a symbol is for

An alias is `<kind>:<name>`. The views look for the aliases of an object in order, and use the
first symbol that has one. A hidden symbol is skipped (section 5). When no symbol matches, the
object is drawn as the plain box it is today.

| Object | Aliases, in the order looked for | Default symbol |
|---|---|---|
| An atom | `atom:<model>` (`atom:.names`, `atom:.latch`, `atom:adder`, `atom:dsp_mult_18`), then `atom:.subckt` for any `.subckt`, then `atom:generic` | the generic atom |
| A LUT, by its number of used inputs | `atom:.names/<k>` (`atom:.names/6`), then `atom:.names` | a LUT box with k inputs |
| A pad | `atom:.input`, `atom:.output` | pad arrows |
| A collapsed cluster (V2) | `cluster:<block type>` (`cluster:clb`, `cluster:io`, `cluster:memory`), then `cluster:generic` | a box with its pins on two sides |
| A collapsed module (V1) | `module:<name of the module>`, then `module:generic` | a box with its ports on two sides |
| An expanded group (a frame) | `frame:module`, `frame:cluster:<block type>`, then `frame:cluster` | the frame drawn now |
| A net of a large fanout | `stub` | the stub drawn now |

A package's file names the models and block types of its own architecture (`cluster:dsp_top`,
`atom:frac_mult`). The default file names those of the VTR architectures in the repository, and has
a generic symbol for each kind.

### The `s:` elements and attributes

They follow netlistsvg, with a few of our own.

| Attribute or element | On | Meaning |
|---|---|---|
| `s:type`, `s:width`, `s:height` | a symbol `g` | the symbol's name, and its box in its own units. `s:type` is what a package refers to (section 5) |
| `s:alias val` | a child of a symbol | an alias of section 3 |
| `s:pid`, `s:x`, `s:y` | an empty `g` | a port at a fixed place. `s:pid` is the name of the port in the atoms or clusters export (`in`, `out`, `clk`, `D`, `Q`) |
| `s:position` | a port | `left`, `right`, `top` or `bottom`: the side, for ELK |
| `s:step` (ours) | a port | the port stands for each bit of a bus: bit `i` is at `s:y + i * s:step` (or at `s:x + i * s:step` on the top and bottom sides) |
| `s:generic="body"` | a `rect` of a generic symbol | grows with the number of ports |
| `s:attribute` | a `text` | `name`, `short_name`, `model`, `type`, `count`, `ports`, or the name of a parameter of the atom when the export gives one |
| `<s:color role light dark/>` (ours) | the root, or a symbol | the colours of a role (section 4) |
| `s:hide="true"` (ours) | a symbol of a package's file | hides the default symbol of the same `s:type` (section 5) |
| `s:layout` (ours; replaces `s:layoutEngine`) | the root | `layerSpacing`, `nodeSpacing`, `direction` (`RIGHT`, `DOWN`). These are the only ELK options a file sets; any other attribute is refused |
| `s:lod` (ours) | a symbol | the screen size in px under which the plain box is drawn in its place |

When the export has a port that a symbol doesn't name with `s:pid`, it is placed as in the generic
symbol, after the named ones, and the view counts these ports once in its status. A symbol is never
refused because a design has a port it doesn't name.

## 4. How the views draw the symbols

1. **Reading.**
   - The default file is read and compiled when the page loads.
   - After `read_device_package`, the app fetches the package's file, if it has one, through a new
     route (`/api/package/netlist_symbols`, from the manifest). It is compiled and merged (section 5).
   - The compiled symbols are cached by the SHA-256 of the files.
2. **Compiling.** For each symbol:
   - paths are flattened to polylines in the symbol's unit box (as `ShapeDef.outline` is today);
   - closed shapes become **rings**, filled with the colour of their role, and open shapes become
     **strokes**;
   - ports become a list of `{pid, x, y, side, step}`;
   - texts become a list of `{attribute, x, y, anchor, size}`;
   - each shape keeps its colour role, from its `class`.
3. **The graph.** `buildGraph` gives each node its symbol, its size (generic symbols grow) and its
   ports. Each edge gets its source pin and its sink pin (`sourcePort`, `targetPort`):
   - for an atom, the pins come from the export;
   - for a collapsed group, there is one port per net that crosses its edge, the port count of
     today.
4. **Layout.** `layout.ts` gives ELK each node's ports with `FIXED_POS`, and edges from port to
   port, so ELK routes to the pins (B10). The ports of groups and frames are not placed by a symbol,
   so they use `portConstraints = FIXED_SIDE` and ELK orders them.
5. **The scene.**
   - Both renderers already draw a `shapes` layer. It gets **strokes** next to its **outlines**
     (`ShapeDef.strokes?: number[][]`), and each item is a symbol in its box.
   - Below `s:lod` px (24 by default), a node is drawn as its box, in the colour of its `body` role.
     This is the level-of-detail rule of section 3.2, and it keeps 2000 nodes fast when zoomed out.
   - Pins go in a `points` layer, drawn from 40 px. They can be picked (`objectKind: "pin"`, which
     is new) and their tooltip gives the pin, its net and its slack.
   - Each text with `s:attribute` goes to the labels layer, at its place in the symbol.
6. **Colours.**
   - **Roles.** Each shape has a role: `body`, `body-alt`, `outline`, `pin`, `label`, `accent`, or a
     role that the file defines. A file can't set a colour on a shape, only on a role.
   - **Defining a role.** `<s:color role="body" light="#e8eef8" dark="#2a3140"/>` gives a role both
     its colours.
     - At the root of a file, it applies to every symbol of that file.
     - Inside a symbol, it applies to that symbol only.
     - Both `light` and `dark` are required, so no symbol is drawn in only one theme.
     - Colours are `#rgb`, `#rrggbb` or `#rrggbbaa`.
     - A role that isn't one of the six above must be defined in the file that uses it, and a
       shape whose class is neither is refused.
   - **Which colour is used.** A shape takes the first one that is set, in this order:
     1. the colour of its role in its symbol;
     2. the colour of its role at the root of its file;
     3. the colour of its role at the root of the default file;
     4. the palette of the view.
   - **Model and block type.** When `body` is set nowhere, its fill is the colour of the model or
     block type, as in the placement, so the views keep matching.
   - **Selection.** The highlight and the selection stay the view's own colours, over every symbol.
   - **Contrast.** `check` warns when a `label` colour has less than 3:1 contrast with the `body`
     colour of the same symbol, in either theme.

## 5. The default symbols, and the symbols of a package

- **The default file.** `openfpga/python/app/frontend/src/views/symbols/default.svg`, built into
  the page. It has:
  - `atom:generic`, `atom:.names` (with the LUT inputs per bit), `atom:.latch`, `atom:.input` and
    `atom:.output`;
  - the `.subckt` models of the repository's architectures: adder, `frac_lut6`, memory and
    multiplier;
  - `cluster:generic`, `cluster:io` and `cluster:clb`;
  - `module:generic`, `frame:module` and `stub`;
  - the light and dark colours of the six roles.
- **The package's file.** A new type in R-20 of docs/spec_device_package.md, `netlist_symbols`:
  - at most one file, `views/netlist_symbols.svg`;
  - checked and checksummed like any file (R-5, R-6, R-30);
  - `check` also compiles it, and fails on an element outside the subset, a role with no colours,
    a symbol with neither an alias nor `s:hide`, or an `s:hide` whose `s:type` is not a default
    symbol.
- **How it merges, symbol by symbol, by `s:type`.**
  - **Replace.** A symbol whose `s:type` is also in the default file replaces the default symbol:
    its drawing, ports, aliases and colours.
  - **Hide.** `<g s:type="cluster_clb" s:hide="true"/>`, with nothing inside it, removes that
    default symbol. Its aliases then fall to the next alias of section 3 (a hidden `cluster:clb`
    falls to `cluster:generic`). If every match is hidden, the object is drawn as the plain box of
    today. A package can hide a generic symbol too, so that it gets plain boxes for anything it
    doesn't draw itself.
  - **Add.** A symbol with a new `s:type` is added.
  - **Aliases.** An alias belongs to one symbol. When a package's symbol has an alias that a
    default symbol also has, the package's symbol takes it.
  - **Colours.** The package's root colours replace the default root colours one role at a time.
  - **Layout.** The package's `s:layout` values replace the default values one at a time.
- **What is shown.**
  - The legend of the view says which symbols drew it, for example "Symbols: default; the package
    replaces 3, hides 1, adds 2".
  - A setting turns the package's symbols off. It is on by default.
- **Corners.** The symbols are per package, not per corner.

## 6. The data the views need

| Need | Have | To do |
|---|---|---|
| The pins of an atom (port, bit, direction, net) | the atoms export, `pins` | nothing |
| The number of used inputs of a LUT | the pins of the atom | nothing |
| The pins of a cluster (port, pin, direction, net) | no | **item 6 of the netlist viewer review**: a `pins` table in the clusters export, needed by the cluster symbols in phase 3 |
| The parameters of a `.subckt` (for `s:attribute`) | no | optional: a `params` table in the atoms export (phase 4) |
| The package's file | the manifest | the type `netlist_symbols`, and an app route to fetch the file |

## 7. Phases

| Phase | Work | Done when |
|---|---|---|
| 1. Format and compiler | <ul><li>the subset parser, curve flattening, roles and colours, ports</li><li>the JSON form, and refusals with messages</li><li>`default.svg` with the generic symbols, the LUT, the flip-flop and the pads</li></ul> | unit tests: each element, each refusal, both themes, and a snapshot of the compiled default file that stays stable |
| 2. Symbols and pins in V1 | <ul><li>`shapes` with strokes in both renderers, and the level of detail</li><li>ports in the graph and in ELK (`FIXED_POS`), edges port to port</li><li>pin picking</li></ul> | e2e: a net's edge ends on the pin of its sink; deck.gl and Canvas2D match in both themes; 2000 nodes still within the P6 times |
| 3. Clusters | <ul><li>the `pins` table of the clusters export (core, schema version)</li><li>cluster symbols with their pins</li><li>the ports of collapsed groups and frames (`FIXED_SIDE`)</li></ul> | a Python test of the table against `block_info`; e2e on V2 |
| 4. Packages | <ul><li>the type `netlist_symbols` in R-20, and `check`</li><li>replace, hide and add</li><li>the app route, the legend and the setting</li><li>a file in one package of the repository, as the example</li></ul> | tests of each merge rule (replace, hide down to the plain box, add, aliases, colours), and a package with a bad file failing `check` |
| 5. Later | LUT functions drawn as gates (section 9), and symbols for an RTL view | when asked |

Phase 2 is the one that most changes how readable V1 is. Phases 1 and 2 need no change in the core.

## 8. Risks

- **Layout time.** Ports with `FIXED_POS` add work to ELK's crossing minimization. Phase 2 measures
  it against P6 (500 nodes in under a second). If it is over, nodes over a size (or graphs over 400
  nodes, as the speed options already do) keep `FIXED_SIDE`.
- **B4.** Fixed pins change where nodes go after an expansion. The existing test measures the
  largest move.
- **A bad file in a package.** `check` refuses it. If one is loaded anyway, the view falls back to
  the default symbols with a message, and never fails.

## 9. Later: LUTs drawn as gates

A LUT of up to 3 inputs whose truth table is an AND, OR, XOR, a MUX or an inverter could be drawn
with a gate symbol (`atom:.names=and2`). This needs the truth table of each `.names` in the atoms
export. netlistsvg's gate symbols could be adapted for this, with their MIT notice. It is optional,
and not part of phases 1 to 4.

## 10. Decisions and open questions

Decided with the owner:

- **O-1** The format is an SVG subset (section 2).
- **O-2** A file sets the light and dark colours of its roles, at its root or in a symbol
  (section 4).
- **O-3** A file sets no ELK option but the spacing and the direction (`s:layout`); the speed
  options and the strategies stay the view's.
- **O-4** The package's file is `netlist_symbols.svg` (type `netlist_symbols`), in `views/`.
- **O-5** A package can replace a default symbol or hide it (section 5).

Nothing is open.

## 11. As built

- **Phase 1.** `frontend/src/views/symbols.ts` compiles a file (`compileSymbols`), merges the
  package's over the default (`mergeSymbols`), finds the symbol of an object (`symbolFor`) and
  places its pins (`placePins`). The default file is `frontend/src/views/symbols/default.svg`.
  Fill with `fill="none"` is the only `fill` allowed: it makes a closed shape an outline.
- **Phase 2.** `netgraph.ts` builds a graph with ports and edges from pin to pin when it is given a
  shaper; `layout.ts` gives ELK the ports (`FIXED_POS` for the pins of a symbol, `FIXED_SIDE` with
  `nodeSize.constraints = PORTS` for the ports of a group, one per net) and reads where they went;
  `symboldraw.ts` draws the symbols in the `shapes`, `polylines` and `labels` layers, and their
  boxes under `s:lod`. The pins are a `points` layer of kind `pin`.
  - **Measured:** ports cost ELK 25% more at 500 nodes and 2 times at 1500, whether placed or
    only on a side. So a graph of more than 400 nodes (`PINS_LIMIT`, where the layout already
    takes its fast options) is laid out without ports, as before, and the status says
    "no pins over 400 nodes".
  - The texts of a symbol are centred at their place: `text-anchor` is read but not used yet.
- **Phase 3.** The clusters export has a table `pins` (block, port, bit, dir, net). A cluster
  places its pins on its symbol; with an older export, it has a port on a side per net. The
  frames of expanded groups have no ports of their own: ELK routes across them.
- **Phase 4.**
  - Reading a package checks its `views/netlist_symbols.svg` (`netlist_symbols_check.cpp`;
    R-52).
  - The app gives the file of the session's package at `/api/package/netlist_symbols`.
  - The views read it when they load, again when the setting "Use the symbols of the device
    package" changes, and say in their legend what the package changed.
  - A hidden symbol that is not a default one is refused by the page, not by the loader, which
    doesn't have the default file.
  - `k6_frac_n10_mem1k_3x2` has a file: a memory cluster with its pins, its memory atom, and the
    multiplier hidden.


## 12. Performance, and what could make it faster

Measured on 2026-11-27 in headless Chromium (deck.gl drawn by SwiftShader, a software renderer, so
its times are those of the slow path), with the real netlist view and renderers, on generated
netlists of LUTs and flip-flops. "Before" hides every default symbol: plain boxes and edges from
node to node, as the view drew before the symbols. Mean of two runs.

| Atoms (pins) | Layout | Scene build | Canvas2D draw, close up | deck.gl frame, close up | Whole graph (boxes) |
|---|---|---|---|---|---|
| 200 (876) | 667 → 704 ms | 1.7 → 3.7 ms | 0.35 → 0.9 ms | 42 → 50 ms | unchanged |
| 400 (1756) | 900 → 912 ms | 2.9 → 5.2 ms | 0.6 → 1.9 ms | 83 → 100 ms | unchanged |
| 1500 (no pins) | 1570 → 1606 ms | 7.1 → 11.4 ms | 1.2 → 10.3 ms | 333 → 400 ms | +6% deck.gl |

Canvas2D kept 60 frames a second in every case. The cost is in drawing the symbols close up, and
it grows with the number of their parts (2 to 2.5 times the items of the boxes). Possible work,
by value:

1. **Canvas2D: draw each symbol from a cached path.** Today each outline of each node is turned
   into world points and added to a `Path2D` on every frame. A `Path2D` per shape of a symbol, in
   its own units, drawn with `setTransform` for each node, would do no arithmetic per point. Even
   cheaper, close up: each symbol rendered once to an offscreen bitmap per zoom step and drawn with
   `drawImage`. This is the case to fix first: 10.3 ms of the 16.7 ms of a frame at 1500 atoms.
2. **deck.gl: instance the symbols.** The `shapes` layer makes a polygon and the segments of its
   outline for every item, in one buffer. A layer that draws the geometry of each symbol once and
   instances it by position, size and colour (as `SimpleMeshLayer` does, or a custom instanced
   layer) would send a few floats per node instead of every point.
3. **Fewer styles and labels.** The styles of a symbol's shapes are keyed by fill, stroke and
   width; the texts of the symbols add labels (a LUT has its name, a .subckt its model and its
   name). A level of detail for the second text of a symbol would halve the labels close up.
4. **Scene build in typed arrays.** `SymbolDrawer` collects `number[]` and copies them into typed
   arrays; sized arrays filled in place, and the outlines of a symbol kept once, would cut the 4 ms
   it adds at 1500 atoms.
5. **Pins over 400 nodes.** Ports cost ELK up to 2 times at 1500 nodes, placed or only on a side,
   so the view lays out larger graphs without them. Laying out without ports and then routing the
   last segment of each edge to its pin ourselves could keep the pins at any size.
