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

#include "vtr_assert.h"

#include "circuit_library.h"

/************************************************************************
 * Member functions for class CircuitLibrary
 ***********************************************************************/

/************************************************************************
 * Constructors
 ***********************************************************************/

/************************************************************************
 * Accessors : aggregates
 ***********************************************************************/
CircuitLibrary::circuit_model_range CircuitLibrary::circuit_models() const {
  return vtr::make_range(circuit_model_ids_.begin(), circuit_model_ids_.end());
}


/************************************************************************
 * Accessors : Methods to find circuit model 
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
 * Mutators 
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
  design_tech_.push_back(NUM_CIRCUIT_MODEL_DESIGN_TECH_TYPES);
  power_gated_.push_back(false);
  
  /* Buffer existence */
  buffer_existence_.emplace_back();
  buffer_circuit_model_names_.emplace_back();
  buffer_circuit_model_ids_.emplace_back();

    /* Pass-gate-related parameters */
  pass_gate_logic_circuit_model_names_.emplace_back();
  pass_gate_logic_circuit_model_ids_.emplace_back();

  /* Port information */
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

  /* Invalidate fast look-up*/


  return circuit_model_id;
}

/************************************************************************
 * Internal Mutators 
 ***********************************************************************/
/* Link the inv_circuit_model_id for each port of a circuit model.
 * We search the inv_circuit_model_name in the CircuitLibrary and 
 * configure the port inv_circuit_model_id
 */
void CircuitLibrary::set_circuit_model_port_inv_circuit_model(const CircuitModelId& circuit_model_id) { 
  return;
}      

/************************************************************************
 * End of file : circuit_library.cpp 
 ***********************************************************************/
