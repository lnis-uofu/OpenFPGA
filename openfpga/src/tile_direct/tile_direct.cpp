/******************************************************************************
 * Memember functions for data structure TileDirect
 ******************************************************************************/
#include "vtr_assert.h"

#include "tile_direct.h"

/* begin namespace openfpga */
namespace openfpga {

/**************************************************
 * Public Accessors 
 *************************************************/
TileDirect::tile_direct_range TileDirect::directs() const {
  return vtr::make_range(direct_ids_.begin(), direct_ids_.end());
}

t_physical_tile_type_ptr TileDirect::from_tile(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return from_tiles_[direct_id];
}

vtr::Point<size_t> TileDirect::from_tile_coordinate(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return from_tile_coords_[direct_id];
}

size_t TileDirect::from_tile_pin(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return from_tile_pins_[direct_id];
}

e_side TileDirect::from_tile_side(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return from_tile_sides_[direct_id];
}

t_physical_tile_type_ptr TileDirect::to_tile(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return to_tiles_[direct_id];
}

vtr::Point<size_t> TileDirect::to_tile_coordinate(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return to_tile_coords_[direct_id];
}

size_t TileDirect::to_tile_pin(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return to_tile_pins_[direct_id];
}

e_side TileDirect::to_tile_side(const TileDirectId& direct_id) const {
  /* Validate the direct_id */
  VTR_ASSERT(valid_direct_id(direct_id));
  return to_tile_sides_[direct_id];
}

/******************************************************************************
 * Private Mutators
 ******************************************************************************/
TileDirectId TileDirect::add_direct(t_physical_tile_type_ptr from_tile,
                                    const vtr::Point<size_t>& from_tile_coord,
                                    const e_side& from_tile_side,
                                    const size_t& from_tile_pin,
                                    t_physical_tile_type_ptr to_tile,
                                    const vtr::Point<size_t>& to_tile_coord,
                                    const e_side& to_tile_side,
                                    const size_t& to_tile_pin) {
  /* Create an new id */
  TileDirectId direct = TileDirectId(direct_ids_.size());
  direct_ids_.push_back(direct);

  /* Allocate other attributes */
  from_tiles_.push_back(from_tile);
  from_tile_coords_.push_back(from_tile_coord);
  from_tile_sides_.push_back(from_tile_side);
  from_tile_pins_.push_back(from_tile_pin);

  to_tiles_.push_back(to_tile);
  to_tile_coords_.push_back(to_tile_coord);
  to_tile_sides_.push_back(to_tile_side);
  to_tile_pins_.push_back(to_tile_pin);

  return direct;
}

/******************************************************************************
 * Private validators/invalidators
 ******************************************************************************/
bool TileDirect::valid_direct_id(const TileDirectId& direct_id) const {
  return ( size_t(direct_id) < direct_ids_.size() ) && ( direct_id == direct_ids_[direct_id] ); 
}

} /* end namespace openfpga */
