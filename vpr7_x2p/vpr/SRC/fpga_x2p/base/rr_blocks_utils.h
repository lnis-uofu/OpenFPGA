/********************************************************************
 * Header file for rr_block_utils.cpp
 *******************************************************************/
#ifndef RR_BLOCKS_UTILS_H
#define RR_BLOCKS_UTILS_H

/* Include other header file required by the function declaration */
#include <vector>
#include "physical_types.h"
#include "circuit_library.h"
#include "rr_blocks.h"

std::vector<CircuitPortId> find_switch_block_global_ports(const RRGSB& rr_gsb, 
                                                          const CircuitLibrary& circuit_lib,
                                                          const std::vector<t_switch_inf>& switch_lib);

#endif
