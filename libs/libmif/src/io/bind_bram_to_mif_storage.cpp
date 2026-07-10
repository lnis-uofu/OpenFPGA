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
  /* type #( .P(V) ) inst_name ( */
  const std::regex inst_re(
    R"(([A-Za-z_][A-Za-z0-9_$]*)\s*(?:#\s*\(([\s\S]*?)\))?\s*([A-Za-z_][A-Za-z0-9_$]*)\s*\()",
    std::regex::ECMAScript);
  const std::regex override_re(
    R"(\.([A-Za-z_][A-Za-z0-9_$]*)\s*\(\s*([A-Za-z_][A-Za-z0-9_$]*|\"[^\"]+\")\s*\))",
    std::regex::ECMAScript);

  /* module_type -> parameter name used by $readmemh, and its default file */
  std::map<std::string, std::pair<std::string, std::string>>
    readmemh_module_param;
  /* module_type -> fixed file if $readmemh("file") */
  std::map<std::string, std::string> readmemh_module_fixed_file;
  /* parent module -> string parameters */
  std::map<std::string, std::map<std::string, std::string>> module_string_params;
  /* parent module -> body text (for instance scan) */
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

  /* Scan instances inside each parent module */
  for (const auto& parent_itor : module_bodies) {
    const std::string& parent_name = parent_itor.first;
    const std::string& body = parent_itor.second;
    const auto& parent_params = module_string_params[parent_name];

    for (std::sregex_iterator i(body.begin(), body.end(), inst_re); i != re_end;
         ++i) {
      const std::string type_name = (*i)[1].str();
      const std::string param_block = (*i)[2].str();
      const std::string instance_name = (*i)[3].str();

      /* Skip the module header itself: "module foo (" is not matched by
       * inst_re because of 'module' keyword; still skip non-readmemh types */
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

        /* Instance override: .MEM_FILE(...) */
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
    const std::string instance_name =
      find_verilog_instance_reading_mif(verilog_path, mif_name);
    if (instance_name.empty()) {
      VTR_LOG_ERROR(
        "bind_bram_to_mif_storage: no Verilog instance with $readmemh "
        "for '%s' in '%s'\n",
        mif_name.c_str(), verilog_path.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    VTR_LOG(
      "bind_bram_to_mif_storage: MIF '%s' is read by instance '%s'\n",
      mif_name.c_str(), instance_name.c_str());

    auto coord_it = inst_coord_map.find(instance_name);
    if (coord_it == inst_coord_map.end() && inst_coord_map.size() == 1) {
      coord_it = inst_coord_map.begin();
      VTR_LOG(
        "bind_bram_to_mif_storage: instance '%s' not in map, use sole "
        "placement entry '%s'\n",
        instance_name.c_str(), coord_it->first.c_str());
    }
    if (coord_it == inst_coord_map.end()) {
      VTR_LOG_ERROR(
        "bind_bram_to_mif_storage: no coordinate for instance '%s'\n",
        instance_name.c_str());
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

    /* Bind one unbound segment per MIF source file */
    bool filled = false;
    for (const MifSegmentId& segment_id : mif_storage.segments()) {
      if (mif_storage.has_xy(segment_id)) {
        continue;
      }
      mif_storage.set_segment_coord_x(segment_id, coord_x);
      mif_storage.set_segment_coord_y(segment_id, coord_y);
      mif_storage.set_segment_ram_id(segment_id, ram_id);
      mif_storage.set_segment_addr_width(segment_id, addr_width);
      mif_storage.set_segment_data_width(segment_id, data_width);
      filled = true;
      break;
    }
    if (!filled) {
      VTR_LOG_ERROR(
        "bind_bram_to_mif_storage: no unbound segment left for "
        "instance '%s'\n",
        instance_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    VTR_LOG(
      "bind_bram_to_mif_storage: instance '%s' -> (x=%d, y=%d) ram_id=%d "
      "addr_width=%d data_width=%d\n",
      instance_name.c_str(), coord_x, coord_y, ram_id, addr_width,
      data_width);
  }

  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
