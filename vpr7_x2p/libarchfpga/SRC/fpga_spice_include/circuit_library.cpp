/**********************************************************
 * MIT License
 *
 * Copyright (c) 2018 LNIS - The University of Utah
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ***********************************************************************/

/************************************************************************
 * Filename:    circuit_library.cpp
 * Created by:   Xifan Tang
 * Change history:
 * +-------------------------------------+
 * |  Date       |    Author   | Notes
 * +-------------------------------------+
 * | 2019/08/07  |  Xifan Tang | Created 
 * +-------------------------------------+
 ***********************************************************************/

#include <algorithm>

#include "vtr_assert.h"

#include "circuit_library.h"

/************************************************************************
 * Member functions for class CircuitLibrary
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/

/************************************************************************
 * Public Accessors : aggregates
 ***********************************************************************/
CircuitLibrary::circuit_model_range CircuitLibrary::circuit_models() const {
  return vtr::make_range(circuit_model_ids_.begin(), circuit_model_ids_.end());
}


/************************************************************************
 * Public Accessors : Methods to find circuit model 
 ***********************************************************************/
/* Find a circuit model by a given name and return its id */
CircuitModelId CircuitLibrary::get_circuit_model_id_by_name(const std::string& name) const { 
  CircuitModelId ret = CIRCUIT_MODEL_OPEN_ID;
  size_t num_found = 0;
  for (circuit_model_string_iterator it = circuit_model_names_.begin();
       it != circuit_model_names_.end();
       it++) {
    /* Bypass unmatched names */
    if ( 0 != name.compare(*it) ) {
      continue;
    }
    /* Find one and record it 
     * FIXME: I feel that we may have a better way in getting the CircuitModelId
     */
    ret = CircuitModelId(it - circuit_model_names_.begin()); 
    num_found++;
  }
  VTR_ASSERT((0 == num_found) || (1 == num_found));
  return ret;
}

/* Get the CircuitModelId of a default circuit model with a given type */
CircuitModelId CircuitLibrary::get_default_circuit_model_id(const enum e_spice_model_type& type) const {
  /* Default circuit model id is the first element by type in the fast look-up */
  return circuit_model_lookup_[size_t(type)].front();
}

/************************************************************************
 * Public Mutators 
 ***********************************************************************/
/* Add a circuit model to the library, and return it Id */
CircuitModelId CircuitLibrary::add_circuit_model() {
  /* Create a new id*/
  CircuitModelId circuit_model_id = CircuitModelId(circuit_model_ids_.size());
  /* Update the id list */
  circuit_model_ids_.push_back(circuit_model_id);
  
  /* Initialize other attributes */
  /* Fundamental information */
  circuit_model_types_.push_back(NUM_CIRCUIT_MODEL_TYPES);
  circuit_model_names_.emplace_back();
  circuit_model_prefix_.emplace_back();
  circuit_model_verilog_netlists_.emplace_back();
  circuit_model_spice_netlists_.emplace_back();
  circuit_model_is_default_.push_back(false);

  /* Verilog generator options */ 
  dump_structural_verilog_.push_back(false);
  dump_explicit_port_map_.push_back(false);
 
  /* Design technology information */ 
  design_tech_types_.push_back(NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES);
  power_gated_.push_back(false);
  
  /* Buffer existence */
  buffer_existence_.emplace_back();
  buffer_circuit_model_names_.emplace_back();
  buffer_circuit_model_ids_.emplace_back();

  /* Pass-gate-related parameters */
  pass_gate_logic_circuit_model_names_.emplace_back();
  pass_gate_logic_circuit_model_ids_.emplace_back();

  /* Port information */
  port_ids_.emplace_back();
  port_types_.emplace_back();
  port_sizes_.emplace_back();
  port_prefix_.emplace_back();
  port_lib_names_.emplace_back();
  port_is_mode_select_.emplace_back();
  port_is_global_.emplace_back();
  port_is_reset_.emplace_back();
  port_is_set_.emplace_back();
  port_is_config_enable_.emplace_back();
  port_is_prog_.emplace_back();
  port_circuit_model_names_.emplace_back();
  port_circuit_model_ids_.emplace_back();
  port_inv_circuit_model_names_.emplace_back();
  port_inv_circuit_model_ids_.emplace_back();
  port_tri_state_maps_.emplace_back();
  port_lut_frac_level_.emplace_back();
  port_lut_output_masks_.emplace_back();
  port_sram_orgz_.emplace_back();

  /* Timing graphs */
  edge_ids_.emplace_back();
  port_in_edge_ids_.emplace_back();
  port_out_edge_ids_.emplace_back();
  edge_src_ports_.emplace_back();
  edge_src_pin_ids_.emplace_back();
  edge_sink_ports_.emplace_back();
  edge_sink_pin_ids_.emplace_back();
  edge_trise_.emplace_back();
  edge_tfall_.emplace_back();

  /* Delay information */
  delay_types_.emplace_back();
  delay_in_port_names_.emplace_back();
  delay_out_port_names_.emplace_back();
  delay_values_.emplace_back();

  /* Buffer/Inverter-related parameters */
  buffer_types_.push_back(NUM_CIRCUIT_MODEL_BUF_TYPES);
  buffer_location_maps_.emplace_back();
  buffer_sizes_.push_back(-1);
  buffer_is_tapered_.push_back(false);
  buffer_num_levels_.push_back(-1);
  buffer_f_per_stage_.push_back(-1);

  /* Pass-gate-related parameters */
  pass_gate_logic_types_.push_back(NUM_CIRCUIT_MODEL_PASS_GATE_TYPES);
  pass_gate_logic_nmos_sizes_.push_back(-1);
  pass_gate_logic_pmos_sizes_.push_back(-1);

  /* Multiplexer-related parameters */
  mux_structure_.push_back(NUM_CIRCUIT_MODEL_STRUCTURE_TYPES);
  mux_num_levels_.push_back(-1);
  mux_add_const_input_.push_back(false);
  mux_const_input_values_.push_back(-1);
  mux_use_local_encoder_.push_back(false);
  mux_advanced_rram_design_.push_back(false);

  /* LUT-related parameters */
  lut_is_fracturable_.push_back(false);

  /* RRAM-related design technology information */
  rram_res_.emplace_back();
  wprog_set_.emplace_back();
  wprog_reset_.emplace_back();

  /* Wire parameters */
  wire_types_.push_back(NUM_WIRE_MODEL_TYPES);
  wire_rc_.emplace_back();
  wire_num_levels_.push_back(-1);

  /* Update circuit port fast look-up */
  circuit_model_port_lookup_.emplace_back();

  /* Invalidate fast look-up*/
  invalidate_circuit_model_lookup();

  return circuit_model_id;
}

/* Set the type of a Circuit Model */
void CircuitLibrary::set_circuit_model_type(const CircuitModelId& circuit_model_id, 
                                            const enum e_spice_model_type& type) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  circuit_model_types_[circuit_model_id] = type;
  return;
}

/* Set the name of a Circuit Model */
void CircuitLibrary::set_circuit_model_name(const CircuitModelId& circuit_model_id, const std::string& name) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  circuit_model_names_[circuit_model_id] = name;
  return;
}

/* Set the prefix of a Circuit Model */
void CircuitLibrary::set_circuit_model_prefix(const CircuitModelId& circuit_model_id, const std::string& prefix) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  circuit_model_prefix_[circuit_model_id] = prefix;
  return;
}

/* Set the verilog_netlist of a Circuit Model */
void CircuitLibrary::set_circuit_model_verilog_netlist(const CircuitModelId& circuit_model_id, const std::string& verilog_netlist) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  circuit_model_verilog_netlists_[circuit_model_id] = verilog_netlist;
  return;
}

/* Set the spice_netlist of a Circuit Model */
void CircuitLibrary::set_circuit_model_spice_netlist(const CircuitModelId& circuit_model_id, const std::string& spice_netlist) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  circuit_model_spice_netlists_[circuit_model_id] = spice_netlist;
  return;
}

/* Set the is_default of a Circuit Model */
void CircuitLibrary::set_circuit_model_is_default(const CircuitModelId& circuit_model_id, const bool& is_default) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  circuit_model_is_default_[circuit_model_id] = is_default;
  return;
}

/* Set the dump_structural_verilog of a Circuit Model */
void CircuitLibrary::set_circuit_model_dump_structural_verilog(const CircuitModelId& circuit_model_id, const bool& dump_structural_verilog) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  dump_structural_verilog_[circuit_model_id] = dump_structural_verilog;
  return;
}

/* Set the dump_explicit_port_map of a Circuit Model */
void CircuitLibrary::set_circuit_model_dump_explicit_port_map(const CircuitModelId& circuit_model_id, const bool& dump_explicit_port_map) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  dump_explicit_port_map_[circuit_model_id] = dump_explicit_port_map;
  return;
}

/* Set the type of design technology of a Circuit Model */
void CircuitLibrary::set_circuit_model_design_tech_type(const CircuitModelId& circuit_model_id, const enum e_spice_model_design_tech& design_tech_type) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  design_tech_types_[circuit_model_id] = design_tech_type;
  return;
}

/* Set the power-gated flag of a Circuit Model */
void CircuitLibrary::set_circuit_model_power_gated(const CircuitModelId& circuit_model_id, const bool& power_gated) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  power_gated_[circuit_model_id] = power_gated;
  return;
}

/* Set input buffer information for the circuit model */
void CircuitLibrary::set_circuit_model_input_buffer(const CircuitModelId& circuit_model_id, 
                                                    const bool& existence, const std::string& circuit_model_name) {
  /* Just call the base function and give the proper type */
  set_circuit_model_buffer(circuit_model_id, INPUT, existence, circuit_model_name);
  return;
}

/* Set output buffer information for the circuit model */
void CircuitLibrary::set_circuit_model_output_buffer(const CircuitModelId& circuit_model_id, 
                                                    const bool& existence, const std::string& circuit_model_name) {
  /* Just call the base function and give the proper type */
  set_circuit_model_buffer(circuit_model_id, OUTPUT, existence, circuit_model_name);
  return;
}

/* Set input buffer information for the circuit model, only applicable to LUTs! */
void CircuitLibrary::set_circuit_model_lut_input_buffer(const CircuitModelId& circuit_model_id, 
                                                        const bool& existence, const std::string& circuit_model_name) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Make sure the circuit model is a LUT! */
  VTR_ASSERT_SAFE(SPICE_MODEL_LUT == circuit_model_types_[circuit_model_id]);
  /* Just call the base function and give the proper type */
  set_circuit_model_buffer(circuit_model_id, LUT_INPUT_BUFFER, existence, circuit_model_name);
  return;
}

/* Set input inverter information for the circuit model, only applicable to LUTs! */
void CircuitLibrary::set_circuit_model_lut_input_inverter(const CircuitModelId& circuit_model_id, 
                                                          const bool& existence, const std::string& circuit_model_name) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Make sure the circuit model is a LUT! */
  VTR_ASSERT_SAFE(SPICE_MODEL_LUT == circuit_model_types_[circuit_model_id]);
  /* Just call the base function and give the proper type */
  set_circuit_model_buffer(circuit_model_id, LUT_INPUT_INVERTER, existence, circuit_model_name);
  return;
}

/* Set intermediate buffer information for the circuit model, only applicable to LUTs! */
void CircuitLibrary::set_circuit_model_lut_intermediate_buffer(const CircuitModelId& circuit_model_id, 
                                                               const bool& existence, const std::string& circuit_model_name) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Make sure the circuit model is a LUT! */
  VTR_ASSERT_SAFE(SPICE_MODEL_LUT == circuit_model_types_[circuit_model_id]);
  /* Just call the base function and give the proper type */
  set_circuit_model_buffer(circuit_model_id, LUT_INTER_BUFFER, existence, circuit_model_name);
  return;
}

/* Set pass-gate logic information of a circuit model */
void CircuitLibrary::set_circuit_model_pass_gate_logic(const CircuitModelId& circuit_model_id, const std::string& circuit_model_name) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  pass_gate_logic_circuit_model_names_[circuit_model_id] = circuit_model_name;
  return;
}

/* Add a port to a circuit model */
CircuitPortId CircuitLibrary::add_circuit_model_port(const CircuitModelId& circuit_model_id) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Create a port id */
  CircuitPortId circuit_port_id = CircuitPortId(port_ids_[circuit_model_id].size()); 
  /* Update the id list */
  port_ids_[circuit_model_id].push_back(circuit_port_id);
  
  /* Initialize other attributes */
  port_types_[circuit_model_id].push_back(NUM_CIRCUIT_MODEL_PORT_TYPES);
  port_sizes_[circuit_model_id].push_back(-1);
  port_prefix_[circuit_model_id].emplace_back();
  port_lib_names_[circuit_model_id].emplace_back();
  port_inv_prefix_[circuit_model_id].emplace_back();
  port_is_mode_select_[circuit_model_id].push_back(false);
  port_is_global_[circuit_model_id].push_back(false);
  port_is_reset_[circuit_model_id].push_back(false);
  port_is_set_[circuit_model_id].push_back(false);
  port_is_config_enable_[circuit_model_id].push_back(false);
  port_is_prog_[circuit_model_id].push_back(false);
  port_circuit_model_names_[circuit_model_id].emplace_back();
  port_circuit_model_ids_[circuit_model_id].push_back(CIRCUIT_MODEL_OPEN_ID);
  port_inv_circuit_model_names_[circuit_model_id].emplace_back();
  port_inv_circuit_model_ids_[circuit_model_id].push_back(CIRCUIT_MODEL_OPEN_ID);
  port_tri_state_maps_[circuit_model_id].emplace_back();
  port_lut_frac_level_[circuit_model_id].push_back(-1);
  port_lut_output_masks_[circuit_model_id].emplace_back();
  port_sram_orgz_[circuit_model_id].push_back(NUM_CIRCUIT_MODEL_SRAM_ORGZ_TYPES);

  return circuit_port_id;
}

/* Set the type for a port of a circuit model */
void CircuitLibrary::set_port_types(const CircuitModelId& circuit_model_id, 
                                    const CircuitPortId& circuit_port_id, 
                                    const enum e_spice_model_port_type& port_type) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_types_[circuit_model_id][circuit_port_id] = port_type;
  return;
}

/* Set the size for a port of a circuit model */
void CircuitLibrary::set_port_sizes(const CircuitModelId& circuit_model_id, 
                                    const CircuitPortId& circuit_port_id, 
                                    const size_t& port_size) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_sizes_[circuit_model_id][circuit_port_id] = port_size;
  return;
}

/* Set the prefix for a port of a circuit model */
void CircuitLibrary::set_port_prefix(const CircuitModelId& circuit_model_id, 
                                     const CircuitPortId& circuit_port_id, 
                                     const std::string& port_prefix) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_prefix_[circuit_model_id][circuit_port_id] = port_prefix;
  return;
}

/* Set the lib_name for a port of a circuit model */
void CircuitLibrary::set_port_lib_name(const CircuitModelId& circuit_model_id, 
                                       const CircuitPortId& circuit_port_id, 
                                       const std::string& lib_name) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_lib_names_[circuit_model_id][circuit_port_id] = lib_name;
  return;
}

/* Set the inv_prefix for a port of a circuit model */
void CircuitLibrary::set_port_inv_prefix(const CircuitModelId& circuit_model_id, 
                                         const CircuitPortId& circuit_port_id, 
                                         const std::string& inv_prefix) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_inv_prefix_[circuit_model_id][circuit_port_id] = inv_prefix;
  return;
}

/* Set the is_mode_select for a port of a circuit model */
void CircuitLibrary::set_port_is_mode_select(const CircuitModelId& circuit_model_id, 
                                             const CircuitPortId& circuit_port_id, 
                                             const bool& is_mode_select) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_is_mode_select_[circuit_model_id][circuit_port_id] = is_mode_select;
  return;
}

/* Set the is_global for a port of a circuit model */
void CircuitLibrary::set_port_is_global(const CircuitModelId& circuit_model_id, 
                                        const CircuitPortId& circuit_port_id, 
                                        const bool& is_global) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_is_global_[circuit_model_id][circuit_port_id] = is_global;
  return;
}

/* Set the is_reset for a port of a circuit model */
void CircuitLibrary::set_port_is_reset(const CircuitModelId& circuit_model_id, 
                                       const CircuitPortId& circuit_port_id, 
                                       const bool& is_reset) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_is_reset_[circuit_model_id][circuit_port_id] = is_reset;
  return;
}

/* Set the is_set for a port of a circuit model */
void CircuitLibrary::set_port_is_set(const CircuitModelId& circuit_model_id, 
                                     const CircuitPortId& circuit_port_id, 
                                     const bool& is_set) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_is_set_[circuit_model_id][circuit_port_id] = is_set;
  return;
}

/* Set the is_config_enable for a port of a circuit model */
void CircuitLibrary::set_port_is_config_enable(const CircuitModelId& circuit_model_id, 
                                               const CircuitPortId& circuit_port_id, 
                                               const bool& is_config_enable) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_is_config_enable_[circuit_model_id][circuit_port_id] = is_config_enable;
  return;
}

/* Set the is_prog for a port of a circuit model */
void CircuitLibrary::set_port_is_prog(const CircuitModelId& circuit_model_id, 
                                      const CircuitPortId& circuit_port_id, 
                                      const bool& is_prog) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_is_prog_[circuit_model_id][circuit_port_id] = is_prog;
  return;
}

/* Set the circuit_model_name for a port of a circuit model */
void CircuitLibrary::set_port_circuit_model_name(const CircuitModelId& circuit_model_id, 
                                                 const CircuitPortId& circuit_port_id, 
                                                 const std::string& circuit_model_name) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_circuit_model_names_[circuit_model_id][circuit_port_id] = circuit_model_name;
  return;
}

/* Set the circuit_model_id for a port of a circuit model */
void CircuitLibrary::set_port_circuit_model_id(const CircuitModelId& circuit_model_id, 
                                               const CircuitPortId& circuit_port_id, 
                                               const CircuitModelId& port_circuit_model_id) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_circuit_model_ids_[circuit_model_id][circuit_port_id] = port_circuit_model_id;
  return;
}

/* Set the inv_circuit_model_name for a port of a circuit model */
void CircuitLibrary::set_port_inv_circuit_model_name(const CircuitModelId& circuit_model_id, 
                                                 const CircuitPortId& circuit_port_id, 
                                                 const std::string& inv_circuit_model_name) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_inv_circuit_model_names_[circuit_model_id][circuit_port_id] = inv_circuit_model_name;
  return;
}

/* Set the inv_circuit_model_id for a port of a circuit model */
void CircuitLibrary::set_port_inv_circuit_model_id(const CircuitModelId& circuit_model_id, 
                                                 const CircuitPortId& circuit_port_id, 
                                                 const CircuitModelId& inv_circuit_model_id) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_inv_circuit_model_ids_[circuit_model_id][circuit_port_id] = inv_circuit_model_id;
  return;
}

/* Set the tri-state map for a port of a circuit model */
void CircuitLibrary::set_port_tri_state_map(const CircuitModelId& circuit_model_id, 
                                            const CircuitPortId& circuit_port_id, 
                                            const std::string& tri_state_map) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  port_tri_state_maps_[circuit_model_id][circuit_port_id] = tri_state_map;
  return;
}

/* Set the LUT fracturable level for a port of a circuit model, only applicable to LUTs */
void CircuitLibrary::set_port_lut_frac_level(const CircuitModelId& circuit_model_id, 
                                             const CircuitPortId& circuit_port_id, 
                                             const size_t& lut_frac_level) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  /* Make sure this is a LUT */
  VTR_ASSERT_SAFE(SPICE_MODEL_LUT == circuit_model_types_[circuit_model_id]);
  port_lut_frac_level_[circuit_model_id][circuit_port_id] = lut_frac_level;
  return;
}

/* Set the LUT fracturable level for a port of a circuit model, only applicable to LUTs */
void CircuitLibrary::set_port_lut_output_mask(const CircuitModelId& circuit_model_id, 
                                              const CircuitPortId& circuit_port_id, 
                                              const std::vector<size_t>& lut_output_masks) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  /* Make sure this is a LUT */
  VTR_ASSERT_SAFE(SPICE_MODEL_LUT == circuit_model_types_[circuit_model_id]);
  port_lut_output_masks_[circuit_model_id][circuit_port_id] = lut_output_masks;
  return;
}

/* Set the SRAM organization for a port of a circuit model, only applicable to SRAM ports */
void CircuitLibrary::set_port_sram_orgz(const CircuitModelId& circuit_model_id, 
                                        const CircuitPortId& circuit_port_id, 
                                        const enum e_sram_orgz& sram_orgz) {
  /* validate the circuit_port_id */
  VTR_ASSERT_SAFE(valid_circuit_port_id(circuit_model_id, circuit_port_id));
  /* Make sure this is a SRAM port */
  VTR_ASSERT_SAFE(SPICE_MODEL_PORT_SRAM == port_types_[circuit_model_id][circuit_port_id]);
  port_sram_orgz_[circuit_model_id][circuit_port_id] = sram_orgz;
  return;
}

/* Delay information */
/* Add a delay info:
 * Check if the delay type is in the range of vector
 * if yes, assign values 
 * if no, resize and assign values
 */
void CircuitLibrary::add_delay_info(const CircuitModelId& circuit_model_id,
                                    const enum spice_model_delay_type& delay_type) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Check the range of vector */
  if (size_t(delay_type) >= delay_types_[circuit_model_id].size()) {
    /* Resize */
    delay_types_[circuit_model_id].resize(size_t(delay_type) + 1); 
    delay_in_port_names_[circuit_model_id].resize(size_t(delay_type) + 1); 
    delay_out_port_names_[circuit_model_id].resize(size_t(delay_type) + 1); 
    delay_values_[circuit_model_id].resize(size_t(delay_type) + 1); 
  }
  delay_types_[circuit_model_id][size_t(delay_type)] = delay_type; 
  return;
}

void CircuitLibrary::set_delay_in_port_names(const CircuitModelId& circuit_model_id,
                                             const enum spice_model_delay_type& delay_type,
                                             const std::string& in_port_names) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Validate delay_type */
  VTR_ASSERT_SAFE(valid_delay_type(circuit_model_id, delay_type));
  delay_in_port_names_[circuit_model_id][size_t(delay_type)] = in_port_names; 
  return; 
}

void CircuitLibrary::set_delay_out_port_names(const CircuitModelId& circuit_model_id,
                                             const enum spice_model_delay_type& delay_type,
                                             const std::string& out_port_names) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Validate delay_type */
  VTR_ASSERT_SAFE(valid_delay_type(circuit_model_id, delay_type));
  delay_out_port_names_[circuit_model_id][size_t(delay_type)] = out_port_names; 
  return; 
}

void CircuitLibrary::set_delay_values(const CircuitModelId& circuit_model_id,
                                     const enum spice_model_delay_type& delay_type,
                                     const std::string& delay_values) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Validate delay_type */
  VTR_ASSERT_SAFE(valid_delay_type(circuit_model_id, delay_type));
  delay_values_[circuit_model_id][size_t(delay_type)] = delay_values; 
  return; 
}

/* Buffer/Inverter-related parameters */

/************************************************************************
 * Internal Mutators 
 ***********************************************************************/
/* Set the information for a buffer 
 * For a buffer type, we check if it is in the range of vector
 * If yes, just assign values
 * If no, resize the vector and then assign values 
 */
void CircuitLibrary::set_circuit_model_buffer(const CircuitModelId& circuit_model_id, const enum e_buffer_type buffer_type, 
                                              const bool& existence, const std::string& circuit_model_name) {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* Check the range of vector */
  if (size_t(buffer_type) >= buffer_existence_[circuit_model_id].size()) {
    /* Resize and assign values */
    buffer_existence_[circuit_model_id].resize(size_t(buffer_type) + 1); 
    buffer_circuit_model_names_[circuit_model_id].resize(size_t(buffer_type) + 1); 
    buffer_circuit_model_ids_[circuit_model_id].resize(size_t(buffer_type) + 1); 
  }
  /* Now we are in the range, assign values */
  buffer_existence_[circuit_model_id][size_t(buffer_type)] = existence;
  buffer_circuit_model_names_[circuit_model_id][size_t(buffer_type)] = circuit_model_name;
  buffer_circuit_model_ids_[circuit_model_id][size_t(buffer_type)] = CIRCUIT_MODEL_OPEN_ID; /* Set an OPEN id here, which will be linked later */
  return;
}

/* Link the inv_circuit_model_id for each port of a circuit model.
 * We search the inv_circuit_model_name in the CircuitLibrary and 
 * configure the port inv_circuit_model_id
 */
void CircuitLibrary::set_circuit_model_port_inv_circuit_model(const CircuitModelId& circuit_model_id) { 
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  /* TODO: complete this function when port mutators are finished */
  return;
}      


/************************************************************************
 * Internal mutators: build fast look-ups 
 ***********************************************************************/
void CircuitLibrary::build_circuit_model_lookup() {
  /* invalidate fast look-up */
  invalidate_circuit_model_lookup();
  /* Classify circuit models by type */
  circuit_model_lookup_.resize(NUM_CIRCUIT_MODEL_TYPES);
  /* Walk through circuit_models and categorize */
  for (auto& id : circuit_model_ids_) {
    circuit_model_lookup_[circuit_model_types_[id]].push_back(id);
  }
  /* Make the default circuit_model to be the first element for each type */
  for (auto& type : circuit_model_lookup_) {
    /* if the first element is already a default model, we skip this  */
    if (true == circuit_model_is_default_[type[0]]) {
      continue;
    }
    /* Check the array, and try to find a default model */
    for (size_t id = 0; id < type.size(); ++id) {
      if (false == circuit_model_is_default_[type[id]]) {
        continue;
      }
      /* Once we find a default model, swap with the first element and finish the loop */
      std::swap(type[0], type[id]);
      break;
    }
  }
  return;
}

/************************************************************************
 * Internal invalidators/validators 
 ***********************************************************************/
/* Validators */
bool CircuitLibrary::valid_circuit_model_id(const CircuitModelId& circuit_model_id) const {
  return ( size_t(circuit_model_id) < circuit_model_ids_.size() ) && ( circuit_model_id == circuit_model_ids_[circuit_model_id] ); 
}

bool CircuitLibrary::valid_circuit_port_id(const CircuitModelId& circuit_model_id, const CircuitPortId& circuit_port_id) const {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  return ( size_t(circuit_port_id) < port_ids_[circuit_model_id].size() ) && ( circuit_port_id == port_ids_[circuit_model_id][circuit_port_id] ); 
}

bool CircuitLibrary::valid_delay_type(const CircuitModelId& circuit_model_id, const enum spice_model_delay_type& delay_type) const { 
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  return ( size_t(delay_type) < delay_types_[circuit_model_id].size() ) && ( delay_type == delay_types_[circuit_model_id][size_t(delay_type)] ); 
}

/* Invalidators */
/* Empty fast lookup for circuit_models*/
void CircuitLibrary::invalidate_circuit_model_lookup() const {
  circuit_model_lookup_.clear();
  return;
}

/* Empty fast lookup for circuit ports for a circuit_model */
void CircuitLibrary::invalidate_circuit_model_port_lookup(const CircuitModelId& circuit_model_id) const {
  /* validate the circuit_model_id */
  VTR_ASSERT_SAFE(valid_circuit_model_id(circuit_model_id));
  circuit_model_port_lookup_[size_t(circuit_model_id)].clear();
  return;
}

/************************************************************************
 * End of file : circuit_library.cpp 
 ***********************************************************************/
