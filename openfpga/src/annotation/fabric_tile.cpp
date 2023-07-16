/************************************************************************
 * Member functions for class FabricTile
 ***********************************************************************/
#include "fabric_tile.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/* namespace openfpga begins */
namespace openfpga {

FabricTile::FabricTile(const vtr::Point<size_t>& max_coord) { init(max_coord); }

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

FabricTileId FabricTile::find_tile(const vtr::Point<size_t>& coord) const {
  if (coord.x() >= tile_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Tile coordinate [%lu][%lu] exceeds the maximum range [%lu][%lu]!\n",
      coord.x(), coord.y(), tile_coord2id_lookup_.size(),
      tile_coord2id_lookup_[0].size());
    return FabricTileId::INVALID();
  }
  if (coord.y() >= tile_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Tile coordinate [%lu][%lu] exceeds the maximum range [%lu][%lu]!\n",
      coord.x(), coord.y(), tile_coord2id_lookup_.size(),
      tile_coord2id_lookup_[0].size());
    return FabricTileId::INVALID();
  }
  return tile_coord2id_lookup_[coord.x()][coord.y()];
}

FabricTileId FabricTile::create_tile(const vtr::Point<size_t>& coord) {
  FabricTileId tile_id = FabricTileId(ids_.size());
  ids_.push_back(tile_id);
  coords_.push_back(coord);
  pb_coords_.emplace_back();
  cbx_coords_.emplace_back();
  cby_coords_.emplace_back();
  sb_coords_.emplace_back();

  /* Register in fast look-up */
  if (register_tile_in_lookup(tile_id, coord)) {
    return tile_id;
  }
  return FabricTileId::INVALID();
}

void FabricTile::init(const vtr::Point<size_t>& max_coord) {
  tile_coord2id_lookup_.resize(max_coord.x());
  for (size_t ix = 0; ix < max_coord.x(); ++ix) {
    tile_coord2id_lookup_[ix].resize(max_coord.y(), FabricTileId::INVALID());
  }
  unique_tile_ids_.resize(max_coord.x());
  for (size_t ix = 0; ix < max_coord.x(); ++ix) {
    unique_tile_ids_[ix].resize(max_coord.y(), FabricTileId::INVALID());
  }
}

bool FabricTile::register_tile_in_lookup(const FabricTileId& tile_id,
                                         const vtr::Point<size_t>& coord) {
  if (coord.x() >= tile_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given x='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.x(), tile_coord2id_lookup_.size());
    return false;
  }
  if (coord.y() >= tile_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given y='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.y(), tile_coord2id_lookup_[coord.x()].size());
    return false;
  }
  /* Throw error if this coord is already registered! */
  if (tile_coord2id_lookup_[coord.x()][coord.y()]) {
    VTR_LOG_ERROR("Tile at [%lu][%lu] has already been registered!\n");
    return false;
  }
  tile_coord2id_lookup_[coord.x()][coord.y()] = tile_id;

  return true;
}

void FabricTile::invalidate_tile_in_lookup(const vtr::Point<size_t>& coord) {
  tile_coord2id_lookup_[coord.x()][coord.y()] = FabricTileId::INVALID();
}

bool FabricTile::set_tile_coordinate(const FabricTileId& tile_id,
                                     const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  /* Invalidate previous coordinate in look-up */
  invalidate_tile_in_lookup(coords_[tile_id]);
  /* update coordinate */
  coords_[tile_id] = coord;
  /* Register in fast look-up */
  return register_tile_in_lookup(tile_id, coord);
}

void FabricTile::add_pb_coordinate(const FabricTileId& tile_id,
                                   const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  pb_coords_[tile_id] = coord;
}

void FabricTile::add_cbx_coordinate(const FabricTileId& tile_id,
                                    const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  cbx_coords_[tile_id].push_back(coord);
}

void FabricTile::add_cby_coordinate(const FabricTileId& tile_id,
                                    const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  cby_coords_[tile_id].push_back(coord);
}

void FabricTile::add_sb_coordinate(const FabricTileId& tile_id,
                                   const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  sb_coords_[tile_id].push_back(coord);
}

void FabricTile::clear() {
  ids_.clear();
  coords_.clear();
  pb_coords_.clear();
  cbx_coords_.clear();
  cby_coords_.clear();
  sb_coords_.clear();
  tile_coord2id_lookup_.clear();
  unique_tile_ids_.clear();
}

bool FabricTile::valid_tile_id(const FabricTileId& tile_id) const {
  return (size_t(tile_id) < ids_.size()) && (tile_id == ids_[tile_id]);
}

} /* End namespace openfpga*/
