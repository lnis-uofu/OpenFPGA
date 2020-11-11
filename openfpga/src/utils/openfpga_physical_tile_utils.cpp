/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from 
 * VPR t_physical_tile_type 
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "openfpga_device_grid_utils.h"
#include "openfpga_physical_tile_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Give a given pin index, find the side where this pin is located 
 * on the physical tile
 * Note:
 *   - Need to check if the pin_width_offset and pin_height_offset
 *     are properly set in VPR!!!
 *******************************************************************/
std::vector<e_side> find_physical_tile_pin_side(t_physical_tile_type_ptr physical_tile,
                                                const int& physical_pin) {
  std::vector<e_side> pin_sides;
  for (const e_side& side_cand : {TOP, RIGHT, BOTTOM, LEFT}) {
    int pin_width_offset = physical_tile->pin_width_offset[physical_pin];
    int pin_height_offset = physical_tile->pin_height_offset[physical_pin];
    if (true == physical_tile->pinloc[pin_width_offset][pin_height_offset][side_cand][physical_pin]) {
      pin_sides.push_back(side_cand);
    } 
  }

  return pin_sides;
}

/********************************************************************
 * Find the Fc of a pin in physical tile 
 *******************************************************************/
float find_physical_tile_pin_Fc(t_physical_tile_type_ptr type,
                                const int& pin) {
  for (const t_fc_specification& fc_spec : type->fc_specs) {
    if (fc_spec.pins.end() != std::find(fc_spec.pins.begin(), fc_spec.pins.end(), pin)) {
      return fc_spec.fc_value; 
    }
  }
  /* Every pin should have a Fc, give a wrong value */
  VTR_LOGF_ERROR(__FILE__, __LINE__,
                "Fail to find the Fc for %s.pin[%lu]\n",
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
std::set<e_side> find_physical_io_tile_located_sides(const DeviceGrid& grids,
                                                     t_physical_tile_type_ptr physical_tile) {
  std::set<e_side> io_sides;
  bool center_io = false;
 
  /* Search the core part */
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
      /* If located in center, we add a NUM_SIDES and finish */
      if (physical_tile == grids[ix][iy].type) {
        io_sides.insert(NUM_SIDES);
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
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates = generate_perimeter_grid_coordinates( grids);

  for (const e_side& fpga_side : FPGA_SIDES_CLOCKWISE) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[fpga_side]) {
      /* If located in center, we add a NUM_SIDES and finish */
      if (physical_tile == grids[io_coordinate.x()][io_coordinate.y()].type) {
        io_sides.insert(fpga_side);
        break;
      } 
    }
  }

  return io_sides;
}

} /* end namespace openfpga */
