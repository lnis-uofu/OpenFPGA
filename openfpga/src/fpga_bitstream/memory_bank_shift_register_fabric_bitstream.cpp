#include "vtr_assert.h"
#include "memory_bank_shift_register_fabric_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

MemoryBankShiftRegisterFabricBitstream::word_range MemoryBankShiftRegisterFabricBitstream::words() const {
  return vtr::make_range(bitstream_word_ids_.begin(), bitstream_word_ids_.end());
}

size_t MemoryBankShiftRegisterFabricBitstream::num_words() const {
  return bitstream_word_ids_.size();
}

size_t MemoryBankShiftRegisterFabricBitstream::word_size() const {
  /* For a fast runtime, we just inspect the last element
   * It is the validator which should ensure all the words have a uniform size
   */
  return bitstream_word_bls_[bitstream_word_ids_.back()].size();
}

size_t MemoryBankShiftRegisterFabricBitstream::bl_width() const {
  /* For a fast runtime, we just inspect the last element
   * It is the validator which should ensure all the words have a uniform size
   */
  return bitstream_word_bls_[bitstream_word_ids_.back()].back().size();
}

size_t MemoryBankShiftRegisterFabricBitstream::wl_width() const {
  /* For a fast runtime, we just inspect the last element
   * It is the validator which should ensure all the words have a uniform size
   */
  return bitstream_word_wls_[bitstream_word_ids_.back()].back().size();
}

std::vector<std::string> MemoryBankShiftRegisterFabricBitstream::bl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const {
  VTR_ASSERT(valid_word_id(word_id));
  return bitstream_word_bls_[word_id];
}

std::vector<std::string> MemoryBankShiftRegisterFabricBitstream::wl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const {
  VTR_ASSERT(valid_word_id(word_id));
  return bitstream_word_wls_[word_id];
}

std::vector<std::string> MemoryBankShiftRegisterFabricBitstream::blwl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const {
  VTR_ASSERT(valid_word_id(word_id));
  std::vector<std::string> blwl_vec = bitstream_word_bls_[word_id];
  VTR_ASSERT(blwl_vec.size() == bitstream_word_wls_[word_id].size());
  for (size_t iwl = 0; iwl < bitstream_word_wls_[word_id].size(); ++iwl) {
    blwl_vec[iwl] += bitstream_word_wls_[word_id][iwl]; 
  }
  return blwl_vec;
}

MemoryBankShiftRegisterFabricBitstreamWordId MemoryBankShiftRegisterFabricBitstream::create_word() {
  /* Create a new id*/
  MemoryBankShiftRegisterFabricBitstreamWordId word_id = MemoryBankShiftRegisterFabricBitstreamWordId(bitstream_word_ids_.size());
  /* Update the id list */
  bitstream_word_ids_.push_back(word_id);
  
  /* Initialize other attributes */
  bitstream_word_bls_.emplace_back();
  bitstream_word_wls_.emplace_back();

  return word_id;
}

void MemoryBankShiftRegisterFabricBitstream::add_bl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id,
                                                            const std::string& bl_vec) {
  VTR_ASSERT(valid_word_id(word_id));
  return bitstream_word_bls_[word_id].push_back(bl_vec);
}

void MemoryBankShiftRegisterFabricBitstream::add_wl_vectors(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id,
                                                            const std::string& wl_vec) {
  VTR_ASSERT(valid_word_id(word_id));
  return bitstream_word_wls_[word_id].push_back(wl_vec);
}

bool MemoryBankShiftRegisterFabricBitstream::valid_word_id(const MemoryBankShiftRegisterFabricBitstreamWordId& word_id) const {
  return ( size_t(word_id) < bitstream_word_ids_.size() ) && ( word_id == bitstream_word_ids_[word_id] ); 
}

} /* end namespace openfpga */
