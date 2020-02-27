/***************************************************************************************
 * This file includes most utilized functions that are used to acquire data from 
 * VPR atom netlist (users' netlist to implement)
 ***************************************************************************************/

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* Headers from vtrutil library */
#include "atom_netlist_utils.h"

#include "openfpga_atom_netlist_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Find the names of all the atom blocks that drive clock nets 
 ***************************************************************************************/
std::vector<std::string> find_atom_netlist_clock_port_names(const AtomNetlist& atom_nlist) {
  std::vector<std::string> clock_names;

  std::set<AtomPinId> clock_pins = find_netlist_logical_clock_drivers(atom_nlist);
  for (const AtomPinId& clock_pin : clock_pins) {
    const AtomBlockId& atom_blk = atom_nlist.port_block(atom_nlist.pin_port(clock_pin));
    clock_names.push_back(atom_nlist.block_name(atom_blk));
  }

  return clock_names;
}

} /* end namespace openfpga */
