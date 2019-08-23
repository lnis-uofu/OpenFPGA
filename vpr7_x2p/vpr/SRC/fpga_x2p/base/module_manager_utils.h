/******************************************************************************
 * This files includes declarations for most utilized functions 
 * for data structures for module management.
 ******************************************************************************/

#ifndef MODULE_MANAGER_UTILS_H
#define MODULE_MANAGER_UTILS_H

ModuleId add_circuit_model_to_module_manager(ModuleManager& module_manager, 
                                             const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model);


#endif

