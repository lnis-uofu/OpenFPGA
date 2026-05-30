#!/usr/bin/env python3
"""
Python equivalent of the CMake add_custom_command that patches the ABC Makefile
for the long linker command fix.

CMake context resolved:
  MAKEFILE_INSERT_LINE = "@echo $(OBJ) > abc_objs.rsp"
  ABC_MAKEFILE_PATCH_FPATH = "Makefile.patched" (sentinel, same dir as Makefile)

Patches applied:
  1. Find the two-line anchor block:
         $(PROG): $(OBJ)
         \t@echo "$(MSG_PREFIX)`...
     and insert "\t@echo $(OBJ) > abc_objs.rsp" immediately after it.
  2. On the next line after the insertion, replace '$^' with '@abc_objs.rsp'.
  3. Touch Makefile.patched as a sentinel so CMake knows the patch was applied.

Usage:
  python3 patch_abc_makefile.py <path/to/Makefile>
"""

import sys
import pathlib


# ---------------------------------------------------------------------------
# CMake variable values (resolved from CMake escape sequences)
# ---------------------------------------------------------------------------
MAKEFILE_INSERT_LINE = "@echo $(OBJ) > abc_objs.rsp"
ABC_MAKEFILE_PATCH_FPATH = "Makefile.patched"

# Two-line anchor after which the new line is inserted.
# Trailing whitespace is ignored when matching.
ANCHOR_LINES = (
    "$(PROG): $(OBJ)",
    '\t@echo "$(MSG_PREFIX)\`\` Building binary:" $(notdir $@)',
)


def find_anchor(lines: list[str]) -> int:
    """
    Return the 0-based index of the LAST line of the anchor block,
    or -1 if the anchor is not found.
    """
    for i in range(len(lines) - 1):
        if (
            lines[i].rstrip() == ANCHOR_LINES[0]
            and lines[i + 1].rstrip() == ANCHOR_LINES[1]
        ):
            return i + 1  # index of the second anchor line
    return -1


def apply_patch(makefile_path: str) -> None:
    makefile = pathlib.Path(makefile_path).resolve()

    if not makefile.exists():
        print(f"Error: Makefile not found: {makefile}", file=sys.stderr)
        sys.exit(1)

    if not makefile.is_file():
        print(f"Error: Path is not a file: {makefile}", file=sys.stderr)
        sys.exit(1)

    sentinel = makefile.parent / ABC_MAKEFILE_PATCH_FPATH
    lines = makefile.read_text().splitlines(keepends=True)

    # ------------------------------------------------------------------
    # Command 1: insert MAKEFILE_INSERT_LINE after the anchor block
    # ------------------------------------------------------------------
    anchor_idx = find_anchor(lines)
    if anchor_idx == -1:
        print(
            "Error: anchor block not found in Makefile:\n"
            f"  {ANCHOR_LINES[0]!r}\n"
            f"  {ANCHOR_LINES[1]!r}",
            file=sys.stderr,
        )
        sys.exit(1)

    insert_at = anchor_idx + 1  # insert immediately after the anchor
    lines.insert(insert_at, f"\t{MAKEFILE_INSERT_LINE}\n")
    print(f"Inserted '{MAKEFILE_INSERT_LINE}' after line {anchor_idx + 1} (1-based)")

    # ------------------------------------------------------------------
    # Command 2: replace '$^' with '@abc_objs.rsp' on the line that now
    # follows the newly inserted line (i.e. insert_at + 1, 0-based)
    # ------------------------------------------------------------------
    replace_idx = insert_at + 1
    if replace_idx >= len(lines):
        print(
            f"Error: no line exists after insertion point (index {replace_idx}).",
            file=sys.stderr,
        )
        sys.exit(1)

    original = lines[replace_idx]
    patched = original.replace("$^", "@abc_objs.rsp")
    if patched == original:
        print(
            f"Warning: '$^' not found on line {replace_idx + 1} (1-based); "
            "no substitution made.",
            file=sys.stderr,
        )
    lines[replace_idx] = patched

    makefile.write_text("".join(lines))
    print(f"Patched:  {makefile}")

    # ------------------------------------------------------------------
    # Command 3: touch sentinel so CMake sees OUTPUT as satisfied
    # ------------------------------------------------------------------
    sentinel.touch()
    print(f"Sentinel: {sentinel}")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(
            f"Usage: {sys.argv[0]} <path/to/Makefile>\n"
            f"  e.g. {sys.argv[0]} path/to/yosys/abc/Makefile",
            file=sys.stderr,
        )
        sys.exit(1)

    apply_patch(sys.argv[1])
