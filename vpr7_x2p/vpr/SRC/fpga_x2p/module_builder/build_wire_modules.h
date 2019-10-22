/***********************************************
 * Header file for verilog_wire.cpp
 **********************************************/

#ifndef BUILD_WIRE_MODULES_H
#define BUILD_WIRE_MODULES_H

/* Include other header files which are dependency on the function declared below */
#include <vector>
#include "physical_types.h"
#include "vpr_types.h"

#include "circuit_library.h"
#include "module_manager.h"

void build_wire_modules(ModuleManager& module_manager,
                        const CircuitLibrary& circuit_lib,
                        std::vector<t_segment_inf> routing_segments);

#endif
