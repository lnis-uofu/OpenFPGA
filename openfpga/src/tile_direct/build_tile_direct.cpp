/***************************************************************************************
 * This file includes functions that build the point-to-point direct connections
 * between tiles (programmable blocks) 
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_tokenizer.h"
#include "openfpga_port.h"
#include "openfpga_port_parser.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "build_tile_direct.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Parse the from tile name from the direct definition  
 * The definition string should be in the following format:
 *   <tile_type_name>.<pin_name>[<pin_lsb>:<pin_msb>]  
 ***************************************************************************************/
static 
std::string parse_direct_tile_name(const std::string& direct_tile_inf) {
  StringToken tokenizer(direct_tile_inf);
  std::vector<std::string> tokens = tokenizer.split('.');
  /* We should have only 2 elements and the first is tile name */
  if (2 != tokens.size()) {
    VTR_LOG_ERROR("Invalid definition on direct tile '%s'!\n\tExpect <tile_type_name>.<pin_name>[<pin_lsb>:<pin_msb>].\n",
                  direct_tile_inf.c_str());
  }

  return tokens[0];
}

/***************************************************************************************
 * Check if a pin is located on a given side of physical tile
 * If the given side is NUM_SIDES, we will search all the sides 
 ***************************************************************************************/
static 
bool is_pin_locate_at_physical_tile_side(t_physical_tile_type_ptr physical_tile,
                                         const size_t& pin_width_offset,
                                         const size_t& pin_height_offset,
                                         const size_t& pin_id,
                                         const e_side& pin_side) {
  if (NUM_SIDES == pin_side) {
    for (size_t side = 0; side < NUM_SIDES; ++side) {
      if (true == physical_tile->pinloc[pin_width_offset][pin_height_offset][side][pin_id]) {
        return true;
      }
    }
  }
  
  return physical_tile->pinloc[pin_width_offset][pin_height_offset][size_t(pin_side)][pin_id];
}

/***************************************************************************************
 * Find the pin ids of a physical tile based on the given port name, LSB and MSB 
 ***************************************************************************************/
static 
std::vector<size_t> find_physical_tile_pin_id(t_physical_tile_type_ptr physical_tile, 
                                              const size_t& pin_width_offset,
                                              const size_t& pin_height_offset,
                                              const BasicPort& tile_port,
                                              const e_side& pin_side) {
  std::vector<size_t> pin_ids;

  /* Walk through the port of the tile */
  for (const t_physical_tile_port& physical_tile_port : physical_tile->ports) {
    if (std::string(physical_tile_port.name) != tile_port.get_name()) {
      continue;
    }
    /* If the wanted port is invalid, it assumes that we want the full port */
    if (false == tile_port.is_valid()) {
      for (int ipin = 0; ipin < physical_tile_port.num_pins; ++ipin) {
        int pin_id = physical_tile_port.absolute_first_pin_index + ipin;
        VTR_ASSERT(pin_id < physical_tile->num_pins);
        /* Check if the pin is located on the wanted side */
        if (true == is_pin_locate_at_physical_tile_side(physical_tile,
                                                        pin_width_offset,
                                                        pin_height_offset,
                                                        pin_id, pin_side)) {
          pin_ids.push_back(pin_id);
        }
      }
      continue;
    }
    /* Find the LSB and MSB of the pin */
    VTR_ASSERT_SAFE(true == tile_port.is_valid());
    BasicPort ref_port(physical_tile_port.name, physical_tile_port.num_pins); 
    if (false == ref_port.contained(tile_port)) {
      VTR_LOG_ERROR("Defined direct port '%s[%lu:%lu]' is out of range for physical port '%s[%lu:%lu]'!\n",
                    tile_port.get_name().c_str(),
                    tile_port.get_lsb(), tile_port.get_msb(),
                    ref_port.get_name().c_str(),
                    ref_port.get_lsb(), ref_port.get_msb());
      exit(1);
    }
    for (const size_t& ipin : tile_port.pins()) {
      int pin_id = physical_tile_port.absolute_first_pin_index + ipin;
      VTR_ASSERT(pin_id < physical_tile->num_pins);
      /* Check if the pin is located on the wanted side */
      if (true == is_pin_locate_at_physical_tile_side(physical_tile,
                                                      pin_width_offset,
                                                      pin_height_offset,
                                                      pin_id, pin_side)) {

        pin_ids.push_back(pin_id);
      }
    }
  }

  return pin_ids;
}

/***************************************************************************************
 * Build the point-to-point direct connections based on 
 *   - original VPR arch definition 
 *     This is limited to the inner-column and inner-row connections
 *
 * Build the inner-column and inner-row connections
 *
 *     +------+
 *     | Tile |
 *     +------+            +------+    +------+
 *        |           or   | Tile |--->| Tile |
 *        v                +------+    +------+
 *     +------+
 *     | Tile |
 *     +------+
 *
 ***************************************************************************************/
static 
void build_inner_column_row_tile_direct(TileDirect& tile_direct,
                                        t_direct_inf& vpr_direct,
                                        const DeviceContext& device_ctx,
                                        const ArchDirectId& arch_direct_id) {
  /* Get the source tile and pin information */
  std::string from_tile_name = parse_direct_tile_name(std::string(vpr_direct.from_pin));
  PortParser from_tile_port_parser(std::string(vpr_direct.from_pin));
  const BasicPort& from_tile_port = from_tile_port_parser.port();

  /* Get the sink tile and pin information */
  std::string to_tile_name = parse_direct_tile_name(std::string(vpr_direct.to_pin));
  PortParser to_tile_port_parser(std::string(vpr_direct.to_pin));
  const BasicPort& to_tile_port = to_tile_port_parser.port();

  /* Walk through the device fabric and find the grid that fit the source */
  for (size_t x = 0; x < device_ctx.grid.width(); ++x) {
    for (size_t y = 0; y < device_ctx.grid.height(); ++y) {
      /* Bypass empty grid */
      if (true == is_empty_type(device_ctx.grid[x][y].type)) {
        continue;
      }

      /* Bypass the grid that does not fit the from_tile name */
      if (from_tile_name != std::string(device_ctx.grid[x][y].type->name)) {
        continue;
      }
      
      /* Try to find the pin in this tile */
      std::vector<size_t> from_pins = find_physical_tile_pin_id(device_ctx.grid[x][y].type,
                                                               device_ctx.grid[x][y].width_offset,
                                                               device_ctx.grid[x][y].height_offset,
                                                               from_tile_port,
                                                               vpr_direct.from_side);
      /* If nothing found, we can continue */
      if (0 == from_pins.size()) {
        continue;
      }
      
      /* We should try to the sink grid for inner-column/row direct connections */
      vtr::Point<size_t> from_grid_coord(x, y);
      vtr::Point<size_t> to_grid_coord(x + vpr_direct.x_offset, y + vpr_direct.y_offset);
      if ((to_grid_coord.x() >= device_ctx.grid.width())
       || (to_grid_coord.y() >= device_ctx.grid.height())) {
        continue;
      }

      /* Bypass the grid that does not fit the from_tile name */
      if (to_tile_name != std::string(device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].type->name)) {
        continue;
      }
      /* Try to find the pin in this tile */
      std::vector<size_t> to_pins = find_physical_tile_pin_id(device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].type,
                                                              device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].width_offset,
                                                              device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].height_offset,
                                                              to_tile_port,
                                                              vpr_direct.to_side);
      /* If nothing found, we can continue */
      if (0 == to_pins.size()) {
        continue;
      }

      /* If from port and to port do not match in sizes, error out */
      if (from_pins.size() != to_pins.size()) {
        VTR_LOG_ERROR("From_port '%s[%lu:%lu] of direct '%s' does not match to_port '%s[%lu:%lu]'!\n",
                      from_tile_port.get_name().c_str(),
                      from_tile_port.get_lsb(),
                      from_tile_port.get_msb(),
                      vpr_direct.name,
                      to_tile_port.get_name().c_str(),
                      to_tile_port.get_lsb(),
                      to_tile_port.get_msb());
        exit(1);
      }

      /* Now add the tile direct */
      for (size_t ipin = 0; ipin < from_pins.size(); ++ipin) {
        TileDirectId tile_direct_id = tile_direct.add_direct(device_ctx.grid[x][y].type,
                                                             from_grid_coord, vpr_direct.from_side, from_pins[ipin],
                                                             device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].type,
                                                             to_grid_coord, vpr_direct.to_side, to_pins[ipin]);
        tile_direct.set_arch_direct_id(tile_direct_id, arch_direct_id);
      }

    }
  }
}

/***************************************************************************************
 * Build the point-to-point direct connections based on 
 *   - OpenFPGA arch definition  
 *     This is limited to the inter-column and inter-row connections
 *
 * Build the inter-column and inter-row connections
 *
 *     +------+     +------+
 *     | Tile |     | Tile |
 *     +------+     +------+
 *        |            ^
 *        |            |
 *        +-------------
 *
 *     +------+
 *     | Tile |-------+ 
 *     +------+       |
 *                    |
 *     +------+       |
 *     | Tile |<------+ 
 *     +------+
 *        
 *
 ***************************************************************************************/
static 
void build_inter_column_row_tile_direct(TileDirect& tile_direct,
                                        t_direct_inf& vpr_direct,
                                        const DeviceContext& device_ctx,
                                        const ArchDirect& arch_direct,
                                        const ArchDirectId& arch_direct_id,
                                        const CircuitLibrary& circuit_lib) {
  /* Get the source tile and pin information */
  std::string from_tile_name = parse_direct_tile_name(std::string(vpr_direct.from_pin));
  PortParser from_tile_port_parser(std::string(vpr_direct.from_pin));
  const BasicPort& from_tile_port = from_tile_port_parser.port();

  /* Get the sink tile and pin information */
  std::string to_tile_name = parse_direct_tile_name(std::string(vpr_direct.to_pin));
  PortParser to_tile_port_parser(std::string(vpr_direct.to_pin));
  const BasicPort& to_tile_port = to_tile_port_parser.port();

  /* Walk through the device fabric and find the grid that fit the source */
  for (size_t x = 0; x < device_ctx.grid.width(); ++x) {
    for (size_t y = 0; y < device_ctx.grid.height(); ++y) {
      /* Bypass empty grid */
      if (true == is_empty_type(device_ctx.grid[x][y].type)) {
        continue;
      }

      /* Bypass the grid that does not fit the from_tile name */
      if (from_tile_name != std::string(device_ctx.grid[x][y].type->name)) {
        continue;
      }
      
      /* Try to find the pin in this tile */
      std::vector<size_t> from_pins = find_physical_tile_pin_id(device_ctx.grid[x][y].type,
                                                               device_ctx.grid[x][y].width_offset,
                                                               device_ctx.grid[x][y].height_offset,
                                                               from_tile_port,
                                                               vpr_direct.from_side);
      /* If nothing found, we can continue */
      if (0 == from_pins.size()) {
        continue;
      }
      
      /* We should try to the sink grid for inner-column/row direct connections */
      vtr::Point<size_t> from_grid_coord(x, y);
      vtr::Point<size_t> to_grid_coord(x + vpr_direct.x_offset, y + vpr_direct.y_offset);
      if ((to_grid_coord.x() >= device_ctx.grid.width())
       || (to_grid_coord.y() >= device_ctx.grid.height())) {
        continue;
      }

      /* Bypass the grid that does not fit the from_tile name */
      if (to_tile_name != std::string(device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].type->name)) {
        continue;
      }
      /* Try to find the pin in this tile */
      std::vector<size_t> to_pins = find_physical_tile_pin_id(device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].type,
                                                              device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].width_offset,
                                                              device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].height_offset,
                                                              to_tile_port,
                                                              vpr_direct.to_side);
      /* If nothing found, we can continue */
      if (0 == to_pins.size()) {
        continue;
      }

      /* If from port and to port do not match in sizes, error out */
      if (from_pins.size() != to_pins.size()) {
        VTR_LOG_ERROR("From_port '%s[%lu:%lu] of direct '%s' does not match to_port '%s[%lu:%lu]'!\n",
                      from_tile_port.get_name().c_str(),
                      from_tile_port.get_lsb(),
                      from_tile_port.get_msb(),
                      vpr_direct.name,
                      to_tile_port.get_name().c_str(),
                      to_tile_port.get_lsb(),
                      to_tile_port.get_msb());
        exit(1);
      }

      /* Now add the tile direct */
      for (size_t ipin = 0; ipin < from_pins.size(); ++ipin) {
        TileDirectId tile_direct_id = tile_direct.add_direct(device_ctx.grid[x][y].type,
                                                             from_grid_coord, vpr_direct.from_side, from_pins[ipin],
                                                             device_ctx.grid[to_grid_coord.x()][to_grid_coord.y()].type,
                                                             to_grid_coord, vpr_direct.to_side, to_pins[ipin]);
        tile_direct.set_arch_direct_id(tile_direct_id, arch_direct_id);
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
                                    const CircuitLibrary& circuit_lib) {
  vtr::ScopedStartFinishTimer timer("Build the annotation about direct connection between tiles");

  TileDirect tile_direct;

  /* Walk through each direct definition in the VPR arch */
  for (int idirect = 0; idirect < device_ctx.arch->num_directs; ++idirect) {
    ArchDirectId arch_direct_id = arch_direct.direct(std::string(device_ctx.arch->Directs[idirect].name));
    VTR_ASSERT(ArchDirectId::INVALID() != arch_direct_id);
    /* Build from original VPR arch definition */
    build_inner_column_row_tile_direct(tile_direct,
                                       device_ctx.arch->Directs[idirect],
                                       device_ctx,
                                       arch_direct_id);
   /* TODO: Build from OpenFPGA arch definition */ 
  } 

  return tile_direct;
}

} /* end namespace openfpga */
