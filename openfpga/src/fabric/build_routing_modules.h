#ifndef BUILD_ROUTING_MODULES_H
#define BUILD_ROUTING_MODULES_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include "circuit_library.h"
#include "decoder_library.h"
#include "device_rr_gsb.h"
#include "module_manager.h"
#include "mux_library.h"
#include "vpr_context.h"
#include "vpr_device_annotation.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

typedef std::pair<ModulePortId, size_t> ModulePinInfo;

static void build_connection_block_module_short_interc(
  ModuleManager& module_manager, const ModuleId& cb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_rr_type& cb_type,
  const e_side& cb_ipin_side, const size_t& ipin_index,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets);

static void build_connection_block_mux_module(
  ModuleManager& module_manager, const ModuleId& cb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_rr_type& cb_type,
  const CircuitLibrary& circuit_lib, const e_side& cb_ipin_side,
  const size_t& ipin_index,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets,
  const bool& group_config_block, const bool& group_routing);

static void build_connection_block_interc_modules(
  ModuleManager& module_manager, const ModuleId& cb_module,
  const VprDeviceAnnotation& device_annotation, const DeviceGrid& grids,
  const RRGraphView& rr_graph, const RRGSB& rr_gsb, const e_rr_type& cb_type,
  const CircuitLibrary& circuit_lib, const e_side& cb_ipin_side,
  const size_t& ipin_index,
  const std::map<ModulePinInfo, ModuleNetId>& input_port_to_module_nets,
  const bool& group_config_block);

void build_flatten_routing_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const DeviceContext& device_ctx, const VprDeviceAnnotation& device_annotation,
  const DeviceRRGSB& device_rr_gsb, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const bool& group_config_block,
  const bool& group_routing, const bool& verbose);

void build_unique_routing_modules(
  ModuleManager& module_manager, DecoderLibrary& decoder_lib,
  const DeviceContext& device_ctx, const VprDeviceAnnotation& device_annotation,
  const DeviceRRGSB& device_rr_gsb, const CircuitLibrary& circuit_lib,
  const e_config_protocol_type& sram_orgz_type,
  const CircuitModelId& sram_model, const bool& group_config_block,
  const bool& group_routing, const bool& verbose);

} /* end namespace openfpga */

#endif
