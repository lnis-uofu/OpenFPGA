/********************************************************************
 * This file includes functions to build bitstream from a mapped
 * FPGA fabric.
 * We decode the bitstream from configuration of routing multiplexers 
 * and Look-Up Tables (LUTs) which locate in CLBs and global routing architecture
 *******************************************************************/
#include <vector>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "openfpga_naming.h"

//#include "build_grid_bitstream.h"
#include "build_routing_bitstream.h"
#include "build_device_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A top-level function to build a bistream from the FPGA device
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
BitstreamManager build_device_bitstream(const VprContext& vpr_ctx,
                                        const OpenfpgaContext& openfpga_ctx,
                                        const bool& verbose) {

  std::string timer_message = std::string("\nBuild fabric-independent bitstream for implementation '") + vpr_ctx.atom().nlist.netlist_name() + std::string("'\n");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Bitstream manager to be built */
  BitstreamManager bitstream_manager;

  /* Assign the SRAM model applied to the FPGA fabric */
  CircuitModelId sram_model = openfpga_ctx.arch().config_protocol.memory_model();  
  VTR_ASSERT(true == openfpga_ctx.arch().circuit_lib.valid_model_id(sram_model));

  /* Create the top-level block for bitstream 
   * This is related to the top-level module of fpga  
   */
  std::string top_block_name = generate_fpga_top_module_name();
  ConfigBlockId top_block = bitstream_manager.add_block(top_block_name);

  /* Create bitstream from grids */
  VTR_LOGV(verbose, "Building grid bitstream...\n");
  //build_grid_bitstream(bitstream_manager, top_block, module_manager, circuit_lib, mux_lib, device_size, grids);
  VTR_LOGV(verbose, "Done\n");

  /* Create bitstream from routing architectures */
  VTR_LOGV(verbose, "Building routing bitstream...\n");
  build_routing_bitstream(bitstream_manager, top_block, 
                          openfpga_ctx.module_graph(),
                          openfpga_ctx.arch().circuit_lib,
                          openfpga_ctx.mux_lib(),
                          openfpga_ctx.vpr_device_annotation(),
                          openfpga_ctx.vpr_routing_annotation(),
                          vpr_ctx.device().rr_graph,
                          openfpga_ctx.device_rr_gsb());
  VTR_LOGV(verbose, "Done\n");

  VTR_LOGV(verbose, "Decoded %lu configuration bits\n", bitstream_manager.bits().size());

  return bitstream_manager;
}

} /* end namespace openfpga */
