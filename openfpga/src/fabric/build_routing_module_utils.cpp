/********************************************************************
 * This file includes most utilized functions that are used to build modules
 * for global routing architecture of a FPGA fabric
 * Covering:
 * 1. Connection blocks
 * 2. Switch blocks
 *******************************************************************/
/* Headers from vtrutil library */
#include "build_routing_module_utils.h"

#include "openfpga_naming.h"
#include "openfpga_rr_graph_utils.h"
#include "openfpga_side_manager.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/*********************************************************************
 * Generate the port name of a grid pin for a routing module,
 * which could be a switch block or a connection block
 * Note that to ensure unique grid port name in the context of a routing module,
 * we need a prefix which denotes the relative location of the port in the
 *routing module
 *
 * The prefix is created by considering the the grid coordinate
 * and switch block coordinate
 * Detailed rules in conversion is as follows:
 *
 *             top_left         top_right
 *             +------------------------+
 *    left_top |                        | right_top
 *             |      Switch Block      |
 *             |         [x][y]         |
 *             |                        |
 *             |                        |
 *  left_right |                        | right_bottom
 *             +------------------------+
 *              bottom_left  bottom_right
 *
 *  +--------------------------------------------------------
 *  | Grid Coordinate | Pin side of grid | module side
 *  +--------------------------------------------------------
 *  | [x][y+1]        | right            | top_left
 *  +--------------------------------------------------------
 *  | [x][y+1]        | bottom           | left_top
 *  +--------------------------------------------------------
 *  | [x+1][y+1]      | left             | top_right
 *  +--------------------------------------------------------
 *  | [x+1][y+1]      | bottom           | right_top
 *  +--------------------------------------------------------
 *  | [x][y]          | top              | left_right
 *  +--------------------------------------------------------
 *  | [x][y]          | right            | bottom_left
 *  +--------------------------------------------------------
 *  | [x+1][y]        | top              | right_bottom
 *  +--------------------------------------------------------
 *  | [x+1][y]        | left             | bottom_right
 *  +--------------------------------------------------------
 *
 *********************************************************************/
std::string generate_sb_module_grid_port_name(
  const e_side& sb_side, const e_side& grid_side,
  const DeviceGrid& vpr_device_grid,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const RRNodeId& rr_node) {
  SideManager sb_side_manager(sb_side);
  SideManager grid_side_manager(grid_side);
  /* Relative location is opposite to the side in grid context */
  grid_side_manager.set_opposite();
  std::string prefix = sb_side_manager.to_string() + std::string("_") +
                       grid_side_manager.to_string();

  /* Collect the attributes of the rr_node required to generate the port name */
  int pin_id = rr_graph.node_pin_num(rr_node);
  e_side pin_side = get_rr_graph_single_node_side(rr_graph, rr_node);
  t_physical_tile_type_ptr physical_tile =
    vpr_device_grid.get_physical_type(t_physical_tile_loc(
      rr_graph.node_xlow(rr_node), rr_graph.node_ylow(rr_node),
      rr_graph.node_layer(rr_node)));
  int pin_width_offset = physical_tile->pin_width_offset[pin_id];
  int pin_height_offset = physical_tile->pin_height_offset[pin_id];
  BasicPort pin_info =
    vpr_device_annotation.physical_tile_pin_port_info(physical_tile, pin_id);
  VTR_ASSERT(true == pin_info.is_valid());
  int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
    physical_tile, pin_id);
  VTR_ASSERT(OPEN != subtile_index && subtile_index < physical_tile->capacity);

  return prefix + std::string("_") +
         generate_routing_module_grid_port_name(
           pin_width_offset, pin_height_offset, subtile_index, pin_side,
           pin_info);
}

/*********************************************************************
 * Generate the port name of a grid pin for a routing module,
 * which could be a switch block or a connection block
 * Note that to ensure unique grid port name in the context of a routing module,
 * we need a prefix which denotes the relative location of the port in the
 *routing module
 *********************************************************************/
std::string generate_cb_module_grid_port_name(
  const e_side& cb_side, const DeviceGrid& vpr_device_grid,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const RRNodeId& rr_node) {
  SideManager side_manager(cb_side);
  std::string prefix = side_manager.to_string();

  /* Collect the attributes of the rr_node required to generate the port name */
  int pin_id = rr_graph.node_pin_num(rr_node);
  e_side pin_side = get_rr_graph_single_node_side(rr_graph, rr_node);
  t_physical_tile_type_ptr physical_tile =
    vpr_device_grid.get_physical_type(t_physical_tile_loc(
      rr_graph.node_xlow(rr_node), rr_graph.node_ylow(rr_node),
      rr_graph.node_layer(rr_node)));
  int pin_width_offset = physical_tile->pin_width_offset[pin_id];
  int pin_height_offset = physical_tile->pin_height_offset[pin_id];
  BasicPort pin_info =
    vpr_device_annotation.physical_tile_pin_port_info(physical_tile, pin_id);
  VTR_ASSERT(true == pin_info.is_valid());
  int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
    physical_tile, pin_id);
  VTR_ASSERT(OPEN != subtile_index && subtile_index < physical_tile->capacity);

  return prefix + std::string("_") +
         generate_routing_module_grid_port_name(
           pin_width_offset, pin_height_offset, subtile_index, pin_side,
           pin_info);
}

/*********************************************************************
 * Find the port id and pin id for a routing track in the switch
 * block module with a given rr_node
 ********************************************************************/
ModulePinInfo find_switch_block_module_chan_port(
  const ModuleManager& module_manager, const ModuleId& sb_module,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_side& chan_side,
  const RRNodeId& cur_rr_node, const PORTS& cur_rr_node_direction) {
  /* Get the index in sb_info of cur_rr_node */
  int index = rr_gsb.get_node_index(rr_graph, cur_rr_node, chan_side,
                                    cur_rr_node_direction);
  /* Make sure this node is included in this sb_info */
  VTR_ASSERT((-1 != index) && (NUM_SIDES != chan_side));

  std::string chan_port_name = generate_sb_module_track_port_name(
    rr_graph.node_type(rr_gsb.get_chan_node(chan_side, index)), chan_side,
    rr_gsb.get_chan_node_direction(chan_side, index));

  /* Must find a valid port id in the Switch Block module */
  ModulePortId chan_port_id =
    module_manager.find_module_port(sb_module, chan_port_name);
  VTR_ASSERT(true ==
             module_manager.valid_module_port_id(sb_module, chan_port_id));
  return ModulePinInfo(chan_port_id, index / 2);
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
ModulePinInfo find_switch_block_module_input_port(
  const ModuleManager& module_manager, const ModuleId& sb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_side& input_side,
  const RRNodeId& input_rr_node) {
  /* Deposit an invalid value */
  ModulePinInfo input_port(ModulePortId::INVALID(), 0);
  /* Generate the input port object */
  switch (rr_graph.node_type(input_rr_node)) {
    /* case SOURCE: */
    case OPIN: {
      /* Find the coordinator (grid_x and grid_y) for the input port */
      vtr::Point<size_t> input_port_coord(rr_graph.node_xlow(input_rr_node),
                                          rr_graph.node_ylow(input_rr_node));

      /* Find the side where the grid pin locates in the grid */
      enum e_side grid_pin_side =
        get_rr_graph_single_node_side(rr_graph, input_rr_node);
      VTR_ASSERT(NUM_SIDES != grid_pin_side);

      std::string input_port_name = generate_sb_module_grid_port_name(
        input_side, grid_pin_side, grids, vpr_device_annotation, rr_graph,
        input_rr_node);
      /* Must find a valid port id in the Switch Block module */
      input_port.first =
        module_manager.find_module_port(sb_module, input_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(sb_module,
                                                             input_port.first));
      break;
    }
    case CHANX:
    case CHANY: {
      input_port = find_switch_block_module_chan_port(
        module_manager, sb_module, rr_graph, rr_gsb, input_side, input_rr_node,
        IN_PORT);
      break;
    }
    default: /* SOURCE, IPIN, SINK are invalid*/
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n");
      exit(1);
  }

  return input_port;
}

/*********************************************************************
 * Generate a list of input ports for routing multiplexer inside the switch
 *block
 ********************************************************************/
std::vector<ModulePinInfo> find_switch_block_module_input_ports(
  const ModuleManager& module_manager, const ModuleId& sb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const std::vector<RRNodeId>& input_rr_nodes) {
  std::vector<ModulePinInfo> input_ports;

  for (const RRNodeId& input_rr_node : input_rr_nodes) {
    /* Find the side where the input locates in the Switch Block */
    enum e_side input_pin_side = NUM_SIDES;
    /* The input could be at any side of the switch block, find it */
    int index = -1;
    rr_gsb.get_node_side_and_index(rr_graph, input_rr_node, IN_PORT,
                                   input_pin_side, index);
    VTR_ASSERT(NUM_SIDES != input_pin_side);
    VTR_ASSERT(-1 != index);

    input_ports.push_back(find_switch_block_module_input_port(
      module_manager, sb_module, grids, vpr_device_annotation, rr_graph, rr_gsb,
      input_pin_side, input_rr_node));
  }

  return input_ports;
}

/*********************************************************************
 * Generate an input port for routing multiplexer inside the connection block
 * which is the middle output of a routing track
 ********************************************************************/
ModulePinInfo find_connection_block_module_chan_port(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type,
  const RRNodeId& chan_rr_node) {
  ModulePinInfo input_port_info;
  /* Generate the input port object */
  switch (rr_graph.node_type(chan_rr_node)) {
    case CHANX:
    case CHANY: {
      /* Create port description for the routing track middle output */
      int chan_node_track_id =
        rr_gsb.get_cb_chan_node_index(cb_type, chan_rr_node);
      /* Create a port description for the middle output */
      std::string input_port_name = generate_cb_module_track_port_name(
        cb_type, IN_PORT, 0 == chan_node_track_id % 2);
      /* Must find a valid port id in the Switch Block module */
      input_port_info.first =
        module_manager.find_module_port(cb_module, input_port_name);
      input_port_info.second = chan_node_track_id / 2;
      VTR_ASSERT(true == module_manager.valid_module_port_id(
                           cb_module, input_port_info.first));
      break;
    }
    default: /* OPIN, SOURCE, IPIN, SINK are invalid*/
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid rr_node type! Should be [OPIN|CHANX|CHANY].\n");
      exit(1);
  }

  return input_port_info;
}

/*********************************************************************
 * Generate a port for a routing track of a swtich block
 ********************************************************************/
ModulePortId find_connection_block_module_ipin_port(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const RRNodeId& src_rr_node) {
  /* Ensure the src_rr_node is an input pin of a CLB */
  VTR_ASSERT(IPIN == rr_graph.node_type(src_rr_node));
  /* Create port description for input pin of a CLB */
  vtr::Point<size_t> port_coord(rr_graph.node_xlow(src_rr_node),
                                rr_graph.node_ylow(src_rr_node));
  /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB
   */
  enum e_side cb_ipin_side = NUM_SIDES;
  int cb_ipin_index = -1;
  rr_gsb.get_node_side_and_index(rr_graph, src_rr_node, OUT_PORT, cb_ipin_side,
                                 cb_ipin_index);
  /* We need to be sure that drive_rr_node is part of the CB */
  VTR_ASSERT((-1 != cb_ipin_index) && (NUM_SIDES != cb_ipin_side));
  std::string port_name = generate_cb_module_grid_port_name(
    cb_ipin_side, grids, vpr_device_annotation, rr_graph,
    rr_gsb.get_ipin_node(cb_ipin_side, cb_ipin_index));

  /* Must find a valid port id in the Switch Block module */
  ModulePortId ipin_port_id =
    module_manager.find_module_port(cb_module, port_name);
  VTR_ASSERT(true ==
             module_manager.valid_module_port_id(cb_module, ipin_port_id));
  return ipin_port_id;
}

/*********************************************************************
 * Generate a port for a connection block
 ********************************************************************/
ModulePortId find_connection_block_module_opin_port(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const RRNodeId& src_rr_node) {
  /* Ensure the src_rr_node is an input pin of a CLB */
  VTR_ASSERT(OPIN == rr_graph.node_type(src_rr_node));
  /* Search all the sides of a SB, see this drive_rr_node is an INPUT of this SB
   */
  enum e_side cb_opin_side = NUM_SIDES;
  int cb_opin_index = -1;
  rr_gsb.get_node_side_and_index(rr_graph, src_rr_node, IN_PORT, cb_opin_side,
                                 cb_opin_index);
  /* We need to be sure that drive_rr_node is part of the CB */
  VTR_ASSERT((-1 != cb_opin_index) && (NUM_SIDES != cb_opin_side));
  std::string port_name = generate_cb_module_grid_port_name(
    cb_opin_side, grids, vpr_device_annotation, rr_graph,
    rr_gsb.get_opin_node(cb_opin_side, cb_opin_index));

  /* Must find a valid port id in the Switch Block module */
  ModulePortId opin_port_id =
    module_manager.find_module_port(cb_module, port_name);
  VTR_ASSERT(true ==
             module_manager.valid_module_port_id(cb_module, opin_port_id));
  return opin_port_id;
}

/*********************************************************************
 * Generate a list of routing track middle output ports
 * for routing multiplexer inside the connection block
 ********************************************************************/
std::vector<ModulePinInfo> find_connection_block_module_input_ports(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type,
  const std::vector<RRNodeId>& input_rr_nodes) {
  std::vector<ModulePinInfo> input_ports;

  for (auto input_rr_node : input_rr_nodes) {
    if (OPIN == rr_graph.node_type(input_rr_node)) {
      input_ports.push_back(ModulePinInfo(
        find_connection_block_module_opin_port(module_manager, cb_module, grids,
                                               vpr_device_annotation, rr_graph,
                                               rr_gsb, input_rr_node),
        0));
    } else {
      input_ports.push_back(find_connection_block_module_chan_port(
        module_manager, cb_module, rr_graph, rr_gsb, cb_type, input_rr_node));
    }
  }

  return input_ports;
}

} /* end namespace openfpga */
