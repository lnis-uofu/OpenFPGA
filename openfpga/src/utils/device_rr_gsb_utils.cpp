/********************************************************************
 * This file includes most utilized functions for data structure
 * DeviceRRGSB
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "device_rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to find out the number of connection block
 * modules in the device rr_gsb array
 *******************************************************************/
size_t find_device_rr_gsb_num_cb_modules(const DeviceRRGSB& device_rr_gsb,
                                         const t_rr_type& cb_type) {
  size_t counter = 0;
  for (size_t x = 0; x < device_rr_gsb.get_gsb_range().x(); ++x) { 
    for (size_t y = 0; y < device_rr_gsb.get_gsb_range().y(); ++y) { 
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(x, y);
      if (true == rr_gsb.is_cb_exist(cb_type)) {
        counter++;
      }
    }
  }

  return counter;
}

/********************************************************************
 * This function aims to find out the number of switch block
 * modules in the device rr_gsb array
 *******************************************************************/
size_t find_device_rr_gsb_num_sb_modules(const DeviceRRGSB& device_rr_gsb) {
  size_t counter = 0;
  for (size_t x = 0; x < device_rr_gsb.get_gsb_range().x(); ++x) { 
    for (size_t y = 0; y < device_rr_gsb.get_gsb_range().y(); ++y) { 
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(x, y);
      if (true == rr_gsb.is_sb_exist()) {
        counter++;
      }
    }
  }

  return counter;
}

/********************************************************************
 * This function aims to find out the number of GSBs
 *******************************************************************/
size_t find_device_rr_gsb_num_gsb_modules(const DeviceRRGSB& device_rr_gsb) {
  size_t counter = 0;
  for (size_t x = 0; x < device_rr_gsb.get_gsb_range().x(); ++x) { 
    for (size_t y = 0; y < device_rr_gsb.get_gsb_range().y(); ++y) { 
      if (true == device_rr_gsb.is_gsb_exist(vtr::Point<size_t>(x, y))) {
        counter++;
      }
    }
  }

  return counter;

}

} /* end namespace openfpga */
