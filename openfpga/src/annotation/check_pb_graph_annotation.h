#ifndef CHECK_PB_GRAPH_ANNOTATION_H
#define CHECK_PB_GRAPH_ANNOTATION_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"
#include "vpr_pb_type_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void check_physical_pb_graph_node_annotation(const DeviceContext& vpr_device_ctx, 
                                             const VprPbTypeAnnotation& vpr_pb_type_annotation);

} /* end namespace openfpga */

#endif
