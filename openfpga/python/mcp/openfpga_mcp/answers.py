"""The shape of every answer of a tool, see DESIGN.md.

{"ok": true,  "data": {...}, "truncated": false, "more": null}
{"ok": false, "diagnostics": [{"severity", "kind", "message", "file", "line", "hint", "data"}]}
"""

import dataclasses
import json
from typing import Any, Dict, List, Optional

from .registry import RegistryError


def ok(data: Any, truncated: bool = False, more: Optional[str] = None) -> Dict[str, Any]:
    return {"ok": True, "data": data, "truncated": truncated, "more": more}


def diagnostic(kind: str, message: str, hint: Optional[str] = None, **data) -> Dict[str, Any]:
    return {
        "severity": "error",
        "kind": kind,
        "message": message,
        "file": None,
        "line": None,
        "hint": hint,
        "data": data,
    }


def failed(diagnostics: List[Dict[str, Any]]) -> Dict[str, Any]:
    return {"ok": False, "diagnostics": diagnostics}


def from_exception(error: BaseException) -> Dict[str, Any]:
    """The answer for a failure: the diagnostics of a command or of a worker, or the kind of a
    failure of the registry. An error that is not known is an answer too, with the kind 'error'"""
    if isinstance(error, RegistryError):
        return failed([diagnostic(error.kind, str(error), error.hint)])
    found = getattr(error, "diagnostics", None)
    if found:
        plain = [_plain(d) for d in found]
    else:
        kind = getattr(error, "kind", None) or "error"
        plain = [diagnostic(kind, str(error), getattr(error, "hint", None))]
    for item in plain:
        if str(item.get("kind", "")).startswith("worker_"):
            # The tool that does it, not the method of the library
            item["hint"] = _RESTORE_HINT
    return failed(plain)


_RESTORE_HINT = (
    "session_restore starts another worker and brings the session back to the last stage "
    "that was finished"
)


def relative(node: Any, folders: List[str]) -> Any:
    """The data with the paths inside the workspace, and inside the designs folders, made
    relative to them: the model sees names that it can give back, and not where things are"""
    if isinstance(node, str):
        for folder in folders:
            prefix = folder.rstrip("/") + "/"
            if prefix in node:
                node = node.replace(prefix, "")
        return node
    if isinstance(node, list):
        return [relative(item, folders) for item in node]
    if isinstance(node, dict):
        return {key: relative(item, folders) for key, item in node.items()}
    return node


def _plain(item) -> Dict[str, Any]:
    plain = dataclasses.asdict(item) if dataclasses.is_dataclass(item) else dict(item)
    plain.pop("source", None)  # where in VPR it was raised: for a bug report, not for the model
    return plain


def text(answer: Dict[str, Any]) -> str:
    """The answer as the text that a tool returns: compact, so that its size is the size that
    the model reads"""
    return json.dumps(answer, separators=(",", ":"), ensure_ascii=False)
