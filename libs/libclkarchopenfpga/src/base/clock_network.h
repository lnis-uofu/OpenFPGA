#ifndef CLOCK_NETWORK_H
#define CLOCK_NETWORK_H

/********************************************************************
 * This file include the declaration of pin constraints
 *******************************************************************/
#include <array>
#include <map>
#include <string>

/* Headers from vtrutil library */
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* Headers from openfpgautil library */
#include "clock_network_fwd.h"
#include "openfpga_port.h"
#include "rr_graph_fwd.h"
#include "rr_node_types.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * A data structure to describe a clock network
 * A clock network consists of a number of clock trees
 * each of which has:
 * - a unique id
 * - different entry point
 *
 * Typical usage:
 * --------------
 *   // Create an object of clock network
 *   ClockNetwork clk_ntwk;
 *   // Create a new clock tree which contains 8 clock pins
 *   ClockTreeId clk_tree_id = clk_ntwk.create_tree("tree1")
 *   // Add a spine to the clock tree
 *   ClockSpineId clk_spine_id = clk_ntwk.create_spine("tree1_spine0");
 *
 *******************************************************************/
class ClockNetwork {
 public: /* Types */
  typedef vtr::vector<ClockTreeId, ClockTreeId>::const_iterator
    clock_tree_iterator;
  /* Create range */
  typedef vtr::Range<clock_tree_iterator> clock_tree_range;
  typedef vtr::vector<ClockInternalDriverId,
                      ClockInternalDriverId>::const_iterator
    clock_internal_driver_iterator;
  /* Create range */
  typedef vtr::Range<clock_internal_driver_iterator>
    clock_internal_driver_range;
  /* Type of tap points */
  enum class e_tap_type : unsigned char { ALL = 0, SINGLE, REGION, NUM_TYPES };

 public: /* Constructors */
  ClockNetwork();

 public: /* Accessors: aggregates */
  size_t num_trees() const;
  clock_tree_range trees() const;
  clock_internal_driver_range internal_drivers() const;
  /* Return the range of clock levels */
  std::vector<ClockLevelId> levels(const ClockTreeId& tree_id) const;
  /* Return a list of spine id under a clock tree */
  std::vector<ClockSpineId> spines(const ClockTreeId& tree_id) const;
  /* Return a list of clock pins in a bus of clock tree at a given level and
   * direction */
  std::vector<ClockTreePinId> pins(const ClockTreeId& tree_id,
                                   const ClockLevelId& level,
                                   const e_rr_type& track_type,
                                   const Direction& direction) const;
  std::vector<ClockTreePinId> pins(const ClockTreeId& tree_id) const;

 public: /* Public Accessors: Basic data query */
  /* Return the number of routing tracks required by a selected clock tree at a
   * given level and direction */
  size_t num_tracks(const ClockTreeId& tree_id, const ClockLevelId& level,
                    const e_rr_type& track_type) const;
  size_t num_tracks(const ClockTreeId& tree_id, const ClockLevelId& level,
                    const e_rr_type& track_type,
                    const Direction& direction) const;
  /* Return the id of default routing segment, use this to find detailed segment
   * information from RRGraph */
  RRSegmentId default_segment() const;
  std::string default_segment_name() const;
  RRSwitchId default_tap_switch() const;
  std::string default_tap_switch_name() const;
  RRSwitchId default_driver_switch() const;
  std::string default_driver_switch_name() const;
  std::string tree_name(const ClockTreeId& tree_id) const;
  BasicPort tree_global_port(const ClockTreeId& tree_id) const;
  size_t tree_width(const ClockTreeId& tree_id) const;
  size_t tree_depth(const ClockTreeId& tree_id) const;
  size_t max_tree_width() const;
  size_t max_tree_depth() const;
  std::vector<ClockSpineId> tree_top_spines(const ClockTreeId& tree_id) const;
  std::string spine_name(const ClockSpineId& spine_id) const;
  vtr::Point<int> spine_start_point(const ClockSpineId& spine_id) const;
  vtr::Point<int> spine_end_point(const ClockSpineId& spine_id) const;
  /* Find the intermediate drivers by the SB coordinate */
  std::vector<ClockInternalDriverId> spine_intermediate_drivers(
    const ClockSpineId& spine_id, const vtr::Point<int>& coord) const;
  /* Find the coordinate of routing track which the intermediate driver will
   * driver. Note that the coordinate may be different than the coordinate of
   * intermeidate driver. One of the exceptions lies in the CHANX with INC
   * direction, which starts actually on the routing tracks on the right side of
   * a SB, resulting in x -> x + 1. Another exception is on the CHANY with INC
   * direction, which starts actually on the routing tracks on the top side of a
   * SB, resulting in y - > y + 1. This function is to provide an official
   * conversion the coordinates. */
  vtr::Point<int> spine_intermediate_driver_routing_track_coord(
    const ClockSpineId& spine_id, const vtr::Point<int>& coord) const;
  /* Find the intermediate drivers by the routing track starting point. Note
   * that the routing track starting point may be different from the SB
   * coordinate. See the exceptions in the
   * spine_intermediate_driver_track_coord() */
  std::vector<ClockInternalDriverId>
  spine_intermediate_drivers_by_routing_track(
    const ClockSpineId& spine_id, const vtr::Point<int>& track_coord) const;

  /* Return the level where the spine locates in the multi-layer clock tree
   * structure */
  ClockLevelId spine_level(const ClockSpineId& spine_id) const;
  /* Return the list of coordinates that a spine will go across */
  std::vector<vtr::Point<int>> spine_coordinates(
    const ClockSpineId& spine_id) const;
  /* Identify the direction of a spine, depending on its starting and ending
   * points
   * - CHANX represents a horizental routing track
   * - CHANY represents a vertical routing track
   */
  e_rr_type spine_track_type(const ClockSpineId& spine_id) const;
  /* Identify the direction of a spine, depending on its starting and ending
   * points INC represents
   *  - a CHANX track goes from left to right, or
   *  - a CHANY track goes from bottom to top
   * DEC represents
   *  - a CHANX track goes from right to left, or
   *  - a CHANY track goes from top to bottom
   */
  Direction spine_direction(const ClockSpineId& spine_id) const;
  /* Return the unique id of switch points under a clock spine*/
  std::vector<ClockSwitchPointId> spine_switch_points(
    const ClockSpineId& spine_id) const;
  ClockSpineId spine_switch_point_tap(
    const ClockSpineId& spine_id,
    const ClockSwitchPointId& switch_point_id) const;
  vtr::Point<int> spine_switch_point(
    const ClockSpineId& spine_id,
    const ClockSwitchPointId& switch_point_id) const;

  /* Find all the switching points at a given coordinate */
  std::vector<ClockSwitchPointId> find_spine_switch_points_with_coord(
    const ClockSpineId& spine_id, const vtr::Point<int>& coord) const;

  std::vector<ClockInternalDriverId> spine_switch_point_internal_drivers(
    const ClockSpineId& spine_id,
    const ClockSwitchPointId& switch_point_id) const;
  std::string internal_driver_from_pin(
    const ClockInternalDriverId& int_driver_id) const;
  std::vector<std::string> flatten_internal_driver_from_pin(
    const ClockInternalDriverId& int_driver_id,
    const ClockTreePinId& clk_pin_id) const;
  BasicPort internal_driver_to_pin(
    const ClockInternalDriverId& int_driver_id) const;

  /* Return the original list of tap pins that is in storage; useful for parsers
   */
  std::vector<ClockTapId> tree_taps(const ClockTreeId& tree_id) const;
  /* Return the source ports for a given tap */
  BasicPort tap_from_port(const ClockTapId& tap_id) const;
  /* Return the destination ports for a given tap */
  std::string tap_to_port(const ClockTapId& tap_id) const;
  /* Find the type of tap point:
   * all -> all coordinates in efpga are required to tap
   * single -> only 1 coordinate is required to tap
   * region -> coordinates in a region required to tap. Steps in region may be
   * required
   */
  e_tap_type tap_type(const ClockTapId& tap_id) const;
  /* Require the type of single */
  size_t tap_x(const ClockTapId& tap_id) const;
  size_t tap_y(const ClockTapId& tap_id) const;
  /* Require the type of region */
  vtr::Rect<size_t> tap_bounding_box(const ClockTapId& tap_id) const;
  /* Steps are only available when type is region */
  size_t tap_step_x(const ClockTapId& tap_id) const;
  size_t tap_step_y(const ClockTapId& tap_id) const;
  /* Return the list of flatten tap pins. For example: clb[0:1].clk[2:2] is
   * flatten to { clb[0].clk[2], clb[1].clk[2] } Useful to build clock routing
   * resource graph Note that the clk_pin_id limits only 1 clock to be accessed
   */
  std::vector<std::string> tree_flatten_tap_to_ports(
    const ClockTreeId& tree_id, const ClockTreePinId& clk_pin_id,
    const vtr::Point<size_t>& tap_coord) const;
  /* Find a spine with a given name, if not found, return an valid id, otherwise
   * return an invalid one */
  ClockSpineId find_spine(const std::string& name) const;
  /* Find a tree with a given name, if not found, return an valid id, otherwise
   * return an invalid one */
  ClockTreeId find_tree(const std::string& name) const;
  /* Check if there are clock tree */
  bool empty() const;
  bool is_valid() const;
  /* Get the level id which is next to the current level
   * Note that this follows the same rule in computing levels in
   * update_tree_depth() If the rule has been changed, this API should be
   * changed as well
   */
  ClockLevelId next_level(const ClockLevelId& lvl) const;

 public: /* Public Mutators */
  /* Reserve a number of spines to be memory efficent */
  void reserve_spines(const size_t& num_spines);
  /* Reserve a number of trees to be memory efficent */
  void reserve_trees(const size_t& num_trees);
  void set_default_segment(const RRSegmentId& seg_id);
  void set_default_tap_switch(const RRSwitchId& switch_id);
  void set_default_driver_switch(const RRSwitchId& switch_id);
  void set_default_segment_name(const std::string& name);
  void set_default_tap_switch_name(const std::string& name);
  void set_default_driver_switch_name(const std::string& name);
  /* Create a new tree, by default the tree can accomodate only 1 clock signal;
   * use width to adjust the size */
  ClockTreeId create_tree(const std::string& name,
                          const BasicPort& global_port);
  /* Create a new spine, if the spine is already created, return an invalid id
   */
  ClockSpineId create_spine(const std::string& name);
  /* Try to create a new spine, if the spine is already existing, return the id.
   * If not, create a new spine and return its id */
  ClockSpineId try_create_spine(const std::string& name);
  /* Set the parent tree for a given spine. It is illegal that a spine which
   * does not belong to any tree */
  void set_spine_parent_tree(const ClockSpineId& spine_id,
                             const ClockTreeId& tree_id);
  void set_spine_start_point(const ClockSpineId& spine_id,
                             const vtr::Point<int>& coord);
  void set_spine_end_point(const ClockSpineId& spine_id,
                           const vtr::Point<int>& coord);
  void set_spine_direction(const ClockSpineId& spine_id, const Direction& dir);
  void set_spine_track_type(const ClockSpineId& spine_id,
                            const e_rr_type& type);
  ClockSwitchPointId add_spine_switch_point(const ClockSpineId& spine_id,
                                            const ClockSpineId& drive_spine_id,
                                            const vtr::Point<int>& coord);
  ClockInternalDriverId add_spine_switch_point_internal_driver(
    const ClockSpineId& spine_id, const ClockSwitchPointId& switch_point_id,
    const std::string& internal_driver_from_port,
    const std::string& internal_driver_to_port);
  ClockInternalDriverId add_spine_intermediate_driver(
    const ClockSpineId& spine_id, const vtr::Point<int>& coord,
    const std::string& internal_driver_from_port,
    const std::string& internal_driver_to_port);
  ClockTapId add_tree_tap(const ClockTreeId& tree_id,
                          const BasicPort& from_port,
                          const std::string& to_port);
  bool set_tap_bounding_box(const ClockTapId& tap_id,
                            const vtr::Rect<size_t>& bb);
  bool set_tap_step_x(const ClockTapId& tap_id, const size_t& step);
  bool set_tap_step_y(const ClockTapId& tap_id, const size_t& step);
  /* Build internal links between clock tree, spines etc. This is also an
   * validator to verify the correctness of the clock network. Must run before
   * using the data! */
  bool link();

 public: /* Public invalidators/validators */
  /* Show if the tree id is a valid for data queries */
  bool valid_tree_id(const ClockTreeId& tree_id) const;
  /* Show if the level id is a valid for a given tree */
  bool valid_level_id(const ClockTreeId& tree_id,
                      const ClockLevelId& lvl_id) const;
  /* Identify if the level is the last level of the given tree */
  bool is_last_level(const ClockTreeId& tree_id, const ClockLevelId& lvl) const;
  /* Identify if the spine is at the last level of its tree */
  bool is_last_level(const ClockSpineId& spine_id) const;
  /* Show if the tree id is a valid for data queries */
  bool valid_spine_id(const ClockSpineId& spine_id) const;
  bool valid_spine_switch_point_id(
    const ClockSpineId& spine_id,
    const ClockSwitchPointId& switch_point_id) const;
  /* Valid starting and ending point should indicate either this is a
   * X-direction spine or a Y-direction spine. Diagonal spine is not supported!
   */
  bool valid_spine_start_end_points(const ClockSpineId& spine_id) const;
  /* Definition of a vague coordinate is that start_x == end_x && start_y ==
   * end_y In such situation, we need specific track type and direction to be
   * provided by user
   */
  bool is_vague_coordinate(const ClockSpineId& spine_id) const;
  /* Validate the internal data. Required to ensure clean data before usage. If
   * validation is successful, is_valid() will return true */
  bool validate() const;

 private: /* Public invalidators/validators */
  /* Ensure tree data is clean. All the spines are valid, and switch points are
   * valid */
  bool validate_tree_taps() const;
  bool validate_tree() const;
  /* Show if the internal driver id is a valid for data queries */
  bool valid_internal_driver_id(
    const ClockInternalDriverId& int_driver_id) const;
  /* Show if the tap id is a valid for data queries */
  bool valid_tap_id(const ClockTapId& tap_id) const;
  /* Check if a given coordinate matches the requirements for a tap point */
  bool valid_tap_coord_in_bb(const ClockTapId& tap_id,
                             const vtr::Point<size_t>& tap_coord) const;

 private: /* Private mutators */
  /* Build internal links between spines under a given tree */
  bool link_tree(const ClockTreeId& tree_id);
  bool link_tree_top_spines(const ClockTreeId& tree_id);
  /* Require link_tree_top_spines() to called before! */
  bool sort_tree_spines(const ClockTreeId& tree_id);
  bool rec_update_spine_level(const ClockSpineId& spine_id);
  /* Require sort_tree_spines() to called before! */
  bool update_tree_depth(const ClockTreeId& tree_id);
  /* Infer track type and directions for each spine by their coordinates */
  bool update_spine_attributes(const ClockTreeId& tree_id);

 private: /* Internal data */
  /* Basic information of each tree */
  vtr::vector<ClockTreeId, ClockTreeId> tree_ids_;
  vtr::vector<ClockTreeId, std::string> tree_names_;
  vtr::vector<ClockTreeId, BasicPort> tree_global_ports_;
  vtr::vector<ClockTreeId, size_t> tree_depths_;
  vtr::vector<ClockTreeId, std::vector<ClockSpineId>> tree_top_spines_;
  vtr::vector<ClockTreeId, std::vector<ClockTapId>> tree_taps_;

  /* Basic information of each spine */
  vtr::vector<ClockSpineId, ClockSpineId> spine_ids_;
  vtr::vector<ClockSpineId, std::string> spine_names_;
  vtr::vector<ClockSpineId, size_t> spine_levels_;
  vtr::vector<ClockSpineId, vtr::Point<int>> spine_start_points_;
  vtr::vector<ClockSpineId, vtr::Point<int>> spine_end_points_;
  vtr::vector<ClockSpineId, Direction> spine_directions_;
  vtr::vector<ClockSpineId, e_rr_type> spine_track_types_;
  vtr::vector<ClockSpineId, std::vector<ClockSpineId>> spine_switch_points_;
  vtr::vector<ClockSpineId, std::vector<vtr::Point<int>>> spine_switch_coords_;
  vtr::vector<ClockSpineId, std::vector<std::vector<ClockInternalDriverId>>>
    spine_switch_internal_drivers_;
  vtr::vector<ClockSpineId,
              std::map<std::string, std::vector<ClockInternalDriverId>>>
    spine_intermediate_drivers_;
  vtr::vector<ClockSpineId, ClockSpineId> spine_parents_;
  vtr::vector<ClockSpineId, std::vector<ClockSpineId>> spine_children_;
  vtr::vector<ClockSpineId, ClockTreeId> spine_parent_trees_;

  /* Basic Information about internal drivers */
  vtr::vector<ClockInternalDriverId, ClockInternalDriverId>
    internal_driver_ids_;
  vtr::vector<ClockInternalDriverId, std::string> internal_driver_from_pins_;
  vtr::vector<ClockInternalDriverId, BasicPort> internal_driver_to_pins_;
  /* Basic information about tap */
  vtr::vector<ClockTapId, ClockTapId> tap_ids_;
  vtr::vector<ClockTapId, BasicPort> tap_from_ports_;
  vtr::vector<ClockTapId, std::string> tap_to_ports_;
  vtr::vector<ClockTapId, vtr::Rect<size_t>>
    tap_bbs_; /* Bounding box for tap points, (xlow, ylow) -> (xhigh, yhigh) */
  vtr::vector<ClockTapId, vtr::Point<size_t>>
    tap_bb_steps_; /* x() -> x-direction step, y() -> y-direction step */

  /* Default routing resource */
  std::string default_segment_name_; /* The routing segment representing the
                                        clock wires */
  RRSegmentId default_segment_id_;
  std::string default_tap_switch_name_; /* The routing switch interconnecting
                                           clock wire */
  RRSwitchId default_tap_switch_id_;
  std::string default_driver_switch_name_; /* The routing switch interconnecting
                                              clock wire */
  RRSwitchId default_driver_switch_id_;

  /* Fast lookup */
  std::map<std::string, ClockTreeId> tree_name2id_map_;
  std::map<std::string, ClockSpineId> spine_name2id_map_;

  /* Constants */
  vtr::Rect<size_t> empty_tap_bb_;

  /* Flags */
  mutable bool is_dirty_;
};

}  // End of namespace openfpga
#endif
