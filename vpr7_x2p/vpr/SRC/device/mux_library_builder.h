/********************************************************************
 * This file includes the function declaration of builders
 * for MuxLibrary.
 * See details in mux_library_builder.cpp
 *******************************************************************/
#ifndef MUX_LIBRARY_BUILDER_H
#define MUX_LIBRARY_BUILDER_H

#include "vpr_types.h"
#include "circuit_library.h"
#include "mux_library.h"

MuxLibrary build_device_mux_library(int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                    t_switch_inf* switches,
                                    const CircuitLibrary& circuit_lib,
                                    t_det_routing_arch* routing_arch);

#endif
