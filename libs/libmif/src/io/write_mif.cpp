#include "write_mif.h"

#include <fstream>

#include "aggregated_mif_storage.h"
#include "mif_openfpga_format.h"
#include "vtr_log.h"

namespace openfpga {

int write_mif(const std::string& file_path,
              const AggregatedMifStorage& aggregated_mif_storage) {
  if (aggregated_mif_storage.empty()) {
    VTR_LOG_ERROR("No aggregated MIF data to write\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  std::ofstream ofs(file_path.c_str());
  if (!ofs.is_open()) {
    VTR_LOG_ERROR("Failed to open preload .mem file '%s' for writing\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  serialize_preload_mem(aggregated_mif_storage, ofs);
  if (!ofs.good()) {
    VTR_LOG_ERROR("I/O error while writing preload .mem file '%s'\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
