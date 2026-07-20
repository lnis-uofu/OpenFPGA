#include "aggregate_mif.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "bind_bram_to_mif_storage.h"
#include "mif_io_utils.h"
#include "vtr_log.h"

namespace openfpga {

namespace {

struct PbAggregateState {
  int addr_width = 0;
  int data_width = 0;
  std::unordered_map<uint64_t, uint64_t> phys_data_map;
};

/* Operating pb path from address_map is the leaf; parent is the physical pb. */
static std::string physical_pb_from_operating_pb(
  const std::string& operating_pb) {
  const size_t pos = operating_pb.rfind('.');
  if (pos == std::string::npos) {
    return operating_pb;
  }
  return operating_pb.substr(0, pos);
}

} /* namespace */

int aggregate_mif(const MifStorage& logical_storage,
                   const std::string& verilog_path,
                   const std::map<std::string, std::string>&
                     instance_pb_type_path_map,
                   const MifAddressMap& mif_address_map,
                   MifStorage& out_aggregated_storage) {
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
  if (mif_address_map.empty()) {
    VTR_LOG_ERROR("aggregate_mif: empty mif_address_map\n");
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
    const MifAddressMapId map_id =
      mif_address_map.find_by_pb_type(operating_pb_type);
    if (!map_id.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: pb_type '%s' not found in mif_address_map\n",
        operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string physical_pb =
      physical_pb_from_operating_pb(mif_address_map.pb_type(map_id));
    const int address_offset = mif_address_map.address_offset(map_id);
    const int data_offset = mif_address_map.data_offset(map_id);

    const int op_addr_width = logical_storage.addr_width(segment_id);
    const int op_data_width = logical_storage.data_width(segment_id);
    if (op_addr_width <= 0 || op_data_width <= 0) {
      VTR_LOG_ERROR("aggregate_mif: invalid widths for segment '%s'\n",
                    seg_mif_base.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    PbAggregateState& pb_state = pb_agg_states[physical_pb];
    pb_state.addr_width =
      std::max(pb_state.addr_width, address_offset + op_addr_width);
    pb_state.data_width =
      std::max(pb_state.data_width, data_offset + op_data_width);

    for (const MifMemoryLineId& line_id :
         logical_storage.segment_memory_lines(segment_id)) {
      const uint64_t logical_addr = logical_storage.memory_line_address(line_id);
      const uint64_t logical_data = logical_storage.memory_line_data(line_id);
      const uint64_t phys_addr = logical_addr << address_offset;
      const uint64_t phys_data = logical_data << data_offset;
      pb_state.phys_data_map[phys_addr] |= phys_data;
    }
  }

  if (pb_agg_states.empty()) {
    VTR_LOG_ERROR("aggregate_mif: no physical pb groups produced\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  for (const auto& pb_kv : pb_agg_states) {
    const std::string& physical_pb = pb_kv.first;
    const PbAggregateState& pb_state = pb_kv.second;
    if (pb_state.phys_data_map.empty()) {
      VTR_LOG_ERROR("aggregate_mif: empty phys_data for pb '%s'\n",
                    physical_pb.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const MifSegmentId out_seg = out_aggregated_storage.create_segment();
    out_aggregated_storage.set_segment_physical_pb(out_seg, physical_pb);
    out_aggregated_storage.set_segment_addr_width(out_seg,
                                                  pb_state.addr_width);
    out_aggregated_storage.set_segment_data_width(out_seg,
                                                  pb_state.data_width);

    std::vector<uint64_t> phys_addrs;
    phys_addrs.reserve(pb_state.phys_data_map.size());
    for (const auto& addr_kv : pb_state.phys_data_map) {
      phys_addrs.push_back(addr_kv.first);
    }
    std::sort(phys_addrs.begin(), phys_addrs.end());

    for (uint64_t phys_addr : phys_addrs) {
      out_aggregated_storage.create_memory_line(
        out_seg, phys_addr, pb_state.phys_data_map.at(phys_addr));
    }
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
