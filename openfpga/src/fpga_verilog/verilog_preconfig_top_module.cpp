/********************************************************************
 * This file includes functions that are used to generate
 * a Verilog module of a pre-configured FPGA fabric
 *******************************************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"
#include "openfpga_digest.h"
#include "openfpga_reserved_words.h"

#include "bitstream_manager_utils.h"
#include "openfpga_atom_netlist_utils.h"

#include "openfpga_naming.h"

#include "verilog_constants.h"
#include "verilog_writer_utils.h"
#include "verilog_testbench_utils.h"
#include "verilog_preconfig_top_module.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print module declaration and ports for the pre-configured
 * FPGA top module
 * The module ports do exactly match the input benchmark
 *******************************************************************/
static 
void print_verilog_preconfig_top_module_ports(std::fstream &fp,
                                              const std::string &circuit_name,
                                              const AtomContext &atom_ctx,
                                              const VprNetlistAnnotation &netlist_annotation,
                                              const BusGroup& bus_group) {

  /* Validate the file stream */
  valid_file_stream(fp);

  /* Module declaration */
  fp << "module " << circuit_name << std::string(FORMAL_VERIFICATION_TOP_MODULE_POSTFIX);
  fp << " (" << std::endl;

  /* Port type-to-type mapping */
  std::map<AtomBlockType, enum e_dump_verilog_port_type> port_type2type_map;
  port_type2type_map[AtomBlockType::INPAD] = VERILOG_PORT_INPUT;
  port_type2type_map[AtomBlockType::OUTPAD] = VERILOG_PORT_OUTPUT;

  /* Ports to be added, this is to avoid any bus port */
  std::vector<BasicPort> port_list;
  std::vector<AtomBlockType> port_types;
  std::vector<bool> port_big_endian;

  /* Print all the I/Os of the circuit implementation to be tested*/
  for (const AtomBlockId &atom_blk : atom_ctx.nlist.blocks()) {
    /* We only care I/O logical blocks !*/
    if ((AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) && (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk))) {
      continue;
    }

    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    /* The block may be renamed as it contains special characters which violate Verilog syntax */
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    /* For output block, remove the prefix which is added by VPR */
    std::vector<std::string> output_port_prefix_to_remove;
    output_port_prefix_to_remove.push_back(std::string(VPR_BENCHMARK_OUT_PORT_PREFIX));
    output_port_prefix_to_remove.push_back(std::string(OPENFPGA_BENCHMARK_OUT_PORT_PREFIX));
    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
      for (const std::string& prefix_to_remove : output_port_prefix_to_remove) {
        if (!prefix_to_remove.empty()) {
          if (0 == block_name.find(prefix_to_remove)) {
            block_name.erase(0, prefix_to_remove.length());
            break;
          }
        }
      }
    }

    /* If the pin is part of a bus,
     * - Check if the bus is already in the list
     *   - If not, add it to the port list
     *   - If yes, do nothing and move onto the next port
     * If the pin does not belong to any bus
     * - Add it to the bus port
     */
    BusGroupId bus_id = bus_group.find_pin_bus(block_name);
    if (bus_id) {
      if (port_list.end() == std::find(port_list.begin(), port_list.end(), bus_group.bus_port(bus_id))) {
        port_list.push_back(bus_group.bus_port(bus_id));
        port_types.push_back(atom_ctx.nlist.block_type(atom_blk));
        port_big_endian.push_back(bus_group.is_big_endian(bus_id));
      }
      continue;
    }

    /* Both input and output ports have only size of 1 */
    BasicPort module_port(std::string(block_name), 1);
    port_list.push_back(module_port);
    port_types.push_back(atom_ctx.nlist.block_type(atom_blk));
    port_big_endian.push_back(true);
  }

  /* After collecting all the ports, now print the port mapping */
  size_t port_counter = 0;
  for (size_t iport = 0; iport < port_list.size(); ++iport) {
    BasicPort module_port = port_list[iport];
    AtomBlockType port_type = port_types[iport];
    if (0 < port_counter) {
      fp << "," << std::endl;
    }

    fp << generate_verilog_port(port_type2type_map[port_type], module_port, true, port_big_endian[iport]);

    /* Update port counter */
    port_counter++;
  }

  fp << ");" << std::endl;

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Print internal wires for the pre-configured FPGA top module
 * The internal wires are tailored for the ports of FPGA top module
 * which will be different in various configuration protocols
 *******************************************************************/
static 
void print_verilog_preconfig_top_module_internal_wires(std::fstream &fp,
                                                       const ModuleManager &module_manager,
                                                       const ModuleId &top_module) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Global ports of top-level module  */
  print_verilog_comment(fp, std::string("----- Local wires for FPGA fabric -----"));
  for (const ModulePortId &module_port_id : module_manager.module_ports(top_module)) {
    BasicPort module_port = module_manager.module_port(top_module, module_port_id);
    /* Add a postfix to the internal wires to be different from other reserved ports */
    module_port.set_name(module_port.get_name() + std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX));
    fp << generate_verilog_port(VERILOG_PORT_WIRE, module_port) << ";" << std::endl;
  }
  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Connect global ports of FPGA top module to constants except:
 * 1. operating clock, which should be wired to the clock port of
 * this pre-configured FPGA top module
 *******************************************************************/
static 
int print_verilog_preconfig_top_module_connect_global_ports(std::fstream &fp,
                                                            const ModuleManager &module_manager,
                                                            const ModuleId &top_module,
                                                            const PinConstraints& pin_constraints,
                                                            const FabricGlobalPortInfo &fabric_global_ports,
                                                            const std::vector<std::string> &benchmark_clock_port_names) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Begin Connect Global ports of FPGA top module -----"));

  for (const FabricGlobalPortId& global_port_id : fabric_global_ports.global_ports()) {
    ModulePortId module_global_port_id = fabric_global_ports.global_module_port(global_port_id);
    VTR_ASSERT(ModuleManager::MODULE_GLOBAL_PORT == module_manager.port_type(top_module, module_global_port_id));
    BasicPort module_global_port = module_manager.module_port(top_module, module_global_port_id);
    /* Now, for operating clock port, we should wire it to the clock of benchmark! */
    if ((true == fabric_global_ports.global_port_is_clock(global_port_id)) 
     && (false == fabric_global_ports.global_port_is_prog(global_port_id))) {
      /* Wiring to each pin of the global port: benchmark clock is always 1-bit */
      for (size_t pin_id = 0; pin_id < module_global_port.pins().size(); ++pin_id) {
        BasicPort module_clock_pin(module_global_port.get_name() + std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX), module_global_port.pins()[pin_id], module_global_port.pins()[pin_id]);

        /* If the clock port name is in the pin constraints, we should wire it to the constrained pin */
        std::string constrained_net_name = pin_constraints.pin_net(BasicPort(module_global_port.get_name(), module_global_port.pins()[pin_id], module_global_port.pins()[pin_id]));

        /* If constrained to an open net or there is no clock in the benchmark, we assign it to a default value */
        if ( (true == pin_constraints.unmapped_net(constrained_net_name))
          || (true == benchmark_clock_port_names.empty())) {
          std::vector<size_t> default_values(1, fabric_global_ports.global_port_default_value(global_port_id));
          print_verilog_wire_constant_values(fp, module_clock_pin, default_values);
          continue;
        }

        std::string clock_name_to_connect;
        if (!pin_constraints.unconstrained_net(constrained_net_name)) {
          clock_name_to_connect = constrained_net_name;
        } else {
          /* Otherwise, we must have a clear one-to-one clock net corresponding!!! */
          if (benchmark_clock_port_names.size() != module_global_port.get_width()) {
            VTR_LOG_ERROR("Unable to map %lu benchmark clocks to %lu clock pins of FPGA!\nRequire clear pin constraints!\n",
                          benchmark_clock_port_names.size(),
                          module_global_port.get_width());
            return CMD_EXEC_FATAL_ERROR;
          }
          clock_name_to_connect = benchmark_clock_port_names[pin_id]; 
        }

        BasicPort benchmark_clock_pin(clock_name_to_connect, 1);
        print_verilog_wire_connection(fp, module_clock_pin, benchmark_clock_pin, false);
      }
      /* Finish, go to the next */
      continue;
    }

    /* For other ports, give an default value */
    for (size_t pin_id = 0; pin_id < module_global_port.pins().size(); ++pin_id) {
      BasicPort module_global_pin(module_global_port.get_name(),
                                  module_global_port.pins()[pin_id],
                                  module_global_port.pins()[pin_id]);

      /* If the global port name is in the pin constraints, we should wire it to the constrained pin */
      std::string constrained_net_name = pin_constraints.pin_net(module_global_pin);

      module_global_pin.set_name(module_global_port.get_name() + std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX));

      /* - If constrained to a given net in the benchmark, we connect the global pin to the net
       * - If constrained to an open net in the benchmark, we assign it to a default value 
       */
      if ( (false == pin_constraints.unconstrained_net(constrained_net_name))
        && (false == pin_constraints.unmapped_net(constrained_net_name))) {
        BasicPort benchmark_pin(constrained_net_name, 1);
        print_verilog_wire_connection(fp, module_global_pin, benchmark_pin, false);
      } else {
        VTR_ASSERT_SAFE(std::string(PIN_CONSTRAINT_OPEN_NET) == constrained_net_name);
        std::vector<size_t> default_values(module_global_pin.get_width(), fabric_global_ports.global_port_default_value(global_port_id));
        /* For configuration done signals, we should enable them in preconfigured wrapper */
        if (fabric_global_ports.global_port_is_config_enable(global_port_id)) {
          VTR_LOG("Config-enable port '%s' is detected with default value '%ld'", module_global_pin.get_name().c_str(), fabric_global_ports.global_port_default_value(global_port_id));
          default_values.clear();
          default_values.resize(module_global_pin.get_width(), 1 - fabric_global_ports.global_port_default_value(global_port_id));
        }
        print_verilog_wire_constant_values(fp, module_global_pin, default_values);
      }
    }
  }

  print_verilog_comment(fp, std::string("----- End Connect Global ports of FPGA top module -----"));

  /* Add an empty line as a splitter */
  fp << std::endl;

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Impose the bitstream on the configuration memories
 * This function uses 'assign' syntax to impost the bitstream at mem port
 * while uses 'force' syntax to impost the bitstream at mem_inv port
 *******************************************************************/
static 
void print_verilog_preconfig_top_module_force_bitstream(std::fstream &fp,
                                                        const ModuleManager &module_manager,
                                                        const ModuleId &top_module,
                                                        const BitstreamManager &bitstream_manager,
                                                        const bool& output_datab_bits) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Begin assign bitstream to configuration memories -----"));

  fp << "initial begin" << std::endl;

  for (const ConfigBlockId &config_block_id : bitstream_manager.blocks()) {
    /* We only cares blocks with configuration bits */
    if (0 == bitstream_manager.block_bits(config_block_id).size()) {
      continue;
    }
    /* Build the hierarchical path of the configuration bit in modules */
    std::vector<ConfigBlockId> block_hierarchy = find_bitstream_manager_block_hierarchy(bitstream_manager, config_block_id);
    /* Drop the first block, which is the top module, it should be replaced by the instance name here */
    /* Ensure that this is the module we want to drop! */
    VTR_ASSERT(0 == module_manager.module_name(top_module).compare(bitstream_manager.block_name(block_hierarchy[0])));
    block_hierarchy.erase(block_hierarchy.begin());
    /* Build the full hierarchy path */
    std::string bit_hierarchy_path(FORMAL_VERIFICATION_TOP_MODULE_UUT_NAME);
    for (const ConfigBlockId &temp_block : block_hierarchy) {
      bit_hierarchy_path += std::string(".");
      bit_hierarchy_path += bitstream_manager.block_name(temp_block);
    }
    bit_hierarchy_path += std::string(".");

    /* Find the bit index in the parent block */
    BasicPort config_data_port(bit_hierarchy_path + generate_configurable_memory_data_out_name(),
                               bitstream_manager.block_bits(config_block_id).size());

    /* Wire it to the configuration bit: access both data out and data outb ports */
    std::vector<size_t> config_data_values;
    for (const ConfigBitId config_bit : bitstream_manager.block_bits(config_block_id)) {
      config_data_values.push_back(bitstream_manager.bit_value(config_bit));
    }
    print_verilog_force_wire_constant_values(fp, config_data_port, config_data_values);

    if (true == output_datab_bits) {
      /* Find the bit index in the parent block */
      BasicPort config_datab_port(bit_hierarchy_path + generate_configurable_memory_inverted_data_out_name(),
                                  bitstream_manager.block_bits(config_block_id).size());

      std::vector<size_t> config_datab_values;
      for (const ConfigBitId config_bit : bitstream_manager.block_bits(config_block_id)) {
        config_datab_values.push_back(!bitstream_manager.bit_value(config_bit));
      }
      print_verilog_force_wire_constant_values(fp, config_datab_port, config_datab_values);
    }
  }

  fp << "end" << std::endl;

  print_verilog_comment(fp, std::string("----- End assign bitstream to configuration memories -----"));
}

/********************************************************************
 * Impose the bitstream on the configuration memories
 * This function uses '$deposit' syntax to do so
 *******************************************************************/
static 
void print_verilog_preconfig_top_module_deposit_bitstream(std::fstream &fp,
                                                          const ModuleManager &module_manager,
                                                          const ModuleId &top_module,
                                                          const BitstreamManager &bitstream_manager,
                                                          const bool& output_datab_bits) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Begin deposit bitstream to configuration memories -----"));

  fp << "initial begin" << std::endl;

  for (const ConfigBlockId &config_block_id : bitstream_manager.blocks()) {
    /* We only cares blocks with configuration bits */
    if (0 == bitstream_manager.block_bits(config_block_id).size()) {
      continue;
    }
    /* Build the hierarchical path of the configuration bit in modules */
    std::vector<ConfigBlockId> block_hierarchy = find_bitstream_manager_block_hierarchy(bitstream_manager, config_block_id);
    /* Drop the first block, which is the top module, it should be replaced by the instance name here */
    /* Ensure that this is the module we want to drop! */
    VTR_ASSERT(0 == module_manager.module_name(top_module).compare(bitstream_manager.block_name(block_hierarchy[0])));
    block_hierarchy.erase(block_hierarchy.begin());
    /* Build the full hierarchy path */
    std::string bit_hierarchy_path(FORMAL_VERIFICATION_TOP_MODULE_UUT_NAME);
    for (const ConfigBlockId &temp_block : block_hierarchy) {
      bit_hierarchy_path += std::string(".");
      bit_hierarchy_path += bitstream_manager.block_name(temp_block);
    }
    bit_hierarchy_path += std::string(".");

    /* Find the bit index in the parent block */
    BasicPort config_data_port(bit_hierarchy_path + generate_configurable_memory_data_out_name(),
                               bitstream_manager.block_bits(config_block_id).size());

    /* Wire it to the configuration bit: access both data out and data outb ports */
    std::vector<size_t> config_data_values;
    for (const ConfigBitId config_bit : bitstream_manager.block_bits(config_block_id)) {
      config_data_values.push_back(bitstream_manager.bit_value(config_bit));
    }
    print_verilog_deposit_wire_constant_values(fp, config_data_port, config_data_values);

    /* Skip datab ports if specified */
    if (false == output_datab_bits) {
      continue;
    }

    BasicPort config_datab_port(bit_hierarchy_path + generate_configurable_memory_inverted_data_out_name(),
                                bitstream_manager.block_bits(config_block_id).size());


    std::vector<size_t> config_datab_values;
    for (const ConfigBitId config_bit : bitstream_manager.block_bits(config_block_id)) {
      config_datab_values.push_back(!bitstream_manager.bit_value(config_bit));
    }
    print_verilog_deposit_wire_constant_values(fp, config_datab_port, config_datab_values);
  }

  fp << "end" << std::endl;

  print_verilog_comment(fp, std::string("----- End deposit bitstream to configuration memories -----"));
}

/********************************************************************
 * Impose the bitstream on the configuration memories
 * We branch here for different simulators:
 * 1. iVerilog Icarus prefers using 'assign' syntax to force the values
 * 2. Mentor Modelsim prefers using '$deposit' syntax to do so
 *******************************************************************/
static 
void print_verilog_preconfig_top_module_load_bitstream(std::fstream &fp,
                                                       const ModuleManager &module_manager,
                                                       const ModuleId &top_module,
                                                       const CircuitLibrary& circuit_lib,
                                                       const CircuitModelId& mem_model,
                                                       const BitstreamManager &bitstream_manager,
                                                       const e_embedded_bitstream_hdl_type& embedded_bitstream_hdl_type) {

  /* Skip the datab port if there is only 1 output port in memory model
   * Currently, it assumes that the data output port is always defined while datab is optional
   * If we see only 1 port, we assume datab is not defined by default.
   * TODO: this switch could be smarter: it should identify if only data or datab
   * ports are defined.
   */
  bool output_datab_bits = true;
  if (1 == circuit_lib.model_ports_by_type(mem_model, CIRCUIT_MODEL_PORT_OUTPUT).size()) {
    output_datab_bits = false;
  }

  print_verilog_comment(fp, std::string("----- Begin load bitstream to configuration memories -----"));

  /* Use assign syntax for Icarus simulator */
  if (EMBEDDED_BITSTREAM_HDL_IVERILOG == embedded_bitstream_hdl_type) {
    print_verilog_preconfig_top_module_force_bitstream(fp, module_manager, top_module,
                                                       bitstream_manager,
                                                       output_datab_bits);
  /* Use deposit syntax for other simulators */
  } else if (EMBEDDED_BITSTREAM_HDL_MODELSIM == embedded_bitstream_hdl_type) {
    print_verilog_preconfig_top_module_deposit_bitstream(fp, module_manager, top_module,
                                                         bitstream_manager,
                                                         output_datab_bits);
  }

  print_verilog_comment(fp, std::string("----- End load bitstream to configuration memories -----"));
}

/********************************************************************
 * Top-level function to generate a Verilog module of
 * a pre-configured FPGA fabric.
 *
 *   Pre-configured FPGA fabric
 *                        +--------------------------------------------
 *                        |
 *                        |          FPGA fabric
 *                        |          +-------------------------------+
 *                        |          |                               |
 *                        |  0/1---->|FPGA global ports              |
 *                        |          |                               |
 *   benchmark_clock----->|--------->|FPGA_clock                     |
 *                        |          |                               |
 *   benchmark_inputs---->|--------->|FPGA mapped I/Os               |
 *                        |          |                               |
 *   benchmark_outputs<---|<---------|FPGA mapped I/Os               |
 *                        |          |                               |
 *                        |  0/1---->|FPGA unmapped I/Os             |
 *                        |          |                               |
 *   fabric_bitstream---->|--------->|Internal_configuration_ports   |
 *                        |          +-------------------------------+
 *                        |
 *                        +-------------------------------------------
 *
 * Note: we do NOT put this module in the module manager.
 * Because, it is not a standard module, where we force configuration signals
 * This module is a wrapper for the FPGA fabric to be compatible in
 * the port map of input benchmark.
 * It includes wires to force constant values to part of FPGA datapath I/Os
 * All these are hard to implement as a module in module manager
 *******************************************************************/
int print_verilog_preconfig_top_module(const ModuleManager &module_manager,
                                       const BitstreamManager &bitstream_manager,
                                       const ConfigProtocol &config_protocol,
                                       const CircuitLibrary &circuit_lib,
                                       const FabricGlobalPortInfo &global_ports,
                                       const AtomContext &atom_ctx,
                                       const PlacementContext &place_ctx,
                                       const PinConstraints& pin_constraints,
                                       const BusGroup& bus_group,
                                       const IoLocationMap &io_location_map,
                                       const VprNetlistAnnotation &netlist_annotation,
                                       const std::string &circuit_name,
                                       const std::string &verilog_fname,
                                       const VerilogTestbenchOption& options) {
  std::string timer_message = std::string("Write pre-configured FPGA top-level Verilog netlist for design '") + circuit_name + std::string("'");

  int status = CMD_EXEC_SUCCESS;

  /* Start time count */
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(verilog_fname, std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  check_file_stream(verilog_fname.c_str(), fp);

  /* Generate a brief description on the Verilog file*/
  std::string title = std::string("Verilog netlist for pre-configured FPGA fabric by design: ") + circuit_name;
  print_verilog_file_header(fp, title, options.time_stamp());

  print_verilog_default_net_type_declaration(fp,
                                             options.default_net_type());

  /* Print module declaration and ports */
  print_verilog_preconfig_top_module_ports(fp, circuit_name, atom_ctx, netlist_annotation, bus_group);

  /* Find the top_module */
  ModuleId top_module = module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  /* Print internal wires */
  print_verilog_preconfig_top_module_internal_wires(fp, module_manager, top_module);

  /* Instanciate FPGA top-level module */
  print_verilog_testbench_fpga_instance(fp, module_manager, top_module,
                                        std::string(FORMAL_VERIFICATION_TOP_MODULE_UUT_NAME),
                                        std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX),
                                        options.explicit_port_mapping());

  /* Find clock ports in benchmark */
  std::vector<std::string> benchmark_clock_port_names = find_atom_netlist_clock_port_names(atom_ctx.nlist, netlist_annotation);

  /* Connect FPGA top module global ports to constant or benchmark global signals! */
  status = print_verilog_preconfig_top_module_connect_global_ports(fp, module_manager, top_module,
                                                                   pin_constraints, global_ports,
                                                                   benchmark_clock_port_names);
  if (CMD_EXEC_FATAL_ERROR == status) {
    return status;
  }

  /* Connect I/Os to benchmark I/Os or constant driver */
  print_verilog_testbench_connect_fpga_ios(fp, module_manager, top_module,
                                           atom_ctx, place_ctx, io_location_map,
                                           netlist_annotation,
                                           bus_group,
                                           std::string(FORMAL_VERIFICATION_TOP_MODULE_PORT_POSTFIX),
                                           std::string(),
                                           std::string(),
                                           std::vector<std::string>(),
                                           (size_t)VERILOG_DEFAULT_SIGNAL_INIT_VALUE);

  /* Assign the SRAM model applied to the FPGA fabric */
  CircuitModelId sram_model = config_protocol.memory_model();  
  VTR_ASSERT(true == circuit_lib.valid_model_id(sram_model));

  /* Assign FPGA internal SRAM/Memory ports to bitstream values, only output when needed */
  print_verilog_preconfig_top_module_load_bitstream(fp, module_manager, top_module,
                                                    circuit_lib, sram_model, 
                                                    bitstream_manager,
                                                    options.embedded_bitstream_hdl_type());

  /* Add signal initialization: 
   * Bypass writing codes to files due to the autogenerated codes are very large.
   */
  if (true == options.include_signal_init()) {
    print_verilog_testbench_signal_initialization(fp,
                                                  std::string(FORMAL_VERIFICATION_TOP_MODULE_UUT_NAME),
                                                  circuit_lib,
                                                  module_manager,
                                                  top_module,
                                                  false);
  }

  /* Testbench ends*/
  print_verilog_module_end(fp, std::string(circuit_name) + std::string(FORMAL_VERIFICATION_TOP_MODULE_POSTFIX));

  /* Close the file stream */
  fp.close();

  return status;
}

} /* end namespace openfpga */
