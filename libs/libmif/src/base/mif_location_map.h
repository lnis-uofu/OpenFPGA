#pragma once

/********************************************************************
 * MIF location map: for any bit of a top-level MIF data port, find
 * the physical MIF instance and its slice on that bus.
 *
 * Outer key: string form of the top-level BasicPort.
 * Inner key: physical location of the instance on that port.
 * Inner value: pb_graph_node plus data_offset/data_width on the bus.
 *
 * Offsets are assigned in GPIN concatenation order (io_children).
 * Pair (t_pl_loc, t_pb_graph_node*) recovers MIF content from
 * openfpga_context; offset/width place that content on the top bus.
 *******************************************************************/

#include <cstddef>
#include <map>
#include <string>

#include "physical_types.h"
#include "vpr_types.h"

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

struct MifPortSlice {
  t_pb_graph_node* pb_graph_node = nullptr;
  size_t data_offset = 0;
  size_t data_width = 0;
};

class MifLocationMap {
 public: /* Public accessors */
  const std::map<t_pl_loc, MifPortSlice>& phy_locs(
    const std::string& port_name) const;
  const std::map<std::string, std::map<t_pl_loc, MifPortSlice>>&
  data_port2phy_loc_map() const;
  size_t size() const;
  bool empty() const;

 public: /* Public mutators */
  void add(const std::string& port_name, const t_pl_loc& phy_loc,
           t_pb_graph_node* pb_graph_node, const size_t& data_offset,
           const size_t& data_width);

 public: /* Public writer */
  int write_to_xml_file(const std::string& fname,
                        const bool& include_time_stamp,
                        const bool& verbose) const;

 private: /* Internal Data */
  /* Outer key is the string version of the BasicPort for a top-level port. */
  std::map<std::string, std::map<t_pl_loc, MifPortSlice>>
    data_port2phy_loc_map_;
};

} /* End namespace openfpga */
