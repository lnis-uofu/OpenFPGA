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

/* Headers from openfpgautil library */
#include "openfpga_tokenizer.h"

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
 * Convert string to the enumerate of circuit model port type
 *******************************************************************/
static 
e_circuit_model_port_type string_to_circuit_model_port_type(const std::string& type_string) {
  if (std::string("input") == type_string) {
    return CIRCUIT_MODEL_PORT_INPUT;
  }

  if (std::string("output") == type_string) {
    return CIRCUIT_MODEL_PORT_OUTPUT;
  }

  if (std::string("clock") == type_string) {
    return CIRCUIT_MODEL_PORT_CLOCK;
  }

  if (std::string("sram") == type_string) {
    return CIRCUIT_MODEL_PORT_SRAM;
  }

  if (std::string("bl") == type_string) {
    return CIRCUIT_MODEL_PORT_BL;
  }

  if (std::string("wl") == type_string) {
    return CIRCUIT_MODEL_PORT_WL;
  }

  if (std::string("blb") == type_string) {
    return CIRCUIT_MODEL_PORT_BLB;
  }

  if (std::string("wlb") == type_string) {
    return CIRCUIT_MODEL_PORT_WLB;
  }

  if (std::string("inout") == type_string) {
    return CIRCUIT_MODEL_PORT_INOUT;
  }

  return NUM_CIRCUIT_MODEL_PORT_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of wire model type
 *******************************************************************/
static 
e_wire_model_type string_to_wire_model_type(const std::string& type_string) {
  if (std::string("pi") == type_string) {
    return WIRE_MODEL_PI;
  }

  if (std::string("t") == type_string) {
    return WIRE_MODEL_T;
  }

  return NUM_WIRE_MODEL_TYPES;
}

/********************************************************************
 * Convert string to the enumerate of delay model type
 *******************************************************************/
static 
e_circuit_model_delay_type string_to_circuit_model_delay_type(const std::string& type_string) {
  if (std::string("rise") == type_string) {
    return CIRCUIT_MODEL_DELAY_RISE;
  }

  if (std::string("fall") == type_string) {
    return CIRCUIT_MODEL_DELAY_FALL;
  }

  return NUM_CIRCUIT_MODEL_DELAY_TYPES;
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
 * This is a generic function to parse XML codes that describe
 * a buffer of a circuit model to circuit library
 * This function will return a string with the circuit model name 
 * linked to the buffer
 * If the return is empty, it means that buffer does NOT exist
  *******************************************************************/
static 
std::string read_xml_buffer(pugi::xml_node& xml_buffer,
                            const pugiutil::loc_data& loc_data) {
  bool buffer_existence = get_attribute(xml_buffer, "exist", loc_data).as_bool(false);
  std::string buffer_circuit_model_name("");
   
  if (true == buffer_existence) { 
    buffer_circuit_model_name = get_attribute(xml_buffer, "circuit_model_name", loc_data).as_string();
  }

  return buffer_circuit_model_name;
}

/********************************************************************
 * Identify the output mask of the port in LUTs, 
 * by default it will be applied to each pin of this port
 * This is only applicable to output ports of a LUT
 *******************************************************************/
static 
void read_xml_output_mask(pugi::xml_node& xml_port,
                          const pugiutil::loc_data& loc_data,
                          CircuitLibrary& circuit_lib, const CircuitPortId& port) {
  const char* output_mask_attr = get_attribute(xml_port, "lut_output_mask", loc_data, pugiutil::ReqOpt::OPTIONAL).value();
  std::vector<size_t> mask_vector;
  if (nullptr != output_mask_attr) {
    /* Split the string with token ',' */
    openfpga::StringToken string_tokenizer(get_attribute(xml_port, "lut_output_mask", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string(nullptr));
    for (const std::string& mask_token : string_tokenizer.split(',')) {
      mask_vector.push_back(std::atoi(mask_token.c_str()));
    }
    /* Make sure that the size of mask fits the port size */
    if (circuit_lib.port_size(port) != mask_vector.size()) {
      archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_port),
                     "Invalid lut_output_mask attribute '%s'! It must match the port size (=%lu)\n",
                     output_mask_attr, circuit_lib.port_size(port));
    } 
  } else {
    /* By default, we give a mask vector covering each pin of the port */
    mask_vector.resize(circuit_lib.port_size(port));
    std::iota(mask_vector.begin(), mask_vector.end(), 0);
  }
  circuit_lib.set_port_lut_output_mask(port, mask_vector);
}

/********************************************************************
 * This is a generic function to parse XML codes that describe
 * a port of a circuit model to circuit library
  *******************************************************************/
static 
void read_xml_circuit_port(pugi::xml_node& xml_port,
                           const pugiutil::loc_data& loc_data,
                           CircuitLibrary& circuit_lib, const CircuitModelId& model) {
  /* Find the type of the circuit port
   * so that we can add a new circuit port to circuit library 
   */
  const char* type_attr = get_attribute(xml_port, "type", loc_data).value();

  /* Translate the type of circuit model to enumerate */
  e_circuit_model_port_type port_type = string_to_circuit_model_port_type(std::string(type_attr));

  if (NUM_CIRCUIT_MODEL_PORT_TYPES == port_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_port),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  CircuitPortId port = circuit_lib.add_model_port(model, port_type);

  /* Parse the name of the port */
  circuit_lib.set_port_prefix(port, get_attribute(xml_port, "prefix", loc_data).as_string());

  /* Parse the name of the port in cell library. By default, the lib_name is the same as port name */
  const char* lib_name_attr = get_attribute(xml_port, "lib_name", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string(nullptr);
  if (nullptr != lib_name_attr) {
    circuit_lib.set_port_lib_name(port, get_attribute(xml_port, "lib_name", loc_data).as_string());
  } else {
    circuit_lib.set_port_lib_name(port, circuit_lib.port_prefix(port));
  }

  /* Parse the port size, by default it will be 1 */
  circuit_lib.set_port_size(port, get_attribute(xml_port, "size", loc_data).as_int(1));

  /* Identify if the port is for mode selection, this is only applicable to SRAM ports.
   * By default, it will NOT be a mode selection port
   */
  if (CIRCUIT_MODEL_PORT_SRAM == circuit_lib.port_type(port)) {
    circuit_lib.set_port_is_mode_select(port, get_attribute(xml_port, "mode_select", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));
  } 

  /* Identify the default value of the port, by default it will be 0 */
  circuit_lib.set_port_default_value(port, get_attribute(xml_port, "default_val", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(0));

  /* Identify the tri-state map of the port, by default it will be nullptr 
   * This is only applicable to input ports of a LUT
   */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model))
    && (CIRCUIT_MODEL_PORT_INPUT == circuit_lib.port_type(port)) ) {
    const char* tri_state_map_attr = get_attribute(xml_port, "tri_state_map", loc_data, pugiutil::ReqOpt::OPTIONAL).value();
    if (nullptr != tri_state_map_attr) {
      circuit_lib.set_port_tri_state_map(port, get_attribute(xml_port, "tri_state_map", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string());
    }
  }

  /* Identify the fracturable-level of the port in LUTs, by default it will be -1
   * This is only applicable to output ports of a LUT
   */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model))
    && (CIRCUIT_MODEL_PORT_OUTPUT == circuit_lib.port_type(port)) ) {
    circuit_lib.set_port_lut_frac_level(port, get_attribute(xml_port, "lut_frac_level", loc_data, pugiutil::ReqOpt::OPTIONAL).as_int(-1));
  }

  /* Identify the output mask of the port in LUTs, by default it will be applied to each pin of this port
   * This is only applicable to output ports of a LUT
   */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model))
    && (CIRCUIT_MODEL_PORT_OUTPUT == circuit_lib.port_type(port)) ) {
    read_xml_output_mask(xml_port, loc_data, circuit_lib, port);
  }

  /* Identify if the port is a global port, by default it is NOT */
  circuit_lib.set_port_is_global(port, get_attribute(xml_port, "is_global", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Identify if the port is a reset port, by default it is NOT */
  circuit_lib.set_port_is_reset(port, get_attribute(xml_port, "is_reset", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Identify if the port is a set port, by default it is NOT */
  circuit_lib.set_port_is_set(port, get_attribute(xml_port, "is_set", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Identify if the port is in programming purpose, by default it is NOT */
  circuit_lib.set_port_is_prog(port, get_attribute(xml_port, "is_prog", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Identify if the port is to enable programming for FPGAs, by default it is NOT */
  circuit_lib.set_port_is_config_enable(port, get_attribute(xml_port, "is_config_enable", loc_data, pugiutil::ReqOpt::OPTIONAL).as_bool(false));

  /* Find the name of circuit model that this port is linked to */
  circuit_lib.set_port_tri_state_model_name(port, get_attribute(xml_port, "circuit_model_name", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string());

  /* Find the name of circuit model that port is used for inversion of signals,
   * This is only applicable to BL/WL/BLB/WLB ports
   */
  if (  (CIRCUIT_MODEL_PORT_BL  == circuit_lib.port_type(port))
     || (CIRCUIT_MODEL_PORT_WL  == circuit_lib.port_type(port))
     || (CIRCUIT_MODEL_PORT_BLB == circuit_lib.port_type(port))
     || (CIRCUIT_MODEL_PORT_WLB == circuit_lib.port_type(port)) ) {
    circuit_lib.set_port_inv_model_name(port, get_attribute(xml_port, "inv_circuit_model_name", loc_data, pugiutil::ReqOpt::OPTIONAL).as_string(nullptr));
  }
}

/********************************************************************
 * This is a generic function to parse XML codes that describe
 * RC parasitics for wire circuit model to circuit library
  *******************************************************************/
static 
void read_xml_wire_param(pugi::xml_node& xml_wire_param,
                         const pugiutil::loc_data& loc_data,
                         CircuitLibrary& circuit_lib, const CircuitModelId& model) {
  /* Find the type of the wire model */
  const char* type_attr = get_attribute(xml_wire_param, "model_type", loc_data).value();

  /* Translate the type of circuit model to enumerate */
  e_wire_model_type wire_model_type = string_to_wire_model_type(std::string(type_attr));

  if (NUM_WIRE_MODEL_TYPES == wire_model_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_wire_param),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  circuit_lib.set_wire_type(model, wire_model_type);

  /* Parse the R and C values */
  circuit_lib.set_wire_r(model, get_attribute(xml_wire_param, "R", loc_data).as_float(0.));
  circuit_lib.set_wire_c(model, get_attribute(xml_wire_param, "C", loc_data).as_float(0.));

  /* Parse the number of levels for the wire model */
  circuit_lib.set_wire_num_levels(model, get_attribute(xml_wire_param, "num_level", loc_data).as_int(0));
}

/********************************************************************
 * This is a generic function to parse XML codes that describe
 * a delay matrix for a circuit model to circuit library
  *******************************************************************/
static 
void read_xml_delay_matrix(pugi::xml_node& xml_delay_matrix,
                           const pugiutil::loc_data& loc_data,
                           CircuitLibrary& circuit_lib, const CircuitModelId& model) {
  /* Find the type of the delay model, so that we can add to circuit library */
  const char* type_attr = get_attribute(xml_delay_matrix, "type", loc_data).value();

  /* Translate the type of delay matrix for a circuit model to enumerate */
  e_circuit_model_delay_type delay_type = string_to_circuit_model_delay_type(std::string(type_attr));

  if (NUM_CIRCUIT_MODEL_DELAY_TYPES == delay_type) {
    archfpga_throw(loc_data.filename_c_str(), loc_data.line(xml_delay_matrix),
                   "Invalid 'type' attribute '%s'\n",
                   type_attr);
  }

  circuit_lib.add_delay_info(model, delay_type);

  /* Parse the input ports */
  circuit_lib.set_delay_in_port_names(model, delay_type, get_attribute(xml_delay_matrix, "in_port", loc_data).as_string());

  /* Parse the output ports */
  circuit_lib.set_delay_out_port_names(model, delay_type, get_attribute(xml_delay_matrix, "out_port", loc_data).as_string());

  /* Parse the delay values */
  circuit_lib.set_delay_values(model, delay_type, std::string(xml_delay_matrix.child_value())); 
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
  
  /* Parse special buffer attributes required by LUTs only */
  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model)) { 
    /* Input buffer of LUTs */
    auto xml_input_buffer = get_single_child(xml_model, "lut_input_buffer", loc_data); 
    std::string input_buffer_circuit_model_name = read_xml_buffer(xml_input_buffer, loc_data);
    circuit_lib.set_model_lut_input_buffer(model, 
                                           true != input_buffer_circuit_model_name.empty(), 
                                           input_buffer_circuit_model_name);

    /* Input inverter of LUTs */
    auto xml_input_inverter = get_single_child(xml_model, "lut_input_inverter", loc_data); 
    std::string input_inverter_circuit_model_name = read_xml_buffer(xml_input_inverter, loc_data);
    circuit_lib.set_model_lut_input_inverter(model, 
                                             true != input_inverter_circuit_model_name.empty(), 
                                             input_inverter_circuit_model_name);

    /* Intermediate buffer of LUTs */
    auto xml_intermediate_buffer = get_single_child(xml_model, "lut_intermediate_buffer", loc_data, pugiutil::ReqOpt::OPTIONAL); 
    if (xml_intermediate_buffer) {
      std::string intermediate_buffer_circuit_model_name = read_xml_buffer(xml_intermediate_buffer, loc_data);
      circuit_lib.set_model_lut_intermediate_buffer(model, 
                                                    true != intermediate_buffer_circuit_model_name.empty(), 
                                                    intermediate_buffer_circuit_model_name);
      /* If intermediate buffer is defined, try to find the location map */
      if (true != intermediate_buffer_circuit_model_name.empty()) { 
        circuit_lib.set_model_lut_intermediate_buffer_location_map(model, get_attribute(xml_intermediate_buffer, "location_map", loc_data).as_string());
      }
    }
  }

  /* Input buffer attributes, NOT required for circuit models which are inverters or buffers */
  if (CIRCUIT_MODEL_INVBUF != circuit_lib.model_type(model)) {
    auto xml_input_buffer = get_single_child(xml_model, "input_buffer", loc_data); 
    std::string input_buffer_circuit_model_name = read_xml_buffer(xml_input_buffer, loc_data);
    circuit_lib.set_model_input_buffer(model, 
                                       true != input_buffer_circuit_model_name.empty(), 
                                       input_buffer_circuit_model_name);
  }

  /* Output buffer attributes, NOT required for circuit models which are inverters or buffers */
  if (CIRCUIT_MODEL_INVBUF != circuit_lib.model_type(model)) {
    auto xml_output_buffer = get_single_child(xml_model, "output_buffer", loc_data); 
    std::string output_buffer_circuit_model_name = read_xml_buffer(xml_output_buffer, loc_data);
    circuit_lib.set_model_output_buffer(model, 
                                        true != output_buffer_circuit_model_name.empty(), 
                                        output_buffer_circuit_model_name);
  }

  /* Pass-gate-logic attributes, required by LUT and MUX */
  if ( (CIRCUIT_MODEL_LUT == circuit_lib.model_type(model)) 
    || (CIRCUIT_MODEL_MUX == circuit_lib.model_type(model)) ) {
    auto xml_pass_gate_logic = get_single_child(xml_model, "pass_gate_logic", loc_data); 
    circuit_lib.set_model_pass_gate_logic(model, get_attribute(xml_pass_gate_logic, "circuit_model_name", loc_data).as_string());
  } 

  /* Parse all the ports belonging to this circuit model
   * We count the number of ports in total and then add one by one 
   */
  size_t num_ports = count_children(xml_model, "port", loc_data, pugiutil::ReqOpt::OPTIONAL);
  for (size_t iport = 0; iport < num_ports; ++iport) {
    auto xml_port = get_first_child(xml_model, "port", loc_data);
    read_xml_circuit_port(xml_port, loc_data, circuit_lib, model);
  }

  /* Parse the parasitics of wires */
  if ( (CIRCUIT_MODEL_WIRE == circuit_lib.model_type(model))
    || (CIRCUIT_MODEL_CHAN_WIRE == circuit_lib.model_type(model)) ) {
    auto xml_wire_param = get_single_child(xml_model, "wire_param", loc_data); 
    read_xml_wire_param(xml_wire_param, loc_data, circuit_lib, model);
  }

  /* Parse all the delay matrix if defined */
  size_t num_delay_matrix = count_children(xml_model, "delay_matrix", loc_data, pugiutil::ReqOpt::OPTIONAL);
  for (size_t idelay_matrix = 0; idelay_matrix < num_delay_matrix; ++idelay_matrix) {
    auto xml_delay_matrix = get_first_child(xml_model, "delay_matrix", loc_data);
    read_xml_delay_matrix(xml_delay_matrix, loc_data, circuit_lib, model);
  }
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

