#ifndef CLOCK_NETWORK_H
#define CLOCK_NETWORK_H

/********************************************************************
 * This file include the declaration of pin constraints
 *******************************************************************/
#include <array>
#include <map>
#include <string>

/* Headers from vtrutil library */
#include "vtr_vector.h"

/* Headers from openfpgautil library */
#include "clock_network_fwd.h"
#include "openfpga_port.h"

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
  clock_tree_range trees() const;

 public: /* Public Accessors: Basic data query */
  /* Find a spine by name, return a valid id if found, otherwise return an invalid id */
  ClockSpineId spine(const std::string& name) const;

  /* Check if there are clock tree */
  bool empty() const;

 public: /* Public Mutators */
  /* Reserve a number of spines to be memory efficent */
  void reserve_spines(const size_t& num_spines);

  /* Reserve a number of trees to be memory efficent */
  void reserve_trees(const size_t& num_trees);

  /* Create a new tree, by default the tree can accomodate only 1 clock signal; use width to adjust the size */
  ClockTreeId create_tree(const std::string& name, const size_t& width = 1);

  /* Create a new spine, if the spine is already created, return an invalid id */
  ClockSpineId create_spine(const std::string& name);

  /* Set the parent tree for a given spine. It is illegal that a spine which does not belong to any tree */
  void set_spine_parent_tree(const ClockSpineId& spine, const ClockTreeId& parent_tree);
  void set_spine_start_point(const ClockSpineId& spine, const vtr::Point<int>& point);
  void set_spine_end_point(const ClockSpineId& spine, const vtr::Point<int>& point);
  void add_spine_switch_point(const ClockSpineId& spine, const ClockSpineId& drive_spine, const vtr::Point<int>& coord);

 public: /* Public invalidators/validators */
  /* Show if the tree id is a valid for data queries */
  bool valid_tree_id(const ClockTreeId& tree_id) const;

  /* Show if the tree id is a valid for data queries */
  bool valid_spine_id(const ClockSpineId& spine_id) const;

 private: /* Internal data */
  /* Basic information of each tree */
  vtr::vector<ClockTreeId, ClockTreeId> clk_tree_ids_;
  vtr::vector<ClockTreeId, std::string> clk_tree_names_;
  vtr::vector<ClockTreeId, std::vector<ClockSpineId>> clk_tree_top_spines_;

  /* Basic information of each spine */
  vtr::vector<ClockSpineId, ClockSpineId> clk_spines_;
  vtr::vector<ClockSpineId, std::string> clk_spine_names_;
  vtr::vector<ClockSpineId, size_t> clk_spine_levels_;
  vtr::vector<ClockSpineId, vtr::Point<int>> clk_spine_start_points_;
  vtr::vector<ClockSpineId, vtr::Point<int>> clk_spine_end_points_;
  vtr::vector<ClockSpineId, std::vector<ClockSpineId>> clk_spine_switch_points_;
  vtr::vector<ClockSpineId, std::vector<vtr::Point<int>>> clk_spine_switch_coords_;
  vtr::vector<ClockSpineId, ClockSpineId> clk_spine_parent_;
  vtr::vector<ClockSpineId, ClockTreeId> clk_spine_parent_tree_;

  /* Fast lookup */
  std::map<std::string, ClockSpineId> spine_name2ids_;

  /* Flags */
  bool is_dirty_;
};

}  // End of namespace openfpga
#endif
