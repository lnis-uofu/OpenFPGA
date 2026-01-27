/********************************************************************
 * This file includes functions to build links between pb_types
 * in particular to annotate the physical mode and physical pb_type
 *******************************************************************/
/* Headers from vtrutil library */
#include "annotate_physical_tiles.h"

#include "command_exit_codes.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/* Identify equivalent sites for each subtile
 * - if there is only 1 equivalent site, it is the physical one 
 * - if there are >1 equivalent sites, there must be a definition */
int build_physical_tile_equivalent_sites(
  const DeviceContext& vpr_device_ctx,
  const TileAnnotation& tile_annotation,
  VprDeviceAnnotation& vpr_device_annotation) {
  size_t num_err = 0;
  vtr::ScopedStartFinishTimer timer(
    "Identify physical equivalent sites for subtiles");
  for (const t_physical_tile_type& phy_tile :
       vpr_device_ctx.physical_tile_types) {
    for (const t_sub_tile& s_tile : phy_tile.sub_tiles) {
      if (s_tile.equivalent_sites.size() == 0) {
        VTR_LOG_ERROR("No equivalent sites are defined under subtile '%s'\n", s_tile.name.c_str());
        num_err++;
        continue;
      }
      if (s_tile.equivalent_sites.size() == 1) {
        /* Auto infer as the physical site */
        vpr_device_annotation.set_subtile_physical_equivalent_site(&phy_tile, s_tile.name, s_tile.equivalent_sites[0]);
        VTR_LOG("Auto infer equivalent site '%s' under subtile '%s' as the physical one\n", s_tile.equivalent_sites[0]->name.c_str(), s_tile.name.c_str());
        continue;
      }
      /* Must have a specific definition */
      if (!tile_annotation.is_physical_equivalent_site_defined(phy_tile.name, s_tile.name)) {
        VTR_LOG_ERROR("More than 1 equivalent sites are defined under subtile '%s' of tile '%s' but no specific physical equivalent site is defined\n", s_tile.name.c_str(), phy_tile.name.c_str());
        num_err++;
        continue;
      }
      std::string req_lb_name = tile_annotation.physical_equivalent_site(phy_tile.name, s_tile.name);
      bool valid_req_lb = false;
      for (auto lb_type : s_tile.equivalent_sites) {
        if (lb_type->name == req_lb_name) {
          vpr_device_annotation.set_subtile_physical_equivalent_site(&phy_tile, s_tile.name, lb_type);
          valid_req_lb = true;
          break;
        }
      }
      if (!valid_req_lb) {
        VTR_LOG_ERROR("For subtile '%s' of tile '%s' but the specified physical equivalent site '%s' is not a valid pb_type\n", s_tile.name.c_str(), phy_tile.name.c_str(), req_lb_name.c_str());
        num_err++;
        continue;
      }
    }
  }
  return num_err ? CMD_EXEC_FATAL_ERROR : CMD_EXEC_SUCCESS;
}

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
