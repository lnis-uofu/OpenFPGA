/******************************************************************************
 * Memember functions for data structure IoLocationMap
 ******************************************************************************/
#include "vtr_assert.h"

#include "io_location_map.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors 
 *************************************************/
size_t IoLocationMap::io_index(const size_t& x, const size_t& y, const size_t& z) const {
  if (x >= io_indices_.size()) {
    return size_t(-1);
  }

  if (y >= io_indices_[x].size()) {
    return size_t(-1);
  }

  if (z >= io_indices_[x][y].size()) {
    return size_t(-1);
  }

  return io_indices_[x][y][z];
}

void IoLocationMap::set_io_index(const size_t& x, const size_t& y, const size_t& z, const size_t& io_index) {
  if (x >= io_indices_.size()) {
    io_indices_.resize(x + 1);
  }

  if (y >= io_indices_[x].size()) {
    io_indices_[x].resize(y + 1);
  }

  if (z >= io_indices_[x][y].size()) {
    io_indices_[x][y].resize(z + 1);
  }

  io_indices_[x][y][z] = io_index;
}

} /* end namespace openfpga */
