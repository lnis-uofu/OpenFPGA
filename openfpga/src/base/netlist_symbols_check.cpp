#include "netlist_symbols_check.h"

#include <algorithm>
#include <cstring>
#include <regex>
#include <set>
#include <string>
#include <vector>

#include "pugixml.hpp"

namespace openfpga {

namespace {

const char* const SYMBOLS_NS = "https://github.com/nturley/netlistsvg";
const char* const SVG_NS = "http://www.w3.org/2000/svg";

/* The namespaces of editors: their elements and attributes are skipped */
const std::set<std::string>& editor_namespaces() {
  static const std::set<std::string> ns = {
    "http://www.inkscape.org/namespaces/inkscape",
    "http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd",
    "http://www.w3.org/1999/02/22-rdf-syntax-ns#",
    "http://purl.org/dc/elements/1.1/", "http://creativecommons.org/ns#"};
  return ns;
}

const std::set<std::string>& base_roles() {
  static const std::set<std::string> roles = {"body", "body-alt", "outline",
                                              "pin",  "label",    "accent"};
  return roles;
}

const std::set<std::string>& svg_attributes() {
  static const std::set<std::string> names = {
    "id",    "class",        "transform",   "d",         "x",       "y",
    "width", "height",       "rx",          "ry",        "cx",      "cy",
    "r",     "x1",           "y1",          "x2",        "y2",      "points",
    "fill",  "stroke-width", "text-anchor", "font-size", "version", "viewBox"};
  return names;
}

const std::set<std::string>& text_attributes() {
  static const std::set<std::string> names = {"name", "short_name", "model",
                                              "type", "count",      "ports"};
  return names;
}

struct Problem {
  std::string text;
};

/* The reader of one file: the prefixes of its namespaces, and the line of an
 * offset */
class Checker {
 public:
  explicit Checker(const std::string& text) : text_(text), buffer_(text) {}

  std::string run() {
    pugi::xml_document doc;
    /* In place: the names of the elements point into the buffer, which gives
     * their lines */
    const pugi::xml_parse_result parsed = doc.load_buffer_inplace(
      &buffer_[0], buffer_.size(), pugi::parse_default | pugi::parse_ws_pcdata);
    if (!parsed) {
      return "it is not well-formed XML (line " +
             std::to_string(line_of(parsed.offset)) +
             "): " + parsed.description();
    }
    const pugi::xml_node root = doc.document_element();
    try {
      read_namespaces(root);
      if (local(root) != "svg" || ns_of(root) != SVG_NS) {
        fail(root, "the root is not an svg element of SVG");
      }
      check_attributes(root);
      const std::set<std::string> root_roles = colors_of(root);
      for (const pugi::xml_node e : root.children()) {
        if (e.type() != pugi::node_element || skipped(e)) {
          continue;
        }
        if (ns_of(e) == SYMBOLS_NS) {
          if (local(e) == "color") {
            continue;
          }
          if (local(e) == "layout") {
            check_layout(e);
            continue;
          }
          fail(e, "s:" + local(e) + " is not allowed here");
        }
        if (ns_of(e) == SVG_NS && local(e) == "g" && !has_s(e, "type")) {
          /* A group of symbols (a layer of an editor) */
          check_attributes(e);
          for (const pugi::xml_node c : e.children()) {
            if (c.type() != pugi::node_element || skipped(c)) {
              continue;
            }
            if (ns_of(c) == SVG_NS && local(c) == "g" && has_s(c, "type")) {
              check_symbol(c, root_roles);
            } else {
              fail(c, "a symbol is a g with s:type");
            }
          }
          continue;
        }
        if (ns_of(e) != SVG_NS || local(e) != "g") {
          fail(e, "a symbol is a g with s:type");
        }
        check_symbol(e, root_roles);
      }
    } catch (const Problem& problem) {
      return problem.text;
    }
    return "";
  }

 private:
  const std::string& text_;
  std::string buffer_;
  /* prefix -> namespace, from the xmlns attributes of the root ("" default) */
  std::vector<std::pair<std::string, std::string>> prefixes_;
  std::set<std::string> types_;

  int line_of(ptrdiff_t offset) const {
    const size_t end = std::min(
      static_cast<size_t>(std::max<ptrdiff_t>(0, offset)), text_.size());
    return 1 + static_cast<int>(
                 std::count(text_.begin(), text_.begin() + end, '\n'));
  }

  /* The offset of an element in the text: pugixml keeps it in its name */
  int line_of(const pugi::xml_node& e) const {
    const char* name = e.name();
    const char* start = buffer_.data();
    if (name >= start && name < start + buffer_.size()) {
      return line_of(name - start);
    }
    return 0;
  }

  [[noreturn]] void fail(const pugi::xml_node& e,
                         const std::string& why) const {
    const int line = line_of(e);
    throw Problem{"<" + std::string(e.name()) + ">" +
                  (line ? " (line " + std::to_string(line) + ")" : "") + ": " +
                  why};
  }

  void read_namespaces(const pugi::xml_node& root) {
    for (const pugi::xml_attribute a : root.attributes()) {
      const std::string name = a.name();
      if (name == "xmlns") {
        prefixes_.push_back({"", a.value()});
      } else if (0 == name.rfind("xmlns:", 0)) {
        prefixes_.push_back({name.substr(6), a.value()});
      }
    }
  }

  std::string ns_of_name(const std::string& name) const {
    const size_t colon = name.find(':');
    const std::string prefix =
      colon == std::string::npos ? "" : name.substr(0, colon);
    for (const auto& p : prefixes_) {
      if (p.first == prefix) {
        return p.second;
      }
    }
    return colon == std::string::npos ? "" : "?";
  }

  std::string ns_of(const pugi::xml_node& e) const {
    return ns_of_name(e.name());
  }

  static std::string local(const pugi::xml_node& e) {
    const std::string name = e.name();
    const size_t colon = name.find(':');
    return colon == std::string::npos ? name : name.substr(colon + 1);
  }

  bool skipped(const pugi::xml_node& e) const {
    const std::string l = local(e);
    return editor_namespaces().count(ns_of(e)) ||
           (ns_of(e) == SVG_NS &&
            (l == "title" || l == "desc" || l == "metadata"));
  }

  /* The attribute s:name of an element, whatever the prefix of the
   * namespace */
  pugi::xml_attribute s_attr(const pugi::xml_node& e,
                             const std::string& name) const {
    for (const pugi::xml_attribute a : e.attributes()) {
      const std::string full = a.name();
      const size_t colon = full.find(':');
      if (colon != std::string::npos && full.substr(colon + 1) == name &&
          ns_of_name(full) == SYMBOLS_NS) {
        return a;
      }
    }
    return pugi::xml_attribute();
  }

  bool has_s(const pugi::xml_node& e, const std::string& name) const {
    return !s_attr(e, name).empty();
  }

  void check_attributes(const pugi::xml_node& e) const {
    for (const pugi::xml_attribute a : e.attributes()) {
      const std::string name = a.name();
      const std::string value = a.value();
      if (name == "xmlns" || 0 == name.rfind("xmlns:", 0)) {
        continue;
      }
      if (name.find(':') != std::string::npos) {
        const std::string ns = ns_of_name(name);
        if (ns == SYMBOLS_NS || editor_namespaces().count(ns)) {
          continue;
        }
        if (name.size() >= 5 && name.substr(name.size() - 5) == ":href") {
          fail(e, "the attribute " + name + " is refused");
        }
        fail(e, "the attribute " + name + " is refused");
      }
      if (name == "href" || 0 == name.rfind("on", 0)) {
        fail(e, "the attribute " + name + " is refused");
      }
      if (name == "style") {
        fail(e, "style is refused: colours are set by s:color, for a role");
      }
      if (name == "stroke" || (name == "fill" && value != "none")) {
        fail(e, name + "=\"" + value +
                  "\" is refused: colours are set by s:color, for a role "
                  "(fill=\"none\" is allowed)");
      }
      if (!svg_attributes().count(name)) {
        fail(e, "the attribute " + name + " is refused");
      }
      if (name == "transform") {
        static const std::regex transform(
          "^(\\s*(translate|scale)\\s*\\([^)]*\\)\\s*,?)*\\s*$");
        if (!std::regex_match(value, transform)) {
          fail(e, "transform=\"" + value +
                    "\": only translate and scale are allowed");
        }
      }
    }
  }

  static bool color(const std::string& text) {
    static const std::regex pattern(
      "^#([0-9a-fA-F]{3}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})$");
    return std::regex_match(text, pattern);
  }

  /* The roles that the s:color children of an element define */
  std::set<std::string> colors_of(const pugi::xml_node& parent) const {
    std::set<std::string> roles;
    static const std::regex role_name("^[a-z][a-z0-9-]*$");
    for (const pugi::xml_node c : parent.children()) {
      if (c.type() != pugi::node_element || ns_of(c) != SYMBOLS_NS ||
          local(c) != "color") {
        continue;
      }
      const std::string role = c.attribute("role").value();
      if (!std::regex_match(role, role_name)) {
        fail(c, "role=\"" + role + "\" is not the name of a role");
      }
      if (!color(c.attribute("light").value()) ||
          !color(c.attribute("dark").value())) {
        fail(c, "the role " + role +
                  " needs both light and dark colours (#rgb, #rrggbb or "
                  "#rrggbbaa)");
      }
      roles.insert(role);
    }
    return roles;
  }

  void check_layout(const pugi::xml_node& e) const {
    for (const pugi::xml_attribute a : e.attributes()) {
      const std::string name = a.name();
      if (name == "layerSpacing" || name == "nodeSpacing") {
        const double v = a.as_double(-1);
        if (!(v > 0 && v <= 1000)) {
          fail(e, name + "=\"" + a.value() + "\" is a number from 0 to 1000");
        }
      } else if (name == "direction") {
        const std::string v = a.value();
        if (v != "RIGHT" && v != "DOWN") {
          fail(e, "direction=\"" + v + "\" is RIGHT or DOWN");
        }
      } else {
        fail(e, name +
                  ": s:layout sets layerSpacing, nodeSpacing and direction "
                  "only");
      }
    }
  }

  void check_symbol(const pugi::xml_node& g,
                    const std::set<std::string>& root_roles) {
    check_attributes(g);
    const std::string type = s_attr(g, "type").value();
    static const std::regex type_name("^[A-Za-z_][A-Za-z0-9_.:-]*$");
    if (!std::regex_match(type, type_name)) {
      fail(g, "s:type=\"" + type + "\" is not a name");
    }
    if (!types_.insert(type).second) {
      fail(g, "the symbol " + type + " is defined twice");
    }
    const pugi::xml_attribute hide = s_attr(g, "hide");
    if (!hide.empty()) {
      if (std::string(hide.value()) != "true") {
        fail(g, "s:hide is \"true\" or absent");
      }
      if (g.first_child()) {
        for (const pugi::xml_node c : g.children()) {
          if (c.type() == pugi::node_element) {
            fail(g, "the hidden symbol " + type + " holds nothing");
          }
        }
      }
      return;
    }
    if (!(s_attr(g, "width").as_double(0) > 0) ||
        !(s_attr(g, "height").as_double(0) > 0)) {
      fail(g, "the symbol " + type + " needs s:width and s:height");
    }
    std::set<std::string> roles = base_roles();
    roles.insert(root_roles.begin(), root_roles.end());
    const std::set<std::string> own = colors_of(g);
    roles.insert(own.begin(), own.end());
    int aliases = 0;
    walk(g, roles, aliases);
    if (0 == aliases) {
      fail(g, "the symbol " + type + " has no s:alias: nothing would use it");
    }
  }

  void walk(const pugi::xml_node& parent, const std::set<std::string>& roles,
            int& aliases) const {
    static const std::set<std::string> allowed = {
      "g",    "path",     "rect",    "circle", "ellipse",
      "line", "polyline", "polygon", "text"};
    static const std::regex alias("^[a-z]+:\\S+$|^stub$");
    for (const pugi::xml_node e : parent.children()) {
      if (e.type() != pugi::node_element || skipped(e)) {
        continue;
      }
      if (ns_of(e) == SYMBOLS_NS) {
        if (local(e) == "alias") {
          const std::string val = e.attribute("val").value();
          if (!std::regex_match(val, alias)) {
            fail(e, "the alias \"" + val +
                      "\" is not <kind>:<name> (atom:.latch, cluster:clb...)");
          }
          ++aliases;
        } else if (local(e) != "color") {
          fail(e, "s:" + local(e) + " is not allowed in a symbol");
        }
        continue;
      }
      if (ns_of(e) != SVG_NS || !allowed.count(local(e))) {
        fail(e, "the element " + std::string(e.name()) + " is refused");
      }
      check_attributes(e);
      const std::string role = e.attribute("class").value();
      if (!role.empty()) {
        if (role.find(' ') != std::string::npos) {
          fail(e, "class=\"" + role + "\": one role only");
        }
        if (!roles.count(role)) {
          fail(e, "the role " + role +
                    " is not one of body, body-alt, outline, pin, label, "
                    "accent, nor defined by an s:color of the file");
        }
      }
      if (local(e) == "text") {
        const pugi::xml_attribute attribute = s_attr(e, "attribute");
        const std::string value = attribute.value();
        if (!value.empty() && !text_attributes().count(value)) {
          fail(e, "s:attribute=\"" + value +
                    "\" is not one of name, short_name, model, type, count, "
                    "ports");
        }
        for (const pugi::xml_node c : e.children()) {
          if (c.type() == pugi::node_element) {
            fail(e, "a text holds only its text");
          }
        }
      }
      if (local(e) == "g" && has_s(e, "pid")) {
        for (const pugi::xml_node c : e.children()) {
          if (c.type() == pugi::node_element) {
            fail(e, "the port " + std::string(s_attr(e, "pid").value()) +
                      " holds nothing");
          }
        }
      } else if (local(e) == "g") {
        walk(e, roles, aliases);
      }
    }
  }
};

}  // namespace

std::string check_netlist_symbols(const std::string& text) {
  return Checker(text).run();
}

} /* end namespace openfpga */
