/********************************************************************
 * This file includes functions to detect and correct any naming
 * in the users' BLIF netlist that violates the syntax of OpenFPGA
 * fabric generator, i.e., Verilog generator and SPICE generator
 *******************************************************************/
#include <string>

/* Headers from vtrutil library */
#include "vtr_time.h"
#include "vtr_assert.h"
#include "vtr_log.h"

#include "check_netlist_naming_conflict.h"

/* Include global variables of VPR */
#include "globals.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function aims to check if the name contains any of the 
 * sensitive characters in the list
 *******************************************************************/
static 
bool name_contain_sensitive_chars(const std::string& name, 
                                  const std::string& sensitive_chars) {
  for (const char& sensitive_char : sensitive_chars) {
    /* Return true since we find a characters */
    if (std::string::npos != name.find(sensitive_char)) {
      return true;
    }
  }

  return false;
}

/********************************************************************
 * Detect and report any naming conflict by checking a list of 
 * sensitive characters
 * - Iterate over all the blocks and see if any block name contain 
 *   any sensitive character
 * - Iterate over all the nets and see if any net name contain 
 *   any sensitive character
 *******************************************************************/
static 
void detect_netlist_naming_conflict(const AtomNetlist& atom_netlist,
                                    const std::string& sensitive_chars) {
  size_t num_conflicts = 0;

  /* Walk through blocks in the netlist */
  for (const auto& block : atom_netlist.blocks()) {
    const std::string& block_name = atom_netlist.block_name(block);
    if (true == name_contain_sensitive_chars(block_name, sensitive_chars)) {
      VTR_LOG("Block '%s' violates the syntax requirement by OpenFPGA!\n",
              block_name.c_str());
      num_conflicts++;
    }
  }

  /* Walk through nets in the netlist */
  for (const auto& net : atom_netlist.nets()) {
    const std::string& net_name = atom_netlist.net_name(net);
    if (true == name_contain_sensitive_chars(net_name, sensitive_chars)) {
      VTR_LOG("Net '%s' violates the syntax requirement by OpenFPGA!\n",
              net_name.c_str());
      num_conflicts++;
    }
  }

  if (0 < num_conflicts) {
    VTR_LOG("Found %ld naming conflicts in the netlist. Please correct so as to use any fabric generators.\n",
            num_conflicts);
  }
} 

/********************************************************************
 * Top-level function to detect and correct any naming
 * in the users' BLIF netlist that violates the syntax of OpenFPGA
 * fabric generator, i.e., Verilog generator and SPICE generator
 *******************************************************************/
void check_netlist_naming_conflict(OpenfpgaContext& openfpga_context,
                                   const Command& cmd, const CommandContext& cmd_context) {
  const std::string& sensitive_chars(".,:;\'\"+-<>()[]{}!@#$%^&*~`?/");

  /* Do the main job first: detect any naming in the BLIF netlist that violates the syntax */
  detect_netlist_naming_conflict(g_vpr_ctx.atom().nlist, sensitive_chars); 
  
} 

} /* end namespace openfpga */

