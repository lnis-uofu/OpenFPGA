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
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_unique_blocks.h"
#include "read_xml_util.h"

/********************************************************************
 * Parse XML codes of a <key> to an object of FabricKey
 *******************************************************************/
static void read_xml_unique_block_info(
  pugi::xml_node& xml_pin_constraint, const pugiutil::loc_data& loc_data) {
    std::string pass = "pass here";
//   /* Create a new design constraint in the storage */
//   RepackDesignConstraintId design_constraint_id =
//     repack_design_constraints.create_design_constraint(
//       RepackDesignConstraints::IGNORE_NET);

//   if (false == repack_design_constraints.valid_design_constraint_id(
//                  design_constraint_id)) {
//     archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_pin_constraint),
//                    "Fail to create design constraint!\n");
//   }

//   std::string pin_ctx_to_parse =
//     get_attribute(xml_pin_constraint, "pin", loc_data).as_string();
//   openfpga::StringToken pin_tokenizer(pin_ctx_to_parse);
//   std::vector<std::string> pin_info = pin_tokenizer.split('.');
//   /* Expect two contents, otherwise error out */
//   if (pin_info.size() != 2) {
//     std::string err_msg =
//       std::string("Invalid content '") + pin_ctx_to_parse +
//       std::string("' to skip, expect <pb_type_name>.<pin>\n");
//     VTR_LOG_ERROR(err_msg.c_str());
//     VTR_ASSERT(pin_info.size() == 2);
//   }
//   std::string pb_type_name = pin_info[0];
//   openfpga::PortParser port_parser(pin_info[1]);
//   openfpga::BasicPort curr_port = port_parser.port();
//   if (!curr_port.is_valid()) {
//     std::string err_msg =
//       std::string("Invalid pin definition '") + pin_ctx_to_parse +
//       std::string("', expect <pb_type_name>.<pin_name>[int:int]\n");
//     VTR_LOG_ERROR(err_msg.c_str());
//     VTR_ASSERT(curr_port.is_valid());
//   }
//   repack_design_constraints.set_pb_type(design_constraint_id, pb_type_name);
//   repack_design_constraints.set_pin(design_constraint_id, curr_port);
//   repack_design_constraints.set_net(
//     design_constraint_id,
//     get_attribute(xml_pin_constraint, "name", loc_data).as_string());
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

    pugi::xml_node xml_root =
      get_single_child(doc, "unique_blocks", loc_data);

    // size_t num_design_constraints =
    //   std::distance(xml_root.children().begin(), xml_root.children().end());
    // /* Reserve memory space for the region */
    // repack_design_constraints.reserve_design_constraints(
    //   num_design_constraints);

    for (pugi::xml_node xml_block_info : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_block_info.name() == std::string("block")) {
        read_xml_unique_block_info(xml_block_info, loc_data);
      } else {
        bad_tag(xml_block_info, loc_data, xml_root,
                {"block"});
        return 1;
      }
    }
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(file_name, e.line(), "%s", e.what());
  }

  return 0;
}
