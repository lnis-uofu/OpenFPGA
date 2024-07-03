#ifndef OPENFPGA_READ_ARCH_TEMPLATE_H
#define OPENFPGA_READ_ARCH_TEMPLATE_H
/********************************************************************
 * This file includes functions to read an OpenFPGA architecture file
 * which are built on the libarchopenfpga library
 *******************************************************************/
#include "check_circuit_library.h"
#include "check_config_protocol.h"
#include "check_tile_annotation.h"
#include "circuit_library_utils.h"
#include "clock_network_utils.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "globals.h"
#include "read_xml_clock_network.h"
#include "read_xml_openfpga_arch.h"
#include "vtr_log.h"
#include "write_xml_clock_network.h"
#include "write_xml_openfpga_arch.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to read an OpenFPGA architecture file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the architecture
 * file provided by users
 *******************************************************************/
template <class T>
int read_openfpga_arch_template(T& openfpga_context, const Command& cmd,
                                const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Reading XML architecture '%s'...\n", arch_file_name.c_str());
  openfpga_context.mutable_arch() =
    read_xml_openfpga_arch(arch_file_name.c_str());

  /* Check the architecture:
   * 1. Circuit library
   * 2. Tile annotation
   * 3. Technology library (TODO)
   * 4. Simulation settings (TODO)
   */
  if (false == check_circuit_library(openfpga_context.arch().circuit_lib)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  if (false == check_config_protocol(openfpga_context.arch().config_protocol,
                                     openfpga_context.arch().circuit_lib)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  if (false == check_tile_annotation(openfpga_context.arch().tile_annotations,
                                     openfpga_context.arch().circuit_lib,
                                     g_vpr_ctx.device().physical_tile_types)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A function to write an OpenFPGA architecture file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the architecture
 * file provided by users
 *******************************************************************/
template <class T>
int write_openfpga_arch_template(const T& openfpga_context, const Command& cmd,
                                 const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Writing XML architecture to '%s'...\n", arch_file_name.c_str());
  write_xml_openfpga_arch(arch_file_name.c_str(), openfpga_context.arch());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Top-level function to read an OpenFPGA simulation setting file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the simulation setting
 * file provided by users
 *******************************************************************/
template <class T>
int read_simulation_setting_template(T& openfpga_context, const Command& cmd,
                                     const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Reading XML simulation setting '%s'...\n", arch_file_name.c_str());
  openfpga_context.mutable_simulation_setting() =
    read_xml_openfpga_simulation_settings(arch_file_name.c_str());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A function to write an OpenFPGA simulation setting file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the simulation setting
 * file provided by users
 *******************************************************************/
template <class T>
int write_simulation_setting_template(const T& openfpga_context,
                                      const Command& cmd,
                                      const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Writing XML simulation setting to '%s'...\n",
          arch_file_name.c_str());
  write_xml_openfpga_simulation_settings(arch_file_name.c_str(),
                                         openfpga_context.simulation_setting());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Top-level function to read an OpenFPGA bitstream setting file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the bitstream setting
 * file provided by users
 *******************************************************************/
template <class T>
int read_bitstream_setting_template(T& openfpga_context, const Command& cmd,
                                    const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Reading XML bitstream setting '%s'...\n", arch_file_name.c_str());
  openfpga_context.mutable_bitstream_setting() =
    read_xml_openfpga_bitstream_settings(arch_file_name.c_str());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A function to write an OpenFPGA bitstream setting file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the simulation setting
 * file provided by users
 *******************************************************************/
template <class T>
int write_bitstream_setting_template(const T& openfpga_context,
                                     const Command& cmd,
                                     const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Writing XML bitstream setting to '%s'...\n", arch_file_name.c_str());
  write_xml_openfpga_bitstream_settings(arch_file_name.c_str(),
                                        openfpga_context.bitstream_setting());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Top-level function to read an OpenFPGA bitstream setting file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the bitstream setting
 * file provided by users
 *******************************************************************/
template <class T>
int read_openfpga_clock_arch_template(T& openfpga_context, const Command& cmd,
                                      const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Reading XML clock architecture '%s'...\n", arch_file_name.c_str());
  openfpga_context.mutable_clock_arch() =
    read_xml_clock_network(arch_file_name.c_str());
  /* Build internal links */
  if (!openfpga_context.mutable_clock_arch().link()) {
    VTR_LOG_ERROR("Link clock network failed!");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (CMD_EXEC_SUCCESS !=
      link_clock_network_rr_graph(openfpga_context.mutable_clock_arch(),
                                  g_vpr_ctx.device().rr_graph)) {
    VTR_LOG_ERROR("Link clock network to routing architecture failed!");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (CMD_EXEC_SUCCESS != check_clock_network_tile_annotation(
                            openfpga_context.clock_arch(),
                            openfpga_context.arch().tile_annotations)) {
    VTR_LOG_ERROR(
      "Check clock network consistency with tile annotation failed!");
    return CMD_EXEC_FATAL_ERROR;
  }
  /* Ensure clean data */
  openfpga_context.clock_arch().validate();
  if (!openfpga_context.clock_arch().is_valid()) {
    VTR_LOG_ERROR("Pre-checking clock architecture failed!");
    return CMD_EXEC_FATAL_ERROR;
  }

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A function to write an OpenFPGA bitstream setting file
 * we use the APIs from the libarchopenfpga library
 *
 * The command will accept an option '--file' which is the simulation setting
 * file provided by users
 *******************************************************************/
template <class T>
int write_openfpga_clock_arch_template(const T& openfpga_context,
                                       const Command& cmd,
                                       const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not
   * Actually, it must be enabled as the shell interface will check
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Writing XML clock architecture to '%s'...\n",
          arch_file_name.c_str());
  write_xml_clock_network(arch_file_name.c_str(),
                          openfpga_context.clock_arch());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
