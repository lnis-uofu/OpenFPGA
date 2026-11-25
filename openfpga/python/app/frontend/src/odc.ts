// ODC (OpenFPGA Design Constraints, docs/spec_odc.md), the constraints of a design in one file: Monaco
// has no such language, so this is one made for the viewer. It colours the commands that the reader
// knows (another word at the start of a line is shown as an error), the queries in brackets
// ([get_ports ...]), the options (-period), the properties of set_property, the locations of the grid
// (X3Y2, X3Y2/1, X1Y1:X4Y6), numbers, strings and comments (#). A line that ends with a backslash
// goes on in the next.

export const ODC_ID = "odc";

/** The commands of version 1, as the reader's table has them (libs/libodc/src/odc_reader.cpp) */
export const ODC_COMMANDS = [
  "create_clock",
  "set_clock_groups",
  "set_clock_latency",
  "set_clock_uncertainty",
  "set_input_delay",
  "set_output_delay",
  "set_false_path",
  "set_max_delay",
  "set_min_delay",
  "set_multicycle_path",
  "set_disable_timing",
  "set_timing_derate",
  "set_property",
  "set_synth_option",
  "create_pblock",
  "resize_pblock",
  "add_cells_to_pblock",
  "set_global_signal",
  "odc_version",
  "include",
];

export const ODC_QUERIES = [
  "get_ports",
  "get_cells",
  "get_nets",
  "get_pins",
  "get_clocks",
  "get_pblocks",
  "get_sites",
  "all_inputs",
  "all_outputs",
  "all_clocks",
];

export const ODC_PROPERTIES = ["KEEP", "DONT_TOUCH", "MAX_FANOUT", "RAM_STYLE", "LOC", "PACKAGE_PIN", "PIN_DEFAULT"];

export const odcConfiguration = {
  comments: { lineComment: "#" },
  brackets: [
    ["[", "]"],
    ["{", "}"],
  ] as [string, string][],
  autoClosingPairs: [
    { open: "[", close: "]" },
    { open: "{", close: "}" },
    { open: '"', close: '"' },
  ],
  wordPattern: /[^\s[\]{}"#\\]+/,
};

export const odcTokens = {
  defaultToken: "",
  commands: ODC_COMMANDS,
  queries: ODC_QUERIES,
  properties: ODC_PROPERTIES,
  tokenizer: {
    root: [
      [/^\s*#.*$/, "comment"],
      [/;\s*#.*$/, "comment"],
      [
        /^(\s*)([A-Za-z_]\w*)/,
        ["", { cases: { "@commands": "keyword", "@default": "invalid" } }],
      ],
      { include: "@words" },
    ],
    words: [
      [/\[/, { token: "delimiter.bracket", next: "@query" }],
      [/[{}]/, "delimiter.curly"],
      [/"/, { token: "string.quote", next: "@string" }],
      [/-[A-Za-z_]\w*/, "attribute.name"],
      [/\bX\d+Y\d+(?:\/\d+)?(?::X\d+Y\d+)?\b/, "number.hex"],
      [/\b\d+(?:\.\d+)?(?:[eE][-+]?\d+)?\b/, "number"],
      [/[A-Z][A-Z_]+\b/, { cases: { "@properties": "type", "@default": "identifier" } }],
      [/\\\s*$/, "delimiter"],
      [/[^\s[\]{}"#\\]+/, "identifier"],
    ],
    query: [
      [/\]/, { token: "delimiter.bracket", next: "@pop" }],
      [/[A-Za-z_]\w*/, { cases: { "@queries": "predefined", "@default": "identifier" } }],
      { include: "@words" },
    ],
    string: [
      [/[^"\\]+/, "string"],
      [/\\./, "string.escape"],
      [/"/, { token: "string.quote", next: "@pop" }],
    ],
  },
};
