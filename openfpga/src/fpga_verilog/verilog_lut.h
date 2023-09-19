#ifndef VERILOG_LUT_H
#define VERILOG_LUT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>

#include "circuit_library.h"
#include "fabric_verilog_options.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_luts(const ModuleManager& module_manager,
                                  NetlistManager& netlist_manager,
                                  const CircuitLibrary& circuit_lib,
                                  const ModuleNameMap& module_name_map,
                                  const std::string& submodule_dir,
                                  const std::string& submodule_dir_name,
                                  const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
