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

/* Headers from libopenfpga util library */
#include "openfpga_port_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_repack_design_constraints.h"

/********************************************************************
 * Parse XML codes of a <key> to an object of FabricKey
 *******************************************************************/
static 
void read_xml_pin_constraint(pugi::xml_node& xml_pin_constraint,
                             const pugiutil::loc_data& loc_data,
                             RepackDesignConstraints& repack_design_constraints) {

  /* Create a new design constraint in the storage */
  RepackDesignConstraintId design_constraint_id = repack_design_constraints.create_design_constraint(RepackDesignConstraints::PIN_ASSIGNMENT);

  if (false == repack_design_constraints.valid_design_constraint_id(design_constraint_id)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_pin_constraint),
                   "Fail to create design constraint!\n");
  }

  repack_design_constraints.set_tile(design_constraint_id,
                                     get_attribute(xml_pin_constraint, "tile", loc_data).as_string());

  openfpga::PortParser port_parser(get_attribute(xml_pin_constraint, "pin", loc_data).as_string());

  repack_design_constraints.set_pin(design_constraint_id,
                                    port_parser.port());

  repack_design_constraints.set_net(design_constraint_id,
                                    get_attribute(xml_pin_constraint, "net", loc_data).as_string());
}

/********************************************************************
 * Parse XML codes about <repack_design_constraints> to an object of RepackDesignConstraints
 *******************************************************************/
RepackDesignConstraints read_xml_repack_design_constraints(const char* design_constraint_fname) {

  vtr::ScopedStartFinishTimer timer("Read Repack Design Constraints");

  RepackDesignConstraints repack_design_constraints;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, design_constraint_fname);

    pugi::xml_node xml_root = get_single_child(doc, "repack_design_constraint", loc_data);

    size_t num_design_constraints = std::distance(xml_root.children().begin(), xml_root.children().end());
    /* Reserve memory space for the region */
    repack_design_constraints.reserve_design_constraints(num_design_constraints);

    for (pugi::xml_node xml_design_constraint : xml_root.children()) {
      /* Error out if the XML child has an invalid name! */
      if (xml_design_constraint.name() != std::string("pin_constraint")) {
        bad_tag(xml_design_constraint, loc_data, xml_root, {"pin_constraint"});
      }
      read_xml_pin_constraint(xml_design_constraint, loc_data, repack_design_constraints);
    } 
  } catch (pugiutil::XmlError& e) {
    archfpga_throw(design_constraint_fname, e.line(),
                   "%s", e.what());
  }

  return repack_design_constraints; 
}

