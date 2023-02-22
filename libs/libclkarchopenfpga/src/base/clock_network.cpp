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
ClockNetwork::ClockNetwork() {
  is_dirty_ = true;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
ClockNetwork::clock_tree_range ClockNetwork::trees() const {
  return vtr::make_range(tree_ids_.begin(), tree_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
bool ClockNetwork::find_spine(const std::string& name) const {
  auto result = spine_name2id_map_.find(name);
  if (result == spine_name2id_map_.end()) {
    return ClockSpineId::INVALID();
  }
  return result->second;
}

bool ClockNetwork::empty() const { return 0 == tree_ids_.size(); }

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void ClockNetwork::reserve_spines(const size_t& num_spines) {
  spine_ids_.reserve(num_spines);
  spine_names_.reserve(num_spines);
  spine_levels_.reserve(num_spines);
  spine_start_points_.reserve(num_spines);
  spine_end_points_.reserve(num_spines);
  spine_switch_points_.reserve(num_spines);
  spine_switch_coords_.reserve(num_spines);
  spine_parent_.reserve(num_spines);
  spine_parent_tree_.reserve(num_spines);
}

void ClockNetwork::reserve_trees(const size_t& num_trees) {
  tree_ids_.reserve(num_trees);
  tree_names_.reserve(num_trees);
  tree_widths_.reserve(num_trees);
  tree_top_spines_.reserve(num_trees);
}

ClockTreeId ClockNetwork::create_tree(const std::string& name, const size_t& width = 1) {
  /* Create a new id */
  ClockTreeId tree_id = ClockTreeId(tree_ids_.size());

  tree_ids_.push_back(tree_id);
  tree_names_.push_back(name);
  tree_widths_.push_back(width);
  tree_top_spines_.emplace_back();

  /* Register to fast look-up */
  auto result = tree_name2id_map_.find(name);
  if (result == tree_name2id_map_.end()) {
    tree_name2id_map_[name] = tree_id;
  } else {
    VTR_LOG_ERROR("Duplicated clock tree name '%s' in clock network\n",
                  name.c_str());
    exit(1);
  }

  return tree_id;
}

ClockSpineId ClockNetwork::create_spine(const std::string& name) {
  /* Check if the name is already used or not */
  auto result = spine_name2id_map_.find(name);
  if (result != spine_name2id_map_.end()) {
    VTR_LOG_WARN("Unable to create a spine with duplicated name '%s' in clock network\nPlease use the existing spine or rename\n",
                 name.c_str());
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
  spine_name2id_map_[name] = spine_id;

  return spine_id;
}

ClockSpineId ClockNetwork::try_create_spine(const std::string& name) {
  ClockSpineId spine_id = find_spine(name);
  if (!spine_id) {
    spine_id = create_spine(name);
  }
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
