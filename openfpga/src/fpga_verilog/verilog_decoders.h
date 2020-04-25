#ifndef VERILOG_DECODERS_H
#define VERILOG_DECODERS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>
#include <vector>

#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void print_verilog_submodule_mux_local_decoders(const ModuleManager& module_manager,
                                                NetlistManager& netlist_manager,
                                                const MuxLibrary& mux_lib,
                                                const CircuitLibrary& circuit_lib,
                                                const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
