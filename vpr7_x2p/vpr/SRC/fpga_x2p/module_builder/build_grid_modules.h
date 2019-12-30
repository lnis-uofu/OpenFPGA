/********************************************************************
 * Header file for build_grid_modules.cpp
 *******************************************************************/
#ifndef BUILD_GRID_MODULES_H
#define BUILD_GRID_MODULES_H

/* Only include headers related to the data structures used in the following function declaration */
#include "vpr_types.h"
#include "module_manager.h"
#include "mux_library.h"

void build_grid_modules(ModuleManager& module_manager,
                        const CircuitLibrary& circuit_lib,
                        const MuxLibrary& mux_lib,
                        const e_sram_orgz& sram_orgz_type,
                        const CircuitModelId& sram_model,
                        const bool& duplicate_grid_pin);

#endif
