#ifndef OPENFPGA_MIF_TEMPLATE_H
#define OPENFPGA_MIF_TEMPLATE_H

#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "read_mif.h"
#include "shell.h"
#include "vtr_assert.h"
#include "vtr_log.h"

#include <string>
#include <vector>

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * A function to read a MIF file and append its contents to MIF storage
 *******************************************************************/
template <class T>
int read_mif_template(T& openfpga_context, const Command& cmd,
                      const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  const std::string& mif_path = cmd_context.option_value(cmd, opt_file);
  const int exec_status =
    read_mif(mif_path, openfpga_context.mutable_mif_storage());
  if (CMD_EXEC_SUCCESS != exec_status) {
    return exec_status;
  }
  VTR_LOG("read_mif: read '%s'\n", mif_path.c_str());
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * A function to write the aggregated MIF data to a file
 *******************************************************************/
template <class T>
int write_mif_template(const T& openfpga_context, const Command& cmd,
                       const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());

  if (openfpga_context.mif_storage().empty()) {
    VTR_LOG_ERROR(
      "write_mif: no aggregated MIF data; run read_mif at least once "
      "with a valid file\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const std::string& out_path = cmd_context.option_value(cmd, opt_file);
  const int exec_status = write_mif(out_path, openfpga_context.mif_storage());
  if (CMD_EXEC_SUCCESS != exec_status) {
    return exec_status;
  }
  VTR_LOG("write_mif: wrote '%s'\n", out_path.c_str());
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */

#endif
