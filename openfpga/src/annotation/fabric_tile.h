#ifndef FABRIC_TILE_H
#define FABRIC_TILE_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <vector>

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
  /** @brief With a given coordinate, find the id of the unique tile (which is
   * the same as the tile in structure) */
  FabricTileId unique_tile(const vtr::Point<size_t>& coord) const;
  /** @brief Find the tile info with a given coordinate */
  FabricTileId find_tile(const vtr::Point<size_t>& coord) const;

 public: /* Mutators */
  FabricTileId create_tile(const vtr::Point<size_t>& coord);
  bool set_tile_coordinate(const FabricTileId& tile_id,
                           const vtr::Point<size_t>& coord);
  void add_pb_coordinate(const FabricTileId& tile_id,
                         const vtr::Point<size_t>& coord);
  void add_cbx_coordinate(const FabricTileId& tile_id,
                          const vtr::Point<size_t>& coord);
  void add_cby_coordinate(const FabricTileId& tile_id,
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

 public: /* Validators */
  bool valid_tile_id(const FabricTileId& tile_id) const;

 private: /* Internal builders */
  void invalidate_tile_in_lookup(const vtr::Point<size_t>& coord);
  bool register_tile_in_lookup(const FabricTileId& tile_id,
                               const vtr::Point<size_t>& coord);

 private: /* Internal Data */
  vtr::vector<FabricTileId, FabricTileId> ids_;
  vtr::vector<FabricTileId, vtr::Point<size_t>> coords_;
  /* Coordinates w.r.t. RRGSB */
  vtr::vector<FabricTileId, vtr::Point<size_t>> pb_coords_;
  vtr::vector<FabricTileId, std::vector<vtr::Point<size_t>>> cbx_coords_;
  vtr::vector<FabricTileId, std::vector<vtr::Point<size_t>>> cby_coords_;
  vtr::vector<FabricTileId, std::vector<vtr::Point<size_t>>> sb_coords_;
  /* A fast lookup to spot tile by coordinate */
  std::vector<std::vector<FabricTileId>> tile_coord2id_lookup_;
  std::vector<std::vector<FabricTileId>>
    unique_tile_ids_; /* Use [x][y] to get the id of the unique tile with a
                         given coordinate */
};

} /* End namespace openfpga*/

#endif
