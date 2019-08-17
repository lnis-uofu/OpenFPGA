/**************************************************
 * This file includes member functions for the 
 * data structures in mux_graph.h
 *************************************************/
#include "util.h"
#include "vtr_assert.h"
#include "mux_utils.h"
#include "mux_graph.h"

/**************************************************
 * Member functions for the class MuxGraph
 *************************************************/

/**************************************************
 * Constructor 
 *************************************************/

/* Create an object based on a Circuit Model which is MUX */
MuxGraph::MuxGraph(const CircuitLibrary& circuit_lib, 
                   const CircuitModelId& circuit_model,
                   const size_t& mux_size) {
  /* Build the graph for a given multiplexer model */
  build_mux_graph(circuit_lib, circuit_model, mux_size);
} 

/**************************************************
 * Private mutators 
 *************************************************/

/* Build the graph for a given multiplexer model */
void MuxGraph::build_mux_graph(const CircuitLibrary& circuit_lib, 
                               const CircuitModelId& circuit_model,
                               const size_t& mux_size) {
  /* Make sure this model is a MUX */
  VTR_ASSERT(SPICE_MODEL_MUX == circuit_lib.circuit_model_type(circuit_model));

  /* Make sure mux_size is valid */
  VTR_ASSERT(valid_mux_implementation_num_inputs(mux_size));

  /* Depends on the mux size, the actual multiplexer structure may change! */

  /* Branch on multiplexer structures, leading to different building strategies */
  switch (circuit_lib.mux_structure(circuit_model)) {
  case SPICE_MODEL_STRUCTURE_ONELEVEL:
    break;
  case SPICE_MODEL_STRUCTURE_MULTILEVEL:
    break;
  case SPICE_MODEL_STRUCTURE_TREE:
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d]) Invalid multiplexer structure for circuit model (name=%s)!\n",
              __FILE__, __LINE__, circuit_lib.circuit_model_name(circuit_model));
    exit(1);
  }
}
 
/**************************************************
 * Private validartors
 *************************************************/

/* valid ids */
bool MuxGraph::valid_node_id(const size_t& node_id) const {
  return (node_id < node_ids_.size());
}


/**************************************************
 * End of Member functions for the class MuxGraph
 *************************************************/

/**************************************************
 * Member functions for the class MuxLibrary
 *************************************************/

/**************************************************
 * End of Member functions for the class MuxLibrary
 *************************************************/

