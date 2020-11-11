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

} /* end namespace openfpga */
