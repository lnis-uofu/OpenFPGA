/********************************************************************
 * This file includes functions to build bitstream from global routing
 * architecture of a mapped FPGA fabric
 * We decode the bitstream from configuration of routing multiplexers
 * which locate in global routing architecture
 *******************************************************************/
#include <vector>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

/* Headers from openfpgautil library */
#include "build_mux_bitstream.h"
#include "build_routing_bitstream.h"
#include "module_manager_utils.h"
#include "mux_bitstream_constants.h"
#include "mux_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "openfpga_rr_graph_utils.h"
#include "openfpga_side_manager.h"
#include "rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function generates bitstream for a routing multiplexer
 * This function will identify if a node indicates a routing multiplexer
 * If not a routing multiplexer, no bitstream is needed here
 * If yes, we will generate the bitstream for the routing multiplexer
 *******************************************************************/
static void build_switch_block_mux_bitstream(
  BitstreamManager& bitstream_manager, const ConfigBlockId& mux_mem_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const RRGraphView& rr_graph,
  const RRNodeId& cur_rr_node, const std::vector<RRNodeId>& drive_rr_nodes,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation) {
  /* Check current rr_node is CHANX or CHANY*/
  VTR_ASSERT((CHANX == rr_graph.node_type(cur_rr_node)) ||
             (CHANY == rr_graph.node_type(cur_rr_node)));

  /* Find the input size of the implementation of a routing multiplexer */
  size_t datapath_mux_size = drive_rr_nodes.size();

  /* Cache input and output nets */
  std::vector<ClusterNetId> input_nets;
  ClusterNetId output_net = routing_annotation.rr_node_net(cur_rr_node);
  for (size_t inode = 0; inode < drive_rr_nodes.size(); ++inode) {
    input_nets.push_back(routing_annotation.rr_node_net(drive_rr_nodes[inode]));
  }
  VTR_ASSERT(input_nets.size() == drive_rr_nodes.size());

  /* Find out which routing path is used in this MUX
   * Two conditions to be considered:
   * - There is no net mapped to cur_rr_node: we use default path id
   * - There is a net mapped to cur_rr_node: we find the path id
   */
  int path_id = DEFAULT_PATH_ID;
  if (ClusterNetId::INVALID() != output_net) {
    /* We must have a valid previous node that is supposed to drive the source
     * node! */
    VTR_ASSERT(routing_annotation.rr_node_prev_node(cur_rr_node));
    for (size_t inode = 0; inode < drive_rr_nodes.size(); ++inode) {
      if ((input_nets[inode] == output_net) &&
          (drive_rr_nodes[inode] ==
           routing_annotation.rr_node_prev_node(cur_rr_node))) {
        path_id = (int)inode;
        break;
      }
    }
  }

  /* Ensure that our path id makes sense! */
  VTR_ASSERT(
    (DEFAULT_PATH_ID == path_id) ||
    ((DEFAULT_PATH_ID < path_id) && (path_id < (int)datapath_mux_size)));

  /* Find the circuit model id of the mux, we need its design technology which
   * matters the bitstream generation */
  std::vector<RRSwitchId> driver_switches =
    get_rr_graph_driver_switches(rr_graph, cur_rr_node);
  VTR_ASSERT(1 == driver_switches.size());
  CircuitModelId mux_model =
    device_annotation.rr_switch_circuit_model(driver_switches[0]);

  /* Generate bitstream depend on both technology and structure of this MUX */
  std::vector<bool> mux_bitstream = build_mux_bitstream(
    circuit_lib, mux_model, mux_lib, datapath_mux_size, path_id);

  /* Find the module in module manager and ensure the bitstream size matches! */
  std::string mem_module_name =
    generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                             std::string(MEMORY_MODULE_POSTFIX));
  ModuleId mux_mem_module = module_manager.find_module(mem_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mux_mem_module));
  ModulePortId mux_mem_out_port_id = module_manager.find_module_port(
    mux_mem_module, generate_configurable_memory_data_out_name());
  VTR_ASSERT(mux_bitstream.size() ==
             module_manager.module_port(mux_mem_module, mux_mem_out_port_id)
               .get_width());

  /* Add the bistream to the bitstream manager */
  bitstream_manager.add_block_bits(mux_mem_block, mux_bitstream);
  /* Record path ids, input and output nets */
  bitstream_manager.add_path_id_to_block(mux_mem_block, path_id);

  /* Add input nets */
  bool need_splitter = false;
  std::string input_net_ids;
  for (const ClusterNetId& input_net : input_nets) {
    /* Add a space as a splitter*/
    if (true == need_splitter) {
      input_net_ids += std::string(" ");
    }
    AtomNetId input_atom_net = atom_ctx.lookup.atom_net(input_net);
    if (true == atom_ctx.nlist.valid_net_id(input_atom_net)) {
      input_net_ids += atom_ctx.nlist.net_name(input_atom_net);
    } else {
      input_net_ids += std::string("unmapped");
    }
    need_splitter = true;
  }
  bitstream_manager.add_input_net_id_to_block(mux_mem_block, input_net_ids);

  /* Add output nets */
  std::string output_net_ids;
  AtomNetId output_atom_net = atom_ctx.lookup.atom_net(output_net);
  if (true == atom_ctx.nlist.valid_net_id(output_atom_net)) {
    output_net_ids += atom_ctx.nlist.net_name(output_atom_net);
  } else {
    output_net_ids += std::string("unmapped");
  }
  bitstream_manager.add_output_net_id_to_block(mux_mem_block, output_net_ids);
}

/********************************************************************
 * This function generates bitstream for an interconnection,
 * i.e., a routing multiplexer, in a Switch Block
 * This function will identify if a node indicates a routing multiplexer
 * If not a routing multiplexer, no bitstream is needed here
 * If yes, we will generate the bitstream for the routing multiplexer
 *******************************************************************/
static void build_switch_block_interc_bitstream(
  BitstreamManager& bitstream_manager,
  const ConfigBlockId& sb_configurable_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const RRGraphView& rr_graph,
  const AtomContext& atom_ctx, const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation, const RRGSB& rr_gsb,
  const e_side& chan_side, const size_t& chan_node_id) {
  std::vector<RRNodeId> driver_rr_nodes;

  /* Get the node */
  const RRNodeId& cur_rr_node = rr_gsb.get_chan_node(chan_side, chan_node_id);

  /* Determine if the interc lies inside a channel wire, that is interc between
   * segments */
  if (false ==
      rr_gsb.is_sb_node_passing_wire(rr_graph, chan_side, chan_node_id)) {
    driver_rr_nodes = get_rr_gsb_chan_node_configurable_driver_nodes(
      rr_graph, rr_gsb, chan_side, chan_node_id);
    /* Special: if there are zero-driver nodes. We skip here */
    if (0 == driver_rr_nodes.size()) {
      return;
    }
  }

  if ((0 == driver_rr_nodes.size()) || (0 == driver_rr_nodes.size())) {
    /* No bitstream generation required by a special direct connection*/
    return;
  } else if (1 < driver_rr_nodes.size()) {
    /* Create the block denoting the memory instances that drives this node in
     * Switch Block */
    std::string mem_block_name = generate_sb_memory_instance_name(
      SWITCH_BLOCK_MEM_INSTANCE_PREFIX, chan_side, chan_node_id,
      std::string(""));
    ConfigBlockId mux_mem_block = bitstream_manager.add_block(mem_block_name);
    bitstream_manager.add_child_block(sb_configurable_block, mux_mem_block);
    /* This is a routing multiplexer! Generate bitstream */
    build_switch_block_mux_bitstream(
      bitstream_manager, mux_mem_block, module_manager, circuit_lib, mux_lib,
      rr_graph, cur_rr_node, driver_rr_nodes, atom_ctx, device_annotation,
      routing_annotation);
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
 * Note that the output nodes typically spread over all the sides of a Switch
 *Block So, we will iterate over that.
 *******************************************************************/
static void build_switch_block_bitstream(
  BitstreamManager& bitstream_manager, const ConfigBlockId& sb_config_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb) {
  /* Iterate over all the multiplexers */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    for (size_t itrack = 0;
         itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      VTR_ASSERT((CHANX == rr_graph.node_type(rr_gsb.get_chan_node(
                             side_manager.get_side(), itrack))) ||
                 (CHANY == rr_graph.node_type(rr_gsb.get_chan_node(
                             side_manager.get_side(), itrack))));
      /* Only output port indicates a routing multiplexer */
      if (OUT_PORT !=
          rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        continue;
      }
      build_switch_block_interc_bitstream(
        bitstream_manager, sb_config_block, module_manager, circuit_lib,
        mux_lib, rr_graph, atom_ctx, device_annotation, routing_annotation,
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
static void build_connection_block_mux_bitstream(
  BitstreamManager& bitstream_manager, const ConfigBlockId& mux_mem_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const e_side& cb_ipin_side, const size_t& ipin_index) {
  RRNodeId src_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, ipin_index);
  /* Find drive_rr_nodes*/
  std::vector<RREdgeId> driver_rr_edges =
    rr_gsb.get_ipin_node_in_edges(rr_graph, cb_ipin_side, ipin_index);
  size_t datapath_mux_size = driver_rr_edges.size();

  /* Cache input and output nets */
  std::vector<ClusterNetId> input_nets;
  ClusterNetId output_net = routing_annotation.rr_node_net(src_rr_node);
  for (const RREdgeId& edge : driver_rr_edges) {
    RRNodeId driver_node = rr_graph.edge_src_node(edge);
    input_nets.push_back(routing_annotation.rr_node_net(driver_node));
  }

  /* Configuration bits for MUX*/
  int path_id = DEFAULT_PATH_ID;
  int edge_index = 0;

  /* Find which path is connected to the output of this routing multiplexer
   * Two conditions to be considered:
   * - There is no net mapped to src_rr_node: we use default path id
   * - There is a net mapped to src_rr_node: we find the path id
   */
  if (ClusterNetId::INVALID() != output_net) {
    for (const RREdgeId& edge : driver_rr_edges) {
      RRNodeId driver_node = rr_graph.edge_src_node(edge);
      /* We must have a valid previous node that is supposed to drive the source
       * node! */
      VTR_ASSERT(routing_annotation.rr_node_prev_node(src_rr_node));
      if ((routing_annotation.rr_node_net(driver_node) == output_net) &&
          (driver_node == routing_annotation.rr_node_prev_node(src_rr_node))) {
        path_id = edge_index;
        break;
      }
      edge_index++;
    }
  }

  /* Ensure that our path id makes sense! */
  VTR_ASSERT(
    (DEFAULT_PATH_ID == path_id) ||
    ((DEFAULT_PATH_ID < path_id) && (path_id < (int)datapath_mux_size)));

  /* Find the circuit model id of the mux, we need its design technology which
   * matters the bitstream generation */
  std::vector<RRSwitchId> driver_switches =
    get_rr_graph_driver_switches(rr_graph, src_rr_node);
  VTR_ASSERT(1 == driver_switches.size());
  CircuitModelId mux_model =
    device_annotation.rr_switch_circuit_model(driver_switches[0]);

  /* Generate bitstream depend on both technology and structure of this MUX */
  std::vector<bool> mux_bitstream = build_mux_bitstream(
    circuit_lib, mux_model, mux_lib, datapath_mux_size, path_id);

  /* Find the module in module manager and ensure the bitstream size matches! */
  std::string mem_module_name =
    generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                             std::string(MEMORY_MODULE_POSTFIX));
  ModuleId mux_mem_module = module_manager.find_module(mem_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mux_mem_module));
  ModulePortId mux_mem_out_port_id = module_manager.find_module_port(
    mux_mem_module, generate_configurable_memory_data_out_name());
  VTR_ASSERT(mux_bitstream.size() ==
             module_manager.module_port(mux_mem_module, mux_mem_out_port_id)
               .get_width());

  /* Add the bistream to the bitstream manager */
  bitstream_manager.add_block_bits(mux_mem_block, mux_bitstream);
  /* Record path ids, input and output nets */
  bitstream_manager.add_path_id_to_block(mux_mem_block, path_id);

  /* Add input nets */
  bool need_splitter = false;
  std::string input_net_ids;
  for (const ClusterNetId& input_net : input_nets) {
    /* Add a space as a splitter*/
    if (true == need_splitter) {
      input_net_ids += std::string(" ");
    }
    AtomNetId input_atom_net = atom_ctx.lookup.atom_net(input_net);
    if (true == atom_ctx.nlist.valid_net_id(input_atom_net)) {
      input_net_ids += atom_ctx.nlist.net_name(input_atom_net);
    } else {
      input_net_ids += std::string("unmapped");
    }
    need_splitter = true;
  }
  bitstream_manager.add_input_net_id_to_block(mux_mem_block, input_net_ids);

  /* Add output nets */
  std::string output_net_ids;
  AtomNetId output_atom_net = atom_ctx.lookup.atom_net(output_net);
  if (true == atom_ctx.nlist.valid_net_id(output_atom_net)) {
    output_net_ids += atom_ctx.nlist.net_name(output_atom_net);
  } else {
    output_net_ids += std::string("unmapped");
  }
  bitstream_manager.add_output_net_id_to_block(mux_mem_block, output_net_ids);
}

/********************************************************************
 * This function generates bitstream for an interconnection,
 * i.e., a routing multiplexer, in a Connection Block
 * This function will identify if a node indicates a routing multiplexer
 * If not a routing multiplexer, no bitstream is needed here
 * If yes, we will generate the bitstream for the routing multiplexer
 *******************************************************************/
static void build_connection_block_interc_bitstream(
  BitstreamManager& bitstream_manager,
  const ConfigBlockId& cb_configurable_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const e_side& cb_ipin_side, const size_t& ipin_index,
  const bool& verbose) {
  RRNodeId src_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, ipin_index);

  VTR_LOGV(verbose, "\tGenerating bitstream for IPIN '%lu'\n", ipin_index);

  /* Consider configurable edges only */
  std::vector<RREdgeId> driver_rr_edges =
    rr_gsb.get_ipin_node_in_edges(rr_graph, cb_ipin_side, ipin_index);
  std::vector<RRNodeId> driver_rr_nodes;
  for (const RREdgeId curr_edge : driver_rr_edges) {
    driver_rr_nodes.push_back(rr_graph.edge_src_node(curr_edge));
  }

  if (1 == driver_rr_nodes.size()) {
    /* No bitstream generation required by a special direct connection*/
  } else if (1 < driver_rr_nodes.size()) {
    /* Create the block denoting the memory instances that drives this node in
     * Switch Block */
    std::string mem_block_name = generate_cb_memory_instance_name(
      CONNECTION_BLOCK_MEM_INSTANCE_PREFIX,
      get_rr_graph_single_node_side(rr_graph, src_rr_node), ipin_index,
      std::string(""));
    ConfigBlockId mux_mem_block = bitstream_manager.add_block(mem_block_name);
    bitstream_manager.add_child_block(cb_configurable_block, mux_mem_block);
    /* This is a routing multiplexer! Generate bitstream */
    build_connection_block_mux_bitstream(
      bitstream_manager, mux_mem_block, module_manager, circuit_lib, mux_lib,
      atom_ctx, device_annotation, routing_annotation, rr_graph, rr_gsb,
      cb_ipin_side, ipin_index);
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
static void build_connection_block_bitstream(
  BitstreamManager& bitstream_manager,
  const ConfigBlockId& cb_configurable_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation, const RRGraphView& rr_graph,
  const RRGSB& rr_gsb, const t_rr_type& cb_type, const bool& verbose) {
  /* Find routing multiplexers on the sides of a Connection block where IPIN
   * nodes locate */
  std::vector<enum e_side> cb_sides = rr_gsb.get_cb_ipin_sides(cb_type);

  for (size_t side = 0; side < cb_sides.size(); ++side) {
    enum e_side cb_ipin_side = cb_sides[side];
    SideManager side_manager(cb_ipin_side);
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side);
         ++inode) {
      VTR_LOGV(verbose, "\tGenerating bitstream for IPIN at '%s' side\n",
               side_manager.to_string().c_str());
      build_connection_block_interc_bitstream(
        bitstream_manager, cb_configurable_block, module_manager, circuit_lib,
        mux_lib, atom_ctx, device_annotation, routing_annotation, rr_graph,
        rr_gsb, cb_ipin_side, inode, verbose);
    }
  }
}

/********************************************************************
 * Create bitstream for a X-direction or Y-direction Connection Blocks
 *******************************************************************/
static void build_connection_block_bitstreams(
  BitstreamManager& bitstream_manager,
  const ConfigBlockId& top_configurable_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const bool& compact_routing_hierarchy,
  const t_rr_type& cb_type, const bool& verbose) {
  vtr::Point<size_t> cb_range = device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < cb_range.x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1)
       * We will skip those modules
       */
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }
      /* Skip if the cb does not contain any configuration bits! */
      if (true ==
          connection_block_contain_only_routing_tracks(rr_gsb, cb_type)) {
        VTR_LOGV(verbose,
                 "\n\tSkipped %s Connection Block [%lu][%lu] as it contains "
                 "only routing tracks\n",
                 cb_type == CHANX ? "X-direction" : "Y-direction",
                 rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
        continue;
      }

      VTR_LOGV(verbose,
               "\n\tGenerating bitstream for %s Connection Block [%lu][%lu]\n",
               cb_type == CHANX ? "X-direction" : "Y-direction",
               rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));

      /* Find the cb module so that we can precisely reserve child blocks */
      vtr::Point<size_t> cb_coord(rr_gsb.get_cb_x(cb_type),
                                  rr_gsb.get_cb_y(cb_type));
      std::string cb_module_name =
        generate_connection_block_module_name(cb_type, cb_coord);
      if (true == compact_routing_hierarchy) {
        vtr::Point<size_t> unique_cb_coord(ix, iy);
        /* Note: use GSB coordinate when inquire for unique modules!!! */
        const RRGSB& unique_mirror =
          device_rr_gsb.get_cb_unique_module(cb_type, unique_cb_coord);
        unique_cb_coord.set_x(unique_mirror.get_cb_x(cb_type));
        unique_cb_coord.set_y(unique_mirror.get_cb_y(cb_type));
        cb_module_name =
          generate_connection_block_module_name(cb_type, unique_cb_coord);
      }
      ModuleId cb_module = module_manager.find_module(cb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(cb_module));

      /* Bypass empty blocks which have none configurable children */
      if (0 == count_module_manager_module_configurable_children(module_manager,
                                                                 cb_module)) {
        continue;
      }

      /* Create a block for the bitstream which corresponds to the Switch block
       */
      ConfigBlockId cb_configurable_block = bitstream_manager.add_block(
        generate_connection_block_module_name(cb_type, cb_coord));
      /* Set switch block as a child of top block */
      bitstream_manager.add_child_block(top_configurable_block,
                                        cb_configurable_block);

      /* Reserve child blocks for new created block */
      bitstream_manager.reserve_child_blocks(
        cb_configurable_block,
        count_module_manager_module_configurable_children(module_manager,
                                                          cb_module));

      build_connection_block_bitstream(
        bitstream_manager, cb_configurable_block, module_manager, circuit_lib,
        mux_lib, atom_ctx, device_annotation, routing_annotation, rr_graph,
        rr_gsb, cb_type, verbose);

      VTR_LOGV(verbose, "\tDone\n");
    }
  }
}

/********************************************************************
 * Top-level function to create bitstream for global routing architecture
 * Two major tasks:
 * 1. Generate bitstreams for Switch Blocks
 * 2. Generate bitstreams for both X-direction and Y-direction Connection Blocks
 *******************************************************************/
void build_routing_bitstream(
  BitstreamManager& bitstream_manager,
  const ConfigBlockId& top_configurable_block,
  const ModuleManager& module_manager, const CircuitLibrary& circuit_lib,
  const MuxLibrary& mux_lib, const AtomContext& atom_ctx,
  const VprDeviceAnnotation& device_annotation,
  const VprRoutingAnnotation& routing_annotation, const RRGraphView& rr_graph,
  const DeviceRRGSB& device_rr_gsb, const bool& compact_routing_hierarchy,
  const bool& verbose) {
  /* Generate bitstream for each switch blocks
   * To organize the bitstream in blocks, we create a block for each switch
   * block and give names which are same as they are in top-level module
   * managers
   */
  VTR_LOG("Generating bitstream for Switch blocks...");
  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();
  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      /* Check if the switch block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (width > 1)
       * We will skip those modules
       */
      if (false == rr_gsb.is_sb_exist()) {
        continue;
      }

      VTR_LOGV(verbose,
               "\n\tGenerating bitstream for Switch blocks[%lu][%lu]...\n", ix,
               iy);

      vtr::Point<size_t> sb_coord(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());

      /* Find the sb module so that we can precisely reserve child blocks */
      std::string sb_module_name = generate_switch_block_module_name(sb_coord);
      if (true == compact_routing_hierarchy) {
        vtr::Point<size_t> unique_sb_coord(ix, iy);
        const RRGSB& unique_mirror =
          device_rr_gsb.get_sb_unique_module(sb_coord);
        unique_sb_coord.set_x(unique_mirror.get_sb_x());
        unique_sb_coord.set_y(unique_mirror.get_sb_y());
        sb_module_name = generate_switch_block_module_name(unique_sb_coord);
      }
      ModuleId sb_module = module_manager.find_module(sb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(sb_module));

      /* Bypass empty blocks which have none configurable children */
      if (0 == count_module_manager_module_configurable_children(module_manager,
                                                                 sb_module)) {
        continue;
      }

      /* Create a block for the bitstream which corresponds to the Switch block
       */
      ConfigBlockId sb_configurable_block = bitstream_manager.add_block(
        generate_switch_block_module_name(sb_coord));
      /* Set switch block as a child of top block */
      bitstream_manager.add_child_block(top_configurable_block,
                                        sb_configurable_block);

      /* Reserve child blocks for new created block */
      bitstream_manager.reserve_child_blocks(
        sb_configurable_block,
        count_module_manager_module_configurable_children(module_manager,
                                                          sb_module));

      build_switch_block_bitstream(bitstream_manager, sb_configurable_block,
                                   module_manager, circuit_lib, mux_lib,
                                   atom_ctx, device_annotation,
                                   routing_annotation, rr_graph, rr_gsb);

      VTR_LOGV(verbose, "\tDone\n");
    }
  }
  VTR_LOG("Done\n");

  /* Generate bitstream for each connection blocks
   * To organize the bitstream in blocks, we create a block for each connection
   * block and give names which are same as they are in top-level module
   * managers
   */
  VTR_LOG("Generating bitstream for X-direction Connection blocks ...");

  build_connection_block_bitstreams(
    bitstream_manager, top_configurable_block, module_manager, circuit_lib,
    mux_lib, atom_ctx, device_annotation, routing_annotation, rr_graph,
    device_rr_gsb, compact_routing_hierarchy, CHANX, verbose);
  VTR_LOG("Done\n");

  VTR_LOG("Generating bitstream for Y-direction Connection blocks ...");

  build_connection_block_bitstreams(
    bitstream_manager, top_configurable_block, module_manager, circuit_lib,
    mux_lib, atom_ctx, device_annotation, routing_annotation, rr_graph,
    device_rr_gsb, compact_routing_hierarchy, CHANY, verbose);
  VTR_LOG("Done\n");
}

} /* end namespace openfpga */
