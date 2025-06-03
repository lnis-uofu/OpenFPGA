/******************************************************************************
 * This file includes member functions for data structure BitstreamReorderMap
 ******************************************************************************/

#include "bitstream_reorder_map.h"

/* Headers from standard C++ library */
#include <cmath>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

#include "vtr_assert.h"

/* begin namespace openfpga */
namespace openfpga {

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

    pugi::xml_node xml_root = pugiutil::get_first_child(doc, "bitstream_reorder_map", loc_data);

    int region_id = 0;
    for (pugi::xml_node xml_region : xml_root.children("region")) {
        VTR_ASSERT(xml_region.attribute("id").as_int() == region_id);

        regions.emplace_back();
        bistream_reorder_region& region = regions.back();

        int tile_id = 0;
        for (pugi::xml_node xml_tile : xml_region.children("tile")) {
            VTR_ASSERT(xml_tile.attribute("id").as_int() == tile_id);

            region.tile_types.emplace_back(xml_tile.attribute("name").as_string());
            region.tile_aliases.emplace_back(xml_tile.attribute("alias").as_string());

            tile_id++;
        }
    }

    for (pugi::xml_node xml_tile_bitmap : xml_root.children("tile_bitmap")) {
        std::string tile_name = xml_tile_bitmap.attribute("name").as_string();
        VTR_ASSERT(tile_bit_maps.find(tile_name) == tile_bit_maps.end());

        tile_bit_map& tile_bit_map = tile_bit_maps[tile_name];

        int bit_id = 0;
        for (pugi::xml_node xml_bit : xml_tile_bitmap.children("bit")) {
            VTR_ASSERT(xml_bit.attribute("id").as_int() == bit_id);
            tile_bit_map.bit_map.emplace_back(xml_bit.text().as_int());
            bit_id++;
        }

        tile_bit_map.num_cbits = xml_tile_bitmap.attribute("cbits").as_int();
        tile_bit_map.num_bls = xml_tile_bitmap.attribute("bl").as_int();
        tile_bit_map.num_wls = xml_tile_bitmap.attribute("wl").as_int();
    }
}

int BitstreamReorderMap::get_bl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderBitId& bit_id) const {
    const auto& region = regions[region_id];

    bool found_tile = false;
    std::string tile_name;
    int num_seen_bls = 0;
    for (const auto& region_tile_id : region.tile_types.keys()) {
        tile_name = region.tile_types[region_tile_id];
        if (tile_name == region.tile_types[block_id]) {
            found_tile = true;
            break;
        }
        num_seen_bls += tile_bit_maps.at(tile_name).num_bls;
    }

    VTR_ASSERT(found_tile);
    int tile_cbit_num = tile_bit_maps.at(tile_name).bit_map.at(bit_id);
    int tile_num_bls = tile_bit_maps.at(tile_name).num_bls;
    int tile_bl_num = tile_cbit_num % tile_num_bls;
    int region_bl_num = num_seen_bls + tile_bl_num;

    return region_bl_num;
}

int BitstreamReorderMap::get_wl_from_index(const BitstreamReorderBitId& bit_id) const {
    
}

} /* end namespace openfpga */