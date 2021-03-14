/********************************************************************
 * This file includes functions that are used to add module nets
 * for direct connections between CLBs/heterogeneous blocks
 * in the top-level module of a FPGA fabric
 *******************************************************************/
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from openfpgautil library */
#include "openfpga_port.h"

/* Headers from vpr library */
#include "vpr_utils.h"

#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"
#include "module_manager_utils.h"

#include "build_top_module_directs.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Add module net for one direction connection between two CLBs or
 * two grids
 * This function will 
 * 1. find the pin id and port id of the source clb port in module manager
 * 2. find the pin id and port id of the destination clb port in module manager
 * 3. add a direct connection module to the top module 
 * 4. add a first module net and configure its source and sink, 
 * in order to connect the source pin to the input of the top module
 * 4. add a second module net and configure its source and sink, 
 * in order to connect the sink pin to the output of the top module
 *******************************************************************/
static 
void add_module_nets_tile_direct_connection(ModuleManager& module_manager, 
                                            const ModuleId& top_module,  
                                            const CircuitLibrary& circuit_lib, 
                                            const VprDeviceAnnotation& vpr_device_annotation,
                                            const DeviceGrid& grids,
                                            const vtr::Matrix<size_t>& grid_instance_ids,
                                            const TileDirect& tile_direct,
                                            const TileDirectId& tile_direct_id,
                                            const ArchDirect& arch_direct) {
  vtr::Point<size_t> device_size(grids.width(), grids.height());

  /* Find the module name of source clb */
  vtr::Point<size_t> src_clb_coord = tile_direct.from_tile_coordinate(tile_direct_id);
  t_physical_tile_type_ptr src_grid_type = grids[src_clb_coord.x()][src_clb_coord.y()].type;
  e_side src_grid_border_side = find_grid_border_side(device_size, src_clb_coord);
  std::string src_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string src_module_name = generate_grid_block_module_name(src_module_name_prefix, std::string(src_grid_type->name), is_io_type(src_grid_type), src_grid_border_side);
  ModuleId src_grid_module = module_manager.find_module(src_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(src_grid_module));
  /* Record the instance id */
  size_t src_grid_instance = grid_instance_ids[src_clb_coord.x()][src_clb_coord.y()];

  /* Find the module name of sink clb */
  vtr::Point<size_t> des_clb_coord = tile_direct.to_tile_coordinate(tile_direct_id);
  t_physical_tile_type_ptr sink_grid_type = grids[des_clb_coord.x()][des_clb_coord.y()].type;
  e_side sink_grid_border_side = find_grid_border_side(device_size, des_clb_coord);
  std::string sink_module_name_prefix(GRID_MODULE_NAME_PREFIX);
  std::string sink_module_name = generate_grid_block_module_name(sink_module_name_prefix, std::string(sink_grid_type->name), is_io_type(sink_grid_type), sink_grid_border_side);
  ModuleId sink_grid_module = module_manager.find_module(sink_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(sink_grid_module));
  /* Record the instance id */
  size_t sink_grid_instance = grid_instance_ids[des_clb_coord.x()][des_clb_coord.y()];

  /* Find the module id of a direct connection module */
  CircuitModelId direct_circuit_model = arch_direct.circuit_model(tile_direct.arch_direct(tile_direct_id));
  std::string direct_module_name = circuit_lib.model_name(direct_circuit_model);
  ModuleId direct_module = module_manager.find_module(direct_module_name);
  VTR_ASSERT(true == module_manager.valid_module_id(direct_module));

  /* Find inputs and outputs of the direct circuit module */
  std::vector<CircuitPortId> direct_input_ports = circuit_lib.model_ports_by_type(direct_circuit_model, CIRCUIT_MODEL_PORT_INPUT, true);
  VTR_ASSERT(1 == direct_input_ports.size());
  ModulePortId direct_input_port_id = module_manager.find_module_port(direct_module, circuit_lib.port_prefix(direct_input_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(direct_module, direct_input_port_id));
  VTR_ASSERT(1 == module_manager.module_port(direct_module, direct_input_port_id).get_width());

  std::vector<CircuitPortId> direct_output_ports = circuit_lib.model_ports_by_type(direct_circuit_model, CIRCUIT_MODEL_PORT_OUTPUT, true);
  VTR_ASSERT(1 == direct_output_ports.size());
  ModulePortId direct_output_port_id = module_manager.find_module_port(direct_module, circuit_lib.port_prefix(direct_output_ports[0]));
  VTR_ASSERT(true == module_manager.valid_module_port_id(direct_module, direct_output_port_id));
  VTR_ASSERT(1 == module_manager.module_port(direct_module, direct_output_port_id).get_width());

  /* Generate the pin name of source port/pin in the grid */
  e_side src_pin_grid_side = tile_direct.from_tile_side(tile_direct_id);
  size_t src_tile_pin = tile_direct.from_tile_pin(tile_direct_id);

  t_physical_tile_type_ptr src_grid_type_descriptor = grids[src_clb_coord.x()][src_clb_coord.y()].type;
  size_t src_pin_width = src_grid_type_descriptor->pin_width_offset[src_tile_pin]; 
  size_t src_pin_height = src_grid_type_descriptor->pin_height_offset[src_tile_pin]; 

  BasicPort src_pin_info = vpr_device_annotation.physical_tile_pin_port_info(src_grid_type_descriptor, src_tile_pin);
  VTR_ASSERT(true == src_pin_info.is_valid());
  int src_subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(src_grid_type_descriptor, src_tile_pin);
  VTR_ASSERT(OPEN != src_subtile_index && src_subtile_index < src_grid_type_descriptor->capacity);
  std::string src_port_name = generate_grid_port_name(src_pin_width, src_pin_height, src_subtile_index, src_pin_grid_side, src_pin_info);
  ModulePortId src_port_id = module_manager.find_module_port(src_grid_module, src_port_name); 
  if (true != module_manager.valid_module_port_id(src_grid_module, src_port_id)) {
    VTR_LOG_ERROR("Fail to find port '%s[%lu][%lu].%s'\n",
                  src_module_name.c_str(), src_clb_coord.x(), src_clb_coord.y(),
                  src_port_name.c_str());
  }
  VTR_ASSERT(true == module_manager.valid_module_port_id(src_grid_module, src_port_id));
  VTR_ASSERT(1 == module_manager.module_port(src_grid_module, src_port_id).get_width());

  /* Generate the pin name of sink port/pin in the grid */
  e_side sink_pin_grid_side = tile_direct.to_tile_side(tile_direct_id);
  size_t sink_tile_pin = tile_direct.to_tile_pin(tile_direct_id);

  t_physical_tile_type_ptr sink_grid_type_descriptor = grids[des_clb_coord.x()][des_clb_coord.y()].type;
  size_t sink_pin_width = sink_grid_type_descriptor->pin_width_offset[src_tile_pin]; 
  size_t sink_pin_height = sink_grid_type_descriptor->pin_height_offset[src_tile_pin]; 

  BasicPort sink_pin_info = vpr_device_annotation.physical_tile_pin_port_info(sink_grid_type_descriptor, sink_tile_pin);
  VTR_ASSERT(true == sink_pin_info.is_valid());
  int sink_subtile_index = vpr_device_annotation.physical_tile_pin_subtile_index(sink_grid_type_descriptor, sink_tile_pin);
  VTR_ASSERT(OPEN != src_subtile_index && src_subtile_index < sink_grid_type_descriptor->capacity);
  std::string sink_port_name = generate_grid_port_name(sink_pin_width, sink_pin_height, sink_subtile_index, sink_pin_grid_side, sink_pin_info);
  ModulePortId sink_port_id = module_manager.find_module_port(sink_grid_module, sink_port_name); 
  VTR_ASSERT(true == module_manager.valid_module_port_id(sink_grid_module, sink_port_id));
  VTR_ASSERT(1 == module_manager.module_port(sink_grid_module, sink_port_id).get_width());

  /* Add a submodule of direct connection module to the top-level module */
  size_t direct_instance_id = module_manager.num_instance(top_module, direct_module);
  module_manager.add_child_module(top_module, direct_module);

  /* Create the 1st module net */
  ModuleNetId net_direct_src = module_manager.create_module_net(top_module); 
  /* Connect the wire between src_pin of clb and direct_instance input*/
  module_manager.add_module_net_source(top_module, net_direct_src, src_grid_module, src_grid_instance, src_port_id, 0);
  module_manager.add_module_net_sink(top_module, net_direct_src, direct_module, direct_instance_id, direct_input_port_id, 0);

  /* Create the 2nd module net
   * Connect the wire between direct_instance output and sink_pin of clb
   */
  ModuleNetId net_direct_sink = create_module_source_pin_net(module_manager, top_module, direct_module, direct_instance_id, direct_output_port_id, 0);
  module_manager.add_module_net_sink(top_module, net_direct_sink, sink_grid_module, sink_grid_instance, sink_port_id, 0);
}

/********************************************************************
 * Add module net of clb-to-clb direct connections to module manager
 * Note that the direct connections are not limited to CLBs only.
 * It can be more generic and thus cover all the grid types,
 * such as heterogeneous blocks
 *******************************************************************/
void add_top_module_nets_tile_direct_connections(ModuleManager& module_manager, 
                                                 const ModuleId& top_module, 
                                                 const CircuitLibrary& circuit_lib, 
                                                 const VprDeviceAnnotation& vpr_device_annotation,
                                                 const DeviceGrid& grids,
                                                 const vtr::Matrix<size_t>& grid_instance_ids,
                                                 const TileDirect& tile_direct,
                                                 const ArchDirect& arch_direct) {

  vtr::ScopedStartFinishTimer timer("Add module nets for inter-tile connections");

  for (const TileDirectId& tile_direct_id : tile_direct.directs()) {
    add_module_nets_tile_direct_connection(module_manager, top_module, circuit_lib, 
                                           vpr_device_annotation,
                                           grids, grid_instance_ids,
                                           tile_direct, tile_direct_id,  
                                           arch_direct);
  }
}

} /* end namespace openfpga */
