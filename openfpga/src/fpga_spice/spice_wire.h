#ifndef SPICE_WIRE_H
#define SPICE_WIRE_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <map>
#include "module_manager.h"
#include "circuit_library.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int print_spice_wire_subckt(std::fstream& fp,
                            const ModuleManager& module_manager,
                            const ModuleId& module_id,
                            const CircuitLibrary& circuit_lib,
                            const CircuitModelId& circuit_model);

} /* end namespace openfpga */

#endif
