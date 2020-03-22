/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from 
 * VPR t_physical_tile_type 
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

#include "openfpga_physical_tile_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find the Fc of a pin in physical tile 
 *******************************************************************/
float find_physical_tile_pin_Fc(t_physical_tile_type_ptr type,
                                const int& pin) {
  for (const t_fc_specification& fc_spec : type->fc_specs) {
    if (fc_spec.pins.end() != std::find(fc_spec.pins.begin(), fc_spec.pins.end(), pin)) {
      return fc_spec.fc_value; 
    }
  }
  /* Every pin should have a Fc, give a wrong value */
  VTR_LOGF_ERROR(__FILE__, __LINE__,
                "Fail to find the Fc for %s.pin[%lu]\n",
                type->name, pin);
  exit(1);
} 

} /* end namespace openfpga */
