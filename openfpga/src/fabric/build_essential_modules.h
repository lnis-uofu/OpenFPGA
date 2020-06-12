#ifndef BUILD_ESSENTIAL_MODULES_H
#define BUILD_ESSENTIAL_MODULES_H

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

void build_essential_modules(ModuleManager& module_manager, 
                             const CircuitLibrary& circuit_lib);

void build_user_defined_modules(ModuleManager& module_manager, 
                                const CircuitLibrary& circuit_lib);

void build_constant_generator_modules(ModuleManager& module_manager);

void rename_primitive_module_port_names(ModuleManager& module_manager, 
                                        const CircuitLibrary& circuit_lib);

} /* end namespace openfpga */

#endif
