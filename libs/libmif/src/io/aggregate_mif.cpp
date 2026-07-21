#include "aggregate_mif.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "mif_io_utils.h"
#include "vtr_log.h"

namespace openfpga {

namespace {

struct PbAggregateState {
  int addr_width = 0;
  int slice_data_width_hint = 0;
  int max_slice_index = -1;
  std::map<uint64_t, uint64_t> phys_data_map;
};

int infer_max_slice_index(const std::string& aggregated_pb_type,
                          const BitstreamSetting& bitstream_setting) {
  int max_slice_index = -1;
  for (const MifAddressMapSettingId& map_id :
       bitstream_setting.mif_address_map_settings()) {
    const std::string map_pb_type =
      bitstream_setting.mif_address_map_pb_type(map_id);
    if (strip_pb_type_indices(map_pb_type) != aggregated_pb_type) {
      continue;
    }
    const int slice_index = extract_pb_type_leaf_index(map_pb_type);
    max_slice_index = std::max(max_slice_index, slice_index);
  }
  return max_slice_index;
}

int infer_aggregated_data_width(int max_slice_index,
                                int slice_data_width_hint) {
  if (max_slice_index < 0 || slice_data_width_hint <= 0) {
    return 0;
  }
  return (max_slice_index + 1) * slice_data_width_hint;
}

} /* namespace */

int aggregate_mif(
  const MifStorage& logical_storage, const std::string& verilog_path,
  const std::map<std::string, std::string>& instance_pb_type_path_map,
  const BitstreamSetting& bitstream_setting,
  AggregatedMifStorage& out_aggregated_storage) {
  if (logical_storage.empty()) {
    VTR_LOG_ERROR("aggregate_mif: empty logical MIF storage\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (verilog_path.empty()) {
    VTR_LOG_ERROR("aggregate_mif: verilog path is required\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (instance_pb_type_path_map.empty()) {
    VTR_LOG_ERROR("aggregate_mif: empty instance_pb_type_path_map\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (bitstream_setting.mif_address_map_settings().empty()) {
    VTR_LOG_ERROR("aggregate_mif: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  out_aggregated_storage.clear();
  std::map<std::string, PbAggregateState> pb_agg_states;

  for (const MifSegmentId& segment_id : logical_storage.segments()) {
    if (!logical_storage.has_source_file(segment_id)) {
      VTR_LOG_ERROR(
        "aggregate_mif: segment has no source_file; segment_id=%zu\n",
        static_cast<size_t>(segment_id));
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string seg_mif_full =
      logical_storage.segment_source_file(segment_id);
    const std::string seg_mif_base = mif_file_basename(seg_mif_full);
    const std::string instance_name =
      find_verilog_instance_reading_mif(verilog_path, seg_mif_base);
    if (instance_name.empty()) {
      VTR_LOG_ERROR("aggregate_mif: cannot find instance for '%s' in '%s'\n",
                    seg_mif_base.c_str(), verilog_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const auto pb_it = instance_pb_type_path_map.find(instance_name);
    if (pb_it == instance_pb_type_path_map.end()) {
      VTR_LOG_ERROR("aggregate_mif: instance '%s' missing from pb_type map\n",
                    instance_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string& operating_pb_type = pb_it->second;
    const MifAddressMapSettingId map_id =
      bitstream_setting.find_mif_address_map_by_pb_type(operating_pb_type);
    if (!map_id.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: pb_type '%s' not found in mif_address_map\n",
        operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string aggregated_pb_type =
      strip_pb_type_indices(bitstream_setting.mif_address_map_pb_type(map_id));
    const int slice_index = extract_pb_type_leaf_index(operating_pb_type);
    if (slice_index < 0) {
      VTR_LOG_ERROR(
        "aggregate_mif: cannot extract RAM slice index from pb_type '%s'\n",
        operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const int op_addr_width = logical_storage.addr_width(segment_id);
    const int op_data_width = logical_storage.data_width(segment_id);
    if (op_addr_width <= 0 || op_data_width <= 0) {
      VTR_LOG_ERROR("aggregate_mif: invalid widths for segment '%s'\n",
                    seg_mif_base.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    PbAggregateState& pb_state = pb_agg_states[aggregated_pb_type];
    pb_state.addr_width = std::max(pb_state.addr_width, op_addr_width);
    pb_state.slice_data_width_hint =
      std::max(pb_state.slice_data_width_hint, op_data_width);
    pb_state.max_slice_index = std::max(pb_state.max_slice_index, slice_index);

    const int max_slice_index =
      infer_max_slice_index(aggregated_pb_type, bitstream_setting);
    if (max_slice_index < 0) {
      VTR_LOG_ERROR("aggregate_mif: cannot infer max slice index for pb '%s'\n",
                    aggregated_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const int aggregated_data_width = infer_aggregated_data_width(
      max_slice_index, pb_state.slice_data_width_hint);
    if (aggregated_data_width <= 0) {
      VTR_LOG_ERROR("aggregate_mif: invalid aggregated data width for '%s'\n",
                    aggregated_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const int data_bit_shift = (max_slice_index - slice_index) * op_data_width;
    if (data_bit_shift < 0 ||
        data_bit_shift + op_data_width > aggregated_data_width) {
      VTR_LOG_ERROR(
        "aggregate_mif: slice index=%d exceeds max=%d for pb '%s'\n",
        slice_index, max_slice_index, operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    for (const MifMemoryLineId& line_id :
         logical_storage.segment_memory_lines(segment_id)) {
      const uint64_t logical_addr =
        logical_storage.memory_line_address(line_id);
      const uint64_t logical_data = logical_storage.memory_line_data(line_id);
      const uint64_t phys_data = logical_data << data_bit_shift;
      pb_state.phys_data_map[logical_addr] |= phys_data;
    }
  }

  if (pb_agg_states.empty()) {
    VTR_LOG_ERROR("aggregate_mif: no physical pb groups produced\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  for (const auto& pb_kv : pb_agg_states) {
    const std::string& aggregated_pb_type = pb_kv.first;
    const PbAggregateState& pb_state = pb_kv.second;
    if (pb_state.phys_data_map.empty()) {
      VTR_LOG_ERROR("aggregate_mif: empty phys_data for pb '%s'\n",
                    aggregated_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const int max_slice_index =
      infer_max_slice_index(aggregated_pb_type, bitstream_setting);
    const int aggregated_data_width = infer_aggregated_data_width(
      max_slice_index, pb_state.slice_data_width_hint);
    if (aggregated_data_width <= 0) {
      VTR_LOG_ERROR("aggregate_mif: invalid aggregated data width for '%s'\n",
                    aggregated_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const MifSegmentId out_seg = out_aggregated_storage.create_segment();
    out_aggregated_storage.set_segment_physical_pb(out_seg, aggregated_pb_type);
    out_aggregated_storage.set_segment_addr_width(out_seg, pb_state.addr_width);
    out_aggregated_storage.set_segment_data_width(out_seg,
                                                  aggregated_data_width);

    std::vector<uint64_t> phys_addrs;
    phys_addrs.reserve(pb_state.phys_data_map.size());
    for (const auto& addr_kv : pb_state.phys_data_map) {
      phys_addrs.push_back(addr_kv.first);
    }
    std::sort(phys_addrs.begin(), phys_addrs.end());

    for (const uint64_t phys_addr : phys_addrs) {
      out_aggregated_storage.create_memory_line(
        out_seg, phys_addr, pb_state.phys_data_map.at(phys_addr));
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
