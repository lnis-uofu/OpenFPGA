#ifndef READ_XML_UNIQUE_BLOCKS_BIN_H
#define READ_XML_UNIQUE_BLOCKS_BIN_H

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
#include "unique_blocks_uxsdcxx.capnp.h"
/********************************************************************
 * This file includes the top-level functions of this library
 * which includes:
 *  -- reads a bin file of unique blocks to the associated
 * data structures: device_rr_gsb
 *******************************************************************/
namespace openfpga {
std::vector<vtr::Point<size_t>> read_bin_unique_instance_coords(
  const ucap::Block::Reader& unique_block);

vtr::Point<size_t> read_bin_unique_block_coord(
  const ucap::Block::Reader& unique_block, ucap::Type& type);

int read_bin_unique_blocks(DeviceRRGSB& device_rr_gsb, const char* file_name,
                           bool verbose_output);
}  // namespace openfpga

#endif
