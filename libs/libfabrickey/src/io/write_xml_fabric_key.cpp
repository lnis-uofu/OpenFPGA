/********************************************************************
 * This file includes functions that outputs a configuration protocol to XML
 *format
 *******************************************************************/
/* Headers from system goes first */
#include <algorithm>
#include <string>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpga util library */
#include "openfpga_digest.h"
#include "openfpga_reserved_words.h"

/* Headers from arch openfpga library */
#include "write_xml_utils.h"

/* Headers from fabrickey library */
#include "fabric_key_xml_constants.h"
#include "write_xml_fabric_key.h"

namespace openfpga {  // Begin namespace openfpga

/********************************************************************
 * A writer to output a component sub key to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_fabric_component_sub_key(
  std::fstream& fp, const FabricKey& fabric_key,
  const FabricSubKeyId& component_key, const size_t& key_idx,
  const size_t& level) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, level);
  fp << "<" << XML_FABRIC_KEY_KEY_NODE_NAME;

  if (false == fabric_key.valid_sub_key_id(component_key)) {
    return 1;
  }

  write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_ID_NAME, key_idx);
  if (!fabric_key.sub_key_name(component_key).empty()) {
    write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_NAME_NAME,
                        fabric_key.sub_key_name(component_key).c_str());
  }
  write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_VALUE_NAME,
                      fabric_key.sub_key_value(component_key));

  if (!fabric_key.sub_key_alias(component_key).empty()) {
    write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_ALIAS_NAME,
                        fabric_key.sub_key_alias(component_key).c_str());
  }

  fp << "/>"
     << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a component key to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_fabric_component_key(std::fstream& fp,
                                          const FabricKey& fabric_key,
                                          const FabricKeyId& component_key,
                                          const size_t& level) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, level);
  fp << "<" << XML_FABRIC_KEY_KEY_NODE_NAME;

  if (false == fabric_key.valid_key_id(component_key)) {
    return 1;
  }

  write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_ID_NAME,
                      size_t(component_key));
  if (!fabric_key.key_name(component_key).empty()) {
    write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_NAME_NAME,
                        fabric_key.key_name(component_key).c_str());
  }
  write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_VALUE_NAME,
                      fabric_key.key_value(component_key));

  if (!fabric_key.key_alias(component_key).empty()) {
    write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_ALIAS_NAME,
                        fabric_key.key_alias(component_key).c_str());
  }

  vtr::Point<int> coord = fabric_key.key_coordinate(component_key);
  if (fabric_key.valid_key_coordinate(coord)) {
    write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_COLUMN_NAME,
                        coord.x());
    write_xml_attribute(fp, XML_FABRIC_KEY_KEY_ATTRIBUTE_ROW_NAME, coord.y());
  }

  fp << "/>"
     << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a BL shift register bank description to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_fabric_bl_shift_register_banks(
  std::fstream& fp, const FabricKey& fabric_key, const FabricRegionId& region,
  const size_t& level) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  /* If we have an empty bank, we just skip it */
  if (0 == fabric_key.bl_banks(region).size()) {
    return 0;
  }

  /* Write the root node */
  openfpga::write_tab_to_file(fp, level);
  fp << "<" << XML_FABRIC_KEY_BL_SHIFT_REGISTER_BANKS_NODE_NAME << ">"
     << "\n";

  for (const auto& bank : fabric_key.bl_banks(region)) {
    openfpga::write_tab_to_file(fp, level + 1);
    fp << "<" << XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME;

    write_xml_attribute(
      fp, XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_ID_NAME,
      size_t(bank));

    std::string port_str;
    for (const auto& port : fabric_key.bl_bank_data_ports(region, bank)) {
      port_str += generate_xml_port_name(port) + ",";
    }
    /* Chop the last comma */
    if (!port_str.empty()) {
      port_str.pop_back();
    }
    write_xml_attribute(
      fp, XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_RANGE_NAME,
      port_str.c_str());

    fp << "/>"
       << "\n";
  }

  openfpga::write_tab_to_file(fp, level);
  fp << "</" << XML_FABRIC_KEY_BL_SHIFT_REGISTER_BANKS_NODE_NAME << ">"
     << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a WL shift register bank description to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
static int write_xml_fabric_wl_shift_register_banks(
  std::fstream& fp, const FabricKey& fabric_key, const FabricRegionId& region,
  const size_t& level) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  /* If we have an empty bank, we just skip it */
  if (0 == fabric_key.wl_banks(region).size()) {
    return 0;
  }

  /* Write the root node */
  openfpga::write_tab_to_file(fp, level);
  fp << "<" << XML_FABRIC_KEY_WL_SHIFT_REGISTER_BANKS_NODE_NAME << ">"
     << "\n";

  for (const auto& bank : fabric_key.wl_banks(region)) {
    openfpga::write_tab_to_file(fp, level + 1);
    fp << "<" << XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_NODE_NAME;

    write_xml_attribute(
      fp, XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_ID_NAME,
      size_t(bank));

    std::string port_str;
    for (const auto& port : fabric_key.wl_bank_data_ports(region, bank)) {
      port_str += generate_xml_port_name(port) + ",";
    }
    /* Chop the last comma */
    if (!port_str.empty()) {
      port_str.pop_back();
    }
    write_xml_attribute(
      fp, XML_FABRIC_KEY_BLWL_SHIFT_REGISTER_BANK_ATTRIBUTE_RANGE_NAME,
      port_str.c_str());

    fp << "/>"
       << "\n";
  }

  openfpga::write_tab_to_file(fp, level);
  fp << "</" << XML_FABRIC_KEY_WL_SHIFT_REGISTER_BANKS_NODE_NAME << ">"
     << "\n";

  return 0;
}

/* Write keys under the top-level module to a file */
static int write_xml_top_module_keys(std::fstream& fp,
                                     const FabricKey& fabric_key,
                                     const size_t& level) {
  int err_code = 0;
  /* Write the module declaration */
  openfpga::write_tab_to_file(fp, level);
  fp << "<" << XML_FABRIC_KEY_MODULE_NODE_NAME << " "
     << XML_FABRIC_KEY_MODULE_ATTRIBUTE_NAME_NAME << "=\""
     << FPGA_TOP_MODULE_NAME << "\""
     << ">\n";

  /* Write region by region */
  for (const FabricRegionId& region : fabric_key.regions()) {
    openfpga::write_tab_to_file(fp, level + 1);
    fp << "<" << XML_FABRIC_KEY_REGION_NODE_NAME << " "
       << XML_FABRIC_KEY_REGION_ATTRIBUTE_ID_NAME << "=\"" << size_t(region)
       << "\""
       << ">\n";

    /* Write shift register banks */
    write_xml_fabric_bl_shift_register_banks(fp, fabric_key, region, level + 2);
    write_xml_fabric_wl_shift_register_banks(fp, fabric_key, region, level + 2);

    /* Write component by component */
    for (const FabricKeyId& key : fabric_key.region_keys(region)) {
      err_code = write_xml_fabric_component_key(fp, fabric_key, key, level + 2);
      if (0 != err_code) {
        return err_code;
      }
    }

    openfpga::write_tab_to_file(fp, level + 1);
    fp << "</" << XML_FABRIC_KEY_REGION_NODE_NAME << ">"
       << "\n";
  }

  openfpga::write_tab_to_file(fp, level);
  fp << "</" << XML_FABRIC_KEY_MODULE_NODE_NAME << ">\n";

  return err_code;
}

/* Write keys under the a given module to a file */
static int write_xml_module_keys(std::fstream& fp, const FabricKey& fabric_key,
                                 const FabricKeyModuleId& module_id,
                                 const size_t& level) {
  int err_code = 0;
  /* Write the module declaration */
  openfpga::write_tab_to_file(fp, level);
  fp << "<" << XML_FABRIC_KEY_MODULE_NODE_NAME << " "
     << XML_FABRIC_KEY_MODULE_ATTRIBUTE_NAME_NAME << "=\""
     << fabric_key.module_name(module_id) << "\""
     << ">\n";

  /* Write component by component */
  size_t key_idx = 0;
  for (const FabricSubKeyId& key : fabric_key.sub_keys(module_id)) {
    err_code = write_xml_fabric_component_sub_key(fp, fabric_key, key, key_idx,
                                                  level + 1);
    if (0 != err_code) {
      return err_code;
    }
    key_idx++;
  }

  openfpga::write_tab_to_file(fp, level);
  fp << "</" << XML_FABRIC_KEY_MODULE_NODE_NAME << ">\n";

  return err_code;
}

/********************************************************************
 * A writer to output a fabric key to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_fabric_key(const char* fname, const FabricKey& fabric_key) {
  vtr::ScopedStartFinishTimer timer("Write Fabric Key");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);

  /* Write the root node */
  fp << "<" << XML_FABRIC_KEY_ROOT_NAME << ">"
     << "\n";

  int err_code = 0;

  /* Write the top-level module */
  err_code = write_xml_top_module_keys(fp, fabric_key, 1);
  if (0 != err_code) {
    return err_code;
  }

  /* Write regular modules */
  for (FabricKeyModuleId module_id : fabric_key.modules()) {
    err_code = write_xml_module_keys(fp, fabric_key, module_id, 1);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</" << XML_FABRIC_KEY_ROOT_NAME << ">"
     << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}

}  // End of namespace openfpga
