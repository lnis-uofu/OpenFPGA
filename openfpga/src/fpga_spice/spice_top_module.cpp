/********************************************************************
 * This file includes functions that are used to print the top-level
 * module for the FPGA fabric in SPICE format
 *******************************************************************/
#include <fstream>
#include <map>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "openfpga_naming.h"

#include "spice_constants.h"
#include "spice_writer_utils.h"
#include "spice_subckt_writer.h"
#include "spice_top_module.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print the top-level module for the FPGA fabric in SPICE format
 * This function will 
 * 1. name the top-level module
 * 2. include dependent netlists 
 *    - User defined netlists
 *    - Auto-generated netlists
 * 3. Add the submodules to the top-level graph
 * 4. Add module nets to connect datapath ports
 * 5. Add module nets/submodules to connect configuration ports
 *******************************************************************/
void print_spice_top_module(NetlistManager& netlist_manager,
                            const ModuleManager& module_manager,
                            const std::string& spice_dir) {
  /* Create a module as the top-level fabric, and add it to the module manager */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Create the file name for SPICE netlist */
  std::string spice_fname(spice_dir + generate_fpga_top_netlist_name(std::string(SPICE_NETLIST_FILE_POSTFIX)));

  VTR_LOG("Writing SPICE netlist for top-level module of FPGA fabric '%s'...",
          spice_fname.c_str());

  /* Create the file stream */
  std::fstream fp;
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(spice_fname.c_str(), fp);

  print_spice_file_header(fp, std::string("Top-level SPICE subckt for FPGA")); 

  /* Write the module content in Verilog format */
  write_spice_subckt_to_file(fp, module_manager, top_module);

  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::TOP_MODULE_NETLIST);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
