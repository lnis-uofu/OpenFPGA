#include "read_mif.h"

#include <fstream>
#include <string>

#include "mif_openfpga_format.h"
#include "vtr_log.h"

namespace openfpga {

/* Read a MIF file line-by-line and append parsed segments to storage. */
int read_mif(const std::string& file_path, MifStorage& mif_storage) {
  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    VTR_LOG_ERROR("Failed to open MIF file '%s' for reading\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  bool has_current_segment = false;
  MifSegmentId current_segment_id;
  size_t line_no = 0;
  size_t total_words = 0;

  std::string raw_line;
  while (std::getline(ifs, raw_line)) {
    ++line_no;
    const int exec_status =
      parse_mif_line(file_path, line_no, raw_line, mif_storage,
                     has_current_segment, current_segment_id, total_words);
    if (CMD_EXEC_SUCCESS != exec_status) {
      return exec_status;
    }
  }

  if (ifs.bad()) {
    VTR_LOG_ERROR("I/O error while reading MIF file '%s'\n", file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  int status =
    finalize_openfpga_mif_parse(mif_storage, has_current_segment,
                                current_segment_id, total_words, file_path);
  return status;
}

/* Write all segments in storage to a MIF file (overwrite). */
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
