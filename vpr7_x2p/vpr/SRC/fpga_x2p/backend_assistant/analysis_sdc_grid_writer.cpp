/********************************************************************
 * This file includes functions that are used to write SDC commands
 * to disable unused ports of grids, such as Configurable Logic Block
 * (CLBs), heterogeneous blocks, etc.
 *******************************************************************/
#include "vtr_assert.h"

#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

#include "analysis_sdc_grid_writer.h" 

#include "globals.h"

/********************************************************************
 * Disable the timing for a fully unused grid!
 * This is very straightforward!
 * Just walk through each pb_type and disable all the ports using wildcards
 *******************************************************************/
static 
void print_analysis_sdc_disable_unused_pb_type(std::fstream& fp,
                                               t_type_ptr grid_type,
                                               const vtr::Point<size_t>& grid_coordinate,
                                               const ModuleManager& module_manager,
                                               const size_t& grid_z,
                                               const e_side& border_side) {
  /* Check code: if this is an IO block, the border side MUST be valid */
  if (IO_TYPE == grid_type) {
    VTR_ASSERT(NUM_SIDES != border_side);
  }

  /* Find an unique name to the grid instane
   * Note: this must be consistent with the instance name we used in build_top_module()!!!
   */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_instance_name = generate_grid_block_instance_name(grid_module_name_prefix, std::string(grid_type->name), IO_TYPE == grid_type, border_side, grid_coordinate);

  /* Find an unique name to the pb instance in this grid
   * Note: this must be consistent with the instance name we used in build_grid_module()!!!
   */
  std::string pb_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string pb_instance_name = generate_grid_physical_block_instance_name(pb_module_name_prefix, grid_type->pb_graph_head->pb_type, border_side, grid_z);

  /* Print comments */
  fp << "#######################################" << std::endl; 
  fp << "# Disable Timing for unused grid[" << grid_coordinate.x() << "][" << grid_coordinate.y() << "][" << grid_z << "]" << std::endl;
  fp << "#######################################" << std::endl; 
          
  /* Disable everything under this level using wildcard */
  fp << "set_disable_timing ";
  fp << grid_instance_name; 
  fp << "/";
  fp << pb_instance_name; 
  fp << "/*";
  fp << std::endl;

  /* TODO: Go recursively through the pb_graph hierarchy, and disable all the ports level by level */
  /*
  rec_verilog_generate_sdc_disable_unused_pb_types(fp, prefix,
                                                   cur_grid_type->pb_type); 
  */
}

/********************************************************************
 * Disable the timing for a fully unused grid!
 * This is very straightforward!
 * Just walk through each pb_type and disable all the ports using wildcards
 *******************************************************************/
static 
void print_analysis_sdc_disable_unused_grid(std::fstream& fp, 
                                            const vtr::Point<size_t>& grid_coordinate,
                                            const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                            const std::vector<t_block>& L_blocks,
                                            const ModuleManager& module_manager,
                                            const e_side& border_side) {
  /* Validate file stream */
  check_file_handler(fp);

  t_type_ptr type = L_grids[grid_coordinate.x()][grid_coordinate.y()].type;
  /* Bypass conditions for grids : 
   * 1. EMPTY type, which is by nature unused
   * 2. Offset > 0, which has already been processed when offset = 0
   */
  if ( (NULL == type) 
    || (EMPTY_TYPE == type)
    || (0 == L_grids[grid_coordinate.x()][grid_coordinate.y()].offset) ) {
    return;
  }

  /* Now we need to find the usage of this grid */
  std::vector<bool> grid_usage(type->capacity, false);

  /* Print comments */
  fp << "#######################################" << std::endl; 
  fp << "# Disable Timing for grid[" << grid_coordinate.x() << "][" << grid_coordinate.y() << "]" << std::endl;
  fp << "#######################################" << std::endl; 

  /* For used grid, find the unused rr_node in the local rr_graph 
   * and then disable each port which is not used
   * as well as the unused inputs of routing multiplexers!
   */
  for (int iblk = 0; iblk < L_grids[grid_coordinate.x()][grid_coordinate.y()].usage; ++iblk) {
    int blk_id = L_grids[grid_coordinate.x()][grid_coordinate.y()].blocks[iblk];
    VTR_ASSERT( (OPEN < L_blocks[blk_id].z) && (L_blocks[blk_id].z < type->capacity) ); 
    /* Mark the grid_usage */
    grid_usage[L_blocks[blk_id].z] = true;
    /* TODO: 
    verilog_generate_sdc_disable_one_unused_block(fp, &(L_blocks[blk_id]));
     */
  }

  /* For unused grid, disable all the pins in the physical_pb_type */
  for (int iblk = 0; iblk < type->capacity; ++iblk) {
    /* Bypass used blocks */
    if (true == grid_usage[iblk]) {
      continue;
    } 
    print_analysis_sdc_disable_unused_pb_type(fp, type, grid_coordinate, module_manager, iblk, border_side);
  }
}

/********************************************************************
 * Top-level function writes SDC commands to disable unused ports 
 * of grids, such as Configurable Logic Block (CLBs), heterogeneous blocks, etc.
 *
 * This function will iterate over all the grids available in the FPGA fabric
 * It will disable the timing analysis for
 * 1. Grids, which are totally not used (no logic has been mapped to)
 * 2. Unused part of grids, including the ports, inputs of routing multiplexers 
 *
 * Note that it is a must to disable the unused inputs of routing multiplexers
 * because it will cause unexpected paths in timing analysis
 * For example:
 *                           +---------------------+
 *     inputA (net0) ------->|                     |
 *                           | Routing multiplexer |----> output (net0)
 *     inputB (net1) ------->|                     |
 *                           +---------------------+
 *
 * During timing analysis, the path from inputA to output should be considered
 * while the path from inputB to output should NOT be considered!!!
 *
 *******************************************************************/
void print_analysis_sdc_disable_unused_grids(std::fstream& fp, 
                                             const vtr::Point<size_t>& device_size,
                                             const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                             const std::vector<t_block>& L_blocks,
                                             const ModuleManager& module_manager) {
  /* TODO: disable inputs of multiplexers
  verilog_generate_sdc_disable_unused_grids_muxs(fp, LL_nx, LL_ny, LL_grid, LL_block);
   */

  /* Process unused core grids */
  for (size_t ix = 1; ix < device_size.x() - 1; ++ix) {
    for (size_t iy = 1; iy < device_size.y() - 1; ++iy) {
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE != L_grids[ix][iy].type);

      print_analysis_sdc_disable_unused_grid(fp, vtr::Point<size_t>(ix, iy),
                                             L_grids, L_blocks, module_manager, NUM_SIDES);
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
      /* We should not meet any I/O grid */
      VTR_ASSERT(IO_TYPE == L_grids[io_coordinate.x()][io_coordinate.y()].type);

      print_analysis_sdc_disable_unused_grid(fp, io_coordinate,
                                             L_grids, L_blocks, module_manager, io_side);
    }
  }
}
