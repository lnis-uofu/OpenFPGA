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
#include "openfpga_reserved_words.h"
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
                                                                            const bool& bit_value_to_skip,
                                                                            const char& dont_care_bit) {
  /* If fast configuration is not enabled, we need all the wl address even some of them have all-dont-care-bits BLs */
  if (!fast_configuration) {
    vtr::vector<FabricBitRegionId, std::map<std::string, std::string>> fabric_bits_per_region;
    fabric_bits_per_region.resize(fabric_bitstream.num_regions());
    for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
      for (const FabricBitId& bit_id : fabric_bitstream.region_bits(region)) {
        /* Create string for BL address with complete don't care bits */
        std::string bl_addr_str(fabric_bitstream.bit_bl_address(bit_id).size(), dont_care_bit);

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
      /* Create string for BL address */
      std::string bl_addr_str;
      for (const char& addr_bit : fabric_bitstream.bit_bl_address(bit_id)) {
        bl_addr_str.push_back(addr_bit);
      }

      /* If this bit should be programmed to 0, convert the 1s in BL to 0s  */
      if (fabric_bitstream.bit_din(bit_id) == bit_value_to_skip) {
        replace_str_bits(bl_addr_str, '1', '0');
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
        result->second = combine_two_1hot_str(result->second, bl_addr_str);
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
       * If the key id is out of bound for the key list in this region, we append an all-'x' string for both BL and WLs
       */
      if (ikey < fabric_bits_per_region_keys[region].size()) {
        cur_wl_vectors.push_back(fabric_bits_per_region_keys[region][ikey]);
        cur_bl_vectors.push_back(fabric_bits_per_region[region].at(fabric_bits_per_region_keys[region][ikey]));
      } else {
        cur_wl_vectors.push_back(std::string(max_blwl_sizes_per_region[region].second, dont_care_bit));
        cur_bl_vectors.push_back(std::string(max_blwl_sizes_per_region[region].first, dont_care_bit));
      }
    }
    /* Add the pair to std map */
    fabric_bits.add_blwl_vectors(cur_bl_vectors, cur_wl_vectors);
  }

  return fabric_bits;
}

/********************************************************************
 * Reshape a list of vectors by aligning all of them to the first element
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
 *   vector 1: 00000011010101xxxx
 *   vector 2: 0010101111000110xx
 * 
 * - Rotate the array by 90 degree
 *   index ----------------------->
 *   vector 0: 000
 *   vector 1: 000
 *   vector 2: 001
 *   ...
 *   vector N: 0xx
 * 
 *******************************************************************/
static 
std::vector<std::string> reshape_bitstream_vectors_to_first_element(const std::vector<std::string>& bitstream_vectors,
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
    reshaped_vectors[col_cnt] += vec;
    reshaped_vectors[col_cnt] += std::string(max_vec_size - vec.size(), default_bit_to_fill);
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

/** @brief Split each BL vector in a configuration region into multiple shift register banks
 *  For example
 *  Original vector: 1xxx010xxx1
 *  Resulting vector (2 register register banks):
 *   1xxx0
 *   10xxx1 
 */
static 
std::vector<std::string> redistribute_bl_vectors_to_shift_register_banks(const std::vector<std::string> bl_vectors, 
                                                                         const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                                                         const char& dont_care_bit) {
  std::vector<std::string> multi_bank_bl_vec;

  /* Resize the vector by counting the dimension */
  /* Compute the start index of each region */
  vtr::vector<ConfigRegionId, size_t> region_start_index;
  region_start_index.resize(blwl_sr_banks.regions().size(), 0);
  size_t total_num_banks = 0;
  for (const auto& region : blwl_sr_banks.regions()) {
    region_start_index[region] = total_num_banks; 
    total_num_banks += blwl_sr_banks.bl_banks(region).size();
  } 
  multi_bank_bl_vec.resize(total_num_banks);

  /* Resize each bank to be memory efficient */
  size_t vec_start_index = 0;
  for (const auto& region : blwl_sr_banks.regions()) {
    for (const auto& bank : blwl_sr_banks.bl_banks(region)) {
      size_t bank_size = blwl_sr_banks.bl_bank_size(region, bank);
      multi_bank_bl_vec[vec_start_index].resize(bank_size, dont_care_bit);
      vec_start_index++;
    }
  }

  for (const std::string& region_bl_vec : bl_vectors) {
    ConfigRegionId region = ConfigRegionId(&region_bl_vec - &bl_vectors[0]);
    for (size_t ibit = 0; ibit < region_bl_vec.size(); ++ibit) {
      /* Find the shift register bank id and the offset in data lines */
      BasicPort bl_port(std::string(MEMORY_BL_PORT_NAME), ibit, ibit);
      FabricBitLineBankId bank_id = blwl_sr_banks.find_bl_shift_register_bank_id(region, bl_port); 
      BasicPort sr_port = blwl_sr_banks.find_bl_shift_register_bank_data_port(region, bl_port); 
      VTR_ASSERT(1 == sr_port.get_width());
      
      size_t vec_index = region_start_index[region] + size_t(bank_id);
      multi_bank_bl_vec[vec_index][sr_port.get_lsb()] = region_bl_vec[ibit];
    }
  }

  return multi_bank_bl_vec;
}

/** @brief Split each WL vector in a configuration region into multiple shift register banks
 *  For example
 *  Original vector: 1xxx010xxx1
 *  Resulting vector (2 register register banks):
 *   1xxx0
 *   10xxx1 
 */
static 
std::vector<std::string> redistribute_wl_vectors_to_shift_register_banks(const std::vector<std::string> wl_vectors, 
                                                                         const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                                                         const char& dont_care_bit) {
  std::vector<std::string> multi_bank_wl_vec;

  /* Resize the vector by counting the dimension */
  /* Compute the start index of each region */
  vtr::vector<ConfigRegionId, size_t> region_start_index;
  region_start_index.resize(blwl_sr_banks.regions().size(), 0);
  size_t total_num_banks = 0;
  for (const auto& region : blwl_sr_banks.regions()) {
    region_start_index[region] = total_num_banks; 
    total_num_banks += blwl_sr_banks.wl_banks(region).size();
  } 
  multi_bank_wl_vec.resize(total_num_banks);

  /* Resize each bank to be memory efficient */
  size_t vec_start_index = 0;
  for (const auto& region : blwl_sr_banks.regions()) {
    for (const auto& bank : blwl_sr_banks.wl_banks(region)) {
      size_t bank_size = blwl_sr_banks.wl_bank_size(region, bank);
      multi_bank_wl_vec[vec_start_index].resize(bank_size, dont_care_bit);
      vec_start_index++;
    }
  }

  for (const std::string& region_wl_vec : wl_vectors) {
    ConfigRegionId region = ConfigRegionId(&region_wl_vec - &wl_vectors[0]);
    for (size_t ibit = 0; ibit < region_wl_vec.size(); ++ibit) {
      /* Find the shift register bank id and the offset in data lines */
      BasicPort wl_port(std::string(MEMORY_WL_PORT_NAME), ibit, ibit);
      FabricWordLineBankId bank_id = blwl_sr_banks.find_wl_shift_register_bank_id(region, wl_port); 
      BasicPort sr_port = blwl_sr_banks.find_wl_shift_register_bank_data_port(region, wl_port); 
      VTR_ASSERT(1 == sr_port.get_width());
      
      size_t vec_index = region_start_index[region] + size_t(bank_id);
      multi_bank_wl_vec[vec_index][sr_port.get_lsb()] = region_wl_vec[ibit];
    }
  }

  return multi_bank_wl_vec;
}

MemoryBankShiftRegisterFabricBitstream build_memory_bank_shift_register_fabric_bitstream(const FabricBitstream& fabric_bitstream,
                                                                                         const MemoryBankShiftRegisterBanks& blwl_sr_banks,
                                                                                         const bool& fast_configuration,
                                                                                         const bool& bit_value_to_skip,
                                                                                         const char& dont_care_bit) {
  MemoryBankFlattenFabricBitstream raw_fabric_bits = build_memory_bank_flatten_fabric_bitstream(fabric_bitstream, fast_configuration, bit_value_to_skip, dont_care_bit);
  MemoryBankShiftRegisterFabricBitstream fabric_bits; 

  /* Iterate over each word */   
  for (const auto& wl_vec : raw_fabric_bits.wl_vectors()) {
    std::vector<std::string> bl_vec = raw_fabric_bits.bl_vector(wl_vec);

    MemoryBankShiftRegisterFabricBitstreamWordId word_id = fabric_bits.create_word();

    /* Redistribute the BL vector to multiple banks */
    std::vector<std::string> multi_bank_bl_vec = redistribute_bl_vectors_to_shift_register_banks(bl_vec, blwl_sr_banks, dont_care_bit);

    std::vector<std::string> reshaped_bl_vectors = reshape_bitstream_vectors_to_first_element(multi_bank_bl_vec, dont_care_bit);
    /* Reverse the vectors due to the shift register chain nature: first-in first-out */
    std::reverse(reshaped_bl_vectors.begin(), reshaped_bl_vectors.end());
    /* Add the BL word to final bitstream */
    for (const auto& reshaped_bl_vec : reshaped_bl_vectors) {
      fabric_bits.add_bl_vectors(word_id, reshaped_bl_vec); 
    }

    /* Redistribute the WL vector to multiple banks */
    std::vector<std::string> multi_bank_wl_vec = redistribute_wl_vectors_to_shift_register_banks(wl_vec, blwl_sr_banks, dont_care_bit);

    std::vector<std::string> reshaped_wl_vectors = reshape_bitstream_vectors_to_first_element(multi_bank_wl_vec, dont_care_bit);
    /* Reverse the vectors due to the shift register chain nature: first-in first-out */
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
