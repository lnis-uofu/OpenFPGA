/********************************************************************
 * Device packages (see device_package.h). The checks and their messages are
 * those of the device packages of the MCP server, whose tests they pass
 *******************************************************************/
#include "device_package.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <system_error>

#include "netlist_symbols_check.h"
#include "nlohmann/json.hpp"
#include "picosha2.h"
#include "pugixml.hpp"
#include "vtr_version.h"

/* begin namespace openfpga */
namespace openfpga {

namespace fs = std::filesystem;
using json = nlohmann::ordered_json;

namespace {

const char* const MANIFEST = "device_package.json";
const char* const CHECKSUMS = "CHECKSUMS.sha256";
const int KNOWN_MAJOR = 1;
const size_t README_CHARS = 1200;
const char* const LICENSE = "LICENSE";
const char* const NETLIST_SYMBOLS = "views/netlist_symbols.svg";

/* The types of R-20. A type that starts with x_ is the package's own */
const std::set<std::string>& known_types() {
  static const std::set<std::string> types = {"vpr_arch",
                                              "openfpga_arch",
                                              "bitstream_setting",
                                              "simulation_setting",
                                              "repack_constraints",
                                              "pin_constraints",
                                              "fabric_key",
                                              "tile_config",
                                              "pcf_config",
                                              "io_info",
                                              "io_map",
                                              "odc_defaults",
                                              "yosys_script",
                                              "yosys_cell_sim",
                                              "yosys_adder_map",
                                              "yosys_dsp_map",
                                              "yosys_ff_map",
                                              "yosys_bram_map",
                                              "yosys_map",
                                              "yosys_bram_rules",
                                              "sdc",
                                              "timing_data",
                                              "timing_lib",
                                              "timing_sdf",
                                              "timing_table",
                                              "power_data",
                                              "corner_info",
                                              "rr_graph",
                                              "router_lookahead",
                                              "place_delay_lookup",
                                              "fabric_bitstream_paths",
                                              "netlist_symbols",
                                              "reference_design"};
  return types;
}

/* The types that a package has at most once (for each corner) */
const std::set<std::string>& single_types() {
  static const std::set<std::string> types = {"yosys_script",
                                              "yosys_cell_sim",
                                              "yosys_adder_map",
                                              "yosys_dsp_map",
                                              "yosys_ff_map",
                                              "yosys_bram_map",
                                              "yosys_bram_rules",
                                              "pcf_config",
                                              "timing_data",
                                              "rr_graph",
                                              "router_lookahead",
                                              "place_delay_lookup",
                                              "fabric_bitstream_paths",
                                              "netlist_symbols"};
  return types;
}

std::string sha256_of(const std::string& text) {
  return picosha2::hash256_hex_string(text.begin(), text.end());
}

DevicePackageError invalid(const std::string& name, const std::string& why) {
  return DevicePackageError("The device package '" + name +
                            "' cannot be used: " + why);
}

bool valid_name(const std::string& name) {
  static const std::regex pattern("^[a-z][a-z0-9_]*$");
  return std::regex_match(name, pattern);
}

std::string read_file(const fs::path& path) {
  std::ifstream stream(path, std::ios::binary);
  if (!stream) {
    throw std::runtime_error("cannot open '" + path.string() + "'");
  }
  return std::string(std::istreambuf_iterator<char>(stream),
                     std::istreambuf_iterator<char>());
}

/* A value as Python's str() writes it, for the messages */
std::string text_of(const json& value) {
  if (value.is_null()) {
    return "None";
  }
  if (value.is_string()) {
    return value.get<std::string>();
  }
  if (value.is_boolean()) {
    return value.get<bool>() ? "True" : "False";
  }
  return value.dump();
}

/* Whether a value is false for Python (missing, null, "", 0, [], {}) */
bool falsy(const json& value) {
  if (value.is_null()) {
    return true;
  }
  if (value.is_boolean()) {
    return !value.get<bool>();
  }
  if (value.is_number()) {
    return 0 == value.get<double>();
  }
  return value.empty();
}

/* The member of an object, or null */
json member(const json& object, const char* key) {
  if (!object.is_object()) {
    return json();
  }
  const auto found = object.find(key);
  return object.end() == found ? json() : *found;
}

/* The files under a directory, as paths inside it with '/', sorted */
std::vector<std::string> files_under(const fs::path& root, const bool links) {
  std::vector<std::string> found;
  std::error_code error;
  for (fs::recursive_directory_iterator it(root, error), end;
       !error && it != end; it.increment(error)) {
    const fs::path& path = it->path();
    if (fs::is_regular_file(path, error) ||
        (links && fs::is_symlink(fs::symlink_status(path, error)))) {
      found.push_back(path.lexically_relative(root).generic_string());
    }
  }
  std::sort(found.begin(), found.end());
  return found;
}

bool has_parent_part(const fs::path& path) {
  for (const fs::path& part : path) {
    if (".." == part.string()) {
      return true;
    }
  }
  return false;
}

/* Whether a path is inside a directory, after the links are followed */
bool resolves_inside(const fs::path& root, const fs::path& target) {
  std::error_code error;
  const fs::path base = fs::canonical(root, error);
  const fs::path real = fs::canonical(target, error);
  if (error) {
    return false;
  }
  const fs::path relative = real.lexically_relative(base);
  return !relative.empty() && "." != relative.string() &&
         !has_parent_part(relative);
}

/* A path of the manifest: a file inside the package. Returns the path inside
 * the package, with '/' */
std::string inside(const fs::path& root, const json& relative,
                   const std::string& name) {
  if (!relative.is_string() || relative.get<std::string>().empty() ||
      std::string::npos != relative.get<std::string>().find('\0')) {
    throw invalid(name, "a path in the manifest is not text");
  }
  const std::string text = relative.get<std::string>();
  const fs::path path(text);
  if (path.is_absolute() || !path.root_path().empty() ||
      has_parent_part(path) || std::string::npos != text.find('\\')) {
    throw invalid(name, "the path '" + text + "' is not inside the package");
  }
  const fs::path target = root / path;
  std::error_code error;
  if (fs::is_symlink(fs::symlink_status(target, error)) ||
      !fs::is_regular_file(target, error)) {
    throw invalid(name, "the file '" + text + "' is missing or is a link");
  }
  if (!resolves_inside(root, target)) {
    throw invalid(name, "the file '" + text + "' leads outside the package");
  }
  return path.lexically_normal().generic_string();
}

/* CHECKSUMS.sha256 lists every file with its SHA-256, and they agree */
void verify(const fs::path& directory, const std::string& name) {
  const fs::path sums = directory / CHECKSUMS;
  std::error_code error;
  if (!fs::is_regular_file(sums, error)) {
    throw invalid(name, std::string("it has no ") + CHECKSUMS);
  }
  std::set<std::string> seen;
  std::istringstream lines(read_file(sums));
  std::string line;
  while (std::getline(lines, line)) {
    if (!line.empty() && '\r' == line.back()) {
      line.pop_back();
    }
    const size_t gap = line.find("  ");
    const std::string digest = line.substr(0, gap);
    const std::string relative =
      std::string::npos == gap ? std::string() : line.substr(gap + 2);
    const fs::path target = directory / relative;
    if (relative.empty() || has_parent_part(fs::path(relative)) ||
        !fs::is_regular_file(target, error)) {
      throw invalid(name, std::string(CHECKSUMS) + " names '" + relative +
                            "', which is not a file");
    }
    const std::string content = read_file(target);
    if (picosha2::hash256_hex_string(content.begin(), content.end()) !=
        digest) {
      throw invalid(name, "the file '" + relative + "' was changed (checksum)");
    }
    seen.insert(fs::path(relative).lexically_normal().generic_string());
  }
  for (const std::string& file : files_under(directory, false)) {
    if (CHECKSUMS != fs::path(file).filename().string() && !seen.count(file)) {
      throw invalid(name,
                    std::string(CHECKSUMS) + " does not list '" + file + "'");
    }
  }
}


/* The variables ${name} of a text, in the order they first appear */
std::vector<std::string> variables_of(const std::string& text) {
  static const std::regex variable("\\$\\{([^}\\s]*)\\}");
  std::vector<std::string> names;
  std::set<std::string> seen;
  for (std::sregex_iterator it(text.begin(), text.end(), variable), end;
       it != end; ++it) {
    if (seen.insert((*it)[1].str()).second) {
      names.push_back((*it)[1].str());
    }
  }
  return names;
}

/* The factor from a unit of timing_units to the unit of VPR, or 0 */
double unit_factor(const std::string& kind, const std::string& unit) {
  static const std::map<std::string, std::map<std::string, double>> factors = {
    {"time", {{"ns", 1e-9}, {"ps", 1e-12}, {"fs", 1e-15}}},
    {"capacitance", {{"pF", 1e-12}, {"fF", 1e-15}}},
    {"resistance", {{"ohm", 1.0}, {"kohm", 1e3}}}};
  const auto units = factors.find(kind);
  if (factors.end() == units) {
    return 0;
  }
  const auto factor = units->second.find(unit);
  return units->second.end() == factor ? 0 : factor->second;
}

/* The values of one corner (R-22 to R-24), in the units of VPR. The name of a
 * variable ends in _T, _C or _R, which says its kind */
std::map<std::string, double> read_timing_data(
  const std::string& name, const fs::path& file, const std::string& corner,
  const json& units, const std::vector<std::string>& variables) {
  const std::string where = "the timing data of corner '" + corner + "'";
  json data;
  try {
    data = json::parse(read_file(file));
  } catch (const std::exception& reason) {
    throw invalid(name, where + " is not readable (" + reason.what() + ")");
  }
  if (!data.is_object()) {
    throw invalid(name, where + " is not a map of names to numbers");
  }
  std::map<std::string, double> values;
  for (const auto& item : data.items()) {
    const std::string& key = item.key();
    if (!item.value().is_number()) {
      throw invalid(name, where + " has a value that is not a number: " + key);
    }
    const char kind = key.empty() ? 0 : key.back();
    const std::string kind_name = 'T' == kind   ? "time"
                                  : 'C' == kind ? "capacitance"
                                  : 'R' == kind ? "resistance"
                                                : "";
    if (key.size() < 3 || '_' != key[key.size() - 2] || kind_name.empty()) {
      throw invalid(name, "the variable '" + key +
                            "' does not end in _T, _C or _R");
    }
    const json unit = member(units, kind_name.c_str());
    const double factor =
      unit.is_string() ? unit_factor(kind_name, unit.get<std::string>()) : 0;
    if (0 == factor) {
      throw invalid(name, "timing_units has no known unit for " + kind_name);
    }
    values[key] = item.value().get<double>() * factor;
  }
  for (const std::string& variable : variables) {
    if (!values.count(variable)) {
      throw invalid(name, where + " has no value for the variable '" +
                            variable + "'");
    }
  }
  return values;
}

/* The fixed layouts of a VPR architecture: name, width and height */
std::vector<std::pair<std::string, std::pair<long long, long long>>> layouts(
  const fs::path& vpr_arch) {
  pugi::xml_document document;
  const pugi::xml_parse_result result =
    document.load_file(vpr_arch.string().c_str());
  if (!result) {
    throw DevicePackageError(
      std::string("The VPR architecture of the package is not readable: ") +
      result.description());
  }
  std::vector<std::pair<std::string, std::pair<long long, long long>>> found;
  for (const pugi::xpath_node& node : document.select_nodes("//fixed_layout")) {
    found.push_back({node.node().attribute("name").as_string(),
                     {node.node().attribute("width").as_llong(),
                      node.node().attribute("height").as_llong()}});
  }
  return found;
}

/* An integer as Python's int() reads it from a number or a text */
bool to_integer(const json& value, long long& out) {
  if (value.is_boolean()) {
    out = value.get<bool>() ? 1 : 0;
    return true;
  }
  if (value.is_number()) {
    out = static_cast<long long>(value.get<double>());
    if (value.is_number_integer()) {
      out = value.get<long long>();
    }
    return true;
  }
  if (value.is_string()) {
    std::string text = value.get<std::string>();
    const auto space = [](unsigned char c) { return std::isspace(c); };
    text.erase(text.begin(), std::find_if_not(text.begin(), text.end(), space));
    text.erase(std::find_if_not(text.rbegin(), text.rend(), space).base(),
               text.end());
    static const std::regex digits("^[+-]?[0-9]+$");
    if (!std::regex_match(text, digits)) {
      return false;
    }
    try {
      out = std::stoll(text);
    } catch (const std::exception&) {
      return false;
    }
    return true;
  }
  return false;
}

/* A value of flow_defaults.vpr_options. False when it is not text, a number
 * or true/false */
bool to_value(const json& value, DevicePackageValue& out) {
  if (value.is_boolean()) {
    out.kind = DevicePackageValue::BOOLEAN;
    out.boolean = value.get<bool>();
    out.text = out.boolean ? "true" : "false";
    out.literal = out.boolean ? "True" : "False";
    out.number = out.boolean ? 1 : 0;
    return true;
  }
  if (value.is_number()) {
    out.kind = value.is_number_float() ? DevicePackageValue::REAL
                                       : DevicePackageValue::INTEGER;
    out.number = value.get<double>();
    out.text = value.dump();
    out.literal = out.text;
    return true;
  }
  if (value.is_string()) {
    out.kind = DevicePackageValue::TEXT;
    out.text = value.get<std::string>();
    out.literal = "'" + out.text + "'";
    return true;
  }
  return false;
}

json value_json(const DevicePackageValue& value) {
  switch (value.kind) {
    case DevicePackageValue::BOOLEAN:
      return json(value.boolean);
    case DevicePackageValue::INTEGER:
      return json::parse(value.text);
    case DevicePackageValue::REAL:
      return json(value.number);
    case DevicePackageValue::TEXT:
    default:
      return json(value.text);
  }
}

/* The first characters of a text, cut on a character of UTF-8 */
std::string first_chars(const std::string& text, const size_t count,
                        bool& cut) {
  size_t chars = 0;
  for (size_t i = 0; i < text.size(); ++i) {
    if (0x80 != (static_cast<unsigned char>(text[i]) & 0xC0)) {
      if (chars == count) {
        cut = true;
        return text.substr(0, i);
      }
      ++chars;
    }
  }
  cut = false;
  return text;
}

/* flow_defaults.bitstream (spec_bitstream.md B-10): flags without a value,
 * and the fixups that build_bitstream knows. Anything else is refused */
void read_bitstream_recipe(const std::string& name, const json& recipe,
                           DevicePackage& package) {
  if (falsy(recipe)) {
    return;
  }
  const std::string what =
    "flow_defaults.bitstream has link_options and fabric_options (flags such "
    "as --compress_routing) and fixups (check_netlist_naming_conflict, "
    "pb_pin_fixup, lut_truth_table_fixup)";
  if (!recipe.is_object()) {
    throw invalid(name, what);
  }
  static const std::regex flag("--[A-Za-z0-9_]+");
  static const std::set<std::string> fixups = {
    "check_netlist_naming_conflict", "pb_pin_fixup", "lut_truth_table_fixup"};
  for (const auto& item : recipe.items()) {
    std::vector<std::string>* list = nullptr;
    if ("link_options" == item.key()) {
      list = &package.bitstream_link_options;
    } else if ("fabric_options" == item.key()) {
      list = &package.bitstream_fabric_options;
    } else if ("fixups" == item.key()) {
      list = &package.bitstream_fixups;
    } else {
      throw invalid(name, what + ", not '" + item.key() + "'");
    }
    if (!item.value().is_array()) {
      throw invalid(name, what);
    }
    for (const json& value : item.value()) {
      const std::string text =
        value.is_string() ? value.get<std::string>() : "";
      const bool fixup = list == &package.bitstream_fixups;
      if (fixup ? 0 == fixups.count(text) : !std::regex_match(text, flag)) {
        throw invalid(name,
                      what + ", not '" + value.dump() + "' in " + item.key());
      }
      list->push_back(text);
    }
  }
  package.has_bitstream_recipe = true;
  if (!recipe.contains("link_options")) {
    package.bitstream_link_options = {"--sort_gsb_chan_node_in_edges"};
  }
  if (!recipe.contains("fabric_options")) {
    package.bitstream_fabric_options = {"--compress_routing"};
  }
}

/* The cached files of the device (R-27): the ones of the corner in use (or of
 * every corner, in a package without corners), and whether they can be read.
 * The key is made of the files that they were made from: the VPR
 * architecture, the timing data of every corner and the channel width */
void read_cache(const std::string& name, const json& manifest,
                const std::map<std::string, std::string>& timing_files,
                DevicePackage& package) {
  for (const DevicePackageFile& file : package.files) {
    bool cached_type = false;
    for (const auto& type : cached_file_types()) {
      cached_type = cached_type || type.first == file.type;
    }
    if (!cached_type) {
      continue;
    }
    if (!package.corners.empty() && file.corner.empty()) {
      throw invalid(name, "the file '" + file.path + "' of the type " +
                            file.type + " has no corner (R-27)");
    }
    if (file.corner == package.corner) {
      package.cached[file.type] = package.directory / file.path;
    }
  }
  std::string key_text =
    "channel_width=" + std::to_string(package.channel_width.value_or(0)) +
    "\nvpr_arch=" + sha256_of(read_file(package.vpr_arch)) + "\n";
  for (const auto& timing : timing_files) {
    key_text += "timing_data." + timing.first + "=" +
                sha256_of(read_file(package.directory / timing.second)) + "\n";
  }
  package.cache_key = sha256_of(key_text);
  const json cache = member(manifest, "cache");
  const json version = member(cache, "vtr_version");
  const json key = member(cache, "key");
  package.cache_vtr_version =
    version.is_string() ? version.get<std::string>() : "";
  package.cache_key_given = key.is_string() ? key.get<std::string>() : "";
  if (package.cached.empty()) {
    return;
  }
  if (package.cache_vtr_version.empty() || package.cache_key_given.empty()) {
    throw invalid(name,
                  "it has cached files and no cache.vtr_version and "
                  "cache.key (R-27)");
  }
  if (package.cache_vtr_version != vtr_build_version()) {
    package.cache_reason = "they were made by VTR " +
                           package.cache_vtr_version + ", and this is VTR " +
                           vtr_build_version();
  } else if (package.cache_key_given != package.cache_key) {
    package.cache_reason =
      "they were made from other files (cache.key): the VPR architecture, the "
      "timing data or the channel width changed since";
  } else {
    package.cache_usable = true;
  }
}

} /* end anonymous namespace */

const std::vector<std::pair<std::string, std::string>>& cached_file_types() {
  static const std::vector<std::pair<std::string, std::string>> types = {
    {"rr_graph", "filename.read_rr_graph_file"},
    {"router_lookahead", "filename.read_router_lookahead"},
    {"place_delay_lookup", "filename.read_placement_delay_lookup"}};
  return types;
}

std::string vtr_build_version() { return vtr::VERSION; }

std::vector<fs::path> DevicePackage::files_of(const std::string& type) const {
  std::vector<fs::path> paths;
  for (const DevicePackageFile& file : files) {
    if (type == file.type) {
      paths.push_back(directory / file.path);
    }
  }
  return paths;
}

DevicePackage load_device_package(const fs::path& given,
                                  const bool verify_checksums,
                                  const std::string& corner) {
  std::error_code error;
  fs::path directory = fs::absolute(given, error).lexically_normal();
  if (directory.has_filename() == false) {
    directory = directory.parent_path();
  }
  const std::string name = directory.filename().string();
  json manifest;
  try {
    manifest = json::parse(read_file(directory / MANIFEST));
  } catch (const std::exception& reason) {
    throw invalid(name, std::string("its ") + MANIFEST + " is not readable (" +
                          reason.what() + ")");
  }
  if (!manifest.is_object()) {
    throw invalid(name, std::string("its ") + MANIFEST + " is not an object");
  }
  const json format = member(manifest, "format_version");
  const std::string version = format.is_null() ? "" : text_of(format);
  static const std::regex major_minor("^([0-9]+)\\.[0-9]+$");
  std::smatch parts;
  if (!std::regex_match(version, parts, major_minor) ||
      std::stoll(parts[1].str()) > KNOWN_MAJOR) {
    throw invalid(
      name, "format_version '" + version + "' is not one that is known (1.x)");
  }
  const json manifest_name = member(manifest, "name");
  if (!manifest_name.is_string() || manifest_name.get<std::string>() != name ||
      !valid_name(name)) {
    throw invalid(name, "its name '" + text_of(manifest_name) +
                          "' is not the name of its directory");
  }
  if (falsy(member(manifest, "license"))) {
    throw invalid(name, "it has no license");
  }
  std::error_code license_error;
  if (fs::is_symlink(fs::symlink_status(directory / LICENSE, license_error)) ||
      !fs::is_regular_file(directory / LICENSE, license_error)) {
    throw invalid(name, std::string("it has no ") + LICENSE +
                          " file with the text of its license (R-9)");
  }
  /* First: a file that was changed is the reason, whatever else follows */
  if (verify_checksums) {
    verify(directory, name);
  }
  const json files = member(manifest, "files");
  if (!files.is_array()) {
    throw invalid(name, "it has no list of files");
  }
  DevicePackage package;
  std::map<std::string, std::vector<std::string>> types;
  std::set<std::string> listed;
  std::set<std::pair<std::string, std::string>> singles;
  for (const json& entry : files) {
    if (!entry.is_object()) {
      throw invalid(name, "an entry of files is not an object");
    }
    const std::string path = inside(directory, member(entry, "path"), name);
    if (!member(entry, "role").is_null()) {
      throw invalid(
        name, "the file '" + path + "' has a role: the field is type (R-20)");
    }
    const json type = member(entry, "type");
    const std::string type_text = type.is_string() ? text_of(type) : "";
    if (0 != type_text.rfind("x_", 0) && !known_types().count(type_text)) {
      throw invalid(name, "the file '" + path + "' has the type '" + type_text +
                            "', which is not one of R-20 (nor x_...)");
    }
    const json corner_of = member(entry, "corner");
    const std::string corner_text =
      corner_of.is_string() ? corner_of.get<std::string>() : "";
    if (single_types().count(type_text) &&
        !singles.insert({type_text, corner_text}).second) {
      throw invalid(
        name, "it lists more than one file of the type " + type_text +
                (corner_text.empty() ? std::string()
                                     : " for corner '" + corner_text + "'"));
    }
    listed.insert(path);
    package.files.push_back({type_text, path, corner_text});
    types[type_text].push_back(path);
  }
  for (const char* type : {"vpr_arch", "openfpga_arch"}) {
    if (1 != types[type].size()) {
      throw invalid(
        name,
        std::string("it must list exactly one file with the type ") + type);
    }
  }
  std::set<std::string> present;
  for (const std::string& file : files_under(directory, true)) {
    if (file != MANIFEST && file != "README.md" && file != CHECKSUMS &&
        file != LICENSE) {
      present.insert(file);
    }
  }
  if (present != listed) {
    std::vector<std::string> extra;
    std::set_difference(present.begin(), present.end(), listed.begin(),
                        listed.end(), std::back_inserter(extra));
    std::set_difference(listed.begin(), listed.end(), present.begin(),
                        present.end(), std::back_inserter(extra));
    std::string text;
    for (size_t i = 0; i < extra.size() && i < 3; ++i) {
      text += (0 == i ? "" : ", ") + extra[i];
    }
    throw invalid(name, "the files and the manifest differ: " + text);
  }
  /* The netlist symbols of the views (docs/plan_netlist_symbols.md): one
   * file, views/netlist_symbols.svg, in the subset that the app reads */
  for (const std::string& path : types["netlist_symbols"]) {
    if (path != NETLIST_SYMBOLS) {
      throw invalid(name, "the file of the netlist symbols is " +
                            std::string(NETLIST_SYMBOLS) + ", not '" + path +
                            "'");
    }
    std::ifstream in(directory / path, std::ios::binary);
    const std::string text((std::istreambuf_iterator<char>(in)),
                           std::istreambuf_iterator<char>());
    const std::string problem = check_netlist_symbols(text);
    if (!problem.empty()) {
      throw invalid(
        name, "its netlist symbols (" + path + ") are refused: " + problem);
    }
  }
  json device = member(manifest, "device");
  if (falsy(device)) {
    device = json::object();
  }
  if (!member(device, "grid").is_null()) {
    throw invalid(name,
                  "device.grid is replaced by device.layout, the name of a "
                  "fixed layout of the VPR architecture (R-7)");
  }
  const json layout = member(device, "layout");
  if (!layout.is_string() || layout.get<std::string>().empty()) {
    throw invalid(name,
                  "device.layout names a fixed layout of the VPR "
                  "architecture (R-7)");
  }
  package.layout = layout.get<std::string>();
  json flow_defaults = member(manifest, "flow_defaults");
  json options =
    falsy(flow_defaults) ? json() : member(flow_defaults, "vpr_options");
  if (!falsy(flow_defaults) && !flow_defaults.is_object()) {
    options = json::array({json::array()}); /* not a map: refused below */
  }
  if (!falsy(options)) {
    bool valid = options.is_object();
    if (valid) {
      for (const auto& item : options.items()) {
        DevicePackageValue value;
        if (!to_value(item.value(), value)) {
          valid = false;
          break;
        }
        package.vpr_options.emplace_back(item.key(), value);
      }
    }
    if (!valid) {
      throw invalid(
        name, "flow_defaults.vpr_options maps the flags of VPR to values");
    }
  }
  std::sort(package.vpr_options.begin(), package.vpr_options.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });
  const json flat_routing = member(device, "flat_routing");
  if (!flat_routing.is_null() && !flat_routing.is_boolean()) {
    throw invalid(name, "device.flat_routing is true or false");
  }
  package.flat_routing = flat_routing.is_boolean() && flat_routing.get<bool>();
  const json bitstream = member(device, "bitstream");
  if (!bitstream.is_null() && !bitstream.is_boolean()) {
    throw invalid(name, "device.bitstream is true or false");
  }
  package.bitstream = !bitstream.is_boolean() || bitstream.get<bool>();
  read_bitstream_recipe(
    name, falsy(flow_defaults) ? json() : member(flow_defaults, "bitstream"),
    package);
  /* The corners (R-16 to R-19) and the variables of the architecture (R-22,
   * R-23). The names of the corners and the file of each are checked; the
   * process, voltage, temperature and rc of a corner are not */
  const json corners = member(manifest, "corners");
  std::map<std::string, std::string> timing_files;
  if (!falsy(corners)) {
    const json list = member(corners, "list");
    if (!corners.is_object() || !list.is_array() || list.empty()) {
      throw invalid(name, "corners has a default and a list of corners");
    }
    for (const json& entry : list) {
      const json corner_name = member(entry, "name");
      if (!corner_name.is_string() ||
          !valid_name(corner_name.get<std::string>())) {
        throw invalid(name, "a corner has no name of [a-z][a-z0-9_]*");
      }
      package.corners.push_back(corner_name.get<std::string>());
    }
    const std::set<std::string> distinct(package.corners.begin(),
                                         package.corners.end());
    if (distinct.size() != package.corners.size()) {
      throw invalid(name, "a corner is listed twice");
    }
    const json fallback = member(corners, "default");
    if (!fallback.is_string() || !distinct.count(fallback.get<std::string>())) {
      throw invalid(name, "corners.default is not one of the corners");
    }
    if (!corner.empty() && !distinct.count(corner)) {
      throw invalid(name, "corner '" + corner + "' is not in the package");
    }
    package.corner = corner.empty() ? fallback.get<std::string>() : corner;
    for (const json& entry : files) {
      const json entry_corner = member(entry, "corner");
      const std::string path = entry["path"].get<std::string>();
      if (entry_corner.is_null()) {
        continue;
      }
      if (!entry_corner.is_string() ||
          !distinct.count(entry_corner.get<std::string>())) {
        throw invalid(name, "the file '" + path + "' is of a corner that is "
                                                  "not in the package");
      }
      const std::string dir = "corners/" + entry_corner.get<std::string>() + "/";
      if (0 != fs::path(path).lexically_normal().generic_string().rfind(dir, 0)) {
        throw invalid(name, "the file '" + path + "' of corner '" +
                              entry_corner.get<std::string>() +
                              "' is not in " + dir);
      }
      if ("timing_data" == member(entry, "type")) {
        timing_files[entry_corner.get<std::string>()] = path;
      }
    }
    for (const std::string& dir : files_under(directory / "corners", false)) {
      const std::string first = dir.substr(0, dir.find('/'));
      if (!distinct.count(first)) {
        throw invalid(name, "the directory corners/" + first +
                              " is not a corner of the manifest");
      }
    }
  } else if (!corner.empty()) {
    throw invalid(name, "it has no corners, and corner '" + corner +
                          "' was asked for");
  }
  const fs::path vpr_arch = directory / types["vpr_arch"][0];
  const std::vector<std::string> variables = variables_of(read_file(vpr_arch));
  package.has_variables = !variables.empty();
  if (package.has_variables && package.corners.empty()) {
    throw invalid(name, "its VPR architecture has variables (" + variables[0] +
                          ") and the package has no corners");
  }
  if (package.has_variables) {
    const json units = member(manifest, "timing_units");
    if (!units.is_object()) {
      throw invalid(name, "it has timing data and no timing_units");
    }
    for (const std::string& each : package.corners) {
      if (!timing_files.count(each)) {
        throw invalid(name, "corner '" + each +
                              "' has no file of the type "
                              "timing_data");
      }
      std::map<std::string, double> values = read_timing_data(
        name, directory / timing_files[each], each, units, variables);
      if (each == package.corner) {
        package.timing_values = std::move(values);
      }
    }
  }
  bool layout_found = false;
  for (const auto& found : layouts(vpr_arch)) {
    if (found.first == package.layout) {
      layout_found = true;
      package.grid_width = found.second.first;
      package.grid_height = found.second.second;
    }
  }
  if (!layout_found) {
    throw invalid(name, "its layout is " + package.layout +
                          " and the VPR architecture has no fixed layout of "
                          "that name");
  }
  package.name = name;
  package.version = text_of(member(manifest, "version").is_null()
                              ? json("")
                              : member(manifest, "version"));
  package.description = text_of(member(manifest, "description").is_null()
                                  ? json("")
                                  : member(manifest, "description"));
  package.license = text_of(member(manifest, "license"));
  package.directory = directory;
  package.vpr_arch = vpr_arch;
  package.openfpga_arch = directory / types["openfpga_arch"][0];
  long long number = 0;
  if (member(device, "lut_size").is_number_integer() &&
      to_integer(member(device, "lut_size"), number)) {
    package.lut_size = number;
  }
  if (member(device, "channel_width").is_number_integer() &&
      to_integer(member(device, "channel_width"), number)) {
    package.channel_width = number;
  }
  /* The channel width of a package is read only, and the routing runs only
   * at a fixed width: a package must give it */
  if (!package.channel_width || *package.channel_width <= 0) {
    throw invalid(name,
                  "device.channel_width is not a number of tracks above 0: "
                  "the package gives the channel width, which the routing "
                  "needs and the session cannot change");
  }
  for (const json& tag : member(device, "tags")) {
    package.tags.push_back(text_of(tag));
  }
  const json reference = member(manifest, "reference");
  package.reference = reference.is_object() ? reference.dump() : "{}";
  json yosys =
    falsy(flow_defaults) ? json() : member(flow_defaults, "yosys_options");
  if (!falsy(yosys)) {
    if (!yosys.is_object()) {
      package.yosys_options_valid = false;
    } else {
      for (const auto& list :
           {std::make_pair("read_verilog", &package.yosys_read_verilog),
            std::make_pair("dsp_map", &package.yosys_dsp_map)}) {
        const json values = member(yosys, list.first);
        if (values.is_null()) {
          continue;
        }
        if (!values.is_array()) {
          package.yosys_options_valid = false;
          continue;
        }
        for (const json& value : values) {
          list.second->push_back(text_of(value));
        }
      }
    }
  }
  read_cache(name, manifest, timing_files, package);
  return package;
}

std::string render_vpr_arch(const DevicePackage& package) {
  std::string text = read_file(package.vpr_arch);
  if (!package.has_variables) {
    return text;
  }
  static const std::regex variable("\\$\\{([^}\\s]*)\\}");
  std::string out;
  out.reserve(text.size());
  size_t from = 0;
  for (std::sregex_iterator it(text.begin(), text.end(), variable), end;
       it != end; ++it) {
    const auto value = package.timing_values.find((*it)[1].str());
    if (package.timing_values.end() == value) {
      throw invalid(package.name, "the corner '" + package.corner +
                                    "' has no value for the variable '" +
                                    (*it)[1].str() + "'");
    }
    char number[40];
    std::snprintf(number, sizeof number, "%.9g", value->second);
    out.append(text, from, it->position() - from);
    out += number;
    from = it->position() + it->length();
  }
  out.append(text, from, std::string::npos);
  return out;
}

DevicePackageOptions device_package_options(
  const DevicePackage& package,
  const std::vector<std::pair<std::string, std::string>>& flags) {
  std::map<std::string, std::string> by_flag(flags.begin(), flags.end());
  DevicePackageOptions applied;
  for (const auto& entry : package.vpr_options) {
    const std::string& key = entry.first;
    const DevicePackageValue& value = entry.second;
    /* The package gives the layout, the channel width and the format of the
     * circuit otherwise: an entry that says the same is not told */
    if ("device" == key) {
      if (!(DevicePackageValue::TEXT == value.kind &&
            value.text == package.layout)) {
        applied.ignored.emplace_back(
          key, "the layout is device.layout of the package, " + package.layout);
      }
      continue;
    }
    if ("route_chan_width" == key) {
      const bool same =
        DevicePackageValue::TEXT != value.kind && package.channel_width &&
        value.number == static_cast<double>(*package.channel_width);
      if (!same) {
        applied.ignored.emplace_back(
          key, "the channel width is device.channel_width of the package, " +
                 (package.channel_width ? std::to_string(*package.channel_width)
                                        : std::string("None")));
      }
      continue;
    }
    if ("circuit_format" == key) {
      continue;
    }
    const auto found = by_flag.find("--" + key);
    if (by_flag.end() == found) {
      applied.ignored.emplace_back(key,
                                   "not an option of VPR that the flow knows");
      continue;
    }
    applied.set.emplace_back(found->second, value);
  }
  return applied;
}

std::string device_package_json(const DevicePackage& package,
                                const DevicePackageOptions* applied) {
  json data;
  data["name"] = package.name;
  data["version"] = package.version;
  data["description"] = package.description;
  data["layout"] = package.layout;
  data["grid"] = {{"width", package.grid_width},
                  {"height", package.grid_height}};
  data["corner"] = package.corner.empty() ? json() : json(package.corner);
  data["corners"] = package.corners;
  data["lut_size"] = package.lut_size ? json(*package.lut_size) : json();
  data["tags"] = package.tags;
  data["flat_routing"] = package.flat_routing;
  data["bitstream"] = package.bitstream;
  data["bitstream_recipe"] =
    package.has_bitstream_recipe
      ? json{{"link_options", package.bitstream_link_options},
             {"fixups", package.bitstream_fixups},
             {"fabric_options", package.bitstream_fabric_options}}
      : json();
  data["license"] = package.license;
  data["channel_width"] =
    package.channel_width ? json(*package.channel_width) : json();
  json options = json::object();
  for (const auto& entry : package.vpr_options) {
    options[entry.first] = value_json(entry.second);
  }
  data["vpr_options"] = options;
  data["vpr_arch"] =
    package.vpr_arch.lexically_relative(package.directory).generic_string();
  data["openfpga_arch"] =
    package.openfpga_arch.lexically_relative(package.directory)
      .generic_string();
  json files = json::array();
  for (const DevicePackageFile& file : package.files) {
    std::error_code error;
    const auto bytes = fs::file_size(package.directory / file.path, error);
    files.push_back({{"type", file.type},
                     {"path", file.path},
                     {"bytes", error ? 0 : static_cast<long long>(bytes)}});
    if (!file.corner.empty()) {
      files.back()["corner"] = file.corner;
    }
  }
  data["files"] = files;
  json cached = json::object();
  for (const auto& entry : package.cached) {
    cached[entry.first] =
      entry.second.lexically_relative(package.directory).generic_string();
  }
  const auto text_or_null = [](const std::string& text) {
    return text.empty() ? json() : json(text);
  };
  data["cache"] = {{"files", cached},
                   {"vtr_version", text_or_null(package.cache_vtr_version)},
                   {"key", text_or_null(package.cache_key_given)},
                   {"expected_key", package.cache_key},
                   {"build_vtr_version", vtr_build_version()},
                   {"usable", package.cache_usable},
                   {"reason", text_or_null(package.cache_reason)}};
  const json reference = json::parse(package.reference);
  if (reference.empty()) {
    data["reference"] = json();
  } else {
    data["reference"] = {{"design", member(reference, "design")},
                         {"top", member(reference, "top")},
                         {"expected", member(reference, "expected")}};
  }
  std::string readme;
  std::error_code error;
  if (fs::is_regular_file(package.directory / "README.md", error)) {
    readme = read_file(package.directory / "README.md");
  }
  bool cut = false;
  data["readme"] = first_chars(readme, README_CHARS, cut);
  data["readme_cut"] = cut;
  data["directory"] = package.directory.generic_string();
  if (nullptr != applied) {
    json set = json::object();
    for (const auto& entry : applied->set) {
      set[entry.first] = value_json(entry.second);
    }
    json ignored = json::object();
    for (const auto& entry : applied->ignored) {
      ignored[entry.first] = entry.second;
    }
    data["options_set"] = set;
    data["options_not_set"] = ignored;
  }
  return data.dump(-1, ' ', false, json::error_handler_t::replace);
}

} /* end namespace openfpga */
