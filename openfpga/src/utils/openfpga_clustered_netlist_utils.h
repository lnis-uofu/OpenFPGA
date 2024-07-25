#ifndef OPENFPGA_CLUSTERED_NETLIST_UTILS_H
#define OPENFPGA_CLUSTERED_NETLIST_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "clustered_netlist.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::vector<ClusterNetId> find_clustered_netlist_global_nets(
  const ClusteredNetlist& clb_nlist);

} /* end namespace openfpga */

#endif
