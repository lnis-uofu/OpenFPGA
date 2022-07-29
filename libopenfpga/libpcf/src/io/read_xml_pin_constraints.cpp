/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML of pin constraints to the associated
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

#include "read_xml_pin_constraints.h"

/********************************************************************
 * Parse XML codes of a <set_io> to an object of PinConstraint
 *******************************************************************/
static 
void read_xml_pin_constraint(pugi::xml_node& xml_pin_constraint,
                             const pugiutil::loc_data& loc_data,
                             PinConstraints& pin_constraints) {

  openfpga::PortParser port_parser(get_attribute(xml_pin_constraint, "pin", loc_data).as_string());

  std::string net_name = get_attribute(xml_pin_constraint, "net", loc_data).as_string();

  /* Create a new pin constraint in the storage */
  PinConstraintId pin_constraint_id = pin_constraints.create_pin_constraint(port_parser.port(), net_name);

  if (false == pin_constraints.valid_pin_constraint_id(pin_constraint_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_pin_constraint),
                   "Fail to create pin constraint!\n");
  }
  
  /* Set default value if defined */
  std::string default_value = get_attribute(xml_pin_constraint, "default_value", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string();
  pin_constraints.set_net_default_value(pin_constraint_id, default_value);
  if (!default_value.empty() && !pin_constraints.valid_net_default_value(pin_constraint_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_pin_constraint),
                   "Invalid default value for pin constraints. Expect [0|1]!\n");
  }
}

/********************************************************************
 * Parse XML codes about <pin_constraints> to an object of PinConstraints
 *******************************************************************/
PinConstraints read_xml_pin_constraints(const char* pin_constraint_fname) {

  vtr::ScopedStartFinishTimer timer("Read Pin Constraints");

  PinConstraints pin_constraints;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, pin_constraint_fname);

    pugi::xml_node xml_root = get_single_child(doc, "pin_constraints", loc_data);

    size_t num_pin_constraints = std::distance(xml_root.children().begin(), xml_root.children().end());
    /* Reserve memory space for the region */
    pin_constraints.reserve_pin_constraints(num_pin_constraints);

    for (pugi::xml_node xml_pin_constraint : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_pin_constraint.name() != std::string("set_io")) {
        bad_tag(xml_pin_constraint, loc_data, xml_root, {"set_io"});
      }
      read_xml_pin_constraint(xml_pin_constraint, loc_data, pin_constraints);
    } 
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(pin_constraint_fname, e.line(),
                   "%s", e.what());
  }

  return pin_constraints; 
}

