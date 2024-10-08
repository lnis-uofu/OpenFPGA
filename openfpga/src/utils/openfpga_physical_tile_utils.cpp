/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from
 * VPR t_physical_tile_type
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_device_grid_utils.h"
#include "openfpga_physical_tile_utils.h"
#include "openfpga_port_parser.h"
#include "openfpga_side_manager.h"
#include "openfpga_tokenizer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Give a given pin index, find the side where this pin is located
 * on the physical tile
 * Note:
 *   - Need to check if the pin_width_offset and pin_height_offset
 *     are properly set in VPR!!!
 *******************************************************************/
std::vector<e_side> find_physical_tile_pin_side(
  t_physical_tile_type_ptr physical_tile, const int& physical_pin,
  const e_side& border_side, const bool& perimeter_cb) {
  std::vector<e_side> pin_sides;
  for (const e_side& side_cand : {TOP, RIGHT, BOTTOM, LEFT}) {
    int pin_width_offset = physical_tile->pin_width_offset[physical_pin];
    int pin_height_offset = physical_tile->pin_height_offset[physical_pin];
    if (true == physical_tile->pinloc[pin_width_offset][pin_height_offset]
                                     [side_cand][physical_pin]) {
      pin_sides.push_back(side_cand);
    }
  }

  /* For regular grid, we should have pin only one side!
   * I/O grids: VPR creates the grid with duplicated pins on every side
   * - In regular cases: the expected side (only used side) will be on the
   * opposite to the border side!
   * - When perimeter cb is on, the expected sides can be on any sides except
   * the border side. But we only expect 1 side
   */
  if (NUM_2D_SIDES == border_side) {
    VTR_ASSERT(1 == pin_sides.size());
  } else if (!perimeter_cb) {
    SideManager side_manager(border_side);
    VTR_ASSERT(pin_sides.end() != std::find(pin_sides.begin(), pin_sides.end(),
                                            side_manager.get_opposite()));
    pin_sides.clear();
    pin_sides.push_back(side_manager.get_opposite());
  } else {
    VTR_ASSERT(1 == pin_sides.size() && pin_sides[0] != border_side);
  }

  return pin_sides;
}

/********************************************************************
 * Find the Fc of a pin in physical tile
 *******************************************************************/
float find_physical_tile_pin_Fc(t_physical_tile_type_ptr type, const int& pin) {
  for (const t_fc_specification& fc_spec : type->fc_specs) {
    if (fc_spec.pins.end() !=
        std::find(fc_spec.pins.begin(), fc_spec.pins.end(), pin)) {
      return fc_spec.fc_value;
    }
  }
  /* Every pin should have a Fc, give a wrong value */
  VTR_LOGF_ERROR(__FILE__, __LINE__, "Fail to find the Fc for %s.pin[%lu]\n",
                 type->name, pin);
  exit(1);
}

/********************************************************************
 * Find sides/locations of a I/O physical tile in the context of a FPGA fabric
 * The I/O grid may locate at
 * - one or more border of a FPGA (TOP, RIGHT, BOTTOM, LEFT)
 *   We will collect each side that the I/O locates
 * - the center of a FPGA
 *   We will add NUM_SIDEs for these I/Os
 *******************************************************************/
std::set<e_side> find_physical_io_tile_located_sides(
  const DeviceGrid& grids, t_physical_tile_type_ptr physical_tile) {
  std::set<e_side> io_sides;
  bool center_io = false;

  /* Search the core part */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      /* If located in center, we add a NUM_2D_SIDES and finish */
      if (physical_tile ==
          grids.get_physical_type(t_physical_tile_loc(ix, iy, 0))) {
        io_sides.insert(NUM_2D_SIDES);
        center_io = true;
        break;
      }
    }
    if (true == center_io) {
      break;
    }
  }

  /* Search the border side */
  /* Create the coordinate range for each side of FPGA fabric */
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates =
    generate_perimeter_grid_coordinates(grids);

  for (const e_side& fpga_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[fpga_side]) {
      /* If located in center, we add a NUM_2D_SIDES and finish */
      if (physical_tile == grids.get_physical_type(t_physical_tile_loc(
                             io_coordinate.x(), io_coordinate.y(), 0))) {
        io_sides.insert(fpga_side);
        break;
      }
    }
  }

  return io_sides;
}

/********************************************************************
 * Find the pin index of a physical tile which matches the given name.
 * For example,
 *   io[5:5].a2f[1]
 * which corresponds to the pin 'a2f[1]' of the 5th subtile 'io' in the physical
 *tile
 *******************************************************************/
int find_physical_tile_pin_index(t_physical_tile_type_ptr physical_tile,
                                 std::string pin_name) {
  /* Deposit an invalid value */
  int pin_idx = physical_tile->num_pins;
  /* precheck: return unfound pin if the tile name does not match */
  StringToken tokenizer(pin_name);
  std::vector<std::string> pin_tokens = tokenizer.split(".");
  if (pin_tokens.size() != 2) {
    VTR_LOG_ERROR("Invalid pin name '%s'. Expect <tile>.<port>\n",
                  pin_name.c_str());
    exit(1);
  }
  PortParser tile_parser(pin_tokens[0]);
  BasicPort tile_info = tile_parser.port();
  if (!tile_info.is_valid()) {
    VTR_LOG_ERROR(
      "Invalid pin name '%s' whose subtile index is not valid, expect [0, "
      "%lu]\n",
      pin_name.c_str(), physical_tile->capacity - 1);
    exit(1);
  }
  /* Bypass unmatched subtiles*/
  if (tile_info.get_name() != std::string(physical_tile->name)) {
    return pin_idx;
  }
  /* precheck: return unfound pin if the subtile index does not match */
  if (tile_info.get_width() != 1) {
    VTR_LOG_ERROR(
      "Invalid pin name '%s' whose subtile index range should be 1. For "
      "example, clb[1:1]\n",
      pin_name.c_str());
    exit(1);
  }
  /* precheck: return unfound pin if the pin index does not match */
  PortParser pin_parser(pin_tokens[1]);
  BasicPort pin_info = pin_parser.port();
  /* precheck: return unfound pin if the subtile index does not match */
  if (pin_info.get_width() != 1) {
    VTR_LOG_ERROR(
      "Invalid pin name '%s' whose pin index range should be 1. For example, "
      "clb[1:1].I[2:2]\n",
      pin_name.c_str());
    exit(1);
  }

  /* Spot the subtile by using the index */
  size_t acc_pin_index = 0;
  for (const t_sub_tile& sub_tile : physical_tile->sub_tiles) {
    /* Bypass unmatched subtiles*/
    if (!sub_tile.capacity.is_in_range(tile_info.get_lsb())) {
      acc_pin_index += sub_tile.num_phy_pins;
      continue;
    }
    for (const t_physical_tile_port& sub_tile_port : sub_tile.ports) {
      if (std::string(sub_tile_port.name) != pin_info.get_name()) {
        continue;
      }
      if (!pin_info.is_valid()) {
        VTR_LOG_ERROR(
          "Invalid pin name '%s' whose pin index is not valid, expect [0, "
          "%lu]\n",
          pin_name.c_str(), sub_tile_port.num_pins - 1);
        exit(1);
      }
      if (pin_info.get_msb() > size_t(sub_tile_port.num_pins) - 1) {
        VTR_LOG_ERROR(
          "Invalid pin name '%s' whose pin index is out of range, expect [0, "
          "%lu]\n",
          pin_name.c_str(), sub_tile_port.num_pins - 1);
        exit(1);
      }
      /* Reach here, we get the port we want, return the accumulated index */
      size_t accumulated_pin_idx =
        acc_pin_index + sub_tile_port.absolute_first_pin_index +
        (sub_tile.num_phy_pins / sub_tile.capacity.total()) *
          (tile_info.get_lsb() - sub_tile.capacity.low) +
        pin_info.get_lsb();
      return accumulated_pin_idx;
    }
  }

  return pin_idx;
}

} /* end namespace openfpga */
