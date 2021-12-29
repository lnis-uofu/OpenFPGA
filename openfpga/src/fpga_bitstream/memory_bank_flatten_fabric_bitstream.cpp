#include "memory_bank_flatten_fabric_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

size_t MemoryBankFlattenFabricBitstream::size() const {
  return bitstream_.size();
}

size_t MemoryBankFlattenFabricBitstream::bl_vector_size() const {
  /* The address sizes and data input sizes are the same across any element, 
   * just get it from the 1st element to save runtime
   */
  size_t bl_vec_size = 0;
  for (const auto& bl_vec : bitstream_.begin()->second) {
    bl_vec_size += bl_vec.size();
  } 
  return bl_vec_size;
}

size_t MemoryBankFlattenFabricBitstream::wl_vector_size() const {
  /* The address sizes and data input sizes are the same across any element, 
   * just get it from the 1st element to save runtime
   */
  size_t wl_vec_size = 0;
  for (const auto& wl_vec : bitstream_.begin()->first) {
    wl_vec_size += wl_vec.size();
  } 
  return wl_vec_size;
}

std::vector<std::string> MemoryBankFlattenFabricBitstream::bl_vector(const std::vector<std::string>& wl_vec) const {
  return bitstream_.at(wl_vec);
}

std::vector<std::vector<std::string>> MemoryBankFlattenFabricBitstream::wl_vectors() const {
  std::vector<std::vector<std::string>> wl_vecs;
  for (const auto& pair : bitstream_) {
    wl_vecs.push_back(pair.first);
  }
  return wl_vecs;
}

void MemoryBankFlattenFabricBitstream::add_blwl_vectors(const std::vector<std::string>& bl_vec,
                                                        const std::vector<std::string>& wl_vec) {
  /* TODO: Add sanity check. Give a warning if the wl vector is already there */
  bitstream_[wl_vec] = bl_vec;
}

} /* end namespace openfpga */
