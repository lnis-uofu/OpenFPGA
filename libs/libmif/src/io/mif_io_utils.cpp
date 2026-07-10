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

bool parse_mif_u64_token(const std::string& tok, uint64_t& out) {
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
  if (tok.size() >= 2 && tok[0] == '0' &&
      (tok[1] == 'x' || tok[1] == 'X')) {
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

bool path_has_file_extension(const std::string& file_path,
                             const char* extension) {
  const size_t dot = file_path.find_last_of('.');
  if (dot == std::string::npos || dot + 1 >= file_path.size()) {
    return false;
  }
  std::string ext = file_path.substr(dot + 1);
  const std::string want = extension;
  if (ext.size() != want.size()) {
    return false;
  }
  for (size_t i = 0; i < ext.size(); ++i) {
    if (std::tolower(static_cast<unsigned char>(ext[i])) !=
        std::tolower(static_cast<unsigned char>(want[i]))) {
      return false;
    }
  }
  return true;
}

} /* namespace openfpga */
