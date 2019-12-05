/********************************************************************
 * This file includes functions to print Verilog modules for a Grid
 * (CLBs, I/Os, heterogeneous blocks etc.) 
 *******************************************************************/
/* System header files */
#include <vector>
#include <fstream>

/* Header files from external libs */
#include "vtr_geometry.h"
#include "util.h"
#include "vtr_assert.h"
#include "circuit_library_utils.h"

/* Header files for VPR */
#include "vpr_types.h"
#include "globals.h"

/* Header files for FPGA X2P tool suite */
#include "fpga_x2p_naming.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_pbtypes_utils.h"
#include "module_manager_utils.h"
#include "fpga_x2p_globals.h"

/* Header files for Verilog generator */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_grid.h"

/********************************************************************
 * Print Verilog modules of a primitive node in the pb_graph_node graph
 * This generic function can support all the different types of primitive nodes
 * i.e., Look-Up Tables (LUTs), Flip-flops (FFs) and hard logic blocks such as adders.
 *
 * The Verilog module will consist of two parts:
 * 1. Logic module of the primitive node
 *    This module performs the logic function of the block
 * 2. Memory module of the primitive node
 *    This module stores the configuration bits for the logic module
 *    if the logic module is a programmable resource, such as LUT
 *
 * Verilog module structure:
 *
 *       Primitive block
 *     +---------------------------------------+
 *     |                                       | 
 *     |      +---------+    +---------+       |
 *  in |----->|         |--->|         |<------|configuration lines
 *     |      |  Logic  |... |  Memory |       |
 *  out|<-----|         |--->|         |       |
 *     |      +---------+    +---------+       |
 *     |                                       | 
 *     +---------------------------------------+
 *
 *******************************************************************/
static 
void print_verilog_primitive_block(std::fstream& fp,
                                   ModuleManager& module_manager,
                                   t_pb_graph_node* primitive_pb_graph_node,
                                   const e_side& io_side,
                                   const bool& use_explicit_mapping) {
  /* Ensure a valid file handler */ 
  check_file_handler(fp);

  /* Ensure a valid pb_graph_node */ 
  if (NULL == primitive_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d]) Invalid primitive_pb_graph_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Generate the module name for this primitive pb_graph_node*/
  std::string primitive_module_name_prefix = generate_grid_block_prefix(std::string(grid_verilog_file_name_prefix), io_side);
  std::string primitive_module_name = generate_physical_block_module_name(primitive_module_name_prefix, primitive_pb_graph_node->pb_type);

  /* Create a module of the primitive LUT and register it to module manager */
  ModuleId primitive_module = module_manager.find_module(primitive_module_name);
  /* Ensure that the module has been created and thus unique! */
  VTR_ASSERT(true == module_manager.valid_module_id(primitive_module));
  
  /* Write the verilog module */
  write_verilog_module_to_file(fp, module_manager, primitive_module, use_explicit_mapping);

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Print Verilog modules of physical blocks inside a grid (CLB, I/O. etc.)
 * This function will traverse the graph of complex logic block (t_pb_graph_node)
 * in a recursive way, using a Depth First Search (DFS) algorithm.
 * As such, primitive physical blocks (LUTs, FFs, etc.), leaf node of the pb_graph
 * will be printed out first, while the top-level will be printed out in the last
 *
 * Note: this function will print a unique Verilog module for each type of 
 * t_pb_graph_node, i.e., t_pb_type, in the graph, in order to enable highly 
 * hierarchical Verilog organization as well as simplify the Verilog file sizes.
 *
 * Note: DFS is the right way. Do NOT use BFS.
 * DFS can guarantee that all the sub-modules can be registered properly
 * to its parent in module manager  
 *******************************************************************/
static 
void print_verilog_physical_blocks_rec(std::fstream& fp,
                                       ModuleManager& module_manager,
                                       t_pb_graph_node* physical_pb_graph_node,
                                       const e_side& io_side,
                                       const bool& use_explicit_mapping) {
  /* Check the file handler*/ 
  check_file_handler(fp);

  /* Check cur_pb_graph_node*/
  if (NULL == physical_pb_graph_node) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d]) Invalid cur_pb_graph_node.\n", 
               __FILE__, __LINE__); 
    exit(1);
  }

  /* Get the pb_type definition related to the node */
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type; 

  /* Find the mode that physical implementation of a pb_type */
  int physical_mode_index = find_pb_type_physical_mode_index((*physical_pb_type));

  /* For non-leaf node in the pb_type graph: 
   * Recursively Depth-First Generate all the child pb_type at the level 
   */
  if (FALSE == is_primitive_pb_type(physical_pb_type)) { 
    for (int ipb = 0; ipb < physical_pb_type->modes[physical_mode_index].num_pb_type_children; ++ipb) {
      /* Go recursive to visit the children */
      print_verilog_physical_blocks_rec(fp, module_manager, 
                                        &(physical_pb_graph_node->child_pb_graph_nodes[physical_mode_index][ipb][0]),
                                        io_side,
                                        use_explicit_mapping);
    }
  }

  /* For leaf node, a primitive Verilog module will be generated.
   * Note that the primitive may be mapped to a standard cell, we force to use that
   */
  if (TRUE == is_primitive_pb_type(physical_pb_type)) { 
    print_verilog_primitive_block(fp, module_manager,
                                  physical_pb_graph_node, 
                                  io_side, 
                                  true);
    /* Finish for primitive node, return */
    return;
  }

  /* Generate the name of the Verilog module for this pb_type */
  std::string pb_module_name_prefix = generate_grid_block_prefix(std::string(grid_verilog_file_name_prefix), io_side);
  std::string pb_module_name = generate_physical_block_module_name(pb_module_name_prefix, physical_pb_type);

  /* Register the Verilog module in module manager */
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Comment lines */
  print_verilog_comment(fp, std::string("----- BEGIN Physical programmable logic block Verilog module: " + std::string(physical_pb_type->name) + " -----"));

  /* Write the verilog module */
  write_verilog_module_to_file(fp, module_manager, pb_module, use_explicit_mapping);

  print_verilog_comment(fp, std::string("----- END Physical programmable logic block Verilog module: " + std::string(physical_pb_type->name) + " -----"));

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/*****************************************************************************
 * This function will create a Verilog file and print out a Verilog netlist 
 * for a type of physical block 
 *
 * For IO blocks: 
 * The param 'border_side' is required, which is specify which side of fabric
 * the I/O block locates at.
 *****************************************************************************/
static 
void print_verilog_grid(ModuleManager& module_manager,
                        std::vector<std::string>& netlist_names,
                        const std::string& verilog_dir,
                        const std::string& subckt_dir,
                        t_type_ptr phy_block_type,
                        const e_side& border_side,
                        const bool& use_explicit_mapping) {
  /* Check code: if this is an IO block, the border side MUST be valid */
  if (IO_TYPE == phy_block_type) {
    VTR_ASSERT(NUM_SIDES != border_side);
  }
  
  /* Give a name to the Verilog netlist */
  /* Create the file name for Verilog */
  std::string verilog_fname(subckt_dir 
                          + generate_grid_block_netlist_name(std::string(phy_block_type->name), 
                                                             IO_TYPE == phy_block_type, 
                                                             border_side, 
                                                             std::string(verilog_netlist_file_postfix))
                           );
  /* TODO: remove the bak file when the file is ready */
  //verilog_fname += ".bak";

  /* Echo status */
  if (IO_TYPE == phy_block_type) {
    Side side_manager(border_side);
    vpr_printf(TIO_MESSAGE_INFO, 
               "Writing FPGA Verilog Netlist (%s) for logic block %s at %s side ...\n",
               verilog_fname.c_str(), phy_block_type->name, 
               side_manager.c_str());
  } else { 
    vpr_printf(TIO_MESSAGE_INFO, 
               "Writing FPGA Verilog Netlist (%s) for logic block %s...\n",
               verilog_fname.c_str(), phy_block_type->name);
  }

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  print_verilog_file_header(fp, std::string("Verilog modules for physical block: " + std::string(phy_block_type->name) + "]")); 

  /* Print preprocessing flags */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Print Verilog modules for all the pb_types/pb_graph_nodes
   * use a Depth-First Search Algorithm to print the sub-modules 
   * Note: DFS is the right way. Do NOT use BFS.
   * DFS can guarantee that all the sub-modules can be registered properly
   * to its parent in module manager  
   */
  print_verilog_comment(fp, std::string("---- BEGIN Sub-module of physical block:" + std::string(phy_block_type->name) + " ----"));

  /* Print Verilog modules starting from the top-level pb_type/pb_graph_node, and traverse the graph in a recursive way */
  print_verilog_physical_blocks_rec(fp, module_manager, 
                                    phy_block_type->pb_graph_head,
                                    border_side,
                                    use_explicit_mapping);

  print_verilog_comment(fp, std::string("---- END Sub-module of physical block:" + std::string(phy_block_type->name) + " ----"));

  /* Create a Verilog Module for the top-level physical block, and add to module manager */
  std::string grid_module_name = generate_grid_block_module_name(std::string(grid_verilog_file_name_prefix), std::string(phy_block_type->name), IO_TYPE == phy_block_type, border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

  /* Write the verilog module */
  print_verilog_comment(fp, std::string("----- BEGIN Grid Verilog module: " + module_manager.module_name(grid_module) + " -----"));
  write_verilog_module_to_file(fp, module_manager, grid_module, use_explicit_mapping);

  print_verilog_comment(fp, std::string("----- END Grid Verilog module: " + module_manager.module_name(grid_module) + " -----"));

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  netlist_names.push_back(verilog_fname);
}

/*****************************************************************************
 * Create logic block modules in a compact way:
 * 1. Only one module for each I/O on each border side (IO_TYPE)
 * 2. Only one module for each CLB (FILL_TYPE)
 * 3. Only one module for each heterogeneous block
 ****************************************************************************/
void print_verilog_grids(ModuleManager& module_manager,
                         const std::string& verilog_dir,
                         const std::string& subckt_dir,
                         const bool& use_explicit_mapping) {
  /* Create a vector to contain all the Verilog netlist names that have been generated in this function */
  std::vector<std::string> netlist_names;

  /* Enumerate the types, dump one Verilog module for each */
  for (int itype = 0; itype < num_types; itype++) {
    if (EMPTY_TYPE == &type_descriptors[itype]) {
    /* Bypass empty type or NULL */
      continue;
    } else if (IO_TYPE == &type_descriptors[itype]) {
      /* Special for I/O block, generate one module for each border side */
      for (int iside = 0; iside < NUM_SIDES; iside++) {
        Side side_manager(iside);
        print_verilog_grid(module_manager, netlist_names,
                           verilog_dir, subckt_dir, 
                           &type_descriptors[itype],
                           side_manager.get_side(),
                           use_explicit_mapping);
      } 
      continue;
    } else if (FILL_TYPE == &type_descriptors[itype]) {
      /* For CLB */
      print_verilog_grid(module_manager, netlist_names, 
                         verilog_dir, subckt_dir, 
                         &type_descriptors[itype],
                         NUM_SIDES,
                         use_explicit_mapping);
      continue;
    } else {
      /* For heterogenenous blocks */
      print_verilog_grid(module_manager, netlist_names,
                         verilog_dir, subckt_dir, 
                         &type_descriptors[itype],
                         NUM_SIDES,
                         use_explicit_mapping);
    }
  }

  /* Output a header file for all the logic blocks */
  vpr_printf(TIO_MESSAGE_INFO, "Generating header file for grid Verilog modules...\n");
  std::string grid_verilog_fname(logic_block_verilog_file_name);
  print_verilog_netlist_include_header_file(netlist_names,
                                            subckt_dir.c_str(),
                                            grid_verilog_fname.c_str());
}

