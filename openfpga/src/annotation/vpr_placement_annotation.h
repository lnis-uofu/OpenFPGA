#ifndef VPR_PLACEMENT_ANNOTATION_H
#define VPR_PLACEMENT_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <map> 

/* Header from vtrutil library */
#include "vtr_geometry.h"

/* Header from vpr library */
#include "device_grid.h"
#include "clustered_netlist.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This is the critical data structure to annotate placement results
 * in VPR context
 *******************************************************************/
class VprPlacementAnnotation {
  public:  /* Public accessors */
    std::vector<ClusterBlockId> grid_blocks(const vtr::Point<size_t>& grid_coord) const;
  public:  /* Public mutators */
    void init_mapped_blocks(const DeviceGrid& grids);
    void add_mapped_block(const vtr::Point<size_t>& grid_coord,
                          const size_t& z, const ClusterBlockId& mapped_block);
  private: /* Internal data */
    /* A direct mapping show each mapped/unmapped blocks in grids
     * The blocks_ array represents each grid on the FPGA fabric
     * For example, block_[x][y] showed the mapped/unmapped blocks 
     * at grid[x][y]. The third coordinate 'z' is the index of the same
     * type of blocks in the grids. This is mainly applied to I/O
     * blocks where you may have >1 I/O in a grid
     *
     * Note that this is different from the grid blocks in PlacementContext
     * VPR considers only mapped blocks while this annotation 
     * considers both unmapped and mapped blocks
     * Unmapped blocks will be labelled as an invalid id in the vector
     */
    vtr::Matrix<std::vector<ClusterBlockId>> blocks_;
};

} /* End namespace openfpga*/

#endif
