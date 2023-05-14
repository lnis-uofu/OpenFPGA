/********************************************************************
 * This file includes functions to build fabric dependent bitstream
 *******************************************************************/
#include <algorithm>
#include <cmath>
#include <string>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "bitstream_manager_utils.h"
#include "build_fabric_bitstream.h"
#include "build_fabric_bitstream_memory_bank.h"
#include "decoder_library_utils.h"
#include "openfpga_decode.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to build a bitstream for configuration chain-like protocol
 * It will walk through all the configurable children under a module
 * in a recursive way, following a Depth-First Search (DFS) strategy
 * For each configuration child, we use its instance name as a key to spot the
 * configuration bits in bitstream manager.
 * Note that it is guarentee that the instance name in module manager is
 * consistent with the block names in bitstream manager
 * We use this link to reorganize the bitstream in the sequence of memories as
 *we stored in the configurable_children() and configurable_child_instances() of
 *each module of module manager
 *******************************************************************/
static void rec_build_module_fabric_dependent_chain_bitstream(
  const BitstreamManager& bitstream_manager, const ConfigBlockId& parent_block,
  const ModuleManager& module_manager, const ModuleId& top_module,
  const ModuleId& parent_module, const ConfigRegionId& config_region,
  FabricBitstream& fabric_bitstream,
  const FabricBitRegionId& fabric_bitstream_region) {
  /* Depth-first search: if we have any children in the parent_block,
   * we dive to the next level first!
   */
  if (0 < bitstream_manager.block_children(parent_block).size()) {
    if (parent_module == top_module) {
      for (size_t child_id = 0; child_id < module_manager
                                             .region_configurable_children(
                                               parent_module, config_region)
                                             .size();
           ++child_id) {
        ModuleId child_module = module_manager.region_configurable_children(
          parent_module, config_region)[child_id];
        size_t child_instance =
          module_manager.region_configurable_child_instances(
            parent_module, config_region)[child_id];
        /* Get the instance name and ensure it is not empty */
        std::string instance_name = module_manager.instance_name(
          parent_module, child_module, child_instance);

        /* Find the child block that matches the instance name! */
        ConfigBlockId child_block =
          bitstream_manager.find_child_block(parent_block, instance_name);
        /* We must have one valid block id! */
        VTR_ASSERT(true == bitstream_manager.valid_block_id(child_block));

        /* Go recursively */
        rec_build_module_fabric_dependent_chain_bitstream(
          bitstream_manager, child_block, module_manager, top_module,
          child_module, config_region, fabric_bitstream,
          fabric_bitstream_region);
      }
    } else {
      for (size_t child_id = 0;
           child_id <
           module_manager.configurable_children(parent_module).size();
           ++child_id) {
        ModuleId child_module =
          module_manager.configurable_children(parent_module)[child_id];
        size_t child_instance =
          module_manager.configurable_child_instances(parent_module)[child_id];
        /* Get the instance name and ensure it is not empty */
        std::string instance_name = module_manager.instance_name(
          parent_module, child_module, child_instance);

        /* Find the child block that matches the instance name! */
        ConfigBlockId child_block =
          bitstream_manager.find_child_block(parent_block, instance_name);
        /* We must have one valid block id! */
        VTR_ASSERT(true == bitstream_manager.valid_block_id(child_block));

        /* Go recursively */
        rec_build_module_fabric_dependent_chain_bitstream(
          bitstream_manager, child_block, module_manager, top_module,
          child_module, config_region, fabric_bitstream,
          fabric_bitstream_region);
      }
    }
    /* Ensure that there should be no configuration bits in the parent block */
    VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());
  }

  /* Note that, reach here, it means that this is a leaf node.
   * We add the configuration bits to the fabric_bitstream,
   * And then, we can return
   */
  for (const ConfigBitId& config_bit :
       bitstream_manager.block_bits(parent_block)) {
    FabricBitId fabric_bit = fabric_bitstream.add_bit(config_bit);
    fabric_bitstream.add_bit_to_region(fabric_bitstream_region, fabric_bit);
  }
}

/********************************************************************
 * This function aims to build a bitstream for memory-bank protocol
 * It will walk through all the configurable children under a module
 * in a recursive way, following a Depth-First Search (DFS) strategy
 * For each configuration child, we use its instance name as a key to spot the
 * configuration bits in bitstream manager.
 * Note that it is guarentee that the instance name in module manager is
 * consistent with the block names in bitstream manager
 * We use this link to reorganize the bitstream in the sequence of memories as
 *we stored in the configurable_children() and configurable_child_instances() of
 *each module of module manager
 *
 * In such configuration organization, each memory cell has an unique index.
 * Using this index, we can infer the address codes for both BL and WL decoders.
 * Note that, we must get the number of BLs and WLs before using this function!
 *******************************************************************/
static void rec_build_module_fabric_dependent_memory_bank_bitstream(
  const BitstreamManager& bitstream_manager, const ConfigBlockId& parent_block,
  const ModuleManager& module_manager, const ModuleId& top_module,
  const ModuleId& parent_module, const ConfigRegionId& config_region,
  const size_t& bl_addr_size, const size_t& wl_addr_size, const size_t& num_bls,
  const size_t& num_wls, size_t& cur_mem_index,
  FabricBitstream& fabric_bitstream,
  const FabricBitRegionId& fabric_bitstream_region) {
  /* Depth-first search: if we have any children in the parent_block,
   * we dive to the next level first!
   */
  if (0 < bitstream_manager.block_children(parent_block).size()) {
    /* For top module:
     *   - Use regional configurable children
     *   - we will skip the two decoders at the end of the configurable children
     * list
     */
    if (parent_module == top_module) {
      std::vector<ModuleId> configurable_children =
        module_manager.region_configurable_children(parent_module,
                                                    config_region);

      VTR_ASSERT(2 <= configurable_children.size());
      size_t num_configurable_children = configurable_children.size() - 2;

      /* Early exit if there is no configurable children */
      if (0 == num_configurable_children) {
        /* Ensure that there should be no configuration bits in the parent block
         */
        VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());
        return;
      }

      for (size_t child_id = 0; child_id < num_configurable_children;
           ++child_id) {
        ModuleId child_module = configurable_children[child_id];
        size_t child_instance =
          module_manager.region_configurable_child_instances(
            parent_module, config_region)[child_id];

        /* Get the instance name and ensure it is not empty */
        std::string instance_name = module_manager.instance_name(
          parent_module, child_module, child_instance);

        /* Find the child block that matches the instance name! */
        ConfigBlockId child_block =
          bitstream_manager.find_child_block(parent_block, instance_name);
        /* We must have one valid block id! */
        VTR_ASSERT(true == bitstream_manager.valid_block_id(child_block));

        /* Go recursively */
        rec_build_module_fabric_dependent_memory_bank_bitstream(
          bitstream_manager, child_block, module_manager, top_module,
          child_module, config_region, bl_addr_size, wl_addr_size, num_bls,
          num_wls, cur_mem_index, fabric_bitstream, fabric_bitstream_region);
      }
    } else {
      VTR_ASSERT(parent_module != top_module);
      /* For other modules:
       *   - Use configurable children directly
       *   - no need to exclude decoders as they are not there
       */
      std::vector<ModuleId> configurable_children =
        module_manager.configurable_children(parent_module);

      size_t num_configurable_children = configurable_children.size();

      /* Early exit if there is no configurable children */
      if (0 == num_configurable_children) {
        /* Ensure that there should be no configuration bits in the parent block
         */
        VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());
        return;
      }

      for (size_t child_id = 0; child_id < num_configurable_children;
           ++child_id) {
        ModuleId child_module = configurable_children[child_id];
        size_t child_instance =
          module_manager.configurable_child_instances(parent_module)[child_id];

        /* Get the instance name and ensure it is not empty */
        std::string instance_name = module_manager.instance_name(
          parent_module, child_module, child_instance);

        /* Find the child block that matches the instance name! */
        ConfigBlockId child_block =
          bitstream_manager.find_child_block(parent_block, instance_name);
        /* We must have one valid block id! */
        VTR_ASSERT(true == bitstream_manager.valid_block_id(child_block));

        /* Go recursively */
        rec_build_module_fabric_dependent_memory_bank_bitstream(
          bitstream_manager, child_block, module_manager, top_module,
          child_module, config_region, bl_addr_size, wl_addr_size, num_bls,
          num_wls, cur_mem_index, fabric_bitstream, fabric_bitstream_region);
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
  for (const ConfigBitId& config_bit :
       bitstream_manager.block_bits(parent_block)) {
    FabricBitId fabric_bit = fabric_bitstream.add_bit(config_bit);

    /* Find BL address */
    size_t cur_bl_index = std::floor(cur_mem_index / num_bls);
    std::vector<char> bl_addr_bits_vec =
      itobin_charvec(cur_bl_index, bl_addr_size);

    /* Find WL address */
    size_t cur_wl_index = cur_mem_index % num_wls;
    std::vector<char> wl_addr_bits_vec =
      itobin_charvec(cur_wl_index, wl_addr_size);

    /* Set BL address */
    fabric_bitstream.set_bit_bl_address(fabric_bit, bl_addr_bits_vec);

    /* Set WL address */
    fabric_bitstream.set_bit_wl_address(fabric_bit, wl_addr_bits_vec);

    /* Set data input */
    fabric_bitstream.set_bit_din(fabric_bit,
                                 bitstream_manager.bit_value(config_bit));

    /* Add the bit to the region */
    fabric_bitstream.add_bit_to_region(fabric_bitstream_region, fabric_bit);

    /* Increase the memory index */
    cur_mem_index++;
  }
}

/********************************************************************
 * This function aims to build a bitstream for frame-based configuration
 *protocol It will walk through all the configurable children under a module in
 *a recursive way, following a Depth-First Search (DFS) strategy For each
 *configuration child, we use its instance name as a key to spot the
 * configuration bits in bitstream manager.
 * Note that it is guarenteed that the instance name in module manager is
 * consistent with the block names in bitstream manager
 * We use this link to reorganize the bitstream in the sequence of memories as
 *we stored in the configurable_children() and configurable_child_instances() of
 *each module of module manager
 *
 * For each configuration bits, we will infer its address based on
 *  - the child index in the configurable children list of current module
 *  - the child index of all the parent modules in their configurable children
 *list until the top in the hierarchy
 *
 * The address will be organized as follows:
 *  <Address_in_top> ... <Address_in_parent_module>
 * The address will be decoded to a binary format
 *
 * For each configuration bit, the data_in for the frame-based decoders will be
 * the same as the configuration bit in bitstream manager.
 *******************************************************************/
static void rec_build_module_fabric_dependent_frame_bitstream(
  const BitstreamManager& bitstream_manager,
  const std::vector<ConfigBlockId>& parent_blocks,
  const ModuleManager& module_manager, const ModuleId& top_module,
  const ConfigRegionId& config_region,
  const std::vector<ModuleId>& parent_modules,
  const std::vector<char>& addr_code, const char& bitstream_dont_care_char,
  FabricBitstream& fabric_bitstream,
  FabricBitRegionId& fabric_bitstream_region) {
  /* Depth-first search: if we have any children in the parent_block,
   * we dive to the next level first!
   */
  if (0 < bitstream_manager.block_children(parent_blocks.back()).size()) {
    const ConfigBlockId& parent_block = parent_blocks.back();
    const ModuleId& parent_module = parent_modules.back();

    std::vector<ModuleId> configurable_children;
    std::vector<size_t> configurable_child_instances;
    if (top_module == parent_module) {
      configurable_children = module_manager.region_configurable_children(
        parent_module, config_region);
      configurable_child_instances =
        module_manager.region_configurable_child_instances(parent_module,
                                                           config_region);
    } else {
      VTR_ASSERT(top_module != parent_module);
      configurable_children =
        module_manager.configurable_children(parent_module);
      configurable_child_instances =
        module_manager.configurable_child_instances(parent_module);
    }

    size_t num_configurable_children = configurable_children.size();

    size_t max_child_addr_code_size = 0;
    bool add_addr_code = true;
    ModuleId decoder_module = ModuleId::INVALID();

    /* Early exit if there is no configurable children */
    if (0 == num_configurable_children) {
      /* Ensure that there should be no configuration bits in the parent block
       */
      VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());
      return;
    }

    /* For only 1 configurable child,
     * there is no frame decoder here, we can pass on addr code directly
     */
    if (1 == num_configurable_children) {
      add_addr_code = false;
    } else {
      /* For more than 2 children, there is a decoder in the tail of the list
       * We will not decode that, but will access the address size from that
       * module So, we reduce the number of children by 1
       */
      VTR_ASSERT(2 < num_configurable_children);
      num_configurable_children--;
      decoder_module = configurable_children.back();

      /* The max address code size is the max address code size of all the
       * configurable children in all the regions
       */
      for (const ModuleId& child_module :
           module_manager.configurable_children(parent_module)) {
        /* Bypass any decoder module (which no configurable children */
        if (module_manager.configurable_children(child_module).empty()) {
          continue;
        }
        const ModulePortId& child_addr_port_id =
          module_manager.find_module_port(
            child_module, std::string(DECODER_ADDRESS_PORT_NAME));
        const BasicPort& child_addr_port =
          module_manager.module_port(child_module, child_addr_port_id);
        max_child_addr_code_size =
          std::max(child_addr_port.get_width(), max_child_addr_code_size);
      }
    }

    for (size_t child_id = 0; child_id < num_configurable_children;
         ++child_id) {
      ModuleId child_module = configurable_children[child_id];
      size_t child_instance = configurable_child_instances[child_id];
      /* Get the instance name and ensure it is not empty */
      std::string instance_name = module_manager.instance_name(
        parent_module, child_module, child_instance);

      /* Find the child block that matches the instance name! */
      ConfigBlockId child_block =
        bitstream_manager.find_child_block(parent_block, instance_name);
      /* We must have one valid block id! */
      VTR_ASSERT(true == bitstream_manager.valid_block_id(child_block));

      /* Pass on the list of blocks, modules and address lists */
      std::vector<ConfigBlockId> child_blocks = parent_blocks;
      child_blocks.push_back(child_block);

      std::vector<ModuleId> child_modules = parent_modules;
      child_modules.push_back(child_module);

      /* Set address, apply binary conversion from the first to the last element
       * in the address list */
      std::vector<char> child_addr_code = addr_code;

      if (true == add_addr_code) {
        /* Find the address port from the decoder module */
        const ModulePortId& decoder_addr_port_id =
          module_manager.find_module_port(
            decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
        const BasicPort& decoder_addr_port =
          module_manager.module_port(decoder_module, decoder_addr_port_id);
        std::vector<char> addr_bits_vec =
          itobin_charvec(child_id, decoder_addr_port.get_width());

        /* For top-level module, the child address should be added to the tail
         * For other modules, the child address should be added to the head
         */
        if (top_module == parent_module) {
          child_addr_code.insert(child_addr_code.end(), addr_bits_vec.begin(),
                                 addr_bits_vec.end());
        } else {
          VTR_ASSERT_SAFE(top_module != parent_module);
          child_addr_code.insert(child_addr_code.begin(), addr_bits_vec.begin(),
                                 addr_bits_vec.end());
        }

        /* Note that the address port size of the child module may be smaller
         * than the maximum of other child modules at this level. We will add
         * dummy '0's to the head of addr_bit_vec.
         *
         * For example:
         *  Decoder is the decoder to access all the child modules
         *  whose address is decoded by the addr_bits_vec
         *  The child modules may use part of the address lines,
         *  we should add dummy '0' to fill the gap
         *
         *  Addr_code for child[0]: '000' + addr_bits_vec
         *  Addr_code for child[1]: '00'  + addr_bits_vec
         *  Addr_code for child[2]: '0' + addr_bits_vec
         *
         *                   Addr[6:8]
         *                     |
         *                     v
         *  +-------------------------------------------+
         *  |            Decoder Module                 |
         *  +-------------------------------------------+
         *
         *     Addr[0:2]       Addr[0:3]        Addr[0:4]
         *        |                |               |
         *        v                v               v
         * +-----------+  +-------------+  +------------+
         * | Child[0]  |  |  Child[1]   |  |  Child[2]  |
         * +-----------+  +-------------+  +------------+
         *
         * Child[2] has the maximum address lines among the children
         *
         */
        const ModulePortId& child_addr_port_id =
          module_manager.find_module_port(
            child_module, std::string(DECODER_ADDRESS_PORT_NAME));
        const BasicPort& child_addr_port =
          module_manager.module_port(child_module, child_addr_port_id);
        if (0 < max_child_addr_code_size - child_addr_port.get_width()) {
          /* Deposit don't care state for the dummy bits */
          std::vector<char> dummy_codes(
            max_child_addr_code_size - child_addr_port.get_width(),
            bitstream_dont_care_char);
          child_addr_code.insert(child_addr_code.begin(), dummy_codes.begin(),
                                 dummy_codes.end());
        }
      }

      /* Go recursively */
      rec_build_module_fabric_dependent_frame_bitstream(
        bitstream_manager, child_blocks, module_manager, top_module,
        config_region, child_modules, child_addr_code, bitstream_dont_care_char,
        fabric_bitstream, fabric_bitstream_region);
    }
    /* Ensure that there should be no configuration bits in the parent block */
    VTR_ASSERT(0 == bitstream_manager.block_bits(parent_block).size());

    return;
  }

  /* Note that, reach here, it means that this is a leaf node.
   * A leaf node (a memory module) always has a decoder inside
   * which is the last of configurable children.
   * We will find the address bit and add it to addr_code
   * Then we can add the configuration bits to the fabric_bitstream.
   */
  std::vector<ModuleId> configurable_children;
  if (top_module == parent_modules.back()) {
    configurable_children = module_manager.region_configurable_children(
      parent_modules.back(), config_region);
  } else {
    VTR_ASSERT(top_module != parent_modules.back());
    configurable_children =
      module_manager.configurable_children(parent_modules.back());
  }

  ModuleId decoder_module = configurable_children.back();
  /* Find the address port from the decoder module */
  const ModulePortId& decoder_addr_port_id = module_manager.find_module_port(
    decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
  const BasicPort& decoder_addr_port =
    module_manager.module_port(decoder_module, decoder_addr_port_id);

  for (size_t ibit = 0;
       ibit < bitstream_manager.block_bits(parent_blocks.back()).size();
       ++ibit) {
    ConfigBitId config_bit =
      bitstream_manager.block_bits(parent_blocks.back())[ibit];
    std::vector<char> addr_bits_vec =
      itobin_charvec(ibit, decoder_addr_port.get_width());

    std::vector<char> child_addr_code = addr_code;

    child_addr_code.insert(child_addr_code.begin(), addr_bits_vec.begin(),
                           addr_bits_vec.end());

    const FabricBitId& fabric_bit = fabric_bitstream.add_bit(config_bit);

    /* Set address */
    fabric_bitstream.set_bit_address(fabric_bit, child_addr_code);

    /* Set data input */
    fabric_bitstream.set_bit_din(fabric_bit,
                                 bitstream_manager.bit_value(config_bit));

    /* Add the bit to the region */
    fabric_bitstream.add_bit_to_region(fabric_bitstream_region, fabric_bit);
  }
}

/********************************************************************
 * Main function to build a fabric-dependent bitstream
 * by considering the configuration protocol types
 *******************************************************************/
static void build_module_fabric_dependent_bitstream(
  const ConfigProtocol& config_protocol, const CircuitLibrary& circuit_lib,
  const BitstreamManager& bitstream_manager, const ConfigBlockId& top_block,
  const ModuleManager& module_manager, const ModuleId& top_module,
  FabricBitstream& fabric_bitstream) {
  switch (config_protocol.type()) {
    case CONFIG_MEM_STANDALONE: {
      /* Reserve bits before build-up */
      fabric_bitstream.reserve_bits(bitstream_manager.num_bits());

      for (const ConfigRegionId& config_region :
           module_manager.regions(top_module)) {
        FabricBitRegionId fabric_bitstream_region =
          fabric_bitstream.add_region();
        rec_build_module_fabric_dependent_chain_bitstream(
          bitstream_manager, top_block, module_manager, top_module, top_module,
          config_region, fabric_bitstream, fabric_bitstream_region);
      }

      break;
    }
    case CONFIG_MEM_SCAN_CHAIN: {
      /* Reserve bits before build-up */
      fabric_bitstream.reserve_bits(bitstream_manager.num_bits());

      for (const ConfigRegionId& config_region :
           module_manager.regions(top_module)) {
        FabricBitRegionId fabric_bitstream_region =
          fabric_bitstream.add_region();
        rec_build_module_fabric_dependent_chain_bitstream(
          bitstream_manager, top_block, module_manager, top_module, top_module,
          config_region, fabric_bitstream, fabric_bitstream_region);
        fabric_bitstream.reverse_region_bits(fabric_bitstream_region);
      }
      break;
    }
    case CONFIG_MEM_MEMORY_BANK: {
      /* Find global BL address port size */
      ModulePortId bl_addr_port = module_manager.find_module_port(
        top_module, std::string(DECODER_BL_ADDRESS_PORT_NAME));
      BasicPort bl_addr_port_info =
        module_manager.module_port(top_module, bl_addr_port);

      /* Find global WL address port size */
      ModulePortId wl_addr_port = module_manager.find_module_port(
        top_module, std::string(DECODER_WL_ADDRESS_PORT_NAME));
      BasicPort wl_addr_port_info =
        module_manager.module_port(top_module, wl_addr_port);

      /* Reserve bits before build-up */
      fabric_bitstream.set_use_address(true);
      fabric_bitstream.set_use_wl_address(true);
      fabric_bitstream.set_bl_address_length(bl_addr_port_info.get_width());
      fabric_bitstream.set_wl_address_length(wl_addr_port_info.get_width());
      fabric_bitstream.reserve_bits(bitstream_manager.num_bits());

      /* Build bitstreams by region */
      for (const ConfigRegionId& config_region :
           module_manager.regions(top_module)) {
        size_t cur_mem_index = 0;

        /* Find port information for local BL and WL decoder in this region */
        std::vector<ModuleId> configurable_children =
          module_manager.region_configurable_children(top_module,
                                                      config_region);
        VTR_ASSERT(2 <= configurable_children.size());
        ModuleId bl_decoder_module =
          configurable_children[configurable_children.size() - 2];
        ModuleId wl_decoder_module =
          configurable_children[configurable_children.size() - 1];

        ModulePortId bl_port = module_manager.find_module_port(
          bl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
        BasicPort bl_port_info =
          module_manager.module_port(bl_decoder_module, bl_port);

        ModulePortId wl_port = module_manager.find_module_port(
          wl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
        BasicPort wl_port_info =
          module_manager.module_port(wl_decoder_module, wl_port);

        /* Build the bitstream for all the blocks in this region */
        FabricBitRegionId fabric_bitstream_region =
          fabric_bitstream.add_region();
        rec_build_module_fabric_dependent_memory_bank_bitstream(
          bitstream_manager, top_block, module_manager, top_module, top_module,
          config_region, bl_addr_port_info.get_width(),
          wl_addr_port_info.get_width(), bl_port_info.get_width(),
          wl_port_info.get_width(), cur_mem_index, fabric_bitstream,
          fabric_bitstream_region);
      }
      break;
    }
    case CONFIG_MEM_QL_MEMORY_BANK: {
      build_module_fabric_dependent_bitstream_ql_memory_bank(
        config_protocol, circuit_lib, bitstream_manager, top_block,
        module_manager, top_module, fabric_bitstream);
      break;
    }
    case CONFIG_MEM_FRAME_BASED: {
      /* Find address port size */
      ModulePortId addr_port = module_manager.find_module_port(
        top_module, std::string(DECODER_ADDRESS_PORT_NAME));
      BasicPort addr_port_info =
        module_manager.module_port(top_module, addr_port);

      /* Reserve bits before build-up */
      fabric_bitstream.set_use_address(true);
      fabric_bitstream.reserve_bits(bitstream_manager.num_bits());
      fabric_bitstream.set_address_length(addr_port_info.get_width());

      /* Avoid use don't care if there is only a region */
      char bitstream_dont_care_char = DONT_CARE_CHAR;
      if (1 == module_manager.regions(top_module).size()) {
        bitstream_dont_care_char = '0';
      }

      /* Find the maximum decoder address among all the configurable regions */
      size_t max_decoder_addr_size = 0;
      for (const ConfigRegionId& config_region :
           module_manager.regions(top_module)) {
        std::vector<ModuleId> configurable_children =
          module_manager.region_configurable_children(top_module,
                                                      config_region);
        /* Bypass the regions that have no decoders */
        if ((0 == configurable_children.size()) ||
            (1 == configurable_children.size())) {
          continue;
        }
        ModuleId decoder_module = configurable_children.back();
        ModulePortId decoder_addr_port_id = module_manager.find_module_port(
          decoder_module, DECODER_ADDRESS_PORT_NAME);
        BasicPort decoder_addr_port =
          module_manager.module_port(decoder_module, decoder_addr_port_id);
        max_decoder_addr_size =
          std::max(max_decoder_addr_size, decoder_addr_port.get_width());
      }

      for (const ConfigRegionId& config_region :
           module_manager.regions(top_module)) {
        std::vector<ModuleId> configurable_children =
          module_manager.region_configurable_children(top_module,
                                                      config_region);

        /* Bypass non-configurable regions */
        if (0 == configurable_children.size()) {
          continue;
        }

        /* Find the idle address bit which should be added to the head of the
         * address bit This depends on the number of address bits required by
         * this region For example: Top-level address is addr[0:4] There are 4
         * decoders in the top-level module, whose address sizes are decoder A:
         * addr[0:4] decoder B: addr[0:3] decoder C: addr[0:2] decoder D:
         * addr[0:3] For decoder A, the address fit well For decoder B, an idle
         * bit should be added '0' + addr[0:3] For decoder C, two idle bits
         * should be added '00' + addr[0:2] For decoder D, an idle bit should be
         * added '0' + addr[0:3]
         */
        ModuleId decoder_module = configurable_children.back();
        ModulePortId decoder_addr_port_id = module_manager.find_module_port(
          decoder_module, DECODER_ADDRESS_PORT_NAME);
        BasicPort decoder_addr_port =
          module_manager.module_port(decoder_module, decoder_addr_port_id);
        VTR_ASSERT(max_decoder_addr_size >= decoder_addr_port.get_width());
        std::vector<char> idle_addr_bits(
          max_decoder_addr_size - decoder_addr_port.get_width(),
          bitstream_dont_care_char);

        FabricBitRegionId fabric_bitstream_region =
          fabric_bitstream.add_region();
        rec_build_module_fabric_dependent_frame_bitstream(
          bitstream_manager, std::vector<ConfigBlockId>(1, top_block),
          module_manager, top_module, config_region,
          std::vector<ModuleId>(1, top_module), idle_addr_bits,
          bitstream_dont_care_char, fabric_bitstream, fabric_bitstream_region);
      }
      break;
    }
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid SRAM organization.\n");
      exit(1);
  }

  /* Time-consuming sanity check: Uncomment these codes only for debugging!!!
   * Check which configuration bits are not touched
   */
  /*
  for (const ConfigBitId& config_bit : bitstream_manager.bits()) {
    std::vector<ConfigBitId>::iterator it = std::find(fabric_bitstream.begin(),
  fabric_bitstream.end(), config_bit); if (it == fabric_bitstream.end()) {
      std::vector<ConfigBlockId> block_hierarchy =
  find_bitstream_manager_block_hierarchy(bitstream_manager,
  bitstream_manager.bit_parent_block(config_bit)); std::string
  block_hierarchy_name; for (const ConfigBlockId& temp_block : block_hierarchy)
  { block_hierarchy_name += std::string("/") +
  bitstream_manager.block_name(temp_block);
      }
      vpr_printf(TIO_MESSAGE_INFO,
                 "bit (parent_block = %s) is not touched!\n",
                 block_hierarchy_name.c_str());
    }
  }
   */

  /* Ensure our fabric bitstream is in the same size as device bistream */
  VTR_ASSERT(bitstream_manager.num_bits() == fabric_bitstream.num_bits());
}

/********************************************************************
 * A top-level function re-organizes the bitstream for a specific
 * FPGA fabric, where configuration bits are organized in the sequence
 * that can be directly loaded to the FPGA configuration protocol.
 * Support:
 * 1. Configuration chain
 * 2. Memory decoders
 * This function does NOT modify the bitstream database
 * Instead, it builds a vector of ids for configuration bits in bitstream
 *manager
 *
 * This function can be called ONLY after the function build_device_bitstream()
 * Note that this function does NOT decode bitstreams from circuit
 *implementation It was done in the function build_device_bitstream()
 *******************************************************************/
FabricBitstream build_fabric_dependent_bitstream(
  const BitstreamManager& bitstream_manager,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const ConfigProtocol& config_protocol, const bool& verbose) {
  FabricBitstream fabric_bitstream;

  vtr::ScopedStartFinishTimer timer("\nBuild fabric dependent bitstream\n");

  /* Get the top module name in module manager, which is our starting point */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Find the top block in bitstream manager, which has not parents */
  std::vector<ConfigBlockId> top_block =
    find_bitstream_manager_top_blocks(bitstream_manager);
  /* Make sure we have only 1 top block and its name matches the top module */
  VTR_ASSERT(1 == top_block.size());
  VTR_ASSERT(
    0 == top_module_name.compare(bitstream_manager.block_name(top_block[0])));

  /* Start build-up formally */
  build_module_fabric_dependent_bitstream(
    config_protocol, circuit_lib, bitstream_manager, top_block[0],
    module_manager, top_module, fabric_bitstream);

  VTR_LOGV(verbose, "Built %lu configuration bits for fabric\n",
           fabric_bitstream.num_bits());

  return fabric_bitstream;
}

/* A version of the above function with caching of some of the larger data
 * structures to allow faster bitstream generation for a fixed fpga fabric.
 *  Currently only works with ql_memory_bank flattened configuration
 */

FabricBitstream load_fabric_dependent_bitstream(
  const BitstreamManager& bitstream_manager,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const ConfigProtocol& config_protocol, const bool& verbose,
  const std::string& infile) {
  VTR_ASSERT(config_protocol.type() == CONFIG_MEM_QL_MEMORY_BANK);

  FabricBitstream fabric_bitstream;

  vtr::ScopedStartFinishTimer timer("\n Load fabric dependent bitstream\n");

  /* Get the top module name in module manager, which is our starting point */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Find the top block in bitstream manager, which has not parents */
  std::vector<ConfigBlockId> top_block =
    find_bitstream_manager_top_blocks(bitstream_manager);
  /* Make sure we have only 1 top block and its name matches the top module */
  VTR_ASSERT(1 == top_block.size());
  VTR_ASSERT(
    0 == top_module_name.compare(bitstream_manager.block_name(top_block[0])));

  /* Start build-up formally */
  load_module_fabric_dependent_bitstream_ql_memory_bank(
    config_protocol, circuit_lib, bitstream_manager, top_block[0],
    module_manager, top_module, fabric_bitstream, infile);

  VTR_LOGV(verbose, "Built %lu configuration bits for fabric\n",
           fabric_bitstream.num_bits());

  return fabric_bitstream;
}

} /* end namespace openfpga */
