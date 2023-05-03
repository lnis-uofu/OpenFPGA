/********************************************************************
 * This file includes most utilized functions that are used to create
 * Verilog testbenches
 *
 * Note: please try to avoid using global variables in this file
 * so that we can make it free to use anywhere
 *******************************************************************/
#include <algorithm>
#include <iomanip>
#include <map>

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "fabric_global_port_info_utils.h"
#include "module_manager_utils.h"
#include "openfpga_atom_netlist_utils.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_port.h"
#include "verilog_constants.h"
#include "verilog_port_types.h"
#include "verilog_testbench_utils.h"
#include "verilog_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print an instance of the FPGA top-level module
 * When net_postfix is not empty, the instance net will contain a postfix like
 *   fpga fpga_core(.in(in_<postfix>),
                    .out(out_postfix>)
                   );
 *******************************************************************/
void print_verilog_testbench_fpga_instance(std::fstream& fp,
                                           const ModuleManager& module_manager,
                                           const ModuleId& top_module,
                                           const std::string& top_instance_name,
                                           const std::string& net_postfix,
                                           const bool& explicit_port_mapping) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Include defined top-level module */
  print_verilog_comment(
    fp, std::string("----- FPGA top-level module to be capsulated -----"));

  /* Create an empty port-to-port name mapping, because we use default names */
  std::map<std::string, BasicPort> port2port_name_map;
  if (!net_postfix.empty()) {
    for (const ModulePortId& module_port_id :
         module_manager.module_ports(top_module)) {
      BasicPort module_port =
        module_manager.module_port(top_module, module_port_id);
      BasicPort net_port = module_port;
      net_port.set_name(module_port.get_name() + net_postfix);
      port2port_name_map[module_port.get_name()] = net_port;
    }
  }

  /* Use explicit port mapping for a clean instanciation */
  print_verilog_module_instance(fp, module_manager, top_module,
                                top_instance_name, port2port_name_map,
                                explicit_port_mapping);

  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Instanciate the input benchmark module
 *******************************************************************/
void print_verilog_testbench_benchmark_instance(
  std::fstream& fp, const std::string& module_name,
  const std::string& instance_name,
  const std::string& module_input_port_postfix,
  const std::string& module_output_port_postfix,
  const std::string& input_port_postfix, const std::string& output_port_postfix,
  const std::vector<std::string>& clock_port_names, const AtomContext& atom_ctx,
  const VprNetlistAnnotation& netlist_annotation,
  const PinConstraints& pin_constraints, const BusGroup& bus_group,
  const bool& use_explicit_port_map) {
  /* Validate the file stream */
  valid_file_stream(fp);

  fp << "\t" << module_name << " " << instance_name << "(" << std::endl;

  /* Consider all the unique port names */
  std::vector<std::string> port_names;
  std::vector<AtomBlockType> port_types;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ((AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) &&
        (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk))) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }

    /* Note that VPR added a prefix "out_" or "out:" to the name of output
     * blocks We can remove this when specified through input argument
     */
    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
      block_name = remove_atom_block_name_prefix(block_name);
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
      if (port_names.end() ==
          std::find(port_names.begin(), port_names.end(),
                    bus_group.bus_port(bus_id).get_name())) {
        port_names.push_back(bus_group.bus_port(bus_id).get_name());
        port_types.push_back(atom_ctx.nlist.block_type(atom_blk));
      }
      continue;
    }

    /* Input port follows the logical block name while output port requires a
     * special postfix */
    port_names.push_back(block_name);
    port_types.push_back(atom_ctx.nlist.block_type(atom_blk));
  }

  /* Print out the instance with port mapping */
  size_t port_counter = 0;
  for (size_t iport = 0; iport < port_names.size(); ++iport) {
    /* The first port does not need a comma */
    if (0 < port_counter) {
      fp << "," << std::endl;
    }

    fp << "\t\t";
    if (AtomBlockType::INPAD == port_types[iport]) {
      if (true == use_explicit_port_map) {
        fp << "." << port_names[iport] << module_input_port_postfix << "(";
      }

      /* For bus ports, include a complete list of pins */
      BusGroupId bus_id = bus_group.find_bus(port_names[iport]);
      if (bus_id) {
        fp << "{";
        int pin_counter = 0;
        /* Include all the pins: If it follows little endian, reverse the pin
         * sequence */
        std::vector<BusPinId> bus_pins = bus_group.bus_pins(bus_id);
        if (!bus_group.is_big_endian(bus_id)) {
          std::reverse(bus_pins.begin(), bus_pins.end());
        }
        for (const BusPinId& pin : bus_pins) {
          if (0 < pin_counter) {
            fp << ", ";
          }
          /* Polarity of some input may have to be inverted, as defined in pin
           * constraints For example, the reset signal of the benchmark is
           * active low while the reset signal of the FPGA fabric is active high
           * (inside FPGA, the reset signal will be inverted) However, to ensure
           * correct stimuli to the benchmark, we have to invert the signal
           */
          if (PinConstraints::LOGIC_HIGH ==
              pin_constraints.net_default_value(bus_group.pin_name(pin))) {
            fp << "~";
          }

          fp << bus_group.pin_name(pin);

          /* For clock ports, skip postfix */
          if (clock_port_names.end() == std::find(clock_port_names.begin(),
                                                  clock_port_names.end(),
                                                  port_names[iport])) {
            fp << input_port_postfix;
          }

          pin_counter++;
        }
        fp << "}";
      } else {
        /* Polarity of some input may have to be inverted, as defined in pin
         * constraints For example, the reset signal of the benchmark is active
         * low while the reset signal of the FPGA fabric is active high (inside
         * FPGA, the reset signal will be inverted) However, to ensure correct
         * stimuli to the benchmark, we have to invert the signal
         */
        if (PinConstraints::LOGIC_HIGH ==
            pin_constraints.net_default_value(port_names[iport])) {
          fp << "~";
        }

        fp << port_names[iport];
        /* For clock ports, skip postfix */
        if (clock_port_names.end() == std::find(clock_port_names.begin(),
                                                clock_port_names.end(),
                                                port_names[iport])) {
          fp << input_port_postfix;
        }
      }

      if (true == use_explicit_port_map) {
        fp << ")";
      }
    } else {
      VTR_ASSERT_SAFE(AtomBlockType::OUTPAD == port_types[iport]);
      if (true == use_explicit_port_map) {
        fp << "." << port_names[iport] << module_output_port_postfix << "(";
      }

      /* For bus ports, include a complete list of pins */
      BusGroupId bus_id = bus_group.find_bus(port_names[iport]);

      if (bus_id) {
        fp << "{";
        int pin_counter = 0;
        /* Include all the pins: If it follows little endian, reverse the pin
         * sequence */
        std::vector<BusPinId> bus_pins = bus_group.bus_pins(bus_id);
        if (!bus_group.is_big_endian(bus_id)) {
          std::reverse(bus_pins.begin(), bus_pins.end());
        }
        for (const BusPinId& pin : bus_pins) {
          if (0 < pin_counter) {
            fp << ", ";
          }
          fp << bus_group.pin_name(pin) << output_port_postfix;
          pin_counter++;
        }
        fp << "}";
      } else {
        fp << port_names[iport] << output_port_postfix;
      }
      if (true == use_explicit_port_map) {
        fp << ")";
      }
    }

    /* Update the counter */
    port_counter++;
  }
  fp << "\n\t);" << std::endl;
}

/********************************************************************
 * This function adds stimuli to I/Os of FPGA fabric
 * 1. For mapped I/Os, this function will wire them to the input ports
 *    of the pre-configured FPGA top module
 * 2. For unmapped I/Os, this function will assign a constant value
 *    by default
 *******************************************************************/
void print_verilog_testbench_connect_fpga_ios(
  std::fstream& fp, const ModuleManager& module_manager,
  const ModuleId& top_module, const AtomContext& atom_ctx,
  const PlacementContext& place_ctx, const IoLocationMap& io_location_map,
  const VprNetlistAnnotation& netlist_annotation, const BusGroup& bus_group,
  const std::string& net_name_postfix,
  const std::string& io_input_port_name_postfix,
  const std::string& io_output_port_name_postfix,
  const std::vector<std::string>& clock_port_names,
  const size_t& unused_io_value) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Only mappable i/o ports can be considered */
  std::vector<ModulePortId> module_io_ports;
  for (const ModuleManager::e_module_port_type& module_io_port_type :
       MODULE_IO_PORT_TYPES) {
    for (const ModulePortId& gpio_port_id :
         module_manager.module_port_ids_by_type(top_module,
                                                module_io_port_type)) {
      /* Only care mappable I/O */
      if (false ==
          module_manager.port_is_mappable_io(top_module, gpio_port_id)) {
        continue;
      }
      module_io_ports.push_back(gpio_port_id);
    }
  }

  /* Keep tracking which I/Os have been used */
  std::map<ModulePortId, std::vector<bool>> io_used;
  for (const ModulePortId& module_io_port_id : module_io_ports) {
    const BasicPort& module_io_port =
      module_manager.module_port(top_module, module_io_port_id);
    io_used[module_io_port_id] =
      std::vector<bool>(module_io_port.get_width(), false);
  }

  /* Type mapping between VPR block and Module port */
  std::map<AtomBlockType, ModuleManager::e_module_port_type>
    atom_block_type_to_module_port_type;
  atom_block_type_to_module_port_type[AtomBlockType::INPAD] =
    ModuleManager::MODULE_GPIN_PORT;
  atom_block_type_to_module_port_type[AtomBlockType::OUTPAD] =
    ModuleManager::MODULE_GPOUT_PORT;

  /* See if this I/O should be wired to a benchmark input/output */
  /* Add signals from blif benchmark and short-wire them to FPGA I/O PADs
   * This brings convenience to checking functionality
   */
  print_verilog_comment(
    fp, std::string("----- Link BLIF Benchmark I/Os to FPGA I/Os -----"));

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ((AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) &&
        (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk))) {
      continue;
    }

    /* If there is a GPIO port, use it directly
     * Otherwise, should find a GPIN for INPAD
     *         or should find a GPOUT for OUTPAD
     */
    std::pair<ModulePortId, size_t> mapped_module_io_info =
      std::make_pair(ModulePortId::INVALID(), -1);
    for (const ModulePortId& module_io_port_id : module_io_ports) {
      const BasicPort& module_io_port =
        module_manager.module_port(top_module, module_io_port_id);

      /* Find the index of the mapped GPIO in top-level FPGA fabric */
      size_t temp_io_index = io_location_map.io_index(
        place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.x,
        place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.y,
        place_ctx.block_locs[atom_ctx.lookup.atom_clb(atom_blk)].loc.sub_tile,
        module_io_port.get_name());

      /* Bypass invalid index (not mapped to this GPIO port) */
      if (size_t(-1) == temp_io_index) {
        continue;
      }

      /* If the port is an GPIO port, just use it */
      if (ModuleManager::MODULE_GPIO_PORT ==
          module_manager.port_type(top_module, module_io_port_id)) {
        mapped_module_io_info =
          std::make_pair(module_io_port_id, temp_io_index);
        break;
      }

      /* If this is an INPAD, we can use an GPIN port (if available) */
      if (atom_block_type_to_module_port_type[atom_ctx.nlist.block_type(
            atom_blk)] ==
          module_manager.port_type(top_module, module_io_port_id)) {
        mapped_module_io_info =
          std::make_pair(module_io_port_id, temp_io_index);
        break;
      }
    }

    /* We must find a valid one */
    VTR_ASSERT(true == module_manager.valid_module_port_id(
                         top_module, mapped_module_io_info.first));
    VTR_ASSERT(size_t(-1) != mapped_module_io_info.second);

    /* Ensure that IO index is in range */
    BasicPort module_mapped_io_port =
      module_manager.module_port(top_module, mapped_module_io_info.first);
    size_t io_index = mapped_module_io_info.second;

    /* Set the port pin index */
    VTR_ASSERT(io_index < module_mapped_io_port.get_width());
    module_mapped_io_port.set_name(module_mapped_io_port.get_name() +
                                   net_name_postfix);
    module_mapped_io_port.set_width(io_index, io_index);

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    /* Note that VPR added a prefix to the name of output blocks
     * We can remove this when specified through input argument
     */
    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
      block_name = remove_atom_block_name_prefix(block_name);
    }

    /* Create the port for benchmark I/O, due to BLIF benchmark, each I/O always
     * has a size of 1 In addition, the input and output ports may have
     * different postfix in naming due to verification context! Here, we give
     * full customization on naming
     */
    BasicPort benchmark_io_port;

    /* If this benchmark pin belongs to any bus group, use the bus pin instead
     */
    BusGroupId bus_id = bus_group.find_pin_bus(block_name);
    BusPinId bus_pin_id = bus_group.find_pin(block_name);
    if (bus_id) {
      block_name = bus_group.bus_port(bus_id).get_name();
      VTR_ASSERT_SAFE(bus_pin_id);
      benchmark_io_port.set_width(bus_group.pin_index(bus_pin_id),
                                  bus_group.pin_index(bus_pin_id));
    } else {
      benchmark_io_port.set_width(1);
    }

    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      /* If the port is a clock, do not add a postfix */
      if (clock_port_names.end() != std::find(clock_port_names.begin(),
                                              clock_port_names.end(),
                                              block_name)) {
        benchmark_io_port.set_name(block_name);
      } else {
        benchmark_io_port.set_name(
          std::string(block_name + io_input_port_name_postfix));
      }
      print_verilog_comment(
        fp, std::string("----- Blif Benchmark input " + block_name +
                        " is mapped to FPGA IOPAD " +
                        module_mapped_io_port.get_name() + "[" +
                        std::to_string(io_index) + "] -----"));
      print_verilog_wire_connection(fp, module_mapped_io_port,
                                    benchmark_io_port, false);
    } else {
      VTR_ASSERT(AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk));
      benchmark_io_port.set_name(
        std::string(block_name + io_output_port_name_postfix));
      print_verilog_comment(
        fp, std::string("----- Blif Benchmark output " + block_name +
                        " is mapped to FPGA IOPAD " +
                        module_mapped_io_port.get_name() + "[" +
                        std::to_string(io_index) + "] -----"));
      print_verilog_wire_connection(fp, benchmark_io_port,
                                    module_mapped_io_port, false);
    }

    /* Mark this I/O has been used/wired */
    io_used[mapped_module_io_info.first][io_index] = true;

    /* Add an empty line as a splitter */
    fp << std::endl;
  }

  /* Wire the unused iopads to a constant */
  print_verilog_comment(
    fp, std::string("----- Wire unused FPGA I/Os to constants -----"));
  for (const ModulePortId& module_io_port_id : module_io_ports) {
    for (size_t io_index = 0; io_index < io_used[module_io_port_id].size();
         ++io_index) {
      /* Bypass used iopads */
      if (true == io_used[module_io_port_id][io_index]) {
        continue;
      }

      /* Bypass unused output pads */
      if (ModuleManager::MODULE_GPOUT_PORT ==
          module_manager.port_type(top_module, module_io_port_id)) {
        continue;
      }

      /* Wire to a contant */
      BasicPort module_unused_io_port =
        module_manager.module_port(top_module, module_io_port_id);
      /* Set the port pin index */
      module_unused_io_port.set_name(module_unused_io_port.get_name() +
                                     net_name_postfix);
      module_unused_io_port.set_width(io_index, io_index);

      std::vector<size_t> default_values(module_unused_io_port.get_width(),
                                         unused_io_value);
      print_verilog_wire_constant_values(fp, module_unused_io_port,
                                         default_values);
    }

    /* Add an empty line as a splitter */
    fp << std::endl;
  }
}

/********************************************************************
 * Print Verilog codes to set up a timeout for the simulation
 * and dump the waveform to VCD files
 *
 * Note that: these codes are tuned for Icarus simulator!!!
 *******************************************************************/
void print_verilog_timeout_and_vcd(
  std::fstream& fp, const std::string& module_name,
  const std::string& vcd_fname,
  const std::string& simulation_start_counter_name,
  const std::string& error_counter_name, const float& simulation_time,
  const bool& no_self_checking) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(
    fp, std::string("----- Begin output waveform to VCD file-------"));

  fp << "\tinitial begin" << std::endl;
  fp << "\t\t$dumpfile(\"" << vcd_fname << "\");" << std::endl;
  fp << "\t\t$dumpvars(1, " << module_name << ");" << std::endl;
  fp << "\tend" << std::endl;

  print_verilog_comment(
    fp, std::string("----- END output waveform to VCD file -------"));

  /* Add an empty line as splitter */
  fp << std::endl;

  BasicPort sim_start_port(simulation_start_counter_name, 1);

  fp << "initial begin" << std::endl;

  if (!no_self_checking) {
    fp << "\t" << generate_verilog_port(VERILOG_PORT_CONKT, sim_start_port)
       << " <= 1'b1;" << std::endl;
  }

  fp << "\t$timeformat(-9, 2, \"ns\", 20);" << std::endl;
  fp << "\t$display(\"Simulation start\");" << std::endl;
  print_verilog_comment(
    fp,
    std::string("----- Can be changed by the user for his/her need -------"));
  fp << "\t#" << std::setprecision(10) << simulation_time << std::endl;

  if (!no_self_checking) {
    fp << "\tif(" << error_counter_name << " == 0) begin" << std::endl;
    fp << "\t\t$display(\"Simulation Succeed\");" << std::endl;
    fp << "\tend else begin" << std::endl;
    fp << "\t\t$display(\"Simulation Failed with " << std::string("%d")
       << " error(s)\", " << error_counter_name << ");" << std::endl;
    fp << "\tend" << std::endl;
  } else {
    VTR_ASSERT_SAFE(no_self_checking);
    fp << "\t$display(\"Simulation Succeed\");" << std::endl;
  }

  fp << "\t$finish;" << std::endl;
  fp << "end" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Generate the clock port name to be used in this testbench
 *
 * Restrictions:
 * Assume this is a single clock benchmark
 *******************************************************************/
std::vector<BasicPort> generate_verilog_testbench_clock_port(
  const std::vector<std::string>& clock_port_names,
  const std::string& default_clock_name) {
  std::vector<BasicPort> clock_ports;
  if (0 == clock_port_names.size()) {
    clock_ports.push_back(BasicPort(default_clock_name, 1));
  } else {
    for (const std::string& clock_port_name : clock_port_names) {
      clock_ports.push_back(BasicPort(clock_port_name, 1));
    }
  }

  return clock_ports;
}

/********************************************************************
 * Print Verilog codes to check the equivalence of output vectors
 *
 * Restriction: this function only supports single clock benchmarks!
 *******************************************************************/
void print_verilog_testbench_check(
  std::fstream& fp, const std::string& simulation_start_counter_name,
  const std::string& benchmark_port_postfix,
  const std::string& fpga_port_postfix,
  const std::string& check_flag_port_postfix,
  const std::string& config_done_name, const std::string& error_counter_name,
  const AtomContext& atom_ctx, const VprNetlistAnnotation& netlist_annotation,
  const std::vector<std::string>& clock_port_names,
  const std::string& default_clock_name) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Add output autocheck */
  print_verilog_comment(
    fp, std::string("----- Begin checking output vectors -------"));

  std::vector<BasicPort> clock_ports =
    generate_verilog_testbench_clock_port(clock_port_names, default_clock_name);

  print_verilog_comment(fp,
                        std::string("----- Skip the first falling edge of "
                                    "clock, it is for initialization -------"));

  BasicPort sim_start_port(simulation_start_counter_name, 1);

  fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, sim_start_port) << ";"
     << std::endl;
  fp << std::endl;

  /* TODO: This is limitation when multiple clock signals exist
   * Ideally, all the input signals are generated by different clock edges,
   * depending which clock domain the signals belong to
   * Currently, as we lack the information, we only use the first clock signal
   */
  VTR_ASSERT(1 <= clock_ports.size());

  fp << "\talways@(negedge "
     << generate_verilog_port(VERILOG_PORT_CONKT, clock_ports[0]) << ") begin"
     << std::endl;
  fp << "\t\tif (1'b1 == "
     << generate_verilog_port(VERILOG_PORT_CONKT, sim_start_port) << ") begin"
     << std::endl;
  fp << "\t\t";
  print_verilog_register_connection(fp, sim_start_port, sim_start_port, true);
  fp << "\t\tend else " << std::endl;
  /* If there is a config done signal specified, consider it as a trigger on
   * checking */
  if (!config_done_name.empty()) {
    fp << "\t\t\tif (1'b1 == " << config_done_name << ") ";
  }
  fp << "begin" << std::endl;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ((AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) &&
        (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk))) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }

    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
      block_name = remove_atom_block_name_prefix(block_name);
      fp << "\t\t\tif(!(" << block_name << fpga_port_postfix;
      fp << " === " << block_name << benchmark_port_postfix;
      fp << ") && !(" << block_name << benchmark_port_postfix;
      fp << " === 1'bx)) begin" << std::endl;
      fp << "\t\t\t\t" << block_name << check_flag_port_postfix << " <= 1'b1;"
         << std::endl;
      fp << "\t\t\tend else begin" << std::endl;
      fp << "\t\t\t\t" << block_name << check_flag_port_postfix << "<= 1'b0;"
         << std::endl;
      fp << "\t\t\tend" << std::endl;
    }
  }
  fp << "\t\tend" << std::endl;
  fp << "\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Only care about output atom blocks ! */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    block_name = remove_atom_block_name_prefix(block_name);

    fp << "\talways@(posedge " << block_name << check_flag_port_postfix
       << ") begin" << std::endl;
    fp << "\t\tif(" << block_name << check_flag_port_postfix << ") begin"
       << std::endl;
    fp << "\t\t\t" << error_counter_name << " = " << error_counter_name
       << " + 1;" << std::endl;
    fp << "\t\t\t$display(\"Mismatch on " << block_name << fpga_port_postfix
       << " at time = " << std::string("%t") << "\", $realtime);" << std::endl;
    fp << "\t\tend" << std::endl;
    fp << "\tend" << std::endl;

    /* Add an empty line as splitter */
    fp << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Generate random stimulus for the clock port
 * This function is designed to drive the clock port of a benchmark module
 * If there is no clock port found, we will give a default clock name
 * In such case, this clock will not be wired to the benchmark module
 * but be only used as a synchronizer in verification
 *******************************************************************/
void print_verilog_testbench_clock_stimuli(
  std::fstream& fp, const PinConstraints& pin_constraints,
  const SimulationSetting& simulation_parameters,
  const std::vector<BasicPort>& clock_ports) {
  /* Validate the file stream */
  valid_file_stream(fp);

  for (const BasicPort& clock_port : clock_ports) {
    print_verilog_comment(fp, std::string("----- Clock '") +
                                clock_port.get_name() +
                                std::string("' Initialization -------"));

    /* Find the corresponding clock frequency from the simulation parameters */
    float clk_freq_to_use =
      (0.5 / simulation_parameters.default_operating_clock_frequency()) /
      VERILOG_SIM_TIMESCALE;
    /* Check pin constraints to see if this clock is constrained to a specific
     * pin If constrained,
     * - connect this clock to default values if it is set to be OPEN
     * - connect this clock to a specific clock source from simulation
     * settings!!!
     */
    VTR_ASSERT(1 == clock_port.get_width());
    for (const PinConstraintId& pin_constraint :
         pin_constraints.pin_constraints()) {
      if (clock_port.get_name() != pin_constraints.net(pin_constraint)) {
        continue;
      }
      /* Skip all the unrelated pin constraints */
      VTR_ASSERT(clock_port.get_name() == pin_constraints.net(pin_constraint));
      /* Try to find which clock source is considered in simulation settings for
       * this pin */
      for (const SimulationClockId& sim_clock_id :
           simulation_parameters.operating_clocks()) {
        if (pin_constraints.pin(pin_constraint) ==
            simulation_parameters.clock_port(sim_clock_id)) {
          clk_freq_to_use =
            (0.5 / simulation_parameters.clock_frequency(sim_clock_id)) /
            VERILOG_SIM_TIMESCALE;
        }
      }
    }

    fp << "\tinitial begin" << std::endl;
    /* Create clock stimuli */
    fp << "\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, clock_port)
       << " <= 1'b0;" << std::endl;
    fp << "\t\twhile(1) begin" << std::endl;
    fp << "\t\t\t#" << std::setprecision(10) << clk_freq_to_use << std::endl;
    fp << "\t\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, clock_port);
    fp << " <= !";
    fp << generate_verilog_port(VERILOG_PORT_CONKT, clock_port);
    fp << ";" << std::endl;
    fp << "\t\tend" << std::endl;

    fp << "\tend" << std::endl;

    /* Add an empty line as splitter */
    fp << std::endl;
  }
}

/********************************************************************
 * Generate random stimulus for the input ports (non-clock signals)
 * For clock signals, please use print_verilog_testbench_clock_stimuli
 *******************************************************************/
void print_verilog_testbench_random_stimuli(
  std::fstream& fp, const AtomContext& atom_ctx,
  const VprNetlistAnnotation& netlist_annotation,
  const ModuleManager& module_manager, const FabricGlobalPortInfo& global_ports,
  const PinConstraints& pin_constraints,
  const std::vector<std::string>& clock_port_names,
  const std::string& input_port_postfix,
  const std::string& check_flag_port_postfix,
  const std::vector<BasicPort>& clock_ports, const bool& no_self_checking) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(fp, std::string("----- Input Initialization -------"));

  fp << "\tinitial begin" << std::endl;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ((AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) &&
        (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk))) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
      block_name = remove_atom_block_name_prefix(block_name);
    }

    /* Bypass clock ports because their stimulus cannot be random */
    if (clock_port_names.end() != std::find(clock_port_names.begin(),
                                            clock_port_names.end(),
                                            block_name)) {
      continue;
    }

    /* Bypass any constained net that are mapped to a global port of the FPGA
     * fabric because their stimulus cannot be random
     */
    if (true ==
        port_is_fabric_global_reset_port(global_ports, module_manager,
                                         pin_constraints.net_pin(block_name))) {
      continue;
    }

    /* TODO: find the clock inputs will be initialized later */
    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      fp << "\t\t" << block_name + input_port_postfix << " <= 1'b0;"
         << std::endl;
    }
  }

  /* Set 0 to registers for checking flags */
  if (!no_self_checking) {
    /* Add an empty line as splitter */
    fp << std::endl;

    for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
      /* Bypass non-I/O atom blocks ! */
      if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
        continue;
      }

      /* The block may be renamed as it contains special characters which
       * violate Verilog syntax */
      std::string block_name = atom_ctx.nlist.block_name(atom_blk);
      if (true == netlist_annotation.is_block_renamed(atom_blk)) {
        block_name = netlist_annotation.block_name(atom_blk);
      }
      block_name = remove_atom_block_name_prefix(block_name);

      /* Each logical block assumes a single-width port */
      BasicPort output_port(std::string(block_name + check_flag_port_postfix),
                            1);
      fp << "\t\t" << generate_verilog_port(VERILOG_PORT_CONKT, output_port)
         << " <= 1'b0;" << std::endl;
    }
  }

  fp << "\tend" << std::endl;
  /* Finish initialization */

  /* Add an empty line as splitter */
  fp << std::endl;

  print_verilog_comment(fp, std::string("----- Input Stimulus -------"));
  /* TODO: This is limitation when multiple clock signals exist
   * Ideally, all the input signals are generated by different clock edges,
   * depending which clock domain the signals belong to
   * Currently, as we lack the information, we only use the first clock signal
   */
  VTR_ASSERT(1 <= clock_ports.size());
  fp << "\talways@(negedge "
     << generate_verilog_port(VERILOG_PORT_CONKT, clock_ports[0]) << ") begin"
     << std::endl;

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* Bypass non-I/O atom blocks ! */
    if ((AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) &&
        (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk))) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    if (AtomBlockType::OUTPAD == atom_ctx.nlist.block_type(atom_blk)) {
      block_name = remove_atom_block_name_prefix(block_name);
    }

    /* Bypass clock ports because their stimulus cannot be random */
    if (clock_port_names.end() != std::find(clock_port_names.begin(),
                                            clock_port_names.end(),
                                            block_name)) {
      continue;
    }

    /* Bypass any constained net that are mapped to a global port of the FPGA
     * fabric because their stimulus cannot be random
     */
    if (true ==
        port_is_fabric_global_reset_port(global_ports, module_manager,
                                         pin_constraints.net_pin(block_name))) {
      continue;
    }

    /* TODO: find the clock inputs will be initialized later */
    if (AtomBlockType::INPAD == atom_ctx.nlist.block_type(atom_blk)) {
      fp << "\t\t" << block_name + input_port_postfix << " <= $random;"
         << std::endl;
    }
  }

  fp << "\tend" << std::endl;

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print Verilog declaration of shared ports appear in testbenches
 * which are
 * 1. the shared input ports (registers) to drive both
 *    FPGA fabric and benchmark instance
 * 2. the output ports (wires) for both FPGA fabric and benchmark instance
 * 3. the checking flag ports to evaluate if outputs matches under the
 *    same input vectors
 *******************************************************************/
void print_verilog_testbench_shared_ports(
  std::fstream& fp, const ModuleManager& module_manager,
  const FabricGlobalPortInfo& global_ports,
  const PinConstraints& pin_constraints, const AtomContext& atom_ctx,
  const VprNetlistAnnotation& netlist_annotation,
  const std::vector<std::string>& clock_port_names,
  const std::string& shared_input_port_postfix,
  const std::string& benchmark_output_port_postfix,
  const std::string& fpga_output_port_postfix,
  const std::string& check_flag_port_postfix, const bool& no_self_checking) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Instantiate register for inputs stimulis */
  print_verilog_comment(fp, std::string("----- Shared inputs -------"));
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are input I/Os */
    if (AtomBlockType::INPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }

    /* TODO: Skip clocks because they are handled in another function */
    if (clock_port_names.end() != std::find(clock_port_names.begin(),
                                            clock_port_names.end(),
                                            block_name)) {
      continue;
    }

    /* Each logical block assumes a single-width port */
    BasicPort input_port(block_name + shared_input_port_postfix, 1);
    /* For global ports, use wires; otherwise, use registers*/
    if (false ==
        port_is_fabric_global_reset_port(global_ports, module_manager,
                                         pin_constraints.net_pin(block_name))) {
      fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, input_port) << ";"
         << std::endl;
    } else {
      fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, input_port) << ";"
         << std::endl;
    }
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate wires for FPGA fabric outputs */
  print_verilog_comment(fp, std::string("----- FPGA fabric outputs -------"));

  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are output I/Os */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    block_name = remove_atom_block_name_prefix(block_name);

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(block_name + fpga_output_port_postfix),
                          1);
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, output_port) << ";"
       << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  if (no_self_checking) {
    return;
  }

  /* Instantiate wire for benchmark output */
  print_verilog_comment(fp, std::string("----- Benchmark outputs -------"));
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are output I/Os */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    block_name = remove_atom_block_name_prefix(block_name);

    /* Each logical block assumes a single-width port */
    BasicPort output_port(
      std::string(block_name + benchmark_output_port_postfix), 1);
    fp << "\t" << generate_verilog_port(VERILOG_PORT_WIRE, output_port) << ";"
       << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;

  /* Instantiate register for output comparison */
  print_verilog_comment(
    fp, std::string("----- Output vectors checking flags -------"));
  for (const AtomBlockId& atom_blk : atom_ctx.nlist.blocks()) {
    /* We care only those logic blocks which are output I/Os */
    if (AtomBlockType::OUTPAD != atom_ctx.nlist.block_type(atom_blk)) {
      continue;
    }

    /* The block may be renamed as it contains special characters which violate
     * Verilog syntax */
    std::string block_name = atom_ctx.nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    block_name = remove_atom_block_name_prefix(block_name);

    /* Each logical block assumes a single-width port */
    BasicPort output_port(std::string(block_name + check_flag_port_postfix), 1);
    fp << "\t" << generate_verilog_port(VERILOG_PORT_REG, output_port) << ";"
       << std::endl;
  }

  /* Add an empty line as splitter */
  fp << std::endl;
}

/********************************************************************
 * Print signal initialization which
 * deposit initial values for the input ports of primitive circuit models
 * This function recusively walk through from the parent_module
 * until reaching a primitive module that matches the circuit_model name
 * The hierarchy walkthrough collects the full paths for the primitive modules
 * in the graph of modules
 *******************************************************************/
static void rec_print_verilog_testbench_primitive_module_signal_initialization(
  std::fstream& fp, const std::string& hie_path,
  const CircuitLibrary& circuit_lib, const CircuitModelId& circuit_model,
  const std::vector<CircuitPortId>& circuit_input_ports,
  const ModuleManager& module_manager, const ModuleId& parent_module,
  const ModuleId& primitive_module, const bool& deposit_random_values) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Return if the current module has no children */
  if (0 == module_manager.child_modules(parent_module).size()) {
    return;
  }

  /* Go through child modules */
  for (const ModuleId& child_module :
       module_manager.child_modules(parent_module)) {
    /* If the child module is not the primitive module,
     * we recursively visit the child module
     */
    for (const size_t& child_instance :
         module_manager.child_module_instances(parent_module, child_module)) {
      std::string instance_name = module_manager.instance_name(
        parent_module, child_module, child_instance);
      /* Use default instanec name if not assigned */
      if (true == instance_name.empty()) {
        instance_name = generate_instance_name(
          module_manager.module_name(child_module), child_instance);
      }

      std::string child_hie_path = hie_path + "." + instance_name;

      if (child_module != primitive_module) {
        rec_print_verilog_testbench_primitive_module_signal_initialization(
          fp, child_hie_path, circuit_lib, circuit_model, circuit_input_ports,
          module_manager, child_module, primitive_module,
          deposit_random_values);
      } else {
        /* If the child module is the primitive module,
         * we output the signal initialization codes for the input ports
         */
        VTR_ASSERT_SAFE(child_module == primitive_module);

        print_verilog_comment(
          fp, std::string("------ BEGIN driver initialization -----"));
        fp << "\tinitial begin" << std::endl;

        for (const auto& input_port : circuit_input_ports) {
          /* Only for formal verification: deposite a zero signal values */
          /* Initialize each input port */
          BasicPort input_port_info(circuit_lib.port_lib_name(input_port),
                                    circuit_lib.port_size(input_port));
          input_port_info.set_origin_port_width(input_port_info.get_width());
          fp << "\t\t$deposit(";
          fp << child_hie_path << ".";
          fp << generate_verilog_port(VERILOG_PORT_CONKT, input_port_info,
                                      false);

          if (!deposit_random_values) {
            fp << ", " << circuit_lib.port_size(input_port) << "'b"
               << std::string(circuit_lib.port_size(input_port), '0');
            fp << ");" << std::endl;
          } else {
            VTR_ASSERT_SAFE(deposit_random_values);
            fp << ", $random % 2 ? 1'b1 : 1'b0);" << std::endl;
          }
        }

        fp << "\tend" << std::endl;
        print_verilog_comment(
          fp, std::string("------ END driver initialization -----"));
      }
    }
  }
}

/********************************************************************
 * Print signal initialization for Verilog testbenches
 * which aim to deposit initial values for the input ports of primitive circuit
 *models:
 * - Passgate
 * - Logic gates (ONLY for MUX2)
 *******************************************************************/
void print_verilog_testbench_signal_initialization(
  std::fstream& fp, const std::string& top_instance_name,
  const CircuitLibrary& circuit_lib, const ModuleManager& module_manager,
  const ModuleId& top_module, const bool& deposit_random_values) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Collect circuit models that need signal initialization */
  std::vector<CircuitModelId> signal_init_circuit_models;

  /* Collect the input ports that require signal initialization */
  std::map<CircuitModelId, std::vector<CircuitPortId>>
    signal_init_circuit_ports;

  for (const CircuitModelId& model :
       circuit_lib.models_by_type(CIRCUIT_MODEL_PASSGATE)) {
    signal_init_circuit_models.push_back(model);
    /* Only 1 input requires signal initialization,
     * which is the first port, i.e., the datapath inputs
     */
    std::vector<CircuitPortId> input_ports =
      circuit_lib.model_input_ports(model);
    VTR_ASSERT(0 < input_ports.size());
    signal_init_circuit_ports[model].push_back(input_ports[0]);
  }

  for (const CircuitModelId& model :
       circuit_lib.models_by_type(CIRCUIT_MODEL_GATE)) {
    if (CIRCUIT_MODEL_GATE_MUX2 == circuit_lib.gate_type(model)) {
      signal_init_circuit_models.push_back(model);
      /* Only 2 input requires signal initialization,
       * which is the first two port, i.e., the datapath inputs
       */
      std::vector<CircuitPortId> input_ports =
        circuit_lib.model_input_ports(model);
      VTR_ASSERT(1 < input_ports.size());
      signal_init_circuit_ports[model].push_back(input_ports[0]);
      signal_init_circuit_ports[model].push_back(input_ports[1]);
    }
  }

  /* If there is no circuit model in the list, return directly */
  if (signal_init_circuit_models.empty()) {
    return;
  }

  /* Add signal initialization Verilog codes */
  fp << std::endl;
  for (const CircuitModelId& signal_init_circuit_model :
       signal_init_circuit_models) {
    /* Find the module id corresponding to the circuit model from module graph
     */
    ModuleId primitive_module = module_manager.find_module(
      circuit_lib.model_name(signal_init_circuit_model));
    VTR_ASSERT(true == module_manager.valid_module_id(primitive_module));

    /* Find all the instances created by the circuit model across the fabric*/
    rec_print_verilog_testbench_primitive_module_signal_initialization(
      fp, top_instance_name, circuit_lib, signal_init_circuit_model,
      signal_init_circuit_ports.at(signal_init_circuit_model), module_manager,
      top_module, primitive_module, deposit_random_values);
  }
}

} /* end namespace openfpga */
