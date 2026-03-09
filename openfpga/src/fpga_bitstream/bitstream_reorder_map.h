/******************************************************************************
 * BitstreamReorderMap — maps tile-order WL×BL intersection indices to their
 * logical config-bit IDs and physical WL×BL memory-array addresses.
 *
 * BACKGROUND
 * ----------
 * An FPGA fabric is programmed by writing configuration bits into a 2-D SRAM
 * array addressed by wordlines (WL, rows) and bitlines (BL, columns).  The
 * order in which the CAD tools assign logical config-bit indices does not in
 * general match the physical WL/BL address layout of the array.  This class
 * parses a "bitstream remap" XML file that describes the mapping and exposes
 * efficient query functions to translate between the two orderings.
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
 *   order and grouped into columns: consecutive tiles are accumulated into a
 *   column until their combined WL count equals W, at which point the next
 *   column begins.  All tiles in a column share the same BL lines.
 *
 * LOGICAL LAYOUT
 * --------------
 * After parsing, each tile instance is assigned a (column_id, column_y)
 * location (tile_location):
 *
 *   column_id (x): 0 = leftmost column, increments per column
 *   column_y  (y): 0 = first (topmost) tile in the column
 *
 * Within a region, the WL address space is divided vertically among tile rows
 * within each column, and the BL address space is divided horizontally among
 * columns.  The global WL index of a bit = (WL start of region) +
 * (WL start of tile within its column) + (local WL within tile).  The global
 * BL index = (BL start of column within region) + (local BL within tile).
 *
 * QUERY
 * -----
 * BitstreamReorderBitId is a flat index into the ordered sequence of all
 * WL×BL intersections, enumerated tile by tile in XML order.  It is neither
 * in logical config-bit space nor in physical (wl, bl) space — it is the
 * tile-order enumeration that the remap file implicitly defines.
 *
 *   get_reorder_bit_id_info(BitstreamReorderBitId)
 *     Given such a tile-order index, returns simultaneously the logical
 *     ConfigBitId and the physical global (wl, bl) address.  Internally
 *     calls get_tile_bit_info → get_bl_from_index / get_wl_from_index.
 *
 *   Callers iterate ibit from 0 to get_total_intersections(), call
 *   get_reorder_bit_id_info(BitstreamReorderBitId(ibit)), and scatter the
 *   bit value into a wl_rows[wl_index][bl_index] output grid.
 *
 * PERFORMANCE
 * -----------
 * All query functions run in O(1) or O(log n_blocks) time after a one-time
 * O(n_blocks) init cost in build_lookup_tables().  No per-bit or per-
 * intersection arrays are allocated; memory overhead is proportional to the
 * number of tile instances (~80 bytes/tile), not to the number of bits.
 *
 *   get_tile_bit_info                O(log n_blocks) — binary search
 *   get_bl_from_index                O(1)            — direct table lookup
 *   get_wl_from_index                O(1)            — direct table lookup
 *   region_bl_wl_intersection_range  O(1)            — prefix sum
 *   get_total_intersections          O(n_regions)    — small constant in
 *practice
 *
 ******************************************************************************/

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "bitstream_manager_fwd.h"
#include "vtr_strong_id.h"
#include "vtr_vector.h"

namespace openfpga {

struct bitstream_reorder_region_id_tag;
struct bitstream_reorder_region_block_id_tag;
struct bitstream_reorder_tile_bit_id_tag;
struct bitstream_reorder_bit_id_tag;

// Region ID for regions defined in the reorder map file
typedef vtr::StrongId<bitstream_reorder_region_id_tag> BitstreamReorderRegionId;

// Block ID for tiles defined under a region tag in the reorder map
typedef vtr::StrongId<bitstream_reorder_region_block_id_tag>
  BitstreamReorderRegionBlockId;

// Reordered Config bit ID at the tile level (defined under tile_bitmap)
typedef vtr::StrongId<bitstream_reorder_tile_bit_id_tag>
  BitstreamReorderTileBitId;

// Reordered Config bit ID globally (unique across all tiles)
typedef vtr::StrongId<bitstream_reorder_bit_id_tag> BitstreamReorderBitId;

// Result of a full bit lookup: given a BitstreamReorderBitId, this provides
// the corresponding config bit and its physical address in the memory array.
struct reorder_bit_id_info {
  ConfigBitId config_bit_id = ConfigBitId::INVALID();
  size_t bl_index = size_t(-1);  // global bitline index
  size_t wl_index = size_t(-1);  // global wordline index
};

// Intermediate result of resolving a BitstreamReorderBitId to its tile-level
// context, used internally before computing the final BL/WL addresses.
struct bitstream_reorder_tile_bit_info {
  BitstreamReorderRegionId region_id = BitstreamReorderRegionId::INVALID();
  BitstreamReorderRegionBlockId block_id =
    BitstreamReorderRegionBlockId::INVALID();
  // Flat WL×BL index within the tile: wl_row * num_bls + bl_col
  BitstreamReorderTileBitId tile_bit_id = BitstreamReorderTileBitId::INVALID();
  // Total WL×BL intersections across all tiles before this one (global offset)
  size_t intersection_num_offset = size_t(-1);
  // Total config bits across all tiles before this one (global offset)
  size_t cbit_num_offset = size_t(-1);
};

// Position of a tile within its region, using a column-major logical layout:
//   x = column_id  (0 = leftmost column)
//   y = column_y   (0 = first/top tile within the column)
// Columns are formed by grouping consecutive tiles (in XML order) until their
// combined WL count equals the region's num_wls.
struct tile_location {
  tile_location() : x(-1), y(-1) {}
  tile_location(int x_, int y_) : x(x_), y(y_) {}
  int x;  // column_id
  int y;  // position within the column
};

// Stores the layout and dimensions of one configuration region.
// A region spans all columns of the device horizontally and num_wls rows
// vertically. Tiles are grouped into columns; all tiles in a column share
// the same BL lines.
struct bistream_reorder_region {
  vtr::vector<BitstreamReorderRegionBlockId, std::string> tile_types;
  vtr::vector<BitstreamReorderRegionBlockId, tile_location> tile_locations;
  size_t num_cbits;  // total config bits in the region (sum over all tiles)
  size_t num_wls;    // wordlines in the region (from XML attribute)
  size_t num_bls;    // bitlines in the region (sum of first-row tile BLs)
};

// Per-tile-type mapping from flat WL×BL positions to config bit indices.
// bit_map[wl_row * num_bls + bl_col] = config bit index within the tile,
// or INVALID if that WL×BL intersection carries no config bit.
struct tile_bit_map {
  vtr::vector<BitstreamReorderTileBitId, ConfigBitId> bit_map;
  size_t num_cbits;  // number of config bits in this tile type
  size_t num_bls;    // number of bitlines
  size_t num_wls;    // number of wordlines
};

// Precomputed per-block offsets for O(1) / O(log n) queries.
// Populated by BitstreamReorderMap::build_lookup_tables() during init.
struct block_precomputed_info {
  size_t
    intersection_offset;  // global flat intersection index before this block
  size_t cbit_offset;     // global config-bit index before this block
  size_t global_wl_base;  // absolute WL index of this block's first row
  size_t global_bl_base;  // absolute BL index of this block's first column
  size_t num_bls;         // cached tile_bit_maps[name].num_bls
  size_t num_wls;         // cached tile_bit_maps[name].num_wls
};

// Entry in the globally-sorted block-range table, used for binary search in
// get_tile_bit_info().  Sorted by intersection_start (ascending).
struct block_range_index {
  size_t intersection_start;
  BitstreamReorderRegionId region_id;
  BitstreamReorderRegionBlockId block_id;
};

class BitstreamReorderMap {
 public:
  BitstreamReorderMap();
  BitstreamReorderMap(const std::string& reorder_map_file);
  ~BitstreamReorderMap();

  /**
   * @brief Initialize the bitstream reorder map from a file
   * @param reorder_map_file The path to the bitstream reorder map file
   */
  void init_from_file(const std::string& reorder_map_file);

  /**
   * @brief Get the index range of bl/wl intersection in the given region
   * @param region_id The id of the region
   *
   * @return A pair of size_t, the first is the starting index (inclusive), the
   * second is the ending index (exclusive)
   */
  std::pair<size_t, size_t> region_bl_wl_intersection_range(
    const BitstreamReorderRegionId& region_id) const;

  /**
   * @brief Get the number of configuration bits in a region
   * @param region_id The id of the region
   */
  size_t num_config_bits(const BitstreamReorderRegionId& region_id) const;

  /**
   * @brief Get the size of the bitline address
   */
  size_t get_bl_address_size() const;

  /**
   * @brief Get the size of the wordline address
   */
  size_t get_wl_address_size() const;

  /**
   * @brief Get the tile name of a block
   * @param region_id The id of the region
   * @param block_id The id of the tile
   */
  std::string get_block_tile_name(
    const BitstreamReorderRegionId& region_id,
    const BitstreamReorderRegionBlockId& block_id) const;

  /**
   * @brief Get the tile info from the bit index
   * @param bit_id The bit id
   */
  bitstream_reorder_tile_bit_info get_tile_bit_info(
    const BitstreamReorderBitId& bit_id) const;

  /**
   * @brief Get the config bit number from the bit index
   * @param bit_id The bit id
   */
  reorder_bit_id_info get_reorder_bit_id_info(
    const BitstreamReorderBitId& bit_id) const;

  /**
   * @brief Get the total number of WL×BL intersections across all regions
   */
  size_t get_total_intersections() const;

 private:
  /**
   * @brief Get the bitline number in the region
   * @param region_id The id of the region
   * @param block_id The id of the tile
   * @param bit_id The bit id
   */
  size_t get_bl_from_index(const BitstreamReorderRegionId& region_id,
                           const BitstreamReorderRegionBlockId& block_id,
                           const BitstreamReorderTileBitId& bit_id) const;

  /**
   * @brief Get the wordline number in the region
   * @param region_id The id of the region
   * @param block_id The id of the tile
   * @param bit_id The bit id
   */
  size_t get_wl_from_index(const BitstreamReorderRegionId& region_id,
                           const BitstreamReorderRegionBlockId& block_id,
                           const BitstreamReorderTileBitId& bit_id) const;

  /**
   * @brief Precompute all per-block offset tables used by the query functions.
   * Must be called once after init_tile_bit_maps() and init_regions().
   */
  void build_lookup_tables();

  vtr::vector<BitstreamReorderRegionId, bistream_reorder_region> regions;
  std::unordered_map<std::string, tile_bit_map> tile_bit_maps;

  // --- Precomputed lookup tables (built by build_lookup_tables()) ---

  // Per-block precomputed info, indexed [region_id][block_id].
  vtr::vector<
    BitstreamReorderRegionId,
    vtr::vector<BitstreamReorderRegionBlockId, block_precomputed_info>>
    block_info_;

  // region_intersection_prefix_[r] = global flat intersection offset for
  // region r (i.e., sum of num_wls*num_bls for all blocks in all regions
  // before r).  Used for O(1) in region_bl_wl_intersection_range().
  vtr::vector<BitstreamReorderRegionId, size_t> region_intersection_prefix_;

  // Globally sorted list of block start-intersection indices.
  // Used for O(log n) binary search in get_tile_bit_info().
  std::vector<block_range_index> sorted_block_ranges_;
};

}  // namespace openfpga
