#pragma once

#include <string>
#include <vector>

#include "command_exit_codes.h"
#include "mif_storage.h"
#include "vpr_context.h"

namespace openfpga {

/* Read Verilog-style init.hex into one logical segment (shell read_mif). */
int read_mif_from_init_hex(const std::string& file_path,
                           MifStorage& mif_storage, const std::string& pb_type);

/********************************************************************
 * Read EBLIF parameters already parsed by VTR into logical MIF storage.
 *
 * eblif_contents lists field selectors from mif_source content=. Both field
 * type and name come from the setting (e.g. ".param INIT", ".attr INIT").
 *
 * For each AtomBlock with a matching parameter:
 *   - resolve pb_type from its AtomBlockId (VPR pack binding)
 *   - create one segment with physical_pb + raw bit-string
 *
 * Clears mif_storage first. Addr/data ranges are filled later in
 * MifPipeline from bitstream setting. Decode of raw data also happens there.
 *******************************************************************/
int read_mif_from_atom_context(MifStorage& mif_storage,
                               const AtomContext& atom_ctx,
                               const std::vector<std::string>& eblif_contents);

} /* namespace openfpga */
