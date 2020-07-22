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

  fp << "\t" << "<key";

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

  fp << "/>" << "\n";

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

  /* Write component by component */ 
  for (const FabricKeyId& key : fabric_key.keys()) {
    err_code = write_xml_fabric_component_key(fp, fabric_key, key);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</fabric_key>" << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}
