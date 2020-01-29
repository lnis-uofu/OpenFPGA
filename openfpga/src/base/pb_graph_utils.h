#ifndef PB_GRAPH_UTILS_H
#define PB_GRAPH_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>
#include "physical_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<t_pb_graph_pin*> pb_graph_pin_inputs(t_pb_graph_pin* pb_graph_pin,
                                                 t_interconnect* selected_interconnect); 

} /* end namespace openfpga */

#endif
