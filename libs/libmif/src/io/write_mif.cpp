#include "write_mif.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "mif_storage_fwd.h"
#include "openfpga_port.h"
#include "vtr_log.h"

namespace openfpga {

static constexpr const char* kPreloadMemTitle =
  "Aggregated MIF for memory preloading interface";

static int hex_digits_for_width(int width_bits) {
  if (width_bits <= 0) {
    return 0;
  }
  return (width_bits + 3) / 4;
}

static std::string format_hex_word(uint64_t value, int width_bits) {
  const int nd = hex_digits_for_width(width_bits);
  std::ostringstream hex_ss;
  hex_ss << std::hex << std::nouppercase << std::setfill('0');
  if (nd > 0) {
    hex_ss << std::setw(nd) << value;
  } else {
    hex_ss << value;
  }
  return hex_ss.str();
}

static void write_mif_segment(const MifStorage& storage,
                              const MifSegmentId& segment_id,
                              std::ostream& os) {
  os << "// " << kPreloadMemTitle << "\n";
  const BasicPort& address_port = storage.addr_range(segment_id);
  if (address_port.is_valid()) {
    os << "// " << address_port.to_verilog_string() << "\n";
  }
  os << "// Data width: " << storage.data_width(segment_id) << "\n";
  os << "// PB_TYPE " << storage.physical_pb(segment_id) << "\n";

  std::vector<uint64_t> addrs;
  for (const MifMemoryLineId& memory_line_id :
       storage.segment_memory_lines(segment_id)) {
    addrs.push_back(storage.memory_line_address(memory_line_id));
  }
  std::sort(addrs.begin(), addrs.end());

  for (const uint64_t addr : addrs) {
    for (const MifMemoryLineId& memory_line_id :
         storage.segment_memory_lines(segment_id)) {
      if (storage.memory_line_address(memory_line_id) != addr) {
        continue;
      }
      os << addr << " 0x"
         << format_hex_word(storage.memory_line_data(memory_line_id),
                            storage.data_width(segment_id))
         << "\n";
      break;
    }
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
