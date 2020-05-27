/******************************************************************************
 * This file includes member functions for data structure FabricBitstream
 ******************************************************************************/
#include <algorithm>

#include "vtr_assert.h"
#include "fabric_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors : Aggregates
 *************************************************/
/* Find all the configuration bits */
FabricBitstream::fabric_bit_range FabricBitstream::bits() const {
  return vtr::make_range(bit_ids_.begin(), bit_ids_.end());
}

/******************************************************************************
 * Public Accessors
 ******************************************************************************/
ConfigBitId FabricBitstream::config_bit(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));

  return config_bit_ids_[bit_id];
}

std::vector<bool> FabricBitstream::bit_address(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));

  return bit_addresses_[bit_id];
}

bool FabricBitstream::bit_din(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));

  return bit_dins_[bit_id];
}

/******************************************************************************
 * Public Mutators
 ******************************************************************************/
FabricBitId FabricBitstream::add_bit(const ConfigBitId& config_bit_id) {
  FabricBitId bit = FabricBitId(bit_ids_.size());
  /* Add a new bit, and allocate associated data structures */
  bit_ids_.push_back(bit);
  config_bit_ids_.push_back(config_bit_id);
  bit_addresses_.emplace_back();
  bit_dins_.push_back(false);

  return bit; 
}

void FabricBitstream::set_bit_address(const FabricBitId& bit_id,
                                      const std::vector<bool>& address) {
  VTR_ASSERT(true == valid_bit_id(bit_id));
  bit_addresses_[bit_id] = address;
}

void FabricBitstream::set_bit_din(const FabricBitId& bit_id,
                                  const bool& din) {
  VTR_ASSERT(true == valid_bit_id(bit_id));
  bit_dins_[bit_id] = din;
}

void FabricBitstream::reverse() {
  std::reverse(config_bit_ids_.begin(), config_bit_ids_.end());
  std::reverse(bit_addresses_.begin(), bit_addresses_.end());
  std::reverse(bit_dins_.begin(), bit_dins_.end());
}

/******************************************************************************
 * Public Validators
 ******************************************************************************/
bool FabricBitstream::valid_bit_id(const FabricBitId& bit_id) const {
  return (size_t(bit_id) < bit_ids_.size()) && (bit_id == bit_ids_[bit_id]);
}

} /* end namespace openfpga */
