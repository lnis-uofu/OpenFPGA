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

} /* end namespace openfpga */
