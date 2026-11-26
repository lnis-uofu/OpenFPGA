import { describe, expect, it, vi } from "vitest";
import { EXPECTED, PINS_BIN, ROUTING_BIN } from "./fixtures";
import {
  ViewFormatError,
  decodeView,
  fetchTile,
  numbers,
  texts,
} from "./format";

const FILES: Record<string, string> = {
  "routing.bin": ROUTING_BIN,
  "pins.bin": PINS_BIN,
};
const fixture = (name: string): ArrayBuffer => {
  const text = atob(FILES[name]);
  const bytes = new Uint8Array(text.length);
  for (let i = 0; i < text.length; i++) bytes[i] = text.charCodeAt(i);
  return bytes.buffer;
};
// What the Python reader read from the same files, which the core wrote
const expected = EXPECTED as Record<string, any>; // eslint-disable-line @typescript-eslint/no-explicit-any

/** A file by hand, as the format says, for the types that the exports do not use */
function encode(
  columns: { name: string; dtype: string; values: number[] | string[] }[],
  rows: number,
): ArrayBuffer {
  const parts: Uint8Array[] = [];
  const headers: Record<string, unknown>[] = [];
  let offset = 0;
  const push = (data: Uint8Array) => {
    parts.push(data);
    const padded = Math.ceil(data.length / 8) * 8;
    if (padded > data.length) parts.push(new Uint8Array(padded - data.length));
    const at = offset;
    offset += padded;
    return at;
  };
  for (const column of columns) {
    if (column.dtype === "str") {
      const encoded = (column.values as string[]).map((s) =>
        new TextEncoder().encode(s),
      );
      const ends = new Uint32Array(encoded.length + 1);
      encoded.forEach((e, i) => (ends[i + 1] = ends[i] + e.length));
      const blob = new Uint8Array(ends[encoded.length]);
      encoded.forEach((e, i) => blob.set(e, ends[i]));
      const at = push(new Uint8Array(ends.buffer));
      const blobAt = push(blob);
      headers.push({
        name: column.name,
        dtype: "str",
        offset: at,
        count: ends.length,
        blob_offset: blobAt,
        blob_bytes: blob.length,
      });
    } else {
      const make = {
        i8: Int8Array,
        i16: Int16Array,
        f64: Float64Array,
        u8: Uint8Array,
      }[column.dtype as "i8"]!;
      const array = new make(column.values as number[]);
      headers.push({
        name: column.name,
        dtype: column.dtype,
        offset: push(new Uint8Array(array.buffer)),
        count: array.length,
      });
    }
  }
  const header = new TextEncoder().encode(
    JSON.stringify({
      format: "openfpga_view",
      format_version: 1,
      kind: "test",
      version: "v1",
      meta: { a: 1 },
      tables: [{ name: "t", rows, columns: headers }],
    }),
  );
  const start = Math.ceil((12 + header.length) / 8) * 8;
  const out = new Uint8Array(start + offset);
  out.set(new TextEncoder().encode("OFVIEW1\0"), 0);
  new DataView(out.buffer).setUint32(8, header.length, true);
  out.set(header, 12);
  let at = start;
  for (const part of parts) {
    out.set(part, at);
    at += part.length;
  }
  return out.buffer;
}

describe("the file of a view", () => {
  it("reads the files that the core writes as the Python reader does", () => {
    for (const name of ["routing", "pins"]) {
      const view = decodeView(fixture(`${name}.bin`));
      const want = expected[name];
      expect(view.kind).toBe(want.kind);
      expect(view.version).toBe(want.version);
      expect(view.meta).toEqual(want.meta);
      expect(Object.keys(view.tables)).toEqual(Object.keys(want.tables));
      for (const [table, columns] of Object.entries(
        want.tables as Record<string, Record<string, unknown[]>>,
      )) {
        for (const [column, values] of Object.entries(columns)) {
          const got = view.tables[table].columns[column];
          const list = ArrayBuffer.isView(got)
            ? Array.from(got as ArrayLike<number>)
            : (got as { all(): string[] }).all();
          expect(list, `${name}.${table}.${column}`).toEqual(values);
        }
      }
    }
  });

  it("gives the columns as typed arrays over the buffer, with no copy", () => {
    const buffer = fixture("routing.bin");
    const view = decodeView(buffer);
    const xlow = numbers(view, "nodes", "xlow");
    expect(xlow).toBeInstanceOf(Uint16Array);
    expect(xlow.buffer).toBe(buffer);
    expect(numbers(view, "nodes", "parent")).toBeInstanceOf(Int32Array);
    expect(texts(view, "nets", "name").get(0)).toBe(
      expected.routing.tables.nets.name[0],
    );
    expect(() => numbers(view, "nets", "name")).toThrow(ViewFormatError);
    expect(() => texts(view, "nope", "x")).toThrow(ViewFormatError);
  });

  it("reads every type, text that is not ASCII, and empty strings", () => {
    const view = decodeView(
      encode(
        [
          { name: "a", dtype: "i8", values: [-1, 2] },
          { name: "b", dtype: "i16", values: [-300, 300] },
          { name: "c", dtype: "f64", values: [1e-12, -3] },
          { name: "s", dtype: "str", values: ["ünï", ""] },
        ],
        2,
      ),
    );
    expect(Array.from(numbers(view, "t", "a"))).toEqual([-1, 2]);
    expect(Array.from(numbers(view, "t", "b"))).toEqual([-300, 300]);
    expect(Array.from(numbers(view, "t", "c"))).toEqual([1e-12, -3]);
    expect(texts(view, "t", "s").all()).toEqual(["ünï", ""]);
    expect(view.meta).toEqual({ a: 1 });
  });

  it("refuses what is not a file of a view", () => {
    expect(() =>
      decodeView(new TextEncoder().encode("not a view at all").buffer),
    ).toThrow("Not a file of a view");
    const cut = fixture("pins.bin").slice(0, 200);
    expect(() => decodeView(cut)).toThrow(ViewFormatError);
  });

  it("says what the server refused", async () => {
    vi.stubGlobal(
      "fetch",
      vi.fn(() =>
        Promise.resolve(
          new Response(
            JSON.stringify({
              ok: false,
              diagnostics: [{ kind: "busy", message: "A call is running" }],
            }),
            { status: 409 },
          ),
        ),
      ),
    );
    await expect(fetchTile("grid", 0, 0, 0)).rejects.toThrow(
      "A call is running",
    );
    vi.unstubAllGlobals();
  });
});
