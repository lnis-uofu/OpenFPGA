#include "clock_network.h"

#include <algorithm>

#include "vtr_assert.h"
#include "vtr_log.h"

namespace openfpga {  // Begin namespace openfpga

/************************************************************************
 * Member functions for class ClockNetwork
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/
ClockNetwork::ClockNetwork() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
ClockNetwork::bus_group_range ClockNetwork::buses() const {
  return vtr::make_range(bus_ids_.begin(), bus_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
openfpga::BasicPort ClockNetwork::bus_port(const ClockNetworkId& bus_id) const {
  VTR_ASSERT(valid_bus_id(bus_id));
  return bus_ports_[bus_id];
}

bool ClockNetwork::is_big_endian(const ClockNetworkId& bus_id) const {
  VTR_ASSERT(valid_bus_id(bus_id));
  return bus_big_endians_[bus_id];
}

std::vector<BusPinId> ClockNetwork::bus_pins(const ClockNetworkId& bus_id) const {
  VTR_ASSERT(valid_bus_id(bus_id));
  return bus_pin_ids_[bus_id];
}

int ClockNetwork::pin_index(const BusPinId& pin_id) const {
  VTR_ASSERT(valid_pin_id(pin_id));
  return pin_indices_[pin_id];
}

std::string ClockNetwork::pin_name(const BusPinId& pin_id) const {
  VTR_ASSERT(valid_pin_id(pin_id));
  return pin_names_[pin_id];
}

ClockNetworkId ClockNetwork::find_pin_bus(const std::string& pin_name) const {
  std::map<std::string, BusPinId>::const_iterator result =
    pin_name2id_map_.find(pin_name);
  if (result == pin_name2id_map_.end()) {
    /* Not found, return an invalid id */
    return ClockNetworkId::INVALID();
  }
  /* Found, we should get the parent bus */
  BusPinId pin_id = result->second;
  return pin_parent_bus_ids_[pin_id];
}

ClockNetworkId ClockNetwork::find_bus(const std::string& bus_name) const {
  std::map<std::string, ClockNetworkId>::const_iterator result =
    bus_name2id_map_.find(bus_name);
  if (result == bus_name2id_map_.end()) {
    /* Not found, return an invalid id */
    return ClockNetworkId::INVALID();
  }
  /* Found, we should get the parent bus */
  return result->second;
}

BusPinId ClockNetwork::find_pin(const std::string& pin_name) const {
  std::map<std::string, BusPinId>::const_iterator result =
    pin_name2id_map_.find(pin_name);
  if (result == pin_name2id_map_.end()) {
    /* Not found, return an invalid id */
    return BusPinId::INVALID();
  }
  /* Found, we should get the parent bus */
  return result->second;
}

bool ClockNetwork::empty() const { return 0 == bus_ids_.size(); }

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void ClockNetwork::reserve_buses(const size_t& num_buses) {
  bus_ids_.reserve(num_buses);
  bus_ports_.reserve(num_buses);
  bus_big_endians_.reserve(num_buses);
  bus_pin_ids_.reserve(num_buses);
}

void ClockNetwork::reserve_pins(const size_t& num_pins) {
  pin_ids_.reserve(num_pins);
  pin_indices_.reserve(num_pins);
  pin_names_.reserve(num_pins);
  pin_parent_bus_ids_.reserve(num_pins);
}

ClockNetworkId ClockNetwork::create_bus(const openfpga::BasicPort& bus_port) {
  /* Create a new id */
  ClockNetworkId bus_id = ClockNetworkId(bus_ids_.size());

  bus_ids_.push_back(bus_id);
  bus_ports_.push_back(bus_port);
  bus_big_endians_.push_back(true);
  bus_pin_ids_.emplace_back();

  /* Register to fast look-up */
  auto result = bus_name2id_map_.find(bus_port.get_name());
  if (result == bus_name2id_map_.end()) {
    bus_name2id_map_[bus_port.get_name()] = bus_id;
  } else {
    VTR_LOG_ERROR("Duplicated bus name '%s' in bus group",
                  bus_port.get_name().c_str());
    exit(1);
  }

  return bus_id;
}

void ClockNetwork::set_bus_big_endian(const ClockNetworkId& bus_id,
                                  const bool& big_endian) {
  VTR_ASSERT(valid_bus_id(bus_id));
  bus_big_endians_[bus_id] = big_endian;
}

ClockSpineId ClockNetwork::create_spine(const std::string& name) {
  /* Check if the name is already used or not */
  auto result = spine_name2ids_.find(name);
  if (result != spine_name2ids_.end()) {
    return ClockSpineId::INVALID();
  }

  /* Create a new id */
  ClockSpineId spine_id = ClockSpineId(spine_ids_.size());
  VTR_ASSERT(valid_spine_id(spine_id));

  spine_ids_.push_back(spine_id);
  spine_names_.push_back(name);
  spine_levels_.emplace_back();
  spine_start_points_.emplace_back();
  spine_end_points_.emplace_back();
  spine_switch_points_.emplace_back();
  spine_switch_coords_.emplace_back();
  spine_parent_.emplace_back();
  spine_parent_tree_.emplace_back();

  /* Register to the lookup */
  spine_name2ids_[name] = spine_id;

  return spine_id;
}

void ClockNetwork::set_spine_parent_tree(const ClockSpineId& spine_id, const ClockTreeId& tree_id) {
  VTR_ASSERT(valid_spine_id(spine_id));
  VTR_ASSERT(valid_tree_id(tree_id));
  spine_parent_tree_[spine_id] = tree_id;
}

void ClockNetwork::set_spine_start_point(const ClockSpineId& spine_id, const vtr::Point<int>& coord) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_start_points_[spine_id] = coord;
}

void ClockNetwork::set_spine_end_point(const ClockSpineId& spine_id, const vtr::Point<int>& coord) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_end_points_[spine_id] = coord;
}

void ClockNetwork::add_spine_switch_point(const ClockSpineId& spine_id, const ClockSpineId& drive_spine, const vtr::Point<int>& coord) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_switch_points_[spine_id].push_back(drive_spine);
  spine_switch_coords_[spine_id].push_back(coord);
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
bool ClockNetwork::valid_tree_id(const ClockTreeId& tree_id) const {
  return (size_t(tree_id) < tree_ids_.size()) && (tree_id == tree_ids_[tree_id]);
}

bool ClockNetwork::valid_spine_id(const ClockSpineId& spine_id) const {
  return (size_t(spine_id) < spine_ids_.size()) && (spine_id == spine_ids_[spine_id]);
}

}  // End of namespace openfpga
