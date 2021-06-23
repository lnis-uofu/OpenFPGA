#ifndef BITSTREAM_MANAGER_UTILS_H
#define BITSTREAM_MANAGER_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "bitstream_manager.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<ConfigBlockId> find_bitstream_manager_block_hierarchy(const BitstreamManager& bitstream_manager, 
                                                                  const ConfigBlockId& block);

std::vector<ConfigBlockId> find_bitstream_manager_top_blocks(const BitstreamManager& bitstream_manager);

size_t find_bitstream_manager_config_bit_index_in_parent_block(const BitstreamManager& bitstream_manager,
                                                               const ConfigBitId& bit_id);

size_t rec_find_bitstream_manager_block_sum_of_bits(const BitstreamManager& bitstream_manager,
                                                    const ConfigBlockId& block);

} /* end namespace openfpga */

#endif
