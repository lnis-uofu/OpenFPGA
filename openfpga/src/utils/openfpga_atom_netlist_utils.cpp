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

#include "openfpga_reserved_words.h"
#include "openfpga_atom_netlist_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/***************************************************************************************
 * Find the names of all the atom blocks that drive clock nets 
 * This function will find if the block has been renamed due to contain sensitive characters
 * that violates the Verilog syntax
 ***************************************************************************************/
std::vector<std::string> find_atom_netlist_clock_port_names(const AtomNetlist& atom_nlist,
                                                            const VprNetlistAnnotation& netlist_annotation) {
  std::vector<std::string> clock_names;

  std::set<AtomPinId> clock_pins = find_netlist_logical_clock_drivers(atom_nlist);
  for (const AtomPinId& clock_pin : clock_pins) {
    const AtomBlockId& atom_blk = atom_nlist.port_block(atom_nlist.pin_port(clock_pin));
    std::string block_name = atom_nlist.block_name(atom_blk);
    if (true == netlist_annotation.is_block_renamed(atom_blk)) {
      block_name = netlist_annotation.block_name(atom_blk);
    }
    clock_names.push_back(block_name);
  }

  return clock_names;
}

/********************************************************************
 * Remove the prefix that is added to the name of a output block (by VPR) 
 *******************************************************************/
std::string remove_atom_block_name_prefix(const std::string& block_name) {
  /* VPR added a prefix of "out_" to the output ports of input benchmark */
  std::vector<std::string> prefix_to_remove;
  prefix_to_remove.push_back(std::string(VPR_BENCHMARK_OUT_PORT_PREFIX));
  prefix_to_remove.push_back(std::string(OPENFPGA_BENCHMARK_OUT_PORT_PREFIX));

  std::string ret_block_name = block_name;

  for (const std::string& cur_prefix_to_remove : prefix_to_remove) {
    if (!cur_prefix_to_remove.empty()) {
      if (0 == ret_block_name.find(cur_prefix_to_remove)) {
        ret_block_name.erase(0, cur_prefix_to_remove.length());
        break;
      }
    }
  }
  
  return ret_block_name;
}

} /* end namespace openfpga */
