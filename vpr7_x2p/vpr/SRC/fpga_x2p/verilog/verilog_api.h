#ifndef VERILOG_API_H 
#define VERILOG_API_H 

#include <string>
#include <vector>
#include "vpr_types.h"
#include "mux_library.h"
#include "rr_blocks.h"
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
                      const DeviceRRGSB& L_device_rr_gsb,
                      const t_vpr_setup& vpr_setup,
                      const t_arch& Arch,
                      const std::string& circuit_name,
                      t_sram_orgz_info* sram_verilog_orgz_info);

/* TODO: Old function to be deprecated */
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
