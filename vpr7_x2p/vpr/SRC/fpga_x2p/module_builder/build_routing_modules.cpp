/********************************************************************
 * This file includes functions that are used to build modules 
 * for global routing architecture of a FPGA fabric
 * Covering:
 * 1. Connection blocks
 * 2. Switch blocks
 *******************************************************************/
#include <ctime>
#include <vector>

#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "sides.h"
#include "util.h"
#include "device_coordinator.h"

#include "fpga_x2p_reserved_words.h"
#include "fpga_x2p_types.h"
#include "fpga_x2p_naming.h"

#include "fpga_x2p_utils.h" 
#include "module_manager_utils.h"
#include "build_module_graph_utils.h"

#include "build_routing_modules.h"
#include "verilog_global.h"

/*********************************************************************
 * Generate a port for a routing track of a swtich block
 ********************************************************************/
static 
ModulePortId find_switch_block_module_chan_port(const ModuleManager& module_manager, 
                                                const ModuleId& sb_module,
                                                const RRGSB& rr_gsb, 
                                                const e_side& chan_side,
                                                t_rr_node* cur_rr_node,
                                                const PORTS& cur_rr_node_direction) {
  /* Get the index in sb_info of cur_rr_node */
  int index = rr_gsb.get_node_index(cur_rr_node, chan_side, cur_rr_node_direction);
  /* Make sure this node is included in this sb_info */
  VTR_ASSERT((-1 != index)&&(NUM_SIDES != chan_side));

  DeviceCoordinator chan_rr_node_coordinator = rr_gsb.get_side_block_coordinator(chan_side);

  vtr::Point<size_t> chan_port_coord(chan_rr_node_coordinator.get_x(), chan_rr_node_coordinator.get_y());
  std::string chan_port_name = generate_routing_track_port_name(rr_gsb.get_chan_node(chan_side, index)->type,
                                                                chan_port_coord, index,  
                                                                rr_gsb.get_chan_node_direction(chan_side, index));
  
  /* Must find a valid port id in the Switch Block module */
  ModulePortId chan_port_id = module_manager.find_module_port(sb_module, chan_port_name); 
  VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module, chan_port_id));
  return chan_port_id; 
}

/*********************************************************************
 * Generate an input port for routing multiplexer inside the switch block
 * In addition to give the Routing Resource node of the input
 * Users should provide the side of input, which is different case by case:
 * 1. When the input is a pin of a CLB/Logic Block, the input_side should
 *    be the side of the node on its grid!
 *    For example, the input pin is on the top side of a switch block
 *    but on the right side of a switch block
 *                      +--------+
 *                      |        |
 *                      |  Grid  |---+
 *                      |        |   |
 *                      +--------+   v input_pin
 *                      +----------------+
 *                      |  Switch Block  |
 *                      +----------------+
 * 2. When the input is a routing track, the input_side should be
 *    the side of the node locating on the switch block
 ********************************************************************/
static 
ModulePortId find_switch_block_module_input_port(const ModuleManager& module_manager,
                                                 const ModuleId& sb_module, 
                                                 const RRGSB& rr_gsb, 
                                                 const std::vector<std::vector<t_grid_tile>>& grids,
                                                 const e_side& input_side,
                                                 t_rr_node* input_rr_node) {
  /* Deposit an invalid value */
  ModulePortId input_port_id = ModulePortId::INVALID();
  /* Generate the input port object */
  switch (input_rr_node->type) {
  /* case SOURCE: */
  case OPIN: {
    /* Find the coordinator (grid_x and grid_y) for the input port */
    vtr::Point<size_t> input_port_coord(input_rr_node->xlow, input_rr_node->ylow);
    std::string input_port_name = generate_grid_side_port_name(grids,
                                                               input_port_coord,
                                                               input_side,
                                                               input_rr_node->ptc_num); 
    /* Must find a valid port id in the Switch Block module */
    input_port_id = module_manager.find_module_port(sb_module, input_port_name); 
    VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module, input_port_id));
    break;
  }
  case CHANX:
  case CHANY: {
    input_port_id = find_switch_block_module_chan_port(module_manager, sb_module, 
                                                       rr_gsb, input_side, input_rr_node, IN_PORT);
    break;
  }
  default: /* SOURCE, IPIN, SINK are invalid*/
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return input_port_id; 
}

/*********************************************************************
 * Generate a list of input ports for routing multiplexer inside the switch block
 ********************************************************************/
static 
std::vector<ModulePortId> find_switch_block_module_input_ports(const ModuleManager& module_manager,
                                                               const ModuleId& sb_module, 
                                                               const RRGSB& rr_gsb, 
                                                               const std::vector<std::vector<t_grid_tile>>& grids,
                                                               const std::vector<t_rr_node*>& input_rr_nodes) {
  std::vector<ModulePortId> input_ports;

  for (auto input_rr_node : input_rr_nodes) {
    enum e_side input_pin_side = NUM_SIDES;
    switch (input_rr_node->type) {
    case OPIN: 
      input_pin_side = rr_gsb.get_opin_node_grid_side(input_rr_node);
      break;
    case CHANX:
    case CHANY: {
      /* The input could be at any side of the switch block, find it */
      int index = -1;
      rr_gsb.get_node_side_and_index(input_rr_node, IN_PORT, &input_pin_side, &index);
      VTR_ASSERT(NUM_SIDES != input_pin_side);
      break;
    }
    default: /* SOURCE, IPIN, SINK are invalid*/
      vpr_printf(TIO_MESSAGE_ERROR, 
                 "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
                 __FILE__, __LINE__);
      exit(1);
    }
    input_ports.push_back(find_switch_block_module_input_port(module_manager, sb_module, rr_gsb, grids, input_pin_side, input_rr_node));
  }

  return input_ports;
}


/*********************************************************************
 * Generate a short interconneciton in switch box
 * There are two cases should be noticed.
 * 1. The actual fan-in of cur_rr_node is 0. In this case,
      the cur_rr_node need to be short connected to itself 
      which is on the opposite side of this switch block
 * 2. The actual fan-in of cur_rr_node is 0. In this case,
 *    The cur_rr_node need to connected to the drive_rr_node
 ********************************************************************/
static 
void build_switch_block_module_short_interc(ModuleManager& module_manager, 
                                            const ModuleId& sb_module,
                                            const RRGSB& rr_gsb,
                                            const e_side& chan_side,
                                            t_rr_node* cur_rr_node,
                                            t_rr_node* drive_rr_node,
                                            const std::vector<std::vector<t_grid_tile>>& grids,
                                            const std::map<ModulePortId, ModuleNetId>& input_port_to_module_nets) {
  /* Find the name of output port */
  ModulePortId output_port_id = find_switch_block_module_chan_port(module_manager, sb_module, rr_gsb, chan_side, cur_rr_node, OUT_PORT);
  enum e_side input_pin_side = chan_side;

  /* Generate the input port object */
  switch (drive_rr_node->type) {
  case OPIN: 
    input_pin_side = rr_gsb.get_opin_node_grid_side(drive_rr_node);
    break;
  case CHANX:
  case CHANY: {
    /* This should be an input in the data structure of RRGSB */
    if (cur_rr_node == drive_rr_node) {
      /* To be strict, the input should locate on the opposite side. 
       * Use the else part if this may change in some architecture.
       */
      Side side_manager(chan_side);
      input_pin_side = side_manager.get_opposite(); 
    } else {
      /* The input could be at any side of the switch block, find it */
      int index = -1;
      rr_gsb.get_node_side_and_index(drive_rr_node, IN_PORT, &input_pin_side, &index);
    }
    break;
  }
  default: /* SOURCE, IPIN, SINK are invalid*/
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
               __FILE__, __LINE__);
    exit(1);
  }
  /* Find the name of input port */
  ModulePortId input_port_id = find_switch_block_module_input_port(module_manager, sb_module, rr_gsb, grids, input_pin_side, drive_rr_node);

  /* The input port and output port must match in size */
  BasicPort input_port = module_manager.module_port(sb_module, input_port_id);
  BasicPort output_port = module_manager.module_port(sb_module, output_port_id);
  VTR_ASSERT(input_port.get_width() == output_port.get_width());
  
  /* Create a module net for this short-wire connection */
  for (size_t pin_id = 0; pin_id < input_port.pins().size(); ++pin_id) {
    ModuleNetId net = input_port_to_module_nets.at(input_port_id);
    /* Skip Configuring the net source, it is done before */
    /* Configure the net sink */
    module_manager.add_module_net_sink(sb_module, net, sb_module, 0, output_port_id, output_port.pins()[pin_id]);
  }
}

/*********************************************************************
 * Build a instance of a routing multiplexer as well as
 * associated memory modules for a connection inside a switch block
 ********************************************************************/
static  
void build_switch_block_mux_module(ModuleManager& module_manager, 
                                   const ModuleId& sb_module, 
                                   const RRGSB& rr_gsb, 
                                   const CircuitLibrary& circuit_lib,
                                   const std::vector<std::vector<t_grid_tile>>& grids,
                                   const std::vector<t_switch_inf>& rr_switches,
                                   const e_side& chan_side,
                                   const size_t& chan_node_id,
                                   t_rr_node* cur_rr_node,
                                   const std::vector<t_rr_node*>& drive_rr_nodes,
                                   const size_t& switch_index,
                                   const std::map<ModulePortId, ModuleNetId>& input_port_to_module_nets) {
  /* Check current rr_node is CHANX or CHANY*/
  VTR_ASSERT((CHANX == cur_rr_node->type)||(CHANY == cur_rr_node->type));

  /* Get the circuit model id of the routing multiplexer */
  CircuitModelId mux_model = rr_switches[switch_index].circuit_model;

  /* Find the input size of the implementation of a routing multiplexer */
  size_t datapath_mux_size = drive_rr_nodes.size();

  /* Find the module name of the multiplexer and try to find it in the module manager */
  std::string mux_module_name = generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size, std::string(""));
  ModuleId mux_module = module_manager.find_module(mux_module_name);
  VTR_ASSERT (true == module_manager.valid_module_id(mux_module));

  /* Get the MUX instance id from the module manager */
  size_t mux_instance_id = module_manager.num_instance(sb_module, mux_module);
  /* Instanciate the MUX Module */
  module_manager.add_child_module(sb_module, mux_module);

  /* Generate input ports that are wired to the input bus of the routing multiplexer */
  std::vector<ModulePortId> sb_input_port_ids = find_switch_block_module_input_ports(module_manager, sb_module, rr_gsb, grids, drive_rr_nodes);

  /* Link input bus port to Switch Block inputs */
  std::vector<CircuitPortId> mux_model_input_ports = circuit_lib.model_ports_by_type(mux_model, SPICE_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == mux_model_input_ports.size());
  /* Find the module port id of the input port */
  ModulePortId mux_input_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_lib_name(mux_model_input_ports[0])); 
  VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module, mux_input_port_id));
  BasicPort mux_input_port = module_manager.module_port(mux_module, mux_input_port_id);

  /* Check port size should match */
  VTR_ASSERT(mux_input_port.get_width() == sb_input_port_ids.size());
  for (size_t pin_id = 0; pin_id < sb_input_port_ids.size(); ++pin_id) {
    /* Use the exising net */
    ModuleNetId net = input_port_to_module_nets.at(sb_input_port_ids[pin_id]);
    /* Configure the net source */
    module_manager.add_module_net_source(sb_module, net, sb_module, 0, sb_input_port_ids[pin_id], 0);
    /* Configure the net sink */
    module_manager.add_module_net_sink(sb_module, net, mux_module, mux_instance_id, mux_input_port_id, mux_input_port.pins()[pin_id]);
  }

  /* Link output port to Switch Block outputs */
  std::vector<CircuitPortId> mux_model_output_ports = circuit_lib.model_ports_by_type(mux_model, SPICE_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == mux_model_output_ports.size());
  /* Use the port name convention in the circuit library */
  ModulePortId mux_output_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_lib_name(mux_model_output_ports[0])); 
  VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module, mux_output_port_id));
  BasicPort mux_output_port = module_manager.module_port(mux_module, mux_output_port_id);
  ModulePortId sb_output_port_id = find_switch_block_module_chan_port(module_manager, sb_module, rr_gsb, chan_side, cur_rr_node, OUT_PORT); 
  BasicPort sb_output_port = module_manager.module_port(sb_module, sb_output_port_id);

  /* Check port size should match */
  VTR_ASSERT(sb_output_port.get_width() == mux_output_port.get_width());
  for (size_t pin_id = 0; pin_id < mux_output_port.pins().size(); ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(sb_module);
    /* Configuring the net source */
    module_manager.add_module_net_source(sb_module, net, mux_module, mux_instance_id, mux_output_port_id, mux_output_port.pins()[pin_id]);
    /* Configure the net sink */
    module_manager.add_module_net_sink(sb_module, net, sb_module, 0, sb_output_port_id, sb_output_port.pins()[pin_id]);
  }

  /* Instanciate memory modules */
  /* Find the name and module id of the memory module */
  std::string mem_module_name = generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size, std::string(MEMORY_MODULE_POSTFIX)); 
  ModuleId mem_module = module_manager.find_module(mem_module_name);
  VTR_ASSERT (true == module_manager.valid_module_id(mem_module));

  size_t mem_instance_id = module_manager.num_instance(sb_module, mem_module);
  module_manager.add_child_module(sb_module, mem_module);
  /* Give an instance name: this name should be consistent with the block name given in bitstream manager,
   * If you want to bind the bitstream generation to modules
   */
  std::string mem_instance_name = generate_sb_memory_instance_name(SWITCH_BLOCK_MEM_INSTANCE_PREFIX, chan_side, chan_node_id, std::string(""));
  module_manager.set_child_instance_name(sb_module, mem_module, mem_instance_id, mem_instance_name);

  /* Add nets to connect regular and mode-select SRAM ports to the SRAM port of memory module */
  add_module_nets_between_logic_and_memory_sram_bus(module_manager, sb_module, 
                                                    mux_module, mux_instance_id,
                                                    mem_module, mem_instance_id,
                                                    circuit_lib, mux_model);
  /* Update memory and instance list */
  module_manager.add_configurable_child(sb_module, mem_module, mem_instance_id);
}

/*********************************************************************
 * Generate child modules for a interconnection inside switch block
 * The interconnection could be either a wire or a routing multiplexer,
 * which depends on the fan-in of the rr_nodes in the switch block
 ********************************************************************/
static  
void build_switch_block_interc_modules(ModuleManager& module_manager,
                                       const ModuleId& sb_module, 
                                       const RRGSB& rr_gsb,
                                       const CircuitLibrary& circuit_lib,
                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                       const std::vector<t_switch_inf>& rr_switches,
                                       const e_side& chan_side,
                                       const size_t& chan_node_id,
                                       const std::map<ModulePortId, ModuleNetId>& input_port_to_module_nets) {
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

  if (0 == drive_rr_nodes.size()) {
    /* Print a special direct connection*/
    build_switch_block_module_short_interc(module_manager, sb_module,
                                           rr_gsb, chan_side, cur_rr_node, 
                                           cur_rr_node, grids,  
                                           input_port_to_module_nets);
  } else if (1 == drive_rr_nodes.size()) {
    /* Print a direct connection*/
    build_switch_block_module_short_interc(module_manager, sb_module,
                                           rr_gsb, chan_side, cur_rr_node, 
                                           drive_rr_nodes[DEFAULT_SWITCH_ID],
                                           grids,
                                           input_port_to_module_nets);
  } else if (1 < drive_rr_nodes.size()) {
    /* Print the multiplexer, fan_in >= 2 */
    build_switch_block_mux_module(module_manager, 
                                  sb_module, rr_gsb, circuit_lib, 
                                  grids, rr_switches, chan_side, chan_node_id, cur_rr_node,  
                                  drive_rr_nodes, 
                                  cur_rr_node->drive_switches[DEFAULT_SWITCH_ID],
                                  input_port_to_module_nets);
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
 *    Grid[x][y+1]    |                                               |    Grid[x+1][y+1]
 *    bottom_pins---->|                                               |<---- bottom_pins
 *                    |                                               |
 * ChanX[x][y]        |              Switch Box [x][y]                |     ChanX[x+1][y]
 * inputs/outputs<--->|                                               |<---> inputs/outputs
 *                    |                                               |
 *    Grid[x][y+1]    |                                               |    Grid[x+1][y+1]
 *       top_pins---->|                                               |<---- top_pins
 *                    |                                               |
 *                    +-----------------------------------------------+
 *                            ^             ^                ^
 *                            |             |                |
 *                            |             v                |
 *                       Grid[x][y]     ChanY[x][y]      Grid[x+1][y] 
 *                       right_pins    inputs/outputs      left_pins
 *
 *
 ********************************************************************/
static 
void build_switch_block_module(ModuleManager& module_manager, 
                               const CircuitLibrary& circuit_lib,
                               const std::vector<std::vector<t_grid_tile>>& grids,
                               const std::vector<t_switch_inf>& rr_switches,
                               const e_sram_orgz& sram_orgz_type,
                               const CircuitModelId& sram_model,
                               const RRGSB& rr_gsb) {
  /* Create a Module of Switch Block and add to module manager */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_sb_x(), rr_gsb.get_sb_y());
  ModuleId sb_module = module_manager.add_module(generate_switch_block_module_name(gsb_coordinate)); 

  /* Create a cache (fast look up) for module nets whose source are input ports */
  std::map<ModulePortId, ModuleNetId> input_port_to_module_nets;

  /* Add routing channel ports at each side of the GSB */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    DeviceCoordinator port_coordinator = rr_gsb.get_side_block_coordinator(side_manager.get_side()); 

    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      vtr::Point<size_t> port_coord(port_coordinator.get_x(), port_coordinator.get_y());
      std::string port_name = generate_routing_track_port_name(rr_gsb.get_chan_node(side_manager.get_side(), itrack)->type,
                                                               port_coord, itrack,  
                                                               rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack));
      BasicPort module_port(port_name, 1); /* Every track has a port size of 1 */

      switch (rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
      case OUT_PORT: 
        module_manager.add_port(sb_module, module_port, ModuleManager::MODULE_OUTPUT_PORT);
        break;
      case IN_PORT: {
        ModulePortId input_port_id = module_manager.add_port(sb_module, module_port, ModuleManager::MODULE_INPUT_PORT);
        /* Cache the input net */
        ModuleNetId net = module_manager.create_module_net(sb_module);
        module_manager.add_module_net_source(sb_module, net, sb_module, 0, input_port_id, 0);
        input_port_to_module_nets[input_port_id] = net;
        break;
      }
      default:
        vpr_printf(TIO_MESSAGE_ERROR, 
                   "(File: %s [LINE%d]) Invalid direction of chan[%d][%d]_track[%d]!\n",
                   __FILE__, __LINE__, rr_gsb.get_sb_x(), rr_gsb.get_sb_y(), itrack);
        exit(1);
      }
    }
    /* Dump OPINs of adjacent CLBs */
    for (size_t inode = 0; inode < rr_gsb.get_num_opin_nodes(side_manager.get_side()); ++inode) {
      vtr::Point<size_t> port_coord(rr_gsb.get_opin_node(side_manager.get_side(), inode)->xlow,
                                    rr_gsb.get_opin_node(side_manager.get_side(), inode)->ylow);
      std::string port_name = generate_grid_side_port_name(grids, port_coord,
                                                           rr_gsb.get_opin_node_grid_side(side_manager.get_side(), inode),
                                                           rr_gsb.get_opin_node(side_manager.get_side(), inode)->ptc_num); 
      BasicPort module_port(port_name, 1); /* Every grid output has a port size of 1 */
      /* Grid outputs are inputs of switch blocks */
      ModulePortId input_port_id = module_manager.add_port(sb_module, module_port, ModuleManager::MODULE_INPUT_PORT);

      /* Cache the input net */
      ModuleNetId net = module_manager.create_module_net(sb_module);
      module_manager.add_module_net_source(sb_module, net, sb_module, 0, input_port_id, 0);
      input_port_to_module_nets[input_port_id] = net;
    } 
  }

  /* Add routing multiplexers as child modules */
  for (size_t side = 0; side < rr_gsb.get_num_sides(); ++side) {
    Side side_manager(side);
    for (size_t itrack = 0; itrack < rr_gsb.get_chan_width(side_manager.get_side()); ++itrack) {
      /* We care INC_DIRECTION tracks at this side*/
      if (OUT_PORT == rr_gsb.get_chan_node_direction(side_manager.get_side(), itrack)) {
        build_switch_block_interc_modules(module_manager, 
                                          sb_module, rr_gsb,
                                          circuit_lib, grids, rr_switches, 
                                          side_manager.get_side(), 
                                          itrack, 
                                          input_port_to_module_nets);
      } 
    }
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, sb_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_shared_config_bits = find_module_num_shared_config_bits_from_child_modules(module_manager, sb_module); 
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, sb_module, module_num_shared_config_bits);
  }

  /* Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_config_bits = find_module_num_config_bits_from_child_modules(module_manager, sb_module, circuit_lib, sram_model, sram_orgz_type); 
  if (0 < module_num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, sb_module, circuit_lib, sram_model, sram_orgz_type, module_num_config_bits);
  }

  /* Add all the nets to connect configuration ports from memory module to primitive modules
   * This is a one-shot addition that covers all the memory modules in this primitive module!
   */
  if (0 < module_manager.configurable_children(sb_module).size()) {
    add_module_nets_memory_config_bus(module_manager, sb_module, 
                                      sram_orgz_type, circuit_lib.design_tech_type(sram_model));
  }
}

/*********************************************************************
 * Generate an input port for routing multiplexer inside the connection block
 * which is the middle output of a routing track 
 ********************************************************************/
static 
ModulePortId find_connection_block_module_chan_port(const ModuleManager& module_manager, 
                                                    const ModuleId& cb_module,
                                                    const RRGSB& rr_gsb, 
                                                    const t_rr_type& cb_type,
                                                    t_rr_node* chan_rr_node) {
  ModulePortId input_port_id;
  /* Generate the input port object */
  switch (chan_rr_node->type) {
  case CHANX:
  case CHANY: {
    /* Create port description for the routing track middle output */
    vtr::Point<size_t> port_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
    int chan_node_track_id = rr_gsb.get_cb_chan_node_index(cb_type, chan_rr_node);
    /* Create a port description for the middle output */
    std::string input_port_name = generate_routing_track_port_name(cb_type,
                                                                   port_coord, chan_node_track_id,  
                                                                   IN_PORT);
    /* Must find a valid port id in the Switch Block module */
    input_port_id = module_manager.find_module_port(cb_module, input_port_name); 
    VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module, input_port_id));
    break;
  }
  default: /* OPIN, SOURCE, IPIN, SINK are invalid*/
    vpr_printf(TIO_MESSAGE_ERROR, 
               "(File:%s, [LINE%d])Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n",
               __FILE__, __LINE__);
    exit(1);
  }

  return input_port_id;
}

/*********************************************************************
 * Generate a port for a routing track of a swtich block
 ********************************************************************/
static 
ModulePortId find_connection_block_module_ipin_port(const ModuleManager& module_manager,
                                                    const ModuleId& cb_module, 
                                                    const RRGSB& rr_gsb, 
                                                    const std::vector<std::vector<t_grid_tile>>& grids,
                                                    t_rr_node* src_rr_node) {

  /* Ensure the src_rr_node is an input pin of a CLB */
  VTR_ASSERT(IPIN == src_rr_node->type);
  /* Create port description for input pin of a CLB */
  vtr::Point<size_t> port_coord(src_rr_node->xlow, src_rr_node->ylow);
  /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB */
  enum e_side cb_ipin_side = NUM_SIDES;
  int cb_ipin_index = -1;
  rr_gsb.get_node_side_and_index(src_rr_node, OUT_PORT, &cb_ipin_side, &cb_ipin_index);
  /* We need to be sure that drive_rr_node is part of the CB */
  VTR_ASSERT((-1 != cb_ipin_index)&&(NUM_SIDES != cb_ipin_side));
  std::string port_name = generate_grid_side_port_name(grids, 
                                                       port_coord,
                                                       rr_gsb.get_ipin_node_grid_side(cb_ipin_side, cb_ipin_index), 
                                                       rr_gsb.get_ipin_node(cb_ipin_side, cb_ipin_index)->ptc_num); 

  /* Must find a valid port id in the Switch Block module */
  ModulePortId ipin_port_id = module_manager.find_module_port(cb_module, port_name); 
  VTR_ASSERT(true == module_manager.valid_module_port_id(cb_module, ipin_port_id));
  return ipin_port_id;
}

/*********************************************************************
 * Generate a list of routing track middle output ports 
 * for routing multiplexer inside the connection block
 ********************************************************************/
static 
std::vector<ModulePortId> find_connection_block_module_input_ports(const ModuleManager& module_manager,
                                                                   const ModuleId& cb_module,
                                                                   const RRGSB& rr_gsb, 
                                                                   const t_rr_type& cb_type,
                                                                   const std::vector<t_rr_node*>& input_rr_nodes) {
  std::vector<ModulePortId> input_ports;

  for (auto input_rr_node : input_rr_nodes) {
    input_ports.push_back(find_connection_block_module_chan_port(module_manager, cb_module, rr_gsb, cb_type, input_rr_node));
  }

  return input_ports;
}

/*********************************************************************
 * Print a short interconneciton in connection
 ********************************************************************/
static 
void build_connection_block_module_short_interc(ModuleManager& module_manager, 
                                                const ModuleId& cb_module,
                                                const RRGSB& rr_gsb,
                                                const t_rr_type& cb_type,
                                                const std::vector<std::vector<t_grid_tile>>& grids,
                                                t_rr_node* src_rr_node,
                                                const std::map<ModulePortId, ModuleNetId>& input_port_to_module_nets) {
  /* Ensure we have only one 1 driver node */
  VTR_ASSERT_SAFE(1 == src_rr_node->fan_in);

  /* Find  the driver node */
  t_rr_node* drive_rr_node = src_rr_node->drive_rr_nodes[0]; 

  /* We have OPINs since we may have direct connections:
   * These connections should be handled by other functions in the compact_netlist.c 
   * So we just return here for OPINs 
   */
  if (OPIN == drive_rr_node->type) {
    return;
  }

  VTR_ASSERT((CHANX == drive_rr_node->type) || (CHANY == drive_rr_node->type));

  /* Create port description for the routing track middle output */
  ModulePortId input_port_id = find_connection_block_module_chan_port(module_manager, cb_module, rr_gsb, cb_type, drive_rr_node);

  /* Create port description for input pin of a CLB */
  ModulePortId ipin_port_id = find_connection_block_module_ipin_port(module_manager, cb_module, rr_gsb, grids, src_rr_node);

  /* The input port and output port must match in size */
  BasicPort input_port = module_manager.module_port(cb_module, input_port_id);
  BasicPort ipin_port = module_manager.module_port(cb_module, ipin_port_id);
  VTR_ASSERT(input_port.get_width() == ipin_port.get_width());
  
  /* Create a module net for this short-wire connection */
  for (size_t pin_id = 0; pin_id < input_port.pins().size(); ++pin_id) {
    ModuleNetId net = input_port_to_module_nets.at(input_port_id);
    /* Skip Configuring the net source, it is done before */
    /* Configure the net sink */
    module_manager.add_module_net_sink(cb_module, net, cb_module, 0, ipin_port_id, ipin_port.pins()[pin_id]);
  }
}

/*********************************************************************
 * Build a instance of a routing multiplexer as well as
 * associated memory modules for a connection inside a connection block
 ********************************************************************/
static  
void build_connection_block_mux_module(ModuleManager& module_manager, 
                                       const ModuleId& cb_module, 
                                       const RRGSB& rr_gsb, 
                                       const t_rr_type& cb_type, 
                                       const CircuitLibrary& circuit_lib,
                                       const std::vector<std::vector<t_grid_tile>>& grids,
                                       const std::vector<t_switch_inf>& rr_switches,
                                       const e_side& cb_ipin_side,
                                       const size_t& ipin_index,
                                       const std::map<ModulePortId, ModuleNetId>& input_port_to_module_nets) {
  t_rr_node* cur_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, ipin_index);
  /* Check current rr_node is an input pin of a CLB */
  VTR_ASSERT(IPIN == cur_rr_node->type);

  /* Build a vector of driver rr_nodes */
  std::vector<t_rr_node*> drive_rr_nodes;
  for (int inode = 0; inode < cur_rr_node->num_drive_rr_nodes; inode++) {
    drive_rr_nodes.push_back(cur_rr_node->drive_rr_nodes[inode]);
  }

  int switch_index = cur_rr_node->drive_switches[DEFAULT_SWITCH_ID];

  /* Get the circuit model id of the routing multiplexer */
  CircuitModelId mux_model = rr_switches[switch_index].circuit_model;

  /* Find the input size of the implementation of a routing multiplexer */
  size_t datapath_mux_size = drive_rr_nodes.size();

  /* Find the module name of the multiplexer and try to find it in the module manager */
  std::string mux_module_name = generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size, std::string(""));
  ModuleId mux_module = module_manager.find_module(mux_module_name);
  VTR_ASSERT (true == module_manager.valid_module_id(mux_module));

  /* Get the MUX instance id from the module manager */
  size_t mux_instance_id = module_manager.num_instance(cb_module, mux_module);
  module_manager.add_child_module(cb_module, mux_module);

  /* TODO: Generate input ports that are wired to the input bus of the routing multiplexer */
  std::vector<ModulePortId> cb_input_port_ids = find_connection_block_module_input_ports(module_manager, cb_module, rr_gsb, cb_type, drive_rr_nodes);

  /* Link input bus port to Switch Block inputs */
  std::vector<CircuitPortId> mux_model_input_ports = circuit_lib.model_ports_by_type(mux_model, SPICE_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == mux_model_input_ports.size());
  /* Find the module port id of the input port */
  ModulePortId mux_input_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_lib_name(mux_model_input_ports[0])); 
  VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module, mux_input_port_id));
  BasicPort mux_input_port = module_manager.module_port(mux_module, mux_input_port_id);

  /* Check port size should match */
  VTR_ASSERT(mux_input_port.get_width() == cb_input_port_ids.size());
  for (size_t pin_id = 0; pin_id < cb_input_port_ids.size(); ++pin_id) {
    /* Use the exising net */
    ModuleNetId net = input_port_to_module_nets.at(cb_input_port_ids[pin_id]);
    /* Configure the net source */
    module_manager.add_module_net_source(cb_module, net, cb_module, 0, cb_input_port_ids[pin_id], 0);
    /* Configure the net sink */
    module_manager.add_module_net_sink(cb_module, net, mux_module, mux_instance_id, mux_input_port_id, mux_input_port.pins()[pin_id]);
  }

  /* Link output port to Switch Block outputs */
  std::vector<CircuitPortId> mux_model_output_ports = circuit_lib.model_ports_by_type(mux_model, SPICE_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == mux_model_output_ports.size());
  /* Use the port name convention in the circuit library */
  ModulePortId mux_output_port_id = module_manager.find_module_port(mux_module, circuit_lib.port_lib_name(mux_model_output_ports[0])); 
  VTR_ASSERT(true == module_manager.valid_module_port_id(mux_module, mux_output_port_id));
  BasicPort mux_output_port = module_manager.module_port(mux_module, mux_output_port_id);
  ModulePortId cb_output_port_id = find_connection_block_module_ipin_port(module_manager, cb_module, rr_gsb, grids, cur_rr_node);
  BasicPort cb_output_port = module_manager.module_port(cb_module, cb_output_port_id);

  /* Check port size should match */
  VTR_ASSERT(cb_output_port.get_width() == mux_output_port.get_width());
  for (size_t pin_id = 0; pin_id < mux_output_port.pins().size(); ++pin_id) {
    ModuleNetId net = module_manager.create_module_net(cb_module);
    /* Configuring the net source */
    module_manager.add_module_net_source(cb_module, net, mux_module, mux_instance_id, mux_output_port_id, mux_output_port.pins()[pin_id]);
    /* Configure the net sink */
    module_manager.add_module_net_sink(cb_module, net, cb_module, 0, cb_output_port_id, cb_output_port.pins()[pin_id]);
  }

  /* Instanciate memory modules */
  /* Find the name and module id of the memory module */
  std::string mem_module_name = generate_mux_subckt_name(circuit_lib, mux_model, datapath_mux_size, std::string(MEMORY_MODULE_POSTFIX)); 
  ModuleId mem_module = module_manager.find_module(mem_module_name);
  VTR_ASSERT (true == module_manager.valid_module_id(mem_module));

  size_t mem_instance_id = module_manager.num_instance(cb_module, mem_module);
  module_manager.add_child_module(cb_module, mem_module);

  /* Give an instance name: this name should be consistent with the block name given in bitstream manager,
   * If you want to bind the bitstream generation to modules
   */
  std::string mem_instance_name = generate_cb_memory_instance_name(CONNECTION_BLOCK_MEM_INSTANCE_PREFIX, rr_gsb.get_ipin_node_grid_side(cb_ipin_side, ipin_index), cur_rr_node->ptc_num, std::string(""));
  module_manager.set_child_instance_name(cb_module, mem_module, mem_instance_id, mem_instance_name);

  /* Add nets to connect regular and mode-select SRAM ports to the SRAM port of memory module */
  add_module_nets_between_logic_and_memory_sram_bus(module_manager, cb_module, 
                                                    mux_module, mux_instance_id,
                                                    mem_module, mem_instance_id,
                                                    circuit_lib, mux_model);
  /* Update memory and instance list */
  module_manager.add_configurable_child(cb_module, mem_module, mem_instance_id);
}

/********************************************************************
 * Print internal connections of a connection block 
 * For a IPIN node that is driven by only 1 fan-in,
 * a short wire will be created
 * For a IPIN node that is driven by more than two fan-ins,
 * a routing multiplexer will be instanciated 
 ********************************************************************/
static 
void build_connection_block_interc_modules(ModuleManager& module_manager,
                                           const ModuleId& cb_module, 
                                           const RRGSB& rr_gsb,
                                           const t_rr_type& cb_type,
                                           const CircuitLibrary& circuit_lib,
                                           const std::vector<std::vector<t_grid_tile>>& grids,
                                           const std::vector<t_switch_inf>& rr_switches,
                                           const e_side& cb_ipin_side,
                                           const size_t& ipin_index,
                                           const std::map<ModulePortId, ModuleNetId>& input_port_to_module_nets) {
  t_rr_node* src_rr_node = rr_gsb.get_ipin_node(cb_ipin_side, ipin_index);
  if (1 > src_rr_node->fan_in) {
    return; /* This port has no driver, skip it */
  } else if (1 == src_rr_node->fan_in) {
    /* Print a direct connection */
    build_connection_block_module_short_interc(module_manager, cb_module, rr_gsb, cb_type, grids, src_rr_node, input_port_to_module_nets);

  } else if (1 < src_rr_node->fan_in) {
    /* Print the multiplexer, fan_in >= 2 */
    build_connection_block_mux_module(module_manager, 
                                      cb_module, rr_gsb, cb_type, 
                                      circuit_lib, grids, rr_switches, 
                                      cb_ipin_side, ipin_index, 
                                      input_port_to_module_nets); 
  } /*Nothing should be done else*/ 
}

/******************************************************************** 
 * Generate a module of a connection Box (Type: [CHANX|CHANY])
 * Actually it is very similiar to switch box but
 * the difference is connection boxes connect Grid INPUT Pins to channels
 * NOTE: direct connection between CLBs should NOT be included inside this
 *       module! They should be added in the top-level module as their connection
 *       is not limited to adjacent CLBs!!!
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
 *  Note that middle output ports are shorted wire from inputs of routing tracks, 
 *  which are also the inputs of routing multiplexer of the connection block 
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
static 
void build_connection_block_module(ModuleManager& module_manager, 
                                   const CircuitLibrary& circuit_lib, 
                                   const std::vector<std::vector<t_grid_tile>>& grids,
                                   const std::vector<t_switch_inf>& rr_switches,
                                   const e_sram_orgz& sram_orgz_type, 
                                   const CircuitModelId& sram_model, 
                                   const RRGSB& rr_gsb,
                                   const t_rr_type& cb_type) {
  /* Create the netlist */
  vtr::Point<size_t> gsb_coordinate(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));

  /* Create a Verilog Module based on the circuit model, and add to module manager */
  ModuleId cb_module = module_manager.add_module(generate_connection_block_module_name(cb_type, gsb_coordinate)); 

  /* Add the input and output ports of routing tracks in the channel 
   * Routing tracks pass through the connection blocks  
   */
  for (size_t itrack = 0; itrack < rr_gsb.get_cb_chan_width(cb_type); ++itrack) {
    vtr::Point<size_t> port_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
    std::string port_name = generate_routing_track_port_name(cb_type,
                                                             port_coord, itrack,  
                                                             IN_PORT);
    BasicPort module_port(port_name, 1); /* Every track has a port size of 1 */
    module_manager.add_port(cb_module, module_port, ModuleManager::MODULE_INPUT_PORT);
  }
  for (size_t itrack = 0; itrack < rr_gsb.get_cb_chan_width(cb_type); ++itrack) {
    vtr::Point<size_t> port_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
    std::string port_name = generate_routing_track_port_name(cb_type,
                                                             port_coord, itrack,  
                                                             OUT_PORT);
    BasicPort module_port(port_name, 1); /* Every track has a port size of 1 */
    module_manager.add_port(cb_module, module_port, ModuleManager::MODULE_OUTPUT_PORT);
  }

  /* Add the input pins of grids, which are output ports of the connection block */
  std::vector<enum e_side> cb_ipin_sides = rr_gsb.get_cb_ipin_sides(cb_type);
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      t_rr_node* ipin_node = rr_gsb.get_ipin_node(cb_ipin_side, inode);
      vtr::Point<size_t> port_coord(ipin_node->xlow, ipin_node->ylow);
      std::string port_name = generate_grid_side_port_name(grids,
                                                           port_coord,
                                                           rr_gsb.get_ipin_node_grid_side(cb_ipin_side, inode),
                                                           ipin_node->ptc_num); 
      BasicPort module_port(port_name, 1); /* Every grid output has a port size of 1 */
      /* Grid outputs are inputs of switch blocks */
      module_manager.add_port(cb_module, module_port, ModuleManager::MODULE_OUTPUT_PORT);
    }
  }

  /* Create a cache (fast look up) for module nets whose source are input ports */
  std::map<ModulePortId, ModuleNetId> input_port_to_module_nets;

  /* Generate short-wire connection for each routing track : 
   * Each input port is short-wired to its output port and middle output port
   *    
   *   in[i] ----------> out[i]
   *             |
   *             +-----> mid_out[i]
   */
  for (size_t itrack = 0; itrack < rr_gsb.get_cb_chan_width(cb_type); ++itrack) {
    vtr::Point<size_t> port_coord(rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type));
    /* Create a port description for the input */
    std::string input_port_name = generate_routing_track_port_name(cb_type,
                                                                   port_coord, itrack,  
                                                                   IN_PORT);
    ModulePortId input_port_id = module_manager.find_module_port(cb_module, input_port_name);
    BasicPort input_port = module_manager.module_port(cb_module, input_port_id);

    /* Create a port description for the output */
    std::string output_port_name = generate_routing_track_port_name(cb_type,
                                                                    port_coord, itrack,  
                                                                    OUT_PORT);
    ModulePortId output_port_id = module_manager.find_module_port(cb_module, output_port_name);
    BasicPort output_port = module_manager.module_port(cb_module, output_port_id);

    /* Ensure port size matching */
    VTR_ASSERT(1 == input_port.get_width());
    VTR_ASSERT(input_port.get_width() == output_port.get_width());

    /* Create short-wires: input port ---> output port
     * Do short-wires: input port ---> middle output port 
     */
    for (size_t pin_id = 0; pin_id < input_port.pins().size(); ++pin_id) {
      ModuleNetId net = module_manager.create_module_net(cb_module);
      module_manager.add_module_net_source(cb_module, net, cb_module, 0, input_port_id, input_port.pins()[pin_id]); 
      module_manager.add_module_net_sink(cb_module, net, cb_module, 0, output_port_id, output_port.pins()[pin_id]); 
      /* Cache the module net */
      input_port_to_module_nets[input_port_id] = net;
    }
  }

  /* Add sub modules of routing multiplexers or direct interconnect*/
  for (size_t iside = 0; iside < cb_ipin_sides.size(); ++iside) {
    enum e_side cb_ipin_side = cb_ipin_sides[iside];
    for (size_t inode = 0; inode < rr_gsb.get_num_ipin_nodes(cb_ipin_side); ++inode) {
      build_connection_block_interc_modules(module_manager,  
                                            cb_module, rr_gsb, cb_type, 
                                            circuit_lib, grids, rr_switches, 
                                            cb_ipin_side, inode,
                                            input_port_to_module_nets);
    }
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the global ports from the child modules and build a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, cb_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_shared_config_bits = find_module_num_shared_config_bits_from_child_modules(module_manager, cb_module); 
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, cb_module, module_num_shared_config_bits);
  }

  /* Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances), 
   * we just need to find all the I/O ports from the child modules and build a list of it
   */
  size_t module_num_config_bits = find_module_num_config_bits_from_child_modules(module_manager, cb_module, circuit_lib, sram_model, sram_orgz_type); 
  if (0 < module_num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, cb_module, circuit_lib, sram_model, sram_orgz_type, module_num_config_bits);
  }

  /* Add all the nets to connect configuration ports from memory module to primitive modules
   * This is a one-shot addition that covers all the memory modules in this primitive module!
   */
  if (0 < module_manager.configurable_children(cb_module).size()) {
    add_module_nets_memory_config_bus(module_manager, cb_module, 
                                      sram_orgz_type, circuit_lib.design_tech_type(sram_model));
  }
}


/********************************************************************
 * Iterate over all the connection blocks in a device
 * and build a module for each of them 
 *******************************************************************/
static 
void build_flatten_connection_block_modules(ModuleManager& module_manager, 
                                            const DeviceRRGSB& L_device_rr_gsb,
                                            const CircuitLibrary& circuit_lib,
                                            const std::vector<std::vector<t_grid_tile>>& grids,
                                            const std::vector<t_switch_inf>& rr_switches,
                                            const e_sram_orgz& sram_orgz_type,
                                            const CircuitModelId& sram_model,
                                            const t_rr_type& cb_type) {
  /* Build unique X-direction connection block modules */
  DeviceCoordinator cb_range = L_device_rr_gsb.get_gsb_range();

  for (size_t ix = 0; ix < cb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < cb_range.get_y(); ++iy) {
      /* Check if the connection block exists in the device!
       * Some of them do NOT exist due to heterogeneous blocks (height > 1) 
       * We will skip those modules
       */
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      if ( (TRUE != is_cb_exist(cb_type, rr_gsb.get_cb_x(cb_type), rr_gsb.get_cb_y(cb_type)))
        || (true != rr_gsb.is_cb_exist(cb_type))) {
        continue;
      }
      build_connection_block_module(module_manager, 
                                    circuit_lib, 
                                    grids, rr_switches,
                                    sram_orgz_type, sram_model, 
                                    rr_gsb, cb_type);
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
void build_flatten_routing_modules(ModuleManager& module_manager,
                                   const DeviceRRGSB& L_device_rr_gsb,
                                   const CircuitLibrary& circuit_lib,
                                   const e_sram_orgz& sram_orgz_type,
                                   const CircuitModelId& sram_model,
                                   const std::vector<std::vector<t_grid_tile>>& grids,
                                   const t_det_routing_arch& routing_arch,
                                   const std::vector<t_switch_inf>& rr_switches) {
  /* Start time count */
  clock_t t_start = clock();

  vpr_printf(TIO_MESSAGE_INFO,
             "Building routing modules...");

  /* We only support uni-directional routing architecture now */
  VTR_ASSERT (UNI_DIRECTIONAL == routing_arch.directionality);

  /* TODO: deprecate DeviceCoordinator, use vtr::Point<size_t> only! */
  DeviceCoordinator sb_range = L_device_rr_gsb.get_gsb_range();

  /* Build unique switch block modules */
  for (size_t ix = 0; ix < sb_range.get_x(); ++ix) {
    for (size_t iy = 0; iy < sb_range.get_y(); ++iy) {
      const RRGSB& rr_gsb = L_device_rr_gsb.get_gsb(ix, iy);
      build_switch_block_module(module_manager, circuit_lib, 
                                grids, rr_switches,
                                sram_orgz_type, sram_model, 
                                rr_gsb);
    }
  }

  build_flatten_connection_block_modules(module_manager, L_device_rr_gsb, 
                                         circuit_lib, 
                                         grids, rr_switches,
                                         sram_orgz_type, sram_model, 
                                         CHANX);

  build_flatten_connection_block_modules(module_manager, L_device_rr_gsb, 
                                         circuit_lib,
                                         grids, rr_switches,
                                         sram_orgz_type, sram_model, 
                                         CHANY);

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %.2g seconds\n", 
             run_time_sec);  
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
void build_unique_routing_modules(ModuleManager& module_manager,
                                  const DeviceRRGSB& L_device_rr_gsb,
                                  const CircuitLibrary& circuit_lib,
                                  const e_sram_orgz& sram_orgz_type,
                                  const CircuitModelId& sram_model,
                                  const std::vector<std::vector<t_grid_tile>>& grids,
                                  const t_det_routing_arch& routing_arch,
                                  const std::vector<t_switch_inf>& rr_switches) {
  /* Start time count */
  clock_t t_start = clock();

  vpr_printf(TIO_MESSAGE_INFO,
             "Building unique routing modules...");

  /* We only support uni-directional routing architecture now */
  VTR_ASSERT (UNI_DIRECTIONAL == routing_arch.directionality);

  /* Build unique switch block modules */
  for (size_t isb = 0; isb < L_device_rr_gsb.get_num_sb_unique_module(); ++isb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_sb_unique_module(isb);
    build_switch_block_module(module_manager, circuit_lib, 
                              grids, rr_switches,
                              sram_orgz_type, sram_model, 
                              unique_mirror);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < L_device_rr_gsb.get_num_cb_unique_module(CHANX); ++icb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(CHANX, icb);

    build_connection_block_module(module_manager, 
                                  circuit_lib,  
                                  grids, rr_switches,
                                  sram_orgz_type, sram_model, 
                                  unique_mirror, CHANX);
  }

  /* Build unique X-direction connection block modules */
  for (size_t icb = 0; icb < L_device_rr_gsb.get_num_cb_unique_module(CHANY); ++icb) {
    const RRGSB& unique_mirror = L_device_rr_gsb.get_cb_unique_module(CHANY, icb);

    build_connection_block_module(module_manager, 
                                  circuit_lib, 
                                  grids, rr_switches,
                                  sram_orgz_type, sram_model, 
                                  unique_mirror, CHANY);
  }

  /* End time count */
  clock_t t_end = clock();

  float run_time_sec = (float)(t_end - t_start) / CLOCKS_PER_SEC;
  vpr_printf(TIO_MESSAGE_INFO, 
             "took %.2g seconds\n", 
             run_time_sec);  
}
