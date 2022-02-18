/********************************************************************
 * This file includes functions that outputs a bus group object to XML format
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

/* Headers from pin constraint library */
#include "bus_group_xml_constants.h"
#include "write_xml_bus_group.h"

/********************************************************************
 * A writer to output a bus to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
static 
int write_xml_bus(std::fstream& fp,
                  const BusGroup& bus_group,
                  const BusGroupId& bus_id) {
  /* Validate the file stream */
  if (false == openfpga::valid_file_stream(fp)) {
    return 2;
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "<" << XML_BUS_NODE_NAME << "";

  if (false == bus_group.valid_bus_id(bus_id)) {
    return 1;
  }

  write_xml_attribute(fp, XML_BUS_PORT_ATTRIBUTE_NAME, generate_xml_port_name(bus_group.bus_port(bus_id)).c_str());
  fp << ">" << "\n";

  /* Output all the pins under this bus */
  for (const BusPinId& pin_id : bus_group.bus_pins(bus_id)) {
    openfpga::write_tab_to_file(fp, 2);
    fp << "<" << XML_PIN_NODE_NAME << "";

    write_xml_attribute(fp, XML_PIN_INDEX_ATTRIBUTE_NAME, bus_group.pin_index(pin_id));
    write_xml_attribute(fp, XML_PIN_NAME_ATTRIBUTE_NAME, bus_group.pin_name(pin_id).c_str());

    fp << "</" << XML_PIN_NODE_NAME << "/>" << "\n";
  }

  openfpga::write_tab_to_file(fp, 1);
  fp << "</" << XML_BUS_NODE_NAME << "";
  fp << ">" << "\n";

  return 0;
}

/********************************************************************
 * A writer to output a bus group object to XML format
 *
 * Return 0 if successful
 * Return 1 if there are more serious bugs in the architecture 
 * Return 2 if fail when creating files
 *******************************************************************/
int write_xml_bus_group(const char* fname,
                        const BusGroup& bus_group) {

  vtr::ScopedStartFinishTimer timer("Write Bus Group");

  /* Create a file handler */
  std::fstream fp;
  /* Open the file stream */
  fp.open(std::string(fname), std::fstream::out | std::fstream::trunc);

  /* Validate the file stream */
  openfpga::check_file_stream(fname, fp);
  
  /* Write the root node */
  fp << "<" << XML_BUS_GROUP_NODE_NAME << ">" << "\n";

  int err_code = 0;

  /* Write each bus */ 
  for (const BusGroupId& bus : bus_group.buses()) {
    /* Write bus */ 
    err_code = write_xml_bus(fp, bus_group, bus);
    if (0 != err_code) {
      return err_code;
    }
  }

  /* Finish writing the root node */
  fp << "</" << XML_BUS_GROUP_NODE_NAME << ">" << "\n";

  /* Close the file stream */
  fp.close();

  return err_code;
}
