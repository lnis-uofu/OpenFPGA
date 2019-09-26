/********************************************************************
 * This file includes functions to generate module/port names for 
 * Verilog and SPICE netlists 
 *
 * IMPORTANT: keep all the naming functions in this file to be 
 * generic for both Verilog and SPICE generators 
 ********************************************************************/
#include "vtr_assert.h"

#include "sides.h"
#include "fpga_x2p_utils.h"
#include "fpga_x2p_naming.h"

/************************************************
 * Generate the node name for a multiplexing structure 
 * Case 1 : If there is an intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in_buf
 * Case 1 : If there is NO intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in
 ***********************************************/
std::string generate_verilog_mux_node_name(const size_t& node_level, 
                                           const bool& add_buffer_postfix) {
  /* Generate the basic node_name */
  std::string node_name = "mux_l" + std::to_string(node_level) + "_in";

  /* Add a postfix upon requests */
  if (true == add_buffer_postfix)  {
    /* '1' indicates that the location is needed */
    node_name += "_buf";
  }

  return node_name;
}

/************************************************
 * Generate the module name for a multiplexer in Verilog format
 * Different circuit model requires different names: 
 * 1. LUTs are named as <model_name>_mux
 * 2. MUXes are named as <model_name>_size<num_inputs>
 ***********************************************/
std::string generate_verilog_mux_subckt_name(const CircuitLibrary& circuit_lib, 
                                             const CircuitModelId& circuit_model, 
                                             const size_t& mux_size, 
                                             const std::string& postfix) {
  std::string module_name = circuit_lib.model_name(circuit_model); 
  /* Check the model type and give different names */
  if (SPICE_MODEL_MUX == circuit_lib.model_type(circuit_model)) {
    module_name += "_size";
    module_name += std::to_string(mux_size);
  } else {  
    VTR_ASSERT(SPICE_MODEL_LUT == circuit_lib.model_type(circuit_model));
    module_name += "_mux";
  }

  /* Add postfix if it is not empty */
  if (!postfix.empty()) {
    module_name += postfix;
  }

  return module_name;
}

/************************************************
 * Generate the module name of a branch for a
 * multiplexer in Verilog format
 ***********************************************/
std::string generate_verilog_mux_branch_subckt_name(const CircuitLibrary& circuit_lib, 
                                                    const CircuitModelId& circuit_model, 
                                                    const size_t& mux_size, 
                                                    const size_t& branch_mux_size, 
                                                    const std::string& postfix) {
  /* If the tgate spice model of this MUX is a MUX2 standard cell,
   * the mux_subckt name will be the name of the standard cell
   */
  CircuitModelId subckt_model = circuit_lib.pass_gate_logic_model(circuit_model);
  if (SPICE_MODEL_GATE == circuit_lib.model_type(subckt_model)) {
    VTR_ASSERT (SPICE_MODEL_GATE_MUX2 == circuit_lib.gate_type(subckt_model));
    return circuit_lib.model_name(subckt_model);
  }
  std::string branch_postfix = postfix + "_size" + std::to_string(branch_mux_size);

  return generate_verilog_mux_subckt_name(circuit_lib, circuit_model, mux_size, branch_postfix);
}

/************************************************
 * Generate the module name of a local decoder
 * for multiplexer
 ***********************************************/
std::string generate_mux_local_decoder_subckt_name(const size_t& addr_size, 
                                                   const size_t& data_size) {
  std::string subckt_name = "decoder";
  subckt_name += std::to_string(addr_size);
  subckt_name += "to";
  subckt_name += std::to_string(data_size);

  return subckt_name;
} 

/************************************************
 * Generate the module name of a routing track wire
 ***********************************************/
std::string generate_segment_wire_subckt_name(const std::string& wire_model_name, 
                                              const size_t& segment_id) {
  std::string segment_wire_subckt_name = wire_model_name + "_seg" + std::to_string(segment_id);

  return segment_wire_subckt_name;
} 

/*********************************************************************
 * Generate the port name for the mid-output of a routing track wire
 * Mid-output is the output that is wired to a Connection block multiplexer.
 *      
 *                  |    CLB     |
 *                  +------------+
 *                        ^
 *                        |
 *           +------------------------------+
 *           | Connection block multiplexer |
 *           +------------------------------+
 *                        ^
 *                        |  mid-output         +--------------
 *              +--------------------+          |
 *    input --->| Routing track wire |--------->| Switch Block
 *              +--------------------+  output  |
 *                                              +--------------
 *
 ********************************************************************/
std::string generate_segment_wire_mid_output_name(const std::string& regular_output_name) {
  /* TODO: maybe have a postfix? */
  return std::string("mid_" + regular_output_name);
} 

/*********************************************************************
 * Generate the module name for a memory sub-circuit 
 ********************************************************************/
std::string generate_memory_module_name(const CircuitLibrary& circuit_lib,
                                        const CircuitModelId& circuit_model, 
                                        const CircuitModelId& sram_model, 
                                        const std::string& postfix) {
  return std::string( circuit_lib.model_name(circuit_model) + "_" + circuit_lib.model_name(sram_model) + postfix );
}

/*********************************************************************
 * Generate the netlist name for a unique routing block 
 * It could be 
 * 1. Routing channel
 * 2. Connection block
 * 3. Switch block
 * A unique block id should be given
 *********************************************************************/
std::string generate_routing_block_netlist_name(const std::string& prefix, 
                                                const size_t& block_id,
                                                const std::string& postfix) {
  return std::string( prefix + std::to_string(block_id) + postfix );
}

/*********************************************************************
 * Generate the netlist name for a routing block with a given coordinate
 * It could be 
 * 1. Routing channel
 * 2. Connection block
 * 3. Switch block
 *********************************************************************/
std::string generate_routing_block_netlist_name(const std::string& prefix, 
                                                const vtr::Point<size_t>& coordinate,
                                                const std::string& postfix) {
  return std::string( prefix + std::to_string(coordinate.x()) + std::string("_") + std::to_string(coordinate.y()) + postfix );
}

/*********************************************************************
 * Generate the module name for a unique routing channel
 *********************************************************************/
std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const size_t& block_id) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT( (CHANX == chan_type) || (CHANY == chan_type) );

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  return std::string( module_prefix_map[chan_type] + std::string("_") + std::to_string(block_id) + std::string("_") );
}

/*********************************************************************
 * Generate the module name for a routing channel with a given coordinate
 *********************************************************************/
std::string generate_routing_channel_module_name(const t_rr_type& chan_type, 
                                                 const vtr::Point<size_t>& coordinate) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT( (CHANX == chan_type) || (CHANY == chan_type) );

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  return std::string( module_prefix_map[chan_type] + std::to_string(coordinate.x()) + std::string("_") + std::to_string(coordinate.y()) + std::string("_") );
}

/*********************************************************************
 * Generate the port name for a routing track with a given coordinate
 * and port direction
 *********************************************************************/
std::string generate_routing_track_port_name(const t_rr_type& chan_type, 
                                             const vtr::Point<size_t>& coordinate,
                                             const size_t& track_id,
                                             const PORTS& port_direction) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT( (CHANX == chan_type) || (CHANY == chan_type) );

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  std::string port_name = module_prefix_map[chan_type]; 
  port_name += std::string("_" + std::to_string(coordinate.x()) + std::string("__") + std::to_string(coordinate.y()) + std::string("__"));

  switch (port_direction) {
  case OUT_PORT:
    port_name += std::string("out_"); 
    break;
  case IN_PORT:
    port_name += std::string("in_"); 
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File: %s [LINE%d]) Invalid direction of chan_rr_node!\n",
               __FILE__, __LINE__);
    exit(1);
  }

  /* Add the track id to the port name */
  port_name += std::to_string(track_id) + std::string("_");

  return port_name;
}

/*********************************************************************
 * Generate the module name for a switch block with a given coordinate
 *********************************************************************/
std::string generate_switch_block_module_name(const vtr::Point<size_t>& coordinate) {
  return std::string( "sb_" + std::to_string(coordinate.x()) + std::string("__") + std::to_string(coordinate.y()) + std::string("_") );
}

/*********************************************************************
 * Generate the port name for a Grid
 * TODO: add more comments about why we need different names for 
 * top and non-top netlists
 *********************************************************************/
std::string generate_grid_port_name(const vtr::Point<size_t>& coordinate,
                                    const size_t& height, 
                                    const e_side& side, 
                                    const size_t& pin_id,
                                    const bool& for_top_netlist) {
  if (true == for_top_netlist) {
    std::string port_name = "grid_";
    port_name += std::to_string(coordinate.x());
    port_name += "__";
    port_name += std::to_string(coordinate.y());
    port_name += "__pin_";
    port_name += std::to_string(height);
    port_name += "__";
    port_name += std::to_string(size_t(side));
    port_name += "__";
    port_name += std::to_string(pin_id);
    port_name += "_";
    return port_name;
  } 
  /* For non-top netlist */
  VTR_ASSERT( false == for_top_netlist );
  Side side_manager(side);
  std::string port_name = std::string(side_manager.to_string());
  port_name += "_height_";
  port_name += std::to_string(height);
  port_name += "__pin_";
  port_name += std::to_string(pin_id);
  port_name += "_";
  return port_name;
}


/*********************************************************************
 * Generate the port name for a reserved sram port, i.e., BLB/WL port
 * When port_type is BLB, a string denoting to the reserved BLB port is generated
 * When port_type is WL, a string denoting to the reserved WL port is generated
 *
 * DO NOT put any SRAM organization check codes HERE!!!
 * Even though the reserved BLB/WL ports are used by RRAM-based FPGA only, 
 * try to keep this function does simple job. 
 * Check codes should be added outside, when print the ports to files!!!  
 *********************************************************************/
std::string generate_reserved_sram_port_name(const e_spice_model_port_type& port_type) {
  VTR_ASSERT( (port_type == SPICE_MODEL_PORT_BLB) || (port_type == SPICE_MODEL_PORT_WL) ); 

  if (SPICE_MODEL_PORT_BLB == port_type) {
    return std::string("reserved_blb");
  }
  return std::string("reserved_wl");
}

/*********************************************************************
 * Generate the port name for a sram port, used for formal verification
 * The port name is named after the cell name of SRAM in circuit library
 * TODO: 
 * Use the new refactored data structure to replace the t_sram_orgz_info
 *********************************************************************/
std::string generate_formal_verification_sram_port_name(const CircuitLibrary& circuit_lib,
                                                        const CircuitModelId& sram_model) {
  std::string port_name = circuit_lib.model_name(sram_model) + std::string("_out_fm");

  return port_name;
}

/*********************************************************************
 * Generate the port name for a regular sram port
 * The port name is named after the cell name of SRAM in circuit library
 * TODO: 
 * Use the new refactored data structure to replace the t_sram_orgz_info
 *********************************************************************/
std::string generate_sram_port_name(const CircuitLibrary& circuit_lib,
                                    const CircuitModelId& sram_model,
                                    const e_sram_orgz& sram_orgz_type,
                                    const e_spice_model_port_type& port_type) {
  /* Get memory_model */

  std::string port_name = circuit_lib.model_name(sram_model) + std::string("_");

  switch (sram_orgz_type) {
  case SPICE_SRAM_STANDALONE: {
    /* Two types of ports are available:  
     * (1) Regular output of a SRAM, enabled by port type of INPUT
     * (2) Inverted output of a SRAM, enabled by port type of OUTPUT
     */
    if (SPICE_MODEL_PORT_INPUT == port_type) {
      port_name += std::string("out"); 
    } else {
      VTR_ASSERT( SPICE_MODEL_PORT_OUTPUT == port_type );
      port_name += std::string("outb"); 
    }
    break;
  }
  case SPICE_SRAM_SCAN_CHAIN:
    /* Two types of ports are available:  
     * (1) Head of a chain of Configuration-chain Flip-Flops (CCFFs), enabled by port type of INPUT
     * (2) Tail of a chian of Configuration-chain Flip-flops (CCFFs), enabled by port type of OUTPUT
     *           +------+    +------+    +------+
     *  Head --->| CCFF |--->| CCFF |--->| CCFF |---> Tail
     *           +------+    +------+    +------+
     */
    if (SPICE_MODEL_PORT_INPUT == port_type) {
      port_name += std::string("ccff_head"); 
    } else {
      VTR_ASSERT( SPICE_MODEL_PORT_OUTPUT == port_type );
      port_name += std::string("ccff_tail"); 
    }
    break;
  case SPICE_SRAM_MEMORY_BANK:
    /* Four types of ports are available:  
     * (1) Bit Lines (BLs) of a SRAM cell, enabled by port type of BL
     * (2) Word Lines (WLs) of a SRAM cell, enabled by port type of WL
     * (3) Inverted Bit Lines (BLBs) of a SRAM cell, enabled by port type of BLB
     * (4) Inverted Word Lines (WLBs) of a SRAM cell, enabled by port type of WLB
     *
     *           BL BLB WL WLB    BL BLB WL WLB    BL BLB WL WLB
     *          [0] [0] [0] [0]  [1] [1] [1] [1]  [i] [i] [i] [i]
     *            ^  ^  ^  ^       ^  ^  ^  ^       ^  ^  ^  ^
     *            |  |  |  |       |  |  |  |       |  |  |  |
     *           +----------+     +----------+     +----------+
     *           |   SRAM   |     |   SRAM   | ... |   SRAM   |         
     *           +----------+     +----------+     +----------+
     */
    if (SPICE_MODEL_PORT_BL == port_type) {
      port_name += std::string("bl"); 
    } else if (SPICE_MODEL_PORT_WL == port_type) {
      port_name += std::string("wl"); 
    } else if (SPICE_MODEL_PORT_BLB == port_type) {
      port_name += std::string("blb"); 
    } else {
      VTR_ASSERT( SPICE_MODEL_PORT_WLB == port_type );
      port_name += std::string("wlb"); 
    }
    break;
  default:
    vpr_printf(TIO_MESSAGE_ERROR,
               "(File:%s,[LINE%d])Invalid type of SRAM organization !\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return port_name;
}
