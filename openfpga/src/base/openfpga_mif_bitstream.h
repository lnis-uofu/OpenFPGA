#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Locate Yosys output *_yosys_out.eblif by scanning the run cwd.
 * Returns empty string if none / multiple matches. */
std::string find_yosys_eblif_file_path();

/* Aggregate logical MIF into physical preload storage.
 * If logical storage is empty and bitstream setting has eblif mif_source,
 * auto-load Yosys eblif first. */
int aggregate_mif_storage(MifStorage& mif_storage,
                          const BitstreamSetting& bitstream_setting,
                          MifStorage& aggregated_mif_storage);

} /* namespace openfpga */
