#ifndef VERILOG_SUBMODULE_H
#define VERILOG_SUBMODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "module_manager.h"
#include "mux_library.h"
#include "verilog_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule(ModuleManager& module_manager, 
                             const MuxLibrary& mux_lib,
                             const CircuitLibrary& circuit_lib, 
                             const std::string& verilog_dir, 
                             const std::string& submodule_dir, 
                             const FabricVerilogOption& fpga_verilog_opts);

} /* end namespace openfpga */

#endif
