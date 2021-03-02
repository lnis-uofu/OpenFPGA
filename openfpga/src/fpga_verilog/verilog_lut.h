#ifndef VERILOG_LUT_H
#define VERILOG_LUT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>

#include "circuit_library.h"
#include "module_manager.h"
#include "netlist_manager.h"
#include "fabric_verilog_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_luts(const ModuleManager& module_manager,
                                  NetlistManager& netlist_manager,
                                  const CircuitLibrary& circuit_lib,
                                  const std::string& submodule_dir,
                                  const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
