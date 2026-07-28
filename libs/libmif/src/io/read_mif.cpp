#include "read_mif.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>
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

static bool parse_int_after_keyword(const std::string& text,
                                    const std::string& keyword, int& out) {
  const size_t pos = text.find(keyword);
  if (pos == std::string::npos) {
    return false;
  }
  size_t i = pos + keyword.size();
  while (i < text.size() && std::isspace(static_cast<unsigned char>(text[i]))) {
    ++i;
  }
  if (i >= text.size() || text[i] == ':') {
    if (i < text.size() && text[i] == ':') {
      ++i;
    }
    while (i < text.size() &&
           std::isspace(static_cast<unsigned char>(text[i]))) {
      ++i;
    }
  }
  if (i >= text.size() || !std::isdigit(static_cast<unsigned char>(text[i]))) {
    return false;
  }
  out = 0;
  while (i < text.size() && std::isdigit(static_cast<unsigned char>(text[i]))) {
    out = out * 10 + (text[i] - '0');
    ++i;
  }
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

static bool try_parse_init_hex_depth_metadata(const std::string& comment_line,
                                              uint64_t& min_addr,
                                              uint64_t& max_addr) {
  if (comment_line.find("depth") == std::string::npos) {
    return false;
  }
  int depth = 0;
  if (!parse_int_after_keyword(comment_line, "depth", depth) || depth <= 0) {
    return false;
  }

  min_addr = 0;
  max_addr = static_cast<uint64_t>(depth - 1);

  const size_t from_pos = comment_line.find("from");
  const size_t to_pos =
    comment_line.find("to", from_pos == std::string::npos ? 0 : from_pos + 4);
  if (from_pos != std::string::npos && to_pos != std::string::npos) {
    int parsed_min = 0;
    int parsed_max = 0;
    if (parse_int_after_keyword(comment_line.substr(from_pos), "from",
                                parsed_min) &&
        parse_int_after_keyword(comment_line.substr(to_pos), "to",
                                parsed_max) &&
        parsed_min >= 0 && parsed_max >= parsed_min) {
      min_addr = static_cast<uint64_t>(parsed_min);
      max_addr = static_cast<uint64_t>(parsed_max);
    }
  }
  return true;
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

static int read_mif_from_init_hex(const std::string& file_path,
                                  MifStorage& mif_storage) {
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
      uint64_t parsed_min_addr = 0;
      uint64_t parsed_max_addr = 0;
      if (try_parse_init_hex_depth_metadata(line, parsed_min_addr,
                                            parsed_max_addr)) {
        has_depth_metadata = true;
        depth_min_addr = parsed_min_addr;
        depth_max_addr = parsed_max_addr;
      }
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

  if (has_depth_metadata) {
    mif_storage.set_segment_addr_range(
      segment_id, BasicPort("address", static_cast<size_t>(depth_min_addr),
                            static_cast<size_t>(depth_max_addr)));
  }

  return CMD_EXEC_SUCCESS;
}

static bool ends_with_ci(const std::string& s, const std::string& suffix) {
  if (s.size() < suffix.size()) {
    return false;
  }
  for (size_t i = 0; i < suffix.size(); ++i) {
    const char a = static_cast<char>(std::tolower(
      static_cast<unsigned char>(s[s.size() - suffix.size() + i])));
    const char b =
      static_cast<char>(std::tolower(static_cast<unsigned char>(suffix[i])));
    if (a != b) {
      return false;
    }
  }
  return true;
}

static bool looks_like_blif_file(const std::string& file_path) {
  return ends_with_ci(file_path, ".eblif") || ends_with_ci(file_path, ".blif");
}

static int read_mif_from_eblif(const std::string& file_path,
                               MifStorage& mif_storage,
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

  /* Build each logical segment as its eblif INIT parameter is read. */
  while (std::getline(ifs, raw_line)) {
    ++line_no;
    std::string line = raw_line;
    trim_mif_line_inplace(line);
    if (line.empty() || line[0] == '#') {
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
      pb_type_resolver ? pb_type_resolver(init_index) : std::string();
    if (pb_type.empty()) {
      VTR_LOG_ERROR(
        "read_mif: cannot resolve VPR pb_type for INIT index %zu in '%s'\n",
        init_index, file_path.c_str());
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

int read_mif(const std::string& file_path, MifStorage& mif_storage) {
  return read_mif(file_path, mif_storage, MifPbTypeResolver());
}

int read_mif(const std::string& file_path, MifStorage& mif_storage,
             const MifPbTypeResolver& pb_type_resolver) {
  if (looks_like_blif_file(file_path)) {
    return read_mif_from_eblif(file_path, mif_storage, pb_type_resolver);
  }
  return read_mif_from_init_hex(file_path, mif_storage);
}

} /* namespace openfpga */
