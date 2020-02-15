#ifndef TILE_DIRECT_H
#define TILE_DIRECT_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* Headers from readarch library */
#include "physical_types.h"

#include "tile_direct_fwd.h"

/* Begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * TileDirect object aims to be a database to store all the information 
 * about direct connection between tiles
 *  - starting tile and end tile for each point-to-point direct connection
 *  - circuit model to implement each direct connection
 *
 * TileDirect is compiled from ArchDirect for a specific FPGA fabric. 
 *******************************************************************/
class TileDirect {
  public: /* Types and ranges */
    typedef vtr::vector<TileDirectId, TileDirectId>::const_iterator tile_direct_iterator;
    typedef vtr::Range<tile_direct_iterator> tile_direct_range;
  public: /* Public aggregators */
    tile_direct_range directs() const;
    t_physical_tile_type_ptr from_tile(const TileDirectId& direct_id) const;
    vtr::Point<size_t> from_tile_coordinate(const TileDirectId& direct_id) const;
    e_side from_tile_side(const TileDirectId& direct_id) const;
    size_t from_tile_pin(const TileDirectId& direct_id) const;
    t_physical_tile_type_ptr to_tile(const TileDirectId& direct_id) const;
    vtr::Point<size_t> to_tile_coordinate(const TileDirectId& direct_id) const;
    e_side to_tile_side(const TileDirectId& direct_id) const;
    size_t to_tile_pin(const TileDirectId& direct_id) const;
  public: /* Public mutators */
    TileDirectId add_direct(t_physical_tile_type_ptr from_tile,
                            const vtr::Point<size_t>& from_tile_coord,
                            const e_side& from_tile_side,
                            const size_t& from_tile_pin,
                            t_physical_tile_type_ptr to_tile,
                            const vtr::Point<size_t>& to_tile_coord,
                            const e_side& to_tile_side,
                            const size_t& to_tile_pin);
  public: /* Public validators/invalidators */
    bool valid_direct_id(const TileDirectId& direct_id) const;
  private: /* Internal Data */
    vtr::vector<TileDirectId, TileDirectId> direct_ids_;

    /* Detailed information about the starting tile
     * - tile type description
     * - tile coordinate
     * - tile pin id
     */
    vtr::vector<TileDirectId, t_physical_tile_type_ptr> from_tiles_;
    vtr::vector<TileDirectId, vtr::Point<size_t>> from_tile_coords_;
    vtr::vector<TileDirectId, e_side> from_tile_sides_;
    vtr::vector<TileDirectId, size_t> from_tile_pins_;

    /* Detailed information about the ending tile
     * - tile type description
     * - tile coordinate
     * - tile pin id
     */
    vtr::vector<TileDirectId, t_physical_tile_type_ptr> to_tiles_;
    vtr::vector<TileDirectId, vtr::Point<size_t>> to_tile_coords_;
    vtr::vector<TileDirectId, e_side> to_tile_sides_;
    vtr::vector<TileDirectId, size_t> to_tile_pins_;
};

} /* End namespace openfpga*/

#endif
