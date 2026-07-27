#include "aggregate_mif.h"

#include <algorithm>
#include <cstdint>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

#include "read_mif.h"
#include "vtr_log.h"

namespace openfpga {

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

static uint64_t extract_mif_bits(uint64_t data, const BasicPort& bits) {
  return (data >> bits.get_lsb()) & width_mask(bits.get_width());
}

static uint64_t place_mif_bits(uint64_t extracted, const BasicPort& bits) {
  return (extracted & width_mask(bits.get_width())) << bits.get_lsb();
}

/* Infer des addr_range / data_width from all <map> rules targeting this des. */
static bool infer_des_header(const std::string& des_pb_type,
                             const std::vector<MifAddressMapSettingId>& map_ids,
                             const BitstreamSetting& bitstream_setting,
                             BasicPort& out_addr_range, int& out_data_width) {
  bool found = false;
  int64_t min_addr = 0;
  int64_t max_addr = 0;
  size_t max_data_msb = 0;

  for (const MifAddressMapSettingId& map_id : map_ids) {
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

  out_addr_range = BasicPort("address", static_cast<size_t>(min_addr),
                             static_cast<size_t>(max_addr));
  out_data_width = static_cast<int>(max_data_msb) + 1;
  return out_data_width > 0;
}

static bool remap_logical_word(
  uint64_t logical_addr, uint64_t logical_data, int op_data_width,
  int des_data_width, const BasicPort& des_addr_range,
  const MifAddressMapSettingId& map_id,
  const BitstreamSetting& bitstream_setting,
  std::map<uint64_t, uint64_t>& phys_data_map) {
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
    if (des_mif_bits.get_msb() >= static_cast<size_t>(des_data_width)) {
      VTR_LOG_ERROR(
        "aggregate_mif: des_mif_bits [%zu:%zu] exceeds aggregated data width "
        "%d at logical addr %lu\n",
        des_mif_bits.get_lsb(), des_mif_bits.get_msb(), des_data_width,
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
    if (!address_in_range(des_addr, des_addr_range)) {
      VTR_LOG_ERROR(
        "aggregate_mif: des_addr %lu outside inferred des address range "
        "[%zu:%zu] (logical addr %lu, des_addr_offset=%d)\n",
        static_cast<unsigned long>(des_addr), des_addr_range.get_lsb(),
        des_addr_range.get_msb(), static_cast<unsigned long>(logical_addr),
        des_addr_offset);
      return false;
    }

    const uint64_t extracted = extract_mif_bits(logical_data, src_mif_bits);
    const uint64_t placed = place_mif_bits(extracted, des_mif_bits);
    const uint64_t des_mask =
      width_mask(des_mif_bits.get_width()) << des_mif_bits.get_lsb();

    auto& phys_word = phys_data_map[des_addr];
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

int aggregate_mif(const std::string& eblif_file_path,
                  MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage) {
  out_aggregated_storage.clear();
  if (eblif_file_path.empty()) {
    VTR_LOG_ERROR("aggregate_mif: empty Yosys eblif file path\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const int read_status =
    read_mif(eblif_file_path, logical_storage, bitstream_setting);
  if (CMD_EXEC_SUCCESS != read_status) {
    return read_status;
  }
  return aggregate_mif(logical_storage, bitstream_setting,
                       out_aggregated_storage);
}

int aggregate_mif(const MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage) {
  out_aggregated_storage.clear();

  std::vector<MifAddressMapSettingId> all_map_ids;
  for (const MifAddressMapSettingId& id :
       bitstream_setting.mif_address_map_settings()) {
    all_map_ids.push_back(id);
  }
  if (all_map_ids.empty()) {
    VTR_LOG_ERROR("aggregate_mif: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  if (logical_storage.empty()) {
    VTR_LOG("aggregate_mif: empty logical MIF storage; nothing to aggregate\n");
    return CMD_EXEC_SUCCESS;
  }

  /* Group maps by des_pb_type (preserve first-seen order). */
  std::vector<std::string> des_order;
  std::map<std::string, std::vector<MifAddressMapSettingId>> des_to_maps;
  for (const MifAddressMapSettingId& map_id : all_map_ids) {
    const std::string& des =
      bitstream_setting.mif_address_map_des_pb_type(map_id);
    if (des_to_maps.find(des) == des_to_maps.end()) {
      des_order.push_back(des);
    }
    des_to_maps[des].push_back(map_id);
  }

  VTR_LOG(
    "aggregate_mif: remapping %zu logical segment(s) via %zu "
    "mif_address_map(s) into %zu des_pb_type(s)\n",
    logical_storage.num_segments(), all_map_ids.size(), des_order.size());

  std::unordered_set<size_t> used_segments;

  for (const std::string& des_pb_type : des_order) {
    const std::vector<MifAddressMapSettingId>& des_maps =
      des_to_maps.at(des_pb_type);

    BasicPort des_addr_range;
    int des_data_width = 0;
    if (!infer_des_header(des_pb_type, des_maps, bitstream_setting,
                          des_addr_range, des_data_width)) {
      return CMD_EXEC_FATAL_ERROR;
    }

    std::map<uint64_t, uint64_t> phys_data_map;

    for (const MifSegmentId& segment_id : logical_storage.segments()) {
      const std::string& seg_pb = logical_storage.physical_pb(segment_id);
      MifAddressMapSettingId map_id = MifAddressMapSettingId::INVALID();
      std::string src_pb_type = seg_pb;
      if (seg_pb.empty()) {
        if (all_map_ids.size() != 1) {
          VTR_LOG_ERROR(
            "aggregate_mif: segment %zu has no pb_type tag, but %zu "
            "mif_address_map entries exist\n",
            static_cast<size_t>(segment_id), all_map_ids.size());
          return CMD_EXEC_FATAL_ERROR;
        }
        map_id = all_map_ids[0];
        src_pb_type =
          bitstream_setting.mif_address_map_src_pb_type(map_id);
      } else {
        for (const MifAddressMapSettingId& id : des_maps) {
          if (bitstream_setting.mif_address_map_src_pb_type(id) == seg_pb) {
            map_id = id;
            break;
          }
        }
      }
      if (!map_id.is_valid()) {
        continue; /* belongs to another des */
      }

      const MifSourceSettingId src_source_id =
        bitstream_setting.find_mif_source_by_pb_type(src_pb_type);
      if (!src_source_id.is_valid()) {
        VTR_LOG_ERROR(
          "aggregate_mif: src_pb_type '%s' has no mif_source for "
          "address_range sanity check\n",
          src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      const BasicPort src_address_range =
        bitstream_setting.mif_source_address_range(src_source_id);
      if (!src_address_range.is_valid()) {
        VTR_LOG_ERROR(
          "aggregate_mif: invalid address_range on mif_source for '%s'\n",
          src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      const BasicPort src_data_range =
        bitstream_setting.mif_source_data_range(src_source_id);
      if (!src_data_range.is_valid()) {
        VTR_LOG_ERROR(
          "aggregate_mif: invalid data_range on mif_source for '%s'\n",
          src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      const int op_data_width = static_cast<int>(src_data_range.get_width());
      if (op_data_width <= 0) {
        VTR_LOG_ERROR("aggregate_mif: invalid data_range width for '%s'\n",
                      src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      const uint64_t data_mask = width_mask(static_cast<size_t>(op_data_width));

      VTR_LOG("aggregate_mif: segment %zu bound src='%s' -> des='%s'\n",
              static_cast<size_t>(segment_id), src_pb_type.c_str(),
              des_pb_type.c_str());

      const BasicPort& seg_addr_range = logical_storage.addr_range(segment_id);
      if (seg_addr_range.is_valid()) {
        if (!address_in_range(seg_addr_range.get_lsb(), src_address_range) ||
            !address_in_range(seg_addr_range.get_msb(), src_address_range)) {
          VTR_LOG_ERROR(
            "aggregate_mif: segment %zu addr range [%zu:%zu] is outside "
            "mif_source address_range [%zu:%zu] for pb_type '%s'\n",
            static_cast<size_t>(segment_id), seg_addr_range.get_lsb(),
            seg_addr_range.get_msb(), src_address_range.get_lsb(),
            src_address_range.get_msb(), src_pb_type.c_str());
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
            src_address_range.get_msb(), src_pb_type.c_str());
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
            src_pb_type.c_str());
          return CMD_EXEC_FATAL_ERROR;
        }
        if (!remap_logical_word(logical_addr, logical_data, op_data_width,
                                des_data_width, des_addr_range, map_id,
                                bitstream_setting, phys_data_map)) {
          return CMD_EXEC_FATAL_ERROR;
        }
      }

      used_segments.insert(static_cast<size_t>(segment_id));
    }

    if (phys_data_map.empty()) {
      continue;
    }

    const MifSegmentId out_seg = out_aggregated_storage.create_segment();
    out_aggregated_storage.set_segment_physical_pb(out_seg, des_pb_type);
    out_aggregated_storage.set_segment_data_width(out_seg, des_data_width);
    out_aggregated_storage.set_segment_addr_range(out_seg, des_addr_range);

    std::vector<uint64_t> phys_addrs;
    phys_addrs.reserve(phys_data_map.size());
    for (const auto& addr_kv : phys_data_map) {
      phys_addrs.push_back(addr_kv.first);
    }
    std::sort(phys_addrs.begin(), phys_addrs.end());
    for (const uint64_t phys_addr : phys_addrs) {
      out_aggregated_storage.create_memory_line(out_seg, phys_addr,
                                                phys_data_map.at(phys_addr));
    }
  }

  for (const MifSegmentId& segment_id : logical_storage.segments()) {
    if (used_segments.count(static_cast<size_t>(segment_id)) == 0) {
      const std::string& seg_pb = logical_storage.physical_pb(segment_id);
      VTR_LOG_ERROR(
        "aggregate_mif: segment %zu pb_type '%s' matched no mif_address_map "
        "src_pb_type\n",
        static_cast<size_t>(segment_id),
        seg_pb.empty() ? "<untagged>" : seg_pb.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  if (out_aggregated_storage.empty()) {
    VTR_LOG_ERROR("aggregate_mif: no aggregated data produced\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
