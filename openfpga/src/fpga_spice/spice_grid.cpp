/********************************************************************
 * This file includes functions to print SPICE subckts for a Grid
 * (CLBs, I/Os, heterogeneous blocks etc.)
 *******************************************************************/
/* System header files */
#include <fstream>
#include <vector>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"

/* Headers from readarch library */
#include "physical_types.h"
#include "physical_types_util.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_side_manager.h"

/* Headers from vpr library */
#include "circuit_library_utils.h"
#include "module_manager_utils.h"
#include "openfpga_naming.h"
#include "openfpga_physical_tile_utils.h"
#include "openfpga_reserved_words.h"
#include "pb_type_utils.h"
#include "spice_constants.h"
#include "spice_grid.h"
#include "spice_subckt_writer.h"
#include "spice_writer_utils.h"
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print SPICE subckts of a primitive node in the pb_graph_node graph
 * This generic function can support all the different types of primitive nodes
 * i.e., Look-Up Tables (LUTs), Flip-flops (FFs) and hard logic blocks such as
 *adders.
 *
 * The SPICE subckt will consist of two parts:
 * 1. Logic module of the primitive node
 *    This module performs the logic function of the block
 * 2. Memory module of the primitive node
 *    This module stores the configuration bits for the logic module
 *    if the logic module is a programmable resource, such as LUT
 *
 * SPICE subckt structure:
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
static void print_spice_primitive_block(
  NetlistManager& netlist_manager, const ModuleManager& module_manager,
  const std::string& subckt_dir, t_pb_graph_node* primitive_pb_graph_node,
  const bool& verbose) {
  /* Ensure a valid pb_graph_node */
  if (nullptr == primitive_pb_graph_node) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid primitive_pb_graph_node!\n");
    exit(1);
  }

  /* Give a name to the Verilog netlist */
  /* Create the file name for Verilog */
  std::string spice_fname(subckt_dir +
                          generate_logical_tile_netlist_name(
                            std::string(), primitive_pb_graph_node,
                            std::string(SPICE_NETLIST_FILE_POSTFIX)));

  VTR_LOG("Writing SPICE netlist '%s' for primitive pb_type '%s' ...",
          spice_fname.c_str(), primitive_pb_graph_node->pb_type->name);
  VTR_LOGV(verbose, "\n");

  /* Create the file stream */
  std::fstream fp;
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(spice_fname.c_str(), fp);

  print_spice_file_header(
    fp, std::string("SPICE subckts for primitive pb_type: " +
                    std::string(primitive_pb_graph_node->pb_type->name)));

  /* Generate the module name for this primitive pb_graph_node*/
  std::string primitive_module_name =
    generate_physical_block_module_name(primitive_pb_graph_node->pb_type);

  /* Create a module of the primitive LUT and register it to module manager */
  ModuleId primitive_module = module_manager.find_module(primitive_module_name);
  /* Ensure that the module has been created and thus unique! */
  VTR_ASSERT(true == module_manager.valid_module_id(primitive_module));

  VTR_LOGV(verbose,
           "Writing SPICE codes of logical tile primitive block '%s'...",
           module_manager.module_name(primitive_module).c_str());

  /* Write the spice module */
  write_spice_subckt_to_file(fp, module_manager, primitive_module);

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id,
                                   NetlistManager::LOGIC_BLOCK_NETLIST);

  VTR_LOGV(verbose, "Done\n");
}

/********************************************************************
 * Print SPICE subckts of physical blocks inside a grid (CLB, I/O. etc.)
 * This function will traverse the graph of complex logic block
 *(t_pb_graph_node) in a recursive way, using a Depth First Search (DFS)
 *algorithm. As such, primitive physical blocks (LUTs, FFs, etc.), leaf node of
 *the pb_graph will be printed out first, while the top-level will be printed
 *out in the last
 *
 * Note: this function will print a unique SPICE subckt for each type of
 * t_pb_graph_node, i.e., t_pb_type, in the graph, in order to enable highly
 * hierarchical Verilog organization as well as simplify the Verilog file sizes.
 *
 * Note: DFS is the right way. Do NOT use BFS.
 * DFS can guarantee that all the sub-modules can be registered properly
 * to its parent in module manager
 *******************************************************************/
static void rec_print_spice_logical_tile(
  NetlistManager& netlist_manager, const ModuleManager& module_manager,
  const VprDeviceAnnotation& device_annotation, const std::string& subckt_dir,
  t_pb_graph_node* physical_pb_graph_node, const bool& verbose) {
  /* Check cur_pb_graph_node*/
  if (nullptr == physical_pb_graph_node) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid physical_pb_graph_node\n");
    exit(1);
  }

  /* Get the pb_type definition related to the node */
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  /* Find the mode that physical implementation of a pb_type */
  t_mode* physical_mode = device_annotation.physical_mode(physical_pb_type);

  /* For non-leaf node in the pb_type graph:
   * Recursively Depth-First Generate all the child pb_type at the level
   */
  if (false == is_primitive_pb_type(physical_pb_type)) {
    for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
      /* Go recursive to visit the children */
      rec_print_spice_logical_tile(
        netlist_manager, module_manager, device_annotation, subckt_dir,
        &(physical_pb_graph_node
            ->child_pb_graph_nodes[physical_mode->index][ipb][0]),
        verbose);
    }
  }

  /* For leaf node, a primitive SPICE subckt will be generated.
   * Note that the primitive may be mapped to a standard cell, we force to use
   * explict port mapping. This aims to avoid any port sequence issues!!!
   */
  if (true == is_primitive_pb_type(physical_pb_type)) {
    print_spice_primitive_block(netlist_manager, module_manager, subckt_dir,
                                physical_pb_graph_node, verbose);
    /* Finish for primitive node, return */
    return;
  }

  /* Give a name to the Verilog netlist */
  /* Create the file name for Verilog */
  std::string spice_fname(subckt_dir +
                          generate_logical_tile_netlist_name(
                            std::string(), physical_pb_graph_node,
                            std::string(SPICE_NETLIST_FILE_POSTFIX)));

  VTR_LOG("Writing SPICE netlist '%s' for pb_type '%s' ...",
          spice_fname.c_str(), physical_pb_type->name);
  VTR_LOGV(verbose, "\n");

  /* Create the file stream */
  std::fstream fp;
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(spice_fname.c_str(), fp);

  print_spice_file_header(fp, std::string("SPICE subckts for pb_type: " +
                                          std::string(physical_pb_type->name)));

  /* Generate the name of the SPICE subckt for this pb_type */
  std::string pb_module_name =
    generate_physical_block_module_name(physical_pb_type);

  /* Register the SPICE subckt in module manager */
  ModuleId pb_module = module_manager.find_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  VTR_LOGV(verbose, "Writing SPICE codes of pb_type '%s'...",
           module_manager.module_name(pb_module).c_str());

  /* Comment lines */
  print_spice_comment(
    fp, std::string("BEGIN Physical programmable logic block SPICE subckt: " +
                    std::string(physical_pb_type->name)));

  /* Write the spice module */
  write_spice_subckt_to_file(fp, module_manager, pb_module);

  print_spice_comment(
    fp, std::string("END Physical programmable logic block SPICE subckt: " +
                    std::string(physical_pb_type->name)));

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id,
                                   NetlistManager::LOGIC_BLOCK_NETLIST);

  VTR_LOGV(verbose, "Done\n");
}

/*****************************************************************************
 * This function will create a Verilog file and print out a Verilog netlist
 * for the logical tile (pb_graph/pb_type)
 *****************************************************************************/
static void print_spice_logical_tile_netlist(
  NetlistManager& netlist_manager, const ModuleManager& module_manager,
  const VprDeviceAnnotation& device_annotation, const std::string& subckt_dir,
  t_pb_graph_node* pb_graph_head, const bool& verbose) {
  VTR_LOG("Writing Verilog netlists for logic tile '%s' ...",
          pb_graph_head->pb_type->name);
  VTR_LOG("\n");

  /* Print SPICE subckts for all the pb_types/pb_graph_nodes
   * use a Depth-First Search Algorithm to print the sub-modules
   * Note: DFS is the right way. Do NOT use BFS.
   * DFS can guarantee that all the sub-modules can be registered properly
   * to its parent in module manager
   */
  /* Print SPICE subckts starting from the top-level pb_type/pb_graph_node, and
   * traverse the graph in a recursive way */
  rec_print_spice_logical_tile(netlist_manager, module_manager,
                               device_annotation, subckt_dir, pb_graph_head,
                               verbose);

  VTR_LOG("Done\n");
  VTR_LOG("\n");
}

/*****************************************************************************
 * This function will create a Verilog file and print out a Verilog netlist
 * for a type of physical block
 *
 * For IO blocks:
 * The param 'border_side' is required, which is specify which side of fabric
 * the I/O block locates at.
 *****************************************************************************/
static void print_spice_physical_tile_netlist(
  NetlistManager& netlist_manager, const ModuleManager& module_manager,
  const std::string& subckt_dir, t_physical_tile_type_ptr phy_block_type,
  const e_side& border_side) {
  /* Check code: if this is an IO block, the border side MUST be valid */
  if (phy_block_type->is_io()) {
    VTR_ASSERT(NUM_2D_SIDES != border_side);
  }

  /* Give a name to the Verilog netlist */
  /* Create the file name for Verilog */
  std::string spice_fname(
    subckt_dir +
    generate_grid_block_netlist_name(
      std::string(GRID_MODULE_NAME_PREFIX) + std::string(phy_block_type->name),
      phy_block_type->is_io(), border_side,
      std::string(SPICE_NETLIST_FILE_POSTFIX)));

  /* Echo status */
  if (phy_block_type->is_io()) {
    SideManager side_manager(border_side);
    VTR_LOG("Writing SPICE Netlist '%s' for physical tile '%s' at %s side ...",
            spice_fname.c_str(), phy_block_type->name.c_str(),
            side_manager.c_str());
  } else {
    VTR_LOG("Writing SPICE Netlist '%s' for physical_tile '%s'...",
            spice_fname.c_str(), phy_block_type->name.c_str());
  }

  /* Create the file stream */
  std::fstream fp;
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(spice_fname.c_str(), fp);

  print_spice_file_header(fp,
                          std::string("SPICE subckts for physical tile: " +
                                      std::string(phy_block_type->name) + "]"));

  /* Create a Verilog Module for the top-level physical block, and add to module
   * manager */
  std::string grid_module_name = generate_grid_block_module_name(
    std::string(GRID_SPICE_FILE_NAME_PREFIX), std::string(phy_block_type->name),
    phy_block_type->is_io(), border_side);
  ModuleId grid_module = module_manager.find_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

  /* Write the spice module */
  print_spice_comment(fp, std::string("BEGIN Grid SPICE subckt: " +
                                      module_manager.module_name(grid_module)));
  write_spice_subckt_to_file(fp, module_manager, grid_module);

  print_spice_comment(fp, std::string("END Grid SPICE subckt: " +
                                      module_manager.module_name(grid_module)));

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id,
                                   NetlistManager::LOGIC_BLOCK_NETLIST);

  VTR_LOG("Done\n");
}

/*****************************************************************************
 * Create logic block modules in a compact way:
 * 1. Only one module for each I/O on each border side (IO_TYPE)
 * 2. Only one module for each CLB (FILL_TYPE)
 * 3. Only one module for each heterogeneous block
 ****************************************************************************/
void print_spice_grids(NetlistManager& netlist_manager,
                       const ModuleManager& module_manager,
                       const DeviceContext& device_ctx,
                       const VprDeviceAnnotation& device_annotation,
                       const std::string& subckt_dir, const bool& verbose) {
  /* Create a vector to contain all the Verilog netlist names that have been
   * generated in this function */
  std::vector<std::string> netlist_names;

  /* Enumerate the types of logical tiles, and build a module for each
   * Write modules for all the pb_types/pb_graph_nodes
   * use a Depth-First Search Algorithm to print the sub-modules
   * Note: DFS is the right way. Do NOT use BFS.
   * DFS can guarantee that all the sub-modules can be registered properly
   * to its parent in module manager
   */
  VTR_LOG("Writing logical tiles...");
  VTR_LOGV(verbose, "\n");
  for (const t_logical_block_type& logical_tile :
       device_ctx.logical_block_types) {
    /* Bypass empty pb_graph */
    if (nullptr == logical_tile.pb_graph_head) {
      continue;
    }
    print_spice_logical_tile_netlist(netlist_manager, module_manager,
                                     device_annotation, subckt_dir,
                                     logical_tile.pb_graph_head, verbose);
  }
  VTR_LOG("Writing logical tiles...");
  VTR_LOG("Done\n");

  VTR_LOG("\n");

  /* Enumerate the types of physical tiles
   * Use the logical tile module to build the physical tiles
   */
  VTR_LOG("Building physical tiles...");
  VTR_LOGV(verbose, "\n");
  for (const t_physical_tile_type& physical_tile :
       device_ctx.physical_tile_types) {
    /* Bypass empty type or nullptr */
    if (physical_tile.is_empty()) {
      continue;
    } else if (physical_tile.is_io()) {
      /* Special for I/O block:
       * We will search the grids and see where the I/O blocks are located:
       * - If a I/O block locates on border sides of FPGA fabric:
       *   i.e., one or more from {TOP, RIGHT, BOTTOM, LEFT},
       *   we will generate one module for each border side
       * - If a I/O block locates in the center of FPGA fabric:
       *   we will generate one module with NUM_2D_SIDES (same treatment as
       * regular grids)
       */
      std::set<e_side> io_type_sides =
        find_physical_io_tile_located_sides(device_ctx.grid, &physical_tile);
      for (const e_side& io_type_side : io_type_sides) {
        print_spice_physical_tile_netlist(netlist_manager, module_manager,
                                          subckt_dir, &physical_tile,
                                          io_type_side);
      }
      continue;
    } else {
      /* For CLB and heterogenenous blocks */
      print_spice_physical_tile_netlist(netlist_manager, module_manager,
                                        subckt_dir, &physical_tile,
                                        NUM_2D_SIDES);
    }
  }
  VTR_LOG("Building physical tiles...");
  VTR_LOG("Done\n");
  VTR_LOG("\n");

  /* Output a header file for all the logic blocks */
  /*
  std::string grid_spice_fname(LOGIC_BLOCK_VERILOG_FILE_NAME);
  VTR_LOG("Writing header file for grid SPICE subckts '%s' ...",
          grid_spice_fname.c_str());
  print_spice_netlist_include_header_file(netlist_names,
                                          subckt_dir.c_str(),
                                          grid_spice_fname.c_str());
  VTR_LOG("Done\n");
   */
}

} /* end namespace openfpga */
