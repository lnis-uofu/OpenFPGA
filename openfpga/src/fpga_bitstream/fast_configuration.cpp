/********************************************************************
 * This file includes functions that are used to create
 * an auto-check top-level testbench for a FPGA fabric
 *******************************************************************/
#include <vector>

/* Headers from vtrutil library */
#include "vtr_log.h"
#include "vtr_assert.h"

#include "fabric_global_port_info_utils.h"
#include "fast_configuration.h"

/* begin namespace openfpga */
namespace openfpga {

/********************************************************************
 * Identify if fast configuration is applicable base on the availability
 * of programming reset and programming set ports of the FPGA fabric
 *******************************************************************/
bool is_fast_configuration_applicable(const FabricGlobalPortInfo& global_ports) {
  /* Preparation: find all the reset/set ports for programming usage */
  std::vector<FabricGlobalPortId> global_prog_reset_ports = find_fabric_global_programming_reset_ports(global_ports);
  std::vector<FabricGlobalPortId> global_prog_set_ports = find_fabric_global_programming_set_ports(global_ports);

  /* Identify if we can apply fast configuration */
  if (global_prog_set_ports.empty() && global_prog_reset_ports.empty()) {
    VTR_LOG_WARN("None of global reset and set ports are defined for programming purpose. Fast configuration is not applicable\n");
    return false;
  }

  return true;
}

/********************************************************************
 * Decide if we should use reset or set signal to acheive fast configuration
 * - If only one type signal is specified, we use that type
 *   For example, only reset signal is defined, we will use reset  
 * - If both are defined, pick the one that will bring bigger reduction
 *   i.e., larger number of configuration bits can be skipped
 *******************************************************************/
bool find_bit_value_to_skip_for_fast_configuration(const e_config_protocol_type& config_protocol_type,  
                                                   const FabricGlobalPortInfo& global_ports,
                                                   const BitstreamManager& bitstream_manager,
                                                   const FabricBitstream& fabric_bitstream) {
  /* Preparation: find all the reset/set ports for programming usage */
  std::vector<FabricGlobalPortId> global_prog_reset_ports = find_fabric_global_programming_reset_ports(global_ports);
  std::vector<FabricGlobalPortId> global_prog_set_ports = find_fabric_global_programming_set_ports(global_ports);

  /* Early exit conditions */
  if (!global_prog_reset_ports.empty() && global_prog_set_ports.empty()) {
    return false; 
  } else if (!global_prog_set_ports.empty() && global_prog_reset_ports.empty()) {
    return true; 
  }

  /* If both types of ports are not defined, the fast configuration is not applicable */
  VTR_ASSERT(!global_prog_set_ports.empty() && !global_prog_reset_ports.empty());
  bool bit_value_to_skip = false;

  VTR_LOG("Both reset and set ports are defined for programming controls, selecting the best-fit one...\n");

  size_t num_ones_to_skip = 0;
  size_t num_zeros_to_skip = 0;

  /* Branch on the type of configuration protocol */
  switch (config_protocol_type) {
  case CONFIG_MEM_STANDALONE:
    break;
  case CONFIG_MEM_SCAN_CHAIN: {
    /* We can only skip the ones/zeros at the beginning of the bitstream */
    /* Count how many logic '1' bits we can skip */
    for (const FabricBitId& bit_id : fabric_bitstream.bits()) {
      if (false == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id))) {
        break;
      }
      VTR_ASSERT(true == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id)));
      num_ones_to_skip++;
    }
    /* Count how many logic '0' bits we can skip */
    for (const FabricBitId& bit_id : fabric_bitstream.bits()) {
      if (true == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id))) {
        break;
      }
      VTR_ASSERT(false == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id)));
      num_zeros_to_skip++;
    }
    break;
  }
  case CONFIG_MEM_QL_MEMORY_BANK:
  case CONFIG_MEM_MEMORY_BANK:
  case CONFIG_MEM_FRAME_BASED: {
    /* Count how many logic '1' and logic '0' bits we can skip */
    for (const FabricBitId& bit_id : fabric_bitstream.bits()) {
      if (false == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id))) {
        num_zeros_to_skip++;
      } else {
        VTR_ASSERT(true == bitstream_manager.bit_value(fabric_bitstream.config_bit(bit_id)));
        num_ones_to_skip++;
      }
    }
    break;
  }
  default:
    VTR_LOGF_ERROR(__FILE__, __LINE__,
                   "Invalid configuration protocol type!\n");
    exit(1);
  }

  VTR_LOG("Using reset will skip %g% (%lu/%lu) of configuration bitstream.\n",
          100. * (float) num_zeros_to_skip / (float) fabric_bitstream.num_bits(),
          num_zeros_to_skip, fabric_bitstream.num_bits());

  VTR_LOG("Using set will skip %g% (%lu/%lu) of configuration bitstream.\n",
          100. * (float) num_ones_to_skip / (float) fabric_bitstream.num_bits(),
          num_ones_to_skip, fabric_bitstream.num_bits());

  /* By default, we prefer to skip zeros (when the numbers are the same */
  if (num_ones_to_skip > num_zeros_to_skip) {
    VTR_LOG("Will use set signal in fast configuration\n");
    bit_value_to_skip = true;
  } else {
    VTR_LOG("Will use reset signal in fast configuration\n");
  }

  return bit_value_to_skip;
}

} /* end namespace openfpga */
