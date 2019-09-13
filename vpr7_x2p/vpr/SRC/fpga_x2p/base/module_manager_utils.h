/******************************************************************************
 * This files includes declarations for most utilized functions 
 * for data structures for module management.
 ******************************************************************************/

#ifndef MODULE_MANAGER_UTILS_H
#define MODULE_MANAGER_UTILS_H

/* Include other header files which are dependency on the function declared below */
#include "circuit_library.h"
#include "module_manager.h"

ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model,
                                             const std::string& module_name);

ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model);

#endif

