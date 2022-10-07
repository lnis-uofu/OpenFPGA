#ifndef IO_NET_PLACE_H
#define IO_NET_PLACE_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <stddef.h>

#include <array>
#include <map>
#include <string>
#include <vector>

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * I/O net place is a data structure to store the coordinate of each nets
 * defined in users' HDL designs on FPGA fabric.
 *
 * For example:
 *       netA                          netB
 *        |                              |
 *        v                              v
 *      ioA[0]   ioA[1]      ioB[0]    ioB[1]     ioA[2]
 *   +-----------------+   +--------+--------+  +--------+
 *   |        |        |   |        |        |  |        |
 *   |  I/O   |  I/O   |   |  I/O   |  I/O   |  |  I/O   |
 *   | [0][y] | [0][y] |   | [1][y] | [1][y] |  | [2][y] | ...
 *   |  [0]   |  [1]   |   |  [0]   |  [1]   |  |  [0]   |
 *   +-----------------+   +--------+--------+  +--------+
 *
 *******************************************************************/
class IoNetPlace {
 public: /* Public aggregators */
  size_t io_x(const std::string& net) const;
  size_t io_y(const std::string& net) const;
  size_t io_z(const std::string& net) const;

 public: /* Writers */
  int write_to_place_file(const std::string& fname,
                          const bool& include_time_stamp,
                          const bool& verbose) const;

 public: /* Public mutators */
  void set_net_coord(const std::string& net, const size_t& x, const size_t& y,
                     const size_t& z);

 private: /* Internal Data */
  /* I/O coordinate fast lookup by net name */
  std::map<std::string, std::array<size_t, 3>> io_coords_;
};

} /* End namespace openfpga*/

#endif
