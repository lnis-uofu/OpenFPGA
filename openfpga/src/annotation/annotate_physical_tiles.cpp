/********************************************************************
 * This file includes functions to build links between pb_types
 * in particular to annotate the physical mode and physical pb_type
 *******************************************************************/
/* Headers from vtrutil library */
#include "annotate_physical_tiles.h"

#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Build the fast look-up for each physical tile between
 * pin index and the physical port information, i.e., port name and port index
 *******************************************************************/
void build_physical_tile_pin2port_info(
  const DeviceContext& vpr_device_ctx,
  VprDeviceAnnotation& vpr_device_annotation) {
  vtr::ScopedStartFinishTimer timer(
    "Build fast look-up for physical tile pins");

  for (const t_physical_tile_type& physical_tile :
       vpr_device_ctx.physical_tile_types) {
    int curr_pin_index = 0;
    /* Walk through each subtile, consider their capacity and num of pins */
    for (const t_sub_tile& sub_tile : physical_tile.sub_tiles) {
      /* Walk through capacity */
      for (int subtile_index = sub_tile.capacity.low;
           subtile_index <= sub_tile.capacity.high; subtile_index++) {
        vpr_device_annotation.add_physical_tile_z_to_start_pin_index(
          &physical_tile, subtile_index, curr_pin_index);
        /* For each sub tile, the starting pin index is (num_pins_per_subtile *
         * index) + abs_index */
        for (const t_physical_tile_port& tile_port : sub_tile.ports) {
          for (int pin_index = 0; pin_index < tile_port.num_pins; ++pin_index) {
            int absolute_pin_index =
              curr_pin_index + tile_port.absolute_first_pin_index + pin_index;
            BasicPort tile_port_info(tile_port.name, pin_index, pin_index);
            vpr_device_annotation.add_physical_tile_pin2port_info_pair(
              &physical_tile, absolute_pin_index, tile_port_info);
            vpr_device_annotation.add_physical_tile_pin_subtile_index(
              &physical_tile, absolute_pin_index, subtile_index);
            vpr_device_annotation.add_physical_tile_z_to_subtile_index(
              &physical_tile, subtile_index,
              &sub_tile - &(physical_tile.sub_tiles[0]));
          }
        }
        /* Count the number of pins for each sub tile */
        curr_pin_index += sub_tile.num_phy_pins / sub_tile.capacity.total();
      }
    }
  }
}

} /* end namespace openfpga */
