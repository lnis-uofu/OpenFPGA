/********************************************************************
 * This file includes functions to read an OpenFPGA architecture file
 * which are built on the libarchopenfpga library
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from archopenfpga library */
#include "read_xml_openfpga_arch.h"
#include "check_circuit_library.h"
#include "circuit_library_utils.h"
#include "write_xml_openfpga_arch.h"

#include "openfpga_read_arch.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Top-level function to read an OpenFPGA architecture file
 * we use the APIs from the libarchopenfpga library 
 *
 * The command will accept an option '--file' which is the architecture
 * file provided by users  
 *******************************************************************/
int read_arch(OpenfpgaContext& openfpga_context,
              const Command& cmd, const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not 
   * Actually, it must be enabled as the shell interface will check 
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Reading XML architecture '%s'...\n",
          arch_file_name.c_str());
  openfpga_context.mutable_arch() = read_xml_openfpga_arch(arch_file_name.c_str());

  /* Check the architecture:
   * 1. Circuit library
   * 2. Technology library (TODO)
   * 3. Simulation settings (TODO)
   */
  if (false == check_circuit_library(openfpga_context.arch().circuit_lib)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  if (false == check_configurable_memory_circuit_model(openfpga_context.arch().config_protocol.type(),
                                                       openfpga_context.arch().circuit_lib,
                                                       openfpga_context.arch().config_protocol.memory_model())) {
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
int write_arch(const OpenfpgaContext& openfpga_context,
               const Command& cmd, const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not 
   * Actually, it must be enabled as the shell interface will check 
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Writing XML architecture to '%s'...\n",
          arch_file_name.c_str());
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
int read_simulation_setting(OpenfpgaContext& openfpga_context,
                            const Command& cmd, const CommandContext& cmd_context) {
  /* Check the option '--file' is enabled or not 
   * Actually, it must be enabled as the shell interface will check 
   * before reaching this fuction
   */
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  std::string arch_file_name = cmd_context.option_value(cmd, opt_file);

  VTR_LOG("Reading XML simulation setting '%s'...\n",
          arch_file_name.c_str());
  openfpga_context.mutable_simulation_setting() = read_xml_openfpga_simulation_settings(arch_file_name.c_str());

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
int write_simulation_setting(const OpenfpgaContext& openfpga_context,
                             const Command& cmd, const CommandContext& cmd_context) {
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
  write_xml_openfpga_simulation_settings(arch_file_name.c_str(), openfpga_context.simulation_setting());

  /* TODO: should identify the error code from internal function execution */
  return CMD_EXEC_SUCCESS;
} 

} /* end namespace openfpga */

