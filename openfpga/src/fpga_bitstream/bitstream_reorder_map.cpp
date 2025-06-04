/******************************************************************************
 * This file includes member functions for data structure BitstreamReorderMap
 ******************************************************************************/

#include "bitstream_reorder_map.h"

/* Headers from standard C++ library */
#include <iostream>
#include <cmath>
#include <string>
#include <regex>
#include <utility>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

#include "vtr_assert.h"

/* begin namespace openfpga */
namespace openfpga {

static std::pair<int, int> extract_tile_indices(const std::string& name) {
    std::regex pattern(R"(tile_(\d+)__(\d+)_?)");
    std::smatch match;

    if (std::regex_match(name, match, pattern)) {
        int first = std::stoi(match[1]);
        int second = std::stoi(match[2]);
        return {first, second};
    } else {
        throw std::invalid_argument("Invalid format: " + name);
    }
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

    int tile_bit_offset = 0;
    for (auto& region: regions) {
        for (const auto& tile_type: region.tile_types) {
            region.tile_bit_offsets.emplace_back(tile_bit_offset);
            tile_bit_offset += tile_bit_maps.at(tile_type).num_cbits;
        }
    }
}

std::vector<BitstreamReorderRegionId> BitstreamReorderMap::get_regions() const {
    return std::vector<BitstreamReorderRegionId>(regions.keys().begin(), regions.keys().end());
}

std::vector<BitstreamReorderRegionBlockId> BitstreamReorderMap::get_region_blocks(const BitstreamReorderRegionId& region_id) const {
    return std::vector<BitstreamReorderRegionBlockId>(regions[region_id].tile_types.keys().begin(), regions[region_id].tile_types.keys().end());
}

std::string BitstreamReorderMap::get_block_alias(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id) const {
    return regions[region_id].tile_aliases[block_id];
}

std::string BitstreamReorderMap::get_block_tile_name(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id) const {
    return regions[region_id].tile_types[block_id];
}

ConfigBitId BitstreamReorderMap::get_config_bit_num(const std::string& tile_name, const BitstreamReorderBitId& bit_id) const {
    return tile_bit_maps.at(tile_name).bit_map.at(bit_id);
}

int BitstreamReorderMap::get_bl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderBitId& bit_id) const {
    const auto& region = regions[region_id];

    /*
    * To find the bitline (BL) corresponding to a given bit_id within a specific region and block:
    * 1. Determine how many tiles exist in the same row as the target block, but appear before it.
    * 2. Sum the number of BLs in those preceding tiles to compute the BL offset.
    * 3. Calculate the local BL index within the current tile.
    * 4. Add the offset to the local BL to obtain the final BL index.
    */
    int num_seen_bls = 0;
    std::string tile_alias_name = get_block_alias(region_id, block_id);
    auto [target_tile_x, target_tile_y] = extract_tile_indices(tile_alias_name);

    // Calculate the number of BLs in the tiles before the target tile
    for (const auto& region_tile_id : region.tile_types.keys()) {
        tile_alias_name = get_block_alias(region_id, region_tile_id);
        auto [curr_tile_x, curr_tile_y] = extract_tile_indices(tile_alias_name);

        if (curr_tile_x < target_tile_x && curr_tile_y == target_tile_y) {
            num_seen_bls += tile_bit_maps.at(region.tile_types[region_tile_id]).num_bls;
        }
    }

    // Calculate the local BL index within the current tile
    int tile_cbit_num = static_cast<int>(size_t(bit_id));
    int tile_num_bls = tile_bit_maps.at(get_block_tile_name(region_id, block_id)).num_bls;
    int tile_bl_num = tile_cbit_num % tile_num_bls;

    /* Return the final BL index */
    return num_seen_bls + tile_bl_num;
}

int BitstreamReorderMap::get_wl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderBitId& bit_id) const {
    const auto& region = regions[region_id];

    /*
    * To find the wordline (WL) corresponding to a given bit_id within a specific region and block:
    * 1. Calculate the local WL index within the current tile.
    * 2. Calculate the number of WLs in the tiles before the target tile (in the same column as the target tile)
    * 3. Add the offset to the local WL to obtain the final WL index.
    */
    int num_seen_wls = 0;
    std::string tile_alias_name = get_block_alias(region_id, block_id);
    auto [target_tile_x, target_tile_y] = extract_tile_indices(tile_alias_name);

    // Calculate the number of WLs in the tiles before the target tile
    for (const auto& region_tile_id : region.tile_types.keys()) {
        tile_alias_name = get_block_alias(region_id, region_tile_id);
        auto [curr_tile_x, curr_tile_y] = extract_tile_indices(tile_alias_name);

        if (curr_tile_x == target_tile_x && curr_tile_y < target_tile_y) {
            num_seen_wls += tile_bit_maps.at(region.tile_types[region_tile_id]).num_wls;
        }
    }

    // Calculate the local WL index within the current tile 
    int tile_cbit_num = static_cast<int>(size_t(bit_id));
    int tile_num_bls = tile_bit_maps.at(get_block_tile_name(region_id, block_id)).num_bls;
    int tile_wl_num = std::floor(static_cast<float>(tile_cbit_num) / tile_num_bls);

    /* Return the final WL index */
    return num_seen_wls + tile_wl_num;
}

} /* end namespace openfpga */