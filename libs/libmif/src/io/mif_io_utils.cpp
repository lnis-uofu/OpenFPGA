#include "mif_io_utils.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>

namespace openfpga {

void trim_mif_line_inplace(std::string& s) {
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
    s.erase(s.begin());
  }
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
    s.pop_back();
  }
}

std::string strip_mif_line_comment(const std::string& raw_line) {
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

bool parse_mif_init_hex_value_token(const std::string& tok, uint64_t& out) {
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

int mif_bit_width_for_max_value(uint64_t max_value) {
  if (max_value == 0) {
    return 1;
  }
  int width = 0;
  while (max_value > 0) {
    ++width;
    max_value >>= 1;
  }
  return width;
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

bool try_parse_init_hex_depth_metadata(const std::string& comment_line,
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
                                parsed_max)) {
      min_addr = static_cast<uint64_t>(parsed_min);
      max_addr = static_cast<uint64_t>(parsed_max);
    }
  }
  return true;
}

bool try_parse_init_hex_width_metadata(const std::string& comment_line,
                                       int& width) {
  if (comment_line.find("width") == std::string::npos) {
    return false;
  }
  return parse_int_after_keyword(comment_line, "width", width) && width > 0;
}

std::string mif_file_basename(const std::string& file_path) {
  const size_t slash = file_path.find_last_of("/\\");
  if (slash == std::string::npos) {
    return file_path;
  }
  return file_path.substr(slash + 1);
}

std::string strip_pb_type_indices(const std::string& indexed_pb_type) {
  std::string result;
  result.reserve(indexed_pb_type.size());
  for (size_t i = 0; i < indexed_pb_type.size(); ++i) {
    if ('[' != indexed_pb_type[i]) {
      result += indexed_pb_type[i];
      continue;
    }
    const size_t end = indexed_pb_type.find(']', i + 1);
    if (std::string::npos == end) {
      result += indexed_pb_type[i];
      continue;
    }
    i = end;
  }
  return result;
}

} /* namespace openfpga */
