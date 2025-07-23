/************************************************************************
 * Member functions for class FabricTile
 ***********************************************************************/
#include "fabric_tile.h"

#include "build_top_module_utils.h"
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* namespace openfpga begins */
namespace openfpga {

vtr::Point<size_t> FabricTile::tile_coordinate(
  const FabricTileId& tile_id) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  return coords_[tile_id];
}

vtr::Point<size_t> FabricTile::unique_tile_coordinate(
  const FabricTileId& tile_id) const {
  vtr::Point<size_t> tile_coord = tile_coordinate(tile_id);
  FabricTileId unique_fabric_tile_id = unique_tile(tile_coord);
  return tile_coordinate(unique_fabric_tile_id);
}

FabricTileId FabricTile::find_unique_tile(const FabricTileId& tile_id) const {
  vtr::Point<size_t> tile_coord = tile_coordinate(tile_id);
  return unique_tile(tile_coord);
}

std::vector<vtr::Point<size_t>> FabricTile::pb_coordinates(
  const FabricTileId& tile_id) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  std::vector<vtr::Point<size_t>> pb_root_coords;
  pb_root_coords.reserve(pb_coords_[tile_id].size());
  for (auto curr_rect : pb_coords_[tile_id]) {
    pb_root_coords.push_back(curr_rect.bottom_left());
  }
  return pb_root_coords;
}

std::vector<vtr::Point<size_t>> FabricTile::cb_coordinates(
  const FabricTileId& tile_id, const e_rr_type& cb_type) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  switch (cb_type) {
    case e_rr_type::CHANX:
      return cbx_coords_[tile_id];
    case e_rr_type::CHANY:
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

FabricTileId FabricTile::find_tile_by_pb_coordinate(
  const vtr::Point<size_t>& coord) const {
  if (pb_coord2id_lookup_.empty()) {
    return FabricTileId::INVALID();
  }
  if (coord.x() >= pb_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Programmable block coordinate [%lu][%lu] exceeds the maximum range "
      "[%lu][%lu]!\n",
      coord.x(), coord.y(), pb_coord2id_lookup_.size(),
      pb_coord2id_lookup_[0].size());
    return FabricTileId::INVALID();
  }
  if (coord.y() >= pb_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Programmable block coordinate [%lu][%lu] exceeds the maximum range "
      "[%lu][%lu]!\n",
      coord.x(), coord.y(), pb_coord2id_lookup_.size(),
      pb_coord2id_lookup_[0].size());
    return FabricTileId::INVALID();
  }
  return pb_coord2id_lookup_[coord.x()][coord.y()];
}

FabricTileId FabricTile::find_tile_by_cb_coordinate(
  const e_rr_type& cb_type, const vtr::Point<size_t>& coord) const {
  switch (cb_type) {
    case e_rr_type::CHANX: {
      if (cbx_coord2id_lookup_.empty()) {
        return FabricTileId::INVALID();
      }
      if (coord.x() >= cbx_coord2id_lookup_.size()) {
        VTR_LOG_ERROR(
          "X-direction connection block coordinate [%lu][%lu] exceeds the "
          "maximum range [%lu][%lu]!\n",
          coord.x(), coord.y(), cbx_coord2id_lookup_.size(),
          cbx_coord2id_lookup_[0].size());
        return FabricTileId::INVALID();
      }
      if (coord.y() >= cbx_coord2id_lookup_[coord.x()].size()) {
        VTR_LOG_ERROR(
          "X-direction connection block coordinate [%lu][%lu] exceeds the "
          "maximum range [%lu][%lu]!\n",
          coord.x(), coord.y(), cbx_coord2id_lookup_.size(),
          cbx_coord2id_lookup_[0].size());
        return FabricTileId::INVALID();
      }
      return cbx_coord2id_lookup_[coord.x()][coord.y()];
    }
    case e_rr_type::CHANY: {
      if (cby_coord2id_lookup_.empty()) {
        return FabricTileId::INVALID();
      }
      if (coord.x() >= cby_coord2id_lookup_.size()) {
        VTR_LOG_ERROR(
          "Y-direction connection block coordinate [%lu][%lu] exceeds the "
          "maximum range [%lu][%lu]!\n",
          coord.x(), coord.y(), cby_coord2id_lookup_.size(),
          cby_coord2id_lookup_[0].size());
        return FabricTileId::INVALID();
      }
      if (coord.y() >= cby_coord2id_lookup_[coord.x()].size()) {
        VTR_LOG_ERROR(
          "Y-direction connection block coordinate [%lu][%lu] exceeds the "
          "maximum range [%lu][%lu]!\n",
          coord.x(), coord.y(), cby_coord2id_lookup_.size(),
          cby_coord2id_lookup_[0].size());
        return FabricTileId::INVALID();
      }
      return cby_coord2id_lookup_[coord.x()][coord.y()];
    }
    default:
      VTR_LOG("Invalid type of connection block!\n");
      exit(1);
  }
}

FabricTileId FabricTile::find_tile_by_sb_coordinate(
  const vtr::Point<size_t>& coord) const {
  if (sb_coord2id_lookup_.empty()) {
    return FabricTileId::INVALID();
  }
  if (coord.x() >= sb_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Switch block coordinate [%lu][%lu] exceeds the maximum range "
      "[%lu][%lu]!\n",
      coord.x(), coord.y(), sb_coord2id_lookup_.size(),
      sb_coord2id_lookup_[0].size());
    return FabricTileId::INVALID();
  }
  if (coord.y() >= sb_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Switch block coordinate [%lu][%lu] exceeds the maximum range "
      "[%lu][%lu]!\n",
      coord.x(), coord.y(), sb_coord2id_lookup_.size(),
      sb_coord2id_lookup_[0].size());
    return FabricTileId::INVALID();
  }
  return sb_coord2id_lookup_[coord.x()][coord.y()];
}

bool FabricTile::pb_in_tile(const FabricTileId& tile_id,
                            const vtr::Point<size_t>& coord,
                            const bool& use_gsb_coord) const {
  if (use_gsb_coord) {
    return !pb_gsb_coords_[tile_id].empty() &&
           find_pb_index_in_tile(tile_id, coord, use_gsb_coord) !=
             pb_gsb_coords_[tile_id].size();
  }
  return !pb_coords_[tile_id].empty() &&
         find_pb_index_in_tile(tile_id, coord) != pb_coords_[tile_id].size();
}

size_t FabricTile::find_pb_index_in_tile(const FabricTileId& tile_id,
                                         const vtr::Point<size_t>& coord,
                                         const bool& use_gsb_coord) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  if (use_gsb_coord) {
    for (size_t idx = 0; idx < pb_gsb_coords_[tile_id].size(); ++idx) {
      vtr::Point<size_t> curr_coord = pb_gsb_coords_[tile_id][idx];
      if (curr_coord == coord) {
        return idx;
      }
    }
    /* Not found, return an invalid index */
    return pb_gsb_coords_[tile_id].size();
  } else {
    for (size_t idx = 0; idx < pb_coords_[tile_id].size(); ++idx) {
      if (pb_coords_[tile_id][idx].coincident(coord)) {
        return idx;
      }
    }
    /* Not found, return an invalid index */
    return pb_coords_[tile_id].size();
  }
}

bool FabricTile::sb_in_tile(const FabricTileId& tile_id,
                            const vtr::Point<size_t>& coord) const {
  return !sb_coords_[tile_id].empty() &&
         find_sb_index_in_tile(tile_id, coord) != sb_coords_[tile_id].size();
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
  return sb_coords_[tile_id].size();
}

bool FabricTile::cb_in_tile(const FabricTileId& tile_id,
                            const e_rr_type& cb_type,
                            const vtr::Point<size_t>& coord) const {
  switch (cb_type) {
    case e_rr_type::CHANX:
      return !cbx_coords_[tile_id].empty() &&
             find_cb_index_in_tile(tile_id, cb_type, coord) !=
               cbx_coords_[tile_id].size();
    case e_rr_type::CHANY:
      return !cby_coords_[tile_id].empty() &&
             find_cb_index_in_tile(tile_id, cb_type, coord) !=
               cby_coords_[tile_id].size();
    default:
      VTR_LOG("Invalid type of connection block!\n");
      exit(1);
  }
}

size_t FabricTile::find_cb_index_in_tile(
  const FabricTileId& tile_id, const e_rr_type& cb_type,
  const vtr::Point<size_t>& coord) const {
  VTR_ASSERT(valid_tile_id(tile_id));
  switch (cb_type) {
    case e_rr_type::CHANX:
      for (size_t idx = 0; idx < cbx_coords_[tile_id].size(); ++idx) {
        vtr::Point<size_t> curr_coord = cbx_coords_[tile_id][idx];
        if (curr_coord == coord) {
          return idx;
        }
      }
      return cbx_coords_[tile_id].size();
    case e_rr_type::CHANY:
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

vtr::Point<size_t> FabricTile::find_cb_coordinate_in_unique_tile(
  const FabricTileId& tile_id, const e_rr_type& cb_type,
  const vtr::Point<size_t>& cb_coord) const {
  size_t cb_idx_in_curr_tile =
    find_cb_index_in_tile(tile_id, cb_type, cb_coord);
  FabricTileId unique_tile = find_unique_tile(tile_id);
  return cb_coordinates(unique_tile, cb_type)[cb_idx_in_curr_tile];
}

vtr::Point<size_t> FabricTile::find_pb_coordinate_in_unique_tile(
  const FabricTileId& tile_id, const vtr::Point<size_t>& pb_coord) const {
  size_t pb_idx_in_curr_tile = find_pb_index_in_tile(tile_id, pb_coord);
  FabricTileId unique_tile = find_unique_tile(tile_id);
  return pb_coordinates(unique_tile)[pb_idx_in_curr_tile];
}

vtr::Point<size_t> FabricTile::find_sb_coordinate_in_unique_tile(
  const FabricTileId& tile_id, const vtr::Point<size_t>& sb_coord) const {
  size_t sb_idx_in_curr_tile = find_sb_index_in_tile(tile_id, sb_coord);
  FabricTileId unique_tile = find_unique_tile(tile_id);
  return sb_coordinates(unique_tile)[sb_idx_in_curr_tile];
}

std::vector<FabricTileId> FabricTile::unique_tiles() const {
  return unique_tile_ids_;
}

bool FabricTile::empty() const { return ids_.empty(); }

FabricTileId FabricTile::create_tile(const vtr::Point<size_t>& coord) {
  FabricTileId tile_id = FabricTileId(ids_.size());
  ids_.push_back(tile_id);
  coords_.push_back(coord);
  pb_coords_.emplace_back();
  pb_gsb_coords_.emplace_back();
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
  pb_coord2id_lookup_.resize(max_coord.x());
  cbx_coord2id_lookup_.resize(max_coord.x());
  cby_coord2id_lookup_.resize(max_coord.x());
  sb_coord2id_lookup_.resize(max_coord.x());
  for (size_t ix = 0; ix < max_coord.x(); ++ix) {
    tile_coord2id_lookup_[ix].resize(max_coord.y(), FabricTileId::INVALID());
    pb_coord2id_lookup_[ix].resize(max_coord.y(), FabricTileId::INVALID());
    cbx_coord2id_lookup_[ix].resize(max_coord.y(), FabricTileId::INVALID());
    cby_coord2id_lookup_[ix].resize(max_coord.y(), FabricTileId::INVALID());
    sb_coord2id_lookup_[ix].resize(max_coord.y(), FabricTileId::INVALID());
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
    VTR_LOG_ERROR("Tile at [%lu][%lu] has already been registered!\n",
                  coord.x(), coord.y());
    return false;
  }
  tile_coord2id_lookup_[coord.x()][coord.y()] = tile_id;

  return true;
}

bool FabricTile::register_pb_in_lookup(const FabricTileId& tile_id,
                                       const vtr::Point<size_t>& coord) {
  if (coord.x() >= pb_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given x='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.x(), pb_coord2id_lookup_.size());
    return false;
  }
  if (coord.y() >= pb_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given y='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.y(), pb_coord2id_lookup_[coord.x()].size());
    return false;
  }
  /* Throw error if this coord is already registered! */
  if (pb_coord2id_lookup_[coord.x()][coord.y()] &&
      pb_coord2id_lookup_[coord.x()][coord.y()] != tile_id) {
    VTR_LOG_ERROR(
      "Programmable block at [%lu][%lu] has already been registered!\n",
      coord.x(), coord.y());
    return false;
  }
  pb_coord2id_lookup_[coord.x()][coord.y()] = tile_id;

  return true;
}

bool FabricTile::register_cbx_in_lookup(const FabricTileId& tile_id,
                                        const vtr::Point<size_t>& coord) {
  if (coord.x() >= cbx_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given x='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.x(), cbx_coord2id_lookup_.size());
    return false;
  }
  if (coord.y() >= cbx_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given y='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.y(), cbx_coord2id_lookup_[coord.x()].size());
    return false;
  }
  /* Throw error if this coord is already registered! */
  if (cbx_coord2id_lookup_[coord.x()][coord.y()]) {
    VTR_LOG_ERROR(
      "X-direction connection block at [%lu][%lu] has already been "
      "registered!\n",
      coord.x(), coord.y());
    return false;
  }
  cbx_coord2id_lookup_[coord.x()][coord.y()] = tile_id;

  return true;
}

bool FabricTile::register_cby_in_lookup(const FabricTileId& tile_id,
                                        const vtr::Point<size_t>& coord) {
  if (coord.x() >= cby_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given x='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.x(), cby_coord2id_lookup_.size());
    return false;
  }
  if (coord.y() >= cby_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given y='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.y(), cby_coord2id_lookup_[coord.x()].size());
    return false;
  }
  /* Throw error if this coord is already registered! */
  if (cby_coord2id_lookup_[coord.x()][coord.y()]) {
    VTR_LOG_ERROR(
      "Y-direction connection block at [%lu][%lu] has already been "
      "registered!\n",
      coord.x(), coord.y());
    return false;
  }
  cby_coord2id_lookup_[coord.x()][coord.y()] = tile_id;

  return true;
}

bool FabricTile::register_sb_in_lookup(const FabricTileId& tile_id,
                                       const vtr::Point<size_t>& coord) {
  if (coord.x() >= sb_coord2id_lookup_.size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given x='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.x(), sb_coord2id_lookup_.size());
    return false;
  }
  if (coord.y() >= sb_coord2id_lookup_[coord.x()].size()) {
    VTR_LOG_ERROR(
      "Fast look-up has not been re-allocated properly! Given y='%lu' exceeds "
      "the upper-bound '%lu'!\n",
      coord.y(), sb_coord2id_lookup_[coord.x()].size());
    return false;
  }
  /* Throw error if this coord is already registered! */
  if (sb_coord2id_lookup_[coord.x()][coord.y()]) {
    VTR_LOG_ERROR("Switch block at [%lu][%lu] has already been registered!\n",
                  coord.x(), coord.y());
    return false;
  }
  sb_coord2id_lookup_[coord.x()][coord.y()] = tile_id;

  return true;
}

void FabricTile::invalidate_tile_in_lookup(const vtr::Point<size_t>& coord) {
  tile_coord2id_lookup_[coord.x()][coord.y()] = FabricTileId::INVALID();
}

void FabricTile::invalidate_pb_in_lookup(const vtr::Point<size_t>& coord) {
  pb_coord2id_lookup_[coord.x()][coord.y()] = FabricTileId::INVALID();
}

void FabricTile::invalidate_cbx_in_lookup(const vtr::Point<size_t>& coord) {
  cbx_coord2id_lookup_[coord.x()][coord.y()] = FabricTileId::INVALID();
}

void FabricTile::invalidate_cby_in_lookup(const vtr::Point<size_t>& coord) {
  cby_coord2id_lookup_[coord.x()][coord.y()] = FabricTileId::INVALID();
}

void FabricTile::invalidate_sb_in_lookup(const vtr::Point<size_t>& coord) {
  sb_coord2id_lookup_[coord.x()][coord.y()] = FabricTileId::INVALID();
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

int FabricTile::add_pb_coordinate(const FabricTileId& tile_id,
                                  const vtr::Point<size_t>& coord,
                                  const vtr::Point<size_t>& gsb_coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  pb_coords_[tile_id].push_back(vtr::Rect<size_t>(coord, coord));
  pb_gsb_coords_[tile_id].push_back(gsb_coord);
  /* Register in fast look-up */
  return register_pb_in_lookup(tile_id, coord);
}

int FabricTile::set_pb_max_coordinate(const FabricTileId& tile_id,
                                      const size_t& pb_index,
                                      const vtr::Point<size_t>& max_coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  if (pb_index >= pb_coords_[tile_id].size()) {
    VTR_LOG_ERROR(
      "Invalid pb_index '%lu' is out of range of programmable block list "
      "(size='%lu') of tile[%lu][%lu]!\n",
      pb_index, pb_coords_[tile_id].size(), tile_coordinate(tile_id).x(),
      tile_coordinate(tile_id).y());
    return CMD_EXEC_FATAL_ERROR;
  }
  if (max_coord.x() < pb_coords_[tile_id][pb_index].xmin() ||
      max_coord.y() < pb_coords_[tile_id][pb_index].ymin()) {
    VTR_LOG_ERROR(
      "Invalid max. coordinate (%lu, %lu) is out of range of programmable "
      "block list (%lu, %lu) <-> (%lu, %lu) of tile[%lu][%lu]!\n",
      max_coord.x(), max_coord.y(), pb_coords_[tile_id][pb_index].xmin(),
      pb_coords_[tile_id][pb_index].ymin(),
      pb_coords_[tile_id][pb_index].xmax(),
      pb_coords_[tile_id][pb_index].ymax(), tile_coordinate(tile_id).x(),
      tile_coordinate(tile_id).y());
    return CMD_EXEC_FATAL_ERROR;
  }
  pb_coords_[tile_id][pb_index].set_xmax(max_coord.x());
  pb_coords_[tile_id][pb_index].set_ymax(max_coord.y());
  /* Update fast lookup */
  for (size_t ix = pb_coords_[tile_id][pb_index].xmin();
       ix <= pb_coords_[tile_id][pb_index].xmax(); ++ix) {
    for (size_t iy = pb_coords_[tile_id][pb_index].ymin();
         iy <= pb_coords_[tile_id][pb_index].ymax(); ++iy) {
      register_pb_in_lookup(tile_id, vtr::Point<size_t>(ix, iy));
    }
  }
  return CMD_EXEC_SUCCESS;
}

int FabricTile::add_cb_coordinate(const FabricTileId& tile_id,
                                  const e_rr_type& cb_type,
                                  const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  switch (cb_type) {
    case e_rr_type::CHANX:
      cbx_coords_[tile_id].push_back(coord);
      /* Register in fast look-up */
      return register_cbx_in_lookup(tile_id, coord);
    case e_rr_type::CHANY:
      cby_coords_[tile_id].push_back(coord);
      /* Register in fast look-up */
      return register_cby_in_lookup(tile_id, coord);
    default:
      VTR_LOG("Invalid type of connection block!\n");
      exit(1);
  }
}

int FabricTile::add_sb_coordinate(const FabricTileId& tile_id,
                                  const vtr::Point<size_t>& coord) {
  VTR_ASSERT(valid_tile_id(tile_id));
  sb_coords_[tile_id].push_back(coord);
  /* Register in fast look-up */
  return register_sb_in_lookup(tile_id, coord);
}

void FabricTile::clear() {
  ids_.clear();
  coords_.clear();
  pb_coords_.clear();
  pb_gsb_coords_.clear();
  cbx_coords_.clear();
  cby_coords_.clear();
  sb_coords_.clear();
  tile_coord2id_lookup_.clear();
  pb_coord2id_lookup_.clear();
  cbx_coord2id_lookup_.clear();
  cby_coord2id_lookup_.clear();
  sb_coord2id_lookup_.clear();
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
      pb_gsb_coords_[tile_a].size() != pb_gsb_coords_[tile_b].size() ||
      cbx_coords_[tile_a].size() != cbx_coords_[tile_b].size() ||
      cby_coords_[tile_a].size() != cby_coords_[tile_b].size() ||
      sb_coords_[tile_a].size() != sb_coords_[tile_b].size()) {
    return false;
  }
  /* The pb of two tiles should be the same, otherwise not equivalent */
  for (size_t iblk = 0; iblk < pb_coords_[tile_a].size(); ++iblk) {
    vtr::Point<size_t> tile_a_pb_coord = pb_coords_[tile_a][iblk].bottom_left();
    vtr::Point<size_t> tile_b_pb_coord = pb_coords_[tile_b][iblk].bottom_left();
    if (generate_grid_block_module_name_in_top_module(std::string(), grids,
                                                      tile_a_pb_coord) !=
        generate_grid_block_module_name_in_top_module(std::string(), grids,
                                                      tile_b_pb_coord)) {
      return false;
    }
  }
  /* Each CBx should have the same unique modules in the device rr_gsb */
  for (size_t iblk = 0; iblk < cbx_coords_[tile_a].size(); ++iblk) {
    if (device_rr_gsb.get_cb_unique_module_index(e_rr_type::CHANX,
                                                 cbx_coords_[tile_a][iblk]) !=
        device_rr_gsb.get_cb_unique_module_index(e_rr_type::CHANX,
                                                 cbx_coords_[tile_b][iblk])) {
      return false;
    }
  }
  for (size_t iblk = 0; iblk < cby_coords_[tile_a].size(); ++iblk) {
    if (device_rr_gsb.get_cb_unique_module_index(e_rr_type::CHANY,
                                                 cby_coords_[tile_a][iblk]) !=
        device_rr_gsb.get_cb_unique_module_index(e_rr_type::CHANY,
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
                                   const DeviceRRGSB& device_rr_gsb,
                                   const bool& verbose) {
  for (size_t ix = 0; ix < grids.width(); ++ix) {
    for (size_t iy = 0; iy < grids.height(); ++iy) {
      if (!valid_tile_id(tile_coord2id_lookup_[ix][iy])) {
        continue; /* Skip invalid tile (which does not exist) */
      }
      bool is_unique_tile = true;
      for (FabricTileId unique_tile_id : unique_tile_ids_) {
        if (equivalent_tile(tile_coord2id_lookup_[ix][iy], unique_tile_id,
                            grids, device_rr_gsb)) {
          VTR_LOGV(verbose,
                   "Tile[%lu][%lu] is a mirror to the unique tile[%lu][%lu]\n",
                   ix, iy, tile_coordinate(unique_tile_id).x(),
                   tile_coordinate(unique_tile_id).y());
          is_unique_tile = false;
          tile_coord2unique_tile_ids_[ix][iy] = unique_tile_id;
          break;
        }
      }
      /* Update list if this is a unique tile */
      if (is_unique_tile) {
        VTR_LOGV(verbose, "Tile[%lu][%lu] is added as a new unique tile\n", ix,
                 iy);
        unique_tile_ids_.push_back(tile_coord2id_lookup_[ix][iy]);
        tile_coord2unique_tile_ids_[ix][iy] = tile_coord2id_lookup_[ix][iy];
      }
    }
  }
  return 0;
}

} /* End namespace openfpga*/
