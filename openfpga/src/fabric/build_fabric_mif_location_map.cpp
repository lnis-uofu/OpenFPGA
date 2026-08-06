/********************************************************************
 * Build MIF location map: (x, y, z, pb) -> (port, data_offset, width).
 *
 * Walks top-module io_children in the same order GPIN buses are
 * concatenated, and assigns a bit offset for each is_mif_data_bus port.
 *******************************************************************/
#include "build_fabric_mif_location_map.h"

#include <map>
#include <string>
#include <vector>

#include "arch_util.h"
#include "openfpga_naming.h"
#include "openfpga_pb_parser.h"
#include "openfpga_reserved_words.h"
#include "vpr_utils.h"
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

static bool tile_matches_mif_pb(t_physical_tile_type_ptr phy_tile_type,
                                const std::string& pb_type_path) {
  const PbParser parser(strip_numeric_pb_index(pb_type_path));
  const std::string top_name =
    parser.parents().empty() ? parser.leaf() : parser.parents().front();
  if (top_name.empty()) {
    return false;
  }
  if (phy_tile_type->name == top_name) {
    return true;
  }
  for (t_logical_block_type_ptr site :
       get_equivalent_sites_set(phy_tile_type)) {
    if (site != nullptr && site->name == top_name) {
      return true;
    }
  }
  return false;
}

static std::vector<std::string> matching_mif_pb_paths(
  t_physical_tile_type_ptr phy_tile_type,
  const std::vector<std::string>& mif_pb_paths) {
  std::vector<std::string> matched;
  for (const std::string& pb_type_path : mif_pb_paths) {
    if (tile_matches_mif_pb(phy_tile_type, pb_type_path)) {
      matched.push_back(pb_type_path);
    }
  }
  return matched;
}

static bool skip_non_root_grid_cell(const DeviceGrid& grids, const int& x,
                                    const int& y, const size_t& layer) {
  const t_physical_tile_loc phy_tile_loc(x, y, layer);
  if (!grids.is_valid_tile_loc(phy_tile_loc)) {
    return true;
  }
  t_physical_tile_type_ptr phy_tile_type =
    grids.get_physical_type(phy_tile_loc);
  if (true == is_empty_type(phy_tile_type)) {
    return true;
  }
  /* Skip non-root cells of multi-width/height tiles. */
  return !grids.is_root_location(phy_tile_loc);
}

static std::map<std::string, size_t> collect_mif_data_bus_ports(
  const CircuitLibrary& circuit_lib) {
  std::map<std::string, size_t> mif_data_ports;
  for (const CircuitPortId& port : circuit_lib.ports()) {
    if (false == circuit_lib.port_is_mif_data_bus(port)) {
      continue;
    }
    const CircuitModelId parent_model = circuit_lib.port_parent_model(port);
    const std::string port_name = generate_fpga_global_io_port_name(
      std::string(GIO_INOUT_PREFIX), circuit_lib, parent_model, port);
    mif_data_ports[port_name] = circuit_lib.port_size(port);
  }
  return mif_data_ports;
}

static void register_subchild_mif_locations(
  MifLocationMap& mif_location_map, const ModuleManager& module_manager,
  const ModuleId& subchild, const int& x, const int& y, const int& z,
  const std::vector<std::string>& pb_paths,
  const std::map<std::string, size_t>& mif_data_ports,
  std::map<std::string, size_t>& offset_counter) {
  for (const ModulePortId& gpin_port_id :
       module_manager.module_port_ids_by_type(
         subchild, ModuleManager::MODULE_GPIN_PORT)) {
    const BasicPort& gpin_port =
      module_manager.module_port(subchild, gpin_port_id);
    auto port_info = mif_data_ports.find(gpin_port.get_name());
    if (port_info == mif_data_ports.end()) {
      continue;
    }

    const std::string& port_name = port_info->first;
    const size_t width = gpin_port.get_width();
    VTR_ASSERT(width == port_info->second);

    size_t& offset = offset_counter[port_name];

    /* One physical instance on the bus; associate all matching pb paths. */
    for (const std::string& pb_path : pb_paths) {
      mif_location_map.set_mif_location(
        static_cast<size_t>(x), static_cast<size_t>(y), static_cast<size_t>(z),
        pb_path, port_name, offset, width);
    }
    offset += width;
  }
}

/* Shared by fine-grained and tiled builders: register MIF offsets for one
 * grid-level module and its capacity (z) io_children. */
static void register_grid_module_mif_locations(
  MifLocationMap& mif_location_map, const ModuleManager& module_manager,
  const DeviceGrid& grids, const size_t& layer, const ModuleId& grid_module,
  const int& x, const int& y, const std::vector<std::string>& mif_pb_paths,
  const std::map<std::string, size_t>& mif_data_ports,
  std::map<std::string, size_t>& offset_counter) {
  if (true == skip_non_root_grid_cell(grids, x, y, layer)) {
    return;
  }

  t_physical_tile_loc phy_tile_loc(x, y, layer);
  t_physical_tile_type_ptr phy_tile_type =
    grids.get_physical_type(phy_tile_loc);
  const std::vector<std::string> pb_paths =
    matching_mif_pb_paths(phy_tile_type, mif_pb_paths);
  if (pb_paths.empty()) {
    return;
  }

  if (size_t(phy_tile_type->capacity) !=
      module_manager.io_children(grid_module).size()) {
    VTR_LOG("%s[%d][%d] capacity: %d while io_child number is %zu\n",
            phy_tile_type->name.c_str(), x, y, phy_tile_type->capacity,
            module_manager.io_children(grid_module).size());
  }

  for (size_t isubchild = 0;
       isubchild < module_manager.io_children(grid_module).size();
       ++isubchild) {
    const ModuleId subchild =
      module_manager.io_children(grid_module)[isubchild];
    const vtr::Point<int>& subchild_coord =
      module_manager.io_child_coordinates(grid_module)[isubchild];
    /* subchild_coord.x() is tile capacity index (VPR subblk); default (-1,-1)
     * when unset — fall back to io_children order index. */
    int z = subchild_coord.x();
    if (0 > z) {
      z = static_cast<int>(isubchild);
    }
    if (z < 0 || size_t(z) >= size_t(phy_tile_type->capacity)) {
      continue;
    }
    register_subchild_mif_locations(mif_location_map, module_manager, subchild,
                                    x, y, z, pb_paths, mif_data_ports,
                                    offset_counter);
  }
}

MifLocationMap build_fabric_mif_location_map(
  const ModuleManager& module_manager, const DeviceGrid& grids,
  const CircuitLibrary& circuit_lib, const BitstreamSetting& bitstream_setting,
  const bool& tiled_fabric) {
  vtr::ScopedStartFinishTimer timer(
    "Create MIF location mapping for fabric grids");

  const std::map<std::string, size_t> mif_data_ports =
    collect_mif_data_bus_ports(circuit_lib);
  if (mif_data_ports.empty()) {
    return MifLocationMap();
  }

  std::vector<std::string> mif_pb_paths;
  for (const MifSourceSettingId& id : bitstream_setting.mif_source_settings()) {
    mif_pb_paths.push_back(bitstream_setting.mif_source_pb_type(id));
  }
  if (mif_pb_paths.empty()) {
    VTR_LOG(
      "Found %zu is_mif_data_bus port(s) but no mif_source settings; skip MIF "
      "location map\n",
      mif_data_ports.size());
    return MifLocationMap();
  }

  MifLocationMap mif_location_map;
  std::map<std::string, size_t> offset_counter;
  const size_t layer = 0;
  const ModuleId top_module =
    module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  for (size_t ichild = 0;
       ichild < module_manager.io_children(top_module).size(); ++ichild) {
    const ModuleId child_module =
      module_manager.io_children(top_module)[ichild];
    if (tiled_fabric) {
      for (size_t igrid = 0;
           igrid < module_manager.io_children(child_module).size(); ++igrid) {
        const ModuleId grid_module =
          module_manager.io_children(child_module)[igrid];
        const vtr::Point<int>& grid_coord =
          module_manager.io_child_coordinates(child_module)[igrid];
        register_grid_module_mif_locations(
          mif_location_map, module_manager, grids, layer, grid_module,
          grid_coord.x(), grid_coord.y(), mif_pb_paths, mif_data_ports,
          offset_counter);
      }
      continue;
    }

    const vtr::Point<int>& grid_coord =
      module_manager.io_child_coordinates(top_module)[ichild];
    register_grid_module_mif_locations(mif_location_map, module_manager, grids,
                                       layer, child_module, grid_coord.x(),
                                       grid_coord.y(), mif_pb_paths,
                                       mif_data_ports, offset_counter);
  }

  VTR_LOG(
    "Built MIF location map: %zu location(s) across %zu mif_data_bus "
    "port(s)\n",
    mif_location_map.mif_locations().size(), mif_data_ports.size());
  return mif_location_map;
}

} /* end namespace openfpga */
