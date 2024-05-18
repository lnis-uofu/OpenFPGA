/***************************************************************************************
 * This file includes functions that build the point-to-point direct connections
 * between tiles (programmable blocks)
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_port_parser.h"
#include "openfpga_tokenizer.h"

/* Headers from vpr library */
#include "build_tile_direct.h"
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Parse the from tile name from the direct definition
 * The definition string should be in the following format:
 *   <tile_type_name>.<pin_name>[<pin_lsb>:<pin_msb>]
 ***************************************************************************************/
static std::string parse_direct_tile_name(const std::string& direct_tile_inf) {
  StringToken tokenizer(direct_tile_inf);
  std::vector<std::string> tokens = tokenizer.split('.');
  /* We should have only 2 elements and the first is tile name */
  if (2 != tokens.size()) {
    VTR_LOG_ERROR(
      "Invalid definition on direct tile '%s'!\n\tExpect "
      "<tile_type_name>.<pin_name>[<pin_lsb>:<pin_msb>].\n",
      direct_tile_inf.c_str());
  }

  return tokens[0];
}

/***************************************************************************************
 * Parse the pin name and port MSB/LSB from the direct definition
 * The definition string should be in the following format:
 *   <tile_type_name>.<pin_name>[<pin_lsb>:<pin_msb>]
 ***************************************************************************************/
static std::string parse_direct_port(const std::string& direct_tile_inf) {
  StringToken tokenizer(direct_tile_inf);
  std::vector<std::string> tokens = tokenizer.split('.');
  /* We should have only 2 elements and the first is tile name */
  if (2 != tokens.size()) {
    VTR_LOG_ERROR(
      "Invalid definition on direct tile '%s'!\n\tExpect "
      "<tile_type_name>.<pin_name>[<pin_lsb>:<pin_msb>].\n",
      direct_tile_inf.c_str());
  }

  return tokens[1];
}

/***************************************************************************************
 * Check if a pin is located on a given side of physical tile
 * If the given side is NUM_SIDES, we will search all the sides
 ***************************************************************************************/
static bool is_pin_locate_at_physical_tile_side(
  t_physical_tile_type_ptr physical_tile, const size_t& pin_width_offset,
  const size_t& pin_height_offset, const size_t& pin_id,
  const e_side& pin_side) {
  if (NUM_SIDES == pin_side) {
    for (size_t side = 0; side < NUM_SIDES; ++side) {
      if (true == physical_tile->pinloc[pin_width_offset][pin_height_offset]
                                       [side][pin_id]) {
        return true;
      }
    }
  }

  return physical_tile
    ->pinloc[pin_width_offset][pin_height_offset][size_t(pin_side)][pin_id];
}

/***************************************************************************************
 * Find the pin ids of a physical tile based on the given port name, LSB and MSB
 ***************************************************************************************/
static std::vector<size_t> find_physical_tile_pin_id(
  t_physical_tile_type_ptr physical_tile, const size_t& pin_width_offset,
  const size_t& pin_height_offset, const BasicPort& tile_port,
  const e_side& pin_side) {
  std::vector<size_t> pin_ids;

  /* Walk through the port of the tile */
  for (const t_sub_tile& sub_tile : physical_tile->sub_tiles) {
    for (const t_physical_tile_port& physical_tile_port : sub_tile.ports) {
      if (std::string(physical_tile_port.name) != tile_port.get_name()) {
        continue;
      }
      /* If the wanted port is invalid, it assumes that we want the full port */
      if (false == tile_port.is_valid()) {
        for (int subtile_index = sub_tile.capacity.low;
             subtile_index <= sub_tile.capacity.high; subtile_index++) {
          for (int ipin = 0; ipin < physical_tile_port.num_pins; ++ipin) {
            int pin_id = (subtile_index - sub_tile.capacity.low) *
                           sub_tile.num_phy_pins / sub_tile.capacity.total() +
                         physical_tile_port.absolute_first_pin_index + ipin;
            VTR_ASSERT(pin_id < physical_tile->num_pins);
            /* Check if the pin is located on the wanted side */
            if (true == is_pin_locate_at_physical_tile_side(
                          physical_tile, pin_width_offset, pin_height_offset,
                          pin_id, pin_side)) {
              pin_ids.push_back(pin_id);
            }
          }
        }
        continue;
      }
      /* Find the LSB and MSB of the pin */
      VTR_ASSERT_SAFE(true == tile_port.is_valid());
      BasicPort ref_port(physical_tile_port.name, physical_tile_port.num_pins);
      if (false == ref_port.contained(tile_port)) {
        VTR_LOG_ERROR(
          "Defined direct port '%s[%lu:%lu]' is out of range for physical port "
          "'%s[%lu:%lu]'!\n",
          tile_port.get_name().c_str(), tile_port.get_lsb(),
          tile_port.get_msb(), ref_port.get_name().c_str(), ref_port.get_lsb(),
          ref_port.get_msb());
        exit(1);
      }
      for (const size_t& ipin : tile_port.pins()) {
        int pin_id = physical_tile_port.absolute_first_pin_index +
                     ipin * sub_tile.num_phy_pins / sub_tile.capacity.total() +
                     ipin;
        VTR_ASSERT(pin_id < physical_tile->num_pins);
        /* Check if the pin is located on the wanted side */
        if (true == is_pin_locate_at_physical_tile_side(
                      physical_tile, pin_width_offset, pin_height_offset,
                      pin_id, pin_side)) {
          pin_ids.push_back(pin_id);
        }
      }
    }
  }

  return pin_ids;
}

/********************************************************************
 * Check if the grid coorindate given is in the device grid range
 *******************************************************************/
static bool is_grid_coordinate_exist_in_device(
  const DeviceGrid& device_grid, const vtr::Point<size_t>& grid_coordinate) {
  return (grid_coordinate <
          vtr::Point<size_t>(device_grid.width(), device_grid.height()));
}

/********************************************************************
 * Find the coordinate of a grid in a specific column
 * with a given type
 * This function will return the coordinate of the grid that satifies
 * the type requirement
 *******************************************************************/
static vtr::Point<size_t> find_grid_coordinate_given_type(
  const DeviceGrid& grids,
  const std::vector<vtr::Point<size_t>>& candidate_coords,
  const std::string& wanted_grid_type_name) {
  for (vtr::Point<size_t> coord : candidate_coords) {
    /* If the next column is not longer in device range, we can return */
    if (false == is_grid_coordinate_exist_in_device(grids, coord)) {
      continue;
    }
    if (wanted_grid_type_name ==
        std::string(
          grids.get_physical_type(t_physical_tile_loc(coord.x(), coord.y(), 0))
            ->name)) {
      return coord;
    }
  }
  /* Return an valid coordinate */
  return vtr::Point<size_t>(grids.width(), grids.height());
}

/********************************************************************
 * Find the coordinate of the destination clb/heterogeneous block
 * considering intra column/row direct connections in core grids
 *******************************************************************/
static vtr::Point<size_t> find_inter_direct_destination_coordinate(
  const DeviceGrid& grids, const vtr::Point<size_t>& src_coord,
  const std::string des_tile_type_name, const ArchDirect& arch_direct,
  const ArchDirectId& arch_direct_id) {
  vtr::Point<size_t> des_coord(grids.width(), grids.height());

  std::vector<size_t> first_search_space;
  std::vector<size_t> second_search_space;

  /* Cross column connection from Bottom to Top on Right
   * The next column may NOT have the grid type we want!
   * Think about heterogeneous architecture!
   * Our search space will start from the next column
   * and ends at the RIGHT side of fabric
   */
  if (e_direct_type::INTER_COLUMN == arch_direct.type(arch_direct_id)) {
    if (POSITIVE_DIR == arch_direct.x_dir(arch_direct_id)) {
      /* Our first search space will be in x-direction:
       *
       *      x      ...      nx
       *   +-----+
       *   |Grid |  ----->
       *   +-----+
       */
      for (size_t ix = src_coord.x() + 1; ix < grids.width() - 1; ++ix) {
        first_search_space.push_back(ix);
      }
    } else {
      VTR_ASSERT(NEGATIVE_DIR == arch_direct.x_dir(arch_direct_id));
      /* Our first search space will be in x-direction:
       *
       *    1   ...      x
       *              +-----+
       *     < -------|Grid |
       *              +-----+
       */
      for (size_t ix = src_coord.x() - 1; ix >= 1; --ix) {
        first_search_space.push_back(ix);
      }
    }

    /* Our second search space will be in y-direction:
     *
     *  +------+
     *  | Grid | ny
     *  +------+
     *     ^     .
     *     |     .
     *     |     .
     *  +------+
     *  | Grid | 1
     *  +------+
     */
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      second_search_space.push_back(iy);
    }

    /* For negative direction, our second search space will be in y-direction:
     *
     *  +------+
     *  | Grid | ny
     *  +------+
     *     |     .
     *     |     .
     *     v     .
     *  +------+
     *  | Grid | 1
     *  +------+
     */
    if (POSITIVE_DIR == arch_direct.y_dir(arch_direct_id)) {
      std::reverse(second_search_space.begin(), second_search_space.end());
    }
  }

  /* Cross row connection from Bottom to Top on Right
   * The next column may NOT have the grid type we want!
   * Think about heterogeneous architecture!
   * Our search space will start from the next column
   * and ends at the RIGHT side of fabric
   */
  if (e_direct_type::INTER_ROW == arch_direct.type(arch_direct_id)) {
    if (POSITIVE_DIR == arch_direct.y_dir(arch_direct_id)) {
      /* Our first search space will be in y-direction:
       *
       *  +------+
       *  | Grid | ny
       *  +------+
       *     ^     .
       *     |     .
       *     |     .
       *  +------+
       *  | Grid | y
       *  +------+
       */
      for (size_t iy = src_coord.y() + 1; iy < grids.height() - 1; ++iy) {
        first_search_space.push_back(iy);
      }
    } else {
      VTR_ASSERT(NEGATIVE_DIR == arch_direct.y_dir(arch_direct_id));
      /* For negative y-direction,
       * Our first search space will be in y-direction:
       *
       *  +------+
       *  | Grid | ny
       *  +------+
       *     |     .
       *     |     .
       *     v     .
       *  +------+
       *  | Grid | y
       *  +------+
       */
      for (size_t iy = src_coord.y() - 1; iy >= 1; --iy) {
        first_search_space.push_back(iy);
      }
    }

    /* Our second search space will be in x-direction:
     *
     *     1      ...     nx
     *  +------+       +------+
     *  | Grid |<------| Grid |
     *  +------+       +------+
     */
    for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
      second_search_space.push_back(ix);
    }

    /* For negative direction,
     * our second search space will be in x-direction:
     *
     *     1      ...     nx
     *  +------+       +------+
     *  | Grid |------>| Grid |
     *  +------+       +------+
     */
    if (POSITIVE_DIR == arch_direct.x_dir(arch_direct_id)) {
      std::reverse(second_search_space.begin(), second_search_space.end());
    }
  }

  for (size_t ix : first_search_space) {
    std::vector<vtr::Point<size_t>> next_col_row_coords;
    for (size_t iy : second_search_space) {
      if (e_direct_type::INTER_COLUMN == arch_direct.type(arch_direct_id)) {
        next_col_row_coords.push_back(vtr::Point<size_t>(ix, iy));
      } else {
        VTR_ASSERT(e_direct_type::INTER_ROW ==
                   arch_direct.type(arch_direct_id));
        /* For cross-row connection, our search space is flipped */
        next_col_row_coords.push_back(vtr::Point<size_t>(iy, ix));
      }
    }
    vtr::Point<size_t> des_coord_cand = find_grid_coordinate_given_type(
      grids, next_col_row_coords, des_tile_type_name);
    /* For a valid coordinate, we can return */
    if (true == is_grid_coordinate_exist_in_device(grids, des_coord_cand)) {
      return des_coord_cand;
    }
  }
  return des_coord;
}

/***************************************************************************************
 * Report error for port matching failure
 ***************************************************************************************/
static void report_direct_from_port_and_to_port_mismatch(
  const t_direct_inf& vpr_direct, const BasicPort& from_tile_port,
  const BasicPort& to_tile_port) {
  VTR_LOG_ERROR(
    "From_port '%s[%lu:%lu] of direct '%s' does not match to_port "
    "'%s[%lu:%lu]'!\n",
    from_tile_port.get_name().c_str(), from_tile_port.get_lsb(),
    from_tile_port.get_msb(), vpr_direct.name, to_tile_port.get_name().c_str(),
    to_tile_port.get_lsb(), to_tile_port.get_msb());
}

/***************************************************************************************
 * Build the point-to-point direct connections based on
 *   - original VPR arch definition
 *     This is limited to the inner-column and inner-row connections
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *
 * This function supports the following type of direct connection:
 * 1. Direct connection between tiles in the same column or row
 *     +------+      +------+
 *     |      |      |      |
 *     | Tile |----->| Tile |
 *     |      |      |      |
 *     +------+      +------+
 *         | direction connection
 *         v
 *     +------+
 *     |      |
 *     | Tile |
 *     |      |
 *     +------+
 *
 ***************************************************************************************/
static void build_inner_column_row_tile_direct(
  TileDirect& tile_direct, const t_direct_inf& vpr_direct,
  const DeviceContext& device_ctx, const ArchDirectId& arch_direct_id,
  const bool& verbose) {
  /* Get the source tile and pin information */
  std::string from_tile_name =
    parse_direct_tile_name(std::string(vpr_direct.from_pin));
  PortParser from_tile_port_parser(
    parse_direct_port(std::string(vpr_direct.from_pin)));
  const BasicPort& from_tile_port = from_tile_port_parser.port();

  /* Get the sink tile and pin information */
  std::string to_tile_name =
    parse_direct_tile_name(std::string(vpr_direct.to_pin));
  PortParser to_tile_port_parser(
    parse_direct_port(std::string(vpr_direct.to_pin)));
  const BasicPort& to_tile_port = to_tile_port_parser.port();

  /* Walk through the device fabric and find the grid that fit the source */
  for (size_t x = 0; x < device_ctx.grid.width(); ++x) {
    for (size_t y = 0; y < device_ctx.grid.height(); ++y) {
      t_physical_tile_loc from_phy_tile_loc(x, y, 0);
      t_physical_tile_type_ptr from_phy_tile_type =
        device_ctx.grid.get_physical_type(from_phy_tile_loc);
      /* Bypass empty grid */
      if (true == is_empty_type(from_phy_tile_type)) {
        continue;
      }

      /* Bypass the grid that does not fit the from_tile name */
      if (from_tile_name != std::string(from_phy_tile_type->name)) {
        continue;
      }

      /* Search all the sides, the from pin may locate any side!
       * Note: the vpr_direct.from_side is NUM_SIDES, which is unintialized
       * This should be reported to VPR!!!
       */
      for (const e_side& from_side : {TOP, RIGHT, BOTTOM, LEFT}) {
        /* Try to find the pin in this tile */
        std::vector<size_t> from_pins = find_physical_tile_pin_id(
          from_phy_tile_type,
          device_ctx.grid.get_width_offset(from_phy_tile_loc),
          device_ctx.grid.get_height_offset(from_phy_tile_loc), from_tile_port,
          from_side);
        /* If nothing found, we can continue */
        if (0 == from_pins.size()) {
          continue;
        }

        /* We should try to the sink grid for inner-column/row direct
         * connections */
        vtr::Point<size_t> from_grid_coord(x, y);
        vtr::Point<size_t> to_grid_coord(x + vpr_direct.x_offset,
                                         y + vpr_direct.y_offset);
        if (false == is_grid_coordinate_exist_in_device(device_ctx.grid,
                                                        to_grid_coord)) {
          continue;
        }

        t_physical_tile_loc to_phy_tile_loc(to_grid_coord.x(),
                                            to_grid_coord.y(), 0);
        t_physical_tile_type_ptr to_phy_tile_type =
          device_ctx.grid.get_physical_type(to_phy_tile_loc);
        /* Bypass the grid that does not fit the from_tile name */
        if (to_tile_name != std::string(to_phy_tile_type->name)) {
          continue;
        }

        /* Search all the sides, the to pin may locate any side!
         * Note: the vpr_direct.to_side is NUM_SIDES, which is unintialized
         * This should be reported to VPR!!!
         */
        for (const e_side& to_side : {TOP, RIGHT, BOTTOM, LEFT}) {
          /* Try to find the pin in this tile */
          std::vector<size_t> to_pins = find_physical_tile_pin_id(
            to_phy_tile_type, device_ctx.grid.get_width_offset(to_phy_tile_loc),
            device_ctx.grid.get_height_offset(to_phy_tile_loc), to_tile_port,
            to_side);
          /* If nothing found, we can continue */
          if (0 == to_pins.size()) {
            continue;
          }

          /* If from port and to port do not match in sizes, error out */
          if (from_pins.size() != to_pins.size()) {
            report_direct_from_port_and_to_port_mismatch(
              vpr_direct, from_tile_port, to_tile_port);
            exit(1);
          }

          /* Now add the tile direct */
          for (size_t ipin = 0; ipin < from_pins.size(); ++ipin) {
            VTR_LOGV(verbose,
                     "Built a inner-column/row tile-to-tile direct from "
                     "%s[%lu][%lu].%s[%lu] at side '%s' to "
                     "%s[%lu][%lu].%s[%lu] at side '%s'\n",
                     from_tile_name.c_str(), x, y,
                     from_tile_port.get_name().c_str(), from_pins[ipin],
                     SIDE_STRING[from_side], to_tile_name.c_str(),
                     to_grid_coord.x(), to_grid_coord.y(),
                     to_tile_port.get_name().c_str(), to_pins[ipin],
                     SIDE_STRING[to_side]);
            TileDirectId tile_direct_id = tile_direct.add_direct(
              from_grid_coord, from_side, from_pins[ipin], to_grid_coord,
              to_side, to_pins[ipin]);
            tile_direct.set_arch_direct_id(tile_direct_id, arch_direct_id);
          }
        }
      }
    }
  }
}

/********************************************************************
 * Build the point-to-point direct connections based on
 *   - OpenFPGA arch definition
 *     This is limited to the inter-column and inter-row connections
 *
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *
 * This function supports the following type of direct connection:
 *
 * 1. Direct connections across columns and rows
 *               +------+
 *               |      |
 *               |      v
 *     +------+  |   +------+
 *     |      |  |   |      |
 *     | Grid |  |   | Grid |
 *     |      |  |   |      |
 *     +------+  |   +------+
 *               |
 *     +------+  |   +------+
 *     |      |  |   |      |
 *     | Grid |  |   | Grid |
 *     |      |  |   |      |
 *     +------+  |   +------+
 *        |      |
 *        +------+
 *
 * Note that: this will only apply to the core grids!
 *            I/Os or any blocks on the border of fabric are NOT supported!
 *
 *******************************************************************/
static void build_inter_column_row_tile_direct(
  TileDirect& tile_direct, const t_direct_inf& vpr_direct,
  const DeviceContext& device_ctx, const ArchDirect& arch_direct,
  const ArchDirectId& arch_direct_id, const bool& verbose) {
  /* Get the source tile and pin information */
  std::string from_tile_name =
    parse_direct_tile_name(std::string(vpr_direct.from_pin));
  PortParser from_tile_port_parser(
    parse_direct_port(std::string(vpr_direct.from_pin)));
  const BasicPort& from_tile_port = from_tile_port_parser.port();

  /* Get the sink tile and pin information */
  std::string to_tile_name =
    parse_direct_tile_name(std::string(vpr_direct.to_pin));
  PortParser to_tile_port_parser(
    parse_direct_port(std::string(vpr_direct.to_pin)));
  const BasicPort& to_tile_port = to_tile_port_parser.port();

  /* Go through the direct connection list, see if we need intra-column/row
   * connection here */
  if ((e_direct_type::INTER_COLUMN != arch_direct.type(arch_direct_id)) &&
      (e_direct_type::INTER_ROW != arch_direct.type(arch_direct_id))) {
    return;
  }
  /* For cross-column connection, we will search the first valid grid in each
   * column from y = 1 to y = ny
   *
   *   +------+
   *   | Grid |  y=ny
   *   +------+
   *      ^
   *      |  search direction (when y_dir is negative)
   *     ...
   *      |
   *   +------+
   *   | Grid |  y=1
   *   +------+
   *
   */
  if (e_direct_type::INTER_COLUMN == arch_direct.type(arch_direct_id)) {
    for (size_t ix = 1; ix < device_ctx.grid.width() - 1; ++ix) {
      std::vector<vtr::Point<size_t>> next_col_src_grid_coords;
      /* For negative y- direction, we should start from y = ny */
      for (size_t iy = 1; iy < device_ctx.grid.height() - 1; ++iy) {
        next_col_src_grid_coords.push_back(vtr::Point<size_t>(ix, iy));
      }
      /* For positive y- direction, we should start from y = 1 */
      if (NEGATIVE_DIR == arch_direct.y_dir(arch_direct_id)) {
        std::reverse(next_col_src_grid_coords.begin(),
                     next_col_src_grid_coords.end());
      }

      /* Bypass the grid that does not fit the from_tile name */
      vtr::Point<size_t> from_grid_coord = find_grid_coordinate_given_type(
        device_ctx.grid, next_col_src_grid_coords, from_tile_name);
      /* Skip if we do not have a valid coordinate for source CLB/heterogeneous
       * block */
      if (false == is_grid_coordinate_exist_in_device(device_ctx.grid,
                                                      from_grid_coord)) {
        continue;
      }

      /* Search all the sides, the from pin may locate any side!
       * Note: the vpr_direct.from_side is NUM_SIDES, which is unintialized
       * This should be reported to VPR!!!
       */
      for (const e_side& from_side : {TOP, RIGHT, BOTTOM, LEFT}) {
        /* Try to find the pin in this tile */
        t_physical_tile_loc from_phy_tile_loc(from_grid_coord.x(),
                                              from_grid_coord.y(), 0);
        std::vector<size_t> from_pins = find_physical_tile_pin_id(
          device_ctx.grid.get_physical_type(from_phy_tile_loc),
          device_ctx.grid.get_width_offset(from_phy_tile_loc),
          device_ctx.grid.get_height_offset(from_phy_tile_loc), from_tile_port,
          from_side);
        /* If nothing found, we can continue */
        if (0 == from_pins.size()) {
          continue;
        }

        /* For a valid coordinate, we can find the coordinate of the destination
         * clb */
        vtr::Point<size_t> to_grid_coord =
          find_inter_direct_destination_coordinate(
            device_ctx.grid, from_grid_coord, to_tile_name, arch_direct,
            arch_direct_id);
        /* If destination clb is valid, we should add something */
        if (false == is_grid_coordinate_exist_in_device(device_ctx.grid,
                                                        to_grid_coord)) {
          continue;
        }

        /* Search all the sides, the to pin may locate any side!
         * Note: the vpr_direct.to_side is NUM_SIDES, which is unintialized
         * This should be reported to VPR!!!
         */
        for (const e_side& to_side : {TOP, RIGHT, BOTTOM, LEFT}) {
          /* Try to find the pin in this tile */
          t_physical_tile_loc to_phy_tile_loc(to_grid_coord.x(),
                                              to_grid_coord.y(), 0);
          std::vector<size_t> to_pins = find_physical_tile_pin_id(
            device_ctx.grid.get_physical_type(to_phy_tile_loc),
            device_ctx.grid.get_width_offset(to_phy_tile_loc),
            device_ctx.grid.get_height_offset(to_phy_tile_loc), to_tile_port,
            to_side);
          /* If nothing found, we can continue */
          if (0 == to_pins.size()) {
            continue;
          }

          /* If from port and to port do not match in sizes, error out */
          if (from_pins.size() != to_pins.size()) {
            report_direct_from_port_and_to_port_mismatch(
              vpr_direct, from_tile_port, to_tile_port);
            exit(1);
          }

          /* Now add the tile direct */
          for (size_t ipin = 0; ipin < from_pins.size(); ++ipin) {
            VTR_LOGV(verbose,
                     "Built a inter-column/row tile-to-tile direct from "
                     "%s[%lu][%lu].%s[%lu] at side '%s' to "
                     "%s[%lu][%lu].%s[%lu] at side '%s'\n",
                     from_tile_name.c_str(), from_grid_coord.x(),
                     from_grid_coord.y(), from_tile_port.get_name().c_str(),
                     from_pins[ipin], SIDE_STRING[from_side],
                     to_tile_name.c_str(), to_grid_coord.x(), to_grid_coord.y(),
                     to_tile_port.get_name().c_str(), to_pins[ipin],
                     SIDE_STRING[to_side]);

            TileDirectId tile_direct_id = tile_direct.add_direct(
              from_grid_coord, from_side, from_pins[ipin], to_grid_coord,
              to_side, to_pins[ipin]);
            tile_direct.set_arch_direct_id(tile_direct_id, arch_direct_id);
          }
        }
      }
    }
    return; /* Go to next direct type */
  }

  /* Reach here, it must be a cross-row connection */
  VTR_ASSERT(e_direct_type::INTER_ROW == arch_direct.type(arch_direct_id));
  /* For cross-row connection, we will search the first valid grid in each
   * column from x = 1 to x = nx
   *
   *     x=1                    x=nx
   *   +------+               +------+
   *   | Grid | <--- ... ---- | Grid |
   *   +------+               +------+
   *
   */
  for (size_t iy = 1; iy < device_ctx.grid.height() - 1; ++iy) {
    std::vector<vtr::Point<size_t>> next_col_src_grid_coords;
    /* For negative x- direction, we should start from x = nx */
    for (size_t ix = 1; ix < device_ctx.grid.width() - 1; ++ix) {
      next_col_src_grid_coords.push_back(vtr::Point<size_t>(ix, iy));
    }
    /* For positive x- direction, we should start from x = 1 */
    if (POSITIVE_DIR == arch_direct.x_dir(arch_direct_id)) {
      std::reverse(next_col_src_grid_coords.begin(),
                   next_col_src_grid_coords.end());
    }

    vtr::Point<size_t> from_grid_coord = find_grid_coordinate_given_type(
      device_ctx.grid, next_col_src_grid_coords, from_tile_name);
    /* Skip if we do not have a valid coordinate for source CLB/heterogeneous
     * block */
    if (false ==
        is_grid_coordinate_exist_in_device(device_ctx.grid, from_grid_coord)) {
      continue;
    }

    /* Search all the sides, the from pin may locate any side!
     * Note: the vpr_direct.from_side is NUM_SIDES, which is unintialized
     * This should be reported to VPR!!!
     */
    for (const e_side& from_side : {TOP, RIGHT, BOTTOM, LEFT}) {
      /* Try to find the pin in this tile */
      t_physical_tile_loc from_phy_tile_loc(from_grid_coord.x(),
                                            from_grid_coord.y(), 0);
      std::vector<size_t> from_pins = find_physical_tile_pin_id(
        device_ctx.grid.get_physical_type(from_phy_tile_loc),
        device_ctx.grid.get_width_offset(from_phy_tile_loc),
        device_ctx.grid.get_height_offset(from_phy_tile_loc), from_tile_port,
        from_side);
      /* If nothing found, we can continue */
      if (0 == from_pins.size()) {
        continue;
      }

      /* For a valid coordinate, we can find the coordinate of the destination
       * clb */
      vtr::Point<size_t> to_grid_coord =
        find_inter_direct_destination_coordinate(device_ctx.grid,
                                                 from_grid_coord, to_tile_name,
                                                 arch_direct, arch_direct_id);
      /* If destination clb is valid, we should add something */
      if (false ==
          is_grid_coordinate_exist_in_device(device_ctx.grid, to_grid_coord)) {
        continue;
      }

      /* Search all the sides, the to pin may locate any side!
       * Note: the vpr_direct.to_side is NUM_SIDES, which is unintialized
       * This should be reported to VPR!!!
       */
      for (const e_side& to_side : {TOP, RIGHT, BOTTOM, LEFT}) {
        /* Try to find the pin in this tile */
        t_physical_tile_loc to_phy_tile_loc(to_grid_coord.x(),
                                            to_grid_coord.y(), 0);
        std::vector<size_t> to_pins = find_physical_tile_pin_id(
          device_ctx.grid.get_physical_type(to_phy_tile_loc),
          device_ctx.grid.get_width_offset(to_phy_tile_loc),
          device_ctx.grid.get_height_offset(to_phy_tile_loc), to_tile_port,
          to_side);
        /* If nothing found, we can continue */
        if (0 == to_pins.size()) {
          continue;
        }

        /* If from port and to port do not match in sizes, error out */
        if (from_pins.size() != to_pins.size()) {
          report_direct_from_port_and_to_port_mismatch(
            vpr_direct, from_tile_port, to_tile_port);
          exit(1);
        }

        /* Now add the tile direct */
        for (size_t ipin = 0; ipin < from_pins.size(); ++ipin) {
          VTR_LOGV(verbose,
                   "Built a inter-column/row tile-to-tile direct from "
                   "%s[%lu][%lu].%s[%lu] at side '%s' to %s[%lu][%lu].%s[%lu] "
                   "at side '%s'\n",
                   from_tile_name.c_str(), from_grid_coord.x(),
                   from_grid_coord.y(), from_tile_port.get_name().c_str(),
                   from_pins[ipin], SIDE_STRING[from_side],
                   to_tile_name.c_str(), to_grid_coord.x(), to_grid_coord.y(),
                   to_tile_port.get_name().c_str(), to_pins[ipin],
                   SIDE_STRING[to_side]);

          TileDirectId tile_direct_id =
            tile_direct.add_direct(from_grid_coord, from_side, from_pins[ipin],
                                   to_grid_coord, to_side, to_pins[ipin]);
          tile_direct.set_arch_direct_id(tile_direct_id, arch_direct_id);
        }
      }
    }
  }
}

/***************************************************************************************
 * Top-level functions that build the point-to-point direct connections
 * between tiles (programmable blocks)
 ***************************************************************************************/
TileDirect build_device_tile_direct(const DeviceContext& device_ctx,
                                    const ArchDirect& arch_direct,
                                    const bool& verbose) {
  vtr::ScopedStartFinishTimer timer(
    "Build the annotation about direct connection between tiles");

  TileDirect tile_direct;

  /* Walk through each direct definition in the VPR arch */
  for (int idirect = 0; idirect < device_ctx.arch->num_directs; ++idirect) {
    ArchDirectId arch_direct_id =
      arch_direct.direct(std::string(device_ctx.arch->Directs[idirect].name));
    if (ArchDirectId::INVALID() == arch_direct_id) {
      VTR_LOG_ERROR(
        "Unable to find an annotation in openfpga architecture XML for "
        "<direct> '%s'!\n",
        device_ctx.arch->Directs[idirect].name);
      exit(1);
    }
    /* Build from original VPR arch definition */
    if (e_direct_type::INNER_COLUMN_OR_ROW ==
        arch_direct.type(arch_direct_id)) {
      build_inner_column_row_tile_direct(tile_direct,
                                         device_ctx.arch->Directs[idirect],
                                         device_ctx, arch_direct_id, verbose);
      /* Skip those direct connections which belong part of a connection block
       */
    }
    /* Build from OpenFPGA arch definition */
    build_inter_column_row_tile_direct(
      tile_direct, device_ctx.arch->Directs[idirect], device_ctx, arch_direct,
      arch_direct_id, verbose);
  }

  VTR_LOG(
    "Built %lu tile-to-tile direct connections\n",
    std::distance(tile_direct.directs().begin(), tile_direct.directs().end()));

  return tile_direct;
}

} /* end namespace openfpga */
