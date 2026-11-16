import type { ComponentChildren } from "preact";

/**
 * A small Markdown for the answers of a model: paragraphs, headings, lists (nested by indent),
 * fenced and inline code, bold, italic, links, quotes, rules and tables. It makes nodes, never
 * HTML, so nothing that a model writes can become markup; a link is an anchor only for http and
 * https. A text that is cut in the middle (a stream) shows as far as it goes: a fence that is not
 * closed is code to the end.
 */

type Node = ComponentChildren;

const LIST = /^(\s*)([-*+]|\d+[.)])\s+(.*)$/;
const FENCE = /^\s*```\s*([\w+-]*)\s*$/;
const HEADING = /^(#{1,6})\s+(.*?)\s*#*\s*$/;
const RULE = /^\s*([-*_])(\s*\1){2,}\s*$/;
const TABLE_SEPARATOR = /^\s*\|?\s*:?-{2,}:?\s*(\|\s*:?-{2,}:?\s*)*\|?\s*$/;

/** The inline marks of a line: `code`, **bold**, *italic*, [text](https://…) */
export function inline(text: string, key = "i"): Node[] {
  const out: Node[] = [];
  let rest = text;
  let n = 0;
  const push = (node: Node) => out.push(node);
  const patterns: [RegExp, (m: RegExpExecArray) => Node][] = [
    [/^`([^`]+)`/, (m) => <code key={`${key}${n++}`}>{m[1]}</code>],
    [/^\*\*([^*]+?)\*\*/, (m) => <strong key={`${key}${n++}`}>{inline(m[1], `${key}b${n}`)}</strong>],
    [/^\*([^*\s][^*]*?)\*/, (m) => <em key={`${key}${n++}`}>{inline(m[1], `${key}e${n}`)}</em>],
    [
      /^\[([^\]]+)\]\(([^)\s]+)\)/,
      (m) =>
        /^https?:\/\//i.test(m[2]) ? (
          <a key={`${key}${n++}`} href={m[2]} target="_blank" rel="noopener noreferrer">
            {m[1]}
          </a>
        ) : (
          m[0]
        ),
    ],
  ];
  let plain = "";
  const flush = () => {
    if (plain) push(plain);
    plain = "";
  };
  while (rest) {
    let matched = false;
    for (const [pattern, make] of patterns) {
      const m = pattern.exec(rest);
      if (m) {
        flush();
        push(make(m));
        rest = rest.slice(m[0].length);
        matched = true;
        break;
      }
    }
    if (!matched) {
      plain += rest[0];
      rest = rest.slice(1);
    }
  }
  flush();
  return out;
}

function cells(line: string): string[] {
  return line
    .trim()
    .replace(/^\|/, "")
    .replace(/\|$/, "")
    .split("|")
    .map((c) => c.trim());
}

function lines(text: string, key: string): Node[] {
  const parts = text.split("\n");
  const out: Node[] = [];
  parts.forEach((part, index) => {
    if (index) out.push(<br key={`${key}br${index}`} />);
    out.push(...inline(part, `${key}${index}_`));
  });
  return out;
}

interface Item {
  ordered: boolean;
  indent: number;
  text: string;
}

/** Items of one list, with the deeper ones inside the one before them */
function list(items: Item[], key: string): Node {
  const level = items[0].indent;
  const ordered = items[0].ordered;
  const children: Node[] = [];
  let i = 0;
  while (i < items.length) {
    const item = items[i];
    let j = i + 1;
    while (j < items.length && items[j].indent > level) j++;
    const nested = items.slice(i + 1, j);
    children.push(
      <li key={`${key}.${i}`}>
        {inline(item.text, `${key}.${i}_`)}
        {nested.length > 0 && list(nested, `${key}.${i}`)}
      </li>,
    );
    i = j;
  }
  return ordered ? <ol key={key}>{children}</ol> : <ul key={key}>{children}</ul>;
}

export function Markdown(props: { text: string }) {
  const source = props.text.replace(/\r\n?/g, "\n").split("\n");
  const blocks: Node[] = [];
  let i = 0;
  const startsBlock = (line: string, next: string | undefined) =>
    FENCE.test(line) ||
    HEADING.test(line) ||
    RULE.test(line) ||
    LIST.test(line) ||
    /^\s*>/.test(line) ||
    (line.includes("|") && next !== undefined && TABLE_SEPARATOR.test(next) && next.includes("-"));
  while (i < source.length) {
    const line = source[i];
    const key = `b${blocks.length}`;
    if (!line.trim()) {
      i++;
      continue;
    }
    const fence = FENCE.exec(line);
    if (fence) {
      const body: string[] = [];
      i++;
      while (i < source.length && !FENCE.test(source[i])) body.push(source[i++]);
      i++; // the closing fence, if there is one
      blocks.push(
        <pre class="md-code" key={key} data-lang={fence[1] || undefined}>
          <code>{body.join("\n")}</code>
        </pre>,
      );
      continue;
    }
    const heading = HEADING.exec(line);
    if (heading) {
      const Tag = `h${Math.min(6, heading[1].length + 2)}` as "h3";
      blocks.push(
        <Tag class="md-heading" key={key}>
          {inline(heading[2], `${key}_`)}
        </Tag>,
      );
      i++;
      continue;
    }
    if (RULE.test(line)) {
      blocks.push(<hr key={key} />);
      i++;
      continue;
    }
    if (/^\s*>/.test(line)) {
      const quoted: string[] = [];
      while (i < source.length && /^\s*>/.test(source[i])) quoted.push(source[i++].replace(/^\s*>\s?/, ""));
      blocks.push(<blockquote key={key}>{lines(quoted.join("\n"), key)}</blockquote>);
      continue;
    }
    if (LIST.test(line)) {
      const items: Item[] = [];
      while (i < source.length) {
        const m = LIST.exec(source[i]);
        if (!m) break;
        items.push({ ordered: /\d/.test(m[2]), indent: m[1].replace(/\t/g, "    ").length, text: m[3] });
        i++;
      }
      blocks.push(list(items, key));
      continue;
    }
    if (line.includes("|") && i + 1 < source.length && TABLE_SEPARATOR.test(source[i + 1]) && source[i + 1].includes("-")) {
      const head = cells(line);
      i += 2;
      const rows: string[][] = [];
      while (i < source.length && source[i].includes("|") && source[i].trim()) rows.push(cells(source[i++]));
      blocks.push(
        <div class="md-table" key={key}>
          <table>
            <thead>
              <tr>
                {head.map((c, k) => (
                  <th key={k}>{inline(c, `${key}h${k}_`)}</th>
                ))}
              </tr>
            </thead>
            <tbody>
              {rows.map((row, r) => (
                <tr key={r}>
                  {row.map((c, k) => (
                    <td key={k}>{inline(c, `${key}r${r}c${k}_`)}</td>
                  ))}
                </tr>
              ))}
            </tbody>
          </table>
        </div>,
      );
      continue;
    }
    const paragraph: string[] = [];
    while (i < source.length && source[i].trim() && (paragraph.length === 0 || !startsBlock(source[i], source[i + 1]))) {
      paragraph.push(source[i++]);
    }
    blocks.push(
      <p class="text" key={key}>
        {lines(paragraph.join("\n"), key)}
      </p>,
    );
  }
  return <div class="md">{blocks}</div>;
}
