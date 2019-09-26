/********************************************************************
 * This file includes most utilized function for rr_block data structures
 *******************************************************************/
#include <vector>
#include <algorithm>

#include "vtr_assert.h"
#include "vpr_types.h"
#include "fpga_x2p_types.h"
#include "rr_blocks_utils.h"
 
/*********************************************************************
 * This function will find the global ports required by a Switch Block
 * module. It will find all the circuit models in the circuit library 
 * that may be included in the Switch Block
 * Collect the global ports from the circuit_models and merge with the same name
 ********************************************************************/
std::vector<CircuitPortId> find_switch_block_global_ports(const RRGSB& rr_gsb, 
                                                          const CircuitLibrary& circuit_lib,
                                                          const std::vector<t_switch_inf>& switch_lib) {
  std::vector<CircuitModelId> sub_models;
  /* Walk through the OUTPUT nodes at each side of a GSB, 
   * get the switch id of incoming edges 
   * and get the circuit model linked to the switch id
   */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      if (OUT_PORT != rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        continue;
      }
      /* Find the driver switch of the node */
      short driver_switch = rr_gsb.get_chan_node(side_manager.get_side(), itrack)->drive_switches[DEFAULT_SWITCH_ID];
      /* Find the circuit model id of the driver switch */
      VTR_ASSERT( (size_t)driver_switch < switch_lib.size() );
      /* Get the model, and try to add to the sub_model list */
      CircuitModelId switch_circuit_model = switch_lib[driver_switch].circuit_model;
      /* Make sure it is a valid id */
      VTR_ASSERT( CircuitModelId::INVALID() != switch_circuit_model );
      /* Get the model, and try to add to the sub_model list */
      if (sub_models.end() == std::find(sub_models.begin(), sub_models.end(), switch_circuit_model)) {
        /* Not yet in the list, add it */
        sub_models.push_back(switch_circuit_model);
      }
    }
  }

  std::vector<CircuitPortId> global_ports;
  /* Iterate over the model list, and add the global ports*/
  for (const auto& model : sub_models) {
    std::vector<CircuitPortId> temp_global_ports = circuit_lib.model_global_ports(model, true);
    /* Add the temp_global_ports to the list to be returned, make sure we do not have any duplicated ports */
    for (const auto& port_candidate : temp_global_ports) {
      if (global_ports.end() == std::find(global_ports.begin(), global_ports.end(), port_candidate)) {
        /* Not yet in the list, add it */
        global_ports.push_back(port_candidate);
      }
    }
  }

  return global_ports;
}

/*********************************************************************
 * This function will find the number of multiplexers required by 
 * a Switch Block module. 
 ********************************************************************/
size_t find_switch_block_number_of_muxes(const RRGSB& rr_gsb) {
  size_t num_muxes = 0;
  /* Walk through the OUTPUT nodes at each side of a GSB, 
   * get the switch id of incoming edges 
   * and get the circuit model linked to the switch id
   */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      if (OUT_PORT != rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        continue;
      }
      /* Check if this node is driven by a multiplexer */
      if (true == rr_gsb.is_sb_node_passing_wire(side_manager.get_side(), itrack)) {
        continue;
      }
      /* This means we need a multiplexer, update the counter */
      num_muxes++;
    }
  }
  return num_muxes;
}

