/******************************************************************************
 * This file includes member functions for data structure FabricBitstream
 ******************************************************************************/
#include "fabric_bitstream.h"

#include <algorithm>

#include "openfpga_decode.h"
#include "vtr_assert.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * FabricBitstreamMemoryBank
 *************************************************/
void FabricBitstreamMemoryBank::add_bit(const fabric_size_t& bit_id,
                                        const fabric_size_t& region_id,
                                        const fabric_size_t& bl,
                                        const fabric_size_t& wl,
                                        const fabric_size_t& bl_addr_size,
                                        const fabric_size_t& wl_addr_size,
                                        bool bit) {
  // Fabric Bit is added in sequential manner and each bit is unique
  VTR_ASSERT((size_t)(bit_id) == fabric_bit_datas.size());
  // Region is added in sequntial manner but it is not unique from fabric bit
  // perspective
  VTR_ASSERT((size_t)(region_id) <= blwl_lengths.size());
  if ((size_t)(region_id) == blwl_lengths.size()) {
    // Add if this is first time
    blwl_lengths.push_back(fabric_blwl_length(bl_addr_size, wl_addr_size));
  } else {
    // Otherwise if the region had been added, it must always be consistent
    VTR_ASSERT(blwl_lengths[region_id].bl == bl_addr_size);
    VTR_ASSERT(blwl_lengths[region_id].wl == wl_addr_size);
  }
  // The BL/WL index must be within respective length
  VTR_ASSERT(bl < blwl_lengths[region_id].bl);
  VTR_ASSERT(wl < blwl_lengths[region_id].wl);
  // We might not need this at all to track the raw data
  // But since it does not use a lot of memory, tracking for good
  fabric_bit_datas.push_back(fabric_bit_data((fabric_size_t)(size_t)(region_id),
                                             (fabric_size_t)(bl),
                                             (fabric_size_t)(wl), bit));
  // This is real compact data
  VTR_ASSERT(datas.size() == masks.size());
  while ((size_t)(region_id) >= datas.size()) {
    datas.emplace_back();
    masks.emplace_back();
  }
  VTR_ASSERT(datas[region_id].size() == masks[region_id].size());
  while ((size_t)(wl) >= datas[region_id].size()) {
    datas[region_id].push_back(std::vector<uint8_t>((bl_addr_size + 7) / 8, 0));
    masks[region_id].push_back(std::vector<uint8_t>((bl_addr_size + 7) / 8, 0));
  }
  // Same uniqie config bit cannot be set twice
  VTR_ASSERT((masks[region_id][wl][bl >> 3] & (1 << (bl & 7))) == 0);
  if (bit) {
    // Mark the data value if bit (or din) is true
    datas[region_id][wl][bl >> 3] |= (1 << (bl & 7));
  }
  // Mark the mask to indicate we had used this bit
  masks[region_id][wl][bl >> 3] |= (1 << (bl & 7));
}

void FabricBitstreamMemoryBank::fast_configuration(
  const bool& fast, const bool& bit_value_to_skip) {
  for (auto& wls : wls_to_skip) {
    wls.clear();
  }
  wls_to_skip.clear();
  for (size_t region = 0; region < datas.size(); region++) {
    wls_to_skip.emplace_back();
    if (fast) {
      for (fabric_size_t wl = 0; wl < blwl_lengths[region].wl; wl++) {
        VTR_ASSERT((size_t)(wl) < datas[region].size());
        bool skip_wl = true;
        for (fabric_size_t bl = 0; bl < blwl_lengths[region].bl && skip_wl;
             bl++) {
          // Only check the bit that being used (marked in the mask),
          // otherwise it is just a don't care, we can skip
          if (masks[region][wl][bl >> 3] & (1 << (bl & 7))) {
            if (datas[region][wl][bl >> 3] & (1 << (bl & 7))) {
              // If bit_value_to_skip=true, and yet the din (recorded in
              // datas) also 1, then we can skip
              skip_wl = bit_value_to_skip;
            } else {
              skip_wl = !bit_value_to_skip;
            }
          }
        }
        if (skip_wl) {
          // Record down that for this region, we will skip this WL
          wls_to_skip[region].push_back(wl);
        }
      }
    }
  }
}

fabric_size_t FabricBitstreamMemoryBank::get_longest_effective_wl_count()
  const {
  // This function check effective WL count
  // Where effective WL is the WL that we want to program after considering
  // fast configuration from all the region, it return the longest
  fabric_size_t longest_wl = 0;
  for (size_t region = 0; region < datas.size(); region++) {
    VTR_ASSERT((size_t)(region) < wls_to_skip.size());
    fabric_size_t current_wl =
      (fabric_size_t)(datas[region].size() - wls_to_skip[region].size());
    if (current_wl > longest_wl) {
      longest_wl = current_wl;
    }
  }
  return longest_wl;
}

fabric_size_t FabricBitstreamMemoryBank::get_total_bl_addr_size() const {
  // Simply total up all the BL addr size
  fabric_size_t bl = 0;
  for (size_t region = 0; region < datas.size(); region++) {
    bl += blwl_lengths[region].bl;
  }
  return bl;
}

fabric_size_t FabricBitstreamMemoryBank::get_total_wl_addr_size() const {
  // Simply total up all the WL addr size
  fabric_size_t wl = 0;
  for (size_t region = 0; region < datas.size(); region++) {
    wl += blwl_lengths[region].wl;
  }
  return wl;
}

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
size_t FabricBitstream::num_bits() const { return num_bits_; }

/* Find all the configuration bits */
FabricBitstream::fabric_bit_range FabricBitstream::bits() const {
  return vtr::make_range(
    fabric_bit_iterator(FabricBitId(0), invalid_bit_ids_),
    fabric_bit_iterator(FabricBitId(num_bits_), invalid_bit_ids_));
}

size_t FabricBitstream::num_regions() const { return num_regions_; }

/* Find all the configuration bits */
FabricBitstream::fabric_bit_region_range FabricBitstream::regions() const {
  return vtr::make_range(
    fabric_bit_region_iterator(FabricBitRegionId(0), invalid_region_ids_),
    fabric_bit_region_iterator(FabricBitRegionId(num_regions_),
                               invalid_region_ids_));
}

std::vector<FabricBitId> FabricBitstream::region_bits(
  const FabricBitRegionId& region_id) const {
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

std::vector<char> FabricBitstream::bit_address(
  const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);

  /* Decode address bits */
  std::vector<char> addr_bits;
  addr_bits.reserve(address_length_);
  for (size_t curr_idx = 0; curr_idx < bit_address_1bits_[bit_id].size();
       curr_idx++) {
    size_t curr_addr_len =
      std::min(size_t(64), address_length_ - curr_idx * 64);
    std::vector<char> curr_addr_vec =
      decode_address_bits(bit_address_1bits_[bit_id][curr_idx],
                          bit_address_xbits_[bit_id][curr_idx], curr_addr_len);
    addr_bits.insert(addr_bits.end(), curr_addr_vec.begin(),
                     curr_addr_vec.end());
  }
  return addr_bits;
}

std::vector<char> FabricBitstream::bit_bl_address(
  const FabricBitId& bit_id) const {
  return bit_address(bit_id);
}

std::vector<char> FabricBitstream::bit_wl_address(
  const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);
  VTR_ASSERT(true == use_wl_address_);

  /* Decode address bits */
  std::vector<char> addr_bits;
  addr_bits.reserve(wl_address_length_);
  for (size_t curr_idx = 0; curr_idx < bit_wl_address_1bits_[bit_id].size();
       curr_idx++) {
    size_t curr_addr_len =
      std::min(size_t(64), wl_address_length_ - curr_idx * 64);
    std::vector<char> curr_addr_vec = decode_address_bits(
      bit_wl_address_1bits_[bit_id][curr_idx],
      bit_wl_address_xbits_[bit_id][curr_idx], curr_addr_len);
    addr_bits.insert(addr_bits.end(), curr_addr_vec.begin(),
                     curr_addr_vec.end());
  }
  return addr_bits;
}

char FabricBitstream::bit_din(const FabricBitId& bit_id) const {
  /* Ensure a valid id */
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);

  return bit_dins_[bit_id];
}

bool FabricBitstream::use_address() const { return use_address_; }

bool FabricBitstream::use_wl_address() const { return use_wl_address_; }

const FabricBitstreamMemoryBank& FabricBitstream::memory_bank_info(
  const bool& fast, const bool& bit_value_to_skip) const {
  VTR_ASSERT(true == use_address_);
  VTR_ASSERT(true == use_wl_address_);
  (const_cast<FabricBitstreamMemoryBank*>(&memory_bank_data_))
    ->fast_configuration(fast, bit_value_to_skip);
  return memory_bank_data_;
}

/******************************************************************************
 * Public Mutators
 ******************************************************************************/
void FabricBitstream::reserve_bits(const size_t& num_bits) {
  config_bit_ids_.reserve(num_bits);

  if (true == use_address_) {
    bit_address_1bits_.reserve(num_bits);
    bit_address_xbits_.reserve(num_bits);
    bit_dins_.reserve(num_bits);

    if (true == use_wl_address_) {
      bit_wl_address_1bits_.reserve(num_bits);
      bit_wl_address_xbits_.reserve(num_bits);
    }
  }
}

FabricBitId FabricBitstream::add_bit(const ConfigBitId& config_bit_id) {
  FabricBitId bit = FabricBitId(num_bits_);
  /* Add a new bit, and allocate associated data structures */
  num_bits_++;
  config_bit_ids_.push_back(config_bit_id);

  if (true == use_address_) {
    bit_address_1bits_.emplace_back();
    bit_address_xbits_.emplace_back();
    bit_dins_.emplace_back();

    if (true == use_wl_address_) {
      bit_wl_address_1bits_.emplace_back();
      bit_wl_address_xbits_.emplace_back();
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
    VTR_ASSERT(address_length_ >= address.size());
  } else {
    VTR_ASSERT(address_length_ == address.size());
  }
  /* Split the address into several 64 vectors */
  for (size_t start_idx = 0; start_idx < address.size();
       start_idx = start_idx + 64) {
    size_t curr_end_idx = std::min(address.size(), start_idx + 64);
    std::vector<char> curr_addr_vec64(address.begin() + start_idx,
                                      address.begin() + curr_end_idx);
    /* Encode bit '1' and bit 'x' into two numbers */
    bit_address_1bits_[bit_id].push_back(encode_address_1bits(curr_addr_vec64));
    bit_address_xbits_[bit_id].push_back(encode_address_xbits(curr_addr_vec64));
  }
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
    VTR_ASSERT(wl_address_length_ >= address.size());
  } else {
    VTR_ASSERT(wl_address_length_ == address.size());
  }
  /* Split the address into several 64 vectors */
  for (size_t start_idx = 0; start_idx < address.size();
       start_idx = start_idx + 64) {
    size_t curr_end_idx = std::min(address.size(), start_idx + 64);
    std::vector<char> curr_addr_vec64(address.begin() + start_idx,
                                      address.begin() + curr_end_idx);
    /* Encode bit '1' and bit 'x' into two numbers */
    bit_wl_address_1bits_[bit_id].push_back(
      encode_address_1bits(curr_addr_vec64));
    bit_wl_address_xbits_[bit_id].push_back(
      encode_address_xbits(curr_addr_vec64));
  }
}

void FabricBitstream::set_bit_din(const FabricBitId& bit_id, const char& din) {
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

void FabricBitstream::set_memory_bank_info(const FabricBitId& bit_id,
                                           const FabricBitRegionId& region_id,
                                           const size_t& bl, const size_t& wl,
                                           const size_t& bl_addr_size,
                                           const size_t& wl_addr_size,
                                           bool bit) {
  // Bit must be valid one
  // We only support this in protocol that use BL and WL address
  VTR_ASSERT(true == valid_bit_id(bit_id));
  VTR_ASSERT(true == use_address_);
  VTR_ASSERT(true == use_wl_address_);
  VTR_ASSERT(bl_addr_size);
  VTR_ASSERT(wl_addr_size);
  // All the basic checking had passed, we can add the data into
  // memory_bank_data_
  memory_bank_data_.add_bit(
    (fabric_size_t)(size_t)(bit_id), (fabric_size_t)(size_t)(region_id),
    (fabric_size_t)(bl), (fabric_size_t)(wl), (fabric_size_t)(bl_addr_size),
    (fabric_size_t)(wl_addr_size), bit);
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
    std::reverse(bit_address_1bits_.begin(), bit_address_1bits_.end());
    std::reverse(bit_address_xbits_.begin(), bit_address_xbits_.end());
    std::reverse(bit_dins_.begin(), bit_dins_.end());

    if (true == use_wl_address_) {
      std::reverse(bit_wl_address_1bits_.begin(), bit_wl_address_1bits_.end());
      std::reverse(bit_wl_address_xbits_.begin(), bit_wl_address_xbits_.end());
    }
  }
}

void FabricBitstream::reverse_region_bits(const FabricBitRegionId& region_id) {
  VTR_ASSERT(true == valid_region_id(region_id));

  std::reverse(region_bit_ids_[region_id].begin(),
               region_bit_ids_[region_id].end());
}

/******************************************************************************
 * Public Validators
 ******************************************************************************/
bool FabricBitstream::valid_bit_id(const FabricBitId& bit_id) const {
  return (size_t(bit_id) < num_bits_);
}

bool FabricBitstream::valid_region_id(
  const FabricBitRegionId& region_id) const {
  return (size_t(region_id) < num_regions_);
}

uint64_t FabricBitstream::encode_address_1bits(
  const std::vector<char>& address) const {
  /* Convert all the 'x' bit into 0 */
  std::vector<char> binary_address = address;
  for (char& bit : binary_address) {
    if (bit == 'x') {
      bit = '0';
    }
  }
  /* Convert the binary address to a number */
  return (uint64_t)bintoi_charvec(binary_address);
}

uint64_t FabricBitstream::encode_address_xbits(
  const std::vector<char>& address) const {
  /* Convert all the '1' bit into 0 and Convert all the 'x' bit into 1 */
  std::vector<char> binary_address = address;
  for (char& bit : binary_address) {
    if (bit == '1') {
      bit = '0';
    }
    if (bit == 'x') {
      bit = '1';
    }
  }
  /* Convert the binary address to a number */
  return (uint64_t)bintoi_charvec(binary_address);
}

std::vector<char> FabricBitstream::decode_address_bits(
  const size_t& bit1, const size_t& bitx, const size_t& addr_len) const {
  /* Decode the bit1 number to a binary vector */
  std::vector<char> ret_vec = itobin_charvec(bit1, addr_len);
  /* Decode the bitx number to a binary vector */
  std::vector<char> bitx_vec = itobin_charvec(bitx, addr_len);
  /* Combine the two vectors: 'x' overwrite any bit '0' and '1' */
  for (size_t ibit = 0; ibit < ret_vec.size(); ++ibit) {
    if (bitx_vec[ibit] == '1') {
      ret_vec[ibit] = 'x';
    }
  }
  return ret_vec;
}

} /* end namespace openfpga */
