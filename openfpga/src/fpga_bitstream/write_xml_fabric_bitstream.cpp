/********************************************************************
 * This file includes functions that output a fabric-dependent
 * bitstream database to files in XML format
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <sstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"
#include "pugixml.hpp"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"
#include "openfpga_gz_xml_writer.h"
#include "command_exit_codes.h"

/* Headers from archopenfpga library */
#include "bitstream_manager_utils.h"
#include "openfpga_naming.h"
#include "write_xml_fabric_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information to a bitstream file
 *******************************************************************/
static void write_fabric_bitstream_xml_file_head(
  pugi::xml_node& root_node, const bool& include_time_stamp) {
  std::stringstream ss;

  ss << "<!--" << std::endl;
  ss << "\t- Fabric bitstream" << std::endl;
  ss << "\t- Author: Xifan TANG" << std::endl;
  ss << "\t- Organization: University of Utah" << std::endl;

  auto end = std::chrono::system_clock::now();
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);
  if (include_time_stamp) {
    ss << "\t- Date: " << std::ctime(&end_time);
  }

  ss << "-->" << std::endl;
  ss << std::endl;

  pugi::xml_node cmt_node = root_node.append_child(pugi::node_comment);
  cmt_node.set_value(ss.str().c_str());
}

/********************************************************************
 * Write a configuration bit into a plain text file
 * General format
 *   <bit id="<fabric_bit>" value="<config_bit_value>">
 *     <hierarchy>
 *       <!-- configurable memory hierarchy -->
 *     </hierarchy>
 *     <!-- address information -->
 *     ...
 *   </bit>
 * The format depends on the type of configuration protocol
 * - Vanilla (standalone): No more information to be included
 * - Configuration chain: No more information to be included
 * - Memory bank :
 *     <bl address="<bl_address_value>"/>
 *     <wl address="<wl_address_value>"/>
 * - Frame-based configuration protocol :
 *     <frame address="<frame_address_value>"/>
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_fabric_config_bit_to_xml_file(
  pugi::xml_node& parent_node, const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream, const FabricBitId& fabric_bit,
  const e_config_protocol_type& config_type, bool fast_xml,
  const int& xml_hierarchy_depth, std::string& bl_addr, std::string& wl_addr,
  const BitstreamWriterOption& options) {
  if (options.value_to_skip(
        bitstream_manager.bit_value(fabric_bitstream.config_bit(fabric_bit)))) {
    return CMD_EXEC_SUCCESS;
  }

  pugi::xml_node bit_node = parent_node.append_child("bit");
  bit_node.append_attribute("id").set_value(static_cast<unsigned long long>(size_t(fabric_bit)));
  if (options.output_value()) {
    bit_node.append_attribute("value").set_value(
      bitstream_manager.bit_value(fabric_bitstream.config_bit(fabric_bit)) ? "1" : "0"
    );
  }

  /* Output hierarchy of this parent*/
  const ConfigBitId& config_bit = fabric_bitstream.config_bit(fabric_bit);
  const ConfigBlockId& config_block =
    bitstream_manager.bit_parent_block(config_bit);

  if (options.output_path()) {
    std::vector<ConfigBlockId> block_hierarchy =
      find_bitstream_manager_block_hierarchy(bitstream_manager, config_block);
    std::string hie_path;
    for (size_t iblk = 0; iblk < block_hierarchy.size(); ++iblk) {
      /* If enabled, pop the last block name */
      if (options.trim_path() && iblk == block_hierarchy.size() - 1) {
        break;
      }
      hie_path += bitstream_manager.block_name(block_hierarchy[iblk]);
      hie_path += std::string(".");
    }
    hie_path += generate_configurable_memory_data_out_name();
    hie_path += std::string("[");
    size_t bit_idx_in_parent_block =
      find_bitstream_manager_config_bit_index_in_parent_block(bitstream_manager,
                                                              config_bit);
    if (options.trim_path()) {
      bit_idx_in_parent_block =
        find_bitstream_manager_config_bit_index_in_grandparent_block(
          bitstream_manager, config_bit);
    }
    hie_path += std::to_string(bit_idx_in_parent_block);
    hie_path += std::string("]");

    bit_node.append_attribute("path").set_value(hie_path.c_str());
  }

  switch (config_type) {
    case CONFIG_MEM_STANDALONE:
    case CONFIG_MEM_SCAN_CHAIN:
      break;
    case CONFIG_MEM_MEMORY_BANK:
    case CONFIG_MEM_QL_MEMORY_BANK: {
      if (fast_xml) {
        // New way of printing XML
        // This is fast (less than 100s) as compared to original 1300s seen in
        // 100K LE FPFA
        const FabricBitstreamMemoryBank& memory_bank =
          fabric_bitstream.memory_bank_info();
        /* Bit line address */
        const fabric_bit_data& bit =
          memory_bank.fabric_bit_datas[(size_t)(fabric_bit)];
        const fabric_blwl_length& lengths =
          memory_bank.blwl_lengths[bit.region];
        if (bl_addr.size() == 0) {
          VTR_ASSERT(wl_addr.size() == 0);
          bl_addr.resize(lengths.bl);
          wl_addr.resize(lengths.wl);
          bl_addr.assign(lengths.bl, 'x');
          wl_addr.assign(lengths.wl, '0');
        } else {
          VTR_ASSERT((fabric_size_t)(bl_addr.size()) == lengths.bl);
          VTR_ASSERT((fabric_size_t)(wl_addr.size()) == lengths.wl);
        }
        bl_addr.replace(bit.bl, 1, "1");
        pugi::xml_node bl_node = bit_node.append_child("bl");
        bl_node.append_attribute("address").set_value(bl_addr.c_str());
        bl_addr.replace(bit.bl, 1, "x");
        /* Word line address */
        wl_addr.replace(bit.wl, 1, "1");
        pugi::xml_node wl_node = bit_node.append_child("wl");
        wl_node.append_attribute("address").set_value(wl_addr.c_str());
        wl_addr.replace(bit.wl, 1, "0");
      } else {
        /* Bit line address */
        pugi::xml_node bl_node = bit_node.append_child("bl");
        std::string bl_addr_bit_str;
        for (const char& addr_bit :
             fabric_bitstream.bit_bl_address(fabric_bit)) {
          bl_addr_bit_str += addr_bit;
        }
        bl_node.append_attribute("address").set_value(bl_addr_bit_str.c_str());

        pugi::xml_node wl_node = bit_node.append_child("wl");
        std::string wl_addr_bit_str;
        for (const char& addr_bit :
             fabric_bitstream.bit_wl_address(fabric_bit)) {
          wl_addr_bit_str += addr_bit;
        }
        wl_node.append_attribute("address").set_value(wl_addr_bit_str.c_str());
      }
      break;
    }
    case CONFIG_MEM_FRAME_BASED: {
      pugi::xml_node frame_node = bit_node.append_child("frame");
      std::string addr_bit_str;
      for (const char& addr_bit : fabric_bitstream.bit_address(fabric_bit)) {
        addr_bit_str += addr_bit;
      }
      frame_node.append_attribute("address").set_value(addr_bit_str.c_str());
      break;
    }
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid configuration protocol type!\n");
      return CMD_EXEC_FATAL_ERROR;
  }

  return CMD_EXEC_SUCCESS;
}

/********************************************************************
 * Write the fabric bitstream in a specific configuration region to an XML file
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
static int write_fabric_regional_config_bit_to_xml_file(
  pugi::xml_node& parent_node, const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream,
  const FabricBitRegionId& fabric_region,
  const e_config_protocol_type& config_type, bool fast_xml,
  const int& xml_hierarchy_depth, const BitstreamWriterOption& options) {
  int status = CMD_EXEC_SUCCESS;
  // Use string to print, instead of char by char
  // This is for Flatten BL/WL protocol
  // You will find this much more faster than char by char
  // We do not need to build the string for every BL/WL
  // It is one-hot and sequal addr
  // We start with all '0' (WL) or 'x' (BL)
  // By setting "1' and resettting ('0' or 'x') at approriate bit position
  // We could create one-hot string much faster
  // Use FPGA 100K as example: old way needs 1300seconds to write 85Gig XML
  // New way only needs 80seconds to write identical XML
  std::string bl_addr = "";
  std::string wl_addr = "";
  pugi::xml_node region_node = parent_node.append_child("region");
  region_node.append_attribute("id").set_value(static_cast<unsigned long long>(size_t(fabric_region)));

  size_t bit_index = 0;
  size_t total_bits = fabric_bitstream.region_bits(fabric_region).size();
  size_t percentage = 0;
  for (const FabricBitId& fabric_bit :
       fabric_bitstream.region_bits(fabric_region)) {
    status = write_fabric_config_bit_to_xml_file(
      region_node, bitstream_manager, fabric_bitstream, fabric_bit, config_type,
      fast_xml, xml_hierarchy_depth + 1, bl_addr, wl_addr, options);
    if (CMD_EXEC_FATAL_ERROR == status) {
      return status;
    }
    // Misc to print percentage of the process
    bit_index++;
    size_t temp = (bit_index * 100) / total_bits;
    if (temp != percentage) {
      VTR_LOGV(options.verbose_output(), "\tProgress: %lu%\r", percentage);
      percentage = temp;
    }
  }

  return status;
}

/********************************************************************
 * Write the fabric bitstream to an XML file
 * Notes:
 *   - This file is designed to be reused by testbench generators, e.g., CocoTB
 *   - It can NOT be directly loaded to the FPGA fabric
 *   - It include configurable memory paths in full hierarchy
 *
 * Return:
 *  - 0 if succeed
 *  - 1 if critical errors occured
 *******************************************************************/
int write_fabric_bitstream_to_xml_file(
  const BitstreamManager& bitstream_manager,
  const FabricBitstream& fabric_bitstream,
  const ConfigProtocol& config_protocol, const BitstreamWriterOption& options) {
  VTR_ASSERT(options.output_file_type() ==
             BitstreamWriterOption::e_bitfile_type::XML);
  /* Ensure that we have a valid file name */
  std::string fname = options.output_file_name();
  if (true == fname.empty()) {
    VTR_LOG_ERROR(
      "Received empty file name to output bitstream!\n\tPlease specify a valid "
      "file name.\n");
    return CMD_EXEC_FATAL_ERROR;
  }

  std::string timer_message =
    std::string("Write ") + std::to_string(fabric_bitstream.num_bits()) +
    std::string(" fabric bitstream into xml file '") + fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  pugi::xml_document doc;
  pugi::xml_node root_node = doc.append_child("fabric_bitstream");
  /* Write XML head */
  write_fabric_bitstream_xml_file_head(root_node, options.time_stamp());

  int xml_hierarchy_depth = 0;

  /* Output fabric bitstream to the file */
  int status = 0;
  for (const FabricBitRegionId& region : fabric_bitstream.regions()) {
    status = write_fabric_regional_config_bit_to_xml_file(
      root_node, bitstream_manager, fabric_bitstream, region, config_protocol.type(),
      BLWL_PROTOCOL_FLATTEN == config_protocol.bl_protocol_type() &&
        BLWL_PROTOCOL_FLATTEN == config_protocol.wl_protocol_type(),
      xml_hierarchy_depth + 1, options);
    if (CMD_EXEC_FATAL_ERROR == status) {
      break;
    }
  }

  if (CMD_EXEC_FATAL_ERROR == status) {
    VTR_LOG_ERROR("Error occurs when fabric bitstream data is organized in XML nodes\n");
    return status;
  }

  VTR_LOGV(options.verbose_output(),
           "Outputting %lu configuration bits to XML file: %s\n",
           fabric_bitstream.bits().size(), fname.c_str());

  /* Output to xml file */
  bool output_success = false;
  if (file_require_gz(fname)) {
    GzXmlWriter writer(fname.c_str());
    if (writer.isValid()) {
      doc.save(writer);
      output_success = true;
    }
  } else {
    output_success = doc.save_file(fname.c_str());
  }
  if (output_success) {
    VTR_LOGV(options.verbose_output(), "Succeed to output XML file: %s\n", fname.c_str());
  } else {
    VTR_LOG_ERROR("Failed to output XML file: %s\n", fname.c_str());
  }
  return output_success ? CMD_EXEC_SUCCESS : CMD_EXEC_FATAL_ERROR;
}

} /* end namespace openfpga */
