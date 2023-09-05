#ifndef OPENFPGA_VERILOG_TEMPLATE_H
#define OPENFPGA_VERILOG_TEMPLATE_H

/********************************************************************
 * This file includes functions to compress the hierachy of routing architecture
 *******************************************************************/
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "openfpga_scale.h"
#include "read_xml_bus_group.h"
#include "read_xml_pin_constraints.h"
#include "verilog_api.h"
#include "verilog_mock_fpga_wrapper.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A wrapper function to call the fabric Verilog generator of FPGA-Verilog
 *******************************************************************/
template <class T>
int write_fabric_verilog_template(T& openfpga_ctx, const Command& cmd,
                                  const CommandContext& cmd_context) {
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_explicit_port_mapping =
    cmd.option("explicit_port_mapping");
  CommandOptionId opt_include_timing = cmd.option("include_timing");
  CommandOptionId opt_print_user_defined_template =
    cmd.option("print_user_defined_template");
  CommandOptionId opt_default_net_type = cmd.option("default_net_type");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_use_relative_path = cmd.option("use_relative_path");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-Verilog Keep it independent from any other outside data structures
   */
  FabricVerilogOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_explicit_port_mapping(
    cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_include_timing(
    cmd_context.option_enable(cmd, opt_include_timing));
  options.set_time_stamp(!cmd_context.option_enable(cmd, opt_no_time_stamp));
  options.set_use_relative_path(
    cmd_context.option_enable(cmd, opt_use_relative_path));
  options.set_print_user_defined_template(
    cmd_context.option_enable(cmd, opt_print_user_defined_template));
  if (true == cmd_context.option_enable(cmd, opt_default_net_type)) {
    options.set_default_net_type(
      cmd_context.option_value(cmd, opt_default_net_type));
  }
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_compress_routing(openfpga_ctx.flow_manager().compress_routing());

  return fpga_fabric_verilog(
    openfpga_ctx.mutable_module_graph(),
    openfpga_ctx.mutable_verilog_netlists(),
    openfpga_ctx.blwl_shift_register_banks(), openfpga_ctx.arch().circuit_lib,
    openfpga_ctx.mux_lib(), openfpga_ctx.decoder_lib(), g_vpr_ctx.device(),
    openfpga_ctx.vpr_device_annotation(), openfpga_ctx.device_rr_gsb(),
    openfpga_ctx.fabric_tile(), options);
}

/********************************************************************
 * A wrapper function to call the full testbench generator of FPGA-Verilog
 *******************************************************************/
template <class T>
int write_full_testbench_template(const T& openfpga_ctx, const Command& cmd,
                                  const CommandContext& cmd_context) {
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_bitstream = cmd.option("bitstream");
  CommandOptionId opt_dut_module = cmd.option("dut_module");
  CommandOptionId opt_fabric_netlist = cmd.option("fabric_netlist_file_path");
  CommandOptionId opt_pcf = cmd.option("pin_constraints_file");
  CommandOptionId opt_bgf = cmd.option("bus_group_file");
  CommandOptionId opt_reference_benchmark =
    cmd.option("reference_benchmark_file_path");
  CommandOptionId opt_fast_configuration = cmd.option("fast_configuration");
  CommandOptionId opt_explicit_port_mapping =
    cmd.option("explicit_port_mapping");
  CommandOptionId opt_default_net_type = cmd.option("default_net_type");
  CommandOptionId opt_include_signal_init = cmd.option("include_signal_init");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_use_relative_path = cmd.option("use_relative_path");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-Verilog Keep it independent from any other outside data structures
   */
  VerilogTestbenchOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_fabric_netlist_file_path(
    cmd_context.option_value(cmd, opt_fabric_netlist));
  options.set_reference_benchmark_file_path(
    cmd_context.option_value(cmd, opt_reference_benchmark));
  options.set_fast_configuration(
    cmd_context.option_enable(cmd, opt_fast_configuration));
  options.set_explicit_port_mapping(
    cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_time_stamp(!cmd_context.option_enable(cmd, opt_no_time_stamp));
  options.set_use_relative_path(
    cmd_context.option_enable(cmd, opt_use_relative_path));
  options.set_print_top_testbench(true);
  options.set_include_signal_init(
    cmd_context.option_enable(cmd, opt_include_signal_init));
  if (true == cmd_context.option_enable(cmd, opt_default_net_type)) {
    options.set_default_net_type(
      cmd_context.option_value(cmd, opt_default_net_type));
  }

  if (true == cmd_context.option_enable(cmd, opt_dut_module)) {
    options.set_dut_module(cmd_context.option_value(cmd, opt_dut_module));
  }

  /* If pin constraints are enabled by command options, read the file */
  PinConstraints pin_constraints;
  if (true == cmd_context.option_enable(cmd, opt_pcf)) {
    pin_constraints =
      read_xml_pin_constraints(cmd_context.option_value(cmd, opt_pcf).c_str());
  }

  /* If bug group file are enabled by command options, read the file */
  BusGroup bus_group;
  if (true == cmd_context.option_enable(cmd, opt_bgf)) {
    bus_group =
      read_xml_bus_group(cmd_context.option_value(cmd, opt_bgf).c_str());
  }

  return fpga_verilog_full_testbench(
    openfpga_ctx.module_graph(), openfpga_ctx.bitstream_manager(),
    openfpga_ctx.fabric_bitstream(), openfpga_ctx.blwl_shift_register_banks(),
    g_vpr_ctx.atom(), g_vpr_ctx.placement(), pin_constraints, bus_group,
    cmd_context.option_value(cmd, opt_bitstream),
    openfpga_ctx.io_location_map(), openfpga_ctx.io_name_map(),
    openfpga_ctx.fabric_global_port_info(),
    openfpga_ctx.vpr_netlist_annotation(), openfpga_ctx.arch().circuit_lib,
    openfpga_ctx.simulation_setting(), openfpga_ctx.arch().config_protocol,
    options);
}

/********************************************************************
 * A wrapper function to call the preconfigured wrapper generator of
 *FPGA-Verilog
 *******************************************************************/
template <class T>
int write_preconfigured_fabric_wrapper_template(
  const T& openfpga_ctx, const Command& cmd,
  const CommandContext& cmd_context) {
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_dut_module = cmd.option("dut_module");
  CommandOptionId opt_fabric_netlist = cmd.option("fabric_netlist_file_path");
  CommandOptionId opt_pcf = cmd.option("pin_constraints_file");
  CommandOptionId opt_bgf = cmd.option("bus_group_file");
  CommandOptionId opt_explicit_port_mapping =
    cmd.option("explicit_port_mapping");
  CommandOptionId opt_default_net_type = cmd.option("default_net_type");
  CommandOptionId opt_include_signal_init = cmd.option("include_signal_init");
  CommandOptionId opt_embed_bitstream = cmd.option("embed_bitstream");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-Verilog Keep it independent from any other outside data structures
   */
  VerilogTestbenchOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_fabric_netlist_file_path(
    cmd_context.option_value(cmd, opt_fabric_netlist));
  options.set_explicit_port_mapping(
    cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_time_stamp(!cmd_context.option_enable(cmd, opt_no_time_stamp));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_include_signal_init(
    cmd_context.option_enable(cmd, opt_include_signal_init));
  options.set_print_formal_verification_top_netlist(true);

  if (true == cmd_context.option_enable(cmd, opt_dut_module)) {
    options.set_dut_module(cmd_context.option_value(cmd, opt_dut_module));
  }

  if (true == cmd_context.option_enable(cmd, opt_default_net_type)) {
    options.set_default_net_type(
      cmd_context.option_value(cmd, opt_default_net_type));
  }

  if (true == cmd_context.option_enable(cmd, opt_embed_bitstream)) {
    options.set_embedded_bitstream_hdl_type(
      cmd_context.option_value(cmd, opt_embed_bitstream));
  }

  /* If pin constraints are enabled by command options, read the file */
  PinConstraints pin_constraints;
  if (true == cmd_context.option_enable(cmd, opt_pcf)) {
    pin_constraints =
      read_xml_pin_constraints(cmd_context.option_value(cmd, opt_pcf).c_str());
  }

  /* If bug group file are enabled by command options, read the file */
  BusGroup bus_group;
  if (true == cmd_context.option_enable(cmd, opt_bgf)) {
    bus_group =
      read_xml_bus_group(cmd_context.option_value(cmd, opt_bgf).c_str());
  }

  return fpga_verilog_preconfigured_fabric_wrapper(
    openfpga_ctx.module_graph(), openfpga_ctx.bitstream_manager(),
    g_vpr_ctx.atom(), g_vpr_ctx.placement(), pin_constraints, bus_group,
    openfpga_ctx.io_location_map(), openfpga_ctx.io_name_map(),
    openfpga_ctx.fabric_global_port_info(),
    openfpga_ctx.vpr_netlist_annotation(), openfpga_ctx.arch().circuit_lib,
    openfpga_ctx.arch().config_protocol, options);
}

/********************************************************************
 * A wrapper function to call the mock fpga wrapper generator of
 *FPGA-Verilog
 *******************************************************************/
template <class T>
int write_mock_fpga_wrapper_template(const T& openfpga_ctx, const Command& cmd,
                                     const CommandContext& cmd_context) {
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_top_module = cmd.option("top_module");
  CommandOptionId opt_pcf = cmd.option("pin_constraints_file");
  CommandOptionId opt_bgf = cmd.option("bus_group_file");
  CommandOptionId opt_explicit_port_mapping =
    cmd.option("explicit_port_mapping");
  CommandOptionId opt_use_relative_path = cmd.option("use_relative_path");
  CommandOptionId opt_default_net_type = cmd.option("default_net_type");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-Verilog Keep it independent from any other outside data structures
   */
  VerilogTestbenchOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_explicit_port_mapping(
    cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_use_relative_path(
    cmd_context.option_enable(cmd, opt_use_relative_path));
  options.set_time_stamp(!cmd_context.option_enable(cmd, opt_no_time_stamp));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));

  if (true == cmd_context.option_enable(cmd, opt_top_module)) {
    options.set_dut_module(cmd_context.option_value(cmd, opt_top_module));
  }

  if (true == cmd_context.option_enable(cmd, opt_default_net_type)) {
    options.set_default_net_type(
      cmd_context.option_value(cmd, opt_default_net_type));
  }

  /* If pin constraints are enabled by command options, read the file */
  PinConstraints pin_constraints;
  if (true == cmd_context.option_enable(cmd, opt_pcf)) {
    pin_constraints =
      read_xml_pin_constraints(cmd_context.option_value(cmd, opt_pcf).c_str());
  }

  /* If bug group file are enabled by command options, read the file */
  BusGroup bus_group;
  if (true == cmd_context.option_enable(cmd, opt_bgf)) {
    bus_group =
      read_xml_bus_group(cmd_context.option_value(cmd, opt_bgf).c_str());
  }

  return fpga_verilog_mock_fpga_wrapper(
    openfpga_ctx.module_graph(), g_vpr_ctx.atom(), g_vpr_ctx.placement(),
    pin_constraints, bus_group, openfpga_ctx.io_location_map(),
    openfpga_ctx.io_name_map(), openfpga_ctx.fabric_global_port_info(),
    openfpga_ctx.vpr_netlist_annotation(), options);
}

/********************************************************************
 * A wrapper function to call the preconfigured testbench generator of
 *FPGA-Verilog
 *******************************************************************/
template <class T>
int write_preconfigured_testbench_template(const T& openfpga_ctx,
                                           const Command& cmd,
                                           const CommandContext& cmd_context) {
  CommandOptionId opt_output_dir = cmd.option("file");
  CommandOptionId opt_pcf = cmd.option("pin_constraints_file");
  CommandOptionId opt_bgf = cmd.option("bus_group_file");
  CommandOptionId opt_fabric_netlist = cmd.option("fabric_netlist_file_path");
  CommandOptionId opt_reference_benchmark =
    cmd.option("reference_benchmark_file_path");
  CommandOptionId opt_explicit_port_mapping =
    cmd.option("explicit_port_mapping");
  CommandOptionId opt_default_net_type = cmd.option("default_net_type");
  CommandOptionId opt_no_time_stamp = cmd.option("no_time_stamp");
  CommandOptionId opt_use_relative_path = cmd.option("use_relative_path");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-Verilog Keep it independent from any other outside data structures
   */
  VerilogTestbenchOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_output_dir));
  options.set_fabric_netlist_file_path(
    cmd_context.option_value(cmd, opt_fabric_netlist));
  options.set_reference_benchmark_file_path(
    cmd_context.option_value(cmd, opt_reference_benchmark));
  options.set_explicit_port_mapping(
    cmd_context.option_enable(cmd, opt_explicit_port_mapping));
  options.set_time_stamp(!cmd_context.option_enable(cmd, opt_no_time_stamp));
  options.set_use_relative_path(
    cmd_context.option_enable(cmd, opt_use_relative_path));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_print_preconfig_top_testbench(true);
  if (true == cmd_context.option_enable(cmd, opt_default_net_type)) {
    options.set_default_net_type(
      cmd_context.option_value(cmd, opt_default_net_type));
  }

  /* If pin constraints are enabled by command options, read the file */
  PinConstraints pin_constraints;
  if (true == cmd_context.option_enable(cmd, opt_pcf)) {
    pin_constraints =
      read_xml_pin_constraints(cmd_context.option_value(cmd, opt_pcf).c_str());
  }

  /* If bug group file are enabled by command options, read the file */
  BusGroup bus_group;
  if (true == cmd_context.option_enable(cmd, opt_bgf)) {
    bus_group =
      read_xml_bus_group(cmd_context.option_value(cmd, opt_bgf).c_str());
  }

  return fpga_verilog_preconfigured_testbench(
    openfpga_ctx.module_graph(), g_vpr_ctx.atom(), pin_constraints, bus_group,
    openfpga_ctx.fabric_global_port_info(),
    openfpga_ctx.vpr_netlist_annotation(), openfpga_ctx.simulation_setting(),
    options);
}

/********************************************************************
 * A wrapper function to call the simulation task information generator of
 *FPGA-Verilog
 *******************************************************************/
template <class T>
int write_simulation_task_info_template(const T& openfpga_ctx,
                                        const Command& cmd,
                                        const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  CommandOptionId opt_hdl_dir = cmd.option("hdl_dir");
  CommandOptionId opt_reference_benchmark =
    cmd.option("reference_benchmark_file_path");
  CommandOptionId opt_tb_type = cmd.option("testbench_type");
  CommandOptionId opt_time_unit = cmd.option("time_unit");
  CommandOptionId opt_verbose = cmd.option("verbose");

  /* This is an intermediate data structure which is designed to modularize the
   * FPGA-Verilog Keep it independent from any other outside data structures
   */
  VerilogTestbenchOption options;
  options.set_output_directory(cmd_context.option_value(cmd, opt_hdl_dir));
  options.set_reference_benchmark_file_path(
    cmd_context.option_value(cmd, opt_reference_benchmark));
  options.set_verbose_output(cmd_context.option_enable(cmd, opt_verbose));
  options.set_print_simulation_ini(cmd_context.option_value(cmd, opt_file));

  if (true == cmd_context.option_enable(cmd, opt_time_unit)) {
    options.set_time_unit(
      string_to_time_unit(cmd_context.option_value(cmd, opt_time_unit)));
  }

  /* Identify testbench type */
  std::string full_tb_tag("full_testbench");
  std::string preconfig_tb_tag("preconfigured_testbench");
  if (true == cmd_context.option_enable(cmd, opt_tb_type)) {
    if (std::string("preconfigured_testbench") ==
        cmd_context.option_value(cmd, opt_tb_type)) {
      options.set_print_preconfig_top_testbench(true);
    } else if (std::string("full_testbench") ==
               cmd_context.option_value(cmd, opt_tb_type)) {
      options.set_print_preconfig_top_testbench(false);
      options.set_print_top_testbench(true);
    } else {
      /* Invalid option, error out */
      VTR_LOG_ERROR(
        "Invalid option value for testbench type: '%s'! Should be either '%s' "
        "or '%s'\n",
        cmd_context.option_value(cmd, opt_tb_type).c_str(), full_tb_tag.c_str(),
        preconfig_tb_tag.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  } else {
    /* Deposit default type which is the preconfigured testbench */
    options.set_print_preconfig_top_testbench(true);
  }

  return fpga_verilog_simulation_task_info(
    openfpga_ctx.module_graph(), openfpga_ctx.bitstream_manager(),
    g_vpr_ctx.atom(), g_vpr_ctx.placement(), openfpga_ctx.io_location_map(),
    openfpga_ctx.simulation_setting(), openfpga_ctx.arch().config_protocol,
    options);
}

} /* end namespace openfpga */

#endif
