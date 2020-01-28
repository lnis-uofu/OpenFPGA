#ifndef ANNOTATE_PB_TYPES_H
#define ANNOTATE_PB_TYPES_H

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

void annotate_pb_types(const DeviceContext& vpr_device_ctx, 
                       const Arch& openfpga_arch,
                       VprPbTypeAnnotation& vpr_pb_type_annotation);

} /* end namespace openfpga */

#endif
