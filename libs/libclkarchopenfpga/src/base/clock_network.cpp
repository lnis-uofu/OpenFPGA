#include "clock_network.h"

#include <algorithm>

#include "openfpga_port_parser.h"
#include "openfpga_tokenizer.h"
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
  default_segment_id_ = RRSegmentId::INVALID();
  default_tap_switch_id_ = RRSwitchId::INVALID();
  default_driver_switch_id_ = RRSwitchId::INVALID();
  is_dirty_ = true;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
size_t ClockNetwork::num_trees() const { return trees().size(); }

ClockNetwork::clock_tree_range ClockNetwork::trees() const {
  return vtr::make_range(tree_ids_.begin(), tree_ids_.end());
}

std::vector<ClockLevelId> ClockNetwork::levels(
  const ClockTreeId& tree_id) const {
  std::vector<ClockLevelId> ret;
  for (size_t ilvl = 0; ilvl < tree_depth(tree_id); ++ilvl) {
    ret.push_back(ClockLevelId(ilvl));
  }
  return ret;
}

std::vector<ClockTreePinId> ClockNetwork::pins(
  const ClockTreeId& tree_id, const ClockLevelId& level,
  const t_rr_type& track_type, const Direction& direction) const {
  std::vector<ClockTreePinId> ret;
  /* Avoid to repeatedly count the tracks which can be shared by spines
   * For two or more spines that locate in different coordinates, they can share
   * the same routing tracks. Therefore, we only ensure that routing tracks in
   * their demanding direction (INC and DEC) are satisfied
   */
  bool dir_flag = false;
  for (ClockSpineId curr_spine : spines(tree_id)) {
    if (spine_levels_[curr_spine] != size_t(level)) {
      continue;
    }
    if (spine_track_type(curr_spine) == track_type) {
      if (!dir_flag && spine_direction(curr_spine) == direction) {
        ret.reserve(ret.size() + tree_width(spine_parent_trees_[curr_spine]));
        for (size_t i = 0; i < tree_width(spine_parent_trees_[curr_spine]);
             ++i) {
          ret.push_back(ClockTreePinId(i));
        }
        dir_flag = true;
      }
    }
  }
  return ret;
}

std::vector<ClockTreePinId> ClockNetwork::pins(
  const ClockTreeId& tree_id) const {
  std::vector<ClockTreePinId> ret;
  for (size_t i = 0; i < tree_width(tree_id); ++i) {
    ret.push_back(ClockTreePinId(i));
  }
  return ret;
}

/************************************************************************
 * Public Accessors : Basic data query
 ***********************************************************************/
t_rr_type ClockNetwork::spine_track_type(const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_start_end_points(spine_id));
  if ((spine_start_point(spine_id).x() == spine_end_point(spine_id).x()) &&
      (spine_start_point(spine_id).y() == spine_end_point(spine_id).y())) {
    return spine_track_types_[spine_id];
  } else if (spine_start_point(spine_id).y() == spine_end_point(spine_id).y()) {
    return CHANX;
  }
  return CHANY;
}

Direction ClockNetwork::spine_direction(const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_start_end_points(spine_id));
  if (spine_track_type(spine_id) == CHANX) {
    if (spine_start_point(spine_id).x() == spine_end_point(spine_id).x()) {
      return spine_directions_[spine_id];
    } else if (spine_start_point(spine_id).x() <
               spine_end_point(spine_id).x()) {
      return Direction::INC;
    }
  } else {
    VTR_ASSERT(spine_track_type(spine_id) == CHANY);
    if (spine_start_point(spine_id).y() == spine_end_point(spine_id).y()) {
      return spine_directions_[spine_id];
    } else if (spine_start_point(spine_id).y() <
               spine_end_point(spine_id).y()) {
      return Direction::INC;
    }
  }
  return Direction::DEC;
}

size_t ClockNetwork::num_tracks(const ClockTreeId& tree_id,
                                const ClockLevelId& level,
                                const t_rr_type& track_type) const {
  size_t num_tracks = 0;
  /* Avoid to repeatedly count the tracks which can be shared by spines
   * For two or more spines that locate in different coordinates, they can share
   * the same routing tracks. Therefore, we only ensure that routing tracks in
   * their demanding direction (INC and DEC) are satisfied
   */
  std::map<Direction, bool> dir_flags;
  dir_flags[Direction::INC] = false;
  dir_flags[Direction::DEC] = false;
  for (ClockSpineId curr_spine : spines(tree_id)) {
    if (spine_levels_[curr_spine] != size_t(level)) {
      continue;
    }
    if (spine_track_type(curr_spine) == track_type) {
      /* TODO: Deposit routing tracks in both INC and DEC direction, currently
       * this is limited by the connection block build-up algorithm in fabric
       * generator */
      return 2 * tree_width(spine_parent_trees_[curr_spine]);
    }
  }
  return num_tracks;
}

size_t ClockNetwork::num_tracks(const ClockTreeId& tree_id,
                                const ClockLevelId& level,
                                const t_rr_type& track_type,
                                const Direction& direction) const {
  size_t num_tracks = 0;
  /* Avoid to repeatedly count the tracks which can be shared by spines
   * For two or more spines that locate in different coordinates, they can share
   * the same routing tracks. Therefore, we only ensure that routing tracks in
   * their demanding direction (INC and DEC) are satisfied
   */
  for (ClockSpineId curr_spine : spines(tree_id)) {
    if (spine_levels_[curr_spine] != size_t(level)) {
      continue;
    }
    if (spine_track_type(curr_spine) == track_type) {
      if (spine_direction(curr_spine) == direction) {
        /* TODO: Deposit routing tracks in both INC and DEC direction, currently
         * this is limited by the connection block build-up algorithm in fabric
         * generator */
        return tree_width(spine_parent_trees_[curr_spine]);
      }
    }
  }
  return num_tracks;
}

std::string ClockNetwork::default_segment_name() const {
  return default_segment_name_;
}

RRSegmentId ClockNetwork::default_segment() const {
  return default_segment_id_;
}

std::string ClockNetwork::default_tap_switch_name() const {
  return default_tap_switch_name_;
}

std::string ClockNetwork::default_driver_switch_name() const {
  return default_driver_switch_name_;
}

RRSwitchId ClockNetwork::default_tap_switch() const { return default_tap_switch_id_; }
RRSwitchId ClockNetwork::default_driver_switch() const { return default_driver_switch_id_; }

std::string ClockNetwork::tree_name(const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_names_[tree_id];
}

size_t ClockNetwork::max_tree_width() const {
  size_t max_size = 0;
  for (auto itree : trees()) {
    max_size = std::max(tree_width(itree), max_size);
  }
  return max_size;
}

size_t ClockNetwork::max_tree_depth() const {
  size_t max_size = 0;
  for (auto itree : trees()) {
    max_size = std::max(tree_depth(itree), max_size);
  }
  return max_size;
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

std::vector<ClockSpineId> ClockNetwork::tree_top_spines(
  const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_top_spines_[tree_id];
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

ClockLevelId ClockNetwork::spine_level(const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  if (is_dirty_) {
    VTR_LOG_ERROR("Unable to identify spine level when data is still dirty!\n");
    exit(1);
  }
  return ClockLevelId(spine_levels_[spine_id]);
}

std::vector<vtr::Point<int>> ClockNetwork::spine_coordinates(
  const ClockSpineId& spine_id) const {
  vtr::Point<int> start_coord = spine_start_point(spine_id);
  vtr::Point<int> end_coord = spine_end_point(spine_id);
  std::vector<vtr::Point<int>> coords;
  if (Direction::INC == spine_direction(spine_id)) {
    if (CHANX == spine_track_type(spine_id)) {
      for (int ix = start_coord.x(); ix <= end_coord.x(); ix++) {
        coords.push_back(vtr::Point<int>(ix, start_coord.y()));
      }
    } else {
      VTR_ASSERT(CHANY == spine_track_type(spine_id));
      for (int iy = start_coord.y(); iy <= end_coord.y(); iy++) {
        coords.push_back(vtr::Point<int>(start_coord.x(), iy));
      }
    }
  } else {
    VTR_ASSERT(Direction::DEC == spine_direction(spine_id));
    if (CHANX == spine_track_type(spine_id)) {
      for (int ix = start_coord.x(); ix >= end_coord.x(); ix--) {
        coords.push_back(vtr::Point<int>(ix, start_coord.y()));
      }
    } else {
      VTR_ASSERT(CHANY == spine_track_type(spine_id));
      for (int iy = start_coord.y(); iy >= end_coord.y(); iy--) {
        coords.push_back(vtr::Point<int>(start_coord.x(), iy));
      }
    }
  }

  return coords;
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

std::vector<std::string> ClockNetwork::tree_taps(
  const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_taps_[tree_id];
}

std::vector<std::string> ClockNetwork::tree_flatten_taps(
  const ClockTreeId& tree_id, const ClockTreePinId& clk_pin_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  std::vector<std::string> flatten_taps;
  for (const std::string& tap_name : tree_taps_[tree_id]) {
    StringToken tokenizer(tap_name);
    std::vector<std::string> pin_tokens = tokenizer.split(".");
    if (pin_tokens.size() != 2) {
      VTR_LOG_ERROR("Invalid pin name '%s'. Expect <tile>.<port>\n",
                    tap_name.c_str());
      exit(1);
    }
    PortParser tile_parser(pin_tokens[0]);
    BasicPort tile_info = tile_parser.port();
    PortParser pin_parser(pin_tokens[1]);
    BasicPort pin_info = pin_parser.port();
    if (!tile_info.is_valid()) {
      VTR_LOG_ERROR("Invalid pin name '%s' whose subtile index is not valid\n",
                    tap_name.c_str());
      exit(1);
    }
    if (!pin_info.is_valid()) {
      VTR_LOG_ERROR("Invalid pin name '%s' whose pin index is not valid\n",
                    tap_name.c_str());
      exit(1);
    }
    for (size_t& tile_idx : tile_info.pins()) {
      std::string flatten_tile_str =
        tile_info.get_name() + "[" + std::to_string(tile_idx) + "]";
      for (size_t& pin_idx : pin_info.pins()) {
        if (pin_idx != size_t(clk_pin_id)) {
          continue;
        }
        std::string flatten_pin_str =
          pin_info.get_name() + "[" + std::to_string(pin_idx) + "]";
        flatten_taps.push_back(flatten_tile_str + "." + flatten_pin_str);
      }
    }
  }
  return flatten_taps;
}

ClockTreeId ClockNetwork::find_tree(const std::string& name) const {
  auto result = tree_name2id_map_.find(name);
  if (result == tree_name2id_map_.end()) {
    return ClockTreeId::INVALID();
  }
  return result->second;
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

ClockLevelId ClockNetwork::next_level(const ClockLevelId& lvl) const {
  return ClockLevelId(size_t(lvl) + 1);
}

/************************************************************************
 * Public Mutators
 ***********************************************************************/
void ClockNetwork::reserve_spines(const size_t& num_spines) {
  spine_ids_.reserve(num_spines);
  spine_names_.reserve(num_spines);
  spine_levels_.reserve(num_spines);
  spine_start_points_.reserve(num_spines);
  spine_end_points_.reserve(num_spines);
  spine_directions_.reserve(num_spines);
  spine_track_types_.reserve(num_spines);
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
  tree_taps_.reserve(num_trees);
}

void ClockNetwork::set_default_segment(const RRSegmentId& seg_id) {
  default_segment_id_ = seg_id;
}

void ClockNetwork::set_default_tap_switch(const RRSwitchId& switch_id) {
  default_tap_switch_id_ = switch_id;
}

void ClockNetwork::set_default_driver_switch(const RRSwitchId& switch_id) {
  default_driver_switch_id_ = switch_id;
}

void ClockNetwork::set_default_segment_name(const std::string& name) {
  default_segment_name_ = name;
}

void ClockNetwork::set_default_tap_switch_name(const std::string& name) {
  default_tap_switch_name_ = name;
}

void ClockNetwork::set_default_driver_switch_name(const std::string& name) {
  default_driver_switch_name_ = name;
}

ClockTreeId ClockNetwork::create_tree(const std::string& name, size_t width) {
  /* Create a new id */
  ClockTreeId tree_id = ClockTreeId(tree_ids_.size());

  tree_ids_.push_back(tree_id);
  tree_names_.push_back(name);
  tree_widths_.push_back(width);
  tree_depths_.emplace_back();
  tree_taps_.emplace_back();
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
  spine_directions_.emplace_back(Direction::NUM_DIRECTIONS);
  spine_track_types_.emplace_back(NUM_RR_TYPES);
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

void ClockNetwork::set_spine_direction(const ClockSpineId& spine_id,
                                       const Direction& dir) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_directions_[spine_id] = dir;
}

void ClockNetwork::set_spine_track_type(const ClockSpineId& spine_id,
                                        const t_rr_type& type) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_track_types_[spine_id] = type;
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

void ClockNetwork::add_tree_tap(const ClockTreeId& tree_id,
                                const std::string& pin_name) {
  VTR_ASSERT(valid_tree_id(tree_id));
  tree_taps_[tree_id].push_back(pin_name);
}

bool ClockNetwork::link() {
  for (ClockTreeId tree_id : trees()) {
    if (!link_tree(tree_id)) {
      return false;
    }
  }
  return true;
}

bool ClockNetwork::validate_tree() const {
  for (ClockTreeId tree_id : trees()) {
    for (ClockSpineId spine_id : spines(tree_id)) {
      for (ClockSwitchPointId switch_point_id : spine_switch_points(spine_id)) {
        if (!valid_spine_switch_point_id(spine_id, switch_point_id)) {
          VTR_LOG_ERROR(
            "Spine '%s' contains invalid switching point (%lu, %lu)\n",
            spine_name(spine_id).c_str(),
            spine_switch_point(spine_id, switch_point_id).x(),
            spine_switch_point(spine_id, switch_point_id).y());
          return false;
        }
      }
      if (!valid_spine_start_end_points(spine_id)) {
        VTR_LOG_ERROR(
          "Spine '%s' contains invalid starting point (%lu, %lu) or ending "
          "point (%lu, %lu)\n",
          spine_name(spine_id).c_str(), spine_start_point(spine_id).x(),
          spine_start_point(spine_id).y(), spine_end_point(spine_id).x(),
          spine_end_point(spine_id).y());
        return false;
      }
      /* Ensure valid track types */
      if (spine_track_type(spine_id) != spine_track_types_[spine_id]) {
        VTR_LOG_ERROR(
          "Spine '%s' has a mismatch between inferred track type '%s' against "
          "user-defined track type '%s'\n",
          spine_name(spine_id).c_str(),
          rr_node_typename[spine_track_type(spine_id)],
          rr_node_typename[spine_track_types_[spine_id]]);
        return false;
      }
      if (spine_direction(spine_id) != spine_directions_[spine_id]) {
        VTR_LOG_ERROR(
          "Spine '%s' has a mismatch between inferred direction '%s' against "
          "user-defined direction '%s'\n",
          spine_name(spine_id).c_str(),
          DIRECTION_STRING[size_t(spine_direction(spine_id))],
          DIRECTION_STRING[size_t(spine_directions_[spine_id])]);
        return false;
      }
      /* parent spine and child spine should be in different track type */
      ClockSpineId parent_spine = spine_parents_[spine_id];
      if (valid_spine_id(parent_spine)) {
        if (spine_track_type(spine_id) == spine_track_type(parent_spine)) {
          VTR_LOG_ERROR(
            "Spine '%s' and its parent '%s' are in the same track type (both "
            "horizental or vertical). Expect they are othorgonal (one "
            "horizental and one vertical)!\n",
            spine_name(spine_id).c_str(), spine_name(parent_spine).c_str());
          return false;
        }
      }
    }
  }
  return true;
}

bool ClockNetwork::validate() const {
  is_dirty_ = true;
  if (default_segment_id_ && default_switch_id_ && validate_tree()) {
    is_dirty_ = false;
  }
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
  if (!update_spine_attributes(tree_id)) {
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

bool ClockNetwork::update_spine_attributes(const ClockTreeId& tree_id) {
  for (ClockSpineId spine_id : spines(tree_id)) {
    spine_track_types_[spine_id] = spine_track_type(spine_id);
    spine_directions_[spine_id] = spine_direction(spine_id);
  }
  return true;
}

/************************************************************************
 * Internal invalidators/validators
 ***********************************************************************/
bool ClockNetwork::valid_tree_id(const ClockTreeId& tree_id) const {
  return (size_t(tree_id) < tree_ids_.size()) &&
         (tree_id == tree_ids_[tree_id]);
}

bool ClockNetwork::valid_level_id(const ClockTreeId& tree_id,
                                  const ClockLevelId& lvl_id) const {
  return valid_tree_id(tree_id) && (size_t(lvl_id) < tree_depth(tree_id));
}

bool ClockNetwork::is_last_level(const ClockTreeId& tree_id,
                                 const ClockLevelId& lvl_id) const {
  return valid_tree_id(tree_id) && (size_t(lvl_id) == tree_depth(tree_id) - 1);
}

bool ClockNetwork::is_last_level(const ClockSpineId& spine_id) const {
  return spine_level(spine_id) ==
         ClockLevelId(tree_depth(spine_parent_trees_[spine_id]) - 1);
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

bool ClockNetwork::valid_spine_start_end_points(
  const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  if ((spine_start_point(spine_id).x() != spine_end_point(spine_id).x()) &&
      (spine_start_point(spine_id).y() != spine_end_point(spine_id).y())) {
    return false;
  }
  return true;
}

bool ClockNetwork::is_vague_coordinate(const ClockSpineId& spine_id) const {
  return ((spine_start_point(spine_id).x() == spine_end_point(spine_id).x()) &&
          (spine_start_point(spine_id).y() == spine_end_point(spine_id).y()));
}

}  // End of namespace openfpga
