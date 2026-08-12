#include "annotate_mif.h"

#include "bitstream_setting_xml_constants.h"
#include "command_exit_codes.h"
#include "mif_vpr_placement.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Build LOGICAL then PHYSICAL MIF for the given bitstream setting.
 *
 * Aggregated physical MIF is stored as physical_ in MifPipeline.
 * Its corresponding placement coords are also stored in MifPipeline.
 *
 * Stage pb semantics:
 *   EBLIF/LOGICAL - operating pb paths (match mif_source /
 *                   mif_address_map src_pb_type strings)
 *   PHYSICAL      - aggregated des_pb_type from mif_address_map
 *
 * Why eblif binding does NOT use VprDeviceAnnotation:
 *   VprDeviceAnnotation maps operating t_pb_type* -> physical t_pb_type*.
 *   Eblif load needs packed operating instances from VPR atom/pack
 *   so segments can match XML mif_source and address-map keys. Device
 *   annotation has no atom->pb instance binding.
 *******************************************************************/
int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline, const AtomContext& atom_ctx,
                       const PlacementContext& place_ctx) {
  const bool has_mif_setting =
    !bitstream_setting.mif_source_settings().empty() ||
    !bitstream_setting.mif_address_map_settings().empty();
  if (!has_mif_setting) {
    return CMD_EXEC_SUCCESS;
  }

  if (bitstream_setting.has_other_mif_source() &&
      mif_pipeline.storage(MifPipeline::Stage::HEX).empty() &&
      !bitstream_setting.has_eblif_mif_source()) {
    VTR_LOG_ERROR(
      "build_physical_mif: hex MIF storage is empty; source='%s' requires "
      "read_mif, or add a source='%s' mif_source\n",
      XML_MIF_SOURCE_SOURCE_OTHERS, XML_MIF_SOURCE_SOURCE_EBLIF);
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Operating pb from VPR pack (not VprDeviceAnnotation). */
  if (bitstream_setting.has_eblif_mif_source()) {
    const int load_status =
      mif_pipeline.load_eblif(bitstream_setting, atom_ctx);
    if (CMD_EXEC_SUCCESS != load_status) {
      return load_status;
    }
  }
  /* merge mif collected from two sources: read_mif command and eblif source */
  int status = mif_pipeline.merge_to_logical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }
  if (mif_pipeline.storage(MifPipeline::Stage::LOGICAL).empty()) {
    VTR_LOG(
      "build_physical_mif: empty logical MIF storage; nothing to "
      "aggregate\n");
    return CMD_EXEC_SUCCESS;
  }

  status = mif_pipeline.decode_logical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }

  /*aggregate to physical_; result stored in mif_pipeline.physical_ */
  status = mif_pipeline.aggregate_to_physical(bitstream_setting);
  if (CMD_EXEC_SUCCESS != status) {
    return status;
  }

  /* Annotate placement coords into the same MifPipeline as physical_. */
  status = annotate_physical_mif_grid_coordinates(
    mif_pipeline, bitstream_setting, atom_ctx, place_ctx);
  return status;
}

} /* end namespace openfpga */
