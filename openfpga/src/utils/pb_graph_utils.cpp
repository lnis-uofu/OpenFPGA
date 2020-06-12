/********************************************************************
 * This file includes most utilized functions for the pb_graph_node
 * and pb_graph_pin data structure in the OpenFPGA context
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "pb_graph_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to find out all the pb_graph_pins that drive 
 * a given pb_graph pin w.r.t. a given interconnect definition
 *******************************************************************/
std::vector<t_pb_graph_pin*> pb_graph_pin_inputs(t_pb_graph_pin* pb_graph_pin,
                                                 t_interconnect* selected_interconnect) { 
  std::vector<t_pb_graph_pin*> inputs;

  /* Search the input edges only, stats on the size of MUX we may need (fan-in) */
  for (int iedge = 0; iedge < pb_graph_pin->num_input_edges; ++iedge) {
    /* We care the only edges in the selected mode */
    if (selected_interconnect != pb_graph_pin->input_edges[iedge]->interconnect) {
      continue;
    }
    for (int ipin = 0; ipin < pb_graph_pin->input_edges[iedge]->num_input_pins; ++ipin) {
      /* Ensure that the pin is unique in the list */
      if (inputs.end() != std::find(inputs.begin(), inputs.end(), pb_graph_pin->input_edges[iedge]->input_pins[ipin])) {
        continue;
      }
      /* Unique pin, push to the vector */ 
      inputs.push_back(pb_graph_pin->input_edges[iedge]->input_pins[ipin]);
    }
  }

  return inputs;
}

/********************************************************************
 * This function aims to find out the interconnect that drives
 * a given pb_graph pin when operating in a select mode
 *******************************************************************/
t_interconnect* pb_graph_pin_interc(t_pb_graph_pin* pb_graph_pin, 
                                    t_mode* selected_mode) {
  t_interconnect* interc = nullptr;

  /* Search the input edges only, stats on the size of MUX we may need (fan-in) */
  for (int iedge = 0; iedge < pb_graph_pin->num_input_edges; ++iedge) {
    /* We care the only edges in the selected mode */
    if (selected_mode != pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
      continue;
    }
    /* There should be one unique interconnect to be found! */
    if (nullptr != interc) {
      VTR_ASSERT(interc == pb_graph_pin->input_edges[iedge]->interconnect);
    } else {
      interc = pb_graph_pin->input_edges[iedge]->interconnect;
    }
  }
  
  return interc; 
} 

} /* end namespace openfpga */
