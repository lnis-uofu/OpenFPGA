/******************************************************************** 
 * Header file for build_lut_modules.cpp
 ********************************************************************/
#ifndef BUILD_LUT_MODULES_H
#define BUILD_LUT_MODULES_H

#include "circuit_library.h"
#include "module_manager.h"

void build_lut_modules(ModuleManager& module_manager,
                       const CircuitLibrary& circuit_lib);

#endif
