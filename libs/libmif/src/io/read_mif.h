#pragma once

#include <cstddef>
#include <functional>
#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

using MifPbTypeResolver = std::function<std::string(size_t)>;

/* Read memory initialization into storage.
 *
 * Dispatch by file type:
 *   - .eblif/.blif: append raw .param INIT to pre-bound VPR segments
 *   - otherwise: Verilog-style init.hex (addr/data lines)
 */
int read_mif(const std::string& file_path, MifStorage& mif_storage);

/* Resolve and set each eblif INIT segment's pb_type while it is read. */
int read_mif(const std::string& file_path, MifStorage& mif_storage,
             const MifPbTypeResolver& pb_type_resolver);

} /* namespace openfpga */
