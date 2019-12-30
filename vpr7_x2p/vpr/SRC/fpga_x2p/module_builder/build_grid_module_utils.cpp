/********************************************************************
 * This file includes most utilized functions for grid module builders
 *******************************************************************/
/* External library headers */
#include "vtr_assert.h"

/* FPGA-X2P headers */
#include "fpga_x2p_naming.h"

/* Module builder headers */
#include "build_grid_module_utils.h"

/* Global variables should be the last to include */
#include "globals.h"

/********************************************************************
 * Find the side where I/O pins locate on a grid I/O block
 * 1. I/O grids on the top    side of FPGA only have ports on its bottom side
 * 2. I/O grids on the right  side of FPGA only have ports on its left   side
 * 3. I/O grids on the bottom side of FPGA only have ports on its top    side
 * 4. I/O grids on the left   side of FPGA only have ports on its right side
 *******************************************************************/
e_side find_grid_module_pin_side(t_type_ptr grid_type_descriptor,
                                 const e_side& border_side) {
  VTR_ASSERT(IO_TYPE == grid_type_descriptor);
  Side side_manager(border_side);
  return side_manager.get_opposite(); 
}

/********************************************************************
 * Add module nets to connect a port of child pb_module
 * to the grid module 
 *******************************************************************/
void add_grid_module_net_connect_pb_graph_pin(ModuleManager& module_manager,
                                              const ModuleId& grid_module,
                                              const ModuleId& child_module,
                                              const size_t& child_instance,
                                              t_type_ptr grid_type_descriptor,
                                              t_pb_graph_pin* pb_graph_pin,
                                              const e_side& border_side,
                                              const enum e_spice_pin2pin_interc_type& pin2pin_interc_type) {
  /* Find the pin side for I/O grids*/
  std::vector<e_side> grid_pin_sides;
  /* For I/O grids, we care only one side
   * Otherwise, we will iterate all the 4 sides  
   */
  if (IO_TYPE == grid_type_descriptor) {
    grid_pin_sides.push_back(find_grid_module_pin_side(grid_type_descriptor, border_side));
  } else {
    grid_pin_sides.push_back(TOP); 
    grid_pin_sides.push_back(RIGHT); 
    grid_pin_sides.push_back(BOTTOM); 
    grid_pin_sides.push_back(LEFT); 
  }

  /* num_pins/capacity = the number of pins that each type_descriptor has.
   * Capacity defines the number of type_descriptors in each grid
   * so the pin index at grid level = pin_index_in_type_descriptor 
   *                                + type_descriptor_index_in_capacity * num_pins_per_type_descriptor
   */
  size_t grid_pin_index = pb_graph_pin->pin_count_in_cluster 
                        + child_instance * grid_type_descriptor->num_pins / grid_type_descriptor->capacity;
  int pin_height = grid_type_descriptor->pin_height[grid_pin_index];
  for (const e_side& side : grid_pin_sides) {
    if (1 != grid_type_descriptor->pinloc[pin_height][side][grid_pin_index]) {
      continue;
    }
    /* Reach here, it means this pin is on this side */
    /* Create a net to connect the grid pin to child module pin */
    ModuleNetId net = module_manager.create_module_net(grid_module);
    /* Find the port in grid_module */
    vtr::Point<size_t> dummy_coordinate;
    std::string grid_port_name = generate_grid_port_name(dummy_coordinate, pin_height, side, grid_pin_index, false);
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
     * For input-to-input connection, net_source is grid pin, while net_sink is pb_graph_pin   
     * For output-to-output connection, net_source is pb_graph_pin, while net_sink is grid pin   
     */
    switch (pin2pin_interc_type) {
    case INPUT2INPUT_INTERC:
      module_manager.add_module_net_source(grid_module, net, grid_module, 0, grid_module_port_id, grid_module_pin_id);
      module_manager.add_module_net_sink(grid_module, net, child_module, child_instance, child_module_port_id, child_module_pin_id);
      break;
    case OUTPUT2OUTPUT_INTERC:
      module_manager.add_module_net_source(grid_module, net, child_module, child_instance, child_module_port_id, child_module_pin_id);
      module_manager.add_module_net_sink(grid_module, net, grid_module, 0, grid_module_port_id, grid_module_pin_id);
      break;
    default:
      vpr_printf(TIO_MESSAGE_ERROR,
                 "(File:%s, [LINE%d]) Invalid pin-to-pin interconnection type!\n",
                 __FILE__, __LINE__);
      exit(1);
    }
  }
}

