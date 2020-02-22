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
                                         t_pb_graph_node* pb_graph_node,
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
                                                  t_pb_graph_node* pb_graph_node,
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
                                     t_pb_graph_node* pb_graph_head,
                                     const VprDeviceAnnotation& device_annotation) {
  VTR_ASSERT(true == phy_pb.empty());

  rec_alloc_physical_pb_from_pb_graph(phy_pb, pb_graph_head, device_annotation);
  rec_build_physical_pb_children_from_pb_graph(phy_pb, pb_graph_head, device_annotation);
}

/************************************************************************
 * Synchronize mapping results from an operating pb to a physical pb
 ***********************************************************************/
void rec_update_physical_pb_from_operating_pb(PhysicalPb& phy_pb, 
                                              t_pb* op_pb) {
}

} /* end namespace openfpga */
