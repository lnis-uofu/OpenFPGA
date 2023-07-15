/********************************************************************
 * This file includes functions that are used to build the location
 * map information for the top-level module of the FPGA fabric
 * It helps OpenFPGA to link the I/O port index in top-level module
 * to the VPR I/O mapping results
 *******************************************************************/
#include <algorithm>
#include <map>

/* Headers from vtrutil library */
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "build_fabric_tile.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Build tiles by following the top-level style.
 * - The programmble block, e.g., clb, is placed on the top-left corner
 * - The connection blocks and switch block are placed on the right and bottom
 *sides
 *******************************************************************/
static int build_fabric_tile_top_left(FabricTile& fabric_tile, const DeviceGrid& grids, const DeviceRRGSB& device_rr_gsb) {
  int status_code = CMD_EXEC_SUCCESS;

  /* Walk through all the device rr_gsb and create tile one by one */
  for (size_t ix = 0; ix < grids.width(); ++ix) {
    for (size_t iy = 0; iy < grids.height(); ++iy) {
      t_physical_tile_type_ptr phy_tile_type = grids.get_physical_type(ix, iy);
      bool skip_add_pb = false;
      vtr::Point<size_t> curr_tile_coord(ix, iy);
      FabricTileId curr_tile_id = FabricTileId::INVALID();
      /* For EMPTY grid, routing blocks may still be required if there is a gsb
       */
      if (true == is_empty_type(phy_tile_type)) {
        skip_add_pb = true;
        /* Need to create a new tile here */
        curr_tile_id = fabric_tile.create_tile();
        fabric_tile.set_coordinate(curr_tile_coord);
      }
      /* Skip width, height > 1 tiles (mostly heterogeneous blocks) */
      if ((0 < grids.get_width_offset(ix, iy)) ||
          (0 < grids.get_height_offset(ix, iy))) {
        /* Find the root of this grid, the instance id should be valid.
         * We just copy it here
         */
        vtr::Point<size_t> root_tile_coord(
          ix - grids.get_width_offset(ix, iy),
          iy - grids.get_height_offset(ix, iy));
        skip_add_pb = true;
        curr_tile_id = fabric_tile.find_tile(root_tile_coord);
      } else {
        /* Need to create a new tile here */
        curr_tile_id = fabric_tile.create_tile();
        fabric_tile.set_coordinate(curr_tile_coord);
      }

      /* Ensure that we have a valid id */
      if (fabric_tile.valid_tile_id(curr_tile_id)) {
        VTR_LOG_ERROR("Failed to get a valid id for tile[%lu][%lu]!\n", ix, iy);
        return CMD_EXEC_FATAL_ERROR;
      }

      /* Add components: pb, cbx, cby, and sb if exists */
      if (!skip_add_pb) {
        fabric_tile.add_pb_coordinate(curr_tile_id, curr_tile_coord);
      }
      /* The gsb coordinate is different than the grid coordinate when the
       * top-left style is considered
       *
       *  +----------+ +----------+
       *  |   Grid   | | CBx      |
       *  |  [x][y]  | | [x][y]   |
       *  +----------+ +----------+
       *  +----------+ +----------+
       *  | CBy      | | SB       |
       *  | [x][y-1] | | [x][y-1] |
       *  +----------+ +----------+
       *
       */
      vtr::Point<size_t> curr_gsb_coord(ix, iy - 1);
      if (!device_rr_gsb.is_gsb_exist(curr_gsb_coord)) {
        continue;
      }
      const RRGSB& curr_rr_gsb = device_rr_gsb.get_gsb(curr_gsb_coord);
      if (curr_rr_gsb.is_cb_exist(CHANX)) {
        fabric_tile.add_cbx_coordinate(curr_tile_id,
                                       curr_rr_gsb.get_cb_coordinate(CHANX));
      }
      if (curr_rr_gsb.is_cb_exist(CHANY)) {
        fabric_tile.add_cby_coordinate(curr_tile_id,
                                       curr_rr_gsb.get_cb_coordinate(CHANY));
      }
      if (curr_rr_gsb.is_sb_exist()) {
        fabric_tile.add_sb_coordinate(curr_tile_id,
                                      curr_rr_gsb.get_sb_coordinate());
      }
    }
  }

  return status_code;
}

/********************************************************************
 * Build tile-level information for a given FPGA fabric, w.r.t. to configuration
 *******************************************************************/
int build_fabric_tile(FabricTile& fabric_tile, const TileConfig& tile_config, const DeviceGrid& grids, const DeviceRRGSB& device_rr_gsb) {
  vtr::ScopedStartFinishTimer timer(
    "Build tile-level information for the FPGA fabric");

  int status_code = CMD_EXEC_SUCCESS;

  /* Depending on the selected style, follow different approaches */
  if (tile_config.style() == TileConfig::e_style::TOP_LEFT) {
    status_code = build_fabric_tile_style_top_left(fabric_tile, grids, device_rr_gsb);
  } else {
    /* Error out for styles that are not supported yet! */
    VTR_LOG_ERROR("Tile style '%s' is not supported yet!\n",
                  tile_config.style_to_string().c_str());
    status_code = CMD_EXEC_FATAL_ERROR;
  }

  return status_code;
}

} /* end namespace openfpga */
