/********************************************************************
 * This file includes functions that are used to add duplicated 
 * pins to each side of a grid
 *
 * These functions are located in this file, being separated from
 * the default functions in build_grid_module.cpp
 * This allows us to keep new features easy to be maintained.
 *
 * Please follow this rules when creating new features!
 *******************************************************************/
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "openfpga_naming.h"
#include "openfpga_interconnect_types.h"

#include "openfpga_physical_tile_utils.h"

#include "build_grid_module_utils.h"
#include "build_grid_module_duplicated_pins.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * This function adds pb_type ports to top-level grid module with duplication
 * For each pin at each side, we create two pins which are short-wired
 * They are driven by the same pin, e.g., pinA in the child module
 * But in this top module, we will create two pins, each of which indicates
 * the physical location of pin.
 * Take the following example: 
 * One is called pinA_upper which is located close to the top side of this grid
 * The other is called pinA_lower which is located close to the bottom side of this grid
 *
 * Similarly, we duplicate pins at TOP, RIGHT, BOTTOM and LEFT sides.
 * For LEFT side, upper and lower pins carry the indication in physical location as RIGHT side.
 * For TOP and BOTTOM side, upper pin is located close to the left side of a grid, while lower
 * pin is located close to the right side of a grid  
 *
 *             pinB_upper   pinB_lower
 *                 ^          ^
 *                 |          |
 *               ---------------+
 *                              |--->pinA_upper
 *                              |
 *                       Grid   |  
 *                              |
 *                              |--->pinA_lower
 *               ---------------+
 *******************************************************************/
void add_grid_module_duplicated_pb_type_ports(ModuleManager& module_manager,
                                              const ModuleId& grid_module,
                                              t_physical_tile_type_ptr grid_type_descriptor,
                                              const e_side& border_side) {
  /* Ensure that we have a valid grid_type_descriptor */
  VTR_ASSERT(false == is_empty_type(grid_type_descriptor));

  /* Find the pin side for I/O grids*/
  std::vector<e_side> grid_pin_sides;
  /* For I/O grids, we care only one side
   * Otherwise, we will iterate all the 4 sides  
   */
  if (true == is_io_type(grid_type_descriptor)) {
    grid_pin_sides = find_grid_module_pin_sides(grid_type_descriptor, border_side);
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
          if (true != grid_type_descriptor->pinloc[iwidth][iheight][side][ipin]) {
            continue;
          }
          /* Reach here, it means this pin is on this side */
          int class_id = grid_type_descriptor->pin_class[ipin];
          e_pin_type pin_class_type = grid_type_descriptor->class_inf[class_id].type;
          /* Generate the pin name 
           * For each RECEIVER PIN or DRIVER PIN for direct connection, 
           * we do not duplicate in these cases */
          if ( (RECEIVER == pin_class_type)
            /* Xifan: I assume that each direct connection pin must have Fc=0. */
            || ( (DRIVER == pin_class_type)
              && (0. == find_physical_tile_pin_Fc(grid_type_descriptor, ipin)) ) ) {
            vtr::Point<size_t> dummy_coordinate;
            std::string port_name = generate_grid_port_name(dummy_coordinate, iwidth, iheight, side, ipin, false);
            BasicPort grid_port(port_name, 0, 0);
            /* Add the port to the module */
            module_manager.add_port(grid_module, grid_port, pin_type2type_map[pin_class_type]);
          } else {
            /* For each DRIVER pin, we create two copies.
             * One with a postfix of upper, indicating it is located on the upper part of a side
             * The other with a postfix of lower, indicating it is located on the lower part of a side
             */
            VTR_ASSERT(DRIVER == pin_class_type);
            std::string upper_port_name = generate_grid_duplicated_port_name(iwidth, iheight, side, ipin, true);
            BasicPort grid_upper_port(upper_port_name, 0, 0);
            /* Add the port to the module */
            module_manager.add_port(grid_module, grid_upper_port, pin_type2type_map[pin_class_type]);

            std::string lower_port_name = generate_grid_duplicated_port_name(iwidth, iheight, side, ipin, false);
            BasicPort grid_lower_port(lower_port_name, 0, 0);
            /* Add the port to the module */
            module_manager.add_port(grid_module, grid_lower_port, pin_type2type_map[pin_class_type]);
          }
        }  
      }
    }
  }
}

/********************************************************************
 * Add module nets to connect a port of child pb_module
 * to the duplicated pins of grid module 
 * Note: This function SHOULD be ONLY applied to pb_graph output pins 
 * of the child module.
 * For each such pin, we connect it to two outputs of the grid module
 * one is named after "upper", and the other is named after "lower"
 *******************************************************************/
static 
void add_grid_module_net_connect_duplicated_pb_graph_pin(ModuleManager& module_manager,
                                                         const ModuleId& grid_module,
                                                         const ModuleId& child_module,
                                                         const size_t& child_instance,
                                                         t_physical_tile_type_ptr grid_type_descriptor,
                                                         t_pb_graph_pin* pb_graph_pin,
                                                         const e_side& border_side,
                                                         const e_pin2pin_interc_type& pin2pin_interc_type) {
  /* Make sure this is ONLY applied to output pins */
  VTR_ASSERT(OUTPUT2OUTPUT_INTERC == pin2pin_interc_type);
  
  /* Find the pin side for I/O grids*/
  std::vector<e_side> grid_pin_sides;
  /* For I/O grids, we care only one side
   * Otherwise, we will iterate all the 4 sides  
   */
  if (true == is_io_type(grid_type_descriptor)) {
    grid_pin_sides = find_grid_module_pin_sides(grid_type_descriptor, border_side);
  } else {
    grid_pin_sides = {TOP, RIGHT, BOTTOM, LEFT}; 
  }

  /* num_pins/capacity = the number of pins that each type_descriptor has.
   * Capacity defines the number of type_descriptors in each grid
   * so the pin index at grid level = pin_index_in_type_descriptor 
   *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
   */
  size_t grid_pin_index = pb_graph_pin->pin_count_in_cluster 
                        + child_instance * grid_type_descriptor->num_pins / grid_type_descriptor->capacity;

  int pin_width = grid_type_descriptor->pin_width_offset[grid_pin_index];
  int pin_height = grid_type_descriptor->pin_height_offset[grid_pin_index];
  for (const e_side& side : grid_pin_sides) {
    if (true != grid_type_descriptor->pinloc[pin_width][pin_height][side][grid_pin_index]) {
      continue;
    }

    /* Pins for direct connection are NOT duplicated.
     * Follow the traditional recipe when adding nets!  
     * Xifan: I assume that each direct connection pin must have Fc=0. 
     */
    if (0. == find_physical_tile_pin_Fc(grid_type_descriptor, grid_pin_index)) {
      /* Create a net to connect the grid pin to child module pin */
      ModuleNetId net = module_manager.create_module_net(grid_module);
      /* Find the port in grid_module */
      vtr::Point<size_t> dummy_coordinate;
      std::string grid_port_name = generate_grid_port_name(dummy_coordinate, pin_width, pin_height, side, grid_pin_index, false);
      ModulePortId grid_module_port_id = module_manager.find_module_port(grid_module, grid_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(grid_module, grid_module_port_id));

      /* Grid port always has only 1 pin, it is assumed when adding these ports to the module
       * if you need a change, please also change the port adding codes  
       */
      size_t grid_module_pin_id = 0;
      /* Find the port in child module */
      std::string child_module_port_name = generate_pb_type_port_name(pb_graph_pin->port);
      ModulePortId child_module_port_id = module_manager.find_module_port(child_module, child_module_port_name);
      VTR_ASSERT(true == module_manager.valid_module_port_id(child_module, child_module_port_id));
      size_t child_module_pin_id = pb_graph_pin->pin_number;
      /* Add net sources and sinks:
       * For output-to-output connection, net_source is pb_graph_pin, while net_sink is grid pin   
       */
      module_manager.add_module_net_source(grid_module, net, child_module, child_instance, child_module_port_id, child_module_pin_id);
      module_manager.add_module_net_sink(grid_module, net, grid_module, 0, grid_module_port_id, grid_module_pin_id);
      continue;
    }
    /* Reach here, it means this pin is on this side */
    /* Create a net to connect the grid pin to child module pin */
    ModuleNetId net = module_manager.create_module_net(grid_module);
    /* Find the upper port in grid_module */
    std::string grid_upper_port_name = generate_grid_duplicated_port_name(pin_width, pin_height, side, grid_pin_index, true);
    ModulePortId grid_module_upper_port_id = module_manager.find_module_port(grid_module, grid_upper_port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(grid_module, grid_module_upper_port_id));

    /* Find the lower port in grid_module */
    std::string grid_lower_port_name = generate_grid_duplicated_port_name(pin_width, pin_height, side, grid_pin_index, false);
    ModulePortId grid_module_lower_port_id = module_manager.find_module_port(grid_module, grid_lower_port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(grid_module, grid_module_lower_port_id));

    /* Grid port always has only 1 pin, it is assumed when adding these ports to the module
     * if you need a change, please also change the port adding codes  
     */
    size_t grid_module_pin_id = 0;
    /* Find the port in child module */
    std::string child_module_port_name = generate_pb_type_port_name(pb_graph_pin->port);
    ModulePortId child_module_port_id = module_manager.find_module_port(child_module, child_module_port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(child_module, child_module_port_id));
    size_t child_module_pin_id = pb_graph_pin->pin_number;

    /* Add net sources and sinks:
     * For output-to-output connection, 
     * net_source is pb_graph_pin, 
     * while net_sinks are grid upper pin and grid lower pin
     */
    module_manager.add_module_net_source(grid_module, net, child_module, child_instance, child_module_port_id, child_module_pin_id);
    module_manager.add_module_net_sink(grid_module, net, grid_module, 0, grid_module_upper_port_id, grid_module_pin_id);
    module_manager.add_module_net_sink(grid_module, net, grid_module, 0, grid_module_lower_port_id, grid_module_pin_id);
  }
}

/********************************************************************
 * Add module nets to connect a port of child pb_module
 * to the duplicated ports of grid module 
 *******************************************************************/
void add_grid_module_nets_connect_duplicated_pb_type_ports(ModuleManager& module_manager,
                                                           const ModuleId& grid_module,
                                                           const ModuleId& child_module,
                                                           const size_t& child_instance,
                                                           t_physical_tile_type_ptr grid_type_descriptor,
                                                           const e_side& border_side) {
  /* Ensure that we have a valid grid_type_descriptor */
  VTR_ASSERT(false == is_empty_type(grid_type_descriptor));

  for (t_logical_block_type_ptr lb_type : grid_type_descriptor->equivalent_sites) {
    t_pb_graph_node* top_pb_graph_node = lb_type->pb_graph_head;
    VTR_ASSERT(nullptr != top_pb_graph_node); 

    for (int iport = 0; iport < top_pb_graph_node->num_input_ports; ++iport) {
      for (int ipin = 0; ipin < top_pb_graph_node->num_input_pins[iport]; ++ipin) {
        add_grid_module_net_connect_pb_graph_pin(module_manager, grid_module,
                                                 child_module, child_instance,
                                                 grid_type_descriptor,
                                                 &(top_pb_graph_node->input_pins[iport][ipin]),
                                                 border_side,
                                                 INPUT2INPUT_INTERC);

      }
    }

    for (int iport = 0; iport < top_pb_graph_node->num_output_ports; ++iport) {
      for (int ipin = 0; ipin < top_pb_graph_node->num_output_pins[iport]; ++ipin) {
        add_grid_module_net_connect_duplicated_pb_graph_pin(module_manager, grid_module,
                                                            child_module, child_instance,
                                                            grid_type_descriptor,
                                                            &(top_pb_graph_node->output_pins[iport][ipin]),
                                                            border_side,
                                                            OUTPUT2OUTPUT_INTERC);
      }
    }

    for (int iport = 0; iport < top_pb_graph_node->num_clock_ports; ++iport) {
      for (int ipin = 0; ipin < top_pb_graph_node->num_clock_pins[iport]; ++ipin) {
        add_grid_module_net_connect_pb_graph_pin(module_manager, grid_module,
                                                 child_module, child_instance,
                                                 grid_type_descriptor,
                                                 &(top_pb_graph_node->clock_pins[iport][ipin]),
                                                 border_side,
                                                 INPUT2INPUT_INTERC);
      } 
    }
  }
}

} /* end namespace openfpga */
