#ifndef ANNOTATE_MIF_H
#define ANNOTATE_MIF_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "mif_pipeline.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

/* Load eblif/hex into MifPipeline and build PHYSICAL MIF.
 * Must run after VPR pack (atom->operating pb) and after device annotation
 * (operating->physical) is available for the final PHYSICAL rewrite.
 *
 * EBLIF/LOGICAL keep operating pb paths to match bitstream-setting keys;
 * VprDeviceAnnotation is applied only to the aggregated PHYSICAL stage.
 * See build_physical_mif() in annotate_mif.cpp for the rationale. */
int build_physical_mif(const BitstreamSetting& bitstream_setting,
                       MifPipeline& mif_pipeline,
                       const DeviceContext& vpr_device_ctx,
                       const VprDeviceAnnotation& vpr_device_annotation);

} /* end namespace openfpga */

#endif
