#ifndef PACKAGE_IO_MAP_H
#define PACKAGE_IO_MAP_H
/********************************************************************
 * The I/O map of a device package (docs/spec_device_package.md, R-25): a CSV
 * file, comma- or tab-separated with a header, with one row for each pin of
 * the fabric: an I/O subtile of the grid of VPR (side, row = y, col = x,
 * subtile), or a global port of the fabric (no location). Each row says the
 * direction and the type of the pin, its name in the fabric (netlist_name)
 * and, optionally, the name a user knows it by (customer_pin_alias).
 *******************************************************************/
#include <filesystem>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include "device_grid.h"
#include "io_location_map.h"

/* begin namespace openfpga */
namespace openfpga {

/* An I/O map that breaks a rule: the message has the file and the line */
class PackageIoMapError : public std::runtime_error {
 public:
  explicit PackageIoMapError(const std::string& message)
    : std::runtime_error(message) {}
};

struct PackageIoMapRow {
  size_t line = 0; /* in the file, 1 for the header */
  std::string side;
  std::string pin_direction;
  std::string pin_type;
  std::optional<int> row; /* y */
  std::optional<int> col; /* x */
  std::optional<int> subtile;
  std::string netlist_name;
  std::string customer_pin_alias;

  bool has_location() const { return row.has_value(); }
};

struct PackageIoMap {
  std::string file;
  std::vector<PackageIoMapRow> rows;
};

/* The columns, in the order that write_io_map() writes them */
const std::vector<std::string>& package_io_map_columns();

/* Read an I/O map and check the rules that need no device: the columns, the
 * values of side, pin_direction and pin_type, a location that is whole or
 * empty (empty only for a CONTROL pin), a location given once for each pin of
 * the fabric (netlist_name), netlist_name unique, and customer_pin_alias
 * unique for each direction. Throws PackageIoMapError with the first problem */
PackageIoMap read_package_io_map(const std::filesystem::path& file);

/* The rules that need the grid of the device: each location is an I/O
 * subtile of the grid (the side is not checked: it is the name that the
 * maker of the package gives). Returns the problems,
 * each with its line; none when the map fits */
std::vector<std::string> check_package_io_map_grid(const PackageIoMap& map,
                                                   const DeviceGrid& grid);

/* A first I/O map of a device: a row for each I/O subtile of the grid, with
 * its side and location, the names of the fabric when the fabric is built
 * (one row per port of the fabric at a location), the direction inout and the
 * type UNDEF, for the maker of the package to fill in */
std::string write_package_io_map(const DeviceGrid& grid,
                                 const IoLocationMap* fabric_ios);

} /* end namespace openfpga */

#endif
