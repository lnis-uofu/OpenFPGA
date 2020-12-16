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

#include "module_manager_utils.h"

#include "build_grid_bitstream.h"
#include "build_routing_bitstream.h"
#include "build_device_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Estimate the number of blocks to be added to the whole device bitstream
 * This function will recursively walk through the module graph 
 * from the specified top module and count the number of configurable children
 * which are the blocks that will be added to the bitstream manager
 *******************************************************************/
static 
size_t rec_estimate_device_bitstream_num_blocks(const ModuleManager& module_manager,
                                                const ModuleId& top_module) {
  size_t num_blocks = 0;

  /* Those child modules which have no children are
   * actually configurable memory elements
   * We skip them in couting
   */
  if (0 == module_manager.configurable_children(top_module).size()) {
    return 0;
  }

  size_t num_configurable_children = module_manager.configurable_children(top_module).size();
  for (size_t ichild = 0; ichild < num_configurable_children; ++ichild) {
    ModuleId child_module = module_manager.configurable_children(top_module)[ichild];
    num_blocks += rec_estimate_device_bitstream_num_blocks(module_manager, child_module);
  }

  /* Add the number of blocks at current level */
  num_blocks++;

  return num_blocks;
}

/********************************************************************
 * Estimate the number of configuration bits to be added to the whole device bitstream
 * This function will recursively walk through the module graph 
 * from the specified top module and count the number of leaf configurable children
 * which are the bits that will be added to the bitstream manager
 *******************************************************************/
static 
size_t rec_estimate_device_bitstream_num_bits(const ModuleManager& module_manager,
                                              const ModuleId& top_module,
                                              const ModuleId& parent_module,
                                              const e_config_protocol_type& config_protocol_type) {
  size_t num_bits = 0;

  /* If a child module has no configurable children, this is a leaf node 
   * We can count it in. Otherwise, we should go recursively.
   */
  if (0 == module_manager.configurable_children(parent_module).size()) {
    return 1;
  }

  /* Two cases to walk through configurable children:
   * - For top-level module:
   *   Iterate over the multiple regions and visit each configuration child under any region
   *   In each region, frame-based configuration protocol or memory bank protocol will contain
   *   decoders. We should bypass them when count the bitstream size
   * - For other modules:
   *   Iterate over the configurable children regardless of regions
   */
  if (parent_module == top_module) {
    for (const ConfigRegionId& config_region : module_manager.regions(parent_module)) {
      size_t curr_region_num_config_child = module_manager.region_configurable_children(parent_module, config_region).size();

      /* Frame-based configuration protocol will have 1 decoder
       * if there are more than 1 configurable children
       */
      if ( (CONFIG_MEM_FRAME_BASED == config_protocol_type)
        && (2 <= curr_region_num_config_child)) {
        curr_region_num_config_child--;
      }

      /* Memory configuration protocol will have 2 decoders
       * at the top-level
       */
      if (CONFIG_MEM_MEMORY_BANK == config_protocol_type) {
        VTR_ASSERT(2 <= curr_region_num_config_child);
        curr_region_num_config_child -= 2;
      }

      /* Visit all the children in a recursively way */
      for (size_t ichild = 0; ichild < curr_region_num_config_child; ++ichild) {
        ModuleId child_module = module_manager.region_configurable_children(parent_module, config_region)[ichild];
        num_bits += rec_estimate_device_bitstream_num_bits(module_manager, top_module, child_module, config_protocol_type);
      }
    }
  } else {
    VTR_ASSERT_SAFE(parent_module != top_module);

    size_t num_configurable_children = module_manager.configurable_children(parent_module).size();

    /* Frame-based configuration protocol will have 1 decoder
     * if there are more than 1 configurable children
     */
    if ( (CONFIG_MEM_FRAME_BASED == config_protocol_type)
      && (2 <= num_configurable_children)) {
      num_configurable_children--;
    }

    for (size_t ichild = 0; ichild < num_configurable_children; ++ichild) {
      ModuleId child_module = module_manager.configurable_children(parent_module)[ichild];
      num_bits += rec_estimate_device_bitstream_num_bits(module_manager, top_module, child_module, config_protocol_type);
    }
  }

  return num_bits;
}

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

  /* Create the top-level block for bitstream 
   * This is related to the top-level module of fpga  
   */
  std::string top_block_name = generate_fpga_top_module_name();
  ConfigBlockId top_block = bitstream_manager.add_block(top_block_name);
  const ModuleId& top_module = openfpga_ctx.module_graph().find_module(top_block_name);
  VTR_ASSERT(true == openfpga_ctx.module_graph().valid_module_id(top_module));

  /* Estimate the number of blocks to be added to the database */
  size_t num_blocks_to_reserve = rec_estimate_device_bitstream_num_blocks(openfpga_ctx.module_graph(),
                                                                          top_module);
  bitstream_manager.reserve_blocks(num_blocks_to_reserve);
  VTR_LOGV(verbose, "Reserved %lu configurable blocks\n", num_blocks_to_reserve);

  /* Estimate the number of bits to be added to the database */
  size_t num_bits_to_reserve = rec_estimate_device_bitstream_num_bits(openfpga_ctx.module_graph(),
                                                                      top_module,
                                                                      top_module,
                                                                      openfpga_ctx.arch().config_protocol.type());
  bitstream_manager.reserve_bits(num_bits_to_reserve);
  VTR_LOGV(verbose, "Reserved %lu configuration bits\n", num_bits_to_reserve);

  /* Reserve child blocks for the top level block */
  bitstream_manager.reserve_child_blocks(top_block,
                                         count_module_manager_module_configurable_children(openfpga_ctx.module_graph(), top_module)); 

  /* Create bitstream from grids */
  VTR_LOGV(verbose, "Building grid bitstream...\n");
  build_grid_bitstream(bitstream_manager, top_block,
                       openfpga_ctx.module_graph(),
                       openfpga_ctx.arch().circuit_lib,
                       openfpga_ctx.mux_lib(),
                       vpr_ctx.device().grid,
                       vpr_ctx.atom(),
                       openfpga_ctx.vpr_device_annotation(),
                       openfpga_ctx.vpr_clustering_annotation(),
                       openfpga_ctx.vpr_placement_annotation(),
                       verbose);
  VTR_LOGV(verbose, "Done\n");

  /* Create bitstream from routing architectures */
  VTR_LOGV(verbose, "Building routing bitstream...\n");
  build_routing_bitstream(bitstream_manager, top_block, 
                          openfpga_ctx.module_graph(),
                          openfpga_ctx.arch().circuit_lib,
                          openfpga_ctx.mux_lib(),
                          vpr_ctx.atom(),
                          openfpga_ctx.vpr_device_annotation(),
                          openfpga_ctx.vpr_routing_annotation(),
                          vpr_ctx.device().rr_graph,
                          openfpga_ctx.device_rr_gsb(),
                          openfpga_ctx.flow_manager().compress_routing());
  VTR_LOGV(verbose, "Done\n");

  VTR_LOGV(verbose,
           "Decoded %lu configuration bits into %lu blocks\n",
           bitstream_manager.num_bits(),
           bitstream_manager.num_blocks());

  VTR_ASSERT(num_blocks_to_reserve == bitstream_manager.num_blocks());
  VTR_ASSERT(num_bits_to_reserve == bitstream_manager.num_bits());

  return bitstream_manager;
}

} /* end namespace openfpga */
