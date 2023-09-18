#ifndef VERILOG_ESSENTIAL_GATES_H
#define VERILOG_ESSENTIAL_GATES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>

#include "circuit_library.h"
#include "fabric_verilog_options.h"
#include "module_manager.h"
#include "module_name_map.h"
#include "netlist_manager.h"
#include "verilog_port_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_essentials(const ModuleManager& module_manager,
                                        NetlistManager& netlist_manager,
                                        const std::string& submodule_dir,
                                        const std::string& submodule_dir_name,
                                        const CircuitLibrary& circuit_lib,
                                        const ModuleNameMap& module_name_map,
                                        const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
