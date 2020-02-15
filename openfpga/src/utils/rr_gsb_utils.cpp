/********************************************************************
 * This file includes most utilized functions for data structure
 * DeviceRRGSB
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"

#include "rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find if a X-direction or Y-direction Connection Block contains
 * routing tracks only (zero configuration bits and routing multiplexers)
 *******************************************************************/
bool connection_block_contain_only_routing_tracks(const RRGSB& rr_gsb,
                                                  const t_rr_type& cb_type) {
  bool routing_track_only = true;

  /* Find routing multiplexers on the sides of a Connection block where IPIN nodes locate */
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    SideManager side_manager(cb_ipin_side);
    if (0 < rr_gsb.get_num_ipin_nodes(cb_ipin_side)) { 
      routing_track_only = false;
      break;
    }
  }
  
  return routing_track_only;
}

} /* end namespace openfpga */
