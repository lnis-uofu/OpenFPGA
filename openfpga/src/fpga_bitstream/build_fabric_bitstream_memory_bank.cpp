/********************************************************************
 * This file includes functions to build fabric dependent bitstream
 * for memory bank configuration protocol
 *******************************************************************/
#include <string>
#include <cmath>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_decode.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "decoder_library_utils.h"
#include "bitstream_manager_utils.h"
#include "memory_bank_utils.h"
#include "build_fabric_bitstream_memory_bank.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to build a bitstream for memory-bank protocol
 * It will walk through all the configurable children under a module
 * in a recursive way, following a Depth-First Search (DFS) strategy
 * For each configuration child, we use its instance name as a key to spot the 
 * configuration bits in bitstream manager.
 * Note that it is guarentee that the instance name in module manager is 
 * consistent with the block names in bitstream manager
 * We use this link to reorganize the bitstream in the sequence of memories as we stored
 * in the configurable_children() and configurable_child_instances() of each module of module manager 
 *
 * In such configuration organization, each memory cell has an unique index.
 * Using this index, we can infer the address codes for both BL and WL decoders.
 * Note that, we must get the number of BLs and WLs before using this function!
 *******************************************************************/
static 
void rec_build_module_fabric_dependent_ql_memory_bank_regional_bitstream(const BitstreamManager& bitstream_manager,
                                                                         const ConfigBlockId& parent_block,
                                                                         const ModuleManager& module_manager,
                                                                         const ModuleId& top_module,
                                                                         const ModuleId& parent_module,
                                                                         const ConfigRegionId& config_region,
                                                                         const size_t& bl_addr_size,
                                                                         const size_t& wl_addr_size,
                                                                         const std::map<int, size_t>& num_bls_per_tile,
                                                                         const std::map<int, size_t>& bl_start_index_per_tile,
                                                                         const std::map<int, size_t>& num_wls_per_tile,
                                                                         const std::map<int, size_t>& wl_start_index_per_tile,
                                                                         vtr::Point<int>& tile_coord,
                                                                         size_t& cur_mem_index,
                                                                         FabricBitstream& fabric_bitstream,
                                                                         const FabricBitRegionId& fabric_bitstream_region) {

  /* Depth-first search: if we have any children in the parent_block, 
   * we dive to the next level first! 
   */
  if (0 < bitstream_manager.block_children(parent_block).size()) {
    /* For top module:
     *   - Use regional configurable children
     *   - we will skip the two decoders at the end of the configurable children list
     */
    if (parent_module == top_module) {
      std::vector<ModuleId> configurable_children = module_manager.region_configurable_children(parent_module, config_region);

      VTR_ASSERT(2 <= configurable_children.size()); 
      size_t num_configurable_children = configurable_children.size() - 2;

      /* Early exit if there is no configurable children */
      if (0 == num_configurable_children) {
        /* Ensure that there should be no configuration bits in the parent block */
        VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());
        return;
      }

      for (size_t child_id = 0; child_id < num_configurable_children; ++child_id) {
        ModuleId child_module = configurable_children[child_id]; 
        size_t child_instance = module_manager.region_configurable_child_instances(parent_module, config_region)[child_id]; 

        if (parent_module == top_module) {
          tile_coord = module_manager.region_configurable_child_coordinates(parent_module, config_region)[child_id]; 
        }

        /* Get the instance name and ensure it is not empty */
        std::string instance_name = module_manager.instance_name(parent_module, child_module, child_instance);
         
        /* Find the child block that matches the instance name! */ 
        ConfigBlockId child_block = bitstream_manager.find_child_block(parent_block, instance_name); 
        /* We must have one valid block id! */
        VTR_ASSERT(true == bitstream_manager.valid_block_id(child_block));

        /* Reset the memory index for each children under the top-level module */
        if (parent_module == top_module) {
          cur_mem_index = 0;
        }

        /* Go recursively */
        rec_build_module_fabric_dependent_ql_memory_bank_regional_bitstream(bitstream_manager, child_block,
                                                                            module_manager, top_module, child_module,
                                                                            config_region,
                                                                            bl_addr_size, wl_addr_size,
                                                                            num_bls_per_tile, bl_start_index_per_tile,
                                                                            num_wls_per_tile, wl_start_index_per_tile,
                                                                            tile_coord,
                                                                            cur_mem_index,
                                                                            fabric_bitstream,
                                                                            fabric_bitstream_region);
      }
    } else {
      VTR_ASSERT(parent_module != top_module);
      /* For other modules:
       *   - Use configurable children directly
       *   - no need to exclude decoders as they are not there
       */
      std::vector<ModuleId> configurable_children = module_manager.configurable_children(parent_module);

      size_t num_configurable_children = configurable_children.size();

      /* Early exit if there is no configurable children */
      if (0 == num_configurable_children) {
        /* Ensure that there should be no configuration bits in the parent block */
        VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());
        return;
      }

      for (size_t child_id = 0; child_id < num_configurable_children; ++child_id) {
        ModuleId child_module = configurable_children[child_id]; 
        size_t child_instance = module_manager.configurable_child_instances(parent_module)[child_id]; 

        /* Get the instance name and ensure it is not empty */
        std::string instance_name = module_manager.instance_name(parent_module, child_module, child_instance);
         
        /* Find the child block that matches the instance name! */ 
        ConfigBlockId child_block = bitstream_manager.find_child_block(parent_block, instance_name); 
        /* We must have one valid block id! */
        VTR_ASSERT(true == bitstream_manager.valid_block_id(child_block));

        /* Go recursively */
        rec_build_module_fabric_dependent_ql_memory_bank_regional_bitstream(bitstream_manager, child_block,
                                                                            module_manager, top_module, child_module,
                                                                            config_region,
                                                                            bl_addr_size, wl_addr_size,
                                                                            num_bls_per_tile, bl_start_index_per_tile,
                                                                            num_wls_per_tile, wl_start_index_per_tile,
                                                                            tile_coord,
                                                                            cur_mem_index,
                                                                            fabric_bitstream,
                                                                            fabric_bitstream_region);
      }
    }
    /* Ensure that there should be no configuration bits in the parent block */
    VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());

    return;
  }

  /* Note that, reach here, it means that this is a leaf node. 
   * We add the configuration bits to the fabric_bitstream,
   * And then, we can return
   */
  for (const ConfigBitId& config_bit : bitstream_manager.block_bits(parent_block)) {
    FabricBitId fabric_bit = fabric_bitstream.add_bit(config_bit);
  
    /* Find BL address */
    size_t cur_bl_index = bl_start_index_per_tile.at(tile_coord.x()) + std::floor(cur_mem_index / num_bls_per_tile.at(tile_coord.x()));
    std::vector<char> bl_addr_bits_vec = itobin_charvec(cur_bl_index, bl_addr_size);

    /* Find WL address */
    size_t cur_wl_index = wl_start_index_per_tile.at(tile_coord.y()) + cur_mem_index % num_wls_per_tile.at(tile_coord.y());
    std::vector<char> wl_addr_bits_vec = itobin_charvec(cur_wl_index, wl_addr_size);

    /* Set BL address */
    fabric_bitstream.set_bit_bl_address(fabric_bit, bl_addr_bits_vec);

    /* Set WL address */
    fabric_bitstream.set_bit_wl_address(fabric_bit, wl_addr_bits_vec);
    
    /* Set data input */
    fabric_bitstream.set_bit_din(fabric_bit, bitstream_manager.bit_value(config_bit));

    /* Add the bit to the region */
    fabric_bitstream.add_bit_to_region(fabric_bitstream_region, fabric_bit);

    /* Increase the memory index */
    cur_mem_index++;
  }
}

/********************************************************************
 * Main function to build a fabric-dependent bitstream
 * by considering the configuration protocol types 
 *******************************************************************/
void build_module_fabric_dependent_bitstream_ql_memory_bank(const ConfigProtocol& config_protocol,
                                                            const CircuitLibrary& circuit_lib,
                                                            const BitstreamManager& bitstream_manager,
                                                            const ConfigBlockId& top_block,
                                                            const ModuleManager& module_manager,
                                                            const ModuleId& top_module,
                                                            FabricBitstream& fabric_bitstream) {
  /* Ensure we are in the correct type of configuration protocol*/
  VTR_ASSERT(config_protocol.type() == CONFIG_MEM_QL_MEMORY_BANK);

  /* Find global BL address port size */
  ModulePortId bl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port_info = module_manager.module_port(top_module, bl_addr_port);

  /* Find global WL address port size */
  ModulePortId wl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port_info = module_manager.module_port(top_module, wl_addr_port);

  /* Reserve bits before build-up */
  fabric_bitstream.set_use_address(true);
  fabric_bitstream.set_use_wl_address(true);
  fabric_bitstream.set_bl_address_length(bl_addr_port_info.get_width());
  fabric_bitstream.set_wl_address_length(wl_addr_port_info.get_width());
  fabric_bitstream.reserve_bits(bitstream_manager.num_bits());

  /* Build bitstreams by region */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    size_t cur_mem_index = 0;

    /* Find port information for local BL and WL decoder in this region */
    std::vector<ModuleId> configurable_children = module_manager.region_configurable_children(top_module, config_region);
    VTR_ASSERT(2 <= configurable_children.size()); 
    ModuleId bl_decoder_module = configurable_children[configurable_children.size() - 2];
    ModuleId wl_decoder_module = configurable_children[configurable_children.size() - 1];

    ModulePortId bl_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
    BasicPort bl_port_info = module_manager.module_port(bl_decoder_module, bl_port);

    ModulePortId wl_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
    BasicPort wl_port_info = module_manager.module_port(wl_decoder_module, wl_port);

    /* Build the bitstream for all the blocks in this region */
    FabricBitRegionId fabric_bitstream_region = fabric_bitstream.add_region();

    /************************************************************** 
     * Precompute the BLs and WLs distribution across the FPGA fabric
     * The distribution is a matrix which contains the starting index of BL/WL for each column or row
     */
    std::pair<int, int> child_x_range = compute_memory_bank_regional_configurable_child_x_range(module_manager, top_module, config_region);
    std::pair<int, int> child_y_range = compute_memory_bank_regional_configurable_child_y_range(module_manager, top_module, config_region);

    std::map<int, size_t> num_bls_per_tile = compute_memory_bank_regional_bitline_numbers_per_tile(module_manager, top_module,
                                                                                                   config_region,
                                                                                                   circuit_lib, config_protocol.memory_model());
    std::map<int, size_t> num_wls_per_tile = compute_memory_bank_regional_wordline_numbers_per_tile(module_manager, top_module,
                                                                                                    config_region,
                                                                                                    circuit_lib, config_protocol.memory_model());

    std::map<int, size_t> bl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_x_range, num_bls_per_tile);
    std::map<int, size_t> wl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_y_range, num_wls_per_tile);

    vtr::Point<int> temp_coord;
    rec_build_module_fabric_dependent_ql_memory_bank_regional_bitstream(bitstream_manager, top_block,
                                                                        module_manager, top_module, top_module, 
                                                                        config_region,
                                                                        bl_addr_port_info.get_width(),
                                                                        wl_addr_port_info.get_width(),
                                                                        num_bls_per_tile, bl_start_index_per_tile,
                                                                        num_wls_per_tile, wl_start_index_per_tile,
                                                                        temp_coord,
                                                                        cur_mem_index,
                                                                        fabric_bitstream,
                                                                        fabric_bitstream_region);
  }
}

} /* end namespace openfpga */
