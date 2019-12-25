/********************************************************************
 * This file includes most utilized functions that are used to build modules 
 * for global routing architecture of a FPGA fabric
 * Covering:
 * 1. Connection blocks
 * 2. Switch blocks
 *******************************************************************/
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "device_coordinator.h"

#include "fpga_x2p_naming.h"

#include "build_routing_module_utils.h"

/*********************************************************************
 * Generate a port for a routing track of a swtich block
 ********************************************************************/
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
  std::string chan_port_name = generate_sb_module_track_port_name(rr_gsb.get_chan_node(chan_side, index)->type,
                                                                  chan_side, index,  
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
ModulePortId find_switch_block_module_input_port(const ModuleManager& module_manager,
                                                 const ModuleId& sb_module, 
                                                 const RRGSB& rr_gsb, 
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

    /* Find the side where the grid pin locates in the grid */
    enum e_side grid_pin_side = rr_gsb.get_opin_node_grid_side(input_rr_node);
    VTR_ASSERT(NUM_SIDES != grid_pin_side);

    std::string input_port_name = generate_sb_module_grid_port_name(input_side,
                                                                    grid_pin_side,
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
std::vector<ModulePortId> find_switch_block_module_input_ports(const ModuleManager& module_manager,
                                                               const ModuleId& sb_module, 
                                                               const RRGSB& rr_gsb, 
                                                               const std::vector<t_rr_node*>& input_rr_nodes) {
  std::vector<ModulePortId> input_ports;

  for (auto input_rr_node : input_rr_nodes) {
    /* Find the side where the input locates in the Switch Block */
    enum e_side input_pin_side = NUM_SIDES;
    /* The input could be at any side of the switch block, find it */
    int index = -1;
    rr_gsb.get_node_side_and_index(input_rr_node, IN_PORT, &input_pin_side, &index);
    VTR_ASSERT(NUM_SIDES != input_pin_side);
    VTR_ASSERT(-1 != index);

    input_ports.push_back(find_switch_block_module_input_port(module_manager, sb_module, rr_gsb, input_pin_side, input_rr_node));
  }

  return input_ports;
}

/*********************************************************************
 * Generate an input port for routing multiplexer inside the connection block
 * which is the middle output of a routing track 
 ********************************************************************/
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
    std::string input_port_name = generate_cb_module_track_port_name(cb_type,
                                                                     chan_node_track_id,  
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
ModulePortId find_connection_block_module_ipin_port(const ModuleManager& module_manager,
                                                    const ModuleId& cb_module, 
                                                    const RRGSB& rr_gsb, 
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
  std::string port_name = generate_cb_module_grid_port_name(cb_ipin_side, 
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
