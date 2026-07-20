#pragma once

#include <string>

#include "mif_address_map_fwd.h"
#include "vtr_vector.h"

/********************************************************************
 * In-memory storage for memory_address_map.xml (operating -> physical
 * bit-slice packing):
 *   <memory_address_map>
 *     <address_map pb_type="dpram8x32[0]{dual}.dpram8x16[0]"
 *                  address_offset="0" data_offset="0"/>
 *   </memory_address_map>
 *
 * address_offset / data_offset are bit starts on the physical addr/data
 * ports (e.g. operating addr[0:2] maps to physical addr[3:5] when
 * address_offset=3).
 *******************************************************************/
namespace openfpga {

class MifAddressMap {
 public: /* Types */
  typedef vtr::vector<MifAddressMapId, MifAddressMapId>::const_iterator
    address_map_iterator;
  typedef vtr::Range<address_map_iterator> address_map_range;

 public: /* Constructors */
  MifAddressMap() = default;

 public: /* Accessors: aggregates */
  address_map_range address_maps() const;

 public: /* Accessors */
  std::string pb_type(const MifAddressMapId& map_id) const;
  int address_offset(const MifAddressMapId& map_id) const;
  int data_offset(const MifAddressMapId& map_id) const;
  /* Find entry by exact pb_type string; INVALID if not found */
  MifAddressMapId find_by_pb_type(const std::string& pb_type) const;
  bool empty() const;
  size_t num_address_maps() const;

 public: /* Mutators */
  void clear();
  MifAddressMapId create_address_map(const std::string& pb_type,
                                     int address_offset, int data_offset);

 public: /* Validators */
  bool valid_address_map_id(const MifAddressMapId& map_id) const;

 private: /* Internal data */
  vtr::vector<MifAddressMapId, MifAddressMapId> address_map_ids_;
  vtr::vector<MifAddressMapId, std::string> address_map_pb_type_;
  vtr::vector<MifAddressMapId, int> address_map_address_offset_;
  vtr::vector<MifAddressMapId, int> address_map_data_offset_;
};

} /* namespace openfpga */
