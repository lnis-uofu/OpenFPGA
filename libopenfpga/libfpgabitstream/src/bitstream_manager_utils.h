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

} /* end namespace openfpga */

#endif
