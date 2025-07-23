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
  /* Set a default invalid bounding box */
  empty_tap_bb_ = vtr::Rect<size_t>(1, 0, 1, 0);
  is_dirty_ = true;
}

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
size_t ClockNetwork::num_trees() const { return trees().size(); }

ClockNetwork::clock_tree_range ClockNetwork::trees() const {
  return vtr::make_range(tree_ids_.begin(), tree_ids_.end());
}

ClockNetwork::clock_internal_driver_range ClockNetwork::internal_drivers()
  const {
  return vtr::make_range(internal_driver_ids_.begin(),
                         internal_driver_ids_.end());
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
  const e_rr_type& track_type, const Direction& direction) const {
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
e_rr_type ClockNetwork::spine_track_type(const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_start_end_points(spine_id));
  if ((spine_start_point(spine_id).x() == spine_end_point(spine_id).x()) &&
      (spine_start_point(spine_id).y() == spine_end_point(spine_id).y())) {
    return spine_track_types_[spine_id];
  } else if (spine_start_point(spine_id).y() == spine_end_point(spine_id).y()) {
    return e_rr_type::CHANX;
  }
  return e_rr_type::CHANY;
}

Direction ClockNetwork::spine_direction(const ClockSpineId& spine_id) const {
  VTR_ASSERT(valid_spine_start_end_points(spine_id));
  if (spine_track_type(spine_id) == e_rr_type::CHANX) {
    if (spine_start_point(spine_id).x() == spine_end_point(spine_id).x()) {
      return spine_directions_[spine_id];
    } else if (spine_start_point(spine_id).x() <
               spine_end_point(spine_id).x()) {
      return Direction::INC;
    }
  } else {
    VTR_ASSERT(spine_track_type(spine_id) == e_rr_type::CHANY);
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
                                const e_rr_type& track_type) const {
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
                                const e_rr_type& track_type,
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

RRSwitchId ClockNetwork::default_tap_switch() const {
  return default_tap_switch_id_;
}
RRSwitchId ClockNetwork::default_driver_switch() const {
  return default_driver_switch_id_;
}

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

BasicPort ClockNetwork::tree_global_port(const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_global_ports_[tree_id];
}

size_t ClockNetwork::tree_width(const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_global_ports_[tree_id].get_width();
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

std::vector<ClockInternalDriverId> ClockNetwork::spine_intermediate_drivers(
  const ClockSpineId& spine_id, const vtr::Point<int>& coord) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  /* Convert coord to a unique string */
  std::string coord_str =
    std::to_string(coord.x()) + std::string(",") + std::to_string(coord.y());
  auto result = spine_intermediate_drivers_[spine_id].find(coord_str);
  if (result == spine_intermediate_drivers_[spine_id].end()) {
    return std::vector<ClockInternalDriverId>();
  }
  return result->second;
}

vtr::Point<int> ClockNetwork::spine_intermediate_driver_routing_track_coord(
  const ClockSpineId& spine_id, const vtr::Point<int>& coord) const {
  vtr::Point<int> des_coord(coord.x(), coord.y());
  Direction des_spine_direction = spine_direction(spine_id);
  /* des node depends on the type of routing track and direction. But it
   * should be a starting point at the current SB[x][y] */
  if (des_spine_direction == Direction::INC &&
      spine_track_type(spine_id) == e_rr_type::CHANX) {
    des_coord.set_x(coord.x() + 1);
  }
  if (des_spine_direction == Direction::INC &&
      spine_track_type(spine_id) == e_rr_type::CHANY) {
    des_coord.set_y(coord.y() + 1);
  }
  return des_coord;
}

std::vector<ClockInternalDriverId>
ClockNetwork::spine_intermediate_drivers_by_routing_track(
  const ClockSpineId& spine_id, const vtr::Point<int>& track_coord) const {
  vtr::Point<int> des_coord(track_coord.x(), track_coord.y());
  Direction des_spine_direction = spine_direction(spine_id);
  /* des node depends on the type of routing track and direction. But it
   * should be a starting point at the current SB[x][y] */
  if (des_spine_direction == Direction::INC &&
      spine_track_type(spine_id) == e_rr_type::CHANX) {
    des_coord.set_x(track_coord.x() - 1);
  }
  if (des_spine_direction == Direction::INC &&
      spine_track_type(spine_id) == e_rr_type::CHANY) {
    des_coord.set_y(track_coord.y() - 1);
  }
  return spine_intermediate_drivers(spine_id, des_coord);
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
    if (e_rr_type::CHANX == spine_track_type(spine_id)) {
      for (int ix = start_coord.x(); ix <= end_coord.x(); ix++) {
        coords.push_back(vtr::Point<int>(ix, start_coord.y()));
      }
    } else {
      VTR_ASSERT(e_rr_type::CHANY == spine_track_type(spine_id));
      for (int iy = start_coord.y(); iy <= end_coord.y(); iy++) {
        coords.push_back(vtr::Point<int>(start_coord.x(), iy));
      }
    }
  } else {
    VTR_ASSERT(Direction::DEC == spine_direction(spine_id));
    if (e_rr_type::CHANX == spine_track_type(spine_id)) {
      for (int ix = start_coord.x(); ix >= end_coord.x(); ix--) {
        coords.push_back(vtr::Point<int>(ix, start_coord.y()));
      }
    } else {
      VTR_ASSERT(e_rr_type::CHANY == spine_track_type(spine_id));
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

std::vector<ClockSwitchPointId>
ClockNetwork::find_spine_switch_points_with_coord(
  const ClockSpineId& spine_id, const vtr::Point<int>& coord) const {
  VTR_ASSERT(valid_spine_id(spine_id));
  std::vector<ClockSwitchPointId> ret;
  for (size_t i = 0; i < spine_switch_points_[spine_id].size(); ++i) {
    if (spine_switch_coords_[spine_id][i] == coord) {
      ret.push_back(ClockSwitchPointId(i));
    }
  }

  return ret;
}

std::vector<ClockInternalDriverId>
ClockNetwork::spine_switch_point_internal_drivers(
  const ClockSpineId& spine_id,
  const ClockSwitchPointId& switch_point_id) const {
  VTR_ASSERT(valid_spine_switch_point_id(spine_id, switch_point_id));
  return spine_switch_internal_drivers_[spine_id][size_t(switch_point_id)];
}

std::string ClockNetwork::internal_driver_from_pin(
  const ClockInternalDriverId& int_driver_id) const {
  VTR_ASSERT(valid_internal_driver_id(int_driver_id));
  return internal_driver_from_pins_[int_driver_id];
}

BasicPort ClockNetwork::internal_driver_to_pin(
  const ClockInternalDriverId& int_driver_id) const {
  VTR_ASSERT(valid_internal_driver_id(int_driver_id));
  return internal_driver_to_pins_[int_driver_id];
}

std::vector<ClockTapId> ClockNetwork::tree_taps(
  const ClockTreeId& tree_id) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  return tree_taps_[tree_id];
}

BasicPort ClockNetwork::tap_from_port(const ClockTapId& tap_id) const {
  VTR_ASSERT(valid_tap_id(tap_id));
  return tap_from_ports_[tap_id];
}

std::string ClockNetwork::tap_to_port(const ClockTapId& tap_id) const {
  VTR_ASSERT(valid_tap_id(tap_id));
  return tap_to_ports_[tap_id];
}

ClockNetwork::e_tap_type ClockNetwork::tap_type(
  const ClockTapId& tap_id) const {
  VTR_ASSERT(valid_tap_id(tap_id));
  /* If not a region, it is a default type covering all the coordinates*/
  if (tap_bbs_[tap_id] == empty_tap_bb_) {
    return ClockNetwork::e_tap_type::ALL;
  }
  /* Now check if this a single point */
  if (tap_bbs_[tap_id].height() == 0 && tap_bbs_[tap_id].width() == 0) {
    return ClockNetwork::e_tap_type::SINGLE;
  }
  return ClockNetwork::e_tap_type::REGION;
}

size_t ClockNetwork::tap_x(const ClockTapId& tap_id) const {
  VTR_ASSERT(tap_type(tap_id) == ClockNetwork::e_tap_type::SINGLE);
  return tap_bbs_[tap_id].xmin();
}

size_t ClockNetwork::tap_y(const ClockTapId& tap_id) const {
  VTR_ASSERT(tap_type(tap_id) == ClockNetwork::e_tap_type::SINGLE);
  return tap_bbs_[tap_id].ymin();
}

vtr::Rect<size_t> ClockNetwork::tap_bounding_box(
  const ClockTapId& tap_id) const {
  VTR_ASSERT(tap_type(tap_id) == ClockNetwork::e_tap_type::REGION);
  return tap_bbs_[tap_id];
}

size_t ClockNetwork::tap_step_x(const ClockTapId& tap_id) const {
  VTR_ASSERT(tap_type(tap_id) == ClockNetwork::e_tap_type::REGION);
  return tap_bb_steps_[tap_id].x();
}

size_t ClockNetwork::tap_step_y(const ClockTapId& tap_id) const {
  VTR_ASSERT(tap_type(tap_id) == ClockNetwork::e_tap_type::REGION);
  return tap_bb_steps_[tap_id].y();
}

bool ClockNetwork::valid_tap_coord_in_bb(
  const ClockTapId& tap_id, const vtr::Point<size_t>& tap_coord) const {
  VTR_ASSERT(valid_tap_id(tap_id));
  if (tap_type(tap_id) == ClockNetwork::e_tap_type::ALL) {
    return true;
  }
  if (tap_type(tap_id) == ClockNetwork::e_tap_type::SINGLE &&
      tap_bbs_[tap_id].coincident(tap_coord)) {
    return true;
  }
  if (tap_type(tap_id) == ClockNetwork::e_tap_type::REGION &&
      tap_bbs_[tap_id].coincident(tap_coord)) {
    /* Check if steps are considered, coords still matches */
    bool x_in_bb = false;
    for (size_t ix = tap_bbs_[tap_id].xmin(); ix <= tap_bbs_[tap_id].xmax();
         ix = ix + tap_bb_steps_[tap_id].x()) {
      if (tap_coord.x() == ix) {
        x_in_bb = true;
        break;
      }
    }
    /* Early exit */
    if (!x_in_bb) {
      return false;
    }
    bool y_in_bb = false;
    for (size_t iy = tap_bbs_[tap_id].ymin(); iy <= tap_bbs_[tap_id].ymax();
         iy = iy + tap_bb_steps_[tap_id].y()) {
      if (tap_coord.y() == iy) {
        y_in_bb = true;
        break;
      }
    }
    if (y_in_bb && x_in_bb) {
      return true;
    }
  }
  return false;
}

std::vector<std::string> ClockNetwork::tree_flatten_tap_to_ports(
  const ClockTreeId& tree_id, const ClockTreePinId& clk_pin_id,
  const vtr::Point<size_t>& tap_coord) const {
  VTR_ASSERT(valid_tree_id(tree_id));
  std::vector<std::string> flatten_taps;
  for (ClockTapId tap_id : tree_taps_[tree_id]) {
    VTR_ASSERT(valid_tap_id(tap_id));
    /* Filter out unmatched from ports. Expect [clk_pin_id:clk_pin_id] */
    BasicPort from_port = tap_from_ports_[tap_id];
    if (!from_port.is_valid()) {
      VTR_LOG_ERROR("Invalid from port name '%s' whose index is not valid\n",
                    from_port.to_verilog_string().c_str());
      exit(1);
    }
    if (from_port.get_width() != 1) {
      VTR_LOG_ERROR("Invalid from port name '%s' whose width is not 1\n",
                    from_port.to_verilog_string().c_str());
      exit(1);
    }
    if (from_port.get_lsb() != size_t(clk_pin_id)) {
      continue;
    }
    /* Filter out unmatched coordinates */
    if (!valid_tap_coord_in_bb(tap_id, tap_coord)) {
      continue;
    }
    std::string tap_name = tap_to_ports_[tap_id];
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
        std::string flatten_pin_str =
          pin_info.get_name() + "[" + std::to_string(pin_idx) + "]";
        flatten_taps.push_back(flatten_tile_str + "." + flatten_pin_str);
      }
    }
  }
  return flatten_taps;
}

std::vector<std::string> ClockNetwork::flatten_internal_driver_from_pin(
  const ClockInternalDriverId& int_driver_id,
  const ClockTreePinId& clk_pin_id) const {
  std::vector<std::string> flatten_taps;
  BasicPort des_pin = internal_driver_to_pin(int_driver_id);
  if (!des_pin.is_valid()) {
    VTR_LOG_ERROR(
      "Invalid internal driver destination port name '%s' whose index is not "
      "valid\n",
      des_pin.to_verilog_string().c_str());
    exit(1);
  }
  if (des_pin.get_width() != 1) {
    VTR_LOG_ERROR(
      "Invalid internal driver destination port name '%s' whose width is not "
      "1\n",
      des_pin.to_verilog_string().c_str());
    exit(1);
  }
  if (des_pin.get_lsb() != size_t(clk_pin_id)) {
    return flatten_taps;
  }

  std::string tap_name = internal_driver_from_pin(int_driver_id);
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
      std::string flatten_pin_str =
        pin_info.get_name() + "[" + std::to_string(pin_idx) + "]";
      flatten_taps.push_back(flatten_tile_str + "." + flatten_pin_str);
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
  spine_switch_internal_drivers_.reserve(num_spines);
  spine_intermediate_drivers_.reserve(num_spines);
  spine_parents_.reserve(num_spines);
  spine_children_.reserve(num_spines);
  spine_parent_trees_.reserve(num_spines);
}

void ClockNetwork::reserve_trees(const size_t& num_trees) {
  tree_ids_.reserve(num_trees);
  tree_names_.reserve(num_trees);
  tree_global_ports_.reserve(num_trees);
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

ClockTreeId ClockNetwork::create_tree(const std::string& name,
                                      const BasicPort& global_port) {
  /* Sanity checks */
  if (!global_port.is_valid()) {
    VTR_LOG_ERROR("Invalid global port '%s' for clock tree name '%s'\n",
                  global_port.to_verilog_string().c_str(), name.c_str());
    exit(1);
  }
  /* Create a new id */
  ClockTreeId tree_id = ClockTreeId(tree_ids_.size());

  tree_ids_.push_back(tree_id);
  tree_names_.push_back(name);
  tree_global_ports_.push_back(global_port);
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
  spine_start_points_.emplace_back(-1, -1);
  spine_end_points_.emplace_back(-1, -1);
  spine_directions_.emplace_back(Direction::NUM_DIRECTIONS);
  spine_track_types_.emplace_back(e_rr_type::NUM_RR_TYPES);
  spine_switch_points_.emplace_back();
  spine_switch_coords_.emplace_back(std::vector<vtr::Point<int>>());
  spine_switch_internal_drivers_.emplace_back();
  spine_intermediate_drivers_.emplace_back();
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
                                        const e_rr_type& type) {
  VTR_ASSERT(valid_spine_id(spine_id));
  spine_track_types_[spine_id] = type;
}

ClockSwitchPointId ClockNetwork::add_spine_switch_point(
  const ClockSpineId& spine_id, const ClockSpineId& drive_spine_id,
  const vtr::Point<int>& coord) {
  VTR_ASSERT(valid_spine_id(spine_id));
  VTR_ASSERT(valid_spine_id(drive_spine_id));
  spine_switch_points_[spine_id].push_back(drive_spine_id);
  spine_switch_coords_[spine_id].push_back(coord);
  spine_switch_internal_drivers_[spine_id].emplace_back();
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
  return ClockSwitchPointId(spine_switch_points_[spine_id].size() - 1);
}

ClockInternalDriverId ClockNetwork::add_spine_switch_point_internal_driver(
  const ClockSpineId& spine_id, const ClockSwitchPointId& switch_point_id,
  const std::string& int_driver_from_port,
  const std::string& int_driver_to_port) {
  VTR_ASSERT(valid_spine_id(spine_id));
  VTR_ASSERT(valid_spine_switch_point_id(spine_id, switch_point_id));
  /* Parse ports */
  PortParser to_pin_parser(int_driver_to_port);
  /* Find any existing id for the driver port */
  for (ClockInternalDriverId int_driver_id : internal_driver_ids_) {
    if (internal_driver_from_pins_[int_driver_id] == int_driver_from_port &&
        internal_driver_to_pins_[int_driver_id] == to_pin_parser.port()) {
      spine_switch_internal_drivers_[spine_id][size_t(switch_point_id)]
        .push_back(int_driver_id);
      return int_driver_id;
    }
  }
  /* Reaching here, no existing id can be reused, create a new one */
  ClockInternalDriverId int_driver_id =
    ClockInternalDriverId(internal_driver_ids_.size());
  internal_driver_ids_.push_back(int_driver_id);
  internal_driver_from_pins_.push_back(int_driver_from_port);
  internal_driver_to_pins_.push_back(to_pin_parser.port());
  spine_switch_internal_drivers_[spine_id][size_t(switch_point_id)].push_back(
    int_driver_id);
  return int_driver_id;
}

ClockInternalDriverId ClockNetwork::add_spine_intermediate_driver(
  const ClockSpineId& spine_id, const vtr::Point<int>& coord,
  const std::string& int_driver_from_port,
  const std::string& int_driver_to_port) {
  VTR_ASSERT(valid_spine_id(spine_id));
  /* Convert coord to a unique string */
  std::string coord_str =
    std::to_string(coord.x()) + std::string(",") + std::to_string(coord.y());
  /* Parse ports */
  PortParser to_pin_parser(int_driver_to_port);
  /* Find any existing id for the driver port */
  ClockInternalDriverId int_driver_id_to_add =
    ClockInternalDriverId(internal_driver_ids_.size());
  for (ClockInternalDriverId int_driver_id : internal_driver_ids_) {
    if (internal_driver_from_pins_[int_driver_id] == int_driver_from_port &&
        internal_driver_to_pins_[int_driver_id] == to_pin_parser.port()) {
      int_driver_id_to_add = int_driver_id;
      break;
    }
  }
  /* Reaching here, no existing id can be reused, create a new one */
  if (int_driver_id_to_add ==
      ClockInternalDriverId(internal_driver_ids_.size())) {
    internal_driver_ids_.push_back(int_driver_id_to_add);
    internal_driver_from_pins_.push_back(int_driver_from_port);
    internal_driver_to_pins_.push_back(to_pin_parser.port());
  }
  /* Add it to existing map, avoid duplicated id */
  auto result = spine_intermediate_drivers_[spine_id].find(coord_str);
  if (result == spine_intermediate_drivers_[spine_id].end()) {
    spine_intermediate_drivers_[spine_id][coord_str].push_back(
      int_driver_id_to_add);
  } else {
    if (std::find(result->second.begin(), result->second.end(),
                  int_driver_id_to_add) == result->second.end()) {
      result->second.push_back(int_driver_id_to_add);
    } else {
      VTR_LOG_WARN(
        "Skip intermediate driver (from_port='%s', to_port='%s') at (%s) as it "
        "is duplicated in the clock architecture description file!\n",
        int_driver_from_port.c_str(), int_driver_to_port.c_str(),
        coord_str.c_str());
    }
  }
  return int_driver_id_to_add;
}

ClockTapId ClockNetwork::add_tree_tap(const ClockTreeId& tree_id,
                                      const BasicPort& from_port,
                                      const std::string& to_port) {
  VTR_ASSERT(valid_tree_id(tree_id));
  /* TODO: Consider find existing tap template and avoid duplication in storage
   */
  ClockTapId tap_id = ClockTapId(tap_ids_.size());
  tap_ids_.push_back(tap_id);
  tap_from_ports_.push_back(from_port);
  tap_to_ports_.push_back(to_port);
  tap_bbs_.emplace_back(empty_tap_bb_);
  tap_bb_steps_.emplace_back(vtr::Point<size_t>(0, 0));
  tree_taps_[tree_id].push_back(tap_id);
  return tap_id;
}

bool ClockNetwork::set_tap_bounding_box(const ClockTapId& tap_id,
                                        const vtr::Rect<size_t>& bb) {
  VTR_ASSERT(valid_tap_id(tap_id));
  /* Check the bounding box, ensure it must be valid */
  if (bb.xmax() < bb.xmin() || bb.ymax() < bb.ymin()) {
    VTR_LOG_ERROR(
      "Invalid bounding box (xlow=%lu, ylow=%lu) -> (xhigh=%lu, yhigh=%lu)! "
      "Must follow: xlow <= xhigh, ylow <= yhigh!\n",
      bb.xmin(), bb.ymin(), bb.xmax(), bb.ymax());
    return false;
  }
  tap_bbs_[tap_id] = bb;
  return true;
}

bool ClockNetwork::set_tap_step_x(const ClockTapId& tap_id,
                                  const size_t& step) {
  VTR_ASSERT(valid_tap_id(tap_id));
  /* Must be a valid step >= 1 */
  if (step == 0) {
    VTR_LOG_ERROR(
      "Invalid x-direction step (=%lu) for any bounding box! Expect an integer "
      ">= 1!\n",
      step);
    return false;
  }
  tap_bb_steps_[tap_id].set_x(step);
  return true;
}

bool ClockNetwork::set_tap_step_y(const ClockTapId& tap_id,
                                  const size_t& step) {
  VTR_ASSERT(valid_tap_id(tap_id));
  /* Must be a valid step >= 1 */
  if (step == 0) {
    VTR_LOG_ERROR(
      "Invalid y-direction step (=%lu) for any bounding box! Expect an integer "
      ">= 1!\n",
      step);
    return false;
  }
  tap_bb_steps_[tap_id].set_y(step);
  return true;
}

bool ClockNetwork::link() {
  for (ClockTreeId tree_id : trees()) {
    if (!link_tree(tree_id)) {
      return false;
    }
  }
  return true;
}

bool ClockNetwork::validate_tree_taps() const {
  for (ClockTreeId tree_id : trees()) {
    for (ClockTapId tap_id : tree_taps(tree_id)) {
      /* The from pin name should match the global port */
      if (!tree_global_port(tree_id).mergeable(tap_from_port(tap_id)) ||
          !tree_global_port(tree_id).contained(tap_from_port(tap_id))) {
        VTR_LOG_ERROR(
          "Tap point from_port '%s' is not part of the global port '%s' of "
          "tree '%s'\n",
          tap_from_port(tap_id).to_verilog_string().c_str(),
          tree_global_port(tree_id).to_verilog_string().c_str(),
          tree_name(tree_id).c_str());
        return false;
      }
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
  if (default_segment_id_ && default_tap_switch_id_ &&
      default_driver_switch_id_ && validate_tree() && validate_tree_taps()) {
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

bool ClockNetwork::valid_internal_driver_id(
  const ClockInternalDriverId& int_driver_id) const {
  return (size_t(int_driver_id) < internal_driver_ids_.size()) &&
         (int_driver_id == internal_driver_ids_[int_driver_id]);
}

bool ClockNetwork::valid_tap_id(const ClockTapId& tap_id) const {
  return (size_t(tap_id) < tap_ids_.size()) && (tap_id == tap_ids_[tap_id]);
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
