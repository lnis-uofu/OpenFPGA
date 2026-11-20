// BLIF (Berkeley Logic Interchange Format), the circuits of a flow: Monaco has no such language, so
// this is one made for the viewer. It colours the directives (.model, .inputs, .names, ...), the
// comments (#), the cubes of a truth table (010-1 1), the = of the pins of a .subckt or .gate, and
// leaves the names of the signals plain. A line that ends with a backslash goes on in the next.

export const BLIF_ID = "blif";

export const blifConfiguration = {
  comments: { lineComment: "#" },
  brackets: [] as [string, string][],
  wordPattern: /[^\s=\\#]+/,
};

export const blifTokens = {
  defaultToken: "",
  tokenizer: {
    root: [
      [/#.*$/, "comment"],
      [/^\s*\.(model|inputs|outputs|clock|names|latch|subckt|gate|search|exdc|end|blackbox)\b/, "keyword"],
      [/^\s*\.(default_input_arrival|default_output_required|input_arrival|output_required)\b/, "keyword"],
      [/^\s*\.[A-Za-z_][\w]*/, "keyword"], // another directive of the format is still one
      [/^\s*[01\-]+(?=\s|$)/, "number"], // a cube of a table (a table with no inputs has only an output)
      [/[01xXzZ23](?=\s*(?:#.*)?$)/, "number"], // the output of a row, and the initial value of a latch
      [/=/, "delimiter"],
      [/\\\s*$/, "delimiter"],
      [/[^\s=\\#]+/, "identifier"],
    ],
  },
};
