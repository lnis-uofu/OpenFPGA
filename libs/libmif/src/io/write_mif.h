#pragma once

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Write all segments in storage to a MIF file (overwrite). */
/********************************************************************
 * Write MIF storage to an OpenFPGA .mif file.
 * Address/data lines use hex (0x...) zero-padded to
 * ceil(addr_width/4) and ceil(data_width/4) digits.
 *******************************************************************/
int write_mif(const std::string& file_path, const MifStorage& mif_storage);

} /* namespace openfpga */
