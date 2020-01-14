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
 * Convert string to the enumerate of model type
 *******************************************************************/
static 
e_circuit_model_design_tech string_to_design_tech_type(const std::string& type_string) {
  if (std::string("cmos") == type_string) {
    return CIRCUIT_MODEL_DESIGN_CMOS;
  }

  if (std::string("rram") == type_string) {
    return CIRCUIT_MODEL_DESIGN_RRAM;
  }

  return NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of buffer type
 *******************************************************************/
static 
e_circuit_model_buffer_type string_to_buffer_type(const std::string& type_string) {
  if (std::string("inverter") == type_string) {
    return CIRCUIT_MODEL_BUF_INV;
  }

  if (std::string("buffer") == type_string) {
    return CIRCUIT_MODEL_BUF_BUF;
  }

  return NUM_CIRCUIT_MODEL_BUF_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of pass-gate-logic type
 *******************************************************************/
static 
e_circuit_model_pass_gate_logic_type string_to_passgate_type(const std::string& type_string) {
  if (std::string("transmission_gate") == type_string) {
    return CIRCUIT_MODEL_PASS_GATE_TRANSMISSION;
  }

  if (std::string("pass_transistor") == type_string) {
    return CIRCUIT_MODEL_PASS_GATE_TRANSISTOR;
  }

  return NUM_CIRCUIT_MODEL_PASS_GATE_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of multiplexer structure
 *******************************************************************/
static 
e_circuit_model_structure string_to_mux_structure_type(const std::string& type_string) {
  if (std::string("tree") == type_string) {
    return CIRCUIT_MODEL_STRUCTURE_TREE;
  }

  if (std::string("one-level") == type_string) {
    return CIRCUIT_MODEL_STRUCTURE_ONELEVEL;
  }

  if (std::string("multi-level") == type_string) {
    return CIRCUIT_MODEL_STRUCTURE_MULTILEVEL;
  }

  return NUM_CIRCUIT_MODEL_STRUCTURE_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of logic gate type
 *******************************************************************/
static 
e_circuit_model_gate_type string_to_gate_type(const std::string& type_string) {
  if (std::string("AND") == type_string) {
    return CIRCUIT_MODEL_GATE_AND;
  }

  if (std::string("OR") == type_string) {
    return CIRCUIT_MODEL_GATE_OR;
  }

  if (std::string("MUX2") == type_string) {
    return CIRCUIT_MODEL_GATE_MUX2;
  }

  return NUM_CIRCUIT_MODEL_GATE_TYPES;
}

/********************************************************************
 * Parse XML codes of design technology of a circuit model to circuit library
 *******************************************************************/
static 
void read_xml_model_design_technology(pugi::xml_node& xml_model,
                                      const pugiutil::loc_data& loc_data,
                                      CircuitLibrary& circuit_lib, const CircuitModelId& model) {

  auto xml_design_tech = get_single_child(xml_model, "design_technology", loc_data); 

  /* Identify if the circuit model power-gated */
  circuit_lib.set_model_is_power_gated(model, get_attribute(xml_design_tech, "power_gated", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Identify the type of design technology */
  const char* type_attr = get_attribute(xml_design_tech, "type", loc_data).value();
  /* Translate the type of design technology to enumerate */
  e_circuit_model_design_tech design_tech_type = string_to_design_tech_type(std::string(type_attr));

  if (NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES == design_tech_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_design_tech),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  circuit_lib.set_model_design_tech_type(model, design_tech_type);
   
  /* Parse exclusive attributes for inverters and buffers */
  if (CIRCUIT_MODEL_INVBUF == circuit_lib.model_type(model)) {
    /* Identify the topology of the buffer */
    const char* topology_attr = get_attribute(xml_design_tech, "topology", loc_data).value();
    /* Translate the type of buffer to enumerate */
    e_circuit_model_buffer_type buf_type = string_to_buffer_type(std::string(topology_attr));

    if (NUM_CIRCUIT_MODEL_BUF_TYPES == buf_type) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_design_tech),
                     "Invalid 'topology' attribute '%s'\n",
                     topology_attr);
    }  

    circuit_lib.set_buffer_type(model, buf_type);

    /* Parse the others options: 
     * 1. size of buffer in the first stage 
     * 2. number of levels 
     * 3. driving strength per stage
     */
    circuit_lib.set_buffer_size(model, get_attribute(xml_design_tech, "size", loc_data).as_float(0.));
    circuit_lib.set_buffer_num_levels(model, get_attribute(xml_design_tech, "tap_drive_level", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(0));
    circuit_lib.set_buffer_f_per_stage(model, get_attribute(xml_design_tech, "f_per_stage", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(4));
  }
  
  /* Parse exclusive attributes for pass-gate logics */
  if (CIRCUIT_MODEL_PASSGATE == circuit_lib.model_type(model)) {
    /* Identify the topology of the pass-gate logic */
    const char* topology_attr = get_attribute(xml_design_tech, "topology", loc_data).value();
    /* Translate the type of pass-gate logic to enumerate */
    e_circuit_model_pass_gate_logic_type passgate_type = string_to_passgate_type(std::string(topology_attr));

    if (NUM_CIRCUIT_MODEL_PASS_GATE_TYPES == passgate_type) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_design_tech),
                     "Invalid 'topology' attribute '%s'\n",
                     topology_attr);
    }  

    circuit_lib.set_pass_gate_logic_type(model, passgate_type);
    /* Parse the others options: 
     * 1. pmos size to be used in the pass gate logic 
     * 2. nmos size to be used in the pass gate logic 
     */
    circuit_lib.set_pass_gate_logic_pmos_size(model, get_attribute(xml_design_tech, "pmos_size", loc_data).as_float(0.));
    circuit_lib.set_pass_gate_logic_nmos_size(model, get_attribute(xml_design_tech, "nmos_size", loc_data).as_float(0.));
  }

  /* Parse exclusive attributes for Look-Up Tables (LUTs) */
  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model)) {
    /* Identify if this is a fracturable LUT */
    circuit_lib.set_lut_is_fracturable(model, get_attribute(xml_design_tech, "fracturable_lut", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));
    /* Set default MUX-relate attributes as LUT contains a tree-like MUX */
    circuit_lib.set_mux_structure(model, CIRCUIT_MODEL_STRUCTURE_TREE);
    circuit_lib.set_mux_use_local_encoder(model, false);
    circuit_lib.set_mux_use_advanced_rram_design(model, false);
  }

  /* Parse exclusive attributes for multiplexers */
  if (CIRCUIT_MODEL_MUX == circuit_lib.model_type(model)) {
    /* Set default values for multiplexer structure */
    if (CIRCUIT_MODEL_DESIGN_CMOS == circuit_lib.design_tech_type(model)) {
      circuit_lib.set_mux_structure(model, CIRCUIT_MODEL_STRUCTURE_TREE);
    } else {
      VTR_ASSERT_SAFE(CIRCUIT_MODEL_DESIGN_RRAM == circuit_lib.design_tech_type(model));
      circuit_lib.set_mux_structure(model, CIRCUIT_MODEL_STRUCTURE_ONELEVEL);
    }
    /* Identify the topology of the multiplexer structure */
    const char* structure_attr = get_attribute(xml_design_tech, "structure", loc_data).value();
    /* Translate the type of multiplexer structure to enumerate */
    e_circuit_model_structure mux_structure = string_to_mux_structure_type(std::string(structure_attr));
  
    if (NUM_CIRCUIT_MODEL_STRUCTURE_TYPES == mux_structure) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_design_tech),
                     "Invalid 'structure' attribute '%s'\n",
                     structure_attr);
    }  

    circuit_lib.set_mux_structure(model, mux_structure);

    /* Parse the others options: 
     * 1. constant input values
     * 2. number of levels if multi-level multiplexer structure is selected
     * 3. if advanced ReRAM design is used
     * 4. if local encoder is to be used 
     */
    if (true == get_attribute(xml_design_tech, "add_const_input", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false)) {
      circuit_lib.set_mux_const_input_value(model, get_attribute(xml_design_tech, "const_input_val", loc_data).as_int(0));
    }
    if (CIRCUIT_MODEL_STRUCTURE_MULTILEVEL == circuit_lib.mux_structure(model)) {
      circuit_lib.set_mux_num_levels(model, get_attribute(xml_design_tech, "num_level", loc_data).as_int(1));
    }
    circuit_lib.set_mux_use_advanced_rram_design(model, get_attribute(xml_design_tech, "advanced_rram_design", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));
    circuit_lib.set_mux_use_local_encoder(model, get_attribute(xml_design_tech, "local_encoder", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));
  }

  /* Parse exclusive attributes for logic gates */
  if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(model)) {
    /* Identify the topology of the logic gate */
    const char* topology_attr = get_attribute(xml_design_tech, "topology", loc_data).value();
    /* Translate the type of logic gate to enumerate */
    e_circuit_model_gate_type gate_type = string_to_gate_type(std::string(topology_attr));

    if (NUM_CIRCUIT_MODEL_GATE_TYPES == gate_type) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_design_tech),
                     "Invalid 'topology' attribute '%s'\n",
                     topology_attr);
    }  

    circuit_lib.set_gate_type(model, gate_type);
  }

  /* Parse exclusive attributes for RRAM */
  if (CIRCUIT_MODEL_DESIGN_RRAM == circuit_lib.design_tech_type(model)) {
    circuit_lib.set_rram_rlrs(model, get_attribute(xml_design_tech, "ron", loc_data).as_float(0.));
    circuit_lib.set_rram_rhrs(model, get_attribute(xml_design_tech, "roff", loc_data).as_float(0.));
    circuit_lib.set_rram_wprog_set_pmos(model, get_attribute(xml_design_tech, "wprog_set_pmos", loc_data).as_float(0.));
    circuit_lib.set_rram_wprog_set_nmos(model, get_attribute(xml_design_tech, "wprog_set_nmos", loc_data).as_float(0.));
    circuit_lib.set_rram_wprog_reset_pmos(model, get_attribute(xml_design_tech, "wprog_reset_pmos", loc_data).as_float(0.));
    circuit_lib.set_rram_wprog_reset_nmos(model, get_attribute(xml_design_tech, "wprog_reset_nmos", loc_data).as_float(0.));
  }

}

/********************************************************************
 * Parse XML codes of a circuit model to circuit library
 *******************************************************************/
static 
void read_xml_circuit_model(pugi::xml_node& xml_model,
                            const pugiutil::loc_data& loc_data,
                            CircuitLibrary& circuit_lib) {
  /* Find the type of the circuit model
   * so that we can add a new circuit model to circuit library 
   */
  const char* type_attr = get_attribute(xml_model, "type", loc_data).value();

  /* Translate the type of circuit model to enumerate */
  e_circuit_model_type model_type = string_to_circuit_model_type(std::string(type_attr));

  if (NUM_CIRCUIT_MODEL_TYPES == model_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_model),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  CircuitModelId model = circuit_lib.add_model(model_type);

  /* Find the name of the circuit model */
  const char* name_attr = get_attribute(xml_model, "name", loc_data).value();
  circuit_lib.set_model_name(model, std::string(name_attr));

  /* TODO: This attribute is going to be DEPRECATED 
   * Find the prefix of the circuit model
   */
  const char* prefix_attr = get_attribute(xml_model, "prefix", loc_data).value();
  circuit_lib.set_model_prefix(model, std::string(prefix_attr));

  /* Find a SPICE netlist which is an optional attribute*/
  circuit_lib.set_model_circuit_netlist(model, get_attribute(xml_model, "spice_netlist", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string(""));

  /* Find a Verilog netlist which is an optional attribute*/
  circuit_lib.set_model_verilog_netlist(model, get_attribute(xml_model, "verilog_netlist", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string(""));

  /* Find if the circuit model is default in its type */
  circuit_lib.set_model_is_default(model, get_attribute(xml_model, "is_default", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));
  
  /* Find if the circuit model is should be dumped in structural verilog */
  circuit_lib.set_model_dump_structural_verilog(model, get_attribute(xml_model, "dump_structural_verilog", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Parse attributes under the <circuit_model> */
  /* Design technology -related attributes */
  read_xml_model_design_technology(xml_model, loc_data, circuit_lib, model);
  
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
  for (pugi::xml_node xml_model : Node.children()) {
    /* Error out if the XML child has an invalid name! */
    if (xml_model.name() != std::string("circuit_model")) {
      bad_tag(xml_model, loc_data, Node, {"circuit_model"});
    }
    read_xml_circuit_model(xml_model, loc_data, circuit_lib);
  } 

  return circuit_lib;
}

