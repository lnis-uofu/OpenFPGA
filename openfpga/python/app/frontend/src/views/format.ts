// The file of the data of a view (docs/plan_design_views.md, 3.1), as the core writes it
// (export_view, view_tile): the columns of its tables are typed arrays over the one buffer of the
// file, with no copy and no parse; a column of text decodes one string when it is asked for.
//
// Layout, little-endian: the magic "OFVIEW1\0", the length of the header (u32), the header (JSON),
// then the data from the next multiple of 8, each column at its offset from the start of the data.

export type NumberArray =
  | Uint8Array
  | Int8Array
  | Uint16Array
  | Int16Array
  | Uint32Array
  | Int32Array
  | Float32Array
  | Float64Array;

/** A column of text: its strings by row, decoded when asked for */
export interface TextColumn {
  readonly length: number;
  get(row: number): string;
  /** Every string, decoded */
  all(): string[];
}

export type Column = NumberArray | TextColumn;

export interface ViewTable {
  rows: number;
  columns: Record<string, Column>;
}

export interface ViewData {
  kind: string;
  version: string;
  meta: Record<string, unknown>;
  tables: Record<string, ViewTable>;
}

interface ColumnHeader {
  name: string;
  dtype: string;
  offset: number;
  count: number;
  blob_offset?: number;
  blob_bytes?: number;
}

const ARRAYS: Record<
  string,
  new (buffer: ArrayBuffer, offset: number, length: number) => NumberArray
> = {
  u8: Uint8Array,
  i8: Int8Array,
  u16: Uint16Array,
  i16: Int16Array,
  u32: Uint32Array,
  i32: Int32Array,
  f32: Float32Array,
  f64: Float64Array,
};

const MAGIC = "OFVIEW1\0";

export class ViewFormatError extends Error {}

/** The tables of a file of a view. The arrays share the buffer: keep it while they are used */
export function decodeView(buffer: ArrayBuffer): ViewData {
  const bytes = new Uint8Array(buffer);
  if (
    bytes.length < 12 ||
    String.fromCharCode(...bytes.subarray(0, 8)) !== MAGIC
  ) {
    throw new ViewFormatError("Not a file of a view");
  }
  const length = new DataView(buffer).getUint32(8, true);
  if (12 + length > bytes.length)
    throw new ViewFormatError("The header goes past the end of the file");
  const header = JSON.parse(
    new TextDecoder().decode(bytes.subarray(12, 12 + length)),
  );
  if (header.format !== "openfpga_view" || header.format_version !== 1) {
    throw new ViewFormatError("A file of a view of an unknown version");
  }
  const start = Math.ceil((12 + length) / 8) * 8;
  const decoder = new TextDecoder();
  const tables: Record<string, ViewTable> = {};
  for (const table of header.tables as {
    name: string;
    rows: number;
    columns: ColumnHeader[];
  }[]) {
    const columns: Record<string, Column> = {};
    for (const column of table.columns) {
      const at = start + column.offset;
      if (column.dtype === "str") {
        const ends = new Uint32Array(buffer, at, column.count);
        const blob = bytes.subarray(
          start + (column.blob_offset ?? 0),
          start + (column.blob_offset ?? 0) + (column.blob_bytes ?? 0),
        );
        const text: TextColumn = {
          length: Math.max(0, column.count - 1),
          get: (row: number) =>
            decoder.decode(blob.subarray(ends[row], ends[row + 1])),
          all: () =>
            Array.from({ length: Math.max(0, column.count - 1) }, (_, row) =>
              text.get(row),
            ),
        };
        columns[column.name] = text;
      } else {
        const make = ARRAYS[column.dtype];
        if (!make)
          throw new ViewFormatError(
            `A column of an unknown type: ${column.dtype}`,
          );
        const size =
          make === Float64Array
            ? 8
            : make === Uint32Array ||
                make === Int32Array ||
                make === Float32Array
              ? 4
              : make === Uint16Array || make === Int16Array
                ? 2
                : 1;
        if (at + size * column.count > bytes.length)
          throw new ViewFormatError(
            `The column ${column.name} goes past the end of the file`,
          );
        columns[column.name] = new make(buffer, at, column.count);
      }
    }
    tables[table.name] = { rows: table.rows, columns };
  }
  return {
    kind: header.kind,
    version: header.version,
    meta: header.meta ?? {},
    tables,
  };
}

/** A column of numbers of a table, or an error that names what is missing */
export function numbers(
  view: ViewData,
  table: string,
  column: string,
): NumberArray {
  const found = view.tables[table]?.columns[column];
  if (!found || !ArrayBuffer.isView(found))
    throw new ViewFormatError(`No column of numbers ${table}.${column}`);
  return found as NumberArray;
}

/** A column of text of a table */
export function texts(
  view: ViewData,
  table: string,
  column: string,
): TextColumn {
  const found = view.tables[table]?.columns[column];
  if (!found || ArrayBuffer.isView(found))
    throw new ViewFormatError(`No column of text ${table}.${column}`);
  return found as TextColumn;
}

/** The file of a view of the session (/api/view): kind and its scope */
export async function fetchView(
  kind: string,
  scope: Record<string, string | number> = {},
): Promise<ViewData> {
  const query = new URLSearchParams({
    kind,
    ...Object.fromEntries(
      Object.entries(scope).map(([k, v]) => [k, String(v)]),
    ),
  });
  return decodeResponse(await fetch(`/api/view?${query}`));
}

/** A tile of a view of the device (/api/tile) */
export async function fetchTile(
  kind: string,
  level: number,
  x: number,
  y: number,
): Promise<ViewData> {
  const query = new URLSearchParams({
    kind,
    level: String(level),
    x: String(x),
    y: String(y),
  });
  return decodeResponse(await fetch(`/api/tile?${query}`));
}

async function decodeResponse(response: Response): Promise<ViewData> {
  if (!response.ok) {
    let message = `${response.status} ${response.statusText}`;
    try {
      const answer = await response.json();
      message = answer.diagnostics?.[0]?.message ?? answer.error ?? message;
    } catch {
      // not JSON: the status says it
    }
    throw new ViewFormatError(message);
  }
  return decodeView(await response.arrayBuffer());
}
