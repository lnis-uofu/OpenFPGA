import { render } from "@testing-library/preact";
import { describe, expect, it } from "vitest";
import { Markdown } from "./markdown";

const html = (text: string) => {
  const { container } = render(<Markdown text={text} />);
  return container.innerHTML;
};

describe("the Markdown of an answer", () => {
  it("makes bold, italic and code, and keeps a name with underscores as it is", () => {
    const out = html("Run **run_stage** with *pack*, then `get_timing` and run_stage_two.");
    expect(out).toContain("<strong>run_stage</strong>");
    expect(out).toContain("<em>pack</em>");
    expect(out).toContain("<code>get_timing</code>");
    expect(out).toContain("run_stage_two.");
  });

  it("makes lists, ordered and not, and the deeper ones inside", () => {
    const out = html("1. first\n2. second\n   - inner a\n   - inner b\n3. third\n\n- x\n- y");
    expect(out).toMatch(/<ol><li>first<\/li><li>second<ul><li>inner a<\/li><li>inner b<\/li><\/ul><\/li><li>third<\/li><\/ol>/);
    expect(out).toContain("<ul><li>x</li><li>y</li></ul>");
  });

  it("makes headings, rules, quotes and paragraphs with line breaks", () => {
    const out = html("## Result\n\nline one\nline two\n\n---\n\n> quoted");
    expect(out).toContain('<h4 class="md-heading">Result</h4>');
    expect(out).toContain("line one<br>line two");
    expect(out).toContain("<hr>");
    expect(out).toContain("<blockquote>quoted</blockquote>");
  });

  it("makes a table", () => {
    const out = html("| Path | Slack |\n|---|---|\n| OUT0 | -0.8 |\n| OUT1 | `-0.6` |");
    expect(out).toContain("<th>Path</th><th>Slack</th>");
    expect(out).toContain("<td>OUT0</td><td>-0.8</td>");
    expect(out).toContain("<td><code>-0.6</code></td>");
  });

  it("shows a fenced block as it is, also while the fence is still open", () => {
    expect(html("```tcl\nread_vpr_arch -f a.xml\n```\nafter")).toContain("<code>read_vpr_arch -f a.xml</code>");
    const open = html("text\n```\nlet x = **1**");
    expect(open).toContain("<code>let x = **1**</code>"); // not formatted: it is code
  });

  it("never makes markup of what a model writes", () => {
    const out = html('<script>alert(1)</script> <img src=x onerror=alert(1)> **b**');
    expect(out).not.toContain("<script");
    expect(out).not.toContain("<img");
    expect(out).toContain("&lt;script&gt;");
    const link = html("[ok](https://example.org) and [bad](javascript:alert(1))");
    expect(link).toContain('href="https://example.org"');
    expect(link).toContain('rel="noopener noreferrer"');
    expect(link).not.toContain("javascript:alert(1)\"");
    expect(link).not.toContain("<a href=\"javascript");
  });

  it("does not stop on text that looks like the start of a mark", () => {
    expect(html("2 * 3 = 6 and a | b and *")).toContain("2 * 3 = 6 and a | b and *");
  });
});
