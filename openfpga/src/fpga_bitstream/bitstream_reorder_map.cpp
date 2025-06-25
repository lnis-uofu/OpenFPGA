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

// Define a hash function for pair<size_t, size_t>
struct pair_hash {
    std::size_t operator()(const std::pair<size_t, size_t>& p) const {
        // Combine the two hash values
        return std::hash<size_t>{}(p.first) ^ (std::hash<size_t>{}(p.second) << 1);
    }
};

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

static void init_regions(const pugi::xml_node& xml_root, 
                         const std::unordered_map<std::string, tile_bit_map>& tile_bit_maps, 
                         vtr::vector<BitstreamReorderRegionId, bistream_reorder_region>& regions) {
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
            num_cbits += tile_bit_maps.at(tile_name).num_cbits;

            
            size_t tile_num_bls = tile_bit_maps.at(tile_name).num_bls;
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
}

// This function take the region id and the BL index as inputs and return the
// column index (x coordinate) corresponding to the BL index. Note that each column
// may have multiple BLs, and this is why this function is required.
static int get_region_x_from_bl_index(const vtr::vector<BitstreamReorderRegionId, bistream_reorder_region>& regions,
                                      const std::unordered_map<std::string, tile_bit_map>& tile_bit_maps,
                                      const BitstreamReorderRegionId& region_id, 
                                      const size_t bl_index) {
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
}

// This function take the WL and BL index as inputs and return the block id
// where bl and wl intersection falls. Note that the BL and WL index are global
// across all regions.
static BitstreamReorderRegionBlockId get_block_id_from_wl_bl(const vtr::vector<BitstreamReorderRegionId, bistream_reorder_region>& regions,
                                                            const std::unordered_map<std::string, tile_bit_map>& tile_bit_maps,
                                                            const size_t bl,
                                                            const size_t wl) {
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
    int block_region_x = get_region_x_from_bl_index(regions, 
                                                    tile_bit_maps, 
                                                    found_region_id, 
                                                    bl);
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
    init_regions(xml_root, tile_bit_maps, regions);
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

    // To get the global BL address size, we need to find the maximum number of BLs
    // across all regions since regions are streched across device width.
    for (const auto& region: regions) {
        bl_address_size = std::max(bl_address_size, region.num_bls);
    }

    return bl_address_size;
}

size_t BitstreamReorderMap::get_wl_address_size() const {
    size_t wl_address_size = 0;

    // To get the global WL address size, we need to sum up the number of WLs
    // across all regions.
    for (const auto& region: regions) {
        wl_address_size += region.num_wls;
    }

    return wl_address_size;
}

std::string BitstreamReorderMap::get_block_tile_name(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id) const {
    return regions[region_id].tile_types[block_id];
}

bitstream_reorder_tile_bit_info BitstreamReorderMap::get_tile_bit_info(const BitstreamReorderBitId& bit_id) const {
    size_t target_bit_index = static_cast<size_t>(bit_id);

    bool found_tile = false;
    BitstreamReorderRegionId found_region_id;
    BitstreamReorderRegionBlockId found_block_id;
    BitstreamReorderTileBitId found_tile_bit_id;

    size_t num_seen_intersections = 0;
    for (const auto& region_id: regions.keys()) {
        for (const auto& block_region_id: regions.at(region_id).tile_types.keys()) {
            const auto& tile_bit_map = tile_bit_maps.at(regions.at(region_id).tile_types.at(block_region_id));
            size_t block_num_intersections = tile_bit_map.num_wls * tile_bit_map.num_bls;
            if (target_bit_index >= num_seen_intersections && target_bit_index < num_seen_intersections + block_num_intersections) {
                found_tile = true;
                found_region_id = region_id;
                found_block_id = block_region_id;
                found_tile_bit_id = BitstreamReorderTileBitId(target_bit_index - num_seen_intersections);
                break;
            }
            num_seen_intersections += block_num_intersections;
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

        if (curr_tile_location.x < target_tile_location.x && curr_tile_location.y == 0) {
            num_seen_bls += tile_bit_maps.at(region.tile_types[region_tile_id]).num_bls;
        }
    }

    // Calculate the local BL index within the current tile
    size_t tile_bit_index = size_t(bit_id);
    size_t tile_num_bls = tile_bit_maps.at(get_block_tile_name(region_id, block_id)).num_bls;
    size_t tile_bl_num = tile_bit_index % tile_num_bls;

    /* Return the final BL index */
    return num_seen_bls + tile_bl_num;
}

size_t BitstreamReorderMap::get_wl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderTileBitId& bit_id) const {
    const auto& target_region = regions[region_id];

    /*
    * To find the wordline (WL) corresponding to a given bit_id within a specific region and block:
    * 1. Calculate the local WL index within the current tile.
    * 2. Calculate the number of WLs in the tiles before the target tile (in the same column as the target tile)
    * 3. Add the offset to the local WL to obtain the final WL index.
    */
    size_t num_seen_wls = 0;
    auto target_tile_location = target_region.tile_locations.at(block_id);

    for (const auto& region_tile_id : regions.keys()) {
        if (region_id == region_tile_id) {
            break;
        }
        num_seen_wls += regions[region_tile_id].num_wls;
    }

    // Calculate the number of WLs in the tiles before the target tile
    for (const auto& region_block_id : target_region.tile_types.keys()) {
        auto curr_tile_location = target_region.tile_locations.at(region_block_id);

        if (curr_tile_location.x == target_tile_location.x && curr_tile_location.y < target_tile_location.y) {
            num_seen_wls += tile_bit_maps.at(target_region.tile_types.at(region_block_id)).num_wls;
        }
    }

    // Calculate the local WL index within the current tile 
    size_t tile_bit_index = size_t(bit_id);
    size_t tile_num_bls = tile_bit_maps.at(get_block_tile_name(region_id, block_id)).num_bls;
    size_t tile_wl_num = tile_bit_index / tile_num_bls;

    /* Return the final WL index */
    return num_seen_wls + tile_wl_num;
}

ConfigBitId BitstreamReorderMap::get_config_bit_num(const BitstreamReorderBitId& bit_id) const {
    size_t target_bit_index = static_cast<size_t>(bit_id);
    size_t num_seen_intersections = 0;
    size_t num_seen_cbits = 0;

    std::string target_tile_name;
    BitstreamReorderRegionId target_region_id = BitstreamReorderRegionId::INVALID();
    BitstreamReorderRegionBlockId target_block_id = BitstreamReorderRegionBlockId::INVALID();

    for (const auto& region_id: regions.keys()) {
        for (const auto& block_id: regions[region_id].tile_types.keys()) {
            std::string tile_name = regions[region_id].tile_types[block_id];
            const auto& tile_bit_map = tile_bit_maps.at(tile_name);
            size_t block_num_intersections = tile_bit_map.num_wls * tile_bit_map.num_bls;
            size_t block_num_cbits = tile_bit_map.num_cbits;
            if (target_bit_index >= num_seen_intersections && target_bit_index < num_seen_intersections + block_num_intersections) {
                target_tile_name = tile_name;
                target_region_id = region_id;
                target_block_id = block_id;
                break;
            }
            num_seen_intersections += block_num_intersections;
            num_seen_cbits += block_num_cbits;
        }
        if (target_region_id.is_valid()) {
            break;
        }
    }
    VTR_ASSERT(target_tile_name.empty() == false);
    VTR_ASSERT(target_region_id.is_valid());
    VTR_ASSERT(target_block_id.is_valid());
    VTR_ASSERT(target_bit_index >= num_seen_intersections);
    BitstreamReorderTileBitId tile_bit_id = BitstreamReorderTileBitId(target_bit_index - num_seen_intersections);
    const auto& target_tile_bit_map = tile_bit_maps.at(target_tile_name);
    auto target_cbit_offset = target_tile_bit_map.bit_map.at(tile_bit_id);
    if (!target_cbit_offset.is_valid()) {
        return ConfigBitId::INVALID();
    }
    return ConfigBitId(num_seen_cbits + static_cast<size_t>(target_cbit_offset));
}

size_t BitstreamReorderMap::get_bl_from_index(const BitstreamReorderBitId& bit_id) const {
    bitstream_reorder_tile_bit_info tile_info = get_tile_bit_info(bit_id);

    return get_bl_from_index(tile_info.region_id, tile_info.block_id, tile_info.tile_bit_id);
}

size_t BitstreamReorderMap::get_wl_from_index(const BitstreamReorderBitId& bit_id) const {
    bitstream_reorder_tile_bit_info tile_info = get_tile_bit_info(bit_id);

    return get_wl_from_index(tile_info.region_id, tile_info.block_id, tile_info.tile_bit_id);
}

BitstreamReorderBitId BitstreamReorderMap::get_reordered_id_from_wl_bl(const size_t& wl_index, const size_t& bl_index) const {
    BitstreamReorderRegionId target_region_id = BitstreamReorderRegionId::INVALID();

    size_t num_seen_wls = 0;
    for (const auto& region_id: regions.keys()) {
        if (wl_index >= num_seen_wls && wl_index < num_seen_wls + regions[region_id].num_wls) {
            target_region_id = region_id;
            break;
        }
        num_seen_wls += regions[region_id].num_wls;
    }
    VTR_ASSERT(target_region_id.is_valid());

    auto target_block_id = get_block_id_from_wl_bl(regions, tile_bit_maps, bl_index, wl_index);
    VTR_ASSERT(target_block_id.is_valid());

    int target_x = get_region_x_from_bl_index(regions, tile_bit_maps, target_region_id, bl_index);

    size_t num_seen_bls = 0;
    for (const auto& block_id: regions[target_region_id].tile_types.keys()) {
        if (regions[target_region_id].tile_locations.at(block_id).x == target_x) {
            break;
        }
        const std::string& tile_type = regions[target_region_id].tile_types.at(block_id);
        if (regions[target_region_id].tile_locations.at(block_id).y == 0) {
            num_seen_bls += tile_bit_maps.at(tile_type).num_bls;
        }
    }

    for (const auto& block_id: regions[target_region_id].tile_types.keys()) {
        if (regions[target_region_id].tile_locations.at(block_id).x == target_x) {
            if (block_id == target_block_id) {
                break;
            }
            num_seen_wls += tile_bit_maps.at(regions[target_region_id].tile_types.at(block_id)).num_wls;
        }
    }

    size_t intersection_num_offset = 0;
    for (const auto& region_id: regions.keys()) {
        for (const auto& block_region_id: regions.at(region_id).tile_types.keys()) {
            if (region_id == target_region_id && block_region_id == target_block_id) {
                break;
            }
            const auto& tile_bit_map = tile_bit_maps.at(regions.at(region_id).tile_types.at(block_region_id));
            size_t block_num_intersections = tile_bit_map.num_wls * tile_bit_map.num_bls;
            intersection_num_offset += block_num_intersections;
        }
        if (region_id == target_region_id) {
            break;
        }
    }

    const std::string& target_tile_type = regions.at(target_region_id).tile_types.at(target_block_id);
    VTR_ASSERT(bl_index >= num_seen_bls);
    VTR_ASSERT(wl_index >= num_seen_wls);
    size_t region_bl_index = bl_index - num_seen_bls;
    size_t region_wl_index = wl_index - num_seen_wls;
    VTR_ASSERT(region_wl_index < tile_bit_maps.at(target_tile_type).num_wls);
    VTR_ASSERT(region_bl_index < tile_bit_maps.at(target_tile_type).num_bls);

    return BitstreamReorderBitId(intersection_num_offset + region_wl_index*tile_bit_maps.at(target_tile_type).num_bls + region_bl_index);
}

} /* end namespace openfpga */