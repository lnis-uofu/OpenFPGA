#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

/********************************************************************
 * Include header files that are required by function declaration
 *******************************************************************/
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
                                  const size_t& num_config_bits);

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
  std::vector<ModuleId>& physical_memory_children, const bool& verbose);

/**
 * @brief Update all the mappings between logical-to-physical memory children
 * with a given root module This function will walk through the module tree in a
 * recursive way until reaching the leaf node (which require configurable
 * memories) Keep a scoreboard of instance number for checking. Note that when
 * calling this the function, use an empty scoreboard!
 */
int rec_update_logical_memory_children_with_physical_mapping(
  ModuleManager& module_manager, const ModuleId& curr_module,
  const ModuleId& phy_mem_module,
  std::map<ModuleId, size_t>& logical_mem_child_inst_count);

} /* end namespace openfpga */

#endif
