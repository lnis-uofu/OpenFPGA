#ifndef BUILD_ROUTING_MODULE_UTILS_H
#define BUILD_ROUTING_MODULE_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/

#include <tuple>
#include <vector>

#include "device_grid.h"
#include "module_manager.h"
#include "rr_gsb.h"
#include "vpr_device_annotation.h"
#include "vpr_types.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

typedef std::pair<ModulePortId, size_t> ModulePinInfo;

std::string generate_sb_module_grid_port_name(
  const e_side& sb_side, const e_side& grid_side,
  const DeviceGrid& vpr_device_grid,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const RRNodeId& rr_node);

std::string generate_cb_module_grid_port_name(
  const e_side& cb_side, const DeviceGrid& vpr_device_grid,
  const VprDeviceAnnotation& vpr_device_annotation, const RRGraphView& rr_graph,
  const RRNodeId& rr_node);

ModulePinInfo find_switch_block_module_chan_port(
  const ModuleManager& module_manager, const ModuleId& sb_module,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_side& chan_side,
  const RRNodeId& cur_rr_node, const PORTS& cur_rr_node_direction);

ModulePinInfo find_switch_block_module_input_port(
  const ModuleManager& module_manager, const ModuleId& sb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_side& input_side,
  const RRNodeId& input_rr_node);

std::vector<ModulePinInfo> find_switch_block_module_input_ports(
  const ModuleManager& module_manager, const ModuleId& sb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const std::vector<RRNodeId>& input_rr_nodes);

ModulePinInfo find_connection_block_module_chan_port(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type,
  const RRNodeId& chan_rr_node);

ModulePortId find_connection_block_module_ipin_port(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const RRNodeId& src_rr_node);

ModulePortId find_connection_block_module_opin_port(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb,
  const RRNodeId& src_rr_node);

std::vector<ModulePinInfo> find_connection_block_module_input_ports(
  const ModuleManager& module_manager, const ModuleId& cb_module,
  const DeviceGrid& grids, const VprDeviceAnnotation& vpr_device_annotation,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const t_rr_type& cb_type,
  const std::vector<RRNodeId>& input_rr_nodes);

} /* end namespace openfpga */

#endif
