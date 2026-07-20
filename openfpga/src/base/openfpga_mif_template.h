#pragma once

#include <map>
#include <utility>
#include <string>
#include <vector>

#include "aggregate_mif.h"
#include "bitstream_setting.h"
#include "command.h"
#include "command_context.h"
#include "command_exit_codes.h"
#include "mif_address_map.h"
#include "mif_vpr_placement.h"
#include "read_mif.h"
#include "shell.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "write_mif.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Build a runtime MifAddressMap from bitstream setting entries.
 *******************************************************************/
inline MifAddressMap mif_address_map_from_bitstream_setting(
  const BitstreamSetting& bitstream_setting) {
  MifAddressMap mif_address_map;
  for (const MifAddressMapSettingId& map_id :
       bitstream_setting.mif_address_map_settings()) {
    mif_address_map.create_address_map(
      bitstream_setting.mif_address_map_pb_type(map_id),
      bitstream_setting.mif_address_map_address_offset(map_id),
      bitstream_setting.mif_address_map_data_offset(map_id));
  }
  return mif_address_map;
}

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
 * Aggregates logical init segments per physical pb using address_map.
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

  const std::string& verilog_path = cmd_context.option_value(cmd, opt_verilog);
  const auto& mif_storage = openfpga_context.mif_storage();

  /* VPR placement: instance_name -> pb_type_path */
  const std::map<std::string, MifPlacementInfo> pl_info_map =
    get_instance_info_from_placement();
  std::map<std::string, std::string> inst_pb_type_path_map;
  for (const auto& itor : pl_info_map) {
    inst_pb_type_path_map[itor.first] = itor.second.pb_type_path;
  }

  openfpga::MifStorage aggregated_storage;
  const MifAddressMap mif_address_map =
    mif_address_map_from_bitstream_setting(openfpga_context.bitstream_setting());
  if (mif_address_map.empty()) {
    VTR_LOG_ERROR(
      "write_mif: no mif_address_map in bitstream setting; run "
      "read_openfpga_bitstream_setting first\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const int agg_status = aggregate_mif(mif_storage, verilog_path,
                                       inst_pb_type_path_map, mif_address_map,
                                       aggregated_storage);
  if (CMD_EXEC_SUCCESS != agg_status) {
    return agg_status;
  }

  const std::string& out_path = cmd_context.option_value(cmd, opt_file);
  const int exec_status = write_mif(out_path, aggregated_storage);
  if (CMD_EXEC_SUCCESS != exec_status) {
    return exec_status;
  }
  VTR_LOG("write_mif: wrote '%s'\n", out_path.c_str());
  return CMD_EXEC_SUCCESS;
}

} /* end namespace openfpga */
