/********************************************************************
 * This file includes functions that output bitstream database
 * to files in different formats
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

#include "openfpga_naming.h"

#include "bitstream_manager_utils.h"
#include "arch_bitstream_writer.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information to a bitstream file
 *******************************************************************/
static 
void write_bitstream_xml_file_head(std::fstream& fp) {
  valid_file_stream(fp);
 
  auto end = std::chrono::system_clock::now(); 
  std::time_t end_time = std::chrono::system_clock::to_time_t(end);

  fp << "<!--" << std::endl;
  fp << "\t- Architecture independent bitstream" << std::endl;
  fp << "\t- Author: Xifan TANG" << std::endl;
  fp << "\t- Organization: University of Utah" << std::endl;
  fp << "\t- Date: " << std::ctime(&end_time) ;
  fp << "-->" << std::endl;
  fp << std::endl;
}

/********************************************************************
 * Recursively write the bitstream of a block to a xml file
 * This function will use a Depth-First Search in outputting bitstream
 * for each block 
 * 1. For block with bits as children, we will output the XML lines
 * 2. For block without bits/child blocks, we can return 
 * 3. For block with child blocks, we visit each child recursively
 *******************************************************************/
static 
void rec_write_block_bitstream_to_xml_file(std::fstream& fp,
                                           const BitstreamManager& bitstream_manager, 
                                           const ConfigBlockId& block) {
  valid_file_stream(fp);

  /* Dive to child blocks if this block has any */
  for (const ConfigBlockId& child_block : bitstream_manager.block_children(block)) {
    rec_write_block_bitstream_to_xml_file(fp, bitstream_manager, child_block);
  }
  
  if (0 == bitstream_manager.block_bits(block).size()) {
    return;
  }

  /* Write the bits of this block */
  fp << "<bitstream_block index=\"" << size_t(block) << "\">" << std::endl;

  std::vector<ConfigBlockId> block_hierarchy = find_bitstream_manager_block_hierarchy(bitstream_manager, block); 
  
  /* Output hierarchy of this parent*/
  fp << "\t<hierarchy>" << std::endl;
  size_t hierarchy_counter = 0;
  for (const ConfigBlockId& temp_block : block_hierarchy) {
    fp << "\t\t<instance level=\"" << hierarchy_counter << "\"";
    fp << " name=\"" << bitstream_manager.block_name(temp_block) << "\"";
    fp << "/>" << std::endl;
    hierarchy_counter++;
  }
  fp << "\t</hierarchy>" << std::endl;

  /* Output child bits under this block */
  size_t bit_counter = 0;
  fp << "\t<bitstream";
  /* Output path id only when it is valid */
  if (true == bitstream_manager.valid_block_path_id(block)) {
    fp << " path_id=\"" << bitstream_manager.block_path_id(block) << "\"";
  }
  fp << ">" << std::endl;
  for (const ConfigBitId& child_bit : bitstream_manager.block_bits(block)) {
    fp << "\t\t<bit";
    fp << " memory_port=\"" << generate_configurable_memory_data_out_name() << "[" << bit_counter << "]" << "\"";
    fp << " value=\"" << bitstream_manager.bit_value(child_bit) << "\"";
    fp << "/>" << std::endl;
    bit_counter++;
  }
  fp << "\t</bitstream>" << std::endl;

  fp << "</bitstream_block>" <<std::endl;
}

/********************************************************************
 * Write the bitstream to a file without binding to the configuration 
 * procotols of a given FPGA fabric in XML format
 *
 * Notes: 
 * This is a very generic representation for bitstream that are implemented
 * by VPR engine. It shows the bitstream for each blocks in the FPGA 
 * architecture that users are modeling.
 * This function can be used to: 
 * 1. Debug the bitstream decoding to see if there is any bug
 * 2. Create an intermediate file to reorganize a bitstream for 
 *    specific FPGAs
 * 3. TODO: support FASM format 
 *******************************************************************/
void write_arch_independent_bitstream_to_xml_file(const BitstreamManager& bitstream_manager,
                                                  const std::string& fname) {
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR("Received empty file name to output bitstream!\n\tPlease specify a valid file name.\n");
  }

  std::string timer_message = std::string("Write ") + std::to_string(bitstream_manager.bits().size()) + std::string(" architecture independent bitstream into XML file '") + fname + std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  /* Create the file stream */
  std::fstream fp;
  fp.open(fname, std::fstream::out | std::fstream::trunc);

  check_file_stream(fname.c_str(), fp);

  /* Put down a brief introduction */
  write_bitstream_xml_file_head(fp);

  std::string top_block_name = generate_fpga_top_module_name();
  /* Find the top block, which has not parents */
  std::vector<ConfigBlockId> top_block = find_bitstream_manager_top_blocks(bitstream_manager);
  /* Make sure we have only 1 top block and its name matches the top module */
  VTR_ASSERT(1 == top_block.size());
  VTR_ASSERT(0 == top_block_name.compare(bitstream_manager.block_name(top_block[0])));

  /* Write bitstream, block by block, in a recursive way */
  rec_write_block_bitstream_to_xml_file(fp, bitstream_manager, top_block[0]);

  /* Close file handler */
  fp.close();
}

} /* end namespace openfpga */
