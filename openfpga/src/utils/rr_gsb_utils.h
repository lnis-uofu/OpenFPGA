#ifndef RR_GSB_UTILS_H
#define RR_GSB_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "rr_gsb.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

bool connection_block_contain_only_routing_tracks(const RRGSB& rr_gsb,
                                                  const t_rr_type& cb_type);

} /* end namespace openfpga */

#endif
