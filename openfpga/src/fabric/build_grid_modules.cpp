/********************************************************************
 * This file includes functions to print Verilog modules for a Grid
 * (CLBs, I/Os, heterogeneous blocks etc.)
 *******************************************************************/
#include <ctime>
#include <vector>

/* Headers from vtrutil library */
#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_geometry.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "build_grid_module_duplicated_pins.h"
#include "build_grid_module_utils.h"
#include "build_grid_modules.h"
#include "build_memory_modules.h"
#include "circuit_library_utils.h"
#include "module_manager_utils.h"
#include "openfpga_interconnect_types.h"
#include "openfpga_naming.h"
#include "openfpga_physical_tile_utils.h"
#include "openfpga_reserved_words.h"
#include "pb_graph_utils.h"
#include "pb_type_utils.h"
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Add ports/pins to a grid module
 * This function will iterate over all the pins that are defined
 * in type_descripter and give a name by its height, side and index
 *
 * In particular, for I/O grid, only part of the ports on required
 * on a specific side.
 *******************************************************************/
static void add_grid_module_pb_type_ports(
  ModuleManager& module_manager, const ModuleId& grid_module,
  const VprDeviceAnnotation& vpr_device_annotation,
  t_physical_tile_type_ptr grid_type_descriptor,
  const TileAnnotation& tile_annotation, const e_side& border_side,
  const bool& perimeter_cb) {
  /* Ensure that we have a valid grid_type_descriptor */
  VTR_ASSERT(nullptr != grid_type_descriptor);

  /* Find the pin side for I/O grids*/
  std::vector<e_side> grid_pin_sides;
  /* For I/O grids, we care only one side
   * Otherwise, we will iterate all the 4 sides
   */
  if (true == is_io_type(grid_type_descriptor)) {
    grid_pin_sides = find_grid_module_pin_sides(grid_type_descriptor,
                                                border_side, perimeter_cb);
  } else {
    grid_pin_sides = {TOP, RIGHT, BOTTOM, LEFT};
  }

  /* Create a map between pin class type and grid pin direction */
  std::map<e_pin_type, ModuleManager::e_module_port_type> pin_type2type_map;
  pin_type2type_map[RECEIVER] = ModuleManager::MODULE_INPUT_PORT;
  pin_type2type_map[DRIVER] = ModuleManager::MODULE_OUTPUT_PORT;

  /* Iterate over sides, height and pins */
  for (const e_side& side : grid_pin_sides) {
    for (int iwidth = 0; iwidth < grid_type_descriptor->width; ++iwidth) {
      for (int iheight = 0; iheight < grid_type_descriptor->height; ++iheight) {
        for (int ipin = 0; ipin < grid_type_descriptor->num_pins; ++ipin) {
          if (true !=
              grid_type_descriptor->pinloc[iwidth][iheight][side][ipin]) {
            continue;
          }
          /* Reach here, it means this pin is on this side */
          int class_id = grid_type_descriptor->pin_class[ipin];
          e_pin_type pin_class_type =
            grid_type_descriptor->class_inf[class_id].type;
          /* Generate the pin name,
           * we give a empty coordinate but it will not be used (see details in
           * the function
           */
          BasicPort pin_info =
            vpr_device_annotation.physical_tile_pin_port_info(
              grid_type_descriptor, ipin);
          VTR_ASSERT(true == pin_info.is_valid());
          int subtile_index =
            vpr_device_annotation.physical_tile_pin_subtile_index(
              grid_type_descriptor, ipin);
          VTR_ASSERT(OPEN != subtile_index &&
                     subtile_index < grid_type_descriptor->capacity);
          std::string port_name = generate_grid_port_name(
            iwidth, iheight, subtile_index, side, pin_info);
          /* If the port is required to be merged, we use a special index
           * index */
          if (tile_annotation.is_tile_port_to_merge(
                std::string(grid_type_descriptor->name), pin_info.get_name())) {
            if (subtile_index == 0) {
              port_name = generate_grid_port_name(0, 0, 0, TOP, pin_info);
            } else {
              continue;
            }
          }
          BasicPort grid_port(port_name, 0, 0);
          /* Add the port to the module */
          ModulePortId grid_port_id = module_manager.add_port(
            grid_module, grid_port, pin_type2type_map[pin_class_type]);
          /* Set port side */
          module_manager.set_port_side(grid_module, grid_port_id, side);
        }
      }
    }
  }
}

/********************************************************************
 * Add module nets to connect ports/pins of a grid module
 * to its child modules
 * This function will iterate over all the pins that are defined
 * in type_descripter and find the corresponding pin in the top
 * pb_graph_node of the grid
 *******************************************************************/
static void add_grid_module_nets_connect_pb_type_ports(
  ModuleManager& module_manager, const ModuleId& grid_module,
  const ModuleId& child_module, const size_t& child_instance,
  const t_sub_tile& sub_tile, const VprDeviceAnnotation& vpr_device_annotation,
  t_physical_tile_type_ptr grid_type_descriptor,
  const TileAnnotation& tile_annotation, const e_side& border_side,
  const bool& perimeter_cb) {
  /* Ensure that we have a valid grid_type_descriptor */
  VTR_ASSERT(nullptr != grid_type_descriptor);

  /* FIXME: Currently support only 1 equivalent site! Should clarify this
   * limitation in documentation! */
  VTR_ASSERT(sub_tile.equivalent_sites.size() == 1);
  t_logical_block_type_ptr lb_type = sub_tile.equivalent_sites[0];
  t_pb_graph_node* top_pb_graph_node = lb_type->pb_graph_head;
  VTR_ASSERT(nullptr != top_pb_graph_node);
  size_t child_inst_subtile_index = sub_tile.capacity.low + child_instance;

  for (int iport = 0; iport < top_pb_graph_node->num_input_ports; ++iport) {
    for (int ipin = 0; ipin < top_pb_graph_node->num_input_pins[iport];
         ++ipin) {
      add_grid_module_net_connect_pb_graph_pin(
        module_manager, grid_module, child_module, child_instance,
        child_inst_subtile_index, vpr_device_annotation, grid_type_descriptor,
        tile_annotation, &(top_pb_graph_node->input_pins[iport][ipin]),
        border_side, perimeter_cb, INPUT2INPUT_INTERC);
    }
  }

  for (int iport = 0; iport < top_pb_graph_node->num_output_ports; ++iport) {
    for (int ipin = 0; ipin < top_pb_graph_node->num_output_pins[iport];
         ++ipin) {
      add_grid_module_net_connect_pb_graph_pin(
        module_manager, grid_module, child_module, child_instance,
        child_inst_subtile_index, vpr_device_annotation, grid_type_descriptor,
        tile_annotation, &(top_pb_graph_node->output_pins[iport][ipin]),
        border_side, perimeter_cb, OUTPUT2OUTPUT_INTERC);
    }
  }

  for (int iport = 0; iport < top_pb_graph_node->num_clock_ports; ++iport) {
    for (int ipin = 0; ipin < top_pb_graph_node->num_clock_pins[iport];
         ++ipin) {
      add_grid_module_net_connect_pb_graph_pin(
        module_manager, grid_module, child_module, child_instance,
        child_inst_subtile_index, vpr_device_annotation, grid_type_descriptor,
        tile_annotation, &(top_pb_graph_node->clock_pins[iport][ipin]),
        border_side, perimeter_cb, INPUT2INPUT_INTERC);
    }
  }
}

/********************************************************************
 * Add module nets between primitive module and its internal circuit module
 * This is only applicable to the primitive module of a grid
 *******************************************************************/
static void add_primitive_module_fpga_global_io_port(
  ModuleManager& module_manager, const ModuleId& primitive_module,
  const ModuleId& logic_module, const size_t& logic_instance_id,
  const ModuleManager::e_module_port_type& module_io_port_type,
  const CircuitLibrary& circuit_lib, const CircuitModelId& primitive_model,
  const CircuitPortId& circuit_port) {
  BasicPort module_port(generate_fpga_global_io_port_name(
                          std::string(GIO_INOUT_PREFIX), circuit_lib,
                          primitive_model, circuit_port),
                        circuit_lib.port_size(circuit_port));
  ModulePortId primitive_io_port_id =
    module_manager.add_port(primitive_module, module_port, module_io_port_type);
  /* Set if the port is mappable or not */
  if (true == circuit_lib.port_is_data_io(circuit_port)) {
    module_manager.set_port_is_mappable_io(primitive_module,
                                           primitive_io_port_id, true);
  }

  ModulePortId logic_io_port_id = module_manager.find_module_port(
    logic_module, circuit_lib.port_prefix(circuit_port));
  BasicPort logic_io_port =
    module_manager.module_port(logic_module, logic_io_port_id);
  VTR_ASSERT(logic_io_port.get_width() == module_port.get_width());

  /* Wire the GPIO port from primitive_module to the logic module!*/
  for (size_t pin_id = 0; pin_id < module_port.pins().size(); ++pin_id) {
    if ((ModuleManager::MODULE_GPIO_PORT == module_io_port_type) ||
        (ModuleManager::MODULE_GPIN_PORT == module_io_port_type)) {
      bool net_exist = true;
      /* If the source port has already a net to drive, we just update the net
       * sinks */
      ModuleNetId net = module_manager.module_instance_port_net(
        primitive_module, primitive_module, 0, primitive_io_port_id,
        module_port.pins()[pin_id]);
      if (net == ModuleNetId::INVALID()) {
        net_exist = false;
        net = module_manager.create_module_net(primitive_module);
      }

      if (false == net_exist) {
        module_manager.add_module_net_source(
          primitive_module, net, primitive_module, 0, primitive_io_port_id,
          module_port.pins()[pin_id]);
      }
      module_manager.add_module_net_sink(primitive_module, net, logic_module,
                                         logic_instance_id, logic_io_port_id,
                                         logic_io_port.pins()[pin_id]);
    } else {
      bool net_exist = true;
      /* If the source port has already a net to drive, we just update the net
       * sinks */
      ModuleNetId net = module_manager.module_instance_port_net(
        primitive_module, logic_module, logic_instance_id, logic_io_port_id,
        logic_io_port.pins()[pin_id]);
      if (net == ModuleNetId::INVALID()) {
        net_exist = false;
        net = module_manager.create_module_net(primitive_module);
      }

      VTR_ASSERT(ModuleManager::MODULE_GPOUT_PORT == module_io_port_type);

      if (false == net_exist) {
        module_manager.add_module_net_source(
          primitive_module, net, logic_module, logic_instance_id,
          logic_io_port_id, logic_io_port.pins()[pin_id]);
      }
      module_manager.add_module_net_sink(
        primitive_module, net, primitive_module, 0, primitive_io_port_id,
        module_port.pins()[pin_id]);
    }
  }
}

/********************************************************************
 * Print Verilog modules of a primitive node in the pb_graph_node graph
 * This generic function can support all the different types of primitive nodes
 * i.e., Look-Up Tables (LUTs), Flip-flops (FFs) and hard logic blocks such as
 *adders.
 *
 * The Verilog module will consist of two parts:
 * 1. Logic module of the primitive node
 *    This module performs the logic function of the block
 * 2. Memory module of the primitive node
 *    This module stores the configuration bits for the logic module
 *    if the logic module is a programmable resource, such as LUT
 *
 * Verilog module structure:
 *
 *       Primitive block
 *     +---------------------------------------+
 *     |                                       |
 *     |      +---------+    +---------+       |
 *  in |----->|         |--->|         |<------|configuration lines
 *     |      |  Logic  |... |  Memory |       |
 *  out|<-----|         |--->|         |       |
 *     |      +---------+    +---------+       |
 *     |                                       |
 *     +---------------------------------------+
 *
 *******************************************************************/
static void build_primitive_block_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, t_pb_graph_node* primitive_pb_graph_node,
  const bool& group_config_block, const bool& verbose) {
  /* Ensure a valid pb_graph_node */
  VTR_ASSERT(nullptr != primitive_pb_graph_node);

  /* Find the circuit model id linked to the pb_graph_node */
  const CircuitModelId& primitive_model =
    device_annotation.pb_type_circuit_model(primitive_pb_graph_node->pb_type);

  /* Generate the module name for this primitive pb_graph_node*/
  std::string primitive_module_name =
    generate_physical_block_module_name(primitive_pb_graph_node->pb_type);

  VTR_LOGV(verbose, "Building primitive module '%s'...\n",
           primitive_module_name.c_str());

  /* Create a module of the primitive LUT and register it to module manager */
  ModuleId primitive_module = module_manager.add_module(primitive_module_name);
  /* Ensure that the module has been created and thus unique! */
  VTR_ASSERT(ModuleId::INVALID() != primitive_module);

  /* Label module usage */
  module_manager.set_module_usage(primitive_module, ModuleManager::MODULE_GRID);

  /* Note: to cooperate with the pb_type hierarchy and connections, we add the
   * port of primitive pb_type here. Since we have linked pb_type ports to
   * circuit models when setting up FPGA-X2P, no ports of the circuit model will
   * be missing here
   */
  add_primitive_pb_type_ports_to_module_manager(
    module_manager, primitive_module, primitive_pb_graph_node->pb_type,
    device_annotation);

  /* Add configuration ports */
  /* Shared SRAM ports*/
  size_t num_shared_config_bits = find_circuit_num_shared_config_bits(
    circuit_lib, primitive_model, sram_orgz_type);
  if (0 < num_shared_config_bits) {
    /* Check: this SRAM organization type must be memory-bank ! */
    VTR_ASSERT(CONFIG_MEM_MEMORY_BANK == sram_orgz_type ||
               CONFIG_MEM_QL_MEMORY_BANK == sram_orgz_type);
    /* Generate a list of ports */
    add_reserved_sram_ports_to_module_manager(module_manager, primitive_module,
                                              num_shared_config_bits);
  }

  /* Regular (independent) SRAM ports */
  e_config_protocol_type mem_module_type =
    group_config_block ? CONFIG_MEM_FEEDTHROUGH : sram_orgz_type;
  size_t num_config_bits =
    find_circuit_num_config_bits(mem_module_type, circuit_lib, primitive_model);
  if (0 < num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, primitive_module,
                                     circuit_lib, sram_model, mem_module_type,
                                     num_config_bits);
  }

  /* Find the module id in the module manager */
  ModuleId logic_module =
    module_manager.find_module(circuit_lib.model_name(primitive_model));
  VTR_ASSERT(ModuleId::INVALID() != logic_module);
  size_t logic_instance_id =
    module_manager.num_instance(primitive_module, logic_module);
  /* Add the logic module as a child of primitive module */
  module_manager.add_child_module(primitive_module, logic_module);

  /* Add nets to connect the logic model ports to pb_type ports */
  add_primitive_pb_type_module_nets(
    module_manager, primitive_module, logic_module, logic_instance_id,
    circuit_lib, primitive_pb_graph_node->pb_type, device_annotation);

  /* Add the associated memory module as a child of primitive module */
  std::string memory_module_name = generate_memory_module_name(
    circuit_lib, primitive_model, sram_model,
    std::string(MEMORY_MODULE_POSTFIX), group_config_block);
  ModuleId memory_module = module_manager.find_module(memory_module_name);

  /* If there is no memory module required, we can skip the assocated net
   * addition */
  if (ModuleId::INVALID() != memory_module) {
    size_t memory_instance_id =
      module_manager.num_instance(primitive_module, memory_module);
    /* Add the memory module as a child of primitive module */
    module_manager.add_child_module(primitive_module, memory_module);
    /* Set an instance name to bind to a block in bitstream generation */
    module_manager.set_child_instance_name(
      primitive_module, memory_module, memory_instance_id, memory_module_name);

    /* Add nets to connect regular and mode-select SRAM ports to the SRAM port
     * of memory module */
    add_module_nets_between_logic_and_memory_sram_bus(
      module_manager, primitive_module, logic_module, logic_instance_id,
      memory_module, memory_instance_id, circuit_lib, primitive_model);
    /* Record memory-related information */
    size_t config_child_id = module_manager.num_configurable_children(
      primitive_module, ModuleManager::e_config_child_type::LOGICAL);
    module_manager.add_configurable_child(
      primitive_module, memory_module, memory_instance_id,
      group_config_block ? ModuleManager::e_config_child_type::LOGICAL
                         : ModuleManager::e_config_child_type::UNIFIED);
    /* For logical memory, define the physical memory here */
    if (group_config_block) {
      std::string physical_memory_module_name =
        generate_memory_module_name(circuit_lib, primitive_model, sram_model,
                                    std::string(MEMORY_MODULE_POSTFIX), false);
      ModuleId physical_memory_module =
        module_manager.find_module(physical_memory_module_name);
      VTR_LOGV(verbose,
               "Mapping feedthrough memory module '%s' to physical memory "
               "module '%s'...\n",
               memory_module_name.c_str(), physical_memory_module_name.c_str());
      VTR_ASSERT(module_manager.valid_module_id(physical_memory_module));
      module_manager.set_logical2physical_configurable_child(
        primitive_module, config_child_id, physical_memory_module);
      module_manager.set_logical2physical_configurable_child_instance_name(
        primitive_module, config_child_id, physical_memory_module_name);
    }
  }

  /* Add all the nets to connect configuration ports from memory module to
   * primitive modules This is a one-shot addition that covers all the memory
   * modules in this primitive module!
   */
  if (0 < module_manager.num_configurable_children(
            primitive_module, ModuleManager::e_config_child_type::LOGICAL)) {
    add_module_nets_memory_config_bus(
      module_manager, decoder_lib, primitive_module, mem_module_type,
      circuit_lib.design_tech_type(sram_model),
      ModuleManager::e_config_child_type::LOGICAL);
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, primitive_module);

  /* Find the inout ports required by the primitive node, and add them to the
   * module This is mainly due to the I/O blocks, which have inout ports for the
   * top-level fabric
   */
  for (const auto& port :
       circuit_lib.model_global_ports(primitive_model, false)) {
    if ((CIRCUIT_MODEL_PORT_INOUT == circuit_lib.port_type(port)) &&
        (true == circuit_lib.port_is_io(port))) {
      add_primitive_module_fpga_global_io_port(
        module_manager, primitive_module, logic_module, logic_instance_id,
        ModuleManager::MODULE_GPIO_PORT, circuit_lib, primitive_model, port);
    } else if ((CIRCUIT_MODEL_PORT_INPUT == circuit_lib.port_type(port)) &&
               (true == circuit_lib.port_is_io(port))) {
      add_primitive_module_fpga_global_io_port(
        module_manager, primitive_module, logic_module, logic_instance_id,
        ModuleManager::MODULE_GPIN_PORT, circuit_lib, primitive_model, port);
    } else if (CIRCUIT_MODEL_PORT_OUTPUT == circuit_lib.port_type(port)) {
      add_primitive_module_fpga_global_io_port(
        module_manager, primitive_module, logic_module, logic_instance_id,
        ModuleManager::MODULE_GPOUT_PORT, circuit_lib, primitive_model, port);
    }
  }

  VTR_LOGV(verbose, "Done\n");
}

/********************************************************************
 * This function add a net for a pin-to-pin connection defined in pb_graph
 * It supports two cases for the pin-to-pin connection
 * 1. The net source is a pb_graph_pin while the net sink is a pin of an
 *interconnection
 * 2. The net source is a pin of an interconnection while the net sink a
 *pb_graph_pin The type is enabled by an argument pin2pin_interc_type
 *******************************************************************/
static void add_module_pb_graph_pin2pin_net(
  ModuleManager& module_manager, const ModuleId& pb_module,
  const ModuleId& interc_module, const size_t& interc_instance,
  const std::string& interc_port_name, const size_t& interc_pin_id,
  t_pb_graph_pin* pb_graph_pin,
  const enum e_pin2pin_interc_type& pin2pin_interc_type) {
  ModuleNetId pin2pin_net = module_manager.create_module_net(pb_module);

  /* Find port and pin ids for the module, which is the parent of pb_graph_pin
   */
  t_pb_type* pin_pb_type = pb_graph_pin->parent_node->pb_type;
  /* Find the module contains the source pin */
  ModuleId pin_pb_type_module = module_manager.find_module(
    generate_physical_block_module_name(pin_pb_type));
  VTR_ASSERT(true == module_manager.valid_module_id(pin_pb_type_module));
  size_t pin_pb_type_instance =
    0; /* Deposite the instance with a zero, which is the default value is the
          source module is actually pb_module itself */
  if (pin_pb_type_module != pb_module) {
    pin_pb_type_instance = pb_graph_pin->parent_node->placement_index;
    /* Ensure this is an valid instance */
    VTR_ASSERT(pin_pb_type_instance <
               module_manager.num_instance(pb_module, pin_pb_type_module));
  }
  ModulePortId pin_module_port_id = module_manager.find_module_port(
    pin_pb_type_module, generate_pb_type_port_name(pb_graph_pin->port));
  VTR_ASSERT(true == module_manager.valid_module_port_id(pin_pb_type_module,
                                                         pin_module_port_id));
  size_t pin_module_pin_id = pb_graph_pin->pin_number;
  /* Ensure this is an valid pin index */
  VTR_ASSERT(pin_module_pin_id <
             module_manager.module_port(pin_pb_type_module, pin_module_port_id)
               .get_width());

  /* Find port and pin ids for the interconnection module */
  ModulePortId interc_port_id =
    module_manager.find_module_port(interc_module, interc_port_name);
  VTR_ASSERT(
    true == module_manager.valid_module_port_id(interc_module, interc_port_id));
  /* Ensure this is an valid pin index */
  VTR_ASSERT(
    interc_pin_id <
    module_manager.module_port(interc_module, interc_port_id).get_width());

  /* Add net sources and sinks:
   * For input-to-input connection, net_source is pin_graph_pin, while net_sink
   * is interc pin For output-to-output connection, net_source is interc pin,
   * while net_sink is pin_graph pin
   */
  switch (pin2pin_interc_type) {
    case INPUT2INPUT_INTERC:
      module_manager.add_module_net_source(
        pb_module, pin2pin_net, pin_pb_type_module, pin_pb_type_instance,
        pin_module_port_id, pin_module_pin_id);
      module_manager.add_module_net_sink(pb_module, pin2pin_net, interc_module,
                                         interc_instance, interc_port_id,
                                         interc_pin_id);
      break;
    case OUTPUT2OUTPUT_INTERC:
      module_manager.add_module_net_source(pb_module, pin2pin_net,
                                           interc_module, interc_instance,
                                           interc_port_id, interc_pin_id);
      module_manager.add_module_net_sink(
        pb_module, pin2pin_net, pin_pb_type_module, pin_pb_type_instance,
        pin_module_port_id, pin_module_pin_id);
      break;
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__,
                     "Invalid pin-to-pin interconnection type!\n");
      exit(1);
  }
}

/********************************************************************
 * We check output_pins of cur_pb_graph_node and its the input_edges
 * Built the interconnections between outputs of cur_pb_graph_node and outputs
 *of child_pb_graph_node src_pb_graph_node.[in|out]_pins ----------------->
 *des_pb_graph_node.[in|out]pins
 *                                        /|\
 *                                         |
 *                         input_pins,   edges,       output_pins
 *
 * This function does the following task:
 * 1. identify pin interconnection type,
 * 2. Identify the number of fan-in (Consider interconnection edges of only
 *selected mode)
 * 3. Add mux/direct connection as a child module to pb_module
 * 4. Add nets related to the mux/direction
 *******************************************************************/
static void add_module_pb_graph_pin_interc(
  ModuleManager& module_manager, const ModuleId& pb_module,
  std::vector<ModuleId>& memory_modules, std::vector<size_t>& memory_instances,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, t_pb_graph_pin* des_pb_graph_pin,
  t_mode* physical_mode, const bool& group_config_block, const bool& verbose) {
  /* Find the number of fan-in and detailed interconnection information
   * related to the destination pb_graph_pin
   */
  t_interconnect* cur_interc =
    pb_graph_pin_interc(des_pb_graph_pin, physical_mode);
  size_t fan_in = pb_graph_pin_inputs(des_pb_graph_pin, cur_interc).size();

  /* If no interconnection is needed, we can return early */
  if ((nullptr == cur_interc) || (0 == fan_in)) {
    return;
  }

  /* Initialize the interconnection type that will be physically implemented in
   * module */
  enum e_interconnect interc_type =
    device_annotation.interconnect_physical_type(cur_interc);
  const CircuitModelId& interc_circuit_model =
    device_annotation.interconnect_circuit_model(cur_interc);

  /* Find input ports of the wire module */
  std::vector<CircuitPortId> interc_model_inputs =
    circuit_lib.model_ports_by_type(
      interc_circuit_model, CIRCUIT_MODEL_PORT_INPUT,
      true); /* the last argument to guarantee that we ignore any global inputs
              */
  /* Find output ports of the wire module */
  std::vector<CircuitPortId> interc_model_outputs =
    circuit_lib.model_ports_by_type(interc_circuit_model,
                                    CIRCUIT_MODEL_PORT_OUTPUT,
                                    true); /* the last argument to guarantee
                                              that we ignore any global ports */

  /* Ensure that we have only 1 input port and 1 output port, this is valid for
   * both wire and MUX */
  VTR_ASSERT(1 == interc_model_inputs.size());
  VTR_ASSERT(1 == interc_model_outputs.size());

  /* Branch on the type of physical implementation,
   * We add instances of programmable interconnection
   */
  switch (interc_type) {
    case DIRECT_INTERC: {
      /* Ensure direct interc has only one fan-in */
      VTR_ASSERT(1 == fan_in);

      /* For more than one mode defined, the direct interc has more than one
       * input_edge , We need to find which edge is connected the pin we want
       */
      t_pb_graph_pin* src_pb_graph_pin =
        pb_graph_pin_inputs(des_pb_graph_pin, cur_interc)[0];

      /* Ensure that circuit model is a wire */
      VTR_ASSERT(CIRCUIT_MODEL_WIRE ==
                 circuit_lib.model_type(interc_circuit_model));
      /* Find the wire module in the module manager */
      ModuleId wire_module = module_manager.find_module(
        circuit_lib.model_name(interc_circuit_model));
      VTR_ASSERT(true == module_manager.valid_module_id(wire_module));
      /* Get the instance id and add an instance of wire */
      size_t wire_instance =
        module_manager.num_instance(pb_module, wire_module);
      module_manager.add_child_module(pb_module, wire_module);

      /* Give an instance name: this name should be consistent with the block
       * name given in SDC generator, If you want to bind the SDC generation to
       * modules
       */
      std::string wire_instance_name = generate_instance_name(
        module_manager.module_name(wire_module), wire_instance);
      module_manager.set_child_instance_name(pb_module, wire_module,
                                             wire_instance, wire_instance_name);

      /* Ensure input and output ports of the wire model has only 1 pin
       * respectively */
      VTR_ASSERT(1 == circuit_lib.port_size(interc_model_inputs[0]));
      VTR_ASSERT(1 == circuit_lib.port_size(interc_model_outputs[0]));

      /* Add nets to connect the wires to ports of pb_module */
      /* First net is to connect input of src_pb_graph_node to input of the wire
       * module */
      add_module_pb_graph_pin2pin_net(
        module_manager, pb_module, wire_module, wire_instance,
        circuit_lib.port_prefix(interc_model_inputs[0]),
        0, /* wire input port has only 1 pin */
        src_pb_graph_pin, INPUT2INPUT_INTERC);

      /* Second net is to connect output of the wire module to output of
       * des_pb_graph_pin */
      add_module_pb_graph_pin2pin_net(
        module_manager, pb_module, wire_module, wire_instance,
        circuit_lib.port_prefix(interc_model_outputs[0]),
        0, /* wire output port has only 1 pin */
        des_pb_graph_pin, OUTPUT2OUTPUT_INTERC);
      break;
    }
    case COMPLETE_INTERC:
    case MUX_INTERC: {
      /* Check: MUX should have at least 2 fan_in */
      VTR_ASSERT((2 == fan_in) || (2 < fan_in));
      /* Ensure that circuit model is a MUX */
      VTR_ASSERT(CIRCUIT_MODEL_MUX ==
                 circuit_lib.model_type(interc_circuit_model));
      /* Find the wire module in the module manager */
      ModuleId mux_module = module_manager.find_module(generate_mux_subckt_name(
        circuit_lib, interc_circuit_model, fan_in, std::string()));
      VTR_ASSERT(true == module_manager.valid_module_id(mux_module));

      /* Instanciate the MUX */
      size_t mux_instance = module_manager.num_instance(pb_module, mux_module);
      module_manager.add_child_module(pb_module, mux_module);
      /* Give an instance name: this name should be consistent with the block
       * name given in SDC generator, If you want to bind the SDC generation to
       * modules
       */
      std::string mux_instance_name = generate_pb_mux_instance_name(
        GRID_MUX_INSTANCE_PREFIX, des_pb_graph_pin, std::string(""));
      module_manager.set_child_instance_name(pb_module, mux_module,
                                             mux_instance, mux_instance_name);

      /* Instanciate a memory module for the MUX */
      std::string mux_mem_module_name =
        generate_mux_subckt_name(circuit_lib, interc_circuit_model, fan_in,
                                 std::string(MEMORY_MODULE_POSTFIX));
      if (group_config_block) {
        mux_mem_module_name = generate_mux_subckt_name(
          circuit_lib, interc_circuit_model, fan_in,
          std::string(MEMORY_FEEDTHROUGH_MODULE_POSTFIX));
      }
      ModuleId mux_mem_module = module_manager.find_module(mux_mem_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(mux_mem_module));
      size_t mux_mem_instance =
        module_manager.num_instance(pb_module, mux_mem_module);
      module_manager.add_child_module(pb_module, mux_mem_module);
      /* Give an instance name: this name should be consistent with the block
       * name given in bitstream manager, If you want to bind the bitstream
       * generation to modules
       */
      std::string mux_mem_instance_name = generate_pb_memory_instance_name(
        GRID_MEM_INSTANCE_PREFIX, des_pb_graph_pin, std::string(""),
        group_config_block);
      module_manager.set_child_instance_name(
        pb_module, mux_mem_module, mux_mem_instance, mux_mem_instance_name);
      /* Add this MUX as a configurable child to the pb_module */
      size_t config_child_id = module_manager.num_configurable_children(
        pb_module, ModuleManager::e_config_child_type::LOGICAL);
      module_manager.add_configurable_child(
        pb_module, mux_mem_module, mux_mem_instance,
        group_config_block ? ModuleManager::e_config_child_type::LOGICAL
                           : ModuleManager::e_config_child_type::UNIFIED);
      if (group_config_block) {
        std::string phy_mem_module_name =
          generate_mux_subckt_name(circuit_lib, interc_circuit_model, fan_in,
                                   std::string(MEMORY_MODULE_POSTFIX));
        ModuleId phy_mem_module =
          module_manager.find_module(phy_mem_module_name);
        VTR_ASSERT(module_manager.valid_module_id(phy_mem_module));
        VTR_LOGV(verbose,
                 "Mapping feedthrough memory module '%s' to physical memory "
                 "module '%s'...\n",
                 mux_mem_module_name.c_str(), phy_mem_module_name.c_str());
        module_manager.set_logical2physical_configurable_child(
          pb_module, config_child_id, phy_mem_module);
        std::string phy_mux_mem_instance_name =
          generate_pb_memory_instance_name(
            GRID_MEM_INSTANCE_PREFIX, des_pb_graph_pin, std::string(""), false);
        module_manager.set_logical2physical_configurable_child_instance_name(
          pb_module, config_child_id, phy_mux_mem_instance_name);
        VTR_LOGV(verbose, "Now use a feedthrough memory for '%s'\n",
                 phy_mem_module_name.c_str());
      }

      /* Add nets to connect SRAM ports of the MUX to the SRAM port of memory
       * module */
      add_module_nets_between_logic_and_memory_sram_bus(
        module_manager, pb_module, mux_module, mux_instance, mux_mem_module,
        mux_mem_instance, circuit_lib, interc_circuit_model);

      /* Update memory modules and memory instance list */
      memory_modules.push_back(mux_mem_module);
      memory_instances.push_back(mux_mem_instance);

      /* Ensure output port of the MUX model has only 1 pin,
       * while the input port size is dependent on the architecture conext,
       * no constaints on the circuit model definition
       */
      VTR_ASSERT(1 == circuit_lib.port_size(interc_model_outputs[0]));

      /* Create nets to wire between the MUX and PB module */
      /* Add a net to wire the inputs of the multiplexer to its source
       * pb_graph_pin inside pb_module Here is a tricky part. Not every input
       * edges from the destination pb_graph_pin is used in the physical_model
       * of pb_type So, we will skip these input edges when building nets
       */
      size_t mux_input_pin_id = 0;
      for (t_pb_graph_pin* src_pb_graph_pin :
           pb_graph_pin_inputs(des_pb_graph_pin, cur_interc)) {
        /* Add a net, set its source and sink */
        add_module_pb_graph_pin2pin_net(
          module_manager, pb_module, mux_module, mux_instance,
          circuit_lib.port_prefix(interc_model_inputs[0]), mux_input_pin_id,
          src_pb_graph_pin, INPUT2INPUT_INTERC);
        mux_input_pin_id++;
      }
      /* Ensure all the fan_in has been covered */
      VTR_ASSERT(mux_input_pin_id == fan_in);

      /* Add a net to wire the output of the multiplexer to des_pb_graph_pin */
      add_module_pb_graph_pin2pin_net(
        module_manager, pb_module, mux_module, mux_instance,
        circuit_lib.port_prefix(interc_model_outputs[0]),
        0, /* MUX should have only 1 pin in its output port */
        des_pb_graph_pin, OUTPUT2OUTPUT_INTERC);
      break;
    }
    default:
      VTR_LOGF_ERROR(
        __FILE__, __LINE__,
        "Invalid interconnection type for %s [at Architecture XML LINE%d]!\n",
        cur_interc->name, cur_interc->line_num);
      exit(1);
  }
}

/********************************************************************
 * Add modules and nets for programmable/non-programmable interconnections
 * which end to a port of pb_module
 * This function will add the following elements to a module
 * 1. Instances of direct connections
 * 2. Instances of programmable routing multiplexers
 * 3. nets to connect direct connections/multiplexer
 *
 *  +-----------------------------------------+
 *  |
 *  |    +--------------+    +------------+
 *  |--->|              |--->|            |
 *  |... | Multiplexers |... |            |
 *  |--->|              |--->|            |
 *  |    +--------------+    |   des_pb_  |
 *  |                        | graph_node |
 *  |    +--------------+    |            |
 *  |--->|              |--->|            |
 *  | ...|    Direct    |... |            |
 *  |--->|  Connections |--->|            |
 *  |    +--------------+    +------------+
 *  |
 *  +----------------------------------------+

 *
 *  Note: this function should be run after ALL the child pb_modules
 *  have been added to the pb_module and ALL the ports defined
 *  in pb_type have been added to the pb_module!!!
 *
 ********************************************************************/
static void add_module_pb_graph_port_interc(
  ModuleManager& module_manager, const ModuleId& pb_module,
  std::vector<ModuleId>& memory_modules, std::vector<size_t>& memory_instances,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, t_pb_graph_node* des_pb_graph_node,
  const e_circuit_pb_port_type& pb_port_type, t_mode* physical_mode,
  const bool& group_config_block, const bool& verbose) {
  switch (pb_port_type) {
    case CIRCUIT_PB_PORT_INPUT: {
      for (int iport = 0; iport < des_pb_graph_node->num_input_ports; ++iport) {
        for (int ipin = 0; ipin < des_pb_graph_node->num_input_pins[iport];
             ++ipin) {
          /* Get the selected edge of current pin*/
          add_module_pb_graph_pin_interc(
            module_manager, pb_module, memory_modules, memory_instances,
            device_annotation, circuit_lib,
            &(des_pb_graph_node->input_pins[iport][ipin]), physical_mode,
            group_config_block, verbose);
        }
      }
      break;
    }
    case CIRCUIT_PB_PORT_OUTPUT: {
      for (int iport = 0; iport < des_pb_graph_node->num_output_ports;
           ++iport) {
        for (int ipin = 0; ipin < des_pb_graph_node->num_output_pins[iport];
             ++ipin) {
          add_module_pb_graph_pin_interc(
            module_manager, pb_module, memory_modules, memory_instances,
            device_annotation, circuit_lib,
            &(des_pb_graph_node->output_pins[iport][ipin]), physical_mode,
            group_config_block, verbose);
        }
      }
      break;
    }
    case CIRCUIT_PB_PORT_CLOCK: {
      for (int iport = 0; iport < des_pb_graph_node->num_clock_ports; ++iport) {
        for (int ipin = 0; ipin < des_pb_graph_node->num_clock_pins[iport];
             ++ipin) {
          add_module_pb_graph_pin_interc(
            module_manager, pb_module, memory_modules, memory_instances,
            device_annotation, circuit_lib,
            &(des_pb_graph_node->clock_pins[iport][ipin]), physical_mode,
            group_config_block, verbose);
        }
      }
      break;
    }
    default:
      VTR_LOGF_ERROR(__FILE__, __LINE__, "Invalid pb port type!\n");
      exit(1);
  }
}

/********************************************************************
 * TODO:
 * Add modules and nets for programmable/non-programmable interconnections
 * inside a module of pb_type
 * This function will add the following elements to a module
 * 1. Instances of direct connections
 * 2. Instances of programmable routing multiplexers
 * 3. nets to connect direct connections/multiplexer
 *
 *   Pb_module
 *  +--------------------------------------------------------------+
 *  |                                                              |
 *  |    +--------------+    +------------+    +--------------+    |
 *  |--->|              |--->|            |--->|              |--->|
 *  |... | Multiplexers |... |            |... | Multiplexers |... |
 *  |--->|              |--->|            |--->|              |--->|
 *  |    +--------------+    |   Child    |    +--------------+    |
 *  |                        | Pb_modules |                        |
 *  |    +--------------+    |            |    +--------------+    |
 *  |--->|              |--->|            |--->|              |--->|
 *  | ...|    Direct    |... |            |... |    Direct    |... |
 *  |--->|  Connections |--->|            |--->|  Connections |--->|
 *  |    +--------------+    +------------+    +--------------+    |
 *  |                                                              |
 *  +--------------------------------------------------------------+
 *
 *  Note: this function should be run after ALL the child pb_modules
 *  have been added to the pb_module and ALL the ports defined
 *  in pb_type have been added to the pb_module!!!
 *
 ********************************************************************/
static void add_module_pb_graph_interc(
  ModuleManager& module_manager, const ModuleId& pb_module,
  std::vector<ModuleId>& memory_modules, std::vector<size_t>& memory_instances,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, t_pb_graph_node* physical_pb_graph_node,
  const int& physical_mode_index, const bool& group_config_block,
  const bool& verbose) {
  /* Check cur_pb_graph_node*/
  VTR_ASSERT(nullptr != physical_pb_graph_node);

  /* Assign physical mode */
  t_mode* physical_mode =
    &(physical_pb_graph_node->pb_type->modes[physical_mode_index]);

  /* We check output_pins of cur_pb_graph_node and its the input_edges
   * Built the interconnections between outputs of cur_pb_graph_node and outputs
   * of child_pb_graph_node child_pb_graph_node.output_pins ----------------->
   * cur_pb_graph_node.outpins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */
  add_module_pb_graph_port_interc(
    module_manager, pb_module, memory_modules, memory_instances,
    device_annotation, circuit_lib, physical_pb_graph_node,
    CIRCUIT_PB_PORT_OUTPUT, physical_mode, group_config_block, verbose);

  /* We check input_pins of child_pb_graph_node and its the input_edges
   * Built the interconnections between inputs of cur_pb_graph_node and inputs
   * of child_pb_graph_node cur_pb_graph_node.input_pins ----------------->
   * child_pb_graph_node.input_pins
   *                                        /|\
   *                                         |
   *                         input_pins,   edges,       output_pins
   */
  for (int child = 0;
       child < physical_pb_graph_node->pb_type->modes[physical_mode_index]
                 .num_pb_type_children;
       ++child) {
    for (int inst = 0;
         inst < physical_pb_graph_node->pb_type->modes[physical_mode_index]
                  .pb_type_children[child]
                  .num_pb;
         ++inst) {
      t_pb_graph_node* child_pb_graph_node =
        &(physical_pb_graph_node
            ->child_pb_graph_nodes[physical_mode_index][child][inst]);
      /* For each child_pb_graph_node input pins*/
      add_module_pb_graph_port_interc(
        module_manager, pb_module, memory_modules, memory_instances,
        device_annotation, circuit_lib, child_pb_graph_node,
        CIRCUIT_PB_PORT_INPUT, physical_mode, group_config_block, verbose);

      /* For each child_pb_graph_node clock pins*/
      add_module_pb_graph_port_interc(
        module_manager, pb_module, memory_modules, memory_instances,
        device_annotation, circuit_lib, child_pb_graph_node,
        CIRCUIT_PB_PORT_CLOCK, physical_mode, group_config_block, verbose);
    }
  }
}

/********************************************************************
 * Print Verilog modules of physical blocks inside a grid (CLB, I/O. etc.)
 * This function will traverse the graph of complex logic block
 *(t_pb_graph_node) in a recursive way, using a Depth First Search (DFS)
 *algorithm. As such, primitive physical blocks (LUTs, FFs, etc.), leaf node of
 *the pb_graph will be printed out first, while the top-level will be printed
 *out in the last
 *
 * Note: this function will print a unique Verilog module for each type of
 * t_pb_graph_node, i.e., t_pb_type, in the graph, in order to enable highly
 * hierarchical Verilog organization as well as simplify the Verilog file sizes.
 *
 * Note: DFS is the right way. Do NOT use BFS.
 * DFS can guarantee that all the sub-modules can be registered properly
 * to its parent in module manager
 *******************************************************************/
static void rec_build_logical_tile_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, t_pb_graph_node* physical_pb_graph_node,
  const bool& group_config_block, const bool& verbose) {
  /* Check cur_pb_graph_node*/
  VTR_ASSERT(nullptr != physical_pb_graph_node);

  /* Get the pb_type definition related to the node */
  t_pb_type* physical_pb_type = physical_pb_graph_node->pb_type;

  /* Find the mode that physical implementation of a pb_type */
  t_mode* physical_mode = device_annotation.physical_mode(physical_pb_type);

  /* For non-leaf node in the pb_type graph:
   * Recursively Depth-First Generate all the child pb_type at the level
   */
  if (false == is_primitive_pb_type(physical_pb_type)) {
    for (int ipb = 0; ipb < physical_mode->num_pb_type_children; ++ipb) {
      /* Go recursive to visit the children */
      rec_build_logical_tile_modules(
        module_manager, decoder_lib, device_annotation, circuit_lib, mux_lib,
        sram_orgz_type, sram_model,
        &(physical_pb_graph_node
            ->child_pb_graph_nodes[physical_mode->index][ipb][0]),
        group_config_block, verbose);
    }
  }

  /* For leaf node, a primitive Verilog module will be generated */
  if (true == is_primitive_pb_type(physical_pb_type)) {
    build_primitive_block_module(module_manager, decoder_lib, device_annotation,
                                 circuit_lib, sram_orgz_type, sram_model,
                                 physical_pb_graph_node, group_config_block,
                                 verbose);
    /* Finish for primitive node, return */
    return;
  }

  /* Generate the name of the Verilog module for this pb_type */
  std::string pb_module_name =
    generate_physical_block_module_name(physical_pb_type);

  VTR_LOGV(verbose, "Building module '%s'...\n", pb_module_name.c_str());

  /* Register the Verilog module in module manager */
  ModuleId pb_module = module_manager.add_module(pb_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

  /* Label module usage */
  module_manager.set_module_usage(pb_module, ModuleManager::MODULE_GRID);

  /* Add ports to the Verilog module */
  add_pb_type_ports_to_module_manager(module_manager, pb_module,
                                      physical_pb_type);

  /* Vectors to record all the memory modules have been added
   * They are used to add module nets of configuration bus
   */
  std::vector<ModuleId> memory_modules;
  std::vector<size_t> memory_instances;

  e_config_protocol_type mem_module_type =
    group_config_block ? CONFIG_MEM_FEEDTHROUGH : sram_orgz_type;

  /* Add all the child Verilog modules as instances */
  for (int ichild = 0; ichild < physical_mode->num_pb_type_children; ++ichild) {
    /* Get the name and module id for this child pb_type */
    std::string child_pb_module_name = generate_physical_block_module_name(
      &(physical_mode->pb_type_children[ichild]));
    ModuleId child_pb_module = module_manager.find_module(child_pb_module_name);
    /* We must have one valid id! */
    VTR_ASSERT(true == module_manager.valid_module_id(child_pb_module));

    /* Each child may exist multiple times in the hierarchy*/
    for (int inst = 0; inst < physical_mode->pb_type_children[ichild].num_pb;
         ++inst) {
      size_t child_instance_id =
        module_manager.num_instance(pb_module, child_pb_module);
      /* Ensure the instance of this child module is the same as placement
       * index, This check is necessary because placement_index is used to
       * identify instance id for children when adding local interconnection for
       * this pb_type
       */
      VTR_ASSERT(child_instance_id ==
                 (size_t)physical_pb_graph_node
                   ->child_pb_graph_nodes[physical_mode->index][ichild][inst]
                   .placement_index);

      /* Add the memory module as a child of primitive module */
      module_manager.add_child_module(pb_module, child_pb_module);

      /* Set an instance name to bind to a block in bitstream generation and SDC
       * generation! */
      std::string child_pb_instance_name =
        generate_physical_block_instance_name(
          &(physical_pb_type->modes[physical_mode->index]
              .pb_type_children[ichild]),
          inst);
      module_manager.set_child_instance_name(
        pb_module, child_pb_module, child_instance_id, child_pb_instance_name);

      VTR_LOGV(verbose, "Building instance '%s'\n",
               child_pb_instance_name.c_str());

      /* Identify if this sub module includes configuration bits,
       * we will update the memory module and instance list
       */
      if (0 < find_module_num_config_bits(module_manager, child_pb_module,
                                          circuit_lib, sram_model,
                                          mem_module_type)) {
        module_manager.add_configurable_child(
          pb_module, child_pb_module, child_instance_id,
          group_config_block ? ModuleManager::e_config_child_type::LOGICAL
                             : ModuleManager::e_config_child_type::UNIFIED);
      }
    }
  }

  /* Add modules and nets for programmable/non-programmable interconnections
   * inside the Verilog module
   */
  VTR_LOGV(verbose, "Building local interconnecting modules\n");
  add_module_pb_graph_interc(module_manager, pb_module, memory_modules,
                             memory_instances, device_annotation, circuit_lib,
                             physical_pb_graph_node, physical_mode->index,
                             group_config_block, verbose);

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, pb_module);

  /* Count GPIO ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  add_module_gpio_ports_from_child_modules(module_manager, pb_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_shared_config_bits =
    find_module_num_shared_config_bits_from_child_modules(module_manager,
                                                          pb_module);
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, pb_module,
                                              module_num_shared_config_bits);
  }

  /* Count SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_config_bits =
    find_module_num_config_bits_from_child_modules(
      module_manager, pb_module, circuit_lib, sram_model, mem_module_type,
      ModuleManager::e_config_child_type::LOGICAL);
  if (0 < module_num_config_bits) {
    add_sram_ports_to_module_manager(module_manager, pb_module, circuit_lib,
                                     sram_model, mem_module_type,
                                     module_num_config_bits);
  }

  /* Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb
   * module!
   */
  if (0 < module_manager.num_configurable_children(
            pb_module, ModuleManager::e_config_child_type::LOGICAL)) {
    add_module_nets_memory_config_bus(
      module_manager, decoder_lib, pb_module, mem_module_type,
      circuit_lib.design_tech_type(sram_model),
      ModuleManager::e_config_child_type::LOGICAL);
  }

  VTR_LOGV(verbose, "Done\n");
}

/*****************************************************************************
 * This function will create a Verilog file and print out a Verilog netlist
 * for a type of physical block
 *
 * For IO blocks:
 * The param 'border_side' is required, which is specify which side of fabric
 * the I/O block locates at.
 *****************************************************************************/
static int build_physical_tile_module(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const VprDeviceAnnotation& vpr_device_annotation,
  const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, t_physical_tile_type_ptr phy_block_type,
  const TileAnnotation& tile_annotation, const e_side& border_side,
  const QLMemoryBankConfigSetting* ql_memory_bank_config_setting,
  const bool& duplicate_grid_pin, const bool& group_config_block,
  const bool& perimeter_cb, const bool& verbose) {
  int status = CMD_EXEC_SUCCESS;
  /* Create a Module for the top-level physical block, and add to module manager
   */
  std::string grid_module_name = generate_grid_block_module_name(
    std::string(GRID_MODULE_NAME_PREFIX), std::string(phy_block_type->name),
    is_io_type(phy_block_type), border_side);
  VTR_LOGV(verbose, "Building physical tile '%s'...", grid_module_name.c_str());

  ModuleId grid_module = module_manager.add_module(grid_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(grid_module));

  /* Now each physical tile may have a number of logical blocks
   * OpenFPGA only considers the physical implementation of the tiles.
   * So, we do not allow multiple equivalent sites to be defined
   * under a physical tile type.
   * If you need different equivalent sites, you can always define
   * it as a mode under a <pb_type>
   */
  for (const t_sub_tile& sub_tile : phy_block_type->sub_tiles) {
    for (int iz = sub_tile.capacity.low; iz < sub_tile.capacity.high + 1;
         ++iz) {
      VTR_ASSERT(1 == sub_tile.equivalent_sites.size());
      t_logical_block_type_ptr lb_type = sub_tile.equivalent_sites[0];
      /* Bypass empty pb_graph */
      if (nullptr == lb_type->pb_graph_head) {
        continue;
      }
      std::string pb_module_name =
        generate_physical_block_module_name(lb_type->pb_graph_head->pb_type);
      ModuleId pb_module = module_manager.find_module(pb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(pb_module));

      /* Add all the sub modules */
      size_t pb_instance_id =
        module_manager.num_instance(grid_module, pb_module);
      module_manager.add_child_module(grid_module, pb_module, false);
      /* Add a custom I/O child with coordinate 'z' */
      module_manager.add_io_child(grid_module, pb_module, pb_instance_id,
                                  vtr::Point<int>(iz, 0));

      /* Give the child module with a unique instance name */
      std::string instance_name = generate_physical_block_instance_name(
        lb_type->pb_graph_head->pb_type, iz);
      /* Set an instance name to bind to a block in bitstream generation */
      module_manager.set_child_instance_name(grid_module, pb_module,
                                             pb_instance_id, instance_name);

      /* Identify if this sub module includes configuration bits,
       * we will update the memory module and instance list
       */
      if (0 < find_module_num_config_bits(
                module_manager, pb_module, circuit_lib, sram_model,
                group_config_block ? CONFIG_MEM_FEEDTHROUGH : sram_orgz_type)) {
        /* Only add logical configurable children here. Since we will add a
         * physical memory block at this level */
        module_manager.add_configurable_child(
          grid_module, pb_module, pb_instance_id,
          group_config_block ? ModuleManager::e_config_child_type::LOGICAL
                             : ModuleManager::e_config_child_type::UNIFIED);
      }
    }
  }

  /* TODO: Add a physical memory block */
  if (group_config_block) {
    status = add_physical_memory_module(module_manager, decoder_lib,
                                        grid_module, std::string(), circuit_lib,
                                        sram_orgz_type, sram_model, verbose);
    if (status != CMD_EXEC_SUCCESS) {
      return CMD_EXEC_FATAL_ERROR;
    }
  }

  /* Add grid ports(pins) to the module */
  if (false == duplicate_grid_pin) {
    /* Default way to add these ports by following the definition in pb_types */
    add_grid_module_pb_type_ports(module_manager, grid_module,
                                  vpr_device_annotation, phy_block_type,
                                  tile_annotation, border_side, perimeter_cb);
    /* Add module nets to connect the pb_type ports to sub modules */
    for (const t_sub_tile& sub_tile : phy_block_type->sub_tiles) {
      VTR_ASSERT(sub_tile.equivalent_sites.size() == 1);
      t_logical_block_type_ptr lb_type = sub_tile.equivalent_sites[0];
      /* Bypass empty pb_graph */
      if (nullptr == lb_type->pb_graph_head) {
        continue;
      }
      std::string pb_module_name =
        generate_physical_block_module_name(lb_type->pb_graph_head->pb_type);
      ModuleId pb_module = module_manager.find_module(pb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(pb_module));
      for (const size_t& child_instance :
           module_manager.child_module_instances(grid_module, pb_module)) {
        add_grid_module_nets_connect_pb_type_ports(
          module_manager, grid_module, pb_module, child_instance, sub_tile,
          vpr_device_annotation, phy_block_type, tile_annotation, border_side,
          perimeter_cb);
      }
    }
  } else {
    VTR_ASSERT_SAFE(true == duplicate_grid_pin);
    /* Add these ports with duplication */
    add_grid_module_duplicated_pb_type_ports(
      module_manager, grid_module, vpr_device_annotation, phy_block_type,
      tile_annotation, border_side, perimeter_cb);

    /* Add module nets to connect the duplicated pb_type ports to sub modules */
    for (const t_sub_tile& sub_tile : phy_block_type->sub_tiles) {
      VTR_ASSERT(sub_tile.equivalent_sites.size() == 1);
      t_logical_block_type_ptr lb_type = sub_tile.equivalent_sites[0];
      /* Bypass empty pb_graph */
      if (nullptr == lb_type->pb_graph_head) {
        continue;
      }
      std::string pb_module_name =
        generate_physical_block_module_name(lb_type->pb_graph_head->pb_type);
      ModuleId pb_module = module_manager.find_module(pb_module_name);
      VTR_ASSERT(true == module_manager.valid_module_id(pb_module));
      for (const size_t& child_instance :
           module_manager.child_module_instances(grid_module, pb_module)) {
        add_grid_module_nets_connect_duplicated_pb_type_ports(
          module_manager, grid_module, pb_module, child_instance, sub_tile,
          vpr_device_annotation, phy_block_type, tile_annotation, border_side,
          perimeter_cb);
      }
    }
  }

  /* Add global ports to the pb_module:
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the global ports from the child modules and build
   * a list of it
   */
  add_module_global_ports_from_child_modules(module_manager, grid_module);

  /* Count GPIO ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  add_module_gpio_ports_from_child_modules(module_manager, grid_module);

  /* Count shared SRAM ports from the sub-modules under this Verilog module
   * This is a much easier job after adding sub modules (instances),
   * we just need to find all the I/O ports from the child modules and build a
   * list of it
   */
  size_t module_num_shared_config_bits =
    find_module_num_shared_config_bits_from_child_modules(module_manager,
                                                          grid_module);
  if (0 < module_num_shared_config_bits) {
    add_reserved_sram_ports_to_module_manager(module_manager, grid_module,
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
      module_manager, grid_module, circuit_lib, sram_model, sram_orgz_type,
      config_child_type);
  if (0 < module_num_config_bits) {
    add_pb_sram_ports_to_module_manager(
      module_manager, grid_module, circuit_lib, sram_model, sram_orgz_type,
      module_num_config_bits,
      ql_memory_bank_config_setting->pb_setting(phy_block_type->name).num_wl);
  }

  /* Add module nets to connect memory cells inside
   * This is a one-shot addition that covers all the memory modules in this pb
   * module!
   */
  if (0 < module_manager.num_configurable_children(grid_module,
                                                   config_child_type)) {
    add_pb_module_nets_memory_config_bus(
      module_manager, decoder_lib, grid_module, sram_orgz_type,
      circuit_lib.design_tech_type(sram_model), config_child_type);
  }

  VTR_LOGV(verbose, "Done\n");

  return status;
}

/*****************************************************************************
 * Create logic block modules in a compact way
 * This function will achieve this goal in two step:
 * - Build the modules for each logical tile which is based on pb_graph
 *   Note that there the pin/port does not carry any fixed physical location
 * - Build the modules for each physical tile which is based on
 *physical_tile_type_ptr Here, multiple logical tiles can be considered and each
 *port/pin has a fixed physical location. This is where the feature of
 *duplicate_pin_pin will be applied
 *   - Only one module for each I/O on each border side (IO_TYPE)
 *   - Only one module for each CLB (FILL_TYPE)
 *   - Only one module for each heterogeneous block
 ****************************************************************************/
int build_grid_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const DeviceContext& device_ctx, const VprDeviceAnnotation& device_annotation,
  const CircuitLibrary& circuit_lib, const MuxLibrary& mux_lib,
  const TileAnnotation& tile_annotation,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model,
  const QLMemoryBankConfigSetting* ql_memory_bank_config_setting,
  const bool& duplicate_grid_pin, const bool& group_config_block,
  const bool& verbose) {
  /* Start time count */
  vtr::ScopedStartFinishTimer timer("Build grid modules");

  int status = CMD_EXEC_SUCCESS;

  /* Enumerate the types of logical tiles, and build a module for each
   * Build modules for all the pb_types/pb_graph_nodes
   * use a Depth-First Search Algorithm to print the sub-modules
   * Note: DFS is the right way. Do NOT use BFS.
   * DFS can guarantee that all the sub-modules can be registered properly
   * to its parent in module manager
   */
  /* Build modules starting from the top-level pb_type/pb_graph_node, and
   * traverse the graph in a recursive way */
  VTR_LOG("Building logical tiles...");
  VTR_LOGV(verbose, "\n");
  for (const t_logical_block_type& logical_tile :
       device_ctx.logical_block_types) {
    /* Bypass empty pb_graph */
    if (nullptr == logical_tile.pb_graph_head) {
      continue;
    }
    rec_build_logical_tile_modules(
      module_manager, decoder_lib, device_annotation, circuit_lib, mux_lib,
      sram_orgz_type, sram_model, logical_tile.pb_graph_head,
      group_config_block, verbose);
  }
  VTR_LOG("Done\n");

  /* Enumerate the types of physical tiles
   * Use the logical tile module to build the physical tiles
   */
  VTR_LOG("Building physical tiles...");
  VTR_LOGV(verbose, "\n");
  for (const t_physical_tile_type& physical_tile :
       device_ctx.physical_tile_types) {
    /* Bypass empty type or nullptr */
    if (true == is_empty_type(&physical_tile)) {
      continue;
    } else if (true == is_io_type(&physical_tile)) {
      /* Special for I/O block:
       * We will search the grids and see where the I/O blocks are located:
       * - If a I/O block locates on border sides of FPGA fabric:
       *   i.e., one or more from {TOP, RIGHT, BOTTOM, LEFT},
       *   we will generate one module for each border side
       * - If a I/O block locates in the center of FPGA fabric:
       *   we will generate one module with NUM_2D_SIDES (same treatment as regular
       * grids)
       */
      std::set<e_side> io_type_sides =
        find_physical_io_tile_located_sides(device_ctx.grid, &physical_tile);
      for (const e_side& io_type_side : io_type_sides) {
        status = build_physical_tile_module(
          module_manager, decoder_lib, device_annotation, circuit_lib,
          sram_orgz_type, sram_model, &physical_tile, tile_annotation,
          io_type_side, ql_memory_bank_config_setting, duplicate_grid_pin,
          group_config_block, device_ctx.arch->perimeter_cb, verbose);
        if (status != CMD_EXEC_SUCCESS) {
          return CMD_EXEC_FATAL_ERROR;
        }
      }
    } else {
      /* For CLB and heterogenenous blocks */
      status = build_physical_tile_module(
        module_manager, decoder_lib, device_annotation, circuit_lib,
        sram_orgz_type, sram_model, &physical_tile, tile_annotation, NUM_2D_SIDES,
        ql_memory_bank_config_setting, duplicate_grid_pin, group_config_block,
        device_ctx.arch->perimeter_cb, verbose);
      if (status != CMD_EXEC_SUCCESS) {
        return CMD_EXEC_FATAL_ERROR;
      }
    }
  }
  VTR_LOG("Done\n");

  return status;
}

} /* end namespace openfpga */
