
#!/usr/bin/env python3

import argparse
import sys
from pathlib import Path

import yaml


def load_replacements(yaml_file: Path) -> dict[str, str]:
    """Load string replacements from a YAML file."""
    with yaml_file.open("r", encoding="utf-8") as f:
        replacements = yaml.safe_load(f) or {}

    if not isinstance(replacements, dict):
        raise ValueError("The YAML file must contain a key-value mapping.")

    for key, value in replacements.items():
        if not isinstance(key, str) or not isinstance(value, str):
            raise ValueError(
                f"Replacement keys and values must be strings: "
                f"{key!r}: {value!r}"
            )

    # Replace longer strings first to avoid shorter keys interfering
    # with longer keys.
    return dict(
        sorted(replacements.items(), key=lambda item: len(item[0]), reverse=True)
    )


def process_file(
    file_path: Path,
    replacements: dict[str, str],
    dry_run: bool = False,
) -> bool:
    """Replace matching strings in one file. Return True if changed."""
    original = file_path.read_text(encoding="utf-8")
    updated = original

    for old, new in replacements.items():
        updated = updated.replace(old, new)

    if updated == original:
        return False

    if dry_run:
        print(f"[DRY RUN] Would update: {file_path}")
    else:
        file_path.write_text(updated, encoding="utf-8")
        print(f"[UPDATED] {file_path}")

    return True


def main():
    parser = argparse.ArgumentParser(
        description="Recursively replace strings in all task.conf files."
    )
    parser.add_argument(
        "root",
        type=Path,
        help="Root directory to search",
    )
    parser.add_argument(
        "yaml_file",
        type=Path,
        help="YAML file containing string replacements",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Report files that would change without modifying them",
    )
    args = parser.parse_args()

    if not args.root.is_dir():
        parser.error(f"Root directory does not exist: {args.root}")

    if not args.yaml_file.is_file():
        parser.error(f"YAML file does not exist: {args.yaml_file}")

    try:
        replacements = load_replacements(args.yaml_file)

        if not replacements:
            print("No replacements defined.")
            return

        files = list(args.root.rglob("task.conf"))
        changed_count = 0

        for file_path in files:
            try:
                if process_file(file_path, replacements, args.dry_run):
                    changed_count += 1
            except (OSError, UnicodeError) as exc:
                print(f"[ERROR] {file_path}: {exc}", file=sys.stderr)

        action = "would be updated" if args.dry_run else "updated"
        print(
            f"\nFound {len(files)} task.conf files; "
            f"{changed_count} {action}."
        )

    except (OSError, yaml.YAMLError, ValueError) as exc:
        print(f"[ERROR] {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()

