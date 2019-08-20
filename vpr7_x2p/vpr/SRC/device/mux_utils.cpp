/**************************************************
 * This file includes a series of most utilized functions 
 * that are used to implement a multiplexer 
 *************************************************/
#include <cmath>

#include "spice_types.h"
#include "util.h"
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
  VTR_ASSERT ((SPICE_MODEL_MUX == circuit_lib.model_type(circuit_model)) 
           || (SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model)) );

  if (SPICE_MODEL_LUT ==  circuit_lib.model_type(circuit_model)) {
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
  /* Ensure the mux size is valid ! */
  VTR_ASSERT(valid_mux_implementation_num_inputs(mux_size));

  /* Branch on the mux sizes */  
  if (2 == mux_size) {
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

/**************************************************
 * Find the number of levels for a tree-like multiplexer implementation
 *************************************************/
size_t find_treelike_mux_num_levels(const size_t& mux_size) {
  /* Do log2(mux_size), have a basic number */ 
  size_t level = (size_t)(log((double)mux_size)/log(2.));
  /* Fix the error, i.e. mux_size=5, level = 2, we have to complete */
  while (mux_size > pow(2.,(double)level)) {
    level++;
  }

  return level;
}

/**************************************************
 * Find the number of inputs for majority of branches 
 * in a multi-level multiplexer implementation
 *************************************************/
size_t find_multilevel_mux_branch_num_inputs(const size_t& mux_size,
                                             const size_t& mux_level) {
  /* Special Case: mux_size = 2 */
  if (2 == mux_size) {
    return mux_size; 
  }
  
  if (1 == mux_level) {
    return mux_size;
  }  

  if (2 == mux_level) {
    size_t num_input_per_unit = (size_t)sqrt(mux_size);
    while ( num_input_per_unit * num_input_per_unit < mux_size) {
      num_input_per_unit++;
    }
    return num_input_per_unit;
  }

  VTR_ASSERT_SAFE(2 < mux_level);

  size_t num_input_per_unit = 2;
  while (pow((double)num_input_per_unit, (double)mux_level) < mux_size) {
    num_input_per_unit++;
  }

  if (!valid_mux_implementation_num_inputs(num_input_per_unit)) {
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s,[LINE%d]) Number of inputs of each basis should be at least 2!\n",
               __FILE__, __LINE__);
    exit(1); 
  }
  
  return num_input_per_unit;
}

/**************************************************
 * Convert a linked list of MUX architecture to MuxLibrary
 * TODO: this function will be deleted when MUXLibrary fully
 * replace legacy data structures
 *************************************************/
MuxLibrary convert_mux_arch_to_library(const CircuitLibrary& circuit_lib, t_llist* muxes_head) {
  t_llist* temp = muxes_head;
  MuxLibrary mux_lib;

  /* Walk through the linked list */
  while(temp) {
    VTR_ASSERT_SAFE(NULL != temp->dptr);
    t_spice_mux_model* cur_spice_mux_model = (t_spice_mux_model*)(temp->dptr);

    /* Bypass the spice models who has a user-defined subckt */
    if (NULL != cur_spice_mux_model->spice_model->verilog_netlist) {
      /* Move on to the next*/
      temp = temp->next;
      continue;
    }

    /* Build a MUX graph for the model */
    /* Find the circuit model id by the name */
    CircuitModelId circuit_model = circuit_lib.model(cur_spice_mux_model->spice_model->name);
    mux_lib.add_mux(circuit_lib, circuit_model, cur_spice_mux_model->size);

    /* Move on to the next*/
    temp = temp->next;
  }

  return mux_lib;
}
