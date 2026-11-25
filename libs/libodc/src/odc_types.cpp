#include "odc_types.h"

/* begin namespace odc */
namespace odc {

std::string Diagnostic::text() const {
  std::string where = file;
  if (0 != line) {
    where += ":" + std::to_string(line);
  }
  return where + (where.empty() ? "" : ": ") +
         (ERROR == level ? "error: " : "warning: ") + message;
}

namespace {

std::string quoted(const std::string& text) {
  std::string out = "\"";
  for (const char c : text) {
    if ('"' == c || '\\' == c) {
      out += '\\';
    }
    out += c;
  }
  return out + "\"";
}

/* A bare word that would not read back as itself needs quotes */
bool needs_quotes(const std::string& text) {
  if (text.empty()) {
    return true;
  }
  if ('"' == text[0] || '{' == text[0] || '[' == text[0]) {
    return true;
  }
  for (const char c : text) {
    if (' ' == c || '\t' == c || '#' == c || '\n' == c) {
      return true;
    }
  }
  return false;
}

}  // namespace

std::string Word::source() const {
  switch (kind) {
    case BARE:
      return needs_quotes(text) ? quoted(text) : text;
    case QUOTED:
      return quoted(text);
    case BRACED:
      return "{" + text + "}";
    case QUERY:
      return "[" + text + "]";
    default:
      break;
  }
  return text;
}

std::string to_string(const Kind& kind) {
  switch (kind) {
    case Kind::HEADER:
      return "header";
    case Kind::INCLUDE:
      return "include";
    case Kind::TIMING:
      return "timing";
    case Kind::SYNTHESIS:
      return "synthesis";
    case Kind::FLOORPLAN:
      return "floorplanning";
    case Kind::ROUTING:
      return "routing";
    case Kind::PIN:
      return "pins";
    default:
      break;
  }
  return "unknown";
}

std::string to_string(const Stage& stage) {
  return Stage::CIRCUIT == stage ? "CIRCUIT" : "DEVICE";
}

std::string to_string(const Consumer& consumer) {
  switch (consumer) {
    case Consumer::NONE:
      return "none";
    case Consumer::YOSYS:
      return "yosys";
    case Consumer::VPR_SDC:
      return "vpr_sdc";
    case Consumer::VPR_CONSTRAINTS:
      return "vpr_constraints";
    case Consumer::PIN_CONSTRAINTS:
      return "pin_constraints";
    default:
      break;
  }
  return "unknown";
}

std::string Command::text() const {
  std::string out;
  for (const Word& word : words) {
    if (!out.empty()) {
      out += ' ';
    }
    out += word.source();
  }
  return out;
}

bool Constraints::has(const Kind& kind) const {
  for (const Command& command : commands) {
    if (kind == command.kind) {
      return true;
    }
  }
  return false;
}

bool Constraints::has_errors() const {
  for (const Diagnostic& diagnostic : diagnostics) {
    if (Diagnostic::ERROR == diagnostic.level) {
      return true;
    }
  }
  return false;
}

const Object* Netlist::port(const std::string& name) const {
  for (const Object& object : ports) {
    if (name == object.name) {
      return &object;
    }
  }
  return nullptr;
}

const Tile* Device::tile(const int x, const int y) const {
  if (x < 0 || y < 0 || x >= width || y >= height) {
    return nullptr;
  }
  const size_t index = static_cast<size_t>(x) * height + y;
  return index < tiles.size() ? &tiles[index] : nullptr;
}

std::string Site::name() const {
  if (!has_location) {
    return netlist_name;
  }
  return "X" + std::to_string(x) + "Y" + std::to_string(y) + "/" +
         std::to_string(subtile);
}

} /* end namespace odc */
