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

typedef vtr::StrongId<bitstream_reorder_region_id_tag> BitstreamReorderRegionId;
typedef vtr::StrongId<bitstream_reorder_region_block_id_tag> BitstreamReorderRegionBlockId;
typedef vtr::StrongId<bitstream_reorder_tile_bit_id_tag> BitstreamReorderTileBitId;
typedef vtr::StrongId<bitstream_reorder_bit_id_tag> BitstreamReorderBitId;

struct bitstream_reorder_tile_bit_info {
    BitstreamReorderRegionId region_id;
    BitstreamReorderRegionBlockId block_id;
    BitstreamReorderTileBitId tile_bit_id;
};

struct bistream_reorder_region {
    vtr::vector<BitstreamReorderRegionBlockId, std::string> tile_types;
    vtr::vector<BitstreamReorderRegionBlockId, int> tile_bit_offsets;
    vtr::vector<BitstreamReorderRegionBlockId, std::string> tile_aliases;
};

struct tile_bit_map {
    vtr::vector<BitstreamReorderTileBitId, ConfigBitId> bit_map;
    int num_cbits;
    int num_bls;
    int num_wls;
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
     * @brief Get the regions in the bitstream reorder map
     */
    std::vector<BitstreamReorderRegionId> get_regions() const;

    /**
     * @brief Get the blocks in a region
     * @param region_id The id of the region
     */
    std::vector<BitstreamReorderRegionBlockId> get_region_blocks(const BitstreamReorderRegionId& region_id) const;

    /**
     * @brief Get the alias name of a block
     * @param region_id The id of the region
     * @param block_id The id of the tile
     */
    std::string get_block_alias(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id) const;

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
     * @brief Get the config bit number in the tile 
     * @param tile_name The name of the tile
     * @param bit_id The bit id
     */
    ConfigBitId get_config_bit_num(const std::string& tile_name, const BitstreamReorderTileBitId& bit_id) const;

    /**
     * @brief Get the bitline number from the bit index
     * @param bit_id The bit id
     */
    int get_bl_from_index(const BitstreamReorderBitId& bit_id) const;

    /**
     * @brief Get the wordline number from the bit index
     * @param bit_id The bit id
     */
    int get_wl_from_index(const BitstreamReorderBitId& bit_id) const;

private:

    /**
     * @brief Get the bitline number in the region
     * @param region_id The id of the region
     * @param block_id The id of the tile
     * @param bit_id The bit id
     */
    int get_bl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderTileBitId& bit_id) const;

    /**
     * @brief Get the wordline number in the region
     * @param region_id The id of the region
     * @param block_id The id of the tile
     * @param bit_id The bit id
     */
    int get_wl_from_index(const BitstreamReorderRegionId& region_id, const BitstreamReorderRegionBlockId& block_id, const BitstreamReorderTileBitId& bit_id) const;

    vtr::vector<BitstreamReorderRegionId, bistream_reorder_region> regions;
    std::unordered_map<std::string, tile_bit_map> tile_bit_maps;
};

}  // namespace openfpga