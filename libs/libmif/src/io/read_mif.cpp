#include "read_mif.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "vtr_log.h"

namespace openfpga {

static void trim_mif_line_inplace(std::string& s) {
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
    s.erase(s.begin());
  }
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
    s.pop_back();
  }
}

static bool parse_mif_u64_token(const std::string& tok, uint64_t& out) {
  if (tok.empty()) {
    return false;
  }
  char* end = nullptr;
  errno = 0;
  const unsigned long long v = std::strtoull(tok.c_str(), &end, 0);
  if (end != tok.c_str() + tok.size() || errno == ERANGE) {
    return false;
  }
  out = static_cast<uint64_t>(v);
  return true;
}

/* Bare hex digits use base 16 (not octal); 0x / other forms use auto base. */
static bool parse_mif_init_hex_value_token(const std::string& tok,
                                           uint64_t& out) {
  if (tok.empty()) {
    return false;
  }
  if (tok.size() >= 2 && tok[0] == '0' && (tok[1] == 'x' || tok[1] == 'X')) {
    return parse_mif_u64_token(tok, out);
  }
  for (char c : tok) {
    if (!std::isxdigit(static_cast<unsigned char>(c))) {
      return parse_mif_u64_token(tok, out);
    }
  }
  char* end = nullptr;
  errno = 0;
  const unsigned long long v = std::strtoull(tok.c_str(), &end, 16);
  if (end != tok.c_str() + tok.size() || errno == ERANGE) {
    return false;
  }
  out = static_cast<uint64_t>(v);
  return true;
}

static std::string strip_mif_line_comment(const std::string& raw_line) {
  std::string line = raw_line;
  trim_mif_line_inplace(line);
  if (line.size() >= 2 && line[0] == '/' && line[1] == '/') {
    return line;
  }
  const size_t slash = line.find("//");
  if (slash != std::string::npos) {
    line.resize(slash);
  }
  trim_mif_line_inplace(line);
  return line;
}

/* Parse one init.hex data line.
 * Returns false on syntax error.
 * When has_data is false, only next_addr was updated (@addr with no data word),
 * matching Verilog $readmemh address-only directives. */
static bool parse_init_hex_line(const std::string& line, uint64_t& next_addr,
                                uint64_t& addr, uint64_t& data,
                                bool& has_data) {
  has_data = false;
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
      /* @<addr> alone: set next write address ($readmemh). */
      if (!parse_mif_init_hex_value_token(first, next_addr)) {
        return false;
      }
      return true;
    }
    if (!parse_mif_init_hex_value_token(first, data)) {
      return false;
    }
    addr = next_addr;
    ++next_addr;
    has_data = true;
    return true;
  }

  if (iss >> extra) {
    return false;
  }

  if (!parse_mif_init_hex_value_token(first, addr) ||
      !parse_mif_init_hex_value_token(second, data)) {
    return false;
  }
  next_addr = addr + 1;
  has_data = true;
  return true;
}

/********************************************************************
 * Read a Verilog-style init.hex into one logical segment.
 * Used by shell command: read_mif --file <hex> --pb_type <pb>
 *
 * Stores only address/data words and the caller-provided pb_type.
 * Address/data ranges come later from bitstream setting in aggregate_mif.
 *******************************************************************/
int read_mif_from_init_hex(const std::string& file_path,
                           MifStorage& mif_storage,
                           const std::string& pb_type) {
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

  std::string raw_line;
  while (std::getline(ifs, raw_line)) {
    ++line_no;

    const std::string line = strip_mif_line_comment(raw_line);
    if (line.empty()) {
      continue;
    }
    /* Skip full-line comments; addr/data ranges come from bitstream setting. */
    if (line.size() >= 2 && line[0] == '/' && line[1] == '/') {
      continue;
    }

    uint64_t addr = 0;
    uint64_t data = 0;
    bool has_data = false;
    if (!parse_init_hex_line(line, next_addr, addr, data, has_data)) {
      VTR_LOG_ERROR("%s:%lu: cannot parse init.hex line: %s\n",
                    file_path.c_str(), static_cast<unsigned long>(line_no),
                    line.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (!has_data) {
      continue;
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

  mif_storage.set_segment_physical_pb(segment_id, pb_type);
  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Read Yosys eblif memory INIT into logical MIF storage.
 * Called by aggregate_mif when bitstream setting has source="eblif".
 *
 * For each .subckt with .param INIT:
 *   - resolve pb_type via pb_type_resolver (VPR internal data)
 *   - create one segment with physical_pb + raw INIT bit-string
 *
 * Addr/data ranges are filled later in
 * aggregate_mif from bitstream setting. Decode of raw INIT also happens
 * there.
 *******************************************************************/
int read_mif_from_eblif(const std::string& file_path, MifStorage& mif_storage,
                        const MifPbTypeResolver& pb_type_resolver) {
  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    VTR_LOG_ERROR("Failed to open eblif file '%s' for reading\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  mif_storage.clear();
  std::string raw_line;
  size_t line_no = 0;
  size_t init_index = 0;
  std::string subckt_model;
  MifEblifPortConnections subckt_connections;

  /* Build each logical segment as its eblif INIT parameter is read. */
  while (std::getline(ifs, raw_line)) {
    ++line_no;
    std::string line = raw_line;
    trim_mif_line_inplace(line);
    if (line.empty() || line[0] == '#') {
      continue;
    }
    if (line.compare(0, 7, ".subckt") == 0) {
      std::istringstream subckt_stream(line);
      std::string dot_subckt;
      std::string connection;
      subckt_connections.clear();
      if (!(subckt_stream >> dot_subckt >> subckt_model) ||
          dot_subckt != ".subckt") {
        VTR_LOG_ERROR("%s:%lu: cannot parse .subckt line: %s\n",
                      file_path.c_str(), static_cast<unsigned long>(line_no),
                      line.c_str());
        return CMD_EXEC_FATAL_ERROR;
      }
      while (subckt_stream >> connection) {
        const size_t equal_pos = connection.find('=');
        if (equal_pos == std::string::npos || equal_pos == 0 ||
            equal_pos + 1 == connection.size()) {
          VTR_LOG_ERROR("%s:%lu: invalid .subckt connection: %s\n",
                        file_path.c_str(), static_cast<unsigned long>(line_no),
                        connection.c_str());
          return CMD_EXEC_FATAL_ERROR;
        }
        subckt_connections.emplace_back(connection.substr(0, equal_pos),
                                        connection.substr(equal_pos + 1));
      }
      continue;
    }
    if (line.compare(0, 6, ".param") != 0) {
      continue;
    }
    std::istringstream iss(line);
    std::string dot_param;
    std::string name;
    std::string value;
    if (!(iss >> dot_param >> name >> value) || dot_param != ".param") {
      VTR_LOG_ERROR("%s:%lu: cannot parse .param line: %s\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no), line.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
      value = value.substr(1, value.size() - 2);
    }
    if (name != "INIT") {
      continue;
    }

    const std::string pb_type =
      pb_type_resolver ? pb_type_resolver(subckt_model, subckt_connections)
                       : std::string();
    if (pb_type.empty()) {
      VTR_LOG_ERROR(
        "%s:%lu: read_mif_from_eblif: failed to resolve pb_type for .subckt "
        "'%s' (.param INIT #%zu)\n",
        file_path.c_str(), static_cast<unsigned long>(line_no),
        subckt_model.c_str(), init_index);
      return CMD_EXEC_FATAL_ERROR;
    }

    const MifSegmentId segment_id = mif_storage.create_segment();
    mif_storage.set_segment_physical_pb(segment_id, pb_type);
    mif_storage.set_segment_raw_data(segment_id, value);
    VTR_LOG("read_mif: eblif .param INIT -> VPR pb_type '%s'\n",
            pb_type.c_str());
    ++init_index;
  }
  if (ifs.bad()) {
    VTR_LOG_ERROR("I/O error while reading eblif file '%s'\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  if (init_index == 0) {
    VTR_LOG_ERROR("read_mif: no .param INIT found in '%s'\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

std::string find_yosys_eblif_file_path() {
  constexpr const char* k_suffix = "_yosys_out.eblif";
  std::string found;

  for (const std::filesystem::directory_entry& entry :
       std::filesystem::directory_iterator(".")) {
    if (!entry.is_regular_file()) {
      continue;
    }
    const std::string name = entry.path().filename().string();
    if (!name.ends_with(k_suffix)) {
      continue;
    }
    if (!found.empty()) {
      VTR_LOG_ERROR("Cannot locate Yosys eblif: multiple '*%s' in cwd\n",
                    k_suffix);
      return std::string();
    }
    found = name;
  }

  if (found.empty()) {
    VTR_LOG_ERROR("Cannot locate Yosys eblif: no '*%s' in cwd\n", k_suffix);
    return std::string();
  }

  VTR_LOG("Located Yosys eblif '%s'\n", found.c_str());
  return found;
}

} /* namespace openfpga */
