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
                                                                         const ConfigProtocol& config_protocol,
                                                                         const CircuitLibrary& circuit_lib,
                                                                         const CircuitModelId& sram_model,
                                                                         const size_t& bl_addr_size,
                                                                         const size_t& wl_addr_size,
                                                                         size_t& num_bls_cur_tile,
                                                                         const std::map<int, size_t>& bl_start_index_per_tile,
                                                                         size_t& num_wls_cur_tile,
                                                                         const std::map<int, size_t>& wl_start_index_per_tile,
                                                                         vtr::Point<int>& tile_coord,
                                                                         std::map<vtr::Point<int>, size_t>& cur_mem_index,
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

        tile_coord = module_manager.region_configurable_child_coordinates(parent_module, config_region)[child_id]; 
        num_bls_cur_tile = find_module_ql_memory_bank_num_blwls(module_manager, child_module, circuit_lib, sram_model, CONFIG_MEM_QL_MEMORY_BANK, CIRCUIT_MODEL_PORT_BL);
        num_wls_cur_tile = find_module_ql_memory_bank_num_blwls(module_manager, child_module, circuit_lib, sram_model, CONFIG_MEM_QL_MEMORY_BANK, CIRCUIT_MODEL_PORT_WL);

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
                                                                            config_protocol,
                                                                            circuit_lib, sram_model,
                                                                            bl_addr_size, wl_addr_size,
                                                                            num_bls_cur_tile, bl_start_index_per_tile,
                                                                            num_wls_cur_tile, wl_start_index_per_tile,
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
                                                                            config_protocol,
                                                                            circuit_lib, sram_model,
                                                                            bl_addr_size, wl_addr_size,
                                                                            num_bls_cur_tile, bl_start_index_per_tile,
                                                                            num_wls_cur_tile, wl_start_index_per_tile,
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
  
    /* The BL address to be decoded depends on the protocol
     * - flatten BLs: use 1-hot decoding
     * - BL decoders: fully encoded
     * - Shift register: use 1-hot decoding
     */
    size_t cur_bl_index = bl_start_index_per_tile.at(tile_coord.x()) + cur_mem_index[tile_coord] % num_bls_cur_tile;
    std::vector<char> bl_addr_bits_vec;
    if (BLWL_PROTOCOL_DECODER == config_protocol.bl_protocol_type()) {
      bl_addr_bits_vec = itobin_charvec(cur_bl_index, bl_addr_size);
    } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.bl_protocol_type()
            || BLWL_PROTOCOL_FLATTEN == config_protocol.bl_protocol_type()) {
      bl_addr_bits_vec = ito1hot_charvec(cur_bl_index, bl_addr_size);
    }

    /* Find WL address */
    size_t cur_wl_index = wl_start_index_per_tile.at(tile_coord.y()) + std::floor(cur_mem_index[tile_coord] / num_bls_cur_tile);
    std::vector<char> wl_addr_bits_vec;
    if (BLWL_PROTOCOL_DECODER == config_protocol.wl_protocol_type()) {
      wl_addr_bits_vec = itobin_charvec(cur_wl_index, wl_addr_size);
    } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.wl_protocol_type()
            || BLWL_PROTOCOL_FLATTEN == config_protocol.wl_protocol_type()) {
      wl_addr_bits_vec = ito1hot_charvec(cur_wl_index, wl_addr_size);
    }

    /* Set BL address */
    fabric_bitstream.set_bit_bl_address(fabric_bit, bl_addr_bits_vec, true);

    /* Set WL address */
    fabric_bitstream.set_bit_wl_address(fabric_bit, wl_addr_bits_vec, true);
    
    /* Set data input */
    fabric_bitstream.set_bit_din(fabric_bit, bitstream_manager.bit_value(config_bit));

    /* Add the bit to the region */
    fabric_bitstream.add_bit_to_region(fabric_bitstream_region, fabric_bit);

    /* Increase the memory index */
    cur_mem_index[tile_coord]++;
  }
}

/********************************************************************
 * Main function to build a fabric-dependent bitstream
 * by considering the QuickLogic memory banks
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

  /* For different BL control protocol, the address ports are different 
   * - flatten BLs: the address port should be raw BL ports at top-level module. 
   *                Due to each configuration region has separated BLs, the address port should be the one with largest size
   * - BL decoders: the address port should be the BL address port at top-level module
   * - Shift register: the address port size will be calculated by the total number of unique BLs per child module in each configuration region 
   *                   Due to each configuration region has separated BLs, the address port should be the one with largest size
   */
  ModulePortId bl_addr_port;
  BasicPort bl_addr_port_info;
  if (BLWL_PROTOCOL_DECODER == config_protocol.bl_protocol_type()) {
    bl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_BL_ADDRESS_PORT_NAME));
    bl_addr_port_info = module_manager.module_port(top_module, bl_addr_port);
  } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.bl_protocol_type()) {
    for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
      ModulePortId temp_bl_addr_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(std::string(MEMORY_BL_PORT_NAME), config_region));
      BasicPort temp_bl_addr_port_info = module_manager.module_port(top_module, temp_bl_addr_port);
      if (!bl_addr_port || (temp_bl_addr_port_info.get_width() > bl_addr_port_info.get_width())) {
        bl_addr_port = temp_bl_addr_port;
        bl_addr_port_info = temp_bl_addr_port_info;
      }
    }
  } else {
    /* TODO */
    VTR_ASSERT(BLWL_PROTOCOL_SHIFT_REGISTER == config_protocol.bl_protocol_type());
  }

  /* For different WL control protocol, the address ports are different 
   * - flatten WLs: the address port should be raw WL ports at top-level module. 
   *                Due to each configuration region has separated WLs, the address port should be the one with largest size
   * - WL decoders: the address port should be the WL address port at top-level module
   * - Shift register: the address port size will be calculated by the total number of unique WLs per child module in each configuration region 
   *                   Due to each configuration region has separated WLs, the address port should be the one with largest size
   */
  ModulePortId wl_addr_port;
  BasicPort wl_addr_port_info;
  if (BLWL_PROTOCOL_DECODER == config_protocol.wl_protocol_type()) {
    wl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_WL_ADDRESS_PORT_NAME));
    wl_addr_port_info = module_manager.module_port(top_module, wl_addr_port);
  } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.wl_protocol_type()) {
    for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
      ModulePortId temp_wl_addr_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(std::string(MEMORY_WL_PORT_NAME), config_region));
      BasicPort temp_wl_addr_port_info = module_manager.module_port(top_module, temp_wl_addr_port);
      if (!wl_addr_port || (temp_wl_addr_port_info.get_width() > wl_addr_port_info.get_width())) {
        wl_addr_port = temp_wl_addr_port;
        wl_addr_port_info = temp_wl_addr_port_info;
      }
    }
  } else {
    /* TODO */
    VTR_ASSERT(BLWL_PROTOCOL_SHIFT_REGISTER == config_protocol.wl_protocol_type());
  }

  /* Reserve bits before build-up */
  fabric_bitstream.set_use_address(true);
  fabric_bitstream.set_use_wl_address(true);
  fabric_bitstream.set_bl_address_length(bl_addr_port_info.get_width());
  fabric_bitstream.set_wl_address_length(wl_addr_port_info.get_width());
  fabric_bitstream.reserve_bits(bitstream_manager.num_bits());

  /* Build bitstreams by region */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    /* Find port information for local BL and WL decoder in this region */
    std::vector<ModuleId> configurable_children = module_manager.region_configurable_children(top_module, config_region);
    VTR_ASSERT(2 <= configurable_children.size()); 

    /* Build the bitstream for all the blocks in this region */
    FabricBitRegionId fabric_bitstream_region = fabric_bitstream.add_region();
    
    /* Find the BL/WL port (different region may have different sizes of BL/WLs) */
    ModulePortId cur_bl_addr_port;
    if (BLWL_PROTOCOL_DECODER == config_protocol.bl_protocol_type()) {
      cur_bl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_BL_ADDRESS_PORT_NAME));
    } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.bl_protocol_type()) {
      cur_bl_addr_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(std::string(MEMORY_BL_PORT_NAME), config_region));
    } else {
      /* TODO */
      VTR_ASSERT(BLWL_PROTOCOL_SHIFT_REGISTER == config_protocol.bl_protocol_type());
    }

    ModulePortId cur_wl_addr_port;
    if (BLWL_PROTOCOL_DECODER == config_protocol.wl_protocol_type()) {
      cur_wl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_WL_ADDRESS_PORT_NAME));
    } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.wl_protocol_type()) {
      cur_wl_addr_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(std::string(MEMORY_WL_PORT_NAME), config_region));
    } else {
      /* TODO */
      VTR_ASSERT(BLWL_PROTOCOL_SHIFT_REGISTER == config_protocol.wl_protocol_type());
    }
    BasicPort cur_wl_addr_port_info = module_manager.module_port(top_module, cur_wl_addr_port);

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
    std::map<vtr::Point<int>, size_t> cur_mem_index;
    size_t temp_num_bls_cur_tile = 0;
    size_t temp_num_wls_cur_tile = 0;

    rec_build_module_fabric_dependent_ql_memory_bank_regional_bitstream(bitstream_manager, top_block,
                                                                        module_manager, top_module, top_module, 
                                                                        config_region,
                                                                        config_protocol,
                                                                        circuit_lib, config_protocol.memory_model(),
                                                                        cur_bl_addr_port_info.get_width(),
                                                                        cur_wl_addr_port_info.get_width(),
                                                                        temp_num_bls_cur_tile, bl_start_index_per_tile,
                                                                        temp_num_wls_cur_tile, wl_start_index_per_tile,
                                                                        temp_coord,
                                                                        cur_mem_index,
                                                                        fabric_bitstream,
                                                                        fabric_bitstream_region);
  }
}

} /* end namespace openfpga */
