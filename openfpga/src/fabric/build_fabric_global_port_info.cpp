/********************************************************************
 * This file includes functions that are used to build the global port
 * information for the top-level module of the FPGA fabric
 * It helps OpenFPGA to build testbenches with the attributes of the global ports
 *******************************************************************/
#include <map>
#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_time.h"
#include "vtr_log.h"

#include "openfpga_naming.h"

#include "circuit_library_utils.h"
#include "build_fabric_global_port_info.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Find all the GPIO ports in the grid module
 * and cache their port/pin index in the top-level module
 *******************************************************************/
FabricGlobalPortInfo build_fabric_global_port_info(const ModuleManager& module_manager,
                                                   const TileAnnotation& tile_annotation,
                                                   const CircuitLibrary& circuit_lib) {
  vtr::ScopedStartFinishTimer timer("Create global port info for top module");

  FabricGlobalPortInfo fabric_global_port_info;

  /* Find top-level module */
  std::string top_module_name = generate_fpga_top_module_name();
  ModuleId top_module = module_manager.find_module(top_module_name);
  if (true != module_manager.valid_module_id(top_module)) {
    VTR_LOG_ERROR("Unable to find the top-level module '%s'!\n",
                  top_module_name.c_str());
    exit(1);
  }

  /* Add the global ports from circuit library */
  for (const CircuitPortId& global_port : find_circuit_library_global_ports(circuit_lib)) {
    /* We should find a port in the top module */
    ModulePortId module_port = module_manager.find_module_port(top_module, circuit_lib.port_prefix(global_port));
    /* Only add those ports have been defined in top-level module */
    if (false == module_manager.valid_module_port_id(top_module, module_port)) {
      continue;
    }
    /* Add the port information */
    FabricGlobalPortId fabric_port = fabric_global_port_info.create_global_port(module_port);
    fabric_global_port_info.set_global_port_is_clock(fabric_port, CIRCUIT_MODEL_PORT_CLOCK == circuit_lib.port_type(global_port));
    fabric_global_port_info.set_global_port_is_reset(fabric_port, circuit_lib.port_is_reset(global_port));
    fabric_global_port_info.set_global_port_is_set(fabric_port, circuit_lib.port_is_set(global_port));
    fabric_global_port_info.set_global_port_is_prog(fabric_port, circuit_lib.port_is_prog(global_port));
    fabric_global_port_info.set_global_port_is_config_enable(fabric_port, circuit_lib.port_is_config_enable(global_port));
    fabric_global_port_info.set_global_port_default_value(fabric_port, circuit_lib.port_default_value(global_port));
  }

  /* Add the global ports from tile annotation */
  for (const TileGlobalPortId& global_port : tile_annotation.global_ports()) {
    /* We should find a port in the top module */
    ModulePortId module_port = module_manager.find_module_port(top_module, tile_annotation.global_port_name(global_port));
    /* Only add those ports have been defined in top-level module */
    if (false == module_manager.valid_module_port_id(top_module, module_port)) {
      continue;
    }
    /* Avoid adding redundant ports */
    bool already_in_list = false;
    for (const FabricGlobalPortId& port : fabric_global_port_info.global_ports()) {
      if (module_port == fabric_global_port_info.global_module_port(port)) {
        already_in_list = true;
        break;
      }
    } 
    if (true == already_in_list) {
      continue;
    }
    /* Add the port information */
    FabricGlobalPortId fabric_port = fabric_global_port_info.create_global_port(module_port);
    fabric_global_port_info.set_global_port_is_clock(fabric_port, tile_annotation.global_port_is_clock(global_port));
    fabric_global_port_info.set_global_port_is_reset(fabric_port, tile_annotation.global_port_is_reset(global_port));
    fabric_global_port_info.set_global_port_is_set(fabric_port, tile_annotation.global_port_is_set(global_port));
    fabric_global_port_info.set_global_port_default_value(fabric_port, tile_annotation.global_port_default_value(global_port));
  }

  return fabric_global_port_info;
}

} /* end namespace openfpga */
