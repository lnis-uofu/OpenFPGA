#include "openfpga_mif_bitstream.h"

#include <cctype>
#include <filesystem>
#include <string>

#include "aggregate_mif.h"
#include "bitstream_setting_xml_constants.h"
#include "mif_vpr_placement.h"
#include "read_mif.h"
#include "vtr_log.h"

namespace openfpga {

std::string find_yosys_eblif_file_path() {
  /* Yosys flow writes <top>_yosys_out.eblif into the run cwd. */
  constexpr const char* k_suffix = "_yosys_out.eblif";
  std::string found;

  for (const std::filesystem::directory_entry& entry :
       std::filesystem::directory_iterator(".")) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const std::string name = entry.path().filename().string();
    if (!name.ends_with(k_suffix)) {
      continue;
    }
    if (!found.empty()) {
      VTR_LOG_ERROR("Cannot locate Yosys eblif: multiple '*%s' in cwd\n",
                    k_suffix);
      return std::string();
    }
    found = name;
  }

  if (found.empty()) {
    VTR_LOG_ERROR("Cannot locate Yosys eblif: no '*%s' in cwd\n", k_suffix);
    return std::string();
  }

  VTR_LOG("Located Yosys eblif '%s'\n", found.c_str());
  return found;
}

static bool bitstream_has_eblif_mif_source(
  const BitstreamSetting& bitstream_setting) {
  for (const MifSourceSettingId& id : bitstream_setting.mif_source_settings()) {
    if (bitstream_setting.mif_source_source(id) ==
        XML_MIF_SOURCE_SOURCE_EBLIF) {
      return true;
    }
  }
  return false;
}

/* Match VPR instance pb path to type-only mif_source pb_type (strip [N]). */
static bool pb_type_matches_mif_source(const std::string& vpr_pb_type,
                                       const std::string& source_pb_type) {
  if (vpr_pb_type == source_pb_type) {
    return true;
  }
  if (vpr_pb_type.empty() || vpr_pb_type.back() != ']') {
    return false;
  }
  const size_t bracket_pos = vpr_pb_type.rfind('[');
  if (bracket_pos == std::string::npos ||
      bracket_pos + 1 >= vpr_pb_type.size() - 1) {
    return false;
  }
  for (size_t i = bracket_pos + 1; i + 1 < vpr_pb_type.size(); ++i) {
    if (!std::isdigit(static_cast<unsigned char>(vpr_pb_type[i]))) {
      return false;
    }
  }
  return vpr_pb_type.substr(0, bracket_pos) == source_pb_type;
}

static bool pb_type_is_eblif_mif_source(
  const std::string& pb_type, const BitstreamSetting& bitstream_setting) {
  for (const MifSourceSettingId& id : bitstream_setting.mif_source_settings()) {
    if (bitstream_setting.mif_source_source(id) !=
        XML_MIF_SOURCE_SOURCE_EBLIF) {
      continue;
    }
    if (pb_type_matches_mif_source(pb_type,
                                   bitstream_setting.mif_source_pb_type(id))) {
      return true;
    }
  }
  return false;
}

/* Load Yosys eblif and overwrite any logical segments whose pb_type is bound
 * to source="eblif". Segments bound to source="others" are kept. */
static int load_or_overwrite_eblif_mif_sources(
  MifStorage& mif_storage, const BitstreamSetting& bitstream_setting) {
  const std::string eblif_file_path = find_yosys_eblif_file_path();
  if (eblif_file_path.empty()) {
    return CMD_EXEC_FATAL_ERROR;
  }

  MifStorage eblif_storage;
  const int read_status =
    read_mif(eblif_file_path, eblif_storage,
             [](const std::string& model_name,
                const MifEblifPortConnections& port_connections) {
               return get_mif_pb_type_from_vpr(model_name, port_connections);
             });
  if (CMD_EXEC_SUCCESS != read_status) {
    return read_status;
  }

  if (mif_storage.empty()) {
    for (const MifSegmentId& segment_id : eblif_storage.segments()) {
      if (!pb_type_is_eblif_mif_source(eblif_storage.physical_pb(segment_id),
                                       bitstream_setting)) {
        continue;
      }
      mif_storage.append_segment_copy(eblif_storage, segment_id);
    }
    if (mif_storage.empty()) {
      VTR_LOG_ERROR(
        "aggregate_mif_storage: eblif loaded but no segment matches any "
        "mif_source with source='%s'\n",
        XML_MIF_SOURCE_SOURCE_EBLIF);
      return CMD_EXEC_FATAL_ERROR;
    }
    return CMD_EXEC_SUCCESS;
  }

  /* Keep others; replace eblif-bound pb_types with freshly read eblif data. */
  MifStorage merged;
  for (const MifSegmentId& segment_id : mif_storage.segments()) {
    if (pb_type_is_eblif_mif_source(mif_storage.physical_pb(segment_id),
                                    bitstream_setting)) {
      VTR_LOG(
        "aggregate_mif_storage: overwrite logical pb_type '%s' from eblif\n",
        mif_storage.physical_pb(segment_id).c_str());
      continue;
    }
    merged.append_segment_copy(mif_storage, segment_id);
  }
  for (const MifSegmentId& segment_id : eblif_storage.segments()) {
    if (!pb_type_is_eblif_mif_source(eblif_storage.physical_pb(segment_id),
                                     bitstream_setting)) {
      continue;
    }
    merged.append_segment_copy(eblif_storage, segment_id);
  }
  mif_storage = std::move(merged);
  return CMD_EXEC_SUCCESS;
}

int aggregate_mif_storage(MifStorage& mif_storage,
                          const BitstreamSetting& bitstream_setting,
                          MifStorage& aggregated_mif_storage) {
  aggregated_mif_storage.clear();

  if (bitstream_setting.mif_address_map_settings().empty()) {
    VTR_LOG_ERROR(
      "aggregate_mif_storage: no mif_address_map in bitstream setting\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  if (bitstream_has_eblif_mif_source(bitstream_setting)) {
    const int eblif_status =
      load_or_overwrite_eblif_mif_sources(mif_storage, bitstream_setting);
    if (CMD_EXEC_SUCCESS != eblif_status) {
      return eblif_status;
    }
  }

  return aggregate_mif(mif_storage, bitstream_setting, aggregated_mif_storage);
}

} /* namespace openfpga */
