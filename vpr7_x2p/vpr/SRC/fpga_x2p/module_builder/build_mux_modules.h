/***********************************************
 * Header file for verilog_mux.cpp
 **********************************************/

#ifndef BUILD_MUX_MODULES_H
#define BUILD_MUX_MODULES_H

/* Include other header files which are dependency on the function declared below */
#include "spice_types.h"
#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"
#include "module_manager.h"

void build_mux_modules(ModuleManager& module_manager,
                       const MuxLibrary& mux_lib,
                       const CircuitLibrary& circuit_lib);

#endif
