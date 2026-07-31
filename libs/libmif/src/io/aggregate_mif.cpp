#include "aggregate_mif.h"

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "bitstream_setting_xml_constants.h"
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

static bool unpack_yosys_init_param(
  const std::string& bits, size_t data_width, size_t depth,
  std::vector<std::pair<uint64_t, uint64_t>>& words) {
  words.clear();
  if (data_width == 0 || depth == 0 || data_width > 64 ||
      bits.size() != data_width * depth) {
    VTR_LOG_ERROR(
      "aggregate_mif: INIT length %zu does not match depth %zu x width %zu\n",
      bits.size(), depth, data_width);
    return false;
  }

  /* Split the Yosys bit-vector into address-ordered logical words. */
  for (size_t addr = 0; addr < depth; ++addr) {
    const size_t end = bits.size() - addr * data_width;
    const std::string word_bits = bits.substr(end - data_width, data_width);
    bool all_undefined = true;
    uint64_t data = 0;
    for (const char bit : word_bits) {
      data <<= 1;
      if (bit == '0' || bit == '1') {
        all_undefined = false;
        data |= static_cast<uint64_t>(bit == '1');
      } else if (bit != 'x' && bit != 'X' && bit != 'z' && bit != 'Z') {
        VTR_LOG_ERROR("aggregate_mif: invalid bit in INIT at address %zu\n",
                      addr);
        return false;
      }
    }
    if (!all_undefined) {
      words.emplace_back(static_cast<uint64_t>(addr), data);
    }
  }
  return true;
}

/* Remap one logical word through one address-map rule. Translate its address,
 * move the selected data bits, and reject conflicting destination writes. */
static bool remap_logical_word(
  uint64_t logical_addr, uint64_t logical_data, int des_addr_offset,
  const BasicPort& src_mif_bits, const BasicPort& des_mif_bits,
  std::map<uint64_t, uint64_t>& phys_data_map,
  std::map<uint64_t, uint64_t>& phys_written_mask) {
  /* Translate the logical address and place its selected data bits. */
  const uint64_t des_addr =
    static_cast<uint64_t>(static_cast<int64_t>(logical_addr) + des_addr_offset);
  const uint64_t placed_data =
    place_mif_bits(extract_mif_bits(logical_data, src_mif_bits), des_mif_bits);
  const uint64_t des_mask = width_mask(des_mif_bits.get_width())
                            << des_mif_bits.get_lsb();

  auto& des_data = phys_data_map[des_addr];
  auto& des_written_mask = phys_written_mask[des_addr];
  const uint64_t overlap_mask = des_written_mask & des_mask;
  const uint64_t conflict_mask = (des_data ^ placed_data) & overlap_mask;

  /* Reject different values written to the same destination bits. */
  if (conflict_mask != 0) {
    VTR_LOG_ERROR(
      "aggregate_mif: conflicting writes to des addr %lu bits [%zu:%zu]: "
      "existing 0x%llx vs new 0x%llx (logical addr %lu)\n",
      static_cast<unsigned long>(des_addr), des_mif_bits.get_lsb(),
      des_mif_bits.get_msb(),
      static_cast<unsigned long long>(des_data & overlap_mask),
      static_cast<unsigned long long>(placed_data & overlap_mask),
      static_cast<unsigned long>(logical_addr));
    return false;
  }

  /* Merge the data and record which destination bits are now written. */
  des_data |= placed_data;
  des_written_mask |= des_mask;
  return true;
}

/* Read Yosys eblif into logical_storage.
 * Empty logical: take eblif result as-is (no merge).
 * Non-empty: overwrite matching eblif pb_types; keep source="others". */
static int merge_eblif_into_logical_storage(
  MifStorage& logical_storage, const BitstreamSetting& bitstream_setting,
  const MifPbTypeResolver& pb_type_resolver, const std::string& eblif_path) {
  /* No prior hex/others data: read eblif directly, skip merge. */
  if (logical_storage.empty()) {
    return read_mif_from_eblif(eblif_path, logical_storage, pb_type_resolver);
  }

  MifStorage eblif_storage;
  const int read_status =
    read_mif_from_eblif(eblif_path, eblif_storage, pb_type_resolver);
  if (CMD_EXEC_SUCCESS != read_status) {
    return read_status;
  }

  size_t eblif_segment_count = 0;
  /* If read_mif already loaded the same pb_type, but bitstream setting says
   * source="eblif", the eblif data overwrites the read_mif content. */
  for (const MifSegmentId& eblif_seg : eblif_storage.segments()) {
    if (!bitstream_setting.pb_type_is_eblif_mif_source(
          eblif_storage.physical_pb(eblif_seg))) {
      continue;
    }
    ++eblif_segment_count;

    const std::string& pb = eblif_storage.physical_pb(eblif_seg);
    const std::string& raw = eblif_storage.raw_data(eblif_seg);

    MifSegmentId matched_seg;
    bool found = false;
    for (const MifSegmentId& logical_seg : logical_storage.segments()) {
      if (logical_storage.physical_pb(logical_seg) != pb) {
        continue;
      }
      matched_seg = logical_seg;
      found = true;
      break;
    }

    if (found) {
      VTR_LOG("aggregate_mif: overwrite logical pb_type '%s' from eblif\n",
              pb.c_str());
      logical_storage.clear_segment_memory_lines(matched_seg);
      logical_storage.set_segment_physical_pb(matched_seg, pb);
      logical_storage.set_segment_raw_data(matched_seg, raw);
    } else {
      const MifSegmentId new_seg = logical_storage.create_segment();
      logical_storage.set_segment_physical_pb(new_seg, pb);
      logical_storage.set_segment_raw_data(new_seg, raw);
    }
  }

  if (eblif_segment_count == 0) {
    VTR_LOG_ERROR(
      "aggregate_mif: eblif loaded but no segment matches any mif_source with "
      "source='%s'\n",
      XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

/* Bind one segment to its mif_source ranges; decode raw INIT if present. */
static int decode_logical_segment(MifStorage& logical_storage,
                                  const MifSegmentId& segment_id,
                                  const BitstreamSetting& bitstream_setting) {
  const std::string vpr_pb_type = logical_storage.physical_pb(segment_id);
  const MifSourceSettingId source_id =
    bitstream_setting.find_mif_source_by_pb_type(vpr_pb_type);
  if (!source_id.is_valid()) {
    VTR_LOG_ERROR("aggregate_mif: segment %zu pb_type '%s' has no mif_source\n",
                  static_cast<size_t>(segment_id), vpr_pb_type.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  logical_storage.set_segment_physical_pb(
    segment_id, bitstream_setting.mif_source_pb_type(source_id));
  const BasicPort addr_range =
    bitstream_setting.mif_source_address_range(source_id);
  const BasicPort data_range =
    bitstream_setting.mif_source_data_range(source_id);
  logical_storage.set_segment_addr_range(segment_id, addr_range);
  logical_storage.set_segment_data_width(
    segment_id, static_cast<int>(data_range.get_width()));

  /* Hex segments already have memory lines; eblif still has raw INIT. */
  if (logical_storage.raw_data(segment_id).empty()) {
    return CMD_EXEC_SUCCESS;
  }

  std::vector<std::pair<uint64_t, uint64_t>> words;
  if (!unpack_yosys_init_param(logical_storage.raw_data(segment_id),
                               data_range.get_width(), addr_range.get_width(),
                               words)) {
    return CMD_EXEC_FATAL_ERROR;
  }
  for (const auto& word : words) {
    logical_storage.create_memory_line(
      segment_id, word.first + addr_range.get_lsb(), word.second);
  }
  logical_storage.set_segment_raw_data(segment_id, std::string());
  return CMD_EXEC_SUCCESS;
}

/* Phase 1: merge eblif into logical if needed.
 * Phase 2: bind every segment and decode remaining raw INIT. */
static int bind_and_decode_logical_storage(
  MifStorage& logical_storage, const BitstreamSetting& bitstream_setting,
  const MifPbTypeResolver& pb_type_resolver,
  const std::string& eblif_file_path) {
  /* source="others" (as logical input) needs read_mif data unless eblif
   * can fill logical_storage. source="none" is des-only metadata and is
   * ignored here. */
  if (bitstream_setting.has_other_mif_source() && logical_storage.empty() &&
      !bitstream_setting.has_eblif_mif_source()) {
    VTR_LOG_ERROR(
      "aggregate_mif: logical MIF storage is empty; source='%s' requires "
      "read_mif, or add a source='%s' mif_source\n",
      XML_MIF_SOURCE_SOURCE_OTHERS, XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }

  /* continue to process eblif source */
  if (bitstream_setting.has_eblif_mif_source()) {
    if (!pb_type_resolver) {
      VTR_LOG_ERROR(
        "aggregate_mif: mif_source source='%s' requires a pb_type resolver\n",
        XML_MIF_SOURCE_SOURCE_EBLIF);
      return CMD_EXEC_FATAL_ERROR;
    }
    const std::string path =
      eblif_file_path.empty() ? find_yosys_eblif_file_path() : eblif_file_path;
    if (path.empty()) {
      return CMD_EXEC_FATAL_ERROR;
    }
    const int merge_status = merge_eblif_into_logical_storage(
      logical_storage, bitstream_setting, pb_type_resolver, path);
    if (CMD_EXEC_SUCCESS != merge_status) {
      return merge_status;
    }
  }

  /* ---- Phase 2: bind + decode all logical segments ---- */
  for (const MifSegmentId& segment_id : logical_storage.segments()) {
    const int decode_status =
      decode_logical_segment(logical_storage, segment_id, bitstream_setting);
    if (CMD_EXEC_SUCCESS != decode_status) {
      return decode_status;
    }
  }
  return CMD_EXEC_SUCCESS;
}

int aggregate_mif(MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage,
                  const MifPbTypeResolver& pb_type_resolver,
                  const std::string& eblif_file_path) {
  out_aggregated_storage.clear();
  if (bitstream_setting.mif_address_map_settings().empty()) {
    VTR_LOG_ERROR("aggregate_mif: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  const int bind_status = bind_and_decode_logical_storage(
    logical_storage, bitstream_setting, pb_type_resolver, eblif_file_path);
  if (CMD_EXEC_SUCCESS != bind_status) {
    return bind_status;
  }
  if (logical_storage.empty()) {
    VTR_LOG("aggregate_mif: empty logical MIF storage; nothing to aggregate\n");
    return CMD_EXEC_SUCCESS;
  }

  /* Aggregated physical data, indexed by destination pb_type and address. */
  std::map<std::string, std::map<uint64_t, uint64_t>> des_data_maps;
  /* Bits already written at each destination word, used to detect conflicts
   * including writes that explicitly set a bit to zero. */
  std::map<std::string, std::map<uint64_t, uint64_t>> des_written_masks;

  /* Aggregate each logical MIF segment into its destination pb_type. */
  for (const MifSegmentId& segment_id : logical_storage.segments()) {
    if (!logical_storage.has_physical_pb(segment_id)) {
      VTR_LOG_ERROR(
        "aggregate_mif: segment %zu has no pb_type from its mif_source\n",
        static_cast<size_t>(segment_id));
      return CMD_EXEC_FATAL_ERROR;
    }
    const std::string& src_pb_type = logical_storage.physical_pb(segment_id);
    const MifAddressMapSettingId map_id =
      bitstream_setting.find_mif_address_map_by_src_pb_type(src_pb_type);
    if (!map_id.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: segment %zu pb_type '%s' has no mif_address_map\n",
        static_cast<size_t>(segment_id), src_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const std::string des_pb_type =
      bitstream_setting.mif_address_map_des_pb_type(map_id);
    const BasicPort& src_addr_range = logical_storage.addr_range(segment_id);
    const uint64_t src_data_mask =
      width_mask(logical_storage.data_width(segment_id));
    auto& phys_data_map = des_data_maps[des_pb_type];
    auto& phys_written_mask = des_written_masks[des_pb_type];

    VTR_LOG("aggregate_mif: segment %zu bound src='%s' -> des='%s'\n",
            static_cast<size_t>(segment_id), src_pb_type.c_str(),
            des_pb_type.c_str());

    /* Remap each initialized logical memory word. */
    for (const MifMemoryLineId& line_id :
         logical_storage.segment_memory_lines(segment_id)) {
      const uint64_t logical_addr =
        logical_storage.memory_line_address(line_id);
      const uint64_t logical_data = logical_storage.memory_line_data(line_id);
      if (!address_in_range(logical_addr, src_addr_range) ||
          (logical_data & ~src_data_mask) != 0) {
        VTR_LOG_ERROR(
          "aggregate_mif: invalid address/data in segment %zu for "
          "mif_source pb_type '%s'\n",
          static_cast<size_t>(segment_id), src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }

      bool matched = false;
      /* Apply every map rule covering this logical address. */
      for (const MifAddressMapRuleId& rule_id :
           bitstream_setting.mif_address_map_rules(map_id)) {
        const BasicPort rule_src_addr_range =
          bitstream_setting.mif_address_map_rule_src_addr_range(rule_id);
        if (!address_in_range(logical_addr, rule_src_addr_range)) {
          continue;
        }
        matched = true;
        if (!remap_logical_word(
              logical_addr, logical_data,
              bitstream_setting.mif_address_map_rule_des_addr_offset(rule_id),
              bitstream_setting.mif_address_map_rule_src_mif_bits(rule_id),
              bitstream_setting.mif_address_map_rule_des_mif_bits(rule_id),
              phys_data_map, phys_written_mask)) {
          return CMD_EXEC_FATAL_ERROR;
        }
      }
      if (!matched) {
        VTR_LOG_ERROR(
          "aggregate_mif: logical addr %lu is not covered by any map rule "
          "for src_pb_type '%s'\n",
          static_cast<unsigned long>(logical_addr), src_pb_type.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }

  /* Emit one aggregated MIF segment for each destination pb_type. */
  for (const auto& des_data_map : des_data_maps) {
    if (des_data_map.second.empty()) {
      continue;
    }
    const std::string& des_pb_type = des_data_map.first;
    const MifSourceSettingId des_source_id =
      bitstream_setting.find_mif_source_by_pb_type(des_pb_type);
    if (!des_source_id.is_valid()) {
      VTR_LOG_ERROR(
        "aggregate_mif: des_pb_type '%s' has no matching mif_source\n",
        des_pb_type.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    const BasicPort des_addr_range =
      bitstream_setting.mif_source_address_range(des_source_id);
    const BasicPort des_data_range =
      bitstream_setting.mif_source_data_range(des_source_id);
    const MifSegmentId out_seg = out_aggregated_storage.create_segment();
    out_aggregated_storage.set_segment_physical_pb(out_seg, des_pb_type);
    out_aggregated_storage.set_segment_data_width(
      out_seg, static_cast<int>(des_data_range.get_width()));
    out_aggregated_storage.set_segment_addr_range(out_seg, des_addr_range);

    /* Store aggregated words in destination address order. */
    for (const auto& addr_data : des_data_map.second) {
      out_aggregated_storage.create_memory_line(out_seg, addr_data.first,
                                                addr_data.second);
    }
  }

  if (out_aggregated_storage.empty()) {
    VTR_LOG_ERROR("aggregate_mif: no aggregated data produced\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
