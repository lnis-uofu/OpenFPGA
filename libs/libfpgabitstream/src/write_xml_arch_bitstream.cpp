/********************************************************************
 * This file includes functions that output bitstream database
 * to files in different formats
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
#include "command_exit_codes.h"
#include "bitstream_manager_utils.h"
#include "openfpga_digest.h"
#include "openfpga_gz_xml_writer.h"
#include "openfpga_reserved_words.h"
#include "openfpga_tokenizer.h"
#include "write_xml_arch_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function write header information to a bitstream file
 *******************************************************************/
static void write_bitstream_xml_file_head(pugi::xml_node& parent_node,
                                          const bool& include_time_stamp) {
  std::stringstream ss;
  ss << "<!--" << std::endl;
  ss << "\t- Architecture independent bitstream" << std::endl;
  ss << "\t- Author: Xifan TANG" << std::endl;
  ss << "\t- Organization: University of Utah" << std::endl;

  if (include_time_stamp) {
    auto end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    ss << "\t- Date: " << std::ctime(&end_time);
  }

  ss << "-->" << std::endl;
  ss << std::endl;
  pugi::xml_node cmt_node = root_node.insert_child_before(pugi::node_comment);
  cmt_node.set_value(ss.str().c_str());
}

/********************************************************************
 * Recursively write the bitstream of a block to a xml file
 * This function will use a Depth-First Search in outputting bitstream
 * for each block
 * 1. For block with bits as children, we will output the XML lines
 * 2. For block without bits/child blocks, we can return
 * 3. For block with child blocks, we visit each child recursively
 *******************************************************************/
static void rec_write_block_bitstream_to_xml_file(
  pugi::xml_node& parent_node, const BitstreamManager& bitstream_manager,
  const ConfigBlockId& block, const size_t& hierarchy_level) {
  /* Write the bits of this block. The block name has been created before entering this function */
  parent_node.append_attribute("name").set_value(bitstream_manager.block_name(block).c_str());
  parent_node.append_attribute("hierarchy_level").set_value(static_cast<unsigned long long>(hierarchy_level));
  
  /* Dive to child blocks if this block has any */
  for (const ConfigBlockId& child_block :
       bitstream_manager.block_children(block)) {
    pugi::xml_node blk_node = parent_node.append_child("bitstream_block");
    rec_write_block_bitstream_to_xml_file(blk_node, bitstream_manager, child_block,
                                          hierarchy_level + 1);
  }

  if (0 == bitstream_manager.block_bits(block).size()) {
    return;
  }

  std::vector<ConfigBlockId> block_hierarchy =
    find_bitstream_manager_block_hierarchy(bitstream_manager, block);

  /* Output hierarchy of this parent*/
  pugi::xml_node hie_node = parent_node.append_child("hierarchy");
  size_t hierarchy_counter = 0;
  for (const ConfigBlockId& temp_block : block_hierarchy) {
    pugi::xml_node inst_node = hie_node.append_child("instance");
    inst_node.append_attribute("level").set_value(static_cast<unsigned long long>(hierarchy_counter));
    inst_node.append_attribute("name").set_value(bitstream_manager.block_name(temp_block).c_str());
    hierarchy_counter++;
  }

  /* Output input/output nets if there are any */
  if (false == bitstream_manager.block_input_net_ids(block).empty()) {
    pugi::xml_node input_nets_node = parent_node.append_child("input_nets");
    size_t path_counter = 0;
    /* Split with space */
    StringToken input_net_tokenizer(
      bitstream_manager.block_input_net_ids(block));
    for (const std::string& net : input_net_tokenizer.split(std::string(" "))) {
      pugi::xml_node path_node = input_nets_node.append_child("path");
      path_node.append_attribute("id").set_value(static_cast<unsigned long long>(path_counter));
      path_node.append_attribute("net_name").set_value(net.c_str());
      path_counter++;
    }
  }

  if (false == bitstream_manager.block_output_net_ids(block).empty()) {
    pugi::xml_node output_nets_node = parent_node.append_child("output_nets");
    size_t path_counter = 0;
    /* Split with space */
    StringToken output_net_tokenizer(
      bitstream_manager.block_output_net_ids(block));
    for (const std::string& net :
         output_net_tokenizer.split(std::string(" "))) {
      pugi::xml_node path_node = output_nets_node.append_child("path");
      path_node.append_attribute("id").set_value(static_cast<unsigned long long>(path_counter));
      path_node.append_attribute("net_name").set_value(net.c_str());
      path_counter++;
    }
  }

  /* Output child bits under this block */
  size_t bit_counter = 0;
  pugi::xml_node bits_node = parent_node.append_child("bitstream");
  /* Output path id only when it is valid */
  if (true == bitstream_manager.valid_block_path_id(block)) {
    bits_node.append_attribute("path_id").set_value(
      bitstream_manager.block_path_id(block)
    );
  }

  for (const ConfigBitId& child_bit : bitstream_manager.block_bits(block)) {
    pugi::xml_node bit_node = bits_node.append_child("bit");
    BasicPort mem_port(CONFIGURABLE_MEMORY_DATA_OUT_NAME, bit_counter, bit_counter);
    bit_node.append_attribute("memory_port").set_value(mem_port.to_simple_verilog_string().c_str());
    bit_node.append_attribute("value").set_value(
      bitstream_manager.bit_value(child_bit) ? "1" : "0"
    );
    bit_counter++;
  }
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
int write_xml_architecture_bitstream(const BitstreamManager& bitstream_manager,
                                      const std::string& fname,
                                      const bool& include_time_stamp) {
  /* Ensure that we have a valid file name */
  if (true == fname.empty()) {
    VTR_LOG_ERROR(
      "Received empty file name to output bitstream!\n\tPlease specify a valid "
      "file name.\n");
  }

  std::string timer_message =
    std::string("Write ") + std::to_string(bitstream_manager.bits().size()) +
    std::string(" architecture independent bitstream into XML file '") + fname +
    std::string("'");
  vtr::ScopedStartFinishTimer timer(timer_message);

  pugi::xml_document doc;
  pugi::xml_node root_node = doc.append_child("bitstream_block");
  
  /* Put down a brief introduction */
  write_bitstream_xml_file_head(root_node, include_time_stamp);

  /* Find the top block, which has not parents */
  std::vector<ConfigBlockId> top_block =
    find_bitstream_manager_top_blocks(bitstream_manager);
  /* Make sure we have only 1 top block */
  if (1 != top_block.size()) {
    VTR_LOG_ERROR(
      "Expect only 1 top-level block in FPGA fabric but found %lu\n\tThis is "
      "an internal error. Please report\n",
      top_block.size());
    return CMD_EXEC_FATAL_ERROR;
  }

  /* Write bitstream, block by block, in a recursive way */
  rec_write_block_bitstream_to_xml_file(root_node, bitstream_manager, top_block[0], 0);

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
    VTR_LOG("Succeed to output XML file: %s\n", fname.c_str());
  } else {
    VTR_LOG_ERROR("Failed to output XML file: %s\n", fname.c_str());
  }
  return output_success ? CMD_EXEC_SUCCESS : CMD_EXEC_FATAL_ERROR;
}

} /* end namespace openfpga */
