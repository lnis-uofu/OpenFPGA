#ifndef BUILD_WIRE_MODULES_H
#define BUILD_WIRE_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "circuit_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_wire_modules(ModuleManager& module_manager,
                        const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
