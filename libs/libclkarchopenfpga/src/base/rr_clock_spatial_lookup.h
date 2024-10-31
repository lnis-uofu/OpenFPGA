#ifndef RR_CLOCK_SPATIAL_LOOKUP_H
#define RR_CLOCK_SPATIAL_LOOKUP_H

/**
 * @file
 * @brief This RRClockSpatialLookup class encapsulates
 *        the node-lookup for clock nodes in a routing resource graph
 *
 * A data structure built to find the id of an routing resource node
 * (rr_node) given information about its physical position and type in a clock
 * network The data structure is mostly needed during building the clock part of
 * a routing resource graph
 *
 * The data structure allows users to
 *
 *   - Update the look-up with new nodes
 *   - Find the id of a node with given information, e.g., x, y, type etc.
 */
#include "clock_network_fwd.h"
#include "physical_types.h"
#include "rr_graph_fwd.h"
#include "rr_node_types.h"
#include "vtr_geometry.h"
#include "vtr_vector.h"

namespace openfpga {  // begin namespace openfpga

class RRClockSpatialLookup {
  /* -- Constructors -- */
 public:
  /* Explicitly define the only way to create an object */
  explicit RRClockSpatialLookup();

  /* Disable copy constructors and copy assignment operator
   * This is to avoid accidental copy because it could be an expensive operation
   * considering that the memory footprint of the data structure could ~ Gb
   * Using the following syntax, we prohibit accidental 'pass-by-value' which
   * can be immediately caught by compiler
   */
  RRClockSpatialLookup(const RRClockSpatialLookup&) = delete;
  void operator=(const RRClockSpatialLookup&) = delete;

  /* -- Accessors -- */
 public:
  /**
   * @brief Returns the index of the specified routing resource node.
   *
   *   @param (x, y) are the grid location within the FPGA
   *   @param clk_tree specifies the id of the clock tree in a clock network,
   *   @param clk_level specifies the level of the clock node in a clock network
   * (typically multi-level),
   *   @param clk_pin specifies the pin id of the clock node in a bus of clock
   * tree (consider multiple clock in a tree)
   *   @param direction specifies how the clock node will propagate the signal
   * (either in a horizental or a vertical way)
   *
   * @note An invalid id will be returned if the node does not exist
   */
  RRNodeId find_node(int x, int y, const ClockTreeId& tree,
                     const ClockLevelId& lvl, const ClockTreePinId& pin,
                     const Direction& direction, const bool& verbose) const;

  /* -- Mutators -- */
 public:
  /**
   * @brief Register a node in the fast look-up
   *
   * @note You must have a valid node id to register the node in the lookup
   *
   *   @param (x, y) are the grid location within the FPGA
   *   @param clk_tree specifies the id of the clock tree in a clock network,
   *   @param clk_level specifies the level of the clock node in a clock network
   (typically multi-level),
   *   @param clk_pin specifies the pin id of the clock node in a bus of clock
   tree (consider multiple clock in a tree)
   *   @param direction specifies how the clock node will propagate the signal
   (either in a horizental or a vertical way)

   *
   * @note a node added with this call will not create a node in the rr_graph
   node list
   * You MUST add the node in the rr_graph so that the node is valid
   */
  void add_node(RRNodeId node, int x, int y, const ClockTreeId& clk_tree,
                const ClockLevelId& clk_lvl, const ClockTreePinId& clk_pin,
                const Direction& direction);

  /**
   * @brief Allocate memory for the lookup with maximum sizes on each dimension
   * .. note:: Must run before any other API!
   */
  void reserve_nodes(int x, int y, int tree, int lvl, int pin);

  /** @brief Clear all the data inside */
  void clear();

 private: /* Private mutators */
  /** @brief Resize the nodes upon needs */
  void resize_nodes(int x, int y, const Direction& direction);

  /* -- Internal data storage -- */
 private:
  /* Fast look-up:
   * [INC|DEC][0..grid_width][0..grid_height][tree_id][level_id][clock_pin_id]
   */
  std::array<vtr::NdMatrix<std::vector<std::vector<std::vector<RRNodeId>>>, 2>,
             2>
    rr_node_indices_;
};

}  // end namespace openfpga

#endif
