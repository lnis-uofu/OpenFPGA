#ifndef ANNOTATE_BITSTREAM_SETTING_H
#define ANNOTATE_BITSTREAM_SETTING_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

int annotate_bitstream_setting(const BitstreamSetting& bitstream_setting, 
                               const DeviceContext& vpr_device_ctx,
                               const VprDeviceAnnotation& vpr_device_annotation,
                               VprBitstreamAnnotation& vpr_bitstream_annotation);

} /* end namespace openfpga */

#endif
