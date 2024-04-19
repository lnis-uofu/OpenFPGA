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

/* Headers from libarchfpga */
#include "arch_error.h"
#include "config_protocol_xml_constants.h"
#include "openfpga_port_parser.h"
#include "read_xml_config_protocol.h"
#include "read_xml_util.h"

/********************************************************************
 * Convert string to the enumerate of configuration protocol type
 *******************************************************************/
static e_config_protocol_type string_to_config_protocol_type(
  const std::string& type_string) {
  for (size_t itype = 0; itype < NUM_CONFIG_PROTOCOL_TYPES; ++itype) {
    if (std::string(CONFIG_PROTOCOL_TYPE_STRING[itype]) == type_string) {
      return static_cast<e_config_protocol_type>(itype);
    }
  }

  return NUM_CONFIG_PROTOCOL_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of BL/WL protocol type
 *******************************************************************/
static e_blwl_protocol_type string_to_blwl_protocol_type(
  const std::string& type_string) {
  for (size_t itype = 0; itype < NUM_BLWL_PROTOCOL_TYPES; ++itype) {
    if (std::string(BLWL_PROTOCOL_TYPE_STRING[itype]) == type_string) {
      return static_cast<e_blwl_protocol_type>(itype);
    }
  }

  return NUM_BLWL_PROTOCOL_TYPES;
}

/********************************************************************
 * Parse XML codes of a <programming_clock> to an object of configuration
 *protocol
 *******************************************************************/
static void read_xml_ccff_prog_clock(pugi::xml_node& xml_progclk,
                                     const pugiutil::loc_data& loc_data,
                                     ConfigProtocol& config_protocol) {
  /* Find the type of configuration protocol */
  std::string port_attr =
    get_attribute(xml_progclk, XML_CONFIG_PROTOCOL_CCFF_PROG_CLOCK_PORT_ATTR,
                  loc_data)
      .as_string();

  std::string indices_attr =
    get_attribute(xml_progclk, XML_CONFIG_PROTOCOL_CCFF_PROG_CLOCK_INDICES_ATTR,
                  loc_data)
      .as_string();

  openfpga::BasicPort port = openfpga::PortParser(port_attr).port();

  config_protocol.set_prog_clock_pin_ccff_head_indices_pair(port, indices_attr);
}

/********************************************************************
 * Parse XML codes of a <bl> to an object of configuration protocol
 *******************************************************************/
static void read_xml_bl_protocol(pugi::xml_node& xml_bl_protocol,
                                 const pugiutil::loc_data& loc_data,
                                 ConfigProtocol& config_protocol) {
  /* Find the type of configuration protocol */
  const char* type_attr =
    get_attribute(xml_bl_protocol, "protocol", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_blwl_protocol_type blwl_protocol_type =
    string_to_blwl_protocol_type(std::string(type_attr));

  if (NUM_BLWL_PROTOCOL_TYPES == blwl_protocol_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_bl_protocol),
                   "Invalid 'protocol' attribute '%s'\n", type_attr);
  }

  config_protocol.set_bl_protocol_type(blwl_protocol_type);

  /* only applicable to shift-registor protocol
   * - Find the memory model to build shift register chains
   * - Find the number of shift register chains for each protocol
   */
  if (BLWL_PROTOCOL_SHIFT_REGISTER == blwl_protocol_type) {
    config_protocol.set_bl_memory_model_name(
      get_attribute(xml_bl_protocol, "circuit_model_name", loc_data)
        .as_string());
    config_protocol.set_bl_num_banks(get_attribute(xml_bl_protocol, "num_banks",
                                                   loc_data,
                                                   pugiutil::ReqOpt::OPTIONAL)
                                       .as_int(1));
  }
}

/********************************************************************
 * Parse XML codes of a <wl> to an object of configuration protocol
 *******************************************************************/
static void read_xml_wl_protocol(pugi::xml_node& xml_wl_protocol,
                                 const pugiutil::loc_data& loc_data,
                                 ConfigProtocol& config_protocol) {
  /* Find the type of configuration protocol */
  const char* type_attr =
    get_attribute(xml_wl_protocol, "protocol", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_blwl_protocol_type blwl_protocol_type =
    string_to_blwl_protocol_type(std::string(type_attr));

  if (NUM_BLWL_PROTOCOL_TYPES == blwl_protocol_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_wl_protocol),
                   "Invalid 'protocol' attribute '%s'\n", type_attr);
  }

  config_protocol.set_wl_protocol_type(blwl_protocol_type);

  /* only applicable to shift-registor protocol
   * - Find the memory model to build shift register chains
   * - Find the number of shift register chains for each protocol
   */
  if (BLWL_PROTOCOL_SHIFT_REGISTER == blwl_protocol_type) {
    config_protocol.set_wl_memory_model_name(
      get_attribute(xml_wl_protocol, "circuit_model_name", loc_data)
        .as_string());
    config_protocol.set_wl_num_banks(get_attribute(xml_wl_protocol, "num_banks",
                                                   loc_data,
                                                   pugiutil::ReqOpt::OPTIONAL)
                                       .as_int(1));
  }
}

/********************************************************************
 * Parse XML codes of a <organization> to an object of configuration protocol
 *******************************************************************/
static void read_xml_config_organization(pugi::xml_node& xml_config_orgz,
                                         const pugiutil::loc_data& loc_data,
                                         ConfigProtocol& config_protocol) {
  /* Find the type of configuration protocol */
  const char* type_attr =
    get_attribute(xml_config_orgz, "type", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_config_protocol_type config_orgz_type =
    string_to_config_protocol_type(std::string(type_attr));

  if (NUM_CONFIG_PROTOCOL_TYPES == config_orgz_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_config_orgz),
                   "Invalid 'type' attribute '%s'\n", type_attr);
  }

  config_protocol.set_type(config_orgz_type);

  /* Find the circuit model used by the configuration protocol */
  config_protocol.set_memory_model_name(
    get_attribute(xml_config_orgz, "circuit_model_name", loc_data).as_string());

  /* Parse the number of configurable regions
   * At least 1 region should be defined, otherwise error out
   */
  config_protocol.set_num_regions(
    get_attribute(xml_config_orgz, XML_CONFIG_PROTOCOL_NUM_REGIONS_ATTR,
                  loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_int(1));
  if (1 > config_protocol.num_regions()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_config_orgz),
                   "Invalid 'num_region=%d' definition. At least 1 region "
                   "should be defined!\n",
                   config_protocol.num_regions());
  }

  /* Parse Configuration chain protocols */
  if (config_protocol.type() == CONFIG_MEM_SCAN_CHAIN) {
    /* First pass: Get the programming clock port size */
    openfpga::BasicPort prog_clk_port;
    for (pugi::xml_node xml_progclk : xml_config_orgz.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_progclk.name() !=
          std::string(XML_CONFIG_PROTOCOL_CCFF_PROG_CLOCK_NODE_NAME)) {
        bad_tag(xml_progclk, loc_data, xml_config_orgz,
                {XML_CONFIG_PROTOCOL_CCFF_PROG_CLOCK_NODE_NAME});
      }
      std::string port_attr =
        get_attribute(xml_progclk,
                      XML_CONFIG_PROTOCOL_CCFF_PROG_CLOCK_PORT_ATTR, loc_data)
          .as_string();
      openfpga::BasicPort port = openfpga::PortParser(port_attr).port();
      if (prog_clk_port.get_name().empty()) {
        prog_clk_port.set_name(port.get_name());
        prog_clk_port.set_width(port.get_lsb(), port.get_msb());
      } else {
        if (prog_clk_port.get_name() != port.get_name()) {
          archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_progclk),
                         "Expect same name for all the programming clocks "
                         "(This: %s, Others: %s)!\n",
                         port.get_name().c_str(),
                         prog_clk_port.get_name().c_str());
        }
        if (prog_clk_port.get_msb() < port.get_msb()) {
          prog_clk_port.set_msb(port.get_msb());
        }
      }
    }
    config_protocol.set_prog_clock_port(prog_clk_port);

    /* Second pass: fill the clock detailed connections */
    for (pugi::xml_node xml_progclk : xml_config_orgz.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_progclk.name() !=
          std::string(XML_CONFIG_PROTOCOL_CCFF_PROG_CLOCK_NODE_NAME)) {
        bad_tag(xml_progclk, loc_data, xml_config_orgz,
                {XML_CONFIG_PROTOCOL_CCFF_PROG_CLOCK_NODE_NAME});
      }
      read_xml_ccff_prog_clock(xml_progclk, loc_data, config_protocol);
    }
  }

  /* Parse BL & WL protocols */
  if (config_protocol.type() == CONFIG_MEM_QL_MEMORY_BANK) {
    pugi::xml_node xml_bl_protocol = get_single_child(
      xml_config_orgz, "bl", loc_data, pugiutil::ReqOpt::OPTIONAL);
    if (xml_bl_protocol) {
      read_xml_bl_protocol(xml_bl_protocol, loc_data, config_protocol);
    }

    pugi::xml_node xml_wl_protocol = get_single_child(
      xml_config_orgz, "wl", loc_data, pugiutil::ReqOpt::OPTIONAL);
    if (xml_wl_protocol) {
      read_xml_wl_protocol(xml_wl_protocol, loc_data, config_protocol);
    }

    /* Throw an execption if the BL/WL protocols are different. We currently do
     * not support it! */
    if (config_protocol.bl_protocol_type() !=
        config_protocol.wl_protocol_type()) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_config_orgz),
                     "Expect same type of protocol for both BL and WL! Other "
                     "combinations are not supported yet\n");
    }
  }
}

/********************************************************************
 * Parse XML codes about <ql_memory_bank_config_setting> to
 *QLMemoryBankConfigSetting
 *******************************************************************/
static void read_xml_ql_memory_bank_config_setting(
  QLMemoryBankConfigSetting* setting, pugi::xml_node& Node,
  const pugiutil::loc_data& loc_data) {
  /* Parse configuration protocol root node */
  pugi::xml_node config_setting =
    get_single_child(Node, "ql_memory_bank_config_setting", loc_data,
                     pugiutil::ReqOpt::OPTIONAL);

  if (config_setting) {
    /* Add to ql_memory_bank_config_setting_ */
    for (pugi::xml_node xml_child : config_setting.children()) {
      if (xml_child.name() != std::string("pb_type")) {
        bad_tag(xml_child, loc_data, config_setting, {"pb_type"});
      }
      const std::string& name_attr =
        get_attribute(xml_child, "name", loc_data).as_string();
      uint32_t num_wl = get_attribute(xml_child, "num_wl", loc_data).as_uint();
      setting->add_pb_setting(name_attr, num_wl);
    }
  }
}

/********************************************************************
 * Parse XML codes about <configuration_protocol> to an object of ConfigProtocol
 *******************************************************************/
ConfigProtocol read_xml_config_protocol(pugi::xml_node& Node,
                                        const pugiutil::loc_data& loc_data) {
  ConfigProtocol config_protocol;

  /* Parse configuration protocol root node */
  pugi::xml_node xml_config =
    get_single_child(Node, "configuration_protocol", loc_data);

  pugi::xml_node xml_config_orgz =
    get_single_child(xml_config, "organization", loc_data);
  read_xml_config_organization(xml_config_orgz, loc_data, config_protocol);

  /* Parse QL Memory Bank configuration setting */
  if (config_protocol.type() == CONFIG_MEM_QL_MEMORY_BANK &&
      config_protocol.bl_protocol_type() == BLWL_PROTOCOL_FLATTEN &&
      config_protocol.wl_protocol_type() == BLWL_PROTOCOL_FLATTEN) {
    read_xml_ql_memory_bank_config_setting(
      config_protocol.get_ql_memory_bank_config_setting(), xml_config,
      loc_data);
  }

  return config_protocol;
}
