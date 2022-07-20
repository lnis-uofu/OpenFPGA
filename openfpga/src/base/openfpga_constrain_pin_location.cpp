/********************************************************************
 * This file includes functions to build bitstream database
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_log.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_reserved_words.h"

#include "openfpga_naming.h"
#include "openfpga_constrain_pin_location.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
extern int pin_constrain_location (std::vector<std::string> args);
namespace openfpga {
  /********************************************************************
 * Top-level function to read an OpenFPGA architecture file
 * we use the APIs from the libarchopenfpga library 
 *
 * The command will accept an option '--file' which is the architecture
 * file provided by users  
 *******************************************************************/
int constrain_pin_location(OpenfpgaContext& openfpga_context, const Command& cmd, const CommandContext& cmd_context) {

  /* initialize arguments with sanity check*/
  std::vector<std::string> pin_constrain_args;
  pin_constrain_args.push_back("pin_c");

  /* todo: create a factory to produce this in the future*/
  std::string pcf_option_name = "pcf";
  CommandOptionId opt_pcf_file = cmd.option(pcf_option_name);
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_pcf_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_pcf_file).empty());
  std::string pcf_file_name = cmd_context.option_value(cmd, opt_pcf_file);
  pin_constrain_args.push_back(std::string("--") + pcf_option_name);
  pin_constrain_args.push_back(pcf_file_name.c_str());

  std::string blif_option_name = "blif";
  CommandOptionId opt_blif_file = cmd.option(blif_option_name);
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_blif_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_blif_file).empty());
  std::string blif_file_name = cmd_context.option_value(cmd, opt_blif_file);
  pin_constrain_args.push_back(std::string("--") + blif_option_name);
  pin_constrain_args.push_back(blif_file_name);

#if 0
  // --net is not supported by pin_c yet
  std::string net_option_name = "net";
  CommandOptionId opt_net_file = cmd.option(net_option_name);
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_net_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_net_file).empty());
  std::string net_file_name = cmd_context.option_value(cmd, opt_net_file);
  pin_constrain_args.push_back(std::string("--") + net_option_name);
  pin_constrain_args.push_back(net_file_name);
#endif

  std::string pinmap_xml_option_name = "pinmap_xml";
  CommandOptionId opt_pinmap_xml_file = cmd.option(pinmap_xml_option_name);
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_pinmap_xml_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_pinmap_xml_file).empty());
  std::string pinmap_xml_file_name = cmd_context.option_value(cmd, opt_pinmap_xml_file);
  pin_constrain_args.push_back(std::string("--xml"));
  pin_constrain_args.push_back(pinmap_xml_file_name);

  std::string csv_file_option_name = "csv_file";
  CommandOptionId opt_csv_file = cmd.option(csv_file_option_name);
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_csv_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_csv_file).empty());
  std::string csv_file_name = cmd_context.option_value(cmd, opt_csv_file);
  pin_constrain_args.push_back(std::string("--csv"));
  pin_constrain_args.push_back(csv_file_name);

  std::string output_option_name = "output";
  CommandOptionId opt_output_file = cmd.option(output_option_name);
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_output_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_output_file).empty());
  std::string output_file_name = cmd_context.option_value(cmd, opt_output_file);
  pin_constrain_args.push_back(std::string("--") + output_option_name);
  pin_constrain_args.push_back(output_file_name);

  if (pin_constrain_location(pin_constrain_args)) {
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
