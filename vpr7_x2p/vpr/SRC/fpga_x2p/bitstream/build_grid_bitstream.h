/********************************************************************
 * Header file for build_grid_bitstream.cpp
 *******************************************************************/
#ifndef BUILD_GRID_BITSTREAM_H
#define BUILD_GRID_BITSTREAM_H

#include <vector>
#include "vtr_geometry.h"
#include "vpr_types.h"
#include "bitstream_manager.h"
#include "module_manager.h"
#include "circuit_library.h"
#include "mux_library.h"

void build_grid_bitstream(BitstreamManager& bitstream_manager,
                          const ConfigBlockId& top_block,
                          const ModuleManager& module_manager,
                          const CircuitLibrary& circuit_lib,
                          const MuxLibrary& mux_lib,
                          const vtr::Point<size_t>& device_size,
                          const std::vector<std::vector<t_grid_tile>>& grids);
#endif
