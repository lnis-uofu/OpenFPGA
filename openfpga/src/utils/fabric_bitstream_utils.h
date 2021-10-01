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
#include <array>
#include "bitstream_manager.h"
#include "memory_bank_flatten_fabric_bitstream.h"
#include "memory_bank_shift_register_banks.h"
#include "memory_bank_shift_register_fabric_bitstream.h"
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

/********************************************************************
 * @ brief Reorganize the fabric bitstream for memory banks which use flatten BL and WLs
 * For each configuration region, we will merge BL address (which are 1-hot codes) under the same WL address
 *
 * Quick Example
 *   <bl of region A>_<bl of region B> <wl of region A>_<wl of region B>
 * An example:
 *   010_111  000_101
 *
 * Note that all the BL/WLs across configuration regions are independent. We will combine them together
 * Quick Example
 *   <bl of region A>_<bl of region B> <wl of region A>_<wl of region B>
 *   001_010 000_000
 *   100_100 000_000
 *   
 *   the bitstream will be merged as
 *   101_110 000_000
 *
 * @note the std::map may cause large memory footprint for large bitstream databases!
 *******************************************************************/
MemoryBankFlattenFabricBitstream build_memory_bank_flatten_fabric_bitstream(const FabricBitstream& fabric_bitstream,
                                                                            const bool& fast_configuration,
                                                                            const bool& bit_value_to_skip);

/********************************************************************
 * @ brief Reorganize the fabric bitstream for memory banks which use shift register to manipulate BL and WLs
 * For each configuration region, we will merge BL address (which are 1-hot codes) under the same WL address
 *
 * Quick Example
 *   <bl of region A>_<bl of region B> <wl of region A>_<wl of region B>
 * An example:
 *   010_111  000_101
 *
 * Note that all the BL/WLs across configuration regions are independent. We will combine them together
 * Quick Example
 *   <bl of region A>_<bl of region B> <wl of region A>_<wl of region B>
 *   001_010 000_000
 *   100_100 000_000
 *   
 *   the bitstream will be merged as
 *   101_110 000_000
 *
 * Because that the BL/WL are loaded through shift registers (perhaps using multiple heads), the bitstream will be reorganized as
 * Considering single head:  
 *   
 *   <bl of region A>_<bl of region B> <wl of region A>_<wl of region B>
 *   1_1 0_0
 *   0_1 0_0
 *   1_0 0_0 
 *
 * @note the std::map may cause large memory footprint for large bitstream databases!
 *******************************************************************/
MemoryBankShiftRegisterFabricBitstream build_memory_bank_shift_register_fabric_bitstream(const FabricBitstream& fabric_bitstream,
                                                                                         const bool& fast_configuration,
                                                                                         //const std::array<MemoryBankShiftRegisterBanks, 2>& blwl_sr_banks,
                                                                                         const bool& bit_value_to_skip);

/* Alias to a specific organization of bitstreams for memory bank configuration protocol */
typedef std::map<std::pair<std::string, std::string>, std::vector<bool>> MemoryBankFabricBitstream;
MemoryBankFabricBitstream build_memory_bank_fabric_bitstream_by_address(const FabricBitstream& fabric_bitstream);

size_t find_memory_bank_fast_configuration_fabric_bitstream_size(const FabricBitstream& fabric_bitstream,
                                                                 const bool& bit_value_to_skip);

} /* end namespace openfpga */

#endif
