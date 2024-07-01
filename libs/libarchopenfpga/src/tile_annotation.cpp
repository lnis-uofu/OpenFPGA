/************************************************************************
 * Member functions for class TileAnnotation
 ***********************************************************************/
#include "tile_annotation.h"

#include <algorithm>

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/
TileAnnotation::TileAnnotation() { return; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
TileAnnotation::global_port_range TileAnnotation::global_ports() const {
  return vtr::make_range(global_port_ids_.begin(), global_port_ids_.end());
}

std::vector<std::string> TileAnnotation::tiles_to_merge_ports() const {
  std::vector<std::string> tile_names;
  for (auto it = tile_ports_to_merge_.begin(); it != tile_ports_to_merge_.end();
       it++) {
    tile_names.push_back(it->first);
  }
  return tile_names;
}

std::vector<std::string> TileAnnotation::tile_ports_to_merge(
  const std::string& tile_name) const {
  std::vector<std::string> port_names;
  const auto& result = tile_ports_to_merge_.find(tile_name);
  if (result == tile_ports_to_merge_.end()) {
    VTR_LOG_WARN("Tile '%s' does not contain any ports to merge!\n",
                 tile_name.c_str());
    return port_names;
  }
  return result->second;
}

/************************************************************************
 * Public Accessors
 ***********************************************************************/
std::string TileAnnotation::global_port_name(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_names_[global_port_id];
}

std::vector<std::string> TileAnnotation::global_port_tile_names(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_tile_names_[global_port_id];
}

std::vector<BasicPort> TileAnnotation::global_port_tile_ports(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_tile_ports_[global_port_id];
}

std::vector<vtr::Point<size_t>> TileAnnotation::global_port_tile_coordinates(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_tile_coordinates_[global_port_id];
}

bool TileAnnotation::global_port_is_clock(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_clock_[global_port_id];
}

bool TileAnnotation::global_port_is_set(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_set_[global_port_id];
}

bool TileAnnotation::global_port_is_reset(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_is_reset_[global_port_id];
}

size_t TileAnnotation::global_port_default_value(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_default_values_[global_port_id];
}

bool TileAnnotation::global_port_thru_dedicated_network(
  const TileGlobalPortId& global_port_id) const {
  return !global_port_clock_arch_tree_name(global_port_id).empty();
}

std::string TileAnnotation::global_port_clock_arch_tree_name(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  return global_port_clock_arch_tree_names_[global_port_id];
}

bool TileAnnotation::is_tile_port_to_merge(const std::string& tile_name,
                                           const std::string& port_name) const {
  const auto& result = tile_ports_to_merge_.find(tile_name);
  if (result == tile_ports_to_merge_.end()) {
    return false;
  }
  return result->second.end() !=
         std::find(result->second.begin(), result->second.end(), port_name);
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
TileGlobalPortId TileAnnotation::create_global_port(
  const std::string& port_name) {
  /* Ensure that the name is unique */
  std::map<std::string, TileGlobalPortId>::iterator it =
    global_port_name2ids_.find(port_name);
  if (it != global_port_name2ids_.end()) {
    return TileGlobalPortId::INVALID();
  }

  /* This is a legal name. we can create a new id */
  TileGlobalPortId port_id = TileGlobalPortId(global_port_ids_.size());
  global_port_ids_.push_back(port_id);
  global_port_names_.push_back(port_name);
  global_port_tile_names_.emplace_back();
  global_port_tile_ports_.emplace_back();
  global_port_tile_coordinates_.emplace_back();
  global_port_is_clock_.push_back(false);
  global_port_clock_arch_tree_names_.emplace_back();
  global_port_is_set_.push_back(false);
  global_port_is_reset_.push_back(false);
  global_port_default_values_.push_back(0);

  /* Register in the name-to-id map */
  global_port_name2ids_[port_name] = port_id;

  return port_id;
}

void TileAnnotation::add_global_port_tile_information(
  const TileGlobalPortId& global_port_id, const std::string& tile_name,
  const BasicPort& tile_port, const vtr::Point<size_t>& tile_coord) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_tile_names_[global_port_id].push_back(tile_name);
  global_port_tile_ports_[global_port_id].push_back(tile_port);
  global_port_tile_coordinates_[global_port_id].push_back(tile_coord);
}

void TileAnnotation::set_global_port_is_clock(
  const TileGlobalPortId& global_port_id, const bool& is_clock) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_clock_[global_port_id] = is_clock;
}

void TileAnnotation::set_global_port_clock_arch_tree_name(
  const TileGlobalPortId& global_port_id, const std::string& clock_tree_name) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_clock_arch_tree_names_[global_port_id] = clock_tree_name;
}

void TileAnnotation::set_global_port_is_set(
  const TileGlobalPortId& global_port_id, const bool& is_set) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_set_[global_port_id] = is_set;
}

void TileAnnotation::set_global_port_is_reset(
  const TileGlobalPortId& global_port_id, const bool& is_reset) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_is_reset_[global_port_id] = is_reset;
}

void TileAnnotation::set_global_port_default_value(
  const TileGlobalPortId& global_port_id, const size_t& default_value) {
  VTR_ASSERT(valid_global_port_id(global_port_id));
  global_port_default_values_[global_port_id] = default_value;
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
/* Validators */
bool TileAnnotation::valid_global_port_id(
  const TileGlobalPortId& global_port_id) const {
  return (size_t(global_port_id) < global_port_ids_.size()) &&
         (global_port_id == global_port_ids_[global_port_id]);
}

bool TileAnnotation::valid_global_port_attributes(
  const TileGlobalPortId& global_port_id) const {
  VTR_ASSERT(valid_global_port_id(global_port_id));

  int attribute_counter = 0;

  if (true == global_port_is_clock_[global_port_id]) {
    attribute_counter++;
  }

  if (true == global_port_is_reset_[global_port_id]) {
    attribute_counter++;
  }

  if (true == global_port_is_set_[global_port_id]) {
    attribute_counter++;
  }

  return ((0 == attribute_counter) || (1 == attribute_counter));
}

int TileAnnotation::add_merge_subtile_ports(const std::string& tile_name,
                                            const std::string& port_name) {
  auto result = tile_ports_to_merge_.find(tile_name);
  if (result == tile_ports_to_merge_.end()) {
    /* Empty list: add a new element */
    tile_ports_to_merge_[tile_name].push_back(port_name);
  } else {
    /* Check if the port name is already in the list, if yes, error out */
    if (result->second.end() ==
        std::find(result->second.begin(), result->second.end(), port_name)) {
      tile_ports_to_merge_[tile_name].push_back(port_name);
    } else {
      VTR_LOG_ERROR(
        "Port '%s' has already been defined twice for tile '%s' to be merged!",
        port_name.c_str(), tile_name.c_str());
      return CMD_EXEC_FATAL_ERROR;
    }
  }
  return CMD_EXEC_SUCCESS;
}

}  // namespace openfpga
