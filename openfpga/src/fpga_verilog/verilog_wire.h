#ifndef VERILOG_WIRE_H
#define VERILOG_WIRE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <vector>

#include "circuit_library.h"
#include "module_manager.h"
#include "netlist_manager.h"
#include "verilog_port_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_wires(const ModuleManager& module_manager,
                                   NetlistManager& netlist_manager,
                                   const CircuitLibrary& circuit_lib,
                                   const std::string& submodule_dir,
                                   const e_verilog_default_net_type& default_net_type);

} /* end namespace openfpga */

#endif
