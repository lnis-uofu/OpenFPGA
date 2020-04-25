#ifndef VERILOG_ESSENTIAL_GATES_H
#define VERILOG_ESSENTIAL_GATES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "circuit_library.h"
#include "module_manager.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_essentials(const ModuleManager& module_manager, 
                                        NetlistManager& netlist_manager,
                                        const std::string& submodule_dir,
                                        const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
