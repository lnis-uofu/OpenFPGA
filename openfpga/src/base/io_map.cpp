/******************************************************************************
 * Memember functions for data structure IoMap
 ******************************************************************************/
#include "vtr_assert.h"

#include "io_map.h"

/* begin namespace openfpga */
namespace openfpga {

IoMap::io_map_range IoMap::io_map() const {
  return vtr::make_range(io_map_ids_.begin(), io_map_ids_.end());
}

BasicPort IoMap::io_port(IoMapId io_map_id) const {
  VTR_ASSERT(valid_io_map_id(io_map_id));
  return io_ports_[io_map_id];
}

BasicPort IoMap::io_net(IoMapId io_map_id) const {
  VTR_ASSERT(valid_io_map_id(io_map_id));
  return mapped_nets_[io_map_id];
}

bool IoMap::is_io_output(IoMapId io_map_id) const {
  VTR_ASSERT(valid_io_map_id(io_map_id));
  return IoMap::IO_MAP_DIR_OUTPUT == io_directionality_[io_map_id];
}

bool IoMap::is_io_input(IoMapId io_map_id) const {
  VTR_ASSERT(valid_io_map_id(io_map_id));
  return IoMap::IO_MAP_DIR_INPUT == io_directionality_[io_map_id];
}

IoMapId IoMap::create_io_mapping(const BasicPort& port,
                                 const BasicPort& net,
                                 IoMap::e_direction dir) {
  /* Create a new id */
  IoMapId io_map_id = IoMapId(io_map_ids_.size());
  io_map_ids_.push_back(io_map_id);

  /* Allocate related attributes */
  io_ports_.push_back(port);
  mapped_nets_.push_back(net);
  io_directionality_.push_back(dir);

  return io_map_id;
} 

bool IoMap::valid_io_map_id(IoMapId io_map_id) const {
  return (size_t(io_map_id) < io_map_ids_.size()) && (io_map_id == io_map_ids_[io_map_id]);
}

} /* end namespace openfpga */
