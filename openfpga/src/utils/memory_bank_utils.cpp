/********************************************************************
 * This file includes functions that are used to organize memories 
 * in the top module of FPGA fabric
 *******************************************************************/
#include <cmath>
#include <limits>

/* Headers from vtrutil library */
#include "vtr_assert.h"
#include "vtr_log.h"
#include "vtr_time.h"

/* Headers from vpr library */
#include "vpr_utils.h"

/* Headers from openfpgashell library */
#include "command_exit_codes.h"

#include "rr_gsb_utils.h"
#include "openfpga_reserved_words.h"
#include "openfpga_naming.h"

#include "memory_utils.h"
#include "decoder_library_utils.h"
#include "module_manager_utils.h"
#include "memory_bank_utils.h"

/* begin namespace openfpga */
namespace openfpga {

std::pair<int, int> compute_memory_bank_regional_configurable_child_x_range(const ModuleManager& module_manager,
                                                                            const ModuleId& top_module,
                                                                            const ConfigRegionId& config_region) {
  std::pair<int, int> child_x_range(std::numeric_limits<int>::max(), std::numeric_limits<int>::min()); // Deposit an invalid range first: LSB->max(); MSB->min()
  for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
    vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 
    child_x_range.first = std::min(coord.x(), child_x_range.first); 
    child_x_range.second = std::max(coord.x(), child_x_range.second); 
  }

  VTR_ASSERT(child_x_range.first <= child_x_range.second);
  return child_x_range;
}

std::pair<int, int> compute_memory_bank_regional_configurable_child_y_range(const ModuleManager& module_manager,
                                                                            const ModuleId& top_module,
                                                                            const ConfigRegionId& config_region) {
  std::pair<int, int> child_y_range(std::numeric_limits<int>::max(), std::numeric_limits<int>::min()); // Deposit an invalid range first: LSB->max(); MSB->min()
  for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
    vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 
    child_y_range.first = std::min(coord.y(), child_y_range.first); 
    child_y_range.second = std::max(coord.y(), child_y_range.second); 
  }

  VTR_ASSERT(child_y_range.first <= child_y_range.second);
  return child_y_range;
}

size_t find_module_ql_memory_bank_num_blwls(const ModuleManager& module_manager,
                                            const ModuleId& module_id,
                                            const CircuitLibrary& circuit_lib,
                                            const CircuitModelId& sram_model,
                                            const e_config_protocol_type& sram_orgz_type,
										    const e_circuit_model_port_type& circuit_port_type) {
  std::vector<std::string> config_port_names = generate_sram_port_names(circuit_lib, sram_model, sram_orgz_type);
  size_t num_blwls = 0; /* By default it has zero configuration bits*/

  /* Try to find these ports in the module manager */
  for (const std::string& config_port_name : config_port_names) {
    ModulePortId module_port_id = module_manager.find_module_port(module_id, config_port_name);
    /* If the port does not exist, go to the next */
    if (false == module_manager.valid_module_port_id(module_id, module_port_id)) {
      continue;
    }
    /* We only care about a give type of ports */
    if (circuit_port_type != circuit_lib.port_type(circuit_lib.model_port(sram_model, config_port_name))) {
      continue;
    }
    /* The port exist, find the port size and update the num_config_bits if the size is larger */
    BasicPort module_port = module_manager.module_port(module_id, module_port_id);
    num_blwls = std::max((int)num_blwls, (int)module_port.get_width());
  }

  return num_blwls;
}

std::map<int, size_t> compute_memory_bank_regional_bitline_numbers_per_tile(const ModuleManager& module_manager,
                                                                            const ModuleId& top_module,
                                                                            const ConfigRegionId& config_region,
                                                                            const CircuitLibrary& circuit_lib,
                                                                            const CircuitModelId& sram_model) {
  std::map<int, size_t> num_bls_per_tile;

  for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
    ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
    vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 
    num_bls_per_tile[coord.x()] = std::max(num_bls_per_tile[coord.x()], find_module_ql_memory_bank_num_blwls(module_manager, child_module, circuit_lib, sram_model, CONFIG_MEM_QL_MEMORY_BANK, CIRCUIT_MODEL_PORT_BL));
  }

  return num_bls_per_tile;
}

std::map<int, size_t> compute_memory_bank_regional_wordline_numbers_per_tile(const ModuleManager& module_manager,
                                                                             const ModuleId& top_module,
                                                                             const ConfigRegionId& config_region,
                                                                             const CircuitLibrary& circuit_lib,
                                                                             const CircuitModelId& sram_model) {
  std::map<int, size_t> num_wls_per_tile;

  for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
    ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
    vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 
    num_wls_per_tile[coord.y()] = std::max(num_wls_per_tile[coord.y()], find_module_ql_memory_bank_num_blwls(module_manager, child_module, circuit_lib, sram_model, CONFIG_MEM_QL_MEMORY_BANK, CIRCUIT_MODEL_PORT_WL));
  }

  return num_wls_per_tile;
}

std::map<int, size_t> compute_memory_bank_regional_blwl_start_index_per_tile(const std::pair<int, int>& child_xy_range,
                                                                             const std::map<int, size_t>& num_blwls_per_tile) {
  std::map<int, size_t> blwl_start_index_per_tile;
  for (int iblwl = child_xy_range.first; iblwl <= child_xy_range.second; ++iblwl) {
    if (iblwl == child_xy_range.first) {
      blwl_start_index_per_tile[iblwl] = 0;
    } else {
      auto result = num_blwls_per_tile.find(iblwl - 1);
      if (result == num_blwls_per_tile.end()) {
        blwl_start_index_per_tile[iblwl] = blwl_start_index_per_tile[iblwl - 1]; 
      } else {
        blwl_start_index_per_tile[iblwl] = result->second + blwl_start_index_per_tile[iblwl - 1]; 
      }
    }
  }  
  return blwl_start_index_per_tile;
}

} /* end namespace openfpga */

