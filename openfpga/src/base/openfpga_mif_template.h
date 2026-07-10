#pragma once

#include <map>
#include <utility>
#include <string>
#include <vector>

#include "bind_bram_to_mif_storage.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "mif_vpr_placement.h"
#include "read_mif.h"
#include "shell.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_mif.h"

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
 * Write processed in-memory MIF data to a MIF file.
 * When MIF is enabled (!mif_storage.empty()), bind BRAM info first.
 *******************************************************************/
template <class T>
int write_mif_template(T& openfpga_context, const Command& cmd,
                       const CommandContext& cmd_context) {
  CommandOptionId opt_file = cmd.option("file");
  CommandOptionId opt_verilog = cmd.option("verilog");
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_file));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_file).empty());
  VTR_ASSERT(true == cmd_context.option_enable(cmd, opt_verilog));
  VTR_ASSERT(false == cmd_context.option_value(cmd, opt_verilog).empty());

  if (openfpga_context.mif_storage().empty()) {
    VTR_LOG_ERROR(
      "write_mif: no aggregated MIF data; run read_mif at least once "
      "with a valid file\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  /* MIF enabled: merge/bind physical BRAM info into mif_storage */
  const std::string& verilog_path = cmd_context.option_value(cmd, opt_verilog);
  auto& mif_storage = openfpga_context.mutable_mif_storage();

  /* VPR placement: model_name -> coordinate (openfpga / g_vpr_ctx) */
  const std::map<std::string, t_pl_loc> pl_coord_map =
    get_instance_info_from_placement();
  std::map<std::string, std::pair<int, int>> inst_coord_map;
  for (const auto& itor : pl_coord_map) {
    inst_coord_map[itor.first] =
      std::make_pair(itor.second.x, itor.second.y);
  }

  const int bind_status = bind_bram_to_mif_storage(
    mif_storage, verilog_path, inst_coord_map,
    openfpga_context.memory_address_map());
  if (CMD_EXEC_SUCCESS != bind_status) {
    return bind_status;
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
