/***********************************************
 * Header file for build_memory_modules.cpp
 **********************************************/

#ifndef BUILD_MEMORY_MODULES_H
#define BUILD_MEMORY_MODULES_H

/* Include other header files which are dependency on the function declared below */

#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"

void build_memory_modules(ModuleManager& module_manager,
                          const MuxLibrary& mux_lib,
                          const CircuitLibrary& circuit_lib,
                          const e_sram_orgz& sram_orgz_type);

#endif
