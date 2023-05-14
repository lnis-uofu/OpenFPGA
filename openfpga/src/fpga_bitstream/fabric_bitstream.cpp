/******************************************************************************
 * This file includes member functions for data structure FabricBitstream
 ******************************************************************************/
#include "fabric_bitstream.h"

#include <algorithm>

#include "openfpga_decode.h"
#include "vtr_assert.h"
#include "fabric_bitstream_schema.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <iostream>
   #include <sys/types.h>
   #include <sys/stat.h>
   #include <fcntl.h>
  #include <stdio.h>

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
    size_t curr_addr_len = size_t(64);
    //std::min(size_t(64), address_length_ - curr_idx * 64);
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
    size_t curr_addr_len = size_t(64);
    //std::min(size_t(64), wl_address_length_ - curr_idx * 64);
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

/******************************************************************************
 * Serialize/deserialize data structure for faster benchmarking after fabric generation
 ******************************************************************************/

int FabricBitstream::write_fabric_bitstream_db(std::string filename){
::capnp::MallocMessageBuilder message;

QLMem_db::FabricBitstreamQLMem::Builder bitstreamDb = message.initRoot<QLMem_db::FabricBitstreamQLMem>();
//numRegions @0 :UInt8 = 1; Always 1 for flattened. Skipping.
//invalidRegionIds @1 :List(UInt8);
auto invalid_reg_list = bitstreamDb.initInvalidRegionIds(invalid_region_ids_.size());
size_t i = 0;
for (const auto& inv: invalid_region_ids_){
  invalid_reg_list.set(i, size_t(inv));
  ++i;
}

//regionBitIds @2 :List(UINT64);
//always only 1 region, so no loop right now
//auto bits_by_reg_builder = bitstreamDb.initRegionBitIds(region_bit_ids_[FabricBitRegionId(0)].size());
//auto bitRegBuilder = bits_by_reg_builder.init(0, region_bit_ids_[FabricBitRegionId(0)].size());
//size_t vec_size = region_bit_ids_[FabricBitRegionId(0)].size() * sizeof(size_t);
//kj::ArrayPtr<byte> bitRegBytes = kj::arrayPtr(region_bit_ids_[0].data(), vec_size);
//bitRegBuilder.setRegionBitIds(0, kj::arrayPtr(region_bit_ids_[FabricBitRegionId(0)].data(), vec_size));
/*
for(size_t k = 0; k<region_bit_ids_[FabricBitRegionId(0)].size(); ++k){
  bits_by_reg_builder.set(k, size_t(region_bit_ids_[FabricBitRegionId(0)].at(FabricBitId(k))));
}
*/

//numBits @3 :UInt64;
bitstreamDb.setNumBits(num_bits_);

//invalidBitIds @4 :List(UInt64);
auto invalid_bit_list = bitstreamDb.initInvalidBitIds(invalid_bit_ids_.size());
i = 0;
for (const auto& inv: invalid_bit_ids_){
  invalid_bit_list.set(i, uint64_t(inv));
  ++i;
}

//configBitIds @5 :List(UInt64);
auto config_id_builder = bitstreamDb.initConfigBitIds(config_bit_ids_.size());
for (size_t j = 0; j < config_bit_ids_.size(); ++j){
  config_id_builder.set(j, size_t(config_bit_ids_[FabricBitId(j)]));
}
// skipping all because they will always have the default value for our purpose
// useAddress @6 :Bool = true;
// useWlAddress @7 :Bool = true;
// addressLength @8 :UInt8 = 64;
// wlAddressLength @9 :UInt8 = 64;

// bitAddress1bits @10 :List(UInt64);
auto bl1_builder = bitstreamDb.initBitAddress1bits(bit_address_1bits_.size());
for (size_t j = 0; j < bit_address_1bits_.size(); ++j){
  auto working_bl = bl1_builder.init(j, bit_address_1bits_[FabricBitId(j)].size());
  for(size_t k = 0; k < bit_address_1bits_[FabricBitId(j)].size(); ++k)
  {
    working_bl.set(k, bit_address_1bits_[FabricBitId(j)][k]);
  }
}

// bitAddressXbits @11 :List(UInt64);
auto blx_builder = bitstreamDb.initBitAddressXbits(bit_address_xbits_.size());
for (size_t j = 0; j < bit_address_xbits_.size(); ++j){
  auto working_bl = blx_builder.init(j, bit_address_xbits_[FabricBitId(j)].size());
  for(size_t k = 0; k < bit_address_1bits_[FabricBitId(j)].size(); ++k)
  {
    working_bl.set(k, bit_address_xbits_[FabricBitId(j)][k]);
  }
}

// bitWlAddress1bits @12 :List(UInt64);
auto wl1_builder = bitstreamDb.initBitWlAddress1bits(bit_wl_address_1bits_.size());
for (size_t j = 0; j < bit_wl_address_1bits_.size(); ++j){
  auto working_wl = wl1_builder.init(j, bit_wl_address_1bits_[FabricBitId(j)].size());
  for(size_t k = 0; k < bit_address_1bits_[FabricBitId(j)].size(); ++k)
  {
    working_wl.set(k, bit_address_xbits_[FabricBitId(j)][k]);
  }
}

// bitWlAddressXbits @13 :List(UInt64);
auto wlx_builder = bitstreamDb.initBitWlAddressXbits(bit_wl_address_xbits_.size());
for (size_t j = 0; j < bit_wl_address_xbits_.size(); ++j){
  auto working_wl = wlx_builder.init(j, bit_wl_address_xbits_[FabricBitId(j)].size());
  for(size_t k = 0; k < bit_wl_address_xbits_[FabricBitId(j)].size(); ++k)
  {
    working_wl.set(k, bit_wl_address_xbits_[FabricBitId(j)][k]);
  }
}

//bitDins @14 :List(Text); skipping for now because it's unused in flatten

//This is POSIX only because of the use of fds.
//it seems like writing to an fd is the only way to get the packing I want in capnproto 0.81
//it will also allow implementing mmap for larger reads in the future. But it's not portable.
//TO-DO: investigate size and speed difference of something like creating a string of bytes & writing & compressing
FILE *f = fopen(filename.c_str(), "w");
int fd = fileno(f);
writePackedMessageToFd(fd, message);
//This should be a void function because the underlying writer is void so I can't pass its exit status along
//TO-DO: change to void and/or reimplement to handle i/o more nicely
return 0;
}

int FabricBitstream::read_fabric_bitstream_db(std::string infile){
  FILE *f = fopen(infile.c_str(), "r");
  int fd = fileno(f);
  ::capnp::PackedFdMessageReader message(fd);
  QLMem_db::FabricBitstreamQLMem::Reader bitstreamDbReader = message.getRoot<QLMem_db::FabricBitstreamQLMem>();
  
  //current device only uses one region. Handle it here instead of build_fabric_bitstream.
  //region bits are handled with config bits 
  num_regions_ = 1;
  region_bit_ids_.emplace_back();

  /*
  //Invalid Bit IDs
  //Commented because it was giving me debugging trouble and the iterator constructors should handle it.
  //Will return to it, either to implement or remove from schema, but it works fine without.
  //I think invalid IDs don't happen in this usage but the lazy iterator code in the header is copied in other places where it could

  auto invalidBitReader = bitstreamDbReader.getInvalidBitIds();
  for (size_t j = 0; j < invalidBitReader.size(); ++j){
    invalid_bit_ids_.insert(FabricBitRegionId(invalidBitReader[j]));
  }
  */

  //config bit IDs
  num_bits_ = bitstreamDbReader.getNumBits();
  auto configBitReader = bitstreamDbReader.getConfigBitIds();
  for (size_t j = 0; j < configBitReader.size(); ++j){
    config_bit_ids_.push_back(ConfigBitId(configBitReader[j]));
    //there's only 1 config region on the current design, so let's save ourselves a loop by adding the IDs to the region list too
    region_bit_ids_[FabricBitRegionId(0)].push_back(FabricBitId(configBitReader[j]));
  }

  //skipping use_address flags & address lengths; both are handled by the calling function
  //bitAddress1bits
  auto bl1Reader = bitstreamDbReader.getBitAddress1bits();
  for (size_t j = 0; j < bl1Reader.size(); ++j){
    auto working_reader = bl1Reader[j];
    for(size_t k = 0; k < working_reader.size(); ++k)
    {
      bit_address_1bits_[FabricBitId(j)].push_back(working_reader[k]);
    }
  }

  //bitAddressXbits
  auto blXReader = bitstreamDbReader.getBitAddressXbits();
  for (size_t j = 0; j < blXReader.size(); ++j){
    auto working_reader = blXReader[j];
    for(size_t k = 0; k < working_reader.size(); ++k)
    {
      bit_address_xbits_[FabricBitId(j)].push_back(working_reader[k]);
    }
  }

  //bit_wl_Address1bits
  auto wl1Reader = bitstreamDbReader.getBitWlAddress1bits();
  for (size_t j = 0; j < wl1Reader.size(); ++j){
    auto working_reader = wl1Reader[j];
    for(size_t k = 0; k < working_reader.size(); ++k)
    {
      bit_wl_address_1bits_[FabricBitId(j)].push_back(working_reader[k]);
    }
  }
  
  //bit_wl_AddressXbits
  auto wlXReader = bitstreamDbReader.getBitWlAddressXbits();
  for (size_t j = 0; j < wlXReader.size(); ++j){
    auto working_reader = wlXReader[j];
    for(size_t k = 0; k < working_reader.size(); ++k)
    {
      bit_wl_address_xbits_[FabricBitId(j)].push_back(working_reader[k]);
    }
  }

  return 0;
}

//private encoding functions
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
