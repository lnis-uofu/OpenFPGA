#include "annotate_mif.h"

#include "bitstream_setting_xml_constants.h"
#include "command_exit_codes.h"
#include "mif_vpr_placement.h"
#include "physical_pb.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

static size_t load_repacked_mif_data(
  MifPipeline& mif_pipeline,
  const VprClusteringAnnotation& clustering_annotation) {
  MifStorage& eblif_storage =
    mif_pipeline.mutable_storage(MifPipeline::Stage::EBLIF);
  eblif_storage.clear();
  size_t num_mif_data = 0;
  for (const auto& cluster_physical_pb : clustering_annotation.physical_pbs()) {
    const PhysicalPb& physical_pb = cluster_physical_pb.second;
    for (const PhysicalPbId& physical_pb_id : physical_pb.primitive_pbs()) {
      for (const PhysicalPb::MifDataInfo& mif_data :
           physical_pb.mif_data(physical_pb_id)) {
        if (mif_data.source != XML_MIF_SOURCE_SOURCE_EBLIF) {
          continue;
        }
        const MifSegmentId segment_id = eblif_storage.create_segment();
        eblif_storage.set_segment_physical_pb(segment_id,
                                              mif_data.operating_pb_path);
        eblif_storage.set_segment_raw_data(segment_id, mif_data.value);
        ++num_mif_data;
      }
    }
  }
  return num_mif_data;
}

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
 * EBLIF values and operating pb paths were cached in PhysicalPb by repack.
 * This stage consumes only OpenFPGA clustering and placement annotations.
 *******************************************************************/
int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline,
                       const VprClusteringAnnotation& clustering_annotation,
                       const VprPlacementAnnotation& placement_annotation) {
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

  if (bitstream_setting.has_eblif_mif_source()) {
    if (load_repacked_mif_data(mif_pipeline, clustering_annotation) == 0) {
      VTR_LOG_ERROR(
        "build_physical_mif: no EBLIF-backed MIF data found in repacked "
        "PhysicalPb annotations\n");
      return CMD_EXEC_FATAL_ERROR;
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
    mif_pipeline, bitstream_setting, clustering_annotation,
    placement_annotation);
  return status;
}

} /* end namespace openfpga */
