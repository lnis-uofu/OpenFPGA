/*********************************************************************
 * This file includes functions to generate Verilog submodules for 
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 ********************************************************************/
#include <string>
#include <algorithm>

#include "util.h"
#include "vtr_assert.h"

/* Device-level header files */
#include "mux_graph.h"
#include "module_manager.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "circuit_library_utils.h"
#include "mux_utils.h"

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_memory.h"

/*********************************************************************
 * Flat memory modules 
 *
 *        in[0]        in[1]           in[N]
 *          |            |               |
 *          v            v               v
 *      +-------+    +-------+       +-------+
 *      | SRAM  |    | SRAM  |  ...  | SRAM  |
 *      |  [0]  |    |  [1]  |       | [N-1] |
 *      +-------+    +-------+       +-------+
 *          |            |      ...      |
 *          v            v               v
 *      +------------------------------------+
 *      |   Multiplexer Configuration port   |
 *
 ********************************************************************/
static 
void print_verilog_memory_standalone_module(ModuleManager& module_manager,
                                            const CircuitLibrary& circuit_lib,
                                            std::fstream& fp,
                                            const std::string& module_name,
                                            const CircuitModelId& sram_model,
                                            const size_t& num_mems) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Create a module and add to the module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Get the global ports required by the SRAM */
  std::vector<enum e_spice_model_port_type> global_port_types;
  global_port_types.push_back(SPICE_MODEL_PORT_CLOCK);
  global_port_types.push_back(SPICE_MODEL_PORT_INPUT);
  std::vector<CircuitPortId> sram_global_ports = circuit_lib.model_global_ports_by_type(sram_model, global_port_types, true, false);
  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_INPUT, true);
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_OUTPUT, true);
  
  /* Add module ports: the ports come from the SRAM modules */
  /* Add each global port */
  for (const auto& port : sram_global_ports) {
    /* Configure each global port: global ports are shared among the SRAMs, so it is independent from the memory size */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  for (const auto& port : sram_input_ports) {
    BasicPort input_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
  }
  /* Add each output port: port width should match the number of memories */
  for (const auto& port : sram_output_ports) {
    BasicPort output_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish dumping ports */

  /* Find the sram module in the module manager */
  ModuleId sram_module_id = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    /* Create a port-to-port map */
    std::map<std::string, BasicPort> port2port_name_map;
    /* Map instance inputs [i] to SRAM module input */
    for (const auto& port : sram_input_ports) {
      BasicPort instance_input_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_input_port; 
    }
    /* Map instance outputs [i] to SRAM module input */
    for (const auto& port : sram_output_ports) {
      BasicPort instance_output_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_output_port; 
    }

    /* Output an instance of the module */
    print_verilog_module_instance(fp, module_manager, module_id, sram_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(sram_model));
    /* IMPORTANT: this update MUST be called after the instance outputting!!!!
     * update the module manager with the relationship between the parent and child modules 
     */
    module_manager.add_child_module(module_id, sram_module_id);
  }

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);
}

/*********************************************************************
 * Scan-chain organization
 *              
 *                +-------+    +-------+            +-------+
 *  scan-chain--->| CCFF  |--->| CCFF  |--->... --->| CCFF  |---->scan-chain
 *  input&clock   |  [0]  |    |  [1]  |            | [N-1] |       output
 *                +-------+    +-------+            +-------+
 *                    |            |      ...           |
 *                    v            v                    v
 *                +-----------------------------------------+
 *                |   Multiplexer Configuration port        |
 *
 ********************************************************************/
static 
void print_verilog_memory_chain_module(ModuleManager& module_manager,
                                       const CircuitLibrary& circuit_lib,
                                       std::fstream& fp,
                                       const std::string& module_name,
                                       const CircuitModelId& sram_model,
                                       const size_t& num_mems) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Create a module and add to the module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Get the global ports required by the SRAM */
  std::vector<enum e_spice_model_port_type> global_port_types;
  global_port_types.push_back(SPICE_MODEL_PORT_CLOCK);
  global_port_types.push_back(SPICE_MODEL_PORT_INPUT);
  std::vector<CircuitPortId> sram_global_ports = circuit_lib.model_global_ports_by_type(sram_model, global_port_types, true, false);
  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_INPUT, true);
  /* Should have only 1 input port */
  VTR_ASSERT( 1 == sram_input_ports.size() );
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_OUTPUT, true);
  /* Should have only 1 or 2 output port */
  VTR_ASSERT( (1 == sram_output_ports.size()) || ( 2 == sram_output_ports.size()) );
  
  /* Add module ports: the ports come from the SRAM modules */
  /* Add each global port */
  for (const auto& port : sram_global_ports) {
    /* Configure each global port: global ports are shared among the SRAMs, so it is independent from the memory size */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add an input port, which is the head of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first input of the CCFF model as the D port,
   * which will be connected to the head of the chain
   */
  BasicPort chain_head_port(generate_configuration_chain_head_name(), 
                            circuit_lib.port_size(sram_input_ports[0]));
  module_manager.add_port(module_id, chain_head_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add an output port, which is the tail of configuration chain in the module */
  /* TODO: restriction!!!
   * consider only the first output of the CCFF model as the Q port,
   * which will be connected to the tail of the chain
   */
  BasicPort chain_tail_port(generate_configuration_chain_tail_name(), 
                            circuit_lib.port_size(sram_output_ports[0]));
  module_manager.add_port(module_id, chain_tail_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each output port: port width should match the number of memories */
  for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
    std::string port_name;
    if (0 == iport) {
      port_name = generate_configuration_chain_data_out_name();
    } else {
      VTR_ASSERT( 1 == iport);
      port_name = generate_configuration_chain_inverted_data_out_name();
    }
    BasicPort output_port(port_name, num_mems);
    module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish dumping ports */

  /* Find the sram module in the module manager */
  ModuleId sram_module_id = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    /* Create a port-to-port map */
    std::map<std::string, BasicPort> port2port_name_map;
    /* Map instance inputs [i] to SRAM module input */
    for (const auto& port : sram_input_ports) {
      BasicPort instance_input_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_input_port; 
    }
    /* Map instance outputs [i] to SRAM module input */
    for (size_t iport = 0; iport < sram_output_ports.size(); ++iport) {
      std::string port_name;
      if (0 == iport) {
        port_name = generate_configuration_chain_data_out_name();
      } else {
        VTR_ASSERT( 1 == iport);
        port_name = generate_configuration_chain_inverted_data_out_name();
      }
      BasicPort instance_output_port(port_name, i, i);
      port2port_name_map[circuit_lib.port_lib_name(sram_output_ports[iport])] = instance_output_port; 
    }

    /* Output an instance of the module */
    print_verilog_module_instance(fp, module_manager, module_id, sram_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(sram_model));
    /* IMPORTANT: this update MUST be called after the instance outputting!!!!
     * update the module manager with the relationship between the parent and child modules 
     */
    module_manager.add_child_module(module_id, sram_module_id);
  }

  /* Wire the memory cells into a chain
   * The head of the chain will be wired to the input port of the first CCFF 
   * The tail of the chain will be wired to the output port of the last CCFF 
   * The output of each CCFF will be wired to the input of the next CCFFF in the chain
   */
  BasicPort first_ccff_input_port(circuit_lib.port_lib_name(sram_input_ports[0]), 0, 0);
  print_verilog_wire_connection(fp, first_ccff_input_port, chain_head_port, false); 

  BasicPort last_ccff_output_port(generate_configuration_chain_data_out_name(), num_mems - 1, num_mems - 1);
  print_verilog_wire_connection(fp, chain_tail_port, last_ccff_output_port, false); 

  BasicPort chain_output_port(generate_configuration_chain_data_out_name(), 0, num_mems - 2);
  BasicPort chain_input_port(circuit_lib.port_lib_name(sram_input_ports[0]), 1, num_mems - 1);
  print_verilog_wire_connection(fp, chain_input_port, chain_output_port, false); 

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);
}

/*********************************************************************
 * Memory bank organization
 *
 *           Bit lines(BL/BLB) Word lines (WL/WLB)
 *               |               |
 *               v               v
 *      +------------------------------------+
 *      |   Memory Module Configuration port |
 *      +------------------------------------+
 *          |            |               |
 *          v            v               v
 *      +-------+    +-------+       +-------+
 *      | SRAM  |    | SRAM  |  ...  | SRAM  |
 *      |  [0]  |    |  [1]  |       | [N-1] |
 *      +-------+    +-------+       +-------+
 *          |            |      ...      |
 *          v            v               v
 *      +------------------------------------+
 *      |   Multiplexer Configuration port   |
 *
 ********************************************************************/
static 
void print_verilog_memory_bank_module(ModuleManager& module_manager,
                                      const CircuitLibrary& circuit_lib,
                                      std::fstream& fp,
                                      const std::string& module_name,
                                      const CircuitModelId& sram_model,
                                      const size_t& num_mems) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Create a module and add to the module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Get the global ports required by the SRAM */
  std::vector<enum e_spice_model_port_type> global_port_types;
  global_port_types.push_back(SPICE_MODEL_PORT_CLOCK);
  global_port_types.push_back(SPICE_MODEL_PORT_INPUT);
  std::vector<CircuitPortId> sram_global_ports = circuit_lib.model_global_ports_by_type(sram_model, global_port_types, true, false);
  /* Get the input ports from the SRAM */
  std::vector<CircuitPortId> sram_input_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_INPUT, true);
  /* A SRAM cell with BL/WL should not have any input */
  VTR_ASSERT( 0 == sram_input_ports.size() );
  /* Get the output ports from the SRAM */
  std::vector<CircuitPortId> sram_output_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_OUTPUT, true);
  /* Get the BL/WL ports from the SRAM */
  std::vector<CircuitPortId> sram_bl_ports  = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_BL, true);
  std::vector<CircuitPortId> sram_blb_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> sram_wl_ports  = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_WL, true);
  std::vector<CircuitPortId> sram_wlb_ports = circuit_lib.model_ports_by_type(sram_model, SPICE_MODEL_PORT_WLB, true);
  
  /* Add module ports: the ports come from the SRAM modules */
  /* Add each global port */
  for (const auto& port : sram_global_ports) {
    /* Configure each global port: global ports are shared among the SRAMs, so it is independent from the memory size */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  for (const auto& port : sram_input_ports) {
    BasicPort input_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
  }
  /* Add each output port: port width should match the number of memories */
  for (const auto& port : sram_output_ports) {
    BasicPort output_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }
  /* Add each output port: port width should match the number of memories */
  for (const auto& port : sram_bl_ports) {
    BasicPort bl_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, bl_port, ModuleManager::MODULE_INPUT_PORT);
  }
  for (const auto& port : sram_blb_ports) {
    BasicPort blb_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, blb_port, ModuleManager::MODULE_INPUT_PORT);
  }
  for (const auto& port : sram_wl_ports) {
    BasicPort wl_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, wl_port, ModuleManager::MODULE_INPUT_PORT);
  }
  for (const auto& port : sram_wlb_ports) {
    BasicPort wlb_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, wlb_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);
  /* Finish dumping ports */

  /* Find the sram module in the module manager */
  ModuleId sram_module_id = module_manager.find_module(circuit_lib.model_name(sram_model));

  /* Instanciate each submodule */
  for (size_t i = 0; i < num_mems; ++i) {
    /* Create a port-to-port map */
    std::map<std::string, BasicPort> port2port_name_map;
    /* Map instance inputs [i] to SRAM module input */
    for (const auto& port : sram_input_ports) {
      BasicPort instance_input_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_input_port; 
    }
    /* Map instance outputs [i] to SRAM module input */
    for (const auto& port : sram_output_ports) {
      BasicPort instance_output_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_output_port; 
    }
    /* Map instance BL[i] and WL[i] to SRAM module input */
    for (const auto& port : sram_bl_ports) {
      BasicPort instance_bl_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_bl_port; 
    }
    for (const auto& port : sram_blb_ports) {
      BasicPort instance_blb_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_blb_port; 
    }
    for (const auto& port : sram_wl_ports) {
      BasicPort instance_wl_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_wl_port; 
    }
    for (const auto& port : sram_wlb_ports) {
      BasicPort instance_wlb_port(circuit_lib.port_lib_name(port), i, i);
      port2port_name_map[circuit_lib.port_lib_name(port)] = instance_wlb_port; 
    }

    /* Output an instance of the module */
    print_verilog_module_instance(fp, module_manager, module_id, sram_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(sram_model));
    /* IMPORTANT: this update MUST be called after the instance outputting!!!!
     * update the module manager with the relationship between the parent and child modules 
     */
    module_manager.add_child_module(module_id, sram_module_id);
  }

  /* TODO: if a local memory decoder is required, instanciate it here */

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);
}


/*********************************************************************
 * Generate Verilog modules for the memories that are used
 * by a circuit model 
 * The organization of memory circuit will depend on the style of
 * configuration protocols
 * Currently, we support 
 * 1. Flat SRAM organization
 * 2. Configuration chain
 * 3. Memory bank (memory decoders)
 ********************************************************************/
static 
void print_verilog_memory_module(ModuleManager& module_manager,
                                 const CircuitLibrary& circuit_lib,
                                 const e_sram_orgz& sram_orgz_type,
                                 std::fstream& fp,
                                 const std::string& module_name,
                                 const CircuitModelId& sram_model,
                                 const size_t& num_mems) {
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    print_verilog_memory_standalone_module(module_manager, circuit_lib, fp, 
                                           module_name, sram_model, num_mems);
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    print_verilog_memory_chain_module(module_manager, circuit_lib, fp, 
                                      module_name, sram_model, num_mems);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    print_verilog_memory_bank_module(module_manager, circuit_lib, fp, 
                                     module_name, sram_model, num_mems);
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, LINE%d) Invalid SRAM organization!\n",
               __FILE__, __LINE__);
  }
}


/*********************************************************************
 * Generate Verilog modules for the memories that are used
 * by multiplexers  
 *
 *            +----------------+
 * mem_in --->|  Memory Module |---> mem_out
 *            +----------------+
 *              |  |  ... |  | 
 *              v  v      v  v SRAM ports of multiplexer
 *          +---------------------+
 *    in--->|  Multiplexer Module |---> out
 *          +---------------------+
 ********************************************************************/
static 
void print_verilog_mux_memory_module(ModuleManager& module_manager,
                                     const CircuitLibrary& circuit_lib,
                                     const e_sram_orgz& sram_orgz_type,
                                     std::fstream& fp,
                                     const CircuitModelId& mux_model,
                                     const MuxGraph& mux_graph) {
  /* Find the actual number of configuration bits, based on the mux graph 
   * Due to the use of local decoders inside mux, this may be 
   */
  size_t num_config_bits = find_mux_num_config_bits(circuit_lib, mux_model, mux_graph, sram_orgz_type);
  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(mux_model)) {
  case SPICE_MODEL_DESIGN_CMOS: {
    /* Generate module name */
    std::string module_name = generate_mux_subckt_name(circuit_lib, mux_model, 
                                                       find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs()), 
                                                       std::string(verilog_mem_posfix));

    /* Get the sram ports from the mux */
    std::vector<CircuitModelId> sram_models = find_circuit_sram_models(circuit_lib, mux_model);
    VTR_ASSERT( 1 == sram_models.size() );

    print_verilog_memory_module(module_manager, circuit_lib, sram_orgz_type, fp, module_name, sram_models[0], num_config_bits);
    break;
  }
  case SPICE_MODEL_DESIGN_RRAM:
    /* We do not need a memory submodule for RRAM MUX,
     * RRAM are embedded in the datapath  
     * TODO: generate local encoders for RRAM-based multiplexers here!!!
     */ 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d]) Invalid design technology of multiplexer (name: %s)\n",
               __FILE__, __LINE__, circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }
}


/*********************************************************************
 * Generate Verilog modules for 
 * the memories that are affiliated to multiplexers and other programmable
 * circuit models, such as IOPADs, LUTs, etc.
 *
 * We keep the memory modules separated from the multiplexers and other 
 * programmable circuit models, for the sake of supporting
 * various configuration schemes.
 * By following such organiztion, the Verilog modules of the circuit models
 * implements the functionality (circuit logic) only, while the memory Verilog
 * modules implements the memory circuits as well as configuration protocols.
 * For example, the local decoders of multiplexers are implemented in the
 * memory modules. 
 * Take another example, the memory circuit can implement the scan-chain or 
 * memory-bank organization for the memories.
 ********************************************************************/
void print_verilog_submodule_memories(ModuleManager& module_manager,
                                      const MuxLibrary& mux_lib,
                                      const CircuitLibrary& circuit_lib,
                                      const e_sram_orgz& sram_orgz_type,
                                      const std::string& verilog_dir,
                                      const std::string& submodule_dir) {
  /* Plug in with the mux subckt */
  std::string verilog_fname(submodule_dir + memories_verilog_file_name);
  verilog_fname += ".bak";

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Print out debugging information for if the file is not opened/created properly */
  vpr_printf(TIO_MESSAGE_INFO,
             "Creating Verilog netlist for memories (%s) ...\n",
             verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Memories used in FPGA"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);
  
  /* Create the memory circuits for the multiplexer */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_model = mux_lib.mux_circuit_model(mux); 
    /* Bypass the non-MUX circuit models (i.e., LUTs). 
     * They should be handled in a different way 
     * Memory circuits of LUT includes both regular and mode-select ports
     */
    if (SPICE_MODEL_MUX != circuit_lib.model_type(mux_model)) {
      continue;
    }
    /* Create a Verilog module for the memories used by the multiplexer */
    print_verilog_mux_memory_module(module_manager, circuit_lib, sram_orgz_type, fp, mux_model, mux_graph);
  }

  /* Create the memory circuits for non-MUX circuit models.
   * In this case, the memory modules are designed to interface
   * the mode-select ports 
   */
  for (const auto& model : circuit_lib.models()) {
    /* Bypass MUXes, they have already been considered */
    if (SPICE_MODEL_MUX == circuit_lib.model_type(model)) {
      continue;
    }
    /* Bypass those modules without any SRAM ports */
    std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(model, SPICE_MODEL_PORT_SRAM, true);
    if (0 == sram_ports.size()) {
      continue;
    }
    /* Find the name of memory module */
    /* Get the total number of SRAMs */
    size_t num_mems = 0; 
    for (const auto& port : sram_ports) {
      num_mems += circuit_lib.port_size(port);
    }
    /* Get the circuit model for the memory circuit used by the multiplexer */
    std::vector<CircuitModelId> sram_models;
    for (const auto& port : sram_ports) {
      CircuitModelId sram_model = circuit_lib.port_tri_state_model(port);
      VTR_ASSERT(CircuitModelId::INVALID() != sram_model);
      /* Found in the vector of sram_models, do not update and go to the next */
      if (sram_models.end() != std::find(sram_models.begin(), sram_models.end(), sram_model)) {
        continue;
      }
      /* sram_model not found in the vector, update the sram_models */
      sram_models.push_back(sram_model);
    }
    /* Should have only 1 SRAM model */
    VTR_ASSERT( 1 == sram_models.size() );
  
    /* Create the module name for the memory block */
    std::string module_name = generate_memory_module_name(circuit_lib, model, sram_models[0], std::string(verilog_mem_posfix));

    /* Create a Verilog module for the memories used by the circuit model */
    print_verilog_memory_module(module_manager, circuit_lib, sram_orgz_type, fp, module_name, sram_models[0], num_mems);
  }

  /* Close the file stream */
  fp.close();

  /* Add fname to the linked list
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_fname.c_str());  
   */
}

