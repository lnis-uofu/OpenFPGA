/******************************************************************************
 * This file includes member functions for data structure BitstreamManager 
 ******************************************************************************/
#include <algorithm>

#include "vtr_assert.h"
#include "bitstream_manager.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructors
 *************************************************/
BitstreamManager::BitstreamManager() {
  num_blocks_ = 0;
  num_bits_ = 0;
  invalid_block_ids_.clear();
  invalid_bit_ids_.clear();
}

/**************************************************
 * Public Accessors : Aggregates
 *************************************************/
/* Find all the configuration bits */
size_t BitstreamManager::num_bits() const {
  return num_bits_;
}

BitstreamManager::config_bit_range BitstreamManager::bits() const {
  return vtr::make_range(config_bit_iterator(ConfigBitId(0), invalid_bit_ids_),
                         config_bit_iterator(ConfigBitId(num_bits_), invalid_bit_ids_));
}

size_t BitstreamManager::num_blocks() const {
  return num_blocks_;
}

/* Find all the configuration blocks */
BitstreamManager::config_block_range BitstreamManager::blocks() const {
  return vtr::make_range(config_block_iterator(ConfigBlockId(0), invalid_block_ids_),
                         config_block_iterator(ConfigBlockId(num_blocks_), invalid_block_ids_));
}

/******************************************************************************
 * Public Accessors
 ******************************************************************************/
bool BitstreamManager::bit_value(const ConfigBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));

  return '1' == bit_values_[bit_id];
}

std::string BitstreamManager::block_name(const ConfigBlockId& block_id) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  return block_names_[block_id];
}

ConfigBlockId BitstreamManager::block_parent(const ConfigBlockId& block_id) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  return parent_block_ids_[block_id];
}

std::vector<ConfigBlockId> BitstreamManager::block_children(const ConfigBlockId& block_id) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  return child_block_ids_[block_id];
}

std::vector<ConfigBitId> BitstreamManager::block_bits(const ConfigBlockId& block_id) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  size_t lsb = block_bit_id_lsbs_[block_id]; 
  size_t length = block_bit_lengths_[block_id]; 

  std::vector<ConfigBitId> bits(length, ConfigBitId::INVALID());

  if (0 == length) {
    return bits;
  }

  for (size_t i = lsb; i < lsb + length; ++i) {
    bits[i - lsb] = ConfigBitId(i);
  }

  return bits;
}

/* Find the child block in a bitstream manager with a given name */
ConfigBlockId BitstreamManager::find_child_block(const ConfigBlockId& block_id, 
                                                 const std::string& child_block_name) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  std::vector<ConfigBlockId> candidates;

  for (const ConfigBlockId& child : block_children(block_id)) {
    if (0 == child_block_name.compare(block_name(child))) {
      candidates.push_back(child);
    }
  }

  /* We should have 0 or 1 candidate! */
  VTR_ASSERT(0 == candidates.size() || 1 == candidates.size());
  if (0 == candidates.size()) {
    /* Not found, return an invalid value */
    return ConfigBlockId::INVALID();
  }
  return candidates[0];
}

int BitstreamManager::block_path_id(const ConfigBlockId& block_id) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  return block_path_ids_[block_id];
}

std::string BitstreamManager::block_input_net_ids(const ConfigBlockId& block_id) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  return block_input_net_ids_[block_id];
}

std::string BitstreamManager::block_output_net_ids(const ConfigBlockId& block_id) const {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));

  return block_output_net_ids_[block_id];
}

/******************************************************************************
 * Public Mutators
 ******************************************************************************/
ConfigBitId BitstreamManager::add_bit(const bool& bit_value) {
  ConfigBitId bit = ConfigBitId(num_bits_);
  /* Add a new bit, and allocate associated data structures */
  num_bits_++;
  if (true == bit_value) {
    bit_values_.push_back('1');
  } else {
    bit_values_.push_back('0');
  }

  return bit; 
}

void BitstreamManager::reserve_blocks(const size_t& num_blocks) {
  block_names_.reserve(num_blocks);
  block_bit_id_lsbs_.reserve(num_blocks);
  block_bit_lengths_.reserve(num_blocks);
  block_path_ids_.reserve(num_blocks);
  block_input_net_ids_.reserve(num_blocks);
  block_output_net_ids_.reserve(num_blocks);
  parent_block_ids_.reserve(num_blocks);
  child_block_ids_.reserve(num_blocks);
}

void BitstreamManager::reserve_bits(const size_t& num_bits) {
  bit_values_.reserve(num_bits);
}

ConfigBlockId BitstreamManager::create_block() {
  ConfigBlockId block = ConfigBlockId(num_blocks_);
  /* Add a new bit, and allocate associated data structures */
  num_blocks_++;
  block_names_.emplace_back();
  block_bit_id_lsbs_.emplace_back(-1);
  block_bit_lengths_.emplace_back(0);
  block_path_ids_.push_back(-2);
  block_input_net_ids_.emplace_back();
  block_output_net_ids_.emplace_back();
  parent_block_ids_.push_back(ConfigBlockId::INVALID());
  child_block_ids_.emplace_back();

  return block; 
}

ConfigBlockId BitstreamManager::add_block(const std::string& block_name) {
  ConfigBlockId block = create_block();
  set_block_name(block, block_name);
  
  return block;
}

void BitstreamManager::set_block_name(const ConfigBlockId& block_id,
                                      const std::string& block_name) {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block_id));
  block_names_[block_id] = block_name;
}

void BitstreamManager::reserve_child_blocks(const ConfigBlockId& parent_block,
                                            const size_t& num_children) {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(parent_block));

  /* Add the child_block to the parent_block */
  child_block_ids_[parent_block].reserve(num_children);
}

void BitstreamManager::add_child_block(const ConfigBlockId& parent_block, const ConfigBlockId& child_block) {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(parent_block));
  VTR_ASSERT(true == valid_block_id(child_block));

  /* We should have only a parent block for each block! */
  VTR_ASSERT(ConfigBlockId::INVALID() == parent_block_ids_[child_block]);

  /* Ensure the child block is not in the list of children of the parent block */
  std::vector<ConfigBlockId>::iterator it = std::find(child_block_ids_[parent_block].begin(), child_block_ids_[parent_block].end(), child_block);
  VTR_ASSERT(it == child_block_ids_[parent_block].end());

  /* Add the child_block to the parent_block */
  child_block_ids_[parent_block].push_back(child_block);
  /* Register the block in the parent of the block */
  parent_block_ids_[child_block] = parent_block;
}

void BitstreamManager::add_block_bits(const ConfigBlockId& block,
                                      const std::vector<bool>& block_bitstream) {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block));

  /* Add the bit to the block, record anchors in bit indexing for block-level searching */
  block_bit_id_lsbs_[block] = num_bits_;
  block_bit_lengths_[block] = block_bitstream.size();
  for (const bool& bit : block_bitstream) {
    add_bit(bit);
  }
}

void BitstreamManager::add_path_id_to_block(const ConfigBlockId& block, const int& path_id) {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block));

  /* Add the bit to the block */
  block_path_ids_[block] = path_id;
}

void BitstreamManager::add_input_net_id_to_block(const ConfigBlockId& block,
                                                 const std::string& input_net_id) {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block));

  /* Add the bit to the block */
  block_input_net_ids_[block] = input_net_id;
}

void BitstreamManager::add_output_net_id_to_block(const ConfigBlockId& block,
                                                  const std::string& output_net_id) {
  /* Ensure the input ids are valid */
  VTR_ASSERT(true == valid_block_id(block));

  /* Add the bit to the block */
  block_output_net_ids_[block] = output_net_id;
}

/******************************************************************************
 * Public Validators
 ******************************************************************************/
bool BitstreamManager::valid_bit_id(const ConfigBitId& bit_id) const {
  return (size_t(bit_id) < num_bits_);
}

bool BitstreamManager::valid_block_id(const ConfigBlockId& block_id) const {
  return (size_t(block_id) < num_blocks_);
}

bool BitstreamManager::valid_block_path_id(const ConfigBlockId& block_id) const {
  return (true == valid_block_id(block_id)) && (-2 != block_path_id(block_id));
}

} /* end namespace openfpga */
