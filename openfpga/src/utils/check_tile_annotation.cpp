/************************************************************************
 * Check functions for the content of tile annotation to avoid conflicts with
 * other data structures
 * These functions are not universal methods for the TileAnnotation class
 * They are made to ease the development in some specific purposes
 * Please classify such functions in this file
 ***********************************************************************/

#include <algorithm>

/* Headers from vtrutil library */
#include "check_tile_annotation.h"
#include "circuit_library_utils.h"
#include "openfpga_physical_tile_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Check if the tile annotation is valid without any conflict with
 * circuit library content.
 * Items to check:
 * - The global port defined in tile annotation has no conflicts with
 * the global ports which are defined in circuit library:
 *   - If a port has the same name, must ensure that its attributes are the same
 *     i.e., is_clock, is_reset, is_set
 *   - Otherwise, error out
 *******************************************************************/
static int check_tile_annotation_conflicts_with_circuit_library(
  const TileAnnotation& tile_annotation, const CircuitLibrary& circuit_lib) {
  int num_err = 0;

  std::vector<CircuitPortId> ckt_global_ports =
    find_circuit_library_global_ports(circuit_lib);
  for (const TileGlobalPortId& tile_global_port :
       tile_annotation.global_ports()) {
    for (const CircuitPortId& ckt_global_port : ckt_global_ports) {
      if (tile_annotation.global_port_name(tile_global_port) !=
          circuit_lib.port_prefix(ckt_global_port)) {
        continue;
      }
      /* All the global clock port here must be operating clock */
      bool is_both_op_signal = !circuit_lib.port_is_prog(ckt_global_port);
      if (false == is_both_op_signal) {
        VTR_LOGF_ERROR(
          __FILE__, __LINE__,
          "Global port '%s' in tile annotation share the same name as global "
          "port '%s' in circuit library, which is defined for programming "
          "usage!\n",
          tile_annotation.global_port_name(tile_global_port).c_str(),
          circuit_lib.port_prefix(ckt_global_port).c_str());
        num_err++;
      }

      /* Error out if one is defined as clock while another is not */
      bool is_clock_attr_same =
        (tile_annotation.global_port_is_clock(tile_global_port) !=
         (CIRCUIT_MODEL_PORT_CLOCK == circuit_lib.port_type(ckt_global_port)));
      if (false == is_clock_attr_same) {
        VTR_LOGF_ERROR(
          __FILE__, __LINE__,
          "Global port '%s' in tile annotation share the same name as global "
          "port '%s' in circuit library but has different definition as "
          "clock!\n",
          tile_annotation.global_port_name(tile_global_port).c_str(),
          circuit_lib.port_prefix(ckt_global_port).c_str());
        num_err++;
      }

      /* Error out if one is defined as reset while another is not */
      bool is_reset_attr_same =
        (tile_annotation.global_port_is_reset(tile_global_port) !=
         circuit_lib.port_is_reset(ckt_global_port));
      if (false == is_reset_attr_same) {
        VTR_LOGF_ERROR(
          __FILE__, __LINE__,
          "Global port '%s' in tile annotation share the same name as global "
          "port '%s' in circuit library but has different definition as "
          "reset!\n",
          tile_annotation.global_port_name(tile_global_port).c_str(),
          circuit_lib.port_prefix(ckt_global_port).c_str());
        num_err++;
      }

      /* Error out if one is defined as set while another is not */
      bool is_set_attr_same =
        (tile_annotation.global_port_is_set(tile_global_port) !=
         circuit_lib.port_is_set(ckt_global_port));
      if (false == is_set_attr_same) {
        VTR_LOGF_ERROR(
          __FILE__, __LINE__,
          "Global port '%s' in tile annotation share the same name as global "
          "port '%s' in circuit library but has different definition as set!\n",
          tile_annotation.global_port_name(tile_global_port).c_str(),
          circuit_lib.port_prefix(ckt_global_port).c_str());
        num_err++;
      }
    }
  }

  return num_err;
}

/********************************************************************
 * Check if the tile annotation is valid without any conflict with
 * physical tile definition.
 * Items to check:
 * - The global port defined in tile annotation is a valid port/pin in
 *   the physical tile definition.
 * - If the port properties match:
 *   - the port in physical tile should have Fc = 0
 *   - a clock port should be also a clock port in physical tile
 *   - a non-clock port should be defined as a non-clock global port in physical
 *tile
 *******************************************************************/
static int check_tile_annotation_conflicts_with_physical_tile(
  const TileAnnotation& tile_annotation,
  const std::vector<t_physical_tile_type>& physical_tile_types) {
  int num_err = 0;

  for (const TileGlobalPortId& tile_global_port :
       tile_annotation.global_ports()) {
    for (size_t tile_info_id = 0;
         tile_info_id <
         tile_annotation.global_port_tile_names(tile_global_port).size();
         ++tile_info_id) {
      /* Must find a valid physical tile in the same name */
      size_t found_matched_physical_tile = 0;

      std::string required_tile_name =
        tile_annotation.global_port_tile_names(tile_global_port)[tile_info_id];
      BasicPort required_tile_port =
        tile_annotation.global_port_tile_ports(tile_global_port)[tile_info_id];

      for (const t_physical_tile_type& physical_tile : physical_tile_types) {
        if (std::string(physical_tile.name) != required_tile_name) {
          continue;
        }

        /* Found a match, increment the counter */
        found_matched_physical_tile++;

        /* Must found a valid port where both port name and port size must
         * match!!! */
        for (const t_sub_tile& sub_tile : physical_tile.sub_tiles) {
          size_t found_matched_physical_tile_port = 0;
          for (const t_physical_tile_port& tile_port : sub_tile.ports) {
            if (std::string(tile_port.name) != required_tile_port.get_name()) {
              continue;
            }

            BasicPort ref_tile_port(
              tile_port.name, sub_tile.capacity.total() * tile_port.num_pins);
            /* Port size must be in range!!! */
            if (false == ref_tile_port.contained(required_tile_port)) {
              VTR_LOG_ERROR(
                "Tile annotation port '%s[%lu:%lu]' is out of the range of "
                "physical tile port '%s[%lu:%lu]'!",
                required_tile_port.get_name().c_str(),
                required_tile_port.get_lsb(), required_tile_port.get_msb(),
                ref_tile_port.get_name().c_str(), ref_tile_port.get_lsb(),
                ref_tile_port.get_msb());
              num_err++;
              continue;
            }

            /* Check if port property matches */
            int grid_pin_index = sub_tile.sub_tile_to_tile_pin_indices
                                   [tile_port.absolute_first_pin_index];

            if (tile_port.is_clock !=
                tile_annotation.global_port_is_clock(tile_global_port)) {
              VTR_LOGF_ERROR(
                __FILE__, __LINE__,
                "Tile port '%s.%s[%ld:%ld]' in tile annotation '%s' does not "
                "match physical tile port %s.%s in clock property (one is "
                "defined as clock while the other is not)!\n",
                required_tile_name.c_str(),
                required_tile_port.get_name().c_str(),
                required_tile_port.get_lsb(), required_tile_port.get_msb(),
                tile_annotation.global_port_name(tile_global_port).c_str(),
                physical_tile.name, tile_port.name);
              num_err++;
            }

            if ((false == tile_port.is_clock) &&
                (false == tile_port.is_non_clock_global)) {
              VTR_LOGF_ERROR(
                __FILE__, __LINE__,
                "Tile port '%s.%s[%ld:%ld]' in tile annotation '%s' match "
                "physical tile port %s.%s  but is not defined as a non-clock "
                "global port!\n",
                required_tile_name.c_str(),
                required_tile_port.get_name().c_str(),
                required_tile_port.get_lsb(), required_tile_port.get_msb(),
                tile_annotation.global_port_name(tile_global_port).c_str(),
                physical_tile.name, tile_port.name);
              num_err++;
            }

            float pin_Fc =
              find_physical_tile_pin_Fc(&physical_tile, grid_pin_index);
            if (0. != pin_Fc) {
              VTR_LOGF_ERROR(
                __FILE__, __LINE__,
                "Tile port '%s.%s[%ld:%ld]' in tile annotation '%s' match "
                "physical tile port %s.%s  but its Fc is not zero '%g' !\n",
                required_tile_name.c_str(),
                required_tile_port.get_name().c_str(),
                required_tile_port.get_lsb(), required_tile_port.get_msb(),
                tile_annotation.global_port_name(tile_global_port).c_str(),
                physical_tile.name, tile_port.name, pin_Fc);
            }

            found_matched_physical_tile_port++;
          }
          if (0 == found_matched_physical_tile_port) {
            VTR_LOGF_ERROR(
              __FILE__, __LINE__,
              "Tile port '%s.%s[%ld:%ld]' in tile annotation '%s' does not "
              "match "
              "any physical tile port!\n",
              required_tile_name.c_str(), required_tile_port.get_name().c_str(),
              required_tile_port.get_lsb(), required_tile_port.get_msb(),
              tile_annotation.global_port_name(tile_global_port).c_str());
            num_err++;
          }
          if (1 < found_matched_physical_tile_port) {
            VTR_LOGF_ERROR(
              __FILE__, __LINE__,
              "Tile port '%s.%s[%ld:%ld]' in tile annotation '%s' match more "
              "than "
              "1 physical tile port!\n",
              required_tile_name.c_str(), required_tile_port.get_name().c_str(),
              required_tile_port.get_lsb(), required_tile_port.get_msb(),
              tile_annotation.global_port_name(tile_global_port).c_str());
            num_err++;
          }
        }
      }

      /* If we found no match, error out */
      if (0 == found_matched_physical_tile) {
        VTR_LOGF_ERROR(
          __FILE__, __LINE__,
          "Tile name '%s' in tile annotation '%s' does not match any physical "
          "tile!\n",
          required_tile_name.c_str(),
          tile_annotation.global_port_name(tile_global_port).c_str());
        num_err++;
      }
      /* If we found more than 1 match, error out */
      if (1 < found_matched_physical_tile) {
        VTR_LOGF_ERROR(
          __FILE__, __LINE__,
          "Tile name '%s' in tile annotation '%s' match more than 1 physical "
          "tile!\n",
          required_tile_name.c_str(),
          tile_annotation.global_port_name(tile_global_port).c_str());
        num_err++;
      }
    }
  }

  return num_err;
}

/********************************************************************
 * Check if the tile annotation is valid without any conflict with
 * circuit library content and physical tiles.
 *******************************************************************/
bool check_tile_annotation(
  const TileAnnotation& tile_annotation, const CircuitLibrary& circuit_lib,
  const std::vector<t_physical_tile_type>& physical_tile_types) {
  int num_err = 0;
  num_err += check_tile_annotation_conflicts_with_circuit_library(
    tile_annotation, circuit_lib);

  num_err += check_tile_annotation_conflicts_with_physical_tile(
    tile_annotation, physical_tile_types);

  VTR_LOG("Found %ld errors when checking tile annotation!\n", num_err);
  return (0 == num_err);
}

} /* end namespace openfpga */
