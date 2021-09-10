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

std::map<int, size_t> compute_memory_bank_regional_bitline_numbers_per_tile(const ModuleManager& module_manager,
                                                                            const ModuleId& top_module,
                                                                            const ConfigRegionId& config_region,
                                                                            const CircuitLibrary& circuit_lib,
                                                                            const CircuitModelId& sram_model) {
  std::map<int, size_t> num_bls_per_tile;

  for (size_t child_id = 0; child_id < module_manager.region_configurable_children(top_module, config_region).size(); ++child_id) {
    ModuleId child_module = module_manager.region_configurable_children(top_module, config_region)[child_id];
    vtr::Point<int> coord = module_manager.region_configurable_child_coordinates(top_module, config_region)[child_id]; 
    num_bls_per_tile[coord.x()] = std::max(num_bls_per_tile[coord.x()], find_memory_decoder_data_size(find_module_num_config_bits(module_manager, child_module, circuit_lib, sram_model, CONFIG_MEM_QL_MEMORY_BANK)));
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
    num_wls_per_tile[coord.y()] = std::max(num_wls_per_tile[coord.y()], find_memory_wl_decoder_data_size(find_module_num_config_bits(module_manager, child_module, circuit_lib, sram_model, CONFIG_MEM_QL_MEMORY_BANK)));
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
      blwl_start_index_per_tile[iblwl] = num_blwls_per_tile.at(iblwl - 1) + blwl_start_index_per_tile[iblwl - 1]; 
    }
  }  
  return blwl_start_index_per_tile;
}

} /* end namespace openfpga */

