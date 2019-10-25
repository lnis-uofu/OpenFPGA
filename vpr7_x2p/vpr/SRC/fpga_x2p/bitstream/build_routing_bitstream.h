/********************************************************************
 * Header file for build_routing_bitstream.cpp
 *******************************************************************/
#ifndef BUILD_ROUTING_BITSTREAM_H
#define BUILD_ROUTING_BITSTREAM_H

#include <vector>
#include "bitstream_manager.h"
#include "vpr_types.h"
#include "module_manager.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "rr_blocks.h"

void build_routing_bitstream(BitstreamManager& bitstream_manager,
                             const ConfigBlockId& top_configurable_block,
                             const ModuleManager& module_manager,
                             const CircuitLibrary& circuit_lib,
                             const MuxLibrary& mux_lib,
                             const std::vector<std::vector<t_grid_tile>>& grids,
                             const std::vector<t_switch_inf>& rr_switches,
                             t_rr_node* L_rr_node,
                             const DeviceRRGSB& L_device_rr_gsb);

#endif
