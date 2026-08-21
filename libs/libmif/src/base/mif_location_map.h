#pragma once

/********************************************************************
 * MIF location map
 *
 * Maps each FPGA-top MIF data bus to the physical memory instances that
 * occupy slices of that bus. Built by walking top-module io_children in
 * GPIN concatenation order (same order as Verilog pin packing).
 *
 * Nested map:
 *   outer key  : top-level port name (BasicPort string)
 *   inner key  : placed instance location (t_pl_loc x/y/sub_tile/layer)
 *   inner value: MifPortSlice (which primitive + [offset, offset+width) )
 *
 * Example (two 16-bit BRAMs on one 32-bit top bus):
 *
 *   circuit model "dpram_8x16_preload", port prefix "mem_init_data"
 *   top port name:
 *     "gfpga_pad_dpram_8x16_preload_mem_init_data"
 *
 *   add("gfpga_pad_dpram_8x16_preload_mem_init_data",
 *       t_pl_loc(2, 1, 0, 0),   // x, y, sub_tile, layer
 *       pb_graph_node_A, 0, 16);  // offset, width
 *   add("gfpga_pad_dpram_8x16_preload_mem_init_data",
 *       t_pl_loc(3, 1, 0, 0),
 *       pb_graph_node_B, 16, 16);
 *
 *   top bus word (LSB at index 0):
 *     [  bits 0..15  |  bits 16..31 ]
 *     [ instance A   |  instance B  ]
 *
 * (t_pl_loc, pb_graph_node*) looks up that instance's MIF in
 * MifPipeline::physical_mifs_. offset/width paste it onto top_mif_.
 *******************************************************************/

#include <cstddef>
#include <map>
#include <string>

#include "physical_types.h"
#include "vpr_types.h"

/* Begin namespace openfpga */
namespace openfpga {

/* XML names used by write_to_xml_file().
 *
 *   <mif_coordinates>
 *     <mif port="gfpga_pad_dpram_8x16_preload_mem_init_data"
 *          pb_type="..." data_offset="0" data_width="16"
 *          x="2" y="1" z="0"/>
 *   </mif_coordinates>
 */
constexpr const char* XML_MIF_COORDINATES_ROOT_NAME = "mif_coordinates";
constexpr const char* XML_MIF_LOCATION_NODE_NAME = "mif";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_PB_TYPE = "pb_type";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_PORT = "port";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_DATA_OFFSET = "data_offset";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_DATA_WIDTH = "data_width";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_X = "x";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_Y = "y";
constexpr const char* XML_MIF_LOCATION_ATTRIBUTE_Z = "z";

/* One instance's occupancy on a top-level MIF data bus. */
struct MifPortSlice {
  /* Physical primitive whose MIF fills this slice. */
  t_pb_graph_node* pb_graph_node = nullptr;
  /* First bit on the top bus (inclusive). Example: 16. */
  size_t data_offset = 0;
  /* Slice width in bits. Example: 16 -> occupies [16, 32). */
  size_t data_width = 0;
};

class MifLocationMap {
 public: /* Public accessors */
  /* Full nested map. Typical use: iterate every port then every loc.
   *
   *   for (const auto& port_entry : map.data_port2phy_loc_map()) {
   *     const std::string& port = port_entry.first;
   *     for (const auto& loc_entry : port_entry.second) {
   *       const t_pl_loc& loc = loc_entry.first;
   *       const MifPortSlice& slice = loc_entry.second;
   *     }
   *   }
   */
  const std::map<std::string, std::map<t_pl_loc, MifPortSlice>>&
  data_port2phy_loc_map() const;

  /* Number of (port, loc) slices, not number of ports.
   * Example: 1 port x 2 BRAMs -> size() == 2. */
  size_t size() const;

  /* True when no ports were registered (no is_mif_data_bus, or no MIF
   * primitives). aggregate_unified_mif skips top_mif_ in that case. */
  bool empty() const;

 public: /* Public mutators */
  /* Register one placed instance on a top-level MIF data bus.
   *
   *   add("gfpga_pad_dpram_8x16_preload_mem_init_data",
   *       t_pl_loc{2, 1, 0, 0}, pb_graph_node, 0, 16);
   *
   * Same (port, loc) overwrites the previous slice. pb_graph_node must
   * be non-null. */
  void add(const std::string& port_name, const t_pl_loc& phy_loc,
           t_pb_graph_node* pb_graph_node, const size_t& data_offset,
           const size_t& data_width);

 public: /* Public writer */
  /* Dump every slice to XML (see XML_* names above). Returns
   * CMD_EXEC_SUCCESS or CMD_EXEC_FATAL_ERROR. */
  int write_to_xml_file(const std::string& fname,
                        const bool& include_time_stamp,
                        const bool& verbose) const;

 private: /* Internal Data */
  /* Nested map described at the top of this file. */
  std::map<std::string, std::map<t_pl_loc, MifPortSlice>>
    data_port2phy_loc_map_;
};

} /* End namespace openfpga */
