/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from 
 * VPR DeviceGrid
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "openfpga_device_grid_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Create a list of the coordinates for the grids on the device perimeter
 * It follows a clockwise sequence when including the coordinates.
 * Detailed sequence is as follows:
 *   - TOP side, from left most to the right
 *   - Right side, from top to the bottom
 *   - Bottom side, from right to the left
 *   - Left side, from bottom to top
 *
 * This function currently does not include corner cells!
 * i.e., the top-left, top-right, bottom-left and bottom-right
 *
 * Note:
 *   - This function offers a standard sequence to walk through the 
 *     grids on the perimeter of an FPGA device
 *     When sequence matters, this function should be used to ensure
 *     consistency between functions.
 *******************************************************************/
std::map<e_side, std::vector<vtr::Point<size_t>>> generate_perimeter_grid_coordinates(const DeviceGrid& grids) {
  /* Search the border side */
  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> fpga_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* TOP side*/
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) { 
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, grids.height() - 1));
  } 

  /* RIGHT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) { 
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(grids.width() - 1, iy));
  } 

  /* BOTTOM side*/
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) { 
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  } 

  /* LEFT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) { 
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  return io_coordinates;
}

} /* end namespace openfpga */
