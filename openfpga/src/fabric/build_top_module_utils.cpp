/********************************************************************
 * This file include most utilized functions for building the module
 * graph for FPGA fabric
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from vpr library */
#include "openfpga_naming.h"
#include "vpr_utils.h"

/* Module builder headers */
#include "build_top_module_utils.h"
#include "openfpga_rr_graph_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Generate the name for a grid block, by considering
 * 1. if it locates on the border with given device size
 * 2. its type
 *
 * This function is mainly used in the top-level module generation
 *******************************************************************/
std::string generate_grid_block_module_name_in_top_module(
  const std::string& prefix, const DeviceGrid& grids,
  const vtr::Point<size_t>& grid_coord) {
  /* Determine if the grid locates at the border */
  vtr::Point<size_t> device_size(grids.width(), grids.height());
  e_side border_side = find_grid_border_side(device_size, grid_coord);
  t_physical_tile_type_ptr phy_tile_type = grids.get_physical_type(
    t_physical_tile_loc(grid_coord.x(), grid_coord.y(), 0));

  return generate_grid_block_module_name(
    prefix, std::string(phy_tile_type->name), is_io_type(phy_tile_type),
    border_side);
}

/********************************************************************
 * Generate the port name for a grid block, by considering
 * 1. its pin index
 * 2. side on the grid
 * 3. relative position in the physical tile
 *
 * This function is mainly used in the top-level module generation
 * Note that it may be useful for other modules but not tried yet!
 *******************************************************************/
std::string generate_grid_module_port_name_in_top_module(
  const DeviceGrid& grids, const vtr::Point<size_t>& grid_coordinate,
  const size_t& sink_grid_pin_index,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const RRNodeId& inode) {
  t_physical_tile_type_ptr grid_type_descriptor = grids.get_physical_type(
    t_physical_tile_loc(grid_coordinate.x(), grid_coordinate.y(), 0));
  size_t sink_grid_pin_width =
    grid_type_descriptor->pin_width_offset[sink_grid_pin_index];
  size_t sink_grid_pin_height =
    grid_type_descriptor->pin_height_offset[sink_grid_pin_index];
  BasicPort sink_grid_pin_info =
    vpr_device_annotation.physical_tile_pin_port_info(grid_type_descriptor,
                                                      sink_grid_pin_index);
  VTR_ASSERT(true == sink_grid_pin_info.is_valid());
  int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
    grid_type_descriptor, sink_grid_pin_index);
  VTR_ASSERT(OPEN != subtile_index &&
             subtile_index < grid_type_descriptor->capacity);
  std::string sink_grid_port_name = generate_grid_port_name(
    sink_grid_pin_width, sink_grid_pin_height, subtile_index,
    get_rr_graph_single_node_side(rr_graph, inode), sink_grid_pin_info);

  return sink_grid_port_name;
}

/********************************************************************
 * Find the cb_type of a GSB in the top-level module
 * depending on the side of SB
 * TOP/BOTTOM side: CHANY
 * RIGHT/LEFT side: CHANX
 *******************************************************************/
t_rr_type find_top_module_cb_type_by_sb_side(const e_side& sb_side) {
  VTR_ASSERT(NUM_2D_SIDES != sb_side);

  if ((TOP == sb_side) || (BOTTOM == sb_side)) {
    return CHANY;
  }

  VTR_ASSERT((RIGHT == sb_side) || (LEFT == sb_side));
  return CHANX;
}

/********************************************************************
 * Find the GSB coordinate for a CB in the top-level module
 * depending on the side of a SB
 * TODO: use vtr::Point<size_t> to replace DeviceCoordinator
 *******************************************************************/
vtr::Point<size_t> find_top_module_gsb_coordinate_by_sb_side(
  const RRGSB& rr_gsb, const e_side& sb_side) {
  VTR_ASSERT(NUM_2D_SIDES != sb_side);

  vtr::Point<size_t> gsb_coordinate;

  if ((BOTTOM == sb_side) || (LEFT == sb_side)) {
    gsb_coordinate.set_x(rr_gsb.get_x());
    gsb_coordinate.set_y(rr_gsb.get_y());
    return gsb_coordinate;
  }

  VTR_ASSERT((RIGHT == sb_side) || (TOP == sb_side));

  /* RIGHT side: x + 1 */
  if (RIGHT == sb_side) {
    gsb_coordinate.set_x(rr_gsb.get_x() + 1);
    gsb_coordinate.set_y(rr_gsb.get_y());
  }

  /* BOTTOM side: y - 1 */
  if (TOP == sb_side) {
    gsb_coordinate.set_x(rr_gsb.get_x());
    gsb_coordinate.set_y(rr_gsb.get_y() + 1);
  }

  return gsb_coordinate;
}

} /* end namespace openfpga */
