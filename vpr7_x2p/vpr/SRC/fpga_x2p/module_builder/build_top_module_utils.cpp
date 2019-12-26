/********************************************************************
 * This file include most utilized functions for building the module 
 * graph for FPGA fabric
 *******************************************************************/
/* External library headers */
#include "vtr_assert.h"

/* FPGA-X2P headers */
#include "device_coordinator.h"
#include "fpga_x2p_naming.h"
#include "rr_blocks.h"

/* Module builder headers */
#include "build_top_module_utils.h"

/* Global variables should be the last to include */
#include "globals.h"

/********************************************************************
 * Generate the name for a grid block, by considering
 * 1. if it locates on the border with given device size
 * 2. its type
 * 
 * This function is mainly used in the top-level module generation
 *******************************************************************/
std::string generate_grid_block_module_name_in_top_module(const std::string& prefix,
                                                          const vtr::Point<size_t>& device_size,
                                                          const std::vector<std::vector<t_grid_tile>>& grids,
                                                          const vtr::Point<size_t>& grid_coordinate) {
  /* Determine if the grid locates at the border */
  e_side border_side = find_grid_border_side(device_size, grid_coordinate);

  return generate_grid_block_module_name(prefix, std::string(grids[grid_coordinate.x()][grid_coordinate.y()].type->name), 
                                         IO_TYPE == grids[grid_coordinate.x()][grid_coordinate.y()].type, border_side);
}

/********************************************************************
 * Find the cb_type of a GSB in the top-level module 
 * depending on the side of SB
 * TOP/BOTTOM side: CHANY
 * RIGHT/LEFT side: CHANX
 *******************************************************************/
t_rr_type find_top_module_cb_type_by_sb_side(const e_side& sb_side) {
  VTR_ASSERT(NUM_SIDES != sb_side);

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
DeviceCoordinator find_top_module_gsb_coordinate_by_sb_side(const RRGSB& rr_gsb,
                                                            const e_side& sb_side) {
  VTR_ASSERT(NUM_SIDES != sb_side);

  DeviceCoordinator gsb_coordinate;

  if ((TOP == sb_side) || (LEFT == sb_side)) {
    gsb_coordinate.set_x(rr_gsb.get_x()); 
    gsb_coordinate.set_y(rr_gsb.get_y()); 
    return gsb_coordinate;
  }
 
  VTR_ASSERT((RIGHT == sb_side) || (BOTTOM == sb_side));

  /* RIGHT side: x + 1 */
  if (RIGHT == sb_side) {
    gsb_coordinate.set_x(rr_gsb.get_x() + 1); 
    gsb_coordinate.set_y(rr_gsb.get_y()); 
  }

  /* BOTTOM side: y - 1 */
  if (BOTTOM == sb_side) {
    gsb_coordinate.set_x(rr_gsb.get_x()); 
    gsb_coordinate.set_y(rr_gsb.get_y() - 1); 
  }

  return gsb_coordinate;
}
