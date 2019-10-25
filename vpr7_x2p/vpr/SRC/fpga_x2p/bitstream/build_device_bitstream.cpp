/********************************************************************
 * This file includes functions to build bitstream from a mapped
 * FPGA fabric.
 * We decode the bitstream from configuration of routing multiplexers 
 * and Look-Up Tables (LUTs) which locate in CLBs and global routing architecture
 *******************************************************************/
#include <vector>
#include <time.h>

#include "vtr_assert.h"
#include "util.h"

#include "fpga_x2p_naming.h"

#include "build_routing_bitstream.h"
#include "build_device_bitstream.h"

/********************************************************************
 * Top-level function to build a bistream from the FPGA device
 * 1. It will organize the bitstream w.r.t. the hierarchy of module graphs 
 *    describing the FPGA fabric
 * 2. It will decode configuration bits from routing multiplexers used in 
 *    global routing architecture
 * 3. It will decode configuration bits from routing multiplexers and LUTs
 *    used in CLBs
 *
 * Note: this function create a bitstream which is binding to the module graphs
 * of the FPGA fabric that FPGA-X2P generates!
 * But it can be used to output a generic bitstream for VPR mapping FPGA
 *******************************************************************/
BitstreamManager build_device_bitstream(const t_vpr_setup& vpr_setup,
                                        const t_arch& arch,
                                        const ModuleManager& module_manager,
                                        const CircuitLibrary& circuit_lib,
                                        const MuxLibrary& mux_lib,
                                        const vtr::Point<size_t>& device_size,
                                        const std::vector<std::vector<t_grid_tile>>& grids,
                                        const std::vector<t_switch_inf>& rr_switches,
                                        t_rr_node* L_rr_node,
                                        const DeviceRRGSB& L_device_rr_gsb) {
  /* Check if the routing architecture we support*/
  if (UNI_DIRECTIONAL != vpr_setup.RoutingArch.directionality) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P only supports uni-directional routing architecture!\n");
    exit(1);
  }

  /* We don't support mrFPGA */
#ifdef MRFPGA_H
  if (is_mrFPGA) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "FPGA X2P does not support mrFPGA!\n");
    exit(1);
  }
#endif

  /* Bistream builder formally starts*/
  vpr_printf(TIO_MESSAGE_INFO, "\nStart building bitstream for FPGA fabric...\n");

  /* Bitstream manager to be built */
  BitstreamManager bitstream_manager;

  /* Start time count */
  clock_t t_start = clock();

  /* Assign the SRAM model applied to the FPGA fabric */
  VTR_ASSERT(NULL != arch.sram_inf.verilog_sram_inf_orgz); /* Check !*/
  t_spice_model* mem_model = arch.sram_inf.verilog_sram_inf_orgz->spice_model;
  /* initialize the SRAM organization information struct */
  CircuitModelId sram_model = arch.spice->circuit_lib.model(mem_model->name);  
  VTR_ASSERT(CircuitModelId::INVALID() != sram_model);

  /* Create the top-level block for bitstream 
   * This is related to the top-level module of fpga  
   */
  std::string top_block_name = generate_fpga_top_module_name();
  ConfigBlockId top_block = bitstream_manager.add_block(top_block_name);

  /* Create bitstream from routing architectures */
  build_routing_bitstream(bitstream_manager, module_manager, circuit_lib, mux_lib, rr_switches, L_rr_node, L_device_rr_gsb);

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "Building bitstream took %g seconds\n", 
             run_time_sec);  

  return bitstream_manager;
}
