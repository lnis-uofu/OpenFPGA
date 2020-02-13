/**************************************************
 * This file includes a series of most utilized functions 
 * that are used to implement a multiplexer 
 *************************************************/
#include <cmath>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"

/* Headers from readarchopenfpga library */
#include "circuit_types.h"

#include "decoder_library_utils.h"
#include "mux_utils.h"

/* Begin namespace openfpga */
namespace openfpga {

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
  VTR_ASSERT ((CIRCUIT_MODEL_MUX == circuit_lib.model_type(circuit_model)) 
           || (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) );

  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
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
  VTR_ASSERT ((CIRCUIT_MODEL_MUX == circuit_lib.model_type(circuit_model)) 
           || (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) );

  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
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
enum e_circuit_model_structure find_mux_implementation_structure(const CircuitLibrary& circuit_lib,
					                                           const CircuitModelId& circuit_model,
						                                       const size_t& mux_size) {
  /* Ensure the mux size is valid ! */
  VTR_ASSERT(valid_mux_implementation_num_inputs(mux_size));

  /* Branch on the mux sizes */  
  if (2 == mux_size) {
    /* Tree-like is the best structure of CMOS MUX2 */
    if (CIRCUIT_MODEL_DESIGN_CMOS == circuit_lib.design_tech_type(circuit_model)) {
      return CIRCUIT_MODEL_STRUCTURE_TREE;
    }
    VTR_ASSERT_SAFE(CIRCUIT_MODEL_DESIGN_RRAM == circuit_lib.design_tech_type(circuit_model));
    /* One-level is the best structure of RRAM MUX2 */
    return CIRCUIT_MODEL_STRUCTURE_ONELEVEL;
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
    VTR_LOG_ERROR("Number of inputs of each basis should be at least 2!\n");
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
  if (CIRCUIT_MODEL_LUT != circuit_lib.model_type(circuit_model)) {
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
 * Find the number of reserved configuration bits for a multiplexer
 * The reserved configuration bits is only used by ReRAM-based multiplexers
 * It is actually the shared BL/WLs among ReRAMs
 *************************************************/
size_t find_mux_num_reserved_config_bits(const CircuitLibrary& circuit_lib,
                                         const CircuitModelId& mux_model,
                                         const MuxGraph& mux_graph) {
  if (CIRCUIT_MODEL_DESIGN_RRAM != circuit_lib.design_tech_type(mux_model)) {
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
 * Find the number of configuration bits for a CMOS multiplexer
 * In general, the number of configuration bits is 
 * the number of memory bits for a mux_graph
 * However, when local decoders are used, 
 * the number of configuration bits are reduced to log2(X)
 *************************************************/
static 
size_t find_cmos_mux_num_config_bits(const CircuitLibrary& circuit_lib,
                                     const CircuitModelId& mux_model,
                                     const MuxGraph& mux_graph,
                                     const e_config_protocol_type& sram_orgz_type) {
  size_t num_config_bits = 0; 

  switch (sram_orgz_type) {
  case CONFIG_MEM_MEMORY_BANK:
  case CONFIG_MEM_SCAN_CHAIN:
  case CONFIG_MEM_STANDALONE:
    num_config_bits = mux_graph.num_memory_bits();
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n"); 
    exit(1);
  }

  if (false == circuit_lib.mux_use_local_encoder(mux_model)) {
    return num_config_bits;
  }

  num_config_bits = 0;
  /* Multiplexer local encoders are applied to memory bits at each stage */
  for (const auto& lvl : mux_graph.levels()) {
    num_config_bits += find_mux_local_decoder_addr_size(mux_graph.num_memory_bits_at_level(lvl));
  } 

  return num_config_bits;
}

/**************************************************
 * Find the number of configuration bits for a RRAM multiplexer
 * In general, the number of configuration bits is 
 * the number of levels for a mux_graph
 * This is due to only the last BL/WL of the multiplexer is
 * independent from each other
 * However, when local decoders are used, 
 * the number of configuration bits should be consider all the
 * shared(reserved) configuration bits and independent bits 
 *************************************************/
static 
size_t find_rram_mux_num_config_bits(const CircuitLibrary& circuit_lib,
                                     const CircuitModelId& mux_model,
                                     const MuxGraph& mux_graph, 
                                     const e_config_protocol_type& sram_orgz_type) {
  size_t num_config_bits = 0; 
  switch (sram_orgz_type) {
  case CONFIG_MEM_MEMORY_BANK:
    /* In memory bank, by intensively share the Bit/Word Lines,
     * we only need 1 additional BL and WL for each MUX level.
     */
    num_config_bits = mux_graph.num_levels();
    break;
  case CONFIG_MEM_SCAN_CHAIN:
  case CONFIG_MEM_STANDALONE:
    /* Currently we DO NOT SUPPORT THESE, given an invalid number */
    num_config_bits = size_t(-1);
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n"); 
    exit(1);
  }

  if (true == circuit_lib.mux_use_local_encoder(mux_model)) {
    /* TODO: this is a to-do work for ReRAM-based multiplexers and FPGAs 
     * The number of states of a local decoder only depends on how many 
     * memory bits that the multiplexer will have
     * This may NOT be correct!!! 
     */
    return find_mux_local_decoder_addr_size(mux_graph.num_memory_bits());
  }

  return num_config_bits;
}

/**************************************************
 * Find the number of configuration bits for 
 * a routing multiplexer
 * Two cases are considered here.
 * They are placed in different branches (sub-functions)
 * in order to be easy in extending to new technology!
 *************************************************/
size_t find_mux_num_config_bits(const CircuitLibrary& circuit_lib,
                                const CircuitModelId& mux_model,
                                const MuxGraph& mux_graph, 
                                const e_config_protocol_type& sram_orgz_type) {
  size_t num_config_bits = size_t(-1);

  switch (circuit_lib.design_tech_type(mux_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    num_config_bits = find_cmos_mux_num_config_bits(circuit_lib, mux_model, mux_graph, sram_orgz_type);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    num_config_bits = find_rram_mux_num_config_bits(circuit_lib, mux_model, mux_graph, sram_orgz_type);
    break;
  default:
    VTR_LOG_ERROR("Invalid design_technology of MUX(name: %s)\n",
                  circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }

  return num_config_bits;
}

/**************************************************
 * Find the number of shared configuration bits for a CMOS multiplexer
 * Currently, all the supported CMOS multiplexers
 * do NOT require any shared configuration bits 
 *************************************************/
static 
size_t find_cmos_mux_num_shared_config_bits(const e_config_protocol_type& sram_orgz_type) {
  size_t num_shared_config_bits = 0; 

  switch (sram_orgz_type) {
  case CONFIG_MEM_MEMORY_BANK:
  case CONFIG_MEM_SCAN_CHAIN:
  case CONFIG_MEM_STANDALONE:
    num_shared_config_bits = 0;
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n"); 
    exit(1);
  }

  return num_shared_config_bits;
}

/**************************************************
 * Find the number of shared configuration bits for a ReRAM multiplexer
 *************************************************/
static 
size_t find_rram_mux_num_shared_config_bits(const CircuitLibrary& circuit_lib,
                                            const CircuitModelId& mux_model,
                                            const MuxGraph& mux_graph, 
                                            const e_config_protocol_type& sram_orgz_type) {
  size_t num_shared_config_bits = 0; 
  switch (sram_orgz_type) {
  case CONFIG_MEM_MEMORY_BANK: {
    /* In memory bank, the number of shared configuration bits is
     * the sum of largest branch size at each level 
     */
    for (auto lvl : mux_graph.node_levels()) {
      /* Find the maximum branch size: 
       * Note that branch_sizes() returns a sorted vector
       * The last one is the maximum
       */
      num_shared_config_bits += mux_graph.branch_sizes(lvl).back();
    }
    break;
  }
  case CONFIG_MEM_SCAN_CHAIN:
  case CONFIG_MEM_STANDALONE:
    /* Currently we DO NOT SUPPORT THESE, given an invalid number */
    num_shared_config_bits = size_t(-1);
    break;
  default:
    VTR_LOG_ERROR("Invalid type of SRAM organization!\n"); 
    exit(1);
  }

  if (true == circuit_lib.mux_use_local_encoder(mux_model)) {
    /* TODO: this is a to-do work for ReRAM-based multiplexers and FPGAs 
     * The number of states of a local decoder only depends on how many 
     * memory bits that the multiplexer will have
     * This may NOT be correct!!! 
     * If local encoders are introduced, zero shared configuration bits are required
     */
    return 0;
  }

  return num_shared_config_bits;
}

/**************************************************
 * Find the number of shared configuration bits for 
 * a routing multiplexer
 * Two cases are considered here.
 * They are placed in different branches (sub-functions)
 * in order to be easy in extending to new technology!
 *
 * Note: currently, shared configuration bits are demanded
 * by ReRAM-based multiplexers only
 *************************************************/
size_t find_mux_num_shared_config_bits(const CircuitLibrary& circuit_lib,
                                       const CircuitModelId& mux_model,
                                       const MuxGraph& mux_graph, 
                                       const e_config_protocol_type& sram_orgz_type) {
  size_t num_shared_config_bits = size_t(-1);

  switch (circuit_lib.design_tech_type(mux_model)) {
  case CIRCUIT_MODEL_DESIGN_CMOS:
    num_shared_config_bits = find_cmos_mux_num_shared_config_bits(sram_orgz_type);
    break;
  case CIRCUIT_MODEL_DESIGN_RRAM:
    num_shared_config_bits = find_rram_mux_num_shared_config_bits(circuit_lib, mux_model, mux_graph, sram_orgz_type);
    break;
  default:
    VTR_LOG_ERROR("Invalid design_technology of MUX(name: %s)\n",
                  circuit_lib.model_name(mux_model).c_str()); 
    exit(1);
  }

  return num_shared_config_bits;
}

} /* End namespace openfpga*/
