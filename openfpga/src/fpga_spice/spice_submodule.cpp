/*********************************************************************
 * This file includes top-level function to generate SPICE primitive modules
 * and print them to files
 ********************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "spice_transistor_wrapper.h"
#include "spice_essential_gates.h"
#include "spice_mux.h"
#include "spice_lut.h"

#include "spice_constants.h"
#include "spice_submodule.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Top-level function to generate primitive modules:
 * 1. Transistor wrapper
 * 2. Logic gates: AND/OR, inverter, buffer and transmission-gate/pass-transistor
 * 3. TODO: Routing multiplexers
 * 4. TODO: Local encoders for routing multiplexers
 * 5. Wires
 * 6. TODO: Configuration memory blocks
 ********************************************************************/
int print_spice_submodule(NetlistManager& netlist_manager,
                          const ModuleManager& module_manager,
                          const Arch& openfpga_arch,
                          const MuxLibrary& mux_lib,
                          const std::string& submodule_dir) {

  int status = CMD_EXEC_SUCCESS;

  /* Transistor wrapper */
  status = print_spice_transistor_wrapper(netlist_manager,
                                          openfpga_arch.tech_lib,
                                          submodule_dir);

  /* Error out if fatal errors have been reported */
  if (CMD_EXEC_SUCCESS != status) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Constant modules: VDD and GND */
  status = print_spice_supply_wrappers(netlist_manager,
                                       module_manager,
                                       submodule_dir);

  /* Error out if fatal errors have been reported */
  if (CMD_EXEC_SUCCESS != status) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Logic gates: 
   *   - AND/OR, 
   *   - inverter, buffer
   *   - transmission-gate/pass-transistor
   *   - wires
   */
  status = print_spice_essential_gates(netlist_manager,
                                       module_manager,
                                       openfpga_arch.circuit_lib,
                                       openfpga_arch.tech_lib,
                                       openfpga_arch.circuit_tech_binding,
                                       submodule_dir);

  /* Error out if fatal errors have been reported */
  if (CMD_EXEC_SUCCESS != status) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Routing multiplexers */
  status = print_spice_submodule_muxes(netlist_manager,
                                       module_manager,
                                       mux_lib,
                                       openfpga_arch.circuit_lib,
                                       submodule_dir);

  /* Error out if fatal errors have been reported */
  if (CMD_EXEC_SUCCESS != status) {
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Look-Up Tables */
  status = print_spice_submodule_luts(netlist_manager,
                                      module_manager,
                                      openfpga_arch.circuit_lib,
                                      submodule_dir);

  /* Error out if fatal errors have been reported */
  if (CMD_EXEC_SUCCESS != status) {
    return CMD_EXEC_FATAL_ERROR;
  }

  return status;
}

} /* end namespace openfpga */
