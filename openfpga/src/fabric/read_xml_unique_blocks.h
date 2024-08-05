#ifndef READ_XML_UNIQUE_BLOCKS_H
#define READ_XML_UNIQUE_BLOCKS_H

/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of unique routing blocks to the associated
 * data structures
 *******************************************************************/

#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_unique_blocks.h"
#include "read_xml_util.h"
#include "rr_gsb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/
template <class T>
int read_xml_unique_blocks(T& openfpga_ctx, const char* file_name,
                           const char* file_type, bool verbose);

/********************************************************************
 * Parse XML codes of a <instance> to an object of unique_blocks
 *******************************************************************/
template <class T>
void read_xml_unique_instance_info(T& device_rr_gsb,
                                   pugi::xml_node& xml_instance_info,
                                   const pugiutil::loc_data& loc_data,
                                   std::string type) {
  std::string pass = "pass here";
  int instance_x = get_attribute(xml_instance_info, "x", loc_data).as_int();
  int instance_y = get_attribute(xml_instance_info, "y", loc_data).as_int();
  if (type == "sb") {
    device_rr_gsb.load_unique_sb_module_from_user_input(instance_x, instance_y);
  } else if (type == "cb") {
    // read_cb_unique_blocks();
    std::cout << "By pass here" << std::endl;
  } else if (type == "gsb") {
    std::cout << "By pass here" << std::endl;
    // read_gsb_unique_blocks();
  }
}

/********************************************************************
 * Parse XML codes about <repack_design_constraints> to an object of
 *RepackDesignConstraints
 *******************************************************************/
template <class T>
int read_xml_unique_blocks(T& openfpga_ctx, const char* file_name,
                           const char* file_type, bool verbose) {
  vtr::ScopedStartFinishTimer timer("Read unique blocks xml file");

  //   RepackDesignConstraints repack_design_constraints;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, file_name);

    pugi::xml_node xml_root = get_single_child(doc, "unique_blocks", loc_data);

    /* get device_rr_gsb data type and initialize it*/
    auto device_rr_gsb = openfpga_ctx.mutable_device_rr_gsb();
    device_rr_gsb.clear();

    /* load unique blocks xml file and set up device_rr_gdb */
    for (pugi::xml_node xml_block_info : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_block_info.name() == std::string("block")) {
        std::string type =
          get_attribute(xml_block_info, "type", loc_data).as_string();
        std::string block_x =
          get_attribute(xml_block_info, "x", loc_data).as_string();
        std::string block_y =
          get_attribute(xml_block_info, "y", loc_data).as_string();
        for (pugi::xml_node xml_instance_info : xml_block_info.children()) {
          if (xml_instance_info.name() == std::string("instance")) {
            read_xml_unique_instance_info(device_rr_gsb, xml_instance_info,
                                          loc_data, type);
          }
          // read_xml_unique_instance_info(xml_instance_info, loc_data);
        }
      } else {
        bad_tag(xml_block_info, loc_data, xml_root, {"block"});
        return 1;
      }
      // std::cout << "what is the root name: " << xml_block_info.name() <<
      // std::endl;
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(file_name, e.line(), "%s", e.what());
  }

  return 0;
}

#endif
