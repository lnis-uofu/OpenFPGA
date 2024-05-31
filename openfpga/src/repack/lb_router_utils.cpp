/***************************************************************************************
 * This file includes functions that are used to redo packing for physical pbs
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "lb_router_utils.h"

#include "vtr_assert.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Add a net to route to a logical block router
 * This function will automatically find the source and sink atom pins
 * based on the given atom net
 ***************************************************************************************/
LbRouter::NetId add_lb_router_net_to_route(
  LbRouter& lb_router, const LbRRGraph& lb_rr_graph,
  const std::vector<LbRRNodeId>& source_nodes,
  const std::vector<LbRRNodeId>& sink_nodes, const AtomContext& atom_ctx,
  const AtomNetId& atom_net_id) {
  VTR_ASSERT(0 < sink_nodes.size());

  LbRouter::NetId lb_net =
    lb_router.create_net_to_route(source_nodes, sink_nodes);

  VTR_ASSERT(AtomNetId::INVALID() != atom_net_id);
  lb_router.add_net_atom_net_id(lb_net, atom_net_id);

  std::vector<AtomPinId> terminal_pins;
  AtomPinId atom_pin_outside_pb = AtomPinId::INVALID();

  for (const LbRRNodeId& sink_node : sink_nodes) {
    t_pb_graph_pin* sink_pb_pin = lb_rr_graph.node_pb_graph_pin(sink_node);
    bool atom_pin_inside_pb = false;
    for (const AtomPinId& atom_pin : atom_ctx.nlist.net_sinks(atom_net_id)) {
      VTR_ASSERT(AtomPinId::INVALID() != atom_pin);
      if (sink_pb_pin ==
          find_pb_graph_pin(atom_ctx.nlist, atom_ctx.lookup, atom_pin)) {
        terminal_pins.push_back(atom_pin);
        atom_pin_inside_pb = true;
        break;
      }
      if (AtomPinId::INVALID() == atom_pin_outside_pb) {
        atom_pin_outside_pb = atom_pin;
      }
    }
    /* Add a atom pin which is not inside the pb */
    if (false == atom_pin_inside_pb) {
      VTR_ASSERT(AtomPinId::INVALID() != atom_pin_outside_pb);
      terminal_pins.push_back(atom_pin_outside_pb);
    }
  }
  VTR_ASSERT(AtomPinId::INVALID() != atom_ctx.nlist.net_driver(atom_net_id));
  if (sink_nodes.size() != terminal_pins.size()) {
    VTR_LOGF_ERROR(
      __FILE__, __LINE__,
      "Net '%s' has %lu sink nodes while has %lu associated atom pins!\n",
      atom_ctx.nlist.net_name(atom_net_id).c_str(), sink_nodes.size(),
      terminal_pins.size());
  }
  VTR_ASSERT(sink_nodes.size() == terminal_pins.size());

  lb_router.add_net_atom_pins(lb_net, atom_ctx.nlist.net_driver(atom_net_id),
                              terminal_pins);

  return lb_net;
}

/***************************************************************************************
 * Load the routing results (routing tree) from lb router to
 * a physical pb data structure
 ***************************************************************************************/
void save_lb_router_results_to_physical_pb(PhysicalPb& phy_pb,
                                           const LbRouter& lb_router,
                                           const LbRRGraph& lb_rr_graph,
                                           const AtomNetlist& atom_netlist,
                                           const bool& verbose) {
  /* Get mapping routing nodes per net */
  for (const LbRouter::NetId& net : lb_router.nets()) {
    std::vector<LbRRNodeId> routed_nodes = lb_router.net_routed_nodes(net);
    for (const LbRRNodeId& node : routed_nodes) {
      t_pb_graph_pin* pb_graph_pin = lb_rr_graph.node_pb_graph_pin(node);
      if (nullptr == pb_graph_pin) {
        continue;
      }
      /* Find the pb id */
      const PhysicalPbId& pb_id = phy_pb.find_pb(pb_graph_pin->parent_node);
      VTR_ASSERT(true == phy_pb.valid_pb_id(pb_id));

      const AtomNetId& atom_net = lb_router.net_atom_net_id(net);

      /* Print info to help debug */
      VTR_LOGV(verbose, "Save net '%s' to physical pb_graph_pin '%s'\n",
               atom_netlist.net_name(atom_net).c_str(),
               pb_graph_pin->to_string().c_str());

      if (AtomNetId::INVALID() ==
          phy_pb.pb_graph_pin_atom_net(pb_id, pb_graph_pin)) {
        phy_pb.set_pb_graph_pin_atom_net(pb_id, pb_graph_pin, atom_net);
      } else {
        VTR_ASSERT(atom_net ==
                   phy_pb.pb_graph_pin_atom_net(pb_id, pb_graph_pin));
      }
    }
  }
}

} /* end namespace openfpga */
