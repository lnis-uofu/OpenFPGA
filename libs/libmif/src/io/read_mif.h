#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

using MifEblifPortConnections =
  std::vector<std::pair<std::string, std::string>>;
using MifPbTypeResolver = std::function<std::string(
  const std::string&, const MifEblifPortConnections&)>;

constexpr const char* K_YOSYS_EBLIF_SUFFIX = "_yosys_out.eblif";

/* Locate Yosys output *_yosys_out.eblif by scanning the run cwd.
 * Returns empty string if none / multiple matches. */
std::string find_yosys_eblif_file_path();

/* Read Verilog-style init.hex into one logical segment (shell read_mif). */
int read_mif_from_init_hex(const std::string& file_path,
                           MifStorage& mif_storage, const std::string& pb_type);

/********************************************************************
 * Read eblif memory init fields into logical MIF storage.
 * Called by aggregate_mif when bitstream setting has source="eblif".
 *
 * eblif_contents lists free-form field selectors from mif_source content=
 * (e.g. ".param INIT", ".param INIT_i"); exact names depend on the synth
 * frontend.
 *
 * For each .subckt with a matching content field:
 *   - resolve pb_type via pb_type_resolver (VPR binding)
 *   - create one segment with physical_pb + raw bit-string
 *
 * Clears mif_storage first. Addr/data ranges are filled later in
 * aggregate_mif from bitstream setting. Decode of raw data also happens
 * there.
 *******************************************************************/
int read_mif_from_eblif(const std::string& file_path, MifStorage& mif_storage,
                        const MifPbTypeResolver& pb_type_resolver,
                        const std::vector<std::string>& eblif_contents);

} /* namespace openfpga */
