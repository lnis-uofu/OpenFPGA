#ifndef VERILOG_TOP_MODULE_H
#define VERILOG_TOP_MODULE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include "module_manager.h"
#include "netlist_manager.h"
#include "fabric_verilog_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_top_module(NetlistManager& netlist_manager,
                              const ModuleManager& module_manager,
                              const std::string& verilog_dir,
                              const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
