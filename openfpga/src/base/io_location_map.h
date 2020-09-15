#ifndef IO_LOCATION_MAP_H
#define IO_LOCATION_MAP_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <stddef.h>
#include <vector>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * I/O location map is a data structure to bridge the index of I/Os
 * in the FPGA fabric, i.e., the module graph, and logical location
 * of the I/O in VPR coordinate system
 *
 * For example 
 *              io[0]   io[1]          io[2]
 *           +-----------------+    +--------+
 *           |        |        |    |        |
 *           |  I/O   |  I/O   |    |  I/O   |
 *           | [0][y] | [0][y] |    | [1][y] |
 *           |  [0]   |  [1]   |    |  [0]   |
 *           +-----------------+    +--------+
 *
 *******************************************************************/
class IoLocationMap {
  public: /* Public aggregators */
    size_t io_index(const size_t& x, const size_t& y, const size_t& z) const;
  public: /* Public mutators */
    void set_io_index(const size_t& x, const size_t& y, const size_t& z, const size_t& io_index);
  private: /* Internal Data */
    /* I/O index fast lookup by [x][y][z] location */
    std::vector<std::vector<std::vector<size_t>>> io_indices_;
};

} /* End namespace openfpga*/

#endif
