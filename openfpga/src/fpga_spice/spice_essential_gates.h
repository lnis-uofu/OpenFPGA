#ifndef SPICE_ESSENTIAL_GATES_H
#define SPICE_ESSENTIAL_GATES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <map>
#include "netlist_manager.h"
#include "module_manager.h"
#include "circuit_library.h"
#include "technology_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_supply_wrappers(NetlistManager& netlist_manager,
                                const ModuleManager& module_manager,
                                const std::string& submodule_dir);

int print_spice_essential_gates(NetlistManager& netlist_manager,
                                const ModuleManager& module_manager,
                                const CircuitLibrary& circuit_lib,
                                const TechnologyLibrary& tech_lib,
                                const std::map<CircuitModelId, TechnologyModelId>& circuit_tech_binding,
                                const std::string& submodule_dir);

} /* end namespace openfpga */

#endif
