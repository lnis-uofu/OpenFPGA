/********************************************************************
 * This file includes functions that are used to build the location
 * map information for the top-level module of the FPGA fabric
 * It helps OpenFPGA to link the I/O port index in top-level module
 * to the VPR I/O mapping results
 *******************************************************************/
#include <algorithm>
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "build_fabric_io_location_map.h"
#include "module_manager_utils.h"
#include "openfpga_device_grid_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find all the GPIO ports in the grid module
 * and cache their port/pin index in the top-level module
 *
 * .. note:: The I/O sequence(indexing) is already determined in the
 *io_children() list of top-level module. Here we just build a fast lookup from
 *(x, y, z) coordinate to the actual indices
 *******************************************************************/
static IoLocationMap build_fabric_fine_grained_io_location_map(
  const ModuleManager& module_manager, const DeviceGrid& grids,
  const size_t& layer) {
  vtr::ScopedStartFinishTimer timer(
    "Create I/O location mapping for top module");

  IoLocationMap io_location_map;

  std::map<std::string, size_t> io_counter;

  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Walk through the I/O child list */
  for (size_t ichild = 0;
       ichild < module_manager.io_children(top_module).size(); ++ichild) {
    ModuleId child = module_manager.io_children(top_module)[ichild];
    vtr::Point<int> coord =
      module_manager.io_child_coordinates(top_module)[ichild];
    t_physical_tile_loc phy_tile_loc(coord.x(), coord.y(), layer);
    t_physical_tile_type_ptr phy_tile_type =
      grids.get_physical_type(phy_tile_loc);

    /* Bypass EMPTY grid */
    if (true == is_empty_type(phy_tile_type)) {
      continue;
    }

    /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
    if ((0 < grids.get_width_offset(phy_tile_loc)) ||
        (0 < grids.get_height_offset(phy_tile_loc))) {
      continue;
    }

    VTR_ASSERT_SAFE(true == module_manager.valid_module_id(child));

    /* Find all the GPIO ports in the grid module */

    /* MUST DO: register in io location mapping!
     * I/O location mapping is a critical look-up for testbench generators
     */
    if (size_t(phy_tile_type->capacity) !=
        module_manager.io_children(child).size()) {
      VTR_LOG("%s[%ld][%ld] capacity: %d while io_child number is %d",
              phy_tile_type->name.c_str(), coord.x(), coord.y(),
              phy_tile_type->capacity,
              module_manager.io_children(child).size());
    }
    VTR_ASSERT(size_t(phy_tile_type->capacity) ==
               module_manager.io_children(child).size());
    for (size_t isubchild = 0;
         isubchild < module_manager.io_children(child).size(); ++isubchild) {
      /* Note that we should use the subchild module when checking the GPIO
       * ports. The child module is actually the grid-level I/O module, while
       * the subchild module is the subtile inside grid-level I/O modules. Note
       * that grid-level I/O module contains all the GPIO ports while the
       * subtile may have part of it. For example, a grid I/O module may have 24
       * GPINs and 12 GPOUTs, while the first subtile only have 4 GPINs, and the
       * second subtile only have 3 GPOUTs. Therefore, to accurately build the
       * I/O location map downto subtile level, we need to check the subchild
       * module here.
       */
      ModuleId subchild = module_manager.io_children(child)[isubchild];
      vtr::Point<int> subchild_coord =
        module_manager.io_child_coordinates(child)[isubchild];

      for (const ModuleManager::e_module_port_type& module_io_port_type :
           MODULE_IO_PORT_TYPES) {
        for (const ModulePortId& gpio_port_id :
             module_manager.module_port_ids_by_type(subchild,
                                                    module_io_port_type)) {
          /* Only care mappable I/O */
          if (false ==
              module_manager.port_is_mappable_io(subchild, gpio_port_id)) {
            continue;
          }
          const BasicPort& gpio_port =
            module_manager.module_port(subchild, gpio_port_id);

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
  for (const ModuleManager::e_module_port_type& module_io_port_type :
       MODULE_IO_PORT_TYPES) {
    for (const ModulePortId& gpio_port_id :
         module_manager.module_port_ids_by_type(top_module,
                                                module_io_port_type)) {
      /* Only care mappable I/O */
      if (false ==
          module_manager.port_is_mappable_io(top_module, gpio_port_id)) {
        continue;
      }

      const BasicPort& gpio_port =
        module_manager.module_port(top_module, gpio_port_id);
      VTR_ASSERT(io_counter[gpio_port.get_name()] == gpio_port.get_width());
    }
  }

  return io_location_map;
}

/********************************************************************
 * Find all the GPIO ports in the tile modules
 * and cache their port/pin index in the top-level module
 *
 * .. note:: The I/O sequence(indexing) is already determined in the
 *io_children() list of top-level module. Here we just build a fast lookup from
 *(x, y, z) coordinate to the actual indices
 *******************************************************************/
static IoLocationMap build_fabric_tiled_io_location_map(
  const ModuleManager& module_manager, const DeviceGrid& grids,
  const size_t& layer) {
  vtr::ScopedStartFinishTimer timer(
    "Create I/O location mapping for top module");

  IoLocationMap io_location_map;

  std::map<std::string, size_t> io_counter;

  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Walk through the I/O child list */
  for (size_t ichild = 0;
       ichild < module_manager.io_children(top_module).size(); ++ichild) {
    ModuleId child = module_manager.io_children(top_module)[ichild];
    vtr::Point<int> coord =
      module_manager.io_child_coordinates(top_module)[ichild];
    t_physical_tile_loc phy_tile_loc(coord.x(), coord.y(), layer);
    t_physical_tile_type_ptr phy_tile_type =
      grids.get_physical_type(phy_tile_loc);

    /* Bypass EMPTY grid */
    if (true == is_empty_type(phy_tile_type)) {
      continue;
    }

    /* Skip width or height > 1 tiles (mostly heterogeneous blocks) */
    if ((0 < grids.get_width_offset(phy_tile_loc)) ||
        (0 < grids.get_height_offset(phy_tile_loc))) {
      continue;
    }

    VTR_ASSERT_SAFE(true == module_manager.valid_module_id(child));

    /* Find all the GPIO ports in the grid module */

    /* MUST DO: register in io location mapping!
     * I/O location mapping is a critical look-up for testbench generators
     */
    for (ModuleId tile_child : module_manager.io_children(child)) {
      /* Note that we should use the subchild of the subchild module when
       * checking the GPIO ports. The child module is the tile module while the
       * subchild module is actually the grid-level I/O module, while the
       * subchild module is the subtile inside grid-level I/O modules. Note that
       * grid-level I/O module contains all the GPIO ports while the subtile may
       * have part of it. For example, a grid I/O module may have 24 GPINs and
       * 12 GPOUTs, while the first subtile only have 4 GPINs, and the second
       * subtile only have 3 GPOUTs. Therefore, to accurately build the I/O
       * location map downto subtile level, we need to check the subchild module
       * here.
       */
      if (size_t(phy_tile_type->capacity) !=
          module_manager.io_children(tile_child).size()) {
        VTR_LOG("%s[%ld][%ld] capacity: %d while io_child number is %d",
                phy_tile_type->name.c_str(), coord.x(), coord.y(),
                phy_tile_type->capacity,
                module_manager.io_children(tile_child).size());
      }
      VTR_ASSERT(size_t(phy_tile_type->capacity) ==
                 module_manager.io_children(tile_child).size());
      for (size_t isubchild = 0;
           isubchild < module_manager.io_children(tile_child).size();
           ++isubchild) {
        ModuleId subchild = module_manager.io_children(tile_child)[isubchild];
        vtr::Point<int> subchild_coord =
          module_manager.io_child_coordinates(tile_child)[isubchild];

        for (const ModuleManager::e_module_port_type& module_io_port_type :
             MODULE_IO_PORT_TYPES) {
          for (const ModulePortId& gpio_port_id :
               module_manager.module_port_ids_by_type(subchild,
                                                      module_io_port_type)) {
            /* Only care mappable I/O */
            if (false ==
                module_manager.port_is_mappable_io(subchild, gpio_port_id)) {
              continue;
            }
            const BasicPort& gpio_port =
              module_manager.module_port(subchild, gpio_port_id);

            auto curr_io_index = io_counter.find(gpio_port.get_name());
            /* Index always start from zero */
            if (curr_io_index == io_counter.end()) {
              io_counter[gpio_port.get_name()] = 0;
            }
            /* This is a dirty hack! */
            io_location_map.set_io_index(
              coord.x(), coord.y(), subchild_coord.x(), gpio_port.get_name(),
              io_counter[gpio_port.get_name()]);
            io_counter[gpio_port.get_name()]++;
          }
        }
      }
    }
  }

  /* Check all the GPIO ports in the top-level module has been mapped */
  for (const ModuleManager::e_module_port_type& module_io_port_type :
       MODULE_IO_PORT_TYPES) {
    for (const ModulePortId& gpio_port_id :
         module_manager.module_port_ids_by_type(top_module,
                                                module_io_port_type)) {
      /* Only care mappable I/O */
      if (false ==
          module_manager.port_is_mappable_io(top_module, gpio_port_id)) {
        continue;
      }

      const BasicPort& gpio_port =
        module_manager.module_port(top_module, gpio_port_id);
      VTR_ASSERT(io_counter[gpio_port.get_name()] == gpio_port.get_width());
    }
  }

  return io_location_map;
}

/********************************************************************
 * Top-level function, if tile modules are built under the top-level module
 * The data to access for I/O location is different than the fine-grained grid
 *modules
 * FIXME: Think about a unified way for the two kinds of fabrics!!!
 *******************************************************************/
IoLocationMap build_fabric_io_location_map(const ModuleManager& module_manager,
                                           const DeviceGrid& grids,
                                           const bool& tiled_fabric) {
  if (tiled_fabric) {
    return build_fabric_tiled_io_location_map(module_manager, grids, 0);
  }
  return build_fabric_fine_grained_io_location_map(module_manager, grids, 0);
}

} /* end namespace openfpga */
