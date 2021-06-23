/************************************************************************
 * Function to perform fundamental operation for the fabric global ports
 * These functions are not universal methods for the FabricGlobalPortInfo class
 * They are made to ease the development in some specific purposes
 * Please classify such functions in this file
 ***********************************************************************/

#include <algorithm>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"

#include "openfpga_naming.h"

#include "fabric_global_port_info_utils.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Identify global reset ports for programming 
 *******************************************************************/
std::vector<FabricGlobalPortId> find_fabric_global_programming_reset_ports(const FabricGlobalPortInfo& fabric_global_port_info) {
  /* Try to find global reset ports for programming */
  std::vector<FabricGlobalPortId> global_prog_reset_ports;
  for (const FabricGlobalPortId& global_port : fabric_global_port_info.global_ports()) {
    if (false == fabric_global_port_info.global_port_is_prog(global_port)) {
      continue;
    }
    VTR_ASSERT(true == fabric_global_port_info.global_port_is_prog(global_port));
    VTR_ASSERT( (false == fabric_global_port_info.global_port_is_reset(global_port))
               || (false == fabric_global_port_info.global_port_is_set(global_port)));
    if (true == fabric_global_port_info.global_port_is_reset(global_port)) {
      global_prog_reset_ports.push_back(global_port);
    }
  }

  return global_prog_reset_ports;
}

/********************************************************************
 * Identify global set ports for programming 
 *******************************************************************/
std::vector<FabricGlobalPortId> find_fabric_global_programming_set_ports(const FabricGlobalPortInfo& fabric_global_port_info) {
  /* Try to find global set ports for programming */
  std::vector<FabricGlobalPortId> global_prog_set_ports;
  for (const FabricGlobalPortId& global_port : fabric_global_port_info.global_ports()) {
    if (false == fabric_global_port_info.global_port_is_prog(global_port)) {
      continue;
    }
    VTR_ASSERT(true == fabric_global_port_info.global_port_is_prog(global_port));
    VTR_ASSERT( (false == fabric_global_port_info.global_port_is_reset(global_port))
               || (false == fabric_global_port_info.global_port_is_set(global_port)));
    if (true == fabric_global_port_info.global_port_is_set(global_port)) {
      global_prog_set_ports.push_back(global_port);
    }
  }

  return global_prog_set_ports;
}

/********************************************************************
 * Identify if a port is in the list of fabric global port
 * and its functionality is a reset port which is not used for programming FPGAs
 *******************************************************************/
bool port_is_fabric_global_reset_port(const FabricGlobalPortInfo& fabric_global_port_info,
                                      const ModuleManager& module_manager,
                                      const BasicPort& port) {
  /* Find the top_module: the fabric global ports are always part of the ports of the top module */
  ModuleId top_module = module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  for (const FabricGlobalPortId& fabric_global_port_id : fabric_global_port_info.global_ports()) {
    if ( (false == fabric_global_port_info.global_port_is_reset(fabric_global_port_id))
      || (true == fabric_global_port_info.global_port_is_prog(fabric_global_port_id))) {
      continue;
    }

    BasicPort module_global_port = module_manager.module_port(top_module, fabric_global_port_info.global_module_port(fabric_global_port_id));
    if ( (true == module_global_port.mergeable(port))
      && (true == module_global_port.contained(port)) ) {
      return true;
    }
  }

  return false;
}

/********************************************************************
 * Find a global port with a given name
 *******************************************************************/
FabricGlobalPortId find_fabric_global_port(const FabricGlobalPortInfo& fabric_global_port_info,
                                           const ModuleManager& module_manager,
                                           const BasicPort& port) {
  /* Find the top_module: the fabric global ports are always part of the ports of the top module */
  ModuleId top_module = module_manager.find_module(generate_fpga_top_module_name());
  VTR_ASSERT(true == module_manager.valid_module_id(top_module));

  for (const FabricGlobalPortId& fabric_global_port_id : fabric_global_port_info.global_ports()) {
    BasicPort module_global_port = module_manager.module_port(top_module, fabric_global_port_info.global_module_port(fabric_global_port_id));
    if ( (true == module_global_port.mergeable(port))
      && (true == module_global_port.contained(port)) ) {
      return fabric_global_port_id;
    }
  }
  return FabricGlobalPortId::INVALID();
}

} /* end namespace openfpga */
