/***************************************************************************************
 * This file includes functions that are used to redo packing for physical pbs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"

#include "lb_router_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Add a net to route to a logical block router
 * This function will automatically find the source and sink atom pins
 * based on the given atom net
 ***************************************************************************************/
LbRouter::NetId add_lb_router_net_to_route(LbRouter& lb_router,
                                           const LbRRNodeId& source_node,
                                           const std::vector<LbRRNodeId>& sink_nodes,
                                           const AtomNetlist& atom_nlist,
                                           const AtomNetId& atom_net_id) {
  VTR_ASSERT(0 < sink_nodes.size());

  LbRouter::NetId lb_net = lb_router.create_net_to_route(source_node, sink_nodes);

  VTR_ASSERT(AtomNetId::INVALID() != atom_net_id);
  lb_router.add_net_atom_net_id(lb_net, atom_net_id);
  std::vector<AtomPinId> terminal_pins;
  for (const AtomPinId& atom_pin : atom_nlist.net_sinks(atom_net_id)) {
    VTR_ASSERT(AtomPinId::INVALID() != atom_pin);
    terminal_pins.push_back(atom_pin);
  }
  VTR_ASSERT(AtomPinId::INVALID() != atom_nlist.net_driver(atom_net_id));
  lb_router.add_net_atom_pins(lb_net, atom_nlist.net_driver(atom_net_id), terminal_pins);

  return lb_net;
}

} /* end namespace openfpga */
