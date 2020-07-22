/********************************************************************
 * This file includes most utilized functions for data structure
 * BitstreamManager
 *
 * Note: These functions are not generic enough so that they
 *       should NOT be a member function! 
 *******************************************************************/
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"

#include "bitstream_manager_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Recursively find the hierarchy of a block of bitstream manager 
 * Return a vector of the block ids, where the top-level block 
 * locates in the head, while the leaf block locates in the tail 
 *   top, next, ... , block
 *******************************************************************/
std::vector<ConfigBlockId> find_bitstream_manager_block_hierarchy(const BitstreamManager& bitstream_manager, 
                                                                  const ConfigBlockId& block) {
  std::vector<ConfigBlockId> block_hierarchy;
  ConfigBlockId temp_block = block;

  /* Generate a tree of parent block */
  while (true == bitstream_manager.valid_block_id(temp_block)) {
    block_hierarchy.push_back(temp_block);
    /* Go to upper level */ 
    temp_block = bitstream_manager.block_parent(temp_block);
  }

  /* Reverse the vector, so that top block stay in the first */
  std::reverse(block_hierarchy.begin(), block_hierarchy.end());

  return block_hierarchy;
}

/********************************************************************
 * Find all the top-level blocks in a bitstream manager, 
 * which have no parents
 *******************************************************************/
std::vector<ConfigBlockId> find_bitstream_manager_top_blocks(const BitstreamManager& bitstream_manager) {
  std::vector<ConfigBlockId> top_blocks;
  for (const ConfigBlockId& blk : bitstream_manager.blocks()) {
    if (ConfigBlockId::INVALID() != bitstream_manager.block_parent(blk)) {
      continue;
    }
    top_blocks.push_back(blk);
  }

  return top_blocks;
}

} /* end namespace openfpga */
