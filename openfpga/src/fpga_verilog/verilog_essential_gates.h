#ifndef VERILOG_ESSENTIAL_GATES_H
#define VERILOG_ESSENTIAL_GATES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_essentials(const ModuleManager& module_manager, 
                                        std::vector<std::string>& netlist_names,
                                        const std::string& verilog_dir, 
                                        const std::string& submodule_dir,
                                        const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
