/**************************************************
 * This file includes a series of most utilized functions 
 * that are used to implement a multiplexer 
 *************************************************/
#include <cmath>
#include <algorithm>

#include "spice_types.h"
#include "util.h"
#include "vtr_assert.h"
#include "decoder_library_utils.h"
#include "mux_utils.h"

/* Validate the number of inputs for a multiplexer implementation,
 * the minimum supported size is 2
 * otherwise, there is no need for a MUX  
 */
bool valid_mux_implementation_num_inputs(const size_t& mux_size) {
  return (2 <= mux_size);
}

/**************************************************
 * Find the actual number of datapath inputs for a multiplexer implementation
 * 1. if there are no requirements on constant inputs, mux_size is the actual one
 * 2. if there exist constant inputs, mux_size should minus 1
 * This function is mainly used to recover the number of datapath inputs
 * for MUXGraphs which is a generic representation without labelling datapath inputs
 *************************************************/
size_t find_mux_num_datapath_inputs(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& circuit_model,
                                    const size_t& mux_size) {
  /* Should be either MUX or LUT
   * LUTs do have an tree-like MUX, but there is no need for a constant input! 
   */
  VTR_ASSERT ((SPICE_MODEL_MUX == circuit_lib.model_type(circuit_model)) 
           || (SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model)) );

  if (SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
    return mux_size;
  }

  if (true == circuit_lib.mux_add_const_input(circuit_model)) {
    return mux_size - 1;
  }
  return mux_size;
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

  if (SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
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
 * Build a location map for intermediate buffers
 * that may appear at the multiplexing structure of a LUT
 * Here is a tricky thing: 
 * By default, the first and last stage should not exist any intermediate buffers
 * For example: 
 * There are 5 stages in a 4-stage multiplexer is available for buffering
 * but only 3 stages [1,2,3] are intermedate buffers 
 * and these are users' specification
 *  
 *          +-------+          +-------+           +-------+           +-------+
 * location | stage | location | stage |  location | stage |  location | stage | location 
 *    [0]   |  [0]  |   [1]    |  [1]  |    [2]    |  [2]  |    [3]    |  [3]  |    [5]
 *          +-------+          +-------+           +-------+           +-------+
 *
 * We will check if the length of location map matches the number of
 * multiplexer levels. And then complete a location map
 * for the given multiplexers
 *************************************************/
std::vector<bool> build_mux_intermediate_buffer_location_map(const CircuitLibrary& circuit_lib, 
                                                             const CircuitModelId& circuit_model,
                                                             const size_t& num_mux_levels) {
  /* Deposite a default location map */
  std::vector<bool> location_map(num_mux_levels, false); 
  std::string location_map_str;

  /* ONLY for LUTs: intermediate buffers may exist if specified */
  if (SPICE_MODEL_LUT != circuit_lib.model_type(circuit_model)) {
    return location_map;
  }

  /* Get location map when the flag of intermediate buffer is on */
  if (true == circuit_lib.is_lut_intermediate_buffered(circuit_model)) { 
    location_map_str = circuit_lib.lut_intermediate_buffer_location_map(circuit_model);
  }

  /* If no location map is specified, we can return here */
  if (location_map_str.empty()) {
    return location_map;
  }

  /* Check if the user-defined location map matches the number of mux levels*/
  VTR_ASSERT(num_mux_levels - 2 == location_map_str.length());

  /* Apply the location_map string to the intermediate stages of multiplexers */
  for (size_t i = 0; i < location_map_str.length(); ++i) {
    /* '1' indicates that an intermediate buffer is needed at the location */
    if ('1' == location_map_str[i]) {
      location_map[i + 1] = true;
    }
  }
  
  return location_map;
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

/**************************************************
 * Find the number of reserved configuration bits for a multiplexer
 * The reserved configuration bits is only used by ReRAM-based multiplexers
 * It is actually the shared BL/WLs among ReRAMs
 *************************************************/
size_t find_mux_num_reserved_config_bits(const CircuitLibrary& circuit_lib,
                                         const CircuitModelId& mux_model,
                                         const MuxGraph& mux_graph) {
  if (SPICE_MODEL_DESIGN_RRAM != circuit_lib.design_tech_type(mux_model)) {
    return 0;
  }

  std::vector<size_t> mux_branch_sizes = mux_graph.branch_sizes(); 
  /* For tree-like multiplexers: they have two shared configuration bits */
  if ( (1 == mux_branch_sizes.size()) 
    && (2 == mux_branch_sizes[0]) ) {
    return mux_branch_sizes[0];
  }
  /* One-level multiplexer */
  if ( 1 == mux_graph.num_levels() ) {
    return mux_graph.num_inputs();
  }
  /* Multi-level multiplexers: TODO: This should be better tested and clarified 
   * Now the multi-level multiplexers are treated as cascaded one-level multiplexers 
   * Use the maximum branch sizes and multiply it by the number of levels 
   */
  std::vector<size_t>::iterator max_mux_branch_size = std::max_element(mux_branch_sizes.begin(), mux_branch_sizes.end());
  return mux_graph.num_levels() * (*max_mux_branch_size);
}

/**************************************************
 * Find the number of configuration bits for a multiplexer
 * In general, the number of configuration bits is 
 * the number of memory bits for a mux_graph
 * However, when local decoders are used, this should be changed!
 *************************************************/
size_t find_mux_num_config_bits(const CircuitLibrary& circuit_lib,
                                const CircuitModelId& mux_model,
                                const MuxGraph& mux_graph) {
  if (true == circuit_lib.mux_use_local_encoder(mux_model)) {
    return find_mux_local_decoder_addr_size(mux_graph.num_memory_bits());
  }

  return mux_graph.num_memory_bits();
}

