/********************************************************************
 * Reading an ODC file: the lines and the words (R-1, R-2), the object
 * queries (R-10), the locations (R-6), and the table of commands (R-60),
 * which checks the arguments of each command.
 *******************************************************************/
#include <algorithm>
#include <cctype>
#include <fstream>
#include <functional>
#include <map>
#include <regex>
#include <sstream>

#include "odc.h"

/* begin namespace odc */
namespace odc {

namespace {

/* A problem with one command: the reader records it with the line */
struct CommandError {
  std::string message;
};

/* A line of commands: one or more lines of the file joined by '\' */
struct Line {
  size_t number = 0;
  std::string text;
};

/* Cut the comment of a line: a '#' outside quotes, braces and brackets */
std::string strip_comment(const std::string& text) {
  bool in_quotes = false;
  int depth = 0;
  for (size_t i = 0; i < text.size(); ++i) {
    const char c = text[i];
    if (in_quotes) {
      if ('\\' == c && i + 1 < text.size()) {
        ++i;
      } else if ('"' == c) {
        in_quotes = false;
      }
      continue;
    }
    if ('"' == c) {
      in_quotes = true;
    } else if ('{' == c || '[' == c) {
      ++depth;
    } else if (('}' == c || ']' == c) && depth > 0) {
      --depth;
    } else if ('#' == c && 0 == depth) {
      return text.substr(0, i);
    }
  }
  return text;
}

std::vector<Line> split_lines(const std::string& text) {
  std::vector<Line> lines;
  std::istringstream stream(text);
  std::string physical;
  size_t number = 0;
  Line current;
  bool continued = false;
  while (std::getline(stream, physical)) {
    ++number;
    if (!physical.empty() && '\r' == physical.back()) {
      physical.pop_back();
    }
    if (!continued) {
      current = Line();
      current.number = number;
    }
    /* A comment ends the line, and a '\' in it does not continue it */
    std::string code = strip_comment(physical);
    const bool cut = code.size() != physical.size();
    if (!cut && !code.empty() && '\\' == code.back()) {
      code.pop_back();
      current.text += code + " ";
      continued = true;
      continue;
    }
    current.text += code;
    continued = false;
    lines.push_back(current);
  }
  if (continued) {
    lines.push_back(current);
  }
  return lines;
}

bool is_space(const char c) { return ' ' == c || '\t' == c; }

/* The index of the character that closes the one at start ('{' or '['), with
 * the nesting of the same pair */
size_t closing(const std::string& text, const size_t start) {
  const char open = text[start];
  const char close = '{' == open ? '}' : ']';
  int depth = 0;
  bool in_quotes = false;
  for (size_t i = start; i < text.size(); ++i) {
    const char c = text[i];
    if ('[' == open && in_quotes) {
      if ('\\' == c && i + 1 < text.size()) {
        ++i;
      } else if ('"' == c) {
        in_quotes = false;
      }
      continue;
    }
    if ('[' == open && '"' == c) {
      in_quotes = true;
    } else if (c == open) {
      ++depth;
    } else if (c == close) {
      if (0 == --depth) {
        return i;
      }
    }
  }
  return std::string::npos;
}

Query parse_query(const std::string& text);

std::vector<Word> parse_words(const std::string& text) {
  std::vector<Word> words;
  size_t i = 0;
  const size_t n = text.size();
  while (true) {
    while (i < n && is_space(text[i])) {
      ++i;
    }
    if (i >= n) {
      break;
    }
    Word word;
    const char c = text[i];
    size_t end = i;
    if ('"' == c) {
      word.kind = Word::QUOTED;
      size_t j = i + 1;
      while (j < n && '"' != text[j]) {
        if ('\\' == text[j] && j + 1 < n &&
            ('"' == text[j + 1] || '\\' == text[j + 1])) {
          word.text += text[j + 1];
          j += 2;
        } else {
          word.text += text[j++];
        }
      }
      if (j >= n) {
        throw CommandError{"a quote is not closed"};
      }
      end = j + 1;
    } else if ('{' == c || '[' == c) {
      const size_t j = closing(text, i);
      if (std::string::npos == j) {
        throw CommandError{std::string("a '") + c + "' is not closed"};
      }
      word.text = text.substr(i + 1, j - i - 1);
      if ('{' == c) {
        word.kind = Word::BRACED;
        word.items = parse_words(word.text);
      } else {
        word.kind = Word::QUERY;
        word.query = parse_query(word.text);
      }
      end = j + 1;
    } else {
      word.kind = Word::BARE;
      size_t j = i;
      while (j < n && !is_space(text[j])) {
        ++j;
      }
      word.text = text.substr(i, j - i);
      end = j;
    }
    if (end < n && !is_space(text[end])) {
      throw CommandError{"'" + text.substr(i, end - i) +
                         "' is followed by text without a space"};
    }
    words.push_back(word);
    i = end;
  }
  return words;
}

/* The kind of the objects of each query command */
const std::map<std::string, std::string>& query_kinds() {
  static const std::map<std::string, std::string> kinds = {
    {"get_ports", "ports"},   {"get_cells", "cells"},
    {"get_nets", "nets"},     {"get_pins", "pins"},
    {"get_clocks", "clocks"}, {"get_pblocks", "pblocks"},
    {"get_sites", "sites"},   {"all_inputs", "ports"},
    {"all_outputs", "ports"}, {"all_clocks", "clocks"}};
  return kinds;
}

Query parse_query(const std::string& text) {
  Query query;
  query.text = text;
  const std::vector<Word> words = parse_words(text);
  if (words.empty() || Word::BARE != words[0].kind) {
    throw CommandError{"'[" + text + "]' is not an object query"};
  }
  query.command = words[0].text;
  const auto kind = query_kinds().find(query.command);
  if (query_kinds().end() == kind) {
    throw CommandError{"unknown object query '" + query.command +
                       "' (get_ports, get_cells, get_nets, get_pins, "
                       "get_clocks, get_pblocks, get_sites, all_inputs, "
                       "all_outputs, all_clocks)"};
  }
  query.kind = kind->second;
  if (0 == query.command.compare(0, 4, "all_")) {
    if (1 != words.size()) {
      throw CommandError{"'" + query.command + "' takes no argument"};
    }
    query.patterns.push_back("*");
    query.hierarchical = true;
    if ("all_inputs" == query.command) {
      query.filter = "direction == in";
    } else if ("all_outputs" == query.command) {
      query.filter = "direction == out";
    }
    return query;
  }
  for (size_t i = 1; i < words.size(); ++i) {
    const Word& word = words[i];
    if (Word::BARE == word.kind && !word.text.empty() && '-' == word.text[0]) {
      const std::string& option = word.text;
      if ("-regexp" == option) {
        query.regexp = true;
      } else if ("-hierarchical" == option || "-hier" == option) {
        query.hierarchical = true;
      } else if ("-nocase" == option) {
        query.nocase = true;
      } else if ("-quiet" == option) {
        /* Nothing: a query that matches nothing is reported anyway */
      } else if ("-filter" == option) {
        if (i + 1 >= words.size()) {
          throw CommandError{"-filter needs an expression"};
        }
        query.filter = words[++i].text;
      } else {
        throw CommandError{"unknown option '" + option + "' of " +
                           query.command};
      }
      continue;
    }
    if (Word::QUERY == word.kind) {
      throw CommandError{"a query inside a query is not supported: '[" +
                         word.text + "]'"};
    }
    if (Word::BRACED == word.kind) {
      for (const Word& item : word.items) {
        if (Word::QUERY == item.kind || Word::BRACED == item.kind) {
          throw CommandError{"'" + item.source() + "' is not a name"};
        }
        query.patterns.push_back(item.text);
      }
    } else {
      query.patterns.push_back(word.text);
    }
  }
  if (query.patterns.empty()) {
    query.patterns.push_back("*");
  }
  if (query.regexp) {
    for (const std::string& pattern : query.patterns) {
      try {
        std::regex check(pattern, std::regex::ECMAScript);
      } catch (const std::regex_error&) {
        throw CommandError{"'" + pattern + "' is not a regular expression"};
      }
    }
  }
  std::string error;
  if (!query.filter.empty() && !check_filter(query.filter, query.kind, error)) {
    throw CommandError{error};
  }
  return query;
}

/* The table of commands (R-60) ------------------------------------------*/

struct CommandRow {
  Kind kind;
  Stage stage;
  Consumer consumer;
  /* Check the arguments and fill the command. Throws CommandError */
  std::function<void(Command&, const ReadOptions&)> parse;
};

bool is_option(const Word& word) {
  return Word::BARE == word.kind && word.text.size() > 1 &&
         '-' == word.text[0] && !std::isdigit(word.text[1]) &&
         '.' != word.text[1];
}

bool is_number(const std::string& text) {
  static const std::regex number(R"([-+]?(\d+\.?\d*|\.\d+)([eE][-+]?\d+)?)");
  return std::regex_match(text, number);
}

/* The objects of a timing command: its queries. The kinds of VPR's SDC */
void parse_timing(Command& command, const ReadOptions&) {
  static const std::set<std::string> kinds = {"ports", "cells", "nets", "pins",
                                              "clocks"};
  for (size_t i = 1; i < command.words.size(); ++i) {
    const Word& word = command.words[i];
    if (Word::QUERY == word.kind) {
      if (0 == kinds.count(word.query.kind)) {
        throw CommandError{"'" + word.query.command +
                           "' cannot be used in a timing command"};
      }
      command.objects.push_back(i);
    }
  }
  if ("create_clock" == command.name) {
    bool period = false;
    for (size_t i = 1; i < command.words.size(); ++i) {
      if ("-period" == command.words[i].text) {
        if (i + 1 >= command.words.size() ||
            !is_number(command.words[i + 1].text)) {
          throw CommandError{"-period needs a number (ns)"};
        }
        period = true;
      }
    }
    if (!period) {
      throw CommandError{"create_clock needs -period"};
    }
  }
}

/* The single object argument of a command, a query of one of the kinds */
void object_argument(Command& command, const size_t index,
                     const std::set<std::string>& kinds,
                     const std::string& what) {
  if (index >= command.words.size()) {
    throw CommandError{command.name + " needs " + what};
  }
  if (index + 1 != command.words.size()) {
    throw CommandError{"'" + command.words[index + 1].source() +
                       "': too many arguments to " + command.name};
  }
  const Word& word = command.words[index];
  if (Word::QUERY != word.kind || 0 == kinds.count(word.query.kind)) {
    throw CommandError{command.name + " needs " + what + ", not '" +
                       word.source() + "'"};
  }
  command.objects.push_back(index);
}

const Query& object_query(const Command& command) {
  return command.words[command.objects.at(0)].query;
}

/* The objects of a synthesis command are matched by Yosys (R-23): its
 * patterns are globs */
void yosys_query(const Command& command) {
  const Query& query = object_query(command);
  if (query.regexp || !query.filter.empty() || query.nocase) {
    throw CommandError{
      "the objects of a synthesis command are matched by Yosys: -regexp, "
      "-nocase and -filter cannot be used"};
  }
}

std::string upper(std::string text) {
  for (char& c : text) {
    c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
  }
  return text;
}

std::string lower(std::string text) {
  for (char& c : text) {
    c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
  }
  return text;
}

bool is_true(const std::string& value) {
  const std::string text = lower(value);
  if ("true" == text || "1" == text) {
    return true;
  }
  if ("false" == text || "0" == text) {
    return false;
  }
  throw CommandError{"'" + value + "' is not true or false"};
}

void parse_set_property(Command& command, const ReadOptions& options) {
  if (command.words.size() < 4) {
    throw CommandError{"set_property needs a property, a value and objects"};
  }
  for (size_t i = 1; i < 3; ++i) {
    if (Word::QUERY == command.words[i].kind) {
      throw CommandError{
        "set_property needs a property and a value before "
        "its objects"};
    }
  }
  command.property = upper(command.words[1].text);
  command.value = command.words[2].text;
  const std::string& property = command.property;
  if ("KEEP" == property || "DONT_TOUCH" == property ||
      "MAX_FANOUT" == property || "RAM_STYLE" == property) {
    command.kind = Kind::SYNTHESIS;
    command.stage = Stage::CIRCUIT;
    command.consumer = Consumer::YOSYS;
    if ("KEEP" == property) {
      is_true(command.value);
      object_argument(command, 3, {"cells", "nets"},
                      "[get_cells] or [get_nets]");
    } else if ("DONT_TOUCH" == property) {
      is_true(command.value);
      object_argument(command, 3, {"cells"}, "[get_cells]");
    } else if ("MAX_FANOUT" == property) {
      static const std::regex positive(R"([1-9]\d*)");
      if (!std::regex_match(command.value, positive)) {
        throw CommandError{"MAX_FANOUT needs a whole number above 0"};
      }
      object_argument(command, 3, {"nets"}, "[get_nets]");
    } else {
      command.value = lower(command.value);
      if ("block" != command.value && "distributed" != command.value) {
        throw CommandError{"RAM_STYLE is block or distributed"};
      }
      object_argument(command, 3, {"cells"}, "[get_cells]");
    }
    yosys_query(command);
    return;
  }
  if ("LOC" == property) {
    object_argument(command, 3, {"cells", "ports"},
                    "[get_cells] or [get_ports]");
    std::string error;
    if (!parse_location(command.value, command.location, error)) {
      throw CommandError{error};
    }
    if (!command.location.is_tile()) {
      throw CommandError{
        "LOC takes a tile (X3Y2) or a subtile (X3Y2/1), not "
        "a rectangle: use a pblock"};
    }
    command.consumer = Consumer::VPR_CONSTRAINTS;
    command.stage = Stage::DEVICE;
    command.kind =
      "ports" == object_query(command).kind ? Kind::PIN : Kind::FLOORPLAN;
    return;
  }
  command.kind = Kind::PIN;
  command.stage = Stage::DEVICE;
  if ("PACKAGE_PIN" == property) {
    object_argument(command, 3, {"ports"}, "[get_ports]");
    command.consumer = Consumer::VPR_CONSTRAINTS;
    return;
  }
  if ("PIN_DEFAULT" == property) {
    if ("0" != command.value && "1" != command.value) {
      throw CommandError{"PIN_DEFAULT is 0 or 1"};
    }
    object_argument(command, 3, {"ports"}, "[get_ports]");
    command.consumer = Consumer::PIN_CONSTRAINTS;
    return;
  }
  if (options.custom_properties.count(command.words[1].text)) {
    throw CommandError{"the property '" + command.words[1].text +
                       "' of the pcf_config of the device package is not "
                       "supported yet"};
  }
  throw CommandError{"unknown property '" + command.words[1].text +
                     "' (KEEP, DONT_TOUCH, MAX_FANOUT, RAM_STYLE, LOC, "
                     "PACKAGE_PIN, PIN_DEFAULT)"};
}

void parse_set_synth_option(Command& command, const ReadOptions&) {
  const std::vector<Word>& words = command.words;
  if (words.size() < 2) {
    throw CommandError{"set_synth_option needs an option"};
  }
  const std::string& option = words[1].text;
  if ("-flatten" == option || "-keep_hierarchy" == option) {
    if (2 != words.size()) {
      throw CommandError{option + " takes no value"};
    }
    command.property = option.substr(1);
    return;
  }
  if ("-top" == option || "-define" == option) {
    if (3 != words.size() || Word::QUERY == words[2].kind) {
      throw CommandError{option + " takes one value"};
    }
    command.property = option.substr(1);
    command.value = words[2].text;
    static const std::regex name(R"([A-Za-z_][A-Za-z0-9_$]*)");
    static const std::regex define(R"([A-Za-z_][A-Za-z0-9_]*(=[^\s"'`;]*)?)");
    if ("-top" == option && !std::regex_match(command.value, name)) {
      throw CommandError{"'" + command.value + "' is not a module name"};
    }
    if ("-define" == option && !std::regex_match(command.value, define)) {
      throw CommandError{"-define takes NAME or NAME=value, not '" +
                         command.value + "'"};
    }
    return;
  }
  throw CommandError{"unknown option '" + option +
                     "' of set_synth_option (-flatten, -keep_hierarchy, "
                     "-top, -define)"};
}

bool is_name(const std::string& text) {
  static const std::regex name(R"([A-Za-z_][A-Za-z0-9_.\-]*)");
  return std::regex_match(text, name);
}

void pblock_name(Command& command) {
  if (command.words.size() < 2 || !is_name(command.words[1].text) ||
      Word::BARE != command.words[1].kind) {
    throw CommandError{command.name + " needs the name of a pblock"};
  }
  command.pblock = command.words[1].text;
}

void parse_create_pblock(Command& command, const ReadOptions&) {
  pblock_name(command);
  if (2 != command.words.size()) {
    throw CommandError{"create_pblock takes a name only"};
  }
}

void parse_resize_pblock(Command& command, const ReadOptions&) {
  pblock_name(command);
  std::string location;
  int layer = 0;
  for (size_t i = 2; i < command.words.size(); ++i) {
    const std::string& word = command.words[i].text;
    if ("-add" == word && i + 1 < command.words.size()) {
      location = command.words[++i].text;
    } else if ("-layer" == word && i + 1 < command.words.size()) {
      const std::string& value = command.words[++i].text;
      static const std::regex whole(R"(\d+)");
      if (!std::regex_match(value, whole)) {
        throw CommandError{"-layer needs a whole number"};
      }
      layer = std::stoi(value);
    } else {
      throw CommandError{"'" + command.words[i].source() +
                         "': resize_pblock takes -add <location> [-layer n]"};
    }
  }
  if (location.empty()) {
    throw CommandError{"resize_pblock needs -add <location>"};
  }
  std::string error;
  if (!parse_location(location, command.location, error)) {
    throw CommandError{error};
  }
  command.location.layer = layer;
}

void parse_add_cells_to_pblock(Command& command, const ReadOptions&) {
  pblock_name(command);
  object_argument(command, 2, {"cells"}, "[get_cells]");
}

void parse_set_global_signal(Command& command, const ReadOptions&) {
  const std::vector<Word>& words = command.words;
  size_t i = 1;
  for (; i < words.size() && is_option(words[i]); ++i) {
    const std::string& option = words[i].text;
    if (i + 1 >= words.size()) {
      throw CommandError{option + " needs a value"};
    }
    if ("-route_model" == option) {
      command.route_model = words[++i].text;
    } else if ("-network" == option) {
      command.network = words[++i].text;
    } else {
      throw CommandError{"unknown option '" + option +
                         "' of set_global_signal (-route_model, -network)"};
    }
  }
  if ("ideal" != command.route_model && "route" != command.route_model &&
      "dedicated_network" != command.route_model) {
    throw CommandError{
      "set_global_signal needs -route_model ideal, route or "
      "dedicated_network"};
  }
  if ("dedicated_network" == command.route_model && command.network.empty()) {
    throw CommandError{
      "-route_model dedicated_network needs -network <name>: the clock "
      "network of the architecture"};
  }
  if ("dedicated_network" != command.route_model && !command.network.empty()) {
    throw CommandError{"-network is only for -route_model dedicated_network"};
  }
  object_argument(command, i, {"nets"}, "[get_nets]");
}

void parse_nothing(Command&, const ReadOptions&) {}

const std::map<std::string, CommandRow>& command_table() {
  static const std::map<std::string, CommandRow> table = [] {
    std::map<std::string, CommandRow> rows;
    for (const char* name :
         {"create_clock", "set_clock_groups", "set_clock_latency",
          "set_clock_uncertainty", "set_input_delay", "set_output_delay",
          "set_false_path", "set_max_delay", "set_min_delay",
          "set_multicycle_path", "set_disable_timing", "set_timing_derate"}) {
      rows[name] = {Kind::TIMING, Stage::DEVICE, Consumer::VPR_SDC,
                    parse_timing};
    }
    /* set_property says its kind by its property */
    rows["set_property"] = {Kind::PIN, Stage::DEVICE, Consumer::NONE,
                            parse_set_property};
    rows["set_synth_option"] = {Kind::SYNTHESIS, Stage::CIRCUIT,
                                Consumer::YOSYS, parse_set_synth_option};
    rows["create_pblock"] = {Kind::FLOORPLAN, Stage::DEVICE,
                             Consumer::VPR_CONSTRAINTS, parse_create_pblock};
    rows["resize_pblock"] = {Kind::FLOORPLAN, Stage::DEVICE,
                             Consumer::VPR_CONSTRAINTS, parse_resize_pblock};
    rows["add_cells_to_pblock"] = {Kind::FLOORPLAN, Stage::DEVICE,
                                   Consumer::VPR_CONSTRAINTS,
                                   parse_add_cells_to_pblock};
    rows["set_global_signal"] = {Kind::ROUTING, Stage::DEVICE,
                                 Consumer::VPR_CONSTRAINTS,
                                 parse_set_global_signal};
    rows["odc_version"] = {Kind::HEADER, Stage::DEVICE, Consumer::NONE,
                           parse_nothing};
    rows["include"] = {Kind::INCLUDE, Stage::DEVICE, Consumer::NONE,
                       parse_nothing};
    return rows;
  }();
  return table;
}

/* What a file is read with: the files it includes, to find a loop */
struct Reader {
  ReadOptions options;
  Constraints constraints;
  std::vector<std::filesystem::path> stack;
  /* The pblocks created so far */
  std::set<std::string> pblocks;

  void error(const std::string& file, const size_t line,
             const std::string& message) {
    constraints.diagnostics.push_back({Diagnostic::ERROR, file, line, message});
  }
  void warning(const std::string& file, const size_t line,
               const std::string& message) {
    constraints.diagnostics.push_back(
      {Diagnostic::WARNING, file, line, message});
  }

  void read_file(const std::filesystem::path& path, const std::string& from,
                 const size_t from_line);
  void read_text(const std::string& text, const std::string& file);
};

void Reader::read_file(const std::filesystem::path& path,
                       const std::string& from, const size_t from_line) {
  std::error_code ignored;
  const std::filesystem::path whole =
    std::filesystem::weakly_canonical(path, ignored);
  if (std::find(stack.begin(), stack.end(), whole) != stack.end()) {
    error(from, from_line,
          "'" + path.string() + "' includes itself, directly or not");
    return;
  }
  std::ifstream stream(path);
  if (!stream) {
    error(from, from_line, "cannot read the file '" + path.string() + "'");
    return;
  }
  std::stringstream text;
  text << stream.rdbuf();
  stack.push_back(whole);
  constraints.files.push_back(path.string());
  read_text(text.str(), path.string());
  stack.pop_back();
}

void Reader::read_text(const std::string& text, const std::string& file) {
  bool first = true;
  for (const Line& line : split_lines(text)) {
    Command command;
    command.file = file;
    command.line = line.number;
    try {
      command.words = parse_words(line.text);
    } catch (const CommandError& problem) {
      error(file, line.number, problem.message);
      first = false;
      continue;
    }
    if (command.words.empty()) {
      continue;
    }
    if (Word::BARE != command.words[0].kind) {
      error(file, line.number,
            "a command starts with its name, not '" +
              command.words[0].source() + "'");
      first = false;
      continue;
    }
    command.name = command.words[0].text;
    const bool header = "odc_version" == command.name;
    if (first && !header) {
      warning(file, line.number,
              "the file does not start with 'odc_version 1': it is read as "
              "version 1");
    }
    if (header && !first) {
      error(file, line.number, "odc_version must be the first command");
      continue;
    }
    first = false;
    const auto row = command_table().find(command.name);
    if (command_table().end() == row) {
      const std::string message = "unknown command '" + command.name + "'";
      if (options.lenient) {
        warning(file, line.number, message + ": skipped");
      } else {
        error(file, line.number, message);
      }
      continue;
    }
    command.kind = row->second.kind;
    command.stage = row->second.stage;
    command.consumer = row->second.consumer;
    try {
      row->second.parse(command, options);
      if (header) {
        static const std::regex whole(R"(\d+)");
        if (2 != command.words.size() ||
            !std::regex_match(command.words[1].text, whole)) {
          throw CommandError{"odc_version takes a version, e.g. 1"};
        }
        if (std::stoi(command.words[1].text) > ODC_VERSION) {
          throw CommandError{"version " + command.words[1].text +
                             " of ODC is newer than this reader (" +
                             std::to_string(ODC_VERSION) + ")"};
        }
        if (std::stoi(command.words[1].text) < 1) {
          throw CommandError{"the versions of ODC start at 1"};
        }
      }
      if ("create_pblock" == command.name) {
        if (!pblocks.insert(command.pblock).second) {
          throw CommandError{"the pblock '" + command.pblock +
                             "' is created twice"};
        }
      } else if (!command.pblock.empty() && !pblocks.count(command.pblock)) {
        throw CommandError{"the pblock '" + command.pblock +
                           "' is not created before (create_pblock)"};
      }
    } catch (const CommandError& problem) {
      error(file, line.number, problem.message);
      continue;
    }
    if (Kind::INCLUDE == command.kind) {
      if (2 != command.words.size() || Word::QUERY == command.words[1].kind) {
        error(file, line.number, "include takes one file");
        continue;
      }
      std::filesystem::path included(command.words[1].text);
      if (included.is_relative()) {
        included = std::filesystem::path(file).parent_path() / included;
      }
      read_file(included, file, line.number);
      continue;
    }
    if (Kind::HEADER == command.kind) {
      continue;
    }
    constraints.commands.push_back(command);
  }
}

}  // namespace

bool parse_location(const std::string& text, Location& location,
                    std::string& error) {
  static const std::regex tile(R"(X(\d+)Y(\d+)(?:/(\d+))?)");
  static const std::regex range(R"(X(\d+)Y(\d+):X(\d+)Y(\d+))");
  std::smatch match;
  location = Location();
  location.text = text;
  if (std::regex_match(text, match, tile)) {
    location.x_low = location.x_high = std::stoi(match[1]);
    location.y_low = location.y_high = std::stoi(match[2]);
    if (match[3].matched) {
      location.subtile = std::stoi(match[3]);
    }
    return true;
  }
  if (std::regex_match(text, match, range)) {
    location.x_low = std::min(std::stoi(match[1]), std::stoi(match[3]));
    location.x_high = std::max(std::stoi(match[1]), std::stoi(match[3]));
    location.y_low = std::min(std::stoi(match[2]), std::stoi(match[4]));
    location.y_high = std::max(std::stoi(match[2]), std::stoi(match[4]));
    return true;
  }
  error =
    "'" + text + "' is not a location: X3Y2, X3Y2/1 (a subtile) or X1Y1:X4Y6";
  return false;
}

Constraints read_odc_file(const std::filesystem::path& file,
                          const ReadOptions& options) {
  Reader reader;
  reader.options = options;
  reader.read_file(file, file.string(), 0);
  return reader.constraints;
}

Constraints read_odc_text(const std::string& text, const std::string& file,
                          const ReadOptions& options) {
  Reader reader;
  reader.options = options;
  std::error_code ignored;
  reader.stack.push_back(std::filesystem::weakly_canonical(file, ignored));
  reader.constraints.files.push_back(file);
  reader.read_text(text, file);
  return reader.constraints;
}

Constraints merge_defaults(const Constraints& defaults,
                           const Constraints& user) {
  Constraints merged = defaults;
  for (Command& command : merged.commands) {
    command.from_defaults = true;
  }
  merged.commands.insert(merged.commands.end(), user.commands.begin(),
                         user.commands.end());
  merged.diagnostics.insert(merged.diagnostics.end(), user.diagnostics.begin(),
                            user.diagnostics.end());
  merged.files.insert(merged.files.end(), user.files.begin(), user.files.end());
  return merged;
}

} /* end namespace odc */
