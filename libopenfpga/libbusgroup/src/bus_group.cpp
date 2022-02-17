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

BusGroupId BusGroup::create_bus(const openfpga::BasicPort& bus_port) {
  /* Create a new id */
  BusGroupId bus_id = BusGroupId(bus_ids_.size());
  
  bus_ids_.push_back(bus_id);
  bus_ports.push_back(bus_port);
  bus_pin_indices_.emplace_back();
  bus_pin_names_.emplace_back();
  
  return bus_id;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool BusGroup::valid_bus_id(const BusGroupId& bus_id) const {
  return ( size_t(bus_id) < bus_ids_.size() ) && ( bus_id == bus_ids_[bus_id] ); 
}

