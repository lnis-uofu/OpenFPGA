#pragma once

#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

using MifEblifPortConnections =
  std::vector<std::pair<std::string, std::string>>;
using MifPbTypeResolver = std::function<std::string(
  const std::string&, const MifEblifPortConnections&)>;

/* Locate Yosys output *_yosys_out.eblif by scanning the run cwd.
 * Returns empty string if none / multiple matches. */
std::string find_yosys_eblif_file_path();

/* Read memory initialization into storage.
 *
 * Dispatch by file type:
 *   - .eblif/.blif: resolve each .subckt carrying .param INIT and store its
 *     pb_type plus raw INIT in a new logical segment (clears storage first)
 *   - otherwise: Verilog-style init.hex (addr/data lines) bound to pb_type
 */
int read_mif(const std::string& file_path, MifStorage& mif_storage,
             const MifPbTypeResolver& pb_type_resolver,
             const std::string& pb_type = "");

/* Read Yosys eblif and merge into logical storage using bitstream setting:
 *   - source="eblif": overwrite matching pb_type segments
 *   - source="others": keep existing logical segments
 */
int read_mif(const std::string& file_path, MifStorage& mif_storage,
             const MifPbTypeResolver& pb_type_resolver,
             const BitstreamSetting& bitstream_setting);

} /* namespace openfpga */
