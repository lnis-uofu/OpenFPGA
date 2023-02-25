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
ClockNetwork::ClockNetwork() { is_dirty_ = true; }

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
size_t ClockNetwork::num_trees() const { return trees().size(); }

ClockNetwork::clock_tree_range ClockNetwork::trees() const {
  return vtr::make_range(tree_ids_.begin(), tree_ids_.end());
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
std::string ClockNetwork::default_segment_name() const {
  return default_segment_name_;
}

std::string ClockNetwork::default_switch_name() const {
  return default_switch_name_;
}

std::string ClockNetwork::tree_name(const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_names_[tree_id];
}

size_t ClockNetwork::tree_width(const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_widths_[tree_id];
}

size_t ClockNetwork::tree_depth(const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  if (is_dirty_) {
    VTR_LOG_ERROR("Unable to identify tree depth when data is still dirty!\n");
    exit(1);
  }
  return tree_depths_[tree_id] + 1;
}

std::vector<ClockSpineId> ClockNetwork::spines(
  const ClockTreeId& tree_id) const {
  std::vector<ClockSpineId> ret;
  for (ClockSpineId spine_id : spine_ids_) {
    if (spine_parent_trees_[spine_id] == tree_id) {
      ret.push_back(spine_id);
    }
  }
  return ret;
}

std::string ClockNetwork::spine_name(const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  return spine_names_[spine_id];
}

vtr::Point<int> ClockNetwork::spine_start_point(
  const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  return spine_start_points_[spine_id];
}

vtr::Point<int> ClockNetwork::spine_end_point(
  const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  return spine_end_points_[spine_id];
}

std::vector<ClockSwitchPointId> ClockNetwork::spine_switch_points(
  const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  std::vector<ClockSwitchPointId> ret;
  ret.reserve(spine_switch_points_[spine_id].size());
  for (size_t i = 0; i < spine_switch_points_[spine_id].size(); ++i) {
    ret.push_back(ClockSwitchPointId(i));
  }

  return ret;
}

ClockSpineId ClockNetwork::spine_switch_point_tap(
  const ClockSpineId& spine_id,
  const ClockSwitchPointId& switch_point_id) const {
  VTR_ASSERT(valid_spine_switch_point_id(spine_id, switch_point_id));
  return spine_switch_points_[spine_id][size_t(switch_point_id)];
}

vtr::Point<int> ClockNetwork::spine_switch_point(
  const ClockSpineId& spine_id,
  const ClockSwitchPointId& switch_point_id) const {
  VTR_ASSERT(valid_spine_switch_point_id(spine_id, switch_point_id));
  return spine_switch_coords_[spine_id][size_t(switch_point_id)];
}

ClockSpineId ClockNetwork::find_spine(const std::string& name) const {
  auto result = spine_name2id_map_.find(name);
  if (result == spine_name2id_map_.end()) {
    return ClockSpineId::INVALID();
  }
  return result->second;
}

bool ClockNetwork::empty() const { return 0 == tree_ids_.size(); }

bool ClockNetwork::is_valid() const { return !is_dirty_; }

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
  spine_parents_.reserve(num_spines);
  spine_children_.reserve(num_spines);
  spine_parent_trees_.reserve(num_spines);
}

void ClockNetwork::reserve_trees(const size_t& num_trees) {
  tree_ids_.reserve(num_trees);
  tree_names_.reserve(num_trees);
  tree_widths_.reserve(num_trees);
  tree_top_spines_.reserve(num_trees);
}

void ClockNetwork::set_default_segment_name(const std::string& name) {
  default_segment_name_ = name;
}

void ClockNetwork::set_default_switch_name(const std::string& name) {
  default_switch_name_ = name;
}

ClockTreeId ClockNetwork::create_tree(const std::string& name, size_t width) {
  /* Create a new id */
  ClockTreeId tree_id = ClockTreeId(tree_ids_.size());

  tree_ids_.push_back(tree_id);
  tree_names_.push_back(name);
  tree_widths_.push_back(width);
  tree_depths_.emplace_back();
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
    VTR_LOG_WARN(
      "Unable to create a spine with duplicated name '%s' in clock "
      "network\nPlease use the existing spine or rename\n",
      name.c_str());
    return ClockSpineId::INVALID();
  }

  /* Create a new id */
  ClockSpineId spine_id = ClockSpineId(spine_ids_.size());

  spine_ids_.push_back(spine_id);
  spine_names_.push_back(name);
  spine_levels_.emplace_back(0);
  spine_start_points_.emplace_back();
  spine_end_points_.emplace_back();
  spine_switch_points_.emplace_back();
  spine_switch_coords_.emplace_back();
  spine_parents_.emplace_back();
  spine_children_.emplace_back();
  spine_parent_trees_.emplace_back();

  /* Register to the lookup */
  VTR_ASSERT(valid_spine_id(spine_id));
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

void ClockNetwork::set_spine_parent_tree(const ClockSpineId& spine_id,
                                         const ClockTreeId& tree_id) {
  VTR_ASSERT(valid_spine_id(spine_id));
  VTR_ASSERT(valid_tree_id(tree_id));
  spine_parent_trees_[spine_id] = tree_id;
}

void ClockNetwork::set_spine_start_point(const ClockSpineId& spine_id,
                                         const vtr::Point<int>& coord) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_start_points_[spine_id] = coord;
}

void ClockNetwork::set_spine_end_point(const ClockSpineId& spine_id,
                                       const vtr::Point<int>& coord) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_end_points_[spine_id] = coord;
}

void ClockNetwork::add_spine_switch_point(const ClockSpineId& spine_id,
                                          const ClockSpineId& drive_spine_id,
                                          const vtr::Point<int>& coord) {
  VTR_ASSERT(valid_spine_id(spine_id));
  VTR_ASSERT(valid_spine_id(drive_spine_id));
  spine_switch_points_[spine_id].push_back(drive_spine_id);
  spine_switch_coords_[spine_id].push_back(coord);
  /* Do not allow any spine has different parents */
  if (spine_parents_[drive_spine_id]) {
    VTR_LOG_ERROR(
      "Detect a spine %s' has two parents '%s' and '%s'. Not allowed in a "
      "clock tree!\n",
      spine_name(drive_spine_id).c_str(),
      spine_name(spine_parents_[drive_spine_id]).c_str(),
      spine_name(spine_id).c_str());
    exit(1);
  }
  spine_parents_[drive_spine_id] = spine_id;
  spine_children_[spine_id].push_back(drive_spine_id);
}

bool ClockNetwork::link() {
  is_dirty_ = true;
  for (ClockTreeId tree_id : trees()) {
    if (!link_tree(tree_id)) {
      return false;
    }
  }
  is_dirty_ = false;
  return true;
}

bool ClockNetwork::link_tree(const ClockTreeId& tree_id) {
  if (!link_tree_top_spines(tree_id)) {
    return false;
  }
  if (!sort_tree_spines(tree_id)) {
    return false;
  }
  if (!update_tree_depth(tree_id)) {
    return false;
  }
  return true;
}

bool ClockNetwork::link_tree_top_spines(const ClockTreeId& tree_id) {
  tree_top_spines_[tree_id].clear();
  /* Sort the spines under a tree; assign levels and identify top-level spines
   */
  for (ClockSpineId spine_id : spines(tree_id)) {
    /* Spines that have no parent are the top-level spines*/
    if (!spine_parents_[spine_id]) {
      tree_top_spines_[tree_id].push_back(spine_id);
    }
  }
  return true;
}

bool ClockNetwork::sort_tree_spines(const ClockTreeId& tree_id) {
  for (ClockSpineId spine_id : tree_top_spines_[tree_id]) {
    spine_levels_[spine_id] = 0;
    rec_update_spine_level(spine_id);
  }
  return true;
}

bool ClockNetwork::rec_update_spine_level(const ClockSpineId& spine_id) {
  for (ClockSpineId child_spine_id : spine_children_[spine_id]) {
    spine_levels_[child_spine_id] = spine_levels_[spine_id] + 1;
    rec_update_spine_level(child_spine_id);
  }
  return true;
}

bool ClockNetwork::update_tree_depth(const ClockTreeId& tree_id) {
  size_t depth = 0;
  for (ClockSpineId spine_id : spines(tree_id)) {
    depth = std::max(depth, spine_levels_[spine_id]);
  }
  tree_depths_[tree_id] = depth;
  return true;
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
bool ClockNetwork::valid_tree_id(const ClockTreeId& tree_id) const {
  return (size_t(tree_id) < tree_ids_.size()) &&
         (tree_id == tree_ids_[tree_id]);
}

bool ClockNetwork::valid_spine_id(const ClockSpineId& spine_id) const {
  return (size_t(spine_id) < spine_ids_.size()) &&
         (spine_id == spine_ids_[spine_id]);
}

bool ClockNetwork::valid_spine_switch_point_id(
  const ClockSpineId& spine_id,
  const ClockSwitchPointId& switch_point_id) const {
  if (!valid_spine_id(spine_id)) {
    return false;
  }
  return size_t(switch_point_id) < spine_switch_points_[spine_id].size();
}

}  // End of namespace openfpga
