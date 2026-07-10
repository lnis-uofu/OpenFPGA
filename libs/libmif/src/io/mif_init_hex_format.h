#pragma once

#include <string>

#include "command_exit_codes.h"
#include "mif_storage.h"

/********************************************************************
 * Verilog $readmemh memory initialization file (init.hex):
 *   // comment
 *   <hex_data>              sequential address, data = token
 *   @<hex_addr> <hex_data>  address jump
 *******************************************************************/
namespace openfpga {

int read_init_hex(const std::string& file_path, MifStorage& mif_storage);

bool is_init_hex_file(const std::string& file_path);

bool parse_init_hex_content_line(const std::string& line, uint64_t& next_addr,
                                 uint64_t& addr, uint64_t& data);

} /* namespace openfpga */
