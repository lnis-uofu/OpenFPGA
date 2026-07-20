#include "mif_verilog_utils.h"

#include <cctype>
#include <fstream>
#include <map>
#include <regex>
#include <sstream>
#include <string>

#include "mif_io_utils.h"
#include "vtr_log.h"

namespace openfpga {

static std::string strip_verilog_comments(const std::string& src) {
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

static bool basename_equal_ignore_case(const std::string& a,
                                       const std::string& b) {
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
  std::map<std::string, std::map<std::string, std::string>> module_string_params;
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
    for (std::sregex_iterator p(body.begin(), body.end(), param_re); p != re_end;
         ++p) {
      string_params[(*p)[1].str()] = (*p)[2].str();
    }
    module_string_params[module_name] = string_params;

    for (std::sregex_iterator r(body.begin(), body.end(), readmemh_re);
         r != re_end; ++r) {
      std::string arg = (*r)[1].str();
      if (!arg.empty() && arg.front() == '"') {
        readmemh_module_fixed_file[module_name] =
          arg.substr(1, arg.size() - 2);
      } else {
        const auto found = string_params.find(arg);
        const std::string default_file =
          (found != string_params.end()) ? found->second : std::string();
        readmemh_module_param[module_name] =
          std::make_pair(arg, default_file);
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
