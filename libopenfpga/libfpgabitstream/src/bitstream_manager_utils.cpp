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

/********************************************************************
 * Find the index of a configuration bit in the children bits of its parent block
 *******************************************************************/
size_t find_bitstream_manager_config_bit_index_in_parent_block(const BitstreamManager& bitstream_manager,
                                                               const ConfigBitId& bit_id) {
  size_t curr_index = 0;
  for (const ConfigBitId& cand_bit : bitstream_manager.block_bits(bitstream_manager.bit_parent_block(bit_id))) {
    if (cand_bit == bit_id) {
      break;
    }
    curr_index++;
  }

  return curr_index;
}

/********************************************************************
 * Find the total number of configuration bits under a block
 * As configuration bits are stored only under the leaf blocks,
 * this function will recursively visit all the child blocks
 * until reaching a leaf block, where we collect the number of bits
 *******************************************************************/
size_t rec_find_bitstream_manager_block_sum_of_bits(const BitstreamManager& bitstream_manager,
                                                    const ConfigBlockId& block) {
  /* For leaf block, return directly with the number of bits, because it has not child block */
  if (0 < bitstream_manager.block_bits(block).size()) {
    VTR_ASSERT_SAFE(bitstream_manager.block_children(block).empty());
    return bitstream_manager.block_bits(block).size();
  }

  size_t sum_of_bits = 0;
  /* Dive to child blocks if this block has any */
  for (const ConfigBlockId& child_block : bitstream_manager.block_children(block)) {
    sum_of_bits += rec_find_bitstream_manager_block_sum_of_bits(bitstream_manager, child_block);
  }

  return sum_of_bits;
}

} /* end namespace openfpga */
