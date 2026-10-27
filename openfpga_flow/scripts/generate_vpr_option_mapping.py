#!/usr/bin/env python3
"""Generate the table of the application options that are options of VPR.

VPR reads its options with read_options(), which also infers defaults from the
options that were specified. To keep that behavior, OpenFPGA gives the options
of the application to read_options() as a command line, as if they were typed
by a user. This script reads the declaration of the options of VPR and writes,
for each of them: its name, group, type, help, the flag of VPR, the values a
selection accepts (with the enumeration of VPR they stand for) and a function
that reads its value from VPR's t_options.

It reads:
  vtr-verilog-to-routing/vpr/src/base/read_options.h    (fields and their types)
  vtr-verilog-to-routing/vpr/src/base/read_options.cpp  (flags, help, parsers)
and writes:
  openfpga/src/base/vpr_option_table.cpp

The default values are not written: they are read from VPR when the options
are created, so that they are always the ones of VPR.

Run it again whenever VTR is updated. It fails when a field of VPR has neither
an option nor a reason to be left out below, so that a new option of VPR is
never ignored by accident. The unit test test_vpr_app_options compares the
generated file with the sources of VPR.
"""

import argparse
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
READ_OPTIONS_H = REPO / "vtr-verilog-to-routing/vpr/src/base/read_options.h"
READ_OPTIONS_CPP = REPO / "vtr-verilog-to-routing/vpr/src/base/read_options.cpp"
OUTPUT_CPP = REPO / "openfpga/src/base/vpr_option_table.cpp"

# The group of options of VPR, and the prefix of the names of its options
GROUP_PREFIX = {
    "pos_grp": "filename",
    "file_grp": "filename",
    "gen_grp": "general",
    "netlist_grp": "atom",
    "pack_grp": "cluster",
    "ap_grp": "aplace",
    "place_grp": "place",
    "place_timing_grp": "tplace",
    "route_grp": "router",
    "route_timing_grp": "trouter",
    "analysis_grp": "analysis",
    "power_grp": "power",
    "crr_grp": "crr",
}

# Groups of VPR left out entirely: (category, reason)
SKIPPED_GROUPS = {
    "stage_grp": ("stage", "Stage switch: the stage commands choose what is run"),
    "gfx_grp": ("graphics", "Graphics are not used by the commands"),
    "noc_grp": ("noc", "NoC is not supported"),
    "server_grp": ("run_control", "Server mode: not applicable to commands"),
}

# Fields of VPR left out one by one: category, reason, fields
SKIPPED_FIELDS = [
    (
        "stage",
        "Stage switch: the stage commands choose what is run",
        ["do_power"],
    ),
    (
        "run_control",
        "Run control of the VPR program: not applicable to commands",
        [
            "show_help",
            "show_version",
            "strict_checks",
            "exit_before_pack",
            "show_arch_resources",
            "verify_file_digests",
            "verify_route_file_switch_id",
            "echo_file",
        ],
    ),
    (
        "verbosity",
        "Verbosity belongs to the command that runs the stage",
        [
            "pack_verbosity",
            "route_verbosity",
            "netlist_verbosity",
            "ap_verbosity",
            "flat_place_verbosity",
        ],
    ),
    (
        "format",
        "Inferred from the extension of the file",
        ["arch_format", "circuit_format"],
    ),
]

# Files that VPR names after the circuit when they are not given, and the
# positional arguments (the architecture and the circuit, which VPR does not
# default). They stay empty in the table
DERIVED_FILES = {
    "ArchFile",
    "CircuitName",
    "CircuitFile",
    "SDCFile",
    "NetFile",
    "PlaceFile",
    "RouteFile",
    "FlatPlaceFile",
    "ActFile",
}


def snake_case(field):
    """Name of a field in the table: lower case, CamelCase becomes snake_case"""
    if "_" in field or field == field.lower():
        return field.lower()
    spaced = re.sub(r"(?<=[a-z0-9])(?=[A-Z])|(?<=[A-Z])(?=[A-Z][a-z])", "_", field)
    return spaced.lower()


def read_fields(text):
    """The fields of t_options and their types: ArgValue<type> name;"""
    return {name: ftype for ftype, name in re.findall(r"ArgValue<(.+?)>\s+(\w+);", text)}


def statement_at(text, start):
    """The text of a statement, from start to the ';' that ends it"""
    depth = 0
    pos = start
    in_string = False
    while True:
        char = text[pos]
        if in_string:
            if char == "\\":
                pos += 1
            elif char == '"':
                in_string = False
        elif char == '"':
            in_string = True
        elif char in "([{":
            depth += 1
        elif char in ")]}":
            depth -= 1
        elif char == ";" and depth == 0:
            return text[start:pos]
        pos += 1


def call_arguments(statement, name):
    """The text between the parentheses of the first call of .name( ... )"""
    marker = "." + name + "("
    begin = statement.find(marker)
    if begin < 0:
        return None
    pos = begin + len(marker)
    start = pos
    depth = 1
    in_string = False
    while depth:
        char = statement[pos]
        if in_string:
            if char == "\\":
                pos += 1
            elif char == '"':
                in_string = False
        elif char == '"':
            in_string = True
        elif char == "(":
            depth += 1
        elif char == ")":
            depth -= 1
        pos += 1
    return statement[start : pos - 1]


def only_string_literals(text):
    """True if the text is made of string literals only (and white space)"""
    return bool(text) and re.fullmatch(r'(?:\s*"(?:[^"\\]|\\.)*")+\s*', text) is not None


def read_declarations(text):
    """Every option declared in read_options.cpp, in order"""
    declarations = []
    pattern = re.compile(r"\b(\w*grp)\.add_argument(?:<([^()]*?)>)?\(\s*args\.(\w+)\s*,")
    for match in pattern.finditer(text):
        group, template, field = match.group(1), match.group(2) or "", match.group(3)
        statement = statement_at(text, match.start())
        first = statement[match.end() - match.start() :].split(")")[0]
        names = re.findall(r'"([^"]+)"', first)
        long_flags = [n for n in names if n.startswith("--")]
        flag = long_flags[0] if long_flags else names[0]
        parser = template.split(",")[1].strip() if "," in template else ""
        help_text = call_arguments(statement, "help")
        declarations.append(
            {
                "group": group,
                "field": field,
                "flag": flag,
                "parser": parser,
                "store_true": "Action::STORE_TRUE" in statement,
                "multi_value": bool(re.search(r"\.nargs\('\+'\)", statement)),
                "help": help_text.strip()
                if help_text and only_string_literals(help_text)
                else '""',
            }
        )
    return declarations


def read_parsers(text):
    """The values that each parser of an enumeration accepts:
    {parser: [(key, enumerator)]}"""
    parsers = {}
    for match in re.finditer(r"^struct (\w+) \{", text, re.M):
        start = match.end()
        depth = 1
        pos = start
        while depth:
            if text[pos] == "{":
                depth += 1
            elif text[pos] == "}":
                depth -= 1
            pos += 1
        body = text[start:pos]
        if "from_str" not in body:
            continue
        from_str = body[body.find("from_str") :]
        from_str = from_str[: from_str.find("to_str")] if "to_str" in from_str else from_str
        pairs = re.findall(
            r'str == "([^"]*)"\)\s*\{?\s*conv_value\.set_value\(([\w:]+)\)', from_str
        )
        if pairs:
            parsers[match.group(1)] = pairs
    return parsers


def getter(field, vpr_type):
    """C++ statement that copies a field of t_options into an AppOptionValue"""
    access = f"options.{field}.value()"
    if vpr_type == "bool":
        return f"out.bool_value = {access};"
    if vpr_type in ("int", "size_t"):
        return f"out.int_value = static_cast<int>({access});"
    if vpr_type in ("float", "double"):
        return f"out.float_value = {access};"
    if vpr_type == "std::string":
        return f"out.string_value = {access};"
    if vpr_type.startswith("std::vector<"):
        return f"out.string_value = join_values({access});"
    return f"out.int_value = static_cast<int>({access});"  # an enumeration


def kind_of(vpr_type, parser, parsers):
    # on/off values have a parser as well, but they are booleans
    if vpr_type == "bool":
        return "BOOLEAN"
    if parser in parsers:
        return "SELECTION"
    if vpr_type in ("int", "size_t"):
        return "INT"
    if vpr_type in ("float", "double"):
        return "FLOAT"
    if vpr_type == "std::string" or vpr_type.startswith("std::vector<"):
        return "STRING"
    return None


def main():
    cli = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    cli.add_argument(
        "--check",
        action="store_true",
        help="fail if the generated file is not up to date",
    )
    args = cli.parse_args()

    header_text = READ_OPTIONS_H.read_text()
    source_text = READ_OPTIONS_CPP.read_text()
    fields = read_fields(header_text)
    declarations = read_declarations(source_text)
    parsers = read_parsers(source_text)
    by_field = {d["field"]: d for d in declarations}

    skipped = {}
    for group, (category, reason) in SKIPPED_GROUPS.items():
        for d in declarations:
            if d["group"] == group:
                skipped[d["field"]] = (category, reason)
    for category, reason, names in SKIPPED_FIELDS:
        for name in names:
            if name in skipped:
                sys.exit(f"{name} is skipped twice")
            skipped[name] = (category, reason)

    problems = []
    # Fields with no flag: VPR infers them
    for field in sorted(fields):
        if field not in by_field:
            skipped[field] = ("inferred", "Has no flag: VPR infers it from other options")
    # Every field has to be in a group that is known
    options = []
    names_seen = {}
    for d in declarations:
        field = d["field"]
        if field in skipped:
            continue
        if d["group"] not in GROUP_PREFIX:
            problems.append(f"{field}: the group {d['group']} has no prefix")
            continue
        if field not in fields:
            problems.append(f"{field} is declared with a flag but is not a field of t_options")
            continue
        vpr_type = fields[field]
        kind = kind_of(vpr_type, d["parser"], parsers)
        if kind is None:
            problems.append(f"{field}: the type {vpr_type} is not supported")
            continue
        name = snake_case(field)
        full = f"{GROUP_PREFIX[d['group']]}.{name}"
        if full in names_seen:
            problems.append(f"{full} is the name of {field} and of {names_seen[full]}")
            continue
        names_seen[full] = field
        options.append(
            dict(d, kind=kind, type=vpr_type, name=name, prefix=GROUP_PREFIX[d["group"]])
        )
    for name in skipped:
        if name not in fields:
            problems.append(f"{name} is left out, but is not a field of t_options any more")
    if problems:
        print("\n".join(problems), file=sys.stderr)
        sys.exit(f"{len(problems)} problem(s)")

    # ---- write the table
    choice_functions = {}  # parser -> function name
    for option in options:
        if option["kind"] == "SELECTION" and option["parser"] not in choice_functions:
            choice_functions[option["parser"]] = "choices_" + re.sub(
                r"^Parse", "", option["parser"]
            )

    out = [
        "// clang-format off\n"
        "/* Generated by openfpga_flow/scripts/generate_vpr_option_mapping.py. "
        "Do not edit. */\n",
        '#include "vpr_option_table.h"\n\n#include <sstream>\n\n',
        "/* Begin namespace openfpga */\nnamespace openfpga {\n\nnamespace {\n\n",
        "/* The values of a list option, as the text of the application option */\n",
        "template <typename T>\n"
        "std::string join_values(const std::vector<T>& values) {\n"
        "  std::ostringstream text;\n"
        "  for (size_t i = 0; i < values.size(); ++i) {\n"
        '    text << (0 == i ? "" : " ") << values[i];\n'
        "  }\n"
        "  return text.str();\n"
        "}\n\n",
    ]
    for parser, function in sorted(choice_functions.items(), key=lambda kv: kv[1]):
        out.append(
            f"/* The values that {parser} accepts */\n"
            f"const std::map<std::string, int>* {function}() {{\n"
            "  static const std::map<std::string, int> choices = {\n"
        )
        for key, enumerator in parsers[parser]:
            out.append(f'    {{"{key}", static_cast<int>({enumerator})}},\n')
        out.append("  };\n  return &choices;\n}\n\n")
    out.append("}  // End anonymous namespace\n\n")

    out.append("const std::vector<VprOptionSpec>& vpr_option_specs() {\n")
    out.append("  static const std::vector<VprOptionSpec> specs = {\n")
    for o in options:
        derived = "true" if o["field"] in DERIVED_FILES else "false"
        choices = choice_functions[o["parser"]] + "()" if o["kind"] == "SELECTION" else "nullptr"
        out.append(
            "    {"
            f'"{o["prefix"]}", "{o["name"]}", AppOptionValue::{o["kind"]}, "{o["field"]}", '
            f'"{o["flag"]}", {str(o["store_true"]).lower()}, {str(o["multi_value"]).lower()}, '
            f"{derived},\n     {o['help']},\n     {choices},\n"
            f"     [](const t_options& options, AppOptionValue& out) {{ {getter(o['field'], o['type'])} }}}},\n"
        )
    out.append("  };\n  return specs;\n}\n\n")

    out.append("const std::vector<VprFieldNotInTable>& vpr_fields_not_in_table() {\n")
    out.append("  static const std::vector<VprFieldNotInTable> fields = {\n")
    for field in sorted(skipped):
        category, reason = skipped[field]
        flag = by_field[field]["flag"] if field in by_field else ""
        out.append(f'    {{"{field}", "{flag}", "{category}", "{reason}"}},\n')
    out.append("  };\n  return fields;\n}\n\n} /* End namespace openfpga */\n")
    out.append("// clang-format on\n")
    text = "".join(out)

    if args.check:
        if not OUTPUT_CPP.exists() or OUTPUT_CPP.read_text() != text:
            sys.exit("not up to date, run the script again: " + str(OUTPUT_CPP.relative_to(REPO)))
        return
    OUTPUT_CPP.write_text(text)
    print(
        f"{len(options)} options, {len(skipped)} fields of t_options left out, "
        f"{len(choice_functions)} sets of values for selections"
    )


if __name__ == "__main__":
    main()
