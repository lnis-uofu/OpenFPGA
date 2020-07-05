/************************************************
 * This file includes functions on 
 * outputting Verilog netlists for essential gates
 * which are inverters, buffers, transmission-gates
 * logic gates etc. 
 ***********************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "spice_constants.h"
#include "spice_essential_gates.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************
 * Generate the SPICE netlist for transistors
 ***********************************************/
void print_spice_transistor_wrapper(NetlistManager& netlist_manager,
                                    const std::string& submodule_dir) {
  std::string spice_fname = submodule_dir + std::string(ESSENTIALS_SPICE_FILE_NAME);

  std::fstream fp;

  /* Create the file stream */
  fp.open(spice_fname, std::fstream::out | std::fstream::trunc);
  /* Check if the file stream if valid or not */
  check_file_stream(spice_fname.c_str(), fp); 

  /* Create file */
  VTR_LOG("Generating SPICE netlist '%s' for transistor wrappers...",
          spice_fname.c_str()); 

  /* Close file handler*/
  fp.close();

  /* Add fname to the netlist name list */
  NetlistId nlist_id = netlist_manager.add_netlist(spice_fname);
  VTR_ASSERT(NetlistId::INVALID() != nlist_id);
  netlist_manager.set_netlist_type(nlist_id, NetlistManager::SUBMODULE_NETLIST);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
