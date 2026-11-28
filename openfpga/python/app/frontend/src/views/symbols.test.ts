import { describe, expect, it } from "vitest";
import {
  SymbolError,
  compileSymbols,
  defaultSymbols,
  flattenPath,
  mergeSymbols,
  parseColor,
  roleColor,
  symbolFor,
  symbolsNote,
  symbolsOf,
} from "./symbols";

const file = (body: string, root = "") =>
  `<svg xmlns="http://www.w3.org/2000/svg" xmlns:s="https://github.com/nturley/netlistsvg">${root}
${body}
</svg>`;

const one = (inside: string, attrs = 's:width="40" s:height="20"') =>
  file(`<g s:type="t" ${attrs}><s:alias val="atom:t"/>${inside}</g>`);

describe("the compiler of the netlist symbols", () => {
  it("compiles the default symbols, the same each time", () => {
    const compiled = defaultSymbols();
    expect(
      compiled.symbols.map((s) => ({
        type: s.type,
        aliases: s.aliases,
        size: [s.width, s.height],
        shapes: s.shapes.length,
        ports: s.ports.map(
          (p) =>
            `${p.pid}@${p.x},${p.y}:${p.side}${p.step ? `/${p.step}` : ""}`,
        ),
        texts: s.texts.map((t) => t.attribute ?? t.text),
      })),
    ).toMatchSnapshot();
    expect([...compiled.colors.keys()].sort()).toEqual([
      "accent",
      "body-alt",
      "label",
      "outline",
      "pin",
    ]);
    // body is left to the views (the colour of a model, of a block type)
    expect(compiled.colors.has("body")).toBe(false);
    expect(compiled.layout).toEqual({
      layerSpacing: 60,
      nodeSpacing: 18,
      direction: "RIGHT",
    });
  });

  it("reads each kind of shape, in the box of its symbol", () => {
    const { symbols } = compileSymbols(
      one(`
        <rect x="0" y="0" width="40" height="20"/>
        <circle cx="10" cy="10" r="5" class="accent"/>
        <ellipse cx="30" cy="10" rx="5" ry="3" fill="none"/>
        <line x1="0" y1="10" x2="40" y2="10"/>
        <polyline points="0,0 10,10 20,0"/>
        <polygon points="0,0 10,10 20,0"/>
        <g transform="translate(10,5) scale(2)"><rect x="0" y="0" width="5" height="5"/></g>
      `),
      "default",
    );
    const shapes = symbols[0].shapes;
    expect(shapes.map((s) => [s.role, s.closed, s.filled])).toEqual([
      ["body", true, true],
      ["accent", true, true],
      ["outline", true, false],
      ["outline", false, false],
      ["outline", false, false],
      ["body", true, true],
      ["body", true, true],
    ]);
    // The rect in its box, as an outline of the unit box with y up
    expect(shapes[0].box).toEqual([0, 0, 40, 20]);
    expect(shapes[0].points).toEqual([0, 1, 1, 1, 1, 0, 0, 0]);
    // A circle is flattened; a line keeps the units of the symbol
    expect(shapes[1].points.length).toBe(48);
    expect(shapes[3].points).toEqual([0, 10, 40, 10]);
    // translate then scale
    expect(shapes[6].box).toEqual([10, 5, 10, 10]);
  });

  it("flattens the curves and the arcs of a path", () => {
    const [arc] = flattenPath("M0,0 L0,25 L15,25 A15 12.5 0 0 0 15,0 Z");
    expect(arc.closed).toBe(true);
    // The arc bulges to the right of x = 15, out to x = 30
    const xs = arc.points.filter((_, k) => k % 2 === 0);
    expect(Math.max(...xs)).toBeCloseTo(30, 0);
    const [curve] = flattenPath("M0,0 C0,10 10,10 10,0");
    expect(curve.closed).toBe(false);
    expect(curve.points.slice(-2)).toEqual([10, 0]);
    expect(curve.points.length).toBe(18);
    const parts = flattenPath("M0,0 h10 v10 z m20,0 l5,5");
    expect(parts.map((p) => p.closed)).toEqual([true, false]);
    expect(parts[1].points).toEqual([20, 0, 25, 5]);
  });

  it("reads the ports, their sides and their buses", () => {
    const { symbols } = compileSymbols(
      one(`
        <g s:pid="in" s:x="0" s:y="4" s:step="8"/>
        <g s:pid="out" s:x="40" s:y="10"/>
        <g s:pid="cin" s:x="20" s:y="0"/>
        <g s:pid="cout" s:x="20" s:y="12" s:position="bottom"/>
      `),
      "default",
    );
    expect(symbols[0].ports).toEqual([
      { pid: "in", x: 0, y: 4, side: "left", step: 8 },
      { pid: "out", x: 40, y: 10, side: "right", step: 0 },
      { pid: "cin", x: 20, y: 0, side: "top", step: 0 },
      { pid: "cout", x: 20, y: 12, side: "bottom", step: 0 },
    ]);
  });

  it("reads the texts: an attribute of the object, or fixed", () => {
    const { symbols } = compileSymbols(
      one(
        `<text x="20" y="12" s:attribute="short_name"/><text x="2" y="8" text-anchor="start">LUT</text>`,
      ),
      "default",
    );
    expect(symbols[0].texts).toEqual([
      {
        attribute: "short_name",
        text: "",
        x: 20,
        y: 12,
        anchor: "middle",
        role: "label",
      },
      {
        attribute: null,
        text: "LUT",
        x: 2,
        y: 8,
        anchor: "start",
        role: "label",
      },
    ]);
  });

  it("skips what an editor writes", () => {
    const { symbols } = compileSymbols(
      `<svg xmlns="http://www.w3.org/2000/svg" xmlns:s="https://github.com/nturley/netlistsvg"
            xmlns:inkscape="http://www.inkscape.org/namespaces/inkscape"
            xmlns:sodipodi="http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd">
         <sodipodi:namedview id="v" inkscape:zoom="1"/>
         <metadata><x/></metadata>
         <g inkscape:groupmode="layer" inkscape:label="Symbols">
           <g s:type="t" s:width="10" s:height="10"><s:alias val="atom:t"/><rect width="10" height="10" inkscape:label="r"/></g>
         </g>
       </svg>`,
      "default",
    );
    expect(symbols.map((s) => s.type)).toEqual(["t"]);
  });

  const refusals: [string, string, RegExp][] = [
    [
      "a script",
      one("<script>alert(1)</script>"),
      /the element script is refused/,
    ],
    ["an image", one('<image href="x.png"/>'), /the element image is refused/],
    ["a use", one('<use href="#a"/>'), /the element use is refused/],
    ["a foreign object", one("<foreignObject/>"), /foreignObject is refused/],
    [
      "an event",
      one('<rect width="1" height="1" onclick="x()"/>'),
      /onclick is refused/,
    ],
    [
      "a style",
      one('<rect width="1" height="1" style="fill:red"/>'),
      /style is refused/,
    ],
    [
      "a colour on a shape",
      one('<rect width="1" height="1" fill="#f00"/>'),
      /fill="#f00" is refused/,
    ],
    [
      "a stroke",
      one('<rect width="1" height="1" stroke="#000"/>'),
      /stroke="#000" is refused/,
    ],
    [
      "a rotation",
      one('<rect width="1" height="1" transform="rotate(45)"/>'),
      /only translate and scale/,
    ],
    [
      "an unknown role",
      one('<rect width="1" height="1" class="shiny"/>'),
      /the role shiny is not one of/,
    ],
    [
      "two roles",
      one('<rect width="1" height="1" class="body accent"/>'),
      /one role only/,
    ],
    [
      "a role with no dark",
      file("", '<s:color role="glow" light="#fff"/>'),
      /needs both light and dark/,
    ],
    [
      "a bad colour",
      file("", '<s:color role="glow" light="red" dark="#000"/>'),
      /needs both light and dark/,
    ],
    [
      "another option of ELK",
      file("", '<s:layout algorithm="force"/>'),
      /layerSpacing, nodeSpacing and direction only/,
    ],
    ["a direction", file("", '<s:layout direction="LEFT"/>'), /RIGHT or DOWN/],
    [
      "no alias",
      file('<g s:type="t" s:width="1" s:height="1"/>'),
      /has no s:alias/,
    ],
    [
      "no size",
      file('<g s:type="t"><s:alias val="atom:t"/></g>'),
      /needs s:width and s:height/,
    ],
    [
      "a bad alias",
      one("", 's:width="1" s:height="1"').replace("atom:t", "lut"),
      /is not <kind>:<name>/,
    ],
    [
      "an unknown attribute of a text",
      one('<text s:attribute="secret"/>'),
      /s:attribute="secret" is not one of/,
    ],
    [
      "a symbol twice",
      file(
        '<g s:type="t" s:width="1" s:height="1"><s:alias val="atom:a"/></g><g s:type="t" s:width="1" s:height="1"><s:alias val="atom:b"/></g>',
      ),
      /defined twice/,
    ],
    ["not a symbol", file("<rect/>"), /a symbol is a g with s:type/],
    ["not XML", "<svg", /not well-formed XML/],
  ];
  it.each(refusals)("refuses %s", (_, text, message) => {
    expect(() => compileSymbols(text, "package")).toThrow(SymbolError);
    expect(() => compileSymbols(text, "package")).toThrow(message);
  });

  it("says where the refused element is", () => {
    const text = one('\n\n<rect width="1" height="1"/>\n<image/>');
    expect(() => compileSymbols(text, "package")).toThrow(/<image> \(line 5\)/);
  });

  it("refuses a hidden symbol in the default file, or one that draws something", () => {
    expect(() =>
      compileSymbols(file('<g s:type="t" s:hide="true"/>'), "default"),
    ).toThrow("the default file hides nothing");
    expect(() =>
      compileSymbols(
        file('<g s:type="t" s:hide="true"><rect/></g>'),
        "package",
      ),
    ).toThrow("holds nothing");
  });

  it("reads the colours: #rgb, #rrggbb, #rrggbbaa", () => {
    expect(parseColor("#fa0")).toEqual([255, 170, 0, 255]);
    expect(parseColor("#102030")).toEqual([16, 32, 48, 255]);
    expect(parseColor("#10203080")).toEqual([16, 32, 48, 128]);
    expect(parseColor("orange")).toBeNull();
  });
});

const PACKAGE = file(
  `
  <g s:type="atom_lut" s:width="30" s:height="30">
    <s:alias val="atom:.names"/>
    <s:color role="body" light="#ffeedd" dark="#332211"/>
    <rect width="30" height="30"/>
  </g>
  <g s:type="cluster_clb" s:hide="true"/>
  <g s:type="dsp" s:width="80" s:height="80">
    <s:alias val="cluster:dsp_top"/>
    <s:alias val="atom:.latch"/>
    <rect width="80" height="80" class="glow"/>
  </g>`,
  `<s:color role="label" light="#000000" dark="#ffffff"/>
   <s:color role="glow" light="#ff0" dark="#880"/>
   <s:layout nodeSpacing="30"/>`,
);

describe("the symbols of a package over the default ones", () => {
  it("replaces, hides and adds symbols, by s:type", () => {
    const set = symbolsOf(PACKAGE);
    expect(set.problem).toBe("");
    expect(set.replaced).toEqual(["atom_lut"]);
    expect(set.hiddenTypes).toEqual(["cluster_clb"]);
    expect(set.added).toEqual(["dsp"]);
    expect(symbolsNote(set)).toBe(
      "Symbols: default; the package replaces 1, hides 1, adds 1",
    );
    // Replaced: its drawing, its ports, its colours
    const lut = symbolFor(set, ["atom:.names"])!;
    expect(lut.origin).toBe("package");
    expect(lut.ports).toEqual([]);
    expect(roleColor(set, lut, "body", false)).toEqual([255, 238, 221, 255]);
    expect(roleColor(set, lut, "body", true)).toEqual([51, 34, 17, 255]);
    // Hidden: the next alias is used
    expect(symbolFor(set, ["cluster:clb", "cluster:generic"])!.type).toBe(
      "cluster_generic",
    );
    // Added, and an alias taken from a default symbol
    expect(symbolFor(set, ["cluster:dsp_top"])!.type).toBe("dsp");
    expect(symbolFor(set, ["atom:.latch"])!.type).toBe("dsp");
    // The colours of the root, one role at a time; the layout, one value at a time
    expect(roleColor(set, null, "label", true)).toEqual([255, 255, 255, 255]);
    expect(roleColor(set, null, "outline", false)).toEqual(
      roleColor(symbolsOf(null), null, "outline", false),
    );
    expect(set.layout).toEqual({
      layerSpacing: 60,
      nodeSpacing: 30,
      direction: "RIGHT",
    });
  });

  it("draws a plain box when every symbol of an object is hidden", () => {
    const set = symbolsOf(
      file(
        '<g s:type="cluster_clb" s:hide="true"/><g s:type="cluster_generic" s:hide="true"/>',
      ),
    );
    expect(symbolFor(set, ["cluster:clb", "cluster:generic"])).toBeNull();
  });

  it("uses the default symbols when the package's file is refused, and says why", () => {
    const bad = symbolsOf(
      file(
        '<g s:type="t" s:width="1" s:height="1"><s:alias val="atom:t"/><image/></g>',
      ),
    );
    expect(bad.problem).toMatch(
      /the symbols of the package are not used: .*image is refused/,
    );
    expect(bad.replaced).toEqual([]);
    const unknown = mergeSymbols(
      defaultSymbols(),
      compileSymbols(file('<g s:type="no_such" s:hide="true"/>'), "package"),
    );
    expect(unknown.problem).toBe(
      "the package hides no_such, which is not a default symbol",
    );
    expect(unknown.hiddenTypes).toEqual([]);
  });

  it("leaves the colour to the view where no file sets it", () => {
    const set = symbolsOf(null);
    const lut = symbolFor(set, ["atom:.names"])!;
    expect(roleColor(set, lut, "body", false)).toBeUndefined();
    expect(roleColor(set, lut, "outline", true)).toEqual([20, 20, 20, 255]);
  });
});

describe("the symbols of a package of the repository", () => {
  it("compile, and change what they say they change", async () => {
    const { default: text } =
      await import("../../../../../../openfpga_flow/device-packages/k6_frac_n10_mem1k_3x2/views/netlist_symbols.svg?raw");
    const set = symbolsOf(text);
    expect(set.problem).toBe("");
    expect(set.replaced).toEqual(["atom_memory"]);
    expect(set.hiddenTypes).toEqual(["atom_mult"]);
    expect(set.added).toEqual(["cluster_memory"]);
    const memory = symbolFor(set, ["cluster:memory"])!;
    expect(memory.ports.map((p) => p.pid)).toEqual([
      "waddr",
      "raddr",
      "data_in",
      "wen",
      "ren",
      "clk",
      "data_out",
    ]);
    expect(roleColor(set, memory, "memory", true)).toEqual([36, 50, 74, 255]);
  });
});
