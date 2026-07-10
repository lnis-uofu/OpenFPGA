#pragma once

#include "memory_address_map_fwd.h"
#include "vtr_vector.h"

/********************************************************************
 * In-memory storage for a memory_address_map.xml:
 *   <memory_address_map>
 *     <memory x="2" y="1" id="0" addr_width="3" data_width="16"/>
 *   </memory_address_map>
 *******************************************************************/
namespace openfpga {

class MemoryAddressMap {
 public: /* Types */
  typedef vtr::vector<MemoryAddressId, MemoryAddressId>::const_iterator
    memory_iterator;
  typedef vtr::Range<memory_iterator> memory_range;

 public: /* Constructors */
  MemoryAddressMap() = default;

 public: /* Accessors: aggregates */
  memory_range memories() const;

 public: /* Accessors */
  int coord_x(const MemoryAddressId& memory_id) const;
  int coord_y(const MemoryAddressId& memory_id) const;
  int ram_id(const MemoryAddressId& memory_id) const;
  int addr_width(const MemoryAddressId& memory_id) const;
  int data_width(const MemoryAddressId& memory_id) const;
  bool empty() const;
  size_t num_memories() const;

 public: /* Mutators */
  void clear();
  MemoryAddressId create_memory(int x, int y, int ram_id, int addr_width,
                                int data_width);

 public: /* Validators */
  bool valid_memory_id(const MemoryAddressId& memory_id) const;

 private: /* Internal data */
  vtr::vector<MemoryAddressId, MemoryAddressId> memory_ids_;
  vtr::vector<MemoryAddressId, int> memory_coord_x_;
  vtr::vector<MemoryAddressId, int> memory_coord_y_;
  vtr::vector<MemoryAddressId, int> memory_ram_id_;
  vtr::vector<MemoryAddressId, int> memory_addr_width_;
  vtr::vector<MemoryAddressId, int> memory_data_width_;
};

} /* namespace openfpga */
