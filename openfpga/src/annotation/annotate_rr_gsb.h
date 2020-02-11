#ifndef ANNOTATE_RR_GSB_H
#define ANNOTATE_RR_GSB_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "vpr_context.h"
#include "openfpga_context.h"
#include "device_rr_gsb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

void annotate_device_rr_gsb(const DeviceContext& vpr_device_ctx, 
                            DeviceRRGSB& device_rr_gsb,
                            const bool& verbose_output);

} /* end namespace openfpga */

#endif
