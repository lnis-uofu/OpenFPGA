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

#include "read_openfpga_xml.h"

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

  if (std::string("passgate") == type_string) {
    return CIRCUIT_MODEL_PASSGATE;
  }

  if (std::string("gate") == type_string) {
    return CIRCUIT_MODEL_GATE;
  }

  /* Reach here, we have an invalid value, error out */
  return NUM_CIRCUIT_MODEL_TYPES;
}

/********************************************************************
 * Parse XML codes about circuit models to circuit library
 *******************************************************************/
static 
CircuitLibrary read_xml_circuit_models(pugi::xml_node& Node,
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
    /* Add a new circuit model to circuit library */
    /* Process the XML attributes under the <circuit_model> tag */
    for (pugi::xml_attribute attr : model_xml.attributes()) {
      /* Find the type of the circuit model */
      if (attr.name() == std::string("type")) {
        /* Translate the type of circuit model to enumerate */
        e_circuit_model_type model_type = string_to_circuit_model_type(attr.value());
        if (NUM_CIRCUIT_MODEL_TYPES == model_type) {
          archfpga_throw(loc_data.filename_c_str(), loc_data.line(Node),
                         "Invalid 'type' attribute '%s'\n",
                         attr.value());
        }

        CircuitModelId model = circuit_lib.add_model(model_type);
      }
    }
  } 

  return circuit_lib;
}

/********************************************************************
 * Top-level function to parse an XML file and load data to :
 * 1. circuit library
 *******************************************************************/
CircuitSettings read_xml_openfpga_arch(const char* arch_file_name) {
  CircuitSettings circuit_settings;

  pugi::xml_node Next;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, arch_file_name);

    /* Root node should be <circuit_settings> */
    auto xml_circuit_settings = get_single_child(doc, "circuit_settings", loc_data); 

    /* Parse circuit_models to circuit library 
     * under the node <module_circuit_models> 
     */
    auto xml_module_circuit_models = get_single_child(xml_circuit_settings, "module_circuit_models", loc_data);
    circuit_settings.circuit_lib = read_xml_circuit_models(xml_module_circuit_models, loc_data);

  } catch (pugiutil::XmlError& e) {
    archfpga_throw(arch_file_name, e.line(),
                   "%s", e.what());
  }

  return circuit_settings;
}
