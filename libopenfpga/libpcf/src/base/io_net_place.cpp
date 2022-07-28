/******************************************************************************
 * Memember functions for data structure IoNetPlace
 ******************************************************************************/
/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

#include "io_net_place.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors 
 *************************************************/
size_t IoNetPlace::io_x(const std::string& net) const {
  auto result = io_coords_.find(net);
  if (result == io_coords_.end()) {
    return size_t(-1);
  }

  return result->second[0];
}

size_t IoNetPlace::io_y(const std::string& net) const {
  auto result = io_coords_.find(net);
  if (result == io_coords_.end()) {
    return size_t(-1);
  }

  return result->second[1];
}

size_t IoNetPlace::io_z(const std::string& net) const {
  auto result = io_coords_.find(net);
  if (result == io_coords_.end()) {
    return size_t(-1);
  }

  return result->second[2];
}

void IoNetPlace::set_net_coord(const std::string& net,
                               const size_t& x,
                               const size_t& y,
                               const size_t& z) {
  /* Warn when there is an attempt to overwrite */ 
  auto result = io_coords_.find(net);
  if (result != io_coords_.end()) {
    VTR_LOG_WARN("Overwrite net '%s' coordinate from (%lu, %lu, %lu) to (%lu, %lu, %lu)!\n",
                 net.c_str(),
                 result->second[0], result->second[1], result->second[2],
                 x, y, z);
  }
  io_coords_[net][0] = x;
  io_coords_[net][1] = y;
  io_coords_[net][2] = z;
}

} /* end namespace openfpga */
