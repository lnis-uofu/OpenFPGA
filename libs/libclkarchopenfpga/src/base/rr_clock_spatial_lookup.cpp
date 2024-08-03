#include "rr_clock_spatial_lookup.h"

#include "vtr_assert.h"
#include "vtr_log.h"

namespace openfpga {  // begin namespace openfpga

RRClockSpatialLookup::RRClockSpatialLookup() {}

RRNodeId RRClockSpatialLookup::find_node(int x, int y, const ClockTreeId& tree,
                                         const ClockLevelId& lvl,
                                         const ClockTreePinId& pin,
                                         const Direction& direction,
                                         const bool& verbose) const {
  size_t dir = size_t(direction);
  /* Pre-check: the x, y, side and ptc should be non negative numbers!
   * Otherwise, return an invalid id */
  if ((x < 0) || (y < 0) ||
      (direction != Direction::INC && direction != Direction::DEC)) {
    return RRNodeId::INVALID();
  }

  /* Sanity check to ensure the x, y, side and ptc are in range
   * - Return an valid id by searching in look-up when all the parameters are in
   * range
   * - Return an invalid id if any out-of-range is detected
   */
  if (size_t(dir) >= rr_node_indices_.size()) {
    VTR_LOGV(verbose, "Direction out of range\n");
    return RRNodeId::INVALID();
  }

  if (size_t(x) >= rr_node_indices_[dir].dim_size(0)) {
    VTR_LOGV(verbose, "X out of range\n");
    return RRNodeId::INVALID();
  }

  if (size_t(y) >= rr_node_indices_[dir].dim_size(1)) {
    VTR_LOG("Y out of range\n");
    return RRNodeId::INVALID();
  }

  if (size_t(tree) >= rr_node_indices_[dir][x][y].size()) {
    VTR_LOGV(verbose, "Tree id out of range\n");
    return RRNodeId::INVALID();
  }

  if (size_t(lvl) == rr_node_indices_[dir][x][y][size_t(tree)].size()) {
    VTR_LOGV(verbose, "Level id out of range\n");
    return RRNodeId::INVALID();
  }

  if (size_t(pin) ==
      rr_node_indices_[dir][x][y][size_t(tree)][size_t(lvl)].size()) {
    VTR_LOGV(verbose, "Pin id out of range\n");
    return RRNodeId::INVALID();
  }

  return rr_node_indices_[dir][x][y][size_t(tree)][size_t(lvl)][size_t(pin)];
}

void RRClockSpatialLookup::add_node(RRNodeId node, int x, int y,
                                    const ClockTreeId& tree,
                                    const ClockLevelId& lvl,
                                    const ClockTreePinId& pin,
                                    const Direction& direction) {
  size_t dir = size_t(direction);
  VTR_ASSERT(node); /* Must have a valid node id to be added */
  VTR_ASSERT_SAFE(2 == rr_node_indices_[dir].ndims());

  resize_nodes(x, y, direction);

  if (size_t(tree) >= rr_node_indices_[dir][x][y].size()) {
    rr_node_indices_[dir][x][y].resize(size_t(tree) + 1);
  }

  if (size_t(lvl) >= rr_node_indices_[dir][x][y][size_t(tree)].size()) {
    rr_node_indices_[dir][x][y][size_t(tree)].resize(size_t(lvl) + 1);
  }

  if (size_t(pin) >=
      rr_node_indices_[dir][x][y][size_t(tree)][size_t(lvl)].size()) {
    rr_node_indices_[dir][x][y][size_t(tree)][size_t(lvl)].resize(size_t(pin) +
                                                                  1);
  }

  /* Resize on demand finished; Register the node */
  rr_node_indices_[dir][x][y][size_t(tree)][size_t(lvl)][size_t(pin)] = node;
}

void RRClockSpatialLookup::reserve_nodes(int x, int y, int tree, int lvl,
                                         int pin) {
  for (Direction dir : {Direction::INC, Direction::DEC}) {
    resize_nodes(x, y, dir);
    for (int ix = 0; ix < x; ++ix) {
      for (int iy = 0; iy < y; ++iy) {
        rr_node_indices_[size_t(dir)][ix][iy].resize(tree);
        for (int itree = 0; itree < tree; ++itree) {
          rr_node_indices_[size_t(dir)][ix][iy][itree].resize(lvl);
          for (int ilvl = 0; ilvl < lvl; ++ilvl) {
            rr_node_indices_[size_t(dir)][ix][iy][itree][ilvl].resize(pin);
          }
        }
      }
    }
  }
}

void RRClockSpatialLookup::resize_nodes(int x, int y,
                                        const Direction& direction) {
  /* Expand the fast look-up if the new node is out-of-range
   * This may seldom happen because the rr_graph building function
   * should ensure the fast look-up well organized
   */
  size_t dir = size_t(direction);
  VTR_ASSERT(dir < rr_node_indices_.size());
  VTR_ASSERT(x >= 0);
  VTR_ASSERT(y >= 0);

  if ((x >= int(rr_node_indices_[dir].dim_size(0))) ||
      (y >= int(rr_node_indices_[dir].dim_size(1)))) {
    rr_node_indices_[dir].resize(
      {std::max(rr_node_indices_[dir].dim_size(0), size_t(x) + 1),
       std::max(rr_node_indices_[dir].dim_size(1), size_t(y) + 1)});
  }
}

void RRClockSpatialLookup::clear() {
  for (auto& data : rr_node_indices_) {
    data.clear();
  }
}

}  // end namespace openfpga
