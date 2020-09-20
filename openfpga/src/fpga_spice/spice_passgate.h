#ifndef SPICE_PASSGATE_H
#define SPICE_PASSGATE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <map>
#include "module_manager.h"
#include "circuit_library.h"
#include "technology_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_passgate_subckt(std::fstream& fp,
                                const ModuleManager& module_manager,
                                const ModuleId& module_id,
                                const CircuitLibrary& circuit_lib,
                                const CircuitModelId& circuit_model,
                                const TechnologyLibrary& tech_lib,
                                const TechnologyModelId& tech_model);

} /* end namespace openfpga */

#endif
