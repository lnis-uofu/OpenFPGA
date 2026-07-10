#include "bind_bram_to_mif_storage.h"

#include <cctype>
#include <fstream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

#include "mif_io_utils.h"
#include "vtr_log.h"

namespace openfpga {

/********************************************************************
 * Remove // line comments and block comments while preserving strings.
 *******************************************************************/
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

std::string find_verilog_module_reading_mif(const std::string& verilog_path,
                                            const std::string& mif_file_name) {
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

  std::sregex_iterator mod_begin(text.begin(), text.end(), module_re);
  std::sregex_iterator re_end;
  for (auto it = mod_begin; it != re_end; ++it) {
    const std::string module_name = (*it)[1].str();
    const size_t body_start = static_cast<size_t>((*it)[0].second - text.begin());

    std::smatch end_match;
    const std::string from_body = text.substr(body_start);
    if (!std::regex_search(from_body, end_match, endmodule_re)) {
      continue;
    }
    const std::string body =
      from_body.substr(0, static_cast<size_t>(end_match.position()));

    std::map<std::string, std::string> string_params;
    for (std::sregex_iterator p(body.begin(), body.end(), param_re); p != re_end;
         ++p) {
      string_params[(*p)[1].str()] = (*p)[2].str();
    }

    for (std::sregex_iterator r(body.begin(), body.end(), readmemh_re);
         r != re_end; ++r) {
      std::string arg = (*r)[1].str();
      if (!arg.empty() && arg.front() == '"') {
        /* $readmemh("init.hex", ...) */
        arg = arg.substr(1, arg.size() - 2);
        if (basename_equal_ignore_case(arg, mif_file_name)) {
          return module_name;
        }
      } else {
        /* $readmemh(MEM_FILE, ...) with parameter default */
        const auto found = string_params.find(arg);
        if (found != string_params.end() &&
            basename_equal_ignore_case(found->second, mif_file_name)) {
          return module_name;
        }
      }
    }
  }

  return std::string();
}

int bind_bram_to_mif_storage(
  MifStorage& mif_storage, const std::string& verilog_path,
  const std::map<std::string, std::pair<int, int>>& inst_coord_map,
  const MemoryAddressMap& memory_address_map) {
  if (mif_storage.empty()) {
    VTR_LOG_ERROR(
      "bind_bram_to_mif_storage: no MIF data; run read_mif first\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (verilog_path.empty()) {
    VTR_LOG_ERROR(
      "bind_bram_to_mif_storage: Verilog file path is required\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (mif_storage.source_files().empty()) {
    VTR_LOG_ERROR(
      "bind_bram_to_mif_storage: no MIF source file recorded\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (inst_coord_map.empty()) {
    VTR_LOG_ERROR(
      "bind_bram_to_mif_storage: empty inst_coord_map\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  if (memory_address_map.empty()) {
    VTR_LOG_ERROR(
      "bind_bram_to_mif_storage: empty memory_address_map\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  for (const std::string& mif_path : mif_storage.source_files()) {
    const std::string mif_name = mif_file_basename(mif_path);
    const std::string module_name =
      find_verilog_module_reading_mif(verilog_path, mif_name);
    if (module_name.empty()) {
      VTR_LOG_ERROR(
        "bind_bram_to_mif_storage: no Verilog module with $readmemh "
        "for '%s' in '%s'\n",
        mif_name.c_str(), verilog_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    VTR_LOG(
      "bind_bram_to_mif_storage: MIF '%s' is read by module '%s'\n",
      mif_name.c_str(), module_name.c_str());

    auto coord_it = inst_coord_map.find(module_name);
    /* Single-BRAM fallback: Verilog module name may differ from BLIF model */
    if (coord_it == inst_coord_map.end() && inst_coord_map.size() == 1) {
      coord_it = inst_coord_map.begin();
      VTR_LOG(
        "bind_bram_to_mif_storage: module '%s' not in map, use sole "
        "placement entry '%s'\n",
        module_name.c_str(), coord_it->first.c_str());
    }
    if (coord_it == inst_coord_map.end()) {
      VTR_LOG_ERROR(
        "bind_bram_to_mif_storage: no coordinate for module '%s'\n",
        module_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    const int coord_x = coord_it->second.first;
    const int coord_y = coord_it->second.second;

    /* Read ram_id / widths from memory_address_map by (x, y) */
    const MemoryAddressId memory_id =
      memory_address_map.find_by_xy(coord_x, coord_y);
    if (!memory_id.is_valid()) {
      VTR_LOG_ERROR(
        "bind_bram_to_mif_storage: (x=%d, y=%d) not found in "
        "memory_address_map\n",
        coord_x, coord_y);
      return CMD_EXEC_FATAL_ERROR;
    }
    const int ram_id = memory_address_map.ram_id(memory_id);
    const int addr_width = memory_address_map.addr_width(memory_id);
    const int data_width = memory_address_map.data_width(memory_id);

    for (const MifSegmentId& segment_id : mif_storage.segments()) {
      if (mif_storage.has_xy(segment_id)) {
        continue;
      }
      mif_storage.set_segment_coord_x(segment_id, coord_x);
      mif_storage.set_segment_coord_y(segment_id, coord_y);
      mif_storage.set_segment_ram_id(segment_id, ram_id);
      mif_storage.set_segment_addr_width(segment_id, addr_width);
      mif_storage.set_segment_data_width(segment_id, data_width);
    }

    VTR_LOG(
      "bind_bram_to_mif_storage: module '%s' -> (x=%d, y=%d) ram_id=%d "
      "addr_width=%d data_width=%d\n",
      module_name.c_str(), coord_x, coord_y, ram_id, addr_width,
      data_width);
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
