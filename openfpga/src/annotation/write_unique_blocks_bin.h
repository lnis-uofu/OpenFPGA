#ifndef WRITE_XML_UNIQUE_BLOCKS_BIN_H
#define WRITE_XML_UNIQUE_BLOCKS_BIN_H

#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "device_rr_gsb_utils.h"

/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- write the unique blocks' information in the associated data structures:
 *device_rr_gsb to a bin file
 *******************************************************************/

namespace openfpga {
int write_bin_unique_blocks(const DeviceRRGSB& device_rr_gsb, const char* fname,
                            bool verbose_output);
int write_bin_atom_block(const std::vector<vtr::Point<size_t>>& instance_map,
                         const vtr::Point<size_t>& unique_block_coord,
                         const ucap::Type type, ucap::Block::Builder& root);
}  // namespace openfpga
#endif
