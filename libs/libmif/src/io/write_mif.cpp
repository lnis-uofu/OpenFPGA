#include "write_mif.h"

#include <fstream>

#include "mif_format.h"
#include "vtr_log.h"

namespace openfpga {

int write_mif(const std::string& file_path, const MifStorage& mif_storage) {
  if (mif_storage.empty()) {
    VTR_LOG_ERROR("No parsed MIF segments to write\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  std::ofstream ofs(file_path.c_str());
  if (!ofs.is_open()) {
    VTR_LOG_ERROR("Failed to open MIF file '%s' for writing\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  serialize_openfpga_mif(mif_storage, ofs);
  if (!ofs.good()) {
    VTR_LOG_ERROR("I/O error while writing MIF file '%s'\n", file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
