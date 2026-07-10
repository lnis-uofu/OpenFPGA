#include "memory_address_map.h"

#include "vtr_assert.h"

namespace openfpga {

MemoryAddressMap::memory_range MemoryAddressMap::memories() const {
  return vtr::make_range(memory_ids_.begin(), memory_ids_.end());
}

int MemoryAddressMap::coord_x(const MemoryAddressId& memory_id) const {
  VTR_ASSERT(valid_memory_id(memory_id));
  return memory_coord_x_[memory_id];
}

int MemoryAddressMap::coord_y(const MemoryAddressId& memory_id) const {
  VTR_ASSERT(valid_memory_id(memory_id));
  return memory_coord_y_[memory_id];
}

int MemoryAddressMap::ram_id(const MemoryAddressId& memory_id) const {
  VTR_ASSERT(valid_memory_id(memory_id));
  return memory_ram_id_[memory_id];
}

int MemoryAddressMap::id_width(const MemoryAddressId& memory_id) const {
  VTR_ASSERT(valid_memory_id(memory_id));
  return memory_id_width_[memory_id];
}

int MemoryAddressMap::addr_width(const MemoryAddressId& memory_id) const {
  VTR_ASSERT(valid_memory_id(memory_id));
  return memory_addr_width_[memory_id];
}

int MemoryAddressMap::data_width(const MemoryAddressId& memory_id) const {
  VTR_ASSERT(valid_memory_id(memory_id));
  return memory_data_width_[memory_id];
}

MemoryAddressId MemoryAddressMap::find_by_xy(int x, int y) const {
  for (const MemoryAddressId& memory_id : memory_ids_) {
    if (memory_coord_x_[memory_id] == x && memory_coord_y_[memory_id] == y) {
      return memory_id;
    }
  }
  return MemoryAddressId::INVALID();
}

bool MemoryAddressMap::empty() const { return memory_ids_.empty(); }

size_t MemoryAddressMap::num_memories() const { return memory_ids_.size(); }

void MemoryAddressMap::clear() {
  memory_ids_.clear();
  memory_coord_x_.clear();
  memory_coord_y_.clear();
  memory_ram_id_.clear();
  memory_id_width_.clear();
  memory_addr_width_.clear();
  memory_data_width_.clear();
}

MemoryAddressId MemoryAddressMap::create_memory(int x, int y, int ram_id,
                                                int id_width, int addr_width,
                                                int data_width) {
  MemoryAddressId memory_id(memory_ids_.size());
  memory_ids_.push_back(memory_id);
  memory_coord_x_.push_back(x);
  memory_coord_y_.push_back(y);
  memory_ram_id_.push_back(ram_id);
  memory_id_width_.push_back(id_width);
  memory_addr_width_.push_back(addr_width);
  memory_data_width_.push_back(data_width);
  return memory_id;
}

bool MemoryAddressMap::valid_memory_id(const MemoryAddressId& memory_id) const {
  return (size_t(memory_id) < memory_ids_.size()) &&
         (memory_id == memory_ids_[memory_id]);
}

} /* namespace openfpga */
