#include "aggregate_mif.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "vtr_log.h"

namespace openfpga {

struct PbAggregateState {
  int aggregated_data_width = 0;
  BasicPort addr_range;
  std::map<uint64_t, uint64_t> phys_data_map;
};

struct DesHeaderMeta {
  bool valid = false;
  BasicPort addr_range;
  int data_width = 0;
};

static bool address_in_range(uint64_t addr, const BasicPort& address_range) {
  return address_range.is_valid() && addr >= address_range.get_lsb() &&
         addr <= address_range.get_msb();
}

static uint64_t width_mask(size_t width) {
  if (width >= 64) {
    return ~uint64_t(0);
  }
  return (uint64_t(1) << width) - 1;
}

/* Extract contiguous bits [lsb:msb] from a word (lsb = bit 0 of word). */
static uint64_t extract_mif_bits(uint64_t data, const BasicPort& bits) {
  return (data >> bits.get_lsb()) & width_mask(bits.get_width());
}

/* Place extracted bits into [lsb:msb] of a destination word. */
static uint64_t place_mif_bits(uint64_t extracted, const BasicPort& bits) {
  return (extracted & width_mask(bits.get_width())) << bits.get_lsb();
}

/* Derive output .mem header addr/data width from <map> rules targeting des. */
static bool infer_des_header_from_map_rules(
  const std::string& des_pb_type, const BitstreamSetting& bitstream_setting,
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
    VTR_LOG_ERROR("aggregate_mif: no map rules found for des pb_type '%s'\n",
                  des_pb_type.c_str());
    return false;
  }

  out_meta.valid = true;
  out_meta.addr_range = BasicPort("address", static_cast<size_t>(min_addr),
                                  static_cast<size_t>(max_addr));
  out_meta.data_width = static_cast<int>(max_data_msb) + 1;
  return true;
}

/* Apply all matching <map> rules for one logical (addr, data) word. */
static bool remap_logical_word_to_physical(
  uint64_t logical_addr, uint64_t logical_data, int op_data_width,
  const MifAddressMapSettingId& map_id,
  const BitstreamSetting& bitstream_setting, PbAggregateState& pb_state) {
  bool matched = false;

  for (const MifAddressMapRuleId& rule_id :
       bitstream_setting.mif_address_map_rules(map_id)) {
    const BasicPort src_addr_range =
      bitstream_setting.mif_address_map_rule_src_addr_range(rule_id);
    if (!address_in_range(logical_addr, src_addr_range)) {
      continue;
    }
    matched = true;

    const int des_addr_offset =
      bitstream_setting.mif_address_map_rule_des_addr_offset(rule_id);
    const BasicPort src_mif_bits =
      bitstream_setting.mif_address_map_rule_src_mif_bits(rule_id);
    const BasicPort des_mif_bits =
      bitstream_setting.mif_address_map_rule_des_mif_bits(rule_id);

    if (src_mif_bits.get_msb() >= static_cast<size_t>(op_data_width)) {
      VTR_LOG_ERROR(
        "aggregate_mif: src_mif_bits [%zu:%zu] exceeds source data width %d "
        "at logical addr %lu\n",
        src_mif_bits.get_lsb(), src_mif_bits.get_msb(), op_data_width,
        static_cast<unsigned long>(logical_addr));
      return false;
    }
    if (des_mif_bits.get_msb() >=
        static_cast<size_t>(pb_state.aggregated_data_width)) {
      VTR_LOG_ERROR(
        "aggregate_mif: des_mif_bits [%zu:%zu] exceeds aggregated data width "
        "%d at logical addr %lu\n",
        des_mif_bits.get_lsb(), des_mif_bits.get_msb(),
        pb_state.aggregated_data_width,
        static_cast<unsigned long>(logical_addr));
      return false;
    }

    const int64_t des_addr_signed =
      static_cast<int64_t>(logical_addr) + des_addr_offset;
    if (des_addr_signed < 0) {
      VTR_LOG_ERROR(
        "aggregate_mif: des_addr %ld < 0 for logical addr %lu "
        "(des_addr_offset=%d)\n",
        static_cast<long>(des_addr_signed),
        static_cast<unsigned long>(logical_addr), des_addr_offset);
      return false;
    }
    const uint64_t des_addr = static_cast<uint64_t>(des_addr_signed);
    if (!address_in_range(des_addr, pb_state.addr_range)) {
      VTR_LOG_ERROR(
        "aggregate_mif: des_addr %lu outside inferred des address range "
        "[%zu:%zu] (logical addr %lu, des_addr_offset=%d)\n",
        static_cast<unsigned long>(des_addr), pb_state.addr_range.get_lsb(),
        pb_state.addr_range.get_msb(),
        static_cast<unsigned long>(logical_addr), des_addr_offset);
      return false;
    }

    const uint64_t extracted = extract_mif_bits(logical_data, src_mif_bits);
    const uint64_t placed = place_mif_bits(extracted, des_mif_bits);
    const uint64_t des_mask =
      width_mask(des_mif_bits.get_width()) << des_mif_bits.get_lsb();

    auto& phys_word = pb_state.phys_data_map[des_addr];
    const uint64_t existing = phys_word & des_mask;
    if (existing != 0 && existing != (placed & des_mask)) {
      VTR_LOG_ERROR(
        "aggregate_mif: conflicting writes to des addr %lu bits [%zu:%zu]: "
        "existing 0x%llx vs new 0x%llx (logical addr %lu)\n",
        static_cast<unsigned long>(des_addr), des_mif_bits.get_lsb(),
        des_mif_bits.get_msb(), static_cast<unsigned long long>(existing),
        static_cast<unsigned long long>(placed & des_mask),
        static_cast<unsigned long>(logical_addr));
      return false;
    }
    phys_word |= placed;
  }

  if (!matched) {
    VTR_LOG_ERROR(
      "aggregate_mif: logical addr %lu is not covered by any <map> "
      "src_addr_range\n",
      static_cast<unsigned long>(logical_addr));
    return false;
  }
  return true;
}

/* Resolve which mif_address_map applies to a logical segment. */
static bool resolve_map_for_segment(
  const MifStorage& logical_storage, const MifSegmentId& segment_id,
  const BitstreamSetting& bitstream_setting,
  const std::vector<MifAddressMapSettingId>& map_ids,
  MifAddressMapSettingId& out_map_id, std::string& out_src_pb_type) {
  const std::string& seg_pb = logical_storage.physical_pb(segment_id);
  if (!seg_pb.empty()) {
    for (const MifAddressMapSettingId& id : map_ids) {
      if (bitstream_setting.mif_address_map_src_pb_type(id) == seg_pb) {
        out_map_id = id;
        out_src_pb_type = seg_pb;
        return true;
      }
    }
    VTR_LOG_ERROR(
      "aggregate_mif: segment %zu pb_type '%s' has no matching "
      "mif_address_map src_pb_type\n",
      static_cast<size_t>(segment_id), seg_pb.c_str());
    return false;
  }

  /* Untagged segment (typical init.hex): only valid with a single map. */
  if (map_ids.size() != 1) {
    VTR_LOG_ERROR(
      "aggregate_mif: segment %zu has no pb_type tag, but %zu "
      "mif_address_map entries exist; tag segments (eblif mif_source) or "
      "use a single map\n",
      static_cast<size_t>(segment_id), map_ids.size());
    return false;
  }
  out_map_id = map_ids[0];
  out_src_pb_type = bitstream_setting.mif_address_map_src_pb_type(out_map_id);
  return true;
}

int aggregate_mif(const MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage) {
  if (logical_storage.empty()) {
    VTR_LOG_ERROR("aggregate_mif: empty logical MIF storage\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  std::vector<MifAddressMapSettingId> map_ids;
  for (const MifAddressMapSettingId& id :
       bitstream_setting.mif_address_map_settings()) {
    map_ids.push_back(id);
  }
  if (map_ids.empty()) {
    VTR_LOG_ERROR("aggregate_mif: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const std::string aggregated_pb_type =
    bitstream_setting.mif_address_map_des_pb_type(map_ids[0]);
  for (size_t i = 1; i < map_ids.size(); ++i) {
    const std::string& des =
      bitstream_setting.mif_address_map_des_pb_type(map_ids[i]);
    if (des != aggregated_pb_type) {
      VTR_LOG_ERROR(
        "aggregate_mif: all mif_address_map entries must share the same "
        "des_pb_type (got '%s' and '%s')\n",
        aggregated_pb_type.c_str(), des.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  DesHeaderMeta header_meta;
  if (!infer_des_header_from_map_rules(aggregated_pb_type, bitstream_setting,
                                       header_meta)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  const int aggregated_data_width = header_meta.data_width;
  if (aggregated_data_width <= 0) {
    VTR_LOG_ERROR(
      "aggregate_mif: invalid data width from map rules for des pb_type "
      "'%s'\n",
      aggregated_pb_type.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  VTR_LOG(
    "aggregate_mif: remapping %zu logical segment(s) via %zu "
    "mif_address_map(s) -> des='%s'\n",
    logical_storage.num_segments(), map_ids.size(),
    aggregated_pb_type.c_str());

  out_aggregated_storage.clear();
  PbAggregateState pb_state;
  pb_state.aggregated_data_width = aggregated_data_width;
  pb_state.addr_range = header_meta.addr_range;

  for (const MifSegmentId& segment_id : logical_storage.segments()) {
    MifAddressMapSettingId map_id = MifAddressMapSettingId::INVALID();
    std::string operating_pb_type;
    if (!resolve_map_for_segment(logical_storage, segment_id, bitstream_setting,
                                 map_ids, map_id, operating_pb_type)) {
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
    const int op_data_width = static_cast<int>(src_data_range.get_width());
    if (op_data_width <= 0) {
      VTR_LOG_ERROR("aggregate_mif: invalid data_range width for '%s'\n",
                    operating_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    const uint64_t data_mask = width_mask(static_cast<size_t>(op_data_width));

    VTR_LOG("aggregate_mif: segment %zu bound src='%s' -> des='%s'\n",
            static_cast<size_t>(segment_id), operating_pb_type.c_str(),
            aggregated_pb_type.c_str());

    /* Optional: if init.hex declared a depth range, check it against
     * mif_source.address_range. Per-line checks always apply. */
    const BasicPort& seg_addr_range = logical_storage.addr_range(segment_id);
    if (seg_addr_range.is_valid()) {
      if (!address_in_range(seg_addr_range.get_lsb(), src_address_range) ||
          !address_in_range(seg_addr_range.get_msb(), src_address_range)) {
        VTR_LOG_ERROR(
          "aggregate_mif: segment %zu addr range [%zu:%zu] is outside "
          "mif_source address_range [%zu:%zu] for pb_type '%s'\n",
          static_cast<size_t>(segment_id), seg_addr_range.get_lsb(),
          seg_addr_range.get_msb(), src_address_range.get_lsb(),
          src_address_range.get_msb(), operating_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
    }

    for (const MifMemoryLineId& line_id :
         logical_storage.segment_memory_lines(segment_id)) {
      const uint64_t logical_addr =
        logical_storage.memory_line_address(line_id);
      if (!address_in_range(logical_addr, src_address_range)) {
        VTR_LOG_ERROR(
          "aggregate_mif: address %lu in segment %zu is outside mif_source "
          "address_range [%zu:%zu] for pb_type '%s'\n",
          static_cast<unsigned long>(logical_addr),
          static_cast<size_t>(segment_id), src_address_range.get_lsb(),
          src_address_range.get_msb(), operating_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      const uint64_t logical_data = logical_storage.memory_line_data(line_id);
      if ((logical_data & ~data_mask) != 0) {
        VTR_LOG_ERROR(
          "aggregate_mif: data 0x%llx at addr %lu in segment %zu exceeds "
          "mif_source data_range width %d for pb_type '%s'\n",
          static_cast<unsigned long long>(logical_data),
          static_cast<unsigned long>(logical_addr),
          static_cast<size_t>(segment_id), op_data_width,
          operating_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      if (!remap_logical_word_to_physical(logical_addr, logical_data,
                                          op_data_width, map_id,
                                          bitstream_setting, pb_state)) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  if (pb_state.phys_data_map.empty()) {
    VTR_LOG_ERROR("aggregate_mif: empty phys_data for pb '%s'\n",
                  aggregated_pb_type.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  const MifSegmentId out_seg = out_aggregated_storage.create_segment();
  out_aggregated_storage.set_segment_physical_pb(out_seg, aggregated_pb_type);
  out_aggregated_storage.set_segment_data_width(out_seg,
                                                pb_state.aggregated_data_width);
  out_aggregated_storage.set_segment_addr_range(out_seg, pb_state.addr_range);

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

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
