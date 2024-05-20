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
#include "vtr_log.h"

/* Headers from libarchfpga */
#include "arch_error.h"
#include "read_xml_routing_circuit.h"
#include "read_xml_util.h"

/********************************************************************
 * Find the circuit model id for a routing switch as defined in XML
 *******************************************************************/
static CircuitModelId find_routing_circuit_model(
  pugi::xml_node& xml_switch, const pugiutil::loc_data& loc_data,
  const CircuitLibrary& circuit_lib, const std::string& switch_model_name,
  const e_circuit_model_type& expected_circuit_model_type) {
  /* Find the circuit model id in circuit library */
  CircuitModelId switch_model = circuit_lib.model(switch_model_name);
  /* Ensure we have a valid circuit model id! */
  if (CircuitModelId::INVALID() == switch_model) {
    archfpga_throw(
      loc_data.filename_c_str(), loc_data.line(xml_switch),
      "Invalid circuit model name '%s'! Unable to find it in circuit library\n",
      switch_model_name.c_str());
  }
  /* Check the type of switch model, it must be a specific type!!!
   * For CB/SB switches, the type must be a multiplexer
   * For routing segments, the type must be a channel wire
   * TODO: decide where to put these checking codes
   * This can be done here or in the function check_arch()
   */
  if (expected_circuit_model_type != circuit_lib.model_type(switch_model)) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_switch),
                   "Circuit model '%s' must be a type of '%s'\n",
                   switch_model_name.c_str(),
                   CIRCUIT_MODEL_TYPE_STRING[expected_circuit_model_type]);
  }

  return switch_model;
}

/********************************************************************
 * Parse XML codes about <connection_block> to an object of name-to-circuit
 *mapping Note: this function should be called AFTER the parsing of circuit
 *library!!!
 *******************************************************************/
std::map<std::string, CircuitModelId> read_xml_cb_switch_circuit(
  pugi::xml_node& Node, const pugiutil::loc_data& loc_data,
  const CircuitLibrary& circuit_lib) {
  std::map<std::string, CircuitModelId> cb_switch2circuit;

  /* Parse cb switch list */
  pugi::xml_node xml_cb_switch =
    get_single_child(Node, "connection_block", loc_data);

  /* Iterate over the children under this node,
   * each child should be named after switch
   */
  for (pugi::xml_node xml_switch : xml_cb_switch.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_switch.name() != std::string("switch")) {
      bad_tag(xml_switch, loc_data, xml_cb_switch, {"switch"});
    }
    /* Get the switch name */
    std::string switch_name =
      get_attribute(xml_switch, "name", loc_data).as_string();

    /* Get the switch circuit model name */
    std::string switch_model_name =
      get_attribute(xml_switch, "circuit_model_name", loc_data).as_string();

    CircuitModelId switch_model = find_routing_circuit_model(
      xml_switch, loc_data, circuit_lib, switch_model_name, CIRCUIT_MODEL_MUX);

    /* Ensure that there is no duplicated switch names defined here */
    std::map<std::string, CircuitModelId>::const_iterator it =
      cb_switch2circuit.find(switch_name);
    if (it != cb_switch2circuit.end()) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_switch),
                     "Switch name '%s' has been defined more than once!\n",
                     switch_name.c_str());
    }

    /* Pass all the check, we can add it to the map */
    cb_switch2circuit[switch_name] = switch_model;
  }

  return cb_switch2circuit;
}

/********************************************************************
 * Parse XML codes about <switch_block> to an object of name-to-circuit mapping
 * Note: this function should be called AFTER the parsing of circuit library!!!
 *******************************************************************/
std::map<std::string, CircuitModelId> read_xml_sb_switch_circuit(
  pugi::xml_node& Node, const pugiutil::loc_data& loc_data,
  const CircuitLibrary& circuit_lib) {
  std::map<std::string, CircuitModelId> sb_switch2circuit;

  /* Parse cb switch list */
  pugi::xml_node xml_sb_switch =
    get_single_child(Node, "switch_block", loc_data);

  /* Iterate over the children under this node,
   * each child should be named after switch
   */
  for (pugi::xml_node xml_switch : xml_sb_switch.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_switch.name() != std::string("switch")) {
      bad_tag(xml_switch, loc_data, xml_sb_switch, {"switch"});
    }
    /* Get the switch name */
    std::string switch_name =
      get_attribute(xml_switch, "name", loc_data).as_string();

    /* Get the switch circuit model name */
    std::string switch_model_name =
      get_attribute(xml_switch, "circuit_model_name", loc_data).as_string();

    CircuitModelId switch_model = find_routing_circuit_model(
      xml_switch, loc_data, circuit_lib, switch_model_name, CIRCUIT_MODEL_MUX);

    /* Ensure that there is no duplicated switch names defined here */
    std::map<std::string, CircuitModelId>::const_iterator it =
      sb_switch2circuit.find(switch_name);
    if (it != sb_switch2circuit.end()) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_switch),
                     "Switch name '%s' has been defined more than once!\n",
                     switch_name.c_str());
    }

    /* Pass all the check, we can add it to the map */
    sb_switch2circuit[switch_name] = switch_model;
  }

  return sb_switch2circuit;
}

/********************************************************************
 * Parse XML codes about <routing_segment> to an object of name-to-circuit
 *mapping Note: this function should be called AFTER the parsing of circuit
 *library!!!
 *******************************************************************/
std::map<std::string, CircuitModelId> read_xml_routing_segment_circuit(
  pugi::xml_node& Node, const pugiutil::loc_data& loc_data,
  const CircuitLibrary& circuit_lib) {
  std::map<std::string, CircuitModelId> seg2circuit;

  /* Parse cb switch list */
  pugi::xml_node xml_segments =
    get_single_child(Node, "routing_segment", loc_data);

  /* Iterate over the children under this node,
   * each child should be named after switch
   */
  for (pugi::xml_node xml_seg : xml_segments.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_seg.name() != std::string("segment")) {
      bad_tag(xml_seg, loc_data, xml_segments, {"segment"});
    }
    /* Get the switch name */
    std::string seg_name = get_attribute(xml_seg, "name", loc_data).as_string();

    /* Get the routing segment circuit model name */
    std::string seg_model_name =
      get_attribute(xml_seg, "circuit_model_name", loc_data).as_string();

    CircuitModelId seg_model = find_routing_circuit_model(
      xml_seg, loc_data, circuit_lib, seg_model_name, CIRCUIT_MODEL_CHAN_WIRE);

    /* Ensure that there is no duplicated seg names defined here */
    std::map<std::string, CircuitModelId>::const_iterator it =
      seg2circuit.find(seg_name);
    if (it != seg2circuit.end()) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_seg),
                     "Segment name '%s' has been defined more than once!\n",
                     seg_name.c_str());
    }

    /* Pass all the check, we can add it to the map */
    seg2circuit[seg_name] = seg_model;
  }

  return seg2circuit;
}

/********************************************************************
 * Convert string to the enumerate of direct type
 *******************************************************************/
static e_direct_type string_to_direct_type(const std::string& type_string) {
  if (std::string("part_of_cb") == type_string) {
    return e_direct_type::PART_OF_CB;
  }
  if (std::string("inner_column_or_row") == type_string) {
    return e_direct_type::INNER_COLUMN_OR_ROW;
  }
  if (std::string("inter_column") == type_string) {
    return e_direct_type::INTER_COLUMN;
  }
  if (std::string("inter_row") == type_string) {
    return e_direct_type::INTER_ROW;
  }

  return e_direct_type::NUM_DIRECT_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of direct direction type
 *******************************************************************/
static e_direct_direction string_to_direct_direction(
  const std::string& type_string) {
  if (std::string("positive") == type_string) {
    return POSITIVE_DIR;
  }

  if (std::string("negative") == type_string) {
    return NEGATIVE_DIR;
  }

  return NUM_DIRECT_DIRECTIONS;
}

/********************************************************************
 * Parse XML codes about <direct_connection> to an object of name-to-circuit
 *mapping Note: this function should be called AFTER the parsing of circuit
 *library!!!
 *******************************************************************/
ArchDirect read_xml_direct_circuit(pugi::xml_node& Node,
                                   const pugiutil::loc_data& loc_data,
                                   const CircuitLibrary& circuit_lib) {
  ArchDirect arch_direct;

  /* Parse direct list, this is optional. May not be used */
  pugi::xml_node xml_directs = get_single_child(
    Node, "direct_connection", loc_data, pugiutil::ReqOpt::OPTIONAL);
  /* Not found, we can return */
  if (!xml_directs) {
    return arch_direct;
  }

  /* Iterate over the children under this node,
   * each child should be named after switch
   */
  for (pugi::xml_node xml_direct : xml_directs.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_direct.name() != std::string("direct")) {
      bad_tag(xml_direct, loc_data, xml_directs, {"direct"});
    }
    /* Get the switch name */
    std::string direct_name =
      get_attribute(xml_direct, "name", loc_data).as_string();

    /* Add to the Arch direct database */
    ArchDirectId direct = arch_direct.add_direct(direct_name);
    if (false == arch_direct.valid_direct_id(direct)) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_direct),
                     "Direct name '%s' has been defined more than once!\n",
                     direct_name.c_str());
    }

    /* Add more information*/
    std::string direct_type_name =
      get_attribute(xml_direct, "type", loc_data, pugiutil::ReqOpt::OPTIONAL)
        .as_string(
          DIRECT_TYPE_STRING[size_t(e_direct_type::INNER_COLUMN_OR_ROW)]);

    e_direct_type direct_type = string_to_direct_type(direct_type_name);

    if (e_direct_type::NUM_DIRECT_TYPES == direct_type) {
      archfpga_throw(
        loc_data.filename_c_str(), loc_data.line(xml_direct),
        "Direct type '%s' is not support! Acceptable values are "
        "[inner_column_or_row|part_of_cb|inter_column|inter_row]\n",
        direct_type_name.c_str());
    }

    arch_direct.set_type(direct, direct_type);

    /* Get the routing segment circuit model name */
    std::string direct_model_name =
      get_attribute(xml_direct, "circuit_model_name", loc_data).as_string();

    /* If a direct connection is part of a connection block, the circuit model
     * should be a MUX */
    e_circuit_model_type expected_circuit_model_type = CIRCUIT_MODEL_WIRE;
    if (arch_direct.type(direct) == e_direct_type::PART_OF_CB) {
      VTR_LOG("Direct '%s' will modelled as part of a connection block.\n",
              direct_name.c_str());
      expected_circuit_model_type = CIRCUIT_MODEL_MUX;
    }
    CircuitModelId direct_model = find_routing_circuit_model(
      xml_direct, loc_data, circuit_lib, direct_model_name,
      expected_circuit_model_type);
    arch_direct.set_circuit_model(direct, direct_model);

    /* The following syntax is only available for inter-column/row */
    if (arch_direct.type(direct) != e_direct_type::INTER_COLUMN &&
        arch_direct.type(direct) != e_direct_type::INTER_ROW) {
      continue;
    }

    std::string x_dir_name =
      get_attribute(xml_direct, "x_dir", loc_data).as_string();
    std::string y_dir_name =
      get_attribute(xml_direct, "y_dir", loc_data).as_string();
    e_direct_direction x_dir = string_to_direct_direction(x_dir_name);
    e_direct_direction y_dir = string_to_direct_direction(y_dir_name);

    if (NUM_DIRECT_DIRECTIONS == x_dir) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_direct),
                     "Direct x-direction '%s' is not support! Acceptable "
                     "values are [positive|column]\n",
                     x_dir_name.c_str());
    }

    if (NUM_DIRECT_DIRECTIONS == y_dir) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_direct),
                     "Direct y-direction '%s' is not support! Acceptable "
                     "values are [positive|column]\n",
                     y_dir_name.c_str());
    }

    arch_direct.set_direction(direct, x_dir, y_dir);
  }

  return arch_direct;
}
