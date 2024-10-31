#ifndef TILE_ANNOTATION_H
#define TILE_ANNOTATION_H

/********************************************************************
 * Include header files required by the data structure definition
 *******************************************************************/
#include <array>
#include <map>
#include <vector>

#include "openfpga_port.h"
#include "tile_annotation_fwd.h"
#include "vtr_geometry.h"
#include "vtr_vector.h"

/* namespace openfpga begins */
namespace openfpga {

/********************************************************************
 * This file include the declaration of data structures
 * to store physical tile annotation, including
 * 1. global port definition where a tile port can be treated as a
 *    global port of the FPGA fabric
 *
 * Note:
 * 1. Keep this data structure as general as possible. It is supposed
 *    to contain the raw data from architecture XML! If you want to link
 *    to other data structures, please create another one in other header files
 *******************************************************************/
class TileAnnotation {
 public: /* Types */
  typedef vtr::vector<TileGlobalPortId, TileGlobalPortId>::const_iterator
    global_port_iterator;
  /* Create range */
  typedef vtr::Range<global_port_iterator> global_port_range;

 public: /* Constructor */
  TileAnnotation();

 public: /* Public accessors: aggregators */
  global_port_range global_ports() const;
  std::vector<std::string> tiles_to_merge_ports() const;
  std::vector<std::string> tile_ports_to_merge(
    const std::string& tile_name) const;

 public: /* Public accessors */
  std::string global_port_name(const TileGlobalPortId& global_port_id) const;
  std::vector<std::string> global_port_tile_names(
    const TileGlobalPortId& global_port_id) const;
  std::vector<BasicPort> global_port_tile_ports(
    const TileGlobalPortId& global_port_id) const;
  std::vector<vtr::Point<size_t>> global_port_tile_coordinates(
    const TileGlobalPortId& global_port_id) const;
  bool global_port_is_clock(const TileGlobalPortId& global_port_id) const;
  bool global_port_is_set(const TileGlobalPortId& global_port_id) const;
  bool global_port_is_reset(const TileGlobalPortId& global_port_id) const;
  bool global_port_thru_dedicated_network(
    const TileGlobalPortId& global_port_id) const;
  std::string global_port_clock_arch_tree_name(
    const TileGlobalPortId& global_port_id) const;
  size_t global_port_default_value(
    const TileGlobalPortId& global_port_id) const;

  /** @brief Check if a given tile port should be merged or not */
  bool is_tile_port_to_merge(const std::string& tile_name,
                             const std::string& port_name) const;

 public: /* Public mutators */
  /* By default, we do not set it as a clock.
   * Users should set it through the set_global_port_is_clock() function
   */
  TileGlobalPortId create_global_port(const std::string& port_name);
  /* Add tile port information */
  void add_global_port_tile_information(const TileGlobalPortId& global_port_id,
                                        const std::string& tile_name,
                                        const BasicPort& tile_port,
                                        const vtr::Point<size_t>& tile_coord);
  void set_global_port_is_clock(const TileGlobalPortId& global_port_id,
                                const bool& is_clock);
  void set_global_port_clock_arch_tree_name(
    const TileGlobalPortId& global_port_id, const std::string& clock_tree_name);
  void set_global_port_is_set(const TileGlobalPortId& global_port_id,
                              const bool& is_set);
  void set_global_port_is_reset(const TileGlobalPortId& global_port_id,
                                const bool& is_reset);
  void set_global_port_default_value(const TileGlobalPortId& global_port_id,
                                     const size_t& default_value);

  int add_merge_subtile_ports(const std::string& tile_name,
                              const std::string& port_name);

 public: /* Public validator */
  bool valid_global_port_id(const TileGlobalPortId& global_port_id) const;
  /* Validate attributes of a given global port
   * - A port can only be defined as clock or set or reset
   */
  bool valid_global_port_attributes(
    const TileGlobalPortId& global_port_id) const;

 private: /* Internal data */
  /* Global port information for tiles */
  vtr::vector<TileGlobalPortId, TileGlobalPortId> global_port_ids_;
  vtr::vector<TileGlobalPortId, std::string> global_port_names_;
  vtr::vector<TileGlobalPortId, std::vector<std::string>>
    global_port_tile_names_;
  vtr::vector<TileGlobalPortId, std::vector<vtr::Point<size_t>>>
    global_port_tile_coordinates_;
  vtr::vector<TileGlobalPortId, std::vector<BasicPort>> global_port_tile_ports_;
  vtr::vector<TileGlobalPortId, bool> global_port_is_clock_;
  vtr::vector<TileGlobalPortId, std::string> global_port_clock_arch_tree_names_;
  vtr::vector<TileGlobalPortId, bool> global_port_is_reset_;
  vtr::vector<TileGlobalPortId, bool> global_port_is_set_;
  vtr::vector<TileGlobalPortId, size_t> global_port_default_values_;

  /* A fast lookup for port names */
  std::map<std::string, TileGlobalPortId> global_port_name2ids_;

  /* Merge port information for tiles */
  std::map<std::string, std::vector<std::string>>
    tile_ports_to_merge_;  // tile_name -> port_name
};

}  // namespace openfpga

#endif
