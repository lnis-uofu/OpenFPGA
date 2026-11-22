"""The HTML report of a stage: written in the folder 'reports' of the session after each stage runs

A report is the template templates/stage_report.html with the facts of the stage put in: a summary
(the stage, its status and time, the design and the device), the figures of the stage (the
packing, the placement, the routing, the timing, the device or the synthesis), the errors and the
warnings that its log has, and the end of the log. Every value is escaped: a name of a net or a
line of a log is text, never HTML.
"""

import datetime
import html
import os
import re
from pathlib import Path
from string import Template
from typing import Any, Iterable, List, Optional, Sequence, Tuple

FOLDER = "reports"
_TEMPLATE = Path(__file__).with_name("templates") / "stage_report.html"
LOG_TAIL = 80  # lines of the log at the end of a report
MAX_MESSAGES = 200  # different warnings (or errors) that a report lists
MAX_ROWS = 200  # rows of a table

# The stages, as the file and the title of their report
TITLES = {
    "synthesis": "Synthesis",
    "device": "Device",
    "pack": "Packing",
    "place": "Placement",
    "route": "Routing",
    "analysis": "Timing analysis",
}

# A warning or an error of VPR ("Warning 12: ..."), of Yosys ("Warning: ...", "ERROR: ..."), or of
# OpenFPGA ("[Warning] ...", "Error ..."): the start of a line, in any case
_MESSAGE = re.compile(r"^\s*(?:\[\s*)?(warning|error)\b(?:\s*\d+)?\s*\]?\s*[:.-]?\s*(.*)$", re.I)
_NOT_A_MESSAGE = re.compile(r"^\s*(?:warning|error)s?\s*(?:count|counts|summary)?\s*[:=]?\s*\d*\s*$", re.I)


def messages(log: Optional[str]) -> Tuple[List[Tuple[str, int]], List[Tuple[str, int]]]:
    """The warnings and the errors of a log, each once with how many times it was said, in the
    order of their first line. A line that only counts them ("Warnings: 3") is not one"""
    found = {"warning": {}, "error": {}}
    for line in (log or "").splitlines():
        match = _MESSAGE.match(line)
        if not match or _NOT_A_MESSAGE.match(line):
            continue
        kind = match.group(1).lower()
        text = (match.group(2) or line).strip()[:400] or line.strip()[:400]
        seen = found[kind]
        if text in seen:
            seen[text] += 1
        elif len(seen) < MAX_MESSAGES:
            seen[text] = 1
    return list(found["warning"].items()), list(found["error"].items())


def _e(value: Any) -> str:
    return html.escape("" if value is None else str(value), quote=True)


def number(value: Any, unit: str = "", digits: int = 3) -> str:
    """A figure as it is shown: '–' for none, at most `digits` decimals for a float"""
    if value is None:
        return "–"
    if isinstance(value, bool):
        return "yes" if value else "no"
    if isinstance(value, float):
        text = ("%." + str(digits) + "f") % value
        text = text.rstrip("0").rstrip(".") if "." in text else text
    elif isinstance(value, int):
        text = "{:,}".format(value)
    else:
        text = str(value)
    return text + (" " + unit if unit else "")


class Section:
    """A part of the figures of a stage: a title, key and value rows, and tables"""

    def __init__(self, title: str):
        self.title = title
        self.rows: List[Tuple[str, str]] = []
        self.tables: List[Tuple[str, Sequence[str], List[Sequence[Any]]]] = []

    def row(self, key: str, value: Any, unit: str = "") -> "Section":
        self.rows.append((key, number(value, unit)))
        return self

    def table(self, title: str, columns: Sequence[str], rows: Iterable[Sequence[Any]]) -> "Section":
        self.tables.append((title, columns, list(rows)[:MAX_ROWS]))
        return self

    def html(self) -> str:
        parts = ["<h2>%s</h2>" % _e(self.title)]
        if self.rows:
            parts.append('<table class="keys">')
            parts.extend("<tr><th>%s</th><td>%s</td></tr>" % (_e(k), _e(v)) for k, v in self.rows)
            parts.append("</table>")
        for title, columns, rows in self.tables:
            if title:
                parts.append("<h3>%s</h3>" % _e(title))
            if not rows:
                parts.append('<p class="none">(none)</p>')
                continue
            numeric = [all(isinstance(r[i], (int, float)) or r[i] is None for r in rows) for i in range(len(columns))]
            parts.append("<table><tr>%s</tr>" % "".join(
                '<th%s>%s</th>' % (' class="num"' if numeric[i] else "", _e(c)) for i, c in enumerate(columns)))
            for r in rows:
                parts.append("<tr>%s</tr>" % "".join(
                    '<td%s>%s</td>' % (' class="num"' if numeric[i] else "", _e(number(v))) for i, v in enumerate(r)))
            parts.append("</table>")
        return "\n".join(parts)


def _message_list(items: List[Tuple[str, int]], kind: str) -> str:
    if not items:
        return '<p class="none">(none)</p>'
    rows = [
        "<li>%s%s</li>" % (_e(text), '<span class="count">× %d</span>' % count if count > 1 else "")
        for text, count in items
    ]
    return '<ul class="messages %s">\n%s\n</ul>' % (kind, "\n".join(rows))


def render(
    stage: str,
    status: str,
    session: str,
    summary: Sequence[Tuple[str, Any]],
    sections: Sequence[Section] = (),
    cards: Sequence[Tuple[str, Any]] = (),
    log: Optional[str] = None,
    errors: Sequence[str] = (),
    now: Optional[datetime.datetime] = None,
) -> str:
    """The HTML of the report of a stage. status is 'passed' or 'failed'; errors are said first,
    before the errors of the log (the reason of a failure, for example)"""
    warnings, logged = messages(log)
    error_items = [(e, 1) for e in errors if e] + [m for m in logged if m[0] not in errors]
    shown = "failed" if status == "failed" else ("passed with warnings" if warnings else "passed")
    status_class = "failed" if status == "failed" else ("warnings" if warnings else "passed")
    all_cards = list(cards) + [("Errors", len(error_items)), ("Warnings", sum(c for _t, c in warnings))]
    card_html = []
    for label, value in all_cards:
        kind = ""
        if label == "Errors" and value:
            kind = " bad"
        elif label == "Warnings" and value:
            kind = " warn"
        card_html.append(
            '<div class="card%s"><div class="label">%s</div><div class="value">%s</div></div>'
            % (kind, _e(label), _e(number(value) if not isinstance(value, str) else value))
        )
    tail = [line[:400] for line in (log or "").splitlines()[-LOG_TAIL:]]
    title = TITLES.get(stage, stage)
    values = {
        "title": _e("%s report · %s" % (title, session)),
        "heading": _e("%s report" % title),
        "status": _e(shown),
        "status_class": status_class,
        "generated": _e((now or datetime.datetime.now()).strftime("%Y-%m-%d %H:%M:%S")),
        "cards": "\n".join(card_html),
        "summary": "\n".join(
            "<tr><th>%s</th><td>%s</td></tr>" % (_e(k), _e(v if isinstance(v, str) else number(v)))
            for k, v in summary
        ),
        "details": "\n\n".join(s.html() for s in sections),
        "error_count": len(error_items),
        "errors": _message_list(error_items, "errors"),
        "warning_count": sum(c for _t, c in warnings),
        "warnings": _message_list(warnings, "warnings"),
        "log_lines": len(tail),
        "log": _e("\n".join(tail)) or "(no log)",
        "session": _e(session),
        "stage": _e(stage),
    }
    return Template(_TEMPLATE.read_text(encoding="utf-8")).substitute(values)


def write(session_folder: Path, stage: str, text: str) -> Path:
    """Write the report of a stage as reports/<stage>.html of the session, in place of the one of
    an earlier run (a half written report is never seen under its name)"""
    folder = session_folder / FOLDER
    folder.mkdir(parents=True, exist_ok=True)
    target = folder / ("%s.html" % stage)
    temporary = folder / (".%s.html.part" % stage)
    temporary.write_text(text, encoding="utf-8")
    os.replace(temporary, target)
    return target
