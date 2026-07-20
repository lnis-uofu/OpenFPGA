#include "mif_address_map.h"

#include "vtr_assert.h"

namespace openfpga {

MifAddressMap::address_map_range MifAddressMap::address_maps() const {
  return vtr::make_range(address_map_ids_.begin(), address_map_ids_.end());
}

std::string MifAddressMap::pb_type(const MifAddressMapId& map_id) const {
  VTR_ASSERT(valid_address_map_id(map_id));
  return address_map_pb_type_[map_id];
}

int MifAddressMap::address_offset(const MifAddressMapId& map_id) const {
  VTR_ASSERT(valid_address_map_id(map_id));
  return address_map_address_offset_[map_id];
}

int MifAddressMap::data_offset(const MifAddressMapId& map_id) const {
  VTR_ASSERT(valid_address_map_id(map_id));
  return address_map_data_offset_[map_id];
}

MifAddressMapId MifAddressMap::find_by_pb_type(
  const std::string& pb_type) const {
  for (const MifAddressMapId& map_id : address_map_ids_) {
    if (address_map_pb_type_[map_id] == pb_type) {
      return map_id;
    }
  }
  return MifAddressMapId::INVALID();
}

bool MifAddressMap::empty() const { return address_map_ids_.empty(); }

size_t MifAddressMap::num_address_maps() const {
  return address_map_ids_.size();
}

void MifAddressMap::clear() {
  address_map_ids_.clear();
  address_map_pb_type_.clear();
  address_map_address_offset_.clear();
  address_map_data_offset_.clear();
}

MifAddressMapId MifAddressMap::create_address_map(const std::string& pb_type,
                                                  int address_offset,
                                                  int data_offset) {
  MifAddressMapId map_id(address_map_ids_.size());
  address_map_ids_.push_back(map_id);
  address_map_pb_type_.push_back(pb_type);
  address_map_address_offset_.push_back(address_offset);
  address_map_data_offset_.push_back(data_offset);
  return map_id;
}

bool MifAddressMap::valid_address_map_id(
  const MifAddressMapId& map_id) const {
  return size_t(map_id) < address_map_ids_.size() &&
         map_id == address_map_ids_[map_id];
}

} /* namespace openfpga */
