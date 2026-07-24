#include "mif_openfpga_format.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "command_exit_codes.h"
#include "mif_io_utils.h"
#include "vtr_log.h"

namespace openfpga {

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

static void serialize_preload_mem_segment(const MifStorage& storage,
                                          const MifSegmentId& segment_id,
                                          std::ostream& os) {
  os << "// " << MIF_PRELOAD_MEM_TITLE << "\n";
  if (storage.has_addr_range(segment_id)) {
    os << "// addr[" << storage.min_addr(segment_id) << ":"
       << storage.max_addr(segment_id) << "]\n";
  }
  os << "// Address width: " << storage.addr_width(segment_id) << "\n";
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

void serialize_preload_mem(const MifStorage& storage, std::ostream& os) {
  bool first = true;
  for (const MifSegmentId& segment_id : storage.segments()) {
    if (!first) {
      os << "\n";
    }
    first = false;
    serialize_preload_mem_segment(storage, segment_id, os);
  }
}

int read_init_hex(const std::string& file_path, MifStorage& mif_storage) {
  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    VTR_LOG_ERROR("Failed to open init.hex file '%s' for reading\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  const MifSegmentId segment_id = mif_storage.create_segment();
  size_t line_no = 0;
  size_t total_words = 0;
  uint64_t next_addr = 0;
  bool has_depth_metadata = false;
  uint64_t depth_min_addr = 0;
  uint64_t depth_max_addr = 0;

  std::string raw_line;
  while (std::getline(ifs, raw_line)) {
    ++line_no;

    const std::string line = strip_mif_line_comment(raw_line);
    if (line.empty()) {
      continue;
    }
    if (line.size() >= 2 && line[0] == '/' && line[1] == '/') {
      int parsed_depth = 0;
      uint64_t parsed_min_addr = 0;
      uint64_t parsed_max_addr = 0;
      if (try_parse_init_hex_depth_metadata(line, parsed_depth, parsed_min_addr,
                                            parsed_max_addr)) {
        has_depth_metadata = true;
        depth_min_addr = parsed_min_addr;
        depth_max_addr = parsed_max_addr;
      }
      /* width comments are ignored; mif_source.data_range is authoritative. */
      continue;
    }

    uint64_t addr = 0;
    uint64_t data = 0;
    if (!parse_init_hex_line(line, next_addr, addr, data)) {
      VTR_LOG_ERROR("%s:%lu: cannot parse init.hex line: %s\n",
                    file_path.c_str(), static_cast<unsigned long>(line_no),
                    line.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    mif_storage.create_memory_line(segment_id, addr, data);
    ++total_words;
  }

  if (ifs.bad()) {
    VTR_LOG_ERROR("I/O error while reading init.hex file '%s'\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  if (total_words == 0) {
    mif_storage.remove_last_segment_if_empty();
    VTR_LOG_ERROR("init.hex parse: no memory data lines found in %s\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  /* depth comment is optional. Without it, leave has_addr_range=false;
   * aggregate uses mif_source.address_range as authoritative.
   * Logical segments do not store addr_width/data_width; those are set only
   * on aggregated segments for .mem headers. */
  if (has_depth_metadata) {
    mif_storage.set_segment_addr_range(segment_id, depth_min_addr,
                                       depth_max_addr);
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
