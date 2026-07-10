#include "mif_format.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>

#include "command_exit_codes.h"
#include "mif_io_utils.h"
#include "vtr_log.h"

namespace openfpga {

/********************************************************************
 * OpenFPGA MIF — serialization
 ********************************************************************/

static int hex_digits_for_width(int width_bits) {
  if (width_bits <= 0) {
    return 0;
  }
  return static_cast<int>((static_cast<unsigned>(width_bits) + 3u) / 4u);
}

static void print_hex_with_width(std::ostream& os, uint64_t v, int width_bits) {
  const int nd = hex_digits_for_width(width_bits);
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

  bool first_segment = true;
  for (const MifSegmentId& segment_id : storage.segments()) {
    if (!first_segment) {
      os << "\n";
    }
    first_segment = false;

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

/********************************************************************
 * OpenFPGA MIF — parsing
 *
 * Each segment is opened by the first recognized // directive. RAM_ID
 * starts a new block; address/data lines must appear after a directive
 * created the current segment.
 ********************************************************************/

/* True when the line is exactly one 0x-prefixed address + data pair. */
static bool is_openfpga_address_data_line(const std::string& line) {
  std::istringstream iss(line);
  std::string addr_tok;
  std::string data_tok;
  if (!(iss >> addr_tok >> data_tok)) {
    return false;
  }
  std::string extra;
  if (iss >> extra) {
    return false;
  }
  return addr_tok.size() >= 2 && addr_tok[0] == '0' &&
         (addr_tok[1] == 'x' || addr_tok[1] == 'X');
}

static bool try_openfpga_address_data_line(const std::string& line,
                                           uint64_t& addr, uint64_t& data) {
  std::istringstream iss(line);
  std::string addr_tok;
  std::string data_tok;
  if (!(iss >> addr_tok >> data_tok)) {
    return false;
  }
  std::string extra;
  if (iss >> extra) {
    return false;
  }
  return parse_mif_u64_token(addr_tok, addr) &&
         parse_mif_u64_token(data_tok, data);
}

static bool segment_has_content(const MifStorage& mif_storage,
                                const MifSegmentId& segment_id) {
  return !mif_storage.segment_memory_lines(segment_id).empty() ||
         mif_storage.has_xy(segment_id) || mif_storage.has_ram_id(segment_id);
}

static void ensure_current_segment(MifStorage& mif_storage,
                                   bool& has_current_segment,
                                   MifSegmentId& current_segment_id) {
  if (!has_current_segment) {
    current_segment_id = mif_storage.create_segment();
    has_current_segment = true;
  }
}

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

static int parse_mif_directive(const std::string& file_path, size_t line_no,
                               const std::string& directive_body,
                               MifStorage& mif_storage,
                               bool& has_current_segment,
                               MifSegmentId& current_segment_id) {
  std::istringstream iss(directive_body);
  std::string key;
  if (!(iss >> key)) {
    return CMD_EXEC_SUCCESS;
  }

  if (key == MIF_DIRECTIVE_RAM_ID) {
    int ram_id = 0;
    if (!(iss >> ram_id)) {
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
    if (!(iss >> value)) {
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
    if (!(iss >> value)) {
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
    if (!(iss >> value)) {
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
    if (!(iss >> value)) {
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
    if (!(iss >> value)) {
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

  /* Unrecognized // line: treat as a free-form comment. */
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
  if (try_openfpga_address_data_line(line, addr, data)) {
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

/********************************************************************
 * Verilog init.hex — format detection
 *
 * .hex extension always selects init.hex. Otherwise sniff the file:
 * OpenFPGA markers (// directives or 0x address/data pairs) win.
 ********************************************************************/

static bool file_contains_openfpga_mif_marker(std::ifstream& ifs) {
  std::string raw_line;
  while (std::getline(ifs, raw_line)) {
    const std::string line = strip_mif_line_comment(raw_line);

    if (line.size() >= 2 && line[0] == '/' && line[1] == '/') {
      std::istringstream directive_iss(line.substr(2));
      std::string key;
      if (directive_iss >> key) {
        if (key == MIF_DIRECTIVE_X || key == MIF_DIRECTIVE_Y ||
            key == MIF_DIRECTIVE_RAM_ID || key == MIF_DIRECTIVE_ADDR_WIDTH ||
            key == MIF_DIRECTIVE_DATA_WIDTH || key == MIF_DIRECTIVE_ID_WIDTH) {
          return true;
        }
      }
    }

    if (is_openfpga_address_data_line(line)) {
      return true;
    }
  }
  return false;
}

bool is_init_hex_file(const std::string& file_path) {
  if (path_has_file_extension(file_path, "hex")) {
    return true;
  }

  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    return false;
  }
  return !file_contains_openfpga_mif_marker(ifs);
}

/********************************************************************
 * Verilog init.hex — parsing
 *
 * Address assignment rules:
 *   - Single token: data at next_addr, then next_addr++.
 *   - @addr data:   explicit jump; next_addr becomes addr + 1.
 * Bare hex digits are parsed in base 16 (Verilog $readmemh convention).
 ********************************************************************/

bool parse_init_hex_content_line(const std::string& line, uint64_t& next_addr,
                                 uint64_t& addr, uint64_t& data) {
  if (line.empty()) {
    return false;
  }

  std::string work = line;
  const bool has_at_jump = (!work.empty() && work.front() == '@');
  if (has_at_jump) {
    work.erase(work.begin());
    trim_mif_line_inplace(work);
  }

  std::istringstream iss(work);
  std::string first;
  std::string second;
  if (!(iss >> first)) {
    return false;
  }

  std::string extra;
  if (!(iss >> second)) {
    /* Sequential data line: implicit address. */
    if (has_at_jump) {
      return false;
    }
    if (!parse_mif_init_hex_value_token(first, data)) {
      return false;
    }
    addr = next_addr;
    ++next_addr;
    return true;
  }

  /* @addr data requires exactly two tokens after '@'. */
  if (iss >> extra || !has_at_jump) {
    return false;
  }

  if (!parse_mif_init_hex_value_token(first, addr) ||
      !parse_mif_init_hex_value_token(second, data)) {
    return false;
  }
  next_addr = addr + 1;
  return true;
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
  uint64_t max_addr = 0;
  uint64_t max_data = 0;
  uint64_t next_addr = 0;

  std::string raw_line;
  while (std::getline(ifs, raw_line)) {
    ++line_no;

    const std::string line = strip_mif_line_comment(raw_line);
    if (line.empty()) {
      continue;
    }
    /* Full-line // comments are not directives in init.hex. */
    if (line.size() >= 2 && line[0] == '/' && line[1] == '/') {
      continue;
    }

    uint64_t addr = 0;
    uint64_t data = 0;
    if (!parse_init_hex_content_line(line, next_addr, addr, data)) {
      VTR_LOG_ERROR("%s:%lu: cannot parse init.hex line: %s\n",
                    file_path.c_str(), static_cast<unsigned long>(line_no),
                    line.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    mif_storage.create_memory_line(segment_id, addr, data);
    ++total_words;
    if (addr > max_addr) {
      max_addr = addr;
    }
    if (data > max_data) {
      max_data = data;
    }
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

  mif_storage.set_segment_addr_width(
    segment_id, mif_bit_width_for_max_value(max_addr));
  mif_storage.set_segment_data_width(
    segment_id, mif_bit_width_for_max_value(max_data));

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
