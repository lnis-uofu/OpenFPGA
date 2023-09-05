/********************************************************************
 * This file includes most utilized functions for grid module builders
 *******************************************************************/
/* Headers from vtrutil library */
#include "vtr_assert.h"

/* Headers from openfpgautil library */
#include "openfpga_side_manager.h"

/* Headers from vpr library */
#include "build_grid_module_utils.h"
#include "openfpga_naming.h"
#include "vpr_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find the side where I/O pins locate on a grid I/O block
 * 1. I/O grids on the top    side of FPGA only have ports on its bottom side
 * 2. I/O grids on the right  side of FPGA only have ports on its left   side
 * 3. I/O grids on the bottom side of FPGA only have ports on its top    side
 * 4. I/O grids on the left   side of FPGA only have ports on its right side
 * 5. I/O grids in the center part of FPGA can have ports on any side
 *******************************************************************/
std::vector<e_side> find_grid_module_pin_sides(
  t_physical_tile_type_ptr grid_type_descriptor, const e_side& border_side) {
  /* We must have an regular (non-I/O) type here */
  VTR_ASSERT(true == is_io_type(grid_type_descriptor));
  SideManager side_manager(border_side);

  if (NUM_SIDES == border_side) {
    return {TOP, RIGHT, BOTTOM, LEFT};
  }

  return std::vector<e_side>(1, side_manager.get_opposite());
}

/********************************************************************
 * Add module nets to connect a port of child pb_module
 * to the grid module
 *******************************************************************/
void add_grid_module_net_connect_pb_graph_pin(
  ModuleManager& module_manager, const ModuleId& grid_module,
  const ModuleId& child_module, const size_t& child_instance,
  const size_t& child_inst_subtile_index,
  const VprDeviceAnnotation& vpr_device_annotation,
  t_physical_tile_type_ptr grid_type_descriptor, t_pb_graph_pin* pb_graph_pin,
  const e_side& border_side, const e_pin2pin_interc_type& pin2pin_interc_type) {
  /* Find the pin side for I/O grids*/
  std::vector<e_side> grid_pin_sides;
  /* For I/O grids, we care only one side
   * Otherwise, we will iterate all the 4 sides
   */
  if (true == is_io_type(grid_type_descriptor)) {
    grid_pin_sides =
      find_grid_module_pin_sides(grid_type_descriptor, border_side);
  } else {
    grid_pin_sides = {TOP, RIGHT, BOTTOM, LEFT};
  }

  /* Note that each grid may contain a number of sub tiles, each type of which
   * may a different capacity and number of pins We need to find the start pin
   * index for a given z offset (instance id), denotes the index of the first
   * pin regarding the current instance. The variable 'pin_count_in_cluster'
   * represent the pin index in the context of current instance only. With the
   * information above, we can then calculate the absolute pin index at
   * grid-level (considering all the sub tiles).
   */
  size_t grid_pin_index =
    pb_graph_pin->pin_count_in_cluster +
    vpr_device_annotation.physical_tile_z_to_start_pin_index(
      grid_type_descriptor, child_inst_subtile_index);
  int pin_height = grid_type_descriptor->pin_height_offset[grid_pin_index];
  int pin_width = grid_type_descriptor->pin_width_offset[grid_pin_index];
  for (const e_side& side : grid_pin_sides) {
    if (true != grid_type_descriptor
                  ->pinloc[pin_width][pin_height][side][grid_pin_index]) {
      continue;
    }
    /* Reach here, it means this pin is on this side */
    /* Create a net to connect the grid pin to child module pin */
    ModuleNetId net = module_manager.create_module_net(grid_module);
    /* Find the port in grid_module */
    BasicPort pin_info = vpr_device_annotation.physical_tile_pin_port_info(
      grid_type_descriptor, grid_pin_index);
    VTR_ASSERT(true == pin_info.is_valid());
    int subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(
      grid_type_descriptor, grid_pin_index);
    VTR_ASSERT(OPEN != subtile_index &&
               subtile_index < grid_type_descriptor->capacity);
    std::string grid_port_name = generate_grid_port_name(
      pin_width, pin_height, subtile_index, side, pin_info);
    ModulePortId grid_module_port_id =
      module_manager.find_module_port(grid_module, grid_port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(
                         grid_module, grid_module_port_id));
    /* Grid port always has only 1 pin, it is assumed when adding these ports to
     * the module if you need a change, please also change the port adding codes
     */
    size_t grid_module_pin_id = 0;
    /* Find the port in child module */
    std::string child_module_port_name =
      generate_pb_type_port_name(pb_graph_pin->port);
    ModulePortId child_module_port_id =
      module_manager.find_module_port(child_module, child_module_port_name);
    VTR_ASSERT(true == module_manager.valid_module_port_id(
                         child_module, child_module_port_id));
    size_t child_module_pin_id = pb_graph_pin->pin_number;
    /* Add net sources and sinks:
     * For input-to-input connection, net_source is grid pin, while net_sink is
     * pb_graph_pin For output-to-output connection, net_source is pb_graph_pin,
     * while net_sink is grid pin
     */
    switch (pin2pin_interc_type) {
      case INPUT2INPUT_INTERC:
        module_manager.add_module_net_source(grid_module, net, grid_module, 0,
                                             grid_module_port_id,
                                             grid_module_pin_id);
        module_manager.add_module_net_sink(grid_module, net, child_module,
                                           child_instance, child_module_port_id,
                                           child_module_pin_id);
        break;
      case OUTPUT2OUTPUT_INTERC:
        module_manager.add_module_net_source(
          grid_module, net, child_module, child_instance, child_module_port_id,
          child_module_pin_id);
        module_manager.add_module_net_sink(grid_module, net, grid_module, 0,
                                           grid_module_port_id,
                                           grid_module_pin_id);
        break;
      default:
        VTR_LOGF_ERROR(__FILE__, __LINE__,
                       "Invalid pin-to-pin interconnection type!\n");
        exit(1);
    }
  }
}

} /* end namespace openfpga */
