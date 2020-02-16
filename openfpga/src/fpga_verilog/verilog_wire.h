#ifndef VERILOG_WIRE_H
#define VERILOG_WIRE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <vector>

#include "circuit_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_wires(const ModuleManager& module_manager,
                                   std::vector<std::string>& netlist_names,
                                   const CircuitLibrary& circuit_lib,
                                   const std::string& verilog_dir,
                                   const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
