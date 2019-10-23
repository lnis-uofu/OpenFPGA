/******************************************************************************
 * This file includes member functions for data structure BitstreamContext 
 ******************************************************************************/
#include "vtr_assert.h"
#include "bitstream_context.h"

/**************************************************
 * Public Accessors : Aggregates
 *************************************************/
/* Find all the configuration bits */
BitstreamContext::config_bit_range BitstreamContext::bits() const {
  return vtr::make_range(bit_ids_.begin(), bit_ids_.end());
}

/******************************************************************************
 * Public Accessors
 ******************************************************************************/
bool BitstreamContext::bit_value(const ConfigBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));

  return bit_values_[bit_id];
}


/******************************************************************************
 * Public Mutators
 ******************************************************************************/
ConfigBitId BitstreamContext::add_bit(const bool& bit_value) {
  ConfigBitId bit = ConfigBitId(bit_ids_.size());
  /* Add a new bit, and allocate associated data structures */
  bit_ids_.push_back(bit);
  bit_values_.push_back(bit_value);
  shared_config_bit_values_.emplace_back();
  bit_parent_modules_.emplace_back();
  bit_parent_instances_.emplace_back();

  return bit; 
}

/******************************************************************************
 * Public Validators
 ******************************************************************************/
bool BitstreamContext::valid_bit_id(const ConfigBitId& bit_id) const {
  return (size_t(bit_id) < bit_ids_.size()) && (bit_id == bit_ids_[bit_id]);
}
