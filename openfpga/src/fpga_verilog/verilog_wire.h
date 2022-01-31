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
#include "fabric_verilog_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_wires(const ModuleManager& module_manager,
                                   NetlistManager& netlist_manager,
                                   const CircuitLibrary& circuit_lib,
                                   const std::string& submodule_dir,
                                   const std::string& submodule_dir_name,
                                   const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
