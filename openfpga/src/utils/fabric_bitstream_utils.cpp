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
 * Reorganize the fabric bitstream for memory banks
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
