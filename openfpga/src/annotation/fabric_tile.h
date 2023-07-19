#ifndef FABRIC_TILE_H
#define FABRIC_TILE_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <vector>

#include "device_grid.h"
#include "device_rr_gsb.h"
#include "fabric_tile_fwd.h"
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * Object models the tiles in an FPGA fabric
 * This includes:
 * 1. a collection of tiles, each which contains a programmable block and
 *surrounding routing blocks
 * 2. a collection of unique tiles
 *******************************************************************/
class FabricTile {
 public: /* Contructors */
  FabricTile(const vtr::Point<size_t>& max_coord);

 public: /* Accessors */
  vtr::Point<size_t> tile_coordinate(const FabricTileId& tile_id) const;
  std::vector<vtr::Point<size_t>> pb_coordinates(
    const FabricTileId& tile_id) const;
  std::vector<vtr::Point<size_t>> cb_coordinates(
    const FabricTileId& tile_id, const t_rr_type& cb_type) const;
  std::vector<vtr::Point<size_t>> sb_coordinates(
    const FabricTileId& tile_id) const;
  /** @brief With a given coordinate, find the id of the unique tile (which is
   * the same as the tile in structure) */
  FabricTileId unique_tile(const vtr::Point<size_t>& coord) const;
  /** @brief Find the tile info with a given coordinate */
  FabricTileId find_tile(const vtr::Point<size_t>& coord) const;
  /** @brief Return a list of unique tiles */
  std::vector<FabricTileId> unique_tiles() const;
  /** @brief Find the index of a programmable block in the internal list by a
   * given coordinate. */
  size_t find_pb_index_in_tile(const FabricTileId& tile_id,
                               const vtr::Point<size_t>& coord) const;
  /** @brief Find the index of a switch block in the internal list by a given
   * coordinate. */
  size_t find_sb_index_in_tile(const FabricTileId& tile_id,
                               const vtr::Point<size_t>& coord) const;
  /** @brief Find the index of a connection block in the internal list by a
   * given coordinate. */
  size_t find_cb_index_in_tile(const FabricTileId& tile_id,
                               const t_rr_type& cb_type,
                               const vtr::Point<size_t>& coord) const;
  /** @brief Check if a programmable block (with a coordinate) exists in a tile
   */
  bool pb_in_tile(const FabricTileId& tile_id,
                  const vtr::Point<size_t>& coord) const;
  /** @brief Check if a switch block (with a coordinate) exists in a tile */
  bool sb_in_tile(const FabricTileId& tile_id,
                  const vtr::Point<size_t>& coord) const;
  /** @brief Check if a connection block (with a coordinate) exists in a tile */
  bool cb_in_tile(const FabricTileId& tile_id, const t_rr_type& cb_type,
                  const vtr::Point<size_t>& coord) const;

 public: /* Mutators */
  FabricTileId create_tile(const vtr::Point<size_t>& coord);
  bool set_tile_coordinate(const FabricTileId& tile_id,
                           const vtr::Point<size_t>& coord);
  void add_pb_coordinate(const FabricTileId& tile_id,
                         const vtr::Point<size_t>& coord);
  void add_cb_coordinate(const FabricTileId& tile_id, const t_rr_type& cb_type,
                         const vtr::Point<size_t>& coord);
  void add_sb_coordinate(const FabricTileId& tile_id,
                         const vtr::Point<size_t>& coord);
  /** @brief Build a list of unique tiles by comparing the coordinates in
   * DeviceRRGSB */
  void build_unique_tiles();
  /** @brief Clear all the content */
  void clear();
  /** @brief Initialize the data with a given range. Used by constructors */
  void init(const vtr::Point<size_t>& max_coord);
  /** @brief Identify the number of unique tiles and keep in the lookup */
  int build_unique_tiles(const DeviceGrid& grids,
                         const DeviceRRGSB& device_rr_gsb);

 public: /* Validators */
  bool valid_tile_id(const FabricTileId& tile_id) const;

 private: /* Internal validators */
  /** @brief Identify if two tile are equivalent in their sub-modules, including
   * pb, cbx, cby and sb */
  bool equivalent_tile(const FabricTileId& tile_a, const FabricTileId& tile_b,
                       const DeviceGrid& grids,
                       const DeviceRRGSB& device_rr_gsb) const;

 private: /* Internal builders */
  void invalidate_tile_in_lookup(const vtr::Point<size_t>& coord);
  bool register_tile_in_lookup(const FabricTileId& tile_id,
                               const vtr::Point<size_t>& coord);

 private: /* Internal Data */
  vtr::vector<FabricTileId, FabricTileId> ids_;
  vtr::vector<FabricTileId, vtr::Point<size_t>> coords_;
  /* Coordinates w.r.t. RRGSB */
  vtr::vector<FabricTileId, std::vector<vtr::Point<size_t>>> pb_coords_;
  vtr::vector<FabricTileId, std::vector<vtr::Point<size_t>>> cbx_coords_;
  vtr::vector<FabricTileId, std::vector<vtr::Point<size_t>>> cby_coords_;
  vtr::vector<FabricTileId, std::vector<vtr::Point<size_t>>> sb_coords_;
  /* A fast lookup to spot tile by coordinate */
  std::vector<std::vector<FabricTileId>> tile_coord2id_lookup_;
  std::vector<std::vector<FabricTileId>>
    tile_coord2unique_tile_ids_; /* Use [x][y] to get the id of the unique tile
                         with a given coordinate */
  std::vector<FabricTileId> unique_tile_ids_;
};

} /* End namespace openfpga*/

#endif
