#include "read_mif.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>

#include "mif_openfpga_format.h"
#include "vtr_log.h"

namespace openfpga {

/* Remove leading and trailing whitespace from a string in place. */
static void trim_inplace(std::string& s) {
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
    s.erase(s.begin());
  }
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
    s.pop_back();
  }
}

/* Strip '#' comments and trim a raw MIF input line for parsing. */
static std::string normalize_mif_line(const std::string& raw_line) {
  std::string line = raw_line;
  const size_t hash = line.find('#');
  if (hash != std::string::npos) {
    line.resize(hash);
  }
  trim_inplace(line);
  return line;
}

/* Parse a token as an unsigned 64-bit integer (decimal or hex). */
static bool parse_hex_u64(const std::string& tok, uint64_t& out) {
  if (tok.empty()) {
    return false;
  }
  char* end = nullptr;
  errno = 0;
  unsigned long long v = std::strtoull(tok.c_str(), &end, 0);
  if (end != tok.c_str() + tok.size() || errno == ERANGE) {
    return false;
  }
  out = static_cast<uint64_t>(v);
  return true;
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

/* Return true if the segment has memory lines, XY coords, or a RAM_ID. */
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
                               const std::string& directive, MifStorage& mif_storage,
                               bool& has_current_segment,
                               MifSegmentId& current_segment_id) {
  std::istringstream ls(directive);
  std::string key;
  if (!(ls >> key)) {
    return CMD_EXEC_SUCCESS;
  }

  if (key == "RAM_ID") {
    int ram_id = 0;
    if (!(ls >> ram_id)) {
      VTR_LOG_ERROR("%s:%lu: expected integer after //RAM_ID\n",
                    file_path.c_str(), static_cast<unsigned long>(line_no));
      return CMD_EXEC_FATAL_ERROR;
    }
    start_new_segment_for_ram_id(mif_storage, has_current_segment,
                                 current_segment_id, ram_id);
    return CMD_EXEC_SUCCESS;
  }

  int value = 0;
  if (key == "X") {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // X directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no));
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment, current_segment_id);
    mif_storage.set_segment_coord_x(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == "Y") {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // Y directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no));
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment, current_segment_id);
    mif_storage.set_segment_coord_y(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == "ADDR_WIDTH") {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // ADDR_WIDTH directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no));
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment, current_segment_id);
    mif_storage.set_segment_addr_width(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == "DATA_WIDTH") {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // DATA_WIDTH directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no));
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment, current_segment_id);
    mif_storage.set_segment_data_width(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }
  if (key == "ID_WIDTH") {
    if (!(ls >> value)) {
      VTR_LOG_ERROR("%s:%lu: bad // ID_WIDTH directive\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no));
      return CMD_EXEC_FATAL_ERROR;
    }
    ensure_current_segment(mif_storage, has_current_segment, current_segment_id);
    mif_storage.set_segment_id_width(current_segment_id, value);
    return CMD_EXEC_SUCCESS;
  }

  /* Unrecognized // line: treat as comment */
  return CMD_EXEC_SUCCESS;
}

/* Dispatch one normalized MIF line to directive or address/data handling. */
static int parse_mif_line(const std::string& file_path, size_t line_no,
                          const std::string& line, MifStorage& mif_storage,
                          bool& has_current_segment,
                          MifSegmentId& current_segment_id, size_t& total_words) {
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
      VTR_LOG_ERROR(
        "%s:%lu: address/data line before any // directive\n",
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
    const int exec_status = parse_mif_line(
      file_path, line_no, normalize_mif_line(raw_line), mif_storage,
      has_current_segment, current_segment_id, total_words);
    if (CMD_EXEC_SUCCESS != exec_status) {
      return exec_status;
    }
  }

  if (ifs.bad()) {
    VTR_LOG_ERROR("I/O error while reading MIF file '%s'\n", file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

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

/* Write all segments in storage to a MIF file (overwrite). */
bool write_mif(const std::string& file_path, const MifStorage& mif_storage) {
  if (mif_storage.empty()) {
    VTR_LOG_ERROR("No parsed MIF segments to write\n");
    return false;
  }

  std::ofstream ofs(file_path.c_str());
  if (!ofs.is_open()) {
    VTR_LOG_ERROR("Failed to open MIF file '%s' for writing\n",
                  file_path.c_str());
    return false;
  }

  serialize_openfpga_mif(mif_storage, ofs);
  if (!ofs.good()) {
    VTR_LOG_ERROR("I/O error while writing MIF file '%s'\n", file_path.c_str());
    return false;
  }
  return true;
}

} /* namespace openfpga */
