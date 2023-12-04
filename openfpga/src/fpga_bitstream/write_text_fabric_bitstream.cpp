/********************************************************************
 * This file includes functions that output a fabric-dependent
 * bitstream database to files in plain text
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "bitstream_manager_utils.h"
#include "fabric_bitstream_utils.h"
#include "fast_configuration.h"
#include "openfpga_decode.h"
#include "openfpga_digest.h"
#include "openfpga_naming.h"
#include "openfpga_version.h"
#include "write_text_fabric_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information to a bitstream file
 *******************************************************************/
static void write_fabric_bitstream_text_file_head(
  std::fstream& fp, const bool& include_time_stamp) {
  valid_file_stream(fp);

  fp << "// Fabric bitstream" << std::endl;

  if (include_time_stamp) {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    /* Note that version is also a type of time stamp */
    fp << "// Version: " << openfpga::VERSION << std::endl;
    fp << "// Date: " << std::ctime(&end_time);
  }
}

/********************************************************************
 * Write the flatten fabric bitstream to a plain text file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_flatten_fabric_bitstream_to_text_file(
  std::fstream& fp, const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream) {
  if (false == valid_file_stream(fp)) {
    return 1;
  }

  /* Output bitstream size information */
  fp << "// Bitstream length: " << fabric_bitstream.num_bits() << std::endl;

  /* Output bitstream data */
  for (const FabricBitId& fabric_bit : fabric_bitstream.bits()) {
    fp << bitstream_manager.bit_value(fabric_bitstream.config_bit(fabric_bit));
  }

  return 0;
}

/********************************************************************
 * Write the fabric bitstream fitting a configuration chain protocol
 * to a plain text file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_config_chain_fabric_bitstream_to_text_file(
  std::fstream& fp, const bool& fast_configuration,
  const bool& bit_value_to_skip, const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream) {
  int status = 0;

  size_t regional_bitstream_max_size =
    find_fabric_regional_bitstream_max_size(fabric_bitstream);
  ConfigChainFabricBitstream regional_bitstreams =
    build_config_chain_fabric_bitstream_by_region(bitstream_manager,
                                                  fabric_bitstream);

  /* For fast configuration, the bitstream size counts from the first bit '1' */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip =
      find_configuration_chain_fabric_bitstream_size_to_be_skipped(
        fabric_bitstream, bitstream_manager, bit_value_to_skip);
    VTR_ASSERT(num_bits_to_skip < regional_bitstream_max_size);
    VTR_LOG(
      "Fast configuration will skip %g% (%lu/%lu) of configuration "
      "bitstream.\n",
      100. * (float)num_bits_to_skip / (float)regional_bitstream_max_size,
      num_bits_to_skip, regional_bitstream_max_size);
  }

  /* Output bitstream size information */
  fp << "// Bitstream length: "
     << regional_bitstream_max_size - num_bits_to_skip << std::endl;
  fp << "// Bitstream width (LSB -> MSB): " << fabric_bitstream.num_regions()
     << std::endl;

  /* Output bitstream data */
  for (size_t ibit = num_bits_to_skip; ibit < regional_bitstream_max_size;
       ++ibit) {
    for (const auto& region_bitstream : regional_bitstreams) {
      fp << region_bitstream[ibit];
    }
    if (ibit < regional_bitstream_max_size - 1) {
      fp << std::endl;
    }
  }

  return status;
}

/********************************************************************
 * Write the fabric bitstream fitting a memory bank protocol
 * to a plain text file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_memory_bank_fabric_bitstream_to_text_file(
  std::fstream& fp, const bool& fast_configuration,
  const bool& bit_value_to_skip, const FabricBitstream& fabric_bitstream) {
  int status = 0;

  MemoryBankFabricBitstream fabric_bits_by_addr =
    build_memory_bank_fabric_bitstream_by_address(fabric_bitstream);

  /* The address sizes and data input sizes are the same across any element,
   * just get it from the 1st element to save runtime
   */
  size_t bl_addr_size = fabric_bits_by_addr.begin()->first.first.size();
  size_t wl_addr_size = fabric_bits_by_addr.begin()->first.second.size();
  size_t din_size = fabric_bits_by_addr.begin()->second.size();

  /* Identify and output bitstream size information */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip =
      fabric_bits_by_addr.size() -
      find_memory_bank_fast_configuration_fabric_bitstream_size(
        fabric_bitstream, bit_value_to_skip);
    VTR_ASSERT(num_bits_to_skip < fabric_bits_by_addr.size());
    VTR_LOG(
      "Fast configuration will skip %g% (%lu/%lu) of configuration "
      "bitstream.\n",
      100. * (float)num_bits_to_skip / (float)fabric_bits_by_addr.size(),
      num_bits_to_skip, fabric_bits_by_addr.size());
  }

  /* Output information about how to intepret the bitstream */
  fp << "// Bitstream length: " << fabric_bits_by_addr.size() - num_bits_to_skip
     << std::endl;
  fp << "// Bitstream width (LSB -> MSB): ";
  fp << "<bl_address " << bl_addr_size << " bits>";
  fp << "<wl_address " << wl_addr_size << " bits>";
  fp << "<data input " << din_size << " bits>";
  fp << std::endl;

  for (const auto& addr_din_pair : fabric_bits_by_addr) {
    /* When fast configuration is enabled,
     * the rule to skip any configuration bit should consider the whole data
     * input values. Only all the bits in the din port match the value to be
     * skipped, the programming cycle can be skipped!
     */
    if (true == fast_configuration) {
      if (addr_din_pair.second ==
          std::vector<bool>(addr_din_pair.second.size(), bit_value_to_skip)) {
        continue;
      }
    }

    /* Write BL address code */
    fp << addr_din_pair.first.first;
    /* Write WL address code */
    fp << addr_din_pair.first.second;
    /* Write data input */
    for (const bool& din_value : addr_din_pair.second) {
      fp << din_value;
    }
    fp << std::endl;
  }

  return status;
}

/********************************************************************
 * Write the fabric bitstream fitting a memory bank protocol
 * to a plain text file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_memory_bank_flatten_fabric_bitstream_to_text_file(
  std::fstream& fp, const bool& fast_configuration,
  const bool& bit_value_to_skip, const FabricBitstream& fabric_bitstream,
  const bool& keep_dont_care_bits) {
  int status = 0;

  char dont_care_bit = '0';
  if (keep_dont_care_bits) {
    dont_care_bit = DONT_CARE_CHAR;
  }
  MemoryBankFlattenFabricBitstream fabric_bits =
    build_memory_bank_flatten_fabric_bitstream(
      fabric_bitstream, fast_configuration, bit_value_to_skip, dont_care_bit);

  /* The address sizes and data input sizes are the same across any element,
   * just get it from the 1st element to save runtime
   */
  size_t bl_addr_size = fabric_bits.bl_vector_size();
  size_t wl_addr_size = fabric_bits.wl_vector_size();

  /* Output information about how to intepret the bitstream */
  fp << "// Bitstream length: " << fabric_bits.size() << std::endl;
  fp << "// Bitstream width (LSB -> MSB): ";
  fp << "<bl_address " << bl_addr_size << " bits>";
  fp << "<wl_address " << wl_addr_size << " bits>";
  fp << std::endl;

  for (const auto& wl_vec : fabric_bits.wl_vectors()) {
    /* Write BL address code */
    for (const auto& bl_unit : fabric_bits.bl_vector(wl_vec)) {
      fp << bl_unit;
    }
    /* Write WL address code */
    for (const auto& wl_unit : wl_vec) {
      fp << wl_unit;
    }
    fp << std::endl;
  }

  return status;
}

/********************************************************************
 * Write the fabric bitstream fitting a memory bank protocol
 * to a plain text file in efficient method
 *
 * Old function is write_memory_bank_flatten_fabric_bitstream_to_text_file()
 *
 * As compared to original function, based on 100K LE FPGA:
 *  1. Original function used 600 seconds and needs high memory usage
 *  2. This new function only needs 1 second and 4M Bytes
 *
 * Old function only print WL in decremental order. It is not by intentional
 * It is because of the map-key ordering
 * In QL Memory Bank with Flatten BL/WL, data is stored by WL address,
 *   where we use WL string as map key
 *     WL #0 --- "1000000000000 .... 0000"
 *     WL #1 --- "0100000000000 .... 0000"
 *     WL #n-1 --- "0000000000000 .... 0001
 * From string comparison wise, WL #n-1 will be first, and WL #0 will be last
 * The sequence of WL does not really matter,  but preferrable in some ordering
 *   manner. Using map key as ordering cannot guarantee the determinstic
 *
 * This new way of writting fabric guarantee the WL order in 100% deterministc
 *   way: either incremental (default) or decremental
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int fast_write_memory_bank_flatten_fabric_bitstream_to_text_file(
  std::fstream& fp, const bool& fast_configuration,
  const bool& bit_value_to_skip, const FabricBitstream& fabric_bitstream,
  const bool& keep_dont_care_bits, const bool& wl_incremental_order) {
  int status = 0;

  std::string dont_care_bit = "0";
  if (keep_dont_care_bits) {
    dont_care_bit = "x";
  }
  const FabricBitstreamMemoryBank& memory_bank =
    fabric_bitstream.memory_bank_info(fast_configuration, bit_value_to_skip);

  fabric_size_t longest_effective_wl_count =
    memory_bank.get_longest_effective_wl_count();
  /* Output information about how to intepret the bitstream */
  fp << "// Bitstream length: " << longest_effective_wl_count << std::endl;
  fp << "// Bitstream width (LSB -> MSB): ";
  fp << "<bl_address " << memory_bank.get_total_bl_addr_size() << " bits>";
  fp << "<wl_address " << memory_bank.get_total_wl_addr_size() << " bits>";
  fp << std::endl;

  // Step 1
  // Initialize wl_indexes for every region
  // The intialization depends the ordering of WL
  // It could either be 0 (if wl_incremental_order=true) or
  // last WL index (if wl_incremental_order=false)
  std::vector<fabric_size_t> wl_indexes;
  for (size_t region = 0; region < memory_bank.datas.size(); region++) {
    if (wl_incremental_order) {
      wl_indexes.push_back(0);
    } else {
      wl_indexes.push_back(
        (fabric_size_t)(memory_bank.datas[region].size() - 1));
    }
  }
  // Step 2
  // Loop through total WL count that we would like to configure
  for (size_t wl_index = 0; wl_index < longest_effective_wl_count; wl_index++) {
    // Step 3
    // Write BL address
    // We cascade all regions: 0, 1, 2 ...
    for (size_t region = 0; region < memory_bank.datas.size(); region++) {
      // Step 3a
      // The sequence of configuration of each region WL is not the same
      //   since WL to skip for each region is not the same
      // If it happen that current WL that we are going to program is
      //   one of the WLs (stored in wls_to_skip) that we had determined
      //   to skip, the we will increment or decrement to next
      //   depending on wl_incremental_order
      const fabric_blwl_length& lengths = memory_bank.blwl_lengths[region];
      fabric_size_t current_wl = wl_indexes[region];
      while (std::find(memory_bank.wls_to_skip[region].begin(),
                       memory_bank.wls_to_skip[region].end(),
                       current_wl) != memory_bank.wls_to_skip[region].end()) {
        // We would like to skip this
        if (wl_incremental_order) {
          wl_indexes[region]++;
        } else {
          wl_indexes[region]--;
        }
        current_wl = wl_indexes[region];
      }
      // Step 3b
      // If current WL still within the valid range, we will print BL
      // Otherwise it is either
      //   overflow (wl_incremental_order=true) or
      //   underflow (to max fabric_blwl_length when wl_incremental_order=false)
      // Since fabric_blwl_length is unsigned, hence underflow of -1 will be
      //   considered as overflow too
      // If it is overflow/underflow, then we just print don't care
      if (current_wl < memory_bank.datas[region].size()) {
        const std::vector<uint8_t>& data =
          memory_bank.datas[region][current_wl];
        const std::vector<uint8_t>& mask =
          memory_bank.masks[region][current_wl];
        // Step 3c
        // Real code to print BL data that we had stored
        // mask tell you each BL is valid
        //   for invalid BL, we will print don't care
        // data tell you the real din value
        // (bl >> 3) - This is to find Byte index of the BL
        // (1 << (bl & 7)) - This is to find Bit index of the BL
        //                   within that Byte index
        // When we '&' both, we can know if that BL is set or unset
        /*
          -----------------------------------------------------------------
          |  bit (bl)  |  Byte index (bl >> 3) | Bit index (1 << (bl & 7)) |
          |----------------------------------------------------------------
          |  0         |             0         |    b0000_0001 (or 0x01)   |
          |  1         |             0         |    b0000_0010 (or 0x02)   |
          |  2         |             0         |    b0000_0100 (or 0x04)   |
          |  3         |             0         |    b0000_1000 (or 0x08)   |
          |  4         |             0         |    b0001_0000 (or 0x10)   |
          |  5         |             0         |    b0010_0000 (or 0x20)   |
          |  6         |             0         |    b0100_0000 (or 0x40)   |
          |  7         |             0         |    b1000_0000 (or 0x80)   |
          |  8         |             1         |    b0000_0001 (or 0x01)   |
          | ...        |            ...        |          ...              |
          ------------------------------------------------------------------
          Each BL can be uniquely represented by bit slice in byte array
        */
        for (size_t bl = 0; bl < lengths.bl; bl++) {
          if (mask[bl >> 3] & (1 << (bl & 7))) {
            if (data[bl >> 3] & (1 << (bl & 7))) {
              fp << "1";
            } else {
              fp << "0";
            }
          } else {
            fp << dont_care_bit.c_str();
          }
        }
      } else {
        /* However not all region has equal WL, for those that is shorter,
         * print 'x' for all BL*/
        for (size_t bl = 0; bl < lengths.bl; bl++) {
          fp << dont_care_bit.c_str();
        }
      }
    }
    // Step 4
    // Write WL address
    // We cascade all regions: 0, 1, 2 ...
    for (size_t region = 0; region < memory_bank.datas.size(); region++) {
      const fabric_blwl_length& lengths = memory_bank.blwl_lengths[region];
      fabric_size_t current_wl = wl_indexes[region];
      // Step 4a
      // If current WL still within the valid range, we will print WL
      // Otherwise it is overflow/underflow then we will print don't care
      if (current_wl < memory_bank.datas[region].size()) {
        // Step 4b
        // One hot printing
        for (size_t wl_temp = 0; wl_temp < lengths.wl; wl_temp++) {
          if (wl_temp == current_wl) {
            fp << "1";
          } else {
            fp << "0";
          }
        }
        // Step 4b
        // Increment or decrement to next depending on wl_incremental_order
        if (wl_incremental_order) {
          wl_indexes[region]++;
        } else {
          wl_indexes[region]--;
        }
      } else {
        /* However not all region has equal WL, for those that is shorter,
         * print 'x' for all WL */
        for (size_t wl_temp = 0; wl_temp < lengths.wl; wl_temp++) {
          fp << dont_care_bit.c_str();
        }
      }
    }
    fp << std::endl;
  }
  return status;
}

/********************************************************************
 * Write the fabric bitstream fitting a memory bank protocol
 * to a plain text file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_memory_bank_shift_register_fabric_bitstream_to_text_file(
  std::fstream& fp, const bool& fast_configuration,
  const bool& bit_value_to_skip, const FabricBitstream& fabric_bitstream,
  const MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const bool& keep_dont_care_bits) {
  int status = 0;

  char dont_care_bit = '0';
  if (keep_dont_care_bits) {
    dont_care_bit = DONT_CARE_CHAR;
  }
  MemoryBankShiftRegisterFabricBitstream fabric_bits =
    build_memory_bank_shift_register_fabric_bitstream(
      fabric_bitstream, blwl_sr_banks, fast_configuration, bit_value_to_skip,
      dont_care_bit);

  /* Output information about how to intepret the bitstream */
  fp << "// Bitstream word count: " << fabric_bits.num_words() << std::endl;
  fp << "// Bitstream bl word size: " << fabric_bits.bl_word_size()
     << std::endl;
  fp << "// Bitstream wl word size: " << fabric_bits.wl_word_size()
     << std::endl;
  fp << "// Bitstream width (LSB -> MSB): ";
  fp << "<bl shift register heads " << fabric_bits.bl_width() << " bits>";
  fp << "<wl shift register heads " << fabric_bits.wl_width() << " bits>";
  fp << std::endl;

  size_t word_cnt = 0;

  for (const auto& word : fabric_bits.words()) {
    fp << "// Word " << word_cnt << std::endl;

    /* Write BL address code */
    fp << "// BL part " << std::endl;
    for (const auto& bl_vec : fabric_bits.bl_vectors(word)) {
      fp << bl_vec;
      fp << std::endl;
    }

    /* Write WL address code */
    fp << "// WL part " << std::endl;
    for (const auto& wl_vec : fabric_bits.wl_vectors(word)) {
      fp << wl_vec;
      fp << std::endl;
    }

    word_cnt++;
  }

  return status;
}

/********************************************************************
 * Write the fabric bitstream fitting a frame-based protocol
 * to a plain text file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_frame_based_fabric_bitstream_to_text_file(
  std::fstream& fp, const bool& fast_configuration,
  const bool& bit_value_to_skip, const FabricBitstream& fabric_bitstream) {
  int status = 0;

  FrameFabricBitstream fabric_bits_by_addr =
    build_frame_based_fabric_bitstream_by_address(fabric_bitstream);

  /* The address sizes and data input sizes are the same across any element,
   * just get it from the 1st element to save runtime
   */
  size_t addr_size = fabric_bits_by_addr.begin()->first.size();
  size_t din_size = fabric_bits_by_addr.begin()->second.size();

  /* Identify and output bitstream size information */
  size_t num_bits_to_skip = 0;
  if (true == fast_configuration) {
    num_bits_to_skip =
      fabric_bits_by_addr.size() -
      find_frame_based_fast_configuration_fabric_bitstream_size(
        fabric_bitstream, bit_value_to_skip);
    VTR_ASSERT(num_bits_to_skip < fabric_bits_by_addr.size());
    VTR_LOG(
      "Fast configuration will skip %g% (%lu/%lu) of configuration "
      "bitstream.\n",
      100. * (float)num_bits_to_skip / (float)fabric_bits_by_addr.size(),
      num_bits_to_skip, fabric_bits_by_addr.size());
  }

  /* Output information about how to intepret the bitstream */
  fp << "// Bitstream length: " << fabric_bits_by_addr.size() - num_bits_to_skip
     << std::endl;
  fp << "// Bitstream width (LSB -> MSB): <address " << addr_size
     << " bits><data input " << din_size << " bits>" << std::endl;

  for (const auto& addr_din_pair : fabric_bits_by_addr) {
    /* When fast configuration is enabled,
     * the rule to skip any configuration bit should consider the whole data
     * input values. Only all the bits in the din port match the value to be
     * skipped, the programming cycle can be skipped!
     */
    if (true == fast_configuration) {
      if (addr_din_pair.second ==
          std::vector<bool>(addr_din_pair.second.size(), bit_value_to_skip)) {
        continue;
      }
    }

    /* Write address code */
    fp << addr_din_pair.first;

    /* Write data input */
    for (const bool& din_value : addr_din_pair.second) {
      fp << din_value;
    }
    fp << std::endl;
  }

  return status;
}

/********************************************************************
 * Write the fabric bitstream to a plain text file
 * Notes:
 *   - This is the final bitstream which is loadable to the FPGA fabric
 *     (Verilog netlists etc.)
 *   - Do NOT include any comments or other characters that the 0|1 bitstream
 *content in this file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
int write_fabric_bitstream_to_text_file(
  const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream,
  const MemoryBankShiftRegisterBanks& blwl_sr_banks,
  const ConfigProtocol& config_protocol,
  const FabricGlobalPortInfo& global_ports,
  const BitstreamWriterOption& options) {
  VTR_ASSERT(options.output_file_type() ==
             BitstreamWriterOption::e_bitfile_type::TEXT);
  std::string fname = options.output_file_name();
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR(
      "Received empty file name to output bitstream!\n\tPlease specify a valid "
      "file name.\n");
  }

  std::string timer_message =
    std::string("Write ") + std::to_string(fabric_bitstream.num_bits()) +
    std::string(" fabric bitstream into plain text file '") + fname +
    std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(fname.c_str(), fp);

  bool apply_fast_configuration =
    is_fast_configuration_applicable(global_ports) &&
    options.fast_configuration();
  if (options.fast_configuration() &&
      apply_fast_configuration != options.fast_configuration()) {
    VTR_LOG_WARN("Disable fast configuration even it is enabled by user\n");
  }

  bool bit_value_to_skip = false;
  if (apply_fast_configuration) {
    bit_value_to_skip = find_bit_value_to_skip_for_fast_configuration(
      config_protocol.type(), global_ports, bitstream_manager,
      fabric_bitstream);
  }

  /* Write file head */
  write_fabric_bitstream_text_file_head(fp, options.time_stamp());

  /* Output fabric bitstream to the file */
  int status = 0;
  switch (config_protocol.type()) {
    case CONFIG_MEM_STANDALONE:
      status = write_flatten_fabric_bitstream_to_text_file(
        fp, bitstream_manager, fabric_bitstream);
      break;
    case CONFIG_MEM_SCAN_CHAIN:
      status = write_config_chain_fabric_bitstream_to_text_file(
        fp, apply_fast_configuration, bit_value_to_skip, bitstream_manager,
        fabric_bitstream);
      break;
    case CONFIG_MEM_QL_MEMORY_BANK: {
      /* Bitstream organization depends on the BL/WL protocols
       * - If BL uses decoders, we have to config each memory cell one by one.
       * - If BL uses flatten, we can configure all the memory cells on the same
       * row by enabling dedicated WL In such case, we will merge the BL data
       * under the same WL address Fast configuration is applicable when a row
       * of BLs are all zeros/ones while we have a global reset/set for all the
       * memory cells
       * - if BL uses shift-register, same as the flatten.
       */
      if (BLWL_PROTOCOL_DECODER == config_protocol.bl_protocol_type()) {
        status = write_memory_bank_fabric_bitstream_to_text_file(
          fp, apply_fast_configuration, bit_value_to_skip, fabric_bitstream);
      } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.bl_protocol_type() &&
                 BLWL_PROTOCOL_FLATTEN == config_protocol.wl_protocol_type()) {
        // If both BL and WL protocols are flatten, use new way to write the
        // bitstream
        status = fast_write_memory_bank_flatten_fabric_bitstream_to_text_file(
          fp, apply_fast_configuration, bit_value_to_skip, fabric_bitstream,
          options.keep_dont_care_bits(), !options.wl_decremental_order());

      } else if (BLWL_PROTOCOL_FLATTEN == config_protocol.bl_protocol_type()) {
        status = write_memory_bank_flatten_fabric_bitstream_to_text_file(
          fp, apply_fast_configuration, bit_value_to_skip, fabric_bitstream,
          options.keep_dont_care_bits());
      } else {
        VTR_ASSERT(BLWL_PROTOCOL_SHIFT_REGISTER ==
                   config_protocol.bl_protocol_type());
        status = write_memory_bank_shift_register_fabric_bitstream_to_text_file(
          fp, apply_fast_configuration, bit_value_to_skip, fabric_bitstream,
          blwl_sr_banks, options.keep_dont_care_bits());
      }
      break;
    }
    case CONFIG_MEM_MEMORY_BANK:
      status = write_memory_bank_fabric_bitstream_to_text_file(
        fp, apply_fast_configuration, bit_value_to_skip, fabric_bitstream);
      break;
    case CONFIG_MEM_FRAME_BASED:
      status = write_frame_based_fabric_bitstream_to_text_file(
        fp, apply_fast_configuration, bit_value_to_skip, fabric_bitstream);
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid configuration protocol type!\n");
      status = 1;
  }

  /* Print an end to the file here */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  VTR_LOGV(options.verbose_output(),
           "Outputted %lu configuration bits to plain text file: %s\n",
           fabric_bitstream.bits().size(), fname.c_str());

  return status;
}

} /* end namespace openfpga */
