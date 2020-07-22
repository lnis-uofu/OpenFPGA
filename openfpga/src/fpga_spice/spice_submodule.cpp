/*********************************************************************
 * This file includes top-level function to generate Spice primitive modules
 * and print them to files
 ********************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "spice_essential_gates.h"

#include "spice_constants.h"
#include "spice_submodule.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Top-level function to generate primitive modules:
 * 1. Transistor wrapper
 * 2. TODO: Logic gates: AND/OR, inverter, buffer and transmission-gate/pass-transistor
 * 3. TODO: Routing multiplexers
 * 4. TODO: Local encoders for routing multiplexers
 * 5. TODO: Wires
 * 6. TODO: Configuration memory blocks
 ********************************************************************/
int print_spice_submodule(NetlistManager& netlist_manager,
                          const TechnologyLibrary& tech_lib,
                          const std::string& submodule_dir) {

  int status = CMD_EXEC_SUCCESS;

  status = print_spice_transistor_wrapper(netlist_manager,
                                          tech_lib,
                                          submodule_dir);

  return status;
}

} /* end namespace openfpga */
