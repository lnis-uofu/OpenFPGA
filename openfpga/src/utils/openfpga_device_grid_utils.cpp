/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from
 * VPR DeviceGrid
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "openfpga_device_grid_utils.h"

#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

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
std::map<e_side, std::vector<vtr::Point<size_t>>>
generate_perimeter_grid_coordinates(const DeviceGrid& grids) {
  /* Search the border side */
  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> fpga_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* TOP side*/
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, grids.height() - 1));
  }

  /* RIGHT side */
  for (size_t iy = grids.height() - 2; iy > 0; --iy) {
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(grids.width() - 1, iy));
  }

  /* BOTTOM side*/
  for (size_t ix = grids.width() - 2; ix > 0; --ix) {
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  }

  /* LEFT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  return io_coordinates;
}

/********************************************************************
 * Create a list of the coordinates for the tiles on the device perimeter
 * It follows a clockwise sequence when including the coordinates.
 * Note that different from the function for grids, this function include corner
 *tiles! Detailed sequence is as follows:
 *   - Top-left
 *   - TOP side, from left most to the right
 *   - Top-right
 *   - Right side, from top to the bottom
 *   - Bottom-right
 *   - Bottom side, from right to the left
 *   - Bottom-left
 *   - Left side, from bottom to top
 *
 * Note:
 *   - This function offers a standard sequence to walk through the
 *     grids on the perimeter of an FPGA device
 *     When sequence matters, this function should be used to ensure
 *     consistency between functions.
 *******************************************************************/
std::map<e_side, std::vector<vtr::Point<size_t>>>
generate_perimeter_tile_coordinates(const DeviceGrid& grids) {
  /* Search the border side */
  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> fpga_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* Top-left */
  io_coordinates[TOP].push_back(vtr::Point<size_t>(0, grids.height() - 1));
  /* TOP side*/
  for (size_t ix = 1; ix < grids.width() - 1; ++ix) {
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, grids.height() - 1));
  }

  /* Top-Right */
  io_coordinates[RIGHT].push_back(
    vtr::Point<size_t>(grids.width() - 1, grids.height() - 1));
  /* RIGHT side */
  for (size_t iy = grids.height() - 2; iy > 0; --iy) {
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(grids.width() - 1, iy));
  }

  /* Bottom-right */
  io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(grids.width() - 1, 0));
  /* BOTTOM side*/
  for (size_t ix = grids.width() - 2; ix > 0; --ix) {
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  }

  /* Bottom-left */
  io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(0, 0));
  /* LEFT side */
  for (size_t iy = 1; iy < grids.height() - 1; ++iy) {
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  return io_coordinates;
}

} /* end namespace openfpga */
