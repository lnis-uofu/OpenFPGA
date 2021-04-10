/********************************************************************
 * Header file for fabric_bitstream_utils.cpp
 *******************************************************************/
#ifndef FABRIC_BITSTREAM_UTILS_H
#define FABRIC_BITSTREAM_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include <map>
#include "bitstream_manager.h"
#include "fabric_bitstream.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

size_t find_fabric_regional_bitstream_max_size(const FabricBitstream& fabric_bitstream);

size_t find_configuration_chain_fabric_bitstream_size_to_be_skipped(const FabricBitstream& fabric_bitstream,
                                                                    const BitstreamManager& bitstream_manager,
                                                                    const bool& bit_value_to_skip);

/* Alias to a specific organization of bitstreams for frame-based configuration protocol */
typedef std::vector<std::vector<bool>> ConfigChainFabricBitstream;
ConfigChainFabricBitstream build_config_chain_fabric_bitstream_by_region(const BitstreamManager& bitstream_manager,
                                                                         const FabricBitstream& fabric_bitstream);

/* Alias to a specific organization of bitstreams for frame-based configuration protocol */
typedef std::map<std::string, std::vector<bool>> FrameFabricBitstream;
FrameFabricBitstream build_frame_based_fabric_bitstream_by_address(const FabricBitstream& fabric_bitstream);

size_t find_frame_based_fast_configuration_fabric_bitstream_size(const FabricBitstream& fabric_bitstream,
                                                                 const bool& bit_value_to_skip);

/* Alias to a specific organization of bitstreams for memory bank configuration protocol */
typedef std::map<std::pair<std::string, std::string>, std::vector<bool>> MemoryBankFabricBitstream;
MemoryBankFabricBitstream build_memory_bank_fabric_bitstream_by_address(const FabricBitstream& fabric_bitstream);

size_t find_memory_bank_fast_configuration_fabric_bitstream_size(const FabricBitstream& fabric_bitstream,
                                                                 const bool& bit_value_to_skip);

} /* end namespace openfpga */

#endif
