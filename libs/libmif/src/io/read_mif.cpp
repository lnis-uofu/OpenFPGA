#include "read_mif.h"

#include "mif_openfpga_format.h"
#include "vtr_log.h"

namespace openfpga {

int read_mif(const std::string& file_path, MifStorage& mif_storage) {
  const size_t seg_count_before = mif_storage.num_segments();
  const int exec_status = read_init_hex(file_path, mif_storage);
  if (CMD_EXEC_SUCCESS != exec_status) {
    return exec_status;
  }

  const size_t seg_count_after = mif_storage.num_segments();
  for (size_t i = seg_count_before; i < seg_count_after; ++i) {
    mif_storage.set_segment_source_file(MifSegmentId(i), file_path);
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
