/********************************************************************
 * Build MIF location map: top-level port -> (t_pl_loc, MifPortSlice).
 *
 * Walks top-module mif_children (modules that contain mif_data_bus) in
 * GPIN concatenation order and assigns data_offset/data_width along that
 * same order. The physical primitive for each instance is taken from
 * bitstream annotation and remapped by device annotation (operating pb ->
 * physical pb).
 *******************************************************************/
#include "build_fabric_mif_location_map.h"

#include <map>
#include <set>
#include <string>
#include <vector>

#include "arch_util.h"
#include "circuit_library_utils.h"
#include "openfpga_naming.h"
#include "openfpga_reserved_words.h"
#include "vpr_utils.h"
#include "vtr_assert.h"
#include "vtr_time.h"

/* begin namespace openfpga */
namespace openfpga {

/** @brief Get a list of unique pb_graph nodes which are
 * - primitive nodes
 * - require memory initialization file (MIF) defined through bitstream annotation
 * TODO: This should be a method of the bitstream annotation
 */ 
static std::vector<t_pb_graph_node*> collect_physical_mif_nodes(
  const VprBitstreamAnnotation& bitstream_annotation,
  const VprDeviceAnnotation& device_annotation) {
  std::vector<t_pb_graph_node*> physical_nodes;
  std::set<t_pb_graph_node*> seen;
  for (size_t i = 0; i < bitstream_annotation.num_mif_sources(); ++i) {
    t_pb_graph_node* node =
      bitstream_annotation.mif_source_pb_graph_node(MifSourceAnnotationId(i));
    if (nullptr == node) {
      continue;
    }
    t_pb_graph_node* physical_node =
      device_annotation.physical_pb_graph_node(node);
    if (nullptr == physical_node) {
      physical_node = node;
    }
    if (false == seen.insert(physical_node).second) {
      continue;
    }
    physical_nodes.push_back(physical_node);
  }
  return physical_nodes;
}

static t_pb_graph_node* matching_physical_mif_node(
  t_physical_tile_type_ptr phy_tile_type,
  const std::vector<t_pb_graph_node*>& physical_nodes) {
  for (t_pb_graph_node* node : physical_nodes) {
    t_pb_graph_node* root = node;
    while (nullptr != root && false == root->is_root()) {
      root = root->parent_pb_graph_node;
    }
    if (nullptr == root || nullptr == root->pb_type) {
      continue;
    }
    const std::string top_name = root->pb_type->name;
    if (phy_tile_type->name == top_name) {
      return node;
    }
    for (t_logical_block_type_ptr site :
         get_equivalent_sites_set(phy_tile_type)) {
      if (site != nullptr && site->name == top_name) {
        return node;
      }
    }
  }
  return nullptr;
}

static std::map<std::string, size_t> collect_mif_data_bus_ports(
  const CircuitLibrary& circuit_lib,
  const VprDeviceAnnotation& device_annotation,
  const std::vector<t_pb_graph_node*>& physical_nodes) {
  std::map<std::string, size_t> mif_data_ports;
  std::set<CircuitModelId> models;
  for (t_pb_graph_node* node : physical_nodes) {
    if (nullptr == node || nullptr == node->pb_type) {
      continue;
    }
    t_pb_type* physical_pb_type =
      device_annotation.physical_pb_type(node->pb_type);
    if (nullptr == physical_pb_type) {
      physical_pb_type = node->pb_type;
    }
    const CircuitModelId model =
      device_annotation.pb_type_circuit_model(physical_pb_type);
    if (true == circuit_lib.valid_model_id(model)) {
      models.insert(model);
    }
  }

  auto add_mif_port = [&](const CircuitPortId& port) {
    if (false == circuit_lib.port_is_mif_data_bus(port)) {
      return;
    }
    const CircuitModelId parent_model = circuit_lib.port_parent_model(port);
    const std::string port_name = generate_fpga_global_io_port_name(
      std::string(GIO_INOUT_PREFIX), circuit_lib, parent_model, port);
    mif_data_ports[port_name] = circuit_lib.port_size(port);
  };

  if (false == models.empty()) {
    for (const CircuitModelId& model : models) {
      for (const CircuitPortId& port : circuit_lib.model_ports(model)) {
        add_mif_port(port);
      }
    }
  } else {
    for (const CircuitPortId& port :
         find_circuit_library_global_ports(circuit_lib)) {
      add_mif_port(port);
    }
  }
  return mif_data_ports;
}

static void register_grid_module_mif_locations(
  MifLocationMap& mif_location_map, const ModuleManager& module_manager,
  const DeviceGrid& grids, const size_t& layer, const ModuleId& grid_module,
  const int& x, const int& y,
  const std::vector<t_pb_graph_node*>& physical_nodes,
  const std::map<std::string, size_t>& mif_data_ports,
  std::map<std::string, size_t>& offset_counter) {
  const t_physical_tile_loc phy_tile_loc(x, y, layer);
  if (!grids.is_valid_tile_loc(phy_tile_loc) ||
      !grids.is_root_location(phy_tile_loc)) {
    return;
  }
  t_physical_tile_type_ptr phy_tile_type =
    grids.get_physical_type(phy_tile_loc);
  if (true == is_empty_type(phy_tile_type)) {
    return;
  }

  t_pb_graph_node* pb_graph_node =
    matching_physical_mif_node(phy_tile_type, physical_nodes);
  if (nullptr == pb_graph_node) {
    return;
  }

  for (size_t isubchild = 0;
       isubchild < module_manager.mif_children(grid_module).size();
       ++isubchild) {
    ModuleId subchild =
      module_manager.mif_children(grid_module)[isubchild];
    vtr::Point<int> subchild_coord =
      module_manager.mif_child_coordinates(grid_module)[isubchild];
    int z = subchild_coord.x();
    if (0 > z) {
      z = static_cast<int>(isubchild);
    }
    if (z < 0 || size_t(z) >= size_t(phy_tile_type->capacity)) {
      continue;
    }

    const t_pl_loc phy_loc(x, y, z, static_cast<int>(layer));
    for (const ModulePortId& gpin_port_id :
         module_manager.module_port_ids_by_type(
           subchild, ModuleManager::MODULE_GPIN_PORT)) {
      const BasicPort& gpin_port =
        module_manager.module_port(subchild, gpin_port_id);
      auto port_info = mif_data_ports.find(gpin_port.get_name());
      if (port_info == mif_data_ports.end()) {
        continue;
      }
      VTR_ASSERT(gpin_port.get_width() == port_info->second);
      size_t offset = offset_counter[port_info->first];
      mif_location_map.add(port_info->first, phy_loc, pb_graph_node, offset,
                           gpin_port.get_width());
      offset += gpin_port.get_width();
    }
  }
}

MifLocationMap build_fabric_mif_location_map(
  const ModuleManager& module_manager, const DeviceGrid& grids,
  const CircuitLibrary& circuit_lib,
  const VprBitstreamAnnotation& bitstream_annotation,
  const VprDeviceAnnotation& device_annotation, const bool& tiled_fabric) {
  vtr::ScopedStartFinishTimer timer(
    "Create MIF location mapping for fabric grids");
  /* Collect unique pb_graph nodes that require MIF file */
  const std::vector<t_pb_graph_node*> physical_nodes =
    collect_physical_mif_nodes(bitstream_annotation, device_annotation);
  if (true == physical_nodes.empty()) {
    return MifLocationMap();
  }
  /* Collect all the data ports (name + port width) of each circuit lib used in the architecture (valid one in the unique pb_graph node list) */
  const std::map<std::string, size_t> mif_data_ports =
    collect_mif_data_bus_ports(circuit_lib, device_annotation, physical_nodes);
  if (true == mif_data_ports.empty()) {
    return MifLocationMap();
  }
  /* Now walk through all the MIF children under the top-level module
   * For each MIF child, record its offset in the mif data bus at top-level module
   */
  MifLocationMap mif_location_map;
  std::map<std::string, size_t> offset_counter;
  const size_t layer = 0;
  const ModuleId top_module =
    module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  for (size_t ichild = 0;
       ichild < module_manager.mif_children(top_module).size(); ++ichild) {
    const ModuleId child_module =
      module_manager.mif_children(top_module)[ichild];
    if (tiled_fabric) {
      for (size_t igrid = 0;
           igrid < module_manager.mif_children(child_module).size(); ++igrid) {
        const ModuleId grid_module =
          module_manager.mif_children(child_module)[igrid];
        vtr::Point<int> curr_grid_coord =
          module_manager.mif_child_coordinates(child_module)[igrid];
        register_grid_module_mif_locations(
          mif_location_map, module_manager, grids, layer, grid_module,
          curr_grid_coord.x(), curr_grid_coord.y(), physical_nodes,
          mif_data_ports, offset_counter);
      }
      continue;
    }

    vtr::Point<int> grid_coord =
      module_manager.mif_child_coordinates(top_module)[ichild];
    register_grid_module_mif_locations(mif_location_map, module_manager, grids,
                                       layer, child_module, grid_coord.x(),
                                       grid_coord.y(), physical_nodes,
                                       mif_data_ports, offset_counter);
  }

  return mif_location_map;
}

} /* end namespace openfpga */
