#ifndef READ_MIF_H
#define READ_MIF_H

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Read a MIF file and append parsed segments to storage. */
int read_mif(const std::string& file_path, MifStorage& mif_storage);

/* Write all segments in storage to a MIF file (overwrite). */
int write_mif(const std::string& file_path, const MifStorage& mif_storage);

} /* namespace openfpga */

#endif
