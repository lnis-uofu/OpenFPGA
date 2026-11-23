/******************************************************************************
 * BitstreamReorderMap — maps tile-order WL×BL intersections to their
 * logical config-bit IDs and physical WL×BL memory-array addresses.
 *
 * XML SCHEMA (bitstream_remap)
 * ----------------------------
 * The XML file has two kinds of top-level children:
 *
 *   <tile_bitmap name="TILE_TYPE" cbits="C" bl="B" wl="W">
 *     <bit index="I">J</bit>  ...
 *   </tile_bitmap>
 *
 *   Describes one tile type.  The tile has C config bits and a B×W BL×WL
 *   grid.  Each <bit> maps logical config-bit index I to flat WL×BL position
 *   J = wl_row * B + bl_col.  Positions with no <bit> entry carry no config
 *   data (e.g. dummy or unused cells).
 *
 *   <region id="R" wl="W" total_cbits="C">
 *     <tile id="T" name="TILE_TYPE" alias="..."/>  ...
 *   </region>
 *
 *   Describes one configuration region of the device.  A region spans the
 *   full device width and W wordlines vertically.  Tiles are listed in XML
 *   order and grouped into columns: consecutive tiles are accumulated until
 *   their combined WL count equals W, at which point the next column begins.
 *   All tiles in a column share the same BL lines.
 *
 * ITERATION
 * ---------
 * The primary interface is for_each_bit(), which iterates all valid WL×BL
 * intersections in tile order.  Global (wl, bl) addresses and the config-bit
 * ID are computed on the fly by tracking column position within each region —
 * no precomputed tables are needed.  This directly mirrors the Python
 * reference implementation.
 *
 ******************************************************************************/

#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "bitstream_manager_fwd.h"

namespace openfpga {

// Per-tile-type mapping from flat WL×BL positions to config bit indices.
// bit_map[wl_row * num_bls + bl_col] = config bit index within the tile,
// or INVALID if that WL×BL intersection carries no config bit.
struct tile_bit_map {
  std::vector<ConfigBitId> bit_map;
  size_t num_cbits;  // number of config bits in this tile type
  size_t num_bls;    // number of bitlines
  size_t num_wls;    // number of wordlines
};

// One configuration region of the device.
// Tiles are listed in XML order; consecutive tiles are grouped into columns
// until their combined WL count equals num_wls.
struct reorder_region {
  std::vector<std::string> tile_names;  // tile types in XML order
  size_t num_wls;                       // total wordlines in this region
  size_t num_bls;  // total bitlines (sum of first-tile-in-column BLs)
};

class BitstreamReorderMap {
 public:
  BitstreamReorderMap() = delete;
  BitstreamReorderMap(const std::string& reorder_map_file);
  ~BitstreamReorderMap();

  /**
   * @brief Total number of bitlines across the device (max over regions).
   */
  size_t get_bl_address_size() const;

  /**
   * @brief Total number of wordlines across the device (sum over regions).
   */
  size_t get_wl_address_size() const;

  /**
   * @brief Iterate over all valid WL×BL intersections in tile order.
   *
   * For each intersection that carries a config bit, invokes
   *   callback(wl_index, bl_index, global_config_bit_id)
   * where global_config_bit_id is the sequential index of the config bit in
   * the fabric bitstream (same order as tiles appear in the remap XML).
   *
   * Global addresses and config-bit IDs are computed on the fly by tracking
   * column position within each region (no precomputed lookup tables needed).
   */
  void for_each_bit(
    const std::function<void(size_t wl, size_t bl, size_t global_cbit_id)>&
      callback) const;

 private:
  void init_from_file(const std::string& reorder_map_file);

  std::unordered_map<std::string, tile_bit_map> tile_bit_maps_;
  std::vector<reorder_region> regions_;
};

}  // namespace openfpga
