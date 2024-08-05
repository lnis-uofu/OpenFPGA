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

/********************************************************************
 * Parse XML codes of a <instance> to an object of unique_blocks
 *******************************************************************/
static void read_xml_unique_instance_info(pugi::xml_node& xml_instance_info,
                                          const pugiutil::loc_data& loc_data) {
  std::string pass = "pass here";
  std::string instance_x = get_attribute(xml_instance_info, "x", loc_data).as_string();
  std::string instance_y = get_attribute(xml_instance_info, "y", loc_data).as_string();
}

/********************************************************************
 * Parse XML codes about <repack_design_constraints> to an object of
 *RepackDesignConstraints
 *******************************************************************/
int read_xml_unique_blocks(const char* file_name, const char* file_type,
                           bool verbose) {
  vtr::ScopedStartFinishTimer timer("Read unique blocks xml file");

  //   RepackDesignConstraints repack_design_constraints;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, file_name);

    pugi::xml_node xml_root = get_single_child(doc, "unique_blocks", loc_data);

    // size_t num_design_constraints =
    //   std::distance(xml_root.children().begin(), xml_root.children().end());
    // /* Reserve memory space for the region */
    // repack_design_constraints.reserve_design_constraints(
    //   num_design_constraints);

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
            read_xml_unique_instance_info(xml_instance_info, loc_data);
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
