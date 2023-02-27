#include "vtr_assert.h"
#include "vtr_log.h"
#include "rr_clock_spatial_lookup.h"

namespace openfpga { // begin namespace openfpga

RRClockSpatialLookup::RRClockSpatialLookup() {
}

RRNodeId RRClockSpatialLookup::find_node(int x,
                                    int y,
                                    const ClockTreeId& tree,
                                    const ClockLevelId& lvl,
                                    const ClockTreePinId& pin,
                                    const Direction& direction) const {
    size_t dir = size_t(direction);
    /* Pre-check: the x, y, side and ptc should be non negative numbers! Otherwise, return an invalid id */
    if ((x < 0) || (y < 0) || (direction != Direction::INC && direction != Direction::DEC)) {
        return RRNodeId::INVALID();
    }

    /* Sanity check to ensure the x, y, side and ptc are in range 
     * - Return an valid id by searching in look-up when all the parameters are in range
     * - Return an invalid id if any out-of-range is detected
     */
    if (size_t(dir) >= rr_node_indices_.size()) {
        return RRNodeId::INVALID();
    }

    if (size_t(x) >= rr_node_indices_[dir].dim_size(0)) {
        return RRNodeId::INVALID();
    }

    if (size_t(y) >= rr_node_indices_[dir].dim_size(1)) {
        return RRNodeId::INVALID();
    }

    auto result_tree = rr_node_indices_[dir][x][y].find(tree);
    if (result_tree == rr_node_indices_[dir][x][y].end()) {
        return RRNodeId::INVALID();
    }

    auto result_lvl = result_tree->second.find(lvl);
    if (result_lvl == result_tree->second.end()) {
        return RRNodeId::INVALID();
    }

    auto result_pin = result_lvl->second.find(pin);
    if (result_pin == result_lvl->second.end()) {
        return RRNodeId::INVALID();
    }

    return result_pin->second;
}


void RRClockSpatialLookup::add_node(RRNodeId node,
                                    int x,
                                    int y,
                                    const ClockTreeId& tree,
                                    const ClockLevelId& lvl,
                                    const ClockTreePinId& pin,
                                    const Direction& direction) {
    size_t dir = size_t(direction);
    VTR_ASSERT(node); /* Must have a valid node id to be added */
    VTR_ASSERT_SAFE(2 == rr_node_indices_[dir].ndims());

    resize_nodes(x, y, direction);

    /* Resize on demand finished; Register the node */
    rr_node_indices_[dir][x][y][tree][lvl][pin] = node;
}

void RRClockSpatialLookup::resize_nodes(int x,
                                        int y,
                                        const Direction& direction) {
    /* Expand the fast look-up if the new node is out-of-range
     * This may seldom happen because the rr_graph building function
     * should ensure the fast look-up well organized  
     */
    size_t dir = size_t(direction);
    VTR_ASSERT(dir < rr_node_indices_.size());
    VTR_ASSERT(x >= 0);
    VTR_ASSERT(y >= 0);

    if ((x >= int(rr_node_indices_[dir].dim_size(0)))
        || (y >= int(rr_node_indices_[dir].dim_size(1)))) {
        rr_node_indices_[dir].resize({std::max(rr_node_indices_[dir].dim_size(0), size_t(x) + 1),
                                       std::max(rr_node_indices_[dir].dim_size(1), size_t(y) + 1)});
    }
}

void RRClockSpatialLookup::clear() {
    for (auto& data : rr_node_indices_) {
        data.clear();
    }
}

} // end namespace openfpga
