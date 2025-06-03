/******************************************************************************
 * This file includes member functions for data structure BitstreamReorderMap
 ******************************************************************************/

#include "bitstream_reorder_map.h"

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

} /* end namespace openfpga */