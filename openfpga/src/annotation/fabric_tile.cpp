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

std::vector<vtr::Point<size_t>> FabricTile::pb_coordinates(
  const FabricTileId& tile_id) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  return pb_coords_[tile_id];
}

std::vector<vtr::Point<size_t>> FabricTile::cb_coordinates(
  const FabricTileId& tile_id, const t_rr_type& cb_type) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  switch (cb_type) {
    case CHANX:
      return cbx_coords_[tile_id];
    case CHANY:
      return cby_coords_[tile_id];
    default:
      VTR_LOG("Invalid type of connection block!\n");
      exit(1);
  }
  return std::vector<vtr::Point<size_t>>();
}

std::vector<vtr::Point<size_t>> FabricTile::sb_coordinates(
  const FabricTileId& tile_id) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  return sb_coords_[tile_id];
}

FabricTileId FabricTile::unique_tile(const vtr::Point<size_t>& coord) const {
  /* Return invalid Id when out of range! */
  if (coord.x() < tile_coord2unique_tile_ids_.size()) {
    if (coord.y() < tile_coord2unique_tile_ids_[coord.x()].size()) {
      return tile_coord2unique_tile_ids_[coord.x()][coord.y()];
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

bool FabricTile::pb_in_tile(const FabricTileId& tile_id,
                            const vtr::Point<size_t>& coord) const {
  return find_pb_index_in_tile(tile_id, coord) != pb_coords_.size();
}

size_t FabricTile::find_pb_index_in_tile(
  const FabricTileId& tile_id, const vtr::Point<size_t>& coord) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  for (size_t idx = 0; idx < pb_coords_[tile_id].size(); ++idx) {
    vtr::Point<size_t> curr_coord = pb_coords_[tile_id][idx];
    if (curr_coord == coord) {
      return idx;
    }
  }
  /* Not found, return an invalid index */
  return pb_coords_.size();
}

bool FabricTile::sb_in_tile(const FabricTileId& tile_id,
                            const vtr::Point<size_t>& coord) const {
  return find_sb_index_in_tile(tile_id, coord) != sb_coords_.size();
}

size_t FabricTile::find_sb_index_in_tile(
  const FabricTileId& tile_id, const vtr::Point<size_t>& coord) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  for (size_t idx = 0; idx < sb_coords_[tile_id].size(); ++idx) {
    vtr::Point<size_t> curr_coord = sb_coords_[tile_id][idx];
    if (curr_coord == coord) {
      return idx;
    }
  }
  /* Not found, return an invalid index */
  return sb_coords_.size();
}

bool FabricTile::cb_in_tile(const FabricTileId& tile_id,
                            const t_rr_type& cb_type,
                            const vtr::Point<size_t>& coord) const {
  switch (cb_type) {
    case CHANX:
      return find_cb_index_in_tile(tile_id, cb_type, coord) ==
             cbx_coords_.size();
    case CHANY:
      return find_cb_index_in_tile(tile_id, cb_type, coord) ==
             cby_coords_.size();
    default:
      VTR_LOG("Invalid type of connection block!\n");
      exit(1);
  }
}

size_t FabricTile::find_cb_index_in_tile(
  const FabricTileId& tile_id, const t_rr_type& cb_type,
  const vtr::Point<size_t>& coord) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  switch (cb_type) {
    case CHANX:
      for (size_t idx = 0; idx < cbx_coords_[tile_id].size(); ++idx) {
        vtr::Point<size_t> curr_coord = cbx_coords_[tile_id][idx];
        if (curr_coord == coord) {
          return idx;
        }
      }
      return cbx_coords_[tile_id].size();
    case CHANY:
      for (size_t idx = 0; idx < cby_coords_[tile_id].size(); ++idx) {
        vtr::Point<size_t> curr_coord = cby_coords_[tile_id][idx];
        if (curr_coord == coord) {
          return idx;
        }
      }
      return cby_coords_[tile_id].size();
    default:
      VTR_LOG("Invalid type of connection block!\n");
      exit(1);
  }
}

std::vector<FabricTileId> FabricTile::unique_tiles() const {
  return unique_tile_ids_;
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
  tile_coord2unique_tile_ids_.resize(max_coord.x());
  for (size_t ix = 0; ix < max_coord.x(); ++ix) {
    tile_coord2unique_tile_ids_[ix].resize(max_coord.y(),
                                           FabricTileId::INVALID());
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
  pb_coords_[tile_id].push_back(coord);
}

void FabricTile::add_cb_coordinate(const FabricTileId& tile_id,
                                   const t_rr_type& cb_type,
                                   const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  switch (cb_type) {
    case CHANX:
      cbx_coords_[tile_id].push_back(coord);
      break;
    case CHANY:
      cby_coords_[tile_id].push_back(coord);
      break;
    default:
      VTR_LOG("Invalid type of connection block!\n");
      exit(1);
  }
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
  tile_coord2unique_tile_ids_.clear();
  unique_tile_ids_.clear();
}

bool FabricTile::valid_tile_id(const FabricTileId& tile_id) const {
  return (size_t(tile_id) < ids_.size()) && (tile_id == ids_[tile_id]);
}

bool FabricTile::equivalent_tile(const FabricTileId& tile_a,
                                 const FabricTileId& tile_b,
                                 const DeviceGrid& grids,
                                 const DeviceRRGSB& device_rr_gsb) const {
  /* The number of cbx, cby and sb blocks should be the same */
  if (pb_coords_[tile_a].size() != pb_coords_[tile_b].size() ||
      cbx_coords_[tile_a].size() != cbx_coords_[tile_b].size() ||
      cby_coords_[tile_a].size() != cby_coords_[tile_b].size() ||
      sb_coords_[tile_a].size() != sb_coords_[tile_b].size()) {
    return false;
  }
  /* The pb of two tiles should be the same, otherwise not equivalent */
  for (size_t iblk = 0; iblk < pb_coords_[tile_a].size(); ++iblk) {
    if (grids.get_physical_type(pb_coords_[tile_a][iblk].x(),
                                pb_coords_[tile_a][iblk].y()) !=
        grids.get_physical_type(pb_coords_[tile_b][iblk].x(),
                                pb_coords_[tile_b][iblk].y())) {
      return false;
    }
  }
  /* Each CBx should have the same unique modules in the device rr_gsb */
  for (size_t iblk = 0; iblk < cbx_coords_[tile_a].size(); ++iblk) {
    if (device_rr_gsb.get_cb_unique_module_index(CHANX,
                                                 cbx_coords_[tile_a][iblk]) !=
        device_rr_gsb.get_cb_unique_module_index(CHANX,
                                                 cbx_coords_[tile_b][iblk])) {
      return false;
    }
  }
  for (size_t iblk = 0; iblk < cby_coords_[tile_a].size(); ++iblk) {
    if (device_rr_gsb.get_cb_unique_module_index(CHANY,
                                                 cby_coords_[tile_a][iblk]) !=
        device_rr_gsb.get_cb_unique_module_index(CHANY,
                                                 cby_coords_[tile_b][iblk])) {
      return false;
    }
  }
  for (size_t iblk = 0; iblk < sb_coords_[tile_a].size(); ++iblk) {
    if (device_rr_gsb.get_sb_unique_module_index(sb_coords_[tile_a][iblk]) !=
        device_rr_gsb.get_sb_unique_module_index(sb_coords_[tile_b][iblk])) {
      return false;
    }
  }
  return true;
}

int FabricTile::build_unique_tiles(const DeviceGrid& grids,
                                   const DeviceRRGSB& device_rr_gsb) {
  for (size_t ix = 0; ix < grids.width(); ++ix) {
    for (size_t iy = 0; iy < grids.height(); ++iy) {
      bool is_unique_tile = true;
      for (FabricTileId unique_tile_id : unique_tile_ids_) {
        if (equivalent_tile(tile_coord2id_lookup_[ix][iy], unique_tile_id,
                            grids, device_rr_gsb)) {
          is_unique_tile = false;
          tile_coord2unique_tile_ids_[ix][iy] = unique_tile_id;
          break;
        }
      }
      /* Update list if this is a unique tile */
      if (is_unique_tile) {
        unique_tile_ids_.push_back(tile_coord2unique_tile_ids_[ix][iy]);
        tile_coord2unique_tile_ids_[ix][iy] = tile_coord2id_lookup_[ix][iy];
      }
    }
  }
  return 0;
}

} /* End namespace openfpga*/
