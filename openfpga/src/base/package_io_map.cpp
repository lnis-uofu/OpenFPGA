/********************************************************************
 * The I/O map of a device package (see package_io_map.h)
 *******************************************************************/
#include "package_io_map.h"

#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <sstream>

#include "physical_types.h"

/* begin namespace openfpga */
namespace openfpga {

namespace {

const std::set<std::string> SIDES = {"TOP",   "BOTTOM", "LEFT",
                                     "RIGHT", "CORE",   ""};
const std::set<std::string> DIRECTIONS = {"input", "output", "inout"};
const std::set<std::string> TYPES = {"GPIO", "CONTROL", "SCAN", "UNDEF"};

std::string trimmed(const std::string& text) {
  const size_t first = text.find_first_not_of(" \t\r\n");
  if (std::string::npos == first) {
    return std::string();
  }
  const size_t last = text.find_last_not_of(" \t\r\n");
  return text.substr(first, last - first + 1);
}

/* The fields of a line of CSV: separated by the separator, a field in double
 * quotes may hold it, and "" is a quote */
std::vector<std::string> fields_of(const std::string& line, const char sep) {
  std::vector<std::string> fields;
  std::string field;
  bool quoted = false;
  for (size_t i = 0; i < line.size(); ++i) {
    const char c = line[i];
    if (quoted) {
      if ('"' == c && i + 1 < line.size() && '"' == line[i + 1]) {
        field += '"';
        ++i;
      } else if ('"' == c) {
        quoted = false;
      } else {
        field += c;
      }
    } else if ('"' == c) {
      quoted = true;
    } else if (sep == c) {
      fields.push_back(trimmed(field));
      field.clear();
    } else {
      field += c;
    }
  }
  fields.push_back(trimmed(field));
  return fields;
}

std::string where(const std::string& file, const size_t line) {
  return file + ":" + std::to_string(line) + ": ";
}

/* A whole number of 0 or more, or nothing */
std::optional<int> number_of(const std::string& text, bool& valid) {
  static const std::regex digits("^[0-9]+$");
  valid = true;
  if (text.empty()) {
    return std::nullopt;
  }
  if (!std::regex_match(text, digits)) {
    valid = false;
    return std::nullopt;
  }
  try {
    return std::stoi(text);
  } catch (const std::exception&) {
    valid = false;
    return std::nullopt;
  }
}

/* The side of the I/O ring that a tile is on, "CORE" inside the grid */
std::string side_of(const size_t x, const size_t y, const DeviceGrid& grid) {
  if (0 == y) {
    return "BOTTOM";
  }
  if (grid.height() - 1 == y) {
    return "TOP";
  }
  if (0 == x) {
    return "LEFT";
  }
  if (grid.width() - 1 == x) {
    return "RIGHT";
  }
  return "CORE";
}

/* The type of the tile at a place, when the place is its root, or null */
t_physical_tile_type_ptr root_type(const DeviceGrid& grid, const int x,
                                   const int y) {
  if (x < 0 || y < 0 || static_cast<size_t>(x) >= grid.width() ||
      static_cast<size_t>(y) >= grid.height()) {
    return nullptr;
  }
  const t_physical_tile_loc loc(x, y, 0);
  if (0 != grid.get_width_offset(loc) || 0 != grid.get_height_offset(loc)) {
    return nullptr;
  }
  return grid.get_physical_type(loc);
}

std::string csv_field(const std::string& text) {
  if (std::string::npos == text.find_first_of(",\"\n")) {
    return text;
  }
  std::string quoted = "\"";
  for (const char c : text) {
    quoted += ('"' == c) ? std::string("\"\"") : std::string(1, c);
  }
  return quoted + "\"";
}

} /* end anonymous namespace */

const std::vector<std::string>& package_io_map_columns() {
  static const std::vector<std::string> columns = {
    "side", "pin_direction", "pin_type",     "row",
    "col",  "subtile",       "netlist_name", "customer_pin_alias"};
  return columns;
}

PackageIoMap read_package_io_map(const std::filesystem::path& path) {
  PackageIoMap map;
  map.file = path.string();
  std::ifstream stream(path);
  if (!stream) {
    throw PackageIoMapError(map.file + ": the I/O map cannot be read");
  }
  std::string header;
  std::getline(stream, header);
  if (!header.empty() && '\xEF' == header[0] && header.size() >= 3) {
    header = header.substr(3); /* the mark of UTF-8 that some editors write */
  }
  const char sep = std::string::npos != header.find('\t') ? '\t' : ',';
  const std::vector<std::string> names = fields_of(header, sep);
  std::map<std::string, size_t> column;
  for (size_t i = 0; i < names.size(); ++i) {
    const auto& known = package_io_map_columns();
    if (known.end() == std::find(known.begin(), known.end(), names[i])) {
      throw PackageIoMapError(where(map.file, 1) + "the column '" + names[i] +
                              "' is not one of the I/O map");
    }
    if (column.count(names[i])) {
      throw PackageIoMapError(where(map.file, 1) + "the column '" + names[i] +
                              "' is given twice");
    }
    column[names[i]] = i;
  }
  for (const std::string& name : package_io_map_columns()) {
    if (!column.count(name)) {
      throw PackageIoMapError(where(map.file, 1) + "the column '" + name +
                              "' is missing");
    }
  }
  std::set<std::string> locations; /* "x,y,z,netlist_name" */
  std::set<std::string> netlist_names;
  std::set<std::string> aliases; /* "direction:alias" */
  std::string line;
  size_t number = 1;
  while (std::getline(stream, line)) {
    ++number;
    if (trimmed(line).empty()) {
      continue;
    }
    const std::vector<std::string> fields = fields_of(line, sep);
    if (fields.size() != names.size()) {
      throw PackageIoMapError(where(map.file, number) + "the row has " +
                              std::to_string(fields.size()) +
                              " fields, and the header " +
                              std::to_string(names.size()));
    }
    PackageIoMapRow row;
    row.line = number;
    row.side = fields[column["side"]];
    row.pin_direction = fields[column["pin_direction"]];
    row.pin_type = fields[column["pin_type"]];
    row.netlist_name = fields[column["netlist_name"]];
    row.customer_pin_alias = fields[column["customer_pin_alias"]];
    const std::string at = where(map.file, number);
    if (!SIDES.count(row.side)) {
      throw PackageIoMapError(at + "the side '" + row.side +
                              "' is not TOP, BOTTOM, LEFT, RIGHT or CORE");
    }
    if (!DIRECTIONS.count(row.pin_direction)) {
      throw PackageIoMapError(at + "the pin_direction '" + row.pin_direction +
                              "' is not input, output or inout");
    }
    if (!TYPES.count(row.pin_type)) {
      throw PackageIoMapError(at + "the pin_type '" + row.pin_type +
                              "' is not GPIO, CONTROL, SCAN or UNDEF");
    }
    bool valid_row = true;
    bool valid_col = true;
    bool valid_subtile = true;
    row.row = number_of(fields[column["row"]], valid_row);
    row.col = number_of(fields[column["col"]], valid_col);
    row.subtile = number_of(fields[column["subtile"]], valid_subtile);
    if (!valid_row || !valid_col || !valid_subtile) {
      throw PackageIoMapError(at +
                              "row, col and subtile are whole numbers of 0 "
                              "or more");
    }
    const int given =
      (row.row ? 1 : 0) + (row.col ? 1 : 0) + (row.subtile ? 1 : 0);
    if (1 == given || 2 == given) {
      throw PackageIoMapError(
        at +
        "row, col and subtile are all given, or all empty for a global "
        "port of the fabric");
    }
    if (0 == given && ("CONTROL" != row.pin_type || !row.side.empty())) {
      throw PackageIoMapError(at +
                              "a pin without a location is a global port of "
                              "the fabric: its pin_type is CONTROL, and it "
                              "has no side");
    }
    if (3 == given) {
      /* A location has several rows only for several pins of the fabric
       * (the input and the output of a pad), each with its name */
      const std::string key =
        std::to_string(*row.col) + "," + std::to_string(*row.row) + "," +
        std::to_string(*row.subtile) + "," + row.netlist_name;
      if (!locations.insert(key).second) {
        throw PackageIoMapError(
          at + "the location X" + std::to_string(*row.col) + "Y" +
          std::to_string(*row.row) + "/" + std::to_string(*row.subtile) +
          " is given twice" +
          (row.netlist_name.empty() ? std::string()
                                    : " for " + row.netlist_name));
      }
    }
    if (!row.netlist_name.empty() &&
        !netlist_names.insert(row.netlist_name).second) {
      throw PackageIoMapError(at + "the netlist_name '" + row.netlist_name +
                              "' is given twice");
    }
    if (!row.customer_pin_alias.empty() &&
        !aliases.insert(row.pin_direction + ":" + row.customer_pin_alias)
           .second) {
      throw PackageIoMapError(
        at + "the customer_pin_alias '" + row.customer_pin_alias +
        "' is given twice for the direction " + row.pin_direction);
    }
    map.rows.push_back(row);
  }
  return map;
}

std::vector<std::string> check_package_io_map_grid(const PackageIoMap& map,
                                                   const DeviceGrid& grid) {
  std::vector<std::string> problems;
  for (const PackageIoMapRow& row : map.rows) {
    if (!row.has_location()) {
      continue;
    }
    const std::string at = where(map.file, row.line);
    const std::string place =
      "X" + std::to_string(*row.col) + "Y" + std::to_string(*row.row);
    const t_physical_tile_type_ptr type = root_type(grid, *row.col, *row.row);
    if (nullptr == type || !type->is_io()) {
      problems.push_back(at + "the tile " + place +
                         " is not an I/O tile of the grid");
      continue;
    }
    if (*row.subtile >= type->capacity) {
      problems.push_back(at + "the I/O tile " + place + " has " +
                         std::to_string(type->capacity) +
                         " subtiles, not a subtile " +
                         std::to_string(*row.subtile));
      continue;
    }
    /* The side is the name that the maker of the package gives: the pin
     * tables of OpenFPGA call the row y = 0 TOP, which is the bottom of the
     * grid of VPR. It is not checked against the grid */
  }
  return problems;
}

std::string write_package_io_map(const DeviceGrid& grid,
                                 const IoLocationMap* fabric_ios) {
  std::ostringstream text;
  const auto& columns = package_io_map_columns();
  for (size_t i = 0; i < columns.size(); ++i) {
    text << (0 == i ? "" : ",") << columns[i];
  }
  text << "\n";
  for (size_t y = 0; y < grid.height(); ++y) {
    for (size_t x = 0; x < grid.width(); ++x) {
      const t_physical_tile_type_ptr type = root_type(grid, x, y);
      if (nullptr == type || !type->is_io()) {
        continue;
      }
      for (int z = 0; z < type->capacity; ++z) {
        std::vector<std::string> names;
        if (nullptr != fabric_ios) {
          for (const BasicPort& port : fabric_ios->io_ports(x, y, z)) {
            names.push_back(port.get_name() + "[" +
                            std::to_string(port.get_lsb()) + "]");
          }
        }
        if (names.empty()) {
          names.push_back(std::string());
        }
        for (const std::string& name : names) {
          text << side_of(x, y, grid) << ",inout,UNDEF," << y << "," << x << ","
               << z << "," << csv_field(name) << ",\n";
        }
      }
    }
  }
  return text.str();
}

} /* end namespace openfpga */
