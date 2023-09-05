/********************************************************************
 * This file includes functions to generate module/port names for
 * Verilog and SPICE netlists
 *
 * IMPORTANT: keep all the naming functions in this file to be
 * generic for both Verilog and SPICE generators
 ********************************************************************/
#include "openfpga_naming.h"

#include "circuit_library_utils.h"
#include "openfpga_reserved_words.h"
#include "openfpga_side_manager.h"
#include "pb_type_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/************************************************
 * A generic function to generate the instance name
 * in the following format:
 * <instance_name>_<id>_
 * This is mainly used by module manager to give a default
 * name for each instance when outputting the module
 * in Verilog/SPICE format
 ***********************************************/
std::string generate_instance_name(const std::string& instance_name,
                                   const size_t& instance_id) {
  return instance_name + std::string("_") + std::to_string(instance_id) +
         std::string("_");
}

/************************************************
 * A generic function to generate the instance name
 * in the following format:
 * <instance_name>_<id>_
 * This is mainly used by module manager to give a default
 * name for each instance when outputting the module
 * in Verilog/SPICE format
 ***********************************************/
std::string generate_instance_wildcard_name(const std::string& instance_name,
                                            const std::string& wildcard_str) {
  return instance_name + std::string("_") + wildcard_str + std::string("_");
}

/************************************************
 * Generate the node name for a multiplexing structure
 * Case 1 : If there is an intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in_buf
 * Case 1 : If there is NO intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in
 ***********************************************/
std::string generate_mux_node_name(const size_t& node_level,
                                   const bool& add_buffer_postfix) {
  /* Generate the basic node_name */
  std::string node_name = "mux_l" + std::to_string(node_level) + "_in";

  /* Add a postfix upon requests */
  if (true == add_buffer_postfix) {
    /* '1' indicates that the location is needed */
    node_name += "_buf";
  }

  return node_name;
}

/************************************************
 * Generate the instance name for a branch circuit in multiplexing structure
 * Case 1 : If there is an intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in_buf
 * Case 1 : If there is NO intermediate buffer followed by,
 *          the node name will be mux_l<node_level>_in
 ***********************************************/
std::string generate_mux_branch_instance_name(const size_t& node_level,
                                              const size_t& node_index_at_level,
                                              const bool& add_buffer_postfix) {
  return std::string(generate_mux_node_name(node_level, add_buffer_postfix) +
                     "_" + std::to_string(node_index_at_level) + "_");
}

/************************************************
 * Generate the module name for a multiplexer in Verilog format
 * Different circuit model requires different names:
 * 1. LUTs are named as <model_name>_mux
 * 2. MUXes are named as <model_name>_size<num_inputs>
 ***********************************************/
std::string generate_mux_subckt_name(const CircuitLibrary& circuit_lib,
                                     const CircuitModelId& circuit_model,
                                     const size_t& mux_size,
                                     const std::string& postfix) {
  std::string module_name = circuit_lib.model_name(circuit_model);
  /* Check the model type and give different names */
  if (CIRCUIT_MODEL_MUX == circuit_lib.model_type(circuit_model)) {
    module_name += "_size";
    module_name += std::to_string(mux_size);
  } else {
    VTR_ASSERT(CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model));
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
std::string generate_mux_branch_subckt_name(const CircuitLibrary& circuit_lib,
                                            const CircuitModelId& circuit_model,
                                            const size_t& branch_mux_size,
                                            const size_t& branch_mem_size,
                                            const std::string& postfix) {
  /* If the tgate circuit model of this MUX is a MUX2 standard cell,
   * the mux_subckt name will be the name of the standard cell
   */
  CircuitModelId subckt_model =
    circuit_lib.pass_gate_logic_model(circuit_model);
  if (CIRCUIT_MODEL_GATE == circuit_lib.model_type(subckt_model)) {
    VTR_ASSERT(CIRCUIT_MODEL_GATE_MUX2 == circuit_lib.gate_type(subckt_model));
    return circuit_lib.model_name(subckt_model);
  }

  /* Include memory size as a second unique signature for the branch module
   * This is due to some branch modules have the same input sizes but different
   * memory sizes
   */
  std::string branch_postfix = postfix + "_input" +
                               std::to_string(branch_mux_size) + "_mem" +
                               std::to_string(branch_mem_size);

  std::string module_name = circuit_lib.model_name(circuit_model);
  if (CIRCUIT_MODEL_LUT == circuit_lib.model_type(circuit_model)) {
    module_name += "_mux";
  } else {
    VTR_ASSERT(CIRCUIT_MODEL_MUX == circuit_lib.model_type(circuit_model));
  }
  module_name += branch_postfix;

  return module_name;
}

/************************************************
 * Generate the module name of a local decoder
 * for multiplexer
 ***********************************************/
std::string generate_mux_local_decoder_subckt_name(const size_t& addr_size,
                                                   const size_t& data_size) {
  std::string subckt_name = "local_encoder";
  subckt_name += std::to_string(addr_size);
  subckt_name += "to";
  subckt_name += std::to_string(data_size);

  return subckt_name;
}

/************************************************
 * Generate the module name of a decoder
 * for frame-based memories
 ***********************************************/
std::string generate_memory_decoder_subckt_name(const size_t& addr_size,
                                                const size_t& data_size) {
  std::string subckt_name = "decoder";
  subckt_name += std::to_string(addr_size);
  subckt_name += "to";
  subckt_name += std::to_string(data_size);

  return subckt_name;
}

/************************************************
 * Generate the module name of a word-line decoder
 * for memories
 ***********************************************/
std::string generate_memory_decoder_with_data_in_subckt_name(
  const size_t& addr_size, const size_t& data_size) {
  std::string subckt_name = "decoder_with_data_in_";
  subckt_name += std::to_string(addr_size);
  subckt_name += "to";
  subckt_name += std::to_string(data_size);

  return subckt_name;
}

/************************************************
 * Generate the module name of a routing track wire
 ***********************************************/
std::string generate_segment_wire_subckt_name(
  const std::string& wire_model_name, const size_t& segment_id) {
  std::string segment_wire_subckt_name =
    wire_model_name + "_seg" + std::to_string(segment_id);

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
std::string generate_segment_wire_mid_output_name(
  const std::string& regular_output_name) {
  /* TODO: maybe have a postfix? */
  return std::string("mid_" + regular_output_name);
}

/*********************************************************************
 * Generate the module name for a memory sub-circuit
 * If this is a module just to feed through memory lines, use a special name
 ********************************************************************/
std::string generate_memory_module_name(const CircuitLibrary& circuit_lib,
                                        const CircuitModelId& circuit_model,
                                        const CircuitModelId& sram_model,
                                        const std::string& postfix,
                                        const bool& feedthrough_memory) {
  std::string mid_name;
  if (feedthrough_memory) {
    mid_name = "feedthrough_";
  }
  return std::string(circuit_lib.model_name(circuit_model) + "_" + mid_name +
                     circuit_lib.model_name(sram_model) + postfix);
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
  return std::string(prefix + std::to_string(block_id) + postfix);
}

/*********************************************************************
 * Generate the netlist name for a routing block with a given coordinate
 * It could be
 * 1. Routing channel
 * 2. Connection block
 * 3. Switch block
 *********************************************************************/
std::string generate_routing_block_netlist_name(
  const std::string& prefix, const vtr::Point<size_t>& coordinate,
  const std::string& postfix) {
  return std::string(prefix + std::to_string(coordinate.x()) +
                     std::string("__") + std::to_string(coordinate.y()) +
                     std::string("_") + postfix);
}

/*********************************************************************
 * Generate the netlist name for a connection block with a given coordinate
 *********************************************************************/
std::string generate_connection_block_netlist_name(
  const t_rr_type& cb_type, const vtr::Point<size_t>& coordinate,
  const std::string& postfix) {
  std::string prefix("cb");
  switch (cb_type) {
    case CHANX:
      prefix += std::string("x_");
      break;
    case CHANY:
      prefix += std::string("y_");
      break;
    default:
      VTR_LOG_ERROR("Invalid type of connection block!\n");
      exit(1);
  }

  return generate_routing_block_netlist_name(prefix, coordinate, postfix);
}

/*********************************************************************
 * Generate the module name for a unique routing channel
 *********************************************************************/
std::string generate_routing_channel_module_name(const t_rr_type& chan_type,
                                                 const size_t& block_id) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT((CHANX == chan_type) || (CHANY == chan_type));

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  return std::string(module_prefix_map[chan_type] + std::string("_") +
                     std::to_string(block_id) + std::string("_"));
}

/*********************************************************************
 * Generate the module name for a routing channel with a given coordinate
 *********************************************************************/
std::string generate_routing_channel_module_name(
  const t_rr_type& chan_type, const vtr::Point<size_t>& coordinate) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT((CHANX == chan_type) || (CHANY == chan_type));

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  return std::string(module_prefix_map[chan_type] +
                     std::to_string(coordinate.x()) + std::string("_") +
                     std::to_string(coordinate.y()) + std::string("_"));
}

/*********************************************************************
 * Generate the port name for a routing track with a given coordinate
 * and port direction
 * This function is mainly used in naming routing tracks in the top-level
 *netlists where we do need unique names (with coordinates) for each routing
 *tracks
 *********************************************************************/
std::string generate_routing_track_port_name(
  const t_rr_type& chan_type, const vtr::Point<size_t>& coordinate,
  const size_t& track_id, const PORTS& port_direction) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT((CHANX == chan_type) || (CHANY == chan_type));

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  std::string port_name = module_prefix_map[chan_type];
  port_name +=
    std::string("_" + std::to_string(coordinate.x()) + std::string("__") +
                std::to_string(coordinate.y()) + std::string("__"));

  switch (port_direction) {
    case OUT_PORT:
      port_name += std::string("out_");
      break;
    case IN_PORT:
      port_name += std::string("in_");
      break;
    default:
      VTR_LOG_ERROR("Invalid direction of chan_rr_node!\n");
      exit(1);
  }

  /* Add the track id to the port name */
  port_name += std::to_string(track_id) + std::string("_");

  return port_name;
}

/*********************************************************************
 * Generate the port name for a routing track in a Switch Block module
 * This function is created to ease the PnR for each unique routing module
 * So it is mainly used when creating non-top-level modules!
 * Note that this function does not include any port coordinate
 * Instead, we use the relative location of the pins in the context of routing
 *modules so that each module can be instanciated across the fabric Even though,
 *port direction must be provided!
 *********************************************************************/
std::string generate_sb_module_track_port_name(const t_rr_type& chan_type,
                                               const e_side& module_side,
                                               const PORTS& port_direction) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT((CHANX == chan_type) || (CHANY == chan_type));

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  std::string port_name = module_prefix_map[chan_type];
  port_name += std::string("_");

  SideManager side_manager(module_side);
  port_name += std::string(side_manager.to_string());
  port_name += std::string("_");

  switch (port_direction) {
    case OUT_PORT:
      port_name += std::string("out");
      break;
    case IN_PORT:
      port_name += std::string("in");
      break;
    default:
      VTR_LOG_ERROR("Invalid direction of chan_rr_node!\n");
      exit(1);
  }

  return port_name;
}

/*********************************************************************
 * Generate the port name for a routing track in a Connection Block module
 * This function is created to ease the PnR for each unique routing module
 * So it is mainly used when creating non-top-level modules!
 * Note that this function does not include any port coordinate
 * Instead, we use the relative location of the pins in the context of routing
 *modules so that each module can be instanciated across the fabric Even though,
 *port direction must be provided!
 *
 * Upper_location: specify if an upper/lower prefix to be added.
 *                 The location indicates where the bus port should be
 *                 placed on the perimeter of the connection block
 *                 - For X-directional CB:
 *                   - upper is the left side
 *                   - lower is the right side
 *                 - For Y-directional CB:
 *                   - upper is the bottom side
 *                   - lower is the top side
 *********************************************************************/
std::string generate_cb_module_track_port_name(const t_rr_type& chan_type,
                                               const PORTS& port_direction,
                                               const bool& upper_location) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT((CHANX == chan_type) || (CHANY == chan_type));

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::map<bool, std::string>> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX][true] = std::string("chanx_left");
  module_prefix_map[CHANX][false] = std::string("chanx_right");
  module_prefix_map[CHANY][true] = std::string("chany_bottom");
  module_prefix_map[CHANY][false] = std::string("chany_top");

  std::string port_name = module_prefix_map[chan_type][upper_location];
  port_name += std::string("_");

  switch (port_direction) {
    case OUT_PORT:
      port_name += std::string("out");
      break;
    case IN_PORT:
      port_name += std::string("in");
      break;
    default:
      VTR_LOG_ERROR("Invalid direction of chan_rr_node!\n");
      exit(1);
  }

  return port_name;
}

/*********************************************************************
 * Generate the middle output port name for a routing track
 * with a given coordinate
 *********************************************************************/
std::string generate_routing_track_middle_output_port_name(
  const t_rr_type& chan_type, const vtr::Point<size_t>& coordinate,
  const size_t& track_id) {
  /* Channel must be either CHANX or CHANY */
  VTR_ASSERT((CHANX == chan_type) || (CHANY == chan_type));

  /* Create a map between chan_type and module_prefix */
  std::map<t_rr_type, std::string> module_prefix_map;
  /* TODO: use a constexpr string to replace the fixed name? */
  module_prefix_map[CHANX] = std::string("chanx");
  module_prefix_map[CHANY] = std::string("chany");

  std::string port_name = module_prefix_map[chan_type];
  port_name +=
    std::string("_" + std::to_string(coordinate.x()) + std::string("__") +
                std::to_string(coordinate.y()) + std::string("__"));

  port_name += std::string("midout_");

  /* Add the track id to the port name */
  port_name += std::to_string(track_id) + std::string("_");

  return port_name;
}

/*********************************************************************
 * Generate the module name for a switch block with a given coordinate
 *********************************************************************/
std::string generate_switch_block_module_name(
  const vtr::Point<size_t>& coordinate) {
  return std::string("sb_" + std::to_string(coordinate.x()) +
                     std::string("__") + std::to_string(coordinate.y()) +
                     std::string("_"));
}

/*********************************************************************
 * Generate the module name for a tile module with a given coordinate
 *********************************************************************/
std::string generate_tile_module_name(const vtr::Point<size_t>& tile_coord) {
  return std::string("tile_" + std::to_string(tile_coord.x()) + "__" +
                     std::to_string(tile_coord.y()) + "_");
}

/*********************************************************************
 * Generate the port name for a tile. Note that use the index to make the tile
 *port name unique!
 *********************************************************************/
std::string generate_tile_module_port_name(const std::string& prefix,
                                           const std::string& port_name) {
  return prefix + std::string("_") + port_name;
}

/*********************************************************************
 * Generate the netlist name of a grid block
 **********************************************************************/
std::string generate_tile_module_netlist_name(const std::string& block_name,
                                              const std::string& postfix) {
  return block_name + postfix;
}

/*********************************************************************
 * Generate the module name of a physical memory module
 **********************************************************************/
std::string generate_physical_memory_module_name(const std::string& prefix,
                                                 const size_t& mem_size) {
  return prefix + std::string("_config_group_mem_size") +
         std::to_string(mem_size);
}

/*********************************************************************
 * Generate the module name for a connection block with a given coordinate
 *********************************************************************/
std::string generate_connection_block_module_name(
  const t_rr_type& cb_type, const vtr::Point<size_t>& coordinate) {
  std::string prefix("cb");
  switch (cb_type) {
    case CHANX:
      prefix += std::string("x_");
      break;
    case CHANY:
      prefix += std::string("y_");
      break;
    default:
      VTR_LOG_ERROR("Invalid type of connection block!\n");
      exit(1);
  }

  return std::string(prefix + std::to_string(coordinate.x()) +
                     std::string("__") + std::to_string(coordinate.y()) +
                     std::string("_"));
}

/*********************************************************************
 * Generate the port name for a grid in top-level netlists, i.e., full FPGA
 *fabric This function will generate a full port name including coordinates so
 *that each pin in top-level netlists is unique!
 *********************************************************************/
std::string generate_grid_port_name(const size_t& width, const size_t& height,
                                    const int& subtile_index,
                                    const e_side& side,
                                    const BasicPort& pin_info) {
  /* Ensure that the pin is 1-bit ONLY !!! */
  VTR_ASSERT(1 == pin_info.get_width());

  SideManager side_manager(side);
  std::string port_name = std::string(side_manager.to_string());
  port_name += std::string("_width_");
  port_name += std::to_string(width);
  port_name += std::string("_height_");
  port_name += std::to_string(height);
  port_name += std::string("_subtile_");
  port_name += std::to_string(subtile_index);
  port_name += std::string("__pin_");
  port_name += pin_info.get_name();
  port_name += std::string("_");
  port_name += std::to_string(pin_info.get_lsb());
  port_name += std::string("_");
  return port_name;
}

/*********************************************************************
 * Generate the port name for a grid with duplication
 * This function will generate two types of port names.
 * One with a postfix of "upper"
 * The other with a postfix of "lower"
 *********************************************************************/
std::string generate_grid_duplicated_port_name(
  const size_t& width, const size_t& height, const int& subtile_index,
  const e_side& side, const BasicPort& pin_info, const bool& upper_port) {
  /* Ensure that the pin is 1-bit ONLY !!! */
  VTR_ASSERT(1 == pin_info.get_width());

  SideManager side_manager(side);
  std::string port_name = std::string(side_manager.to_string());
  port_name += std::string("_width_");
  port_name += std::to_string(width);
  port_name += std::string("_height_");
  port_name += std::to_string(height);
  port_name += std::string("_subtile_");
  port_name += std::to_string(subtile_index);
  port_name += std::string("__pin_");
  port_name += pin_info.get_name();
  port_name += std::string("_");
  port_name += std::to_string(pin_info.get_lsb());
  port_name += std::string("_");

  if (true == upper_port) {
    port_name += std::string("upper");
  } else {
    VTR_ASSERT_SAFE(false == upper_port);
    port_name += std::string("lower");
  }

  return port_name;
}

/*********************************************************************
 * Generate the port name for a grid in the context of a routing module
 * To keep a short and simple name, this function will not
 * include any grid coorindate information!
 **********************************************************************/
std::string generate_routing_module_grid_port_name(const size_t& width,
                                                   const size_t& height,
                                                   const int& subtile_index,
                                                   const e_side& side,
                                                   const BasicPort& pin_info) {
  /* For non-top netlist */
  std::string port_name = std::string("grid_");
  return port_name +
         generate_grid_port_name(width, height, subtile_index, side, pin_info);
}

/*********************************************************************
 * Generate the port name for a reserved sram port, i.e., BLB/WL port
 * When port_type is BLB, a string denoting to the reserved BLB port is
 *generated When port_type is WL, a string denoting to the reserved WL port is
 *generated
 *
 * DO NOT put any SRAM organization check codes HERE!!!
 * Even though the reserved BLB/WL ports are used by RRAM-based FPGA only,
 * try to keep this function does simple job.
 * Check codes should be added outside, when print the ports to files!!!
 *********************************************************************/
std::string generate_reserved_sram_port_name(
  const e_circuit_model_port_type& port_type) {
  VTR_ASSERT((port_type == CIRCUIT_MODEL_PORT_BLB) ||
             (port_type == CIRCUIT_MODEL_PORT_WL));

  if (CIRCUIT_MODEL_PORT_BLB == port_type) {
    return std::string("reserved_blb");
  }
  return std::string("reserved_wl");
}

/*********************************************************************
 * Generate the port name for a sram port, used for formal verification
 * The port name is named after the cell name of SRAM in circuit library
 *********************************************************************/
std::string generate_formal_verification_sram_port_name(
  const CircuitLibrary& circuit_lib, const CircuitModelId& sram_model) {
  std::string port_name =
    circuit_lib.model_name(sram_model) + std::string("_out_fm");

  return port_name;
}

/*********************************************************************
 * Generate the head port name of a configuration chain
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_configuration_chain_head_name() {
  return std::string(CONFIGURABLE_MEMORY_CHAIN_IN_NAME);
}

/*********************************************************************
 * Generate the tail port name of a configuration chain
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_configuration_chain_tail_name() {
  return std::string(CONFIGURABLE_MEMORY_CHAIN_OUT_NAME);
}

/*********************************************************************
 * Generate the memory output port name of a configurable memory
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_configurable_memory_data_out_name() {
  return std::string(CONFIGURABLE_MEMORY_DATA_OUT_NAME);
}

/*********************************************************************
 * Generate the inverted memory output port name of a configurable memory
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_configurable_memory_inverted_data_out_name() {
  return std::string(CONFIGURABLE_MEMORY_INVERTED_DATA_OUT_NAME);
}

/*********************************************************************
 * Generate the addr port (input) for a local decoder of a multiplexer
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_mux_local_decoder_addr_port_name() {
  return std::string("addr");
}

/*********************************************************************
 * Generate the data port (output) for a local decoder of a multiplexer
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_mux_local_decoder_data_port_name() {
  return std::string("data");
}

/*********************************************************************
 * Generate the inverted data port (output) for a local decoder of a multiplexer
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_mux_local_decoder_data_inv_port_name() {
  return std::string("data_inv");
}

/*********************************************************************
 * Generate the port name of a local configuration bus
 * TODO: This could be replaced as a constexpr string
 *********************************************************************/
std::string generate_local_config_bus_port_name() {
  return std::string("config_bus");
}

/*********************************************************************
 * Generate the port name for a regular sram port which appears in the
 * port list of a module
 * The port name is named after the cell name of SRAM in circuit library
 *********************************************************************/
std::string generate_sram_port_name(
  const e_config_protocol_type& sram_orgz_type,
  const e_circuit_model_port_type& port_type) {
  std::string port_name;

  switch (sram_orgz_type) {
    case CONFIG_MEM_FEEDTHROUGH:
      /* Two types of ports are available:
       * (1) BL indicates the mem port
       * (2) BLB indicates the inverted mem port
       *
       *               mem  mem_inv
       *               [0]   [0]
       *                |     |
       *                v     v
       *           +----------------+
       *           |   Virtual Mem  |
       *           +----------------+
       */
      if (CIRCUIT_MODEL_PORT_BL == port_type) {
        port_name = std::string(MEMORY_FEEDTHROUGH_DATA_IN_PORT_NAME);
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_PORT_BLB == port_type);
        port_name = std::string(MEMORY_FEEDTHROUGH_DATA_IN_INV_PORT_NAME);
      }
      break;
    case CONFIG_MEM_SCAN_CHAIN:
      /* Two types of ports are available:
       * (1) Head of a chain of Configuration-chain Flip-Flops (CCFFs), enabled
       * by port type of INPUT (2) Tail of a chian of Configuration-chain
       * Flip-flops (CCFFs), enabled by port type of OUTPUT
       *           +------+    +------+    +------+
       *  Head --->| CCFF |--->| CCFF |--->| CCFF |---> Tail
       *           +------+    +------+    +------+
       */
      if (CIRCUIT_MODEL_PORT_INPUT == port_type) {
        port_name = std::string("ccff_head");
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_PORT_OUTPUT == port_type);
        port_name = std::string("ccff_tail");
      }
      break;
    case CONFIG_MEM_STANDALONE:
    case CONFIG_MEM_QL_MEMORY_BANK:
    case CONFIG_MEM_MEMORY_BANK:
      /* Two types of ports are available:
       * (1) Bit Lines (BLs) of a SRAM cell, enabled by port type of BL
       * (2) Word Lines (WLs) of a SRAM cell, enabled by port type of WL
       *
       *           BL     WL        BL     WL        BL     WL
       *          [0]     [0]      [1]     [1]      [i]     [i]
       *            ^     ^          ^     ^          ^     ^
       *            |     |          |     |          |     |
       *           +----------+     +----------+     +----------+
       *           |   SRAM   |     |   SRAM   | ... |   SRAM   |
       *           +----------+     +----------+     +----------+
       */
      if (CIRCUIT_MODEL_PORT_BL == port_type) {
        port_name = std::string(MEMORY_BL_PORT_NAME);
      } else if (CIRCUIT_MODEL_PORT_WL == port_type) {
        port_name = std::string(MEMORY_WL_PORT_NAME);
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_PORT_WLR == port_type);
        port_name = std::string(MEMORY_WLR_PORT_NAME);
      }
      break;
    case CONFIG_MEM_FRAME_BASED:
      /* Only one input port is required, which is the address port
       *
       *        EN         ADDR      DATA_IN
       *         |          |           |
       *         v          v           v
       *     +---------------------------------+
       *     |       Frame decoder             |
       *     +---------------------------------+
       *
       */
      VTR_ASSERT(port_type == CIRCUIT_MODEL_PORT_INPUT);
      port_name = std::string(DECODER_ADDRESS_PORT_NAME);
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid type of SRAM organization !\n");
      exit(1);
  }

  return port_name;
}

/*********************************************************************
 * Generate the port name for a regular sram port which is an internal
 * wire of a module
 * The port name is named after the cell name of SRAM in circuit library
 *********************************************************************/
std::string generate_sram_local_port_name(
  const CircuitLibrary& circuit_lib, const CircuitModelId& sram_model,
  const e_config_protocol_type& sram_orgz_type,
  const e_circuit_model_port_type& port_type) {
  std::string port_name = circuit_lib.model_name(sram_model) + std::string("_");

  switch (sram_orgz_type) {
    case CONFIG_MEM_STANDALONE: {
      /* Two types of ports are available:
       * (1) Regular output of a SRAM, enabled by port type of INPUT
       * (2) Inverted output of a SRAM, enabled by port type of OUTPUT
       */
      if (CIRCUIT_MODEL_PORT_INPUT == port_type) {
        port_name += std::string("out_local_bus");
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_PORT_OUTPUT == port_type);
        port_name += std::string("outb_local_bus");
      }
      break;
    }
    case CONFIG_MEM_SCAN_CHAIN:
      /* Three types of ports are available:
       * (1) Input of Configuration-chain Flip-Flops (CCFFs), enabled by port
       * type of INPUT (2) Output of a chian of Configuration-chain Flip-flops
       * (CCFFs), enabled by port type of OUTPUT (2) Inverted output of a chian
       * of Configuration-chain Flip-flops (CCFFs), enabled by port type of
       * INOUT
       *           +------+    +------+    +------+
       *  Head --->| CCFF |--->| CCFF |--->| CCFF |---> Tail
       *           +------+    +------+    +------+
       */
      if (CIRCUIT_MODEL_PORT_INPUT == port_type) {
        port_name += std::string("ccff_in_local_bus");
      } else if (CIRCUIT_MODEL_PORT_OUTPUT == port_type) {
        port_name += std::string("ccff_out_local_bus");
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_PORT_INOUT == port_type);
        port_name += std::string("ccff_outb_local_bus");
      }
      break;
    case CONFIG_MEM_MEMORY_BANK: {
      /* Two types of ports are available:
       * (1) Regular output of a SRAM, enabled by port type of INPUT
       * (2) Inverted output of a SRAM, enabled by port type of OUTPUT
       */
      if (CIRCUIT_MODEL_PORT_INPUT == port_type) {
        port_name += std::string("out_local_bus");
      } else {
        VTR_ASSERT(CIRCUIT_MODEL_PORT_OUTPUT == port_type);
        port_name += std::string("outb_local_bus");
      }
      break;
    }
    default:
      VTR_LOG_ERROR("Invalid type of SRAM organization !\n");
      exit(1);
  }

  return port_name;
}

/*********************************************************************
 * Generate the BL/WL port names for the top-level module of an FPGA fabric
 * Each BL/WL bus drive a specific configuration region has an unique name
 *********************************************************************/
std::string generate_regional_blwl_port_name(
  const std::string& blwl_port_prefix, const ConfigRegionId& region_id) {
  return blwl_port_prefix + std::string("_config_region_") +
         std::to_string(size_t(region_id));
}

/*********************************************************************
 * Generate the module name for a shift register chain which configures BLs
 *********************************************************************/
std::string generate_bl_shift_register_module_name(
  const std::string& memory_model_name, const size_t& shift_register_size) {
  return std::string("bl_shift_register_") + memory_model_name +
         std::string("_size") + std::to_string(shift_register_size);
}

/*********************************************************************
 * Generate the module name for a shift register chain which configures WLs
 *********************************************************************/
std::string generate_wl_shift_register_module_name(
  const std::string& memory_model_name, const size_t& shift_register_size) {
  return std::string("wl_shift_register_") + memory_model_name +
         std::string("_size") + std::to_string(shift_register_size);
}

/*********************************************************************
 * Generate the port name for the input bus of a routing multiplexer
 * This is very useful in Verilog code generation where the inputs of
 * a routing multiplexer may come from different ports.
 * On the other side, the datapath input of a routing multiplexer
 * is defined as a bus port.
 * Therefore, to interface, a bus port is required, and this function
 * give a name to the bus port
 * To keep the bus port name unique to each multiplexer we will instance,
 * a mux_instance_id should be provided by user
 *********************************************************************/
std::string generate_mux_input_bus_port_name(const CircuitLibrary& circuit_lib,
                                             const CircuitModelId& mux_model,
                                             const size_t& mux_size,
                                             const size_t& mux_instance_id) {
  std::string postfix =
    std::string("_") + std::to_string(mux_instance_id) + std::string("_inbus");
  return generate_mux_subckt_name(circuit_lib, mux_model, mux_size, postfix);
}

/*********************************************************************
 * Generate the name of a bus port which is wired to the configuration
 * ports of a routing multiplexer
 * This port is supposed to be used locally inside a Verilog/SPICE module
 *********************************************************************/
std::string generate_mux_config_bus_port_name(const CircuitLibrary& circuit_lib,
                                              const CircuitModelId& mux_model,
                                              const size_t& mux_size,
                                              const size_t& bus_id,
                                              const bool& inverted) {
  std::string postfix = std::string("_configbus") + std::to_string(bus_id);
  /* Add a bar to the end of the name for inverted bus ports */
  if (true == inverted) {
    postfix += std::string("_b");
  }

  return generate_mux_subckt_name(circuit_lib, mux_model, mux_size, postfix);
}

/*********************************************************************
 * Generate the port name for a SRAM port of a circuit
 * This name is used for local wires that connecting SRAM ports
 * of a circuit model inside a Verilog/SPICE module
 * Note that the SRAM ports share the same naming
 * convention regardless of their configuration style
 *********************************************************************/
std::string generate_local_sram_port_name(
  const std::string& port_prefix, const size_t& instance_id,
  const e_circuit_model_port_type& port_type) {
  std::string port_name = port_prefix + std::string("_") +
                          std::to_string(instance_id) + std::string("_");

  if (CIRCUIT_MODEL_PORT_INPUT == port_type) {
    port_name += std::string("out");
  } else {
    VTR_ASSERT(CIRCUIT_MODEL_PORT_OUTPUT == port_type);
    port_name += std::string("outb");
  }

  return port_name;
}

/*********************************************************************
 * Generate the port name for a SRAM port of a routing multiplexer
 * This name is used for local wires that connecting SRAM ports
 * of routing multiplexers inside a Verilog/SPICE module
 * Note that the SRAM ports of routing multiplexers share the same naming
 * convention regardless of their configuration style
 **********************************************************************/
std::string generate_mux_sram_port_name(
  const CircuitLibrary& circuit_lib, const CircuitModelId& mux_model,
  const size_t& mux_size, const size_t& mux_instance_id,
  const e_circuit_model_port_type& port_type) {
  std::string prefix =
    generate_mux_subckt_name(circuit_lib, mux_model, mux_size, std::string());
  return generate_local_sram_port_name(prefix, mux_instance_id, port_type);
}

/*********************************************************************
 * Generate the netlist name of a logical tile
 **********************************************************************/
std::string generate_logical_tile_netlist_name(
  const std::string& prefix, const t_pb_graph_node* pb_graph_head,
  const std::string& postfix) {
  /* Add the name of physical block */
  std::string module_name =
    prefix + generate_physical_block_module_name(pb_graph_head->pb_type);

  module_name += postfix;

  return module_name;
}

/*********************************************************************
 * Generate the prefix for naming a grid block netlist or a grid module
 * This function will consider the io side and add it to the prefix
 **********************************************************************/
std::string generate_grid_block_prefix(const std::string& prefix,
                                       const e_side& io_side) {
  std::string block_prefix(prefix);

  if (NUM_SIDES != io_side) {
    SideManager side_manager(io_side);
    block_prefix += std::string(side_manager.to_string());
    block_prefix += std::string("_");
  }

  return block_prefix;
}

/*********************************************************************
 * Generate the netlist name of a grid block
 **********************************************************************/
std::string generate_grid_block_netlist_name(const std::string& block_name,
                                             const bool& is_block_io,
                                             const e_side& io_side,
                                             const std::string& postfix) {
  /* Add the name of physical block */
  std::string module_name(block_name);

  if ((true == is_block_io) && (NUM_SIDES != io_side)) {
    SideManager side_manager(io_side);
    module_name += std::string("_");
    module_name += std::string(side_manager.to_string());
  }

  module_name += postfix;

  return module_name;
}

/*********************************************************************
 * Generate the module name of a grid block
 **********************************************************************/
std::string generate_grid_block_module_name(const std::string& prefix,
                                            const std::string& block_name,
                                            const bool& is_block_io,
                                            const e_side& io_side) {
  std::string module_name(prefix);

  module_name += generate_grid_block_netlist_name(block_name, is_block_io,
                                                  io_side, std::string());

  return module_name;
}

/*********************************************************************
 * Generate the instance name for a programmable routing multiplexer module
 * in a Switch Block
 * To keep a unique name in each module and also consider unique routing
 *modules, please do NOT include any coordinates in the naming!!! Consider only
 *relative coordinate, such as side!
 ********************************************************************/
std::string generate_sb_mux_instance_name(const std::string& prefix,
                                          const e_side& sb_side,
                                          const size_t& track_id,
                                          const std::string& postfix) {
  std::string instance_name(prefix);
  instance_name += SideManager(sb_side).to_string();
  instance_name += std::string("_track_") + std::to_string(track_id);
  instance_name += postfix;

  return instance_name;
}

/*********************************************************************
 * Generate the instance name for a configurable memory module in a Switch Block
 * To keep a unique name in each module and also consider unique routing
 *modules, please do NOT include any coordinates in the naming!!! Consider only
 *relative coordinate, such as side!
 ********************************************************************/
std::string generate_sb_memory_instance_name(const std::string& prefix,
                                             const e_side& sb_side,
                                             const size_t& track_id,
                                             const std::string& postfix,
                                             const bool& feedthrough_memory) {
  std::string instance_name(prefix);
  if (feedthrough_memory) {
    instance_name = std::string("feedthrough_") + instance_name;
  }
  instance_name += SideManager(sb_side).to_string();
  instance_name += std::string("_track_") + std::to_string(track_id);
  instance_name += postfix;

  return instance_name;
}

/*********************************************************************
 * Generate the instance name for a programmable routing multiplexer module
 * in a Connection Block
 * To keep a unique name in each module and also consider unique routing
 *modules, please do NOT include any coordinates in the naming!!! Consider only
 *relative coordinate, such as side!
 ********************************************************************/
std::string generate_cb_mux_instance_name(const std::string& prefix,
                                          const e_side& cb_side,
                                          const size_t& pin_id,
                                          const std::string& postfix) {
  std::string instance_name(prefix);

  instance_name += SideManager(cb_side).to_string();
  instance_name += std::string("_ipin_") + std::to_string(pin_id);
  instance_name += postfix;

  return instance_name;
}

/*********************************************************************
 * Generate the instance name for a configurable memory module in a Connection
 *Block To keep a unique name in each module and also consider unique routing
 *modules, please do NOT include any coordinates in the naming!!! Consider only
 *relative coordinate, such as side!
 ********************************************************************/
std::string generate_cb_memory_instance_name(const std::string& prefix,
                                             const e_side& cb_side,
                                             const size_t& pin_id,
                                             const std::string& postfix,
                                             const bool& feedthrough_memory) {
  std::string instance_name(prefix);
  if (feedthrough_memory) {
    instance_name = std::string("feedthrough_") + instance_name;
  }

  instance_name += SideManager(cb_side).to_string();
  instance_name += std::string("_ipin_") + std::to_string(pin_id);
  instance_name += postfix;

  return instance_name;
}

/*********************************************************************
 * Generate the instance name for a programmable routing multiplexer
 * module in a physical block of a grid
 * To guarentee a unique name for pb_graph pin,
 * the instance name includes the index of parent node
 * as well as the port name and pin index of this pin
 *
 * Exceptions:
 * For OUTPUT ports, due to hierarchical module organization,
 * their parent nodes will be uniquified
 * So, we should not add any index here
 ********************************************************************/
std::string generate_pb_mux_instance_name(const std::string& prefix,
                                          t_pb_graph_pin* pb_graph_pin,
                                          const std::string& postfix) {
  std::string instance_name(prefix);
  instance_name += std::string(pb_graph_pin->parent_node->pb_type->name);

  if (IN_PORT == pb_graph_pin->port->type) {
    instance_name += std::string("_");
    instance_name += std::to_string(pb_graph_pin->parent_node->placement_index);
  }

  instance_name += std::string("_");
  instance_name += std::string(pb_graph_pin->port->name);
  instance_name += std::string("_");
  instance_name += std::to_string(pb_graph_pin->pin_number);
  instance_name += postfix;

  return instance_name;
}

/*********************************************************************
 * Generate the instance name for a configurable memory module in a
 * physical block of a grid
 * To guarentee a unique name for pb_graph pin,
 * the instance name includes the index of parent node
 * as well as the port name and pin index of this pin
 *
 * Exceptions:
 * For OUTPUT ports, due to hierarchical module organization,
 * their parent nodes will be uniquified
 * So, we should not add any index here
 ********************************************************************/
std::string generate_pb_memory_instance_name(const std::string& prefix,
                                             t_pb_graph_pin* pb_graph_pin,
                                             const std::string& postfix,
                                             const bool& feedthrough_memory) {
  std::string mid_name = feedthrough_memory ? "virtual_" : "";
  std::string instance_name(mid_name + prefix);
  instance_name += std::string(pb_graph_pin->parent_node->pb_type->name);

  if (IN_PORT == pb_graph_pin->port->type) {
    instance_name += std::string("_");
    instance_name += std::to_string(pb_graph_pin->parent_node->placement_index);
  }

  instance_name += std::string("_");
  instance_name += std::string(pb_graph_pin->port->name);
  instance_name += std::string("_");
  instance_name += std::to_string(pb_graph_pin->pin_number);
  instance_name += postfix;

  return instance_name;
}

/*********************************************************************
 * Generate the instance name of a grid block
 **********************************************************************/
std::string generate_grid_block_instance_name(
  const std::string& prefix, const std::string& block_name,
  const bool& is_block_io, const e_side& io_side,
  const vtr::Point<size_t>& grid_coord) {
  std::string module_name(prefix);

  module_name += generate_grid_block_netlist_name(block_name, is_block_io,
                                                  io_side, std::string());
  module_name += std::string("_");
  module_name += std::to_string(grid_coord.x());
  module_name += std::string("__");
  module_name += std::to_string(grid_coord.y());
  module_name += std::string("_");

  return module_name;
}

/*********************************************************************
 * Generate the module name of a logical block type (pb_type)
 * Since the logical block does not carry any physical attributes,
 * this logical block will have a common prefix 'logical_type'
 * To ensure a unique name for each physical block inside the graph of complex
 *blocks (pb_graph_nodes), this function trace backward to the top-level node in
 *the graph and add the name of these parents The final name will be in the
 *following format:
 * <top_physical_block_name>_<mode_name>_<parent_physical_block_name> ...
 *<current_physical_block_name>
 *
 * TODO: to make sure the length of this name does not exceed the size of
 * chars in a line of a file!!!
 **********************************************************************/
std::string generate_physical_block_module_name(t_pb_type* physical_pb_type) {
  std::string module_name(physical_pb_type->name);

  t_pb_type* parent_pb_type = physical_pb_type;

  /* Backward trace until we meet the top-level pb_type */
  while (1) {
    /* If there is no parent mode, this is a top-level pb_type, quit the loop
     * here */
    t_mode* parent_mode = parent_pb_type->parent_mode;
    if (NULL == parent_mode) {
      break;
    }

    /* Add the mode name to the module name */
    module_name = std::string("mode_") + std::string(parent_mode->name) +
                  std::string("__") + module_name;

    /* Backtrace to the upper level */
    parent_pb_type = parent_mode->parent_pb_type;

    /* If there is no parent pb_type, this is a top-level pb_type, quit the loop
     * here */
    if (NULL == parent_pb_type) {
      break;
    }

    /* Add the current pb_type name to the module name */
    module_name =
      std::string(parent_pb_type->name) + std::string("_") + module_name;
  }

  /* Exception for top-level pb_type: add an virtual mode name (same name as the
   * pb_type) This is to follow the naming convention as non top-level pb_types
   * In addition, the name can be really unique, being different than the grid
   * blocks
   */
  if (NULL == physical_pb_type->parent_mode) {
    module_name += std::string("_mode_") + std::string(physical_pb_type->name) +
                   std::string("_");
  }

  /* Add the prefix */
  module_name = LOGICAL_MODULE_NAME_PREFIX + module_name;

  return module_name;
}

/*********************************************************************
 * Generate the instance name for physical block with a given index
 **********************************************************************/
std::string generate_physical_block_instance_name(t_pb_type* pb_type,
                                                  const size_t& index) {
  std::string instance_name = generate_physical_block_module_name(pb_type);
  /* Add index to the name */
  instance_name += std::string("_");
  instance_name += std::to_string(index);

  return instance_name;
}

/********************************************************************
 * This function try to infer if a grid locates at the border of a
 * FPGA fabric, i.e., TOP/RIGHT/BOTTOM/LEFT sides
 * 1. if this grid is on the border, it will return the side it locates,
 * 2. if this grid is in the center, it will return an valid value NUM_SIDES
 *
 * In this function, we assume that the corner grids are actually empty!
 *
 *   +-------+  +----------------------------+  +-------+
 *   | EMPTY |  |      TOP side I/O          |  | EMPTY |
 *   +-------+  +----------------------------+  +-------+
 *
 *   +-------+  +----------------------------+  +-------+
 *   |       |  |                            |  |       |
 *   |       |  |                            |  |       |
 *   |       |  |                            |  |       |
 *   | LEFT  |  |                            |  | RIGHT |
 *   | side  |  |         Core grids         |  | side  |
 *   | I/O   |  |                            |  |  I/O  |
 *   |       |  |                            |  |       |
 *   |       |  |                            |  |       |
 *   |       |  |                            |  |       |
 *   |       |  |                            |  |       |
 *   +-------+  +----------------------------+  +-------+
 *
 *   +-------+  +----------------------------+  +-------+
 *   | EMPTY |  |    BOTTOM side I/O         |  | EMPTY |
 *   +-------+  +----------------------------+  +-------+
 *******************************************************************/
e_side find_grid_border_side(const vtr::Point<size_t>& device_size,
                             const vtr::Point<size_t>& grid_coordinate) {
  e_side grid_side = NUM_SIDES;

  if (device_size.y() - 1 == grid_coordinate.y()) {
    return TOP;
  }
  if (device_size.x() - 1 == grid_coordinate.x()) {
    return RIGHT;
  }
  if (0 == grid_coordinate.y()) {
    return BOTTOM;
  }
  if (0 == grid_coordinate.x()) {
    return LEFT;
  }

  return grid_side;
}

/********************************************************************
 * This function try to infer if a grid locates at the border of the
 * core FPGA fabric, i.e., TOP/RIGHT/BOTTOM/LEFT sides
 * 1. if this grid is on the border and it matches the given side, return true,
 * 2. if this grid is in the center, return false
 *
 * In this function, we assume that the corner grids are actually empty!
 *
 *   +-------+  +----------------------------+  +-------+
 *   | EMPTY |  |      TOP side I/O          |  | EMPTY |
 *   +-------+  +----------------------------+  +-------+
 *
 *   +-------+  +----------------------------+  +-------+
 *   |       |  |          TOP               |  |       |
 *   |       |  |----------------------------|  |       |
 *   |       |  |      |              |      |  |       |
 *   | LEFT  |  |      |              |      |  | RIGHT |
 *   | side  |  | LEFT |  Core grids  | RIGHT|  | side  |
 *   | I/O   |  |      |              |      |  |  I/O  |
 *   |       |  |      |              |      |  |       |
 *   |       |  |      |              |      |  |       |
 *   |       |  |---------------------|      |  |       |
 *   |       |  |          BOTTOM     |      |  |       |
 *   +-------+  +----------------------------+  +-------+
 *
 *   +-------+  +----------------------------+  +-------+
 *   | EMPTY |  |    BOTTOM side I/O         |  | EMPTY |
 *   +-------+  +----------------------------+  +-------+
 *
 *   Note: for the blocks on the four corners of the core grids
 *   Please refer to the figure above to infer its border_side
 *******************************************************************/
bool is_core_grid_on_given_border_side(
  const vtr::Point<size_t>& device_size,
  const vtr::Point<size_t>& grid_coordinate, const e_side& border_side) {
  if ((device_size.y() - 2 == grid_coordinate.y()) && (TOP == border_side)) {
    return true;
  }
  if ((device_size.x() - 2 == grid_coordinate.x()) && (RIGHT == border_side)) {
    return true;
  }
  if ((1 == grid_coordinate.y()) && (BOTTOM == border_side)) {
    return true;
  }
  if ((1 == grid_coordinate.x()) && (LEFT == border_side)) {
    return true;
  }

  return false;
}

/*********************************************************************
 * Generate the port name of a Verilog module describing a pb_type
 * The name convention is
 * <pb_type_name>_<port_name>
 ********************************************************************/
std::string generate_pb_type_port_name(t_pb_type* pb_type,
                                       t_port* pb_type_port) {
  std::string port_name;

  port_name = std::string(pb_type->name) + std::string("_") +
              std::string(pb_type_port->name);

  return port_name;
}

/*********************************************************************
 * Generate the port name of a Verilog module describing a pb_type
 * The name convention is
 * <pb_type_name>_<port_name>
 *
 * This is a wrapper on the generate_pb_type_port_name() function
 * which can infer the parent_pb_type
 ********************************************************************/
std::string generate_pb_type_port_name(t_port* pb_type_port) {
  return generate_pb_type_port_name(pb_type_port->parent_pb_type, pb_type_port);
}

/*********************************************************************
 * Generate the global I/O port name of a Verilog module
 * This is mainly used by I/O circuit models
 ********************************************************************/
std::string generate_fpga_global_io_port_name(
  const std::string& prefix, const CircuitLibrary& circuit_lib,
  const CircuitModelId& circuit_model, const CircuitPortId& circuit_port) {
  std::string port_name(prefix);

  port_name += circuit_lib.model_name(circuit_model);
  port_name += std::string("_");
  port_name += circuit_lib.port_prefix(circuit_port);

  return port_name;
}

/*********************************************************************
 * Generate the module name for the top-level module
 * The top-level module is actually the FPGA fabric
 * We give a fixed name here, because it is independent from benchmark file
 ********************************************************************/
std::string generate_fpga_top_module_name() {
  return std::string(FPGA_TOP_MODULE_NAME);
}

/*********************************************************************
 * Generate the module name for the fpga core module
 * We give a fixed name here, because it is independent from benchmark file
 ********************************************************************/
std::string generate_fpga_core_module_name() {
  return std::string(FPGA_CORE_MODULE_NAME);
}

/*********************************************************************
 * Generate the module name for the fpga core module
 * We give a fixed name here, because it is independent from benchmark file
 ********************************************************************/
std::string generate_fpga_core_instance_name() {
  return std::string(FPGA_CORE_INSTANCE_NAME);
}

/*********************************************************************
 * Generate the netlist name for the top-level module
 * The top-level module is actually the FPGA fabric
 * We give a fixed name here, because it is independent from benchmark file
 ********************************************************************/
std::string generate_fpga_top_netlist_name(const std::string& postfix) {
  return std::string(FPGA_TOP_MODULE_NAME + postfix);
}

/*********************************************************************
 * Generate the netlist name for the top-level module
 * The top-level module is actually the FPGA fabric
 * We give a fixed name here, because it is independent from benchmark file
 ********************************************************************/
std::string generate_fpga_core_netlist_name(const std::string& postfix) {
  return std::string(FPGA_CORE_MODULE_NAME + postfix);
}

/*********************************************************************
 * Generate the module name for a constant generator
 * either VDD or GND, depending on the input argument
 ********************************************************************/
std::string generate_const_value_module_name(const size_t& const_val) {
  if (0 == const_val) {
    return std::string("const0");
  }

  VTR_ASSERT(1 == const_val);
  return std::string("const1");
}

/*********************************************************************
 * Generate the output port name for a constant generator module
 * either VDD or GND, depending on the input argument
 ********************************************************************/
std::string generate_const_value_module_output_port_name(
  const size_t& const_val) {
  return generate_const_value_module_name(const_val);
}

/*********************************************************************
 * Generate the analysis SDC file name
 * The format is
 *   <circuit_name>_<postfix>
 ********************************************************************/
std::string generate_analysis_sdc_file_name(
  const std::string& circuit_name, const std::string& file_name_postfix) {
  return circuit_name + "_" + file_name_postfix;
}

} /* end namespace openfpga */
