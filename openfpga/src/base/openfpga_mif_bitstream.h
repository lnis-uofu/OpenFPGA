#pragma once

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Aggregate logical MIF into physical preload storage.
 * When bitstream setting has source="eblif", aggregate_mif calls read_mif. */
int aggregate_mif_storage(MifStorage& mif_storage,
                          const BitstreamSetting& bitstream_setting,
                          MifStorage& aggregated_mif_storage);

} /* namespace openfpga */
