/******************************************************************************
 * This file includes member functions for data structure FabricBitstream
 ******************************************************************************/
#include <algorithm>

#include "vtr_assert.h"
#include "openfpga_decode.h"
#include "fabric_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Constructor
 *************************************************/
FabricBitstream::FabricBitstream() {
  num_bits_ = 0;
  invalid_bit_ids_.clear();
  address_length_ = 0;
  wl_address_length_ = 0;

  num_regions_ = 0;
  invalid_region_ids_.clear();

  use_address_ = false;
  use_wl_address_ = false;
}

/**************************************************
 * Public Accessors : Aggregates
 *************************************************/
size_t FabricBitstream::num_bits() const {
  return num_bits_;
}

/* Find all the configuration bits */
FabricBitstream::fabric_bit_range FabricBitstream::bits() const {
  return vtr::make_range(fabric_bit_iterator(FabricBitId(0), invalid_bit_ids_),
                         fabric_bit_iterator(FabricBitId(num_bits_), invalid_bit_ids_));
}

size_t FabricBitstream::num_regions() const {
  return num_regions_;
}

/* Find all the configuration bits */
FabricBitstream::fabric_bit_region_range FabricBitstream::regions() const {
  return vtr::make_range(fabric_bit_region_iterator(FabricBitRegionId(0), invalid_region_ids_),
                         fabric_bit_region_iterator(FabricBitRegionId(num_regions_), invalid_region_ids_));
}

std::vector<FabricBitId> FabricBitstream::region_bits(const FabricBitRegionId& region_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_region_id(region_id));

  return region_bit_ids_[region_id];
}

/******************************************************************************
 * Public Accessors
 ******************************************************************************/
ConfigBitId FabricBitstream::config_bit(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));

  return config_bit_ids_[bit_id];
}

std::vector<char> FabricBitstream::bit_address(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);

  return bit_addresses_[bit_id];
}

std::vector<char> FabricBitstream::bit_bl_address(const FabricBitId& bit_id) const {
  return bit_address(bit_id);
}

std::vector<char> FabricBitstream::bit_wl_address(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);
  VTR_ASSERT(true == use_wl_address_);

  return bit_wl_addresses_[bit_id];
}

char FabricBitstream::bit_din(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);

  return bit_dins_[bit_id];
}

bool FabricBitstream::use_address() const {
  return use_address_;
}

bool FabricBitstream::use_wl_address() const {
  return use_wl_address_;
}

/******************************************************************************
 * Public Mutators
 ******************************************************************************/
void FabricBitstream::reserve_bits(const size_t& num_bits) {
  config_bit_ids_.reserve(num_bits);
 
  if (true == use_address_) {
    bit_addresses_.reserve(num_bits);
    bit_dins_.reserve(num_bits);
 
    if (true == use_wl_address_) {
      bit_wl_addresses_.reserve(num_bits);
    }
  }
}

FabricBitId FabricBitstream::add_bit(const ConfigBitId& config_bit_id) {
  FabricBitId bit = FabricBitId(num_bits_);
  /* Add a new bit, and allocate associated data structures */
  num_bits_++;
  config_bit_ids_.push_back(config_bit_id);

  if (true == use_address_) {
    bit_addresses_.emplace_back();
    bit_dins_.emplace_back();
 
    if (true == use_wl_address_) {
      bit_wl_addresses_.emplace_back();
    }
  }


  return bit; 
}

void FabricBitstream::set_bit_address(const FabricBitId& bit_id,
                                      const std::vector<char>& address,
                                      const bool& tolerant_short_address) {
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);
  if (tolerant_short_address) {
    VTR_ASSERT(address_length_ => address.size());
  } else {
    VTR_ASSERT(address_length_ == address.size());
  }
  bit_addresses_[bit_id] = address;
}

void FabricBitstream::set_bit_bl_address(const FabricBitId& bit_id,
                                         const std::vector<char>& address,
                                         const bool& tolerant_short_address) {
  set_bit_address(bit_id, address, tolerant_short_address);
}

void FabricBitstream::set_bit_wl_address(const FabricBitId& bit_id,
                                         const std::vector<char>& address,
                                         const bool& tolerant_short_address) {
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);
  VTR_ASSERT(true == use_wl_address_);
  if (tolerant_short_address) {
    VTR_ASSERT(wl_address_length_ => address.size());
  } else {
    VTR_ASSERT(wl_address_length_ == address.size());
  }
  bit_wl_addresses_[bit_id] = address;
}

void FabricBitstream::set_bit_din(const FabricBitId& bit_id,
                                  const char& din) {
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);
  bit_dins_[bit_id] = din;
}

void FabricBitstream::set_use_address(const bool& enable) {
  /* Add a lock, only can be modified when num bits are zero*/
  if (0 == num_bits_) {
    use_address_ = enable;
  }
}

void FabricBitstream::set_address_length(const size_t& length) {
  if (true == use_address_) {
    address_length_ = length; 
  }
}

void FabricBitstream::set_bl_address_length(const size_t& length) {
  set_address_length(length);
}

void FabricBitstream::set_use_wl_address(const bool& enable) {
  /* Add a lock, only can be modified when num bits are zero*/
  if (0 == num_bits_) {
    use_wl_address_ = enable;
  }
}

void FabricBitstream::set_wl_address_length(const size_t& length) {
  if (true == use_address_) {
    wl_address_length_ = length; 
  }
}

void FabricBitstream::reserve_regions(const size_t& num_regions) {
  region_bit_ids_.reserve(num_regions);
}

FabricBitRegionId FabricBitstream::add_region() {
  FabricBitRegionId region = FabricBitRegionId(num_regions_);
  /* Add a new bit, and allocate associated data structures */
  num_regions_++;
  region_bit_ids_.emplace_back();

  return region; 
}

void FabricBitstream::add_bit_to_region(const FabricBitRegionId& region_id,
                                        const FabricBitId& bit_id) {
  VTR_ASSERT(true == valid_region_id(region_id));
  VTR_ASSERT(true == valid_bit_id(bit_id));
 
  region_bit_ids_[region_id].push_back(bit_id); 
}

void FabricBitstream::reverse() {
  std::reverse(config_bit_ids_.begin(), config_bit_ids_.end());

  if (true == use_address_) {
    std::reverse(bit_addresses_.begin(), bit_addresses_.end());
    std::reverse(bit_dins_.begin(), bit_dins_.end());

    if (true == use_wl_address_) {
      std::reverse(bit_wl_addresses_.begin(), bit_wl_addresses_.end());
    }
  }
}

void FabricBitstream::reverse_region_bits(const FabricBitRegionId& region_id) {
  VTR_ASSERT(true == valid_region_id(region_id));

  std::reverse(region_bit_ids_[region_id].begin(), region_bit_ids_[region_id].end());
}

/******************************************************************************
 * Public Validators
 ******************************************************************************/
bool FabricBitstream::valid_bit_id(const FabricBitId& bit_id) const {
  return (size_t(bit_id) < num_bits_);
}

bool FabricBitstream::valid_region_id(const FabricBitRegionId& region_id) const {
  return (size_t(region_id) < num_regions_);
}

} /* end namespace openfpga */
