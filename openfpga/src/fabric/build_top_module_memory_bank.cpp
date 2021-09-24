/********************************************************************
 * This file includes functions that are used to organize memories 
 * in the top module of FPGA fabric
 *******************************************************************/
#include <cmath>
#include <limits>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "vpr_utils.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "rr_gsb_utils.h"
#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "memory_utils.h"
#include "decoder_library_utils.h"
#include "module_manager_utils.h"
#include "memory_bank_utils.h"
#include "build_decoder_modules.h"
#include "build_top_module_memory_bank.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Top-level function to add nets for quicklogic memory banks
 * Each configuration region has independent memory bank circuitry
 * - Find the number of BLs and WLs required for each region
 * - Create BL and WL decoders, and add them to decoder library
 * - Create nets to connect from top-level module inputs to inputs of decoders
 * - Create nets to connect from outputs of decoders to BL/WL of configurable children
 *
 * Detailed schematic of how memory banks are connected in the top-level:
 * Consider a random Region X, local BL address lines are aligned to the LSB of the
 * top-level BL address lines
 *
 *            top_bl_addr[N-1:0]
 *                 ^
 *                 | local_bl_addr[N-1:0]
 *                 |
 *           +-----+------------------+
 *           |     |                  |
 *           |  +-------------------+ |
 *           |  | Word Line Decoder | |
 *           |  +-------------------+ |
 *           |                        |
 *
 * The BL/WL decoders should have the same circuit designs no matter what region 
 * they are placed even when the number of configuration bits are different 
 * from one region to another!
 * This is designed to avoid any address collision between memory banks
 * since they are programmed in the same clock cycle
 * For example: 
 *   - Memory Bank A has 36 memory cells.
 *     Its BL decoder has 3 address bit and 6 data output bit
 *     Its WL decoder has 3 address bit and 6 data output bit
 *   - Memory Bank B has 16 memory cells.
 *     Its BL decoder has 2 address bit and 4 data output bit
 *     Its WL decoder has 2 address bit and 4 data output bit
 *   - If we try to program the 36th memory cell in bank A
 *     the BL address will be 3'b110
 *     the WL address will be 3'b110
 *     the data input will be 1'b0
 *   - If we try to program the 4th memory cell in bank A
 *     the BL address will be 3'b010
 *     the WL address will be 3'b010
 *     the data input will be 1'b1
 *     However, in both cases, this will trigger a parasitic programming in bank B
 *     the BL address will be 2'b10 
 *     the WL address will be 2'b10
 *     Assume the data input is expected to be 1'b1 for bank B
 *     but it will be overwritten to 1'b0 when programming the 36th cell in bank A!
 *
 * Detailed schematic of each memory bank:
 * @note The numbers are just made to show a simplified example, practical cases are more complicated!
 *
 *                            WL_enable  WL address 
 *                               |           |
 *                               v           v
 *                           +-----------------------------------------------+
 *                           |     Word Line Decoder                         |
 *                           +-----------------------------------------------+
 *               +---------+    |              |                    | 
 *  BL           |         |    |              |                    |
 *  enable  ---->|         |-----------+---------------+----  ...   |------+--> BL[0:2]
 *               |         |    |      |       |       |            |      |
 *               |         |    |      v       |       v            |      v
 *               | Bit     |    |   +-------+  |   +-------+        |  +------+
 *  BL           | Line    |    +-->| SRAM  |  +-->| SRAM  |        +->| SRAM |
 *  address ---->| Decoder |    |   | [0:8] |  |   | [0:5] |   ...  |  | [0:7]|
 *               |         |    |   +-------+  |   +-------+        |  +------+
 *               |         |    |              |                    |
 *               |         |-----------+--------------+---------    | -----+--> BL[0:9]
 *               |         |    |      |       |       |            |      |
 *               |         |    |      v       |       v            |      v
 *               |         |    |   +-------+  |   +-------+        |  +-------+
 *               |         |    +-->| SRAM  |  |   | SRAM  |        +->| SRAM  |
 *               |         |    |   | [0:80]|  |   | [0:63]|   ...  |  | [0:31]|
 *               |         |    |   +-------+  |   +-------+        |  +-------+
 *               |         |    |                                   |
 *               |         |    |     ...     ...    ...            |    ...
 *               |         |    |              |                    |
 *               |         |-----------+---------------+----  ---   | -----+--> BL[0:3]
 *               |         |    |      |       |       |            |      |
 *               |         |    |      v       |       v            |      v
 *               |         |    |   +-------+  |   +-------+        |  +-------+
 *               |         |    +-->| SRAM  |  +-->| SRAM  |        +->| SRAM  |
 *               |         |    |   |[0:5]  |  |   | [0:8] |   ...  |  | [0:2] |
 *               |         |    |   +-------+  |   +-------+        |  +-------+
 *  BL           |         |    v              v                    v
 *  data_in ---->|         |  WL[0:9]          WL[0:7]                WL[0:4]
 *               +---------+
 *
 **********************************************************************/
void add_top_module_nets_cmos_ql_memory_bank_config_bus(ModuleManager& module_manager,
                                                        DecoderLibrary& decoder_lib,
                                                        const ModuleId& top_module,
                                                        const CircuitLibrary& circuit_lib,
                                                        const CircuitModelId& sram_model,
                                                        const TopModuleNumConfigBits& num_config_bits) {
  /* Find Enable port from the top-level module */ 
  ModulePortId en_port = module_manager.find_module_port(top_module, std::string(DECODER_ENABLE_PORT_NAME));
  BasicPort en_port_info = module_manager.module_port(top_module, en_port);

  /* Find data-in port from the top-level module */ 
  ModulePortId din_port = module_manager.find_module_port(top_module, std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port_info = module_manager.module_port(top_module, din_port);

  /* Data in port should match the number of configuration regions */
  VTR_ASSERT(din_port_info.get_width() == module_manager.regions(top_module).size());

  /* Find readback port from the top-level module */ 
  ModulePortId readback_port = module_manager.find_module_port(top_module, std::string(DECODER_READBACK_PORT_NAME));
  BasicPort readback_port_info;

  /* Readback port if available, should be a 1-bit port */
  if (readback_port) {
    readback_port_info = module_manager.module_port(top_module, readback_port);
    VTR_ASSERT(readback_port_info.get_width() == 1);
  }

  /* Find BL and WL address port from the top-level module */ 
  ModulePortId bl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port_info = module_manager.module_port(top_module, bl_addr_port);

  ModulePortId wl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port_info = module_manager.module_port(top_module, wl_addr_port);

  /* Find the top-level number of BLs and WLs required to access each memory bit */
  size_t bl_addr_size = bl_addr_port_info.get_width();
  size_t wl_addr_size = wl_addr_port_info.get_width();

  /* Each memory bank has a unified number of BL/WLs */
  size_t num_bls = 0;
  for (const auto& curr_config_bits : num_config_bits) {
     num_bls = std::max(num_bls, curr_config_bits.first);
  }

  size_t num_wls = 0;
  for (const auto& curr_config_bits : num_config_bits) {
     num_wls = std::max(num_wls, curr_config_bits.second);
  }

  /* Create separated memory bank circuitry, i.e., BL/WL decoders for each region */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    /************************************************************** 
     * Add the BL decoder module 
     * Search the decoder library
     * If we find one, we use the module.
     * Otherwise, we create one and add it to the decoder library
     */
    DecoderId bl_decoder_id = decoder_lib.find_decoder(bl_addr_size, num_bls,
                                                       true, true, false, false);
    if (DecoderId::INVALID() == bl_decoder_id) {
      bl_decoder_id = decoder_lib.add_decoder(bl_addr_size, num_bls, true, true, false, false);
    }
    VTR_ASSERT(DecoderId::INVALID() != bl_decoder_id);

    /* Create a module if not existed yet */
    std::string bl_decoder_module_name = generate_memory_decoder_with_data_in_subckt_name(bl_addr_size, num_bls);
    ModuleId bl_decoder_module = module_manager.find_module(bl_decoder_module_name);
    if (ModuleId::INVALID() == bl_decoder_module) {
      /* BL decoder has the same ports as the frame-based decoders
       * We reuse it here
       */
      bl_decoder_module = build_bl_memory_decoder_module(module_manager,
                                                         decoder_lib,
                                                         bl_decoder_id);
    }
    VTR_ASSERT(ModuleId::INVALID() != bl_decoder_module);
    size_t curr_bl_decoder_instance_id = module_manager.num_instance(top_module, bl_decoder_module);
    module_manager.add_child_module(top_module, bl_decoder_module);

    /************************************************************** 
     * Add the WL decoder module 
     * Search the decoder library
     * If we find one, we use the module.
     * Otherwise, we create one and add it to the decoder library
     */
    DecoderId wl_decoder_id = decoder_lib.find_decoder(wl_addr_size, num_wls,
                                                       true, false, false, readback_port != ModulePortId::INVALID());
    if (DecoderId::INVALID() == wl_decoder_id) {
      wl_decoder_id = decoder_lib.add_decoder(wl_addr_size, num_wls, true, false, false, readback_port != ModulePortId::INVALID());
    }
    VTR_ASSERT(DecoderId::INVALID() != wl_decoder_id);

    /* Create a module if not existed yet */
    std::string wl_decoder_module_name = generate_memory_decoder_subckt_name(wl_addr_size, num_wls);
    ModuleId wl_decoder_module = module_manager.find_module(wl_decoder_module_name);
    if (ModuleId::INVALID() == wl_decoder_module) {
      /* BL decoder has the same ports as the frame-based decoders
       * We reuse it here
       */
      wl_decoder_module = build_wl_memory_decoder_module(module_manager,
                                                         decoder_lib,
                                                         wl_decoder_id);
    }
    VTR_ASSERT(ModuleId::INVALID() != wl_decoder_module);
    size_t curr_wl_decoder_instance_id = module_manager.num_instance(top_module, wl_decoder_module);
    module_manager.add_child_module(top_module, wl_decoder_module);

    /************************************************************** 
     * Add module nets from the top module to BL decoder's inputs
     */
    ModulePortId bl_decoder_en_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_ENABLE_PORT_NAME));
    BasicPort bl_decoder_en_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_en_port);

    ModulePortId bl_decoder_addr_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
    BasicPort bl_decoder_addr_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_addr_port);

    ModulePortId bl_decoder_din_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_DATA_IN_PORT_NAME));
    BasicPort bl_decoder_din_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_din_port);

    /* Data in port of the local BL decoder should always be 1 */
    VTR_ASSERT(1 == bl_decoder_din_port_info.get_width());

    /* Top module Enable port -> BL Decoder Enable port */
    add_module_bus_nets(module_manager,
                        top_module,
                        top_module, 0, en_port,
                        bl_decoder_module, curr_bl_decoder_instance_id, bl_decoder_en_port);

    /* Top module Address port -> BL Decoder Address port */
    add_module_bus_nets(module_manager,
                        top_module,
                        top_module, 0, bl_addr_port,
                        bl_decoder_module, curr_bl_decoder_instance_id, bl_decoder_addr_port);

    /* Top module data_in port -> BL Decoder data_in port:
     * Note that each region has independent data_in connection from the top-level module 
     * The pin index is the configuration region index
     */
    ModuleNetId din_net = create_module_source_pin_net(module_manager, top_module, 
                                                       top_module, 0, 
                                                       din_port,
                                                       din_port_info.pins()[size_t(config_region)]);
    VTR_ASSERT(ModuleNetId::INVALID() != din_net);

    /* Configure the net sink */
    module_manager.add_module_net_sink(top_module, din_net, bl_decoder_module, curr_bl_decoder_instance_id, bl_decoder_din_port, bl_decoder_din_port_info.pins()[0]);

    /************************************************************** 
     * Add module nets from the top module to WL decoder's inputs 
     */
    ModulePortId wl_decoder_en_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_ENABLE_PORT_NAME));
    BasicPort wl_decoder_en_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_en_port);

    ModulePortId wl_decoder_addr_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_ADDRESS_PORT_NAME));
    BasicPort wl_decoder_addr_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_addr_port);

    ModulePortId wl_decoder_readback_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_READBACK_PORT_NAME));
    BasicPort wl_decoder_readback_port_info;
    if (wl_decoder_readback_port) { 
      wl_decoder_readback_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_readback_port);
    }

    /* Top module Enable port -> WL Decoder Enable port */
    add_module_bus_nets(module_manager,
                        top_module,
                        top_module, 0, en_port,
                        wl_decoder_module, curr_wl_decoder_instance_id, wl_decoder_en_port);

    /* Top module Address port -> WL Decoder Address port */
    add_module_bus_nets(module_manager,
                        top_module,
                        top_module, 0, wl_addr_port,
                        wl_decoder_module, curr_wl_decoder_instance_id, wl_decoder_addr_port);

    /* Top module readback port -> WL Decoder readback port */
    if (wl_decoder_readback_port) { 
      add_module_bus_nets(module_manager,
                          top_module,
                          top_module, 0, readback_port,
                          wl_decoder_module, curr_wl_decoder_instance_id, wl_decoder_readback_port);
    }

    /************************************************************** 
     * Precompute the BLs and WLs distribution across the FPGA fabric
     * The distribution is a matrix which contains the starting index of BL/WL for each column or row
     */
    std::pair<int, int> child_x_range = compute_memory_bank_regional_configurable_child_x_range(module_manager, top_module, config_region);
    std::pair<int, int> child_y_range = compute_memory_bank_regional_configurable_child_y_range(module_manager, top_module, config_region);

    std::map<int, size_t> num_bls_per_tile = compute_memory_bank_regional_bitline_numbers_per_tile(module_manager, top_module,
                                                                                                   config_region,
                                                                                                   circuit_lib, sram_model);
    std::map<int, size_t> num_wls_per_tile = compute_memory_bank_regional_wordline_numbers_per_tile(module_manager, top_module,
                                                                                                    config_region,
                                                                                                    circuit_lib, sram_model);

    std::map<int, size_t> bl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_x_range, num_bls_per_tile);
    std::map<int, size_t> wl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_y_range, num_wls_per_tile);

    /************************************************************** 
     * Add nets from BL data out to each configurable child
     * BL data output pins are connected to the BL input pins of each PB/CB/SB
     * For all the PB/CB/SB in the same column, they share the same set of BLs
     * A quick example
     *
     *   BL[i .. i + sqrt(N)]     
     *     |
     *     |    CLB[1][H]
     *     |   +---------+     
     *     |   |  SRAM   |     
     *     +-->|  [0..N] |     
     *     |   +---------+
     *     |
     *    ...
     *     |    CLB[1][1]
     *     |   +---------+     
     *     |   |  SRAM   |     
     *     +-->|  [0..N] |     
     *     |   +---------+
     *     |
     */
    ModulePortId bl_decoder_dout_port = module_manager.find_module_port(bl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
    BasicPort bl_decoder_dout_port_info = module_manager.module_port(bl_decoder_module, bl_decoder_dout_port);

    for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
      ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
      vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 

      size_t child_instance = module_manager.region_configurable_child_instances(top_module, config_region)[child_id];

      /* Find the BL port */
      ModulePortId child_bl_port = module_manager.find_module_port(child_module, std::string(MEMORY_BL_PORT_NAME));
      BasicPort child_bl_port_info = module_manager.module_port(child_module, child_bl_port);

      size_t cur_bl_index = 0;

      for (const size_t& sink_bl_pin : child_bl_port_info.pins()) {
        size_t bl_pin_id = bl_start_index_per_tile[coord.x()] + cur_bl_index;
        /* Find the BL decoder data index: 
         * It should be the starting index plus an offset which is the residual when divided by the number of BLs in this tile
         */
        VTR_ASSERT(bl_pin_id < bl_decoder_dout_port_info.pins().size());

        /* Create net */
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                       bl_decoder_module, curr_bl_decoder_instance_id,
                                                       bl_decoder_dout_port,
                                                       bl_decoder_dout_port_info.pins()[bl_pin_id]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);

        /* Add net sink */
        module_manager.add_module_net_sink(top_module, net,
                                           child_module, child_instance, child_bl_port, sink_bl_pin);

        cur_bl_index++;
      }
    }

    /************************************************************** 
     * Add nets from WL data out to each configurable child
     */
    ModulePortId wl_decoder_dout_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
    BasicPort wl_decoder_dout_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_dout_port);

    for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
      ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
      vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 

      size_t child_instance = module_manager.region_configurable_child_instances(top_module, config_region)[child_id];

      /* Find the WL port */
      ModulePortId child_wl_port = module_manager.find_module_port(child_module, std::string(MEMORY_WL_PORT_NAME));
      BasicPort child_wl_port_info = module_manager.module_port(child_module, child_wl_port);

      size_t cur_wl_index = 0;

      for (const size_t& sink_wl_pin : child_wl_port_info.pins()) {
        size_t wl_pin_id = wl_start_index_per_tile[coord.y()] + cur_wl_index;
        VTR_ASSERT(wl_pin_id < wl_decoder_dout_port_info.pins().size());

        /* Create net */
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                       wl_decoder_module, curr_wl_decoder_instance_id,
                                                       wl_decoder_dout_port,
                                                       wl_decoder_dout_port_info.pins()[wl_pin_id]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);

        /* Add net sink */
        module_manager.add_module_net_sink(top_module, net,
                                           child_module, child_instance, child_wl_port, sink_wl_pin);
      
        cur_wl_index++;
      }
    }

    /************************************************************** 
     * Optional: Add nets from WLR data out to each configurable child
     */
    ModulePortId wl_decoder_data_ren_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_DATA_READ_ENABLE_PORT_NAME));
    BasicPort wl_decoder_data_ren_port_info;
    if (wl_decoder_data_ren_port) { 
      wl_decoder_data_ren_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_data_ren_port);
      for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
        ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
        vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 
  
        size_t child_instance = module_manager.region_configurable_child_instances(top_module, config_region)[child_id];
  
        /* Find the WL port */
        ModulePortId child_wlr_port = module_manager.find_module_port(child_module, std::string(MEMORY_WLR_PORT_NAME));
        BasicPort child_wlr_port_info = module_manager.module_port(child_module, child_wlr_port);
  
        size_t cur_wlr_index = 0;
  
        for (const size_t& sink_wlr_pin : child_wlr_port_info.pins()) {
          size_t wlr_pin_id = wl_start_index_per_tile[coord.y()] + cur_wlr_index;
          VTR_ASSERT(wlr_pin_id < wl_decoder_data_ren_port_info.pins().size());
  
          /* Create net */
          ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                         wl_decoder_module, curr_wl_decoder_instance_id,
                                                         wl_decoder_data_ren_port,
                                                         wl_decoder_data_ren_port_info.pins()[wlr_pin_id]);
          VTR_ASSERT(ModuleNetId::INVALID() != net);
  
          /* Add net sink */
          module_manager.add_module_net_sink(top_module, net,
                                             child_module, child_instance, child_wlr_port, sink_wlr_pin);
        
          cur_wlr_index++;
        }
      }
    }

    /************************************************************** 
     * Add the BL and WL decoders to the end of configurable children list
     * Note: this MUST be done after adding all the module nets to other regular configurable children
     */
    module_manager.add_configurable_child(top_module, bl_decoder_module, curr_bl_decoder_instance_id);
    module_manager.add_configurable_child_to_region(top_module,
                                                    config_region,
                                                    bl_decoder_module, 
                                                    curr_bl_decoder_instance_id,
                                                    module_manager.configurable_children(top_module).size() - 1);

    module_manager.add_configurable_child(top_module, wl_decoder_module, curr_wl_decoder_instance_id);
    module_manager.add_configurable_child_to_region(top_module,
                                                    config_region,
                                                    wl_decoder_module, 
                                                    curr_wl_decoder_instance_id,
                                                    module_manager.configurable_children(top_module).size() - 1);
  }
}

/********************************************************************
 * Add a list of ports that are used for SRAM configuration to the FPGA 
 * top-level module
 * - Add ports for BL control circuitry:
 *   - Decoder
 *     - an enable signals
 *     - an BL address port
 *     - a data-in port
 *   - Flatten
 *     - BL ports
 *   - TODO: Shift registers
 *     - Head of shift register chain for BLs
 *     - Tail of shift register chain for BLs
 *
 * - Add ports for WL control circuitry:
 *   - Decoder
 *     - an WL address port
 *     - a Readback port (Optional, only needed when WLR is required)
 *   - Flatten
 *     - WL ports
 *     - WLR ports (Optional)
 *   - TODO: Shift registers
 *     - Head of shift register chain for WLs
 *     - Tail of shift register chain for WLs
 *     - a Readback port (Optional, only needed when WLR is required)
 *
 * @note In this memory decoders, the address size will be computed in a different way than the regular one
 ********************************************************************/
void add_top_module_ql_memory_bank_sram_ports(ModuleManager& module_manager, 
                                              const ModuleId& module_id,
                                              const CircuitLibrary& circuit_lib,
                                              const ConfigProtocol& config_protocol,
                                              const TopModuleNumConfigBits& num_config_bits) {
  VTR_ASSERT_SAFE(CONFIG_MEM_QL_MEMORY_BANK == config_protocol.type());
  CircuitModelId sram_model = config_protocol.memory_model();

  switch (config_protocol.bl_protocol_type()) {
    case BLWL_PROTOCOL_DECODER: {
      /* Add enable signals */
      BasicPort en_port(std::string(DECODER_ENABLE_PORT_NAME), 1);
      module_manager.add_port(module_id, en_port, ModuleManager::MODULE_INPUT_PORT);

      /* BL address size is the largest among all the regions */
      size_t bl_addr_size = 0;
      for (const ConfigRegionId& config_region : module_manager.regions(module_id)) {
         bl_addr_size = std::max(bl_addr_size, find_mux_local_decoder_addr_size(num_config_bits[config_region].first));
      }
      BasicPort bl_addr_port(std::string(DECODER_BL_ADDRESS_PORT_NAME), bl_addr_size);
      module_manager.add_port(module_id, bl_addr_port, ModuleManager::MODULE_INPUT_PORT);

      /* Data input should be dependent on the number of configuration regions*/
      BasicPort din_port(std::string(DECODER_DATA_IN_PORT_NAME), config_protocol.num_regions());
      module_manager.add_port(module_id, din_port, ModuleManager::MODULE_INPUT_PORT);
      break;
    }
    case BLWL_PROTOCOL_FLATTEN: {
      /* Each region will have independent BLs */
      for (const ConfigRegionId& config_region : module_manager.regions(module_id)) {
        size_t bl_size = num_config_bits[config_region].first;
        BasicPort bl_port(generate_regional_blwl_port_name(std::string(MEMORY_BL_PORT_NAME), config_region), bl_size);
        module_manager.add_port(module_id, bl_port, ModuleManager::MODULE_INPUT_PORT);
      }
      break;
    }
    case BLWL_PROTOCOL_SHIFT_REGISTER: {
      /* TODO */
      break;
    }
    default: {
      VTR_LOG_ERROR("Invalid BL protocol");
      exit(1);
    }
  }

  switch (config_protocol.wl_protocol_type()) {
    case BLWL_PROTOCOL_DECODER: {
      /* WL address size is the largest among all the regions */
      size_t wl_addr_size = 0;
      for (const ConfigRegionId& config_region : module_manager.regions(module_id)) {
         wl_addr_size = std::max(wl_addr_size, find_mux_local_decoder_addr_size(num_config_bits[config_region].second));
      }
      BasicPort wl_addr_port(std::string(DECODER_WL_ADDRESS_PORT_NAME), wl_addr_size);
      module_manager.add_port(module_id, wl_addr_port, ModuleManager::MODULE_INPUT_PORT);

      /* Optional: If we have WLR port, we should add a read-back port */
      if (!circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WLR).empty()) {
        BasicPort readback_port(std::string(DECODER_READBACK_PORT_NAME), config_protocol.num_regions());
        module_manager.add_port(module_id, readback_port, ModuleManager::MODULE_INPUT_PORT);
      }
      break;
    }
    case BLWL_PROTOCOL_FLATTEN: {
      /* Each region will have independent WLs */
      for (const ConfigRegionId& config_region : module_manager.regions(module_id)) {
        size_t wl_size = num_config_bits[config_region].first;
        BasicPort wl_port(generate_regional_blwl_port_name(std::string(MEMORY_WL_PORT_NAME), config_region), wl_size);
        module_manager.add_port(module_id, wl_port, ModuleManager::MODULE_INPUT_PORT);
      }
      break;
    }
    case BLWL_PROTOCOL_SHIFT_REGISTER: {
      /* TODO */
      break;
    }
    default: {
      VTR_LOG_ERROR("Invalid WL protocol");
      exit(1);
    }
  }
}

} /* end namespace openfpga */
