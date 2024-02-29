#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
#include <string>
#include <vector>

#include "circuit_types.h"
#include "config_protocol.h"
#include "module_manager.h"
#include "openfpga_port.h"

/********************************************************************
 * Function declaration
 *******************************************************************/

/* begin namespace openfpga */
namespace openfpga {

std::map<std::string, BasicPort> generate_mem_module_port2port_map(
  const BasicPort& config_bus,
  const std::vector<BasicPort>& mem_output_bus_ports,
  const e_circuit_model_design_tech& mem_design_tech,
  const e_config_protocol_type& sram_orgz_type);

void update_mem_module_config_bus(
  const e_config_protocol_type& sram_orgz_type,
  const e_circuit_model_design_tech& mem_design_tech,
  const size_t& num_config_bits, BasicPort& config_bus);

bool check_mem_config_bus(const e_config_protocol_type& sram_orgz_type,
                          const BasicPort& config_bus,
                          const size_t& local_expected_msb);

std::vector<std::string> generate_sram_port_names(
  const CircuitLibrary& circuit_lib, const CircuitModelId& sram_model,
  const e_config_protocol_type sram_orgz_type);

size_t generate_sram_port_size(const e_config_protocol_type sram_orgz_type,
                               const size_t& num_config_bits);

size_t generate_pb_sram_port_size(const e_config_protocol_type sram_orgz_type,
                                  const size_t& num_config_bits,
                                  const size_t& defined_num_wl);

/**
 * @brief Compute the number of configurable children to be skipped for a given
 * configuration protocol For some configuration protocol, the decoders are not
 * counted as configurable children (they are included in the list for bitstream
 * generator usage) The number of decoders depends on the type of configuration
 * protocol.
 */
size_t estimate_num_configurable_children_to_skip_by_config_protocol(
  const ConfigProtocol& config_protocol, size_t curr_region_num_config_child);

/**
 * @brief Find the physical memory child modules with a given root module
 * This function will walk through the module tree in a recursive way until
 * reaching the leaf node (which require configurable memories) Return a list of
 * modules
 */
int rec_find_physical_memory_children(
  const ModuleManager& module_manager, const ModuleId& curr_module,
  std::vector<ModuleId>& physical_memory_children,
  std::vector<std::string>& physical_memory_instance_names,
  const bool& verbose);

} /* end namespace openfpga */

#endif
