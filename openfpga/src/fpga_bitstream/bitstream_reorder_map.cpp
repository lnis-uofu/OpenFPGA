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

static void init_tile_bit_maps(const pugi::xml_node& xml_root, std::unordered_map<std::string, tile_bit_map>& tile_bit_maps) {
    for (pugi::xml_node xml_tile_bitmap : xml_root.children("tile_bitmap")) {
        std::string tile_name = xml_tile_bitmap.attribute("name").as_string();
        VTR_ASSERT(tile_bit_maps.find(tile_name) == tile_bit_maps.end());

        tile_bit_map& tile_bit_map = tile_bit_maps[tile_name];

        tile_bit_map.num_cbits = xml_tile_bitmap.attribute("cbits").as_uint();
        VTR_ASSERT(tile_bit_map.num_cbits > 0);
        tile_bit_map.num_bls = xml_tile_bitmap.attribute("bl").as_uint();
        VTR_ASSERT(tile_bit_map.num_bls > 0);
        tile_bit_map.num_wls = xml_tile_bitmap.attribute("wl").as_uint();
        VTR_ASSERT(tile_bit_map.num_wls > 0);

        tile_bit_map.bit_map.resize(tile_bit_map.num_wls * tile_bit_map.num_bls, ConfigBitId::INVALID());
        for (pugi::xml_node xml_bit : xml_tile_bitmap.children("bit")) {
            ConfigBitId config_bit_id = 
                ConfigBitId(static_cast<size_t>(xml_bit.attribute("index").as_int()));
            BitstreamReorderTileBitId bitstream_reorder_tile_bit_id = 
                BitstreamReorderTileBitId(static_cast<size_t>(xml_bit.text().as_int()));
            // Config bit id should not exceed the number of C bits in the tile
            VTR_ASSERT(static_cast<size_t>(config_bit_id) < tile_bit_map.num_cbits);
            // The reordered bit id should not exceed the number of bits (intersections of WL and BL) in the tile
            VTR_ASSERT(static_cast<size_t>(bitstream_reorder_tile_bit_id) < tile_bit_map.num_wls * tile_bit_map.num_bls);
            tile_bit_map.bit_map[bitstream_reorder_tile_bit_id] = config_bit_id;
        }
    }
}

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
    init_tile_bit_maps(xml_root, tile_bit_maps);

    /*
    * Store the information under region tags
    */
    int xml_region_id = 0;
    for (pugi::xml_node xml_region : xml_root.children("region")) {
        VTR_ASSERT(xml_region.attribute("id").as_int() == xml_region_id);

        regions.emplace_back();
        bistream_reorder_region& region = regions.back();
            
        int tile_id = 0;
        size_t num_cbits = 0;
        size_t num_bls = 0;
        int prev_tile_x = -1;
        int compressed_tile_x = 0;
        int compressed_tile_y = 0;
        for (pugi::xml_node xml_tile : xml_region.children("tile")) {
            VTR_ASSERT(xml_tile.attribute("id").as_int() == tile_id);
            std::string tile_name = xml_tile.attribute("name").as_string();
            std::string tile_alias = xml_tile.attribute("alias").as_string();
            auto [tile_x, tile_y] = extract_tile_indices(tile_alias);

            if (tile_id == 0) {
                prev_tile_x = tile_x;
            } else {
                if (prev_tile_x != tile_x) {
                    compressed_tile_x += 1;
                    compressed_tile_y = 0;
                } else {
                    compressed_tile_y += 1;
                }
            }

            region.tile_types.emplace_back(tile_name);
            region.tile_aliases.emplace_back(tile_alias);
            // Block location stored in alias is the blocks absolute location in grid (across multiple regions)
            // We convert it to relative location in the current region
            region.tile_locations.emplace_back(compressed_tile_x, compressed_tile_y);
            num_cbits += tile_bit_maps[tile_name].num_cbits;

            
            size_t tile_num_bls = tile_bit_maps[tile_name].num_bls;
            // To get the number BLs, we iterate over all tiles in the first row
            // of the region and sum up the number of BLs in each tile
            if (compressed_tile_y == 0) {
                num_bls += tile_num_bls;
            }

            prev_tile_x = tile_x;
            tile_id++;
        }
        
        region.num_cbits = num_cbits;
        region.num_wls = xml_region.attribute("wl").as_uint();
        region.num_bls = num_bls;
        xml_region_id++;
    }

    // This function take the region id and the BL index as inputs and return the
    // column index (x coordinate) corresponding to the BL index. Note that each column
    // may have multiple BLs, and this is why this function is required.
    auto get_region_x_from_bl_index = [&](BitstreamReorderRegionId region_id, size_t bl_index) -> int {
        size_t num_seen_bls = 0;
        for (BitstreamReorderRegionBlockId block_id: regions[region_id].tile_types.keys()) {
            const std::string& tile_name = regions[region_id].tile_types.at(block_id);
            // We only consider the first row of the region, since the BL lines are shared
            // across columns
            if (regions[region_id].tile_locations.at(block_id).y != 0) {
                continue;
            }

            if (bl_index >= num_seen_bls && bl_index < num_seen_bls + tile_bit_maps.at(tile_name).num_bls) {
                return regions[region_id].tile_locations.at(block_id).x;
            }

            num_seen_bls += tile_bit_maps.at(tile_name).num_bls;
        }
        return -1;
    };

    // This function take the WL and BL index as inputs and return the block id
    // where bl and wl intersection falls. Note that the BL and WL index are global
    // across all regions.
    auto get_block_id_from_wl_bl = [&](const size_t bl, const size_t wl) -> BitstreamReorderRegionBlockId {
        // Stores the wl offset for the region where the intersection falls
        size_t globla_num_seen_wls = 0;
        BitstreamReorderRegionId found_region_id;
        // Iterate over all region to find the region where the intersection falls
        for (BitstreamReorderRegionId region_id: regions.keys()) {
            if (wl >= globla_num_seen_wls && wl < globla_num_seen_wls + regions[region_id].num_wls) {
                found_region_id = region_id;
                break;
            }
            globla_num_seen_wls += regions[region_id].num_wls;
        }

        VTR_ASSERT(found_region_id);

        // Find the column index (x coordinate) of the BL in the region
        int block_region_x = get_region_x_from_bl_index(found_region_id, bl);
        VTR_ASSERT(block_region_x != -1);
        for (BitstreamReorderRegionBlockId block_id: regions[found_region_id].tile_types.keys()) {
            // Iterate over the tiles in the column to find the exact tile where the intersection falls
            if (regions[found_region_id].tile_locations.at(block_id).x == block_region_x) {
                const auto& tile_bit_map = tile_bit_maps.at(regions[found_region_id].tile_types.at(block_id));
                if (wl >= (globla_num_seen_wls) && wl < (globla_num_seen_wls + tile_bit_map.num_wls)) {
                    return block_id;
                }

                globla_num_seen_wls += tile_bit_map.num_wls;
            }
        }
        return BitstreamReorderRegionBlockId::INVALID();
    };
    
    // Keep track of the number of intersections seen so far
    size_t num_seen_intersections = 0;

    // Iterate over all region, and for each region, iterate over BLs first,
    // and for each BL, Iterate over tiles falling on the BL and store the indices
    // in the tile.
    size_t wl_index_offset = 0;
    for (auto& region: regions) {
        region.tile_intersection_index_map.resize(region.tile_types.size());
        for (size_t bl_index = 0; bl_index < region.num_bls; bl_index++) {
            for (size_t wl_index = wl_index_offset; wl_index < wl_index_offset + region.num_wls; /*wl_index is increased in the loop*/) {
                // Find the block id where the BL and WL intersection falls
                BitstreamReorderRegionBlockId curr_block_id = get_block_id_from_wl_bl(bl_index, wl_index);
                VTR_ASSERT(curr_block_id);
                
                const auto& tile_bit_map = tile_bit_maps.at(region.tile_types[curr_block_id]);
                // All intersestions from num_seen_intersections to num_seen_intersections+tile_bit_map.num_wls-1 belongs to this tile 
                region.tile_intersection_index_map.at(curr_block_id).emplace_back(num_seen_intersections, num_seen_intersections+tile_bit_map.num_wls);

                // Increment the offset
                num_seen_intersections += tile_bit_map.num_wls;
                // Increment the iterator to move to the next tile
                wl_index += tile_bit_map.num_wls;
            }
        }

        for (const auto& region_tile_id: region.tile_types.keys()) {
            const auto& tile_type = region.tile_types.at(region_tile_id);
            const auto& tile_bit_map = tile_bit_maps.at(tile_type);
            VTR_ASSERT(region.tile_intersection_index_map.at(region_tile_id).size() == tile_bit_map.num_bls);
        }

        wl_index_offset += region.num_wls;
    }
}

std::pair<size_t, size_t> BitstreamReorderMap::region_bl_wl_intersection_range(const BitstreamReorderRegionId& region_id) const {
    size_t starting_id = 0;

    for (const auto& prev_region_id: regions.keys()) {
        if (prev_region_id < region_id) {
            starting_id += regions[prev_region_id].num_bls * regions[prev_region_id].num_wls;
        }
    }

    return {starting_id, starting_id + regions[region_id].num_bls * regions[region_id].num_wls};
}

size_t BitstreamReorderMap::num_config_bits(const BitstreamReorderRegionId& region_id) const {
    return regions[region_id].num_cbits;
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

    bool found_tile = false;
    BitstreamReorderRegionId found_region_id;
    BitstreamReorderRegionBlockId found_block_id;
    BitstreamReorderTileBitId found_tile_bit_id;

    for (const auto& region_id: regions.keys()) {
        for (const auto& block_region_id: regions.at(region_id).tile_types.keys()) {
            for (size_t pair_index = 0; pair_index < regions.at(region_id).tile_intersection_index_map.at(block_region_id).size(); pair_index++) {
                const auto& intersection_index = regions.at(region_id).tile_intersection_index_map.at(block_region_id).at(pair_index);
                if (bit_index >= intersection_index.first && bit_index < intersection_index.second) {
                    const auto& tile_bit_map = tile_bit_maps.at(regions.at(region_id).tile_types.at(block_region_id));
                    found_tile = true;
                    found_region_id = region_id;
                    found_block_id = block_region_id;
                    size_t found_tile_bit_index_offset = 0;
                    for (size_t prev_pair_index = 0; prev_pair_index < pair_index; prev_pair_index++) {
                        const auto& prev_intersection_index = regions.at(region_id).tile_intersection_index_map.at(block_region_id).at(prev_pair_index);
                        found_tile_bit_index_offset += prev_intersection_index.second - prev_intersection_index.first;
                    }
                    found_tile_bit_id = BitstreamReorderTileBitId(found_tile_bit_index_offset + bit_index - intersection_index.first);
                    VTR_ASSERT(size_t(found_tile_bit_id) < tile_bit_map.num_wls * tile_bit_map.num_bls);
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
    auto target_tile_location = region.tile_locations.at(block_id);

    // Calculate the number of BLs in the tiles before the target tile
    for (const auto& region_tile_id : region.tile_types.keys()) {
        auto curr_tile_location = region.tile_locations.at(region_tile_id);

        if (curr_tile_location.x < target_tile_location.x && curr_tile_location.y == target_tile_location.y) {
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
    auto target_tile_location = region.tile_locations.at(block_id);

    // Calculate the number of WLs in the tiles before the target tile
    for (const auto& region_tile_id : region.tile_types.keys()) {
        auto curr_tile_location = region.tile_locations.at(region_tile_id);

        if (curr_tile_location.x == target_tile_location.x && curr_tile_location.y < target_tile_location.y) {
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