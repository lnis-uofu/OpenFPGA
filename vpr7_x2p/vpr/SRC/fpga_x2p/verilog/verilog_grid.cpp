/********************************************************************
 * This file includes functions to print Verilog modules for a Grid
 * (CLBs, I/Os, heterogeneous blocks etc.) 
 *******************************************************************/
/* System header files */
#include <vector>
#include <fstream>

/* Header files from external libs */
#include "util.h"
#include "vtr_assert.h"

/* Header files for VPR */
#include "vpr_types.h"
#include "globals.h"

/* Header files for FPGA X2P tool suite */
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* Header files for Verilog generator */
#include "verilog_global.h"
#include "verilog_utils.h"
#include "verilog_writer_utils.h"
#include "verilog_grid.h"

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
                        const MuxLibrary& mux_lib,
                        const CircuitLibrary& circuit_lib,
                        t_sram_orgz_info* cur_sram_orgz_info, 
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
                          + generate_physical_block_netlist_name(std::string(phy_block_type->name), 
                                                                 IO_TYPE == phy_block_type, 
                                                                 border_side, 
                                                                 std::string(verilog_netlist_file_postfix))
                           );
  /* TODO: remove the bak file when the file is ready */
  verilog_fname += ".bak";

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

  /* TODO: Print Verilog modules for all the pb_types/pb_graph_nodes */
  for (int iz = 0; iz < phy_block_type->capacity; ++iz) {
    /* ONLY output one Verilog module (which is unique), others are the same */
    if (0 < iz) {
      continue;
    }
    /* TODO: use a Depth-First Search Algorithm to print the sub-modules 
     * Note: DFS is the right way. Do NOT use BFS.
     * DFS can guarantee that all the sub-modules can be registered properly
     * to its parent in module manager  
     */
    print_verilog_comment(fp, std::string("---- BEGIN Sub-module of physical block:" + std::string(phy_block_type->name) + " ----"));

    /* Print Verilog modules starting from the top-level pb_type/pb_graph_node, and traverse the graph in a recursive way */
    /*
    dump_verilog_phy_pb_graph_node_rec(cur_sram_orgz_info, fp, subckt_name_prefix, 
                                       phy_block_type->pb_graph_head, iz,
                                       is_explicit_mapping);
     */
    print_verilog_comment(fp, std::string("---- END Sub-module of physical block:" + std::string(phy_block_type->name) + " ----"));
  }

  /* TODO: Create a Verilog Module for the top-level physical block, and add to module manager */
  std::string module_name = generate_physical_block_module_name(std::string(grid_verilog_file_name_prefix), phy_block_type->name, IO_TYPE == phy_block_type, border_side);
  ModuleId module_id = module_manager.add_module(module_name); 

  /* TODO: Add ports to the module */


  /* TODO: Print the module definition for the top-level Verilog module of physical block */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish printing ports */

  /* Print an empty line a splitter */
  fp << std::endl;

  /* TODO: instanciate all the sub modules */
  for (int iz = 0; iz < phy_block_type->capacity; ++iz) {
  }

  /* Put an end to the top-level Verilog module of physical block */
  print_verilog_module_end(fp, module_manager.module_name(module_id));

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the linked list */
  /*
  grid_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(grid_verilog_subckt_file_path_head, verilog_fname.c_str());  
   */
}

/*****************************************************************************
 * Create logic block modules in a compact way:
 * 1. Only one module for each I/O on each border side (IO_TYPE)
 * 2. Only one module for each CLB (FILL_TYPE)
 * 3. Only one module for each heterogeneous block
 ****************************************************************************/
void print_verilog_grids(ModuleManager& module_manager,
                         const CircuitLibrary& circuit_lib,
                         const MuxLibrary& mux_lib,
                         t_sram_orgz_info* cur_sram_orgz_info,
                         const std::string& verilog_dir,
                         const std::string& subckt_dir,
                         const bool& is_explicit_mapping) {
  /* Enumerate the types, dump one Verilog module for each */
  for (int itype = 0; itype < num_types; itype++) {
    if (EMPTY_TYPE == &type_descriptors[itype]) {
    /* Bypass empty type or NULL */
      continue;
    } else if (IO_TYPE == &type_descriptors[itype]) {
      /* Special for I/O block, generate one module for each border side */
      for (int iside = 0; iside < NUM_SIDES; iside++) {
        Side side_manager(iside);
        print_verilog_grid(module_manager, mux_lib, circuit_lib,
                           cur_sram_orgz_info, 
                           verilog_dir, subckt_dir, 
                           &type_descriptors[itype],
                           side_manager.get_side(),
                           is_explicit_mapping);
      } 
      continue;
    } else if (FILL_TYPE == &type_descriptors[itype]) {
      /* For CLB */
      print_verilog_grid(module_manager, mux_lib, circuit_lib,
                         cur_sram_orgz_info, 
                         verilog_dir, subckt_dir, 
                         &type_descriptors[itype],
                         NUM_SIDES,
                         is_explicit_mapping);
      continue;
    } else {
      /* For heterogenenous blocks */
      print_verilog_grid(module_manager, mux_lib, circuit_lib,
                         cur_sram_orgz_info, 
                         verilog_dir, subckt_dir, 
                         &type_descriptors[itype],
                         NUM_SIDES,
                         is_explicit_mapping);
    }
  }

  /* Output a header file for all the logic blocks */
  vpr_printf(TIO_MESSAGE_INFO, "Generating header file for grid Verilog modules...\n");
  std::string grid_verilog_fname(logic_block_verilog_file_name);
  /* TODO: remove .bak when it is ready */
  grid_verilog_fname += ".bak";
  dump_verilog_subckt_header_file(grid_verilog_subckt_file_path_head,
                                  subckt_dir.c_str(),
                                  grid_verilog_fname.c_str());
}

