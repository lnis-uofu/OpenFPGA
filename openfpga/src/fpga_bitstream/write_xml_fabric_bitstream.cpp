/********************************************************************
 * This file includes functions that output a fabric-dependent 
 * bitstream database to files in XML format
 *******************************************************************/
#include <chrono>
#include <ctime>
#include <fstream>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_digest.h"

/* Headers from archopenfpga library */

#include "openfpga_naming.h"

#include "bitstream_manager_utils.h"
#include "write_xml_fabric_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information to a bitstream file
 *******************************************************************/
static 
void write_fabric_bitstream_xml_file_head(std::fstream& fp) {
  valid_file_stream(fp);
 
  auto end = std::chrono::system_clock::now(); 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  fp << "<!--" << std::endl;
  fp << "\t- Fabric bitstream" << std::endl;
  fp << "\t- Author: Xifan TANG" << std::endl;
  fp << "\t- Organization: University of Utah" << std::endl;
  fp << "\t- Date: " << std::ctime(&end_time) ;
  fp << "-->" << std::endl;
  fp << std::endl;
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
static 
int write_fabric_config_bit_to_xml_file(std::fstream& fp,
                                        const BitstreamManager& bitstream_manager,
                                        const FabricBitstream& fabric_bitstream,
                                        const FabricBitId& fabric_bit,
                                        const e_config_protocol_type& config_type) {
  if (false == valid_file_stream(fp)) {
    return 1;
  }

  fp << "<bit id=\"" << size_t(fabric_bit) << "\" ";
  fp << "value=\"";
  fp << bitstream_manager.bit_value(fabric_bitstream.config_bit(fabric_bit));
  fp << "\">\n";

  /* Output hierarchy of this parent*/
  const ConfigBitId& config_bit = fabric_bitstream.config_bit(fabric_bit);
  const ConfigBlockId& config_block = bitstream_manager.bit_parent_block(config_bit);
  std::vector<ConfigBlockId> block_hierarchy = find_bitstream_manager_block_hierarchy(bitstream_manager, config_block); 
  write_tab_to_file(fp, 1);
  fp << "<hierarchy>\n";
  size_t hierarchy_counter = 0;
  for (const ConfigBlockId& temp_block : block_hierarchy) {
    write_tab_to_file(fp, 2);
    fp << "<instance level=\"" << hierarchy_counter << "\"";
    if (0 < bitstream_manager.block_bits(temp_block).size()) {
      fp << " width=\"" << bitstream_manager.block_bits(temp_block).size() << "\"";
    }
    fp << " name=\"" << bitstream_manager.block_name(temp_block) << "\"";
    fp << "/>\n";
    hierarchy_counter++;
  }
  write_tab_to_file(fp, 1);
  fp << "</hierarchy>\n";

  switch (config_type) {
  case CONFIG_MEM_STANDALONE: 
  case CONFIG_MEM_SCAN_CHAIN:
    break;
  case CONFIG_MEM_MEMORY_BANK: { 
    /* Bit line address */
    write_tab_to_file(fp, 1);
    fp << "<bl address=\"";
    for (const char& addr_bit : fabric_bitstream.bit_bl_address(fabric_bit)) {
      fp << addr_bit;
    }
    fp << "\"/>\n";   
 
    write_tab_to_file(fp, 1);
    fp << "<wl address=\"";
    for (const char& addr_bit : fabric_bitstream.bit_wl_address(fabric_bit)) {
      fp << addr_bit;
    }
    fp << "\"/>\n";   
    break;
  }
  case CONFIG_MEM_FRAME_BASED: {
    write_tab_to_file(fp, 1);
    fp << "<frame address=\"";
    for (const char& addr_bit : fabric_bitstream.bit_address(fabric_bit)) {
      fp << addr_bit;
    }
    fp << "\"/>\n";   
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid configuration protocol type!\n");
    return 1;
  }

  fp << "</bit>\n";

  return 0;
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
int write_fabric_bitstream_to_xml_file(const BitstreamManager& bitstream_manager,
                                       const FabricBitstream& fabric_bitstream,
                                       const ConfigProtocol& config_protocol,
                                       const std::string& fname,
                                       const bool& verbose) {
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR("Received empty file name to output bitstream!\n\tPlease specify a valid file name.\n");
  }

  std::string timer_message = std::string("Write ") + std::to_string(fabric_bitstream.num_bits()) + std::string(" fabric bitstream into xml file '") + fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(fname.c_str(), fp);

  /* Write XML head */
  write_fabric_bitstream_xml_file_head(fp);

  /* Output fabric bitstream to the file */
  int status = 0;
  for (const FabricBitId& fabric_bit : fabric_bitstream.bits()) {
    status = write_fabric_config_bit_to_xml_file(fp, bitstream_manager,
                                                 fabric_bitstream,
                                                 fabric_bit,
                                                 config_protocol.type());
    if (1 == status) {
      break;
    }
  }
  /* Print an end to the file here */
  fp << std::endl;

  /* Close file handler */
  fp.close();

  VTR_LOGV(verbose,
           "Outputted %lu configuration bits to XML file: %s\n",
           fabric_bitstream.bits().size(),
           fname.c_str());

  return status;
}

} /* end namespace openfpga */
