/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of a fabric key to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "openfpga_reserved_words.h"

#include "read_xml_arch_bitstream.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Parse XML codes of a <bitstream_block> to an object of BitstreamManager
 * This function goes recursively until we reach the leaf node
 *******************************************************************/
static 
void rec_read_xml_bitstream_block(pugi::xml_node& xml_bitstream_block,
                                  const pugiutil::loc_data& loc_data,
                                  BitstreamManager& bitstream_manager,
                                  const ConfigBlockId& parent_block) {

  /* Find the name of this bitstream block */
  const std::string& block_name = get_attribute(xml_bitstream_block, "name", loc_data).as_string();

  /* Create the bitstream block */
  ConfigBlockId curr_block = bitstream_manager.add_block(block_name);

  /* Add it to parent block */
  bitstream_manager.add_child_block(parent_block, curr_block);

  /* Parse input nets if defined */
  pugi::xml_node xml_input_nets = get_single_child(xml_bitstream_block, "input_nets", loc_data, pugiutil::ReqOpt::OPTIONAL);
  if (xml_input_nets) {
    std::vector<std::string> input_nets;
    size_t num_input_nets = count_children(xml_input_nets, "path", loc_data, pugiutil::ReqOpt::OPTIONAL);
    input_nets.resize(num_input_nets);

    /* Find the child paths/nets */
    for (pugi::xml_node xml_input_net : xml_input_nets.children()) {
      /* We only care child bitstream blocks here */
      if (xml_input_net.name() != std::string("path")) {
        bad_tag(xml_input_net, loc_data, xml_input_nets, {"path"});
      }
      const int& id = get_attribute(xml_input_net, "id", loc_data).as_int();
      const std::string& net_name = get_attribute(xml_input_net, "name", loc_data).as_string();
      VTR_ASSERT((size_t)id < input_nets.size());
      input_nets[id] = net_name; 
    } 

    for (const std::string& input_net : input_nets) {
      bitstream_manager.add_input_net_id_to_block(curr_block, input_net); 
    }
  }

  /* Parse output nets if defined */
  pugi::xml_node xml_output_nets = get_single_child(xml_bitstream_block, "output_nets", loc_data, pugiutil::ReqOpt::OPTIONAL);
  if (xml_output_nets) {
    std::vector<std::string> output_nets;
    size_t num_output_nets = count_children(xml_output_nets, "path", loc_data, pugiutil::ReqOpt::OPTIONAL);
    output_nets.resize(num_output_nets);

    /* Find the child paths/nets */
    for (pugi::xml_node xml_output_net : xml_output_nets.children()) {
      /* We only care child bitstream blocks here */
      if (xml_output_net.name() != std::string("path")) {
        bad_tag(xml_output_net, loc_data, xml_output_nets, {"path"});
      }
      const int& id = get_attribute(xml_output_net, "id", loc_data).as_int();
      const std::string& net_name = get_attribute(xml_output_net, "name", loc_data).as_string();
      VTR_ASSERT((size_t)id < output_nets.size());
      output_nets[id] = net_name; 
    } 

    for (const std::string& output_net : output_nets) {
      bitstream_manager.add_output_net_id_to_block(curr_block, output_net); 
    }
  }

  /* Parse configuration bits */
  pugi::xml_node xml_bitstream = get_single_child(xml_bitstream_block, "bitstream", loc_data, pugiutil::ReqOpt::OPTIONAL);
  if (xml_bitstream) {
    /* Parse path_id: -2 is an invalid value defined in the bitstream manager internally */
    const int& path_id = get_attribute(xml_bitstream, "path_id", loc_data).as_int();
    if (-2 < path_id) {
      bitstream_manager.add_path_id_to_block(curr_block, path_id); 
    }

    /* Find the child paths/nets */
    for (pugi::xml_node xml_bit : xml_bitstream.children()) {
      /* We only care child bitstream blocks here */
      if (xml_bit.name() != std::string("bit")) {
        bad_tag(xml_bit, loc_data, xml_bitstream, {"bit"});
      }
      const int& bit_value = get_attribute(xml_bit, "value", loc_data).as_int();
      ConfigBitId bit = bitstream_manager.add_bit(1 == bit_value);
      /* Link the bit to parent block */
      bitstream_manager.add_bit_to_block(curr_block, bit);
    } 
  }
  
  /* Go recursively: find all the child blocks and parse */
  for (pugi::xml_node xml_child : xml_bitstream_block.children()) {
    /* We only care child bitstream blocks here */
    if (xml_child.name() == std::string("bitstream_block")) {
      rec_read_xml_bitstream_block(xml_bitstream_block, loc_data, bitstream_manager, curr_block);
    }
  } 
}

/********************************************************************
 * Parse XML codes about <bitstream> to an object of Bitstream
 *******************************************************************/
BitstreamManager read_xml_architecture_bitstream(const char* fname) {

  vtr::ScopedStartFinishTimer timer("Read Architecture Bitstream file");

  BitstreamManager bitstream_manager;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    /* Count the child <bitstream_block> */

    pugi::xml_node xml_root = get_single_child(doc, "bitstream_block", loc_data);

    /* Find the name of the top block*/
    const std::string& top_block_name = get_attribute(xml_root, "name", loc_data).as_string();
    
    if (top_block_name != std::string(FPGA_TOP_MODULE_NAME)) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_root),
                     "Top-level block must be named as '%s'!\n",
                     FPGA_TOP_MODULE_NAME);
    }

    /* Create the top-level block */
    ConfigBlockId top_block = bitstream_manager.add_block(top_block_name);

    size_t num_blks = count_children(xml_root, "bitstream_block", loc_data, pugiutil::ReqOpt::OPTIONAL);

    /* Reserve bitstream blocks in the data base */
    bitstream_manager.reserve_blocks(num_blks);

    /* Iterate over the children under this node,
     * each child should be named after circuit_model
     */
    for (pugi::xml_node xml_blk : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_blk.name() != std::string("bitstream_block")) {
        bad_tag(xml_blk, loc_data, xml_root, {"bitstream_block"});
      }
      rec_read_xml_bitstream_block(xml_blk, loc_data, bitstream_manager, top_block);
    } 
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(fname, e.line(),
                   "%s", e.what());
  }

  return bitstream_manager; 
}

} /* end namespace openfpga */

