/********************************************************************
 * Synthesis with Yosys, and the design directories (see design_synthesis.h)
 *******************************************************************/
#include "design_synthesis.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <sstream>
#include <system_error>

#include "nlohmann/json.hpp"

/* begin namespace openfpga */
namespace openfpga {

namespace fs = std::filesystem;
using json = nlohmann::ordered_json;

const char* const SYNTHESIS_SCRIPT_NAME = "synth.ys";

namespace {

SynthesisTargetProvider& target_provider() {
  static SynthesisTargetProvider provider;
  return provider;
}

std::string& constraints_command() {
  static std::string name;
  return name;
}

const std::vector<std::string> SOURCE_SUFFIXES = {".v", ".sv", ".vh"};
const size_t MAX_SOURCE_FILES = 20;
const uintmax_t MAX_SOURCE_BYTES = 1024 * 1024;

/* The script of a device with no script of its own: LUT mapping with ABC, as
 * the flow scripts do it */
const char* const GENERIC_SCRIPT =
  "# Yosys synthesis script for ${TOP_MODULE}\n"
  "read_verilog ${READ_VERILOG_OPTIONS} ${VERILOG_FILES}\n"
  "hierarchy -check -top ${TOP_MODULE}\n"
  "${CONSTRAINT_SYNTHESIS}\n"
  "proc\n"
  "flatten\n"
  "opt_expr\n"
  "opt_clean\n"
  "check\n"
  "opt -nodffe -nosdff\n"
  "fsm\n"
  "opt -nodffe -nosdff\n"
  "wreduce\n"
  "peepopt\n"
  "opt_clean\n"
  "opt -nodffe -nosdff\n"
  "memory -nomap\n"
  "opt_clean\n"
  "opt -fast -full -nodffe -nosdff\n"
  "memory_map\n"
  "opt -full -nodffe -nosdff\n"
  "techmap\n"
  "opt -fast -nodffe -nosdff\n"
  "clean\n"
  "abc -lut ${LUT_SIZE}\n"
  "opt_clean -purge\n"
  "write_blif ${OUTPUT_BLIF}\n";

bool matches(const std::string& text, const char* pattern) {
  return std::regex_match(text, std::regex(pattern));
}

std::string lower(std::string text) {
  std::transform(text.begin(), text.end(), text.begin(),
                 [](unsigned char c) { return std::tolower(c); });
  return text;
}

bool is_source(const fs::path& path) {
  const std::string suffix = lower(path.extension().string());
  return SOURCE_SUFFIXES.end() !=
         std::find(SOURCE_SUFFIXES.begin(), SOURCE_SUFFIXES.end(), suffix);
}

std::string read_text(const fs::path& path) {
  std::ifstream stream(path, std::ios::binary);
  return std::string(std::istreambuf_iterator<char>(stream),
                     std::istreambuf_iterator<char>());
}

bool has_parent_part(const fs::path& path) {
  for (const fs::path& part : path) {
    if (".." == part.string()) {
      return true;
    }
  }
  return false;
}

std::string joined(const std::vector<std::string>& words,
                   const std::string& sep) {
  std::string text;
  for (const std::string& word : words) {
    text += (text.empty() ? "" : sep) + word;
  }
  return text;
}

void check_top(const std::string& top) {
  if (!matches(top, "^[A-Za-z_][A-Za-z0-9_$]{0,99}$")) {
    throw SynthesisRefused(
      "top is the name of the top module (letters, digits, _ and $)");
  }
}

/* The files are Verilog of a size that a netlist is made from, and none of
 * them takes a file from outside its folder */
void check_sources(const std::vector<fs::path>& paths) {
  if (paths.empty()) {
    throw SynthesisRefused(
      "files lists the Verilog files of the design (.v, .sv, .vh)");
  }
  if (paths.size() > MAX_SOURCE_FILES) {
    throw SynthesisRefused("at most " + std::to_string(MAX_SOURCE_FILES) +
                           " files can be synthesized together");
  }
  static const std::regex file_use(
    "(?:`include|\\$readmem[hb])\\s*(?:\\(\\s*)?\"([^\"]*)\"|`include\\s*<(["
    "^>]*)>");
  for (const fs::path& path : paths) {
    const std::string name = path.filename().string();
    if (!is_source(path)) {
      throw SynthesisRefused("'" + name +
                             "' is not a Verilog file (.v, .sv, .vh)");
    }
    std::error_code error;
    if (fs::file_size(path, error) > MAX_SOURCE_BYTES && !error) {
      throw SynthesisRefused("'" + name + "' is larger than the " +
                             std::to_string(MAX_SOURCE_BYTES) +
                             " bytes that are synthesized");
    }
    const std::string text = read_text(path);
    for (auto it = std::sregex_iterator(text.begin(), text.end(), file_use);
         it != std::sregex_iterator(); ++it) {
      const std::string used =
        (*it)[1].matched ? (*it)[1].str() : (*it)[2].str();
      if ((!used.empty() && ('/' == used[0] || '~' == used[0])) ||
          has_parent_part(fs::path(used)) ||
          std::string::npos != used.find('\\')) {
        throw SynthesisRefused("'" + name + "' names the file '" + used +
                               "', which is outside the session: use a name "
                               "inside it");
      }
    }
  }
}

/* The paths of a script: Yosys does not take every character in them */
void check_paths(const std::vector<std::string>& paths) {
  for (const std::string& path : paths) {
    if (!matches(path, "^[A-Za-z0-9_./+-]+$")) {
      throw SynthesisRefused("The path '" + path +
                             "' has characters that a Yosys script does not "
                             "take");
    }
  }
}

/* The values of a target are written in a script as they are: a value has no
 * character that could start another command or another line */
void check_values(const SynthesisTarget& target) {
  for (const auto& value : target.values) {
    if (!matches(value.first, "^[A-Z][A-Z0-9_]*$") ||
        !matches(value.second, "^[A-Za-z0-9_./+=, -]*$")) {
      throw SynthesisRefused("The value ${" + value.first +
                             "} of the synthesis target '" + target.name +
                             "' has characters that a Yosys script does not "
                             "take");
    }
  }
  const auto read = target.values.find("READ_VERILOG_OPTIONS");
  if (target.values.end() != read) {
    std::stringstream words(read->second);
    std::string word;
    while (words >> word) {
      if (!matches(word, "^-[A-Za-z]{1,20}$")) {
        throw SynthesisRefused(
          "The options of read_verilog of the synthesis "
          "target '" +
          target.name + "' are options like -nolatches");
      }
    }
  }
}

/* The options of read_verilog for a request, after base (the ones of the
 * target): its read options, defines and include folders, and -sv for
 * SystemVerilog */
std::string read_options_of(const SynthesisRequest& request,
                            const std::string& base) {
  std::vector<std::string> extra;
  for (const std::string& option : request.read_options) {
    if (!matches(option, "^-[A-Za-z]{1,20}$")) {
      throw SynthesisRefused(
        "read_options of the design are options like -nolatches");
    }
    extra.push_back(option);
  }
  for (const auto& define : request.defines) {
    const std::string option = "-D " + define.first + "=" + define.second;
    if (!matches(option, "^-D [A-Za-z_][A-Za-z0-9_]*=[A-Za-z0-9_]+$")) {
      throw SynthesisRefused("The define '" + define.first +
                             "' of the design has characters that a Yosys "
                             "script does not take");
    }
    extra.push_back(option);
  }
  for (const fs::path& folder : request.include_dirs) {
    if (!matches(folder.string(), "^[A-Za-z0-9_./+-]+$")) {
      throw SynthesisRefused("The include folder '" + folder.string() +
                             "' has characters that a Yosys script does not "
                             "take");
    }
    extra.push_back("-I" + folder.string());
  }
  for (const fs::path& file : request.files) {
    if (".sv" == lower(file.extension().string())) {
      extra.push_back("-sv");
      break;
    }
  }
  std::string read = joined({base, joined(extra, " ")}, " ");
  read.erase(0, read.find_first_not_of(' '));
  read.erase(read.find_last_not_of(' ') + 1);
  return read;
}

/* A template as Python's string.Template.safe_substitute() fills it:
 * ${NAME} and $NAME of a known name become its value, $$ becomes $, and
 * the rest stays (a script has Yosys names with a dollar, as $mul) */
std::string substitute(const std::string& text,
                       const std::map<std::string, std::string>& values) {
  std::string out;
  const auto identifier_start = [](char c) {
    return '_' == c || std::isalpha(static_cast<unsigned char>(c));
  };
  const auto identifier_char = [](char c) {
    return '_' == c || std::isalnum(static_cast<unsigned char>(c));
  };
  for (size_t i = 0; i < text.size(); ++i) {
    if ('$' != text[i] || i + 1 >= text.size()) {
      out += text[i];
      continue;
    }
    const char next = text[i + 1];
    if ('$' == next) {
      out += '$';
      ++i;
      continue;
    }
    if ('{' == next) {
      const size_t end = text.find('}', i + 2);
      if (std::string::npos != end) {
        const std::string name = text.substr(i + 2, end - i - 2);
        const auto found = values.find(name);
        if (values.end() != found && !name.empty() &&
            identifier_start(name[0]) &&
            std::all_of(name.begin(), name.end(), identifier_char)) {
          out += found->second;
          i = end;
          continue;
        }
      }
      out += '$';
      continue;
    }
    if (identifier_start(next)) {
      size_t end = i + 1;
      while (end < text.size() && identifier_char(text[end])) {
        ++end;
      }
      const auto found = values.find(text.substr(i + 1, end - i - 1));
      if (values.end() != found) {
        out += found->second;
        i = end - 1;
        continue;
      }
    }
    out += '$';
  }
  return out;
}

std::string text_of(const json& value) {
  if (value.is_string()) {
    return value.get<std::string>();
  }
  if (value.is_null()) {
    return "None";
  }
  return value.dump();
}

} /* end anonymous namespace */

void set_synthesis_target_provider(SynthesisTargetProvider provider) {
  target_provider() = std::move(provider);
}

bool synthesis_target(const OpenfpgaContext& ctx, SynthesisTarget& target) {
  target = SynthesisTarget();
  return target_provider() && target_provider()(ctx, target);
}

void set_design_constraints_command(const std::string& name) {
  constraints_command() = name;
}

const std::string& design_constraints_command() {
  return constraints_command();
}

SynthesisPlan make_synthesis_plan(const SynthesisRequest& request,
                                  const SynthesisTarget* target) {
  check_top(request.top);
  check_sources(request.files);
  std::string script_template = GENERIC_SCRIPT;
  std::map<std::string, std::string> values;
  values["READ_VERILOG_OPTIONS"] = "";
  int size = request.lut_size ? *request.lut_size : 0;
  if (nullptr != target) {
    check_values(*target);
    for (const auto& value : target->values) {
      values[value.first] = value.second;
    }
    /* A target without a script of its own gets the generic one */
    if (!target->yosys_script.empty()) {
      std::error_code error;
      if (!fs::is_regular_file(target->yosys_script, error)) {
        throw SynthesisRefused(
          "The Yosys script '" + target->yosys_script.string() +
          "' of the synthesis target '" + target->name + "' does not exist");
      }
      script_template = read_text(target->yosys_script);
    }
    if (target->lut_size) {
      size = *target->lut_size;
    }
  }
  if (size < 2 || size > 8) {
    throw SynthesisRefused(
      "lut_size (2 to 8) is needed: give it, or a synthesis target that has "
      "it");
  }
  /* A device whose circuits are extended BLIF (.param, .attr: DSP and memory
   * cells) gets a netlist with the extension that the flow reads that format
   * from */
  bool extended = false;
  if (nullptr != target) {
    for (const auto& option : target->vpr_options) {
      extended = extended ||
                 ("circuit_format" == option.first && "eblif" == option.second);
    }
  }
  const fs::path netlist =
    request.folder / (request.top + (extended ? ".eblif" : ".blif"));
  std::vector<std::string> names;
  for (const fs::path& file : request.files) {
    names.push_back(file.string());
  }
  std::vector<std::string> checked = names;
  checked.push_back(netlist.string());
  for (const auto& value : values) {
    if (0 == value.first.rfind("YOSYS_", 0) &&
        std::string::npos == value.first.find("PARAMETERS")) {
      checked.push_back(value.second);
    }
  }
  check_paths(checked);
  values["READ_VERILOG_OPTIONS"] =
    read_options_of(request, values["READ_VERILOG_OPTIONS"]);
  values["TOP_MODULE"] = request.top;
  values["VERILOG_FILES"] = joined(names, " ");
  values["LUT_SIZE"] = std::to_string(size);
  values["OUTPUT_BLIF"] = netlist.string();
  /* The hook of the synthesis constraints of the session, right after the
   * hierarchy is elaborated. A script without it cannot take them */
  if (!request.constraints.empty() &&
      std::string::npos == script_template.find("${CONSTRAINT_SYNTHESIS}")) {
    throw SynthesisRefused(
      "The constraints of the session have synthesis commands, and the Yosys "
      "script " +
      (nullptr != target && !target->yosys_script.empty()
         ? "of the synthesis target '" + target->name + "'"
         : std::string("of the synthesis")) +
      " has no line ${CONSTRAINT_SYNTHESIS} to take them");
  }
  std::string constraints = request.constraints;
  while (!constraints.empty() && '\n' == constraints.back()) {
    constraints.pop_back();
  }
  values["CONSTRAINT_SYNTHESIS"] = constraints;
  const std::string script = substitute(script_template, values);
  static const std::regex left("\\$\\{([A-Z0-9_]+)\\}");
  std::vector<std::string> missing;
  for (auto it = std::sregex_iterator(script.begin(), script.end(), left);
       it != std::sregex_iterator(); ++it) {
    const std::string name = "${" + (*it)[1].str() + "}";
    if (missing.end() == std::find(missing.begin(), missing.end(), name)) {
      missing.push_back(name);
    }
  }
  if (!missing.empty()) {
    std::sort(missing.begin(), missing.end());
    throw SynthesisRefused("The script uses " + joined(missing, ", ") +
                           ", which the tool does not give: the device needs "
                           "a script of its own");
  }
  SynthesisPlan plan;
  plan.top = request.top;
  plan.script = script;
  plan.script_file = request.folder / SYNTHESIS_SCRIPT_NAME;
  plan.netlist = netlist;
  plan.lut_size = size;
  plan.files = request.files;
  plan.target = nullptr != target ? target->name : std::string();
  plan.constraints = !constraints.empty();
  return plan;
}

std::string make_hierarchy_script(const SynthesisRequest& request,
                                  const SynthesisTarget* target,
                                  const fs::path& json_file) {
  check_top(request.top);
  check_sources(request.files);
  std::map<std::string, std::string> values;
  std::vector<std::string> libraries;
  if (nullptr != target) {
    check_values(*target);
    values = target->values;
    for (const fs::path& library : target->cell_libraries) {
      libraries.push_back(library.string());
    }
  }
  std::vector<std::string> names;
  for (const fs::path& file : request.files) {
    names.push_back(file.string());
  }
  std::vector<std::string> checked = names;
  checked.push_back(json_file.string());
  checked.insert(checked.end(), libraries.begin(), libraries.end());
  check_paths(checked);
  std::string script = "# The hierarchy of " + request.top + "\n";
  script += joined({"read_verilog",
                    read_options_of(request, values["READ_VERILOG_OPTIONS"]),
                    joined(names, " ")},
                   " ") +
            "\n";
  /* The cells of the device that the RTL instantiates are known, as
   * blackboxes: the synthesis reads them the same way */
  for (const std::string& library : libraries) {
    script += "read_verilog -lib -specify " + library + "\n";
  }
  /* Without -check: a module that no file has stays in the tree, as unknown */
  script += "hierarchy -top " + request.top + "\n";
  script += "proc\n";
  script += "write_json " + json_file.string() + "\n";
  /* Two spaces where an option was empty */
  std::string tidy;
  for (const char c : script) {
    if (!(' ' == c && !tidy.empty() && ' ' == tidy.back())) {
      tidy += c;
    }
  }
  return tidy;
}

DesignDirectory read_design_directory(const fs::path& given,
                                      const std::string& label) {
  std::error_code error;
  DesignDirectory design;
  design.folder = fs::absolute(given, error).lexically_normal();
  if (!design.folder.has_filename()) {
    design.folder = design.folder.parent_path();
  }
  design.name = design.folder.filename().string();
  json manifest;
  try {
    manifest = json::parse(read_text(design.folder / "design.json"));
  } catch (const std::exception& reason) {
    throw SynthesisRefused("The design directory '" + design.name +
                           "' has no design.json that can be read (" +
                           reason.what() + ")");
  }
  if (!manifest.is_object()) {
    throw SynthesisRefused("The design.json of '" + design.name +
                           "' is not an object");
  }
  if (manifest.contains("name") && manifest["name"].is_string() &&
      !manifest["name"].get<std::string>().empty()) {
    design.name = manifest["name"].get<std::string>();
  }
  const fs::path base = fs::weakly_canonical(design.folder, error);
  /* A path of the manifest, inside the design */
  const auto inside = [&](const json& relative, const std::string& what) {
    if (!relative.is_string() ||
        fs::path(relative.get<std::string>()).is_absolute() ||
        has_parent_part(fs::path(relative.get<std::string>()))) {
      throw SynthesisRefused("The manifest of '" + design.name + "' has the " +
                             what + " '" + text_of(relative) +
                             "', which is not inside the design");
    }
    const fs::path path =
      fs::weakly_canonical(design.folder / relative.get<std::string>(), error);
    const fs::path from_base = path.lexically_relative(base);
    if (from_base.empty() || "." == from_base.string() ||
        has_parent_part(from_base)) {
      throw SynthesisRefused("The " + what + " '" +
                             relative.get<std::string>() + "' of '" +
                             design.name + "' leads outside the design");
    }
    return path;
  };
  std::vector<json> entries;
  if (manifest.contains("files") && manifest["files"].is_array()) {
    for (const json& entry : manifest["files"]) {
      if (entry.is_object()) {
        entries.push_back(entry);
      }
    }
  }
  const auto role_of = [](const json& entry) {
    return entry.contains("role") && entry["role"].is_string()
             ? entry["role"].get<std::string>()
             : std::string();
  };
  std::vector<json> rtl;
  std::vector<json> netlists;
  for (const json& entry : entries) {
    if ("rtl" == role_of(entry)) {
      rtl.push_back(entry);
    } else if ("netlist" == role_of(entry)) {
      netlists.push_back(entry);
    } else if ("sdc" == role_of(entry)) {
      const std::string stage =
        entry.contains("stage") && entry["stage"].is_string()
          ? entry["stage"].get<std::string>()
          : std::string("both");
      if ("synthesis" != stage && design.sdc.empty()) {
        design.sdc = inside(entry.value("path", json()), "file");
      }
    } else if ("constraints" == role_of(entry)) {
      if (!design.constraints.empty()) {
        throw SynthesisRefused("The design '" + design.name +
                               "' has more than one file of the role "
                               "constraints");
      }
      design.constraints = inside(entry.value("path", json()), "file");
    }
  }
  /* The netlist to read: the one of a label, or the only one of a design
   * with no RTL (a design with RTL is synthesized, even when it has a
   * netlist) */
  const json* chosen = nullptr;
  if (!label.empty()) {
    for (const json& entry : netlists) {
      if (entry.contains("label") && entry["label"] == label) {
        chosen = &entry;
      }
    }
    if (nullptr == chosen) {
      throw SynthesisRefused("The design '" + design.name +
                             "' has no netlist labelled '" + label + "'");
    }
  } else if (rtl.empty() && !netlists.empty()) {
    chosen = &netlists[0];
  }
  if (nullptr != chosen) {
    const json relative = chosen->value("path", json());
    if (!relative.is_string() ||
        fs::path(relative.get<std::string>()).is_absolute() ||
        has_parent_part(fs::path(relative.get<std::string>()))) {
      throw SynthesisRefused("The manifest of '" + design.name +
                             "' has the netlist '" + text_of(relative) +
                             "', which is not inside the design");
    }
    const std::string text = relative.get<std::string>();
    const std::string suffix = lower(fs::path(text).extension().string());
    if (".blif" != suffix && ".eblif" != suffix) {
      throw SynthesisRefused("The design '" + design.name +
                             "' has only the netlist '" + text +
                             "', which the flow does not read (it reads BLIF "
                             "and extended BLIF)");
    }
    const fs::path path = fs::weakly_canonical(design.folder / text, error);
    const fs::path from_base = path.lexically_relative(base);
    if (from_base.empty() || has_parent_part(from_base) ||
        !fs::is_regular_file(path, error)) {
      throw SynthesisRefused("The netlist '" + text + "' of '" + design.name +
                             "' does not exist");
    }
    design.netlist = path;
    return design;
  }
  if (rtl.empty()) {
    throw SynthesisRefused("The design '" + design.name +
                           "' has no RTL to synthesize");
  }
  for (const json& entry : rtl) {
    const json relative = entry.value("path", json());
    const fs::path path = inside(relative, "file");
    if (!is_source(path)) {
      throw SynthesisRefused("The design '" + design.name + "' has the file '" +
                             text_of(relative) +
                             "', which is not Verilog or SystemVerilog (this "
                             "tool synthesizes those)");
    }
    if (!fs::is_regular_file(path, error)) {
      throw SynthesisRefused("The file '" + text_of(relative) + "' of '" +
                             design.name + "' does not exist");
    }
    design.rtl.push_back(path);
  }
  const json block = manifest.contains("rtl") && manifest["rtl"].is_object()
                       ? manifest["rtl"]
                       : json::object();
  if (block.contains("include_dirs") && block["include_dirs"].is_array()) {
    for (const json& folder : block["include_dirs"]) {
      design.include_dirs.push_back(inside(folder, "include folder"));
    }
  }
  if (!manifest.contains("top") || !manifest["top"].is_string()) {
    throw SynthesisRefused("The manifest of '" + design.name + "' has no top");
  }
  design.top = manifest["top"].get<std::string>();
  if (block.contains("defines") && !block["defines"].is_null()) {
    if (!block["defines"].is_object()) {
      throw SynthesisRefused(
        "defines of the design are NAME: value pairs of text");
    }
    for (const auto& item : block["defines"].items()) {
      if (!item.value().is_string()) {
        throw SynthesisRefused(
          "defines of the design are NAME: value pairs of text");
      }
      design.defines.emplace_back(item.key(), item.value().get<std::string>());
    }
  }
  if (block.contains("read_options") && block["read_options"].is_array()) {
    for (const json& option : block["read_options"]) {
      design.read_options.push_back(text_of(option));
    }
  }
  return design;
}

} /* end namespace openfpga */
