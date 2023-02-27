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

 public: /* Constructors */
  ClockNetwork();

 public: /* Accessors: aggregates */
  size_t num_trees() const;
  clock_tree_range trees() const;
  /* Return the range of clock levels */
  std::vector<ClockLevelId> levels(const ClockTreeId& tree_id) const;
  /* Return a list of spine id under a clock tree */
  std::vector<ClockSpineId> spines(const ClockTreeId& tree_id) const;
  /* Return a list of clock pins in a bus of clock tree at a given level and
   * direction */
  std::vector<ClockTreePinId> pins(const ClockTreeId& tree_id,
                                   const ClockLevelId& level,
                                   const t_rr_type& track_type,
                                   const Direction& direction) const;

 public: /* Public Accessors: Basic data query */
  /* Return the number of routing tracks required by a selected clock tree at a
   * given level and direction */
  size_t num_tracks(const ClockTreeId& tree_id, const ClockLevelId& level,
                    const t_rr_type& track_type) const;
  size_t num_tracks(const ClockTreeId& tree_id, const ClockLevelId& level,
                    const t_rr_type& track_type,
                    const Direction& direction) const;
  /* Return the id of default routing segment, use this to find detailed segment
   * information from RRGraph */
  RRSegmentId default_segment() const;
  std::string default_segment_name() const;
  RRSwitchId default_switch() const;
  std::string default_switch_name() const;
  std::string tree_name(const ClockTreeId& tree_id) const;
  size_t tree_width(const ClockTreeId& tree_id) const;
  size_t tree_depth(const ClockTreeId& tree_id) const;
  std::string spine_name(const ClockSpineId& spine_id) const;
  vtr::Point<int> spine_start_point(const ClockSpineId& spine_id) const;
  vtr::Point<int> spine_end_point(const ClockSpineId& spine_id) const;
  /* Identify the direction of a spine, depending on its starting and ending
   * points
   * - CHANX represents a horizental routing track
   * - CHANY represents a vertical routing track
   */
  t_rr_type spine_track_type(const ClockSpineId& spine_id) const;
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
  /* Find a spine with a given name, if not found, return an valid id, otherwise
   * return an invalid one */
  ClockSpineId find_spine(const std::string& name) const;
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
  void set_default_switch(const RRSwitchId& switch_id);
  void set_default_segment_name(const std::string& name);
  void set_default_switch_name(const std::string& name);
  /* Create a new tree, by default the tree can accomodate only 1 clock signal;
   * use width to adjust the size */
  ClockTreeId create_tree(const std::string& name, size_t width = 1);
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
  void add_spine_switch_point(const ClockSpineId& spine_id,
                              const ClockSpineId& drive_spine_id,
                              const vtr::Point<int>& coord);
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
  /* Show if the tree id is a valid for data queries */
  bool valid_spine_id(const ClockSpineId& spine_id) const;
  bool valid_spine_switch_point_id(
    const ClockSpineId& spine_id,
    const ClockSwitchPointId& switch_point_id) const;
  /* Valid starting and ending point should indicate either this is a
   * X-direction spine or a Y-direction spine. Diagonal spine is not supported!
   */
  bool valid_spine_start_end_points(const ClockSpineId& spine_id) const;
  /* Validate the internal data. Required to ensure clean data before usage. If
   * validation is successful, is_valid() will return true */
  bool validate() const;

 private: /* Public invalidators/validators */
  /* Ensure tree data is clean. All the spines are valid, and switch points are
   * valid */
  bool validate_tree() const;

 private: /* Private mutators */
  /* Build internal links between spines under a given tree */
  bool link_tree(const ClockTreeId& tree_id);
  bool link_tree_top_spines(const ClockTreeId& tree_id);
  /* Require link_tree_top_spines() to called before! */
  bool sort_tree_spines(const ClockTreeId& tree_id);
  bool rec_update_spine_level(const ClockSpineId& spine_id);
  /* Require sort_tree_spines() to called before! */
  bool update_tree_depth(const ClockTreeId& tree_id);

 private: /* Internal data */
  /* Basic information of each tree */
  vtr::vector<ClockTreeId, ClockTreeId> tree_ids_;
  vtr::vector<ClockTreeId, std::string> tree_names_;
  vtr::vector<ClockTreeId, size_t> tree_widths_;
  vtr::vector<ClockTreeId, size_t> tree_depths_;
  vtr::vector<ClockTreeId, std::vector<ClockSpineId>> tree_top_spines_;

  /* Basic information of each spine */
  vtr::vector<ClockSpineId, ClockSpineId> spine_ids_;
  vtr::vector<ClockSpineId, std::string> spine_names_;
  vtr::vector<ClockSpineId, size_t> spine_levels_;
  vtr::vector<ClockSpineId, vtr::Point<int>> spine_start_points_;
  vtr::vector<ClockSpineId, vtr::Point<int>> spine_end_points_;
  vtr::vector<ClockSpineId, std::vector<ClockSpineId>> spine_switch_points_;
  vtr::vector<ClockSpineId, std::vector<vtr::Point<int>>> spine_switch_coords_;
  vtr::vector<ClockSpineId, ClockSpineId> spine_parents_;
  vtr::vector<ClockSpineId, std::vector<ClockSpineId>> spine_children_;
  vtr::vector<ClockSpineId, ClockTreeId> spine_parent_trees_;

  /* Default routing resource */
  std::string default_segment_name_; /* The routing segment representing the
                                        clock wires */
  RRSegmentId default_segment_id_;
  std::string
    default_switch_name_; /* The routing switch interconnecting clock wire */
  RRSwitchId default_switch_id_;

  /* Fast lookup */
  std::map<std::string, ClockTreeId> tree_name2id_map_;
  std::map<std::string, ClockSpineId> spine_name2id_map_;

  /* Flags */
  mutable bool is_dirty_;
};

}  // End of namespace openfpga
#endif
