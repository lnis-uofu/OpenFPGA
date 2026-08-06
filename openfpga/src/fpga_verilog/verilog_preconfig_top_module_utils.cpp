/********************************************************************
 * This file includes functions that are used to generate
 * a Verilog module of a pre-configured FPGA fabric
 *******************************************************************/
#include <fstream>

/* Headers from vtrutil library */
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_atom_netlist_utils.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_port.h"
#include "verilog_preconfig_top_module_utils.h"
#include "verilog_writer_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Print internal wires for the pre-configured FPGA top module
 * The internal wires are tailored for the ports of FPGA top module
 * which will be different in various configuration protocols
 *******************************************************************/
void print_verilog_preconfig_top_module_internal_wires(
  std::fstream &fp, const ModuleManager &module_manager,
  const ModuleId &top_module, const std::string &port_postfix,
  const bool &little_endian) {
  /* Validate the file stream */
  valid_file_stream(fp);

  /* Global ports of top-level module  */
  print_verilog_comment(fp,
                        std::string("----- Local wires for FPGA fabric -----"));
  for (const ModulePortId &module_port_id :
       module_manager.module_ports(top_module)) {
    BasicPort module_port =
      module_manager.module_port(top_module, module_port_id);
    /* Add a postfix to the internal wires to be different from other reserved
     * ports */
    module_port.set_name(module_port.get_name() + port_postfix);
    fp << generate_verilog_port(VERILOG_PORT_WIRE, module_port, true,
                                little_endian)
       << ";" << std::endl;
  }
  /* Add an empty line as a splitter */
  fp << std::endl;
}

/********************************************************************
 * Connect global ports of FPGA top module to constants except:
 * 1. operating clock, which should be wired to the clock port of
 * this pre-configured FPGA top module
 *******************************************************************/
int print_verilog_preconfig_top_module_connect_global_ports(
  std::fstream &fp, const ModuleManager &module_manager,
  const ModuleId &top_module, const PinConstraints &pin_constraints,
  const AtomContext &atom_ctx, const VprNetlistAnnotation &netlist_annotation,
  const FabricGlobalPortInfo &fabric_global_ports,
  const std::vector<std::string> &benchmark_clock_port_names,
  const std::string &port_postfix, const bool &little_endian) {
  /* Validate the file stream */
  valid_file_stream(fp);

  print_verilog_comment(
    fp,
    std::string("----- Begin Connect Global ports of FPGA top module -----"));

  for (const FabricGlobalPortId &global_port_id :
       fabric_global_ports.global_ports()) {
    ModulePortId module_global_port_id =
      fabric_global_ports.global_module_port(global_port_id);
    VTR_ASSERT(ModuleManager::MODULE_GLOBAL_PORT ==
               module_manager.port_type(top_module, module_global_port_id));
    BasicPort module_global_port =
      module_manager.module_port(top_module, module_global_port_id);
    /* Now, for operating clock port, we should wire it to the clock of
     * benchmark! */
    if ((true == fabric_global_ports.global_port_is_clock(global_port_id)) &&
        (false == fabric_global_ports.global_port_is_prog(global_port_id))) {
      /* Wiring to each pin of the global port: benchmark clock is always 1-bit
       */
      for (size_t pin_id = 0; pin_id < module_global_port.pins().size();
           ++pin_id) {
        BasicPort module_clock_pin(module_global_port.get_name() + port_postfix,
                                   module_global_port.pins()[pin_id],
                                   module_global_port.pins()[pin_id]);

        /* If the clock port name is in the pin constraints, we should wire it
         * to the constrained pin */
        std::string constrained_net_name = pin_constraints.pin_net(BasicPort(
          module_global_port.get_name(), module_global_port.pins()[pin_id],
          module_global_port.pins()[pin_id]));

        /* If constrained to an open net or there is no clock in the benchmark,
         * we assign it to a default value */
        if ((true == pin_constraints.unmapped_net(constrained_net_name)) ||
            (true == benchmark_clock_port_names.empty())) {
          std::vector<size_t> default_values(
            1, fabric_global_ports.global_port_default_value(global_port_id));
          print_verilog_wire_constant_values(fp, module_clock_pin,
                                             default_values, little_endian);
          continue;
        }

        std::string clock_name_to_connect;
        if (!pin_constraints.unconstrained_net(constrained_net_name)) {
          clock_name_to_connect = constrained_net_name;
        } else {
          /* Otherwise, we must have a clear one-to-one clock net
           * corresponding!!! */
          if (benchmark_clock_port_names.size() !=
              module_global_port.get_width()) {
            VTR_LOG_ERROR(
              "Unable to map %lu benchmark clocks to %lu clock pins of "
              "FPGA!\nRequire clear pin constraints!\n",
              benchmark_clock_port_names.size(),
              module_global_port.get_width());
            return CMD_EXEC_FATAL_ERROR;
          }
          clock_name_to_connect = benchmark_clock_port_names[pin_id];
        }
        /* The clock name must be a valid primary input. Otherwise, it could be
         * a signal generated by internal logics, e.g., clb */
        AtomBlockId atom_blk =
          atom_ctx.netlist().find_block(clock_name_to_connect);
        if ((AtomBlockType::INPAD != atom_ctx.netlist().block_type(atom_blk))) {
          VTR_LOG(
            "Global net '%s' is not a primary input of the netlist (which "
            "could a signal generated by internal logic). Will not wire it to "
            "any FPGA primary input pin\n",
            clock_name_to_connect.c_str());
          continue;
        }
        /* The block may be renamed as it contains special characters which
         * violate Verilog syntax */
        if (true == netlist_annotation.is_block_renamed(atom_blk)) {
          VTR_LOG(
            "Replace pin name '%s' with '%s' as it is renamed to comply "
            "verilog syntax\n",
            clock_name_to_connect.c_str(),
            netlist_annotation.block_name(atom_blk).c_str());
          clock_name_to_connect = netlist_annotation.block_name(atom_blk);
        }
        BasicPort benchmark_clock_pin(clock_name_to_connect, 1);
        print_verilog_wire_connection(fp, module_clock_pin, benchmark_clock_pin,
                                      false, little_endian);
      }
      /* Finish, go to the next */
      continue;
    }

    /* For other ports, give an default value */
    for (size_t pin_id = 0; pin_id < module_global_port.pins().size();
         ++pin_id) {
      BasicPort module_global_pin(module_global_port.get_name(),
                                  module_global_port.pins()[pin_id],
                                  module_global_port.pins()[pin_id]);

      /* If the global port name is in the pin constraints, we should wire it to
       * the constrained pin */
      std::string constrained_net_name =
        pin_constraints.pin_net(module_global_pin);

      module_global_pin.set_name(module_global_port.get_name() + port_postfix);

      /* - If constrained to a given net in the benchmark, we connect the global
       * pin to the net
       * - If constrained to an open net in the benchmark, we assign it to a
       * default value
       */
      if ((false == pin_constraints.unconstrained_net(constrained_net_name)) &&
          (false == pin_constraints.unmapped_net(constrained_net_name))) {
        /* The clock name must be a valid primary input. Otherwise, it could be
         * a signal generated by internal logics, e.g., clb */
        AtomBlockId atom_blk =
          atom_ctx.netlist().find_block(constrained_net_name);
        if ((AtomBlockType::INPAD != atom_ctx.netlist().block_type(atom_blk))) {
          VTR_LOG(
            "Global net '%s' is not a primary input of the netlist (which "
            "could a signal generated by internal logic). Will not wire it to "
            "any FPGA primary input pin\n",
            constrained_net_name.c_str());
          continue;
        }
        /* The block may be renamed as it contains special characters which
         * violate Verilog syntax */
        if (true == netlist_annotation.is_block_renamed(atom_blk)) {
          VTR_LOG(
            "Replace pin name '%s' with '%s' as it is renamed to comply "
            "verilog syntax\n",
            constrained_net_name.c_str(),
            netlist_annotation.block_name(atom_blk).c_str());
          constrained_net_name = netlist_annotation.block_name(atom_blk);
        }
        BasicPort benchmark_pin(constrained_net_name, 1);
        print_verilog_wire_connection(fp, module_global_pin, benchmark_pin,
                                      false, little_endian);
      } else {
        VTR_ASSERT_SAFE(std::string(PIN_CONSTRAINT_OPEN_NET) ==
                        constrained_net_name);
        std::vector<size_t> default_values(
          module_global_pin.get_width(),
          fabric_global_ports.global_port_default_value(global_port_id));
        /* For configuration done signals, we should enable them in
         * preconfigured wrapper */
        if (fabric_global_ports.global_port_is_config_enable(global_port_id)) {
          VTR_LOG(
            "Config-enable port '%s' is detected with default value '%ld'",
            module_global_pin.get_name().c_str(),
            fabric_global_ports.global_port_default_value(global_port_id));
          default_values.clear();
          default_values.resize(
            module_global_pin.get_width(),
            1 - fabric_global_ports.global_port_default_value(global_port_id));
        }
        print_verilog_wire_constant_values(fp, module_global_pin,
                                           default_values, little_endian);
      }
    }
  }

  print_verilog_comment(
    fp, std::string("----- End Connect Global ports of FPGA top module -----"));

  /* Add an empty line as a splitter */
  fp << std::endl;

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
