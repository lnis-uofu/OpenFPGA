#ifndef CHECK_PB_TYPE_ANNOTATION_H
#define CHECK_PB_TYPE_ANNOTATION_H

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

void check_vpr_physical_pb_mode_annotation(const DeviceContext& vpr_device_ctx, 
                                           const VprPbTypeAnnotation& vpr_pb_type_annotation);

void check_vpr_physical_pb_type_annotation(const DeviceContext& vpr_device_ctx, 
                                           const VprPbTypeAnnotation& vpr_pb_type_annotation);

void check_vpr_pb_type_circuit_model_annotation(const DeviceContext& vpr_device_ctx, 
                                                const CircuitLibrary& circuit_lib,
                                                const VprPbTypeAnnotation& vpr_pb_type_annotation);

void check_vpr_pb_type_mode_bits_annotation(const DeviceContext& vpr_device_ctx, 
                                            const CircuitLibrary& circuit_lib,
                                            const VprPbTypeAnnotation& vpr_pb_type_annotation);

} /* end namespace openfpga */

#endif
