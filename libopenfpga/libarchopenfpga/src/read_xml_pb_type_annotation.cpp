/********************************************************************
 * This file includes the top-level function of this library
 * which reads an XML modeling OpenFPGA architecture to the associated
 * data structures
 *******************************************************************/
#include <string>

/* Headers from pugi XML library */
#include "pugixml.hpp"
#include "pugixml_util.hpp"

/* Headers from vtr util library */
#include "vtr_assert.h"

/* Headers from openfpga util library */
#include "openfpga_port_parser.h"
#include "openfpga_pb_parser.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_util.h"

#include "read_xml_pb_type_annotation.h"

/********************************************************************
 * Parse XML description for an interconnection annotation 
 * under a <pb_type> XML node
 *******************************************************************/
static 
void read_xml_interc_annotation(pugi::xml_node& xml_interc,
                                 const pugiutil::loc_data& loc_data,
                                 openfpga::PbTypeAnnotation& pb_type_annotation) {
  /* We have two mandatory XML attribute
   * 1. name of the interconnect
   * 2. circuit model name of the interconnect 
   */
  const std::string& name_attr = get_attribute(xml_interc, "name", loc_data).as_string();
  const std::string& circuit_model_name_attr = get_attribute(xml_interc, "circuit_model_name", loc_data).as_string();

  pb_type_annotation.add_interconnect_circuit_model_pair(name_attr, circuit_model_name_attr);
}

/********************************************************************
 * Parse XML description for a pb_type port annotation 
 * under a <pb_type> XML node
 *******************************************************************/
static 
void read_xml_pb_port_annotation(pugi::xml_node& xml_port,
                                 const pugiutil::loc_data& loc_data,
                                 openfpga::PbTypeAnnotation& pb_type_annotation) {
  /* We have two mandatory XML attribute
   * 1. name of the port
   * 2. name of the port to be binded in physical mode
   */
  const std::string& name_attr = get_attribute(xml_port, "name", loc_data).as_string();
  const std::string& physical_mode_port_attr = get_attribute(xml_port, "physical_mode_port", loc_data).as_string();

  /* Split the physical mode port attributes with space */
  openfpga::StringToken port_tokenizer(physical_mode_port_attr);
  const std::vector<std::string> physical_mode_ports = port_tokenizer.split();

  /* Parse the mode port using openfpga port parser */
  for (const auto& physical_mode_port : physical_mode_ports) {
    openfpga::PortParser port_parser(physical_mode_port); 
    pb_type_annotation.add_pb_type_port_pair(name_attr, port_parser.port());
  }

  /* We have an optional attribute: physical_mode_pin_initial_offset
   * Split based on the number of physical pb_type ports that have been defined
   */
  const std::string& physical_pin_initial_offset_attr = get_attribute(xml_port, "physical_mode_pin_initial_offset", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string();

  if (false == physical_pin_initial_offset_attr.empty()) {
    /* Split the physical mode port attributes with space */
    openfpga::StringToken offset_tokenizer(physical_pin_initial_offset_attr);
    const std::vector<std::string> initial_offsets = offset_tokenizer.split();
   
    /* Error out if the offset does not match the port definition */
    if (physical_mode_ports.size() != initial_offsets.size()) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_port),
                     "Defined %lu physical mode ports but only %lu physical pin initial offset are defined! Expect size matching.\n",
                      physical_mode_ports.size(), initial_offsets.size());
    }

    for (size_t iport = 0; iport < physical_mode_ports.size(); ++iport) {
      openfpga::PortParser port_parser(physical_mode_ports[iport]); 
      pb_type_annotation.set_physical_pin_initial_offset(name_attr,
                                                         port_parser.port(),
                                                         std::stoi(initial_offsets[iport]));
    }
  }

  /* We have an optional attribute: physical_mode_pin_rotate_offset
   * Split based on the number of physical pb_type ports that have been defined
   */
  const std::string& physical_pin_rotate_offset_attr = get_attribute(xml_port, "physical_mode_pin_rotate_offset", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string();

  if (false == physical_pin_rotate_offset_attr.empty()) {
    /* Split the physical mode port attributes with space */
    openfpga::StringToken offset_tokenizer(physical_pin_rotate_offset_attr);
    const std::vector<std::string> rotate_offsets = offset_tokenizer.split();
   
    /* Error out if the offset does not match the port definition */
    if (physical_mode_ports.size() != rotate_offsets.size()) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_port),
                     "Defined %lu physical mode ports but only %lu physical pin rotate offset are defined! Expect size matching.\n",
                      physical_mode_ports.size(), rotate_offsets.size());
    }

    for (size_t iport = 0; iport < physical_mode_ports.size(); ++iport) {
      openfpga::PortParser port_parser(physical_mode_ports[iport]); 
      pb_type_annotation.set_physical_pin_rotate_offset(name_attr,
                                                        port_parser.port(),
                                                        std::stoi(rotate_offsets[iport]));
    }
  }
}

/********************************************************************
 * Parse mode_bits: convert from string to array of digits
 * We only allow the bit to either '0' or '1'
 *******************************************************************/
static 
std::vector<size_t> parse_mode_bits(pugi::xml_node& xml_mode_bits,
                                    const pugiutil::loc_data& loc_data,
                                    const std::string& mode_bit_str) {
  std::vector<size_t> mode_bits;

  for (const char& bit_char : mode_bit_str) {
    if ('0' == bit_char) {
      mode_bits.push_back(0);
    } else if ('1' == bit_char) {
      mode_bits.push_back(1);
    } else {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_mode_bits),
                     "Unexpected '%c' character found in the mode bit '%s'! Only allow either '0' or '1'\n",
                     bit_char, mode_bit_str.c_str());
    }
  } 

  return mode_bits;
}

/********************************************************************
 * Parse XML description for a pb_type annotation under a <pb_type> XML node
 *******************************************************************/
static 
void read_xml_pb_type_annotation(pugi::xml_node& xml_pb_type,
                                 const pugiutil::loc_data& loc_data,
                                 std::vector<openfpga::PbTypeAnnotation>& pb_type_annotations) {
  openfpga::PbTypeAnnotation pb_type_annotation;

  /* Find the name of pb_type */
  const std::string& name_attr = get_attribute(xml_pb_type, "name", loc_data).as_string();
  const std::string& physical_name_attr = get_attribute(xml_pb_type, "physical_pb_type_name", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string();

  /* If both names are not empty, this is a operating pb_type */
  if ( (false == name_attr.empty()) 
    && (false == physical_name_attr.empty()) ) {
    /* Parse the attributes for operating pb_type */
    openfpga::PbParser operating_pb_parser(name_attr);
    pb_type_annotation.set_operating_pb_type_name(operating_pb_parser.leaf());
    if (0 < operating_pb_parser.parents().size()) {
      pb_type_annotation.set_operating_parent_pb_type_names(operating_pb_parser.parents());
    }
    if (0 < operating_pb_parser.modes().size()) {
      pb_type_annotation.set_operating_parent_mode_names(operating_pb_parser.modes());
    }

    openfpga::PbParser physical_pb_parser(physical_name_attr);
    pb_type_annotation.set_physical_pb_type_name(physical_pb_parser.leaf());
    if (0 < physical_pb_parser.parents().size()) {
      pb_type_annotation.set_physical_parent_pb_type_names(physical_pb_parser.parents());
    }
    if (0 < physical_pb_parser.modes().size()) {
      pb_type_annotation.set_physical_parent_mode_names(physical_pb_parser.modes());
    }
  } 

  /* If there is only a name, this is a physical pb_type */
  if ( (false == name_attr.empty()) 
    && (true == physical_name_attr.empty()) ) {
    openfpga::PbParser physical_pb_parser(name_attr);
    pb_type_annotation.set_physical_pb_type_name(physical_pb_parser.leaf());
    if (0 < physical_pb_parser.parents().size()) {
      pb_type_annotation.set_physical_parent_pb_type_names(physical_pb_parser.parents());
    }
    if (0 < physical_pb_parser.modes().size()) {
      pb_type_annotation.set_physical_parent_mode_names(physical_pb_parser.modes());
    }
  }

  /* Parse physical mode name which are applied to both pb_types */
  pb_type_annotation.set_physical_mode_name(get_attribute(xml_pb_type, "physical_mode_name", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string());

  /* Parse idle mode name which are applied to both pb_types */
  pb_type_annotation.set_idle_mode_name(get_attribute(xml_pb_type, "idle_mode_name", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string());

  /* Parse mode bits which are applied to both pb_types */
  std::vector<size_t> mode_bit_data = parse_mode_bits(xml_pb_type, loc_data, get_attribute(xml_pb_type, "mode_bits", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string());
  pb_type_annotation.set_mode_bits(mode_bit_data);

  /* If this is a physical pb_type, circuit model name is an optional attribute, 
   * which is applicable to leaf pb_type in the hierarchy 
   */
  if (true == pb_type_annotation.is_physical_pb_type()) {
    pb_type_annotation.set_circuit_model_name(get_attribute(xml_pb_type, "circuit_model_name", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string());
  }

  /* If this is an operating pb_type, index factor and offset may be optional needed */
  if (true == pb_type_annotation.is_operating_pb_type()) {
    pb_type_annotation.set_physical_pb_type_index_factor(get_attribute(xml_pb_type, "physical_pb_type_index_factor", loc_data, pugiutil::ReqOpt::OPTIONAL).as_float(1.));
    pb_type_annotation.set_physical_pb_type_index_offset(get_attribute(xml_pb_type, "physical_pb_type_index_offset", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(0));
  }

  /* Parse all the interconnect-to-circuit binding under this node
   * All the bindings are defined in child node <interconnect>
   */
  size_t num_intercs = count_children(xml_pb_type, "interconnect", loc_data, pugiutil::ReqOpt::OPTIONAL);
  if (0 < num_intercs) {
    pugi::xml_node xml_interc = get_first_child(xml_pb_type, "interconnect", loc_data);
    while (xml_interc) {
      read_xml_interc_annotation(xml_interc, loc_data, pb_type_annotation);
      xml_interc = xml_interc.next_sibling(xml_interc.name());
    } 
  }

  /* Parse all the port-to-port binding from operating pb_type to physical pb_type under this node
   * All the bindings are defined in child node <port>
   * This is only applicable to operating pb_type
   */
  if (true == pb_type_annotation.is_operating_pb_type()) {
    size_t num_ports = count_children(xml_pb_type, "port", loc_data, pugiutil::ReqOpt::OPTIONAL);
    if (0 < num_ports) {
      pugi::xml_node xml_port = get_first_child(xml_pb_type, "port", loc_data);
      while (xml_port) {
        read_xml_pb_port_annotation(xml_port, loc_data, pb_type_annotation);
        xml_port = xml_port.next_sibling(xml_port.name());
      } 
    }
  }

  /* Finish parsing and add it to the vector */ 
  pb_type_annotations.push_back(pb_type_annotation);
}

/********************************************************************
 * Top function to parse XML description about pb_type annotation 
 *******************************************************************/
std::vector<openfpga::PbTypeAnnotation> read_xml_pb_type_annotations(pugi::xml_node& Node,
                                                                     const pugiutil::loc_data& loc_data) {
  std::vector<openfpga::PbTypeAnnotation> pb_type_annotations;

  /* Parse configuration protocol root node */
  pugi::xml_node xml_annotations = get_single_child(Node, "pb_type_annotations", loc_data);

  /* Iterate over the children under this node,
   * each child should be named after <pb_type>
   */
  for (pugi::xml_node xml_pb_type : xml_annotations.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_pb_type.name() != std::string("pb_type")) {
      bad_tag(xml_pb_type, loc_data, xml_annotations, {"pb_type"});
    }
    read_xml_pb_type_annotation(xml_pb_type, loc_data, pb_type_annotations);
  } 

  return pb_type_annotations;
}
