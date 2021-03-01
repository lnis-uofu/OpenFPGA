#ifndef VERILOG_MUX_H
#define VERILOG_MUX_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <vector>

#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"
#include "netlist_manager.h"
#include "fabric_verilog_options.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_muxes(ModuleManager& module_manager,
                                   NetlistManager& netlist_manager,
                                   const MuxLibrary& mux_lib,
                                   const CircuitLibrary& circuit_lib,
                                   const std::string& submodule_dir,
                                   const FabricVerilogOption& options);

} /* end namespace openfpga */

#endif
