#ifndef VERILOG_SUBMODULE_UTILS_H
#define VERILOG_SUBMODULE_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>
#include "module_manager.h"
#include "circuit_library.h"
#include "verilog_port_types.h"
#include "fabric_verilog_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_timing(std::fstream& fp, 
                                    const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model);

void add_user_defined_verilog_modules(ModuleManager& module_manager, 
                                      const CircuitLibrary& circuit_lib);

void print_verilog_submodule_templates(const ModuleManager& module_manager,
                                       const CircuitLibrary& circuit_lib,
                                       const std::string& submodule_dir,
                                       const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
