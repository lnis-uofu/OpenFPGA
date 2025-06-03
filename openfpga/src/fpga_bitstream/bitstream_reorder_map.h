/******************************************************************************
 * This file introduces a data structure to store the reordering map for bitstream
 *
 ******************************************************************************/

#pragma once

#include <string>

#include "vtr_strong_id.h"
#include "vtr_vector.h"

namespace openfpga {

struct bitstream_reorder_region_id_tag;
struct bitstream_reorder_block_id_tag;
struct bitstream_reorder_bit_id_tag;

typedef vtr::StrongId<bitstream_reorder_region_id_tag> BitstreamReorderRegionId;
typedef vtr::StrongId<bitstream_reorder_block_id_tag> BitstreamReorderBlockId;
typedef vtr::StrongId<bitstream_reorder_bit_id_tag> BitstreamReorderBitId;

struct bistream_reorder_region {
    vtr::vector<BitstreamReorderBlockId, std::string> tile_types;
    vtr::vector<BitstreamReorderBlockId, std::string> tile_aliases;
    vtr::vector<BitstreamReorderBlockId, int> tile_num_cbits;
};

struct tile_bit_map {
    vtr::vector<BitstreamReorderBitId, int> bit_map;
    int num_cbits;
    int num_bls;
    int num_wls;
};

class BitstreamReorderMap {
 public:
  BitstreamReorderMap();
  BitstreamReorderMap(const std::string& reorder_map_file);
  ~BitstreamReorderMap();

  void init_from_file(const std::string& reorder_map_file);

 private:
  vtr::vector<BitstreamReorderRegionId, bistream_reorder_region> regions;
  std::unordered_map<std::string, tile_bit_map> tile_bit_maps;
};

}  // namespace openfpga