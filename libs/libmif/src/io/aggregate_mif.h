#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"
#include "read_mif.h"

namespace openfpga {

/* Complete and aggregate logical MIF storage.
 *
 * Algorithm:
 *   1. clear the aggregated output and validate address-map availability
 *   2. bind_and_decode in two phases:
 *        - Phase 1: merge eblif into logical once if source="eblif";
 *          else empty logical is an error
 *        - Phase 2: bind every segment and decode remaining raw INIT
 *   3. remap logical memory lines through <map> rules, detecting conflicts
 *   4. emit one aggregated segment for each destination pb_type
 *
 * pb_type_resolver / eblif_file_path are required when bitstream setting has
 * source="eblif". Empty eblif_file_path auto-discovers *_yosys_out.eblif in
 * cwd.
 */
int aggregate_mif(MifStorage& logical_storage,
                  const BitstreamSetting& bitstream_setting,
                  MifStorage& out_aggregated_storage,
                  const MifPbTypeResolver& pb_type_resolver = {},
                  const std::string& eblif_file_path = "");

} /* namespace openfpga */
