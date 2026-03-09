/******************************************************************************
 * This file includes member functions for data structure BitstreamReorderMap
 ******************************************************************************/

#include "bitstream_reorder_map.h"

/* Headers from standard C++ library */
#include <algorithm>
#include <string>
#include <unordered_map>
#include <utility>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"
#include "vtr_assert.h"

/* begin namespace openfpga */
namespace openfpga {

// Parses all <tile_bitmap> tags from the bitstream remap XML and builds a
// lookup table from tile name → tile_bit_map.
//
// Each tile_bitmap defines:
//   - cbits / bl / wl: dimensions of the tile's config bits and WL×BL grid
//   - A list of <bit index="I">J</bit> entries, meaning:
//       config bit index I maps to WL×BL position J
//     Stored as bit_map[J] = I (i.e., wlbl_index → config_bit_index).
static void init_tile_bit_maps(
  const pugi::xml_node& xml_root,
  std::unordered_map<std::string, tile_bit_map>& tile_bit_maps) {
  for (pugi::xml_node xml_tile_bitmap : xml_root.children("tile_bitmap")) {
    std::string tile_name = xml_tile_bitmap.attribute("name").as_string();
    VTR_ASSERT(tile_bit_maps.find(tile_name) == tile_bit_maps.end());

    tile_bit_map& tile_bit_map = tile_bit_maps[tile_name];

    tile_bit_map.num_cbits = xml_tile_bitmap.attribute("cbits").as_uint();
    tile_bit_map.num_bls = xml_tile_bitmap.attribute("bl").as_uint();
    tile_bit_map.num_wls = xml_tile_bitmap.attribute("wl").as_uint();

    // Pre-fill with INVALID so positions with no mapping are detected
    tile_bit_map.bit_map.resize(tile_bit_map.num_wls * tile_bit_map.num_bls,
                                ConfigBitId::INVALID());
    for (pugi::xml_node xml_bit : xml_tile_bitmap.children("bit")) {
      // index="I" → config bit index within the tile
      // text J    → flat WL×BL position (wl_row * num_bls + bl_col)
      ConfigBitId config_bit_id =
        ConfigBitId(static_cast<size_t>(xml_bit.attribute("index").as_int()));
      BitstreamReorderTileBitId wlbl_pos =
        BitstreamReorderTileBitId(static_cast<size_t>(xml_bit.text().as_int()));
      VTR_ASSERT(static_cast<size_t>(config_bit_id) < tile_bit_map.num_cbits);
      VTR_ASSERT(static_cast<size_t>(wlbl_pos) <
                 tile_bit_map.num_wls * tile_bit_map.num_bls);
      tile_bit_map.bit_map[wlbl_pos] = config_bit_id;
    }
  }
}

// Parses all <region> tags and populates the regions vector.
//
// Tiles within a region are grouped into columns by consuming them in XML
// order and accumulating their WL counts until the total equals region.num_wls.
// This mirrors the Python algorithm:
//
//   while tile is not None:
//     while len(region_bitstream_block) < region_wl:
//       process tile; tile = next(tiles)
//
// Each column gets a unique column_id (the x coordinate in tile_location).
// Tiles within a column are numbered by column_y (0 = first/top tile).
// This layout-agnostic grouping works regardless of how tiles are ordered in
// the XML (column-by-column or row-by-row).
static void init_regions(
  const pugi::xml_node& xml_root,
  const std::unordered_map<std::string, tile_bit_map>& tile_bit_maps,
  vtr::vector<BitstreamReorderRegionId, bistream_reorder_region>& regions) {
  int xml_region_id = 0;
  for (pugi::xml_node xml_region : xml_root.children("region")) {
    VTR_ASSERT(xml_region.attribute("id").as_int() == xml_region_id);

    regions.emplace_back();
    bistream_reorder_region& region = regions.back();
    region.num_wls = xml_region.attribute("wl").as_uint();

    int tile_id = 0;
    size_t num_cbits = 0;
    size_t num_bls = 0;
    int column_id = 0;
    int column_y = 0;
    size_t accumulated_wls_in_column = 0;

    for (pugi::xml_node xml_tile : xml_region.children("tile")) {
      VTR_ASSERT(xml_tile.attribute("id").as_int() == tile_id);
      std::string tile_name = xml_tile.attribute("name").as_string();

      region.tile_types.emplace_back(tile_name);
      // column_id = which column this tile belongs to (x)
      // column_y  = position within the column (y), 0 = first tile
      region.tile_locations.emplace_back(column_id, column_y);

      num_cbits += tile_bit_maps.at(tile_name).num_cbits;

      // BL lines are shared within a column, so count BLs only from the
      // first tile of each column (column_y == 0)
      if (column_y == 0) {
        num_bls += tile_bit_maps.at(tile_name).num_bls;
      }

      accumulated_wls_in_column += tile_bit_maps.at(tile_name).num_wls;
      VTR_ASSERT(accumulated_wls_in_column <= region.num_wls);
      if (accumulated_wls_in_column == region.num_wls) {
        // This tile completes the column — advance to the next one
        column_id++;
        column_y = 0;
        accumulated_wls_in_column = 0;
      } else {
        column_y++;
      }

      tile_id++;
    }

    region.num_cbits = num_cbits;
    region.num_bls = num_bls;
    xml_region_id++;
  }
}

// Builds all precomputed lookup tables for O(1)/O(log n) queries.
//
// Must be called after init_tile_bit_maps() and init_regions().
//
// For each tile (block) we precompute:
//   - intersection_offset: global flat intersection index before this block
//   - cbit_offset:         global config-bit index before this block
//   - global_wl_base:      absolute WL index of this block's first WL row
//   - global_bl_base:      absolute BL index of this block's first BL column
//   - num_bls / num_wls:   cached dimensions (avoids repeated unordered_map
//                          lookups in the hot query paths)
//
// We also build:
//   - sorted_block_ranges_: globally sorted by intersection_offset, for
//     O(log n) binary search in get_tile_bit_info().
//   - region_intersection_prefix_: global intersection start per region,
//     for O(1) in region_bl_wl_intersection_range().
void BitstreamReorderMap::build_lookup_tables() {
  const size_t n_regions = regions.size();

  block_info_.resize(n_regions);
  region_intersection_prefix_.resize(n_regions);

  size_t global_wl = 0;
  size_t global_intersections = 0;
  size_t global_cbits = 0;

  for (auto region_id : regions.keys()) {
    auto& region = regions[region_id];
    const size_t n_blocks = region.tile_types.size();

    region_intersection_prefix_[region_id] = global_intersections;
    block_info_[region_id].resize(n_blocks);

    // Pass 1 (y==0 tiles only): compute BL base for each column.
    // Columns are assigned x = 0, 1, 2, ... in XML order, so the first
    // y==0 tile encountered has x == col_bl_starts.size() each time.
    std::vector<std::pair<size_t, int>> col_bl_starts;
    size_t running_bl = 0;
    for (auto block_id : region.tile_types.keys()) {
      const auto& loc = region.tile_locations[block_id];
      if (loc.y == 0) {
        VTR_ASSERT(size_t(loc.x) == col_bl_starts.size());
        const auto& tbm = tile_bit_maps.at(region.tile_types[block_id]);
        col_bl_starts.emplace_back(running_bl, loc.x);
        running_bl += tbm.num_bls;
      }
    }

    // Pass 2: fill per-block info.
    const size_t n_cols = col_bl_starts.size();
    // col_wl_accumulated[x] tracks the cumulative WL count within column x
    // (relative to global_wl, i.e., the region's WL base).
    std::vector<size_t> col_wl_accumulated(n_cols, 0);

    for (auto block_id : region.tile_types.keys()) {
      const auto& loc = region.tile_locations[block_id];
      const auto& tbm = tile_bit_maps.at(region.tile_types[block_id]);

      auto& bpi = block_info_[region_id][block_id];
      bpi.intersection_offset = global_intersections;
      bpi.cbit_offset = global_cbits;
      bpi.global_bl_base = col_bl_starts[loc.x].first;
      bpi.global_wl_base = global_wl + col_wl_accumulated[loc.x];
      bpi.num_bls = tbm.num_bls;
      bpi.num_wls = tbm.num_wls;

      col_wl_accumulated[loc.x] += tbm.num_wls;

      sorted_block_ranges_.push_back(
        {global_intersections, region_id, block_id});

      global_intersections += tbm.num_wls * tbm.num_bls;
      global_cbits += tbm.num_cbits;
    }

    global_wl += region.num_wls;
  }

  // sorted_block_ranges_ is already in ascending intersection_offset order
  // because we process blocks in the canonical region×block order, but
  // assert it is sorted to catch any future reordering bugs.
  VTR_ASSERT(
    std::is_sorted(sorted_block_ranges_.begin(), sorted_block_ranges_.end(),
                   [](const block_range_index& a, const block_range_index& b) {
                     return a.intersection_start < b.intersection_start;
                   }));
}

BitstreamReorderMap::BitstreamReorderMap() {}

BitstreamReorderMap::BitstreamReorderMap(const std::string& reorder_map_file) {
  init_from_file(reorder_map_file);
}

BitstreamReorderMap::~BitstreamReorderMap() {}

void BitstreamReorderMap::init_from_file(const std::string& reorder_map_file) {
  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  loc_data = pugiutil::load_xml(doc, reorder_map_file);

  pugi::xml_node xml_root =
    pugiutil::get_first_child(doc, "bitstream_remap", loc_data);

  /*
   * Store the information under tile_bitmap tags
   */
  init_tile_bit_maps(xml_root, tile_bit_maps);

  /*
   * Store the information under region tags
   */
  init_regions(xml_root, tile_bit_maps, regions);

  /*
   * Precompute per-block offset tables for O(1)/O(log n) queries
   */
  build_lookup_tables();
}

// O(1): directly read from the precomputed prefix table.
std::pair<size_t, size_t> BitstreamReorderMap::region_bl_wl_intersection_range(
  const BitstreamReorderRegionId& region_id) const {
  size_t start = region_intersection_prefix_[region_id];
  return {start,
          start + regions[region_id].num_bls * regions[region_id].num_wls};
}

size_t BitstreamReorderMap::num_config_bits(
  const BitstreamReorderRegionId& region_id) const {
  return regions[region_id].num_cbits;
}

size_t BitstreamReorderMap::get_bl_address_size() const {
  size_t bl_address_size = 0;

  // To get the global BL address size, we need to find the maximum number of
  // BLs across all regions since regions are streched across device width.
  for (const auto& region : regions) {
    bl_address_size = std::max(bl_address_size, region.num_bls);
  }

  return bl_address_size;
}

size_t BitstreamReorderMap::get_wl_address_size() const {
  size_t wl_address_size = 0;

  // To get the global WL address size, we need to sum up the number of WLs
  // across all regions.
  for (const auto& region : regions) {
    wl_address_size += region.num_wls;
  }

  return wl_address_size;
}

std::string BitstreamReorderMap::get_block_tile_name(
  const BitstreamReorderRegionId& region_id,
  const BitstreamReorderRegionBlockId& block_id) const {
  return regions[region_id].tile_types[block_id];
}

// O(log n_blocks): binary search on sorted_block_ranges_.
bitstream_reorder_tile_bit_info BitstreamReorderMap::get_tile_bit_info(
  const BitstreamReorderBitId& bit_id) const {
  size_t target = static_cast<size_t>(bit_id);

  // upper_bound gives the first entry with intersection_start > target;
  // stepping back gives the last entry with intersection_start <= target.
  auto it =
    std::upper_bound(sorted_block_ranges_.begin(), sorted_block_ranges_.end(),
                     target, [](size_t val, const block_range_index& e) {
                       return val < e.intersection_start;
                     });
  VTR_ASSERT(it != sorted_block_ranges_.begin());
  --it;

  const auto& bpi = block_info_[it->region_id][it->block_id];
  return {it->region_id, it->block_id,
          BitstreamReorderTileBitId(target - it->intersection_start),
          it->intersection_start, bpi.cbit_offset};
}

// O(1): BL base is precomputed; local BL is bit_id % num_bls.
size_t BitstreamReorderMap::get_bl_from_index(
  const BitstreamReorderRegionId& region_id,
  const BitstreamReorderRegionBlockId& block_id,
  const BitstreamReorderTileBitId& bit_id) const {
  const auto& bpi = block_info_[region_id][block_id];
  size_t tile_bl = static_cast<size_t>(bit_id) % bpi.num_bls;
  return bpi.global_bl_base + tile_bl;
}

// O(1): WL base is precomputed; local WL is bit_id / num_bls.
size_t BitstreamReorderMap::get_wl_from_index(
  const BitstreamReorderRegionId& region_id,
  const BitstreamReorderRegionBlockId& block_id,
  const BitstreamReorderTileBitId& bit_id) const {
  const auto& bpi = block_info_[region_id][block_id];
  size_t tile_wl = static_cast<size_t>(bit_id) / bpi.num_bls;
  return bpi.global_wl_base + tile_wl;
}

reorder_bit_id_info BitstreamReorderMap::get_reorder_bit_id_info(
  const BitstreamReorderBitId& bit_id) const {
  reorder_bit_id_info reorder_bit_id_info;

  bitstream_reorder_tile_bit_info tile_bit_info = get_tile_bit_info(bit_id);
  std::string target_tile_name =
    regions[tile_bit_info.region_id].tile_types[tile_bit_info.block_id];

  const auto& target_tile_bit_map = tile_bit_maps.at(target_tile_name);
  auto target_cbit_offset =
    target_tile_bit_map.bit_map.at(tile_bit_info.tile_bit_id);
  if (!target_cbit_offset.is_valid()) {
    return reorder_bit_id_info;
  }
  size_t target_bl_index = get_bl_from_index(
    tile_bit_info.region_id, tile_bit_info.block_id, tile_bit_info.tile_bit_id);
  size_t target_wl_index = get_wl_from_index(
    tile_bit_info.region_id, tile_bit_info.block_id, tile_bit_info.tile_bit_id);

  reorder_bit_id_info.config_bit_id = ConfigBitId(
    tile_bit_info.cbit_num_offset + static_cast<size_t>(target_cbit_offset));
  reorder_bit_id_info.bl_index = target_bl_index;
  reorder_bit_id_info.wl_index = target_wl_index;
  return reorder_bit_id_info;
}

size_t BitstreamReorderMap::get_total_intersections() const {
  size_t total = 0;
  for (const auto& region : regions) {
    total += region.num_wls * region.num_bls;
  }
  return total;
}

} /* end namespace openfpga */
