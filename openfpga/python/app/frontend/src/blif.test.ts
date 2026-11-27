import { describe, expect, it } from "vitest";
import { blifFoldingRanges } from "./blif";

describe("the folding of a BLIF", () => {
  it("folds each .names with its rows, up to the next directive", () => {
    const lines = [
      ".model top", // 1
      ".inputs a b \\", // 2
      "  c", // 3
      ".outputs y", // 4
      ".names a b n1", // 5
      "11 1", // 6
      "", // 7
      "# the next", // 8
      ".names n1 c \\", // 9
      "  y", // 10
      "1- 1", // 11
      "-1 1", // 12
      ".latch n1 q re clk 0", // 13
      ".names y", // 14
      ".subckt adder a=a \\", // 15
      "  b=b s=s", // 16
      ".end", // 17
    ];
    expect(blifFoldingRanges(lines)).toEqual([
      [5, 6], // the comment and the empty line stay out
      [9, 12], // the line that goes on with a backslash is in, not a directive
      [15, 16],
    ]);
  });

  it("folds nothing in a file with no table", () => {
    expect(blifFoldingRanges([".model m", ".inputs a", ".end"])).toEqual([]);
  });
});
