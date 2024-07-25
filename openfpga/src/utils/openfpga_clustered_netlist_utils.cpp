/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from
 * VPR clustered netlist (post-packing netlist)
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vtrutil library */
#include "openfpga_clustered_netlist_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Find the names of all the atom blocks that drive clock nets
 * This function will find if the block has been renamed due to contain
 *sensitive characters that violates the Verilog syntax
 ***************************************************************************************/
std::vector<ClusterNetId> find_clustered_netlist_global_nets(
  const ClusteredNetlist& clb_nlist) {
  std::vector<ClusterNetId> gnets;

  for (ClusterNetId net_id : clb_nlist.nets()) {
    if (clb_nlist.net_is_ignored(net_id)) {
      gnets.push_back(net_id);
    }
  }

  return gnets;
}

} /* end namespace openfpga */
