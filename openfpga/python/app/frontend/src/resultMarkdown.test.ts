import { describe, expect, it } from "vitest";
import { resultMarkdown } from "./resultMarkdown";

const ok = (data: unknown, more: Record<string, unknown> = {}) => ({ ok: true, data, truncated: false, more: null, ...more }) as never;

describe("the answer of a tool as Markdown", () => {
  it("says the simple answers in a sentence", () => {
    expect(resultMarkdown("run_stage", ok({ stage: "pack", seconds: 0.4, reached: ["DEVICE", "PACKED"] }))).toBe(
      "**pack** finished in 0.4 s. Stages reached: DEVICE → PACKED.",
    );
    expect(resultMarkdown("session_open", ok({ session: "s1", name: "x", directory: "s1" }))).toBe("Session **s1** opened (directory `s1`).");
    expect(resultMarkdown("session_close", ok({ session: "s1", closed: true }))).toContain("Session **s1** closed");
    expect(resultMarkdown("set_option", ok({ option: "place.seed", before: 1, after: 3, dropped: ["PLACED", "ROUTED"] }))).toBe(
      "**place.seed**: 1 → 3. Stages dropped: PLACED → ROUTED.",
    );
    expect(resultMarkdown("set_constraint", ok({ option: "router.route_chan_width", before: 20, after: 24, dropped: [] }))).toContain("No stage was dropped.");
  });

  it("makes a table of the fields, and leaves out what is not for a person", () => {
    const md = resultMarkdown(
      "get_device",
      ok({ kind: "device", schema_version: 1, stage: "DEVICE", grid: { width: 4, height: 4, layers: 1, layout: "2x2" }, locations: null }),
    );
    expect(md).toContain("| Field | Value |");
    expect(md).toContain("| stage | `DEVICE` |");
    expect(md).toContain("| grid | width 4; height 4; layers 1; layout `2x2` |");
    expect(md).not.toContain("schema version");
    expect(md).not.toContain("locations");
  });

  it("makes a table of a list of records, with a heading for the part", () => {
    const md = resultMarkdown(
      "get_utilization",
      ok({ device_utilization: 0.18, block_types: [{ name: "io", used: 7, available: 64 }, { name: "clb", used: 2, available: 4 }] }),
    );
    expect(md).toContain("| device utilization | 0.18 |");
    expect(md).toContain("### block types");
    expect(md).toContain("| name | used | available |");
    expect(md).toContain("| `io` | 7 | 64 |");
  });

  it("cuts a long table and says how many rows there are more", () => {
    const rows = Array.from({ length: 40 }, (_, i) => ({ n: i }));
    const md = resultMarkdown("get_placement", ok({ blocks: rows }));
    expect(md.split("\n").filter((l) => l.startsWith("| ")).length).toBe(2 + 25); // the head, the line under it, 25 rows
    expect(md).toContain("… and 15 more rows (the JSON has them all)");
  });

  it("puts a long text in a code block, and a name that has marks in code", () => {
    const md = resultMarkdown("x", ok({ log: "line one\nline two", pattern: "a*b", note: "a sentence of words" }));
    expect(md).toContain("```\nline one\nline two\n```");
    expect(md).toContain("| pattern | `a*b` |");
    expect(md).toContain("| note | a sentence of words |");
  });

  it("rounds a long number, and counts what is nested in a cell instead of spelling it out", () => {
    const md = resultMarkdown(
      "get_timing",
      ok({ paths: [{ index: 0, slack_ns: -1.4196428477220024, fmax: 704, points: [{ a: 1 }, { a: 2 }, { a: 3 }] }] }),
    );
    expect(md).toContain("| 0 | -1.4196 | 704 | 3 items |");
    expect(resultMarkdown("x", ok({ v: 7.07e-10 }))).toContain("| v | 7.07e-10 |");
  });

  it("keeps a bar of a value out of the table", () => {
    expect(resultMarkdown("x", ok({ v: "a|b" }))).toContain("| v | `a¦b` |");
  });

  it("gives a failure as its diagnostics, with the hint and what else the answer has", () => {
    const md = resultMarkdown("run_stage", {
      ok: false,
      diagnostics: [{ kind: "route_failed", message: "The router found no legal routing", hint: "Widen the channel" }],
      reached: ["DEVICE", "PACKED"],
    } as never);
    expect(md).toContain("**Failed**");
    expect(md).toContain("- **route_failed**: The router found no legal routing");
    expect(md).toContain("Try: Widen the channel");
    expect(md).toContain("| reached | `DEVICE`, `PACKED` |");
  });

  it("tells that the answer was cut", () => {
    expect(resultMarkdown("find", ok({ items: [] }, { truncated: true, more: "call find again with a narrower pattern" }))).toContain(
      "> call find again with a narrower pattern",
    );
  });
});
