/************************************************************************
 * Member functions for class VprPlacementAnnotation
 ***********************************************************************/
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vpr_placement_annotation.h"

/* namespace openfpga begins */
namespace openfpga {

/************************************************************************
 * Constructors
 ***********************************************************************/

/************************************************************************
 * Public accessors
 ***********************************************************************/
std::vector<ClusterBlockId> VprPlacementAnnotation::grid_blocks(const vtr::Point<size_t>& grid_coord) const {
  return blocks_[grid_coord.x()][grid_coord.y()];
}

/************************************************************************
 * Public mutators
 ***********************************************************************/
void VprPlacementAnnotation::init_mapped_blocks(const DeviceGrid& grids) {
  /* Size the block array with grid sizes */
  blocks_.resize({grids.width(), grids.height()});

  /* Resize the number of blocks allowed per grid by the capacity of the type */
  for (size_t x = 0; x < grids.width(); ++x) {
    for (size_t y = 0; y < grids.height(); ++y) {
      /* Deposit invalid ids and we will fill later */
      blocks_[x][y].resize(grids[x][y].type->capacity, ClusterBlockId::INVALID());
    }
  }
}

void VprPlacementAnnotation::add_mapped_block(const vtr::Point<size_t>& grid_coord,
                                              const size_t& z,
                                              const ClusterBlockId& mapped_block) {
  VTR_ASSERT(z < grid_blocks(grid_coord).size());
  if (ClusterBlockId::INVALID() != blocks_[grid_coord.x()][grid_coord.y()][z]) {
    VTR_LOG("Override mapped blocks at grid[%lu][%lu][%lu]!\n",
            grid_coord.x(), grid_coord.y(), z);
  }
  blocks_[grid_coord.x()][grid_coord.y()][z] = mapped_block;
}

} /* End namespace openfpga*/
