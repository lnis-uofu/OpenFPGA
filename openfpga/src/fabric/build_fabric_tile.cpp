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
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * With a given coordinate of a grid, find an existing fabric tile
 * or create a new fabric tile
 * - A grid may never exist in any fabric tile (no coordinate matches)
 *   Create a new one
 * - A grid already in another fabric tile (occur in heterogeneous blocks)
 *   Find the existing one
 *******************************************************************/
static int find_or_create_one_fabric_tile_from_grid(
  FabricTile& fabric_tile, FabricTileId& curr_tile_id, const DeviceGrid& grids,
  const t_physical_tile_loc& tile_loc, const bool& verbose) {
  t_physical_tile_type_ptr phy_tile_type = grids.get_physical_type(tile_loc);
  vtr::Point<size_t> curr_tile_coord(tile_loc.x, tile_loc.y);
  vtr::Point<size_t> curr_gsb_coord(tile_loc.x, tile_loc.y);

  bool skip_add_pb = false;
  /* For EMPTY grid, routing blocks may still be required if there is a gsb
   */
  if (true == is_empty_type(phy_tile_type)) {
    return CMD_EXEC_SUCCESS;
  } else if ((0 < grids.get_width_offset(tile_loc)) ||
             (0 < grids.get_height_offset(tile_loc))) {
    /* Skip width, height > 1 tiles (mostly heterogeneous blocks) */
    /* Find the root of this grid, the instance id should be valid.
     * We just copy it here
     */
    vtr::Point<size_t> root_tile_coord(
      curr_tile_coord.x() - grids.get_width_offset(tile_loc),
      curr_tile_coord.y() - grids.get_height_offset(tile_loc));
    skip_add_pb = true;
    VTR_LOGV(verbose,
             "Tile[%lu][%lu] contains a heterogeneous block which is "
             "rooted from tile[%lu][%lu]\n",
             curr_tile_coord.x(), curr_tile_coord.y(), root_tile_coord.x(),
             root_tile_coord.y());
    curr_tile_id = fabric_tile.find_tile(root_tile_coord);
    /* Update the coordinates of the pb in tiles */
    size_t root_pb_idx_in_curr_tile =
      fabric_tile.find_pb_index_in_tile(curr_tile_id, root_tile_coord);
    int status_code = fabric_tile.set_pb_max_coordinate(
      curr_tile_id, root_pb_idx_in_curr_tile, curr_tile_coord);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  } else {
    /* Need to create a new tile here */
    VTR_LOGV(verbose, "Create a regular tile[%lu][%lu]\n", curr_tile_coord.x(),
             curr_tile_coord.y());
    curr_tile_id = fabric_tile.create_tile(curr_tile_coord);
  }

  /* Ensure that we have a valid id */
  if (!fabric_tile.valid_tile_id(curr_tile_id)) {
    VTR_LOG_ERROR("Failed to get a valid id for tile[%lu][%lu]!\n",
                  curr_tile_coord.x(), curr_tile_coord.y());
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Add components: pb, cbx, cby, and sb if exists */
  if (!skip_add_pb) {
    fabric_tile.add_pb_coordinate(curr_tile_id, curr_tile_coord,
                                  curr_gsb_coord);
  }
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Build tiles by following the bottom style.
 * - The programmble block, e.g., clb, is placed on the bottom-left corner
 * - The connection blocks and switch block are placed on the top and bottom
 *sides
 * This is exactly how GSB is organized. Just need to transfer data from one GSB
 * The gsb coordinate is the same as the grid coordinate when the
 * bottom-left style is considered
 *
 * ------------------------------
 *  +----------+ +----------+   ^
 *  | CBx      | | SB       |   |
 *  | [x][y]   | | [x][y]   |  GSB[x][y]
 *  +----------+ +----------+   |
 *  +----------+ +----------+   |
 *  |   Grid   | | CBy      |   |
 *  |  [x][y]  | | [x][y]   |   |
 *  +----------+ +----------+   v
 * ------------------------------
 *
 *******************************************************************/
static int build_fabric_tile_style_bottom_left(FabricTile& fabric_tile,
                                               const DeviceGrid& grids,
                                               const size_t& layer,
                                               const RRGraphView& rr_graph,
                                               const DeviceRRGSB& device_rr_gsb,
                                               const bool& verbose) {
  int status_code = CMD_EXEC_SUCCESS;

  /* Walk through all the device rr_gsb and create tile one by one */
  for (size_t ix = 0; ix < grids.width(); ++ix) {
    for (size_t iy = 0; iy < grids.height(); ++iy) {
      t_physical_tile_loc tile_loc(ix, iy, layer);
      FabricTileId curr_tile_id = FabricTileId::INVALID();
      status_code = find_or_create_one_fabric_tile_from_grid(
        fabric_tile, curr_tile_id, grids, tile_loc, verbose);
      if (status_code != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
      /* If no tile is created for the pb, check if routing exists */
      vtr::Point<size_t> curr_tile_coord(tile_loc.x, tile_loc.y);
      vtr::Point<size_t> curr_gsb_coord(ix, iy);
      if (!fabric_tile.valid_tile_id(curr_tile_id)) {
        if (!device_rr_gsb.is_gsb_exist(rr_graph, curr_gsb_coord)) {
          VTR_LOGV(verbose, "Skip tile[%lu][%lu] as it is empty\n",
                   curr_tile_coord.x(), curr_tile_coord.y());
          continue;
        }
        /* Need to create a new tile here */
        VTR_LOGV(verbose,
                 "Create tile[%lu][%lu] which only has routing but not a "
                 "programmable block\n",
                 curr_tile_coord.x(), curr_tile_coord.y());
        curr_tile_id = fabric_tile.create_tile(curr_tile_coord);
      }
      if (fabric_tile.valid_tile_id(curr_tile_id) &&
          !device_rr_gsb.is_gsb_exist(rr_graph, curr_gsb_coord)) {
        VTR_LOGV(
          verbose,
          "Skip to add routing to tile[%lu][%lu] as it is not required\n",
          curr_tile_coord.x(), curr_tile_coord.y());
        continue;
      }
      const RRGSB& curr_rr_gsb = device_rr_gsb.get_gsb(curr_gsb_coord);
      for (e_rr_type cb_type : {e_rr_type::CHANX, e_rr_type::CHANY}) {
        if (curr_rr_gsb.is_cb_exist(cb_type)) {
          fabric_tile.add_cb_coordinate(curr_tile_id, cb_type,
                                        curr_rr_gsb.get_sb_coordinate());
          VTR_LOGV(verbose,
                   "Added %s connection block [%lu][%lu] to tile[%lu][%lu]\n",
                   cb_type == e_rr_type::CHANX ? "x-" : "y-",
                   curr_rr_gsb.get_cb_x(cb_type), curr_rr_gsb.get_cb_y(cb_type),
                   ix, iy);
        }
      }
      if (curr_rr_gsb.is_sb_exist(rr_graph)) {
        fabric_tile.add_sb_coordinate(curr_tile_id,
                                      curr_rr_gsb.get_sb_coordinate());
        VTR_LOGV(verbose, "Added switch block [%lu][%lu] to tile[%lu][%lu]\n",
                 curr_rr_gsb.get_sb_x(), curr_rr_gsb.get_sb_y(), ix, iy);
      }
    }
  }

  return status_code;
}

/********************************************************************
 * Build tiles by following the top-left style.
 * - The programmble block, e.g., clb, is placed on the top-left corner
 * - The connection blocks and switch block are placed on the right and bottom
 *sides
 * Tile[x][y]
 * ------------------------------
 *  +----------+ +----------+   ^
 *  |   Grid   | | CBy      |   GSB[x][y]
 *  |  [x][y]  | | [x][y]   |   |
 *  +----------+ +----------+   v
 * ------------------------------
 *  +----------+ +----------+   ^
 *  | CBx      | | SB       |   |
 *  | [x][y-1] | | [x][y-1] |  GSB[x][y-1]
 *  +----------+ +----------+   |
 * ------------------------------

 *******************************************************************/
static int build_fabric_tile_style_top_left(FabricTile& fabric_tile,
                                            const DeviceGrid& grids,
                                            const size_t& layer,
                                            const RRGraphView& rr_graph,
                                            const DeviceRRGSB& device_rr_gsb,
                                            const bool& verbose) {
  int status_code = CMD_EXEC_SUCCESS;

  /* Walk through all the device rr_gsb and create tile one by one */
  for (size_t ix = 0; ix < grids.width(); ++ix) {
    for (size_t iy = 0; iy < grids.height(); ++iy) {
      t_physical_tile_loc tile_loc(ix, iy, layer);
      FabricTileId curr_tile_id = FabricTileId::INVALID();
      status_code = find_or_create_one_fabric_tile_from_grid(
        fabric_tile, curr_tile_id, grids, tile_loc, verbose);
      if (status_code != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
      /* If no valid tile is created/found by the pb, check if there is any
       * routing inside */
      vtr::Point<size_t> curr_tile_coord(tile_loc.x, tile_loc.y);
      vtr::Point<size_t> curr_gsb_coord(ix, iy);
      vtr::Point<size_t> neighbor_gsb_coord(ix, iy - 1);
      if (!fabric_tile.valid_tile_id(curr_tile_id)) {
        bool routing_exist = false;
        if (device_rr_gsb.is_gsb_exist(rr_graph, curr_gsb_coord)) {
          const RRGSB& routing_rr_gsb = device_rr_gsb.get_gsb(curr_gsb_coord);
          if (routing_rr_gsb.is_cb_exist(e_rr_type::CHANY)) {
            routing_exist = true;
          }
        }
        if (device_rr_gsb.is_gsb_exist(rr_graph, neighbor_gsb_coord)) {
          const RRGSB& routing_rr_gsb =
            device_rr_gsb.get_gsb(neighbor_gsb_coord);
          if (routing_rr_gsb.is_cb_exist(e_rr_type::CHANX) ||
              routing_rr_gsb.is_sb_exist(rr_graph)) {
            routing_exist = true;
          }
        }
        if (!routing_exist) {
          VTR_LOGV(verbose, "Skip tile[%lu][%lu] as it is empty\n",
                   curr_tile_coord.x(), curr_tile_coord.y());
          continue;
        }
        /* Need to create a new tile here */
        VTR_LOGV(verbose,
                 "Create tile[%lu][%lu] which only has routing but not a "
                 "programmable block\n",
                 curr_tile_coord.x(), curr_tile_coord.y());
        curr_tile_id = fabric_tile.create_tile(curr_tile_coord);
      }
      /* For the cby in the same gsb */
      if (device_rr_gsb.is_gsb_exist(rr_graph, curr_gsb_coord)) {
        const RRGSB& curr_rr_gsb = device_rr_gsb.get_gsb(curr_gsb_coord);
        if (curr_rr_gsb.is_cb_exist(e_rr_type::CHANY)) {
          fabric_tile.add_cb_coordinate(curr_tile_id, e_rr_type::CHANY,
                                        curr_rr_gsb.get_sb_coordinate());
          VTR_LOGV(verbose,
                   "Added y- connection block [%lu][%lu] to tile[%lu][%lu]\n",
                   curr_rr_gsb.get_cb_x(e_rr_type::CHANY),
                   curr_rr_gsb.get_cb_y(e_rr_type::CHANY), ix, iy);
        }
      }
      /* For the cbx and sb in the neighbour gsb */
      if (device_rr_gsb.is_gsb_exist(rr_graph, neighbor_gsb_coord)) {
        const RRGSB& neighbor_rr_gsb =
          device_rr_gsb.get_gsb(neighbor_gsb_coord);
        if (neighbor_rr_gsb.is_cb_exist(e_rr_type::CHANX)) {
          fabric_tile.add_cb_coordinate(curr_tile_id, e_rr_type::CHANX,
                                        neighbor_rr_gsb.get_sb_coordinate());

          VTR_LOGV(verbose,
                   "Added x- connection block [%lu][%lu] to tile[%lu][%lu]\n",
                   neighbor_rr_gsb.get_cb_x(e_rr_type::CHANX),
                   neighbor_rr_gsb.get_cb_y(e_rr_type::CHANX), ix, iy);
        }
        if (neighbor_rr_gsb.is_sb_exist(rr_graph)) {
          fabric_tile.add_sb_coordinate(curr_tile_id,
                                        neighbor_rr_gsb.get_sb_coordinate());
          VTR_LOGV(verbose, "Added switch block [%lu][%lu] to tile[%lu][%lu]\n",
                   neighbor_rr_gsb.get_sb_x(), neighbor_rr_gsb.get_sb_y(), ix,
                   iy);
        }
      }
    }
  }

  return status_code;
}

/********************************************************************
 * Build tile-level information for a given FPGA fabric, w.r.t. to configuration
 *******************************************************************/
int build_fabric_tile(FabricTile& fabric_tile, const TileConfig& tile_config,
                      const DeviceGrid& grids, const RRGraphView& rr_graph,
                      const DeviceRRGSB& device_rr_gsb, const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Build tile-level information for the FPGA fabric");

  int status_code = CMD_EXEC_SUCCESS;

  fabric_tile.init(vtr::Point<size_t>(grids.width(), grids.height()));

  /* Depending on the selected style, follow different approaches */
  if (tile_config.style() == TileConfig::e_style::TOP_LEFT) {
    status_code = build_fabric_tile_style_top_left(
      fabric_tile, grids, 0, rr_graph, device_rr_gsb, verbose);
  } else if (tile_config.style() == TileConfig::e_style::BOTTOM_LEFT) {
    status_code = build_fabric_tile_style_bottom_left(
      fabric_tile, grids, 0, rr_graph, device_rr_gsb, verbose);

  } else {
    /* Error out for styles that are not supported yet! */
    VTR_LOG_ERROR("Tile style '%s' is not supported yet!\n",
                  tile_config.style_to_string().c_str());
    status_code = CMD_EXEC_FATAL_ERROR;
  }

  if (status_code != CMD_EXEC_SUCCESS) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Build unique tiles to compress the number of tile modules to be built in
   * later steps */
  status_code = fabric_tile.build_unique_tiles(grids, device_rr_gsb, verbose);
  VTR_LOGV(verbose, "Extracted %lu uniques tiles from the FPGA fabric\n",
           fabric_tile.unique_tiles().size());

  return status_code;
}

} /* end namespace openfpga */
