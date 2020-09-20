#ifndef SPICE_LUT_H
#define SPICE_LUT_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <fstream>
#include <string>

#include "circuit_library.h"
#include "module_manager.h"
#include "netlist_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_submodule_luts(NetlistManager& netlist_manager,
                               const ModuleManager& module_manager,
                               const CircuitLibrary& circuit_lib,
                               const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
