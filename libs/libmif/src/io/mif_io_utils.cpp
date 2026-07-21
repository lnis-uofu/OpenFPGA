#include "mif_io_utils.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <map>
#include <regex>
#include <sstream>

#include "vtr_log.h"

namespace openfpga {

namespace {

void trim_mif_line_inplace(std::string& s) {
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front()))) {
    s.erase(s.begin());
  }
  while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back()))) {
    s.pop_back();
  }
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

bool parse_int_after_keyword(const std::string& text,
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

std::string strip_verilog_comments(const std::string& src) {
  std::string out;
  out.reserve(src.size());
  enum class State { kCode, kLineComment, kBlockComment, kString };
  State state = State::kCode;
  for (size_t i = 0; i < src.size(); ++i) {
    const char c = src[i];
    const char next = (i + 1 < src.size()) ? src[i + 1] : '\0';
    switch (state) {
      case State::kCode:
        if (c == '/' && next == '/') {
          state = State::kLineComment;
          ++i;
        } else if (c == '/' && next == '*') {
          state = State::kBlockComment;
          ++i;
        } else if (c == '"') {
          state = State::kString;
          out.push_back(c);
        } else {
          out.push_back(c);
        }
        break;
      case State::kLineComment:
        if (c == '\n') {
          state = State::kCode;
          out.push_back(c);
        }
        break;
      case State::kBlockComment:
        if (c == '*' && next == '/') {
          state = State::kCode;
          ++i;
        }
        break;
      case State::kString:
        out.push_back(c);
        if (c == '\\' && next != '\0') {
          out.push_back(next);
          ++i;
        } else if (c == '"') {
          state = State::kCode;
        }
        break;
      default:
        break;
    }
  }
  return out;
}

bool basename_equal_ignore_case(const std::string& a, const std::string& b) {
  const std::string ba = mif_file_basename(a);
  const std::string bb = mif_file_basename(b);
  if (ba.size() != bb.size()) {
    return false;
  }
  for (size_t i = 0; i < ba.size(); ++i) {
    if (std::tolower(static_cast<unsigned char>(ba[i])) !=
        std::tolower(static_cast<unsigned char>(bb[i]))) {
      return false;
    }
  }
  return true;
}

} /* namespace */

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

bool try_parse_init_hex_depth_metadata(const std::string& comment_line,
                                       int& depth, uint64_t& max_addr) {
  if (comment_line.find("depth") == std::string::npos) {
    return false;
  }
  if (!parse_int_after_keyword(comment_line, "depth", depth) || depth <= 0) {
    return false;
  }

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

int extract_pb_type_leaf_index(const std::string& indexed_pb_type) {
  int last_index = -1;
  for (size_t i = 0; i < indexed_pb_type.size(); ++i) {
    if ('[' != indexed_pb_type[i]) {
      continue;
    }
    const size_t end = indexed_pb_type.find(']', i + 1);
    if (std::string::npos == end) {
      continue;
    }
    const std::string index_str = indexed_pb_type.substr(i + 1, end - i - 1);
    if (index_str.empty()) {
      i = end;
      continue;
    }
    bool all_digit = true;
    for (const char ch : index_str) {
      if (!std::isdigit(static_cast<unsigned char>(ch))) {
        all_digit = false;
        break;
      }
    }
    if (all_digit) {
      last_index = std::stoi(index_str);
    }
    i = end;
  }
  return last_index;
}

bool parse_init_hex_line(const std::string& line, uint64_t& next_addr,
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

std::string find_verilog_instance_reading_mif(
  const std::string& verilog_path, const std::string& mif_file_name) {
  std::ifstream ifs(verilog_path.c_str());
  if (!ifs.is_open()) {
    VTR_LOG_ERROR("Failed to open Verilog file '%s'\n", verilog_path.c_str());
    return std::string();
  }

  std::ostringstream oss;
  oss << ifs.rdbuf();
  const std::string text = strip_verilog_comments(oss.str());

  const std::regex module_re(
    R"((?:^|\n)\s*module\s+([A-Za-z_][A-Za-z0-9_$]*)\b)",
    std::regex::ECMAScript);
  const std::regex endmodule_re(R"((?:^|\n)\s*endmodule\b)",
                                std::regex::ECMAScript);
  const std::regex param_re(
    R"(parameter\s+(?:\[[^\]]*\]\s*)?([A-Za-z_][A-Za-z0-9_$]*)\s*=\s*\"([^\"]+)\")",
    std::regex::ECMAScript);
  const std::regex readmemh_re(
    R"(\$readmemh\s*\(\s*([A-Za-z_][A-Za-z0-9_$]*|\"[^\"]+\"))",
    std::regex::ECMAScript);
  const std::regex inst_re(
    R"(([A-Za-z_][A-Za-z0-9_$]*)\s*(?:#\s*\(([\s\S]*?)\))?\s*([A-Za-z_][A-Za-z0-9_$]*)\s*\()",
    std::regex::ECMAScript);
  const std::regex override_re(
    R"(\.([A-Za-z_][A-Za-z0-9_$]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_$]*|\"[^\"]+\")\s*\))",
    std::regex::ECMAScript);

  std::map<std::string, std::pair<std::string, std::string>>
    readmemh_module_param;
  std::map<std::string, std::string> readmemh_module_fixed_file;
  std::map<std::string, std::map<std::string, std::string>>
    module_string_params;
  std::map<std::string, std::string> module_bodies;

  std::sregex_iterator mod_begin(text.begin(), text.end(), module_re);
  std::sregex_iterator re_end;
  for (auto it = mod_begin; it != re_end; ++it) {
    const std::string module_name = (*it)[1].str();
    const size_t body_start =
      static_cast<size_t>((*it)[0].second - text.begin());

    std::smatch end_match;
    const std::string from_body = text.substr(body_start);
    if (!std::regex_search(from_body, end_match, endmodule_re)) {
      continue;
    }
    const std::string body =
      from_body.substr(0, static_cast<size_t>(end_match.position()));
    module_bodies[module_name] = body;

    std::map<std::string, std::string> string_params;
    for (std::sregex_iterator p(body.begin(), body.end(), param_re);
         p != re_end; ++p) {
      string_params[(*p)[1].str()] = (*p)[2].str();
    }
    module_string_params[module_name] = string_params;

    for (std::sregex_iterator r(body.begin(), body.end(), readmemh_re);
         r != re_end; ++r) {
      std::string arg = (*r)[1].str();
      if (!arg.empty() && arg.front() == '"') {
        readmemh_module_fixed_file[module_name] = arg.substr(1, arg.size() - 2);
      } else {
        const auto found = string_params.find(arg);
        const std::string default_file =
          (found != string_params.end()) ? found->second : std::string();
        readmemh_module_param[module_name] = std::make_pair(arg, default_file);
      }
    }
  }

  for (const auto& parent_itor : module_bodies) {
    const std::string& parent_name = parent_itor.first;
    const std::string& body = parent_itor.second;
    const auto& parent_params = module_string_params[parent_name];

    for (std::sregex_iterator i(body.begin(), body.end(), inst_re); i != re_end;
         ++i) {
      const std::string type_name = (*i)[1].str();
      const std::string param_block = (*i)[2].str();
      const std::string instance_name = (*i)[3].str();

      const bool is_param_readmemh =
        readmemh_module_param.count(type_name) != 0;
      const bool is_fixed_readmemh =
        readmemh_module_fixed_file.count(type_name) != 0;
      if (!is_param_readmemh && !is_fixed_readmemh) {
        continue;
      }

      std::string resolved_file;
      if (is_fixed_readmemh) {
        resolved_file = readmemh_module_fixed_file[type_name];
      } else {
        const std::string& param_name = readmemh_module_param[type_name].first;
        resolved_file = readmemh_module_param[type_name].second;

        for (std::sregex_iterator o(param_block.begin(), param_block.end(),
                                    override_re);
             o != re_end; ++o) {
          if ((*o)[1].str() != param_name) {
            continue;
          }
          std::string ov = (*o)[2].str();
          if (!ov.empty() && ov.front() == '"') {
            resolved_file = ov.substr(1, ov.size() - 2);
          } else {
            const auto pfound = parent_params.find(ov);
            if (pfound != parent_params.end()) {
              resolved_file = pfound->second;
            }
          }
        }
      }

      if (!resolved_file.empty() &&
          basename_equal_ignore_case(resolved_file, mif_file_name)) {
        return instance_name;
      }
    }
  }

  return std::string();
}

} /* namespace openfpga */
