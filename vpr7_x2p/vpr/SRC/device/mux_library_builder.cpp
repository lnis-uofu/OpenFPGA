/********************************************************************
 * This file includes the functions of builders for MuxLibrary.
 *******************************************************************/
#include <cmath>
#include <stdio.h>
#include "vtr_assert.h"

/* Device-level header files */
#include "util.h"
#include "vpr_types.h"
#include "globals.h"

/* FPGA-X2P context header files */
#include "fpga_x2p_utils.h"

#include "spice_types.h"
#include "circuit_library.h"
#include "mux_library.h"
#include "mux_library_builder.h"

/********************************************************************
 * Update MuxLibrary with the unique multiplexer structures
 * found in the global routing architecture
 *******************************************************************/
static 
void build_routing_arch_mux_library(MuxLibrary& mux_lib,
                                    int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                    t_switch_inf* switches,
                                    const CircuitLibrary& circuit_lib,
                                    t_det_routing_arch* routing_arch) {
  /* Current Version: Support Uni-directional routing architecture only*/ 
  if (UNI_DIRECTIONAL != routing_arch->directionality) {
    vpr_printf(TIO_MESSAGE_ERROR,
               "(FILE:%s, LINE[%d]) FPGA X2P Only supports uni-directional routing architecture.\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* The routing path is. 
   * OPIN ----> CHAN ----> ... ----> CHAN ----> IPIN
   * Each edge is a switch, for IPIN, the switch is a connection block,
   * for the rest is a switch box
   */
  /* Count the sizes of muliplexers in routing architecture */  
  for (int inode = 0; inode < LL_num_rr_nodes; inode++) {
    t_rr_node& node = LL_rr_node[inode]; 
    switch (node.type) {
    case IPIN: { 
      /* Have to consider the fan_in only, it is a connection block (multiplexer)*/
      VTR_ASSERT((node.fan_in > 0) || (0 == node.fan_in));
      if ( (0 == node.fan_in) || (1 == node.fan_in)) {
        break; 
      }
      /* Find the circuit_model for multiplexers in connection blocks */
      const CircuitModelId& cb_switch_circuit_model = switches[node.driver_switch].circuit_model;
      /* we should select a circuit model for the connection box*/
      VTR_ASSERT(CircuitModelId::INVALID() != cb_switch_circuit_model);
      /* Add the mux to mux_library */
      mux_lib.add_mux(circuit_lib, cb_switch_circuit_model, node.fan_in); 
      break;
    }
    case CHANX:
    case CHANY: {
      /* Channels are the same, have to consider the fan_in as well, 
       * it could be a switch box if previous rr_node is a channel
       * or it could be a connection box if previous rr_node is a IPIN or OPIN
       */
      VTR_ASSERT((node.fan_in > 0) || (0 == node.fan_in));
      if ((0 == node.fan_in) || (1 == node.fan_in)) {
        break; 
      }
      /* Find the spice_model for multiplexers in switch blocks*/
      const CircuitModelId& sb_switch_circuit_model = switches[node.driver_switch].circuit_model;
      /* we should select a circuit model for the Switch box*/
      VTR_ASSERT(CircuitModelId::INVALID() != sb_switch_circuit_model);
      /* Add the mux to mux_library */
      mux_lib.add_mux(circuit_lib, sb_switch_circuit_model, node.fan_in); 
      break;
    }
    default:
      /* We do not care other types of rr_node */
      break;
    }
  }
}

/********************************************************************
 * Update MuxLibrary with the unique multiplexer structures 
 * found in programmable logic blocks
 ********************************************************************/
static 
void build_pb_type_mux_library_rec(MuxLibrary& mux_lib,
                                   const CircuitLibrary& circuit_lib,
                                   t_pb_type* cur_pb_type) {
  VTR_ASSERT(nullptr != cur_pb_type);

  /* If there is spice_model_name, this is a leaf node!*/
  if (TRUE == is_primitive_pb_type(cur_pb_type)) {
    /* What annoys me is VPR create a sub pb_type for each lut which suppose to be a leaf node
     * This may bring software convience but ruins circuit modeling
     */
    VTR_ASSERT(CircuitModelId::INVALID() != cur_pb_type->phy_pb_type->circuit_model);
    return;
  }

  /* Traversal the hierarchy, find all the multiplexer from the interconnection part */
  for (int imode = 0; imode < cur_pb_type->num_modes; imode++) {
    /* Then we have to statisitic the interconnections*/
    for (int jinterc = 0; jinterc < cur_pb_type->modes[imode].num_interconnect; jinterc++) {
      /* Check the num_mux and fan_in of an interconnection */
      VTR_ASSERT ((0 == cur_pb_type->modes[imode].interconnect[jinterc].num_mux)
               || (0 < cur_pb_type->modes[imode].interconnect[jinterc].num_mux));
      if (0 == cur_pb_type->modes[imode].interconnect[jinterc].num_mux) {
        continue;
      }
      CircuitModelId& interc_circuit_model = cur_pb_type->modes[imode].interconnect[jinterc].circuit_model;
      VTR_ASSERT(CircuitModelId::INVALID() != interc_circuit_model); 
      /* Add the mux model to library */
      mux_lib.add_mux(circuit_lib, interc_circuit_model, cur_pb_type->modes[imode].interconnect[jinterc].fan_in);
    }
  }

  /* Go recursively to the lower level */
  for (int imode = 0; imode < cur_pb_type->num_modes; imode++) {
    for (int ichild = 0; ichild < cur_pb_type->modes[imode].num_pb_type_children; ichild++) {
      build_pb_type_mux_library_rec(mux_lib, circuit_lib,
                                    &cur_pb_type->modes[imode].pb_type_children[ichild]);
    }
  }
}

/********************************************************************
 * Update MuxLibrary with the unique multiplexers required by
 * LUTs in the circuit library
 ********************************************************************/
static 
void build_lut_mux_library(MuxLibrary& mux_lib,
                           const CircuitLibrary& circuit_lib) {
  /* Find all the circuit models which are LUTs in the circuit library */
  for (const auto& circuit_model : circuit_lib.models()) {
    /* Bypass non-LUT circuit models */
    if (SPICE_MODEL_LUT != circuit_lib.model_type(circuit_model)) {
      continue;
    }
    /* Find the MUX size required by the LUT */
    /* Get input ports which are not global ports! */
    std::vector<CircuitPortId> input_ports = circuit_lib.model_ports_by_type(circuit_model, SPICE_MODEL_PORT_INPUT, true);
    VTR_ASSERT(1 == input_ports.size());
    /* MUX size = 2^lut_size */
    size_t lut_mux_size = (size_t)pow(2., (double)(circuit_lib.port_size(input_ports[0])));
    /* Add mux to the mux library */
    mux_lib.add_mux(circuit_lib, circuit_model, lut_mux_size);
  }
}

/* Statistic for all the multiplexers in FPGA
 * We determine the sizes and its structure (according to spice_model) for each type of multiplexers
 * We search multiplexers in Switch Blocks, Connection blocks and Configurable Logic Blocks
 * In additional to multiplexers, this function also consider crossbars.
 * All the statistics are stored in a linked list, as a return value
 */
MuxLibrary build_device_mux_library(int LL_num_rr_nodes, t_rr_node* LL_rr_node,
                                    t_switch_inf* switches,
                                    const CircuitLibrary& circuit_lib,
                                    t_det_routing_arch* routing_arch) {
  /* MuxLibrary to store the information of Multiplexers*/
  MuxLibrary mux_lib;

  /* Step 1: We should check the multiplexer spice models defined in routing architecture.*/
  build_routing_arch_mux_library(mux_lib, LL_num_rr_nodes, LL_rr_node, switches, circuit_lib, routing_arch);

  /* Step 2: Count the sizes of multiplexers in complex logic blocks */  
  for (int itype = 0; itype < num_types; itype++) {
    if (NULL != type_descriptors[itype].pb_type) {
      build_pb_type_mux_library_rec(mux_lib, circuit_lib, type_descriptors[itype].pb_type);
    }
  }

  /* Step 3: count the size of multiplexer that will be used in LUTs*/
  build_lut_mux_library(mux_lib, circuit_lib); 

  return mux_lib;
}



