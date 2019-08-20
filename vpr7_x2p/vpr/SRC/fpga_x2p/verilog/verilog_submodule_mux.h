/***********************************************
 * Header file for verilog_submodule_mux.cpp
 **********************************************/

#ifndef VERILOG_SUBMODULE_MUX_H
#define VERILOG_SUBMODULE_MUX_H

/* Include other header files which are dependency on the function declared below */
#include <fstream>

#include "circuit_library.h"
#include "mux_graph.h"
#include "mux_library.h"

void generate_verilog_mux_branch_module(std::fstream& fp, 
                                        const CircuitLibrary& circuit_lib, 
                                        const CircuitModelId& circuit_model, 
                                        const MuxGraph& mux_graph);

#endif
