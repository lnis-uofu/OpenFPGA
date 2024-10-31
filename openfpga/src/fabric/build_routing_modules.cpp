/********************************************************************
 * This file includes functions that are used to build modules
 * for global routing architecture of a FPGA fabric
 * Covering:
 * 1. Connection blocks
 * 2. Switch blocks
 *******************************************************************/
#include <vector>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "build_memory_modules.h"
#include "build_module_graph_utils.h"
#include "build_routing_module_utils.h"
#include "build_routing_modules.h"
#include "module_manager_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "openfpga_rr_graph_utils.h"
#include "openfpga_side_manager.h"
#include "rr_gsb_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Generate a short interconneciton in switch box
 * There are two cases should be noticed.
 * 1. The actual fan-in of cur_rr_node is 0. In this case,
      the cur_rr_node need to be short connected to itself
      which is on the opposite side of this switch block
 * 2. The actual fan-in of cur_rr_node is 0. In this case,
 *    The cur_rr_node need to connected to the drive_rr_node
 ********************************************************************/
static void build_switch_block_module_short_interc(
  ModuleManager& module_manager, const ModuleId& sb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_side& chan_side,
  const RRNodeId& cur_rr_node, const RRNodeId& drive_rr_node,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets) {
  /* Find the name of output port */
  ModulePinInfo output_port_info = find_switch_block_module_chan_port(
    module_manager, sb_module, rr_graph, rr_gsb, chan_side, cur_rr_node,
    OUT_PORT);
  enum e_side input_pin_side = chan_side;
  int index = -1;

  /* Generate the input port object */
  switch (rr_graph.node_type(drive_rr_node)) {
    case OPIN: {
      rr_gsb.get_node_side_and_index(rr_graph, drive_rr_node, IN_PORT,
                                     input_pin_side, index);
      break;
    }
    case CHANX:
    case CHANY: {
      /* This should be an input in the data structure of RRGSB */
      if (cur_rr_node == drive_rr_node) {
        /* To be strict, the input should locate on the opposite side.
         * Use the else part if this may change in some architecture.
         */
        SideManager side_manager(chan_side);
        input_pin_side = side_manager.get_opposite();
      } else {
        /* The input could be at any side of the switch block, find it */
        rr_gsb.get_node_side_and_index(rr_graph, drive_rr_node, IN_PORT,
                                       input_pin_side, index);
      }
      break;
    }
    default: /* SOURCE, IPIN, SINK are invalid*/
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n");
      exit(1);
  }
  /* Find the name of input port */
  ModulePinInfo input_port_info = find_switch_block_module_input_port(
    module_manager, sb_module, grids, device_annotation, rr_graph, rr_gsb,
    input_pin_side, drive_rr_node);

  /* The input port and output port must match in size */
  BasicPort input_port =
    module_manager.module_port(sb_module, input_port_info.first);
  BasicPort output_port =
    module_manager.module_port(sb_module, output_port_info.first);

  /* Create a module net for this short-wire connection */
  ModuleNetId net = input_port_to_module_nets.at(input_port_info);
  /* Skip Configuring the net source, it is done before */
  /* Configure the net sink */
  module_manager.add_module_net_sink(sb_module, net, sb_module, 0,
                                     output_port_info.first,
                                     output_port_info.second);
}

/*********************************************************************
 * Build a instance of a routing multiplexer as well as
 * associated memory modules for a connection inside a switch block
 ********************************************************************/
static void build_switch_block_mux_module(
  ModuleManager& module_manager, const ModuleId& sb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const CircuitLibrary& circuit_lib, const e_side& chan_side,
  const size_t& chan_node_id, const RRNodeId& cur_rr_node,
  const std::vector<RRNodeId>& driver_rr_nodes, const RRSwitchId& switch_index,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets,
  const bool& group_config_block) {
  /* Check current rr_node is CHANX or CHANY*/
  VTR_ASSERT((CHANX == rr_graph.node_type(cur_rr_node)) ||
             (CHANY == rr_graph.node_type(cur_rr_node)));

  /* Get the circuit model id of the routing multiplexer */
  CircuitModelId mux_model =
    device_annotation.rr_switch_circuit_model(switch_index);

  /* Find the input size of the implementation of a routing multiplexer */
  size_t datapath_mux_size = driver_rr_nodes.size();

  /* Find the module name of the multiplexer and try to find it in the module
   * manager */
  std::string mux_module_name = generate_mux_subckt_name(
    circuit_lib, mux_model, datapath_mux_size, std::string(""));
  ModuleId mux_module = module_manager.find_module(mux_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mux_module));

  /* Get the MUX instance id from the module manager */
  size_t mux_instance_id = module_manager.num_instance(sb_module, mux_module);
  /* Instanciate the MUX Module */
  module_manager.add_child_module(sb_module, mux_module);

  /* Give an instance name: this name should be consistent with the block name
   * given in SDC manager, If you want to bind the SDC generation to modules
   */
  std::string mux_instance_name = generate_sb_memory_instance_name(
    SWITCH_BLOCK_MUX_INSTANCE_PREFIX, chan_side, chan_node_id, std::string(""));
  module_manager.set_child_instance_name(sb_module, mux_module, mux_instance_id,
                                         mux_instance_name);

  /* Generate input ports that are wired to the input bus of the routing
   * multiplexer */
  std::vector<ModulePinInfo> sb_input_port_ids =
    find_switch_block_module_input_ports(module_manager, sb_module, grids,
                                         device_annotation, rr_graph, rr_gsb,
                                         driver_rr_nodes);

  /* Link input bus port to Switch Block inputs */
  std::vector<CircuitPortId> mux_model_input_ports =
    circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == mux_model_input_ports.size());
  /* Find the module port id of the input port */
  ModulePortId mux_input_port_id = module_manager.find_module_port(
    mux_module, circuit_lib.port_prefix(mux_model_input_ports[0]));
  VTR_ASSERT(
    true == module_manager.valid_module_port_id(mux_module, mux_input_port_id));
  BasicPort mux_input_port =
    module_manager.module_port(mux_module, mux_input_port_id);

  /* Check port size should match */
  VTR_ASSERT(mux_input_port.get_width() == sb_input_port_ids.size());
  for (size_t pin_id = 0; pin_id < sb_input_port_ids.size(); ++pin_id) {
    /* Use the exising net */
    ModuleNetId net = input_port_to_module_nets.at(sb_input_port_ids[pin_id]);
    /* Configure the net source only if it is not yet in the source list */
    if (false ==
        module_manager.net_source_exist(sb_module, net, sb_module, 0,
                                        sb_input_port_ids[pin_id].first,
                                        sb_input_port_ids[pin_id].second)) {
      module_manager.add_module_net_source(sb_module, net, sb_module, 0,
                                           sb_input_port_ids[pin_id].first,
                                           sb_input_port_ids[pin_id].second);
    }
    /* Configure the net sink */
    module_manager.add_module_net_sink(sb_module, net, mux_module,
                                       mux_instance_id, mux_input_port_id,
                                       mux_input_port.pins()[pin_id]);
  }

  /* Link output port to Switch Block outputs */
  std::vector<CircuitPortId> mux_model_output_ports =
    circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == mux_model_output_ports.size());
  /* Use the port name convention in the circuit library */
  ModulePortId mux_output_port_id = module_manager.find_module_port(
    mux_module, circuit_lib.port_prefix(mux_model_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module,
                                                         mux_output_port_id));
  BasicPort mux_output_port =
    module_manager.module_port(mux_module, mux_output_port_id);
  ModulePinInfo sb_output_port_id = find_switch_block_module_chan_port(
    module_manager, sb_module, rr_graph, rr_gsb, chan_side, cur_rr_node,
    OUT_PORT);
  BasicPort sb_output_port =
    module_manager.module_port(sb_module, sb_output_port_id.first);

  /* Check port size should match */
  VTR_ASSERT(1 == mux_output_port.get_width());
  for (size_t pin_id = 0; pin_id < mux_output_port.pins().size(); ++pin_id) {
    /* Configuring the net source */
    ModuleNetId net = create_module_source_pin_net(
      module_manager, sb_module, mux_module, mux_instance_id,
      mux_output_port_id, mux_output_port.pins()[pin_id]);
    /* Configure the net sink */
    module_manager.add_module_net_sink(sb_module, net, sb_module, 0,
                                       sb_output_port_id.first,
                                       sb_output_port_id.second);
  }

  /* Instanciate memory modules */
  /* Find the name and module id of the memory module */
  std::string mem_module_name =
    generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                             std::string(MEMORY_MODULE_POSTFIX));
  if (group_config_block) {
    mem_module_name =
      generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                               std::string(MEMORY_FEEDTHROUGH_MODULE_POSTFIX));
  }
  ModuleId mem_module = module_manager.find_module(mem_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));

  size_t mem_instance_id = module_manager.num_instance(sb_module, mem_module);
  module_manager.add_child_module(sb_module, mem_module);
  /* Give an instance name: this name should be consistent with the block name
   * given in bitstream manager, If you want to bind the bitstream generation to
   * modules
   */
  std::string mem_instance_name = generate_sb_memory_instance_name(
    SWITCH_BLOCK_MEM_INSTANCE_PREFIX, chan_side, chan_node_id, std::string(""),
    group_config_block);
  module_manager.set_child_instance_name(sb_module, mem_module, mem_instance_id,
                                         mem_instance_name);

  /* Add nets to connect regular and mode-select SRAM ports to the SRAM port of
   * memory module */
  add_module_nets_between_logic_and_memory_sram_bus(
    module_manager, sb_module, mux_module, mux_instance_id, mem_module,
    mem_instance_id, circuit_lib, mux_model);
  /* Update memory and instance list */
  size_t config_child_id = module_manager.num_configurable_children(
    sb_module, ModuleManager::e_config_child_type::LOGICAL);
  module_manager.add_configurable_child(
    sb_module, mem_module, mem_instance_id,
    group_config_block ? ModuleManager::e_config_child_type::LOGICAL
                       : ModuleManager::e_config_child_type::UNIFIED);
  /* For logical memory, define the physical memory here */
  if (group_config_block) {
    std::string physical_mem_module_name =
      generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                               std::string(MEMORY_MODULE_POSTFIX));
    ModuleId physical_mem_module =
      module_manager.find_module(physical_mem_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(physical_mem_module));
    module_manager.set_logical2physical_configurable_child(
      sb_module, config_child_id, physical_mem_module);
    std::string physical_mem_instance_name = generate_sb_memory_instance_name(
      SWITCH_BLOCK_MEM_INSTANCE_PREFIX, chan_side, chan_node_id,
      std::string(""), false);
    module_manager.set_logical2physical_configurable_child_instance_name(
      sb_module, config_child_id, physical_mem_instance_name);
  }
}

/*********************************************************************
 * Generate child modules for a interconnection inside switch block
 * The interconnection could be either a wire or a routing multiplexer,
 * which depends on the fan-in of the rr_nodes in the switch block
 ********************************************************************/
static void build_switch_block_interc_modules(
  ModuleManager& module_manager, const ModuleId& sb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const CircuitLibrary& circuit_lib, const e_side& chan_side,
  const size_t& chan_node_id,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets,
  const bool& group_config_block) {
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

  if (0 == driver_rr_nodes.size()) {
    /* Print a special direct connection*/
    build_switch_block_module_short_interc(
      module_manager, sb_module, device_annotation, grids, rr_graph, rr_gsb,
      chan_side, cur_rr_node, cur_rr_node, input_port_to_module_nets);
  } else if (1 == driver_rr_nodes.size()) {
    /* Print a direct connection*/
    build_switch_block_module_short_interc(
      module_manager, sb_module, device_annotation, grids, rr_graph, rr_gsb,
      chan_side, cur_rr_node, driver_rr_nodes[0], input_port_to_module_nets);
  } else if (1 < driver_rr_nodes.size()) {
    /* Print the multiplexer, fan_in >= 2 */
    std::vector<RRSwitchId> driver_switches =
      get_rr_graph_driver_switches(rr_graph, cur_rr_node);
    VTR_ASSERT(1 == driver_switches.size());
    build_switch_block_mux_module(
      module_manager, sb_module, device_annotation, grids, rr_graph, rr_gsb,
      circuit_lib, chan_side, chan_node_id, cur_rr_node, driver_rr_nodes,
      driver_switches[0], input_port_to_module_nets, group_config_block);
  } /*Nothing should be done else*/
}

/********************************************************************
 * Build a module for a switch block whose detailed description is
 * available in a RRGSB object
 * A Switch Box module consists of following ports:
 * 1. Channel Y [x][y] inputs
 * 2. Channel X [x+1][y] inputs
 * 3. Channel Y [x][y-1] outputs
 * 4. Channel X [x][y] outputs
 * 5. Grid[x][y+1] Right side outputs pins
 * 6. Grid[x+1][y+1] Left side output pins
 * 7. Grid[x+1][y+1] Bottom side output pins
 * 8. Grid[x+1][y] Top side output pins
 * 9. Grid[x+1][y] Left side output pins
 * 10. Grid[x][y] Right side output pins
 * 11. Grid[x][y] Top side output pins
 * 12. Grid[x][y+1] Bottom side output pins
 *
 * Location of a Switch Box in FPGA fabric:
 *
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |  [x][y+1]  | [x][y+1] | [x+1][y+1] |
 *    |            |          |            |
 *    --------------          --------------
 *                  ----------
 *       ChanX      | Switch |     ChanX
 *       [x][y]     |   Box  |    [x+1][y]
 *                  | [x][y] |
 *                  ----------
 *    --------------          --------------
 *    |            |          |            |
 *    |    Grid    |  ChanY   |    Grid    |
 *    |   [x][y]   |  [x][y]  |  [x+1][y]  |
 *    |            |          |            |
 *    --------------          --------------
 *
 * Switch Block pin location map
 *
 *                       Grid[x][y+1]   ChanY[x][y+1]  Grid[x+1][y+1]
 *                        right_pins  inputs/outputs     left_pins
 *                            |             ^                |
 *                            |             |                |
 *                            v             v                v
 *                    +-----------------------------------------------+
 *                    |                                               |
 *    Grid[x][y+1]    |                                               |
 *Grid[x+1][y+1] bottom_pins---->| |<---- bottom_pins | | ChanX[x][y]        |
 *Switch Box [x][y]                |     ChanX[x+1][y] inputs/outputs<--->|
 *|<---> inputs/outputs |                                               |
 *    Grid[x][y+1]    |                                               |
 *Grid[x+1][y+1] top_pins---->| |<---- top_pins | |
 *                    +-----------------------------------------------+
 *                            ^             ^                ^
 *                            |             |                |
 *                            |             v                |
 *                       Grid[x][y]     ChanY[x][y]      Grid[x+1][y]
 *                       right_pins    inputs/outputs      left_pins
 *
 *
 ********************************************************************/
static void build_switch_block_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const DeviceRRGSB& device_rr_gsb,
  const RRGSB& rr_gsb, const bool& group_config_block, const bool& verbose) {
  /* Create a Module of Switch Block and add to module manager */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  ModuleId sb_module = module_manager.add_module(
    generate_switch_block_module_name(gsb_coordinate));

  /* Label module usage */
  module_manager.set_module_usage(sb_module, ModuleManager::MODULE_SB);

  VTR_LOGV(verbose, "Building module '%s'...",
           generate_switch_block_module_name(gsb_coordinate).c_str());

  /* Create a cache (fast look up) for module nets whose source are input ports
   */
  std::map<ModulePinInfo, ModuleNetId> input_port_to_module_nets;

  /* Add routing channel ports at each side of the GSB */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    SideManager side_manager(side);

    /* Count input and output port sizes */
    size_t chan_input_port_size = 0;
    size_t chan_output_port_size = 0;

    for (size_t itrack = 0;
         itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      switch (rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        case OUT_PORT:
          chan_output_port_size++;
          break;
        case IN_PORT:
          chan_input_port_size++;
          break;
        default:
          VTR_LOGF_ERROR(__FILE__, __LINE__,
                         "Invalid direction of chan[%d][%d]_track[%d]!\n",
                         rr_gsb.get_sb_x(), rr_gsb.get_sb_y(), itrack);
          exit(1);
      }
    }

    /* Do only when we have routing tracks */
    if (0 < rr_gsb.get_chan_width(side_manager.get_side())) {
      t_rr_type chan_type = rr_gsb.get_chan_type(side_manager.get_side());

      std::string chan_input_port_name = generate_sb_module_track_port_name(
        chan_type, side_manager.get_side(), IN_PORT);
      BasicPort chan_input_port(chan_input_port_name, chan_input_port_size);
      ModulePortId chan_input_port_id = module_manager.add_port(
        sb_module, chan_input_port, ModuleManager::MODULE_INPUT_PORT);
      /* Add side to the port */
      module_manager.set_port_side(sb_module, chan_input_port_id,
                                   side_manager.get_side());

      /* Cache the input net */
      for (const size_t& pin : chan_input_port.pins()) {
        ModuleNetId net = create_module_source_pin_net(
          module_manager, sb_module, sb_module, 0, chan_input_port_id, pin);
        input_port_to_module_nets[ModulePinInfo(chan_input_port_id, pin)] = net;
      }

      std::string chan_output_port_name = generate_sb_module_track_port_name(
        chan_type, side_manager.get_side(), OUT_PORT);
      BasicPort chan_output_port(chan_output_port_name, chan_output_port_size);
      ModulePortId chan_output_port_id = module_manager.add_port(
        sb_module, chan_output_port, ModuleManager::MODULE_OUTPUT_PORT);
      /* Add side to the port */
      module_manager.set_port_side(sb_module, chan_output_port_id,
                                   side_manager.get_side());
    }

    /* Dump OPINs of adjacent CLBs */
    for (size_t inode = 0;
         inode < rr_gsb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      vtr::Point<size_t> port_coord(rr_graph.node_xlow(rr_gsb.get_opin_node(
                                      side_manager.get_side(), inode)),
                                    rr_graph.node_ylow(rr_gsb.get_opin_node(
                                      side_manager.get_side(), inode)));
      std::string port_name = generate_sb_module_grid_port_name(
        side_manager.get_side(),
        get_rr_graph_single_node_side(
          rr_graph, rr_gsb.get_opin_node(side_manager.get_side(), inode)),
        grids, device_annotation, rr_graph,
        rr_gsb.get_opin_node(side_manager.get_side(), inode));
      BasicPort module_port(port_name,
                            1); /* Every grid output has a port size of 1 */
      /* Grid outputs are inputs of switch blocks */
      ModulePortId input_port_id = module_manager.add_port(
        sb_module, module_port, ModuleManager::MODULE_INPUT_PORT);
      /* Add side to the port */
      module_manager.set_port_side(sb_module, input_port_id,
                                   side_manager.get_side());

      /* Cache the input net */
      ModuleNetId net = create_module_source_pin_net(
        module_manager, sb_module, sb_module, 0, input_port_id, 0);
      input_port_to_module_nets[ModulePinInfo(input_port_id, 0)] = net;
    }
  }

  /* Add routing multiplexers as child modules */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    SideManager side_manager(side);
    for (size_t itrack = 0;
         itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      /* We care OUTPUT tracks at this time only */
      if (OUT_PORT ==
          rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        build_switch_block_interc_modules(
          module_manager, sb_module, device_annotation, grids, rr_graph, rr_gsb,
          circuit_lib, side_manager.get_side(), itrack,
          input_port_to_module_nets, group_config_block);
      }
    }
  }

  /* Build a physical memory block */
  if (group_config_block) {
    std::string mem_module_name_prefix =
      generate_switch_block_module_name_using_index(
        device_rr_gsb.get_sb_unique_module_index(gsb_coordinate));
    add_physical_memory_module(module_manager, decoder_lib, sb_module,
                               mem_module_name_prefix, circuit_lib,
                               sram_orgz_type, sram_model, verbose);
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, sb_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_shared_config_bits =
    find_module_num_shared_config_bits_from_child_modules(module_manager,
                                                          sb_module);
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, sb_module,
                                              module_num_shared_config_bits);
  }

  /* Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  ModuleManager::e_config_child_type config_child_type =
    group_config_block ? ModuleManager::e_config_child_type::PHYSICAL
                       : ModuleManager::e_config_child_type::LOGICAL;
  size_t module_num_config_bits =
    find_module_num_config_bits_from_child_modules(
      module_manager, sb_module, circuit_lib, sram_model, sram_orgz_type,
      config_child_type);
  if (0 < module_num_config_bits) {
    add_pb_sram_ports_to_module_manager(module_manager, sb_module, circuit_lib,
                                        sram_model, sram_orgz_type,
                                        module_num_config_bits);
  }

  /* Add all the nets to connect configuration ports from memory module to
   * primitive modules This is a one-shot addition that covers all the memory
   * modules in this primitive module!
   */
  if (0 <
      module_manager.num_configurable_children(sb_module, config_child_type)) {
    add_pb_module_nets_memory_config_bus(
      module_manager, decoder_lib, sb_module, sram_orgz_type,
      circuit_lib.design_tech_type(sram_model), config_child_type);
  }

  VTR_LOGV(verbose, "Done\n");
}

/*********************************************************************
 * Print a short interconneciton in connection
 ********************************************************************/
static void build_connection_block_module_short_interc(
  ModuleManager& module_manager, const ModuleId& cb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type,
  const e_side& cb_ipin_side, const size_t& ipin_index,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets) {
  /* Ensure we have only one 1 driver node */
  const RRNodeId& src_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, ipin_index);
  std::vector<RREdgeId> driver_rr_edges =
    rr_gsb.get_ipin_node_in_edges(rr_graph, cb_ipin_side, ipin_index);
  std::vector<RRNodeId> driver_rr_nodes;
  for (const RREdgeId curr_edge : driver_rr_edges) {
    driver_rr_nodes.push_back(rr_graph.edge_src_node(curr_edge));
  }

  /* We have OPINs since we may have direct connections:
   * These connections should be handled by other functions in the
   * compact_netlist.c So we just return here for OPINs
   */
  if (0 == driver_rr_nodes.size()) {
    return;
  }

  /* Find the driver node */
  VTR_ASSERT_SAFE(1 == driver_rr_nodes.size());
  const RRNodeId& driver_rr_node = driver_rr_nodes[0];

  /* Xifan Tang: VPR considers delayless switch to be configurable
   * As a result, the direct connection is considered to be configurable...
   * Here, I simply kick out OPINs in CB connection because they should be built
   * in the top mopdule.
   *
   * Note: this MUST BE reconsidered if we do have OPIN connected to IPINs
   * through a programmable multiplexer!!!
   */
  if (OPIN == rr_graph.node_type(driver_rr_node)) {
    return;
  }

  VTR_ASSERT((CHANX == rr_graph.node_type(driver_rr_node)) ||
             (CHANY == rr_graph.node_type(driver_rr_node)));

  /* Create port description for the routing track middle output */
  ModulePinInfo input_port_info = find_connection_block_module_chan_port(
    module_manager, cb_module, rr_graph, rr_gsb, cb_type, driver_rr_node);

  /* Create port description for input pin of a CLB */
  ModulePortId ipin_port_id = find_connection_block_module_ipin_port(
    module_manager, cb_module, grids, device_annotation, rr_graph, rr_gsb,
    src_rr_node);

  /* The input port and output port must match in size */
  BasicPort input_port =
    module_manager.module_port(cb_module, input_port_info.first);
  BasicPort ipin_port = module_manager.module_port(cb_module, ipin_port_id);
  VTR_ASSERT(1 == ipin_port.get_width());

  /* Create a module net for this short-wire connection */
  ModuleNetId net = input_port_to_module_nets.at(input_port_info);
  /* Skip Configuring the net source, it is done before */
  /* Configure the net sink */
  module_manager.add_module_net_sink(cb_module, net, cb_module, 0, ipin_port_id,
                                     ipin_port.pins()[0]);
}

/*********************************************************************
 * Build a instance of a routing multiplexer as well as
 * associated memory modules for a connection inside a connection block
 ********************************************************************/
static void build_connection_block_mux_module(
  ModuleManager& module_manager, const ModuleId& cb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type,
  const CircuitLibrary& circuit_lib, const e_side& cb_ipin_side,
  const size_t& ipin_index,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets,
  const bool& group_config_block) {
  const RRNodeId& cur_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, ipin_index);
  /* Check current rr_node is an input pin of a CLB */
  VTR_ASSERT(IPIN == rr_graph.node_type(cur_rr_node));

  /* Build a vector of driver rr_nodes */
  std::vector<RREdgeId> driver_rr_edges =
    rr_gsb.get_ipin_node_in_edges(rr_graph, cb_ipin_side, ipin_index);
  std::vector<RRNodeId> driver_rr_nodes;
  for (const RREdgeId curr_edge : driver_rr_edges) {
    driver_rr_nodes.push_back(rr_graph.edge_src_node(curr_edge));
  }

  std::vector<RRSwitchId> driver_switches =
    get_rr_graph_driver_switches(rr_graph, cur_rr_node);
  VTR_ASSERT(1 == driver_switches.size());

  /* Get the circuit model id of the routing multiplexer */
  CircuitModelId mux_model =
    device_annotation.rr_switch_circuit_model(driver_switches[0]);

  /* Find the input size of the implementation of a routing multiplexer */
  size_t datapath_mux_size = driver_rr_nodes.size();

  /* Find the module name of the multiplexer and try to find it in the module
   * manager */
  std::string mux_module_name = generate_mux_subckt_name(
    circuit_lib, mux_model, datapath_mux_size, std::string(""));
  ModuleId mux_module = module_manager.find_module(mux_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mux_module));

  /* Get the MUX instance id from the module manager */
  size_t mux_instance_id = module_manager.num_instance(cb_module, mux_module);
  module_manager.add_child_module(cb_module, mux_module);

  /* Give an instance name: this name should be consistent with the block name
   * given in SDC manager, If you want to bind the SDC generation to modules
   */
  std::string mux_instance_name = generate_cb_mux_instance_name(
    CONNECTION_BLOCK_MUX_INSTANCE_PREFIX,
    get_rr_graph_single_node_side(
      rr_graph, rr_gsb.get_ipin_node(cb_ipin_side, ipin_index)),
    ipin_index, std::string(""));
  module_manager.set_child_instance_name(cb_module, mux_module, mux_instance_id,
                                         mux_instance_name);

  /* TODO: Generate input ports that are wired to the input bus of the routing
   * multiplexer */
  std::vector<ModulePinInfo> cb_input_port_ids =
    find_connection_block_module_input_ports(module_manager, cb_module, grids,
                                             device_annotation, rr_graph,
                                             rr_gsb, cb_type, driver_rr_nodes);

  /* Link input bus port to Switch Block inputs */
  std::vector<CircuitPortId> mux_model_input_ports =
    circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == mux_model_input_ports.size());
  /* Find the module port id of the input port */
  ModulePortId mux_input_port_id = module_manager.find_module_port(
    mux_module, circuit_lib.port_prefix(mux_model_input_ports[0]));
  VTR_ASSERT(
    true == module_manager.valid_module_port_id(mux_module, mux_input_port_id));
  BasicPort mux_input_port =
    module_manager.module_port(mux_module, mux_input_port_id);

  /* Check port size should match */
  VTR_ASSERT(mux_input_port.get_width() == cb_input_port_ids.size());
  for (size_t pin_id = 0; pin_id < cb_input_port_ids.size(); ++pin_id) {
    /* Use the exising net */
    ModuleNetId net = input_port_to_module_nets.at(cb_input_port_ids[pin_id]);
    /* No need to configure the net source since it is already done before */
    /* Configure the net sink */
    module_manager.add_module_net_sink(cb_module, net, mux_module,
                                       mux_instance_id, mux_input_port_id,
                                       mux_input_port.pins()[pin_id]);
  }

  /* Link output port to Switch Block outputs */
  std::vector<CircuitPortId> mux_model_output_ports =
    circuit_lib.model_ports_by_type(mux_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == mux_model_output_ports.size());
  /* Use the port name convention in the circuit library */
  ModulePortId mux_output_port_id = module_manager.find_module_port(
    mux_module, circuit_lib.port_prefix(mux_model_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module,
                                                         mux_output_port_id));
  BasicPort mux_output_port =
    module_manager.module_port(mux_module, mux_output_port_id);
  ModulePortId cb_output_port_id = find_connection_block_module_ipin_port(
    module_manager, cb_module, grids, device_annotation, rr_graph, rr_gsb,
    cur_rr_node);
  BasicPort cb_output_port =
    module_manager.module_port(cb_module, cb_output_port_id);

  /* Check port size should match */
  VTR_ASSERT(cb_output_port.get_width() == mux_output_port.get_width());
  for (size_t pin_id = 0; pin_id < mux_output_port.pins().size(); ++pin_id) {
    /* Configuring the net source */
    ModuleNetId net = create_module_source_pin_net(
      module_manager, cb_module, mux_module, mux_instance_id,
      mux_output_port_id, mux_output_port.pins()[pin_id]);
    /* Configure the net sink */
    module_manager.add_module_net_sink(cb_module, net, cb_module, 0,
                                       cb_output_port_id,
                                       cb_output_port.pins()[pin_id]);
  }

  /* Instanciate memory modules */
  /* Find the name and module id of the memory module */
  std::string mem_module_name =
    generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                             std::string(MEMORY_MODULE_POSTFIX));
  if (group_config_block) {
    mem_module_name =
      generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                               std::string(MEMORY_FEEDTHROUGH_MODULE_POSTFIX));
  }
  ModuleId mem_module = module_manager.find_module(mem_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(mem_module));

  size_t mem_instance_id = module_manager.num_instance(cb_module, mem_module);
  module_manager.add_child_module(cb_module, mem_module);

  /* Give an instance name: this name should be consistent with the block name
   * given in bitstream manager, If you want to bind the bitstream generation to
   * modules
   */
  std::string mem_instance_name = generate_cb_memory_instance_name(
    CONNECTION_BLOCK_MEM_INSTANCE_PREFIX,
    get_rr_graph_single_node_side(
      rr_graph, rr_gsb.get_ipin_node(cb_ipin_side, ipin_index)),
    ipin_index, std::string(""), group_config_block);
  module_manager.set_child_instance_name(cb_module, mem_module, mem_instance_id,
                                         mem_instance_name);

  /* Add nets to connect regular and mode-select SRAM ports to the SRAM port of
   * memory module */
  add_module_nets_between_logic_and_memory_sram_bus(
    module_manager, cb_module, mux_module, mux_instance_id, mem_module,
    mem_instance_id, circuit_lib, mux_model);
  /* Update memory and instance list */
  size_t config_child_id = module_manager.num_configurable_children(
    cb_module, ModuleManager::e_config_child_type::LOGICAL);
  module_manager.add_configurable_child(
    cb_module, mem_module, mem_instance_id,
    group_config_block ? ModuleManager::e_config_child_type::LOGICAL
                       : ModuleManager::e_config_child_type::UNIFIED);
  /* For logical memory, define the physical memory here */
  if (group_config_block) {
    std::string physical_mem_module_name =
      generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size,
                               std::string(MEMORY_MODULE_POSTFIX));
    ModuleId physical_mem_module =
      module_manager.find_module(physical_mem_module_name);
    VTR_ASSERT(true == module_manager.valid_module_id(physical_mem_module));
    module_manager.set_logical2physical_configurable_child(
      cb_module, config_child_id, physical_mem_module);
    std::string physical_mem_instance_name = generate_cb_memory_instance_name(
      CONNECTION_BLOCK_MEM_INSTANCE_PREFIX,
      get_rr_graph_single_node_side(
        rr_graph, rr_gsb.get_ipin_node(cb_ipin_side, ipin_index)),
      ipin_index, std::string(""), false);
    module_manager.set_logical2physical_configurable_child_instance_name(
      cb_module, config_child_id, physical_mem_instance_name);
  }
}

/********************************************************************
 * Print internal connections of a connection block
 * For a IPIN node that is driven by only 1 fan-in,
 * a short wire will be created
 * For a IPIN node that is driven by more than two fan-ins,
 * a routing multiplexer will be instanciated
 ********************************************************************/
static void build_connection_block_interc_modules(
  ModuleManager& module_manager, const ModuleId& cb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type,
  const CircuitLibrary& circuit_lib, const e_side& cb_ipin_side,
  const size_t& ipin_index,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets,
  const bool& group_config_block) {
  std::vector<RREdgeId> driver_rr_edges =
    rr_gsb.get_ipin_node_in_edges(rr_graph, cb_ipin_side, ipin_index);

  if (1 > driver_rr_edges.size()) {
    return; /* This port has no driver, skip it */
  } else if (1 == driver_rr_edges.size()) {
    /* Print a direct connection */
    build_connection_block_module_short_interc(
      module_manager, cb_module, device_annotation, grids, rr_graph, rr_gsb,
      cb_type, cb_ipin_side, ipin_index, input_port_to_module_nets);

  } else if (1 < driver_rr_edges.size()) {
    /* Print the multiplexer, fan_in >= 2 */
    build_connection_block_mux_module(
      module_manager, cb_module, device_annotation, grids, rr_graph, rr_gsb,
      cb_type, circuit_lib, cb_ipin_side, ipin_index, input_port_to_module_nets,
      group_config_block);
  } /*Nothing should be done else*/
}

/********************************************************************
 * Generate a module of a connection Box (Type: [CHANX|CHANY])
 * Actually it is very similiar to switch box but
 * the difference is connection boxes connect Grid INPUT Pins to channels
 * NOTE: direct connection between CLBs should NOT be included inside this
 *       module! They should be added in the top-level module as their
 *connection is not limited to adjacent CLBs!!!
 *
 * Location of a X- and Y-direction Connection Block in FPGA fabric
 *               +------------+       +-------------+
 *               |            |------>|             |
 *               |     CLB    |<------| Y-direction |
 *               |            | ...   |  Connection |
 *               |            |------>|    Block    |
 *               +------------+       +-------------+
 *                 |  ^ ... |            | ^ ... |
 *                 v  |     v            v |     v
 *           +-------------------+    +-------------+
 *       --->|                   |--->|             |
 *       <---|     X-direction   |<---|    Switch   |
 *        ...|  Connection block |... |    Block    |
 *       --->|                   |--->|             |
 *           +-------------------+    +-------------+
 *
 *  Internal structure:
 *  This is an example of a X-direction connection block
 *  Note that middle output ports are shorted wire from inputs of routing
 *tracks, which are also the inputs of routing multiplexer of the connection
 *block
 *
 *                      CLB Input Pins
 *                         (IPINs)
 *                       ^   ^     ^
 *                       |   | ... |
 *              +--------------------------+
 *              |       ^    ^     ^       |
 *              |       |    | ... |       |
 *              |  +--------------------+  |
 *              |  |       routing      |  |
 *              |  |    multiplexers    |  |
 *              |  +--------------------+  |
 *              |      middle outputs      |
 *              |    of routing channel    |
 *              |    ^ ^ ^ ^     ^ ^ ^ ^   |
 *              |    | | | | ... | | | |   |
 *     in[0] -->|------------------------->|---> out[0]
 *    out[1] <--|<-------------------------|<--- in[1]
 *              |           ...            |
 *   in[W-2] -->|------------------------->|---> out[W-2]
 *  out[W-1] <--|<-------------------------|<--- in[W-1]
 *              +--------------------------+
 *
 *  W: routing channel width
 *
 ********************************************************************/
static void build_connection_block_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const DeviceRRGSB& device_rr_gsb,
  const RRGSB& rr_gsb, const t_rr_type& cb_type, const bool& group_config_block,
  const bool& verbose) {
  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type),
                                    rr_gsb.get_cb_y(cb_type));

  /* Create a Verilog Module based on the circuit model, and add to module
   * manager */
  ModuleId cb_module = module_manager.add_module(
    generate_connection_block_module_name(cb_type, gsb_coordinate));

  /* Label module usage */
  module_manager.set_module_usage(cb_module, ModuleManager::MODULE_CB);

  VTR_LOGV(
    verbose, "Building module '%s'...",
    generate_connection_block_module_name(cb_type, gsb_coordinate).c_str());

  /* Add the input and output ports of routing tracks in the channel
   * Routing tracks pass through the connection blocks
   */
  VTR_ASSERT(0 == rr_gsb.get_cb_chan_width(cb_type) % 2);

  /* Upper input port: W/2 == 0 tracks */
  std::string chan_upper_input_port_name =
    generate_cb_module_track_port_name(cb_type, IN_PORT, true);
  BasicPort chan_upper_input_port(chan_upper_input_port_name,
                                  rr_gsb.get_cb_chan_width(cb_type) / 2);
  ModulePortId chan_upper_input_port_id = module_manager.add_port(
    cb_module, chan_upper_input_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add side to the port */
  module_manager.set_port_side(cb_module, chan_upper_input_port_id,
                               get_cb_module_track_port_side(cb_type, true));

  /* Lower input port: W/2 == 1 tracks */
  std::string chan_lower_input_port_name =
    generate_cb_module_track_port_name(cb_type, IN_PORT, false);
  BasicPort chan_lower_input_port(chan_lower_input_port_name,
                                  rr_gsb.get_cb_chan_width(cb_type) / 2);
  ModulePortId chan_lower_input_port_id = module_manager.add_port(
    cb_module, chan_lower_input_port, ModuleManager::MODULE_INPUT_PORT);
  /* Add side to the port */
  module_manager.set_port_side(cb_module, chan_lower_input_port_id,
                               get_cb_module_track_port_side(cb_type, false));

  /* Upper output port: W/2 == 0 tracks */
  std::string chan_upper_output_port_name =
    generate_cb_module_track_port_name(cb_type, OUT_PORT, true);
  BasicPort chan_upper_output_port(chan_upper_output_port_name,
                                   rr_gsb.get_cb_chan_width(cb_type) / 2);
  ModulePortId chan_upper_output_port_id = module_manager.add_port(
    cb_module, chan_upper_output_port, ModuleManager::MODULE_OUTPUT_PORT);
  /* Add side to the port */
  module_manager.set_port_side(cb_module, chan_upper_output_port_id,
                               get_cb_module_track_port_side(cb_type, true));

  /* Lower output port: W/2 == 1 tracks */
  std::string chan_lower_output_port_name =
    generate_cb_module_track_port_name(cb_type, OUT_PORT, false);
  BasicPort chan_lower_output_port(chan_lower_output_port_name,
                                   rr_gsb.get_cb_chan_width(cb_type) / 2);
  ModulePortId chan_lower_output_port_id = module_manager.add_port(
    cb_module, chan_lower_output_port, ModuleManager::MODULE_OUTPUT_PORT);
  /* Add side to the port */
  module_manager.set_port_side(cb_module, chan_lower_output_port_id,
                               get_cb_module_track_port_side(cb_type, false));

  /* Add the input pins of grids, which are output ports of the connection block
   */
  std::vector<enum e_side> cb_ipin_sides = rr_gsb.get_cb_ipin_sides(cb_type);
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side);
         ++inode) {
      RRNodeId ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      std::string port_name = generate_cb_module_grid_port_name(
        cb_ipin_side, grids, device_annotation, rr_graph, ipin_node);
      BasicPort module_port(port_name,
                            1); /* Every grid output has a port size of 1 */
      /* Grid outputs are inputs of switch blocks */
      ModulePortId module_port_id = module_manager.add_port(
        cb_module, module_port, ModuleManager::MODULE_OUTPUT_PORT);
      /* Add side to the port */
      module_manager.set_port_side(cb_module, module_port_id, cb_ipin_side);
    }
  }

  /* Add the output pins of grids which are input ports of the connection block,
   * if there is any */
  std::vector<ModulePortId> opin_module_port_ids;
  std::vector<enum e_side> cb_opin_sides = rr_gsb.get_cb_opin_sides(cb_type);
  for (size_t iside = 0; iside < cb_opin_sides.size(); ++iside) {
    enum e_side cb_opin_side = cb_opin_sides[iside];
    for (size_t inode = 0;
         inode < rr_gsb.get_num_cb_opin_nodes(cb_type, cb_opin_side); ++inode) {
      RRNodeId opin_node =
        rr_gsb.get_cb_opin_node(cb_type, cb_opin_side, inode);
      std::string port_name = generate_cb_module_grid_port_name(
        cb_opin_side, grids, device_annotation, rr_graph, opin_node);
      BasicPort module_port(port_name,
                            1); /* Every grid output has a port size of 1 */
      /* Grid outputs are inputs of switch blocks */
      ModulePortId module_port_id = module_manager.add_port(
        cb_module, module_port, ModuleManager::MODULE_INPUT_PORT);
      /* Add side to the port */
      module_manager.set_port_side(cb_module, module_port_id, cb_opin_side);
      opin_module_port_ids.push_back(module_port_id);
    }
  }

  /* Create a cache (fast look up) for module nets whose source are input ports
   */
  std::map<ModulePinInfo, ModuleNetId> input_port_to_module_nets;

  /* Generate short-wire connection for each routing track :
   * Each input port is short-wired to its output port
   *
   *   upper_in[i] ----------> lower_out[i]
   *   lower_in[i] <---------- upper_out[i]
   */
  /* Create short-wires: input port ---> output port */
  VTR_ASSERT(chan_upper_input_port.get_width() ==
             chan_lower_output_port.get_width());
  for (size_t pin_id = 0; pin_id < chan_upper_input_port.pins().size();
       ++pin_id) {
    ModuleNetId net = create_module_source_pin_net(
      module_manager, cb_module, cb_module, 0, chan_upper_input_port_id,
      chan_upper_input_port.pins()[pin_id]);
    module_manager.add_module_net_sink(cb_module, net, cb_module, 0,
                                       chan_lower_output_port_id,
                                       chan_lower_output_port.pins()[pin_id]);
    /* Cache the module net */
    input_port_to_module_nets[ModulePinInfo(
      chan_upper_input_port_id, chan_upper_input_port.pins()[pin_id])] = net;
  }

  VTR_ASSERT(chan_lower_input_port.get_width() ==
             chan_upper_output_port.get_width());
  for (size_t pin_id = 0; pin_id < chan_lower_input_port.pins().size();
       ++pin_id) {
    ModuleNetId net = create_module_source_pin_net(
      module_manager, cb_module, cb_module, 0, chan_lower_input_port_id,
      chan_lower_input_port.pins()[pin_id]);
    module_manager.add_module_net_sink(cb_module, net, cb_module, 0,
                                       chan_upper_output_port_id,
                                       chan_upper_output_port.pins()[pin_id]);
    /* Cache the module net */
    input_port_to_module_nets[ModulePinInfo(
      chan_lower_input_port_id, chan_lower_input_port.pins()[pin_id])] = net;
  }

  for (ModulePortId opin_module_port_id : opin_module_port_ids) {
    ModuleNetId net = create_module_source_pin_net(
      module_manager, cb_module, cb_module, 0, opin_module_port_id, 0);
    /* Cache the module net */
    input_port_to_module_nets[ModulePinInfo(opin_module_port_id, 0)] = net;
  }

  /* Add sub modules of routing multiplexers or direct interconnect*/
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side);
         ++inode) {
      build_connection_block_interc_modules(
        module_manager, cb_module, device_annotation, grids, rr_graph, rr_gsb,
        cb_type, circuit_lib, cb_ipin_side, inode, input_port_to_module_nets,
        group_config_block);
    }
  }

  /* Build a physical memory block */
  if (group_config_block) {
    std::string mem_module_name_prefix =
      generate_connection_block_module_name_using_index(
        cb_type,
        device_rr_gsb.get_cb_unique_module_index(cb_type, gsb_coordinate));
    add_physical_memory_module(module_manager, decoder_lib, cb_module,
                               mem_module_name_prefix, circuit_lib,
                               sram_orgz_type, sram_model, verbose);
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, cb_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_shared_config_bits =
    find_module_num_shared_config_bits_from_child_modules(module_manager,
                                                          cb_module);
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, cb_module,
                                              module_num_shared_config_bits);
  }

  /* Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  ModuleManager::e_config_child_type config_child_type =
    group_config_block ? ModuleManager::e_config_child_type::PHYSICAL
                       : ModuleManager::e_config_child_type::LOGICAL;
  size_t module_num_config_bits =
    find_module_num_config_bits_from_child_modules(
      module_manager, cb_module, circuit_lib, sram_model, sram_orgz_type,
      config_child_type);
  if (0 < module_num_config_bits) {
    add_pb_sram_ports_to_module_manager(module_manager, cb_module, circuit_lib,
                                        sram_model, sram_orgz_type,
                                        module_num_config_bits);
  }

  /* Add all the nets to connect configuration ports from memory module to
   * primitive modules This is a one-shot addition that covers all the memory
   * modules in this primitive module!
   */
  if (0 <
      module_manager.num_configurable_children(cb_module, config_child_type)) {
    add_pb_module_nets_memory_config_bus(
      module_manager, decoder_lib, cb_module, sram_orgz_type,
      circuit_lib.design_tech_type(sram_model), config_child_type);
  }

  VTR_LOGV(verbose, "Done\n");
}

/********************************************************************
 * Iterate over all the connection blocks in a device
 * and build a module for each of them
 *******************************************************************/
static void build_flatten_connection_block_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const DeviceContext& device_ctx, const VprDeviceAnnotation& device_annotation,
  const DeviceRRGSB& device_rr_gsb, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const t_rr_type& cb_type,
  const bool& group_config_block, const bool& verbose) {
  /* Build unique X-direction connection block modules */
  vtr::Point<size_t> cb_range = device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < cb_range.x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1)
       * We will skip those modules
       */
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      if (false == rr_gsb.is_cb_exist(cb_type)) {
        continue;
      }
      build_connection_block_module(
        module_manager, decoder_lib, device_annotation, device_ctx.grid,
        device_ctx.rr_graph, circuit_lib, sram_orgz_type, sram_model,
        device_rr_gsb, rr_gsb, cb_type, group_config_block, verbose);
    }
  }
}

/********************************************************************
 * A top-level function of this file
 * Build all the modules for global routing architecture of a FPGA fabric
 * in a flatten way:
 *   Each connection block and switch block will be generated as a unique module
 * Covering:
 * 1. Connection blocks
 * 2. Switch blocks
 *******************************************************************/
void build_flatten_routing_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const DeviceContext& device_ctx, const VprDeviceAnnotation& device_annotation,
  const DeviceRRGSB& device_rr_gsb, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const bool& group_config_block,
  const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build routing modules...");

  vtr::Point<size_t> sb_range = device_rr_gsb.get_gsb_range();

  /* Build unique switch block modules */
  for (size_t ix = 0; ix < sb_range.x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.y(); ++iy) {
      const RRGSB& rr_gsb = device_rr_gsb.get_gsb(ix, iy);
      if (false == rr_gsb.is_sb_exist(device_ctx.rr_graph)) {
        continue;
      }
      build_switch_block_module(
        module_manager, decoder_lib, device_annotation, device_ctx.grid,
        device_ctx.rr_graph, circuit_lib, sram_orgz_type, sram_model,
        device_rr_gsb, rr_gsb, group_config_block, verbose);
    }
  }

  build_flatten_connection_block_modules(
    module_manager, decoder_lib, device_ctx, device_annotation, device_rr_gsb,
    circuit_lib, sram_orgz_type, sram_model, CHANX, group_config_block,
    verbose);

  build_flatten_connection_block_modules(
    module_manager, decoder_lib, device_ctx, device_annotation, device_rr_gsb,
    circuit_lib, sram_orgz_type, sram_model, CHANY, group_config_block,
    verbose);
}

/********************************************************************
 * A top-level function of this file
 * Build all the unique modules for global routing architecture of a FPGA fabric
 * This function will use unique module list built in device_rr_gsb,
 * to build only unique modules (in terms of graph connections) of
 * 1. Connection blocks
 * 2. Switch blocks
 *
 * Note: this function SHOULD be called only when
 * the option compact_routing_hierarchy is turned on!!!
 *******************************************************************/
void build_unique_routing_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const DeviceContext& device_ctx, const VprDeviceAnnotation& device_annotation,
  const DeviceRRGSB& device_rr_gsb, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const bool& group_config_block,
  const bool& verbose) {
  vtr::ScopedStartFinishTimer timer("Build unique routing modules...");

  /* Build unique switch block modules */
  for (size_t isb = 0; isb < device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_sb_unique_module(isb);
    build_switch_block_module(module_manager, decoder_lib, device_annotation,
                              device_ctx.grid, device_ctx.rr_graph, circuit_lib,
                              sram_orgz_type, sram_model, device_rr_gsb,
                              unique_mirror, group_config_block, verbose);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANX);
       ++icb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANX, icb);

    build_connection_block_module(
      module_manager, decoder_lib, device_annotation, device_ctx.grid,
      device_ctx.rr_graph, circuit_lib, sram_orgz_type, sram_model,
      device_rr_gsb, unique_mirror, CHANX, group_config_block, verbose);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < device_rr_gsb.get_num_cb_unique_module(CHANY);
       ++icb) {
    const RRGSB& unique_mirror = device_rr_gsb.get_cb_unique_module(CHANY, icb);

    build_connection_block_module(
      module_manager, decoder_lib, device_annotation, device_ctx.grid,
      device_ctx.rr_graph, circuit_lib, sram_orgz_type, sram_model,
      device_rr_gsb, unique_mirror, CHANY, group_config_block, verbose);
  }
}

} /* end namespace openfpga */
