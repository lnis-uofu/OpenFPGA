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
 *   0. if any mif_source has source="eblif", call read_mif_from_eblif
 *      (overwrite eblif-bound pb_types; keep source="others")
 *   1. clear the aggregated output and validate address-map availability
 *   2. bind each logical segment's VPR pb_type to its mif_source
 *   3. persist source address range/data width in logical_storage
 *   4. decode raw eblif INIT into logical memory lines and clear the raw data
 *   5. remap logical memory lines through <map> rules, detecting conflicts
 *   6. emit one aggregated segment for each destination pb_type
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
