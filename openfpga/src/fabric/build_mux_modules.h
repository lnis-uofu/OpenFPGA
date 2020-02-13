#ifndef BUILD_MUX_MODULES_H
#define BUILD_MUX_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "circuit_library.h"
#include "mux_library.h"
#include "module_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void build_mux_modules(ModuleManager& module_manager,
                       const MuxLibrary& mux_lib,
                       const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
