/********************************************************************
 * This file includes most utilized functions for the pb_graph_node
 * and pb_graph_pin data structure in the OpenFPGA context
 *******************************************************************/
/* Headers from vtrutil library */
#include "pb_graph_utils.h"

#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to find out all the pb_graph_pins that drive
 * a given pb_graph pin w.r.t. a given interconnect definition
 *******************************************************************/
std::vector<t_pb_graph_pin*> pb_graph_pin_inputs(
  t_pb_graph_pin* pb_graph_pin, t_interconnect* selected_interconnect) {
  std::vector<t_pb_graph_pin*> inputs;

  /* Search the input edges only, stats on the size of MUX we may need (fan-in)
   */
  for (int iedge = 0; iedge < pb_graph_pin->num_input_edges; ++iedge) {
    /* We care the only edges in the selected mode */
    if (selected_interconnect !=
        pb_graph_pin->input_edges[iedge]->interconnect) {
      continue;
    }
    for (int ipin = 0; ipin < pb_graph_pin->input_edges[iedge]->num_input_pins;
         ++ipin) {
      /* Ensure that the pin is unique in the list */
      if (inputs.end() !=
          std::find(inputs.begin(), inputs.end(),
                    pb_graph_pin->input_edges[iedge]->input_pins[ipin])) {
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

  /* Search the input edges only, stats on the size of MUX we may need (fan-in)
   */
  for (int iedge = 0; iedge < pb_graph_pin->num_input_edges; ++iedge) {
    /* We care the only edges in the selected mode */
    if (selected_mode !=
        pb_graph_pin->input_edges[iedge]->interconnect->parent_mode) {
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

/********************************************************************
 * This function identifies if two pb graph pins share at least one interconnect
 *model The two pins should be in the same type of port, for example, both are
 *inputs. Each pin may drive a number of outgoing edges while each edge
 *represents different interconnect model By iterating over outgoing edges for
 *each pin, common interconnect model may be found
 *******************************************************************/
bool is_pb_graph_pins_share_interc(const t_pb_graph_pin* pinA,
                                   const t_pb_graph_pin* pinB) {
  if (pinA->port->type != pinB->port->type) {
    return false;
  }
  std::vector<t_interconnect*> pinA_interc_list;
  for (auto out_edge : pinA->output_edges) {
    if (pinA_interc_list.end() == std::find(pinA_interc_list.begin(),
                                            pinA_interc_list.end(),
                                            out_edge->interconnect)) {
      pinA_interc_list.push_back(out_edge->interconnect);
    }
  }
  for (auto out_edge : pinB->output_edges) {
    if (pinA_interc_list.end() != std::find(pinA_interc_list.begin(),
                                            pinA_interc_list.end(),
                                            out_edge->interconnect)) {
      return true;
    }
  }
  return false;
}

} /* end namespace openfpga */
