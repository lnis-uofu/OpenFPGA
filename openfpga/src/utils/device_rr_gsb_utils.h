#ifndef DEVICE_RR_GSB_UTILS_H
#define DEVICE_RR_GSB_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "device_rr_gsb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

size_t find_device_rr_gsb_num_cb_modules(const DeviceRRGSB& device_rr_gsb,
                                         const t_rr_type& cb_type);

size_t find_device_rr_gsb_num_sb_modules(const DeviceRRGSB& device_rr_gsb);

size_t find_device_rr_gsb_num_gsb_modules(const DeviceRRGSB& device_rr_gsb);

} /* end namespace openfpga */

#endif
