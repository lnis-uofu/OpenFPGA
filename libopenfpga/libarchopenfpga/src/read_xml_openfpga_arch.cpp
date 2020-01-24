/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtrutil library */
#include "vtr_time.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_technology_library.h"
#include "read_xml_circuit_library.h"
#include "read_xml_simulation_setting.h"
#include "read_xml_config_protocol.h"
#include "read_xml_routing_circuit.h"
#include "read_xml_openfpga_arch.h"
#include "openfpga_arch_linker.h"

/********************************************************************
 * Top-level function to parse an XML file and load data to :
 * 1. circuit library
 *******************************************************************/
openfpga::Arch read_xml_openfpga_arch(const char* arch_file_name) {
  vtr::ScopedStartFinishTimer timer("Read OpenFPGA architecture");

  openfpga::Arch openfpga_arch;

  pugi::xml_node Next;

  /* Parse the file */
  pugi::xml_document doc;
  pugiutil::loc_data loc_data;

  try {
    loc_data = pugiutil::load_xml(doc, arch_file_name);

    /* First node should be <openfpga_architecture> */
    auto xml_openfpga_arch = get_single_child(doc, "openfpga_architecture", loc_data); 

    /* Parse circuit_models to circuit library 
     * under the node <module_circuit_models> 
     */
    auto xml_circuit_models = get_single_child(xml_openfpga_arch, "circuit_library", loc_data);
    openfpga_arch.circuit_lib = read_xml_circuit_library(xml_circuit_models, loc_data);

    /* Build the internal links for the circuit library */
    openfpga_arch.circuit_lib.build_model_links();
  
    /* Build the timing graph inside the circuit library */
    openfpga_arch.circuit_lib.build_timing_graphs();

    /* Parse technology library */
    auto xml_tech_lib = get_single_child(xml_openfpga_arch, "technology_library", loc_data); 
    openfpga_arch.tech_lib = read_xml_technology_library(xml_tech_lib, loc_data);
    /* Build the internal link for technology library */
    openfpga_arch.tech_lib.link_models_to_variations();

    /* Parse configuration protocol to data structure */
    openfpga_arch.config_protocol = read_xml_config_protocol(xml_openfpga_arch, loc_data);

    /* Build the internal link between configuration protocol and circuit library */
    link_config_protocol_to_circuit_library(openfpga_arch);

    /* Parse the connection block circuit definition */
    openfpga_arch.cb_switch2circuit = read_xml_cb_switch_circuit(xml_openfpga_arch, loc_data,
                                                                 openfpga_arch.circuit_lib);

    /* Parse the connection block circuit definition */
    openfpga_arch.cb_switch2circuit = read_xml_cb_switch_circuit(xml_openfpga_arch, loc_data,
                                                                 openfpga_arch.circuit_lib);

    /* Parse the switch block circuit definition */
    openfpga_arch.sb_switch2circuit = read_xml_sb_switch_circuit(xml_openfpga_arch, loc_data,
                                                                 openfpga_arch.circuit_lib);

    /* Parse the routing segment circuit definition */
    openfpga_arch.routing_seg2circuit = read_xml_routing_segment_circuit(xml_openfpga_arch, loc_data,
                                                                         openfpga_arch.circuit_lib);

    /* Parse the routing segment circuit definition */
    openfpga_arch.direct2circuit = read_xml_direct_circuit(xml_openfpga_arch, loc_data,
                                                           openfpga_arch.circuit_lib);

    /* Second node should be <openfpga_simulation_setting> */
    auto xml_simulation_settings = get_single_child(doc, "openfpga_simulation_setting", loc_data); 

    /* Parse simulation settings to data structure */
    openfpga_arch.sim_setting = read_xml_simulation_setting(xml_simulation_settings, loc_data);

  } catch (pugiutil::XmlError& e) {
    archfpga_throw(arch_file_name, e.line(),
                   "%s", e.what());
  }

  return openfpga_arch;
}
