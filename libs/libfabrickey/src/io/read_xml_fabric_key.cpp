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

/* Headers from openfpga util library */
#include "openfpga_port_parser.h"
#include "openfpga_reserved_words.h"
#include "openfpga_tokenizer.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "fabric_key_xml_constants.h"
#include "read_xml_fabric_key.h"
#include "read_xml_util.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * Parse XML codes of a <key> to an object of FabricKey
 *******************************************************************/
static void read_xml_module_key(pugi::xml_node& xml_component_key,
                                const pugiutil::loc_data& loc_data,
                                FabricKey& fabric_key,
                                const FabricKeyModuleId& module_id) {
  /* Find the id of component key */
  size_t id = get_attribute(xml_component_key,
                            XML_FABRIC_KEY_KEY_ATTRIBUTE_ID_NAME, loc_data)
                .as_int();

  FabricSubKeyId sub_key_id = fabric_key.sub_keys(module_id)[id];

  if (false == fabric_key.valid_sub_key_id(sub_key_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_component_key),
                   "Invalid 'id' attribute '%d' (in total %lu keys)!\n", id,
                   fabric_key.sub_keys(module_id).size());
  }

  VTR_ASSERT_SAFE(true == fabric_key.valid_sub_key_id(sub_key_id));

  /* If we have an alias, set the value as well */
  const std::string& alias =
    get_attribute(xml_component_key, XML_FABRIC_KEY_KEY_ATTRIBUTE_ALIAS_NAME,
                  loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_string();
  if (!alias.empty()) {
    fabric_key.set_sub_key_alias(sub_key_id, alias);
  }

  /* If we have the alias set, name and valus are optional then
   * Otherwise, they are mandatory attributes
   */
  pugiutil::ReqOpt required_name_value = pugiutil::ReqOpt::OPTIONAL;
  if (true == alias.empty()) {
    required_name_value = pugiutil::ReqOpt::REQUIRED;
  }

  const std::string& name =
    get_attribute(xml_component_key, XML_FABRIC_KEY_KEY_ATTRIBUTE_NAME_NAME,
                  loc_data, required_name_value)
      .as_string();
  const size_t& value =
    get_attribute(xml_component_key, XML_FABRIC_KEY_KEY_ATTRIBUTE_VALUE_NAME,
                  loc_data, required_name_value)
      .as_int();

  fabric_key.set_sub_key_name(sub_key_id, name);
  fabric_key.set_sub_key_value(sub_key_id, value);
}

/********************************************************************
 * Parse XML codes of a <key> to an object of FabricKey
 *******************************************************************/
static void read_xml_region_key(pugi::xml_node& xml_component_key,
                                const pugiutil::loc_data& loc_data,
                                FabricKey& fabric_key,
                                const FabricRegionId& fabric_region) {
  /* Find the id of component key */
  const size_t& id =
    get_attribute(xml_component_key, XML_FABRIC_KEY_KEY_ATTRIBUTE_ID_NAME,
                  loc_data)
      .as_int();

  if (false == fabric_key.valid_key_id(FabricKeyId(id))) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_component_key),
                   "Invalid 'id' attribute '%d' (in total %lu keys)!\n", id,
                   fabric_key.keys().size());
  }

  VTR_ASSERT_SAFE(true == fabric_key.valid_key_id(FabricKeyId(id)));

  /* If we have an alias, set the value as well */
  const std::string& alias =
    get_attribute(xml_component_key, XML_FABRIC_KEY_KEY_ATTRIBUTE_ALIAS_NAME,
                  loc_data, pugiutil::ReqOpt::OPTIONAL)
      .as_string();
  if (!alias.empty()) {
    fabric_key.set_key_alias(FabricKeyId(id), alias);
  }

  /* If we have the alias set, name and valus are optional then
   * Otherwise, they are mandatory attributes
   */
  pugiutil::ReqOpt required_name_value = pugiutil::ReqOpt::OPTIONAL;
  if (true == alias.empty()) {
    required_name_value = pugiutil::ReqOpt::REQUIRED;
  }

  const std::string& name =
    get_attribute(xml_component_key, XML_FABRIC_KEY_KEY_ATTRIBUTE_NAME_NAME,
                  loc_data, required_name_value)
      .as_string();
  const size_t& value =
    get_attribute(xml_component_key, XML_FABRIC_KEY_KEY_ATTRIBUTE_VALUE_NAME,
                  loc_data, required_name_value)
      .as_int();

  fabric_key.set_key_name(FabricKeyId(id), name);
  fabric_key.set_key_value(FabricKeyId(id), value);
  fabric_key.add_key_to_region(fabric_region, FabricKeyId(id));

  /* Parse coordinates */
  vtr::Point<int> coord(-1, -1);
  coord.set_x(get_attribute(xml_component_key,
                            XML_FABRIC_KEY_KEY_ATTRIBUTE_COLUMN_NAME, loc_data,
                            pugiutil::ReqOpt::OPTIONAL)
                .as_int(-1));
  coord.set_y(get_attribute(xml_component_key,
                            XML_FABRIC_KEY_KEY_ATTRIBUTE_ROW_NAME, loc_data,
                            pugiutil::ReqOpt::OPTIONAL)
                .as_int(-1));
  if (fabric_key.valid_key_coordinate(coord)) {
    fabric_key.set_key_coordinate(FabricKeyId(id), coord);
  }
}

/********************************************************************
 * Parse XML codes of a <bank> under <bl_shift_register_banks> to an object of
 *FabricKey
 *******************************************************************/
static void read_xml_region_bl_shift_register_bank(
  pugi::xml_node& xml_bank, const pugiutil::loc_data& loc_data,
  FabricKey& fabric_key, const FabricRegionId& fabric_region) {
  /* Find the id of the bank */
  FabricBitLineBankId bank_id = FabricBitLineBankId(
    get_attribute(xml_bank,
                  XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_ID_NAME,
                  loc_data)
      .as_int());

  if (!fabric_key.valid_bl_bank_id(fabric_region, bank_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_bank),
                   "Invalid 'id' attribute '%lu' (in total %lu BL banks)!\n",
                   size_t(bank_id), fabric_key.bl_banks(fabric_region).size());
  }

  VTR_ASSERT_SAFE(true == fabric_key.valid_bl_bank_id(fabric_region, bank_id));

  /* Parse the ports */
  std::string data_ports =
    get_attribute(xml_bank,
                  XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_RANGE_NAME,
                  loc_data)
      .as_string();
  /* Split with ',' if we have multiple ports */
  openfpga::StringToken tokenizer(data_ports);
  for (const std::string& data_port : tokenizer.split(',')) {
    openfpga::PortParser data_port_parser(data_port);
    fabric_key.add_data_port_to_bl_shift_register_bank(fabric_region, bank_id,
                                                       data_port_parser.port());
  }
}

/********************************************************************
 * Parse XML codes of a <bl_shift_register_banks> to an object of FabricKey
 *******************************************************************/
static void read_xml_region_bl_shift_register_banks(
  pugi::xml_node& xml_bl_bank, const pugiutil::loc_data& loc_data,
  FabricKey& fabric_key, const FabricRegionId& fabric_region) {
  size_t num_banks = count_children(
    xml_bl_bank, XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME, loc_data,
    pugiutil::ReqOpt::OPTIONAL);
  fabric_key.reserve_bl_shift_register_banks(fabric_region, num_banks);

  for (size_t ibank = 0; ibank < num_banks; ++ibank) {
    fabric_key.create_bl_shift_register_bank(fabric_region);
  }

  for (pugi::xml_node xml_bank : xml_bl_bank.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_bank.name() !=
        std::string(XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME)) {
      bad_tag(xml_bank, loc_data, xml_bl_bank,
              {XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME});
    }
    read_xml_region_bl_shift_register_bank(xml_bank, loc_data, fabric_key,
                                           fabric_region);
  }
}

/********************************************************************
 * Parse XML codes of a <bank> under <wl_shift_register_banks> to an object of
 *FabricKey
 *******************************************************************/
static void read_xml_region_wl_shift_register_bank(
  pugi::xml_node& xml_bank, const pugiutil::loc_data& loc_data,
  FabricKey& fabric_key, const FabricRegionId& fabric_region) {
  /* Find the id of the bank */
  FabricWordLineBankId bank_id = FabricWordLineBankId(
    get_attribute(xml_bank,
                  XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_ID_NAME,
                  loc_data)
      .as_int());

  if (!fabric_key.valid_wl_bank_id(fabric_region, bank_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_bank),
                   "Invalid 'id' attribute '%lu' (in total %lu WL banks)!\n",
                   size_t(bank_id), fabric_key.wl_banks(fabric_region).size());
  }

  VTR_ASSERT_SAFE(true == fabric_key.valid_wl_bank_id(fabric_region, bank_id));

  /* Parse the ports */
  std::string data_ports =
    get_attribute(xml_bank,
                  XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_RANGE_NAME,
                  loc_data)
      .as_string();
  /* Split with ',' if we have multiple ports */
  openfpga::StringToken tokenizer(data_ports);
  for (const std::string& data_port : tokenizer.split(',')) {
    openfpga::PortParser data_port_parser(data_port);
    fabric_key.add_data_port_to_wl_shift_register_bank(fabric_region, bank_id,
                                                       data_port_parser.port());
  }
}

/********************************************************************
 * Parse XML codes of a <bl_shift_register_banks> to an object of FabricKey
 *******************************************************************/
static void read_xml_region_wl_shift_register_banks(
  pugi::xml_node& xml_wl_bank, const pugiutil::loc_data& loc_data,
  FabricKey& fabric_key, const FabricRegionId& fabric_region) {
  size_t num_banks = count_children(
    xml_wl_bank, XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME, loc_data,
    pugiutil::ReqOpt::OPTIONAL);
  fabric_key.reserve_wl_shift_register_banks(fabric_region, num_banks);

  for (size_t ibank = 0; ibank < num_banks; ++ibank) {
    fabric_key.create_wl_shift_register_bank(fabric_region);
  }

  for (pugi::xml_node xml_bank : xml_wl_bank.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_bank.name() !=
        std::string(XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME)) {
      bad_tag(xml_bank, loc_data, xml_wl_bank,
              {XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME});
    }
    read_xml_region_wl_shift_register_bank(xml_bank, loc_data, fabric_key,
                                           fabric_region);
  }
}

/********************************************************************
 * Parse XML codes of a <key> to an object of FabricKey
 *******************************************************************/
static void read_xml_fabric_region(pugi::xml_node& xml_region,
                                   const pugiutil::loc_data& loc_data,
                                   FabricKey& fabric_key) {
  /* Find the unique id for the region */
  const FabricRegionId& region_id = FabricRegionId(
    get_attribute(xml_region, XML_FABRIC_KEY_REGION_ATTRIBUTE_ID_NAME, loc_data)
      .as_int());
  if (false == fabric_key.valid_region_id(region_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_region),
                   "Invalid region id '%lu' (in total %lu regions)!\n",
                   size_t(region_id), fabric_key.regions().size());
  }
  VTR_ASSERT_SAFE(true == fabric_key.valid_region_id(region_id));

  /* Reserve memory space for the keys in the region */
  size_t num_keys = count_children(xml_region, XML_FABRIC_KEY_KEY_NODE_NAME,
                                   loc_data, pugiutil::ReqOpt::OPTIONAL);
  fabric_key.reserve_region_keys(region_id, num_keys);

  /* Parse the key for this region */
  if (0 < num_keys) {
    pugi::xml_node xml_key =
      get_first_child(xml_region, XML_FABRIC_KEY_KEY_NODE_NAME, loc_data);
    while (xml_key) {
      read_xml_region_key(xml_key, loc_data, fabric_key, region_id);
      xml_key = xml_key.next_sibling(xml_key.name());
    }
  }

  /* Parse the BL shift register bank for this region */
  pugi::xml_node xml_bl_bank = get_single_child(
    xml_region, XML_FABRIC_KEY_BL_SHIFT_REGISTER_BANKS_NODE_NAME, loc_data,
    pugiutil::ReqOpt::OPTIONAL);
  read_xml_region_bl_shift_register_banks(xml_bl_bank, loc_data, fabric_key,
                                          region_id);

  /* Parse the WL shift register bank for this region */
  pugi::xml_node xml_wl_bank = get_single_child(
    xml_region, XML_FABRIC_KEY_WL_SHIFT_REGISTER_BANKS_NODE_NAME, loc_data,
    pugiutil::ReqOpt::OPTIONAL);
  read_xml_region_wl_shift_register_banks(xml_wl_bank, loc_data, fabric_key,
                                          region_id);
}

/********************************************************************
 * Parse XML codes of a <module> which is a top-level module
 *******************************************************************/
static void read_xml_fabric_key_top_module(pugi::xml_node& xml_module,
                                           const pugiutil::loc_data& loc_data,
                                           FabricKey& fabric_key) {
  size_t num_regions =
    std::distance(xml_module.children().begin(), xml_module.children().end());
  /* Reserve memory space for the region */
  fabric_key.reserve_regions(num_regions);
  for (size_t iregion = 0; iregion < num_regions; ++iregion) {
    fabric_key.create_region();
  }

  /* Reserve memory space for the keys */
  size_t num_keys = 0;

  for (pugi::xml_node xml_region : xml_module.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_region.name() != std::string(XML_FABRIC_KEY_REGION_NODE_NAME)) {
      bad_tag(xml_region, loc_data, xml_module,
              {XML_FABRIC_KEY_REGION_NODE_NAME});
    }
    num_keys +=
      std::distance(xml_region.children().begin(), xml_region.children().end());
  }

  fabric_key.reserve_keys(num_keys);
  for (size_t ikey = 0; ikey < num_keys; ++ikey) {
    fabric_key.create_key();
  }

  /* Iterate over the children under this node,
   * each child should be named after circuit_model
   */
  for (pugi::xml_node xml_region : xml_module.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_region.name() != std::string(XML_FABRIC_KEY_REGION_NODE_NAME)) {
      bad_tag(xml_region, loc_data, xml_module,
              {XML_FABRIC_KEY_REGION_NODE_NAME});
    }
    read_xml_fabric_region(xml_region, loc_data, fabric_key);
  }
}

/********************************************************************
 * Parse XML codes of a <module> which is a regular module
 *******************************************************************/
static void read_xml_fabric_key_module(pugi::xml_node& xml_module,
                                       const pugiutil::loc_data& loc_data,
                                       FabricKey& fabric_key) {
  std::string name =
    get_attribute(xml_module, XML_FABRIC_KEY_MODULE_ATTRIBUTE_NAME_NAME,
                  loc_data)
      .as_string();
  FabricKeyModuleId module_id = fabric_key.create_module(name);

  /* Parse all the sub keys */
  size_t num_keys =
    std::distance(xml_module.children().begin(), xml_module.children().end());

  /* Reserve for better memory efficiency */
  fabric_key.reserve_module_keys(module_id, num_keys);
  for (size_t ikey = 0; ikey < num_keys; ++ikey) {
    fabric_key.create_module_key(module_id);
  }

  for (pugi::xml_node xml_key : xml_module.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_key.name() != std::string(XML_FABRIC_KEY_KEY_NODE_NAME)) {
      bad_tag(xml_key, loc_data, xml_module, {XML_FABRIC_KEY_KEY_NODE_NAME});
    }
    read_xml_module_key(xml_key, loc_data, fabric_key, module_id);
  }
}

/********************************************************************
 * Parse XML codes of a <module> to an object of FabricKey
 * - For top-level module, we expect a fixed name. If so, we use a special
 *parser
 * - For regular module, we follow regular parser
 *******************************************************************/
static void read_xml_fabric_keys_by_modules(pugi::xml_node& xml_module,
                                            const pugiutil::loc_data& loc_data,
                                            FabricKey& fabric_key) {
  std::string name =
    get_attribute(xml_module, XML_FABRIC_KEY_MODULE_ATTRIBUTE_NAME_NAME,
                  loc_data)
      .as_string();
  if (name == std::string(openfpga::FPGA_TOP_MODULE_NAME)) {
    read_xml_fabric_key_top_module(xml_module, loc_data, fabric_key);
  } else {
    read_xml_fabric_key_module(xml_module, loc_data, fabric_key);
  }
}

/********************************************************************
 * Parse XML codes about <fabric> to an object of FabricKey
 *******************************************************************/
FabricKey read_xml_fabric_key(const char* key_fname) {
  vtr::ScopedStartFinishTimer timer("Read Fabric Key");

  FabricKey fabric_key;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, key_fname);

    pugi::xml_node xml_root =
      get_single_child(doc, XML_FABRIC_KEY_ROOT_NAME, loc_data);

    /* Under the root node, we expect only modules */
    size_t num_modules =
      std::distance(xml_root.children().begin(), xml_root.children().end());

    /* Reserve for memory efficiency */
    fabric_key.reserve_modules(num_modules);

    /* Walk through the modules, fill keys one by one */
    for (pugi::xml_node xml_module : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_module.name() != std::string(XML_FABRIC_KEY_MODULE_NODE_NAME)) {
        bad_tag(xml_module, loc_data, xml_root,
                {XML_FABRIC_KEY_MODULE_NODE_NAME});
      }
      /* Parse fabric keys by module */
      read_xml_fabric_keys_by_modules(xml_module, loc_data, fabric_key);
    }

  } catch (pugiutil::XmlError& e) {
    archfpga_throw(key_fname, e.line(), "%s", e.what());
  }

  return fabric_key;
}

}  // End of namespace openfpga
