#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "bus_group.h"

namespace openfpga { // Begin namespace openfpga

/************************************************************************
 * Member functions for class BusGroup
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
BusGroup::BusGroup() {
  return;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
BusGroup::bus_group_range BusGroup::buses() const {
  return vtr::make_range(bus_ids_.begin(), bus_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query 
 ***********************************************************************/
openfpga::BasicPort BusGroup::bus_port(const BusGroupId& bus_id) const {
  VTR_ASSERT(valid_bus_id(bus_id));
  return bus_ports_[bus_id]; 
}

std::vector<BusPinId> BusGroup::bus_pins(const BusGroupId& bus_id) const {
  VTR_ASSERT(valid_bus_id(bus_id));
  return bus_pin_ids_[bus_id]; 
}

int BusGroup::pin_index(const BusPinId& pin_id) const {
  VTR_ASSERT(valid_pin_id(pin_id));
  return pin_indices_[pin_id]; 
}

std::string BusGroup::pin_name(const BusPinId& pin_id) const {
  VTR_ASSERT(valid_pin_id(pin_id));
  return pin_names_[pin_id]; 
}

BusGroupId BusGroup::find_pin_bus(const std::string& pin_name) const {
  std::map<std::string, BusPinId>::const_iterator result = pin_name2id_map_.find(pin_name);
  if (result == pin_name2id_map_.end()) {
    /* Not found, return an invalid id */
    return BusGroupId::INVALID();
  }
  /* Found, we should get the parent bus */
  BusPinId pin_id = result->second; 
  return pin_parent_bus_ids_[pin_id];
}

BusGroupId BusGroup::find_bus(const std::string& bus_name) const {
  std::map<std::string, BusGroupId>::const_iterator result = bus_name2id_map_.find(bus_name);
  if (result == bus_name2id_map_.end()) {
    /* Not found, return an invalid id */
    return BusGroupId::INVALID();
  }
  /* Found, we should get the parent bus */
  return result->second; 
}

BusPinId BusGroup::find_pin(const std::string& pin_name) const {
  std::map<std::string, BusPinId>::const_iterator result = pin_name2id_map_.find(pin_name);
  if (result == pin_name2id_map_.end()) {
    /* Not found, return an invalid id */
    return BusPinId::INVALID();
  }
  /* Found, we should get the parent bus */
  return result->second; 
}

bool BusGroup::empty() const {
  return 0 == bus_ids_.size();
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void BusGroup::reserve_buses(const size_t& num_buses) {
  bus_ids_.reserve(num_buses);
  bus_ports_.reserve(num_buses);
  bus_pin_ids_.reserve(num_buses);
}

void BusGroup::reserve_pins(const size_t& num_pins) {
  pin_ids_.reserve(num_pins);
  pin_indices_.reserve(num_pins);
  pin_names_.reserve(num_pins);
  pin_parent_bus_ids_.reserve(num_pins);
}

BusGroupId BusGroup::create_bus(const openfpga::BasicPort& bus_port) {
  /* Create a new id */
  BusGroupId bus_id = BusGroupId(bus_ids_.size());
  
  bus_ids_.push_back(bus_id);
  bus_ports_.push_back(bus_port);
  bus_pin_ids_.emplace_back();

  /* Register to fast look-up */
  auto result = bus_name2id_map_.find(bus_port.get_name());
  if (result == bus_name2id_map_.end()) {
    bus_name2id_map_[bus_port.get_name()] = bus_id;
  } else {
    VTR_LOG_ERROR("Duplicated bus name '%s' in bus group", bus_port.get_name().c_str());
    exit(1);
  }
  
  return bus_id;
}

BusPinId BusGroup::create_pin(const BusGroupId& bus_id, const int& index) {
  /* Create a new id */
  BusPinId pin_id = BusPinId(pin_ids_.size());
  
  pin_ids_.push_back(pin_id);

  pin_indices_.push_back(index);
  pin_names_.emplace_back();

  /* Register the pin to the bus */
  VTR_ASSERT(valid_bus_id(bus_id));
  pin_parent_bus_ids_.push_back(bus_id);
  
  /* If the pin index is beyond the range of the bus_pin_ids, resize it */
  if (size_t(index) >= bus_pin_ids_[bus_id].size()) {
    bus_pin_ids_[bus_id].resize(index + 1);
  }
  bus_pin_ids_[bus_id][index] = pin_id;

  return pin_id;
}


void BusGroup::set_pin_name(const BusPinId& pin_id, const std::string& name) {
  VTR_ASSERT(valid_pin_id(pin_id));
  pin_names_[pin_id] = name;

  /* Register to fast look-up */
  auto result = pin_name2id_map_.find(name);
  if (result == pin_name2id_map_.end()) {
    pin_name2id_map_[name] = pin_id;
  } else {
    VTR_LOG_ERROR("Duplicated pin name '%s' in bus group", name.c_str());
    exit(1);
  }
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
bool BusGroup::valid_bus_id(const BusGroupId& bus_id) const {
  return ( size_t(bus_id) < bus_ids_.size() ) && ( bus_id == bus_ids_[bus_id] ); 
}

bool BusGroup::valid_pin_id(const BusPinId& pin_id) const {
  return ( size_t(pin_id) < pin_ids_.size() ) && ( pin_id == pin_ids_[pin_id] ); 
}

} // End of namespace openfpga
