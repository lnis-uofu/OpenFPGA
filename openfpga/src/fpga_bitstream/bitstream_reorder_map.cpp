/******************************************************************************
 * This file includes member functions for data structure BitstreamReorderMap
 ******************************************************************************/

#include "bitstream_reorder_map.h"

#include <string>
#include <unordered_map>

#include "pugixml.hpp"
#include "pugixml_util.hpp"
#include "vtr_assert.h"

namespace openfpga {

// Parses all <tile_bitmap> tags from the bitstream remap XML and populates
// tile_bit_maps.
//
// Each tile_bitmap defines:
//   - cbits / bl / wl: dimensions of the tile's config bits and WL×BL grid
//   - <bit index="I">J</bit>: config bit I maps to flat WL×BL position J
//     Stored as bit_map[J] = I (wlbl_index → config_bit_index).
static void init_tile_bit_maps(
  const pugi::xml_node& xml_root,
  std::unordered_map<std::string, tile_bit_map>& tile_bit_maps) {
  for (pugi::xml_node xml_tb : xml_root.children("tile_bitmap")) {
    std::string tile_name = xml_tb.attribute("name").as_string();
    VTR_ASSERT(tile_bit_maps.find(tile_name) == tile_bit_maps.end());

    tile_bit_map& tbm = tile_bit_maps[tile_name];
    tbm.num_cbits = xml_tb.attribute("cbits").as_uint();
    tbm.num_bls = xml_tb.attribute("bl").as_uint();
    tbm.num_wls = xml_tb.attribute("wl").as_uint();

    // Pre-fill with INVALID so positions with no <bit> mapping are detectable
    tbm.bit_map.assign(tbm.num_wls * tbm.num_bls, ConfigBitId::INVALID());

    for (pugi::xml_node xml_bit : xml_tb.children("bit")) {
      // index="I" → config bit index within the tile
      // text J    → flat WL×BL position (wl_row * num_bls + bl_col)
      size_t config_bit_idx =
        static_cast<size_t>(xml_bit.attribute("index").as_int());
      size_t wlbl_pos = static_cast<size_t>(xml_bit.text().as_int());

      VTR_ASSERT(config_bit_idx < tbm.num_cbits);
      VTR_ASSERT(wlbl_pos < tbm.num_wls * tbm.num_bls);
      tbm.bit_map[wlbl_pos] = ConfigBitId(config_bit_idx);
    }
  }
}

// Parses all <region> tags and populates the regions vector.
//
// Tiles within a region are grouped into columns: consecutive tiles (in XML
// order) are accumulated until their combined WL count equals region.num_wls,
// at which point the column is complete and the next one starts.  BL lines
// are shared within a column, so only the first tile of each column
// contributes its BL count to the region's total.
static void init_regions(
  const pugi::xml_node& xml_root,
  const std::unordered_map<std::string, tile_bit_map>& tile_bit_maps,
  std::vector<reorder_region>& regions) {
  int xml_region_id = 0;
  for (pugi::xml_node xml_region : xml_root.children("region")) {
    VTR_ASSERT(xml_region.attribute("id").as_int() == xml_region_id++);

    reorder_region region;
    region.num_wls = xml_region.attribute("wl").as_uint();
    region.num_bls = 0;

    size_t col_wl_acc = 0;  // accumulated WLs in the current column
    int tile_id = 0;
    for (pugi::xml_node xml_tile : xml_region.children("tile")) {
      VTR_ASSERT(xml_tile.attribute("id").as_int() == tile_id++);

      std::string tile_name = xml_tile.attribute("name").as_string();
      region.tile_names.push_back(tile_name);

      const tile_bit_map& tbm = tile_bit_maps.at(tile_name);

      // Count BLs only from the first tile of each column (col_wl_acc == 0),
      // since all tiles in a column share the same BL lines.
      if (col_wl_acc == 0) {
        region.num_bls += tbm.num_bls;
      }

      col_wl_acc += tbm.num_wls;
      VTR_ASSERT(col_wl_acc <= region.num_wls);
      if (col_wl_acc == region.num_wls) {
        col_wl_acc = 0;  // column complete — start a new one
      }
    }

    regions.push_back(std::move(region));
  }
}

BitstreamReorderMap::BitstreamReorderMap(const std::string& reorder_map_file) {
  init_from_file(reorder_map_file);
}

BitstreamReorderMap::~BitstreamReorderMap() {}

void BitstreamReorderMap::init_from_file(const std::string& reorder_map_file) {
  pugi::xml_document doc;
  pugiutil::loc_data loc_data = pugiutil::load_xml(doc, reorder_map_file);
  pugi::xml_node xml_root =
    pugiutil::get_first_child(doc, "bitstream_remap", loc_data);

  init_tile_bit_maps(xml_root, tile_bit_maps_);
  init_regions(xml_root, tile_bit_maps_, regions_);
}

// BL address size = max region BLs (regions span the full device width, so
// the widest region determines the total number of BL lines).
size_t BitstreamReorderMap::get_bl_address_size() const {
  size_t max_bls = 0;
  for (const auto& region : regions_) {
    max_bls = std::max(max_bls, region.num_bls);
  }
  return max_bls;
}

// WL address size = sum of all region WL heights (regions stack vertically).
size_t BitstreamReorderMap::get_wl_address_size() const {
  size_t total_wls = 0;
  for (const auto& region : regions_) {
    total_wls += region.num_wls;
  }
  return total_wls;
}

// Iterate over all valid WL×BL intersections in tile order.
//
// For each region, tiles are consumed in XML order and grouped into columns:
// consecutive tiles are accumulated until their combined WL count equals
// region.num_wls.  Within a column, tiles stack vertically (col_wl_offset
// advances per tile).  Columns are placed side by side (col_bl_base advances
// per completed column).
//
// For each WL×BL position that carries a config bit, the global (wl, bl)
// address and the global config-bit ID are computed inline and passed to the
// callback.
void BitstreamReorderMap::for_each_bit(
  const std::function<void(size_t wl, size_t bl, size_t global_cbit_id)>&
    callback) const {
  // Total config bits across all tiles — used to bounds-check every emission
  size_t total_cbits = 0;
  for (const auto& region : regions_)
    for (const auto& name : region.tile_names)
      total_cbits += tile_bit_maps_.at(name).num_cbits;

  size_t global_wl_base = 0;      // WL start of the current region
  size_t global_cbit_offset = 0;  // global config-bit index before current tile

  for (const auto& region : regions_) {
    size_t col_bl_base = 0;    // BL start of the current column
    size_t col_wl_offset = 0;  // WL offset of this tile within its column
    size_t col_wl_acc = 0;     // WLs accumulated in the current column

    for (const auto& tile_name : region.tile_names) {
      const tile_bit_map& tbm = tile_bit_maps_.at(tile_name);

      size_t tile_wl_base = global_wl_base + col_wl_offset;
      size_t tile_bl_base = col_bl_base;

      // Scatter each valid WL×BL intersection to its global (wl, bl) address
      for (size_t wlbl_pos = 0; wlbl_pos < tbm.bit_map.size(); ++wlbl_pos) {
        if (!tbm.bit_map[wlbl_pos].is_valid()) continue;

        size_t wl_row = wlbl_pos / tbm.num_bls;
        size_t bl_col = wlbl_pos % tbm.num_bls;
        size_t global_cbit_id =
          global_cbit_offset + static_cast<size_t>(tbm.bit_map[wlbl_pos]);
        VTR_ASSERT(global_cbit_id < total_cbits);

        callback(tile_wl_base + wl_row, tile_bl_base + bl_col, global_cbit_id);
      }

      global_cbit_offset += tbm.num_cbits;
      col_wl_offset += tbm.num_wls;
      col_wl_acc += tbm.num_wls;

      if (col_wl_acc == region.num_wls) {
        // Column complete — advance BL base and reset column-local trackers
        col_bl_base += tbm.num_bls;
        col_wl_offset = 0;
        col_wl_acc = 0;
      }
    }

    global_wl_base += region.num_wls;
  }
}

}  // namespace openfpga
