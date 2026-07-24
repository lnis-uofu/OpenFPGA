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
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "bitstream_setting_xml_constants.h"
#include "command_exit_codes.h"
#include "read_xml_bitstream_setting.h"
#include "read_xml_openfpga_arch_utils.h"
#include "read_xml_util.h"
/********************************************************************
 * Parse XML description for a pb_type annotation under a <pb_type> XML node
 *******************************************************************/
static void read_xml_bitstream_pb_type_setting(
  pugi::xml_node& xml_pb_type, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& name_attr =
    get_attribute(xml_pb_type, XML_PB_TYPE_ATTRIBUTE_NAME, loc_data)
      .as_string();
  const std::string& source_attr =
    get_attribute(xml_pb_type, XML_PB_TYPE_ATTRIBUTE_SOURCE, loc_data)
      .as_string();
  const std::string& content_attr =
    get_attribute(xml_pb_type, XML_PB_TYPE_ATTRIBUTE_CONTENT, loc_data)
      .as_string();

  /* Parse the attributes for operating pb_type */
  openfpga::PbParser operating_pb_parser(name_attr);

  /* Add to bitstream setting */
  BitstreamPbTypeSettingId bitstream_pb_type_id =
    bitstream_setting.add_bitstream_pb_type_setting(
      operating_pb_parser.leaf(), operating_pb_parser.parents(),
      operating_pb_parser.modes(), source_attr, content_attr);

  /* Parse if the bitstream overwritting is applied to mode bits of a pb_type */
  const bool& is_mode_select_bitstream =
    get_attribute(xml_pb_type, XML_PB_TYPE_ATTRIBUTE_IS_MODE_SELECT_BITSTREAM,
                  loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_bool(false);
  bitstream_setting.set_mode_select_bitstream(bitstream_pb_type_id,
                                              is_mode_select_bitstream);

  const int& offset =
    get_attribute(xml_pb_type, XML_PB_TYPE_ATTRIBUTE_BITSTREAM_OFFSET, loc_data,
                  pugiutil::ReqOpt::OPTIONAL)
      .as_int(0);
  bitstream_setting.set_bitstream_offset(bitstream_pb_type_id, offset);
}

/********************************************************************
 * Parse XML description for a pb_type annotation under a <default_mode_bits>
 *XML node
 *******************************************************************/
static void read_xml_bitstream_default_mode_setting(
  pugi::xml_node& xml_pb_type, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& name_attr =
    get_attribute(xml_pb_type, XML_DEFAULT_MODE_BITS_ATTRIBUTE_NAME, loc_data)
      .as_string();
  /* Parse the attributes for operating pb_type */
  openfpga::PbParser operating_pb_parser(name_attr);

  const std::string& mode_bits_attr =
    get_attribute(xml_pb_type, XML_DEFAULT_MODE_BITS_ATTRIBUTE_MODE_BITS,
                  loc_data)
      .as_string();
  /* Operating pb_type always allow dont care bits */
  std::vector<char> mode_bits =
    parse_mode_bits(xml_pb_type, loc_data, mode_bits_attr, true);

  /* Add to bitstream setting */
  bitstream_setting.add_bitstream_default_mode_setting(
    operating_pb_parser.leaf(), operating_pb_parser.parents(),
    operating_pb_parser.modes(), mode_bits);
}

/********************************************************************
 * Parse XML description for a pb_type annotation under a <default_mode_bits>
 *XML node
 *******************************************************************/
static void read_xml_bitstream_clock_routing_setting(
  pugi::xml_node& xml_clk_routing, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& ntwk_attr =
    get_attribute(xml_clk_routing, XML_CLOCK_ROUTING_ATTRIBUTE_NETWORK,
                  loc_data)
      .as_string();

  const std::string& pin_attr =
    get_attribute(xml_clk_routing, XML_CLOCK_ROUTING_ATTRIBUTE_PIN, loc_data)
      .as_string();
  /* Parse the port and apply sanity checks */
  openfpga::PortParser port_parser(pin_attr);
  openfpga::BasicPort pin = port_parser.port();
  if (!pin.is_valid()) {
    archfpga_throw(
      loc_data.filename_c_str(), loc_data.line(xml_clk_routing),
      "Invalid pin '%s' which should be valid port. For example, clk[1:1]\n",
      pin_attr.c_str());
  }
  if (1 != pin.get_width()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_clk_routing),
                   "Invalid pin '%s' with a width of '%lu'. Only allow pin "
                   "definition with width of 1. For example, clk[2:2]\n",
                   pin_attr.c_str(), pin.get_width());
  }

  /* Add to bitstream setting */
  bitstream_setting.add_bitstream_clock_routing_setting(ntwk_attr, pin);
}

/********************************************************************
 * Parse XML description for a pb_type annotation under a <interconect> XML node
 *******************************************************************/
static void read_xml_bitstream_interconnect_setting(
  pugi::xml_node& xml_pb_type, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& name_attr =
    get_attribute(xml_pb_type, XML_INTERCONNECT_ATTRIBUTE_NAME, loc_data)
      .as_string();
  const std::string& default_path_attr =
    get_attribute(xml_pb_type, XML_INTERCONNECT_ATTRIBUTE_DEFAULT_PATH,
                  loc_data)
      .as_string();

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
    get_attribute(xml_non_fabric, XML_NON_FABRIC_ATTRIBUTE_NAME, loc_data)
      .as_string();
  const std::string& file_attr =
    get_attribute(xml_non_fabric, XML_NON_FABRIC_ATTRIBUTE_FILE, loc_data)
      .as_string();
  /* Add to non-fabric */
  bitstream_setting.add_non_fabric(name_attr, file_attr);
  for (pugi::xml_node xml_child : xml_non_fabric.children()) {
    if (xml_child.name() != std::string(XML_NON_FABRIC_PB_NODE_NAME)) {
      bad_tag(xml_child, loc_data, xml_non_fabric,
              {XML_NON_FABRIC_PB_NODE_NAME});
    }
    const std::string& pb_name_attr =
      get_attribute(xml_child, XML_NON_FABRIC_PB_ATTRIBUTE_NAME, loc_data)
        .as_string();
    const std::string& content_attr =
      get_attribute(xml_child, XML_NON_FABRIC_PB_ATTRIBUTE_CONTENT, loc_data)
        .as_string();
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
    if (xml_bit.name() != std::string(XML_OVERWRITE_BITSTREAM_ATTRIBUTE_BIT)) {
      bad_tag(xml_bit, loc_data, xml_overwrite_bitstream,
              {XML_OVERWRITE_BITSTREAM_ATTRIBUTE_BIT});
    }
    const std::string& path_attr =
      get_attribute(xml_bit, XML_OVERWRITE_BITSTREAM_ATTRIBUTE_PATH, loc_data)
        .as_string();
    const std::string& value_attr =
      get_attribute(xml_bit, XML_OVERWRITE_BITSTREAM_ATTRIBUTE_VALUE, loc_data)
        .as_string();
    if (value_attr != "0" && value_attr != "1") {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_bit),
                     "Invalid value of overwrite_bitstream bit. Expect [0|1]");
    }
    /* Add to bit */
    bitstream_setting.add_overwrite_bitstream(path_attr, value_attr == "1");
  }
}

/********************************************************************
 * Parse a bare range string such as "[0:15]" into a BasicPort.
 *******************************************************************/
static openfpga::BasicPort parse_mif_range_attribute(
  const std::string& range_attr, const pugi::xml_node& xml_node,
  const pugiutil::loc_data& loc_data, const char* attr_name) {
  openfpga::PortParser port_parser(range_attr,
                                   openfpga::PORT_PARSER_SUPPORT_ALL_FORMAT,
                                   true /* only_range */);
  openfpga::BasicPort range_port = port_parser.port();
  if (!port_parser.valid() || !range_port.is_valid()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_node),
                   "Invalid %s='%s'. Expect a decimal range like [0:15]\n",
                   attr_name, range_attr.c_str());
  }
  return range_port;
}

/********************************************************************
 * Parse XML description for a <mif_source> XML node
 *******************************************************************/
static void read_xml_mif_source_setting(
  pugi::xml_node& xml_mif_source, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& pb_type_attr =
    get_attribute(xml_mif_source, XML_MIF_SOURCE_ATTRIBUTE_PB_TYPE, loc_data)
      .as_string();
  const std::string& source_attr =
    get_attribute(xml_mif_source, XML_MIF_SOURCE_ATTRIBUTE_SOURCE, loc_data)
      .as_string();
  const std::string& content_attr =
    get_attribute(xml_mif_source, XML_MIF_SOURCE_ATTRIBUTE_CONTENT, loc_data)
      .as_string();
  const std::string& address_range_attr =
    get_attribute(xml_mif_source, XML_MIF_SOURCE_ATTRIBUTE_ADDRESS_RANGE,
                  loc_data)
      .as_string();
  const std::string& data_range_attr =
    get_attribute(xml_mif_source, XML_MIF_SOURCE_ATTRIBUTE_DATA_RANGE, loc_data)
      .as_string();

  const openfpga::BasicPort address_range =
    parse_mif_range_attribute(address_range_attr, xml_mif_source, loc_data,
                              XML_MIF_SOURCE_ATTRIBUTE_ADDRESS_RANGE);
  const openfpga::BasicPort data_range =
    parse_mif_range_attribute(data_range_attr, xml_mif_source, loc_data,
                              XML_MIF_SOURCE_ATTRIBUTE_DATA_RANGE);

  bitstream_setting.add_mif_source_setting(
    pb_type_attr, source_attr, content_attr, address_range, data_range);
}

/********************************************************************
 * Parse XML description for a <map> child under <mif_address_map>
 *******************************************************************/
static void read_xml_mif_address_map_rule(
  pugi::xml_node& xml_map_rule, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting,
  const MifAddressMapSettingId& map_id,
  const openfpga::BasicPort& src_address_range) {
  const std::string& src_addr_range_attr =
    get_attribute(xml_map_rule,
                  XML_MIF_ADDRESS_MAP_RULE_ATTRIBUTE_SRC_ADDR_RANGE, loc_data)
      .as_string();
  const int des_addr_offset =
    get_attribute(xml_map_rule,
                  XML_MIF_ADDRESS_MAP_RULE_ATTRIBUTE_DES_ADDR_OFFSET, loc_data)
      .as_int();
  const std::string& src_mif_bits_attr =
    get_attribute(xml_map_rule, XML_MIF_ADDRESS_MAP_RULE_ATTRIBUTE_SRC_MIF_BITS,
                  loc_data)
      .as_string();
  const std::string& des_mif_bits_attr =
    get_attribute(xml_map_rule, XML_MIF_ADDRESS_MAP_RULE_ATTRIBUTE_DES_MIF_BITS,
                  loc_data)
      .as_string();

  const openfpga::BasicPort src_addr_range = parse_mif_range_attribute(
    src_addr_range_attr, xml_map_rule, loc_data,
    XML_MIF_ADDRESS_MAP_RULE_ATTRIBUTE_SRC_ADDR_RANGE);
  const openfpga::BasicPort src_mif_bits =
    parse_mif_range_attribute(src_mif_bits_attr, xml_map_rule, loc_data,
                              XML_MIF_ADDRESS_MAP_RULE_ATTRIBUTE_SRC_MIF_BITS);
  const openfpga::BasicPort des_mif_bits =
    parse_mif_range_attribute(des_mif_bits_attr, xml_map_rule, loc_data,
                              XML_MIF_ADDRESS_MAP_RULE_ATTRIBUTE_DES_MIF_BITS);

  if (!src_address_range.contained(src_addr_range)) {
    archfpga_throw(
      loc_data.filename_c_str(), loc_data.line(xml_map_rule),
      "src_addr_range='%s' is outside the address_range '[%zu:%zu]' defined "
      "by mif_source for the same src_pb_type\n",
      src_addr_range_attr.c_str(), src_address_range.get_lsb(),
      src_address_range.get_msb());
  }

  bitstream_setting.add_mif_address_map_rule(
    map_id, src_addr_range, des_addr_offset, src_mif_bits, des_mif_bits);
}

/********************************************************************
 * Parse XML description for a <mif_address_map> XML node
 *******************************************************************/
static void read_xml_mif_address_map_setting(
  pugi::xml_node& xml_mif_address_map, const pugiutil::loc_data& loc_data,
  openfpga::BitstreamSetting& bitstream_setting) {
  const std::string& src_pb_type_attr =
    get_attribute(xml_mif_address_map,
                  XML_MIF_ADDRESS_MAP_ATTRIBUTE_SRC_PB_TYPE, loc_data)
      .as_string();
  const std::string& des_pb_type_attr =
    get_attribute(xml_mif_address_map,
                  XML_MIF_ADDRESS_MAP_ATTRIBUTE_DES_PB_TYPE, loc_data)
      .as_string();

  const MifSourceSettingId src_source_id =
    bitstream_setting.find_mif_source_by_pb_type(src_pb_type_attr);
  if (!src_source_id.is_valid()) {
    archfpga_throw(loc_data.filename_c_str(),
                   loc_data.line(xml_mif_address_map),
                   "mif_address_map src_pb_type='%s' has no matching "
                   "mif_source definition\n",
                   src_pb_type_attr.c_str());
  }

  const MifSourceSettingId des_source_id =
    bitstream_setting.find_mif_source_by_pb_type(des_pb_type_attr);
  if (!des_source_id.is_valid()) {
    archfpga_throw(loc_data.filename_c_str(),
                   loc_data.line(xml_mif_address_map),
                   "mif_address_map des_pb_type='%s' has no matching "
                   "mif_source definition\n",
                   des_pb_type_attr.c_str());
  }

  const MifAddressMapSettingId map_id =
    bitstream_setting.add_mif_address_map_setting(src_pb_type_attr,
                                                  des_pb_type_attr);
  const openfpga::BasicPort src_address_range =
    bitstream_setting.mif_source_address_range(src_source_id);

  bool has_map_rule = false;
  for (pugi::xml_node xml_child : xml_mif_address_map.children()) {
    if (xml_child.name() != std::string(XML_MIF_ADDRESS_MAP_RULE_NODE_NAME)) {
      bad_tag(xml_child, loc_data, xml_mif_address_map,
              {XML_MIF_ADDRESS_MAP_RULE_NODE_NAME});
    }
    read_xml_mif_address_map_rule(xml_child, loc_data, bitstream_setting,
                                  map_id, src_address_range);
    has_map_rule = true;
  }

  if (!has_map_rule) {
    archfpga_throw(loc_data.filename_c_str(),
                   loc_data.line(xml_mif_address_map),
                   "mif_address_map requires at least one <map> child\n");
  }
}

/********************************************************************
 * Parse XML codes about <openfpga_bitstream_setting> to an object
 *******************************************************************/
int read_xml_bitstream_setting(pugi::xml_node& Node,
                               const pugiutil::loc_data& loc_data,
                               openfpga::BitstreamSetting& bitstream_setting) {
  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_child : Node.children()) {
    bool valid_node = false;
    for (auto valid_node_name : XML_VALID_NODE_NAMES) {
      if (xml_child.name() == std::string(valid_node_name)) {
        valid_node = true;
        break;
      }
    }
    if (!valid_node) {
      std::vector<std::string> vec_valid_node_names;
      for (auto valid_node_name : XML_VALID_NODE_NAMES) {
        vec_valid_node_names.push_back(std::string(valid_node_name));
      }
      bad_tag(xml_child, loc_data, Node, vec_valid_node_names);
      return openfpga::CMD_EXEC_FATAL_ERROR;
    }

    if (xml_child.name() == std::string(XML_PB_TYPE_NODE_NAME)) {
      read_xml_bitstream_pb_type_setting(xml_child, loc_data,
                                         bitstream_setting);
    } else if (xml_child.name() ==
               std::string(XML_DEFAULT_MODE_BITS_NODE_NAME)) {
      read_xml_bitstream_default_mode_setting(xml_child, loc_data,
                                              bitstream_setting);
    } else if (xml_child.name() == std::string(XML_CLOCK_ROUTING_NODE_NAME)) {
      read_xml_bitstream_clock_routing_setting(xml_child, loc_data,
                                               bitstream_setting);
    } else if (xml_child.name() == std::string(XML_INTERCONNECT_NODE_NAME)) {
      read_xml_bitstream_interconnect_setting(xml_child, loc_data,
                                              bitstream_setting);
    } else if (xml_child.name() == std::string(XML_NON_FABRIC_NODE_NAME)) {
      read_xml_non_fabric_bitstream_setting(xml_child, loc_data,
                                            bitstream_setting);
    } else if (xml_child.name() ==
               std::string(XML_OVERWRITE_BITSTREAM_NODE_NAME)) {
      read_xml_overwrite_bitstream_setting(xml_child, loc_data,
                                           bitstream_setting);
    } else if (xml_child.name() == std::string(XML_MIF_SOURCE_NODE_NAME)) {
      read_xml_mif_source_setting(xml_child, loc_data, bitstream_setting);
    } else {
      VTR_ASSERT_SAFE(xml_child.name() ==
                      std::string(XML_MIF_ADDRESS_MAP_NODE_NAME));
      read_xml_mif_address_map_setting(xml_child, loc_data, bitstream_setting);
    }
  }

  return openfpga::CMD_EXEC_SUCCESS;
}
