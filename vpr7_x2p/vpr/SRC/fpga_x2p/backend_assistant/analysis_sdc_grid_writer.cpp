/********************************************************************
 * This file includes functions that are used to write SDC commands
 * to disable unused ports of grids, such as Configurable Logic Block
 * (CLBs), heterogeneous blocks, etc.
 *******************************************************************/
#include "vtr_assert.h"

#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"

#include "analysis_sdc_grid_writer.h" 

#include "globals.h"

/********************************************************************
 * Recursively visit all the pb_types in the hierarchy 
 * and disable all the ports
 *
 * Note: it is a must to disable all the ports in all the child pb_types!
 * This can prohibit timing analyzer to consider any FF-to-FF path or 
 * combinatinal path inside an unused grid, when finding critical paths!!!
 *******************************************************************/
static 
void rec_print_analysis_sdc_disable_unused_pb_graph_nodes(std::fstream& fp, 
                                                          const ModuleManager& module_manager,
                                                          const ModuleId& parent_module,
                                                          const std::string& hierarchy_name,
                                                          t_pb_graph_node* physical_pb_graph_node,
                                                          const e_side& border_side) {
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;
    
  /* Disable all the ports of current module (parent_module)!
   * Hierarchy name already includes the instance name of parent_module 
   */
  fp << "set_disable_timing ";
  fp << hierarchy_name; 
  fp << "/*";
  fp << std::endl;

  /* Return if this is the primitive pb_type */
  if (TRUE == is_primitive_pb_type(physical_pb_type)) {
    return;
  }

  /* Go recursively */
  int physical_mode_index = find_pb_type_physical_mode_index(*physical_pb_type);

  /* Disable all the ports by iterating over its instance in the parent module */
  for (int ichild = 0; ichild < physical_pb_type->modes[physical_mode_index].num_pb_type_children; ++ichild) {
    /* Generate the name of the Verilog module for this child */
    std::string child_module_name_prefix = generate_grid_block_prefix(std::string(GRID_MODULE_NAME_PREFIX), border_side);
    std::string child_module_name = generate_physical_block_module_name(child_module_name_prefix, &(physical_pb_type->modes[physical_mode_index].pb_type_children[ichild]));

    ModuleId child_module = module_manager.find_module(child_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(child_module));

    /* Each child may exist multiple times in the hierarchy*/
    for (int inst = 0; inst < physical_pb_type->modes[physical_mode_index].pb_type_children[ichild].num_pb; ++inst) {
      std::string child_instance_name = module_manager.instance_name(parent_module, child_module, module_manager.child_module_instances(parent_module, child_module)[inst]);
      /* Must have a valid instance name!!! */
      VTR_ASSERT(false == child_instance_name.empty()); 

      std::string updated_hierarchy_name = hierarchy_name + std::string("/") + child_instance_name + std::string("/");

      rec_print_analysis_sdc_disable_unused_pb_graph_nodes(fp, module_manager, child_module, hierarchy_name, 
                                                           &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode_index][ichild][inst]), 
                                                           border_side); 
    }
  }
}

/********************************************************************
 * Disable the timing for a fully unused grid!
 * This is very straightforward!
 * Just walk through each pb_type and disable all the ports using wildcards
 *******************************************************************/
static 
void print_analysis_sdc_disable_unused_pb_block(std::fstream& fp,
                                                t_type_ptr grid_type,
                                                const vtr::Point<size_t>& grid_coordinate,
                                                const ModuleManager& module_manager,
                                                const std::string& grid_instance_name,
                                                const size_t& grid_z,
                                                const e_side& border_side) {
  /* Check code: if this is an IO block, the border side MUST be valid */
  if (IO_TYPE == grid_type) {
    VTR_ASSERT(NUM_SIDES != border_side);
  }

  /* Find an unique name to the pb instance in this grid
   * Note: this must be consistent with the instance name we used in build_grid_module()!!!
   */
  /* TODO: validate that the instance name is used in module manager!!! */
  std::string pb_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string pb_module_name = generate_grid_physical_block_module_name(pb_module_name_prefix, grid_type->pb_graph_head->pb_type, border_side);
  std::string pb_instance_name = generate_grid_physical_block_instance_name(pb_module_name_prefix, grid_type->pb_graph_head->pb_type, border_side, grid_z);

  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Print comments */
  fp << "#######################################" << std::endl; 
  fp << "# Disable Timing for unused grid[" << grid_coordinate.x() << "][" << grid_coordinate.y() << "][" << grid_z << "]" << std::endl;
  fp << "#######################################" << std::endl; 

  std::string hierarchy_name = grid_instance_name + std::string("/") + pb_instance_name + std::string("/");

  /* Go recursively through the pb_graph hierarchy, and disable all the ports level by level */
  rec_print_analysis_sdc_disable_unused_pb_graph_nodes(fp, module_manager, pb_module, hierarchy_name, grid_type->pb_graph_head, border_side); 
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

  t_type_ptr grid_type = L_grids[grid_coordinate.x()][grid_coordinate.y()].type;
  /* Bypass conditions for grids : 
   * 1. EMPTY type, which is by nature unused
   * 2. Offset > 0, which has already been processed when offset = 0
   */
  if ( (NULL == grid_type) 
    || (EMPTY_TYPE == grid_type)
    || (0 == L_grids[grid_coordinate.x()][grid_coordinate.y()].offset) ) {
    return;
  }

  /* Find an unique name to the grid instane
   * Note: this must be consistent with the instance name we used in build_top_module()!!!
   */
  /* TODO: validate that the instance name is used in module manager!!! */
  std::string grid_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string grid_module_name = generate_grid_block_module_name(grid_module_name_prefix, std::string(grid_type->name), IO_TYPE == grid_type, border_side);
  std::string grid_instance_name = generate_grid_block_instance_name(grid_module_name_prefix, std::string(grid_type->name), IO_TYPE == grid_type, border_side, grid_coordinate);

  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

  /* Now we need to find the usage of this grid */
  std::vector<bool> grid_usage(grid_type->capacity, false);

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
    VTR_ASSERT( (OPEN < L_blocks[blk_id].z) && (L_blocks[blk_id].z < grid_type->capacity) ); 
    /* Mark the grid_usage */
    grid_usage[L_blocks[blk_id].z] = true;
    /* TODO: 
    verilog_generate_sdc_disable_one_unused_block(fp, &(L_blocks[blk_id]));
     */
  }

  /* For unused grid, disable all the pins in the physical_pb_type */
  for (int iblk = 0; iblk < grid_type->capacity; ++iblk) {
    /* Bypass used blocks */
    if (true == grid_usage[iblk]) {
      continue;
    } 
    print_analysis_sdc_disable_unused_pb_block(fp, grid_type, grid_coordinate, module_manager, grid_instance_name, iblk, border_side);
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
