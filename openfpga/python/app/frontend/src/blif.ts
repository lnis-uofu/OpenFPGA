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

/** The directives that start a block of lines: a cell and what follows it (the rows of the table
 * of a .names, the lines that go on with a backslash) */
const BLOCK = /^\s*\.(names|latch|subckt|gate)\b/;

/** The blocks of a BLIF that fold, as [first, last] line numbers from 1: each .names (and .latch,
 * .subckt, .gate) with the lines up to the next directive, without the empty lines and the
 * comments at its end. A block of one line does not fold */
export function blifFoldingRanges(lines: string[]): [number, number][] {
  const ranges: [number, number][] = [];
  let start = -1;
  let last = -1;
  let continued = false;
  const close = () => {
    if (start >= 0 && last > start) ranges.push([start + 1, last + 1]);
    start = -1;
  };
  lines.forEach((line, i) => {
    const text = line.trim();
    // A line that goes on from the one before belongs to it
    const directive = !continued && text.startsWith(".");
    continued = /\\\s*$/.test(line);
    if (directive) {
      close();
      if (BLOCK.test(line)) start = last = i;
      return;
    }
    if (start >= 0 && text && !text.startsWith("#")) last = i;
  });
  close();
  return ranges;
}
