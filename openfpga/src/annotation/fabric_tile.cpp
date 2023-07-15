/************************************************************************
 * Member functions for class FabricTile
 ***********************************************************************/
#include "fabric_tile.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/* namespace openfpga begins */
namespace openfpga {

FabricTile::FabricTile(const DeviceRRGSB& device_rr_gsb)
  : device_rr_gsb_(device_rr_gsb) {}

vtr::Point<size_t> FabricTile::tile_coordinate(
  const FabricTileId& tile_id) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  return coords_[tile_id];
}

FabricTileId FabricTile::unique_tile(const vtr::Point<size_t>& coord) const {
  /* Return invalid Id when out of range! */
  if (coord.x() < unique_tile_ids_.size()) {
    if (coord.y() < unique_tile_ids_[coord.x()].size()) {
      return unique_tile_ids_[coord.x()][coord.y()];
    }
  }
  return FabricTileId::INVALID();
}

FabricTileId FabricTile::create_tile() {
  FabricTileId tile_id = FabricTileId(ids_.size());
  ids_.push_back(tile_id);
  coords_.emplace_back();
  pb_coords_.emplace_back();
  cbx_coords_.emplace_back();
  cby_coords_.emplace_back();
  sb_coords_.emplace_back();

  return tile_id;
}

void set_tile_coordinate(const FabricTileId& tile_id,
                         const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  coords_[tile_id] = coord;
}

void add_pb_coordinate(const FabricTileId& tile_id,
                       const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  pb_coords_[tile_id] = coord;
}

void add_cbx_coordinate(const FabricTileId& tile_id,
                        const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  /* Ensure the coordinates are not duplicated */
  auto result = std::find(cbx_coords_.begin(), cbx_coords_.end(), coord);
  if (result == cbx_coords_.end()) {
    cbx_coords_[tile_id].push_back(coord);
  }
}

void add_cby_coordinate(const FabricTileId& tile_id,
                        const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  /* Ensure the coordinates are not duplicated */
  auto result = std::find(cby_coords_.begin(), cby_coords_.end(), coord);
  if (result == cby_coords_.end()) {
    cby_coords_[tile_id].push_back(coord);
  }
}

void add_sb_coordinate(const FabricTileId& tile_id,
                       const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  /* Ensure the coordinates are not duplicated */
  auto result = std::find(sb_coords_.begin(), sb_coords_.end(), coord);
  if (result == sb_coords_.end()) {
    sb_coords_[tile_id].push_back(coord);
  }
}

bool FabricTileId::valid_tile_id(const FabricTileId& tile_id) const {
  return (size_t(tile_id) < ids_.size()) && (tile_id == ids_[tile_id]);
}

} /* End namespace openfpga*/
