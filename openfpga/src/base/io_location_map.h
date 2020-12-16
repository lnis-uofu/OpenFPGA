#ifndef IO_LOCATION_MAP_H
#define IO_LOCATION_MAP_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <stddef.h>
#include <vector>
#include <string>
#include <map>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * I/O location map is a data structure to bridge the index of I/Os
 * in the FPGA fabric, i.e., the module graph, and logical location
 * of the I/O in VPR coordinate system
 *
 * For example: 
 *
 *      ioA[0]   ioA[1]      ioB[0]    ioB[1]     ioA[2]
 *   +-----------------+   +--------+--------+  +--------+
 *   |        |        |   |        |        |  |        |
 *   |  I/O   |  I/O   |   |  I/O   |  I/O   |  |  I/O   |
 *   | [0][y] | [0][y] |   | [1][y] | [1][y] |  | [2][y] | ...
 *   |  [0]   |  [1]   |   |  [0]   |  [1]   |  |  [0]   |
 *   +-----------------+   +--------+--------+  +--------+
 *
 *******************************************************************/
class IoLocationMap {
  public: /* Public aggregators */
    size_t io_index(const size_t& x,
                    const size_t& y,
                    const size_t& z,
                    const std::string& io_port_name) const;
  public: /* Public mutators */
    void set_io_index(const size_t& x,
                      const size_t& y,
                      const size_t& z,
                      const std::string& io_port_name,
                      const size_t& io_index);
  private: /* Internal Data */
    /* I/O index fast lookup by [x][y][z] location */
    std::vector<std::vector<std::vector<std::map<std::string, size_t>>>> io_indices_;
};

} /* End namespace openfpga*/

#endif
