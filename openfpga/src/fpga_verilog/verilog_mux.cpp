/***********************************************
 * This file includes functions to generate
 * Verilog submodules for multiplexers.
 * including both fundamental submodules
 * such as a branch in a multiplexer 
 * and the full multiplexer
 **********************************************/
#include <string>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from readarch library */
#include "physical_types.h"

/* Headers from readarcopenfpga library */
#include "circuit_types.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "mux_graph.h"
#include "module_manager.h"
#include "mux_utils.h"
#include "circuit_library_utils.h"
#include "decoder_library_utils.h"

#include "openfpga_naming.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_module_writer.h"
#include "verilog_mux.h"

/* begin namespace openfpga */
namespace openfpga {

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
  VTR_ASSERT(true == valid_file_stream(fp));

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
void print_verilog_cmos_mux_branch_module_behavioral(ModuleManager& module_manager,
                                                     const CircuitLibrary& circuit_lib, 
                                                     std::fstream& fp,
                                                     const CircuitModelId& mux_model, 
                                                     const std::string& module_name, 
                                                     const MuxGraph& mux_graph) {
  /* Get the tgate model */
  CircuitModelId tgate_model = circuit_lib.pass_gate_logic_model(mux_model);

  /* Skip output if the tgate model is a MUX2, it is handled by essential-gate generator */
  if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(tgate_model)) {
    VTR_ASSERT(CIRCUIT_MODEL_GATE_MUX2 == circuit_lib.gate_type(tgate_model));
    return;
  }

  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

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
  ModuleId mux_module = module_manager.find_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(mux_module));
  /* Find module ports */
  /* Find each input port */
  BasicPort input_port("in", num_inputs);
  /* Find each output port */
  BasicPort output_port("out", num_outputs);
  /* Find each memory port */
  BasicPort mem_port("mem", num_mems);

  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, mux_module);

  /* Print the internal logic in behavioral Verilog codes */
  /* Get the default value of SRAM ports */
  std::vector<CircuitPortId> regular_sram_ports = find_circuit_regular_sram_ports(circuit_lib, mux_model); 
  VTR_ASSERT(1 == regular_sram_ports.size());
  std::string mem_default_val = std::to_string(circuit_lib.port_default_value(regular_sram_ports[0]));
  /* Mem string must be only 1-bit! */
  VTR_ASSERT(1 == mem_default_val.length());
  generate_verilog_cmos_mux_branch_body_behavioral(fp, input_port, output_port, mem_port, mux_graph, mem_default_val[0]);

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
  VTR_ASSERT(true == valid_file_stream(fp));

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
  VTR_ASSERT(true == valid_file_stream(fp));

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
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "Unable to find a config_enable signal with default value 0 for a RRAM MUX '%s'!\n",
                   circuit_lib.model_name(circuit_model).c_str()); 
    exit(1);
  }
  if (false == find_prog_ENb) {
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "Unable to find a config_enable signal with default value 1 for a RRAM MUX '%s'!\n",
                   circuit_lib.model_name(circuit_model).c_str()); 
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
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  /* Get the BL and WL ports from the mux */
  std::vector<CircuitPortId> mux_blb_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> mux_wl_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_WL, true);

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
  /* MUX graph must have only 1 input and 1 BLB and 1 WL port */
  VTR_ASSERT(1 == mux_input_ports.size());
  VTR_ASSERT(1 == mux_output_ports.size());
  VTR_ASSERT(1 == mux_blb_ports.size());
  VTR_ASSERT(1 == mux_wl_ports.size());

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.find_module(module_name); 
  VTR_ASSERT(true == module_manager.valid_module_id(module_id));

  /* Find each input port */
  BasicPort input_port(circuit_lib.port_prefix(mux_input_ports[0]), num_inputs);

  /* Find each output port */
  BasicPort output_port(circuit_lib.port_prefix(mux_output_ports[0]), num_outputs);

  /* Find RRAM programming ports, 
   * RRAM MUXes require one more pair of BLB and WL 
   * to configure the memories. See schematic for details
   */
  BasicPort blb_port(circuit_lib.port_prefix(mux_blb_ports[0]), num_mems + 1);

  BasicPort wl_port(circuit_lib.port_prefix(mux_wl_ports[0]), num_mems + 1);

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
                                        const CircuitModelId& mux_model, 
                                        const size_t& mux_size, 
                                        const MuxGraph& mux_graph,
                                        const bool& use_explicit_port_map) {
  std::string module_name = generate_mux_branch_subckt_name(circuit_lib, mux_model, mux_size, mux_graph.num_inputs(), VERILOG_MUX_BASIS_POSTFIX);

  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(mux_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    /* Skip module writing if the branch subckt is a standard cell! */
    if (true == circuit_lib.valid_model_id(circuit_lib.model(module_name))) {
      /* This model must be a MUX2 gate */
      VTR_ASSERT(CIRCUIT_MODEL_GATE == circuit_lib.model_type(circuit_lib.model(module_name)));
      VTR_ASSERT(CIRCUIT_MODEL_GATE_MUX2 == circuit_lib.gate_type(circuit_lib.model(module_name)));
      break;
    }
    if (true == circuit_lib.dump_structural_verilog(mux_model)) {
      /* Structural verilog can be easily generated by module writer */
      ModuleId mux_module = module_manager.find_module(module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(mux_module));
      write_verilog_module_to_file(fp, module_manager, mux_module, 
                                   use_explicit_port_map || circuit_lib.dump_explicit_port_map(mux_model));
      /* Add an empty line as a splitter */
      fp << std::endl;
    } else {
      /* Behavioral verilog requires customized generation */
      print_verilog_cmos_mux_branch_module_behavioral(module_manager, circuit_lib, fp, mux_model, module_name, mux_graph);
    }
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    generate_verilog_rram_mux_branch_module(module_manager, circuit_lib, fp, mux_model, module_name, mux_graph, 
                                            circuit_lib.dump_structural_verilog(mux_model));
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid design technology of multiplexer '%s'\n",
                   circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }
}

/********************************************************************
 * Generate the input bufferes for a multiplexer or LUT in Verilog codes 
 * 1. If input are required to be buffered (specified by users),
 *    buffers will be added to all the datapath inputs.
 * 2. If input are required to NOT be buffered (specified by users),
 *    all the datapath inputs will be short wired to MUX inputs. 
 *
 * For those Multiplexers or LUTs require a constant input:
 *    the last input of multiplexer will be wired to a constant voltage level
 *******************************************************************/
static 
void generate_verilog_cmos_mux_module_input_buffers(ModuleManager& module_manager,
                                                    const CircuitLibrary& circuit_lib, 
                                                    std::fstream& fp, 
                                                    const ModuleId& module_id, 
                                                    const CircuitModelId& circuit_model, 
                                                    const MuxGraph& mux_graph) {
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* We should have only 1 input port! */
  VTR_ASSERT(1 == mux_input_ports.size());

  /* Get the input port from MUX module */
  ModulePortId module_input_port_id = module_manager.find_module_port(module_id, circuit_lib.port_prefix(mux_input_ports[0]));
  VTR_ASSERT(ModulePortId::INVALID() != module_input_port_id);
  /* Get the port from module */
  BasicPort module_input_port = module_manager.module_port(module_id, module_input_port_id);

  /* Iterate over all the inputs in the MUX graph */
  for (const auto& input_node : mux_graph.inputs()) {
    /* Fetch fundamental information from MUX graph w.r.t. the input node */
    MuxInputId input_index = mux_graph.input_id(input_node);
    VTR_ASSERT(MuxInputId::INVALID() != input_index);   
 
    size_t input_node_level = mux_graph.node_level(input_node);
    size_t input_node_index_at_level = mux_graph.node_index_at_level(input_node);

    /* Create the port information of the MUX input, which is the input of buffer instance */
    BasicPort instance_input_port(module_input_port.get_name(), size_t(input_index), size_t(input_index));

    /* Create the port information of the MUX graph input, which is the output of buffer instance */
    BasicPort instance_output_port(generate_mux_node_name(input_node_level, false), input_node_index_at_level, input_node_index_at_level);

    /* For last input:
     * Add a constant value to the last input, if this MUX needs a constant input
     */
    if (  (MuxInputId(mux_graph.num_inputs() - 1) == mux_graph.input_id(input_node)) 
       && (true == circuit_lib.mux_add_const_input(circuit_model)) ) {
      /* Get the constant input value */
      size_t const_value = circuit_lib.mux_const_input_value(circuit_model);
      VTR_ASSERT( (0 == const_value) || (1 == const_value) ); 
      /* For the output of the buffer instance:
       * Get the last inputs from the MUX graph and generate the node name in MUX module.
       */
      print_verilog_comment(fp, std::string("---- BEGIN short-wire a multiplexing structure input to a constant value -----"));
      print_verilog_wire_constant_values(fp, instance_output_port, std::vector<size_t>(1, const_value));
      print_verilog_comment(fp, std::string("---- END short-wire a multiplexing structure input to a constant value -----"));
      fp << std::endl;
      continue; /* Finish here */
    }

    /* If the inputs are not supposed to be buffered */
    if (false == circuit_lib.is_input_buffered(circuit_model)) {
      print_verilog_comment(fp, std::string("---- BEGIN short-wire a multiplexing structure input to MUX module input -----"));

      /* Short wire all the datapath inputs to the MUX inputs */
      print_verilog_wire_connection(fp, instance_output_port, instance_input_port, false);      

      print_verilog_comment(fp, std::string("---- END short-wire a multiplexing structure input to MUX module input -----"));
      fp << std::endl;
      continue; /* Finish here */
    }

    /* Reach here, we need a buffer, create a port-to-port map and output the buffer instance */
    print_verilog_comment(fp, std::string("---- BEGIN Instanciation of an input buffer module -----"));

    /* Now we need to add intermediate buffers by instanciating the modules */
    CircuitModelId buffer_model = circuit_lib.input_buffer_model(circuit_model);
    /* We must have a valid model id */
    VTR_ASSERT(CircuitModelId::INVALID() != buffer_model);

    print_verilog_buffer_instance(fp, module_manager, circuit_lib, module_id, buffer_model, instance_input_port, instance_output_port);

    print_verilog_comment(fp, std::string("---- END Instanciation of an input buffer module -----"));
    fp << std::endl;
  } 
}

/********************************************************************
 * Generate the output bufferes for a multiplexer or LUT in Verilog codes 
 * 1. If output are required to be buffered (specified by users),
 *    buffers will be added to all the outputs.
 * 2. If output are required to NOT be buffered (specified by users),
 *    all the outputs will be short wired to MUX outputs. 
 *******************************************************************/
static 
void generate_verilog_cmos_mux_module_output_buffers(ModuleManager& module_manager,
                                                     const CircuitLibrary& circuit_lib, 
                                                     std::fstream& fp, 
                                                     const ModuleId& module_id, 
                                                     const CircuitModelId& circuit_model, 
                                                     const MuxGraph& mux_graph) {
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);

  /* Iterate over all the outputs in the MUX module */
  for (const auto& output_port : mux_output_ports) {
    /* Get the output port from MUX module */
    ModulePortId module_output_port_id = module_manager.find_module_port(module_id, circuit_lib.port_prefix(output_port));
    VTR_ASSERT(ModulePortId::INVALID() != module_output_port_id);
    /* Get the port from module */
    BasicPort module_output_port = module_manager.module_port(module_id, module_output_port_id);

    /* Iterate over each pin of the output port */
    for (const auto& pin : circuit_lib.pins(output_port)) {
      /* Fetch fundamental information from MUX graph w.r.t. the input node */
      /* Deposite the last level of the graph, which is a default value */
      size_t output_node_level = mux_graph.num_node_levels() - 1; 
      /* If there is a fracturable level specified for the output, we find the exact level */
      if (size_t(-1) != circuit_lib.port_lut_frac_level(output_port)) {
        output_node_level = circuit_lib.port_lut_frac_level(output_port);
      }
      /* Deposite a zero, which is a default value */
      size_t output_node_index_at_level = 0; 
      /* If there are output masks, we find the node_index */
      if (!circuit_lib.port_lut_output_mask(output_port).empty()) {
        output_node_index_at_level = circuit_lib.port_lut_output_mask(output_port).at(pin);
      } 
      /* Double check the node exists in the Mux Graph */
      VTR_ASSERT(MuxNodeId::INVALID() != mux_graph.node_id(output_node_level, output_node_index_at_level));

      /* Create the port information of the MUX input, which is the input of buffer instance */
      BasicPort instance_input_port(generate_mux_node_name(output_node_level, false), output_node_index_at_level, output_node_index_at_level);

      /* Create the port information of the module output at the given pin range, which is the output of buffer instance */
      BasicPort instance_output_port(module_output_port.get_name(), pin, pin);

      /* If the output is not supposed to be buffered */
      if (false == circuit_lib.is_output_buffered(circuit_model)) {
        print_verilog_comment(fp, std::string("---- BEGIN short-wire a multiplexing structure output to MUX module output -----"));

        /* Short wire all the datapath inputs to the MUX inputs */
        print_verilog_wire_connection(fp, instance_output_port, instance_input_port, false);      

        print_verilog_comment(fp, std::string("---- END short-wire a multiplexing structure output to MUX module output -----"));
        fp << std::endl;
        continue; /* Finish here */
      }

      /* Reach here, we need a buffer, create a port-to-port map and output the buffer instance */
      print_verilog_comment(fp, std::string("---- BEGIN Instanciation of an output buffer module -----"));

      /* Now we need to add intermediate buffers by instanciating the modules */
      CircuitModelId buffer_model = circuit_lib.output_buffer_model(circuit_model);
      /* We must have a valid model id */
      VTR_ASSERT(CircuitModelId::INVALID() != buffer_model);

      print_verilog_buffer_instance(fp, module_manager, circuit_lib, module_id, buffer_model, instance_input_port, instance_output_port);

      print_verilog_comment(fp, std::string("---- END Instanciation of an output buffer module -----"));
      fp << std::endl;
    }
  }
}

/********************************************************************
 * Generate the 4T1R-based internal logic 
 * (multiplexing structure) for a multiplexer in Verilog codes 
 * This function will : 
 * 1. build a multiplexing structure by instanciating the branch circuits
 *    generated before
 * 2. add intermediate buffers between multiplexing stages if specified.
 *******************************************************************/
static 
void generate_verilog_rram_mux_module_multiplexing_structure(ModuleManager& module_manager,
                                                             const CircuitLibrary& circuit_lib, 
                                                             std::fstream& fp, 
                                                             const ModuleId& module_id, 
                                                             const CircuitModelId& circuit_model, 
                                                             const MuxGraph& mux_graph) {
  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Find the actual mux size */
  size_t mux_size = find_mux_num_datapath_inputs(circuit_lib, circuit_model, mux_graph.num_inputs());

  /* Get the BL and WL ports from the mux */
  std::vector<CircuitPortId> mux_blb_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> mux_wl_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_WL, true);
  /* MUX graph must have only 1 BLB and 1 WL port */
  VTR_ASSERT(1 == mux_blb_ports.size());
  VTR_ASSERT(1 == mux_wl_ports.size());

  /* Build the location map of intermediate buffers */
  std::vector<bool> inter_buffer_location_map = build_mux_intermediate_buffer_location_map(circuit_lib, circuit_model, mux_graph.num_node_levels());

  print_verilog_comment(fp, std::string("---- BEGIN Internal Logic of a RRAM-based MUX module -----"));

  print_verilog_comment(fp, std::string("---- BEGIN Internal wires of a RRAM-based MUX module -----"));
  /* Print local wires which are the nodes in the mux graph */
  for (size_t level = 0; level < mux_graph.num_levels(); ++level) {
    /* Print the internal wires located at this level */
    BasicPort internal_wire_port(generate_mux_node_name(level, false), mux_graph.num_nodes_at_level(level));
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, internal_wire_port) << ";" << std::endl;
    /* Identify if an intermediate buffer is needed */
    if (false == inter_buffer_location_map[level]) { 
      continue;
    }
    BasicPort internal_wire_buffered_port(generate_mux_node_name(level, true), mux_graph.num_nodes_at_level(level));
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, internal_wire_buffered_port) << std::endl;
  }
  print_verilog_comment(fp, std::string("---- END Internal wires of a RRAM-based MUX module -----"));
  fp << std::endl;

  /* Iterate over all the internal nodes and output nodes in the mux graph */
  for (const auto& node : mux_graph.non_input_nodes()) {
    print_verilog_comment(fp, std::string("---- BEGIN Instanciation of a branch RRAM-based MUX module -----"));
    /* Get the size of branch circuit 
     * Instanciate an branch circuit by the size (fan-in) of the node 
     */
    size_t branch_size = mux_graph.node_in_edges(node).size();

    /* Get the node level and index in the current level */
    size_t output_node_level = mux_graph.node_level(node);
    size_t output_node_index_at_level = mux_graph.node_index_at_level(node);

    /* Get the nodes which drive the root_node */
    std::vector<MuxNodeId> input_nodes; 
    for (const auto& edge : mux_graph.node_in_edges(node)) {
      /* Get the nodes drive the edge */
      for (const auto& src_node : mux_graph.edge_src_nodes(edge)) {
        input_nodes.push_back(src_node);
      }
    }
    /* Number of inputs should match the branch_input_size!!! */
    VTR_ASSERT(input_nodes.size() == branch_size);

    /* Get the mems in the branch circuits */
    std::vector<MuxMemId> mems; 
    for (const auto& edge : mux_graph.node_in_edges(node)) {
      /* Get the mem control the edge */
      MuxMemId mem = mux_graph.find_edge_mem(edge);
      /* Add the mem if it is not in the list */
      if (mems.end() == std::find(mems.begin(), mems.end(), mem)) {
        mems.push_back(mem);
      }
    }

    /* Instanciate the branch module which is a tgate-based module  
     */
    std::string branch_module_name= generate_mux_branch_subckt_name(circuit_lib, circuit_model, mux_size, branch_size, VERILOG_MUX_BASIS_POSTFIX);
    /* Get the moduleId for the submodule */
    ModuleId branch_module_id = module_manager.find_module(branch_module_name);
    /* We must have one */
    VTR_ASSERT(ModuleId::INVALID() != branch_module_id);

    /* Create a port-to-port map */
    std::map<std::string, BasicPort> port2port_name_map;
    /* TODO: the branch module name should NOT be hard-coded. Use the port lib_name given by users! */

    /* All the input node names organized in bus */
    std::vector<BasicPort> branch_node_input_ports;
    for (const auto& input_node : input_nodes) {
      /* Generate the port info of each input node */
      size_t input_node_level = mux_graph.node_level(input_node);
      size_t input_node_index_at_level = mux_graph.node_index_at_level(input_node);
      BasicPort branch_node_input_port(generate_mux_node_name(input_node_level, inter_buffer_location_map[input_node_level]), input_node_index_at_level, input_node_index_at_level);
      branch_node_input_ports.push_back(branch_node_input_port);  
    } 

    /* Create the port info for the input */
    /* TODO: the naming could be more flexible? */
    BasicPort instance_input_port = generate_verilog_bus_port(branch_node_input_ports, std::string(generate_mux_node_name(output_node_level, false) + "_in"));
    /* If we have more than 1 port in the combined instance ports , 
     * output a local wire */
    if (1 < combine_verilog_ports(branch_node_input_ports).size()) {
      /* Print a local wire for the merged ports */
      fp << "\t" << generate_verilog_local_wire(instance_input_port, branch_node_input_ports) << std::endl;
    } else {
      /* Safety check */
      VTR_ASSERT(1 == combine_verilog_ports(branch_node_input_ports).size());
    }

    /* Link nodes to input ports for the branch module */
    ModulePortId module_input_port_id = module_manager.find_module_port(branch_module_id, "in");
    VTR_ASSERT(ModulePortId::INVALID() != module_input_port_id);
    /* Get the port from module */
    BasicPort module_input_port = module_manager.module_port(branch_module_id, module_input_port_id);
    port2port_name_map[module_input_port.get_name()] = instance_input_port; 

    /* Link nodes to output ports for the branch module */
    BasicPort instance_output_port(generate_mux_node_name(output_node_level, false), output_node_index_at_level, output_node_index_at_level);
    ModulePortId module_output_port_id = module_manager.find_module_port(branch_module_id, "out");
    VTR_ASSERT(ModulePortId::INVALID() != module_output_port_id);
    /* Get the port from module */
    BasicPort module_output_port = module_manager.module_port(branch_module_id, module_output_port_id);
    port2port_name_map[module_output_port.get_name()] = instance_output_port; 

    /* All the mem node names organized in bus 
     * RRAM-based MUX uses BLB and WL to control memories  
     */
    std::vector<BasicPort> branch_node_blb_ports;
    for (const auto& mem : mems) {
      /* Generate the port info of each mem node:
       */
      BasicPort branch_node_blb_port(circuit_lib.port_prefix(mux_blb_ports[0]), size_t(mem), size_t(mem));
      branch_node_blb_ports.push_back(branch_node_blb_port);  
    } 
    /* Every stage, we have an additonal BLB and WL in controlling purpose 
     * The additional BLB is arranged at the tail of BLB port  
     * For example: 
     *    The total port width is BLB[0 ... <num_mem> + <num_levels> - 1]
     *    The regular BLB used by branches are  BLB[0 .. <num_mem> - 1]
     *    The additional BLB used by branches are BLB[<num_mem> .. <num_mem> + <num_levels> - 1]
     *
     * output_node_level is always larger than the mem_level by 1
     */
    branch_node_blb_ports.push_back(BasicPort(circuit_lib.port_prefix(mux_blb_ports[0]), 
                                              mux_graph.num_memory_bits() + output_node_level - 1, 
                                              mux_graph.num_memory_bits() + output_node_level - 1) 
                                    );

    /* Create the port info for the input */
    /* TODO: the naming could be more flexible? */
    BasicPort instance_blb_port = generate_verilog_bus_port(branch_node_blb_ports, std::string(generate_mux_node_name(output_node_level, false) + "_blb"));
    /* If we have more than 1 port in the combined instance ports , 
     * output a local wire */
    if (1 < combine_verilog_ports(branch_node_blb_ports).size()) {
      /* Print a local wire for the merged ports */
      fp << "\t" << generate_verilog_local_wire(instance_blb_port, branch_node_blb_ports) << std::endl;
    } else {
      /* Safety check */
      VTR_ASSERT(1 == combine_verilog_ports(branch_node_blb_ports).size());
    }

    /* Link nodes to BLB ports for the branch module */
    ModulePortId module_blb_port_id = module_manager.find_module_port(branch_module_id, circuit_lib.port_prefix(mux_blb_ports[0]));
    VTR_ASSERT(ModulePortId::INVALID() != module_blb_port_id);
    /* Get the port from module */
    BasicPort module_blb_port = module_manager.module_port(branch_module_id, module_blb_port_id);
    port2port_name_map[module_blb_port.get_name()] = instance_blb_port; 

    std::vector<BasicPort> branch_node_wl_ports;
    for (const auto& mem : mems) {
      /* Generate the port info of each mem node:
       */
      BasicPort branch_node_blb_port(circuit_lib.port_prefix(mux_wl_ports[0]), size_t(mem), size_t(mem));
      branch_node_wl_ports.push_back(branch_node_blb_port);  
    } 
    /* Every stage, we have an additonal BLB and WL in controlling purpose 
     * The additional BLB is arranged at the tail of BLB port  
     * For example: 
     *    The total port width is WL[0 ... <num_mem> + <num_levels> - 1]
     *    The regular BLB used by branches are  WL[0 .. <num_mem> - 1]
     *    The additional BLB used by branches are WL[<num_mem> .. <num_mem> + <num_levels> - 1]
     *
     * output_node_level is always larger than the mem_level by 1
     */
    branch_node_wl_ports.push_back(BasicPort(circuit_lib.port_prefix(mux_wl_ports[0]), 
                                             mux_graph.num_memory_bits() + output_node_level - 1, 
                                             mux_graph.num_memory_bits() + output_node_level - 1) 
                                   );

    /* Create the port info for the WL */
    /* TODO: the naming could be more flexible? */
    BasicPort instance_wl_port = generate_verilog_bus_port(branch_node_wl_ports, std::string(generate_mux_node_name(output_node_level, false) + "_wl"));
    /* If we have more than 1 port in the combined instance ports , 
     * output a local wire */
    if (1 < combine_verilog_ports(branch_node_wl_ports).size()) {
      /* Print a local wire for the merged ports */
      fp << "\t" << generate_verilog_local_wire(instance_wl_port, branch_node_wl_ports) << std::endl;
    } else {
      /* Safety check */
      VTR_ASSERT(1 == combine_verilog_ports(branch_node_wl_ports).size());
    }

    /* Link nodes to BLB ports for the branch module */
    ModulePortId module_wl_port_id = module_manager.find_module_port(branch_module_id, circuit_lib.port_prefix(mux_wl_ports[0]));
    VTR_ASSERT(ModulePortId::INVALID() != module_wl_port_id);
    /* Get the port from module */
    BasicPort module_wl_port = module_manager.module_port(branch_module_id, module_wl_port_id);
    port2port_name_map[module_wl_port.get_name()] = instance_wl_port; 

    /* Output an instance of the module */
    print_verilog_module_instance(fp, module_manager, module_id, branch_module_id, port2port_name_map, circuit_lib.dump_explicit_port_map(circuit_model));
    /* IMPORTANT: this update MUST be called after the instance outputting!!!!
     * update the module manager with the relationship between the parent and child modules 
     */
    module_manager.add_child_module(module_id, branch_module_id);

    print_verilog_comment(fp, std::string("---- END Instanciation of a branch RRAM-based MUX module -----"));
    fp << std::endl;

    if (false == inter_buffer_location_map[output_node_level]) {
      continue; /* No need for intermediate buffers */
    }

    print_verilog_comment(fp, std::string("---- BEGIN Instanciation of an intermediate buffer modules -----"));

    /* Now we need to add intermediate buffers by instanciating the modules */
    CircuitModelId buffer_model = circuit_lib.lut_intermediate_buffer_model(circuit_model);
    /* We must have a valid model id */
    VTR_ASSERT(CircuitModelId::INVALID() != buffer_model);
   
    BasicPort buffer_instance_input_port(generate_mux_node_name(output_node_level, false), output_node_index_at_level, output_node_index_at_level);
    BasicPort buffer_instance_output_port(generate_mux_node_name(output_node_level, true), output_node_index_at_level, output_node_index_at_level);

    print_verilog_buffer_instance(fp, module_manager, circuit_lib, module_id, buffer_model, buffer_instance_input_port, buffer_instance_output_port);

    print_verilog_comment(fp, std::string("---- END Instanciation of an intermediate buffer module -----"));
    fp << std::endl;
  }

  print_verilog_comment(fp, std::string("---- END Internal Logic of a RRAM-based MUX module -----"));
  fp << std::endl;
}

/*********************************************************************
 * Generate Verilog codes modeling a RRAM-based multiplexer with the given size 
 * The Verilog module will consist of three parts:
 * 1. instances of the branch circuits of multiplexers which are generated before  
 *    This builds up the 4T1R-based multiplexing structure
 *
 *                    BLB   WL
 *                     |    |       ...
 *                     v    v
 *                   +--------+            
 *           in[0]-->|        |            BLB   WL 
 *                ...| Branch |-----+       |    |
 *             in -->|   0    |     |       v    v
 *            [N-1]  +--------+     |     +--------+
 *                      ...            -->|        |
 *                    BLBs WLs         ...| Branch |
 *                     |    |    ...   -->|   X    |
 *                     v    v             +--------+
 *                   +--------+    |
 *                -->|        |    |
 *                ...| Branch |----+
 *                -->|   i    |
 *                   +--------+
 *
 * 2. Input buffers/inverters
 * 3. Output buffers/inverters
 *********************************************************************/
static 
void generate_verilog_rram_mux_module(ModuleManager& module_manager,
                                      const CircuitLibrary& circuit_lib, 
                                      std::fstream& fp,
                                      const CircuitModelId& circuit_model, 
                                      const std::string& module_name, 
                                      const MuxGraph& mux_graph) {
  /* Error out for the conditions where we are not yet supported! */
  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
    /* RRAM LUT is not supported now... */
    VTR_LOGF_ERROR(__FILE__, __LINE__, 
                   "RRAM-based LUT is not supported for circuit model '%s'!\n",
                   circuit_lib.model_name(circuit_model).c_str());
    exit(1);
  }

  /* Get the global ports required by MUX (and any submodules) */
  std::vector<CircuitPortId> mux_global_ports = circuit_lib.model_global_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true, true);
  /* Get the input ports from the mux */
  std::vector<CircuitPortId> mux_input_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  /* Get the output ports from the mux */
  std::vector<CircuitPortId> mux_output_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  /* Get the BL and WL ports from the mux */
  std::vector<CircuitPortId> mux_blb_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_BLB, true);
  std::vector<CircuitPortId> mux_wl_ports = circuit_lib.model_ports_by_type(circuit_model, CIRCUIT_MODEL_PORT_WL, true);

  /* Make sure we have a valid file handler*/
  VTR_ASSERT(true == valid_file_stream(fp));

  /* Generate the Verilog netlist according to the mux_graph */
  /* Find out the number of data-path inputs */ 
  size_t num_inputs = find_mux_num_datapath_inputs(circuit_lib, circuit_model, mux_graph.num_inputs());
  /* Find out the number of outputs */ 
  size_t num_outputs = mux_graph.num_outputs();
  /* Find out the number of memory bits */ 
  size_t num_mems = mux_graph.num_memory_bits();

  /* Check codes to ensure the port of Verilog netlists will match */
  /* MUX graph must have only 1 input and 1 BLB and 1 WL port */
  VTR_ASSERT(1 == mux_input_ports.size());
  VTR_ASSERT(1 == mux_blb_ports.size());
  VTR_ASSERT(1 == mux_wl_ports.size());

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId module_id = module_manager.add_module(module_name); 
  VTR_ASSERT(ModuleId::INVALID() != module_id);
  /* Add module ports */
  /* Add each global port */
  for (const auto& port : mux_global_ports) {
    /* Configure each global port */
    BasicPort global_port(circuit_lib.port_prefix(port), circuit_lib.port_size(port));
    module_manager.add_port(module_id, global_port, ModuleManager::MODULE_GLOBAL_PORT);
  }
  /* Add each input port */
  size_t input_port_cnt = 0;
  for (const auto& port : mux_input_ports) {
    BasicPort input_port(circuit_lib.port_prefix(port), num_inputs);
    module_manager.add_port(module_id, input_port, ModuleManager::MODULE_INPUT_PORT);
    /* Update counter */
    input_port_cnt++;
  }
  /* Double check: We should have only 1 input port generated here! */
  VTR_ASSERT(1 == input_port_cnt);

  for (const auto& port : mux_output_ports) {
    BasicPort output_port(circuit_lib.port_prefix(port), num_outputs);
    if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
      output_port.set_width(circuit_lib.port_size(port));
    }
    module_manager.add_port(module_id, output_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* BLB port */
  for (const auto& port : mux_blb_ports) {
    /* IMPORTANT: RRAM-based MUX has an additional BLB pin per level 
     * So, the actual port width of BLB should be added by the number of levels of the MUX graph 
     */
    BasicPort blb_port(circuit_lib.port_prefix(port), num_mems + mux_graph.num_levels());
    module_manager.add_port(module_id, blb_port, ModuleManager::MODULE_INPUT_PORT);
  }

  /* WL port */
  for (const auto& port : mux_wl_ports) {
    /* IMPORTANT: RRAM-based MUX has an additional WL pin per level 
     * So, the actual port width of WL should be added by the number of levels of the MUX graph 
     */
    BasicPort wl_port(circuit_lib.port_prefix(port), num_mems + mux_graph.num_levels());
    module_manager.add_port(module_id, wl_port, ModuleManager::MODULE_INPUT_PORT);
  }
 
  /* dump module definition + ports */
  print_verilog_module_declaration(fp, module_manager, module_id);

  /* TODO: Print the internal logic in Verilog codes */
  generate_verilog_rram_mux_module_multiplexing_structure(module_manager, circuit_lib, fp, module_id, circuit_model, mux_graph);

  /* Print the input and output buffers in Verilog codes */
  /* TODO, we should rename the follow functions to a generic name? Since they are applicable to both MUXes */
  generate_verilog_cmos_mux_module_input_buffers(module_manager, circuit_lib, fp, module_id, circuit_model, mux_graph);
  generate_verilog_cmos_mux_module_output_buffers(module_manager, circuit_lib, fp, module_id, circuit_model, mux_graph);

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
                                 const CircuitModelId& mux_model, 
                                 const MuxGraph& mux_graph,
                                 const bool& use_explicit_port_map) {
  std::string module_name = generate_mux_subckt_name(circuit_lib, mux_model, 
                                                     find_mux_num_datapath_inputs(circuit_lib, mux_model, mux_graph.num_inputs()), 
                                                     std::string(""));
 
  /* Multiplexers built with different technology is in different organization */
  switch (circuit_lib.design_tech_type(mux_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS: {
    /* Use Verilog writer to print the module to file */
    ModuleId mux_module = module_manager.find_module(module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(mux_module));
    write_verilog_module_to_file(fp, module_manager, mux_module, 
                                  ( use_explicit_port_map 
                                 || circuit_lib.dump_explicit_port_map(mux_model) 
                                 || circuit_lib.dump_explicit_port_map(circuit_lib.pass_gate_logic_model(mux_model)) ) 
                                 );
    /* Add an empty line as a splitter */
    fp << std::endl;
    break;
  }
  case CIRCUIT_MODEL_DESIGN_RRAM:
    /* TODO: RRAM-based Multiplexer Verilog module generation */
    generate_verilog_rram_mux_module(module_manager, circuit_lib, fp, mux_model, module_name, mux_graph);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid design technology of multiplexer '%s'\n",
                   circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }
}


/***********************************************
 * Generate Verilog modules for all the unique
 * multiplexers in the FPGA device
 **********************************************/
void print_verilog_submodule_muxes(ModuleManager& module_manager,
                                   std::vector<std::string>& netlist_names,
                                   const MuxLibrary& mux_lib,
                                   const CircuitLibrary& circuit_lib,
                                   const std::string& verilog_dir,
                                   const std::string& submodule_dir,
                                   const bool& use_explicit_port_map) {

  std::string verilog_fname(submodule_dir + std::string(MUXES_VERILOG_FILE_NAME));

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(verilog_fname.c_str(), fp);

  /* Print out debugging information for if the file is not opened/created properly */
  VTR_LOG("Writing Verilog netlist for Multiplexers '%s' ...\n",
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
                                         branch_mux_graph, use_explicit_port_map);
    }
  }

  /* Generate unique Verilog modules for the multiplexers */
  for (auto mux : mux_lib.muxes()) {
    const MuxGraph& mux_graph = mux_lib.mux_graph(mux);
    CircuitModelId mux_circuit_model = mux_lib.mux_circuit_model(mux); 
    /* Create MUX circuits */
    generate_verilog_mux_module(module_manager, circuit_lib, fp, mux_circuit_model, mux_graph, use_explicit_port_map);
  }

  /* Close the file stream */
  fp.close();

  /* Add fname to the netlist name list */
  netlist_names.push_back(verilog_fname);

  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
