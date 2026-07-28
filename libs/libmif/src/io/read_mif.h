#pragma once

#include <cstddef>
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

/* Read memory initialization into storage.
 *
 * Dispatch by file type:
 *   - .eblif/.blif: resolve each .subckt carrying .param INIT and store its
 *     pb_type plus raw INIT in a new logical segment
 *   - otherwise: Verilog-style init.hex (addr/data lines)
 */
int read_mif(const std::string& file_path, MifStorage& mif_storage);

/* Resolve and set each eblif INIT segment's pb_type while it is read. */
int read_mif(const std::string& file_path, MifStorage& mif_storage,
             const MifPbTypeResolver& pb_type_resolver);

} /* namespace openfpga */
