/******************************************************************************
 * This file includes member functions for data structure BitstreamReorderMap
 ******************************************************************************/

#include "bitstream_reorder_map.h"

/* Headers from standard C++ library */
#include <iostream>
#include <cmath>
#include <string>
#include <regex>
#include <algorithm>
#include <utility>
#include <unordered_set>

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

    pugi::xml_node xml_root = pugiutil::get_first_child(doc, "bitstream_remap", loc_data);

    /*
    * Store the information under tile_bitmap tags
    */
    for (pugi::xml_node xml_tile_bitmap : xml_root.children("tile_bitmap")) {
        std::string tile_name = xml_tile_bitmap.attribute("name").as_string();
        VTR_ASSERT(tile_bit_maps.find(tile_name) == tile_bit_maps.end());

        tile_bit_map& tile_bit_map = tile_bit_maps[tile_name];

        tile_bit_map.num_cbits = xml_tile_bitmap.attribute("cbits").as_uint();
        tile_bit_map.num_bls = xml_tile_bitmap.attribute("bl").as_uint();
        tile_bit_map.num_wls = xml_tile_bitmap.attribute("wl").as_uint();

        tile_bit_map.bit_map.resize(tile_bit_map.num_cbits);
        for (pugi::xml_node xml_bit : xml_tile_bitmap.children("bit")) {
            ConfigBitId config_bit_id = 
                ConfigBitId(static_cast<size_t>(xml_bit.attribute("index").as_int()-1));
            BitstreamReorderTileBitId bitstream_reorder_tile_bit_id = 
                BitstreamReorderTileBitId(static_cast<size_t>(xml_bit.text().as_int()));
            tile_bit_map.bit_map[bitstream_reorder_tile_bit_id] = config_bit_id;
        }
    }

    /*
    * Store the information under region tags
    */
    int region_id = 0;
    for (pugi::xml_node xml_region : xml_root.children("region")) {
        VTR_ASSERT(xml_region.attribute("id").as_int() == region_id);

        regions.emplace_back();
        bistream_reorder_region& region = regions.back();
            
        int tile_id = 0;
        size_t num_cbits = 0;
        size_t num_wls = 0;
        size_t num_bls = 0;
        int first_tile_x = -1;
        int first_tile_y = -1;
        for (pugi::xml_node xml_tile : xml_region.children("tile")) {
            VTR_ASSERT(xml_tile.attribute("id").as_int() == tile_id);
            std::string tile_name = xml_tile.attribute("name").as_string();
            std::string tile_alias = xml_tile.attribute("alias").as_string();
            auto [tile_x, tile_y] = extract_tile_indices(tile_alias);

            if (tile_id == 0) {
                first_tile_x = tile_x;
                first_tile_y = tile_y;
            }

            region.tile_types.emplace_back(tile_name);
            region.tile_aliases.emplace_back(tile_alias);
            region.tile_locations.emplace_back(tile_x - first_tile_x, tile_y - first_tile_y);
            num_cbits += tile_bit_maps[tile_name].num_cbits;

            
            size_t tile_num_wls = tile_bit_maps[tile_name].num_wls;
            size_t tile_num_bls = tile_bit_maps[tile_name].num_bls;
            num_wls += tile_num_wls;
            if (tile_y - first_tile_y == 0) {
                num_bls += tile_num_bls;
            }

            tile_id++;
        }
        
        region.num_cbits = num_cbits;
        region.num_wls = num_wls;
        region.num_bls = num_bls;
        region_id++;
    }

    auto get_region_x_from_bl_index = [&](BitstreamReorderRegionId region_id, size_t bl_index) -> int {
        size_t num_seen_bls = 0;
        for (BitstreamReorderRegionBlockId block_id: regions[region_id].tile_types.keys()) {
            const std::string& tile_name = regions[region_id].tile_types.at(block_id);
            if (regions[region_id].tile_locations.at(block_id).y != 0) {
                continue;
            }

            if (bl_index >= num_seen_bls && bl_index < num_seen_bls + tile_bit_maps.at(tile_name).num_bls) {
                return regions[region_id].tile_locations.at(block_id).x;
            }

            num_seen_bls += tile_bit_maps.at(tile_name).num_bls;
        }
    };

    auto get_block_id_from_wl_bl = [&](size_t bl, size_t wl) -> BitstreamReorderRegionBlockId {
        size_t globla_num_seen_wls = 0;
        BitstreamReorderRegionId found_region_id;
        for (BitstreamReorderRegionId region_id: regions.keys()) {
            if (wl >= globla_num_seen_wls && wl < globla_num_seen_wls + regions[region_id].num_wls) {
                found_region_id = region_id;
                break;
            }
            globla_num_seen_wls += regions[region_id].num_wls;
        }

        VTR_ASSERT(found_region_id);

        for (size_t bl_index = 0; bl_index < regions[found_region_id].num_bls; bl_index++) {
            int block_region_x = get_region_x_from_bl_index(found_region_id, bl_index);
            size_t region_num_seen_wls = globla_num_seen_wls;
            for (BitstreamReorderRegionBlockId block_id: regions[found_region_id].tile_types.keys()) {
                if (regions[found_region_id].tile_locations.at(block_id).x == block_region_x) {
                    const auto& tile_bit_map = tile_bit_maps.at(regions[found_region_id].tile_types.at(block_id));
                    if (wl >= (region_num_seen_wls) && wl < (region_num_seen_wls + tile_bit_map.num_wls)) {
                        return block_id;
                    }

                    region_num_seen_wls += tile_bit_map.num_wls;
                }
            }
        }

    };
    
    size_t num_seen_intersections = 0;
    for (auto& region: regions) {
        for (size_t bl_index = 0; bl_index < region.num_bls; bl_index++) {
            for (size_t wl_index = 0; wl_index < region.num_wls; /*wl_index is increased in the loop*/) {
                BitstreamReorderRegionBlockId curr_block_id = get_block_id_from_wl_bl(bl_index, wl_index);
                VTR_ASSERT(curr_block_id);
                
                const auto& tile_bit_map = tile_bit_maps.at(region.tile_types[curr_block_id]);
                region.tile_intersection_index_map.at(curr_block_id).emplace_back(num_seen_intersections, num_seen_intersections+tile_bit_map.num_wls-1);

                num_seen_intersections += tile_bit_map.num_wls;
                wl_index += tile_bit_map.num_wls;
            }
        }
    }
}

size_t BitstreamReorderMap::num_config_bits() const {
    size_t num_config_bits = 0;
    
    for (const auto& region: regions) {
        for (const auto& tile_type: region.tile_types) {
            num_config_bits += tile_bit_maps.at(tile_type).num_cbits;
        }
    }

    return num_config_bits;
}

size_t BitstreamReorderMap::get_bl_address_size() const {
    size_t bl_address_size = 0;

    for (const auto& region: regions) {
        bl_address_size = std::max(bl_address_size, region.num_bls);
    }

    return bl_address_size;
}

size_t BitstreamReorderMap::get_wl_address_size() const {
    size_t wl_address_size = 0;

    for (const auto& region: regions) {
        wl_address_size += region.num_wls;
    }

    return wl_address_size;
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

bitstream_reorder_tile_bit_info BitstreamReorderMap::get_tile_bit_info(const BitstreamReorderBitId& bit_id) const {
    size_t bit_index = static_cast<size_t>(bit_id);

    size_t num_seen_bits = 0;
    bool found_tile = false;
    BitstreamReorderRegionId found_region_id;
    BitstreamReorderRegionBlockId found_block_id;
    BitstreamReorderTileBitId found_tile_bit_id;

    for (const auto& region_id: regions.keys()) {
        for (const auto& block_region_id: regions.at(region_id).tile_types.keys()) {
            for (size_t pair_index = 0; pair_index < regions.at(region_id).tile_intersection_index_map.at(block_region_id).size(); pair_index++) {
                const auto& intersection_index = regions.at(region_id).tile_intersection_index_map.at(block_region_id).at(pair_index);
                if (bit_index >= intersection_index.first && bit_index <= intersection_index.second) {
                    found_tile = true;
                    found_region_id = region_id;
                    found_block_id = block_region_id;
                    size_t found_tile_bit_index_offset = 0;
                    for (size_t prev_pair_index = 0; prev_pair_index < pair_index; prev_pair_index++) {
                        const auto& prev_intersection_index = regions.at(region_id).tile_intersection_index_map.at(block_region_id).at(prev_pair_index);
                        found_tile_bit_index_offset += prev_intersection_index.second - prev_intersection_index.first + 1;
                    }
                    found_tile_bit_id = BitstreamReorderTileBitId(found_tile_bit_index_offset + bit_index - intersection_index.first);
                    break;
                }
            }

            if (found_tile) {
                break;
            }
        }

        if (found_tile) {
            break;
        }
    }

    VTR_ASSERT(found_tile);

    return {found_region_id, found_block_id, found_tile_bit_id};
}

ConfigBitId BitstreamReorderMap::get_config_bit_num(const std::string& tile_name, const BitstreamReorderTileBitId& bit_id) const {
    return tile_bit_maps.at(tile_name).bit_map.at(bit_id);
}

size_t BitstreamReorderMap::get_bl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderTileBitId& bit_id) const {
    const auto& region = regions[region_id];

    /*
    * To find the bitline (BL) corresponding to a given bit_id within a specific region and block:
    * 1. Determine how many tiles exist in the same row as the target block, but appear before it.
    * 2. Sum the number of BLs in those preceding tiles to compute the BL offset.
    * 3. Calculate the local BL index within the current tile.
    * 4. Add the offset to the local BL to obtain the final BL index.
    */
    size_t num_seen_bls = 0;
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
    size_t tile_cbit_num = size_t(bit_id);
    size_t tile_num_bls = tile_bit_maps.at(get_block_tile_name(region_id, block_id)).num_bls;
    size_t tile_bl_num = tile_cbit_num % tile_num_bls;

    /* Return the final BL index */
    return num_seen_bls + tile_bl_num;
}

size_t BitstreamReorderMap::get_wl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderTileBitId& bit_id) const {
    const auto& region = regions[region_id];

    /*
    * To find the wordline (WL) corresponding to a given bit_id within a specific region and block:
    * 1. Calculate the local WL index within the current tile.
    * 2. Calculate the number of WLs in the tiles before the target tile (in the same column as the target tile)
    * 3. Add the offset to the local WL to obtain the final WL index.
    */
    size_t num_seen_wls = 0;
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
    size_t tile_cbit_num = size_t(bit_id);
    size_t tile_num_bls = tile_bit_maps.at(get_block_tile_name(region_id, block_id)).num_bls;
    size_t tile_wl_num = tile_cbit_num / tile_num_bls;

    /* Return the final WL index */
    return num_seen_wls + tile_wl_num;
}

ConfigBitId BitstreamReorderMap::get_config_bit_num(const BitstreamReorderBitId& bit_id) const {
    bitstream_reorder_tile_bit_info tile_info = get_tile_bit_info(bit_id);
    const std::string& tile_name = get_block_tile_name(tile_info.region_id, tile_info.block_id);

    return get_config_bit_num(tile_name, tile_info.tile_bit_id);
}

size_t BitstreamReorderMap::get_bl_from_index(const BitstreamReorderBitId& bit_id) const {
    bitstream_reorder_tile_bit_info tile_info = get_tile_bit_info(bit_id);

    return get_bl_from_index(tile_info.region_id, tile_info.block_id, tile_info.tile_bit_id);
}

size_t BitstreamReorderMap::get_wl_from_index(const BitstreamReorderBitId& bit_id) const {
    bitstream_reorder_tile_bit_info tile_info = get_tile_bit_info(bit_id);

    return get_wl_from_index(tile_info.region_id, tile_info.block_id, tile_info.tile_bit_id);
}

} /* end namespace openfpga */