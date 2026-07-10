#include "mif_init_hex_format.h"

#include <fstream>
#include <sstream>
#include <string>

#include "command_exit_codes.h"
#include "mif_text_utils.h"
#include "vtr_log.h"

namespace openfpga {

static bool openfpga_mif_address_data_line(const std::string& line) {
  std::istringstream iss(line);
  std::string first;
  std::string second;
  if (!(iss >> first >> second)) {
    return false;
  }
  std::string extra;
  if (iss >> extra) {
    return false;
  }
  return first.size() >= 2 && first[0] == '0' &&
         (first[1] == 'x' || first[1] == 'X');
}

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

static bool file_contains_openfpga_mif_marker(std::ifstream& ifs) {
  std::string raw_line;
  while (std::getline(ifs, raw_line)) {
    const std::string line = strip_mif_line_comment(raw_line);
    if (line.size() >= 2 && line[0] == '/' && line[1] == '/') {
      std::istringstream directive_iss(line.substr(2));
      std::string key;
      if (directive_iss >> key) {
        if (key == "X" || key == "Y" || key == "RAM_ID" ||
            key == "ADDR_WIDTH" || key == "DATA_WIDTH" || key == "ID_WIDTH") {
          return true;
        }
      }
    }
    if (openfpga_mif_address_data_line(line)) {
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
