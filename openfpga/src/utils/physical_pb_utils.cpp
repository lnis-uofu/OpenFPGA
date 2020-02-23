/************************************************************************
 * Function to perform fundamental operation for the physical pb using
 * data structures
 ***********************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "openfpga_naming.h"
#include "pb_type_utils.h"
#include "physical_pb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************************************
 * Allocate an empty physical pb graph based on pb_graph 
 * This function should start with an empty physical pb object!!!
 * Suggest to check this before executing this function
 *   VTR_ASSERT(true == phy_pb.empty());
 ***********************************************************************/
static 
void rec_alloc_physical_pb_from_pb_graph(PhysicalPb& phy_pb,
                                         const t_pb_graph_node* pb_graph_node,
                                         const VprDeviceAnnotation& device_annotation) {
  t_pb_type* pb_type = pb_graph_node->pb_type;

  t_mode* physical_mode = device_annotation.physical_mode(pb_type);

  PhysicalPbId cur_phy_pb_id = phy_pb.create_pb(pb_graph_node);
  VTR_ASSERT(true == phy_pb.valid_pb_id(cur_phy_pb_id));

  /* Finish for primitive node */
  if (true == is_primitive_pb_type(pb_type)) {
    return;
  } 

  /* Find the physical mode */
  VTR_ASSERT(nullptr != physical_mode);

  /* Go to the leaf nodes first. This aims to build all the primitive nodes first 
   * and then we build the parents and create links
   */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
      rec_alloc_physical_pb_from_pb_graph(phy_pb,
                                          &(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]),
                                          device_annotation);
    }
  }
}

/************************************************************************
 * Build all the relationships between parent and children 
 * inside a physical pb graph 
 * This function must be executed after rec_alloc_physical_pb_from_pb_graph()!!!
 ***********************************************************************/
static 
void rec_build_physical_pb_children_from_pb_graph(PhysicalPb& phy_pb,
                                                  const t_pb_graph_node* pb_graph_node,
                                                  const VprDeviceAnnotation& device_annotation) {
  t_pb_type* pb_type = pb_graph_node->pb_type;

  /* Finish for primitive node */
  if (true == is_primitive_pb_type(pb_type)) {
    return;
  } 

  t_mode* physical_mode = device_annotation.physical_mode(pb_type);
  VTR_ASSERT(nullptr != physical_mode);

  /* Please use the openfpga naming function so that you can build the link to module manager */
  PhysicalPbId parent_pb_id = phy_pb.find_pb(pb_graph_node);
  VTR_ASSERT(true == phy_pb.valid_pb_id(parent_pb_id));

  /* Add all the children */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
      PhysicalPbId child_pb_id = phy_pb.find_pb(&(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]));
      VTR_ASSERT(true == phy_pb.valid_pb_id(child_pb_id));
      phy_pb.add_child(parent_pb_id, child_pb_id, &(physical_mode->pb_type_children[ipb]));
    }
  }

  /* Go to the leaf nodes first. This aims to build all the primitive nodes first 
   * and then we build the parents and create links
   */
  for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
    for (int jpb = 0; jpb < physical_mode->pb_type_children[ipb].num_pb; ++jpb) {
      rec_build_physical_pb_children_from_pb_graph(phy_pb,
                                                   &(pb_graph_node->child_pb_graph_nodes[physical_mode->index][ipb][jpb]),
                                                   device_annotation);
    }
  }
}

/************************************************************************
 * Allocate an empty physical pb graph based on pb_graph 
 * This function should start with an empty physical pb object!!!
 * Suggest to check this before executing this function
 *   VTR_ASSERT(true == phy_pb.empty());
 ***********************************************************************/
void alloc_physical_pb_from_pb_graph(PhysicalPb& phy_pb,
                                     const t_pb_graph_node* pb_graph_head,
                                     const VprDeviceAnnotation& device_annotation) {
  VTR_ASSERT(true == phy_pb.empty());

  rec_alloc_physical_pb_from_pb_graph(phy_pb, pb_graph_head, device_annotation);
  rec_build_physical_pb_children_from_pb_graph(phy_pb, pb_graph_head, device_annotation);
}

/************************************************************************
 * Update a mapping net from a pin of an operating primitive pb to a 
 * physical pb data base
 ***********************************************************************/
static 
void update_primitive_physical_pb_pin_atom_net(PhysicalPb& phy_pb, 
                                               const PhysicalPbId& primitive_pb,
                                               const t_pb_graph_pin* pb_graph_pin,
                                               const t_pb_routes& pb_route,
                                               const VprDeviceAnnotation& device_annotation) {
  int node_index = pb_graph_pin->pin_count_in_cluster;
  if (pb_route.count(node_index)) {
    /* The pin is mapped to a net, find the original pin in the atom netlist */
    AtomNetId atom_net = pb_route[node_index].atom_net_id;

    VTR_ASSERT(atom_net);
   
    /* Find the physical pb_graph_pin */
    t_pb_graph_pin* physical_pb_graph_pin = device_annotation.physical_pb_graph_pin(pb_graph_pin);
    VTR_ASSERT(nullptr != physical_pb_graph_pin);

    /* Check if the pin has been mapped to a net.
     * If yes, the atom net must be the same 
     */
    if (AtomNetId::INVALID() == phy_pb.pb_graph_pin_atom_net(primitive_pb, physical_pb_graph_pin)) {
      phy_pb.set_pb_graph_pin_atom_net(primitive_pb, physical_pb_graph_pin, atom_net);
    } else {
      VTR_ASSERT(atom_net == phy_pb.pb_graph_pin_atom_net(primitive_pb, physical_pb_graph_pin));
    }
  }
}

/************************************************************************
 * Synchronize mapping nets from an operating primitive pb to a physical pb
 ***********************************************************************/
static 
void synchronize_primitive_physical_pb_atom_nets(PhysicalPb& phy_pb, 
                                                 const PhysicalPbId& primitive_pb,
                                                 const t_pb_graph_node* pb_graph_node,
                                                 const t_pb_routes& pb_route,
                                                 const AtomContext& atom_ctx,
                                                 const AtomBlockId& atom_blk,
                                                 const VprDeviceAnnotation& device_annotation) {
  /* Iterate over all the ports: input, output and clock */
  for (int iport = 0; iport < pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_input_pins[iport]; ++ipin) {
      /* Port exists (some LUTs may have no input and hence no port in the atom netlist) */
      t_model_ports* model_port = pb_graph_node->input_pins[iport][ipin].port->model_port;
      if (nullptr == model_port) {
        continue;
      } 

      AtomPortId atom_port = atom_ctx.nlist.find_atom_port(atom_blk, model_port);
      if (!atom_port) { 
        continue;
      }
      /* Find the atom nets mapped to the pin
       * Note that some inputs may not be used, we set them to be open by default
       */
      update_primitive_physical_pb_pin_atom_net(phy_pb, primitive_pb,
                                                &(pb_graph_node->input_pins[iport][ipin]),
                                                pb_route, device_annotation);
    }
  }

  for (int iport = 0; iport < pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_output_pins[iport]; ++ipin) {
      /* Port exists (some LUTs may have no input and hence no port in the atom netlist) */
      t_model_ports* model_port = pb_graph_node->output_pins[iport][ipin].port->model_port;
      if (nullptr == model_port) {
        continue;
      } 

      AtomPortId atom_port = atom_ctx.nlist.find_atom_port(atom_blk, model_port);
      if (!atom_port) { 
        continue;
      }
      /* Find the atom nets mapped to the pin
       * Note that some inputs may not be used, we set them to be open by default
       */
      update_primitive_physical_pb_pin_atom_net(phy_pb, primitive_pb,
                                                &(pb_graph_node->output_pins[iport][ipin]),
                                                pb_route, device_annotation);
    }
  }

  for (int iport = 0; iport < pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < pb_graph_node->num_clock_pins[iport]; ++ipin) {
      /* Port exists (some LUTs may have no input and hence no port in the atom netlist) */
      t_model_ports* model_port = pb_graph_node->clock_pins[iport][ipin].port->model_port;
      if (nullptr == model_port) {
        continue;
      } 

      AtomPortId atom_port = atom_ctx.nlist.find_atom_port(atom_blk, model_port);
      if (!atom_port) { 
        continue;
      }
      /* Find the atom nets mapped to the pin
       * Note that some inputs may not be used, we set them to be open by default
       */
      update_primitive_physical_pb_pin_atom_net(phy_pb, primitive_pb,
                                                &(pb_graph_node->clock_pins[iport][ipin]),
                                                pb_route, device_annotation);
    }
  }
}

/************************************************************************
 * Synchronize mapping results from an operating pb to a physical pb
 ***********************************************************************/
void rec_update_physical_pb_from_operating_pb(PhysicalPb& phy_pb, 
                                              const t_pb* op_pb,
                                              const t_pb_routes& pb_route,
                                              const AtomContext& atom_ctx,
                                              const VprDeviceAnnotation& device_annotation) {
  t_pb_graph_node* pb_graph_node = op_pb->pb_graph_node;
  t_pb_type* pb_type = pb_graph_node->pb_type;

  if (true == is_primitive_pb_type(pb_type)) {
    t_pb_graph_node* physical_pb_graph_node = device_annotation.physical_pb_graph_node(pb_graph_node);
    VTR_ASSERT(nullptr != physical_pb_graph_node);
    /* Find the physical pb */
    const PhysicalPbId& physical_pb = phy_pb.find_pb(physical_pb_graph_node);
    VTR_ASSERT(true == phy_pb.valid_pb_id(physical_pb));

    /* Set the mode bits */
    phy_pb.set_mode_bits(physical_pb, device_annotation.pb_type_mode_bits(physical_pb_graph_node->pb_type));

    /* Find mapped atom block and add to this physical pb */
    AtomBlockId atom_blk = atom_ctx.nlist.find_block(op_pb->name);
    VTR_ASSERT(atom_blk);

    phy_pb.add_atom_block(physical_pb, atom_blk);

    /* TODO: Iterate over ports and annotate the atom pins */
    synchronize_primitive_physical_pb_atom_nets(phy_pb, physical_pb,
                                                pb_graph_node, 
                                                pb_route,
                                                atom_ctx, atom_blk,
                                                device_annotation);
    return;
  }

  /* Walk through the pb recursively but only visit the mapped modes and child pbs */
  t_mode* mapped_mode = &(pb_graph_node->pb_type->modes[op_pb->mode]);
  for (int ipb = 0; ipb < mapped_mode->num_pb_type_children; ++ipb) {
    /* Each child may exist multiple times in the hierarchy*/
    for (int jpb = 0; jpb < mapped_mode->pb_type_children[ipb].num_pb; ++jpb) {
      if ((nullptr != op_pb->child_pbs[ipb]) && (nullptr != op_pb->child_pbs[ipb][jpb].name)) {
        rec_update_physical_pb_from_operating_pb(phy_pb, 
                                                 &(op_pb->child_pbs[ipb][jpb]),
                                                 pb_route,
                                                 atom_ctx,
                                                 device_annotation);
      }
    }
  }
}

} /* end namespace openfpga */
