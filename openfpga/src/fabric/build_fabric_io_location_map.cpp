/********************************************************************
 * This file includes functions that are used to build the location
 * map information for the top-level module of the FPGA fabric
 * It helps OpenFPGA to link the I/O port index in top-level module
 * to the VPR I/O mapping results
 *******************************************************************/
#include <map>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "build_fabric_io_location_map.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find all the GPIO ports in the grid module
 * and cache their port/pin index in the top-level module
 *******************************************************************/
IoLocationMap build_fabric_io_location_map(const ModuleManager& module_manager,
                                           const DeviceGrid& grids) {
  vtr::ScopedStartFinishTimer timer("Create I/O location mapping for top module");

  IoLocationMap io_location_map;

  std::map<std::string, size_t> io_counter;

  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> io_sides{TOP, RIGHT, BOTTOM, LEFT};
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

  /* Walk through all the grids on the perimeter, which are I/O grids */
  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      /* Bypass EMPTY grid */
      if (true == is_empty_type(grids[io_coordinate.x()][io_coordinate.y()].type)) {
        continue;
      } 

      /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
      if ( (0 < grids[io_coordinate.x()][io_coordinate.y()].width_offset)
        || (0 < grids[io_coordinate.x()][io_coordinate.y()].height_offset)) {
        continue;
      }

      t_physical_tile_type_ptr grid_type = grids[io_coordinate.x()][io_coordinate.y()].type;

      /* Find the module name for this type of grid */
      std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
      std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), is_io_type(grid_type), io_side);
      ModuleId grid_module = module_manager.find_module(grid_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

      /* Find all the GPIO ports in the grid module */

      /* MUST DO: register in io location mapping!
       * I/O location mapping is a critical look-up for testbench generators
       * As we add the I/O grid instances to top module by following order:
       * TOP -> RIGHT -> BOTTOM -> LEFT
       * The I/O index will increase in this way as well.
       * This organization I/O indices is also consistent to the way 
       * that GPIOs are wired in function connect_gpio_module()
       *
       * Note: if you change the GPIO function, you should update here as well!
       */
      for (int z = 0; z < grids[io_coordinate.x()][io_coordinate.y()].type->capacity; ++z) {
        for (const BasicPort& gpio_port : module_manager.module_ports_by_type(grid_module, ModuleManager::MODULE_GPIO_PORT)) {
          auto curr_io_index = io_counter.find(gpio_port.get_name());
          /* Index always start from zero */
          if (curr_io_index == io_counter.end()) {
            io_counter[gpio_port.get_name()] = 0;
          }
          io_location_map.set_io_index(io_coordinate.x(), io_coordinate.y(), z,
                                       gpio_port.get_name(),
                                       io_counter[gpio_port.get_name()]);
          io_counter[gpio_port.get_name()]++;
        }
      }
    }
  }

  /* Check all the GPIO ports in the top-level module has been mapped */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  for (const BasicPort& gpio_port : module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT)) {
    VTR_ASSERT(io_counter[gpio_port.get_name()] == gpio_port.get_width());
  }

  return io_location_map;
}

} /* end namespace openfpga */
