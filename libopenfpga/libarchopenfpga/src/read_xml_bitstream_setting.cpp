/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"

/* Headers from openfpga util library */
#include "openfpga_pb_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_bitstream_setting.h"

/********************************************************************
 * Parse XML description for a pb_type annotation under a <pb_type> XML node
 *******************************************************************/
static 
void read_xml_bitstream_pb_type_setting(pugi::xml_node& xml_pb_type,
                                        const pugiutil::loc_data& loc_data,
                                        openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& name_attr = get_attribute(xml_pb_type, "name", loc_data).as_string();
  const std::string& source_attr = get_attribute(xml_pb_type, "source", loc_data).as_string();
  const std::string& content_attr = get_attribute(xml_pb_type, "content", loc_data).as_string();

  /* Parse the attributes for operating pb_type */
  openfpga::PbParser operating_pb_parser(name_attr);

  /* Add to bitstream setting */
  bitstream_setting.add_bitstream_pb_type_setting(operating_pb_parser.leaf(),
                                                  operating_pb_parser.parents(),
                                                  operating_pb_parser.modes(),
                                                  source_attr,
                                                  content_attr);
}

/********************************************************************
 * Parse XML codes about <openfpga_bitstream_setting> to an object
 *******************************************************************/
openfpga::BitstreamSetting read_xml_bitstream_setting(pugi::xml_node& Node,
                                                      const pugiutil::loc_data& loc_data) {
  openfpga::BitstreamSetting bitstream_setting;

  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_pb_type : Node.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_pb_type.name() != std::string("pb_type")) {
      bad_tag(xml_pb_type, loc_data, Node, {"pb_type"});
    }
    read_xml_bitstream_pb_type_setting(xml_pb_type, loc_data, bitstream_setting);
  } 

  return bitstream_setting;
}
