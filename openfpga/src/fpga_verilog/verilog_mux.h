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

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_muxes(ModuleManager& module_manager,
                                   std::vector<std::string>& netlist_names,
                                   const MuxLibrary& mux_lib,
                                   const CircuitLibrary& circuit_lib,
                                   const std::string& verilog_dir,
                                   const std::string& submodule_dir,
                                   const bool& use_explicit_port_map);

} /* end namespace openfpga */

#endif
