/********************************************************************
 * This file includes functions to build bitstream from global routing
 * architecture of a mapped FPGA fabric
 * We decode the bitstream from configuration of routing multiplexers 
 * which locate in global routing architecture
 *******************************************************************/
#include <vector>
#include <time.h>

#include "vtr_assert.h"
#include "util.h"
#include "mux_utils.h"
#include "rr_blocks_utils.h"
#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_naming.h"
#include "fpga_x2p_utils.h"

#include "build_mux_bitstream.h"
#include "build_routing_bitstream.h"

/********************************************************************
 * This function generates bitstream for a routing multiplexer
 * This function will identify if a node indicates a routing multiplexer
 * If not a routing multiplexer, no bitstream is needed here
 * If yes, we will generate the bitstream for the routing multiplexer
 *******************************************************************/
static 
void build_switch_block_mux_bitstream(BitstreamManager& bitstream_manager,
                                      const ConfigBlockId& mux_mem_block,
                                      const ModuleManager& module_manager,
                                      const CircuitLibrary& circuit_lib,
                                      const MuxLibrary& mux_lib,
                                      const std::vector<t_switch_inf>& rr_switches,
                                      t_rr_node* L_rr_node,
                                      t_rr_node* cur_rr_node,
                                      const std::vector<t_rr_node*>& drive_rr_nodes,
                                      const int& switch_index) {
  /* Check current rr_node is CHANX or CHANY*/
  VTR_ASSERT((CHANX == cur_rr_node->type)||(CHANY == cur_rr_node->type));
  
  /* Find the circuit model id of the mux, we need its design technology which matters the bitstream generation */
  CircuitModelId mux_model = rr_switches[switch_index].circuit_model;

  /* Find the input size of the implementation of a routing multiplexer */
  size_t datapath_mux_size = drive_rr_nodes.size();

  /* Find out which routing path is used in this MUX */
  int path_id = DEFAULT_PATH_ID;
  for (size_t inode = 0; inode < drive_rr_nodes.size(); ++inode) {
    if (drive_rr_nodes[inode] == &(L_rr_node[cur_rr_node->prev_node])) {
      path_id = (int)inode;
      break;
    }
  }

  /* Ensure that our path id makes sense! */
  VTR_ASSERT( (DEFAULT_PATH_ID == path_id)
           || ( (DEFAULT_PATH_ID < path_id) && (path_id < (int)datapath_mux_size) ) 
            );

  /* Generate bitstream depend on both technology and structure of this MUX */
  std::vector<bool> mux_bitstream = build_mux_bitstream(circuit_lib, mux_model, mux_lib, datapath_mux_size, path_id); 

  /* Find the module in module manager and ensure the bitstream size matches! */
  std::string mem_module_name = generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size, std::string(MEMORY_MODULE_POSTFIX)); 
  ModuleId mux_mem_module = module_manager.find_module(mem_module_name); 
  VTR_ASSERT (true == module_manager.valid_module_id(mux_mem_module));
  ModulePortId mux_mem_out_port_id = module_manager.find_module_port(mux_mem_module, generate_configuration_chain_data_out_name());
  VTR_ASSERT(mux_bitstream.size() == module_manager.module_port(mux_mem_module, mux_mem_out_port_id).get_width());

  /* Add the bistream to the bitstream manager */
  for (const bool& bit : mux_bitstream) {
    ConfigBitId config_bit = bitstream_manager.add_bit(bit);
    /* Link the memory bits to the mux mem block */
    bitstream_manager.add_bit_to_block(mux_mem_block, config_bit);
  }
}

/********************************************************************
 * This function generates bitstream for an interconnection, 
 * i.e., a routing multiplexer, in a Switch Block
 * This function will identify if a node indicates a routing multiplexer
 * If not a routing multiplexer, no bitstream is needed here
 * If yes, we will generate the bitstream for the routing multiplexer
 *******************************************************************/
static 
void build_switch_block_interc_bitstream(BitstreamManager& bitstream_manager,
                                         const ConfigBlockId& sb_configurable_block,
                                         const ModuleManager& module_manager,
                                         const CircuitLibrary& circuit_lib,
                                         const MuxLibrary& mux_lib,
                                         const std::vector<t_switch_inf>& rr_switches,
                                         t_rr_node* L_rr_node,
                                         const RRGSB& rr_gsb,
                                         const e_side& chan_side,
                                         const size_t& chan_node_id) {

  std::vector<t_rr_node*> drive_rr_nodes;

  /* Get the node */
  t_rr_node* cur_rr_node = rr_gsb.get_chan_node(chan_side, chan_node_id);

  /* Determine if the interc lies inside a channel wire, that is interc between segments */
  if (false == rr_gsb.is_sb_node_passing_wire(chan_side, chan_node_id)) {
    for (int i = 0; i < cur_rr_node->num_drive_rr_nodes; ++i) {
      drive_rr_nodes.push_back(cur_rr_node->drive_rr_nodes[i]);
    }
    /* Special: if there are zero-driver nodes. We skip here */
    if (0 == drive_rr_nodes.size()) {
      return; 
    }
  }

  if ( (0 == drive_rr_nodes.size())
    || (0 == drive_rr_nodes.size()) ) {
    /* No bitstream generation required by a special direct connection*/
    return;
  } else if (1 < drive_rr_nodes.size()) {
    /* Create the block denoting the memory instances that drives this node in Switch Block */
    std::string mem_block_name = generate_sb_memory_instance_name(SWITCH_BLOCK_MEM_INSTANCE_PREFIX, chan_side, chan_node_id, std::string(""));
    ConfigBlockId mux_mem_block = bitstream_manager.add_block(mem_block_name);
    bitstream_manager.add_child_block(sb_configurable_block, mux_mem_block);
    /* This is a routing multiplexer! Generate bitstream */
    build_switch_block_mux_bitstream(bitstream_manager, mux_mem_block, module_manager,
                                     circuit_lib, mux_lib, rr_switches, L_rr_node, 
                                     cur_rr_node, drive_rr_nodes, 
                                     cur_rr_node->drive_switches[DEFAULT_SWITCH_ID]);
  } /*Nothing should be done else*/ 
}

/********************************************************************
 * This function generates bitstream for a Switch Block
 * and add it to the bitstream manager
 * This function will spot all the routing multiplexers in a Switch Block
 * using a simple but effective rule:
 * The fan-in of each output node. 
 * If there are more than 2 fan-in, there is a routing multiplexer 
 *
 * Note that the output nodes typically spread over all the sides of a Switch Block
 * So, we will iterate over that.
 *******************************************************************/
static 
void build_switch_block_bitstream(BitstreamManager& bitstream_manager,
                                  const ConfigBlockId& sb_config_block,
                                  const ModuleManager& module_manager,
                                  const CircuitLibrary& circuit_lib,
                                  const MuxLibrary& mux_lib,
                                  const std::vector<t_switch_inf>& rr_switches,
                                  t_rr_node* L_rr_node,
                                  const RRGSB& rr_gsb) {

  /* Iterate over all the multiplexers */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      VTR_ASSERT( (CHANX == rr_gsb.get_chan_node(side_manager.get_side(), itrack)->type)
               || (CHANY == rr_gsb.get_chan_node(side_manager.get_side(), itrack)->type));
      /* Only output port indicates a routing multiplexer */
      if (OUT_PORT != rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        continue;
      }
      build_switch_block_interc_bitstream(bitstream_manager, sb_config_block, 
                                          module_manager, 
                                          circuit_lib, mux_lib, rr_switches, L_rr_node,
                                          rr_gsb, side_manager.get_side(), itrack);
    }
  }
}

/********************************************************************
 * This function generates bitstream for a routing multiplexer
 * in a Connection block
 * This function will identify if a node indicates a routing multiplexer
 * If not a routing multiplexer, no bitstream is needed here
 * If yes, we will generate the bitstream for the routing multiplexer
 *******************************************************************/
static 
void build_connection_block_mux_bitstream(BitstreamManager& bitstream_manager,
                                          const ConfigBlockId& mux_mem_block,
                                          const ModuleManager& module_manager,
                                          const CircuitLibrary& circuit_lib,
                                          const MuxLibrary& mux_lib,
                                          const std::vector<t_switch_inf>& rr_switches,
                                          t_rr_node* L_rr_node,
                                          t_rr_node* src_rr_node) {
  /* Find the circuit model id of the mux, we need its design technology which matters the bitstream generation */
  int switch_index = src_rr_node->drive_switches[DEFAULT_SWITCH_ID];
  CircuitModelId mux_model = rr_switches[switch_index].circuit_model;

  /* Find drive_rr_nodes*/
  size_t datapath_mux_size = (size_t)(src_rr_node->num_drive_rr_nodes);

  /* Configuration bits for MUX*/
  int path_id = DEFAULT_PATH_ID;
  for (size_t inode = 0; inode < datapath_mux_size; ++inode) {
    if (src_rr_node->drive_rr_nodes[inode] == &(L_rr_node[src_rr_node->prev_node])) {
      path_id = (int)inode;
      break;
    }
  }

  /* Ensure that our path id makes sense! */
  VTR_ASSERT( (DEFAULT_PATH_ID == path_id)
           || ( (DEFAULT_PATH_ID < path_id) && (path_id < (int)datapath_mux_size) ) 
            );

  /* Generate bitstream depend on both technology and structure of this MUX */
  std::vector<bool> mux_bitstream = build_mux_bitstream(circuit_lib, mux_model, mux_lib, datapath_mux_size, path_id); 

  /* Find the module in module manager and ensure the bitstream size matches! */
  std::string mem_module_name = generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size, std::string(MEMORY_MODULE_POSTFIX)); 
  ModuleId mux_mem_module = module_manager.find_module(mem_module_name); 
  VTR_ASSERT (true == module_manager.valid_module_id(mux_mem_module));
  ModulePortId mux_mem_out_port_id = module_manager.find_module_port(mux_mem_module, generate_configuration_chain_data_out_name());
  VTR_ASSERT(mux_bitstream.size() == module_manager.module_port(mux_mem_module, mux_mem_out_port_id).get_width());

  /* Add the bistream to the bitstream manager */
  for (const bool& bit : mux_bitstream) {
    ConfigBitId config_bit = bitstream_manager.add_bit(bit);
    /* Link the memory bits to the mux mem block */
    bitstream_manager.add_bit_to_block(mux_mem_block, config_bit);
  }
}


/********************************************************************
 * This function generates bitstream for an interconnection, 
 * i.e., a routing multiplexer, in a Connection Block
 * This function will identify if a node indicates a routing multiplexer
 * If not a routing multiplexer, no bitstream is needed here
 * If yes, we will generate the bitstream for the routing multiplexer
 *******************************************************************/
static 
void build_connection_block_interc_bitstream(BitstreamManager& bitstream_manager,
                                         const ConfigBlockId& cb_configurable_block,
                                         const ModuleManager& module_manager,
                                         const CircuitLibrary& circuit_lib,
                                         const MuxLibrary& mux_lib,
                                         const std::vector<t_switch_inf>& rr_switches,
                                         t_rr_node* L_rr_node,
                                         const RRGSB& rr_gsb,
                                         const e_side& cb_ipin_side, 
                                         const size_t& ipin_index) {

  t_rr_node* src_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, ipin_index);
  if (1 == src_rr_node->fan_in) {
    /* No bitstream generation required by a special direct connection*/
  } else if (1 < src_rr_node->fan_in) {
    /* Create the block denoting the memory instances that drives this node in Switch Block */
    std::string mem_block_name = generate_cb_memory_instance_name(CONNECTION_BLOCK_MEM_INSTANCE_PREFIX, rr_gsb.get_ipin_node_grid_side(cb_ipin_side, ipin_index), ipin_index, std::string(""));
    ConfigBlockId mux_mem_block = bitstream_manager.add_block(mem_block_name);
    bitstream_manager.add_child_block(cb_configurable_block, mux_mem_block);
    /* This is a routing multiplexer! Generate bitstream */
    build_connection_block_mux_bitstream(bitstream_manager, mux_mem_block, 
                                         module_manager, circuit_lib, mux_lib, rr_switches, 
                                         L_rr_node, src_rr_node);
  } /*Nothing should be done else*/ 
}

/********************************************************************
 * This function generates bitstream for a Connection Block
 * and add it to the bitstream manager
 * This function will spot all the routing multiplexers in a Connection Block
 * using a simple but effective rule:
 * The fan-in of each output node. 
 * If there are more than 2 fan-in, there is a routing multiplexer 
 *
 * Note that the output nodes are the IPIN rr node in a Connection Block
 * So, we will iterate over that.
 *******************************************************************/
static 
void build_connection_block_bitstream(BitstreamManager& bitstream_manager,
                                      const ConfigBlockId& cb_configurable_block,
                                      const ModuleManager& module_manager,
                                      const CircuitLibrary& circuit_lib,
                                      const MuxLibrary& mux_lib,
                                      const std::vector<t_switch_inf>& rr_switches,
                                      t_rr_node* L_rr_node,
                                      const RRGSB& rr_gsb,
                                      const t_rr_type& cb_type) {
   
  /* Find routing multiplexers on the sides of a Connection block where IPIN nodes locate */
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    Side side_manager(cb_ipin_side);
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) { 
      build_connection_block_interc_bitstream(bitstream_manager, cb_configurable_block,
                                              module_manager, circuit_lib, mux_lib, 
                                              rr_switches, L_rr_node, 
                                              rr_gsb,
                                              cb_ipin_side, inode);
    }
  }
}

/********************************************************************
 * Create bitstream for a X-direction or Y-direction Connection Blocks
 *******************************************************************/
static 
void build_connection_block_bitstreams(BitstreamManager& bitstream_manager,
                                       const ConfigBlockId& top_configurable_block,
                                       const ModuleManager& module_manager,
                                       const CircuitLibrary& circuit_lib,
                                       const MuxLibrary& mux_lib,
                                       const std::vector<t_switch_inf>& rr_switches,
                                       t_rr_node* L_rr_node,
                                       const DeviceRRGSB& L_device_rr_gsb,
                                       const t_rr_type& cb_type) {

  DeviceCoordinator cb_range = L_device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < cb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }
      /* Skip if the cb does not contain any configuration bits! */
      if (true == connection_block_contain_only_routing_tracks(rr_gsb, cb_type)) {
        continue;
      }
      /* Create a block for the bitstream which corresponds to the Switch block */
      vtr::Point<size_t> cb_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
      ConfigBlockId cb_configurable_block = bitstream_manager.add_block(generate_connection_block_module_name(cb_type, cb_coord));
      /* Set switch block as a child of top block */
      bitstream_manager.add_child_block(top_configurable_block, cb_configurable_block);
  
      build_connection_block_bitstream(bitstream_manager, cb_configurable_block, module_manager,  
                                       circuit_lib, mux_lib, rr_switches, L_rr_node,
                                       rr_gsb, cb_type);
    }
  }
}

/********************************************************************
 * Top-level function to create bitstream for global routing architecture
 * Two major tasks: 
 * 1. Generate bitstreams for Switch Blocks
 * 2. Generate bitstreams for both X-direction and Y-direction Connection Blocks
 *******************************************************************/
void build_routing_bitstream(BitstreamManager& bitstream_manager,
                             const ConfigBlockId& top_configurable_block,
                             const ModuleManager& module_manager,
                             const CircuitLibrary& circuit_lib,
                             const MuxLibrary& mux_lib,
                             const std::vector<t_switch_inf>& rr_switches,
                             t_rr_node* L_rr_node,
                             const DeviceRRGSB& L_device_rr_gsb) {

  /* Generate bitstream for each switch blocks
   * To organize the bitstream in blocks, we create a block for each switch block 
   * and give names which are same as they are in top-level module managers
   */
  vpr_printf(TIO_MESSAGE_INFO, 
             "Generating bitstream for Switch blocks...\n");
  DeviceCoordinator sb_range = L_device_rr_gsb.get_gsb_range();
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      /* Create a block for the bitstream which corresponds to the Switch block */
      vtr::Point<size_t> sb_coord(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
      ConfigBlockId sb_configurable_block = bitstream_manager.add_block(generate_switch_block_module_name(sb_coord));
      /* Set switch block as a child of top block */
      bitstream_manager.add_child_block(top_configurable_block, sb_configurable_block);

      build_switch_block_bitstream(bitstream_manager, sb_configurable_block, module_manager,  
                                   circuit_lib, mux_lib, rr_switches, L_rr_node,
                                   rr_gsb);
    }
  }

  /* Generate bitstream for each connection blocks
   * To organize the bitstream in blocks, we create a block for each connection block 
   * and give names which are same as they are in top-level module managers
   */
  vpr_printf(TIO_MESSAGE_INFO,"Generating bitstream for X-direction Connection blocks ...\n");

  build_connection_block_bitstreams(bitstream_manager, top_configurable_block, module_manager,  
                                    circuit_lib, mux_lib, rr_switches, L_rr_node,
                                    L_device_rr_gsb, CHANX);

  vpr_printf(TIO_MESSAGE_INFO,"Generating bitstream for Y-direction Connection blocks ...\n");

  build_connection_block_bitstreams(bitstream_manager, top_configurable_block, module_manager,  
                                    circuit_lib, mux_lib, rr_switches, L_rr_node,
                                    L_device_rr_gsb, CHANY);

}
