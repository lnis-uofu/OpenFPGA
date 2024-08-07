/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include "verilog_tile.h"

#include <algorithm>
#include <fstream>
#include <map>

#include "command_exit_codes.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "verilog_constants.h"
#include "verilog_module_writer.h"
#include "verilog_writer_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print the tile module for the FPGA fabric in Verilog format
 *******************************************************************/
static int print_verilog_tile_module_netlist(
  NetlistManager& netlist_manager, const ModuleManager& module_manager,
  const ModuleNameMap& module_name_map, const std::string& verilog_dir,
  const FabricTile& fabric_tile, const FabricTileId& fabric_tile_id,
  const std::string& subckt_dir_name, const FabricVerilogOption& options) {
  /* Create a module as the top-level fabric, and add it to the module manager
   */
  vtr::Point<size_t> tile_coord = fabric_tile.tile_coordinate(fabric_tile_id);
  std::string tile_module_name =
    module_name_map.name(generate_tile_module_name(tile_coord));
  ModuleId tile_module = module_manager.find_module(tile_module_name);
  if (!module_manager.valid_module_id(tile_module)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Start printing out Verilog netlists */
  /* Create the file name for Verilog netlist */
  std::string verilog_fname(generate_tile_module_netlist_name(
    tile_module_name, std::string(VERILOG_NETLIST_FILE_POSTFIX)));
  std::string verilog_fpath(verilog_dir + verilog_fname);

  VTR_LOG("Writing Verilog netlist '%s' for tile module '%s'...",
          verilog_fpath.c_str(), tile_module_name.c_str());

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fpath, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fpath.c_str(), fp);

  print_verilog_file_header(fp, std::string("Tile Verilog module for FPGA"),
                            options.time_stamp());

  /* Write the module content in Verilog format */
  write_verilog_module_to_file(fp, module_manager, tile_module, options);

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = NetlistId::INVALID();
  if (options.use_relative_path()) {
    nlist_id = netlist_manager.add_netlist(subckt_dir_name + verilog_fname);
  } else {
    nlist_id = netlist_manager.add_netlist(verilog_fpath);
  }

  VTR_ASSERT(nlist_id);
  netlist_manager.set_netlist_type(nlist_id,
                                   NetlistManager::TILE_MODULE_NETLIST);

  VTR_LOG("Done\n");

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Print all the tile modules for the FPGA fabric in Verilog format
 *******************************************************************/
int print_verilog_tiles(NetlistManager& netlist_manager,
                        const ModuleManager& module_manager,
                        const ModuleNameMap& module_name_map,
                        const std::string& verilog_dir,
                        const FabricTile& fabric_tile,
                        const std::string& subckt_dir_name,
                        const FabricVerilogOption& options) {
  vtr::ScopedStartFinishTimer timer("Build tile modules for the FPGA fabric");

  int status_code = CMD_EXEC_SUCCESS;

  /* Build a module for each unique tile  */
  for (FabricTileId fabric_tile_id : fabric_tile.unique_tiles()) {
    status_code = print_verilog_tile_module_netlist(
      netlist_manager, module_manager, module_name_map, verilog_dir,
      fabric_tile, fabric_tile_id, subckt_dir_name, options);
    if (status_code != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  return status_code;
}

} /* end namespace openfpga */
