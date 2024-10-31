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
#include "read_xml_bitstream_setting.h"
#include "read_xml_util.h"

/********************************************************************
 * Parse XML description for a pb_type annotation under a <pb_type> XML node
 *******************************************************************/
static void read_xml_bitstream_pb_type_setting(
  pugi::xml_node& xml_pb_type, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& name_attr =
    get_attribute(xml_pb_type, "name", loc_data).as_string();
  const std::string& source_attr =
    get_attribute(xml_pb_type, "source", loc_data).as_string();
  const std::string& content_attr =
    get_attribute(xml_pb_type, "content", loc_data).as_string();

  /* Parse the attributes for operating pb_type */
  openfpga::PbParser operating_pb_parser(name_attr);

  /* Add to bitstream setting */
  BitstreamPbTypeSettingId bitstream_pb_type_id =
    bitstream_setting.add_bitstream_pb_type_setting(
      operating_pb_parser.leaf(), operating_pb_parser.parents(),
      operating_pb_parser.modes(), source_attr, content_attr);

  /* Parse if the bitstream overwritting is applied to mode bits of a pb_type */
  const bool& is_mode_select_bitstream =
    get_attribute(xml_pb_type, "is_mode_select_bitstream", loc_data,
                  pugiutil::ReqOpt::OPTIONAL)
      .as_bool(false);
  bitstream_setting.set_mode_select_bitstream(bitstream_pb_type_id,
                                              is_mode_select_bitstream);

  const int& offset = get_attribute(xml_pb_type, "bitstream_offset", loc_data,
                                    pugiutil::ReqOpt::OPTIONAL)
                        .as_int(0);
  bitstream_setting.set_bitstream_offset(bitstream_pb_type_id, offset);
}

/********************************************************************
 * Parse XML description for a pb_type annotation under a <interconect> XML node
 *******************************************************************/
static void read_xml_bitstream_interconnect_setting(
  pugi::xml_node& xml_pb_type, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& name_attr =
    get_attribute(xml_pb_type, "name", loc_data).as_string();
  const std::string& default_path_attr =
    get_attribute(xml_pb_type, "default_path", loc_data).as_string();

  /* Parse the attributes for operating pb_type */
  openfpga::PbParser operating_pb_parser(name_attr);

  /* Add to bitstream setting */
  bitstream_setting.add_bitstream_interconnect_setting(
    operating_pb_parser.leaf(), operating_pb_parser.parents(),
    operating_pb_parser.modes(), default_path_attr);
}

/********************************************************************
 * Parse XML description for a non_fabric annotation under a <non_fabric> XML
 *node
 *******************************************************************/
static void read_xml_non_fabric_bitstream_setting(
  pugi::xml_node& xml_non_fabric, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& name_attr =
    get_attribute(xml_non_fabric, "name", loc_data).as_string();
  const std::string& file_attr =
    get_attribute(xml_non_fabric, "file", loc_data).as_string();
  /* Add to non-fabric */
  bitstream_setting.add_non_fabric(name_attr, file_attr);
  for (pugi::xml_node xml_child : xml_non_fabric.children()) {
    if (xml_child.name() != std::string("pb")) {
      bad_tag(xml_child, loc_data, xml_non_fabric, {"pb"});
    }
    const std::string& pb_name_attr =
      get_attribute(xml_child, "name", loc_data).as_string();
    const std::string& content_attr =
      get_attribute(xml_child, "content", loc_data).as_string();
    /* Add PB to non-fabric */
    bitstream_setting.add_non_fabric_pb(pb_name_attr, content_attr);
  }
}

/********************************************************************
 * Parse XML description for a bit setting under a <bit> XML node
 *******************************************************************/
static void read_xml_overwrite_bitstream_setting(
  pugi::xml_node& xml_overwrite_bitstream, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  // Loopthrough bit
  for (pugi::xml_node xml_bit : xml_overwrite_bitstream.children()) {
    if (xml_bit.name() != std::string("bit")) {
      bad_tag(xml_bit, loc_data, xml_overwrite_bitstream, {"bit"});
    }
    const std::string& path_attr =
      get_attribute(xml_bit, "path", loc_data).as_string();
    const std::string& value_attr =
      get_attribute(xml_bit, "value", loc_data).as_string();
    if (value_attr != "0" && value_attr != "1") {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_bit),
                     "Invalid value of overwrite_bitstream bit. Expect [0|1]");
    }
    /* Add to bit */
    bitstream_setting.add_overwrite_bitstream(path_attr, value_attr == "1");
  }
}

/********************************************************************
 * Parse XML codes about <openfpga_bitstream_setting> to an object
 *******************************************************************/
openfpga::BitstreamSetting read_xml_bitstream_setting(
  pugi::xml_node& Node, const pugiutil::loc_data& loc_data) {
  openfpga::BitstreamSetting bitstream_setting;

  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_child : Node.children()) {
    /* Error out if the XML child has an invalid name! */
    if ((xml_child.name() != std::string("pb_type")) &&
        (xml_child.name() != std::string("interconnect")) &&
        (xml_child.name() != std::string("non_fabric")) &&
        (xml_child.name() != std::string("overwrite_bitstream"))) {
      bad_tag(xml_child, loc_data, Node,
              {"pb_type | interconnect | non_fabric | overwrite_bitstream"});
    }

    if (xml_child.name() == std::string("pb_type")) {
      read_xml_bitstream_pb_type_setting(xml_child, loc_data,
                                         bitstream_setting);
    } else if (xml_child.name() == std::string("interconnect")) {
      read_xml_bitstream_interconnect_setting(xml_child, loc_data,
                                              bitstream_setting);
    } else if (xml_child.name() == std::string("non_fabric")) {
      read_xml_non_fabric_bitstream_setting(xml_child, loc_data,
                                            bitstream_setting);
    } else {
      VTR_ASSERT_SAFE(xml_child.name() == std::string("overwrite_bitstream"));
      read_xml_overwrite_bitstream_setting(xml_child, loc_data,
                                           bitstream_setting);
    }
  }

  return bitstream_setting;
}
