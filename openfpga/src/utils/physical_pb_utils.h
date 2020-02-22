/********************************************************************
 * Header file for circuit_library_utils.cpp
 *******************************************************************/
#ifndef PHYSICAL_PB_UTILS_H
#define PHYSICAL_PB_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <vector>
#include "physical_types.h"
#include "vpr_device_annotation.h"
#include "vpr_context.h"
#include "physical_pb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void alloc_physical_pb_from_pb_graph(PhysicalPb& phy_pb,
                                     const t_pb_graph_node* pb_graph_head,
                                     const VprDeviceAnnotation& device_annotation);

void rec_update_physical_pb_from_operating_pb(PhysicalPb& phy_pb, 
                                              const t_pb* op_pb,
                                              const t_pb_routes& pb_route,
                                              const AtomContext& atom_ctx,
                                              const VprDeviceAnnotation& device_annotation);

} /* end namespace openfpga */

#endif
