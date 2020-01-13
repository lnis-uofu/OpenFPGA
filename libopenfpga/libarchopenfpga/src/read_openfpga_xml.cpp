/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_openfpga_xml.h"

/********************************************************************
 * Parse XML codes about circuit models to circuit library
 *******************************************************************/
static 
void read_xml_circuit_models(pugi::xml_node Node,
                             const pugiutil::loc_data& loc_data,
                             CircuitLibrary& circuit_lib) {
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
      }
    }
  } 
}

/********************************************************************
 * Top-level function to parse an XML file and load data to :
 * 1. circuit library
 *******************************************************************/
void read_xml_openfpga_arch(const char* arch_file_name,
                            CircuitLibrary& circuit_lib) {
  pugi::xml_node Next;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, arch_file_name);

    /* Parse circuit_models to circuit library */
    auto module_circuit_models = get_single_child(doc, "module_circuit_models", loc_data);
    read_xml_circuit_models(module_circuit_models, loc_data, circuit_lib);

  } catch (pugiutil::XmlError& e) {
    archfpga_throw(arch_file_name, e.line(),
                   "%s", e.what());
  }
}
