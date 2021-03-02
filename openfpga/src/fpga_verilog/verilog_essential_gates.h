#ifndef VERILOG_ESSENTIAL_GATES_H
#define VERILOG_ESSENTIAL_GATES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "circuit_library.h"
#include "module_manager.h"
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
                                        const CircuitLibrary& circuit_lib,
                                        const e_verilog_default_net_type& default_net_type);

} /* end namespace openfpga */

#endif
