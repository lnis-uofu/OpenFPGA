/********************************************************************
 * Device packages (see device_package.h). The checks and their messages are
 * those of the device packages of the MCP server, whose tests they pass
 *******************************************************************/
#include "device_package.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <map>
#include <regex>
#include <set>
#include <sstream>
#include <system_error>

#include "nlohmann/json.hpp"
#include "picosha2.h"
#include "pugixml.hpp"

/* begin namespace openfpga */
namespace openfpga {

namespace fs = std::filesystem;
using json = nlohmann::ordered_json;

namespace {

const char* const MANIFEST = "device_package.json";
const char* const CHECKSUMS = "CHECKSUMS.sha256";
const int KNOWN_MAJOR = 1;
const size_t README_CHARS = 1200;

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

/* The names of the fixed layouts of a VPR architecture */
std::vector<std::string> layouts(const fs::path& vpr_arch) {
  pugi::xml_document document;
  const pugi::xml_parse_result result =
    document.load_file(vpr_arch.string().c_str());
  if (!result) {
    throw DevicePackageError(
      std::string("The VPR architecture of the package is not readable: ") +
      result.description());
  }
  std::vector<std::string> names;
  for (const pugi::xpath_node& node : document.select_nodes("//fixed_layout")) {
    names.push_back(node.node().attribute("name").as_string());
  }
  return names;
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

} /* end anonymous namespace */

std::vector<fs::path> DevicePackage::files_of(const std::string& role) const {
  std::vector<fs::path> paths;
  for (const DevicePackageFile& file : files) {
    if (role == file.role) {
      paths.push_back(directory / file.path);
    }
  }
  return paths;
}

DevicePackage load_device_package(const fs::path& given,
                                  const bool verify_checksums) {
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
  /* First: a file that was changed is the reason, whatever else follows */
  if (verify_checksums) {
    verify(directory, name);
  }
  const json files = member(manifest, "files");
  if (!files.is_array()) {
    throw invalid(name, "it has no list of files");
  }
  DevicePackage package;
  std::map<std::string, std::vector<std::string>> roles;
  std::set<std::string> listed;
  for (const json& entry : files) {
    if (!entry.is_object()) {
      throw invalid(name, "an entry of files is not an object");
    }
    const std::string path = inside(directory, member(entry, "path"), name);
    const json role = member(entry, "role");
    const std::string role_text = role.is_null() ? "" : text_of(role);
    listed.insert(path);
    package.files.push_back({role_text, path});
    roles[role_text].push_back(path);
  }
  for (const char* role : {"vpr_arch", "openfpga_arch"}) {
    if (1 != roles[role].size()) {
      throw invalid(
        name,
        std::string("it must list exactly one file with the role ") + role);
    }
  }
  std::set<std::string> present;
  for (const std::string& file : files_under(directory, true)) {
    if (file != MANIFEST && file != "README.md" && file != CHECKSUMS) {
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
  json device = member(manifest, "device");
  if (falsy(device)) {
    device = json::object();
  }
  json grid = member(device, "grid");
  long long width = 0;
  long long height = 0;
  if (!to_integer(member(grid, "width"), width) ||
      !to_integer(member(grid, "height"), height)) {
    throw invalid(name, "device.grid needs a width and a height");
  }
  package.layout = std::to_string(width) + "x" + std::to_string(height);
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
  const fs::path vpr_arch = directory / roles["vpr_arch"][0];
  const std::vector<std::string> names = layouts(vpr_arch);
  if (names.end() == std::find(names.begin(), names.end(), package.layout)) {
    throw invalid(name, "its grid is " + package.layout +
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
  package.openfpga_arch = directory / roles["openfpga_arch"][0];
  long long number = 0;
  if (member(device, "lut_size").is_number_integer() &&
      to_integer(member(device, "lut_size"), number)) {
    package.lut_size = number;
  }
  if (member(device, "channel_width").is_number_integer() &&
      to_integer(member(device, "channel_width"), number)) {
    package.channel_width = number;
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
  return package;
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
          key, "the layout is the grid of the package, " + package.layout);
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
  data["lut_size"] = package.lut_size ? json(*package.lut_size) : json();
  data["tags"] = package.tags;
  data["flat_routing"] = package.flat_routing;
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
    files.push_back({{"role", file.role},
                     {"path", file.path},
                     {"bytes", error ? 0 : static_cast<long long>(bytes)}});
  }
  data["files"] = files;
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
