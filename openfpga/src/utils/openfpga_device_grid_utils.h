#ifndef OPENFPGA_DEVICE_GRID_UTILS_H
#define OPENFPGA_DEVICE_GRID_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <string>
#include <map>
#include "device_grid.h"
#include "vtr_geometry.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* A constant array to walk through FPGA border sides clockwise*/
constexpr std::array<e_side, 4> FPGA_SIDES_CLOCKWISE{TOP, RIGHT, BOTTOM, LEFT};

std::map<e_side, std::vector<vtr::Point<size_t>>> generate_perimeter_grid_coordinates(const DeviceGrid& grids);

} /* end namespace openfpga */

#endif
