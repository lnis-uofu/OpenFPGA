#include "openfpga_mif_bitstream.h"

#include "aggregate_mif.h"
#include "openfpga_digest.h"
#include "vtr_log.h"
#include "write_mif.h"

namespace openfpga {

std::string default_preload_mem_file_path(const std::string& write_file_path) {
  const size_t slash = write_file_path.find_last_of("/\\");
  const size_t dot = write_file_path.find_last_of('.');
  if (dot != std::string::npos && (slash == std::string::npos || dot > slash)) {
    return write_file_path.substr(0, dot) + "_memory.mem";
  }
  return write_file_path + "_memory.mem";
}

int aggregate_mif_storage(
  const MifStorage& mif_storage, const BitstreamSetting& bitstream_setting,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const std::string& verilog_path,
  MifStorage& aggregated_mif_storage) {
  aggregated_mif_storage.clear();

  if (mif_storage.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  if (bitstream_setting.mif_address_map_settings().empty()) {
    VTR_LOG_ERROR(
      "aggregate_mif_storage: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  return aggregate_mif(mif_storage, verilog_path, instance_pb_type_path_map,
                       bitstream_setting, aggregated_mif_storage);
}

int aggregate_mif_storage_and_write_preload_mem(
  const MifStorage& mif_storage, const BitstreamSetting& bitstream_setting,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const std::string& verilog_path, MifStorage& aggregated_mif_storage,
  const std::string& mem_file_path) {
  const int agg_status = aggregate_mif_storage(
    mif_storage, bitstream_setting, instance_pb_type_path_map, verilog_path,
    aggregated_mif_storage);
  if (CMD_EXEC_SUCCESS != agg_status) {
    return agg_status;
  }

  if (aggregated_mif_storage.empty() || mem_file_path.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  const std::string mem_dir = find_path_dir_name(mem_file_path);
  create_directory(mem_dir, true, false);

  const int write_status = write_mif(mem_file_path, aggregated_mif_storage);
  if (CMD_EXEC_SUCCESS != write_status) {
    return write_status;
  }

  VTR_LOG("Wrote aggregated preload memory to '%s'\n", mem_file_path.c_str());
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
