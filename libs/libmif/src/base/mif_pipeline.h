#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "read_mif.h"

namespace openfpga {

/* VPR grid location for a PHYSICAL-stage segment (sub_tile = z). */
struct MifGridCoord {
  int x = -1;
  int y = -1;
  int z = -1;

  bool is_valid() const { return x >= 0 && y >= 0 && z >= 0; }
};

/********************************************************************
 * Unified MIF pipeline with staged MifStorage and explicit transforms.
 *
 * Stages:
 *   HEX       - init.hex from read_mif (source="others")
 *   EBLIF     - raw .param INIT from Yosys eblif
 *   LOGICAL   - merged + decoded logical words per mif_source
 *   PHYSICAL  - remapped aggregated preload per destination pb_type
 *
 * Aggregated physical MIF is stored as physical_. Its corresponding
 * placement coordinates (x, y, sub_tile) are also stored in this
 * pipeline (physical_segment_grid_coords_), parallel to physical_
 * segments.
 ********************************************************************/
class MifPipeline {
 public:
  enum class Stage { HEX, EBLIF, LOGICAL, PHYSICAL };

 public:
  const MifStorage& storage(Stage stage) const;
  MifStorage& mutable_storage(Stage stage);

  void clear();
  void clear(Stage stage);

  /* Load Yosys eblif .param INIT into the EBLIF stage. */
  int load_eblif(const std::string& eblif_path,
                 const BitstreamSetting& bitstream_setting,
                 const AtomContext& atom_ctx, const DeviceContext& device_ctx,
                 const MifPbTypeResolver& pb_type_resolver);

  /* Merge HEX/EBLIF into LOGICAL by mif_source: eblif pb_types use EBLIF
   * only; others use HEX only (no concatenation or overwrite). */
  int merge_to_logical(const BitstreamSetting& bitstream_setting);

  /* Bind mif_source metadata and decode raw INIT into memory lines. */
  int decode_logical(const BitstreamSetting& bitstream_setting);

  /* Zero-fill undefined address slots in LOGICAL (sparse init / x bits). */
  int pad_logical_zeros(const BitstreamSetting& bitstream_setting);

  /* Remap LOGICAL through mif_address_map into physical_. */
  int aggregate_to_physical(const BitstreamSetting& bitstream_setting);

  /* Debug dump of any stage via write_mif. */
  int write_stage(const std::string& file_path, Stage stage) const;

  /* Placement coords for physical_ segments (must pass physical_ ids). */
  bool physical_segment_has_grid_coord(const MifSegmentId& segment_id) const;
  int physical_segment_grid_x(const MifSegmentId& segment_id) const;
  int physical_segment_grid_y(const MifSegmentId& segment_id) const;
  int physical_segment_grid_z(const MifSegmentId& segment_id) const;
  void set_physical_segment_grid_coord(const MifSegmentId& segment_id, int x,
                                       int y, int z);

 private:
  void clear_physical_grid_coords();

  MifStorage hex_;
  MifStorage eblif_;
  MifStorage logical_;
  MifStorage physical_; /* Aggregated physical MIF */
  /* Parallel to physical_ only; indexed by physical_ segment id.
   * Do not index with HEX/EBLIF/LOGICAL segment ids (same StrongId type,
   * but each stage has its own id space). */
  vtr::vector<MifSegmentId, MifGridCoord> physical_segment_grid_coords_;
};

/* Deep-copy all segments/lines between two MifStorage objects. */
void copy_mif_storage(const MifStorage& src, MifStorage& dest);

} /* namespace openfpga */
