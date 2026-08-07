#include "write_mif.h"

#include <fstream>
#include <ostream>
#include <string>

#include "mif_storage_fwd.h"
#include "openfpga_decode.h"
#include "openfpga_port.h"
#include "vtr_log.h"

namespace openfpga {

static void write_mif_segment(const MifStorage& storage,
                              const MifSegmentId& segment_id,
                              std::ostream& os) {
  os << "// " << K_PRELOAD_MEM_TITLE << "\n";
  const BasicPort& address_port = storage.addr_range(segment_id);
  if (address_port.is_valid()) {
    os << "// " << address_port.to_verilog_string() << "\n";
  }
  os << "// Data width: " << storage.data_width(segment_id) << "\n";
  os << "// PB_TYPE " << storage.physical_pb(segment_id) << "\n";

  /* $readmemh @addr allows sparse / out-of-order writes; no sort needed. */
  for (const MifMemoryLineId& memory_line_id :
       storage.segment_memory_lines(segment_id)) {
    os << "@" << storage.memory_line_address(memory_line_id) << " "
       << format_hex_word(storage.memory_line_data(memory_line_id),
                          storage.data_width(segment_id))
       << "\n";
  }
}

static void write_mif_to_stream(const MifStorage& storage, std::ostream& os) {
  bool first = true;
  for (const MifSegmentId& segment_id : storage.segments()) {
    if (!first) {
      os << "\n";
    }
    first = false;
    write_mif_segment(storage, segment_id, os);
  }
}

int write_mif(const std::string& file_path,
              const MifStorage& aggregated_mif_storage) {
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

  write_mif_to_stream(aggregated_mif_storage, ofs);
  if (!ofs.good()) {
    VTR_LOG_ERROR("I/O error while writing preload .mem file '%s'\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
