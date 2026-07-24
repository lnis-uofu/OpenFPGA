#pragma once

#include <string>

#include "bitstream_setting.h"
#include "command_exit_codes.h"
#include "mif_storage.h"

namespace openfpga {

/* Read memory initialization into storage.
 *
 * Dispatch by file type / mif_source:
 *   - .eblif/.blif with mif_source source="eblif" content=".param <NAME>":
 *     unpack each matching .param bit-vector using address_range/data_range
 *   - otherwise: Verilog-style init.hex (addr/data lines)
 *
 * bitstream_setting provides mif_source metadata for eblif binding.
 */
int read_mif(const std::string& file_path, MifStorage& mif_storage,
             const BitstreamSetting& bitstream_setting);

/* Convenience for init.hex-only unit tests (empty bitstream setting). */
int read_mif(const std::string& file_path, MifStorage& mif_storage);

} /* namespace openfpga */
