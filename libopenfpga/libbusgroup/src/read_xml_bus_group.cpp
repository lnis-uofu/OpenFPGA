/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of bus group file to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "bus_group_xml_constants.h"
#include "read_xml_bus_group.h"

namespace openfpga { // Begin namespace openfpga

/********************************************************************
 * Parse XML codes of a <pin> to an object of BusGroup
 *******************************************************************/
static 
void read_xml_pin(pugi::xml_node& xml_pin,
                  const pugiutil::loc_data& loc_data,
                  BusGroup& bus_group,
                  const BusGroupId& bus_id) {
  if (false == bus_group.valid_bus_id(bus_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_pin),
                   "Invalid id of a bus group!\n");
  }

  int pin_index = get_attribute(xml_pin, XML_PIN_INDEX_ATTRIBUTE_NAME, loc_data).as_int();
  std::string pin_name = get_attribute(xml_pin, XML_PIN_NAME_ATTRIBUTE_NAME, loc_data).as_string();

  /* Before update storage, check if the pin index is in the range */
  BasicPort pin_port(bus_group.bus_port(bus_id).get_name(), pin_index, pin_index);
  if (!bus_group.bus_port(bus_id).contained(pin_port)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_pin),
                   "Pin index is out of range of the bus port width!\n");
  }

  BusPinId pin_id = bus_group.create_pin(bus_id);
  bus_group.set_pin_index(pin_id, pin_index);
  bus_group.set_pin_name(pin_id, pin_name);
}

/********************************************************************
 * Parse XML codes of a <bus> to an object of BusGroup
 *******************************************************************/
static 
void read_xml_bus(pugi::xml_node& xml_bus,
                  const pugiutil::loc_data& loc_data,
                  BusGroup& bus_group) {

  openfpga::PortParser port_parser(get_attribute(xml_bus, XML_BUS_PORT_ATTRIBUTE_NAME, loc_data).as_string());

  /* Create a new bus in the storage */
  BusGroupId bus_id = bus_group.create_bus(port_parser.port());

  if (false == bus_group.valid_bus_id(bus_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_bus),
                   "Fail to create a bus group!\n");
  }

  /* Ensure the bus port is valid */
  if (!bus_group.bus_port(bus_id).is_valid()) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_bus),
                   "Bus port is invalid, check LSB and MSB!\n");
  }

  for (pugi::xml_node xml_pin : xml_bus.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_pin.name() != std::string(XML_PIN_NODE_NAME)) {
      bad_tag(xml_pin, loc_data, xml_bus, {XML_PIN_NODE_NAME});
    }
    read_xml_pin(xml_pin, loc_data, bus_group, bus_id);
  } 
}

/********************************************************************
 * Parse XML codes about <bus_group> to an object of BusGroup
 *******************************************************************/
BusGroup read_xml_bus_group(const char* fname) {

  vtr::ScopedStartFinishTimer timer("Read Bus Group");

  BusGroup bus_group;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, fname);

    pugi::xml_node xml_root = get_single_child(doc, XML_BUS_GROUP_NODE_NAME, loc_data);

    size_t num_buses = std::distance(xml_root.children().begin(), xml_root.children().end());

    /* Count the total number of pins */
    size_t num_pins = 0;
    for (pugi::xml_node xml_bus : xml_root.children()) {
      num_pins += std::distance(xml_bus.children().begin(), xml_bus.children().end());
    }
  
    /* Reserve memory space for the buses */
    bus_group.reserve_buses(num_buses);
    /* Reserve memory space for the pins */
    bus_group.reserve_pins(num_pins);

    for (pugi::xml_node xml_bus : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_bus.name() != std::string(XML_BUS_NODE_NAME)) {
        bad_tag(xml_bus, loc_data, xml_root, {XML_BUS_NODE_NAME});
      }
      read_xml_bus(xml_bus, loc_data, bus_group);
    } 
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(fname, e.line(),
                   "%s", e.what());
  }

  return bus_group; 
}

} // End of namespace openfpga
