/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_circuit_library.h"

/********************************************************************
 * Convert string to the enumerate of model type
 *******************************************************************/
static 
e_circuit_model_type string_to_circuit_model_type(const std::string& type_string) {
  if (std::string("chan_wire") == type_string) {
    return CIRCUIT_MODEL_CHAN_WIRE;
  }

  if (std::string("wire") == type_string) {
    return CIRCUIT_MODEL_WIRE;
  }

  if (std::string("mux") == type_string) {
    return CIRCUIT_MODEL_MUX;
  }

  if (std::string("lut") == type_string) {
    return CIRCUIT_MODEL_LUT;
  }

  if (std::string("ff") == type_string) {
    return CIRCUIT_MODEL_FF;
  }

  if (std::string("sram") == type_string) {
    return CIRCUIT_MODEL_SRAM;
  }

  if (std::string("hard_logic") == type_string) {
    return CIRCUIT_MODEL_HARDLOGIC;
  }

  if (std::string("ccff") == type_string) {
    return CIRCUIT_MODEL_CCFF;
  }

  if (std::string("iopad") == type_string) {
    return CIRCUIT_MODEL_IOPAD;
  }

  if (std::string("inv_buf") == type_string) {
    return CIRCUIT_MODEL_INVBUF;
  }

  if (std::string("pass_gate") == type_string) {
    return CIRCUIT_MODEL_PASSGATE;
  }

  if (std::string("gate") == type_string) {
    return CIRCUIT_MODEL_GATE;
  }

  /* Reach here, we have an invalid value, error out */
  return NUM_CIRCUIT_MODEL_TYPES;
}

/********************************************************************
 * Parse XML codes of a circuit model to circuit library
 *******************************************************************/
static 
void read_xml_circuit_model(pugi::xml_node& model_xml,
                            const pugiutil::loc_data& loc_data,
                            CircuitLibrary& circuit_lib) {
  /* Find the type of the circuit model
   * so that we can add a new circuit model to circuit library 
   */
  const char* type_attr = get_attribute(model_xml, "type", loc_data).value();

  /* Translate the type of circuit model to enumerate */
  e_circuit_model_type model_type = string_to_circuit_model_type(std::string(type_attr));

  if (NUM_CIRCUIT_MODEL_TYPES == model_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(model_xml),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  CircuitModelId model = circuit_lib.add_model(model_type);

  /* Find the name of the circuit model */
  const char* name_attr = get_attribute(model_xml, "name", loc_data).value();
  circuit_lib.set_model_name(model, std::string(name_attr));
}

/********************************************************************
 * Parse XML codes about circuit models to circuit library
 *******************************************************************/
CircuitLibrary read_xml_circuit_library(pugi::xml_node& Node,
                                        const pugiutil::loc_data& loc_data) {
  CircuitLibrary circuit_lib;

  /* Iterate over the children under this node,
   * each child should be named after circuit_model
   */
  for (pugi::xml_node model_xml : Node.children()) {
    /* Error out if the XML child has an invalid name! */
    if (model_xml.name() != std::string("circuit_model")) {
      bad_tag(model_xml, loc_data, Node, {"circuit_model"});
    }
    read_xml_circuit_model(model_xml, loc_data, circuit_lib);
  } 

  return circuit_lib;
}

