/********************************************************************
 * The hierarchy of a design from the JSON of Yosys (see design_hierarchy.h)
 *******************************************************************/
#include "design_hierarchy.h"

#include <algorithm>
#include <cmath>
#include <deque>
#include <map>
#include <regex>
#include <set>
#include <stdexcept>
#include <vector>

/* begin namespace openfpga */
namespace openfpga {

using json = nlohmann::json;
using ordered = nlohmann::ordered_json;

namespace {

/* A value of a parameter of Yosys (s32'00..01000) as a number when it is
 * one, or as it is written */
ordered parameter_value(const std::string& text) {
  static const std::regex bits("^(s?)([0-9]+)'([01]+)$");
  std::smatch found;
  if (!std::regex_match(text, found, bits) || found[3].length() > 63) {
    return ordered(text);
  }
  const std::string digits = found[3].str();
  long long value = 0;
  for (const char c : digits) {
    value = value * 2 + ('1' == c ? 1 : 0);
  }
  if (!found[1].str().empty() && !digits.empty() && '1' == digits[0]) {
    value -= 1LL << digits.size();
  }
  return ordered(value);
}

/* An attribute of Yosys that is a number (1, written in binary) */
bool attribute_set(const json& attributes, const char* name) {
  if (!attributes.is_object() || !attributes.contains(name)) {
    return false;
  }
  const json& value = attributes[name];
  if (value.is_number()) {
    return 0 != value.get<long long>();
  }
  return value.is_string() &&
         std::string::npos != value.get<std::string>().find('1');
}

/* Where the RTL has it (the attribute src, file:line.column-line.column),
 * or null */
ordered source_of(const json& attributes) {
  if (!attributes.is_object() || !attributes.contains("src") ||
      !attributes["src"].is_string()) {
    return ordered();
  }
  std::string src = attributes["src"].get<std::string>();
  src = src.substr(0, src.find('|'));
  static const std::regex where("^(.*):([0-9]+)(\\.[0-9]+)?(-.*)?$");
  std::smatch found;
  if (!std::regex_match(src, found, where)) {
    return ordered();
  }
  ordered answer;
  answer["file"] = found[1].str();
  answer["line"] = std::stoi(found[2].str());
  return answer;
}

struct Node {
  std::string instance;
  std::string type;
  ordered src; /* where the RTL instantiates it: the src of its cell */
  std::vector<size_t> children;
  size_t more = 0;
};

/* The instances of a module that are not cells of Yosys, by name */
std::vector<std::pair<std::string, std::string>> instances_of(
  const json& module, const json& modules) {
  std::vector<std::pair<std::string, std::string>> found;
  if (!module.contains("cells") || !module["cells"].is_object()) {
    return found;
  }
  for (auto it = module["cells"].begin(); it != module["cells"].end(); ++it) {
    const std::string type = it.value().value("type", std::string());
    if (!type.empty() && ('$' != type[0] || modules.contains(type))) {
      found.emplace_back(it.key(), type);
    }
  }
  std::sort(found.begin(), found.end());
  return found;
}

/* The cells of Yosys of a module (its logic, not its instances) */
size_t cells_of(const json& module, const json& modules) {
  size_t count = 0;
  if (module.contains("cells") && module["cells"].is_object()) {
    for (const auto& cell : module["cells"]) {
      const std::string type = cell.value("type", std::string());
      count += !type.empty() && '$' == type[0] && !modules.contains(type);
    }
  }
  return count;
}

} /* end anonymous namespace */

ordered module_name(const std::string& given) {
  std::string name = given;
  if (!name.empty() && '\\' == name[0]) {
    name.erase(0, 1);
  }
  ordered answer;
  answer["module"] = name;
  answer["parameters"] = ordered::object();
  if (0 != name.rfind("$paramod", 0)) {
    return answer;
  }
  /* $paramod\sub\A=..\B=.., or $paramod$hash\sub when the names are long */
  std::vector<std::string> parts;
  size_t start = 0;
  while (true) {
    const size_t next = name.find('\\', start);
    parts.push_back(name.substr(start, next - start));
    if (std::string::npos == next) {
      break;
    }
    start = next + 1;
  }
  if (parts.size() < 2 || parts[1].empty()) {
    return answer;
  }
  answer["module"] = parts[1];
  for (size_t i = 2; i < parts.size(); ++i) {
    const size_t eq = parts[i].find('=');
    if (std::string::npos == eq) {
      continue;
    }
    answer["parameters"][parts[i].substr(0, eq)] =
      parameter_value(parts[i].substr(eq + 1));
  }
  return answer;
}

ordered hierarchy_of(const json& yosys, const std::string& top,
                     const size_t limit) {
  const json modules =
    yosys.contains("modules") ? yosys["modules"] : json::object();
  if (!modules.is_object() || !modules.contains(top)) {
    throw std::runtime_error("Yosys wrote no module '" + top + "'");
  }
  const auto kind_of = [&](const std::string& type) -> std::string {
    if (!modules.contains(type)) {
      return "unknown";
    }
    const json& attributes = modules[type].value("attributes", json::object());
    return attribute_set(attributes, "blackbox") ||
               attribute_set(attributes, "whitebox")
             ? "library"
             : "module";
  };
  /* The modules of the tree, the children before their parents */
  std::vector<std::string> order;
  std::set<std::string> seen;
  std::vector<std::pair<std::string, size_t>> stack = {{top, 0}};
  seen.insert(top);
  std::map<std::string, std::vector<std::pair<std::string, std::string>>>
    children;
  while (!stack.empty()) {
    auto& [type, next] = stack.back();
    if (0 == next && !children.count(type)) {
      children[type] = "module" == kind_of(type)
                         ? instances_of(modules[type], modules)
                         : std::vector<std::pair<std::string, std::string>>();
    }
    const auto& list = children[type];
    if (next < list.size()) {
      const std::string child = list[next++].second;
      if (seen.insert(child).second) {
        stack.emplace_back(child, 0);
      }
      continue;
    }
    order.push_back(type);
    stack.pop_back();
  }
  /* The instances of each module in the whole tree: the parents first */
  std::map<std::string, double> count;
  count[top] = 1;
  for (auto it = order.rbegin(); it != order.rend(); ++it) {
    for (const auto& child : children[*it]) {
      count[child.second] += count[*it];
    }
  }
  /* The tree, breadth first */
  std::vector<Node> nodes(1);
  nodes[0].type = top;
  std::deque<size_t> queue = {0};
  bool truncated = false;
  while (!queue.empty()) {
    const size_t at = queue.front();
    queue.pop_front();
    for (const auto& child : children[nodes[at].type]) {
      if (nodes.size() >= limit) {
        ++nodes[at].more;
        truncated = true;
        continue;
      }
      Node node;
      node.instance = child.first;
      node.type = child.second;
      const json& cell = modules[nodes[at].type]["cells"][child.first];
      node.src = source_of(cell.value("attributes", json::object()));
      nodes.push_back(node);
      nodes[at].children.push_back(nodes.size() - 1);
      queue.push_back(nodes.size() - 1);
    }
  }
  /* Each node as JSON, the children before their parent */
  std::vector<ordered> made(nodes.size());
  for (size_t i = nodes.size(); i-- > 0;) {
    const Node& node = nodes[i];
    ordered item;
    item["instance"] =
      node.instance.empty() ? ordered() : ordered(node.instance);
    item["name"] = node.type;
    const ordered name = module_name(node.type);
    item["module"] = name["module"];
    item["kind"] = kind_of(node.type);
    if (!name["parameters"].empty()) {
      item["parameters"] = name["parameters"];
    }
    if (!node.src.is_null()) {
      item["src"] = node.src;
    }
    item["children"] = ordered::array();
    for (const size_t child : node.children) {
      item["children"].push_back(std::move(made[child]));
    }
    if (0 != node.more) {
      item["more"] = node.more;
    }
    made[i] = std::move(item);
  }
  /* The modules, most instances first */
  ordered list = ordered::array();
  std::vector<std::string> types(order.begin(), order.end());
  std::stable_sort(types.begin(), types.end(),
                   [&](const std::string& a, const std::string& b) {
                     return count[a] != count[b] ? count[a] > count[b] : a < b;
                   });
  for (const std::string& type : types) {
    ordered item;
    item["name"] = type;
    const ordered name = module_name(type);
    item["module"] = name["module"];
    item["parameters"] = name["parameters"];
    item["kind"] = kind_of(type);
    item["instances"] =
      count[type] < 9e15
        ? ordered(static_cast<long long>(std::llround(count[type])))
        : ordered(count[type]);
    item["cells"] =
      modules.contains(type) ? cells_of(modules[type], modules) : 0;
    item["src"] =
      modules.contains(type)
        ? source_of(modules[type].value("attributes", json::object()))
        : ordered();
    list.push_back(item);
  }
  double total = 0;
  for (const auto& entry : count) {
    total += entry.second;
  }
  ordered answer;
  answer["top"] = top;
  answer["modules"] = list;
  answer["tree"] = std::move(made[0]);
  answer["nodes"] = nodes.size();
  answer["instances"] = total < 9e15
                          ? ordered(static_cast<long long>(std::llround(total)))
                          : ordered(total);
  answer["truncated"] = truncated;
  return answer;
}

} /* end namespace openfpga */
