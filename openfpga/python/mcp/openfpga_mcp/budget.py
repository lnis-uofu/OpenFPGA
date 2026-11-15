"""The size of an answer: when it is too long, the longest list is cut"""

import copy
from typing import Any, Dict, List, Optional, Tuple

from . import answers

Path = Tuple[Any, ...]


def _lists(node: Any, path: Path = ()):
    """Every list of the data, with where it is"""
    if isinstance(node, list):
        yield path, node
        for index, item in enumerate(node):
            yield from _lists(item, path + (index,))
    elif isinstance(node, dict):
        for key, item in node.items():
            yield from _lists(item, path + (key,))


def _where(path: Path) -> str:
    """Where a list is, for the model: data.long, data.paths[0].points"""
    text = ""
    for part in path:
        text += "[%d]" % part if isinstance(part, int) else ("." if text else "") + str(part)
    return text


def path_ok(path):
    return len(path) > 0


def truncated_by_query(node: Any) -> bool:
    """A query that has a limit says when it cut: a 'truncated' that is true, anywhere"""
    if isinstance(node, dict):
        return node.get("truncated") is True or any(truncated_by_query(v) for v in node.values())
    if isinstance(node, list):
        return any(truncated_by_query(v) for v in node)
    return False


# What the note of a cut answer may take of the size: the answer is cut to fit without it
_NOTE_CHARS = 400


def fit(
    data: Any, limit: int, more: Optional[str] = None, from_start: bool = False
) -> Dict[str, Any]:
    """The answer for data, of at most limit characters. The longest list is cut from its end
    (from its start with from_start), until it fits; the answer says that it was cut, and where.
    The data that a query cut itself make a truncated answer too"""
    answer = answers.ok(copy.deepcopy(data))
    size = len(answers.text(answer))
    by_query = truncated_by_query(data)
    if size <= limit and not by_query:
        return answer
    room = max(limit - _NOTE_CHARS, limit // 2)
    cuts: Dict[Path, List[int]] = {}
    # An answer that fits is not cut: it only says that the query cut it
    target = room if size > limit else size
    while size > target:
        candidates = [
            (len(answers.text(items)), path, items)
            for path, items in _lists(answer["data"])
            if len(items) > 0
        ]
        if not candidates:
            return answers.failed(
                [
                    answers.diagnostic(
                        "answer_too_large",
                        "The answer is %d characters and has no list to cut: the limit is %d"
                        % (size, limit),
                        "Ask for something smaller",
                    )
                ]
            )
        _, path, items = max(candidates, key=lambda c: c[0])
        # Keep what fits, by proportion, and always at least one item less
        before = len(items)
        keep = min(before - 1, int(before * target / size))
        if from_start:
            del items[: before - keep]
        else:
            del items[keep:]
        # A list that is cut again says its first length and its last
        cuts.setdefault(path, [before, keep])[1] = keep
        size = len(answers.text(answer))
    notes = []
    if cuts:
        said = [
            "%s to %d of %d items" % ("data." + _where(p) if path_ok(p) else "data", k, b)
            for p, (b, k) in cuts.items()
        ]
        notes.append("The answer was cut to fit its size: " + "; ".join(said[:3]) + ".")
    if more:
        notes.append(more)
    answer["truncated"] = True
    answer["more"] = " ".join(notes) or None
    return answer
