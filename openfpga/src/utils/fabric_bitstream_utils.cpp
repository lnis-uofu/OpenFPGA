/************************************************************************
 * Function to perform fundamental operation for fabric bitstream class
 * These functions are not universal methods for the FabricBitstream class
 * They are made to ease the development in some specific purposes
 * Please classify such functions in this file
 ***********************************************************************/

#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "openfpga_decode.h"

#include "fabric_bitstream_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find the longest bitstream size of a fabric bitstream
 *******************************************************************/
size_t find_fabric_regional_bitstream_max_size(const FabricBitstream& fabric_bitstream) {
  size_t regional_bitstream_max_size = 0;
  /* Find the longest regional bitstream */
  for (const auto& region : fabric_bitstream.regions()) {
    if (regional_bitstream_max_size < fabric_bitstream.region_bits(region).size()) {
      regional_bitstream_max_size = fabric_bitstream.region_bits(region).size();
    }
  }
  return regional_bitstream_max_size;
}

/********************************************************************
 * For fast configuration, the number of bits to be skipped
 * depends on each regional bitstream
 * For example:
 *   Region 0: 000000001111101010
 *   Region 1:     00000011010101
 *   Region 2:   0010101111000110
 * The number of bits that can be skipped is limited by Region 2
 * Find the longest bitstream size of a fabric bitstream
 *******************************************************************/
size_t find_configuration_chain_fabric_bitstream_size_to_be_skipped(const FabricBitstream& fabric_bitstream,
                                                                    const BitstreamManager& bitstream_manager,
                                                                    const bool& bit_value_to_skip) {
  size_t regional_bitstream_max_size = find_fabric_regional_bitstream_max_size(fabric_bitstream);

  size_t num_bits_to_skip = size_t(-1);
  for (const auto& region : fabric_bitstream.regions()) {
    size_t curr_region_num_bits_to_skip = 0;
    for (const FabricBitId& bit_id : fabric_bitstream.region_bits(region)) {
      if (bit_value_to_skip != bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id))) {
        break;
      }
      curr_region_num_bits_to_skip++;
    }
    /* For regional bitstream which is short than the longest region bitstream,
     * The number of bits to skip 
     */
    curr_region_num_bits_to_skip += regional_bitstream_max_size - fabric_bitstream.region_bits(region).size();
    num_bits_to_skip = std::min(curr_region_num_bits_to_skip, num_bits_to_skip); 
  }

  return num_bits_to_skip;
}

/********************************************************************
 * Build a fabric bitstream which can be directly loaded to a configuration 
 * chain (either single-head or multi-bit)
 * We will organize the bitstreams in each region and align them 
 * Logic '0' bits may be deposited to those bitstream whose length is smaller
 * than the maximum bitstream among all the regions
 * For example:
 *   Region 0: 000000001111101010 <- max. bitstream length
 *   Region 1:     00000011010101 <- shorter bitstream than the max.; add zeros to the head
 *   Region 2:   0010101111000110 <- shorter bitstream than the max.; add zeros to the head
 *******************************************************************/
ConfigChainFabricBitstream build_config_chain_fabric_bitstream_by_region(const BitstreamManager& bitstream_manager,
                                                                         const FabricBitstream& fabric_bitstream) {
  /* Find the longest bitstream */
  size_t regional_bitstream_max_size = find_fabric_regional_bitstream_max_size(fabric_bitstream);

  ConfigChainFabricBitstream regional_bitstreams;
  regional_bitstreams.reserve(fabric_bitstream.regions().size());
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    std::vector<bool> curr_regional_bitstream;
    curr_regional_bitstream.resize(regional_bitstream_max_size, false);
    /* Starting index should consider the offset between the current bitstream size and 
     * the maximum size of regional bitstream
     */
    size_t offset = regional_bitstream_max_size - fabric_bitstream.region_bits(region).size();
    for (const FabricBitId& bit_id : fabric_bitstream.region_bits(region)) {
      curr_regional_bitstream[offset] = bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id));
      offset++;
    }
    VTR_ASSERT(offset == regional_bitstream_max_size);
   
    /* Add the adapt sub-bitstream */
    regional_bitstreams.push_back(curr_regional_bitstream);
  }
  return regional_bitstreams;
}

/********************************************************************
 * Reorganize the fabric bitstream for frame-based protocol
 * by the same address across regions:
 * This is due to that the length of fabric bitstream could be different in each region.
 * Template:
 *   <address> <din_values_from_different_regions>
 * An example:
 *   000000 1011
 *
 * Note: the std::map may cause large memory footprint for large bitstream databases!
 *******************************************************************/
FrameFabricBitstream build_frame_based_fabric_bitstream_by_address(const FabricBitstream& fabric_bitstream) {
  FrameFabricBitstream fabric_bits_by_addr;
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    for (const FabricBitId& bit_id : fabric_bitstream.region_bits(region)) {
      /* Create string for address */
      std::string addr_str;
      for (const char& addr_bit : fabric_bitstream.bit_address(bit_id)) {
        addr_str.push_back(addr_bit);
      }

      /* Expand all the don't care bits */
      for (const std::string& curr_addr_str : expand_dont_care_bin_str(addr_str)) {
        /* Place the config bit */
        auto result = fabric_bits_by_addr.find(curr_addr_str);
        if (result == fabric_bits_by_addr.end()) {
          /* This is a new bit, resize the vector to the number of regions
           * and deposit '0' to all the bits
           */
          fabric_bits_by_addr[curr_addr_str] = std::vector<bool>(fabric_bitstream.regions().size(), false);
          fabric_bits_by_addr[curr_addr_str][size_t(region)] = fabric_bitstream.bit_din(bit_id);
        } else {
          VTR_ASSERT_SAFE(result != fabric_bits_by_addr.end());
          result->second[size_t(region)] = fabric_bitstream.bit_din(bit_id);
        }
      }
    }
  }
  
  return fabric_bits_by_addr;
}

/********************************************************************
 * For fast configuration, the number of bits to be skipped
 * the rule to skip any configuration bit should consider the whole data input values.
 * Only all the bits in the din port match the value to be skipped,
 * the programming cycle can be skipped!
 * For example:
 *   Address: 010101
 *     Region 0: 0
 *     Region 1: 1 
 *     Region 2: 0 
 *   This bit cannot be skipped if the bit_value_to_skip is 0
 *
 *   Address: 010101
 *     Region 0: 0
 *     Region 1: 0 
 *     Region 2: 0 
 *   This bit can be skipped if the bit_value_to_skip is 0
 *******************************************************************/
size_t find_frame_based_fast_configuration_fabric_bitstream_size(const FabricBitstream& fabric_bitstream,
                                                                 const bool& bit_value_to_skip) {
  FrameFabricBitstream fabric_bits_by_addr = build_frame_based_fabric_bitstream_by_address(fabric_bitstream);

  size_t num_bits = 0;

  for (const auto& addr_din_pair : fabric_bits_by_addr) {
    bool skip_curr_bits = true;
    for (const bool& bit : addr_din_pair.second) {
      if (bit_value_to_skip != bit) {
        skip_curr_bits = false;
        break;
      }
    }

    if (false == skip_curr_bits) {
      num_bits++;
    }
  }

  return num_bits;
}

/********************************************************************
 * Reorganize the fabric bitstream for memory banks which use BL and WL decoders
 * by the same address across regions:
 * This is due to that the length of fabric bitstream could be different in each region.
 * Template:
 *   <bl_address> <wl_address> <din_values_from_different_regions>
 * An example:
 *   000000  00000 1011
 *
 * Note: the std::map may cause large memory footprint for large bitstream databases!
 *******************************************************************/
MemoryBankFabricBitstream build_memory_bank_fabric_bitstream_by_address(const FabricBitstream& fabric_bitstream) {
  MemoryBankFabricBitstream fabric_bits_by_addr;
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    for (const FabricBitId& bit_id : fabric_bitstream.region_bits(region)) {
      /* Create string for BL address */
      std::string bl_addr_str;
      for (const char& addr_bit : fabric_bitstream.bit_bl_address(bit_id)) {
        bl_addr_str.push_back(addr_bit);
      }

      /* Create string for WL address */
      std::string wl_addr_str;
      for (const char& addr_bit : fabric_bitstream.bit_wl_address(bit_id)) {
        wl_addr_str.push_back(addr_bit);
      }

      /* Place the config bit */
      auto result = fabric_bits_by_addr.find(std::make_pair(bl_addr_str, wl_addr_str));
      if (result == fabric_bits_by_addr.end()) {
        /* This is a new bit, resize the vector to the number of regions
         * and deposit '0' to all the bits
         */
        fabric_bits_by_addr[std::make_pair(bl_addr_str, wl_addr_str)] = std::vector<bool>(fabric_bitstream.regions().size(), false);
        fabric_bits_by_addr[std::make_pair(bl_addr_str, wl_addr_str)][size_t(region)] = fabric_bitstream.bit_din(bit_id);
      } else {
        VTR_ASSERT_SAFE(result != fabric_bits_by_addr.end());
        result->second[size_t(region)] = fabric_bitstream.bit_din(bit_id);
      }
    }
  }

  return fabric_bits_by_addr;
}

MemoryBankFlattenFabricBitstream build_memory_bank_flatten_fabric_bitstream(const FabricBitstream& fabric_bitstream,
                                                                            const bool& fast_configuration,
                                                                            const bool& bit_value_to_skip) {
  /* If fast configuration is not enabled, we need all the wl address even some of them have all-zero BLs */
  if (!fast_configuration) {
    vtr::vector<FabricBitRegionId, std::map<std::string, std::string>> fabric_bits_per_region;
    fabric_bits_per_region.resize(fabric_bitstream.num_regions());
    for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
      for (const FabricBitId& bit_id : fabric_bitstream.region_bits(region)) {
        /* Create string for BL address */
        std::string bl_addr_str(fabric_bitstream.bit_bl_address(bit_id).size(), bit_value_to_skip);

        /* Create string for WL address */
        std::string wl_addr_str;
        for (const char& addr_bit : fabric_bitstream.bit_wl_address(bit_id)) {
          wl_addr_str.push_back(addr_bit);
        }

        /* Deposit the config bit */
        fabric_bits_per_region[region][wl_addr_str] = bl_addr_str;
      }
    }
  }

  /* Build the bitstream by each region, here we use (WL, BL) pairs when storing bitstreams */
  vtr::vector<FabricBitRegionId, std::map<std::string, std::string>> fabric_bits_per_region;
  fabric_bits_per_region.resize(fabric_bitstream.num_regions());
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    for (const FabricBitId& bit_id : fabric_bitstream.region_bits(region)) {
      /* Skip din because they should be pre-configured through programming reset/set */
      if (fabric_bitstream.bit_din(bit_id) == bit_value_to_skip) {
        continue;
      }
      /* Create string for BL address */
      std::string bl_addr_str;
      for (const char& addr_bit : fabric_bitstream.bit_bl_address(bit_id)) {
        bl_addr_str.push_back(addr_bit);
      }

      /* Create string for WL address */
      std::string wl_addr_str;
      for (const char& addr_bit : fabric_bitstream.bit_wl_address(bit_id)) {
        wl_addr_str.push_back(addr_bit);
      }

      /* Place the config bit */
      auto result = fabric_bits_per_region[region].find(wl_addr_str);
      if (result == fabric_bits_per_region[region].end()) {
        fabric_bits_per_region[region][wl_addr_str] = bl_addr_str;
      } else {
        VTR_ASSERT_SAFE(result != fabric_bits_per_region[region].end());
        result->second = combine_two_1hot_str(bl_addr_str, result->second);
      }
    }
  }

  /* Find all the keys for the hash tables containing bitstream of each region */
  vtr::vector<FabricBitRegionId, std::vector<std::string>> fabric_bits_per_region_keys;
  fabric_bits_per_region_keys.resize(fabric_bitstream.num_regions());
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    /* Pre-allocate memory, because the key size may be large */
    fabric_bits_per_region_keys[region].reserve(fabric_bits_per_region[region].size());
    for (const auto& pair : fabric_bits_per_region[region]) {
      fabric_bits_per_region_keys[region].push_back(pair.first);
    } 
  }

  /* Find the maxium key size */
  size_t max_key_size = 0;
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    max_key_size = std::max(max_key_size, fabric_bits_per_region_keys[region].size());
  } 

  /* Find the BL/WL sizes per region; Pair convention is (BL, WL)
   * The address sizes are the same across any element, 
   * just get it from the 1st element to save runtime
   */
  vtr::vector<FabricBitRegionId, std::pair<size_t, size_t>> max_blwl_sizes_per_region;
  max_blwl_sizes_per_region.resize(fabric_bitstream.num_regions());
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    max_blwl_sizes_per_region[region].first = std::max(max_blwl_sizes_per_region[region].first, fabric_bits_per_region[region].begin()->second.size());
    max_blwl_sizes_per_region[region].second = std::max(max_blwl_sizes_per_region[region].second, fabric_bits_per_region[region].begin()->first.size());
  }

  /* Combine the bitstream from different region into a unique one. Now we follow the convention: use (WL, BL) pairs */
  MemoryBankFlattenFabricBitstream fabric_bits;
  for (size_t ikey = 0; ikey < max_key_size; ikey++) {
    /* Prepare the final BL/WL vectors to be added to the bitstream database */
    std::vector<std::string> cur_bl_vectors;
    std::vector<std::string> cur_wl_vectors;
    for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
      /* If the key id is in bound for the key list in this region, find the BL and WL and add to the final bitstream database
       * If the key id is out of bound for the key list in this region, we append an all-zero string for both BL and WLs
       */
      if (ikey < fabric_bits_per_region_keys[region].size()) {
        cur_wl_vectors.push_back(fabric_bits_per_region_keys[region][ikey]);
        cur_bl_vectors.push_back(fabric_bits_per_region[region].at(fabric_bits_per_region_keys[region][ikey]));
      } else {
        cur_wl_vectors.push_back(std::string(max_blwl_sizes_per_region[region].second, '0'));
        cur_bl_vectors.push_back(std::string(max_blwl_sizes_per_region[region].first, '0'));
      }
    }
    /* Add the pair to std map */
    fabric_bits.add_blwl_vectors(cur_bl_vectors, cur_wl_vectors);
  }

  return fabric_bits;
}

/********************************************************************
 * Reshape a list of vectors by aligning all of them to the last element
 * For example:
 * - Align vectors to the last element
 *
 *   index ---------------------->
 *   vector 0: 000000001111101010
 *   vector 1:     00000011010101
 *   vector 2:   0010101111000110
 * 
 * - Fill void in each vector with desired bits (Here assume fill 'x'
 *   index ---------------------->
 *   vector 0: 000000001111101010
 *   vector 1: xxxx00000011010101
 *   vector 2: xx0010101111000110
 * 
 * - Rotate the array by 90 degree
 *   index ----------------------->
 *   vector 0: 0xx
 *   vector 1: 0xx
 *   ...
 *   vector N: 010
 * 
 *******************************************************************/
static 
std::vector<std::string> reshape_bitstream_vectors_to_last_element(const std::vector<std::string>& bitstream_vectors,
                                                                   const char& default_bit_to_fill) {
  /* Find the max sizes of BL bits, this determines the size of shift register chain */
  size_t max_vec_size = 0;
  for (const auto& vec : bitstream_vectors) {
    max_vec_size = std::max(max_vec_size, vec.size());
  }
  /* Reshape the BL vectors */
  std::vector<std::string> reshaped_vectors(bitstream_vectors.size(), std::string());
  size_t col_cnt = 0;
  for (const auto& vec : bitstream_vectors) {
    reshaped_vectors[col_cnt].resize(max_vec_size - vec.size(), default_bit_to_fill);
    reshaped_vectors[col_cnt] += vec;
    col_cnt++;
  }

  /* Add the BL word to final bitstream */
  std::vector<std::string> rotated_vectors;
  for (size_t irow = 0; irow < max_vec_size; ++irow) {
    std::string cur_vec;
    for (size_t icol = 0; icol < reshaped_vectors.size(); ++icol) {
      cur_vec.push_back(reshaped_vectors[icol][irow]);
    }
    rotated_vectors.push_back(cur_vec); 
  }
 
  return rotated_vectors;
}

MemoryBankShiftRegisterFabricBitstream build_memory_bank_shift_register_fabric_bitstream(const FabricBitstream& fabric_bitstream,
                                                                                         const bool& fast_configuration,
                                                                                         //const std::array<MemoryBankShiftRegisterBanks, 2>& blwl_sr_banks,
                                                                                         const bool& bit_value_to_skip) {
  MemoryBankFlattenFabricBitstream raw_fabric_bits = build_memory_bank_flatten_fabric_bitstream(fabric_bitstream, fast_configuration, bit_value_to_skip);
  MemoryBankShiftRegisterFabricBitstream fabric_bits; 

  /* Iterate over each word */   
  for (const auto& wl_vec : raw_fabric_bits.wl_vectors()) {
    std::vector<std::string> bl_vec = raw_fabric_bits.bl_vector(wl_vec);

    MemoryBankShiftRegisterFabricBitstreamWordId word_id = fabric_bits.create_word();

    std::vector<std::string> reshaped_bl_vectors = reshape_bitstream_vectors_to_last_element(bl_vec, '0');
    /* Reverse the vector due to shift register nature: first-in first-out */
    std::reverse(reshaped_bl_vectors.begin(), reshaped_bl_vectors.end());
    /* Add the BL word to final bitstream */
    for (const auto& reshaped_bl_vec : reshaped_bl_vectors) {
      fabric_bits.add_bl_vectors(word_id, reshaped_bl_vec); 
    }

    std::vector<std::string> reshaped_wl_vectors = reshape_bitstream_vectors_to_last_element(wl_vec, '0');
    /* Reverse the vector due to shift register nature: first-in first-out */
    std::reverse(reshaped_wl_vectors.begin(), reshaped_wl_vectors.end());
    /* Add the BL word to final bitstream */
    for (const auto& reshaped_wl_vec : reshaped_wl_vectors) {
      fabric_bits.add_wl_vectors(word_id, reshaped_wl_vec); 
    }
  }

  return fabric_bits;
}

/********************************************************************
 * For fast configuration, the number of bits to be skipped
 * the rule to skip any configuration bit should consider the whole data input values.
 * Only all the bits in the din port match the value to be skipped,
 * the programming cycle can be skipped!
 * For example:
 *   BL Address: 010101
 *   WL Address: 010101
 *     Region 0: 0
 *     Region 1: 1 
 *     Region 2: 0 
 *   This bit cannot be skipped if the bit_value_to_skip is 0
 *
 *   BL Address: 010101
 *   WL Address: 010101
 *     Region 0: 0
 *     Region 1: 0 
 *     Region 2: 0 
 *   This bit can be skipped if the bit_value_to_skip is 0
 *******************************************************************/
size_t find_memory_bank_fast_configuration_fabric_bitstream_size(const FabricBitstream& fabric_bitstream,
                                                                 const bool& bit_value_to_skip) {
  MemoryBankFabricBitstream fabric_bits_by_addr = build_memory_bank_fabric_bitstream_by_address(fabric_bitstream);

  size_t num_bits = 0;

  for (const auto& addr_din_pair : fabric_bits_by_addr) {
    bool skip_curr_bits = true;
    for (const bool& bit : addr_din_pair.second) {
      if (bit_value_to_skip != bit) {
        skip_curr_bits = false;
        break;
      }
    }

    if (false == skip_curr_bits) {
      num_bits++;
    }
  }

  return num_bits;
}

} /* end namespace openfpga */
