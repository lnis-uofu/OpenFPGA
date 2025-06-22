/******************************************************************************
 * This file introduces a data structure to store the reordering map for bitstream
 *
 ******************************************************************************/

#pragma once

#include <string>

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
typedef vtr::StrongId<bitstream_reorder_region_block_id_tag> BitstreamReorderRegionBlockId;

// Reordered Config bit ID at the tile level (defined under tile_bitmap)
typedef vtr::StrongId<bitstream_reorder_tile_bit_id_tag> BitstreamReorderTileBitId;

// Reordered Config bit ID globally (unique across all tiles)
typedef vtr::StrongId<bitstream_reorder_bit_id_tag> BitstreamReorderBitId;

struct bitstream_reorder_tile_bit_info {
    BitstreamReorderRegionId region_id;
    BitstreamReorderRegionBlockId block_id;
    BitstreamReorderTileBitId tile_bit_id;
};

struct tile_location {
    tile_location() : x(-1), y(-1) {}
    tile_location(int x_, int y_) : x(x_), y(y_) {}
    int x;
    int y;
};

struct bistream_reorder_region {
    vtr::vector<BitstreamReorderRegionBlockId, std::string> tile_types;
    vtr::vector<BitstreamReorderRegionBlockId, tile_location> tile_locations;
    vtr::vector<BitstreamReorderRegionBlockId, std::string> tile_aliases;
    size_t num_cbits;
    size_t num_wls;
    size_t num_bls;
};

struct tile_bit_map {
    vtr::vector<BitstreamReorderTileBitId, ConfigBitId> bit_map;
    size_t num_cbits;
    size_t num_bls;
    size_t num_wls;
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
     * @return A pair of size_t, the first is the starting index (inclusive), the second is the ending index (exclusive)
     */
    std::pair<size_t, size_t> region_bl_wl_intersection_range(const BitstreamReorderRegionId& region_id) const;

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
    std::string get_block_tile_name(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id) const;

    /**
     * @brief Get the tile info from the bit index
     * @param bit_id The bit id
     */
    bitstream_reorder_tile_bit_info get_tile_bit_info(const BitstreamReorderBitId& bit_id) const;

    /**
     * @brief Get the config bit number from the bit index
     * @param bit_id The bit id
     */
    ConfigBitId get_config_bit_num(const BitstreamReorderBitId& bit_id) const;

    /**
     * @brief Get the bitline number from the bit index
     * @param bit_id The bit id
     */
    size_t get_bl_from_index(const BitstreamReorderBitId& bit_id) const;

    /**
     * @brief Get the wordline number from the bit index
     * @param bit_id The bit id
     */
    size_t get_wl_from_index(const BitstreamReorderBitId& bit_id) const;

    /**
     * @brief Get the reordered bit id from the wordline and bitline index
     * @param wl_index The wordline index
     * @param bl_index The bitline index
     */
    BitstreamReorderBitId get_reordered_id_from_wl_bl(const size_t& wl_index, const size_t& bl_index) const;

private:

    /**
     * @brief Get the bitline number in the region
     * @param region_id The id of the region
     * @param block_id The id of the tile
     * @param bit_id The bit id
     */
    size_t get_bl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderTileBitId& bit_id) const;

    /**
     * @brief Get the wordline number in the region
     * @param region_id The id of the region
     * @param block_id The id of the tile
     * @param bit_id The bit id
     */
    size_t get_wl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderTileBitId& bit_id) const;

    /**
     * @brief Get the config bit number in the tile 
     * @param tile_name The name of the tile
     * @param bit_id The bit id
     */
    ConfigBitId get_config_bit_num(const std::string& tile_name, const BitstreamReorderTileBitId& bit_id) const;

    vtr::vector<BitstreamReorderRegionId, bistream_reorder_region> regions;
    std::unordered_map<std::string, tile_bit_map> tile_bit_maps;
};

}  // namespace openfpga