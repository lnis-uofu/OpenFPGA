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
#include "build_memory_modules.h"
#include "build_decoder_modules.h"
#include "memory_bank_shift_register_banks.h"
#include "build_top_module_memory_bank.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Connect all the memory modules under the parent module in a chain
 * 
 *                +--------+    +--------+            +--------+
 *  ccff_head --->| Memory |--->| Memory |--->... --->| Memory |----> ccff_tail
 *                | Module |    | Module |            | Module |
 *                |   [0]  |    |   [1]  |            |  [N-1] |             
 *                +--------+    +--------+            +--------+
 *  For the 1st memory module:
 *    net source is the configuration chain head of the primitive module
 *    net sink is the configuration chain head of the next memory module
 *
 *  For the rest of memory modules:
 *    net source is the configuration chain tail of the previous memory module
 *    net sink is the configuration chain head of the next memory module
 *
 *  Note that:
 *    This function is designed for memory modules ONLY!
 *    Do not use it to replace the 
 *      add_module_nets_cmos_memory_chain_config_bus() !!!
 *********************************************************************/
static 
void add_module_nets_to_ql_memory_bank_shift_register_module(ModuleManager& module_manager,
                                                             const ModuleId& parent_module,
                                                             const CircuitLibrary& circuit_lib,
                                                             const CircuitPortId& model_input_port,
                                                             const CircuitPortId& model_output_port,
                                                             const std::string& chain_head_port_name,
                                                             const std::string& chain_tail_port_name) {
  for (size_t mem_index = 0; mem_index < module_manager.configurable_children(parent_module).size(); ++mem_index) {
    ModuleId net_src_module_id;
    size_t net_src_instance_id;
    ModulePortId net_src_port_id;

    ModuleId net_sink_module_id;
    size_t net_sink_instance_id;
    ModulePortId net_sink_port_id;

    if (0 == mem_index) {
      /* Find the port name of configuration chain head */
      std::string src_port_name = chain_head_port_name;
      net_src_module_id = parent_module; 
      net_src_instance_id = 0;
      net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_prefix(model_input_port);
      net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
      net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
      net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 
    } else {
      /* Find the port name of previous memory module */
      std::string src_port_name = circuit_lib.port_prefix(model_output_port);
      net_src_module_id = module_manager.configurable_children(parent_module)[mem_index - 1]; 
      net_src_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index - 1];
      net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

      /* Find the port name of next memory module */
      std::string sink_port_name = circuit_lib.port_prefix(model_input_port);
      net_sink_module_id = module_manager.configurable_children(parent_module)[mem_index]; 
      net_sink_instance_id = module_manager.configurable_child_instances(parent_module)[mem_index];
      net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 
    }

    /* Get the pin id for source port */
    BasicPort net_src_port = module_manager.module_port(net_src_module_id, net_src_port_id); 
    /* Get the pin id for sink port */
    BasicPort net_sink_port = module_manager.module_port(net_sink_module_id, net_sink_port_id); 
    /* Port sizes of source and sink should match */
    VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());
    
    /* Create a net for each pin */
    for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
      /* Create a net and add source and sink to it */
      ModuleNetId net = create_module_source_pin_net(module_manager, parent_module, net_src_module_id, net_src_instance_id, net_src_port_id, net_src_port.pins()[pin_id]);
      /* Add net sink */
      module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);
    }
  }

  /* For the last memory module:
   *    net source is the configuration chain tail of the previous memory module
   *    net sink is the configuration chain tail of the primitive module
   */
  /* Find the port name of previous memory module */
  std::string src_port_name = circuit_lib.port_prefix(model_output_port);
  ModuleId net_src_module_id = module_manager.configurable_children(parent_module).back(); 
  size_t net_src_instance_id = module_manager.configurable_child_instances(parent_module).back();
  ModulePortId net_src_port_id = module_manager.find_module_port(net_src_module_id, src_port_name); 

  /* Find the port name of next memory module */
  std::string sink_port_name = chain_tail_port_name;
  ModuleId net_sink_module_id = parent_module; 
  size_t net_sink_instance_id = 0;
  ModulePortId net_sink_port_id = module_manager.find_module_port(net_sink_module_id, sink_port_name); 

  /* Get the pin id for source port */
  BasicPort net_src_port = module_manager.module_port(net_src_module_id, net_src_port_id); 
  /* Get the pin id for sink port */
  BasicPort net_sink_port = module_manager.module_port(net_sink_module_id, net_sink_port_id); 
  /* Port sizes of source and sink should match */
  VTR_ASSERT(net_src_port.get_width() == net_sink_port.get_width());
  
  /* Create a net for each pin */
  for (size_t pin_id = 0; pin_id < net_src_port.pins().size(); ++pin_id) {
    /* Create a net and add source and sink to it */
    ModuleNetId net = create_module_source_pin_net(module_manager, parent_module, net_src_module_id, net_src_instance_id, net_src_port_id, net_src_port.pins()[pin_id]);
    /* Add net sink */
    module_manager.add_module_net_sink(parent_module, net, net_sink_module_id, net_sink_instance_id, net_sink_port_id, net_sink_port.pins()[pin_id]);
  }
}

/*********************************************************************
 * BL shift register chain module organization
 *              
 *                +-------+    +-------+            +-------+
 *    chain   --->| CCFF  |--->| CCFF  |--->... --->| CCFF  |---->chain
 *  input&clock   |  [0]  |    |  [1]  |            | [N-1] |     output
 *                +-------+    +-------+            +-------+
 *                    |            |      ...           | config-memory output
 *                    v            v                    v
 *                +-----------------------------------------+
 *                |   BLs of configurable modules           |
 *
 ********************************************************************/
static 
ModuleId build_bl_shift_register_chain_module(ModuleManager& module_manager,
                                              const CircuitLibrary& circuit_lib,
                                              const std::string& module_name,
                                              const CircuitModelId& sram_model,
                                              const size_t& num_mems) {

  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == sram_input_ports.size());

  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == sram_output_ports.size());

  /* Get the BL ports from the SRAM */
  std::vector<CircuitPortId> sram_bl_ports = circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_BL, true);
  VTR_ASSERT(1 == sram_bl_ports.size());

  /* Create a module and add to the module manager */
  ModuleId mem_module = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));

  /* Label module usage */
  module_manager.set_module_usage(mem_module, ModuleManager::MODULE_CONFIG);
  
  /* Add an input port, which is the head of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first input of the CCFF model as the D port,
   * which will be connected to the head of the chain
   */
  BasicPort chain_head_port(BL_SHIFT_REGISTER_CHAIN_HEAD_NAME, 
                            circuit_lib.port_size(sram_input_ports[0]));
  module_manager.add_port(mem_module, chain_head_port, ModuleManager::MODULE_INPUT_PORT);

  /* Add an output port, which is the tail of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first output of the CCFF model as the Q port,
   * which will be connected to the tail of the chain
   */
  BasicPort chain_tail_port(BL_SHIFT_REGISTER_CHAIN_TAIL_NAME, 
                            circuit_lib.port_size(sram_output_ports[0]));
  module_manager.add_port(mem_module, chain_tail_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Add the output ports to output BL signals */
  BasicPort chain_bl_port(BL_SHIFT_REGISTER_CHAIN_BL_OUT_NAME, 
                          num_mems);
  module_manager.add_port(mem_module, chain_bl_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    size_t sram_mem_instance = module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);
    module_manager.add_configurable_child(mem_module, sram_mem_module, sram_mem_instance);

    /* Build module nets to wire bl outputs of sram modules to BL outputs of memory module */
    for (const auto& child_module_output_port : sram_bl_ports) {
      std::string chain_output_port_name = std::string(BL_SHIFT_REGISTER_CHAIN_BL_OUT_NAME); 
      add_module_output_nets_to_chain_mem_modules(module_manager, mem_module, 
                                                  chain_output_port_name, circuit_lib, 
                                                  child_module_output_port,
                                                  sram_mem_module, i, sram_mem_instance);
    }
  }

  /* Build module nets to wire the configuration chain */
  add_module_nets_to_ql_memory_bank_shift_register_module(module_manager, mem_module,
                                                          circuit_lib, sram_input_ports[0], sram_output_ports[0],
                                                          std::string(BL_SHIFT_REGISTER_CHAIN_HEAD_NAME),
                                                          std::string(BL_SHIFT_REGISTER_CHAIN_TAIL_NAME));

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);

  return mem_module;
}

/*********************************************************************
 * WL shift register chain module organization
 *              
 *                +-------+    +-------+            +-------+
 *    chain   --->| CCFF  |--->| CCFF  |--->... --->| CCFF  |---->chain
 *  input&clock   |  [0]  |    |  [1]  |            | [N-1] |     output
 *                +-------+    +-------+            +-------+
 *                    |            |      ...           | config-memory output
 *                    v            v                    v
 *                +-----------------------------------------+
 *                |   WL/WLRs of configurable modules       |
 *
 ********************************************************************/
static 
ModuleId build_wl_shift_register_chain_module(ModuleManager& module_manager,
                                              const CircuitLibrary& circuit_lib,
                                              const std::string& module_name,
                                              const CircuitModelId& sram_model,
                                              const size_t& num_mems) {

  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == sram_input_ports.size());

  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == sram_output_ports.size());

  /* Get the WL ports from the SRAM */
  std::vector<CircuitPortId> sram_wl_ports = circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WL, true);
  VTR_ASSERT(1 == sram_wl_ports.size());

  /* Get the optional WLR ports from the SRAM */
  std::vector<CircuitPortId> sram_wlr_ports = circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WLR, true);
  VTR_ASSERT(0 == sram_wlr_ports.size() || 1 == sram_wlr_ports.size());

  /* Create a module and add to the module manager */
  ModuleId mem_module = module_manager.add_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));

  /* Label module usage */
  module_manager.set_module_usage(mem_module, ModuleManager::MODULE_CONFIG);
  
  /* Add an input port, which is the head of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first input of the CCFF model as the D port,
   * which will be connected to the head of the chain
   */
  BasicPort chain_head_port(WL_SHIFT_REGISTER_CHAIN_HEAD_NAME, 
                            circuit_lib.port_size(sram_input_ports[0]));
  module_manager.add_port(mem_module, chain_head_port, ModuleManager::MODULE_INPUT_PORT);

  /* Add an output port, which is the tail of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first output of the CCFF model as the Q port,
   * which will be connected to the tail of the chain
   */
  BasicPort chain_tail_port(WL_SHIFT_REGISTER_CHAIN_TAIL_NAME, 
                            circuit_lib.port_size(sram_output_ports[0]));
  module_manager.add_port(mem_module, chain_tail_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Add the output ports to output BL signals */
  BasicPort chain_wl_port(WL_SHIFT_REGISTER_CHAIN_WL_OUT_NAME, 
                          num_mems);
  module_manager.add_port(mem_module, chain_wl_port, ModuleManager::MODULE_OUTPUT_PORT);

  /* Find the sram module in the module manager */
  ModuleId sram_mem_module = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    size_t sram_mem_instance = module_manager.num_instance(mem_module, sram_mem_module);
    module_manager.add_child_module(mem_module, sram_mem_module);
    module_manager.add_configurable_child(mem_module, sram_mem_module, sram_mem_instance);

    /* Build module nets to wire wl outputs of sram modules to WL outputs of memory module */
    for (const auto& child_module_output_port : sram_wl_ports) {
      std::string chain_output_port_name = std::string(WL_SHIFT_REGISTER_CHAIN_WL_OUT_NAME); 
      add_module_output_nets_to_chain_mem_modules(module_manager, mem_module, 
                                                  chain_output_port_name, circuit_lib, 
                                                  child_module_output_port,
                                                  sram_mem_module, i, sram_mem_instance);
    }

    /* Build module nets to wire wlr outputs of sram modules to WLR outputs of memory module */
    for (const auto& child_module_output_port : sram_wlr_ports) {
      std::string chain_output_port_name = std::string(WL_SHIFT_REGISTER_CHAIN_WLR_OUT_NAME); 
      add_module_output_nets_to_chain_mem_modules(module_manager, mem_module, 
                                                  chain_output_port_name, circuit_lib, 
                                                  child_module_output_port,
                                                  sram_mem_module, i, sram_mem_instance);
    }
  }

  /* Build module nets to wire the configuration chain */
  add_module_nets_to_ql_memory_bank_shift_register_module(module_manager, mem_module,
                                                          circuit_lib, sram_input_ports[0], sram_output_ports[0],
                                                          std::string(WL_SHIFT_REGISTER_CHAIN_HEAD_NAME),
                                                          std::string(WL_SHIFT_REGISTER_CHAIN_TAIL_NAME));

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, mem_module);

  return mem_module;
}

/*********************************************************************
 * This function to add nets for quicklogic memory banks
 * Each configuration region has independent memory bank circuitry
 * - Find the number of BLs and WLs required for each region
 * - Create BL and WL decoders, and add them to decoder library
 * - Create nets to connect from top-level module inputs to inputs of decoders
 * - Create nets to connect from outputs of decoders to BL/WL of configurable children
 *
 * @note this function only adds the BL configuration bus for decoders
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
static 
void add_top_module_nets_cmos_ql_memory_bank_bl_decoder_config_bus(ModuleManager& module_manager,
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

  /* Find BL and WL address port from the top-level module */ 
  ModulePortId bl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port_info = module_manager.module_port(top_module, bl_addr_port);

  /* Find the top-level number of BLs and WLs required to access each memory bit */
  size_t bl_addr_size = bl_addr_port_info.get_width();

  /* Each memory bank has a unified number of BL/WLs */
  size_t num_bls = 0;
  for (const auto& curr_config_bits : num_config_bits) {
     num_bls = std::max(num_bls, curr_config_bits.first);
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
     * Precompute the BLs and WLs distribution across the FPGA fabric
     * The distribution is a matrix which contains the starting index of BL/WL for each column or row
     */
    std::pair<int, int> child_x_range = compute_memory_bank_regional_configurable_child_x_range(module_manager, top_module, config_region);
    std::map<int, size_t> num_bls_per_tile = compute_memory_bank_regional_bitline_numbers_per_tile(module_manager, top_module,
                                                                                                   config_region,
                                                                                                   circuit_lib, sram_model);
    std::map<int, size_t> bl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_x_range, num_bls_per_tile);

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
     * Add the BL and WL decoders to the end of configurable children list
     * Note: this MUST be done after adding all the module nets to other regular configurable children
     */
    module_manager.add_configurable_child(top_module, bl_decoder_module, curr_bl_decoder_instance_id);
    module_manager.add_configurable_child_to_region(top_module,
                                                    config_region,
                                                    bl_decoder_module, 
                                                    curr_bl_decoder_instance_id,
                                                    module_manager.configurable_children(top_module).size() - 1);
  }
}

/*********************************************************************
 * Top-level function to add nets for quicklogic memory banks
 * Each configuration region has independent memory bank circuitry
 * - Find the number of BLs and WLs required for each region
 * - Create BL and WL decoders, and add them to decoder library
 * - Create nets to connect from top-level module inputs to inputs of decoders
 * - Create nets to connect from outputs of decoders to BL/WL of configurable children
 *
 * @note this function only adds the WL configuration bus for decoders
 *
 * @note see detailed explanation on the bus connection in function add_top_module_nets_cmos_ql_memory_bank_bl_decoder_config_bus()
 **********************************************************************/
static 
void add_top_module_nets_cmos_ql_memory_bank_wl_decoder_config_bus(ModuleManager& module_manager,
                                                                   DecoderLibrary& decoder_lib,
                                                                   const ModuleId& top_module,
                                                                   const CircuitLibrary& circuit_lib,
                                                                   const CircuitModelId& sram_model,
                                                                   const TopModuleNumConfigBits& num_config_bits) {
  /* Find Enable port from the top-level module */ 
  ModulePortId en_port = module_manager.find_module_port(top_module, std::string(DECODER_ENABLE_PORT_NAME));
  BasicPort en_port_info = module_manager.module_port(top_module, en_port);
  
  /* Find readback port from the top-level module */ 
  ModulePortId readback_port = module_manager.find_module_port(top_module, std::string(DECODER_READBACK_PORT_NAME));
  BasicPort readback_port_info;

  /* Readback port if available, should be a 1-bit port */
  if (readback_port) {
    readback_port_info = module_manager.module_port(top_module, readback_port);
    VTR_ASSERT(readback_port_info.get_width() == 1);
  }

  /* Find BL and WL address port from the top-level module */ 
  ModulePortId wl_addr_port = module_manager.find_module_port(top_module, std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port_info = module_manager.module_port(top_module, wl_addr_port);

  /* Find the top-level number of BLs and WLs required to access each memory bit */
  size_t wl_addr_size = wl_addr_port_info.get_width();

  /* Each memory bank has a unified number of BL/WLs */
  size_t num_wls = 0;
  for (const auto& curr_config_bits : num_config_bits) {
     num_wls = std::max(num_wls, curr_config_bits.second);
  }

  /* Create separated memory bank circuitry, i.e., BL/WL decoders for each region */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
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
    std::pair<int, int> child_y_range = compute_memory_bank_regional_configurable_child_y_range(module_manager, top_module, config_region);
    std::map<int, size_t> num_wls_per_tile = compute_memory_bank_regional_wordline_numbers_per_tile(module_manager, top_module,
                                                                                                    config_region,
                                                                                                    circuit_lib, sram_model);
    std::map<int, size_t> wl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_y_range, num_wls_per_tile);

    /************************************************************** 
     * Add nets from WL data out to each configurable child
     */
    ModulePortId wl_decoder_dout_port = module_manager.find_module_port(wl_decoder_module, std::string(DECODER_DATA_OUT_PORT_NAME));
    BasicPort wl_decoder_dout_port_info = module_manager.module_port(wl_decoder_module, wl_decoder_dout_port);

    /* Note we skip the last child which is the bl decoder added */
    for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
      ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
      vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 

      size_t child_instance = module_manager.region_configurable_child_instances(top_module, config_region)[child_id];

      /* Find the WL port. If the child does not have WL port, bypass it. It is usually a decoder module */
      ModulePortId child_wl_port = module_manager.find_module_port(child_module, std::string(MEMORY_WL_PORT_NAME));
      if (!child_wl_port) {
        continue;
      }
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
  
        /* Find the WLR port. If the child does not have WLR port, bypass it. It is usually a decoder module */
        ModulePortId child_wlr_port = module_manager.find_module_port(child_module, std::string(MEMORY_WLR_PORT_NAME));
        if (!child_wlr_port) {
          continue;
        }
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
    module_manager.add_configurable_child(top_module, wl_decoder_module, curr_wl_decoder_instance_id);
    module_manager.add_configurable_child_to_region(top_module,
                                                    config_region,
                                                    wl_decoder_module, 
                                                    curr_wl_decoder_instance_id,
                                                    module_manager.configurable_children(top_module).size() - 1);
  }
}

/*********************************************************************
 * This function to add nets for quicklogic memory banks using flatten BLs and WLs
 * Each configuration region has independent BL/WLs
 * - Find the number of BLs and WLs required for each region
 * - Create nets to connect from top-level module inputs to BL/WL of configurable children
 *
 * @note this function only adds the BL configuration bus
 *
 * Detailed schematic of each memory bank:
 * @note The numbers are just made to show a simplified example, practical cases are more complicated!
 *
 *                 BL[0:9]        BL[10:17]          BL[18:22]
 *                   |              |                    | 
 *                   |              |                    |
 *     WL[0:3]-->-----------+---------------+----  ...   |------+-->        
 *                   |      |       |       |            |      |
 *                   |      v       |       v            |      v
 *                   |   +-------+  |   +-------+        |  +------+
 *                   +-->| SRAM  |  +-->| SRAM  |        +->| SRAM |
 *                   |   | [0:8] |  |   | [0:5] |   ...  |  | [0:7]|
 *                   |   +-------+  |   +-------+        |  +------+
 *                   |              |                    |
 *     WL[4:14]  -----------+--------------+---------    | -----+-->        
 *                   |      |       |       |            |      |
 *                   |      v       |       v            |      v
 *                   |   +-------+  |   +-------+        |  +-------+
 *                   +-->| SRAM  |  |   | SRAM  |        +->| SRAM  |
 *                   |   | [0:80]|  |   | [0:63]|   ...  |  | [0:31]|
 *                   |   +-------+  |   +-------+        |  +-------+
 *                   |                                   |
 *                   |     ...     ...    ...            |    ...
 *                   |              |                    |
 *     WL[15:18] -----------+---------------+----  ---   | -----+-->        
 *                   |      |       |       |            |      |
 *                   |      v       |       v            |      v
 *                   |   +-------+  |   +-------+        |  +-------+
 *                   +-->| SRAM  |  +-->| SRAM  |        +->| SRAM  |
 *                   |   |[0:5]  |  |   | [0:8] |   ...  |  | [0:2] |
 *                   |   +-------+  |   +-------+        |  +-------+
 *                   v              v                    v
 *                 WL[0:9]          WL[0:7]                WL[0:4]
 *               
 **********************************************************************/
static 
void add_top_module_nets_cmos_ql_memory_bank_bl_flatten_config_bus(ModuleManager& module_manager,
                                                                   const ModuleId& top_module,
                                                                   const CircuitLibrary& circuit_lib,
                                                                   const CircuitModelId& sram_model) {
  /* Create connections between BLs of top-level module and BLs of child modules for each region */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    /************************************************************** 
     * Precompute the BLs and WLs distribution across the FPGA fabric
     * The distribution is a matrix which contains the starting index of BL/WL for each column or row
     */
    std::pair<int, int> child_x_range = compute_memory_bank_regional_configurable_child_x_range(module_manager, top_module, config_region);
    std::map<int, size_t> num_bls_per_tile = compute_memory_bank_regional_bitline_numbers_per_tile(module_manager, top_module,
                                                                                                   config_region,
                                                                                                   circuit_lib, sram_model);
    std::map<int, size_t> bl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_x_range, num_bls_per_tile);

    /************************************************************** 
     * Add BL nets from top module to each configurable child
     * BL pins of top module are connected to the BL input pins of each PB/CB/SB
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
    ModulePortId top_module_bl_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(std::string(MEMORY_BL_PORT_NAME), config_region));
    BasicPort top_module_bl_port_info = module_manager.module_port(top_module, top_module_bl_port);

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
        VTR_ASSERT(bl_pin_id < top_module_bl_port_info.pins().size());

        /* Create net */
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                       top_module, 0,
                                                       top_module_bl_port,
                                                       top_module_bl_port_info.pins()[bl_pin_id]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);

        /* Add net sink */
        module_manager.add_module_net_sink(top_module, net,
                                           child_module, child_instance, child_bl_port, sink_bl_pin);

        cur_bl_index++;
      }
    }
  }
}

/*********************************************************************
 * Top-level function to add nets for quicklogic memory banks using flatten BL/WLs
 * Each configuration region has independent BL/WLs
 * - Find the number of BLs and WLs required for each region
 * - Create nets to connect from top-level module inputs to BL/WL of configurable children
 *
 * @note this function only adds the WL configuration bus
 *
 * @note see detailed explanation on the bus connection in function add_top_module_nets_cmos_ql_memory_bank_bl_flatten_config_bus()
 **********************************************************************/
static 
void add_top_module_nets_cmos_ql_memory_bank_wl_flatten_config_bus(ModuleManager& module_manager,
                                                                   const ModuleId& top_module,
                                                                   const CircuitLibrary& circuit_lib,
                                                                   const CircuitModelId& sram_model) {
  /* Create connections between WLs of top-level module and WLs of child modules for each region */
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    /************************************************************** 
     * Precompute the BLs and WLs distribution across the FPGA fabric
     * The distribution is a matrix which contains the starting index of BL/WL for each column or row
     */
    std::pair<int, int> child_y_range = compute_memory_bank_regional_configurable_child_y_range(module_manager, top_module, config_region);
    std::map<int, size_t> num_wls_per_tile = compute_memory_bank_regional_wordline_numbers_per_tile(module_manager, top_module,
                                                                                                    config_region,
                                                                                                    circuit_lib, sram_model);
    std::map<int, size_t> wl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_y_range, num_wls_per_tile);

    /************************************************************** 
     * Add WL nets from top module to each configurable child
     */
    ModulePortId top_module_wl_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(std::string(MEMORY_WL_PORT_NAME), config_region));
    BasicPort top_module_wl_port_info = module_manager.module_port(top_module, top_module_wl_port);

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
        VTR_ASSERT(wl_pin_id < top_module_wl_port_info.pins().size());

        /* Create net */
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                       top_module, 0,
                                                       top_module_wl_port,
                                                       top_module_wl_port_info.pins()[wl_pin_id]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);

        /* Add net sink */
        module_manager.add_module_net_sink(top_module, net,
                                           child_module, child_instance, child_wl_port, sink_wl_pin);
      
        cur_wl_index++;
      }
    }

    /************************************************************** 
     * Optional: Add WLR nets from top module to each configurable child
     */
    ModulePortId top_module_wlr_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(std::string(MEMORY_WLR_PORT_NAME), config_region));
    BasicPort top_module_wlr_port_info;
    if (top_module_wlr_port) { 
      top_module_wlr_port_info = module_manager.module_port(top_module, top_module_wlr_port);
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
          VTR_ASSERT(wlr_pin_id < top_module_wlr_port_info.pins().size());
  
          /* Create net */
          ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                         top_module, 0,
                                                         top_module_wlr_port,
                                                         top_module_wlr_port_info.pins()[wlr_pin_id]);
          VTR_ASSERT(ModuleNetId::INVALID() != net);
  
          /* Add net sink */
          module_manager.add_module_net_sink(top_module, net,
                                             child_module, child_instance, child_wlr_port, sink_wlr_pin);
        
          cur_wlr_index++;
        }
      }
    }
  }
}

/*********************************************************************
 * This function to add nets for QuickLogic memory bank
 * We build the net connects between the head ports of shift register banks 
 * and the head ports of top-level module
 * @note This function is applicable to both BL and WL shift registers
 **********************************************************************/
static 
void add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_heads(ModuleManager& module_manager,
                                                                       const ModuleId& top_module,
                                                                       const MemoryBankShiftRegisterBanks& sr_banks,
                                                                       const std::string& head_port_name) {
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    ModulePortId blsr_head_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(head_port_name, config_region));
    BasicPort blsr_head_port_info = module_manager.module_port(top_module, blsr_head_port);

    for (size_t iinst = 0; iinst < sr_banks.shift_register_bank_modules(config_region).size(); ++iinst) {
      ModuleId sr_bank_module = sr_banks.shift_register_bank_modules(config_region)[iinst];
      size_t sr_bank_instance = sr_banks.shift_register_bank_instances(config_region)[iinst];
      VTR_ASSERT(sr_bank_module);

      ModulePortId sr_module_head_port = module_manager.find_module_port(sr_bank_module, head_port_name);
      BasicPort sr_module_head_port_info = module_manager.module_port(sr_bank_module, sr_module_head_port);
      VTR_ASSERT(sr_module_head_port_info.get_width() == blsr_head_port_info.get_width());
      for (size_t ipin = 0; ipin < sr_module_head_port_info.pins().size(); ++ipin) {
        /* Create net */
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                       top_module, 0,
                                                       blsr_head_port,
                                                       blsr_head_port_info.pins()[ipin]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);

        /* Add net sink */
        module_manager.add_module_net_sink(top_module, net,
                                           sr_bank_module, sr_bank_instance, sr_module_head_port, sr_module_head_port_info.pins()[ipin]);
      }
    }
  }
}

/*********************************************************************
 * This function to add nets for QuickLogic memory bank
 * We build the net connects between the head ports of shift register banks 
 * and the head ports of top-level module
 * @note This function is applicable to both BL and WL shift registers
 **********************************************************************/
static 
void add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_tails(ModuleManager& module_manager,
                                                                       const ModuleId& top_module,
                                                                       const MemoryBankShiftRegisterBanks& sr_banks,
                                                                       const std::string& tail_port_name) {
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    ModulePortId blsr_tail_port = module_manager.find_module_port(top_module, generate_regional_blwl_port_name(tail_port_name, config_region));
    BasicPort blsr_tail_port_info = module_manager.module_port(top_module, blsr_tail_port);

    for (size_t iinst = 0; iinst < sr_banks.shift_register_bank_modules(config_region).size(); ++iinst) {
      ModuleId sr_bank_module = sr_banks.shift_register_bank_modules(config_region)[iinst];
      size_t sr_bank_instance = sr_banks.shift_register_bank_instances(config_region)[iinst];
      VTR_ASSERT(sr_bank_module);

      ModulePortId sr_module_tail_port = module_manager.find_module_port(sr_bank_module, tail_port_name);
      BasicPort sr_module_tail_port_info = module_manager.module_port(sr_bank_module, sr_module_tail_port);
      VTR_ASSERT(sr_module_tail_port_info.get_width() == blsr_tail_port_info.get_width());
      for (size_t ipin = 0; ipin < sr_module_tail_port_info.pins().size(); ++ipin) {
        /* Create net */
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                       sr_bank_module, sr_bank_instance,
                                                       sr_module_tail_port, sr_module_tail_port_info.pins()[ipin]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);

        /* Add net sink */
        module_manager.add_module_net_sink(top_module, net,
                                           top_module, 0,
                                           blsr_tail_port, blsr_tail_port_info.pins()[ipin]);
      }
    }
  }
}

/************************************************************** 
 * Add BL/WL nets from shift register module to each configurable child
 * BL pins of shift register module are connected to the BL input pins of each PB/CB/SB
 * For all the PB/CB/SB in the same column, they share the same set of BLs
 * A quick example
 *
 *  +-----------------------+
 *  |  Shift register chain |
 *  +-----------------------+
 *    BL[i .. i + sqrt(N)]     
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
 * @note optional BL/WL is applicable to WLR, which may not always exist
 */
static 
void add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_blwls(ModuleManager& module_manager,
                                                                       const ModuleId& top_module,
                                                                       const MemoryBankShiftRegisterBanks& sr_banks,
                                                                       const std::string& sr_blwl_port_name,
                                                                       const std::string& child_blwl_port_name,
                                                                       const bool& optional_blwl = false) {
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    for (size_t iinst = 0; iinst < sr_banks.shift_register_bank_modules(config_region).size(); ++iinst) {
      ModuleId sr_bank_module = sr_banks.shift_register_bank_modules(config_region)[iinst];
      size_t sr_bank_instance = sr_banks.shift_register_bank_instances(config_region)[iinst];
      VTR_ASSERT(sr_bank_module);

      ModulePortId sr_module_blwl_port = module_manager.find_module_port(sr_bank_module, sr_blwl_port_name);
      if (!sr_module_blwl_port && optional_blwl) {
        continue;
      }
      VTR_ASSERT(sr_module_blwl_port);
      BasicPort sr_module_blwl_port_info = module_manager.module_port(sr_bank_module, sr_module_blwl_port);

      size_t cur_sr_module_blwl_pin_id = 0;

      for (size_t sink_id = 0; sink_id < sr_banks.shift_register_bank_sink_child_ids(config_region, sr_bank_module, sr_bank_instance).size(); ++sink_id) {
        size_t child_id = sr_banks.shift_register_bank_sink_child_ids(config_region, sr_bank_module, sr_bank_instance)[sink_id];
        ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
        size_t child_instance = module_manager.region_configurable_child_instances(top_module, config_region)[child_id];

        /* Find the BL port */
        ModulePortId child_blwl_port = module_manager.find_module_port(child_module, child_blwl_port_name);
        BasicPort child_blwl_port_info = module_manager.module_port(child_module, child_blwl_port);

        /* Create net */
        ModuleNetId net = create_module_source_pin_net(module_manager, top_module,
                                                       sr_bank_module, sr_bank_instance,
                                                       sr_module_blwl_port,
                                                       sr_module_blwl_port_info.pins()[cur_sr_module_blwl_pin_id]);
        VTR_ASSERT(ModuleNetId::INVALID() != net);

        /* Add net sink */
        size_t sink_pin_id = sr_banks.shift_register_bank_sink_pin_ids(config_region, sr_bank_module, sr_bank_instance)[sink_id];
        module_manager.add_module_net_sink(top_module, net,
                                           child_module, child_instance, child_blwl_port, sink_pin_id);

        cur_sr_module_blwl_pin_id++;
      }
    }
  }
}

/*********************************************************************
 * This function to add nets for quicklogic memory banks using shift registers to manipulate BL/WLs
 * Each configuration region has independent BL/WL shift register banks
 * - Find the number of BLs and WLs required for each region
 * - Find the number of BL and WL shift register chains required for each region
 * - Create the module of shift register chain for each unique size 
 * - Create nets to connect from top-level module inputs to BL/WL shift register chains
 * - Create nets to connect from BL/WL shift register chains to BL/WL of configurable children
 *
 * @note this function only adds the BL protocol
 *
 * Detailed schematic of each memory bank:
 * @note The numbers are just made to show a simplified example, practical cases are more complicated!
 *
 *                                  sr_clk               sr_head     sr_tail
 *                                    |                   |             ^
 *                                    v                   v             |
 *                           +-------------------------------------------------+                  
 *                           |         Bit Line shift register chains          |
 *                           +-------------------------------------------------+                  
 *                               |              |                    | 
 *    +---------+              BL[0:9]        BL[10:17]          BL[18:22]
 *    |         |                |              |                    | 
 *    |         |                |              |                    |
 *    | Word    |--WL[0:3]-->-----------+---------------+----  ...   |------+-->        
 *    |         |                |      |       |       |            |      |
 *    | Line    |                |      v       |       v            |      v
 *    |         |                |   +-------+  |   +-------+        |  +------+
 *    | shift   |                +-->| SRAM  |  +-->| SRAM  |        +->| SRAM |
 *    |         |                |   | [0:8] |  |   | [0:5] |   ...  |  | [0:7]|
 *    | register|                |   +-------+  |   +-------+        |  +------+
 *    |         |                |              |                    |
 *    | chains  |--WL[4:14]  -----------+--------------+---------    | -----+-->        
 *    |         |                |      |       |       |            |      |
 *    |         |                |      v       |       v            |      v
 *    |         |                |   +-------+  |   +-------+        |  +-------+
 *    |         |                +-->| SRAM  |  |   | SRAM  |        +->| SRAM  |
 *    |         |                |   | [0:80]|  |   | [0:63]|   ...  |  | [0:31]|
 *    |         |                |   +-------+  |   +-------+        |  +-------+
 *    |         |                |                                   |
 *    |         |                |     ...     ...    ...            |    ...
 *    |         |                |              |                    |
 *    |         |--WL[15:18] -----------+---------------+----  ---   | -----+-->        
 *    |         |                |      |       |       |            |      |
 *    |         |                |      v       |       v            |      v
 *    +---------+                |   +-------+  |   +-------+        |  +-------+
 *                               +-->| SRAM  |  +-->| SRAM  |        +->| SRAM  |
 *                               |   |[0:5]  |  |   | [0:8] |   ...  |  | [0:2] |
 *                               |   +-------+  |   +-------+        |  +-------+
 *                               v              v                    v
 *                             WL[0:9]          WL[0:7]                WL[0:4]
 *               
 **********************************************************************/
static 
void add_top_module_nets_cmos_ql_memory_bank_bl_shift_register_config_bus(ModuleManager& module_manager,
                                                                          MemoryBankShiftRegisterBanks& sr_banks,
                                                                          const ModuleId& top_module,
                                                                          const CircuitLibrary& circuit_lib,
                                                                          const ConfigProtocol& config_protocol) {
  CircuitModelId sram_model = config_protocol.memory_model();
  CircuitModelId bl_memory_model = config_protocol.bl_memory_model();
  /* Find out the unique shift register chain sizes */
  vtr::vector<ConfigRegionId, size_t> unique_sr_sizes;
  unique_sr_sizes.resize(module_manager.regions(top_module).size());
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    /* TODO: Need to find how to cut the BLs when there are multiple banks for shift registers in a region */
    size_t num_bls = compute_memory_bank_regional_num_bls(module_manager, top_module,
                                                          config_region,
                                                          circuit_lib, sram_model);
    unique_sr_sizes[config_region] = num_bls; 
  }

  /* Build submodules for shift register chains */ 
  for (const size_t& sr_size : unique_sr_sizes) {
    std::string sr_module_name = generate_bl_shift_register_module_name(circuit_lib.model_name(bl_memory_model), sr_size); 
    build_bl_shift_register_chain_module(module_manager,
                                         circuit_lib,
                                         sr_module_name,
                                         bl_memory_model,
                                         sr_size);
  }

  /* Instanciate the shift register chains in the top-level module */ 
  sr_banks.resize_regions(module_manager.regions(top_module).size());
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    std::string sr_module_name = generate_bl_shift_register_module_name(circuit_lib.model_name(bl_memory_model), unique_sr_sizes[config_region]); 
    ModuleId sr_bank_module = module_manager.find_module(sr_module_name);
    VTR_ASSERT(sr_bank_module);

    size_t cur_inst = module_manager.num_instance(top_module, sr_bank_module);
    module_manager.add_child_module(top_module, sr_bank_module);

    sr_banks.add_shift_register_instance(config_region, sr_bank_module, cur_inst);

    /************************************************************** 
     * Precompute the BLs and WLs distribution across the FPGA fabric
     * The distribution is a matrix which contains the starting index of BL/WL for each column or row
     */
    std::pair<int, int> child_x_range = compute_memory_bank_regional_configurable_child_x_range(module_manager, top_module, config_region);
    std::map<int, size_t> num_bls_per_tile = compute_memory_bank_regional_bitline_numbers_per_tile(module_manager, top_module,
                                                                                                   config_region,
                                                                                                   circuit_lib, sram_model);
    std::map<int, size_t> bl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_x_range, num_bls_per_tile);

    for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
      ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
      vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 

      /* Find the BL port */
      ModulePortId child_bl_port = module_manager.find_module_port(child_module, std::string(MEMORY_BL_PORT_NAME));
      BasicPort child_bl_port_info = module_manager.module_port(child_module, child_bl_port);

      size_t cur_bl_index = 0;

      for (const size_t& sink_bl_pin : child_bl_port_info.pins()) {
        size_t bl_pin_id = bl_start_index_per_tile[coord.x()] + cur_bl_index;

        sr_banks.add_shift_register_sink_nodes(config_region, sr_bank_module, cur_inst, child_id, sink_bl_pin);
        sr_banks.add_shift_register_sink_blwls(config_region, sr_bank_module, cur_inst, bl_pin_id);
      }
    }
  }

  /* Create connections between top-level module and the BL shift register banks 
   * - Connect the head port from top-level module to each shift register bank
   * - Connect the tail port from each shift register bank to top-level module
   */
  add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_heads(module_manager, top_module, sr_banks, 
                                                                    std::string(BL_SHIFT_REGISTER_CHAIN_HEAD_NAME));

  add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_tails(module_manager, top_module, sr_banks, 
                                                                    std::string(BL_SHIFT_REGISTER_CHAIN_TAIL_NAME));

  /* Create connections between BLs of top-level module and BLs of child modules for each region */
  add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_blwls(module_manager, top_module, sr_banks,
                                                                    std::string(BL_SHIFT_REGISTER_CHAIN_BL_OUT_NAME),
                                                                    std::string(MEMORY_BL_PORT_NAME));
}

/*********************************************************************
 * Top-level function to add nets for quicklogic memory banks using shift registers to control BL/WLs
 *
 * @note this function only adds the WL configuration bus
 *
 * @note see detailed explanation on the bus connection in function add_top_module_nets_cmos_ql_memory_bank_bl_shift_register_config_bus()
 **********************************************************************/
static 
void add_top_module_nets_cmos_ql_memory_bank_wl_shift_register_config_bus(ModuleManager& module_manager,
                                                                          MemoryBankShiftRegisterBanks& sr_banks,
                                                                          const ModuleId& top_module,
                                                                          const CircuitLibrary& circuit_lib,
                                                                          const ConfigProtocol& config_protocol) {
  CircuitModelId sram_model = config_protocol.memory_model();
  CircuitModelId wl_memory_model = config_protocol.wl_memory_model();
  /* Find out the unique shift register chain sizes */
  vtr::vector<ConfigRegionId, size_t> unique_sr_sizes;
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    /* TODO: Need to find how to cut the BLs when there are multiple banks for shift registers in a region */
    size_t num_wls = compute_memory_bank_regional_num_wls(module_manager, top_module,
                                                          config_region,
                                                          circuit_lib, sram_model);
    unique_sr_sizes.push_back(num_wls); 
  }

  /* TODO: Build submodules for shift register chains */ 
  for (const size_t& sr_size : unique_sr_sizes) {
    std::string sr_module_name = generate_wl_shift_register_module_name(circuit_lib.model_name(wl_memory_model), sr_size); 
    build_wl_shift_register_chain_module(module_manager,
                                         circuit_lib,
                                         sr_module_name,
                                         wl_memory_model,
                                         sr_size);
  }

  /* Instanciate the shift register chains in the top-level module */ 
  sr_banks.resize_regions(module_manager.regions(top_module).size());
  for (const ConfigRegionId& config_region : module_manager.regions(top_module)) {
    std::string sr_module_name = generate_wl_shift_register_module_name(circuit_lib.model_name(wl_memory_model), unique_sr_sizes[config_region]); 
    ModuleId sr_bank_module = module_manager.find_module(sr_module_name);
    VTR_ASSERT(sr_bank_module);

    size_t cur_inst = module_manager.num_instance(top_module, sr_bank_module);
    module_manager.add_child_module(top_module, sr_bank_module);

    sr_banks.add_shift_register_instance(config_region, sr_bank_module, cur_inst);

    /************************************************************** 
     * Precompute the BLs and WLs distribution across the FPGA fabric
     * The distribution is a matrix which contains the starting index of BL/WL for each column or row
     */
    std::pair<int, int> child_y_range = compute_memory_bank_regional_configurable_child_y_range(module_manager, top_module, config_region);
    std::map<int, size_t> num_wls_per_tile = compute_memory_bank_regional_wordline_numbers_per_tile(module_manager, top_module,
                                                                                                    config_region,
                                                                                                    circuit_lib, sram_model);
    std::map<int, size_t> wl_start_index_per_tile = compute_memory_bank_regional_blwl_start_index_per_tile(child_y_range, num_wls_per_tile);

    for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
      ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
      vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 

      size_t cur_wl_index = 0;

      /* Find the WL port */
      ModulePortId child_wl_port = module_manager.find_module_port(child_module, std::string(MEMORY_WL_PORT_NAME));
      BasicPort child_wl_port_info = module_manager.module_port(child_module, child_wl_port);

      for (const size_t& sink_wl_pin : child_wl_port_info.pins()) {
        size_t wl_pin_id = wl_start_index_per_tile[coord.y()] + cur_wl_index;
        sr_banks.add_shift_register_sink_nodes(config_region, sr_bank_module, cur_inst, child_id, sink_wl_pin);
        sr_banks.add_shift_register_sink_blwls(config_region, sr_bank_module, cur_inst, wl_pin_id);
      }
    }
  }

  /* Create connections between top-level module and the BL shift register banks 
   * - Connect the head port from top-level module to each shift register bank
   * - Connect the tail port from each shift register bank to top-level module
   */
  add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_heads(module_manager, top_module, sr_banks, 
                                                                    std::string(WL_SHIFT_REGISTER_CHAIN_HEAD_NAME));

  add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_tails(module_manager, top_module, sr_banks, 
                                                                    std::string(WL_SHIFT_REGISTER_CHAIN_TAIL_NAME));

  /* Create connections between BLs of top-level module and BLs of child modules for each region */
  add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_blwls(module_manager, top_module, sr_banks,
                                                                    std::string(WL_SHIFT_REGISTER_CHAIN_WL_OUT_NAME),
                                                                    std::string(MEMORY_WL_PORT_NAME));
  add_top_module_nets_cmos_ql_memory_bank_shift_register_bank_blwls(module_manager, top_module, sr_banks,
                                                                    std::string(WL_SHIFT_REGISTER_CHAIN_WLR_OUT_NAME),
                                                                    std::string(MEMORY_WLR_PORT_NAME),
                                                                    true);
}

/*********************************************************************
 * Top-level function to add nets for quicklogic memory banks
 * - Each configuration region has independent memory bank circuitry
 * - BL and WL may have different circuitry and wire connection, e.g., decoder, flatten or shift-registers
 * - BL control circuitry
 *   - Decoder: Add a BL decoder; Connect enable, address and data-in (din) between top-level and decoders; Connect data ports between between the decoder and configurable child modules
 *   - Flatten: Connect BLs between the top-level port and configurable child modules
 *   - TODO: Shift registers: add blocks of shift register chain (could be multi-head); Connect shift register outputs to configurable child modules
 *
 * - WL control circuitry
 *   - Decoder: Add a WL decoder; Connect address ports between top-level and decoders; Connect data ports between the decoder and configurable child modules
 *   - Flatten: Connect BLs between the top-level port and configurable child modules
 *   - TODO: Shift registers: add blocks of shift register chain (could be multi-head); Connect shift register outputs to configurable child modules
 ********************************************************************/
void add_top_module_nets_cmos_ql_memory_bank_config_bus(ModuleManager& module_manager,
                                                        DecoderLibrary& decoder_lib,
                                                        std::array<MemoryBankShiftRegisterBanks, 2>& blwl_sr_banks,
                                                        const ModuleId& top_module,
                                                        const CircuitLibrary& circuit_lib,
                                                        const ConfigProtocol& config_protocol,
                                                        const TopModuleNumConfigBits& num_config_bits) {
  VTR_ASSERT_SAFE(CONFIG_MEM_QL_MEMORY_BANK == config_protocol.type());
  CircuitModelId sram_model = config_protocol.memory_model();

  switch (config_protocol.bl_protocol_type()) {
    case BLWL_PROTOCOL_DECODER: {
      add_top_module_nets_cmos_ql_memory_bank_bl_decoder_config_bus(module_manager, decoder_lib, top_module, circuit_lib, sram_model, num_config_bits);
      break;
    }
    case BLWL_PROTOCOL_FLATTEN: {
      add_top_module_nets_cmos_ql_memory_bank_bl_flatten_config_bus(module_manager, top_module, circuit_lib, sram_model);
      break;
    }
    case BLWL_PROTOCOL_SHIFT_REGISTER: {
      add_top_module_nets_cmos_ql_memory_bank_bl_shift_register_config_bus(module_manager, blwl_sr_banks[0], top_module, circuit_lib, config_protocol);
      break;
    }
    default: {
      VTR_LOG_ERROR("Invalid BL protocol");
      exit(1);
    }
  }

  switch (config_protocol.wl_protocol_type()) {
    case BLWL_PROTOCOL_DECODER: {
      add_top_module_nets_cmos_ql_memory_bank_wl_decoder_config_bus(module_manager, decoder_lib, top_module, circuit_lib, sram_model, num_config_bits);
      break;
    }
    case BLWL_PROTOCOL_FLATTEN: {
      add_top_module_nets_cmos_ql_memory_bank_wl_flatten_config_bus(module_manager, top_module, circuit_lib, sram_model);
      break;
    }
    case BLWL_PROTOCOL_SHIFT_REGISTER: {
      add_top_module_nets_cmos_ql_memory_bank_wl_shift_register_config_bus(module_manager, blwl_sr_banks[1], top_module, circuit_lib, config_protocol);
      break;
    }
    default: {
      VTR_LOG_ERROR("Invalid WL protocol");
      exit(1);
    }
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
      /* Each region will have independent shift register banks */
      for (const ConfigRegionId& config_region : module_manager.regions(module_id)) {
        size_t num_heads = config_protocol.bl_num_banks();
        BasicPort blsr_head_port(generate_regional_blwl_port_name(std::string(BL_SHIFT_REGISTER_CHAIN_HEAD_NAME), config_region), num_heads);
        module_manager.add_port(module_id, blsr_head_port, ModuleManager::MODULE_INPUT_PORT);
        BasicPort blsr_tail_port(generate_regional_blwl_port_name(std::string(BL_SHIFT_REGISTER_CHAIN_TAIL_NAME), config_region), num_heads);
        module_manager.add_port(module_id, blsr_tail_port, ModuleManager::MODULE_INPUT_PORT);
      }
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
        size_t wl_size = num_config_bits[config_region].second;
        BasicPort wl_port(generate_regional_blwl_port_name(std::string(MEMORY_WL_PORT_NAME), config_region), wl_size);
        module_manager.add_port(module_id, wl_port, ModuleManager::MODULE_INPUT_PORT);

        /* Optional: If we have WLR port, we should add a read-back port */
        if (!circuit_lib.model_ports_by_type(sram_model, CIRCUIT_MODEL_PORT_WLR).empty()) {
          BasicPort wlr_port(generate_regional_blwl_port_name(std::string(MEMORY_WLR_PORT_NAME), config_region), wl_size);
          module_manager.add_port(module_id, wlr_port, ModuleManager::MODULE_INPUT_PORT);
        }
      }
      break;
    }
    case BLWL_PROTOCOL_SHIFT_REGISTER: {
      /* Each region will have independent shift register banks */
      for (const ConfigRegionId& config_region : module_manager.regions(module_id)) {
        size_t num_heads = config_protocol.wl_num_banks();
        BasicPort wlsr_head_port(generate_regional_blwl_port_name(std::string(WL_SHIFT_REGISTER_CHAIN_HEAD_NAME), config_region), num_heads);
        module_manager.add_port(module_id, wlsr_head_port, ModuleManager::MODULE_INPUT_PORT);
        BasicPort wlsr_tail_port(generate_regional_blwl_port_name(std::string(WL_SHIFT_REGISTER_CHAIN_TAIL_NAME), config_region), num_heads);
        module_manager.add_port(module_id, wlsr_tail_port, ModuleManager::MODULE_INPUT_PORT);
      }
      break;
    }
    default: {
      VTR_LOG_ERROR("Invalid WL protocol");
      exit(1);
    }
  }
}

} /* end namespace openfpga */
