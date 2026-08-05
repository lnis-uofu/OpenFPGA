#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "read_mif.h"

namespace openfpga {

/********************************************************************
 * Unified MIF pipeline with staged MifStorage and explicit transforms.
 *
 * Stages:
 *   HEX       - init.hex from read_mif (source="others")
 *   EBLIF     - raw .param INIT from Yosys eblif
 *   LOGICAL   - merged + decoded logical words per mif_source
 *   PHYSICAL  - remapped aggregated preload per destination pb_type
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
                 const MifPbTypeResolver& pb_type_resolver);

  /* Copy HEX into LOGICAL, then merge matching EBLIF segments (eblif wins). */
  int merge_to_logical(const BitstreamSetting& bitstream_setting);

  /* Bind mif_source metadata and decode raw INIT into memory lines. */
  int decode_logical(const BitstreamSetting& bitstream_setting);

  /* Zero-fill undefined address slots in LOGICAL (sparse init / x bits). */
  int pad_logical_zeros(const BitstreamSetting& bitstream_setting);

  /* Remap LOGICAL through mif_address_map into PHYSICAL. */
  int aggregate_to_physical(const BitstreamSetting& bitstream_setting);

  /* Full flow: merge -> decode -> pad -> aggregate. */
  int run(const BitstreamSetting& bitstream_setting,
          const MifPbTypeResolver& pb_type_resolver,
          const std::string& eblif_file_path = "");

  /* Debug dump of any stage via write_mif. */
  int write_stage(const std::string& file_path, Stage stage) const;

 private:
  MifStorage hex_;
  MifStorage eblif_;
  MifStorage logical_;
  MifStorage physical_;
};

/* Deep-copy all segments/lines between two MifStorage objects. */
void copy_mif_storage(const MifStorage& src, MifStorage& dest);

} /* namespace openfpga */
