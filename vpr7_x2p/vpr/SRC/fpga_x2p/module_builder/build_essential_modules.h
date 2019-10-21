#ifndef BUILD_ESSENTIAL_MODULES_H
#define BUILD_ESSENTIAL_MODULES_H

#include "circuit_library.h"
#include "module_manager.h"

void build_essential_modules(ModuleManager& module_manager, 
                             const CircuitLibrary& circuit_lib);

void build_user_defined_modules(ModuleManager& module_manager, 
                                const CircuitLibrary& circuit_lib, 
                                const std::vector<t_segment_inf>& routing_segments);

void build_constant_generator_modules(ModuleManager& module_manager);

#endif
