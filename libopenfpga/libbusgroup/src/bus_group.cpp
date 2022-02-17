#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

#include "bus_group.h"

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
  /* validate the bus_id */
  VTR_ASSERT(valid_bus_id(bus_id));
  return bus_ports_[bus_id]; 
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
  bus_pin_indices_.reserve(num_buses);
  bus_pin_names_.reserve(num_buses);
}

void BusGroup::reserve_pins(const size_t& num_pins) {
  pin_ids_.reserve(num_pins);
  pin_indices_.reserve(num_pins);
  pin_names_.reserve(num_pins);
}

BusGroupId BusGroup::create_bus(const openfpga::BasicPort& bus_port) {
  /* Create a new id */
  BusGroupId bus_id = BusGroupId(bus_ids_.size());
  
  bus_ids_.push_back(bus_id);
  bus_ports.push_back(bus_port);
  bus_pin_indices_.emplace_back();
  bus_pin_names_.emplace_back();
  
  return bus_id;
}

BusPinId BasicGroup::create_pin(const BusGroupId& bus_id) {
  /* Create a new id */
  BusPinId pin_id = BusPinId(pin_ids_.size());
  
  pin_ids_.push_back(pin_id);

  pin_indices_.emplace_back();
  pin_names_.emplace_back();

  /* Register the pin to the bus */
  VTR_ASSERT(valid_bus_id(bus_id));
  bus_pin_ids_[bus_id].push_back(pin_id);
  
  return bus_id;
}

void BusGroup::set_pin_index(const BusPinId& pin_id, const int& index) {
  VTR_ASSERT(valid_pin_id(pin_id));
  pin_indices_[pin_id] = index;
}

void BusGroup::set_pin_name(const BusPinId& pin_id, const std::string& name) {
  VTR_ASSERT(valid_pin_id(pin_id));
  pin_names_[pin_id] = name;
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

