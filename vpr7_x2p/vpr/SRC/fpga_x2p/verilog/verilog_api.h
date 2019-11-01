#ifndef VERILOG_API_H 
#define VERILOG_API_H 

#include <vector>
#include "vpr_types.h"
#include "mux_library.h"
#include "module_manager.h"
#include "bitstream_manager.h"

void vpr_fpga_verilog(ModuleManager& module_manager,
                      const BitstreamManager& bitstream_manager,
                      const std::vector<ConfigBitId>& fabric_bitstream,
                      const MuxLibrary& mux_lib,
                      const std::vector<t_logical_block>& L_logical_blocks,
                      const vtr::Point<size_t>& device_size,
                      const std::vector<std::vector<t_grid_tile>>& L_grids, 
                      const std::vector<t_block>& L_blocks,
                      t_vpr_setup vpr_setup,
                      t_arch Arch,
                      char* circuit_name);

#endif
