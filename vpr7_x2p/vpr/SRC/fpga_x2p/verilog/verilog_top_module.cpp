/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <fstream>
#include <map>

#include "vtr_assert.h"

#include "vpr_types.h"
#include "globals.h"

#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "module_manager_utils.h"

#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_top_module.h"


/********************************************************************
 * Add a instance of a grid module to the top module
 *******************************************************************/
static 
void add_top_module_grid_instance(ModuleManager& module_manager,
                                  const ModuleId& top_module,
                                  t_type_ptr grid_type,
                                  const e_side& border_side) {
  /* Find the module name for this type of grid */
  std::string grid_module_name_prefix(grid_verilog_file_name_prefix);
  /* Add side string to the name if it is valid */
  if (NUM_SIDES != border_side) {
    Side side_manager(border_side);
    grid_module_name_prefix += std::string(side_manager.to_string());
    grid_module_name_prefix += std::string("_");
  }
  std::string grid_module_name = generate_physical_block_module_name(grid_module_name_prefix, grid_type->pb_graph_head->pb_type);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));
  /* Add the module to top_module */ 
  module_manager.add_child_module(top_module, grid_module);
}

/********************************************************************
 * Add all the grids as sub-modules across the fabric
 * The grid modules are created for each unique type of grid (based 
 * on the type in data structure data_structure
 * Here, we will iterate over the full fabric (coordinates)
 * and instanciate the grid modules 
 *
 * This function assumes an island-style floorplanning for FPGA fabric 
 *
 *                +-----------------------------------+
 *                |              I/O grids            |
 *                |              TOP side             |
 *                +-----------------------------------+
 *
 * +-----------+  +-----------------------------------+ +------------+
 * |           |  |                                   | |            |
 * | I/O grids |  |          Core grids               | | I/O grids  |
 * | LEFT side |  | (CLB, Heterogeneous blocks, etc.) | | RIGHT side |
 * |           |  |                                   | |            |
 * +-----------+  +-----------------------------------+ +------------+
 *
 *                +-----------------------------------+
 *                |              I/O grids            |
 *                |             BOTTOM side           |
 *                +-----------------------------------+
 *
 *******************************************************************/
static 
void add_top_module_grid_instances(ModuleManager& module_manager,
                                   const ModuleId& top_module,
                                   const vtr::Point<size_t>& device_size,
                                   const std::vector<std::vector<t_grid_tile>>& grids) {
  /* Instanciate core grids */
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grids[ix][iy].type) {
        continue;
      } 
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[ix][iy].offset) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE != grids[ix][iy].type);
      /* Add a grid module to top_module*/
      add_top_module_grid_instance(module_manager, top_module,
                                   grids[ix][iy].type,
                                   NUM_SIDES);
    }
  }

  /* Instanciate I/O grids */
  /* Create the coordinate range for each side of FPGA fabric */
  std::vector<e_side> io_sides{TOP, RIGHT, BOTTOM, LEFT};
  std::map<e_side, std::vector<vtr::Point<size_t>>> io_coordinates;

  /* TOP side*/
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) { 
    io_coordinates[TOP].push_back(vtr::Point<size_t>(ix, device_size.y() - 1));
  } 

  /* RIGHT side */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) { 
    io_coordinates[RIGHT].push_back(vtr::Point<size_t>(device_size.x() - 1, iy));
  } 

  /* BOTTOM side*/
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) { 
    io_coordinates[BOTTOM].push_back(vtr::Point<size_t>(ix, 0));
  } 

  /* LEFT side */
  for (size_t iy = 1; iy < device_size.y() - 1; ++iy) { 
    io_coordinates[LEFT].push_back(vtr::Point<size_t>(0, iy));
  }

  /* Add instances of I/O grids to top_module */
  for (const e_side& io_side : io_sides) {
    for (const vtr::Point<size_t>& io_coordinate : io_coordinates[io_side]) {
      /* Bypass EMPTY grid */
      if (EMPTY_TYPE == grids[io_coordinate.x()][io_coordinate.y()].type) {
        continue;
      } 
      /* Skip height > 1 tiles (mostly heterogeneous blocks) */
      if (0 < grids[io_coordinate.x()][io_coordinate.y()].offset) {
        continue;
      }
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE == grids[io_coordinate.x()][io_coordinate.y()].type);
      /* Add a grid module to top_module*/
      add_top_module_grid_instance(module_manager, top_module,
                                   grids[io_coordinate.x()][io_coordinate.y()].type,
                                   io_side);
    }
  }
}

/********************************************************************
 * Print the top-level module for the FPGA fabric in Verilog format
 * This function will 
 * 1. name the top-level module
 * 2. include dependent netlists 
 *    - User defined netlists
 *    - Auto-generated netlists
 * 3. Add the submodules to the top-level graph
 * 4. Add module nets to connect datapath ports
 * 5. Add module nets/submodules to connect configuration ports
 *******************************************************************/
void print_verilog_top_module(ModuleManager& module_manager,
                              const CircuitLibrary& circuit_lib,
                              const vtr::Point<size_t>& device_size,
                              const std::vector<std::vector<t_grid_tile>>& grids,
                              t_sram_orgz_info* cur_sram_orgz_info,
                              const std::string& arch_name,
                              const std::string& verilog_dir,
                              const bool& use_explicit_mapping) {
  /* Create a module as the top-level fabric, and add it to the module manager */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.add_module(top_module_name);
 
  /* TODO: Add sub modules, which are grid, SB and CBX/CBY modules as instances */
  /* Add all the grids across the fabric */
  add_top_module_grid_instances(module_manager, top_module, device_size, grids);
  /* Add all the SBs across the fabric */
  /* Add all the CBX and CBYs across the fabric */

  /* TODO: Add module nets to connect the sub modules */
  /* TODO: Add inter-CLB direct connections */

  /* TODO: Add global ports to the top-level module */

  /* TODO: Add module nets to connect the top-level ports to sub modules */

  /* TODO: Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, top_module);

  /* TODO: Count GPIO ports from the sub-modules under this Verilog module 
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  add_module_gpio_ports_from_child_modules(module_manager, top_module);

  /* TODO: Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_shared_config_bits = find_module_num_shared_config_bits_from_child_modules(module_manager, top_module); 
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, top_module, module_num_shared_config_bits);
  }

  /* TODO: this should be added to the cur_sram_orgz_info !!! */
  t_spice_model* mem_model = NULL;
  get_sram_orgz_info_mem_model(cur_sram_orgz_info, & mem_model);
  CircuitModelId sram_model = circuit_lib.model(mem_model->name);  
  VTR_ASSERT(CircuitModelId::INVALID() != sram_model);

  /* TODO: Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_config_bits = find_module_num_config_bits_from_child_modules(module_manager, top_module, circuit_lib, sram_model, cur_sram_orgz_info->type); 
  if (0 < module_num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, top_module, circuit_lib, sram_model, cur_sram_orgz_info->type, module_num_config_bits);
  }

  /* Vectors to record all the memory modules have been added
   * They are used to add module nets of configuration bus
   */
  std::vector<ModuleId> memory_modules;
  std::vector<size_t> memory_instances;

  /* TODO: Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb module!
   */
  if (false == memory_modules.empty()) {
    add_module_nets_memory_config_bus(module_manager, top_module, 
                                      memory_modules, memory_instances,
                                      cur_sram_orgz_info->type, circuit_lib.design_tech_type(sram_model));
  }

  /* Start printing out Verilog netlists */
  /* Create the file name for Verilog netlist */
  std::string verilog_fname(verilog_dir + generate_fpga_top_netlist_name(std::string(verilog_netlist_file_postfix)));
  /* TODO: remove the bak file when the file is ready */
  verilog_fname += ".bak";

  vpr_printf(TIO_MESSAGE_INFO,
             "Writing Verilog Netlist for top-level module of FPGA fabric (%s)...\n",
             verilog_fname.c_str());

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  print_verilog_file_header(fp, std::string("Top-level Verilog module for FPGA architecture: " + std::string(arch_name))); 

  /* Print preprocessing flags */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Write the module content in Verilog format */
  write_verilog_module_to_file(fp, module_manager, top_module, use_explicit_mapping);

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();
}
