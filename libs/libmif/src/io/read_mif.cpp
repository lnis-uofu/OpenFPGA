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
                                              int& depth, uint64_t& min_addr,
                                              uint64_t& max_addr) {
  if (comment_line.find("depth") == std::string::npos) {
    return false;
  }
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

static bool parse_init_hex_line(const std::string& line, uint64_t& next_addr,
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

  if (iss >> extra) {
    return false;
  }

  if (!parse_mif_init_hex_value_token(first, addr) ||
      !parse_mif_init_hex_value_token(second, data)) {
    return false;
  }
  next_addr = addr + 1;
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
      int parsed_depth = 0;
      uint64_t parsed_min_addr = 0;
      uint64_t parsed_max_addr = 0;
      if (try_parse_init_hex_depth_metadata(line, parsed_depth, parsed_min_addr,
                                            parsed_max_addr)) {
        has_depth_metadata = true;
        depth_min_addr = parsed_min_addr;
        depth_max_addr = parsed_max_addr;
      }
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
    const char a = static_cast<char>(
      std::tolower(static_cast<unsigned char>(s[s.size() - suffix.size() + i])));
    const char b = static_cast<char>(
      std::tolower(static_cast<unsigned char>(suffix[i])));
    if (a != b) {
      return false;
    }
  }
  return true;
}

static bool looks_like_blif_file(const std::string& file_path) {
  return ends_with_ci(file_path, ".eblif") || ends_with_ci(file_path, ".blif");
}

static bool parse_mif_source_param_name(const std::string& content,
                                        std::string& param_name) {
  std::istringstream iss(content);
  std::string tok0;
  std::string tok1;
  if (!(iss >> tok0 >> tok1)) {
    return false;
  }
  if (tok0 != ".param" || tok1.empty()) {
    return false;
  }
  param_name = tok1;
  return true;
}

static bool unpack_yosys_init_param(
  const std::string& bits, size_t data_width, size_t depth,
  std::vector<std::pair<uint64_t, uint64_t>>& out_words) {
  out_words.clear();
  if (data_width == 0 || depth == 0 || data_width > 64) {
    return false;
  }
  const size_t expect = data_width * depth;
  if (bits.size() != expect) {
    VTR_LOG_ERROR(
      "read_mif eblif: INIT length %zu != address_depth(%zu)*data_width(%zu)\n",
      bits.size(), depth, data_width);
    return false;
  }

  for (size_t addr = 0; addr < depth; ++addr) {
    const size_t end = bits.size() - addr * data_width;
    const size_t begin = end - data_width;
    const std::string word_bits = bits.substr(begin, data_width);

    bool all_undef = true;
    bool has_bad = false;
    uint64_t data = 0;
    for (size_t i = 0; i < data_width; ++i) {
      const char c = word_bits[i];
      if (c == '0' || c == '1') {
        all_undef = false;
        data = (data << 1) | static_cast<uint64_t>(c == '1' ? 1 : 0);
      } else if (c == 'x' || c == 'X' || c == 'z' || c == 'Z') {
        data <<= 1;
      } else {
        has_bad = true;
        break;
      }
    }
    if (has_bad) {
      VTR_LOG_ERROR("read_mif eblif: invalid bit in INIT at addr %zu\n", addr);
      return false;
    }
    if (all_undef) {
      continue;
    }
    out_words.emplace_back(static_cast<uint64_t>(addr), data);
  }
  return true;
}

static int read_mif_from_eblif(const std::string& file_path,
                               MifStorage& mif_storage,
                               const BitstreamSetting& bitstream_setting) {
  std::vector<MifSourceSettingId> eblif_sources;
  for (const MifSourceSettingId& id : bitstream_setting.mif_source_settings()) {
    if (bitstream_setting.mif_source_source(id) == "eblif") {
      eblif_sources.push_back(id);
    }
  }
  if (eblif_sources.empty()) {
    VTR_LOG_ERROR(
      "read_mif: file '%s' looks like eblif/blif but no mif_source with "
      "source=\"eblif\" is defined in bitstream setting\n",
      file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  std::ifstream ifs(file_path.c_str());
  if (!ifs.is_open()) {
    VTR_LOG_ERROR("Failed to open eblif file '%s' for reading\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  std::vector<std::pair<std::string, std::string>> params;
  std::string raw_line;
  size_t line_no = 0;
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
    params.emplace_back(name, value);
  }
  if (ifs.bad()) {
    VTR_LOG_ERROR("I/O error while reading eblif file '%s'\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }

  size_t consumed_params = 0;
  size_t segments_created = 0;
  for (const MifSourceSettingId& source_id : eblif_sources) {
    std::string want_param;
    if (!parse_mif_source_param_name(
          bitstream_setting.mif_source_content(source_id), want_param)) {
      VTR_LOG_ERROR(
        "read_mif: invalid mif_source content '%s' (expect '.param <NAME>')\n",
        bitstream_setting.mif_source_content(source_id).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const BasicPort addr_range =
      bitstream_setting.mif_source_address_range(source_id);
    const BasicPort data_range =
      bitstream_setting.mif_source_data_range(source_id);
    if (!addr_range.is_valid() || !data_range.is_valid()) {
      VTR_LOG_ERROR(
        "read_mif: invalid address/data range on mif_source for '%s'\n",
        bitstream_setting.mif_source_pb_type(source_id).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    const size_t depth = addr_range.get_width();
    const size_t data_width = data_range.get_width();

    size_t found = params.size();
    for (size_t i = consumed_params; i < params.size(); ++i) {
      if (params[i].first == want_param) {
        found = i;
        break;
      }
    }
    if (found == params.size()) {
      VTR_LOG_ERROR(
        "read_mif: no .param %s found in '%s' for mif_source pb_type '%s'\n",
        want_param.c_str(), file_path.c_str(),
        bitstream_setting.mif_source_pb_type(source_id).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    consumed_params = found + 1;

    std::vector<std::pair<uint64_t, uint64_t>> words;
    if (!unpack_yosys_init_param(params[found].second, data_width, depth,
                                 words)) {
      return CMD_EXEC_FATAL_ERROR;
    }
    if (words.empty()) {
      VTR_LOG_ERROR(
        "read_mif: .param %s for pb_type '%s' has no defined memory words\n",
        want_param.c_str(),
        bitstream_setting.mif_source_pb_type(source_id).c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const MifSegmentId segment_id = mif_storage.create_segment();
    mif_storage.set_segment_physical_pb(
      segment_id, bitstream_setting.mif_source_pb_type(source_id));
    mif_storage.set_segment_addr_range(
      segment_id,
      BasicPort("address", addr_range.get_lsb(), addr_range.get_msb()));
    for (const auto& word : words) {
      mif_storage.create_memory_line(
        segment_id, word.first + addr_range.get_lsb(), word.second);
    }
    ++segments_created;
    VTR_LOG(
      "read_mif: eblif .param %s -> pb_type '%s' (%zu defined words, "
      "data_width=%zu)\n",
      want_param.c_str(),
      bitstream_setting.mif_source_pb_type(source_id).c_str(), words.size(),
      data_width);
  }

  if (0 == segments_created) {
    VTR_LOG_ERROR("read_mif: no MIF segments created from '%s'\n",
                  file_path.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

int read_mif(const std::string& file_path, MifStorage& mif_storage,
             const BitstreamSetting& bitstream_setting) {
  if (looks_like_blif_file(file_path)) {
    return read_mif_from_eblif(file_path, mif_storage, bitstream_setting);
  }
  return read_mif_from_init_hex(file_path, mif_storage);
}

int read_mif(const std::string& file_path, MifStorage& mif_storage) {
  BitstreamSetting empty_setting;
  return read_mif(file_path, mif_storage, empty_setting);
}

} /* namespace openfpga */
