/********************************************************************
 * Header file for build_device_bitstream.cpp
 *******************************************************************/
#ifndef BUILD_DEVICE_BITSTREAM_H
#define BUILD_DEVICE_BITSTREAM_H

#include <vector>
#include "bitstream_manager.h"
#include "vpr_types.h"
#include "module_manager.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "rr_blocks.h"

BitstreamManager build_device_bitstream(const t_vpr_setup& vpr_setup,
                                        const t_arch& arch,
                                        const ModuleManager& module_manager,
                                        const CircuitLibrary& circuit_lib,
                                        const MuxLibrary& mux_lib,
                                        const vtr::Point<size_t>& device_size,
                                        const std::vector<std::vector<t_grid_tile>>& grids,
                                        const std::vector<t_switch_inf>& rr_switches,
                                        t_rr_node* L_rr_node,
                                        const DeviceRRGSB& L_device_rr_gsb);

#endif
