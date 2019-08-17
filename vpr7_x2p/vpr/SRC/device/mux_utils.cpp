/**************************************************
 * This file includes a series of most utilized functions 
 * that are used to implement a multiplexer 
 *************************************************/

#include "vtr_assert.h"
#include "mux_utils.h"

/* Validate the number of inputs for a multiplexer implementation,
 * the minimum supported size is 2
 * otherwise, there is no need for a MUX  
 */
bool valid_mux_implementation_num_inputs(const size_t& mux_size) {
  return (2 <= mux_size);
}

/**************************************************
 * Find the actual number of inputs for a multiplexer implementation
 * 1. if there are no requirements on constant inputs, mux_size is the actual one
 * 2. if there exist constant inputs, mux_size should plus 1
 *************************************************/
size_t find_mux_implementation_num_inputs(const CircuitLibrary& circuit_lib,
                                          const CircuitModelId& circuit_model,
                                          const size_t& mux_size) {
  /* Should be either MUX or LUT
   * LUTs do have an tree-like MUX, but there is no need for a constant input! 
   */
  VTR_ASSERT ((SPICE_MODEL_MUX == circuit_lib.circuit_model_type(circuit_model)) 
           || (SPICE_MODEL_LUT == circuit_lib.circuit_model_type(circuit_model)) );

  if (SPICE_MODEL_LUT ==  circuit_lib.circuit_model_type(circuit_model)) {
    return mux_size;
  }

  if (true == circuit_lib.mux_add_const_input(circuit_model)) {
    return mux_size + 1;
  }
  return mux_size;
}

/**************************************************
 * Find the structure for a multiplexer implementation
 * 1. In most cases, the structure should follow the 
 * mux_structure defined by users in the CircuitLibrary
 * 2. However, a special case may apply when mux_size is 2 
 * In such case, we will force a TREE structure
 * regardless of users' specification as this is the 
 * most efficient structure  
 *************************************************/
enum e_spice_model_structure find_mux_implementation_structure(const CircuitLibrary& circuit_lib,
					                                           const CircuitModelId& circuit_model,
						                                       const size_t& mux_size) {
  /* Get the number of inputs */
  size_t impl_mux_size = find_mux_implementation_num_inputs(circuit_lib, circuit_model, mux_size);
  /* Ensure the mux size is valid ! */
  VTR_ASSERT(valid_mux_implementation_num_inputs(impl_mux_size));

  /* Branch on the mux sizes */  
  if (2 == impl_mux_size) {
    /* Tree-like is the best structure of CMOS MUX2 */
    if (SPICE_MODEL_DESIGN_CMOS == circuit_lib.design_tech_type(circuit_model)) {
      return SPICE_MODEL_STRUCTURE_TREE;
    }
    VTR_ASSERT_SAFE(SPICE_MODEL_DESIGN_RRAM == circuit_lib.design_tech_type(circuit_model));
    /* One-level is the best structure of RRAM MUX2 */
    return SPICE_MODEL_STRUCTURE_ONELEVEL;
  }

  return circuit_lib.mux_structure(circuit_model);
}



