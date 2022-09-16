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

#include "module_manager_utils.h"
#include "openfpga_device_grid_utils.h"
#include "build_fabric_io_location_map.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find all the GPIO ports in the grid module
 * and cache their port/pin index in the top-level module
 *
 * .. note:: The I/O sequence(indexing) is already determined in the io_children() list of top-level module. Here we just build a fast lookup from (x, y, z) coordinate to the actual indices
 *******************************************************************/
IoLocationMap build_fabric_io_location_map(const ModuleManager& module_manager,
                                           const DeviceGrid& grids) {
  vtr::ScopedStartFinishTimer timer("Create I/O location mapping for top module");

  IoLocationMap io_location_map;

  std::map<std::string, size_t> io_counter;

  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Walk through the I/O child list */
  for (size_t ichild = 0; ichild < module_manager.io_children(top_module).size(); ++ichild) {
    ModuleId child = module_manager.io_children(top_module)[ichild];
    vtr::Point<int> coord = module_manager.io_child_coordinates(top_module)[ichild];

    /* Bypass EMPTY grid */
    if (true == is_empty_type(grids[coord.x()][coord.y()].type)) {
      continue;
    } 

    /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
    if ( (0 < grids[coord.x()][coord.y()].width_offset)
      || (0 < grids[coord.x()][coord.y()].height_offset)) {
      continue;
    }

    VTR_ASSERT_SAFE(true == module_manager.valid_module_id(child));

    /* Find all the GPIO ports in the grid module */

    /* MUST DO: register in io location mapping!
     * I/O location mapping is a critical look-up for testbench generators
     */
    if (size_t(grids[coord.x()][coord.y()].type->capacity) != module_manager.io_children(child).size()) {
      VTR_LOG("%s[%ld][%ld] capacity: %d while io_child number is %d", grids[coord.x()][coord.y()].type->name, coord.x(), coord.y(), grids[coord.x()][coord.y()].type->capacity, module_manager.io_children(child).size());
    }
    VTR_ASSERT(size_t(grids[coord.x()][coord.y()].type->capacity) == module_manager.io_children(child).size());
    for (size_t isubchild = 0; isubchild < module_manager.io_children(child).size(); ++isubchild) {
      vtr::Point<int> subchild_coord = module_manager.io_child_coordinates(child)[isubchild];

      for (const ModuleManager::e_module_port_type& module_io_port_type : MODULE_IO_PORT_TYPES) {
        for (const ModulePortId& gpio_port_id : module_manager.module_port_ids_by_type(child, module_io_port_type)) {
          /* Only care mappable I/O */
          if (false == module_manager.port_is_mappable_io(child, gpio_port_id)) {
            continue;
          }
          const BasicPort& gpio_port = module_manager.module_port(child, gpio_port_id);

          auto curr_io_index = io_counter.find(gpio_port.get_name());
          /* Index always start from zero */
          if (curr_io_index == io_counter.end()) {
            io_counter[gpio_port.get_name()] = 0;
          }
          io_location_map.set_io_index(coord.x(), coord.y(), subchild_coord.x(),
                                       gpio_port.get_name(),
                                       io_counter[gpio_port.get_name()]);
          io_counter[gpio_port.get_name()]++;
        }
      }
    }
  }

  /* Check all the GPIO ports in the top-level module has been mapped */
  for (const ModuleManager::e_module_port_type& module_io_port_type : MODULE_IO_PORT_TYPES) {
    for (const ModulePortId& gpio_port_id : module_manager.module_port_ids_by_type(top_module, module_io_port_type)) {
      /* Only care mappable I/O */
      if (false == module_manager.port_is_mappable_io(top_module, gpio_port_id)) {
        continue;
      }
      
      const BasicPort& gpio_port = module_manager.module_port(top_module, gpio_port_id);
      VTR_ASSERT(io_counter[gpio_port.get_name()] == gpio_port.get_width());
    }
  }

  return io_location_map;
}

} /* end namespace openfpga */
