/********************************************************************
 * This file includes functions that are used to create
 * an auto-check top-level testbench for a FPGA fabric
 *******************************************************************/
#include <fstream>
#include <iomanip>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"

#include "bitstream_manager_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "simulation_utils.h"
#include "openfpga_atom_netlist_utils.h"

#include "fast_configuration.h"
#include "fabric_bitstream_utils.h"
#include "fabric_global_port_info_utils.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"
#include "verilog_top_testbench.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Local variables used only in this file
 *******************************************************************/
constexpr char* TOP_TESTBENCH_REFERENCE_INSTANCE_NAME = "REF_DUT";
constexpr char* TOP_TESTBENCH_FPGA_INSTANCE_NAME = "FPGA_DUT";
constexpr char* TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX = "_benchmark";
constexpr char* TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX = "_fpga";

constexpr char* TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX = "_flag";

constexpr char* TOP_TESTBENCH_PROG_TASK_NAME = "prog_cycle_task";

constexpr char* TOP_TESTBENCH_SIM_START_PORT_NAME = "sim_start";

constexpr char* TOP_TESTBENCH_ERROR_COUNTER = "nb_error";

constexpr char* TOP_TB_RESET_PORT_NAME = "greset";
constexpr char* TOP_TB_SET_PORT_NAME = "gset";
constexpr char* TOP_TB_PROG_RESET_PORT_NAME = "prog_reset";
constexpr char* TOP_TB_PROG_SET_PORT_NAME = "prog_set";
constexpr char* TOP_TB_CONFIG_DONE_PORT_NAME = "config_done";
constexpr char* TOP_TB_OP_CLOCK_PORT_NAME = "op_clock";
constexpr char* TOP_TB_OP_CLOCK_PORT_PREFIX = "operating_clk_";
constexpr char* TOP_TB_PROG_CLOCK_PORT_NAME = "prog_clock";
constexpr char* TOP_TB_INOUT_REG_POSTFIX = "_reg";
constexpr char* TOP_TB_CLOCK_REG_POSTFIX = "_reg";
constexpr char* TOP_TB_BITSTREAM_LENGTH_VARIABLE = "BITSTREAM_LENGTH";
constexpr char* TOP_TB_BITSTREAM_WIDTH_VARIABLE = "BITSTREAM_WIDTH";
constexpr char* TOP_TB_BITSTREAM_MEM_REG_NAME = "bit_mem";
constexpr char* TOP_TB_BITSTREAM_INDEX_REG_NAME = "bit_index";
constexpr char* TOP_TB_BITSTREAM_ITERATOR_REG_NAME = "ibit";
constexpr char* TOP_TB_BITSTREAM_SKIP_FLAG_REG_NAME = "skip_bits";

constexpr char* AUTOCHECK_TOP_TESTBENCH_VERILOG_MODULE_POSTFIX = "_autocheck_top_tb";

/********************************************************************
 * Generate a simulation clock port name
 * This function is designed to produce a uniform clock naming for these ports
 *******************************************************************/
static 
std::string generate_top_testbench_clock_name(const std::string& prefix, 
                                              const std::string& port_name) {
  return prefix + port_name;
}


/********************************************************************
 * Print local wires for flatten memory (standalone) configuration protocols
 *******************************************************************/
static
void print_verilog_top_testbench_flatten_memory_port(std::fstream& fp,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& top_module) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Print the port for Bit-Line */
  print_verilog_comment(fp, std::string("---- Bit Line ports -----"));
  ModulePortId bl_port_id = module_manager.find_module_port(top_module, std::string(MEMORY_BL_PORT_NAME));
  BasicPort bl_port = module_manager.module_port(top_module, bl_port_id);
  fp << generate_verilog_port(VERILOG_PORT_REG, bl_port) << ";" << std::endl;

  /* Print the port for Word-Line */
  print_verilog_comment(fp, std::string("---- Word Line ports -----"));
  ModulePortId wl_port_id = module_manager.find_module_port(top_module, std::string(MEMORY_WL_PORT_NAME));
  BasicPort wl_port = module_manager.module_port(top_module, wl_port_id);
  fp << generate_verilog_port(VERILOG_PORT_REG, wl_port) << ";" << std::endl;
}


/********************************************************************
 * Print local wires for configuration chain protocols
 *******************************************************************/
static
void print_verilog_top_testbench_config_chain_port(std::fstream& fp,
                                                   const ModuleManager& module_manager,
                                                   const ModuleId& top_module) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Print the head of configuraion-chains here */
  print_verilog_comment(fp, std::string("---- Configuration-chain head -----"));
  ModulePortId cc_head_port_id = module_manager.find_module_port(top_module, generate_configuration_chain_head_name());
  BasicPort config_chain_head_port = module_manager.module_port(top_module, cc_head_port_id);
  fp << generate_verilog_port(VERILOG_PORT_REG, config_chain_head_port) << ";" << std::endl;

  /* Print the tail of configuration-chains here */
  print_verilog_comment(fp, std::string("---- Configuration-chain tail -----"));
  ModulePortId cc_tail_port_id = module_manager.find_module_port(top_module, generate_configuration_chain_tail_name());
  BasicPort config_chain_tail_port = module_manager.module_port(top_module, cc_tail_port_id);
  fp << generate_verilog_port(VERILOG_PORT_WIRE, config_chain_tail_port) << ";" << std::endl;
}

/********************************************************************
 * Print local wires for memory bank configuration protocols
 *******************************************************************/
static
void print_verilog_top_testbench_memory_bank_port(std::fstream& fp,
                                                  const ModuleManager& module_manager,
                                                  const ModuleId& top_module) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Print the address port for the Bit-Line decoder here */
  print_verilog_comment(fp, std::string("---- Address port for Bit-Line decoder -----"));
  ModulePortId bl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port = module_manager.module_port(top_module, bl_addr_port_id);

  fp << generate_verilog_port(VERILOG_PORT_REG, bl_addr_port) << ";" << std::endl;

  /* Print the address port for the Word-Line decoder here */
  print_verilog_comment(fp, std::string("---- Address port for Word-Line decoder -----"));
  ModulePortId wl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port = module_manager.module_port(top_module, wl_addr_port_id);

  fp << generate_verilog_port(VERILOG_PORT_REG, wl_addr_port) << ";" << std::endl;

  /* Print the data-input port for the frame-based decoder here */
  print_verilog_comment(fp, std::string("---- Data input port for memory decoders -----"));
  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  fp << generate_verilog_port(VERILOG_PORT_REG, din_port) << ";" << std::endl;

  /* Generate enable signal waveform here:
   * which is a 90 degree phase shift than the programming clock   
   */
  print_verilog_comment(fp, std::string("---- Wire enable port of memory decoders  -----"));
  ModulePortId en_port_id = module_manager.find_module_port(top_module,
                                                            std::string(DECODER_ENABLE_PORT_NAME));
  BasicPort en_port = module_manager.module_port(top_module, en_port_id);
  BasicPort en_register_port(std::string(en_port.get_name() + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);

  fp << generate_verilog_port(VERILOG_PORT_WIRE, en_port) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_REG, en_register_port) << ";" << std::endl;

  write_tab_to_file(fp, 1);
  fp << "assign ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, en_port);
  fp << "= ";
  fp << "~" << generate_verilog_port(VERILOG_PORT_CONKT, en_register_port);
  fp << " & ";
  fp << "~" << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port);
  fp << ";" << std::endl;
}


/********************************************************************
 * Print local wires for frame-based decoder protocols
 *******************************************************************/
static
void print_verilog_top_testbench_frame_decoder_port(std::fstream& fp,
                                                    const ModuleManager& module_manager,
                                                    const ModuleId& top_module) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Print the address port for the frame-based decoder here */
  print_verilog_comment(fp, std::string("---- Address port for frame-based decoder -----"));
  ModulePortId addr_port_id = module_manager.find_module_port(top_module,
                                                              std::string(DECODER_ADDRESS_PORT_NAME));
  BasicPort addr_port = module_manager.module_port(top_module, addr_port_id);

  fp << generate_verilog_port(VERILOG_PORT_REG, addr_port) << ";" << std::endl;

  /* Print the data-input port for the frame-based decoder here */
  print_verilog_comment(fp, std::string("---- Data input port for frame-based decoder -----"));
  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  fp << generate_verilog_port(VERILOG_PORT_REG, din_port) << ";" << std::endl;

  /* Generate enable signal waveform here:
   * which is a 90 degree phase shift than the programming clock   
   */
  print_verilog_comment(fp, std::string("---- Wire enable port of frame-based decoders  -----"));
  ModulePortId en_port_id = module_manager.find_module_port(top_module,
                                                            std::string(DECODER_ENABLE_PORT_NAME));
  BasicPort en_port = module_manager.module_port(top_module, en_port_id);
  BasicPort en_register_port(std::string(en_port.get_name() + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);

  fp << generate_verilog_port(VERILOG_PORT_WIRE, en_port) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_REG, en_register_port) << ";" << std::endl;

  write_tab_to_file(fp, 1);
  fp << "assign ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, en_port);
  fp << "= ";
  fp << "~" << generate_verilog_port(VERILOG_PORT_CONKT, en_register_port);
  fp << " & ";
  fp << "~" << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port);
  fp << ";" << std::endl;
}

/********************************************************************
 * Print local wires for different types of configuration protocols
 *******************************************************************/
static
void print_verilog_top_testbench_config_protocol_port(std::fstream& fp,
                                                      const ConfigProtocol& config_protocol,
                                                      const ModuleManager& module_manager,
                                                      const ModuleId& top_module) {
  switch(config_protocol.type()) {
  case CONFIG_MEM_STANDALONE:
    print_verilog_top_testbench_flatten_memory_port(fp, module_manager, top_module);
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    print_verilog_top_testbench_config_chain_port(fp, module_manager, top_module);
    break;
  case CONFIG_MEM_MEMORY_BANK:
    print_verilog_top_testbench_memory_bank_port(fp, module_manager, top_module);
    break;
  case CONFIG_MEM_FRAME_BASED:
    print_verilog_top_testbench_frame_decoder_port(fp,
                                                   module_manager, top_module);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization!\n");
    exit(1);
  }
}

/********************************************************************
 * Wire the global clock ports of FPGA fabric to local wires
 *******************************************************************/
static
void print_verilog_top_testbench_global_clock_ports_stimuli(std::fstream& fp,
                                                            const ModuleManager& module_manager,
                                                            const ModuleId& top_module,
                                                            const FabricGlobalPortInfo& fabric_global_port_info,
                                                            const SimulationSetting& simulation_parameters) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Connect global clock ports to operating or programming clock signal */
  for (const FabricGlobalPortId& fabric_global_port : fabric_global_port_info.global_ports()) {
    if (false == fabric_global_port_info.global_port_is_clock(fabric_global_port)) {
      continue;
    }
    /* Reach here, it means we have a global clock to deal with:
     * 1. if the port is identified as a programming clock,
     *    connect it to the local wire of programming clock
     * 2. if the port is identified as an operating clock
     *    connect it to the local wire of operating clock
     */
    /* Find the module port */
    ModulePortId module_global_port = fabric_global_port_info.global_module_port(fabric_global_port);
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort stimuli_clock_port;
    if (true == fabric_global_port_info.global_port_is_prog(fabric_global_port)) {
      stimuli_clock_port.set_name(std::string(TOP_TB_PROG_CLOCK_PORT_NAME));
      stimuli_clock_port.set_width(1);
    } else {
      VTR_ASSERT_SAFE(false == fabric_global_port_info.global_port_is_prog(fabric_global_port));
      stimuli_clock_port.set_name(std::string(TOP_TB_OP_CLOCK_PORT_NAME));
      stimuli_clock_port.set_width(1);
    }
    /* Wire the port to the input stimuli:
     * The wiring will be inverted if the default value of the global port is 1
     * Otherwise, the wiring will not be inverted!
     */
    for (const size_t& pin : module_manager.module_port(top_module, module_global_port).pins()) {
      BasicPort global_port_to_connect(module_manager.module_port(top_module, module_global_port).get_name(), pin, pin);
      /* Should try to find a port defintion from simulation parameters
       * If found, it means that we need to use special clock name! 
       */
      for (const SimulationClockId& sim_clock : simulation_parameters.clocks()) { 
        if (global_port_to_connect == simulation_parameters.clock_port(sim_clock)) {
          stimuli_clock_port.set_name(generate_top_testbench_clock_name(std::string(TOP_TB_OP_CLOCK_PORT_PREFIX), simulation_parameters.clock_name(sim_clock)));
        }
      }

      print_verilog_wire_connection(fp, global_port_to_connect,
                                    stimuli_clock_port,
                                    1 == fabric_global_port_info.global_port_default_value(fabric_global_port));
    }
  }
}

/********************************************************************
 * Wire the global config done ports of FPGA fabric to local wires
 *******************************************************************/
static
void print_verilog_top_testbench_global_config_done_ports_stimuli(std::fstream& fp,
                                                                  const ModuleManager& module_manager,
                                                                  const ModuleId& top_module,
                                                                  const FabricGlobalPortInfo& fabric_global_port_info) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Connect global configuration done ports to configuration done signal */
  for (const FabricGlobalPortId& fabric_global_port : fabric_global_port_info.global_ports()) {
    /* Bypass clock signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_clock(fabric_global_port)) {
      continue;
    }
    if (false == fabric_global_port_info.global_port_is_config_enable(fabric_global_port)) {
      continue;
    }
    /* Reach here, it means we have a configuration done port to deal with */
    /* Find the module port */
    ModulePortId module_global_port = fabric_global_port_info.global_module_port(fabric_global_port);
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort stimuli_config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
    /* Wire the port to the input stimuli:
     * The wiring will be inverted if the default value of the global port is 1
     * Otherwise, the wiring will not be inverted!
     */
    print_verilog_wire_connection(fp, module_manager.module_port(top_module, module_global_port),
                                  stimuli_config_done_port,
                                  1 == fabric_global_port_info.global_port_default_value(fabric_global_port));
  }
}

/********************************************************************
 * Wire the global reset ports of FPGA fabric to local wires
 *******************************************************************/
static
void print_verilog_top_testbench_global_reset_ports_stimuli(std::fstream& fp,
                                                            const ModuleManager& module_manager,
                                                            const ModuleId& top_module,
                                                            const PinConstraints& pin_constraints,
                                                            const FabricGlobalPortInfo& fabric_global_port_info,
                                                            const bool& active_global_prog_reset) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Connect global reset ports to operating or programming reset signal */
  for (const FabricGlobalPortId& fabric_global_port : fabric_global_port_info.global_ports()) {
    /* Bypass clock signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_clock(fabric_global_port)) {
      continue;
    }
    /* Bypass config_done signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_config_enable(fabric_global_port)) {
      continue;
    }

    if (false == fabric_global_port_info.global_port_is_reset(fabric_global_port)) {
      continue;
    }
    /* Reach here, it means we have a reset port to deal with */
    /* Find the module port */
    ModulePortId module_global_port = fabric_global_port_info.global_module_port(fabric_global_port);
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    /* For global programming reset port, we will active only when specified */
    BasicPort stimuli_reset_port;
    bool activate = true;
    if (true == fabric_global_port_info.global_port_is_prog(fabric_global_port)) {
      stimuli_reset_port.set_name(std::string(TOP_TB_PROG_RESET_PORT_NAME));
      stimuli_reset_port.set_width(1);
      activate = active_global_prog_reset;
    } else {
      VTR_ASSERT_SAFE(false == fabric_global_port_info.global_port_is_prog(fabric_global_port));
      stimuli_reset_port.set_name(std::string(TOP_TB_RESET_PORT_NAME));
      stimuli_reset_port.set_width(1);
    }

    BasicPort module_global_port_info = module_manager.module_port(top_module, module_global_port);

    for (size_t pin_id = 0; pin_id < module_global_port_info.pins().size(); ++pin_id) {
      BasicPort module_global_pin(module_global_port_info.get_name(),
                                  module_global_port_info.pins()[pin_id],
                                  module_global_port_info.pins()[pin_id]);

      /* Regular reset port can be mapped by a net from user design */
      if (false == fabric_global_port_info.global_port_is_prog(fabric_global_port)) {
        /* If the global port name is in the pin constraints, we should wire it to the constrained pin */
        std::string constrained_net_name = pin_constraints.pin_net(module_global_pin);

        /* - If constrained to a given net in the benchmark, we connect the global pin to the net */
        if ( (false == pin_constraints.unconstrained_net(constrained_net_name))
          && (false == pin_constraints.unmapped_net(constrained_net_name))) {
          BasicPort benchmark_pin(constrained_net_name, 1);
          print_verilog_wire_connection(fp, module_global_pin,
                                        benchmark_pin,
                                        false);
          continue; /* Finish the net assignment for this reset pin */
        }
      }
      
      /* Wire the port to the input stimuli:
       * The wiring will be inverted if the default value of the global port is 1
       * Otherwise, the wiring will not be inverted!
       */
      if (true == activate) {
        print_verilog_wire_connection(fp, module_global_pin,
                                      stimuli_reset_port,
                                      1 == fabric_global_port_info.global_port_default_value(fabric_global_port));
      } else {
        VTR_ASSERT_SAFE(false == activate);
        print_verilog_wire_constant_values(fp, module_global_pin,
                                           std::vector<size_t>(1, fabric_global_port_info.global_port_default_value(fabric_global_port)));
      }
    }
  }
}

/********************************************************************
 * Wire the global set ports of FPGA fabric to local wires
 *******************************************************************/
static
void print_verilog_top_testbench_global_set_ports_stimuli(std::fstream& fp,
                                                          const ModuleManager& module_manager,
                                                          const ModuleId& top_module,
                                                          const FabricGlobalPortInfo& fabric_global_port_info,
                                                          const bool& active_global_prog_set) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Connect global set ports to operating or programming set signal */
  for (const FabricGlobalPortId& fabric_global_port : fabric_global_port_info.global_ports()) {
    /* Bypass clock signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_clock(fabric_global_port)) {
      continue;
    }
    /* Bypass config_done signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_config_enable(fabric_global_port)) {
      continue;
    }

    /* Bypass reset signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_reset(fabric_global_port)) {
      continue;
    }

    if (false == fabric_global_port_info.global_port_is_set(fabric_global_port)) {
      continue;
    }
    /* Reach here, it means we have a set port to deal with */
    /* Find the module port */
    ModulePortId module_global_port = fabric_global_port_info.global_module_port(fabric_global_port);
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    /* For global programming set port, we will active only when specified */
    BasicPort stimuli_set_port;
    bool activate = true;
    if (true == fabric_global_port_info.global_port_is_prog(fabric_global_port)) {
      stimuli_set_port.set_name(std::string(TOP_TB_PROG_SET_PORT_NAME));
      stimuli_set_port.set_width(1);
      activate = active_global_prog_set;
    } else {
      VTR_ASSERT_SAFE(false == fabric_global_port_info.global_port_is_prog(fabric_global_port));
      stimuli_set_port.set_name(std::string(TOP_TB_SET_PORT_NAME));
      stimuli_set_port.set_width(1);
    }
    /* Wire the port to the input stimuli:
     * The wiring will be inverted if the default value of the global port is 1
     * Otherwise, the wiring will not be inverted!
     */
    if (true == activate) {
      print_verilog_wire_connection(fp, module_manager.module_port(top_module, module_global_port),
                                    stimuli_set_port,
                                    1 == fabric_global_port_info.global_port_default_value(fabric_global_port));
    } else {
      VTR_ASSERT_SAFE(false == activate);
      print_verilog_wire_constant_values(fp, module_manager.module_port(top_module, module_global_port),
                                         std::vector<size_t>(1, fabric_global_port_info.global_port_default_value(fabric_global_port)));
    }
  }
}

/********************************************************************
 * Wire the regular global ports of FPGA fabric to local wires
 *******************************************************************/
static
void print_verilog_top_testbench_regular_global_ports_stimuli(std::fstream& fp,
                                                              const ModuleManager& module_manager,
                                                              const ModuleId& top_module,
                                                              const FabricGlobalPortInfo& fabric_global_port_info) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* For the rest of global ports, wire them to constant signals */
  for (const FabricGlobalPortId& fabric_global_port : fabric_global_port_info.global_ports()) {
    /* Bypass clock signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_clock(fabric_global_port)) {
      continue;
    }
    /* Bypass config_done signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_config_enable(fabric_global_port)) {
      continue;
    }

    /* Bypass reset signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_reset(fabric_global_port)) {
      continue;
    }

    /* Bypass set signals, they have been processed */
    if (true == fabric_global_port_info.global_port_is_set(fabric_global_port)) {
      continue;
    }

    /* Bypass io signals, they do not need any drivers */
    if (true == fabric_global_port_info.global_port_is_io(fabric_global_port)) {
      continue;
    }

    /* Find the port name, gpio port has special names */
    std::string port_name;
    VTR_ASSERT_SAFE(false == fabric_global_port_info.global_port_is_io(fabric_global_port));

    /* Reach here, it means we have a port to deal with */
    /* Find the module port and wire it to constant values */
    ModulePortId module_global_port = fabric_global_port_info.global_module_port(fabric_global_port);
    VTR_ASSERT(true == module_manager.valid_module_port_id(top_module, module_global_port));

    BasicPort module_port = module_manager.module_port(top_module, module_global_port);
    std::vector<size_t> default_values(module_port.get_width(), fabric_global_port_info.global_port_default_value(fabric_global_port));
    print_verilog_wire_constant_values(fp, module_port, default_values);
  }
}

/********************************************************************
 * Wire the global ports of FPGA fabric to local wires
 *******************************************************************/
static
void print_verilog_top_testbench_global_ports_stimuli(std::fstream& fp,
                                                      const ModuleManager& module_manager,
                                                      const ModuleId& top_module,
                                                      const PinConstraints& pin_constraints,
                                                      const FabricGlobalPortInfo& fabric_global_port_info,
                                                      const SimulationSetting& simulation_parameters,
                                                      const bool& active_global_prog_reset,
                                                      const bool& active_global_prog_set) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Begin connecting global ports of FPGA fabric to stimuli -----"));

  print_verilog_top_testbench_global_clock_ports_stimuli(fp,
                                                         module_manager,
                                                         top_module,
                                                         fabric_global_port_info,
                                                         simulation_parameters);

  print_verilog_top_testbench_global_config_done_ports_stimuli(fp,
                                                               module_manager,
                                                               top_module,
                                                               fabric_global_port_info);


  print_verilog_top_testbench_global_reset_ports_stimuli(fp,
                                                         module_manager,
                                                         top_module,
                                                         pin_constraints,
                                                         fabric_global_port_info,
                                                         active_global_prog_reset);

  print_verilog_top_testbench_global_set_ports_stimuli(fp,
                                                       module_manager,
                                                       top_module,
                                                       fabric_global_port_info,
                                                       active_global_prog_set);


  print_verilog_top_testbench_regular_global_ports_stimuli(fp,
                                                           module_manager,
                                                           top_module,
                                                           fabric_global_port_info);

  print_verilog_comment(fp, std::string("----- End connecting global ports of FPGA fabric to stimuli -----"));
}

/********************************************************************
 * This function prints the clock ports for all the benchmark clock nets
 * It will search the pin constraints to see if a clock is constrained to a specific pin
 * If constrained, 
 * - connect this clock to default values if it is set to be OPEN
 * - connect this clock to a specific clock source from simulation settings!!!
 * Otherwise,
 * - connect this clock to the default clock port
 *******************************************************************/
static 
void print_verilog_top_testbench_benchmark_clock_ports(std::fstream& fp,
                                                       const ModuleManager& module_manager,
                                                       const ModuleId& top_module,
                                                       const std::vector<std::string>& clock_port_names,
                                                       const PinConstraints& pin_constraints,
                                                       const SimulationSetting& simulation_parameters,
                                                       const BasicPort& default_clock_port) {
  /* Create a clock port if the benchmark have one but not in the default name!
   * We will wire the clock directly to the operating clock directly
   */
  for (const std::string clock_port_name : clock_port_names) {
    if (0 == clock_port_name.compare(default_clock_port.get_name())) {
      continue;
    }
    /* Ensure the clock port name is not a duplication of global ports of the FPGA module */
    bool print_clock_port = true;
    for (const BasicPort& module_port : module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GLOBAL_PORT)) {
      if (0 == clock_port_name.compare(module_port.get_name())) {
        print_clock_port = false;
      }
    }
    if (false == print_clock_port) {
      continue;
    }

    BasicPort clock_source_to_connect = default_clock_port;
   
    /* Check pin constraints to see if this clock is constrained to a specific pin
     * If constrained, 
     * - connect this clock to default values if it is set to be OPEN
     * - connect this clock to a specific clock source from simulation settings!!!
     */
    for (const PinConstraintId& pin_constraint : pin_constraints.pin_constraints()) {
      if (clock_port_name != pin_constraints.net(pin_constraint)) {
        continue;
      }
      /* Skip all the unrelated pin constraints */
      VTR_ASSERT(clock_port_name == pin_constraints.net(pin_constraint));
      /* Try to find which clock source is considered in simulation settings for this pin */
      for (const SimulationClockId& sim_clock_id : simulation_parameters.clocks()) {
        if (pin_constraints.pin(pin_constraint) == simulation_parameters.clock_port(sim_clock_id)) {
          std::string sim_clock_port_name = generate_top_testbench_clock_name(std::string(TOP_TB_OP_CLOCK_PORT_PREFIX), simulation_parameters.clock_name(sim_clock_id));
          clock_source_to_connect = BasicPort(sim_clock_port_name, 1);
        }
      }
    }

    /* Print the clock and wire it to the clock source */
    print_verilog_comment(fp, std::string("----- Create a clock for benchmark and wire it to " + clock_source_to_connect.get_name() + " -------"));
    BasicPort clock_port(clock_port_name, 1);
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, clock_port) << ";" << std::endl;
    print_verilog_wire_connection(fp, clock_port, clock_source_to_connect, false);
  }
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
                                       const AtomContext& atom_ctx,
                                       const VprNetlistAnnotation& netlist_annotation,
                                       const std::vector<std::string>& clock_port_names,
                                       const PinConstraints& pin_constraints,
                                       const SimulationSetting& simulation_parameters,
                                       const ConfigProtocol& config_protocol,
                                       const std::string& circuit_name){
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_default_net_type_declaration(fp,
                                             VERILOG_DEFAULT_NET_TYPE_NONE);

  /* Print module definition */
  fp << "module " << circuit_name << std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_MODULE_POSTFIX);
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

  for (const BasicPort& module_port : module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPIN_PORT)) {
    fp << generate_verilog_port(VERILOG_PORT_WIRE, module_port) << ";" << std::endl;
  }
  /* Add an empty line as a splitter */
  fp << std::endl;

  for (const BasicPort& module_port : module_manager.module_ports_by_type(top_module, ModuleManager::MODULE_GPOUT_PORT)) {
    fp << generate_verilog_port(VERILOG_PORT_WIRE, module_port) << ";" << std::endl;
  }
  /* Add an empty line as a splitter */
  fp << std::endl;

  /* Add local wires/registers that drive stimulus
   * We create these general purpose ports here,
   * and then wire them to the ports of FPGA fabric depending on their usage
   */
  /* Configuration done port */
  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, config_done_port) << ";" << std::endl;

  /* Programming clock */
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);
  fp << generate_verilog_port(VERILOG_PORT_WIRE, prog_clock_port) << ";" << std::endl;
  BasicPort prog_clock_register_port(std::string(std::string(TOP_TB_PROG_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, prog_clock_register_port) << ";" << std::endl;

  /* Multiple operating clocks based on the simulation settings */
  for (const SimulationClockId& sim_clock : simulation_parameters.clocks()) {
    std::string sim_clock_port_name = generate_top_testbench_clock_name(std::string(TOP_TB_OP_CLOCK_PORT_PREFIX), simulation_parameters.clock_name(sim_clock));
    BasicPort sim_clock_port(sim_clock_port_name, 1);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, sim_clock_port) << ";" << std::endl;
    BasicPort sim_clock_register_port(std::string(sim_clock_port_name + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);
    fp << generate_verilog_port(VERILOG_PORT_REG, sim_clock_register_port) << ";" << std::endl;
  }

  /* FIXME: Actually, for multi-clock implementations, input and output ports
   *        should be synchronized by different clocks. Currently, we lack the information 
   *        about what inputs are driven by which clock. Therefore, we use a unified clock 
   *        signal to do the job. However, this has to be fixed later!!! 
   * Create an operating clock_port to synchronize checkers stimulus generator 
   */
  BasicPort op_clock_port(std::string(TOP_TB_OP_CLOCK_PORT_NAME), 1);
  fp << generate_verilog_port(VERILOG_PORT_WIRE, op_clock_port) << ";" << std::endl;
  BasicPort op_clock_register_port(std::string(std::string(TOP_TB_OP_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, op_clock_register_port) << ";" << std::endl;

  /* Programming set and reset */
  BasicPort prog_reset_port(std::string(TOP_TB_PROG_RESET_PORT_NAME), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, prog_reset_port) << ";" << std::endl;
  BasicPort prog_set_port(std::string(TOP_TB_PROG_SET_PORT_NAME), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, prog_set_port) << ";" << std::endl;

  /* Global set and reset */
  BasicPort reset_port(std::string(TOP_TB_RESET_PORT_NAME), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, reset_port) << ";" << std::endl;
  BasicPort set_port(std::string(TOP_TB_SET_PORT_NAME), 1);
  fp << generate_verilog_port(VERILOG_PORT_REG, set_port) << ";" << std::endl;

  /* Configuration ports depend on the organization of SRAMs */
  print_verilog_top_testbench_config_protocol_port(fp, config_protocol,
                                                   module_manager, top_module);

  /* Print clock ports */
  print_verilog_top_testbench_benchmark_clock_ports(fp,
                                                    module_manager, top_module,
                                                    clock_port_names,
                                                    pin_constraints,
                                                    simulation_parameters,
                                                    op_clock_port);

  print_verilog_testbench_shared_ports(fp, atom_ctx, netlist_annotation,
                                       clock_port_names,
                                       std::string(TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX),
                                       std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX),
                                       std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                       std::string(AUTOCHECKED_SIMULATION_FLAG));

  /* Instantiate an integer to count the number of error and
   * determine if the simulation succeed or failed
   */
  print_verilog_comment(fp, std::string("----- Error counter: Deposit an error for config_done signal is not raised at the beginning -----"));
  fp << "\tinteger " << TOP_TESTBENCH_ERROR_COUNTER << "= 1;" << std::endl;
}

/********************************************************************
 * Estimate the number of configuration clock cycles
 * by traversing the linked-list and count the number of SRAM=1 or BL=1&WL=1 in it.
 * We plus 1 additional config clock cycle here because we need to reset everything during the first clock cycle
 * If we consider fast configuration, the number of clock cycles will be
 * the number of non-zero data points in the fabric bitstream
 * Note that this will not applicable to configuration chain!!!
 *******************************************************************/
static
size_t calculate_num_config_clock_cycles(const e_config_protocol_type& sram_orgz_type,
                                         const bool& fast_configuration,
                                         const bool& bit_value_to_skip,
                                         const BitstreamManager& bitstream_manager,
                                         const FabricBitstream& fabric_bitstream) {
  /* Find the longest regional bitstream */
  size_t regional_bitstream_max_size = find_fabric_regional_bitstream_max_size(fabric_bitstream);

  size_t num_config_clock_cycles = 1 + regional_bitstream_max_size;

  /* Branch on the type of configuration protocol */
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* We just need 1 clock cycle to load all the configuration bits
     * since all the ports are exposed at the top-level
     */
    num_config_clock_cycles = 2;
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    /* For fast configuration, the bitstream size counts from the first bit '1' */
    if (true == fast_configuration) {
      /* For fast configuration, the number of bits to be skipped
       * depends on each regional bitstream
       * For example:
       *   Region 0: 000000001111101010
       *   Region 1:     00000011010101
       *   Region 2:   0010101111000110
       * The number of bits that can be skipped is limited by Region 2
       */
      size_t num_bits_to_skip = find_configuration_chain_fabric_bitstream_size_to_be_skipped(fabric_bitstream, bitstream_manager, bit_value_to_skip);

      num_config_clock_cycles = 1 + regional_bitstream_max_size - num_bits_to_skip;

      VTR_LOG("Fast configuration reduces number of configuration clock cycles from %lu to %lu (compression_rate = %f%)\n",
              1 + regional_bitstream_max_size,
              num_config_clock_cycles,
              100. * ((float)num_config_clock_cycles / (float)(1 + regional_bitstream_max_size) - 1.));
    }
    break;
  case CONFIG_MEM_MEMORY_BANK: {
    /* For fast configuration, we will skip all the zero data points */
    num_config_clock_cycles = 1 + build_memory_bank_fabric_bitstream_by_address(fabric_bitstream).size();
    if (true == fast_configuration) {
      size_t full_num_config_clock_cycles = num_config_clock_cycles;
      num_config_clock_cycles = 1 + find_memory_bank_fast_configuration_fabric_bitstream_size(fabric_bitstream, bit_value_to_skip);
      VTR_LOG("Fast configuration reduces number of configuration clock cycles from %lu to %lu (compression_rate = %f%)\n",
              full_num_config_clock_cycles,
              num_config_clock_cycles,
              100. * ((float)num_config_clock_cycles / (float)full_num_config_clock_cycles - 1.));
    }
    break;
  }
  case CONFIG_MEM_FRAME_BASED: {
    num_config_clock_cycles = 1 + build_frame_based_fabric_bitstream_by_address(fabric_bitstream).size();
    if (true == fast_configuration) {
      size_t full_num_config_clock_cycles = num_config_clock_cycles;
      num_config_clock_cycles = 1 + find_frame_based_fast_configuration_fabric_bitstream_size(fabric_bitstream, bit_value_to_skip);
      VTR_LOG("Fast configuration reduces number of configuration clock cycles from %lu to %lu (compression_rate = %f%)\n",
              full_num_config_clock_cycles,
              num_config_clock_cycles,
              100. * ((float)num_config_clock_cycles / (float)full_num_config_clock_cycles - 1.));
    }
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid SRAM organization type!\n");
    exit(1);
  }

  VTR_LOG("Will use %ld configuration clock cycles to top testbench\n",
          num_config_clock_cycles);

  return num_config_clock_cycles;
}

/********************************************************************
 * Instanciate the input benchmark module
 *******************************************************************/
static
void print_verilog_top_testbench_benchmark_instance(std::fstream& fp,
                                                    const std::string& reference_verilog_top_name,
                                                    const AtomContext& atom_ctx,
                                                    const VprNetlistAnnotation& netlist_annotation,
                                                    const bool& explicit_port_mapping) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Benchmark is instanciated conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, std::string(AUTOCHECKED_SIMULATION_FLAG));

  print_verilog_comment(fp, std::string("----- Reference Benchmark Instanication -------"));

  /* Do NOT use explicit port mapping here:
   * VPR added a prefix of "out_" to the output ports of input benchmark
   */
  std::vector<std::string> prefix_to_remove;
  prefix_to_remove.push_back(std::string(VPR_BENCHMARK_OUT_PORT_PREFIX));
  prefix_to_remove.push_back(std::string(OPENFPGA_BENCHMARK_OUT_PORT_PREFIX));
  print_verilog_testbench_benchmark_instance(fp, reference_verilog_top_name,
                                             std::string(TOP_TESTBENCH_REFERENCE_INSTANCE_NAME),
                                             std::string(),
                                             std::string(),
                                             prefix_to_remove,
                                             std::string(TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX),
                                             atom_ctx, netlist_annotation,
                                             explicit_port_mapping);

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
void print_verilog_top_testbench_load_bitstream_task_configuration_chain(std::fstream& fp,
                                                                         const ModuleManager& module_manager,
                                                                         const ModuleId& top_module) {

  /* Validate the file stream */
  valid_file_stream(fp);

  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);
  ModulePortId cc_head_port_id = module_manager.find_module_port(top_module, generate_configuration_chain_head_name());
  BasicPort cc_head_port = module_manager.module_port(top_module, cc_head_port_id);
  BasicPort cc_head_value(generate_configuration_chain_head_name() + std::string("_val"), cc_head_port.get_width());

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Feed the scan-chain input at each falling edge of programming clock
   * It aims at avoid racing the programming clock (scan-chain data changes at the rising edge).
   */
  print_verilog_comment(fp, std::string("----- Task: input values during a programming clock cycle -----"));
  fp << "task " << std::string(TOP_TESTBENCH_PROG_TASK_NAME) << ";" << std::endl;
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
 * Print tasks (processes) in Verilog format,
 * which is very useful in generating stimuli for each clock cycle
 * This function is tuned for memory bank manipulation:
 * During each programming cycle, we feed
 * - an address to the BL address port of top module
 * - an address to the WL address port of top module
 * - a data input to the din port of top module
 *******************************************************************/
static
void print_verilog_top_testbench_load_bitstream_task_memory_bank(std::fstream& fp,
                                                                 const ModuleManager& module_manager,
                                                                 const ModuleId& top_module) {

  /* Validate the file stream */
  valid_file_stream(fp);

  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);

  ModulePortId bl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port = module_manager.module_port(top_module, bl_addr_port_id);
  BasicPort bl_addr_value = bl_addr_port;
  bl_addr_value.set_name(std::string(MEMORY_BL_PORT_NAME) + std::string("_val"));

  ModulePortId wl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port = module_manager.module_port(top_module, wl_addr_port_id);
  BasicPort wl_addr_value = wl_addr_port;
  wl_addr_value.set_name(std::string(MEMORY_WL_PORT_NAME) + std::string("_val"));

  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  BasicPort din_value = din_port;
  din_value.set_name(std::string(DECODER_DATA_IN_PORT_NAME) + std::string("_val"));

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Feed the address and data input at each falling edge of programming clock
   * As the enable signal is wired to the programming clock, we should synchronize
   * address and data with the enable signal
   */
  print_verilog_comment(fp, std::string("----- Task: assign BL and WL address, and data values at rising edge of enable signal -----"));
  fp << "task " << std::string(TOP_TESTBENCH_PROG_TASK_NAME) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_INPUT, bl_addr_value) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_INPUT, wl_addr_value) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_INPUT, din_value) << ";" << std::endl;
  fp << "\tbegin" << std::endl;
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;

  fp << "\t\t\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, bl_addr_port);
  fp << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, bl_addr_value);
  fp << ";" << std::endl;
  fp << std::endl;

  fp << "\t\t\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, wl_addr_port);
  fp << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, wl_addr_value);
  fp << ";" << std::endl;
  fp << std::endl;

  fp << "\t\t\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, din_port);
  fp << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, din_value);
  fp << ";" << std::endl;
  fp << std::endl;

  fp << "\tend" << std::endl;
  fp << "endtask" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}


/********************************************************************
 * Print tasks (processes) in Verilog format,
 * which is very useful in generating stimuli for each clock cycle
 * This function is tuned for frame-based memory manipulation:
 * During each programming cycle, we feed
 * - an address to the address port of top module
 * - a data input to the din port of top module
 *******************************************************************/
static
void print_verilog_top_testbench_load_bitstream_task_frame_decoder(std::fstream& fp,
                                                                   const ModuleManager& module_manager,
                                                                   const ModuleId& top_module) {

  /* Validate the file stream */
  valid_file_stream(fp);

  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);

  ModulePortId addr_port_id = module_manager.find_module_port(top_module,
                                                              std::string(DECODER_ADDRESS_PORT_NAME));
  BasicPort addr_port = module_manager.module_port(top_module, addr_port_id);
  BasicPort addr_value = addr_port;
  addr_value.set_name(std::string(DECODER_ADDRESS_PORT_NAME) + std::string("_val"));

  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  BasicPort din_value = din_port;
  din_value.set_name(std::string(DECODER_DATA_IN_PORT_NAME) + std::string("_val"));

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Feed the address and data input at each falling edge of programming clock
   * As the enable signal is wired to the programming clock, we should synchronize
   * address and data with the enable signal
   */
  print_verilog_comment(fp, std::string("----- Task: assign address and data values at rising edge of enable signal -----"));
  fp << "task " << std::string(TOP_TESTBENCH_PROG_TASK_NAME) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_INPUT, addr_value) << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_INPUT, din_value) << ";" << std::endl;
  fp << "\tbegin" << std::endl;
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;

  fp << "\t\t\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, addr_port);
  fp << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, addr_value);
  fp << ";" << std::endl;
  fp << std::endl;

  fp << "\t\t\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, din_port);
  fp << " = ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, din_value);
  fp << ";" << std::endl;
  fp << std::endl;

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
                                                     const e_config_protocol_type& sram_orgz_type,
                                                     const ModuleManager& module_manager,
                                                     const ModuleId& top_module) {
  switch (sram_orgz_type) {
  case CONFIG_MEM_STANDALONE:
    /* No need to have a specific task. Loading is done in 1 clock cycle */
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    print_verilog_top_testbench_load_bitstream_task_configuration_chain(fp,
                                                                        module_manager,
                                                                        top_module);
    break;
  case CONFIG_MEM_MEMORY_BANK:
    print_verilog_top_testbench_load_bitstream_task_memory_bank(fp,
                                                                module_manager,
                                                                top_module);
    break;
  case CONFIG_MEM_FRAME_BASED:
    print_verilog_top_testbench_load_bitstream_task_frame_decoder(fp,
                                                                  module_manager,
                                                                  top_module);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid type of SRAM organization!\n");
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
                                                  const SimulationSetting& simulation_parameters,
                                                  const size_t& num_config_clock_cycles,
                                                  const float& prog_clock_period,
                                                  const float& op_clock_period,
                                                  const float& timescale) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Number of clock cycles in configuration phase: " + std::to_string(num_config_clock_cycles) + " -----"));

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);

  BasicPort op_clock_port(std::string(TOP_TB_OP_CLOCK_PORT_NAME), 1);
  BasicPort op_clock_register_port(std::string(std::string(TOP_TB_OP_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);

  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);
  BasicPort prog_clock_register_port(std::string(std::string(TOP_TB_PROG_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);

  BasicPort prog_reset_port(std::string(TOP_TB_PROG_RESET_PORT_NAME), 1);
  BasicPort prog_set_port(std::string(TOP_TB_PROG_SET_PORT_NAME), 1);

  BasicPort reset_port(std::string(TOP_TB_RESET_PORT_NAME), 1);
  BasicPort set_port(std::string(TOP_TB_SET_PORT_NAME), 1);

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

  /* Generate stimuli waveform for multiple user-defined operating clock signals */
  for (const SimulationClockId& sim_clock : simulation_parameters.clocks()) {
    print_verilog_comment(fp, "----- Begin raw operating clock signal '" + simulation_parameters.clock_name(sim_clock) + "' generation -----");
    std::string sim_clock_port_name = generate_top_testbench_clock_name(std::string(TOP_TB_OP_CLOCK_PORT_PREFIX), simulation_parameters.clock_name(sim_clock));
    BasicPort sim_clock_port(sim_clock_port_name, 1);
    BasicPort sim_clock_register_port(std::string(sim_clock_port_name + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);

    float sim_clock_period = 1. / simulation_parameters.clock_frequency(sim_clock); 
    print_verilog_clock_stimuli(fp, sim_clock_register_port,
                                0, /* Initial value */
                                0.5 * sim_clock_period / timescale,
                                std::string("~" + reset_port.get_name()));
    print_verilog_comment(fp, "----- End raw operating clock signal generation -----");

    /* Operation clock should be enabled after programming phase finishes.
     * Before configuration is done (config_done is enabled), operation clock should be always zero.
     */
    print_verilog_comment(fp, std::string("----- Actual operating clock is triggered only when " + config_done_port.get_name() + " is enabled -----"));
    fp << "\tassign " << generate_verilog_port(VERILOG_PORT_CONKT, sim_clock_port);
    fp << " = " << generate_verilog_port(VERILOG_PORT_CONKT, sim_clock_register_port);
    fp << " & " << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port);
    fp << ";" << std::endl;

    fp << std::endl;
  }

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
  fp << " & " << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port);
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
  print_verilog_comment(fp, "----- Begin programming set signal generation -----");
  print_verilog_pulse_stimuli(fp, prog_set_port,
                              1, /* Initial value */
                              prog_clock_period / timescale, 0);
  print_verilog_comment(fp, "----- End programming set signal generation -----");

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
 * Print input stimuli for configuration protocol
 * include:
 * - memory bank 
 *   1. the enable signal 
 * - frame-based 
 *   1. the enable signal 
 *******************************************************************/
static
void print_verilog_top_testbench_configuration_protocol_stimulus(std::fstream& fp,
                                                                 const e_config_protocol_type& config_protocol_type, 
                                                                 const ModuleManager& module_manager,
                                                                 const ModuleId& top_module,
                                                                 const float& prog_clock_period,
                                                                 const float& timescale) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Branch on the type of configuration protocol */
  switch (config_protocol_type) {
  case CONFIG_MEM_STANDALONE:
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    break;
  case CONFIG_MEM_MEMORY_BANK:
  case CONFIG_MEM_FRAME_BASED: {
    ModulePortId en_port_id = module_manager.find_module_port(top_module,
                                                              std::string(DECODER_ENABLE_PORT_NAME));
    BasicPort en_port = module_manager.module_port(top_module, en_port_id);
    BasicPort en_register_port(std::string(en_port.get_name() + std::string(TOP_TB_CLOCK_REG_POSTFIX)), 1);
    print_verilog_comment(fp, std::string("---- Generate enable signal waveform  -----"));
    print_verilog_shifted_clock_stimuli(fp, en_register_port,
                                        0.25 * prog_clock_period / timescale,
                                        0.5 * prog_clock_period / timescale, 0);
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid SRAM organization type!\n");
    exit(1);
  }
}

/********************************************************************
 * Print stimulus for a FPGA fabric with a flatten memory (standalone) configuration protocol
 * We will load the bitstream in the second clock cycle, right after the first reset cycle
 *******************************************************************/
static
void print_verilog_top_testbench_vanilla_bitstream(std::fstream& fp,
                                                   const ModuleManager& module_manager,
                                                   const ModuleId& top_module,
                                                   const BitstreamManager& bitstream_manager,
                                                   const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Find Bit-Line and Word-Line port */
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);

  /* Find Bit-Line and Word-Line port */
  ModulePortId bl_port_id = module_manager.find_module_port(top_module, std::string(MEMORY_BL_PORT_NAME));
  BasicPort bl_port = module_manager.module_port(top_module, bl_port_id);

  ModulePortId wl_port_id = module_manager.find_module_port(top_module, std::string(MEMORY_WL_PORT_NAME));
  BasicPort wl_port = module_manager.module_port(top_module, wl_port_id);

  /* Initial value should be the first configuration bits
   * In the rest of programming cycles,
   * configuration bits are fed at the falling edge of programming clock.
   * We do not care the value of scan_chain head during the first programming cycle
   * It is reset anyway
   */
  std::vector<size_t> initial_bl_values(bl_port.get_width(), 0);
  std::vector<size_t> initial_wl_values(wl_port.get_width(), 0);

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;
  print_verilog_comment(fp, "----- Configuration chain default input -----");
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(bl_port, initial_bl_values);
  fp << ";" << std::endl;
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(wl_port, initial_wl_values);
  fp << ";" << std::endl;

  fp << std::endl;

  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ") begin" << std::endl;

  /* Enable all the WLs */
  std::vector<size_t> enabled_wl_values(wl_port.get_width(), 1);
  fp << "\t\t\t";
  fp << generate_verilog_port_constant_values(wl_port, enabled_wl_values);
  fp << ";" << std::endl;

  size_t ibit = 0;
  for (const FabricBitId& bit_id : fabric_bitstream.bits()) {
    BasicPort cur_bl_port(bl_port);
    cur_bl_port.set_width(ibit, ibit);

    fp << "\t\t\t";
    fp << generate_verilog_port(VERILOG_PORT_CONKT, cur_bl_port);
    fp << " = ";
    fp << "1'b" << (size_t)bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id));
    fp << ";" << std::endl;

    ibit++;
  }

  fp << "\t\tend" << std::endl;

  /* Disable all the WLs */
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;

  fp << "\t\t\t";
  fp << generate_verilog_port_constant_values(wl_port, initial_wl_values);
  fp << ";" << std::endl;

  /* Raise the flag of configuration done when bitstream loading is complete */
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
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
 * Decide if we should use reset or set signal to acheive fast configuration
 * - If only one type signal is specified, we use that type
 *   For example, only reset signal is defined, we will use reset  
 * - If both are defined, pick the one that will bring bigger reduction
 *   i.e., larger number of configuration bits can be skipped
 *******************************************************************/
static 
bool find_bit_value_to_skip_for_fast_configuration(const e_config_protocol_type& config_protocol_type,  
                                                   const bool& fast_configuration,
                                                   const std::vector<FabricGlobalPortId>& global_prog_reset_ports,
                                                   const std::vector<FabricGlobalPortId>& global_prog_set_ports,
                                                   const BitstreamManager& bitstream_manager,
                                                   const FabricBitstream& fabric_bitstream) {

  /* Early exit conditions */
  if (!global_prog_reset_ports.empty() && global_prog_set_ports.empty()) {
    return false; 
  } else if (!global_prog_set_ports.empty() && global_prog_reset_ports.empty()) {
    return true; 
  } else if (global_prog_set_ports.empty() && global_prog_reset_ports.empty()) {
    /* If both types of ports are not defined, the fast configuration should be turned off */
    VTR_ASSERT(false == fast_configuration); 
    return false;
  }

  VTR_ASSERT(!global_prog_set_ports.empty() && !global_prog_reset_ports.empty());
  bool bit_value_to_skip = false;

  VTR_LOG("Both reset and set ports are defined for programming controls, selecting the best-fit one...\n");

  size_t num_ones_to_skip = 0;
  size_t num_zeros_to_skip = 0;

  /* Branch on the type of configuration protocol */
  switch (config_protocol_type) {
  case CONFIG_MEM_STANDALONE:
    break;
  case CONFIG_MEM_SCAN_CHAIN: {
    /* We can only skip the ones/zeros at the beginning of the bitstream */
    /* Count how many logic '1' bits we can skip */
    for (const FabricBitId& bit_id : fabric_bitstream.bits()) {
      if (false == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id))) {
        break;
      }
      VTR_ASSERT(true == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id)));
      num_ones_to_skip++;
    }
    /* Count how many logic '0' bits we can skip */
    for (const FabricBitId& bit_id : fabric_bitstream.bits()) {
      if (true == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id))) {
        break;
      }
      VTR_ASSERT(false == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id)));
      num_zeros_to_skip++;
    }
    break;
  }
  case CONFIG_MEM_MEMORY_BANK:
  case CONFIG_MEM_FRAME_BASED: {
    /* Count how many logic '1' and logic '0' bits we can skip */
    for (const FabricBitId& bit_id : fabric_bitstream.bits()) {
      if (false == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id))) {
        num_zeros_to_skip++;
      } else {
        VTR_ASSERT(true == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id)));
        num_ones_to_skip++;
      }
    }
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid SRAM organization type!\n");
    exit(1);
  }

  VTR_LOG("Using reset will skip %g% (%lu/%lu) of configuration bitstream.\n",
          100. * (float) num_zeros_to_skip / (float) fabric_bitstream.num_bits(),
          num_zeros_to_skip, fabric_bitstream.num_bits());

  VTR_LOG("Using set will skip %g% (%lu/%lu) of configuration bitstream.\n",
          100. * (float) num_ones_to_skip / (float) fabric_bitstream.num_bits(),
          num_ones_to_skip, fabric_bitstream.num_bits());

  /* By default, we prefer to skip zeros (when the numbers are the same */
  if (num_ones_to_skip > num_zeros_to_skip) {
    VTR_LOG("Will use set signal in fast configuration\n");
    bit_value_to_skip = true;
  } else {
    VTR_LOG("Will use reset signal in fast configuration\n");
  }

  return bit_value_to_skip;
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
                                                               const bool& fast_configuration,
                                                               const bool& bit_value_to_skip,
                                                               const ModuleManager& module_manager,
                                                               const ModuleId& top_module,
                                                               const BitstreamManager& bitstream_manager,
                                                               const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Initial value should be the first configuration bits
   * In the rest of programming cycles,
   * configuration bits are fed at the falling edge of programming clock.
   * We do not care the value of scan_chain head during the first programming cycle
   * It is reset anyway
   */
  ModulePortId cc_head_port_id = module_manager.find_module_port(top_module, generate_configuration_chain_head_name());
  BasicPort config_chain_head_port = module_manager.module_port(top_module, cc_head_port_id);
  std::vector<size_t> initial_values(config_chain_head_port.get_width(), 0);

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;
  print_verilog_comment(fp, "----- Configuration chain default input -----");
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(config_chain_head_port, initial_values);
  fp << ";";

  fp << std::endl;

  /* Find the longest bitstream */
  size_t regional_bitstream_max_size = find_fabric_regional_bitstream_max_size(fabric_bitstream);

  /* For fast configuration, the bitstream size counts from the first bit '1' */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip = find_configuration_chain_fabric_bitstream_size_to_be_skipped(fabric_bitstream, bitstream_manager, bit_value_to_skip);
  }
  VTR_ASSERT(num_bits_to_skip < regional_bitstream_max_size);

  /* Reorganize the regional bitstreams to be the same size */
  ConfigChainFabricBitstream regional_bitstreams = build_config_chain_fabric_bitstream_by_region(bitstream_manager, fabric_bitstream);

  /* Attention: when the fast configuration is enabled, we will start from the first bit '1'
   * This requires a reset signal (as we forced in the first clock cycle)
   *
   * Note that bitstream may come from different regions
   * The bitstream value to be loaded should be organized as follows
   *
   *        cycleA
   *              |
   *   Region 0: 0|00000001111101010
   *   Region 1:  |   00000011010101
   *   Region 2:  | 0010101111000110
   *
   *   Zero bits will be added to the head of those bitstreams are shorter 
   *   than the longest bitstream
   */
  for (size_t ibit = num_bits_to_skip; ibit < regional_bitstream_max_size; ++ibit) { 
    std::vector<size_t> curr_cc_head_val;
    curr_cc_head_val.reserve(fabric_bitstream.regions().size());
    for (const auto& region_bitstream : regional_bitstreams) {
      curr_cc_head_val.push_back((size_t)region_bitstream[ibit]);
    }

    fp << "\t\t" << std::string(TOP_TESTBENCH_PROG_TASK_NAME);
    fp << "(" << generate_verilog_constant_values(curr_cc_head_val) << ");" << std::endl;
  }

  /* Raise the flag of configuration done when bitstream loading is complete */
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
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
 * Print stimulus for a FPGA fabric with a memory bank configuration protocol
 * where configuration bits are programming in serial (one by one)
 *
 * We will use the programming task function created before
 *******************************************************************/
static
void print_verilog_top_testbench_memory_bank_bitstream(std::fstream& fp,
                                                       const bool& fast_configuration,
                                                       const bool& bit_value_to_skip,
                                                       const ModuleManager& module_manager,
                                                       const ModuleId& top_module,
                                                       const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Feed addresss and data input pair one by one
   * Note: the first cycle is reserved for programming reset
   * We should give dummy values
   */
  ModulePortId bl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port = module_manager.module_port(top_module, bl_addr_port_id);
  std::vector<size_t> initial_bl_addr_values(bl_addr_port.get_width(), 0);

  ModulePortId wl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port = module_manager.module_port(top_module, wl_addr_port_id);
  std::vector<size_t> initial_wl_addr_values(wl_addr_port.get_width(), 0);

  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  std::vector<size_t> initial_din_values(din_port.get_width(), 0);

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;
  print_verilog_comment(fp, "----- Address port default input -----");
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(bl_addr_port, initial_bl_addr_values);
  fp << ";";
  fp << std::endl;

  fp << "\t\t";
  fp << generate_verilog_port_constant_values(wl_addr_port, initial_wl_addr_values);
  fp << ";";
  fp << std::endl;

  print_verilog_comment(fp, "----- Data-input port default input -----");
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(din_port, initial_din_values);
  fp << ";";

  fp << std::endl;

  /* Reorganize the fabric bitstream by the same address across regions */
  MemoryBankFabricBitstream fabric_bits_by_addr = build_memory_bank_fabric_bitstream_by_address(fabric_bitstream);

  for (const auto& addr_din_pair : fabric_bits_by_addr) {
    /* When fast configuration is enabled,
     * the rule to skip any configuration bit should consider the whole data input values.
     * Only all the bits in the din port match the value to be skipped,
     * the programming cycle can be skipped!
     */
    if (true == fast_configuration) {
      bool skip_curr_bits = true;
      for (const bool& bit : addr_din_pair.second) {
        if (bit_value_to_skip != bit) {
          skip_curr_bits = false;
          break;
        }
      }

      if (true == skip_curr_bits) {
        continue;
      }
    }

    fp << "\t\t" << std::string(TOP_TESTBENCH_PROG_TASK_NAME);
    fp << "(" << bl_addr_port.get_width() << "'b";
    VTR_ASSERT(bl_addr_port.get_width() == addr_din_pair.first.first.length());
    fp << addr_din_pair.first.first;

    fp << ", ";
    fp << wl_addr_port.get_width() << "'b";
    VTR_ASSERT(wl_addr_port.get_width() == addr_din_pair.first.second.length());
    fp << addr_din_pair.first.second;

    fp << ", ";
    fp << din_port.get_width() << "'b";
    VTR_ASSERT(din_port.get_width() == addr_din_pair.second.size());
    for (const bool& din_value : addr_din_pair.second) {
      if (true == din_value) {
        fp << "1";
      } else {
        VTR_ASSERT(false == din_value);
        fp << "0";
      }
    }
    fp << ");" << std::endl;
  }

  /* Raise the flag of configuration done when bitstream loading is complete */
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
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
 * Print stimulus for a FPGA fabric with a frame-based configuration protocol
 * where configuration bits are programming in serial (one by one)
 *
 * We will use the programming task function created before
 *******************************************************************/
static
void print_verilog_top_testbench_frame_decoder_bitstream(std::fstream& fp,
                                                         const bool& fast_configuration,
                                                         const bool& bit_value_to_skip,
                                                         const ModuleManager& module_manager,
                                                         const ModuleId& top_module,
                                                         const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Feed addresss and data input pair one by one
   * Note: the first cycle is reserved for programming reset
   * We should give dummy values
   */
  ModulePortId addr_port_id = module_manager.find_module_port(top_module,
                                                              std::string(DECODER_ADDRESS_PORT_NAME));
  BasicPort addr_port = module_manager.module_port(top_module, addr_port_id);
  std::vector<size_t> initial_addr_values(addr_port.get_width(), 0);

  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  std::vector<size_t> initial_din_values(din_port.get_width(), 0);

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  fp << "initial" << std::endl;
  fp << "\tbegin" << std::endl;
  print_verilog_comment(fp, "----- Address port default input -----");
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(addr_port, initial_addr_values);
  fp << ";";
  fp << std::endl;

  print_verilog_comment(fp, "----- Data-input port default input -----");
  fp << "\t\t";
  fp << generate_verilog_port_constant_values(din_port, initial_din_values);
  fp << ";";

  fp << std::endl;

  /* Reorganize the fabric bitstream by the same address across regions */
  FrameFabricBitstream fabric_bits_by_addr = build_frame_based_fabric_bitstream_by_address(fabric_bitstream);

  for (const auto& addr_din_pair : fabric_bits_by_addr) {
    /* When fast configuration is enabled,
     * the rule to skip any configuration bit should consider the whole data input values.
     * Only all the bits in the din port match the value to be skipped,
     * the programming cycle can be skipped!
     */
    if (true == fast_configuration) {
      bool skip_curr_bits = true;
      for (const bool& bit : addr_din_pair.second) {
        if (bit_value_to_skip != bit) {
          skip_curr_bits = false;
          break;
        }
      }

      if (true == skip_curr_bits) {
        continue;
      }
    }

    fp << "\t\t" << std::string(TOP_TESTBENCH_PROG_TASK_NAME);
    fp << "(" << addr_port.get_width() << "'b";
    VTR_ASSERT(addr_port.get_width() == addr_din_pair.first.size());
    fp << addr_din_pair.first;
    fp << ", ";
    fp << din_port.get_width() << "'b";
    VTR_ASSERT(din_port.get_width() == addr_din_pair.second.size());
    for (const bool& din_value : addr_din_pair.second) {
      if (true == din_value) {
        fp << "1";
      } else {
        VTR_ASSERT(false == din_value);
        fp << "0";
      }
    }
    fp << ");" << std::endl;
  }

  /* Disable the address and din 
  fp << "\t\t" << std::string(TOP_TESTBENCH_PROG_TASK_NAME);
  fp << "(" << addr_port.get_width() << "'b";
  std::vector<size_t> all_zero_addr(addr_port.get_width(), 0);
  for (const size_t& addr_bit : all_zero_addr) {
    fp << addr_bit;
  }
  fp << ", ";
  fp << generate_verilog_constant_values(initial_din_values);
  fp << ");" << std::endl;
  */

  /* Raise the flag of configuration done when bitstream loading is complete */
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME), 1);
  fp << "\t\t@(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ");" << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
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
                                           const e_config_protocol_type& config_protocol_type,
                                           const bool& fast_configuration,
                                           const bool& bit_value_to_skip,
                                           const ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const BitstreamManager& bitstream_manager,
                                           const FabricBitstream& fabric_bitstream) {

  /* Branch on the type of configuration protocol */
  switch (config_protocol_type) {
  case CONFIG_MEM_STANDALONE:
    print_verilog_top_testbench_vanilla_bitstream(fp,
                                                  module_manager, top_module,
                                                  bitstream_manager, fabric_bitstream);
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    print_verilog_top_testbench_configuration_chain_bitstream(fp, fast_configuration, 
                                                              bit_value_to_skip,
                                                              module_manager, top_module,
                                                              bitstream_manager, fabric_bitstream);
    break;
  case CONFIG_MEM_MEMORY_BANK:
    print_verilog_top_testbench_memory_bank_bitstream(fp, fast_configuration,
                                                      bit_value_to_skip,
                                                      module_manager, top_module,
                                                      fabric_bitstream);
    break;
  case CONFIG_MEM_FRAME_BASED:
    print_verilog_top_testbench_frame_decoder_bitstream(fp, fast_configuration,
                                                        bit_value_to_skip,
                                                        module_manager, top_module,
                                                        fabric_bitstream);
    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid SRAM organization type!\n");
    exit(1);
  }
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
void print_verilog_full_testbench_configuration_chain_bitstream(std::fstream& fp,
                                                                const std::string& bitstream_file,
                                                                const bool& fast_configuration,
                                                                const bool& bit_value_to_skip,
                                                                const ModuleManager& module_manager,
                                                                const ModuleId& top_module,
                                                                const BitstreamManager& bitstream_manager,
                                                                const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");

  /* Find the longest bitstream */
  size_t regional_bitstream_max_size = find_fabric_regional_bitstream_max_size(fabric_bitstream);

  /* For fast configuration, the bitstream size counts from the first bit '1' */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip = find_configuration_chain_fabric_bitstream_size_to_be_skipped(fabric_bitstream, bitstream_manager, bit_value_to_skip);
  }
  VTR_ASSERT(num_bits_to_skip < regional_bitstream_max_size);

  /* Define a constant for the bitstream length */
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_LENGTH_VARIABLE), regional_bitstream_max_size - num_bits_to_skip); 
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_WIDTH_VARIABLE), fabric_bitstream.num_regions()); 

  /* Initial value should be the first configuration bits
   * In the rest of programming cycles,
   * configuration bits are fed at the falling edge of programming clock.
   * We do not care the value of scan_chain head during the first programming cycle
   * It is reset anyway
   */
  ModulePortId cc_head_port_id = module_manager.find_module_port(top_module, generate_configuration_chain_head_name());
  BasicPort config_chain_head_port = module_manager.module_port(top_module, cc_head_port_id);
  std::vector<size_t> initial_values(config_chain_head_port.get_width(), 0);

  /* Declare local variables for bitstream loading in Verilog */
  print_verilog_comment(fp, "----- Virtual memory to store the bitstream from external file -----");
  fp << "reg [0:`" << TOP_TB_BITSTREAM_WIDTH_VARIABLE << " - 1] ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[0:`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << " - 1];";
  fp << std::endl;

  fp << "reg [$clog2(`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << ") - 1:0] " << TOP_TB_BITSTREAM_INDEX_REG_NAME << ";" << std::endl;

  BasicPort bit_skip_reg(TOP_TB_BITSTREAM_SKIP_FLAG_REG_NAME, 1);
  print_verilog_comment(fp, "----- Registers used for fast configuration logic -----");
  fp << "reg [$clog2(`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << ") - 1:0] " << TOP_TB_BITSTREAM_ITERATOR_REG_NAME << ";" << std::endl;
  fp << generate_verilog_port(VERILOG_PORT_REG, bit_skip_reg) << ";" << std::endl;

  print_verilog_comment(fp, "----- Preload bitstream file to a virtual memory -----");
  fp << "initial begin" << std::endl;
  fp << "\t";
  fp << "$readmemb(\"" << bitstream_file << "\", " << TOP_TB_BITSTREAM_MEM_REG_NAME << ");";
  fp << std::endl;

  print_verilog_comment(fp, "----- Configuration chain default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(config_chain_head_port, initial_values, true);
  fp << ";";
  fp << std::endl;

  fp << "\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " <= 0";
  fp << ";";
  fp << std::endl;

  std::vector<size_t> bit_skip_values(bit_skip_reg.get_width(), fast_configuration ? 1 : 0);
  fp << "\t";
  fp << generate_verilog_port_constant_values(bit_skip_reg, bit_skip_values, true);
  fp << ";";
  fp << std::endl;

  fp << "\t";
  fp << "for (" << TOP_TB_BITSTREAM_ITERATOR_REG_NAME << " = 0; ";
  fp << TOP_TB_BITSTREAM_ITERATOR_REG_NAME << " < `" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << " + 1; "; 
  fp << TOP_TB_BITSTREAM_ITERATOR_REG_NAME << " = " << TOP_TB_BITSTREAM_ITERATOR_REG_NAME << " + 1)";
  fp << " begin"; 
  fp << std::endl;

  fp << "\t\t";
  fp << "if (";
  fp << generate_verilog_constant_values(std::vector<size_t>(fabric_bitstream.num_regions(), bit_value_to_skip));
  fp << " == ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[" << TOP_TB_BITSTREAM_ITERATOR_REG_NAME << "]";
  fp << ")";
  fp << " begin";
  fp << std::endl;

  fp << "\t\t\t";
  fp << "if (";
  fp << generate_verilog_constant_values(std::vector<size_t>(bit_skip_reg.get_width(), 1));
  fp << " == ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, bit_skip_reg) << ")"; 
  fp << " begin";
  fp << std::endl;

  fp << "\t\t\t\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " + 1";
  fp << ";" << std::endl;

  fp << "\t\t\t";
  fp << "end";
  fp << std::endl;

  fp << "\t\t";
  fp << "end else begin";
  fp << std::endl;

  fp << "\t\t\t";
  fp << generate_verilog_port_constant_values(bit_skip_reg, std::vector<size_t>(bit_skip_reg.get_width(), 0), true);
  fp << ";" << std::endl;

  fp << "\t\t";
  fp << "end";
  fp << std::endl;

  fp << "\t";
  fp << "end";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX), 1);
  fp << "always";
  fp << " @(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ")"; 
  fp << " begin";
  fp << std::endl;

  fp << "\t";
  fp << "if (";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " >= ";
  fp << "`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE;
  fp << ") begin";
  fp << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
  fp << "\t\t";
  std::vector<size_t> config_done_final_values(config_done_port.get_width(), 1);
  fp << generate_verilog_port_constant_values(config_done_port, config_done_final_values, true);
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end else begin";
  fp << std::endl;

  fp << "\t\t";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, config_chain_head_port); 
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[" << TOP_TB_BITSTREAM_INDEX_REG_NAME << "]";
  fp << ";" << std::endl;

  fp << "\t\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " + 1";
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  print_verilog_comment(fp, "----- End bitstream loading during configuration phase -----");
}

/********************************************************************
 * Print stimulus for a FPGA fabric with a memory bank configuration protocol
 * where configuration bits are programming in serial (one by one)
 *******************************************************************/
static
void print_verilog_full_testbench_memory_bank_bitstream(std::fstream& fp,
                                                        const std::string& bitstream_file,
                                                        const bool& fast_configuration,
                                                        const bool& bit_value_to_skip,
                                                        const ModuleManager& module_manager,
                                                        const ModuleId& top_module,
                                                        const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Reorganize the fabric bitstream by the same address across regions */
  MemoryBankFabricBitstream fabric_bits_by_addr = build_memory_bank_fabric_bitstream_by_address(fabric_bitstream);

  /* For fast configuration, identify the final bitstream size to be used */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip = fabric_bits_by_addr.size() - find_memory_bank_fast_configuration_fabric_bitstream_size(fabric_bitstream, bit_value_to_skip);
  }
  VTR_ASSERT(num_bits_to_skip < fabric_bits_by_addr.size());

  /* Feed address and data input pair one by one
   * Note: the first cycle is reserved for programming reset
   * We should give dummy values
   */
  ModulePortId bl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_BL_ADDRESS_PORT_NAME));
  BasicPort bl_addr_port = module_manager.module_port(top_module, bl_addr_port_id);
  std::vector<size_t> initial_bl_addr_values(bl_addr_port.get_width(), 0);

  ModulePortId wl_addr_port_id = module_manager.find_module_port(top_module,
                                                                 std::string(DECODER_WL_ADDRESS_PORT_NAME));
  BasicPort wl_addr_port = module_manager.module_port(top_module, wl_addr_port_id);
  std::vector<size_t> initial_wl_addr_values(wl_addr_port.get_width(), 0);

  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  std::vector<size_t> initial_din_values(din_port.get_width(), 0);

  /* Define a constant for the bitstream length */
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_LENGTH_VARIABLE), fabric_bits_by_addr.size() - num_bits_to_skip); 
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_WIDTH_VARIABLE), bl_addr_port.get_width() + wl_addr_port.get_width() + din_port.get_width()); 

  /* Declare local variables for bitstream loading in Verilog */
  print_verilog_comment(fp, "----- Virtual memory to store the bitstream from external file -----");
  fp << "reg [0:`" << TOP_TB_BITSTREAM_WIDTH_VARIABLE << " - 1] ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[0:`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << " - 1];";
  fp << std::endl;

  fp << "reg [$clog2(`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << ") - 1:0] " << TOP_TB_BITSTREAM_INDEX_REG_NAME << ";" << std::endl;

  print_verilog_comment(fp, "----- Preload bitstream file to a virtual memory -----");
  fp << "initial begin" << std::endl;
  fp << "\t";
  fp << "$readmemb(\"" << bitstream_file << "\", " << TOP_TB_BITSTREAM_MEM_REG_NAME << ");";
  fp << std::endl;

  print_verilog_comment(fp, "----- Bit-Line Address port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(bl_addr_port, initial_bl_addr_values);
  fp << ";";
  fp << std::endl;

  print_verilog_comment(fp, "----- Word-Line Address port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(wl_addr_port, initial_wl_addr_values);
  fp << ";";
  fp << std::endl;

  print_verilog_comment(fp, "----- Data-input port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(din_port, initial_din_values);
  fp << ";";
  fp << std::endl;

  fp << "\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " <= 0";
  fp << ";";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX), 1);
  fp << "always";
  fp << " @(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ")"; 
  fp << " begin";
  fp << std::endl;

  fp << "\t";
  fp << "if (";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " >= ";
  fp << "`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE;
  fp << ") begin";
  fp << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
  fp << "\t\t";
  std::vector<size_t> config_done_final_values(config_done_port.get_width(), 1);
  fp << generate_verilog_port_constant_values(config_done_port, config_done_final_values, true);
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end else begin";
  fp << std::endl;

  fp << "\t\t";
  fp << "{";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, bl_addr_port); 
  fp << ", ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, wl_addr_port); 
  fp << ", ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, din_port); 
  fp << "}";
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[" << TOP_TB_BITSTREAM_INDEX_REG_NAME << "]";
  fp << ";" << std::endl;

  fp << "\t\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " + 1";
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  print_verilog_comment(fp, "----- End bitstream loading during configuration phase -----");
}

/********************************************************************
 * Print stimulus for a FPGA fabric with a frame-based configuration protocol
 * where configuration bits are programming in serial (one by one)
 *******************************************************************/
static
void print_verilog_full_testbench_frame_decoder_bitstream(std::fstream& fp,
                                                          const std::string& bitstream_file,
                                                          const bool& fast_configuration,
                                                          const bool& bit_value_to_skip,
                                                          const ModuleManager& module_manager,
                                                          const ModuleId& top_module,
                                                          const FabricBitstream& fabric_bitstream) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Reorganize the fabric bitstream by the same address across regions */
  FrameFabricBitstream fabric_bits_by_addr = build_frame_based_fabric_bitstream_by_address(fabric_bitstream);

  /* For fast configuration, identify the final bitstream size to be used */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip = fabric_bits_by_addr.size() - find_frame_based_fast_configuration_fabric_bitstream_size(fabric_bitstream, bit_value_to_skip);
  }
  VTR_ASSERT(num_bits_to_skip < fabric_bits_by_addr.size());

  /* Feed address and data input pair one by one
   * Note: the first cycle is reserved for programming reset
   * We should give dummy values
   */
  ModulePortId addr_port_id = module_manager.find_module_port(top_module,
                                                              std::string(DECODER_ADDRESS_PORT_NAME));
  BasicPort addr_port = module_manager.module_port(top_module, addr_port_id);
  std::vector<size_t> initial_addr_values(addr_port.get_width(), 0);

  ModulePortId din_port_id = module_manager.find_module_port(top_module,
                                                             std::string(DECODER_DATA_IN_PORT_NAME));
  BasicPort din_port = module_manager.module_port(top_module, din_port_id);
  std::vector<size_t> initial_din_values(din_port.get_width(), 0);

  /* Define a constant for the bitstream length */
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_LENGTH_VARIABLE), fabric_bits_by_addr.size() - num_bits_to_skip); 
  print_verilog_define_flag(fp, std::string(TOP_TB_BITSTREAM_WIDTH_VARIABLE), addr_port.get_width() + din_port.get_width()); 

  /* Declare local variables for bitstream loading in Verilog */
  print_verilog_comment(fp, "----- Virtual memory to store the bitstream from external file -----");
  fp << "reg [0:`" << TOP_TB_BITSTREAM_WIDTH_VARIABLE << " - 1] ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[0:`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << " - 1];";
  fp << std::endl;

  fp << "reg [$clog2(`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE << ") - 1:0] " << TOP_TB_BITSTREAM_INDEX_REG_NAME << ";" << std::endl;

  print_verilog_comment(fp, "----- Preload bitstream file to a virtual memory -----");
  fp << "initial begin" << std::endl;
  fp << "\t";
  fp << "$readmemb(\"" << bitstream_file << "\", " << TOP_TB_BITSTREAM_MEM_REG_NAME << ");";
  fp << std::endl;

  print_verilog_comment(fp, "----- Address port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(addr_port, initial_addr_values);
  fp << ";";
  fp << std::endl;

  print_verilog_comment(fp, "----- Data-input port default input -----");
  fp << "\t";
  fp << generate_verilog_port_constant_values(din_port, initial_din_values);
  fp << ";";
  fp << std::endl;

  fp << "\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " <= 0";
  fp << ";";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  print_verilog_comment(fp, "----- Begin bitstream loading during configuration phase -----");
  BasicPort prog_clock_port(std::string(TOP_TB_PROG_CLOCK_PORT_NAME) + std::string(TOP_TB_CLOCK_REG_POSTFIX), 1);
  fp << "always";
  fp << " @(negedge " << generate_verilog_port(VERILOG_PORT_CONKT, prog_clock_port) << ")"; 
  fp << " begin";
  fp << std::endl;

  fp << "\t";
  fp << "if (";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " >= ";
  fp << "`" << TOP_TB_BITSTREAM_LENGTH_VARIABLE;
  fp << ") begin";
  fp << std::endl;

  BasicPort config_done_port(std::string(TOP_TB_CONFIG_DONE_PORT_NAME), 1);
  fp << "\t\t";
  std::vector<size_t> config_done_final_values(config_done_port.get_width(), 1);
  fp << generate_verilog_port_constant_values(config_done_port, config_done_final_values, true);
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end else begin";
  fp << std::endl;

  fp << "\t\t";
  fp << "{";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, addr_port); 
  fp << ", ";
  fp << generate_verilog_port(VERILOG_PORT_CONKT, din_port); 
  fp << "}";
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_MEM_REG_NAME << "[" << TOP_TB_BITSTREAM_INDEX_REG_NAME << "]";
  fp << ";" << std::endl;

  fp << "\t\t";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME;
  fp << " <= ";
  fp << TOP_TB_BITSTREAM_INDEX_REG_NAME << " + 1";
  fp << ";" << std::endl;

  fp << "\t";
  fp << "end";
  fp << std::endl;

  fp << "end";
  fp << std::endl;

  print_verilog_comment(fp, "----- End bitstream loading during configuration phase -----");
}

/********************************************************************
 * Generate the stimuli for the full testbench
 * The simulation consists of two phases: configuration phase and operation phase
 * Configuration bits are loaded serially.
 * This is actually what we do for a physical FPGA
 *******************************************************************/
static
void print_verilog_full_testbench_bitstream(std::fstream& fp,
                                            const std::string& bitstream_file,
                                            const e_config_protocol_type& config_protocol_type,
                                            const bool& fast_configuration,
                                            const bool& bit_value_to_skip,
                                            const ModuleManager& module_manager,
                                            const ModuleId& top_module,
                                            const BitstreamManager& bitstream_manager,
                                            const FabricBitstream& fabric_bitstream) {

  /* Branch on the type of configuration protocol */
  switch (config_protocol_type) {
  case CONFIG_MEM_STANDALONE:
    break;
  case CONFIG_MEM_SCAN_CHAIN:
    print_verilog_full_testbench_configuration_chain_bitstream(fp, bitstream_file,
                                                               fast_configuration, 
                                                               bit_value_to_skip,
                                                               module_manager, top_module,
                                                               bitstream_manager,
                                                               fabric_bitstream);
    break;
  case CONFIG_MEM_MEMORY_BANK:
    print_verilog_full_testbench_memory_bank_bitstream(fp, bitstream_file,
                                                       fast_configuration, 
                                                       bit_value_to_skip,
                                                       module_manager, top_module,
                                                       fabric_bitstream);
    break;
  case CONFIG_MEM_FRAME_BASED:
    print_verilog_full_testbench_frame_decoder_bitstream(fp, bitstream_file,
                                                         fast_configuration, 
                                                         bit_value_to_skip,
                                                         module_manager, top_module,
                                                         fabric_bitstream);

    break;
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid configuration protocol type!\n");
    exit(1);
  }
}


/********************************************************************
 * Connect proper stimuli to the reset port
 * This function is designed to drive the reset port of a benchmark module
 *******************************************************************/
static 
void print_verilog_top_testbench_reset_stimuli(std::fstream& fp,
                                               const AtomContext& atom_ctx,
                                               const VprNetlistAnnotation& netlist_annotation,
                                               const ModuleManager& module_manager,
                                               const FabricGlobalPortInfo& global_ports,
                                               const PinConstraints& pin_constraints,
                                               const std::vector<std::string>& clock_port_names) {
  valid_file_stream(fp);

  print_verilog_comment(fp, "----- Begin reset signal generation -----");

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-input atom blocks ! */
    if (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    } 

    /* Bypass clock ports because their stimulus cannot be random */
    if (clock_port_names.end() != std::find(clock_port_names.begin(), clock_port_names.end(), block_name)) {
      continue;
    }

    /* Bypass any constained net that are mapped to a global port of the FPGA fabric
     * because their stimulus cannot be random
     */
    if (false == port_is_fabric_global_reset_port(global_ports, module_manager, pin_constraints.net_pin(block_name))) { 
      continue;
    }

    /* Connect stimuli to greset with an optional inversion, depending on the default value */
    BasicPort reset_port(block_name, 1);
    print_verilog_wire_connection(fp, reset_port,
                                  BasicPort(TOP_TB_RESET_PORT_NAME, 1),
                                  1 == global_ports.global_port_default_value(find_fabric_global_port(global_ports, module_manager, pin_constraints.net_pin(block_name))));
  }
}

/********************************************************************
 * Add auto-check codes for the full testbench
 * in particular for the configuration phase:
 * - Check that the configuration done signal is raised, indicating
 *   that the configuration phase is finished
 *******************************************************************/
static
void print_verilog_top_testbench_check(std::fstream& fp, 
                                       const std::string& autochecked_preprocessing_flag,
                                       const std::string& config_done_port_name,
                                       const std::string& error_counter_name) {

  /* Validate the file stream */
  valid_file_stream(fp);

  /* Add output autocheck conditionally: only when a preprocessing flag is enable */
  print_verilog_preprocessing_flag(fp, autochecked_preprocessing_flag); 

  print_verilog_comment(fp, std::string("----- Configuration done must be raised in the end -------"));

  BasicPort config_done_port(config_done_port_name, 1);

  write_tab_to_file(fp, 1);
  fp << "always@(posedge " << generate_verilog_port(VERILOG_PORT_CONKT, config_done_port) << ") begin" << std::endl;

  write_tab_to_file(fp, 2);
  fp << error_counter_name << " = " << error_counter_name << " - 1;" << std::endl;

  write_tab_to_file(fp, 1);
  fp << "end" << std::endl;

  /* Condition ends */
  print_verilog_endif(fp);

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * TODO: This top function will be deprecated!!! The only top function is
 *   print_verilog_full_testbench()
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
                                 const FabricBitstream& fabric_bitstream,
                                 const CircuitLibrary& circuit_lib,
                                 const ConfigProtocol& config_protocol,
                                 const FabricGlobalPortInfo& global_ports,
                                 const AtomContext& atom_ctx,
                                 const PlacementContext& place_ctx,
                                 const PinConstraints& pin_constraints,
                                 const IoLocationMap& io_location_map,
                                 const VprNetlistAnnotation& netlist_annotation,
                                 const std::string& circuit_name,
                                 const std::string& verilog_fname,
                                 const SimulationSetting& simulation_parameters,
                                 const VerilogTestbenchOption& options) {

  bool fast_configuration = options.fast_configuration();
  bool explicit_port_mapping = options.explicit_port_mapping();

  std::string timer_message = std::string("Write autocheck testbench for FPGA top-level Verilog netlist for '") + circuit_name + std::string("'");

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Generate a brief description on the Verilog file*/
  std::string title = std::string("FPGA Verilog Testbench for Top-level netlist of Design: ") + circuit_name;
  print_verilog_file_header(fp, title);

  /* Find the top_module */
  ModuleId top_module = module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Preparation: find all the clock ports */
  std::vector<std::string> clock_port_names = find_atom_netlist_clock_port_names(atom_ctx.nlist, netlist_annotation);

  /* Preparation: find all the reset/set ports for programming usage */
  std::vector<FabricGlobalPortId> global_prog_reset_ports = find_fabric_global_programming_reset_ports(global_ports);
  std::vector<FabricGlobalPortId> global_prog_set_ports = find_fabric_global_programming_set_ports(global_ports);

  /* Identify if we can apply fast configuration */
  bool apply_fast_configuration = fast_configuration;
  if ( (global_prog_set_ports.empty() && global_prog_reset_ports.empty())
     && (true == fast_configuration)) {
    VTR_LOG_WARN("None of global reset and set ports are defined for programming purpose. Fast configuration is turned off\n");
    apply_fast_configuration = false;
  }
  bool bit_value_to_skip = find_bit_value_to_skip_for_fast_configuration(config_protocol.type(),
                                                                         apply_fast_configuration,
                                                                         global_prog_reset_ports, 
                                                                         global_prog_set_ports, 
                                                                         bitstream_manager, fabric_bitstream);

  /* Start of testbench */
  print_verilog_top_testbench_ports(fp, module_manager, top_module,
                                    atom_ctx, netlist_annotation,
                                    clock_port_names,
                                    pin_constraints,
                                    simulation_parameters, config_protocol,
                                    circuit_name);

  /* Find the clock period */
  float prog_clock_period = (1./simulation_parameters.programming_clock_frequency());
  float default_op_clock_period = (1./simulation_parameters.default_operating_clock_frequency());
  float max_op_clock_period = 0.;
  for (const SimulationClockId& clock_id : simulation_parameters.clocks()) {
    max_op_clock_period = std::max(max_op_clock_period, (float)(1./simulation_parameters.clock_frequency(clock_id)));
  }

  /* Estimate the number of configuration clock cycles */
  size_t num_config_clock_cycles = calculate_num_config_clock_cycles(config_protocol.type(),
                                                                     apply_fast_configuration,
                                                                     bit_value_to_skip,
                                                                     bitstream_manager,
                                                                     fabric_bitstream);

  /* Generate stimuli for general control signals */
  print_verilog_top_testbench_generic_stimulus(fp,
                                               simulation_parameters,
                                               num_config_clock_cycles,
                                               prog_clock_period,
                                               default_op_clock_period,
                                               VERILOG_SIM_TIMESCALE);

  /* Generate stimuli for programming interface */
  print_verilog_top_testbench_configuration_protocol_stimulus(fp, 
                                                              config_protocol.type(),
                                                              module_manager, top_module,
                                                              prog_clock_period,
                                                              VERILOG_SIM_TIMESCALE);
                                                      
  /* Identify the stimulus for global reset/set for programming purpose:
   * - If only reset port is seen we turn on Reset 
   * - If only set port is seen we turn on Reset 
   * - If both reset and set port is defined,
   *   we pick the one which is consistent with the bit value to be skipped
   */
  bool active_global_prog_reset = false; 
  bool active_global_prog_set = false; 

  if (!global_prog_reset_ports.empty()) {
    active_global_prog_reset = true;
  }

  if (!global_prog_set_ports.empty()) {
    active_global_prog_set = true;
  }

  /* Ensure that at most only one of the two switches is activated */
  if ( (true == active_global_prog_reset)
    && (true == active_global_prog_set) ) { 
    /* If we will skip logic '0', we will activate programming reset */
    active_global_prog_reset = !bit_value_to_skip;
    /* If we will skip logic '1', we will activate programming set */
    active_global_prog_set = bit_value_to_skip;
  }

  /* Generate stimuli for global ports or connect them to existed signals */
  print_verilog_top_testbench_global_ports_stimuli(fp,
                                                   module_manager, top_module,
                                                   pin_constraints,
                                                   global_ports,
                                                   simulation_parameters,
                                                   active_global_prog_reset,
                                                   active_global_prog_set);

  /* Instanciate FPGA top-level module */
  print_verilog_testbench_fpga_instance(fp, module_manager, top_module,
                                        std::string(TOP_TESTBENCH_FPGA_INSTANCE_NAME),
                                        explicit_port_mapping);

  /* Connect I/Os to benchmark I/Os or constant driver */
  print_verilog_testbench_connect_fpga_ios(fp, module_manager, top_module,
                                           atom_ctx, place_ctx, io_location_map,
                                           netlist_annotation,
                                           std::string(),
                                           std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX),
                                           (size_t)VERILOG_DEFAULT_SIGNAL_INIT_VALUE);

  /* Instanciate input benchmark */
  print_verilog_top_testbench_benchmark_instance(fp,
                                                 circuit_name,
                                                 atom_ctx,
                                                 netlist_annotation,
                                                 explicit_port_mapping);

  /* Print tasks used for loading bitstreams */
  print_verilog_top_testbench_load_bitstream_task(fp,
                                                  config_protocol.type(),
                                                  module_manager, top_module);

  /* load bitstream to FPGA fabric in a configuration phase */
  print_verilog_top_testbench_bitstream(fp, config_protocol.type(),
                                        apply_fast_configuration,
                                        bit_value_to_skip,
                                        module_manager, top_module,
                                        bitstream_manager, fabric_bitstream);

  /* Add signal initialization: 
   * Bypass writing codes to files due to the autogenerated codes are very large.
   */
  if (true == options.include_signal_init()) {
    print_verilog_testbench_signal_initialization(fp,
                                                  std::string(TOP_TESTBENCH_FPGA_INSTANCE_NAME),
                                                  circuit_lib,
                                                  module_manager,
                                                  top_module);
  }


  /* Add stimuli for reset, set, clock and iopad signals */
  print_verilog_top_testbench_reset_stimuli(fp, 
                                            atom_ctx,
                                            netlist_annotation, 
                                            module_manager,
                                            global_ports, 
                                            pin_constraints, 
                                            clock_port_names);
  print_verilog_testbench_random_stimuli(fp, atom_ctx,
                                         netlist_annotation,
                                         module_manager,
                                         global_ports,
                                         pin_constraints,
                                         clock_port_names,
                                         std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                         std::vector<BasicPort>(1, BasicPort(std::string(TOP_TB_OP_CLOCK_PORT_NAME), 1)));

  /* Add output autocheck */
  print_verilog_testbench_check(fp,
                                std::string(AUTOCHECKED_SIMULATION_FLAG),
                                std::string(TOP_TESTBENCH_SIM_START_PORT_NAME),
                                std::string(TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX),
                                std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX),
                                std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                std::string(TOP_TESTBENCH_ERROR_COUNTER),
                                atom_ctx,
                                netlist_annotation,
                                clock_port_names,
                                std::string(TOP_TB_OP_CLOCK_PORT_NAME));

  /* Add autocheck for configuration phase */
  print_verilog_top_testbench_check(fp, 
                                    std::string(AUTOCHECKED_SIMULATION_FLAG),
                                    std::string(TOP_TB_CONFIG_DONE_PORT_NAME),
                                    std::string(TOP_TESTBENCH_ERROR_COUNTER));

  /* Find simulation time */
  float simulation_time = find_simulation_time_period(VERILOG_SIM_TIMESCALE,
                                                      num_config_clock_cycles,
										              1./simulation_parameters.programming_clock_frequency(),
                                                      simulation_parameters.num_clock_cycles(),
                                                      1./simulation_parameters.default_operating_clock_frequency());


  /* Add Icarus requirement: 
   * Always ceil the simulation time so that we test a sufficient length of period!!!
   */
  print_verilog_timeout_and_vcd(fp,
                                std::string(circuit_name + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_MODULE_POSTFIX)),
                                std::string(circuit_name + std::string("_formal.vcd")),
                                std::string(TOP_TESTBENCH_SIM_START_PORT_NAME),
                                std::string(TOP_TESTBENCH_ERROR_COUNTER),
                                std::ceil(simulation_time));


  /* Testbench ends*/
  print_verilog_module_end(fp, std::string(circuit_name) + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_MODULE_POSTFIX));

  /* Close the file stream */
  fp.close();
}

/********************************************************************
 * The top-level function to generate a full testbench, in order to verify:
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
int print_verilog_full_testbench(const ModuleManager& module_manager,
                                 const BitstreamManager& bitstream_manager,
                                 const FabricBitstream& fabric_bitstream,
                                 const CircuitLibrary& circuit_lib,
                                 const ConfigProtocol& config_protocol,
                                 const FabricGlobalPortInfo& global_ports,
                                 const AtomContext& atom_ctx,
                                 const PlacementContext& place_ctx,
                                 const PinConstraints& pin_constraints,
                                 const std::string& bitstream_file,
                                 const IoLocationMap& io_location_map,
                                 const VprNetlistAnnotation& netlist_annotation,
                                 const std::string& circuit_name,
                                 const std::string& verilog_fname,
                                 const SimulationSetting& simulation_parameters,
                                 const VerilogTestbenchOption& options) {

  bool fast_configuration = options.fast_configuration();
  bool explicit_port_mapping = options.explicit_port_mapping();

  std::string timer_message = std::string("Write autocheck testbench for FPGA top-level Verilog netlist for '") + circuit_name + std::string("'");

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Generate a brief description on the Verilog file*/
  std::string title = std::string("FPGA Verilog full testbench for top-level netlist of design: ") + circuit_name;
  print_verilog_file_header(fp, title);

  /* Find the top_module */
  ModuleId top_module = module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Preparation: find all the clock ports */
  std::vector<std::string> clock_port_names = find_atom_netlist_clock_port_names(atom_ctx.nlist, netlist_annotation);

  /* Preparation: find all the reset/set ports for programming usage */
  std::vector<FabricGlobalPortId> global_prog_reset_ports = find_fabric_global_programming_reset_ports(global_ports);
  std::vector<FabricGlobalPortId> global_prog_set_ports = find_fabric_global_programming_set_ports(global_ports);

  /* Identify if we can apply fast configuration */
  bool apply_fast_configuration = fast_configuration;
  if ( (global_prog_set_ports.empty() && global_prog_reset_ports.empty())
     && (true == fast_configuration)) {
    VTR_LOG_WARN("None of global reset and set ports are defined for programming purpose. Fast configuration is turned off\n");
    apply_fast_configuration = false;
  }
  bool bit_value_to_skip = find_bit_value_to_skip_for_fast_configuration(config_protocol.type(),
                                                                         apply_fast_configuration,
                                                                         global_prog_reset_ports, 
                                                                         global_prog_set_ports, 
                                                                         bitstream_manager, fabric_bitstream);

  /* Start of testbench */
  print_verilog_top_testbench_ports(fp, module_manager, top_module,
                                    atom_ctx, netlist_annotation,
                                    clock_port_names,
                                    pin_constraints,
                                    simulation_parameters, config_protocol,
                                    circuit_name);

  /* Find the clock period */
  float prog_clock_period = (1./simulation_parameters.programming_clock_frequency());
  float default_op_clock_period = (1./simulation_parameters.default_operating_clock_frequency());
  float max_op_clock_period = 0.;
  for (const SimulationClockId& clock_id : simulation_parameters.clocks()) {
    max_op_clock_period = std::max(max_op_clock_period, (float)(1./simulation_parameters.clock_frequency(clock_id)));
  }

  /* Estimate the number of configuration clock cycles */
  size_t num_config_clock_cycles = calculate_num_config_clock_cycles(config_protocol.type(),
                                                                     apply_fast_configuration,
                                                                     bit_value_to_skip,
                                                                     bitstream_manager,
                                                                     fabric_bitstream);

  /* Generate stimuli for general control signals */
  print_verilog_top_testbench_generic_stimulus(fp,
                                               simulation_parameters,
                                               num_config_clock_cycles,
                                               prog_clock_period,
                                               default_op_clock_period,
                                               VERILOG_SIM_TIMESCALE);

  /* Generate stimuli for programming interface */
  print_verilog_top_testbench_configuration_protocol_stimulus(fp, 
                                                              config_protocol.type(),
                                                              module_manager, top_module,
                                                              prog_clock_period,
                                                              VERILOG_SIM_TIMESCALE);
                                                      
  /* Identify the stimulus for global reset/set for programming purpose:
   * - If only reset port is seen we turn on Reset 
   * - If only set port is seen we turn on Reset 
   * - If both reset and set port is defined,
   *   we pick the one which is consistent with the bit value to be skipped
   */
  bool active_global_prog_reset = false; 
  bool active_global_prog_set = false; 

  if (!global_prog_reset_ports.empty()) {
    active_global_prog_reset = true;
  }

  if (!global_prog_set_ports.empty()) {
    active_global_prog_set = true;
  }

  /* Ensure that at most only one of the two switches is activated */
  if ( (true == active_global_prog_reset)
    && (true == active_global_prog_set) ) { 
    /* If we will skip logic '0', we will activate programming reset */
    active_global_prog_reset = !bit_value_to_skip;
    /* If we will skip logic '1', we will activate programming set */
    active_global_prog_set = bit_value_to_skip;
  }

  /* Generate stimuli for global ports or connect them to existed signals */
  print_verilog_top_testbench_global_ports_stimuli(fp,
                                                   module_manager, top_module,
                                                   pin_constraints,
                                                   global_ports,
                                                   simulation_parameters,
                                                   active_global_prog_reset,
                                                   active_global_prog_set);

  /* Instanciate FPGA top-level module */
  print_verilog_testbench_fpga_instance(fp, module_manager, top_module,
                                        std::string(TOP_TESTBENCH_FPGA_INSTANCE_NAME),
                                        explicit_port_mapping);

  /* Connect I/Os to benchmark I/Os or constant driver */
  print_verilog_testbench_connect_fpga_ios(fp, module_manager, top_module,
                                           atom_ctx, place_ctx, io_location_map,
                                           netlist_annotation,
                                           std::string(),
                                           std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX),
                                           (size_t)VERILOG_DEFAULT_SIGNAL_INIT_VALUE);

  /* Instanciate input benchmark */
  print_verilog_top_testbench_benchmark_instance(fp,
                                                 circuit_name,
                                                 atom_ctx,
                                                 netlist_annotation,
                                                 explicit_port_mapping);

  /* Print tasks used for loading bitstreams */
  print_verilog_top_testbench_load_bitstream_task(fp,
                                                  config_protocol.type(),
                                                  module_manager, top_module);

  /* load bitstream to FPGA fabric in a configuration phase */
  print_verilog_full_testbench_bitstream(fp,
                                         bitstream_file,
                                         config_protocol.type(),
                                         apply_fast_configuration,
                                         bit_value_to_skip,
                                         module_manager, top_module,
                                         bitstream_manager, fabric_bitstream);

  /* Add signal initialization: 
   * Bypass writing codes to files due to the autogenerated codes are very large.
   */
  if (true == options.include_signal_init()) {
    print_verilog_testbench_signal_initialization(fp,
                                                  std::string(TOP_TESTBENCH_FPGA_INSTANCE_NAME),
                                                  circuit_lib,
                                                  module_manager,
                                                  top_module);
  }


  /* Add stimuli for reset, set, clock and iopad signals */
  print_verilog_top_testbench_reset_stimuli(fp, 
                                            atom_ctx,
                                            netlist_annotation, 
                                            module_manager,
                                            global_ports, 
                                            pin_constraints, 
                                            clock_port_names);
  print_verilog_testbench_random_stimuli(fp, atom_ctx,
                                         netlist_annotation,
                                         module_manager,
                                         global_ports,
                                         pin_constraints,
                                         clock_port_names,
                                         std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                         std::vector<BasicPort>(1, BasicPort(std::string(TOP_TB_OP_CLOCK_PORT_NAME), 1)));

  /* Add output autocheck */
  print_verilog_testbench_check(fp,
                                std::string(AUTOCHECKED_SIMULATION_FLAG),
                                std::string(TOP_TESTBENCH_SIM_START_PORT_NAME),
                                std::string(TOP_TESTBENCH_REFERENCE_OUTPUT_POSTFIX),
                                std::string(TOP_TESTBENCH_FPGA_OUTPUT_POSTFIX),
                                std::string(TOP_TESTBENCH_CHECKFLAG_PORT_POSTFIX),
                                std::string(TOP_TESTBENCH_ERROR_COUNTER),
                                atom_ctx,
                                netlist_annotation,
                                clock_port_names,
                                std::string(TOP_TB_OP_CLOCK_PORT_NAME));

  /* Add autocheck for configuration phase */
  print_verilog_top_testbench_check(fp, 
                                    std::string(AUTOCHECKED_SIMULATION_FLAG),
                                    std::string(TOP_TB_CONFIG_DONE_PORT_NAME),
                                    std::string(TOP_TESTBENCH_ERROR_COUNTER));

  /* Find simulation time */
  float simulation_time = find_simulation_time_period(VERILOG_SIM_TIMESCALE,
                                                      num_config_clock_cycles,
										              1./simulation_parameters.programming_clock_frequency(),
                                                      simulation_parameters.num_clock_cycles(),
                                                      1./simulation_parameters.default_operating_clock_frequency());


  /* Add Icarus requirement: 
   * Always ceil the simulation time so that we test a sufficient length of period!!!
   */
  print_verilog_timeout_and_vcd(fp,
                                std::string(circuit_name + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_MODULE_POSTFIX)),
                                std::string(circuit_name + std::string("_formal.vcd")),
                                std::string(TOP_TESTBENCH_SIM_START_PORT_NAME),
                                std::string(TOP_TESTBENCH_ERROR_COUNTER),
                                std::ceil(simulation_time));


  /* Testbench ends*/
  print_verilog_module_end(fp, std::string(circuit_name) + std::string(AUTOCHECK_TOP_TESTBENCH_VERILOG_MODULE_POSTFIX));

  /* Close the file stream */
  fp.close();

  return 0;
}


} /* end namespace openfpga */
