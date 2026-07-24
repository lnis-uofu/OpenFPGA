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
  int aggregated_data_width = 0;
  bool has_addr_range = false;
  uint64_t min_addr = 0;
  uint64_t max_addr = 0;
  std::map<uint64_t, uint64_t> phys_data_map;
};

struct DesHeaderMeta {
  bool valid = false;
  uint64_t min_addr = 0;
  uint64_t max_addr = 0;
  int addr_width = 0;
  int data_width = 0;
};

bool address_in_range(uint64_t addr, const BasicPort& address_range) {
  return address_range.is_valid() && addr >= address_range.get_lsb() &&
         addr <= address_range.get_msb();
}

/* Derive output .mem header addr/data width from <map> rules targeting des. */
bool infer_des_header_from_map_rules(const std::string& des_pb_type,
                                     const BitstreamSetting& bitstream_setting,
                                     DesHeaderMeta& out_meta) {
  bool found = false;
  int64_t min_addr = 0;
  int64_t max_addr = 0;
  size_t max_data_msb = 0;

  for (const MifAddressMapSettingId& map_id :
       bitstream_setting.mif_address_map_settings()) {
    if (bitstream_setting.mif_address_map_des_pb_type(map_id) != des_pb_type) {
      continue;
    }
    for (const MifAddressMapRuleId& rule_id :
         bitstream_setting.mif_address_map_rules(map_id)) {
      const BasicPort src_addr_range =
        bitstream_setting.mif_address_map_rule_src_addr_range(rule_id);
      const int des_addr_offset =
        bitstream_setting.mif_address_map_rule_des_addr_offset(rule_id);
      const BasicPort src_mif_bits =
        bitstream_setting.mif_address_map_rule_src_mif_bits(rule_id);
      const BasicPort des_mif_bits =
        bitstream_setting.mif_address_map_rule_des_mif_bits(rule_id);

      if (!src_addr_range.is_valid() || !src_mif_bits.is_valid() ||
          !des_mif_bits.is_valid()) {
        VTR_LOG_ERROR(
          "aggregate_mif: invalid map rule ranges for des pb_type '%s'\n",
          des_pb_type.c_str());
        return false;
      }
      if (src_mif_bits.get_width() != des_mif_bits.get_width()) {
        VTR_LOG_ERROR(
          "aggregate_mif: src_mif_bits width %zu != des_mif_bits width %zu "
          "for des pb_type '%s'\n",
          src_mif_bits.get_width(), des_mif_bits.get_width(),
          des_pb_type.c_str());
        return false;
      }

      const int64_t des_min =
        static_cast<int64_t>(src_addr_range.get_lsb()) + des_addr_offset;
      const int64_t des_max =
        static_cast<int64_t>(src_addr_range.get_msb()) + des_addr_offset;
      if (des_min < 0 || des_max < 0 || des_min > des_max) {
        VTR_LOG_ERROR(
          "aggregate_mif: mapped address range [%ld:%ld] invalid for des "
          "pb_type '%s' (src_addr_range + des_addr_offset=%d)\n",
          static_cast<long>(des_min), static_cast<long>(des_max),
          des_pb_type.c_str(), des_addr_offset);
        return false;
      }

      if (!found) {
        min_addr = des_min;
        max_addr = des_max;
        max_data_msb = des_mif_bits.get_msb();
        found = true;
      } else {
        min_addr = std::min(min_addr, des_min);
        max_addr = std::max(max_addr, des_max);
        max_data_msb = std::max(max_data_msb, des_mif_bits.get_msb());
      }
    }
  }

  if (!found) {
    VTR_LOG_ERROR(
      "aggregate_mif: no map rules found for des pb_type '%s'\n",
      des_pb_type.c_str());
    return false;
  }

  out_meta.valid = true;
  out_meta.min_addr = static_cast<uint64_t>(min_addr);
  out_meta.max_addr = static_cast<uint64_t>(max_addr);
  out_meta.data_width = static_cast<int>(max_data_msb) + 1;
  out_meta.addr_width = mif_bit_width_for_max_value(out_meta.max_addr);
  return true;
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
  std::map<std::string, DesHeaderMeta> des_header_metas;

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
      bitstream_setting.find_mif_address_map_by_src_pb_type(operating_pb_type);
    if (!map_id.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: src_pb_type '%s' not found in mif_address_map\n",
        operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const MifSourceSettingId src_source_id =
      bitstream_setting.find_mif_source_by_pb_type(operating_pb_type);
    if (!src_source_id.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: src_pb_type '%s' has no mif_source for address_range "
        "sanity check\n",
        operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    const BasicPort src_address_range =
      bitstream_setting.mif_source_address_range(src_source_id);
    if (!src_address_range.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: invalid address_range on mif_source for '%s'\n",
        operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const BasicPort src_data_range =
      bitstream_setting.mif_source_data_range(src_source_id);
    if (!src_data_range.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: invalid data_range on mif_source for '%s'\n",
        operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    /* Source data width comes from bitstream mif_source, not init.hex. */
    const int op_data_width = static_cast<int>(src_data_range.get_width());

    const std::string aggregated_pb_type =
      bitstream_setting.mif_address_map_des_pb_type(map_id);

    DesHeaderMeta& header_meta = des_header_metas[aggregated_pb_type];
    if (!header_meta.valid) {
      if (!infer_des_header_from_map_rules(aggregated_pb_type, bitstream_setting,
                                           header_meta)) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
    const int aggregated_data_width = header_meta.data_width;
    if (aggregated_data_width <= 0) {
      VTR_LOG_ERROR(
        "aggregate_mif: invalid data width from map rules for des pb_type "
        "'%s'\n",
        aggregated_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    if (op_data_width <= 0) {
      VTR_LOG_ERROR("aggregate_mif: invalid data_range width for '%s'\n",
                    operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (op_data_width > aggregated_data_width) {
      VTR_LOG_ERROR(
        "aggregate_mif: source data width %d exceeds destination width %d for "
        "'%s'\n",
        op_data_width, aggregated_data_width, operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    /* Optional: if init.hex declared a depth range, check it against
     * mif_source.address_range. Per-line checks always apply. */
    if (logical_storage.has_addr_range(segment_id)) {
      const uint64_t seg_min = logical_storage.min_addr(segment_id);
      const uint64_t seg_max = logical_storage.max_addr(segment_id);
      if (!address_in_range(seg_min, src_address_range) ||
          !address_in_range(seg_max, src_address_range)) {
        VTR_LOG_ERROR(
          "aggregate_mif: segment '%s' addr range [%lu:%lu] is outside "
          "mif_source address_range [%zu:%zu] for pb_type '%s'\n",
          seg_mif_base.c_str(), static_cast<unsigned long>(seg_min),
          static_cast<unsigned long>(seg_max), src_address_range.get_lsb(),
          src_address_range.get_msb(), operating_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
    }

    PbAggregateState& pb_state = pb_agg_states[aggregated_pb_type];
    pb_state.addr_width = header_meta.addr_width;
    pb_state.aggregated_data_width = aggregated_data_width;
    pb_state.min_addr = header_meta.min_addr;
    pb_state.max_addr = header_meta.max_addr;
    pb_state.has_addr_range = true;

    /* Aggregate by RAM index when present; full map-rule remapping of
     * addr/data bits is applied later in the bitstream stage. */
    const int slice_index = extract_pb_type_leaf_index(operating_pb_type);
    const int data_bit_shift =
      (slice_index > 0) ? slice_index * op_data_width : 0;
    if (data_bit_shift + op_data_width > aggregated_data_width) {
      VTR_LOG_ERROR(
        "aggregate_mif: slice packing for '%s' exceeds des data width %d\n",
        operating_pb_type.c_str(), aggregated_data_width);
      return CMD_EXEC_FATAL_ERROR;
    }

    const uint64_t data_mask =
      (op_data_width >= 64) ? ~uint64_t(0)
                            : ((uint64_t(1) << op_data_width) - 1);

    for (const MifMemoryLineId& line_id :
         logical_storage.segment_memory_lines(segment_id)) {
      const uint64_t logical_addr =
        logical_storage.memory_line_address(line_id);
      if (!address_in_range(logical_addr, src_address_range)) {
        VTR_LOG_ERROR(
          "aggregate_mif: address %lu in '%s' is outside mif_source "
          "address_range [%zu:%zu] for pb_type '%s'\n",
          static_cast<unsigned long>(logical_addr), seg_mif_base.c_str(),
          src_address_range.get_lsb(), src_address_range.get_msb(),
          operating_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      const uint64_t logical_data = logical_storage.memory_line_data(line_id);
      if ((logical_data & ~data_mask) != 0) {
        VTR_LOG_ERROR(
          "aggregate_mif: data 0x%llx at addr %lu in '%s' exceeds "
          "mif_source data_range width %d for pb_type '%s'\n",
          static_cast<unsigned long long>(logical_data),
          static_cast<unsigned long>(logical_addr), seg_mif_base.c_str(),
          op_data_width, operating_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
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

    const MifSegmentId out_seg = out_aggregated_storage.create_segment();
    out_aggregated_storage.set_segment_physical_pb(out_seg, aggregated_pb_type);
    out_aggregated_storage.set_segment_addr_width(out_seg, pb_state.addr_width);
    out_aggregated_storage.set_segment_data_width(
      out_seg, pb_state.aggregated_data_width);
    out_aggregated_storage.set_segment_addr_range(
      out_seg, pb_state.min_addr, pb_state.max_addr);

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
