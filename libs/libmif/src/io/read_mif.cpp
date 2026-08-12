#include "read_mif.h"

#include <cctype>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "aggregate_mif_util.h"
#include "atom_netlist.h"
#include "openfpga_decode.h"
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
static bool parse_mif_init_hex_addr_token(const std::string& tok,
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

/* Accept arbitrary-width hex data words (optional 0x prefix). */
static bool parse_mif_init_hex_data_token(const std::string& tok,
                                          std::string& hex_out) {
  if (tok.empty()) {
    return false;
  }
  std::string hex = tok;
  if (hex.size() >= 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
    hex = hex.substr(2);
  }
  if (hex.empty()) {
    return false;
  }
  for (const char c : hex) {
    if (!std::isxdigit(static_cast<unsigned char>(c))) {
      return false;
    }
  }
  hex_out = hex;
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
                                uint64_t& addr, std::string& data_hex,
                                bool& has_data) {
  has_data = false;
  data_hex.clear();
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
      if (!parse_mif_init_hex_addr_token(first, next_addr)) {
        return false;
      }
      return true;
    }
    if (!parse_mif_init_hex_data_token(first, data_hex)) {
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

  if (!parse_mif_init_hex_addr_token(first, addr) ||
      !parse_mif_init_hex_data_token(second, data_hex)) {
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
 * Address/data ranges come later from bitstream setting in MifPipeline.
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
    std::string data_hex;
    bool has_data = false;
    if (!parse_init_hex_line(line, next_addr, addr, data_hex, has_data)) {
      VTR_LOG_ERROR("%s:%lu: cannot parse init.hex line: %s\n",
                    file_path.c_str(), static_cast<unsigned long>(line_no),
                    line.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (!has_data) {
      continue;
    }

    const size_t width_bits = data_hex.size() * 4;
    const std::string data_bits = hex_to_bit_string(data_hex, width_bits);
    if (data_bits.empty()) {
      VTR_LOG_ERROR("%s:%lu: invalid hex data word: %s\n", file_path.c_str(),
                    static_cast<unsigned long>(line_no), data_hex.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }

    mif_storage.create_memory_line(segment_id, addr, data_bits);
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

struct MifEblifContentSelector {
  /* Supported selectors:
   *   .param <name> - read an AtomBlock parameter
   *   .attr  <name> - read an AtomBlock attribute
   * Both tokens come from mif_source content in the bitstream setting. */
  std::string field_type;
  std::string field_name;
};

static bool parse_eblif_content_selector(
  const std::string& content, MifEblifContentSelector& selector) {
  std::istringstream content_stream(content);
  std::string extra;
  if (!(content_stream >> selector.field_type >> selector.field_name) ||
      (content_stream >> extra)) {
    return false;
  }
  return true;
}

int read_mif_from_atom_context(MifStorage& mif_storage,
                               const AtomContext& atom_ctx,
                               const std::vector<std::string>& eblif_contents) {
  if (eblif_contents.empty()) {
    VTR_LOG_ERROR(
      "read_mif_from_atom_context: no mif_source content selectors were "
      "provided\n");
    return CMD_EXEC_FATAL_ERROR;
  }
  std::vector<MifEblifContentSelector> selectors;
  selectors.reserve(eblif_contents.size());
  for (const std::string& content : eblif_contents) {
    MifEblifContentSelector selector;
    if (!parse_eblif_content_selector(content, selector)) {
      VTR_LOG_ERROR(
        "read_mif_from_atom_context: invalid mif_source content '%s'; "
        "expected '<field_type> <field_name>'\n",
        content.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    if (selector.field_type != ".param" && selector.field_type != ".attr") {
      VTR_LOG_ERROR(
        "read_mif_from_atom_context: unsupported EBLIF field type '%s' in "
        "mif_source content '%s'\n",
        selector.field_type.c_str(), content.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
    selectors.push_back(selector);
  }

  mif_storage.clear();
  size_t field_index = 0;
  for (const AtomBlockId atom_block : atom_ctx.netlist().blocks()) {
    for (const MifEblifContentSelector& selector : selectors) {
      std::string value;
      bool field_found = false;
      if (selector.field_type == ".param") {
        for (const auto& parameter :
             atom_ctx.netlist().block_params(atom_block)) {
          if (parameter.first == selector.field_name) {
            value = parameter.second;
            field_found = true;
            break;
          }
        }
      } else {
        for (const auto& attribute :
             atom_ctx.netlist().block_attrs(atom_block)) {
          if (attribute.first == selector.field_name) {
            value = attribute.second;
            field_found = true;
            break;
          }
        }
      }
      if (!field_found) {
        continue;
      }

      const t_pb* leaf_pb =
        atom_ctx.lookup().atom_pb_bimap().atom_pb(atom_block);
      const std::string pb_type = generate_mif_pb_path(leaf_pb);
      if (pb_type.empty()) {
        VTR_LOG_ERROR(
          "read_mif_from_atom_context: AtomBlock '%s' has no valid packed pb "
          "path (%s %s #%zu); run this after VPR pack\n",
          atom_ctx.netlist().block_name(atom_block).c_str(),
          selector.field_type.c_str(), selector.field_name.c_str(),
          field_index);
        return CMD_EXEC_FATAL_ERROR;
      }

      if (value.size() >= 2 && value.front() == '"' && value.back() == '"') {
        value = value.substr(1, value.size() - 2);
      }
      const MifSegmentId segment_id = mif_storage.create_segment();
      mif_storage.set_segment_physical_pb(segment_id, pb_type);
      mif_storage.set_segment_raw_data(segment_id, value);
      VTR_LOG("read_mif: AtomBlock '%s' %s %s -> VPR pb_type '%s'\n",
              atom_ctx.netlist().block_name(atom_block).c_str(),
              selector.field_type.c_str(), selector.field_name.c_str(),
              pb_type.c_str());
      ++field_index;
    }
  }

  if (field_index == 0) {
    std::string expected;
    for (size_t i = 0; i < eblif_contents.size(); ++i) {
      if (i > 0) {
        expected += ", ";
      }
      expected += "'";
      expected += eblif_contents[i];
      expected += "'";
    }
    VTR_LOG_ERROR(
      "read_mif: no AtomBlock parameters matching configured content(s) "
      "[%s]\n",
      expected.c_str());
    return CMD_EXEC_FATAL_ERROR;
  }
  return CMD_EXEC_SUCCESS;
}

} /* namespace openfpga */
