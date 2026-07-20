#include "read_mif.h"

#include <fstream>
#include <string>

#include "mif_openfpga_format.h"
#include "vtr_log.h"

namespace openfpga {

/* Read a MIF file line-by-line and append parsed segments to storage. */
int read_mif(const std::string& file_path, MifStorage& mif_storage) {
  int exec_status = CMD_EXEC_FATAL_ERROR;
  const size_t seg_count_before = mif_storage.num_segments();
  if (is_init_hex_file(file_path)) {
    exec_status = read_init_hex(file_path, mif_storage);
  } else {
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
      exec_status =
        parse_mif_line(file_path, line_no, raw_line, mif_storage,
                       has_current_segment, current_segment_id, total_words);
      if (CMD_EXEC_SUCCESS != exec_status) {
        return exec_status;
      }
    }

    if (ifs.bad()) {
      VTR_LOG_ERROR("I/O error while reading MIF file '%s'\n",
                    file_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    exec_status = finalize_openfpga_mif_parse(
      mif_storage, has_current_segment, current_segment_id, total_words,
      file_path);
  }

  if (CMD_EXEC_SUCCESS == exec_status) {
    const size_t seg_count_after = mif_storage.num_segments();
    for (size_t i = seg_count_before; i < seg_count_after; ++i) {
      mif_storage.set_segment_source_file(MifSegmentId(i), file_path);
    }
    mif_storage.add_source_file(file_path);
  }
  return exec_status;
}

} /* namespace openfpga */
