#ifndef SPICE_MUX_H
#define SPICE_MUX_H

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

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_submodule_muxes(NetlistManager& netlist_manager,
                                const ModuleManager& module_manager,
                                const MuxLibrary& mux_lib,
                                const CircuitLibrary& circuit_lib,
                                const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
