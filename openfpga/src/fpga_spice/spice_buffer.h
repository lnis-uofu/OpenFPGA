#ifndef SPICE_BUFFER_H
#define SPICE_BUFFER_H

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

int print_spice_inverter_subckt(std::fstream& fp,
                                const ModuleManager& module_manager,
                                const ModuleId& module_id,
                                const CircuitLibrary& circuit_lib,
                                const CircuitModelId& circuit_model,
                                const TechnologyLibrary& tech_lib,
                                const TechnologyModelId& tech_model);

int print_spice_buffer_subckt(std::fstream& fp,
                              const ModuleManager& module_manager,
                              const ModuleId& module_id,
                              const CircuitLibrary& circuit_lib,
                              const CircuitModelId& circuit_model,
                              const TechnologyLibrary& tech_lib,
                              const TechnologyModelId& tech_model);


} /* end namespace openfpga */

#endif
