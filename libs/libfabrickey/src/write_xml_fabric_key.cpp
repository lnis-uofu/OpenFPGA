/********************************************************************
 * This file includes functions that outputs a configuration protocol to XML format
 *******************************************************************/
/* Headers from system goes first */
#include <string>
#include <algorithm>

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpga util library */
#include "openfpga_digest.h"

/* Headers from arch openfpga library */
#include "write_xml_utils.h" 

/* Headers from fabrickey library */
#include "write_xml_fabric_key.h"

/********************************************************************
 * A writer to output a component key to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
static 
int write_xml_fabric_component_key(std::fstream& fp,
                                   const FabricKey& fabric_key,
                                   const FabricKeyId& component_key) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 2);
  fp << "<key";

  if (false == fabric_key.valid_key_id(component_key)) {
    return 1;
  }

  write_xml_attribute(fp, "id", size_t(component_key));
  if (!fabric_key.key_name(component_key).empty()) {
    write_xml_attribute(fp, "name", fabric_key.key_name(component_key).c_str());
  }
  write_xml_attribute(fp, "value", fabric_key.key_value(component_key));

  if (!fabric_key.key_alias(component_key).empty()) {
    write_xml_attribute(fp, "alias", fabric_key.key_alias(component_key).c_str());
  }

  vtr::Point<int> coord = fabric_key.key_coordinate(component_key);
  if (fabric_key.valid_key_coordinate(coord)) {
    write_xml_attribute(fp, "column", coord.x());
    write_xml_attribute(fp, "row", coord.y());
  }

  fp << "/>" << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a BL shift register bank description to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
static 
int write_xml_fabric_bl_shift_register_banks(std::fstream& fp,
                                             const FabricKey& fabric_key,
                                             const FabricRegionId& region) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  /* If we have an empty bank, we just skip it */
  if (0 == fabric_key.bl_banks(region).size()) {
    return 0;
  }

  /* Write the root node */
  openfpga::write_tab_to_file(fp, 2);
  fp << "<bl_shift_register_banks>" << "\n";

  for (const auto& bank : fabric_key.bl_banks(region)) {
    openfpga::write_tab_to_file(fp, 3);
    fp << "<bank";

    write_xml_attribute(fp, "id", size_t(bank));
    
    std::string port_str;
    for (const auto& port : fabric_key.bl_bank_data_ports(region, bank)) {
      port_str += generate_xml_port_name(port) + ",";
    }
    /* Chop the last comma */
    if (!port_str.empty()) {
      port_str.pop_back();
    }
    write_xml_attribute(fp, "range", port_str.c_str());

    fp << "/>" << "\n";
  }

  openfpga::write_tab_to_file(fp, 2);
  fp << "</bl_shift_register_banks>" << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a WL shift register bank description to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
static 
int write_xml_fabric_wl_shift_register_banks(std::fstream& fp,
                                             const FabricKey& fabric_key,
                                             const FabricRegionId& region) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  /* If we have an empty bank, we just skip it */
  if (0 == fabric_key.wl_banks(region).size()) {
    return 0;
  }

  /* Write the root node */
  openfpga::write_tab_to_file(fp, 2);
  fp << "<wl_shift_register_banks>" << "\n";

  for (const auto& bank : fabric_key.wl_banks(region)) {
    openfpga::write_tab_to_file(fp, 3);
    fp << "<bank";

    write_xml_attribute(fp, "id", size_t(bank));
    
    std::string port_str;
    for (const auto& port : fabric_key.wl_bank_data_ports(region, bank)) {
      port_str += generate_xml_port_name(port) + ",";
    }
    /* Chop the last comma */
    if (!port_str.empty()) {
      port_str.pop_back();
    }
    write_xml_attribute(fp, "range", port_str.c_str());

    fp << "/>" << "\n";
  }

  openfpga::write_tab_to_file(fp, 2);
  fp << "</wl_shift_register_banks>" << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a fabric key to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_fabric_key(const char* fname,
                         const FabricKey& fabric_key) {

  vtr::ScopedStartFinishTimer timer("Write Fabric Key");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "<fabric_key>" << "\n";

  int err_code = 0;

  /* Write region by region */ 
  for (const FabricRegionId& region : fabric_key.regions()) {
    openfpga::write_tab_to_file(fp, 1);
    fp << "<region id=\"" << size_t(region) << "\"" << ">\n";

    /* Write shift register banks */
    write_xml_fabric_bl_shift_register_banks(fp, fabric_key, region);
    write_xml_fabric_wl_shift_register_banks(fp, fabric_key, region);

    /* Write component by component */ 
    for (const FabricKeyId& key : fabric_key.region_keys(region)) {
      err_code = write_xml_fabric_component_key(fp, fabric_key, key);
      if (0 != err_code) {
        return err_code;
      }
    }

    openfpga::write_tab_to_file(fp, 1);
    fp << "</region>" << "\n";
  }

  /* Finish writing the root node */
  fp << "</fabric_key>" << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}
