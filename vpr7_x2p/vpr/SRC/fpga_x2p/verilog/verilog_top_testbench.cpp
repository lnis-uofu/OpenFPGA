/********************************************************************
 * This file includes functions that are used to create
 * an auto-check top-level testbench for a FPGA fabric
 *******************************************************************/
#include <fstream>
#include <ctime>
#include <iomanip>
#include <algorithm>

#include "vtr_assert.h"
#include "device_port.h"
#include "util.h"

#include "bitstream_manager_utils.h"

#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"
#include "simulation_utils.h"
#include "fpga_x2p_benchmark_utils.h"

#include "verilog_global.h"
#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"
#include "verilog_top_testbench.h"

/********************************************************************
 * Local variables used only in this file
 *******************************************************************/
constexpr char* TOP_TESTBENCH_REFERENCE_INSTANCE_NAME = "REF_DUT";
constexpr char* TOP_TESTBENCH_FPGA_INSTANCE_NAME = "FPGA_DUT";
constexpr char* TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX = "_benchmark";
constexpr char* TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX = "_fpga";

constexpr char* TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX = "_flag";

constexpr char* TOP_TESTBENCH_CC_PROG_TASK_NAME = "prog_cycle_task";

constexpr char* TOP_TESTBENCH_SIM_START_PORT_NAME = "sim_start";

constexpr int TOP_TESTBENCH_MAGIC_NUMBER_FOR_SIMULATION_TIME = 200;
constexpr char* TOP_TESTBENCH_ERROR_COUNTER = "nb_error";

/********************************************************************
 * Print local wires for configuration chain protocols
 *******************************************************************/
static 
void print_verilog_top_testbench_config_chain_port(std::fstream& fp) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* Print the head of configuraion-chains here */
  print_verilog_comment(fp, std::string("---- Configuration-chain head -----"));
  BasicPort config_chain_head_port(generate_configuration_chain_head_name(), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, config_chain_head_port) << ";" << std::endl;

  /* Print the tail of configuration-chains here */
  print_verilog_comment(fp, std::string("---- Configuration-chain tail -----"));
  BasicPort config_chain_tail_port(generate_configuration_chain_tail_name(), 1);
  fp << generate_verilog_port(VERILOG_PORT_WIRE, config_chain_tail_port) << ";" << std::endl;
}

/********************************************************************
 * Print local wires for different types of configuration protocols
 *******************************************************************/
static 
void print_verilog_top_testbench_config_protocol_port(std::fstream& fp,
                                                      const e_sram_orgz& sram_orgz_type) {
  switch(sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* TODO */
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    print_verilog_top_testbench_config_chain_port(fp);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s, [LINE%d]) Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/********************************************************************
 * Wire the global ports of FPGA fabric to local wires
 *******************************************************************/
static 
void print_verilog_top_testbench_global_ports_stimuli(std::fstream& fp,
                                                      const ModuleManager& module_manager,
                                                      const ModuleId& top_module,
                                                      const CircuitLibrary& circuit_lib,
                                                      const std::vector<CircuitPortId>& global_ports) {
  /* Validate the file stream */
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Begin connecting global ports of FPGA fabric to stimuli -----"));

  /* Connect global clock ports to operating or programming clock signal */
  for (const CircuitPortId& model_global_port : global_ports) {
    if (SPICE_MODEL_PORT_CLOCK != circuit_lib.port_type(model_global_port)) {
      continue;
    }
    /* Reach here, it means we have a global clock to deal with:
     * 1. if the port is identified as a programming clock, 
     *    connect it to the local wire of programming clock
     * 2. if the port is identified as an operating clock 
     *    connect it to the local wire of operating clock
     */
    /* Find the module port */
    ModulePortId module_global_port = module_manager.find_module_port(top_module, circuit_lib.port_lib_name(model_global_port));
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort stimuli_clock_port;
    if (true == circuit_lib.port_is_prog(model_global_port)) {
      stimuli_clock_port.set_name(std::string(top_tb_prog_clock_port_name));
      stimuli_clock_port.set_width(1);
    } else {
      VTR_ASSERT_SAFE(false == circuit_lib.port_is_prog(model_global_port));
      stimuli_clock_port.set_name(std::string(top_tb_op_clock_port_name));
      stimuli_clock_port.set_width(1);
    }
    /* Wire the port to the input stimuli:
     * The wiring will be inverted if the default value of the global port is 1
     * Otherwise, the wiring will not be inverted!
     */
    print_verilog_wire_connection(fp, module_manager.module_port(top_module, module_global_port), 
                                  stimuli_clock_port, 
                                  1 == circuit_lib.port_default_value(model_global_port)); 
  }

  /* Connect global configuration done ports to configuration done signal */
  for (const CircuitPortId& model_global_port : global_ports) {
    /* Bypass clock signals, they have been processed */
    if (SPICE_MODEL_PORT_CLOCK == circuit_lib.port_type(model_global_port)) {
      continue;
    }
    if (false == circuit_lib.port_is_config_enable(model_global_port)) {
      continue;
    }
    /* Reach here, it means we have a configuration done port to deal with */
    /* Find the module port */
    ModulePortId module_global_port = module_manager.find_module_port(top_module, circuit_lib.port_lib_name(model_global_port));
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort stimuli_config_done_port(std::string(top_tb_config_done_port_name), 1);
    /* Wire the port to the input stimuli:
     * The wiring will be inverted if the default value of the global port is 1
     * Otherwise, the wiring will not be inverted!
     */
    print_verilog_wire_connection(fp, module_manager.module_port(top_module, module_global_port), 
                                  stimuli_config_done_port, 
                                  1 == circuit_lib.port_default_value(model_global_port)); 
  }

  /* Connect global reset ports to operating or programming reset signal */
  for (const CircuitPortId& model_global_port : global_ports) {
    /* Bypass clock signals, they have been processed */
    if (SPICE_MODEL_PORT_CLOCK == circuit_lib.port_type(model_global_port)) {
      continue;
    }
    /* Bypass config_done signals, they have been processed */
    if (true == circuit_lib.port_is_config_enable(model_global_port)) {
      continue;
    }

    if (false == circuit_lib.port_is_reset(model_global_port)) {
      continue;
    }
    /* Reach here, it means we have a reset port to deal with */
    /* Find the module port */
    ModulePortId module_global_port = module_manager.find_module_port(top_module, circuit_lib.port_lib_name(model_global_port));
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort stimuli_reset_port;
    if (true == circuit_lib.port_is_prog(model_global_port)) {
      stimuli_reset_port.set_name(std::string(top_tb_prog_reset_port_name));
      stimuli_reset_port.set_width(1);
    } else {
      VTR_ASSERT_SAFE(false == circuit_lib.port_is_prog(model_global_port));
      stimuli_reset_port.set_name(std::string(top_tb_reset_port_name));
      stimuli_reset_port.set_width(1);
    }
    /* Wire the port to the input stimuli:
     * The wiring will be inverted if the default value of the global port is 1
     * Otherwise, the wiring will not be inverted!
     */
    print_verilog_wire_connection(fp, module_manager.module_port(top_module, module_global_port), 
                                  stimuli_reset_port, 
                                  1 == circuit_lib.port_default_value(model_global_port)); 
  }

  /* Connect global set ports to operating or programming set signal */
  for (const CircuitPortId& model_global_port : global_ports) {
    /* Bypass clock signals, they have been processed */
    if (SPICE_MODEL_PORT_CLOCK == circuit_lib.port_type(model_global_port)) {
      continue;
    }
    /* Bypass config_done signals, they have been processed */
    if (true == circuit_lib.port_is_config_enable(model_global_port)) {
      continue;
    }

    /* Bypass reset signals, they have been processed */
    if (true == circuit_lib.port_is_reset(model_global_port)) {
      continue;
    }

    if (false == circuit_lib.port_is_set(model_global_port)) {
      continue;
    }
    /* Reach here, it means we have a set port to deal with */
    /* Find the module port */
    ModulePortId module_global_port = module_manager.find_module_port(top_module, circuit_lib.port_lib_name(model_global_port));
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort stimuli_set_port;
    if (true == circuit_lib.port_is_prog(model_global_port)) {
      stimuli_set_port.set_name(std::string(top_tb_prog_set_port_name));
      stimuli_set_port.set_width(1);
    } else {
      VTR_ASSERT_SAFE(false == circuit_lib.port_is_prog(model_global_port));
      stimuli_set_port.set_name(std::string(top_tb_set_port_name));
      stimuli_set_port.set_width(1);
    }
    /* Wire the port to the input stimuli:
     * The wiring will be inverted if the default value of the global port is 1
     * Otherwise, the wiring will not be inverted!
     */
    print_verilog_wire_connection(fp, module_manager.module_port(top_module, module_global_port), 
                                  stimuli_set_port, 
                                  1 == circuit_lib.port_default_value(model_global_port)); 
  }

  /* For the rest of global ports, wire them to constant signals */
  for (const CircuitPortId& model_global_port : global_ports) {
    /* Bypass clock signals, they have been processed */
    if (SPICE_MODEL_PORT_CLOCK == circuit_lib.port_type(model_global_port)) {
      continue;
    }
    /* Bypass config_done signals, they have been processed */
    if (true == circuit_lib.port_is_config_enable(model_global_port)) {
      continue;
    }

    /* Bypass reset signals, they have been processed */
    if (true == circuit_lib.port_is_reset(model_global_port)) {
      continue;
    }

    /* Bypass set signals, they have been processed */
    if (true == circuit_lib.port_is_set(model_global_port)) {
      continue;
    }

    /* Reach here, it means we have a port to deal with */
    /* Find the module port and wire it to constant values */
    ModulePortId module_global_port = module_manager.find_module_port(top_module, circuit_lib.port_lib_name(model_global_port));
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort module_port = module_manager.module_port(top_module, module_global_port);
    std::vector<size_t> default_values(module_port.get_width(), circuit_lib.port_default_value(model_global_port));
    print_verilog_wire_constant_values(fp, module_port, default_values);
  }

  print_verilog_comment(fp, std::string("----- End connecting global ports of FPGA fabric to stimuli -----"));
}

/********************************************************************
 * This function prints the top testbench module declaration 
 * and internal wires/port declaration
 * Ports can be classified in two categories:
 * 1. General-purpose ports, which are datapath I/Os, clock signals
 *    for the FPGA fabric and input benchmark
 * 2. Fabric-featured ports, which are required by configuration
 *    protocols.
 *    Due the difference in configuration protocols, the internal
 *    wires and ports will be different:
 *    (a) configuration-chain: we will have two ports,
 *        a head and a tail for the configuration chain,
 *        in addition to the regular ports.
 *    (b) memory-decoders: we will have a few ports to drive 
 *        address lines for decoders and a bit input port to feed
 *        configuration bits
 *******************************************************************/
static 
void print_verilog_top_testbench_ports(std::fstream& fp,
                                       const ModuleManager& module_manager,
                                       const ModuleId& top_module,
                                       const std::vector<t_logical_block>& L_logical_blocks,
                                       const e_sram_orgz& sram_orgz_type,
                                       const std::string& circuit_name){
  /* Validate the file stream */
  check_file_handler(fp);

  /* Print module definition */
  fp << "module " << circuit_name << std::string(modelsim_autocheck_testbench_module_postfix);
  fp << ";" << std::endl;

  /* Print regular local wires:
   * 1. global ports, i.e., reset, set and clock signals
   * 2. datapath I/O signals
   */
  /* Global ports of top-level module  */
  print_verilog_comment(fp, std::string("----- Local wires for global ports of FPGA fabric -----"));
  for (const BasicPort& module_port : module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GLOBAL_PORT)) {
    fp << generate_verilog_port(VERILOG_PORT_WIRE, module_port) << ";" << std::endl;
  }
  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Datapath I/Os of top-level module  */
  print_verilog_comment(fp, std::string("----- Local wires for I/Os of FPGA fabric -----"));
  for (const BasicPort& module_port : module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIO_PORT)) {
    fp << generate_verilog_port(VERILOG_PORT_WIRE, module_port) << ";" << std::endl;
  }
  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Add local wires/registers that drive stimulus
   * We create these general purpose ports here,
   * and then wire them to the ports of FPGA fabric depending on their usage 
   */
  /* Configuration done port */
  BasicPort config_done_port(std::string(top_tb_config_done_port_name), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, config_done_port) << ";" << std::endl;

  /* Programming clock */
  BasicPort prog_clock_port(std::string(top_tb_prog_clock_port_name), 1);
  fp << generate_verilog_port(VERILOG_PORT_WIRE, prog_clock_port) << ";" << std::endl;
  BasicPort prog_clock_register_port(std::string(std::string(top_tb_prog_clock_port_name) + std::string(top_tb_clock_reg_postfix)), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, prog_clock_register_port) << ";" << std::endl;

  /* Operating clock */
  BasicPort op_clock_port(std::string(top_tb_op_clock_port_name), 1);
  fp << generate_verilog_port(VERILOG_PORT_WIRE, op_clock_port) << ";" << std::endl;
  BasicPort op_clock_register_port(std::string(std::string(top_tb_op_clock_port_name) + std::string(top_tb_clock_reg_postfix)), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, op_clock_register_port) << ";" << std::endl;

  /* Programming set and reset */
  BasicPort prog_reset_port(std::string(top_tb_prog_reset_port_name), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, prog_reset_port) << ";" << std::endl;
  BasicPort prog_set_port(std::string(top_tb_prog_set_port_name), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, prog_set_port) << ";" << std::endl;

  /* Global set and reset */
  BasicPort reset_port(std::string(top_tb_reset_port_name), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, reset_port) << ";" << std::endl;
  BasicPort set_port(std::string(top_tb_set_port_name), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, set_port) << ";" << std::endl;

  /* Configuration ports depend on the organization of SRAMs */
  print_verilog_top_testbench_config_protocol_port(fp, sram_orgz_type);

  print_verilog_testbench_shared_ports(fp, L_logical_blocks,
                                       std::string(TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX),
                                       std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX),
                                       std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                       std::string(autochecked_simulation_flag));

  /* Instantiate an integer to count the number of error and 
   * determine if the simulation succeed or failed
   */
  print_verilog_comment(fp, std::string("----- Error counter -----"));
  fp << "\tinteger " << TOP_TESTBENCH_ERROR_COUNTER << "= 0;" << std::endl;
}

/********************************************************************
 * Instanciate the input benchmark module
 *******************************************************************/
static
void print_verilog_top_testbench_benchmark_instance(std::fstream& fp, 
                                                    const std::string& reference_verilog_top_name,
                                                    const std::vector<t_logical_block>& L_logical_blocks) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* Benchmark is instanciated conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(autochecked_simulation_flag)); 

  print_verilog_comment(fp, std::string("----- Reference Benchmark Instanication -------"));

  /* Do NOT use explicit port mapping here: 
   * VPR added a prefix of "out_" to the output ports of input benchmark
   */
  print_verilog_testbench_benchmark_instance(fp, reference_verilog_top_name,
                                             std::string(TOP_TESTBENCH_REFERENCE_INSTANCE_NAME),
                                             std::string(),
                                             std::string(),
                                             std::string(TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX), L_logical_blocks,
                                             false);

  print_verilog_comment(fp, std::string("----- End reference Benchmark Instanication -------"));

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Condition ends for the benchmark instanciation */
  print_verilog_endif(fp);

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print tasks (processes) in Verilog format, 
 * which is very useful in generating stimuli for each clock cycle 
 * This function is tuned for configuration-chain manipulation: 
 * During each programming cycle, we feed the input of scan chain with a memory bit
 *******************************************************************/
static 
void print_verilog_top_testbench_load_bitstream_task_configuration_chain(std::fstream& fp) {

  /* Validate the file stream */
  check_file_handler(fp);

  BasicPort prog_clock_port(std::string(top_tb_prog_clock_port_name), 1);
  BasicPort cc_head_port(generate_configuration_chain_head_name(), 1);
  BasicPort cc_head_value(generate_configuration_chain_head_name() + std::string("_val"), 1);

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Feed the scan-chain input at each falling edge of programming clock 
   * It aims at avoid racing the programming clock (scan-chain data changes at the rising edge). 
   */
  print_verilog_comment(fp, std::string("----- Task: input values during a programming clock cycle -----"));
  fp << "task " << std::string(TOP_TESTBENCH_CC_PROG_TASK_NAME) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_INPUT, cc_head_value) << ";" << std::endl;
  fp << "\tbegin" << std::endl;
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;
  fp << "\t\t\t"; 
  fp << generate_verilog_port(VERILOG_PORT_CONKT, cc_head_port);
  fp << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, cc_head_value);
  fp << ";" << std::endl;

  fp << "\tend" << std::endl;
  fp << "endtask" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print tasks, which is very useful in generating stimuli for each clock cycle 
 *******************************************************************/
static 
void print_verilog_top_testbench_load_bitstream_task(std::fstream& fp,
                                                     const e_sram_orgz& sram_orgz_type) {
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    print_verilog_top_testbench_load_bitstream_task_configuration_chain(fp);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO: 
    dump_verilog_top_testbench_stimuli_serial_version_tasks_memory_bank(cur_sram_orgz_info, fp);
     */
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/********************************************************************
 * Print generatic input stimuli for the top testbench
 * include:
 * 1. configuration done signal
 * 2. programming clock
 * 3. operating clock
 * 4. programming reset signal
 * 5. programming set signal
 * 6. reset signal
 * 7. set signal
 *******************************************************************/
static 
void print_verilog_top_testbench_generic_stimulus(std::fstream& fp,
                                                  const size_t& num_config_clock_cycles,
                                                  const float& prog_clock_period,
                                                  const float& op_clock_period,
                                                  const float& timescale) {
  /* Validate the file stream */
  check_file_handler(fp);

  print_verilog_comment(fp, std::string("----- Number of clock cycles in configuration phase: " + std::to_string(num_config_clock_cycles) + " -----")); 

  BasicPort config_done_port(std::string(top_tb_config_done_port_name), 1);

  BasicPort op_clock_port(std::string(top_tb_op_clock_port_name), 1);
  BasicPort op_clock_register_port(std::string(std::string(top_tb_op_clock_port_name) + std::string(top_tb_clock_reg_postfix)), 1);

  BasicPort prog_clock_port(std::string(top_tb_prog_clock_port_name), 1);
  BasicPort prog_clock_register_port(std::string(std::string(top_tb_prog_clock_port_name) + std::string(top_tb_clock_reg_postfix)), 1);

  BasicPort prog_reset_port(std::string(top_tb_prog_reset_port_name), 1);
  BasicPort prog_set_port(std::string(top_tb_prog_set_port_name), 1);

  BasicPort reset_port(std::string(top_tb_reset_port_name), 1);
  BasicPort set_port(std::string(top_tb_set_port_name), 1);

  /* Generate stimuli waveform for configuration done signals */
  print_verilog_comment(fp, "----- Begin configuration done signal generation -----"); 
  print_verilog_pulse_stimuli(fp, config_done_port, 
                              0, /* Initial value */
                              num_config_clock_cycles * prog_clock_period / timescale, 0); 
  print_verilog_comment(fp, "----- End configuration done signal generation -----"); 
  fp << std::endl;

  /* Generate stimuli waveform for programming clock signals */
  print_verilog_comment(fp, "----- Begin raw programming clock signal generation -----"); 
  print_verilog_clock_stimuli(fp, prog_clock_register_port, 
                              0, /* Initial value */
                              0.5 * prog_clock_period / timescale,
                              std::string()); 
  print_verilog_comment(fp, "----- End raw programming clock signal generation -----"); 
  fp << std::endl;

  /* Programming clock should be only enabled during programming phase.
   * When configuration is done (config_done is enabled), programming clock should be always zero.
   */
  print_verilog_comment(fp, std::string("----- Actual programming clock is triggered only when " + config_done_port.get_name() + " and " + prog_reset_port.get_name() + " are disabled -----")); 
  fp << "\tassign " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port);
  fp << " = " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_register_port);
  fp << " & (~" << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port) << ")";
  fp << " & (~" << generate_verilog_port(VERILOG_PORT_CONKT, prog_reset_port) << ")";
  fp << ";" << std::endl;

  fp << std::endl;

  /* Generate stimuli waveform for operating clock signals */
  print_verilog_comment(fp, "----- Begin raw operating clock signal generation -----"); 
  print_verilog_clock_stimuli(fp, op_clock_register_port, 
                              0, /* Initial value */
                              0.5 * op_clock_period / timescale,
                              std::string("~" + reset_port.get_name())); 
  print_verilog_comment(fp, "----- End raw operating clock signal generation -----"); 

  /* Operation clock should be enabled after programming phase finishes.
   * Before configuration is done (config_done is enabled), operation clock should be always zero.
   */
  print_verilog_comment(fp, std::string("----- Actual operating clock is triggered only when " + config_done_port.get_name() + " is enabled -----")); 
  fp << "\tassign " << generate_verilog_port(VERILOG_PORT_CONKT, op_clock_port);
  fp << " = " << generate_verilog_port(VERILOG_PORT_CONKT, op_clock_register_port);
  fp << " & (~" << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port) << ")";
  fp << ";" << std::endl;

  fp << std::endl;

  /* Reset signal for configuration circuit: 
   * only enable during the first clock cycle in programming phase 
   */
  print_verilog_comment(fp, "----- Begin programming reset signal generation -----"); 
  print_verilog_pulse_stimuli(fp, prog_reset_port, 
                              1, /* Initial value */
                              prog_clock_period / timescale, 0); 
  print_verilog_comment(fp, "----- End programming reset signal generation -----"); 

  fp << std::endl;

  /* Programming set signal for configuration circuit : always disabled */
  print_verilog_comment(fp, "----- Begin programming set signal generation: always disabled -----"); 
  print_verilog_pulse_stimuli(fp, prog_set_port, 
                              0, /* Initial value */
                              prog_clock_period / timescale, 0); 
  print_verilog_comment(fp, "----- End programming set signal generation: always disabled -----"); 

  fp << std::endl;

  /* Operating reset signals: only enabled during the first clock cycle in operation phase */
  std::vector<float> reset_pulse_widths;
  reset_pulse_widths.push_back(op_clock_period / timescale);
  reset_pulse_widths.push_back(2 * op_clock_period / timescale);

  std::vector<size_t> reset_flip_values;
  reset_flip_values.push_back(1);
  reset_flip_values.push_back(0);

  print_verilog_comment(fp, "----- Begin operating reset signal generation -----"); 
  print_verilog_comment(fp, "----- Reset signal is enabled until the first clock cycle in operation phase -----");
  print_verilog_pulse_stimuli(fp, reset_port,
                              1,
                              reset_pulse_widths,
                              reset_flip_values,
                              config_done_port.get_name());
  print_verilog_comment(fp, "----- End operating reset signal generation -----"); 

  /* Operating set signal for configuration circuit : always disabled */
  print_verilog_comment(fp, "----- Begin operating set signal generation: always disabled -----"); 
  print_verilog_pulse_stimuli(fp, set_port, 
                              0, /* Initial value */
                              op_clock_period / timescale, 0); 
  print_verilog_comment(fp, "----- End operating set signal generation: always disabled -----"); 

  fp << std::endl;
}

/********************************************************************
 * Print stimulus for a FPGA fabric with a configuration chain protocol 
 * where configuration bits are programming in serial (one by one)
 * Task list:
 * 1. For clock signal, we should create voltage waveforms for two types of clock signals:
 *    a. operation clock
 *    b. programming clock 
 * 2. For Set/Reset, we reset the chip after programming phase ends 
 *    and before operation phase starts
 * 3. For input/output clb nets (mapped to I/O grids), 
 *    we should create voltage waveforms only after programming phase 
 *******************************************************************/
static 
void print_verilog_top_testbench_configuration_chain_bitstream(std::fstream& fp,
                                                               const BitstreamManager& bitstream_manager,
                                                               const std::vector<ConfigBitId>& fabric_bitstream) {
  /* Validate the file stream */
  check_file_handler(fp);

  /* Initial value should be the first configuration bits
   * In the rest of programming cycles, 
   * configuration bits are fed at the falling edge of programming clock.
   * We do not care the value of scan_chain head during the first programming cycle 
   * It is reset anyway
   */
  BasicPort config_chain_head_port(generate_configuration_chain_head_name(), 1);
  std::vector<size_t> initial_values(config_chain_head_port.get_width(), 0);

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;
  print_verilog_comment(fp, "----- Configuration chain default input -----");
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(config_chain_head_port, initial_values);
  fp << ";";

  fp << std::endl;

  /* Attention: the configuration chain protcol requires the last configuration bit is fed first
   * We will visit the fabric bitstream in a reverse way  
   */
  std::vector<ConfigBitId> cc_bitstream = fabric_bitstream;
  std::reverse(cc_bitstream.begin(), cc_bitstream.end());
  for (const ConfigBitId& bit_id : cc_bitstream) {
    fp << "\t\t" << std::string(TOP_TESTBENCH_CC_PROG_TASK_NAME);
    fp << "(1'b" << (size_t)bitstream_manager.bit_value(bit_id) << ");" << std::endl;
  }

  /* Raise the flag of configuration done when bitstream loading is complete */
  BasicPort prog_clock_port(std::string(top_tb_prog_clock_port_name), 1);
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;
  
  BasicPort config_done_port(std::string(top_tb_config_done_port_name), 1);
  fp << "\t\t\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port);
  fp << " <= ";
  std::vector<size_t> config_done_enable_values(config_done_port.get_width(), 1);
  fp << generate_verilog_constant_values(config_done_enable_values);
  fp << ";" << std::endl;

  fp << "\tend" << std::endl;
  print_verilog_comment(fp, "----- End bitstream loading during configuration phase -----");
}

/********************************************************************
 * Generate the stimuli for the top-level testbench 
 * The simulation consists of two phases: configuration phase and operation phase
 * Configuration bits are loaded serially. 
 * This is actually what we do for a physical FPGA
 *******************************************************************/
static 
void print_verilog_top_testbench_bitstream(std::fstream& fp,
                                           const e_sram_orgz& sram_orgz_type,
                                           const BitstreamManager& bitstream_manager,
                                           const std::vector<ConfigBitId>& fabric_bitstream) {
  /* Branch on the type of configuration protocol */
  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE:
    /* TODO */
    break;
  case SPICE_SRAM_SCAN_CHAIN:
    print_verilog_top_testbench_configuration_chain_bitstream(fp, bitstream_manager, fabric_bitstream);
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* TODO */
    break;
  default: 
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid SRAM organization type!\n",
               __FILE__, __LINE__);
    exit(1);
  }
}

/********************************************************************
 * The top-level function to generate a testbench, in order to verify: 
 * 1. Configuration phase of the FPGA fabric, where the bitstream is 
 *    loaded to the configuration protocol of the FPGA fabric
 * 2. Operating phase of the FPGA fabric, where input stimuli are
 *    fed to the I/Os of the FPGA fabric
 *                                    +----------+
 *                                    |   FPGA   |       +------------+
 *                             +----->|  Fabric  |------>|            |
 *                             |      |          |       |            |
 *                             |      +----------+       |            |
 *                             |                         | Output     | 
 *   random_input_vectors -----+                         | Vector     |---->Functional correct?
 *                             |                         | Comparator |    
 *                             |      +-----------+      |            |
 *                             |      |  Input    |      |            |
 *                             +----->| Benchmark |----->|            |
 *                                    +-----------+      +------------+
 *
 *******************************************************************/
void print_verilog_top_testbench(const ModuleManager& module_manager,
                                 const BitstreamManager& bitstream_manager,
                                 const std::vector<ConfigBitId>& fabric_bitstream,
                                 const e_sram_orgz& sram_orgz_type,
                                 const CircuitLibrary& circuit_lib,
                                 const std::vector<CircuitPortId>& global_ports,
                                 const std::vector<t_logical_block>& L_logical_blocks,
                                 const vtr::Point<size_t>& device_size,
                                 const std::vector<std::vector<t_grid_tile>>& L_grids, 
                                 const std::vector<t_block>& L_blocks,
                                 const std::string& circuit_name,
                                 const std::string& verilog_fname,
                                 const std::string& verilog_dir,
                                 const t_spice_params& simulation_parameters) {
  vpr_printf(TIO_MESSAGE_INFO, 
             "Writing Autocheck Testbench for FPGA Top-level Verilog netlist for %s...", 
             circuit_name.c_str());

  /* Start time count */
  clock_t t_start = clock();

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_handler(fp);

  /* Generate a brief description on the Verilog file*/
  std::string title = std::string("FPGA Verilog Testbench for Top-level netlist of Design: ") + circuit_name;
  print_verilog_file_header(fp, title); 

  /* Print preprocessing flags and external netlists */
  print_verilog_include_defines_preproc_file(fp, verilog_dir);

  /* Find the top_module */
  ModuleId top_module = module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Start of testbench */
  //dump_verilog_top_auto_testbench_ports(fp, cur_sram_orgz_info, circuit_name, fpga_verilog_opts);
  print_verilog_top_testbench_ports(fp, module_manager, top_module, L_logical_blocks,
                                    sram_orgz_type, circuit_name);

  /* Find the clock period */
  float prog_clock_period = (1./simulation_parameters.stimulate_params.prog_clock_freq);
  float op_clock_period = (1./simulation_parameters.stimulate_params.op_clock_freq);
  /* Estimate the number of configuration clock cycles 
   * by traversing the linked-list and count the number of SRAM=1 or BL=1&WL=1 in it.
   * We plus 1 additional config clock cycle here because we need to reset everything during the first clock cycle
   */
  size_t num_config_clock_cycles = 1 + fabric_bitstream.size();

  /* Generate stimuli for general control signals */
  print_verilog_top_testbench_generic_stimulus(fp,
                                               num_config_clock_cycles,
                                               prog_clock_period,
                                               op_clock_period,
                                               verilog_sim_timescale);

  /* Generate stimuli for global ports or connect them to existed signals */
  print_verilog_top_testbench_global_ports_stimuli(fp,
                                                   module_manager, top_module,
                                                   circuit_lib, global_ports);

  /* Instanciate FPGA top-level module */
  print_verilog_testbench_fpga_instance(fp, module_manager, top_module, 
                                        std::string(TOP_TESTBENCH_FPGA_INSTANCE_NAME)); 
  
  /* Connect I/Os to benchmark I/Os or constant driver */
  print_verilog_testbench_connect_fpga_ios(fp, module_manager, top_module,
                                           L_logical_blocks, device_size, L_grids, 
                                           L_blocks, 
                                           std::string(), 
                                           std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX), 
                                           (size_t)verilog_default_signal_init_value);

  /* Instanciate input benchmark */
  print_verilog_top_testbench_benchmark_instance(fp, 
                                                 circuit_name,
                                                 L_logical_blocks);

  /* Print tasks used for loading bitstreams */
  print_verilog_top_testbench_load_bitstream_task(fp, sram_orgz_type);

  /* load bitstream to FPGA fabric in a configuration phase */
  print_verilog_top_testbench_bitstream(fp, sram_orgz_type,
                                        bitstream_manager, fabric_bitstream);

  /* Preparation: find all the clock ports */
  std::vector<std::string> clock_port_names = find_benchmark_clock_port_name(L_logical_blocks);

  /* Add stimuli for reset, set, clock and iopad signals */
  print_verilog_testbench_random_stimuli(fp, L_logical_blocks, 
                                         std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                         BasicPort(std::string(top_tb_op_clock_port_name), 1));

  /* Add output autocheck */
  print_verilog_testbench_check(fp, 
                                std::string(autochecked_simulation_flag),
                                std::string(TOP_TESTBENCH_SIM_START_PORT_NAME),
                                std::string(TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX),
                                std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX),
                                std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                std::string(TOP_TESTBENCH_ERROR_COUNTER),
                                L_logical_blocks, clock_port_names, std::string(top_tb_op_clock_port_name));

  /* Find simulation time */
  float simulation_time = find_simulation_time_period(verilog_sim_timescale,
                                                      num_config_clock_cycles,
										              1./simulation_parameters.stimulate_params.prog_clock_freq,
                                                      simulation_parameters.meas_params.sim_num_clock_cycle,
                                                      1./simulation_parameters.stimulate_params.op_clock_freq);


  /* Add Icarus requirement */
  print_verilog_timeout_and_vcd(fp, 
                                std::string(icarus_simulator_flag),
                                std::string(circuit_name + std::string(modelsim_autocheck_testbench_module_postfix)),
                                std::string(circuit_name + std::string("_formal.vcd")), 
                                std::string(TOP_TESTBENCH_SIM_START_PORT_NAME),
                                std::string(TOP_TESTBENCH_ERROR_COUNTER),
                                (int)simulation_time);


  /* Testbench ends*/
  print_verilog_module_end(fp, std::string(circuit_name) + std::string(modelsim_autocheck_testbench_module_postfix));

  /* Close the file stream */
  fp.close();

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %g seconds\n", 
             run_time_sec);  
}
