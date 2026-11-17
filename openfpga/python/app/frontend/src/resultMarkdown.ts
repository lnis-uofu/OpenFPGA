// The answer of a tool as Markdown, for a person: tables for what has fields and rows, headings for
// the parts, one line for the tools whose answer is a sentence. The JSON that the model reads is not
// changed: this is only how the page shows it (the JSON is one click away).
import type { Answer } from "./api";

type Json = null | boolean | number | string | Json[] | { [key: string]: Json };
type Dict = { [key: string]: Json };

const HIDDEN = new Set(["schema_version", "kind", "more", "truncated"]);
const MAX_ROWS = 25;
const MAX_CELL = 90;

const isScalar = (v: Json): v is null | boolean | number | string => v === null || typeof v !== "object";
const isDict = (v: Json): v is Dict => v !== null && typeof v === "object" && !Array.isArray(v);
const label = (key: string) => key.replace(/_/g, " ");

/** A word or a name is code (the Markdown of the page reads * and [ in a name as marks); a sentence is text */
function text(s: string): string {
  const flat = s.replace(/\s*\n\s*/g, " ").replace(/\|/g, "¦").trim();
  if (!flat) return "–";
  return /\s/.test(flat) && !/[*[\]`]/.test(flat) ? flat : "`" + flat.replace(/`/g, "'") + "`";
}

function scalar(v: null | boolean | number | string): string {
  if (v === null) return "–";
  if (typeof v === "boolean") return v ? "yes" : "no";
  if (typeof v === "number") return Number.isInteger(v) ? String(v) : String(Number(v.toPrecision(5))); // not 16 digits
  return text(v);
}

const clip = (s: string) => (s.length > MAX_CELL ? s.slice(0, MAX_CELL - 1) + "…" : s);

/** One value in a cell of a table or after a name: a few words, never a block */
function cell(v: Json): string {
  if (isScalar(v)) return scalar(v);
  if (Array.isArray(v)) {
    if (v.length && v.every((x) => !isScalar(x))) return `${v.length} item${v.length === 1 ? "" : "s"}`; // the JSON has them
    return v.length ? clip(v.map(cell).join(", ")) : "–";
  }
  const parts = Object.entries(v)
    .filter(([k, x]) => !HIDDEN.has(k) && x !== null)
    .map(([k, x]) => `${label(k)} ${cell(x)}`);
  return parts.length ? clip(parts.join("; ")) : "–";
}

/** A value that fits in a line: a scalar, a short list of them, a small record of them */
function simple(v: Json): boolean {
  if (isScalar(v)) return typeof v !== "string" || !v.includes("\n");
  if (Array.isArray(v)) return v.length <= 20 && v.every((x) => isScalar(x) && (typeof x !== "string" || !x.includes("\n")));
  const entries = Object.values(v);
  return entries.length <= 5 && entries.every((x) => isScalar(x) && (typeof x !== "string" || !x.includes("\n")));
}

function table(rows: Dict[]): string[] {
  const columns: string[] = [];
  for (const row of rows) for (const k of Object.keys(row)) if (!HIDDEN.has(k) && !columns.includes(k)) columns.push(k);
  const shown = rows.slice(0, MAX_ROWS);
  const out = [
    "| " + columns.map(label).join(" | ") + " |",
    "| " + columns.map(() => "---").join(" | ") + " |",
    ...shown.map((row) => "| " + columns.map((c) => (c in row ? cell(row[c]) : "–")).join(" | ") + " |"),
  ];
  if (rows.length > shown.length) out.push("", `… and ${rows.length - shown.length} more rows (the JSON has them all)`);
  return out;
}

function code(s: string): string[] {
  return ["```", s.replace(/```/g, "'''"), "```"];
}

function heading(level: number, name: string): string {
  return `${"#".repeat(Math.min(4, 3 + level))} ${label(name)}`;
}

/** The parts of a value, as blocks of lines */
function body(v: Json, level: number): string[] {
  if (typeof v === "string") return v.includes("\n") ? code(v) : [text(v)];
  if (isScalar(v)) return [scalar(v)];
  if (Array.isArray(v)) {
    if (v.length === 0) return ["(none)"];
    if (v.every(isDict)) return table(v as Dict[]);
    return v.map((x) => `- ${cell(x)}`);
  }
  const entries = Object.entries(v).filter(([k]) => !HIDDEN.has(k));
  const fields = entries.filter(([, x]) => x !== null && simple(x));
  const parts = entries.filter(([, x]) => x !== null && !simple(x));
  const out: string[] = [];
  if (fields.length) out.push("| Field | Value |", "| --- | --- |", ...fields.map(([k, x]) => `| ${label(k)} | ${cell(x)} |`));
  for (const [k, x] of parts) out.push("", heading(level, k), "", ...body(x, level + 1));
  return out;
}

const list = (v: unknown) => (Array.isArray(v) ? v.map(String).join(" → ") : "–");

/** The tools whose answer is a sentence */
function headline(tool: string, data: Dict): string | null {
  switch (tool) {
    case "run_stage":
      return `**${data.stage}** finished in ${data.seconds} s. Stages reached: ${list(data.reached)}.`;
    case "session_open":
      return `Session **${data.session}** opened (directory \`${data.directory}\`).`;
    case "session_close":
      return `Session **${data.session}** closed. Its files stay in the workspace.`;
    case "set_option":
    case "set_constraint": {
      const dropped = Array.isArray(data.dropped) && data.dropped.length ? `Stages dropped: ${list(data.dropped)}.` : "No stage was dropped.";
      return `**${data.option}**: ${cell(data.before ?? null)} → ${cell(data.after ?? null)}. ${dropped}`;
    }
    default:
      return null;
  }
}

/** The answer of a tool call, for the page */
export function resultMarkdown(tool: string, result: Answer): string {
  const out: string[] = [];
  if (!result.ok) {
    out.push("**Failed**", "");
    for (const d of result.diagnostics ?? []) {
      const where = (d as { file?: string | null; line?: number | null }).file;
      out.push(`- **${d.kind}**: ${d.message.replace(/\n+/g, " ")}${where ? ` (${where})` : ""}`);
      if (d.hint) out.push(`  Try: ${d.hint.replace(/\n+/g, " ")}`);
    }
    const extra: Dict = {};
    for (const [k, v] of Object.entries(result)) if (!["ok", "diagnostics", "data", ...HIDDEN].includes(k)) extra[k] = v as Json;
    if (Object.keys(extra).length) out.push("", ...body(extra, 0));
  } else {
    const data = (result.data ?? null) as Json;
    const line = isDict(data) ? headline(tool, data) : null;
    out.push(...(line !== null ? [line] : data === null ? ["Done."] : body(data, 0)));
  }
  if (result.truncated) out.push("", `> ${result.more ?? "The result was cut."}`);
  return out.join("\n");
}
