#pragma once

/********************************************************************
 * MIF location map bridges VPR grid coordinates and the bit offset of
 * each physical MIF instance on a fabric-top MIF data bus (GPIN).
 *
 * Each entry records:
 *   (x, y, z), pb_type_path, global port name, data_offset, data_width
 *
 * Offsets are assigned in the same order that GPIN buses are
 * concatenated in the module graph (see build_fabric_mif_location_map).
 *******************************************************************/

#include <array>
#include <cstddef>
#include <map>
#include <string>
#include <vector>

/* Begin namespace openfpga */
namespace openfpga {

/* XML element and attribute names used by the location-map writer. */
constexpr const char* XML_MIF_COORDINATES_ROOT_NAME = "mif_coordinates";
constexpr const char* XML_MIF_LOCATION_NODE_NAME = "mif";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_PB_TYPE = "pb_type";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_PORT = "port";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_DATA_OFFSET = "data_offset";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_DATA_WIDTH = "data_width";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_X = "x";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_Y = "y";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_Z = "z";

struct MifLocation {
  size_t x = 0;
  size_t y = 0;
  size_t z = 0;
  std::string pb_type_path;
  std::string port_name;
  size_t data_offset = 0;
  size_t data_width = 0;
};

class MifLocationMap {
 public: /* Public aggregators */
  bool has_mif_location(const size_t& x, const size_t& y, const size_t& z,
                        const std::string& pb_type_path) const;
  const MifLocation& mif_location(const size_t& x, const size_t& y,
                                  const size_t& z,
                                  const std::string& pb_type_path) const;
  size_t data_offset(const size_t& x, const size_t& y, const size_t& z,
                     const std::string& pb_type_path) const;
  size_t data_width(const size_t& x, const size_t& y, const size_t& z,
                    const std::string& pb_type_path) const;
  std::string port_name(const size_t& x, const size_t& y, const size_t& z,
                        const std::string& pb_type_path) const;
  const std::vector<MifLocation>& mif_locations() const;
  bool empty() const;

 public: /* Public mutators */
  void set_mif_location(const size_t& x, const size_t& y, const size_t& z,
                        const std::string& pb_type_path,
                        const std::string& port_name, const size_t& data_offset,
                        const size_t& data_width);

 public: /* Public writer */
  int write_to_xml_file(const std::string& fname,
                        const bool& include_time_stamp,
                        const bool& verbose) const;

 private: /* Internal helpers */
  /* Returns index into mif_locations_, or size_t(-1) if missing. */
  size_t find_mif_location_index(const size_t& x, const size_t& y,
                                 const size_t& z,
                                 const std::string& pb_type_path) const;

 private: /* Internal Data */
  std::vector<MifLocation> mif_locations_;
  /* Fast lookup by [x][y][z] into indices of mif_locations_. */
  std::map<std::array<size_t, 3>, std::vector<size_t>> mif_coord_lookup_;
};

} /* End namespace openfpga */
