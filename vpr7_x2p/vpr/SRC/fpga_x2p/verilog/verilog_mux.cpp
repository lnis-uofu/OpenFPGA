/***********************************************
 * This file includes functions to generate
 * Verilog submodules for multiplexers.
 * including both fundamental submodules
 * such as a branch in a multiplexer 
 * and the full multiplexer
 **********************************************/
#include <string>

#include "util.h"
#include "vtr_assert.h"

/* Device-level header files */
#include "mux_graph.h"
#include "module_manager.h"
#include "physical_types.h"
#include "vpr_types.h"
#include "mux_utils.h"

/* FPGA-X2P context header files */
#include "spice_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

/* FPGA-Verilog context header files */
#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_mux.h"

/*********************************************************************
 * Generate structural Verilog codes (consist of transmission-gates or
 * pass-transistor) modeling an branch circuit 
 * for a multiplexer with the given size 
 *********************************************************************/
static 
void generate_verilog_cmos_mux_branch_body_structural(ModuleManager& module_manager,
                                                      const CircuitLibrary& circuit_lib, 
                                                      std::fstream& fp,
                                                      const CircuitModelId& tgate_model, 
                                                      const ModuleId& module_id, 
                                                      const BasicPort& input_port,
                                                      const BasicPort& output_port,
                                                      const BasicPort& mem_port,
                                                      const BasicPort& mem_inv_port,
                                                      const MuxGraph& mux_graph) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Get the module id of tgate in Module manager */
  ModuleId tgate_module_id = module_manager.find_module(circuit_lib.model_name(tgate_model));
  VTR_ASSERT(ModuleId::INVALID() != tgate_module_id);

  /* TODO: move to check_circuit_library? Get model ports of tgate */
  std::vector<CircuitPortId> tgate_input_ports = circuit_lib.model_ports_by_type(tgate_model, SPICE_MODEL_PORT_INPUT, true);
  std::vector<CircuitPortId> tgate_output_ports = circuit_lib.model_ports_by_type(tgate_model, SPICE_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(3 == tgate_input_ports.size());
  VTR_ASSERT(1 == tgate_output_ports.size());

  /* Verilog Behavior description for a MUX */
  print_verilog_comment(fp, std::string("---- Structure-level description -----"));

  /* Output the netlist following the connections in mux_graph */
  /* Iterate over the inputs */
  for (const auto& mux_input : mux_graph.inputs()) {
    BasicPort cur_input_port(input_port.get_name(), size_t(mux_graph.input_id(mux_input)), size_t(mux_graph.input_id(mux_input)));
    /* Iterate over the outputs */
    for (const auto& mux_output : mux_graph.outputs()) {
      BasicPort cur_output_port(output_port.get_name(), size_t(mux_graph.output_id(mux_output)), size_t(mux_graph.output_id(mux_output)));
      /* if there is a connection between the input and output, a tgate will be outputted */
      std::vector<MuxEdgeId> edges = mux_graph.find_edges(mux_input, mux_output);
      /* There should be only one edge or no edge*/
      VTR_ASSERT((1 == edges.size()) || (0 == edges.size()));
      /* No need to output tgates if there are no edges between two nodes */
      if (0 == edges.size()) {
        continue;
      }
      /* TODO: Output a tgate use a module manager */
      /* Create a port-to-port name map */
      std::map<std::string, BasicPort> port2port_name_map;
      /* input port */
      port2port_name_map[circuit_lib.port_lib_name(tgate_input_ports[0])] = cur_input_port;
      /* output port */
      port2port_name_map[circuit_lib.port_lib_name(tgate_output_ports[0])] = cur_output_port;
      /* Find the mem_id controlling the edge */
      MuxMemId mux_mem = mux_graph.find_edge_mem(edges[0]);
      BasicPort cur_mem_port(mem_port.get_name(), size_t(mux_mem), size_t(mux_mem));
      BasicPort cur_mem_inv_port(mem_inv_port.get_name(), size_t(mux_mem), size_t(mux_mem));
      /* mem port */
      if (false == mux_graph.is_edge_use_inv_mem(edges[0])) {
        /* wire mem to mem of module, and wire mem_inv to mem_inv of module */
        port2port_name_map[circuit_lib.port_lib_name(tgate_input_ports[1])] = cur_mem_port;
        port2port_name_map[circuit_lib.port_lib_name(tgate_input_ports[2])] = cur_mem_inv_port;
      } else {
        /* wire mem_inv to mem of module, wire mem to mem_inv of module */
        port2port_name_map[circuit_lib.port_lib_name(tgate_input_ports[1])] = cur_mem_inv_port;
        port2port_name_map[circuit_lib.port_lib_name(tgate_input_ports[2])] = cur_mem_port;
      }  
      /* Output an instance of the module */
      print_verilog_module_instance(fp, module_manager, module_id, tgate_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(tgate_model));
      /* IMPORTANT: this update MUST be called after the instance outputting!!!!
       * update the module manager with the relationship between the parent and child modules 
       */
      module_manager.add_child_module(module_id, tgate_module_id);
    }
  }
}

/*********************************************************************
 * Generate behavior-level Verilog codes modeling an branch circuit 
 * for a multiplexer with the given size 
 *********************************************************************/
static 
void generate_verilog_cmos_mux_branch_body_behavioral(std::fstream& fp,
                                                      const BasicPort& input_port,
                                                      const BasicPort& output_port,
                                                      const BasicPort& mem_port,
                                                      const MuxGraph& mux_graph,
                                                      const size_t& default_mem_val) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Verilog Behavior description for a MUX */
  print_verilog_comment(fp, std::string("---- Behavioral-level description -----"));

  /* Add an internal register for the output */
  BasicPort outreg_port("out_reg", mux_graph.num_outputs());
  /* Print the port */
  fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, outreg_port) << ";" << std::endl; 

  /* Generate the case-switch table */
  fp << "\talways @(" << generate_verilog_port(VERILOG_PORT_CONKT, input_port) << ", " << generate_verilog_port(VERILOG_PORT_CONKT, mem_port) << ")" << std::endl; 
  fp << "\tcase (" << generate_verilog_port(VERILOG_PORT_CONKT, mem_port) << ")" << std::endl;

  /* Output the netlist following the connections in mux_graph */
  /* Iterate over the inputs */
  for (const auto& mux_input : mux_graph.inputs()) {
    BasicPort cur_input_port(input_port.get_name(), size_t(mux_graph.input_id(mux_input)), size_t(mux_graph.input_id(mux_input)));
    /* Iterate over the outputs */
    for (const auto& mux_output : mux_graph.outputs()) {
      BasicPort cur_output_port(output_port.get_name(), size_t(mux_graph.output_id(mux_output)), size_t(mux_graph.output_id(mux_output)));
      /* if there is a connection between the input and output, a tgate will be outputted */
      std::vector<MuxEdgeId> edges = mux_graph.find_edges(mux_input, mux_output);
      /* There should be only one edge or no edge*/
      VTR_ASSERT((1 == edges.size()) || (0 == edges.size()));
      /* No need to output tgates if there are no edges between two nodes */
      if (0 == edges.size()) {
        continue;
      }
      /* For each case, generate the logic levels for all the inputs */
      /* In each case, only one mem is enabled */
      fp << "\t\t" << mem_port.get_width() << "'b";
      std::string case_code(mem_port.get_width(), default_mem_val);

      /* Find the mem_id controlling the edge */
      MuxMemId mux_mem = mux_graph.find_edge_mem(edges[0]);
      /* Flip a bit by the mem_id */
      if (false == mux_graph.is_edge_use_inv_mem(edges[0])) {
        case_code[size_t(mux_mem)] = '1';
      } else {
        case_code[size_t(mux_mem)] = '0';
      }
      fp << case_code << ": " << generate_verilog_port(VERILOG_PORT_CONKT, outreg_port) << " <= ";
      fp << generate_verilog_port(VERILOG_PORT_CONKT, cur_input_port) << ";" << std::endl;
    }
  }

  /* Default case: outputs are at high-impedance state 'z' */
  std::string default_case(mux_graph.num_outputs(), 'z');
  fp << "\t\tdefault: " << generate_verilog_port(VERILOG_PORT_CONKT, outreg_port) << " <= ";
  fp << mux_graph.num_outputs() << "'b" << default_case << ";" << std::endl;

  /* End the case */
  fp << "\tendcase" << std::endl;

  /* Wire registers to output ports */
  fp << "\tassign " << generate_verilog_port(VERILOG_PORT_CONKT, output_port) << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, outreg_port) << ";" << std::endl;
}

/*********************************************************************
 * Generate  Verilog codes modeling an branch circuit 
 * for a CMOS multiplexer with the given size 
 * Support structural and behavioral Verilog codes
 *********************************************************************/
static 
void generate_verilog_cmos_mux_branch_module(ModuleManager& module_manager,
                                             const CircuitLibrary& circuit_lib, 
                                             std::fstream& fp,
                                             const CircuitModelId& circuit_model, 
                                             const std::string& module_name, 
                                             const MuxGraph& mux_graph,
                                             const bool& use_structural_verilog) {
  /* Get the tgate model */
  CircuitModelId tgate_model = circuit_lib.pass_gate_logic_model(circuit_model);

  /* Skip output if the tgate model is a MUX2, it is handled by essential-gate generator */
  if (SPICE_MODEL_GATE == circuit_lib.model_type(tgate_model)) {
    VTR_ASSERT(SPICE_MODEL_GATE_MUX2 == circuit_lib.gate_type(tgate_model));
    return;
  }

  std::vector<CircuitPortId> tgate_global_ports = circuit_lib.model_global_ports_by_type(tgate_model, SPICE_MODEL_PORT_INPUT, true);

  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of inputs */ 
  size_t num_inputs = mux_graph.num_inputs();
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 output */
  VTR_ASSERT(1 == num_outputs);
  /* MUX graph must have only 1 level*/
  VTR_ASSERT(1 == mux_graph.num_levels());

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Add module ports */
  /* Add each global port */
  for (const auto& port : tgate_global_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  BasicPort input_port("in", num_inputs);
  module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each output port */
  BasicPort output_port("out", num_outputs);
  module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  /* Add each memory port */
  BasicPort mem_port("mem", num_mems);
  module_manager.add_port(module_id, mem_port, ModuleManager::MODULE_INPUT_PORT);
  BasicPort mem_inv_port("mem_inv", num_mems);
  module_manager.add_port(module_id, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);

  /* Print the internal logic in either structural or behavioral Verilog codes */
  if (true == use_structural_verilog) {
    generate_verilog_cmos_mux_branch_body_structural(module_manager, circuit_lib, fp, tgate_model, module_id, input_port, output_port, mem_port, mem_inv_port, mux_graph);
  } else {
    VTR_ASSERT_SAFE(false == use_structural_verilog);
    /* Get the default value of SRAM ports */
    std::vector<CircuitPortId> sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM, true);
    std::vector<CircuitPortId> non_mode_select_sram_ports;
    /* We should have only have 1 sram port except those are mode_bits */
    for (const auto& port : sram_ports) { 
      if (true == circuit_lib.port_is_mode_select(port)) {
        continue;
      }
      non_mode_select_sram_ports.push_back(port);
    }
    VTR_ASSERT(1 == non_mode_select_sram_ports.size());
    std::string mem_default_val = std::to_string(circuit_lib.port_default_value(non_mode_select_sram_ports[0]));
    /* Mem string must be only 1-bit! */
    VTR_ASSERT(1 == mem_default_val.length());
    generate_verilog_cmos_mux_branch_body_behavioral(fp, input_port, output_port, mem_port, mux_graph, mem_default_val[0]);
  }

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);
}

/*********************************************************************
 * Dump a structural verilog for RRAM MUX basis module 
 * This is only called when structural verilog dumping option is enabled for this spice model
 * IMPORTANT: the structural verilog can NOT be used for functionality verification!!!
 * TODO: This part is quite restricted to the way we implemented our RRAM FPGA
 *       Should be reworked to be more generic !!!
 *
 * By structural the schematic is splitted into two parts: left part and right part
 * The left part includes BLB[0..N-1] and WL[0..N-1] signals as well as RRAMs
 * The right part includes BLB[N] and WL[N] 
 * Corresponding Schematic is as follows:
 *
 *      LEFT PART   |    RIGHT PART
 *
 *         BLB[0]         BLB[N]
 *            |             |
 *           \|/           \|/
 *   in[0] ---->RRAM[0]-----+
 *                          |
 *         BLB[1]           |
 *           |              |
 *          \|/             |
 *   in[1] ---->RRAM[1]-----+ 
 *                          |-----> out[0]
 *              ...         
 *                          |
 * in[N-1] ---->RRAM[N-1]---+ 
 *           /|\           /|\
 *            |             |
 *          BLB[N-1]       WL[N]
 *
 * Working principle:
 * 1. Set a RRAM[i]: enable BLB[i] and WL[N]
 * 2. Reset a RRAM[i]: enable BLB[N] and WL[i]
 * 3. Operation: disable all BLBs and WLs
 *
 * The structure is done in the way we implement the physical layout of RRAM MUX
 * It is NOT the only road to the goal!!!
 *********************************************************************/
static 
void generate_verilog_rram_mux_branch_body_structural(ModuleManager& module_manager,
                                                      const CircuitLibrary& circuit_lib, 
                                                      std::fstream& fp,
                                                      const ModuleId& module_id, 
                                                      const CircuitModelId& circuit_model, 
                                                      const BasicPort& input_port,
                                                      const BasicPort& output_port,
                                                      const BasicPort& blb_port,
                                                      const BasicPort& wl_port,
                                                      const MuxGraph& mux_graph) {
  std::string progTE_module_name("PROG_TE");
  std::string progBE_module_name("PROG_BE");  

  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Verilog Behavior description for a MUX */
  print_verilog_comment(fp, std::string("---- Structure-level description of RRAM MUX -----"));

  /* Print internal structure of 4T1R programming structures
   * Written in structural Verilog
   * The whole structure-level description is divided into two parts:
   * 1. Left part consists of N PROG_TE modules, each of which
   * includes a PMOS, a NMOS and a RRAM, which is actually the left
   * part of a 4T1R programming structure 
   * 2. Right part includes only a PROG_BE module, which consists
   * of a PMOS and a NMOS, which is actually the right part of a
   * 4T1R programming sturcture
   */
  /* Create a module for the progTE and register it in the module manager 
   * Structure of progTE
   *
   *        +----------+
   *  in--->|          |
   *  BLB-->|  progTE  |--> out
   *  WL--->|          |
   *        +----------+
   */
  ModuleId progTE_module_id = module_manager.add_module(progTE_module_name);
  /* If there is already such as module inside, we just ned to find the module id */
  if (ModuleId::INVALID() == progTE_module_id) {
    progTE_module_id = module_manager.find_module(progTE_module_name);
    /* We should have a valid id! */
    VTR_ASSERT(ModuleId::INVALID() != progTE_module_id);
  }
  /* Add ports to the module */
  /* input port */
  BasicPort progTE_in_port("A", 1);
  module_manager.add_port(progTE_module_id, progTE_in_port, ModuleManager::MODULE_INPUT_PORT);
  /* WL port */
  BasicPort progTE_wl_port("WL", 1);
  module_manager.add_port(progTE_module_id, progTE_wl_port, ModuleManager::MODULE_INPUT_PORT);
  /* BLB port */
  BasicPort progTE_blb_port("BLB", 1);
  module_manager.add_port(progTE_module_id, progTE_blb_port, ModuleManager::MODULE_INPUT_PORT);
  /* output port */
  BasicPort progTE_out_port("Z", 1);
  module_manager.add_port(progTE_module_id, progTE_out_port, ModuleManager::MODULE_INPUT_PORT);

  /* LEFT part: Verilog code generation */
  /* Iterate over the inputs */
  for (const auto& mux_input : mux_graph.inputs()) {
    BasicPort cur_input_port(input_port.get_name(), size_t(mux_graph.input_id(mux_input)), size_t(mux_graph.input_id(mux_input)));
    /* Iterate over the outputs */
    for (const auto& mux_output : mux_graph.outputs()) {
      BasicPort cur_output_port(output_port.get_name(), size_t(mux_graph.output_id(mux_output)), size_t(mux_graph.output_id(mux_output)));
      /* if there is a connection between the input and output, a tgate will be outputted */
      std::vector<MuxEdgeId> edges = mux_graph.find_edges(mux_input, mux_output);
      /* There should be only one edge or no edge*/
      VTR_ASSERT((1 == edges.size()) || (0 == edges.size()));
      /* No need to output tgates if there are no edges between two nodes */
      if (0 == edges.size()) {
        continue;
      }
      /* Create a port-to-port name map */
      std::map<std::string, BasicPort> port2port_name_map;
      /* input port */
      port2port_name_map[progTE_in_port.get_name()] = cur_input_port;
      /* output port */
      port2port_name_map[progTE_out_port.get_name()] = cur_output_port;
      /* Find the mem_id controlling the edge */
      MuxMemId mux_mem = mux_graph.find_edge_mem(edges[0]);
      BasicPort cur_blb_port(blb_port.get_name(), size_t(mux_mem), size_t(mux_mem));
      BasicPort cur_wl_port(wl_port.get_name(), size_t(mux_mem), size_t(mux_mem));
      /* RRAM configuration port: there should not be any inverted edge in RRAM MUX! */
      VTR_ASSERT( false == mux_graph.is_edge_use_inv_mem(edges[0]) );
      /* wire mem to mem of module, and wire mem_inv to mem_inv of module */
      port2port_name_map[progTE_blb_port.get_name()] = cur_blb_port;
      port2port_name_map[progTE_wl_port.get_name()] = cur_wl_port;
      /* Output an instance of the module */
      print_verilog_module_instance(fp, module_manager, module_id, progTE_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(circuit_model));
      /* IMPORTANT: this update MUST be called after the instance outputting!!!!
       * update the module manager with the relationship between the parent and child modules 
       */
      module_manager.add_child_module(module_id, progTE_module_id);
    }
  }

  /* Create a module for the progBE and register it in the module manager 
   * Structure of progBE
   *
   *        +----------+
   *        |          |
   *  BLB-->|  progBE  |<-> out
   *  WL--->|          |
   *        +----------+
   */
  ModuleId progBE_module_id = module_manager.add_module(progBE_module_name);
  /* If there is already such as module inside, we just ned to find the module id */
  if (ModuleId::INVALID() == progBE_module_id) {
    progBE_module_id = module_manager.find_module(progBE_module_name);
    /* We should have a valid id! */
    VTR_ASSERT(ModuleId::INVALID() != progBE_module_id);
  }
  /* Add ports to the module */
  /* inout port */
  BasicPort progBE_inout_port("INOUT", 1);
  module_manager.add_port(progBE_module_id, progBE_inout_port, ModuleManager::MODULE_INOUT_PORT);
  /* WL port */
  BasicPort progBE_wl_port("WL", 1);
  module_manager.add_port(progBE_module_id, progBE_wl_port, ModuleManager::MODULE_INPUT_PORT);
  /* BLB port */
  BasicPort progBE_blb_port("BLB", 1);
  module_manager.add_port(progBE_module_id, progBE_blb_port, ModuleManager::MODULE_INPUT_PORT);

  /* RIGHT part: Verilog code generation */
  /* Iterate over the outputs */
  for (const auto& mux_output : mux_graph.outputs()) {
    BasicPort cur_output_port(output_port.get_name(), size_t(mux_graph.output_id(mux_output)), size_t(mux_graph.output_id(mux_output)));
    /* Create a port-to-port name map */
    std::map<std::string, BasicPort> port2port_name_map;
    /* Wire the output port to the INOUT port */
    port2port_name_map[progBE_inout_port.get_name()] = cur_output_port;
    /* Find the mem_id controlling the edge */
    BasicPort cur_blb_port(blb_port.get_name(), mux_graph.num_memory_bits(), mux_graph.num_memory_bits());
    BasicPort cur_wl_port(wl_port.get_name(), mux_graph.num_memory_bits(), mux_graph.num_memory_bits());
    port2port_name_map[progBE_blb_port.get_name()] = cur_blb_port;
    port2port_name_map[progBE_wl_port.get_name()] = cur_wl_port;
    /* Output an instance of the module */
    print_verilog_module_instance(fp, module_manager, module_id, progBE_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(circuit_model));
    /* IMPORTANT: this update MUST be called after the instance outputting!!!!
     * update the module manager with the relationship between the parent and child modules 
     */
    module_manager.add_child_module(module_id, progBE_module_id);
  }
}

/*********************************************************************
 * Generate behavior-level Verilog codes modeling an branch circuit 
 * for a RRAM-based multiplexer with the given size 
 * Corresponding Schematic is as follows:
 *
 *         BLB[0]         BLB[N]
 *            |             |
 *           \|/           \|/
 *   in[0] ---->RRAM[0]-----+
 *                          |
 *         BLB[1]           |
 *           |              |
 *          \|/             |
 *   in[1] ---->RRAM[1]-----+ 
 *                          |-----> out[0]
 *              ...         
 *                          |
 * in[N-1] ---->RRAM[N-1]---+ 
 *           /|\           /|\
 *            |             |
 *          BLB[N-1]       WL[N]
 *
 * Working principle:
 * 1. Set a RRAM[i]: enable BLB[i] and WL[N]
 * 2. Reset a RRAM[i]: enable BLB[N] and WL[i]
 * 3. Operation: disable all BLBs and WLs
 *
 * TODO: Elaborate the codes to output the circuit logic
 * following the mux_graph! 
 *********************************************************************/
static 
void generate_verilog_rram_mux_branch_body_behavioral(std::fstream& fp,
                                                      const CircuitLibrary& circuit_lib, 
                                                      const CircuitModelId& circuit_model, 
                                                      const BasicPort& input_port,
                                                      const BasicPort& output_port,
                                                      const BasicPort& blb_port,
                                                      const BasicPort& wl_port,
                                                      const MuxGraph& mux_graph) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Verilog Behavior description for a MUX */
  print_verilog_comment(fp, std::string("---- Behavioral-level description of RRAM MUX -----"));

  /* Add an internal register for the output */
  BasicPort outreg_port("out_reg", mux_graph.num_inputs());
  /* Print the port */
  fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, outreg_port) << ";" << std::endl; 

  /* Print the internal logics */
  fp << "\t" << "always @(";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, blb_port); 
  fp << ", ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, wl_port); 
  fp << ")";
  fp << " begin" << std::endl;

  /* Only when the last bit of wl is enabled, 
   * the propagating path can be changed 
   * (RRAM value can be changed) */
  fp << "\t\t" << "if (";
  BasicPort set_enable_port(wl_port.get_name(), wl_port.get_width() - 1, wl_port.get_width() - 1); 
  fp << generate_verilog_port(VERILOG_PORT_CONKT, set_enable_port); 
  /* We need two config-enable ports: prog_EN and prog_ENb */
  bool find_prog_EN = false;
  bool find_prog_ENb = false;
  for (const auto& port : circuit_lib.model_global_ports(circuit_model, true)) {
    /* Bypass non-config-enable ports */
    if (false == circuit_lib.port_is_config_enable(port)) {
      continue;
    }
    /* Reach here, the port should be is_config_enable */   
    /* Create a port object */
    fp << " && "; 
    BasicPort prog_en_port(circuit_lib.port_prefix(port), circuit_lib.port_size(port));
    if ( 0 == circuit_lib.port_default_value(port)) {
      /* Default value = 0 means that this is a prog_EN port */
      fp << generate_verilog_port(VERILOG_PORT_CONKT, prog_en_port); 
      find_prog_EN = true;
    } else {
      VTR_ASSERT ( 1 == circuit_lib.port_default_value(port));
      /* Default value = 1 means that this is a prog_ENb port, add inversion in the if condition */
      fp << "(~" << generate_verilog_port(VERILOG_PORT_CONKT, prog_en_port) << ")"; 
      find_prog_ENb = true;
    }
  }
  /* Check if we find any config_enable signals */
  if (false == find_prog_EN) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s,[LINE%d])Unable to find a config_enable signal with default value 0 for a RRAM MUX (%s)!\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model).c_str()); 
    exit(1);
  }
  if (false == find_prog_ENb) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s,[LINE%d])Unable to find a config_enable signal with default value 1 for a RRAM MUX (%s)!\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model).c_str()); 
    exit(1);
  }

  /* Finish the if clause */
  fp << ") begin" << std::endl;

  for (const auto& mux_input : mux_graph.inputs()) {
    /* First if clause need tabs */
    if ( 0 == size_t(mux_graph.input_id(mux_input)) ) {
      fp << "\t\t\t";
    }
    fp << "if (1 == ";
    /* Create a temp port of a BLB bit */
    BasicPort cur_blb_port(blb_port.get_name(), size_t(mux_graph.input_id(mux_input)), size_t(mux_graph.input_id(mux_input)));
    fp << generate_verilog_port(VERILOG_PORT_CONKT, cur_blb_port); 
    fp << ") begin" << std::endl;
    fp << "\t\t\t\t" << "assign ";
  fp << outreg_port.get_name(); 
    fp << " = " << size_t(mux_graph.input_id(mux_input)) << ";" << std::endl;
    fp << "\t\t\t" << "end else ";
  }
  fp << "begin" << std::endl;
  fp << "\t\t\t\t" << "assign ";
  fp << outreg_port.get_name(); 
  fp << " = 0;" << std::endl;
  fp << "\t\t\t" << "end" << std::endl;
  fp << "\t\t" << "end" << std::endl;
  fp << "\t" << "end" << std::endl;
 
  fp << "\t" << "assign ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, output_port);
  fp << " = "; 
  fp << input_port.get_name() << "[";
  fp << outreg_port.get_name(); 
  fp << "];" << std::endl;
}

/*********************************************************************
 * Generate  Verilog codes modeling an branch circuit 
 * for a RRAM-based multiplexer with the given size 
 * Support structural and behavioral Verilog codes
 *********************************************************************/
static 
void generate_verilog_rram_mux_branch_module(ModuleManager& module_manager,
                                             const CircuitLibrary& circuit_lib, 
                                             std::fstream& fp,
                                             const CircuitModelId& circuit_model, 
                                             const std::string& module_name, 
                                             const MuxGraph& mux_graph,
                                             const bool& use_structural_verilog) {
  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of inputs */ 
  size_t num_inputs = mux_graph.num_inputs();
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 output */
  VTR_ASSERT(1 == num_outputs);
  /* MUX graph must have only 1 level*/
  VTR_ASSERT(1 == mux_graph.num_levels());

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);

  /* Add module ports */
  /* Add each global programming enable/disable ports */
  std::vector<CircuitPortId> prog_enable_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  for (const auto& port : prog_enable_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  BasicPort input_port("in", num_inputs);
  module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add each output port */
  BasicPort output_port("out", num_outputs);
  module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  /* Add RRAM programming ports, 
   * RRAM MUXes require one more pair of BLB and WL 
   * to configure the memories. See schematic for details
   */
  BasicPort blb_port("blb", num_mems + 1);
  module_manager.add_port(module_id, blb_port, ModuleManager::MODULE_INPUT_PORT);
  BasicPort wl_port("wl", num_mems + 1);
  module_manager.add_port(module_id, wl_port, ModuleManager::MODULE_INPUT_PORT);

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);

  /* Print the internal logic in either structural or behavioral Verilog codes */
  if (true == use_structural_verilog) {
    generate_verilog_rram_mux_branch_body_structural(module_manager, circuit_lib, fp, module_id, circuit_model, input_port, output_port, blb_port, wl_port, mux_graph);
  } else {
    generate_verilog_rram_mux_branch_body_behavioral(fp, circuit_lib, circuit_model, input_port, output_port, blb_port, wl_port, mux_graph);
  }

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);
}

/***********************************************
 * Generate Verilog codes modeling an branch circuit 
 * for a multiplexer with the given size 
 **********************************************/
static 
void generate_verilog_mux_branch_module(ModuleManager& module_manager,
                                        const CircuitLibrary& circuit_lib, 
                                        std::fstream& fp, 
                                        const CircuitModelId& circuit_model, 
                                        const size_t& mux_size, 
                                        const MuxGraph& mux_graph) {
  std::string module_name = generate_verilog_mux_branch_subckt_name(circuit_lib, circuit_model, mux_size, mux_graph.num_inputs(), verilog_mux_basis_posfix);

  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(circuit_model)) {
  case SPICE_MODEL_DESIGN_CMOS:
    generate_verilog_cmos_mux_branch_module(module_manager, circuit_lib, fp, circuit_model, module_name, mux_graph, 
                                            circuit_lib.dump_structural_verilog(circuit_model));
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    generate_verilog_rram_mux_branch_module(module_manager, circuit_lib, fp, circuit_model, module_name, mux_graph, 
                                            circuit_lib.dump_structural_verilog(circuit_model));
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d]) Invalid design technology of multiplexer (name: %s)\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model).c_str()); 
    exit(1);
  }
}

/*********************************************************************
 * Generate Verilog codes modeling a CMOS multiplexer with the given size 
 * The Verilog module will consist of three parts:
 * 1. instances of the branch circuits of multiplexers which are generated before  
 *    This builds up the multiplexing structure
 * 2. Input buffers/inverters
 * 3. Output buffers/inverters
 *********************************************************************/
static 
void generate_verilog_cmos_mux_module(ModuleManager& module_manager,
                                      const CircuitLibrary& circuit_lib, 
                                      std::fstream& fp,
                                      const CircuitModelId& circuit_model, 
                                      const std::string& module_name, 
                                      const MuxGraph& mux_graph) {
  /* Get the global ports required by MUX (and any submodules) */
  std::vector<CircuitPortId> mux_global_ports = circuit_lib.model_global_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_OUTPUT, true);
  /* Get the sram ports from the mux */
  std::vector<CircuitPortId> mux_sram_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_SRAM, true);

  /* Make sure we have a valid file handler*/
  check_file_handler(fp);

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of data-path inputs */ 
  size_t num_inputs = find_mux_num_datapath_inputs(circuit_lib, circuit_model, mux_graph.num_inputs());
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 output */
  VTR_ASSERT(1 == mux_input_ports.size());
  VTR_ASSERT(1 == mux_input_ports.size());
  /* A quick check on the model ports */
  if ((SPICE_MODEL_MUX == circuit_lib.model_type(circuit_model))
    || ((SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model))
       && (false == circuit_lib.is_lut_fracturable(circuit_model))) ) {
    VTR_ASSERT(1 == mux_output_ports.size());
    VTR_ASSERT(1 == circuit_lib.port_size(mux_output_ports[0])); 
  } else {
    VTR_ASSERT_SAFE( (SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model)) 
                 && (true == circuit_lib.is_lut_fracturable(circuit_model)) );
    for (const auto& port : mux_output_ports) {
      VTR_ASSERT(0 < circuit_lib.port_size(port));
    }
  }

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Add module ports */
  /* Add each global port */
  for (const auto& port : mux_global_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_lib_name(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port
   * Treat MUX and LUT differently 
   * 1. MUXes: we do not have a specific input/output sizes, it is inferred by architecture 
   * 2. LUTes: we do have specific input/output sizes, 
   *           but the inputs of MUXes are the SRAM ports of LUTs
   *           and the SRAM ports of MUXes are the inputs of LUTs
   */
  size_t input_port_cnt = 0;
  for (const auto& port : mux_input_ports) {
    BasicPort input_port(circuit_lib.port_lib_name(port), num_inputs);
    module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
    /* Update counter */
    input_port_cnt++;
  }
  /* Double check: We should have only 1 input port generated here! */
  VTR_ASSERT(1 == input_port_cnt);

  for (const auto& port : mux_output_ports) {
    BasicPort output_port(circuit_lib.port_lib_name(port), num_outputs);
    if (SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
      output_port.set_width(circuit_lib.port_size(port));
    }
    module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  size_t sram_port_cnt = 0;
  for (const auto& port : mux_sram_ports) {
    /* Multiplexing structure does not mode_sram_ports, they are handled in LUT modules
     * Here we just bypass it.
     */
    if (true == circuit_lib.port_is_mode_select(port)) {
      continue;
    }
    BasicPort mem_port(circuit_lib.port_lib_name(port), num_mems);
    module_manager.add_port(module_id, mem_port, ModuleManager::MODULE_INPUT_PORT);
    BasicPort mem_inv_port(std::string(circuit_lib.port_lib_name(port) + "_inv"), num_mems);
    module_manager.add_port(module_id, mem_inv_port, ModuleManager::MODULE_INPUT_PORT);
    /* Update counter */
    sram_port_cnt++;
  }
 
  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);

  /* TODO: Print the internal logic in Verilog codes */
  /* TODO: Print the Multiplexing structure in Verilog codes */
  /* TODO: Print the input buffers in Verilog codes */
  /* TODO: Print the output buffers in Verilog codes */

  /* Put an end to the Verilog module */
  print_verilog_module_end(fp, module_name);
}

/***********************************************
 * Generate Verilog codes modeling a multiplexer 
 * with the given graph-level description
 **********************************************/
static 
void generate_verilog_mux_module(ModuleManager& module_manager,
                                 const CircuitLibrary& circuit_lib, 
                                 std::fstream& fp, 
                                 const CircuitModelId& circuit_model, 
                                 const MuxGraph& mux_graph) {
  std::string module_name = generate_verilog_mux_subckt_name(circuit_lib, circuit_model, 
                                                             find_mux_num_datapath_inputs(circuit_lib, circuit_model, mux_graph.num_inputs()), 
                                                             std::string(""));
 
  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(circuit_model)) {
  case SPICE_MODEL_DESIGN_CMOS:
    /* SRAM-based Multiplexer Verilog module generation */
    generate_verilog_cmos_mux_module(module_manager, circuit_lib, fp, circuit_model, module_name, mux_graph);
    break;
  case SPICE_MODEL_DESIGN_RRAM:
    /* TODO: RRAM-based Multiplexer Verilog module generation */
    /*
    generate_verilog_rram_mux_branch_module(module_manager, circuit_lib, fp, circuit_model, module_name, mux_graph, 
                                            circuit_lib.dump_structural_verilog(circuit_model));
     */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s,LINE[%d]) Invalid design technology of multiplexer (name: %s)\n",
               __FILE__, __LINE__, circuit_lib.model_name(circuit_model).c_str()); 
    exit(1);
  }
}


/***********************************************
 * Generate Verilog modules for all the unique
 * multiplexers in the FPGA device
 **********************************************/
void print_verilog_submodule_muxes(ModuleManager& module_manager,
                                   const MuxLibrary& mux_lib,
                                   const CircuitLibrary& circuit_lib,
                                   t_sram_orgz_info* cur_sram_orgz_info,
                                   char* verilog_dir,
                                   char* submodule_dir) {

  /* TODO: Generate modules into a .bak file now. Rename after it is verified */
  std::string verilog_fname(my_strcat(submodule_dir, muxes_verilog_file_name));
  verilog_fname += ".bak";

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_handler(fp);

  /* Print out debugging information for if the file is not opened/created properly */
  vpr_printf(TIO_MESSAGE_INFO,
             "Creating Verilog netlist for Multiplexers (%s) ...\n",
             verilog_fname.c_str()); 

  print_verilog_file_header(fp, "Multiplexers"); 

  print_verilog_include_defines_preproc_file(fp, verilog_dir);
  
  /* Generate basis sub-circuit for unique branches shared by the multiplexers */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_circuit_model = mux_lib.mux_circuit_model(mux); 
    /* Create a mux graph for the branch circuit */
    std::vector<MuxGraph> branch_mux_graphs = mux_graph.build_mux_branch_graphs();
    /* Create branch circuits, which are N:1 one-level or 2:1 tree-like MUXes */
    for (auto branch_mux_graph : branch_mux_graphs) {
      generate_verilog_mux_branch_module(module_manager, circuit_lib, fp, mux_circuit_model, 
                                         find_mux_num_datapath_inputs(circuit_lib, mux_circuit_model, mux_graph.num_inputs()), 
                                         branch_mux_graph);
    }
  }

  /* Generate unique Verilog modules for the multiplexers */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_circuit_model = mux_lib.mux_circuit_model(mux); 
    /* Create MUX circuits */
    generate_verilog_mux_module(module_manager, circuit_lib, fp, mux_circuit_model, mux_graph);
  }

  /* Close the file steam */
  fp.close();

  /* TODO: 
   * Scan-chain configuration circuit does not need any BLs/WLs! 
   * SRAM MUX does not need any reserved BL/WLs!
   */
  /* Determine reserved Bit/Word Lines if a memory bank is specified,
   * At least 1 BL/WL should be reserved! 
   */
  try_update_sram_orgz_info_reserved_blwl(cur_sram_orgz_info, 
                                          mux_lib.max_mux_size(), mux_lib.max_mux_size());

  /* TODO: Add fname to the linked list when debugging is finished */
  /*
  submodule_verilog_subckt_file_path_head = add_one_subckt_file_name_to_llist(submodule_verilog_subckt_file_path_head, verilog_name);  
   */
}

