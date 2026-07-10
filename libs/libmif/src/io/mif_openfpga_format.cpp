#include "mif_openfpga_format.h"

#include <cctype>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ostream>
#include <sstream>

#include "command_exit_codes.h"
#include "mif_text_utils.h"
#include "vtr_log.h"

namespace openfpga {

static int hex_digits_for_width(int width_bits) {
  if (width_bits <= 0) {
    return 0;
  }
  return static_cast<int>((static_cast<unsigned>(width_bits) + 3u) / 4u);
}

static void print_hex_with_width(std::ostream& os, uint64_t v, int width_bits) {
  int nd = hex_digits_for_width(width_bits);
  if (nd <= 0) {
    os << "0x" << std::hex << v;
  } else {
    os << "0x" << std::hex << std::setw(nd) << std::setfill('0') << v;
  }
  os << std::dec << std::setfill(' ');
}

void serialize_openfpga_mif(const MifStorage& storage, std::ostream& os) {
  os << "// This is a comment\n";
  os << "// All the address and data in HEX format\n";

  bool first = true;
  for (const MifSegmentId& segment_id : storage.segments()) {
    if (!first) {
      os << "\n";
    }
    first = false;

    if (storage.has_xy(segment_id)) {
      os << "// " << MIF_DIRECTIVE_X << " " << storage.coord_x(segment_id)
         << " // Coordinates of the RAM block\n";
      os << "// " << MIF_DIRECTIVE_Y << " " << storage.coord_y(segment_id)
         << " // Coordinates of the RAM block\n";
    }
    if (!storage.has_ram_id(segment_id)) {
      if (storage.addr_width(segment_id) >= 0) {
        os << "// " << MIF_DIRECTIVE_ADDR_WIDTH << " "
           << storage.addr_width(segment_id) << "\n";
      }
      if (storage.data_width(segment_id) >= 0) {
        os << "// " << MIF_DIRECTIVE_DATA_WIDTH << " "
           << storage.data_width(segment_id) << "\n";
      }
    } else {
      os << "//" << MIF_DIRECTIVE_RAM_ID << " " << storage.ram_id(segment_id)
         << "\n";
      if (storage.id_width(segment_id) >= 0) {
        os << "//" << MIF_DIRECTIVE_ID_WIDTH << " "
           << storage.id_width(segment_id) << "\n";
      }
      if (storage.addr_width(segment_id) >= 0) {
        os << "// " << MIF_DIRECTIVE_ADDR_WIDTH << " "
           << storage.addr_width(segment_id) << "\n";
      }
      if (storage.data_width(segment_id) >= 0) {
        os << "// " << MIF_DIRECTIVE_DATA_WIDTH << " "
           << storage.data_width(segment_id) << "\n";
      }
    }
    os << "// Address Data \n";
    for (const MifMemoryLineId& memory_line_id :
         storage.segment_memory_lines(segment_id)) {
      print_hex_with_width(os, storage.memory_line_address(memory_line_id),
                           storage.addr_width(segment_id));
      os << " ";
      print_hex_with_width(os, storage.memory_line_data(memory_line_id),
                           storage.data_width(segment_id));
      os << "\n";
    }
  }
}

/* Parse a token as an unsigned 64-bit integer (decimal or hex). */
static bool parse_hex_u64(const std::string& tok, uint64_t& out) {
  return parse_mif_u64_token(tok, out);
}

/* Return true if the line is exactly one 'address data' hex pair. */
static bool try_address_data_line(const std::string& line, uint64_t& addr,
                                  uint64_t& data) {
  std::istringstream iss(line);
  std::string ta;
  std::string td;
  if (!(iss >> ta >> td)) {
    return false;
  }
  std::string extra;
  if (iss >> extra) {
    return false;
  }
  return parse_hex_u64(ta, addr) && parse_hex_u64(td, data);
}

static bool segment_has_content(const MifStorage& mif_storage,
                                const MifSegmentId& segment_id) {
  return !mif_storage.segment_memory_lines(segment_id).empty() ||
         mif_storage.has_xy(segment_id) || mif_storage.has_ram_id(segment_id);
}

/* Create a segment when the first recognized // directive is seen. */
static void ensure_current_segment(MifStorage& mif_storage,
                                   bool& has_current_segment,
                                   MifSegmentId& current_segment_id) {
  if (!has_current_segment) {
    current_segment_id = mif_storage.create_segment();
    has_current_segment = true;
  }
}

/* Open or reset a segment and assign RAM_ID for a new RAM block section. */
static void start_new_segment_for_ram_id(MifStorage& mif_storage,
                                         bool& has_current_segment,
                                         MifSegmentId& current_segment_id,
                                         int ram_id) {
  if (!has_current_segment) {
    current_segment_id = mif_storage.create_segment();
    has_current_segment = true;
  } else if (!mif_storage.segment_memory_lines(current_segment_id).empty() ||
             mif_storage.has_xy(current_segment_id)) {
    current_segment_id = mif_storage.create_segment();
  } else {
    mif_storage.reset_segment(current_segment_id);
  }
  mif_storage.set_segment_ram_id(current_segment_id, ram_id);
}

/* Parse the body of a '//' directive line and update the current segment. */
static int parse_mif_directive(const std::string& file_path, size_t line_no,
                               const std::string& directive,
                               MifStorage& mif_storage,
                               bool& has_current_segment,
                               MifSegmentId& current_segment_id) {
  std::istringstream ls(directive);
  std::string key;
  if (!(ls >> key)) {
    return CMD_EXEC_SUCCESS;
  }

  if (key == MIF_DIRECTIVE_RAM_ID) {
    int ram_id = 0;
    if (!(ls >> ram_id)) {
      VTR_LOG_ERROR("%s:%lu: expected integer after //%s\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no), MIF_DIRECTIVE_RAM_ID);
      return CMD_EXEC_FATAL_ERROR;
    }
    start_new_segment_for_ram_id(mif_storage, has_current_segment,
                                 current_segment_id, ram_id);
    return CMD_EXEC_SUCCESS;
  }

  int value = 0;
  if (key == MIF_DIRECTIVE_X) {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // %s directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no), MIF_DIRECTIVE_X);
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment,
                           current_segment_id);
    mif_storage.set_segment_coord_x(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == MIF_DIRECTIVE_Y) {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // %s directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no), MIF_DIRECTIVE_Y);
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment,
                           current_segment_id);
    mif_storage.set_segment_coord_y(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == MIF_DIRECTIVE_ADDR_WIDTH) {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // %s directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no),
                    MIF_DIRECTIVE_ADDR_WIDTH);
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment,
                           current_segment_id);
    mif_storage.set_segment_addr_width(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == MIF_DIRECTIVE_DATA_WIDTH) {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // %s directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no),
                    MIF_DIRECTIVE_DATA_WIDTH);
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment,
                           current_segment_id);
    mif_storage.set_segment_data_width(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == MIF_DIRECTIVE_ID_WIDTH) {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // %s directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no),
                    MIF_DIRECTIVE_ID_WIDTH);
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment,
                           current_segment_id);
    mif_storage.set_segment_id_width(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }

  /* Unrecognized // line: treat as comment */
  return CMD_EXEC_SUCCESS;
}

int parse_mif_line(const std::string& file_path, size_t line_no,
                   const std::string& raw_line, MifStorage& mif_storage,
                   bool& has_current_segment, MifSegmentId& current_segment_id,
                   size_t& total_words) {
  const std::string line = strip_mif_line_comment(raw_line);
  if (line.empty()) {
    return CMD_EXEC_SUCCESS;
  }

  if (line.size() >= 2 && line[0] == '/' && line[1] == '/') {
    return parse_mif_directive(file_path, line_no, line.substr(2), mif_storage,
                               has_current_segment, current_segment_id);
  }

  uint64_t addr = 0;
  uint64_t data = 0;
  if (try_address_data_line(line, addr, data)) {
    if (!has_current_segment) {
      VTR_LOG_ERROR("%s:%lu: address/data line before any // directive\n",
                    file_path.c_str(), static_cast<unsigned long>(line_no));
      return CMD_EXEC_FATAL_ERROR;
    }
    mif_storage.create_memory_line(current_segment_id, addr, data);
    ++total_words;
    return CMD_EXEC_SUCCESS;
  }

  VTR_LOG_ERROR("%s:%lu: cannot parse line: %s\n", file_path.c_str(),
                static_cast<unsigned long>(line_no), line.c_str());
  return CMD_EXEC_FATAL_ERROR;
}

int finalize_openfpga_mif_parse(MifStorage& mif_storage,
                                bool has_current_segment,
                                const MifSegmentId& current_segment_id,
                                size_t total_words,
                                const std::string& file_path) {
  if (has_current_segment &&
      !segment_has_content(mif_storage, current_segment_id)) {
    mif_storage.remove_last_segment_if_empty();
  }

  if (total_words == 0) {
    VTR_LOG_ERROR(
      "OpenFPGA MIF parse: no 'address data' hex lines found in %s\n",
      file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
