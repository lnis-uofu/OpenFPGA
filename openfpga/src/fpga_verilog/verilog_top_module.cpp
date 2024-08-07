/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in Verilog format
 *******************************************************************/
#include <algorithm>
#include <fstream>
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "verilog_constants.h"
#include "verilog_module_writer.h"
#include "verilog_top_module.h"
#include "verilog_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print the wrapper module for the FPGA fabric in Verilog format
 *******************************************************************/
void print_verilog_core_module(NetlistManager& netlist_manager,
                               const ModuleManager& module_manager,
                               const ModuleNameMap& module_name_map,
                               const std::string& verilog_dir,
                               const FabricVerilogOption& options) {
  /* Create a module as the top-level fabric, and add it to the module manager
   */
  std::string core_module_name = generate_fpga_core_module_name();
  if (module_name_map.name_exist(core_module_name)) {
    core_module_name = module_name_map.name(core_module_name);
  }
  ModuleId core_module = module_manager.find_module(core_module_name);
  /* It could happen that the module does not exist, just return with no errors
   */
  if (!module_manager.valid_module_id(core_module)) {
    return;
  }

  /* Start printing out Verilog netlists */
  /* Create the file name for Verilog netlist */
  std::string verilog_fname(
    generate_fpga_core_netlist_name(std::string(VERILOG_NETLIST_FILE_POSTFIX)));
  std::string verilog_fpath(verilog_dir + verilog_fname);

  VTR_LOG("Writing Verilog netlist for wrapper module of FPGA fabric '%s'...",
          verilog_fpath.c_str());

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fpath, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fpath.c_str(), fp);

  print_verilog_file_header(fp, std::string("Wrapper Verilog module for FPGA"),
                            options.time_stamp());

  /* Write the module content in Verilog format */
  write_verilog_module_to_file(fp, module_manager, core_module, options);

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = NetlistId::INVALID();
  if (options.use_relative_path()) {
    nlist_id = netlist_manager.add_netlist(verilog_fname);
  } else {
    nlist_id = netlist_manager.add_netlist(verilog_fpath);
  }
  VTR_ASSERT(nlist_id);
  netlist_manager.set_netlist_type(nlist_id,
                                   NetlistManager::TOP_MODULE_NETLIST);

  VTR_LOG("Done\n");
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
void print_verilog_top_module(NetlistManager& netlist_manager,
                              const ModuleManager& module_manager,
                              const ModuleNameMap& module_name_map,
                              const std::string& verilog_dir,
                              const FabricVerilogOption& options) {
  /* Create a module as the top-level fabric, and add it to the module manager
   */
  std::string top_module_name = generate_fpga_top_module_name();
  top_module_name = module_name_map.name(top_module_name);
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Start printing out Verilog netlists */
  /* Create the file name for Verilog netlist */
  std::string verilog_fname(
    generate_fpga_top_netlist_name(std::string(VERILOG_NETLIST_FILE_POSTFIX)));
  std::string verilog_fpath(verilog_dir + verilog_fname);

  VTR_LOG("Writing Verilog netlist for top-level module of FPGA fabric '%s'...",
          verilog_fpath.c_str());

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fpath, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fpath.c_str(), fp);

  print_verilog_file_header(
    fp, std::string("Top-level Verilog module for FPGA"), options.time_stamp());

  /* Write the module content in Verilog format */
  write_verilog_module_to_file(fp, module_manager, top_module, options);

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = NetlistId::INVALID();
  if (options.use_relative_path()) {
    nlist_id = netlist_manager.add_netlist(verilog_fname);
  } else {
    nlist_id = netlist_manager.add_netlist(verilog_fpath);
  }
  VTR_ASSERT(nlist_id);
  netlist_manager.set_netlist_type(nlist_id,
                                   NetlistManager::TOP_MODULE_NETLIST);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
