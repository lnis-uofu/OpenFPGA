#!/usr/bin/env python3
"""Generate an OpenFPGA app options header from VPR's t_options struct."""

from __future__ import annotations

import argparse
import re
from dataclasses import dataclass
from pathlib import Path
from typing import List, Optional


ignore_app_options = [
    "arch_format",
    "circuit_format",
    # All these options are set to false by default and
    # inferred based on shell commands
    "do_packing",
    "do_legalize",
    "do_placement",
    "do_analytical_placement",
    "do_routing",
    "do_analysis",
    "do_power"
    # Graphic options are not relevant for OpenFPGA tcl flow,
    # and can be ignored to reduce the number of options we need to maintain.
    "show_graphics",
    "GraphPause",
    "save_graphics",
    "graphics_commands"

    # Not applicable for TCL flow
    "show_help",
    "show_version",
    "show_arch_resources",
    "create_echo_file",
    "verify_file_digests",
    "verify_route_file_switch_id",
    "exit_before_pack",
    "strict_checks"
]

@dataclass
class CxxField:
    cpp_type: str
    name: str
    category: Optional[str] = None

    @property
    def option_name(self) -> str:
        return camel_to_snake(self.name)

    @property
    def kind(self) -> str:
        return infer_option_kind(self.cpp_type)

    @property
    def default_value(self) -> str:
        return {
            'string': '""',
            'int': '0',
            'float': '0.0',
            'boolean': 'false',
        }.get(self.kind, '""')

    @property
    def help_message(self) -> str:
        return format_help_message(self.name)


@dataclass
class CxxStruct:
    name: str
    fields: List[CxxField]


def camel_to_snake(name: str) -> str:
    s1 = re.sub(r'(.)([A-Z][a-z]+)', r'\1_\2', name)
    snake = re.sub(r'([a-z0-9])([A-Z])', r'\1_\2', s1).lower()
    return snake


def split_camel_case(name: str) -> List[str]:
    tokens = re.findall(r'[A-Z]+(?=[A-Z][a-z]|$)|[A-Z][a-z]*|[a-z]+|\d+', name)
    return [tok for tok in tokens if tok]


WORD_MAP = {
    'Arch': 'architecture',
    'Cmos': 'CMOS',
    'SDC': 'SDC',
    'VPR': 'VPR',
    'FPGA': 'FPGA',
    'CLB': 'CLB',
    'IO': 'IO',
    'Net': 'net',
    'File': 'file',
    'Name': 'name',
    'Route': 'route',
    'Chan': 'channel',
    'Graph': 'graph',
    'Type': 'type',
    'Bool': 'boolean',
    'Tech': 'technology',
    'Act': 'activity',
    'Power': 'power',
    'Current': 'current',
    'Enable': 'enable',
    'Disable': 'disable',
}


def format_help_message(name: str) -> str:
    tokens = split_camel_case(name)
    words = []
    for token in tokens:
        if token in WORD_MAP:
            words.append(WORD_MAP[token])
        elif token.isupper() and len(token) > 1:
            words.append(token)
        else:
            words.append(token.lower())
    help_text = ' '.join(words)
    return help_text[:1].upper() + help_text[1:]


def infer_option_kind(cpp_type: str) -> str:
    normalized = cpp_type.replace(' ', '')
    inner = cpp_type
    m = re.search(r'ArgValue\s*<\s*([^>]+)\s*>', normalized)
    if m:
        inner = m.group(1)

    if 'bool' in inner.lower():
        return 'boolean'
    if 'float' in inner.lower() or 'double' in inner.lower():
        return 'float'
    if 'size_t' in inner or 'int' in inner.lower() or 'long' in inner.lower() or 'short' in inner.lower():
        return 'int'
    if 'string' in inner.lower() or 'char' in inner.lower():
        return 'string'
    return inner.lower()


def strip_comments(code: str) -> str:
    code = re.sub(r'//.*', '', code)
    code = re.sub(r'/\*.*?\*/', '', code, flags=re.S)
    return code


def normalize_block_comments(code: str) -> str:
    def replacement(match: re.Match[str]) -> str:
        comment_text = match.group(0)[2:-2]
        lines = [line.strip() for line in comment_text.splitlines()]
        normalized = []
        for line in lines:
            if line:
                normalized.append(f'// {line}')
            else:
                normalized.append('//')
        return '\n'.join(normalized)

    return re.sub(r'/\*.*?\*/', replacement, code, flags=re.S)


def extract_struct(code: str, struct_name: str) -> Optional[CxxStruct]:
    clean_code = strip_comments(code)
    pattern = rf'\bstruct\s+{re.escape(struct_name)}\s*\{{'
    match = re.search(pattern, clean_code)
    if not match:
        return None

    start = clean_code.index('{', match.end() - 1)
    depth = 0
    end = None
    for idx in range(start, len(clean_code)):
        char = clean_code[idx]
        if char == '{':
            depth += 1
        elif char == '}':
            depth -= 1
            if depth == 0:
                end = idx
                break
    if end is None:
        return None

    body = code[start + 1 : end]
    body = normalize_block_comments(body)

    fields: List[CxxField] = []
    current_category: Optional[str] = None
    category_buffer: List[str] = []
    for raw_line in body.splitlines():
        line = raw_line.strip()
        if not line:
            if category_buffer:
                current_category = ' '.join(category_buffer).strip()
                category_buffer = []
            continue

        if line.startswith('//'):
            comment_text = line[2:].strip()
            if comment_text:
                category_buffer.append(comment_text)
            continue

        if ';' not in line:
            continue

        declaration = line.split(';', 1)[0].strip()
        declaration = declaration.split('//', 1)[0].strip()
        if not declaration:
            continue

        m = re.match(r'(?P<type>.+?)\s+(?P<name>\w+)$', declaration)
        if not m:
            continue

        cpp_type = m.group('type').strip()
        name = m.group('name').strip()

        if category_buffer:
            current_category = ' '.join(category_buffer).strip()
            category_buffer = []

        fields.append(
            CxxField(
                cpp_type=cpp_type,
                name=name,
                category=current_category,
            )
        )

    return CxxStruct(name=struct_name, fields=fields)


def generate_header(struct_decl: CxxStruct, source_path: Path) -> str:
    lines = [
        '// Generated by collect_vpr_options.py from',
        f'// {source_path.name}',
        '#pragma once',
        '',
        '#include <array>',
        '#include <map>',
        '#include <string>',
        '#include <string_view>',
        '#include <type_traits>',
        '',
        '#include "app_option_selection_map.h"',
        '#include "vpr_types.h"',
        '',
        'template <typename T>',
        'using app_option_storage_t =',
        '  std::conditional_t<std::is_convertible_v<std::decay_t<T>, const char*>,',
        '                     std::string, std::decay_t<T>>;',
        '',
        'struct AppOptionValue {',
        '  enum e_type {',
        '    EMPTY,',
        '    STRING,',
        '    INT,',
        '    FLOAT,',
        '    BOOLEAN,',
        '    SELECTION,',
        '  };',
        '',
        '  e_type type = EMPTY;',
        '  std::string help_message;',
        '  std::string string_value;',
        '  std::map<std::string, int> selection_values;',
        '  int int_value = 0;',
        '  float float_value = 0.f;',
        '  bool bool_value = false;',
        '',
        '  static AppOptionValue make_empty() {',
        '    AppOptionValue value;',
        '    value.type = EMPTY;',
        '    return value;',
        '  }',
        '',
        '  static AppOptionValue make_string(const std::string& string_value,',
        '                                    std::string help_message = "") {',
        '    AppOptionValue value;',
        '    value.type = STRING;',
        '    value.string_value = string_value;',
        '    value.help_message = help_message;',
        '    return value;',
        '  }',
        '',
        '  static AppOptionValue make_int(const int int_value,',
        '                                 std::string help_message = "") {',
        '    AppOptionValue value;',
        '    value.type = INT;',
        '    value.int_value = int_value;',
        '    value.help_message = help_message;',
        '    return value;',
        '  }',
        '',
        '  static AppOptionValue make_boolean(const bool bool_value,',
        '                                     std::string help_message = "") {',
        '    AppOptionValue value;',
        '    value.type = BOOLEAN;',
        '    value.bool_value = bool_value;',
        '    value.help_message = help_message;',
        '    return value;',
        '  }',
        '',
        '  static AppOptionValue make_float(const float float_value,',
        '                                   std::string help_message = "") {',
        '    AppOptionValue value;',
        '    value.type = FLOAT;',
        '    value.float_value = float_value;',
        '    value.help_message = help_message;',
        '    return value;',
        '  }',
        '',
        '  static AppOptionValue make_selection(',
        '    const std::string& selected_value,',
        '    const std::map<std::string, int>& selection_enums,',
        '    std::string help_message = "") {',
        '    AppOptionValue value;',
        '    value.type = SELECTION;',
        '    value.string_value = selected_value;',
        '    value.selection_values = selection_enums;',
        '    value.help_message = help_message;',
        '    const auto selected_it = value.selection_values.find(selected_value);',
        '    if (selected_it != value.selection_values.end()) {',
        '      value.int_value = selected_it->second;',
        '    }',
        '    return value;',
        '  }',
        '',
        '  int to_enum() const {',
        '    if (SELECTION != type) {',
        '      return 0;',
        '    }',
        '',
        '    const auto selected_it = selection_values.find(string_value);',
        '    if (selected_it == selection_values.end()) {',
        '      return 0;',
        '    }',
        '',
        '    return selected_it->second;',
        '  }',
        '',
        '  std::string to_string() const {',
        '    switch (type) {',
        '      case STRING:',
        '        return string_value;',
        '      case INT:',
        '        return std::to_string(int_value);',
        '      case BOOLEAN:',
        '        return bool_value ? std::string("true") : std::string("false");',
        '      case FLOAT:',
        '        return std::to_string(float_value);',
        '      case SELECTION:',
        '        return string_value;',
        '      default:',
        '        return std::string();',
        '    }',
        '  }',
        '',
        '  void update(const std::string& new_value) {',
        '    if (type == SELECTION) {',
        '      const auto it = selection_values.find(new_value);',
        '      if (it == selection_values.end()) {',
        '        return; /* invalid selection - caller should validate first */',
        '      }',
        '      string_value = new_value;',
        '      int_value = it->second;',
        '    } else {',
        '      string_value = new_value;',
        '    }',
        '  }',
        '',
        '  void update(const int new_value) { int_value = new_value; }',
        '  void update(const float new_value) { float_value = new_value; }',
        '  void update(const bool new_value) { bool_value = new_value; }',
        '};',
        '',
        '#define STRING_APP_OPTION(name, value, help_message) \\',
        '  AppOptionValue name = AppOptionValue::make_string(value, help_message);',

        '',
        '#define INT_APP_OPTION(name, value, help_message) \\',
        '  AppOptionValue name = AppOptionValue::make_int(value, help_message);',
        '',
        '#define FLOAT_APP_OPTION(name, value, help_message) \\',
        '  AppOptionValue name = AppOptionValue::make_float(value, help_message);',
        '',
        '#define BOOLEAN_APP_OPTION(name, value, help_message) \\',
        '  AppOptionValue name = AppOptionValue::make_boolean(value, help_message);',
        '',
        '#define SELECTION_APP_OPTION(name, value, selection_values, help_message) \\',
        '  AppOptionValue name =                                                   \\',
        '    AppOptionValue::make_selection(value, selection_values, help_message);',
        '',
        '#define IGNORE_APP_OPTION(name, value, help_message)',
        '',
    ]

    last_category: Optional[str] = None
    for field in struct_decl.fields:
        if field.category and field.category != last_category:
            lines.append('')
            lines.append(f'  /* {field.category} */')
            lines.append(f'#define DEFINE_{field.category.replace(" ", "_").upper()}_OPTIONS_FIELDS \\')
            last_category = field.category

        macro = {
            'string': 'STRING_APP_OPTION',
            'int': 'INT_APP_OPTION',
            'float': 'FLOAT_APP_OPTION',
            'boolean': 'BOOLEAN_APP_OPTION',
        }.get(field.kind, 'SELECTION_APP_OPTION')

        if field.option_name in ignore_app_options:
            macro = 'IGNORE_APP_OPTION'
        if macro == 'SELECTION_APP_OPTION':
            selection_map_name = f'{field.kind.upper()}_SELECTION_MAP'
            lines.append(
                f'  {macro}({field.option_name}, {field.default_value}, {selection_map_name}, "{field.help_message}") \\'
            )
        else:
            lines.append(
                f'  {macro}({field.option_name}, {field.default_value}, "{field.help_message}") \\'
            )

    for field in struct_decl.fields:
        if field.category and field.category != last_category:
            lines.append(f'')
            lines.append(f'/* {field.category} */')
            lines.append(f'struct {field.category.replace(" ", "_").lower()}_opts {{')
            lines.append(f'  DEFINE_{field.category.replace(" ", "_").upper()}_OPTIONS_FIELDS;')
            lines.append('};')
            last_category = field.category

    lines.extend([
        "",
        "#undef APP_OPTION",
        "#undef STRING_APP_OPTION",
        "#undef INT_APP_OPTION",
        "#undef FLOAT_APP_OPTION",
        "#undef BOOLEAN_APP_OPTION",
        "#undef SELECTION_APP_OPTION",
        "#undef IGNORE_APP_OPTION"
    ])

    # Concatenate lines and remove trailing backslash from the last line
    if lines and lines[-1].endswith(' \\'):
        lines[-1] = lines[-1][:-2]

    return '\n'.join(lines) + '\n'


def main() -> int:
    parser = argparse.ArgumentParser(
        description='Generate app_options.h from a VPR t_options struct declaration.'
    )
    parser.add_argument(
        '--input',
        type=Path,
        default=(Path(__file__).resolve().parents[2] / 'vtr-verilog-to-routing/vpr/src/base/read_options.h'),
        help='Path to the VPR read_options.h header.',
    )
    parser.add_argument(
        '--struct',
        default='t_options',
        help='Name of the struct to parse from the input header.',
    )
    parser.add_argument(
        '--output',
        type=Path,
        default=None,
        help='Path to write the generated app_options.h. If omitted, prints to stdout.',
    )
    args = parser.parse_args()

    if not args.input.exists():
        raise FileNotFoundError(f'Input file not found: {args.input}')

    content = args.input.read_text()
    struct_decl = extract_struct(content, args.struct)
    if struct_decl is None:
        raise ValueError(f'Struct {args.struct} not found in {args.input}')

    output_text = generate_header(struct_decl, args.input)

    if args.output:
        args.output.write_text(output_text)
    else:
        print(output_text)

    return 0


if __name__ == '__main__':
    raise SystemExit(main())
